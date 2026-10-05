# SBOM for OPCFoundation/UA-LDS

This project publishes its own CycloneDX 1.6 SBOM, covering what this
repository produces and links: `opcualds.exe`, `dnssd.dll`, the vendored OPC UA
ANSI C stack, OpenSSL, and the statically linked MSVC runtime.

## Generate

```powershell
.\sbom\New-Sbom.ps1 -OutputPath .\ua-lds.cdx.json -Architecture x64
```

No other arguments are needed. The manifest beside the script is picked up
automatically, and every path it declares resolves relative to this repository
root, so the result does not depend on how or from where the build was driven.

Useful extras:

| Argument | Effect |
|---|---|
| `-BinDir <dir>` | Hash the built binaries and record SHA-256 digests |
| `-CrtVersion <v>` | Record the exact MSVC runtime version instead of `v143` |
| `-Architecture x86\|x64` | Recorded in metadata; pick the one you built |

## Files

| File | Purpose |
|---|---|
| `components.json` | The component inventory.|
| `New-Sbom.ps1` | Renderer |

