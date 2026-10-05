<#
.SYNOPSIS
    Renders component manifests into CycloneDX 1.6 SBOMs.

.DESCRIPTION
    Every project in this product line owns its own manifest and publishes its
    own SBOM:

        LDS/sbom/components.json            -> OPCFoundation/UA-LDS
        mDNSResponder/sbom/components.json  -> OPCFoundation/UA-LDS-mDNSResponder
        sbom/components.json                -> this repository (the installer)

    This script runs in two modes:

      -ProjectOnly   Render one manifest as a standalone SBOM. That is the
                     document the owning project publishes with its releases.

      (default)      Render the installer manifest and COMPOSE the subprojects
                     named in its "subprojects" list into a single SBOM for the
                     installed product.

    Composition merges rather than merely links, because a market surveillance
    authority asking for "the SBOM" should get one self-contained document, and
    most tooling does not follow CycloneDX BOM-Links. The composed document
    still records an externalReference of type "bom" (with a SHA-256) back to
    each subproject's own SBOM, so the authoritative source stays traceable.

    Components appearing in more than one manifest - OpenSSL and the static
    MSVC runtime are linked by several projects - are deduplicated on purl, or
    on name+version where no purl exists. Dependency edges from every duplicate
    are redirected onto the surviving component, so the graph stays connected.

    Each manifest is self-contained: versionFrom.path and shaFrom are resolved
    relative to the PROJECT ROOT of the manifest that declares them (the parent
    of its sbom/ directory), so a submodule manifest works unchanged whether it
    is rendered here or in its own repository.

.PARAMETER OutputPath
    Path of the .cdx.json file to write.

.PARAMETER Version
    Product version (e.g. 1.4.420.34), used by versionFrom kind "product".

.PARAMETER Architecture
    Build target recorded in metadata: x86 or x64.

.PARAMETER ManifestPath
    Manifest to render. Defaults to this script's sibling components.json.

.PARAMETER ProjectOnly
    Render only this manifest, ignoring any "subprojects".

.PARAMETER SubprojectBomDir
    Directory holding the already-rendered subproject SBOMs. Used to attach
    externalReferences and their digests during composition.

.PARAMETER BinDir, ExtraBinDir
    Staged binary directories. Shipped artifacts found here are hashed and the
    SHA-256 recorded against their component.

.PARAMETER CrtVersion
    MSVC toolset/runtime version, for versionFrom kind "parameter".

.EXAMPLE
    # The SBOM OPCFoundation/UA-LDS publishes
    .\New-Sbom.ps1 -ManifestPath ..\LDS\sbom\components.json -ProjectOnly `
                   -OutputPath .\out\ua-lds.cdx.json -Architecture x64

.EXAMPLE
    # The composed product SBOM
    .\New-Sbom.ps1 -OutputPath .\out\lds-x64.cdx.json -Version 1.4.420.34 `
                   -Architecture x64 -SubprojectBomDir .\out
#>

[CmdletBinding()]
param(
    [Parameter(Mandatory)][string]$OutputPath,
    [string]$Version,
    [ValidateSet('x86', 'x64')][string]$Architecture = 'x64',
    [string]$ManifestPath,
    [switch]$ProjectOnly,
    [string]$SubprojectBomDir,
    [string]$BinDir,
    [string]$ExtraBinDir,
    [string]$CrtVersion = 'v143'
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$SbomDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
if (-not $ManifestPath) { $ManifestPath = Join-Path $SbomDir 'components.json' }
if (-not (Test-Path $ManifestPath)) { throw "SBOM manifest not found: $ManifestPath" }

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

# Property access that tolerates absent members under Set-StrictMode.
function Get-Prop {
    param($Object, [string]$Name, $Default = $null)
    if ($null -eq $Object) { return $Default }
    $prop = $Object.PSObject.Properties[$Name]
    if ($null -eq $prop -or $null -eq $prop.Value) { return $Default }
    return $prop.Value
}

# Run git and return trimmed stdout, or $null on any failure.
#
# Native stderr is not suppressed by `2>$null` when $ErrorActionPreference is
# 'Stop' - PowerShell raises NativeCommandError instead. Git failures here are
# expected and recoverable (no tags yet, submodule not initialised,
# safe.directory refusing a repo owned by another user), so they are relaxed to
# a $null return and reported by the caller as a provenance warning.
function Invoke-Git {
    param([string]$Path, [string[]]$Arguments)
    $previous = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try {
        $output = & git -C $Path @Arguments 2>&1
        if ($LASTEXITCODE -ne 0) { return $null }
        if (-not $output) { return $null }
        return ("$output").Trim()
    } catch {
        return $null
    } finally {
        $ErrorActionPreference = $previous
    }
}

function Resolve-GitSha {
    param([string]$Path)
    if (-not (Test-Path $Path)) { return $null }
    return Invoke-Git $Path @('rev-parse', 'HEAD')
}

# "MIT" -> {license:{id}};  {name:"..."} -> {license:{name}}
function Convert-License {
    param($Entry)
    if ($Entry -is [string]) { return @{ license = @{ id = $Entry } } }
    $name = Get-Prop $Entry 'name'
    if ($name) { return @{ license = @{ name = $name } } }
    $id = Get-Prop $Entry 'id'
    if ($id) { return @{ license = @{ id = $id } } }
    return $null
}

function New-Property {
    param([string]$Name, $Value)
    return @{ name = $Name; value = "$Value" }
}

function Get-FileSha256 {
    param([string]$Path)
    return (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash.ToLowerInvariant()
}

# ---------------------------------------------------------------------------
# Manifest loading
#
# A manifest's project root is the parent of its sbom/ directory. Every path it
# declares is relative to that, which is what lets a submodule manifest be
# rendered from here or from its own repository with identical results.
# ---------------------------------------------------------------------------
function Import-Manifest {
    param([string]$Path)

    $full = (Resolve-Path -LiteralPath $Path).Path
    $manifest = Get-Content -Raw -LiteralPath $full | ConvertFrom-Json
    $projectRoot = Split-Path -Parent (Split-Path -Parent $full)

    $project = Get-Prop $manifest 'project'
    if (-not $project) { throw "Manifest $Path has no 'project' block." }

    return [pscustomobject]@{
        Manifest    = $manifest
        Project     = $project
        Id          = Get-Prop $project 'id' 'unknown'
        ProjectRoot = $projectRoot
        ManifestPath= $full
    }
}

# Reads a dotted version out of #define macros in a C header.
function Resolve-HeaderVersion {
    param($VersionFrom, [string]$ProjectRoot, [string]$Fallback, [string]$Label)

    $relative = Get-Prop $VersionFrom 'path'
    $macros   = @(Get-Prop $VersionFrom 'macros' @())

    if (-not $relative -or $macros.Count -eq 0) {
        Write-Warning "SBOM: $Label has an incomplete 'header' versionFrom - using '$Fallback'."
        return $Fallback
    }

    $headerPath = Join-Path $ProjectRoot $relative
    if (-not (Test-Path $headerPath)) {
        Write-Warning "SBOM: $relative not found - using '$Fallback' for $Label."
        return $Fallback
    }

    $text  = Get-Content -Raw -LiteralPath $headerPath
    $parts = @()

    foreach ($macro in $macros) {
        $pattern = '(?m)^\s*#\s*define\s+' + [regex]::Escape($macro) + '\s+(\S+)'
        if ($text -match $pattern) {
            $parts += $Matches[1]
        } else {
            Write-Warning "SBOM: $macro not found in $relative - using '$Fallback' for $Label."
            return $Fallback
        }
    }

    return ($parts -join '.')
}

# Resolves versionFrom for a project block or a component.
function Resolve-Version {
    param($Owner, [string]$ProjectRoot, [string]$Label, [string]$ProjectVersion)

    $vf = Get-Prop $Owner 'versionFrom'
    $fallback = Get-Prop $Owner 'version' 'unknown'
    $kind = if ($vf) { Get-Prop $vf 'kind' 'literal' } else { 'literal' }

    switch ($kind) {
        'literal'   { return $fallback }
        'project'   { if ($ProjectVersion) { return $ProjectVersion } return $fallback }
        'product'   {
            if ($Version) { return $Version }
            Write-Warning "SBOM: $Label wants the product version but -Version was not supplied - using '$fallback'."
            return $fallback
        }
        'parameter' {
            if ((Get-Prop $vf 'parameter') -eq 'CrtVersion') { return $CrtVersion }
            return $fallback
        }
        'header'    { return (Resolve-HeaderVersion $vf $ProjectRoot $fallback $Label) }
        default     { return $fallback }
    }
}

# ---------------------------------------------------------------------------
# Component rendering
#
# bom-refs are namespaced "component:<projectId>/<key>" so that two projects
# can use the same key without colliding, and so a reader can tell at a glance
# which repository owns a component.
# ---------------------------------------------------------------------------
function New-ComponentSet {
    param($Info)

    $projectRoot = $Info.ProjectRoot
    $projectId   = $Info.Id
    $projectVersion = Resolve-Version $Info.Project $projectRoot "project '$projectId'" $null

    # Resolve each distinct shaFrom path once. "." means this project's own tree.
    $shaCache = @{}
    foreach ($comp in $Info.Manifest.components) {
        $shaFrom = Get-Prop $comp 'shaFrom'
        if (-not $shaFrom -or $shaCache.ContainsKey($shaFrom)) { continue }
        $full = if ($shaFrom -eq '.') { $projectRoot } else { Join-Path $projectRoot $shaFrom }
        $shaCache[$shaFrom] = Resolve-GitSha $full
        if (-not $shaCache[$shaFrom]) {
            Write-Warning "SBOM: could not resolve git state for '$shaFrom' in $projectId - provenance will be incomplete."
        }
    }

    $components = @()

    foreach ($comp in $Info.Manifest.components) {
        $key = $comp.key
        $ver = Resolve-Version $comp $projectRoot "$projectId/$key" $projectVersion
        $shaFrom = Get-Prop $comp 'shaFrom'
        $sha = if ($shaFrom -and $shaCache.ContainsKey($shaFrom)) { $shaCache[$shaFrom] } else { $null }

        $entry = [ordered]@{
            'bom-ref' = "component:$projectId/$key"
            type      = Get-Prop $comp 'type' 'library'
            name      = $comp.name
            version   = "$ver"
        }

        $supplier = Get-Prop $comp 'supplier'
        if ($supplier) { $entry.supplier = @{ name = "$supplier" } }

        $licenses = @(Get-Prop $comp 'licenses' @() | ForEach-Object { Convert-License $_ } | Where-Object { $_ })
        if ($licenses.Count -gt 0) { $entry.licenses = $licenses }

        # purl/cpe templates: {version} and {sha}
        foreach ($field in 'purl', 'cpe') {
            $tpl = Get-Prop $comp $field
            if (-not $tpl) { continue }
            if ($tpl -like '*{sha}*') {
                if (-not $sha) { continue }   # omit rather than emit a bogus identifier
                $tpl = $tpl.Replace('{sha}', $sha)
            }
            $entry[$field] = $tpl.Replace('{version}', "$ver")
        }

        $notes = Get-Prop $comp 'notes'
        if ($notes) { $entry.description = "$notes" }

        # SHA-256 of the artifact as actually staged for this architecture.
        $artifact = Get-Prop $comp 'artifactFile'
        if ($artifact) {
            $artifactPath = $null
            foreach ($dir in @($BinDir, $ExtraBinDir)) {
                if (-not $dir) { continue }
                $candidate = Join-Path $dir $artifact
                if (Test-Path $candidate) { $artifactPath = $candidate; break }
            }

            if ($artifactPath) {
                $entry.hashes = @(@{ alg = 'SHA-256'; content = (Get-FileSha256 $artifactPath) })
            } elseif ($BinDir -or $ExtraBinDir) {
                Write-Warning "SBOM: '$artifact' not found in the staged output - its digest will be omitted."
            }
        }

        $props = @()
        $props += New-Property 'cra:ownerProject' (Get-Prop $Info.Project 'repository' $projectId)
        $props += New-Property 'cra:cveMonitoring' (Get-Prop $comp 'cveMonitoring' 'manual')
        $props += New-Property 'cra:exposure'      (Get-Prop $comp 'exposure' 'unassessed')
        $status = Get-Prop $comp 'status'
        if ($status) { $props += New-Property 'cra:status' $status }
        $eol = Get-Prop $comp 'endOfLife'
        if ($eol) { $props += New-Property 'cra:endOfLife' $eol }
        if ($sha) { $props += New-Property 'cra:sourceCommit' $sha }
        if ($artifact) { $props += New-Property 'cra:artifactFile' $artifact }
        $entry.properties = $props

        $components += $entry
    }

    # Dependency edges, with keys namespaced the same way. A key already
    # containing '/' is a cross-project reference and is taken as written.
    $dependencies = @()
    $depMap = Get-Prop $Info.Manifest 'dependencies'
    if ($depMap) {
        foreach ($prop in $depMap.PSObject.Properties) {
            if ($prop.Name -eq 'product') { continue }   # handled by the composer
            $dependencies += [ordered]@{
                ref       = "component:$projectId/$($prop.Name)"
                dependsOn = @($prop.Value | ForEach-Object {
                    if ($_ -like '*/*') { "component:$_" } else { "component:$projectId/$_" }
                })
            }
        }
    }

    return [pscustomobject]@{
        Info         = $Info
        Version      = $projectVersion
        Components   = $components
        Dependencies = $dependencies
    }
}

# ---------------------------------------------------------------------------
# Deduplication
#
# OpenSSL and the static MSVC runtime are declared by every project that links
# them, which is correct for each project's own SBOM but would list them several
# times in a composed one. Collapse on purl, falling back to name+version where
# there is no purl, and redirect dependency edges onto the survivor.
# ---------------------------------------------------------------------------
function Merge-Components {
    param([object[]]$Sets)

    $components = @()
    $identities = @{}
    $remap      = @{}

    foreach ($set in $Sets) {
        foreach ($entry in $set.Components) {
            # Identity includes the name, not just the purl. Several artifacts
            # can legitimately come from one source package and therefore share
            # a purl - opcualds.exe and dnssd.dll both build from UA-LDS at the
            # same commit - and collapsing those would silently drop a shipped
            # binary from the inventory.
            $identity = if ($entry.Contains('purl')) {
                "purl:$($entry['purl'])|$($entry['name'])"
            } else {
                "nv:$($entry['name'])|$($entry['version'])"
            }

            if ($identities.ContainsKey($identity)) {
                $survivor = $identities[$identity]
                $remap[$entry['bom-ref']] = $survivor

                # Keep a record of every project that declared it, so the
                # composed document still shows who links what.
                $existing = $components | Where-Object { $_['bom-ref'] -eq $survivor } | Select-Object -First 1
                $owner = ($entry.properties | Where-Object { $_.name -eq 'cra:ownerProject' } | Select-Object -First 1)
                if ($existing -and $owner) {
                    $alsoIn = $existing.properties | Where-Object { $_.name -eq 'cra:alsoDeclaredBy' } | Select-Object -First 1
                    if ($alsoIn) {
                        if ($alsoIn.value -notlike "*$($owner.value)*") { $alsoIn.value += ", $($owner.value)" }
                    } else {
                        $existing.properties += (New-Property 'cra:alsoDeclaredBy' $owner.value)
                    }
                }

                continue
            }

            $identities[$identity] = $entry['bom-ref']
            $components += $entry
        }
    }

    return [pscustomobject]@{ Components = $components; Remap = $remap }
}

function Resolve-Ref {
    param([string]$Ref, [hashtable]$Remap)
    if ($Remap.ContainsKey($Ref)) { return $Remap[$Ref] }
    return $Ref
}

function Merge-Dependencies {
    param([object[]]$Sets, [hashtable]$Remap, [string[]]$ProductDependsOn, [string]$ProductRef)

    $merged = [ordered]@{}

    if ($ProductRef) {
        $merged[$ProductRef] = @($ProductDependsOn | ForEach-Object { Resolve-Ref $_ $Remap } | Select-Object -Unique)
    }

    foreach ($set in $Sets) {
        foreach ($dep in $set.Dependencies) {
            $ref = Resolve-Ref $dep.ref $Remap
            $on  = @($dep.dependsOn | ForEach-Object { Resolve-Ref $_ $Remap })

            if ($merged.Contains($ref)) {
                $merged[$ref] = @($merged[$ref] + $on | Select-Object -Unique)
            } else {
                $merged[$ref] = @($on | Select-Object -Unique)
            }
        }
    }

    $result = @()
    foreach ($key in $merged.Keys) {
        $result += [ordered]@{ ref = $key; dependsOn = @($merged[$key] | Where-Object { $_ -ne $key }) }
    }
    return $result
}

# ---------------------------------------------------------------------------
# Load the root manifest and any subprojects
# ---------------------------------------------------------------------------
$rootInfo = Import-Manifest $ManifestPath
$rootSet  = New-ComponentSet $rootInfo

$subSets = @()
$subBomRefs = @()

if (-not $ProjectOnly) {
    foreach ($sub in @(Get-Prop $rootInfo.Manifest 'subprojects' @())) {
        $subManifest = Join-Path $rootInfo.ProjectRoot $sub.manifest

        if (-not (Test-Path $subManifest)) {
            throw @"
Subproject manifest not found: $subManifest
The '$($sub.id)' subproject declares its own components. If the submodule is not
initialised, run: git submodule update --init
"@
        }

        $subInfo = Import-Manifest $subManifest
        $subSets += (New-ComponentSet $subInfo)
        $subBomRefs += [pscustomobject]@{ Info = $subInfo; Declared = $sub }
    }
}

$allSets = @($rootSet) + $subSets
$merged  = Merge-Components $allSets

# ---------------------------------------------------------------------------
# externalReferences back to each subproject's own SBOM
# ---------------------------------------------------------------------------
foreach ($subRef in $subBomRefs) {
    $primaryKey = Get-Prop $subRef.Info.Project 'primaryComponent'
    if (-not $primaryKey) { continue }

    $targetRef = Resolve-Ref "component:$($subRef.Info.Id)/$primaryKey" $merged.Remap
    $component = $merged.Components | Where-Object { $_['bom-ref'] -eq $targetRef } | Select-Object -First 1
    if (-not $component) { continue }

    $references = @(
        @{ type = 'vcs'; url = (Get-Prop $subRef.Info.Project 'repository' '') }
    )

    # Link to the subproject's own rendered SBOM, with its digest, so the
    # authoritative document is identifiable and tamper-evident.
    # bomFile is a pattern so the naming convention lives in the manifest
    # rather than being duplicated in build.ps1.
    $bomName = Get-Prop $subRef.Declared 'bomFile'
    if ($bomName) {
        $bomName = $bomName.Replace('{version}', "$Version").Replace('{arch}', $Architecture)
    }
    if ($SubprojectBomDir -and $bomName) {
        $bomPath = Join-Path $SubprojectBomDir $bomName
        if (Test-Path $bomPath) {
            $references += @{
                type    = 'bom'
                url     = $bomName
                hashes  = @(@{ alg = 'SHA-256'; content = (Get-FileSha256 $bomPath) })
                comment = "Authoritative SBOM published by $(Get-Prop $subRef.Info.Project 'repository' $subRef.Info.Id)"
            }
        } else {
            Write-Warning "SBOM: subproject BOM '$bomName' not found in $SubprojectBomDir - its externalReference will be omitted."
        }
    }

    $component.externalReferences = $references
}

# ---------------------------------------------------------------------------
# Assemble
# ---------------------------------------------------------------------------
$project = $rootInfo.Project
$isProduct = [bool](Get-Prop $project 'isProduct' $false) -and -not $ProjectOnly

$productRef = "component:$($rootInfo.Id)"
$productVersion = $rootSet.Version

# What the top-level component depends on.
$declaredProductDeps = @()
$depMap = Get-Prop $rootInfo.Manifest 'dependencies'
if ($depMap -and $depMap.PSObject.Properties['product']) {
    $declaredProductDeps = @($depMap.product | ForEach-Object {
        if ($_ -like '*/*') { "component:$_" } else { "component:$($rootInfo.Id)/$_" }
    })
}

if ($declaredProductDeps.Count -eq 0) {
    # No explicit product list: depend on the roots of this project's graph -
    # components nothing else depends on.
    $depended = @()
    foreach ($set in $allSets) {
        foreach ($dep in $set.Dependencies) { $depended += $dep.dependsOn }
    }
    $declaredProductDeps = @($merged.Components |
        Where-Object { $depended -notcontains $_['bom-ref'] } |
        ForEach-Object { $_['bom-ref'] })
}

$dependencies = Merge-Dependencies $allSets $merged.Remap $declaredProductDeps $productRef

$productLicenses = @(Convert-License (Get-Prop $project 'license' 'MIT')) | Where-Object { $_ }

$topComponent = [ordered]@{
    'bom-ref'   = $productRef
    type        = 'application'
    group       = Get-Prop $project 'group' 'org.opcfoundation'
    name        = Get-Prop $project 'name' $rootInfo.Id
    version     = "$productVersion"
    description = Get-Prop $project 'description' ''
    licenses    = $productLicenses
    supplier    = @{ name = $project.supplier.name }
    externalReferences = @(
        @{ type = 'vcs';              url = Get-Prop $project 'repository' '' }
        @{ type = 'security-contact'; url = "$(Get-Prop $project 'repository' '')/blob/master/SECURITY.md" }
    )
}

$toolComponents = @()
foreach ($set in $allSets) {
    foreach ($tool in @(Get-Prop $set.Info.Manifest 'buildTools' @())) {
        $name = Get-Prop $tool 'name' 'unknown'
        if ($toolComponents | Where-Object { $_.name -eq $name }) { continue }
        $toolComponents += [ordered]@{
            type     = 'application'
            name     = $name
            version  = "$(Get-Prop $tool 'version' 'unknown')"
            supplier = @{ name = "$(Get-Prop $tool 'supplier' 'unknown')" }
        }
    }
}

$metadataProperties = @(
    New-Property 'cra:role'               (Get-Prop $project 'craRole' 'open-source-steward')
    New-Property 'cra:targetArchitecture' $Architecture
    New-Property 'cra:ownerProject'       (Get-Prop $project 'repository' $rootInfo.Id)
)

if ($isProduct) {
    $metadataProperties += New-Property 'cra:composedFrom' (
        ($subBomRefs | ForEach-Object { Get-Prop $_.Info.Project 'repository' $_.Info.Id }) -join ', '
    )
}

$bom = [ordered]@{
    bomFormat    = 'CycloneDX'
    specVersion  = '1.6'
    serialNumber = "urn:uuid:$([guid]::NewGuid().ToString())"
    version      = 1
    metadata     = [ordered]@{
        timestamp  = (Get-Date).ToUniversalTime().ToString('yyyy-MM-ddTHH:mm:ssZ')
        lifecycles = @(@{ phase = 'build' })
        tools      = @{ components = $toolComponents }
        # metadata.supplier, not metadata.manufacturer: OPC Federation AISBL
        # publishes this software as an open-source software steward under CRA
        # Article 24, not as a manufacturer placing a product on the market.
        supplier   = [ordered]@{
            name = $project.supplier.name
            url  = @(Get-Prop $project.supplier 'url' '')
        }
        authors    = @(@{ name = $project.supplier.name })
        component  = $topComponent
        properties = $metadataProperties
    }
    components   = $merged.Components
    dependencies = $dependencies
}

# Declare how complete this inventory is. The manifests are hand-curated under
# a process rule (a new component must be added in the same pull request), and
# composition pulls in every subproject's own declaration, so the assembly is
# asserted complete rather than left "unknown".
$bom.compositions = @(
    [ordered]@{
        'bom-ref'  = 'composition:product'
        aggregate  = 'complete'
        assemblies = @($productRef)
    }
)

# ---------------------------------------------------------------------------
# Write
# ---------------------------------------------------------------------------
$OutDirectory = Split-Path -Parent $OutputPath
if ($OutDirectory -and -not (Test-Path $OutDirectory)) {
    New-Item -ItemType Directory -Path $OutDirectory -Force | Out-Null
}

$json = $bom | ConvertTo-Json -Depth 32
# UTF-8 without BOM: CycloneDX consumers are JSON parsers, and a BOM trips some.
[System.IO.File]::WriteAllText($OutputPath, $json, (New-Object System.Text.UTF8Encoding($false)))

$label = if ($isProduct) { "composed from $($subSets.Count + 1) project(s)" } else { "project '$($rootInfo.Id)'" }
Write-Host "  SBOM: $OutputPath ($($merged.Components.Count) components, $Architecture, $label)"

if ($merged.Remap.Count -gt 0) {
    Write-Host "    deduplicated $($merged.Remap.Count) component declaration(s) shared between projects"
}

$pending = @($merged.Components | Where-Object {
    $_.properties | Where-Object { $_.name -eq 'cra:status' -and $_.value -eq 'pending-removal' }
})
if ($pending.Count -gt 0) {
    Write-Host "    note: $($pending.Count) component(s) flagged pending-removal: $(($pending | ForEach-Object { $_.name }) -join ', ')"
}
