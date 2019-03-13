/* Copyright (c) 1996-2018, OPC Foundation. All rights reserved.

The source code in this file is covered under a dual-license scenario:
- RCL: for OPC Foundation members in good-standing
- GPL V2: everybody else

RCL license terms accompanied with this source code. See http://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code. See http://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* system includes */
#include <stdlib.h>
#include <errno.h>
#include <time.h>
/* uastack includes */
#include <opcua_serverstub.h>
#include <opcua_core.h>
#include <opcua_memory.h>
#include <opcua_string.h>
#include <opcua_pkifactory.h>
#include <opcua_endpoint.h>
/* openssl includes */
#if OPCUA_SUPPORT_PKI
# include <openssl/sha.h>
#endif /* OPCUA_SUPPORT_PKI */
/* local includes */
#include "config.h"
#include "ualds.h"
#include "utils.h"
#ifdef _WIN32
#include "service.h"
#endif /* _WIN32 */
#include "settings.h"
#ifdef HAVE_HDS
# include "zeroconf.h"
# include "findserversonnetwork.h"
#endif
/* local platform includes */
#include <platform.h>
#include <log.h>
#if OPCUA_SUPPORT_PKI_WIN32
# include <certstore.h>
#endif /* OPCUA_SUPPORT_PKI_WIN32 */

static int g_shutdown = 0;
static OpcUa_P_OpenSSL_CertificateStore_Config g_PKIConfig;
static OpcUa_PKIProvider                       g_PkiProvider;
static OpcUa_P_OpenSSL_CertificateStore_Config g_Win32Config;
static OpcUa_PKIProvider                       g_Win32Override;
static char g_szCertificateFile[PATH_MAX];
static char g_szCertificateKeyFile[PATH_MAX];
static char g_szCRLPath[PATH_MAX];
static char g_szTrustListPath[PATH_MAX];
static char g_szIssuerPath[PATH_MAX];
static char g_szRejectedPath[PATH_MAX];

static char g_szCertificateStorePath[PATH_MAX];

static char g_szTrustListPathOldEditedLocation[PATH_MAX];

OpcUa_ByteString g_server_certificate = OPCUA_BYTESTRING_STATICINITIALIZER;
static OpcUa_Key        g_server_key;
extern OpcUa_P_TraceHook g_OpcUa_P_TraceHook;
static OpcUa_UInt32     g_StackTraceLevel = OPCUA_TRACE_OUTPUT_LEVEL_NONE;
static OpcUa_UInt32     g_numEndpoints;
static ualds_endpoint  *g_pEndpoints;
static char             g_szServerUri[UALDS_CONF_MAX_URI_LENGTH];
static char             g_szProductUri[UALDS_CONF_MAX_URI_LENGTH];
static char             g_szApplicationName[UALDS_CONF_MAX_URI_LENGTH];
static char             g_szHostname[256];
static int              g_ExpirationMaxAge = 600; /* 10 minutes */
static int              g_bAllowLocalRegistration = 0;
static int              g_MaxRejectedCertificates = 5;
static int              g_MaxAgeRejectedCertificates = 1; /* days */
#ifdef _WIN32
static int              g_bWin32StoreCheck = 0;
#endif /* _WIN32 */

OpcUa_Mutex g_mutex = OpcUa_Null;
int g_bEnableZeroconf = 0;

#if HAVE_OPENSSL
/* basic extensions */
#define EXT_COUNT 6
static OpcUa_Crypto_Extension ext_ent[EXT_COUNT] =
{
    {"subjectAltName", 0},
    {"basicConstraints", "critical, CA:FALSE"},
    {"subjectKeyIdentifier", "hash"},
    {"authorityKeyIdentifier", "keyid, issuer:always"},
    {"keyUsage", "critical, nonRepudiation, digitalSignature, keyEncipherment, dataEncipherment, keyCertSign"},
    {"extendedKeyUsage", "critical, serverAuth"}
};
#endif /* HAVE_OPENSSL */

/** event table for enum/string conversion */
static const char *g_szEndpointEventNames[] = {
    "Invalid",
    "SecureChannelOpened",
    "SecureChannelClosed",
    "SecureChannelRenewed",
    "SecureChannelOpenVerifyCertificate",
    "SecureChannelRenewVerifyCertificate",
    "UnsupportedServiceRequested",
    "RawRequest",
    "DecoderError"
};

#define SECONDS_PER_YEAR 86400*365

/** Returns a list of configured UA LDS endpoints. */
const ualds_endpoint* ualds_endpoints(OpcUa_UInt32 *pNumEndpoints)
{
    if (pNumEndpoints)
    {
        *pNumEndpoints = g_numEndpoints;
    }

    return g_pEndpoints;
}

/** Returns the lds server uri. */
const char* ualds_serveruri()
{
    return g_szServerUri;
}

/** Returns the lds product uri. */
const char* ualds_producturi()
{
    return g_szProductUri;
}

/** Returns the lds application name for the given locale. */
const char* ualds_applicationname(const char *szLocale)
{
    /** TODO: add locale handling */
    OpcUa_ReferenceParameter(szLocale);
    return g_szApplicationName;
}

/* OPC UA Service forward declarations */
OpcUa_StatusCode ualds_findservers(
    OpcUa_Endpoint        hEndpoint,
    OpcUa_Handle          hContext,
    OpcUa_Void          **ppRequest,
    OpcUa_EncodeableType *pRequestType);
OpcUa_StatusCode ualds_getendpoints(
    OpcUa_Endpoint        hEndpoint,
    OpcUa_Handle          hContext,
    OpcUa_Void          **ppRequest,
    OpcUa_EncodeableType *pRequestType);
OpcUa_StatusCode ualds_registerserver(
    OpcUa_Endpoint        hEndpoint,
    OpcUa_Handle          hContext,
    OpcUa_Void          **ppRequest,
    OpcUa_EncodeableType *pRequestType);
static OpcUa_StatusCode ualds_delete_security_policies();
#ifdef HAVE_HDS
OpcUa_StatusCode ualds_registerserver2(OpcUa_Endpoint        hEndpoint,
                                       OpcUa_Handle          hContext,
                                       OpcUa_Void          **ppRequest,
                                       OpcUa_EncodeableType *pRequestType);
OpcUa_StatusCode ualds_findserversonnetwork(OpcUa_Endpoint        hEndpoint,
                                            OpcUa_Handle          hContext,
                                            OpcUa_Void          **ppRequest,
                                            OpcUa_EncodeableType *pRequestType);
#endif /* HAVE_HDS */

/* OPC UA STACK Service Type configurations */
static OpcUa_ServiceType FindServersService =
{
    OpcUaId_FindServersRequest,
    &OpcUa_FindServersResponse_EncodeableType,
    ualds_findservers,
    0
};

static OpcUa_ServiceType GetEndPointsService =
{
    OpcUaId_GetEndpointsRequest,
    &OpcUa_GetEndpointsResponse_EncodeableType,
    ualds_getendpoints,
    0
};

static OpcUa_ServiceType RegisterServerService =
{
    OpcUaId_RegisterServerRequest,
    &OpcUa_RegisterServerResponse_EncodeableType,
    ualds_registerserver,
    0
};

#ifdef HAVE_HDS
static OpcUa_ServiceType RegisterServer2Service =
{
    OpcUaId_RegisterServer2Request,
    &OpcUa_RegisterServer2Response_EncodeableType,
    ualds_registerserver2,
    0
};

static OpcUa_ServiceType FindServersOnNetworkService =
{
    OpcUaId_FindServersOnNetworkRequest,
    &OpcUa_FindServersOnNetworkResponse_EncodeableType,
    ualds_findserversonnetwork,
    0
};
#endif /* HAVE_HDS */

/** Service table for EndPointOpen */
static OpcUa_ServiceType *g_ServiceTable[] =
{
    &FindServersService,
    &GetEndPointsService,
    &RegisterServerService,
#ifdef HAVE_HDS
    &RegisterServer2Service,
    &FindServersOnNetworkService,
#endif /* HAVE_HDS */
    0
};

static OpcUa_ServiceType *g_ServiceTableHttps[] =
{
    &FindServersService,
    &GetEndPointsService,
#ifdef HAVE_HDS
    &FindServersOnNetworkService,
#endif /* HAVE_HDS */
    0
};

/** Initializes the given \c pEndpoint structure. */
void ualds_endpoint_initialize(ualds_endpoint *pEndpoint)
{
    pEndpoint->szUrl[0] = 0;
    pEndpoint->nNoOfSecurityPolicies = 0;
    pEndpoint->pSecurityPolicies = 0;
}

/** Cleans up all resources referenced by \c pEndpoint. */
void ualds_endpoint_clear(ualds_endpoint *pEndpoint)
{
    OpcUa_UInt32 i;

    if (pEndpoint->pSecurityPolicies)
    {
        for (i=0; i<pEndpoint->nNoOfSecurityPolicies; i++)
        {
            OpcUa_String_Clear(&pEndpoint->pSecurityPolicies[i].sSecurityPolicy);
            if (pEndpoint->pSecurityPolicies[i].pbsClientCertificate)
            {
                OpcUa_ByteString_Clear(pEndpoint->pSecurityPolicies[i].pbsClientCertificate);
            }
        }
        OpcUa_Free(pEndpoint->pSecurityPolicies);
    }
}

static OpcUa_StatusCode ualds_create_security_policies()
{
    OpcUa_UInt32 i, n;
    int j;
    char szSecurityPolicies[256] = "";
    char szUrl[256] = "";
    char szMessageSecurity[50] = "";
    const char **szPolicyArray = 0;
    const char **szModeArray = 0;
    int numModes = 0;
    size_t size;
    OpcUa_StatusCode ret = OpcUa_Good;
    int tmpInt;

    int retCode = ualds_settings_begingroup("General");
    if (retCode > 0)
    {
        ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read General settings from config file.");
        return OpcUa_BadConfigurationError;
    }

    retCode = ualds_settings_beginreadarray("Endpoints", &tmpInt);
    if (retCode > 0)
    {
        ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read number of endpoints from config file.");
        return OpcUa_BadConfigurationError;
    }

    g_numEndpoints = tmpInt;
    /* create endpoint configurastion array */
    g_pEndpoints = OpcUa_Alloc(sizeof(ualds_endpoint) * g_numEndpoints);
    if (g_pEndpoints == 0)
    {
        ualds_log(UALDS_LOG_CRIT, "Could not create endpoint configurastion array. Out of memory.");
        ualds_settings_endarray();
        ualds_settings_endgroup();
        return OpcUa_BadOutOfMemory;
    }
    OpcUa_MemSet(g_pEndpoints, 0, sizeof(ualds_endpoint) * g_numEndpoints);
    /* read in all endpoints configuration */
    for (n=0; n<g_numEndpoints; n++)
    {
        retCode = ualds_settings_setarrayindex(n);
        if (retCode > 0)
        {
            ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read endpoint index from config file.");
            ualds_delete_security_policies();
            return OpcUa_BadConfigurationError;
        }

        retCode = ualds_settings_readstring("Url", g_pEndpoints[n].szUrl, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode > 0)
        {
            ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read endpoint url from config file.");
            ret =  OpcUa_BadConfigurationError;
            break;
        }

        replace_string(g_pEndpoints[n].szUrl, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", g_szHostname);
        retCode = ualds_settings_readstring("SecurityPolicies", szSecurityPolicies, sizeof(szSecurityPolicies));
        if (retCode > 0)
        {
            ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read security policies for endpoint from config file.");
            ret = OpcUa_BadConfigurationError;
            break;
        }

        // check if there is a None secured configured. If not, it must be added manually.
        char* findNoneSecured = strstr(szSecurityPolicies, "SecurityPolicy_None");
        if (findNoneSecured == NULL)
        {
            strlcat(szSecurityPolicies, ", SecurityPolicy_None", 256);
        }
        g_pEndpoints[n].nNoOfSecurityPolicies = split_string(szSecurityPolicies, ',', &szPolicyArray);
        if (g_pEndpoints[n].nNoOfSecurityPolicies < 1)
        {
            ualds_log(UALDS_LOG_ERR, "configuration error: no security policies found.");
            ret = OpcUa_BadConfigurationError;
            break;
        }
        /* allocate policy array */
        size = sizeof(OpcUa_Endpoint_SecurityPolicyConfiguration) * (g_pEndpoints[n].nNoOfSecurityPolicies);
        g_pEndpoints[n].pSecurityPolicies = (OpcUa_Endpoint_SecurityPolicyConfiguration*)OpcUa_Alloc((OpcUa_UInt32)size);
        if (g_pEndpoints[n].pSecurityPolicies == 0)
        {
            /** Note: we need to cast here to non-const (void*) because the MS compiler doesn't understand,
             * that this is a pointer to pointer to const data. Freeing the pointer does not modify the data,
             * it only frees the array of pointers. With other compilers like GCC this works without the cast.
             */
            free((void*)szPolicyArray);
            szPolicyArray = 0;
            ret = OpcUa_BadOutOfMemory;
            break;
        }
        OpcUa_MemSet(g_pEndpoints[n].pSecurityPolicies, 0, size);
        /* fill policy array */
        for (i=0; i<g_pEndpoints[n].nNoOfSecurityPolicies; i++)
        {
            /* temporary store the security policy configuration name */
            OpcUa_String_Initialize(&g_pEndpoints[n].pSecurityPolicies[i].sSecurityPolicy);
            OpcUa_String_AttachCopy(&g_pEndpoints[n].pSecurityPolicies[i].sSecurityPolicy, (OpcUa_StringA)szPolicyArray[i]);
        }

        free((void*)szPolicyArray);
        szPolicyArray = 0;
    }
    ualds_settings_endarray();
    ualds_settings_endgroup();

    if (OpcUa_IsGood(ret))
    {
        /* fill security policy information */
        for (n=0; n<g_numEndpoints; n++)
        {
            for (i=0; i<g_pEndpoints[n].nNoOfSecurityPolicies; i++)
            {
                retCode = ualds_settings_begingroup(OpcUa_String_GetRawString(&g_pEndpoints[n].pSecurityPolicies[i].sSecurityPolicy));
                if (retCode > 0)
                {
                    ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read security policies group from config file.");
                    ret = OpcUa_BadConfigurationError;
                    break;
                }

                retCode = ualds_settings_readstring("Url", szUrl, sizeof(szUrl));
                if (retCode > 0)
                {
                    ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read Url from security policies group from config file.");
                    ret = OpcUa_BadConfigurationError;
                    break;
                }

                retCode = ualds_settings_readstring("MessageSecurity", szMessageSecurity, sizeof(szMessageSecurity));
                if (retCode > 0)
                {
                    ualds_log(UALDS_LOG_ERR, "Configuration error: Could not read MessageSecurity from security policies group from config file.");
                    ret = OpcUa_BadConfigurationError;
                    break;
                }

                ualds_settings_endgroup();

                OpcUa_String_Clear(&g_pEndpoints[n].pSecurityPolicies[i].sSecurityPolicy);
                OpcUa_String_AttachCopy(&g_pEndpoints[n].pSecurityPolicies[i].sSecurityPolicy, szUrl);
                g_pEndpoints[n].pSecurityPolicies[i].uMessageSecurityModes = 0;

                szModeArray = 0;
                numModes = split_string(szMessageSecurity, ',', &szModeArray);
                if (szModeArray == NULL)
                {
                    return OpcUa_BadConfigurationError;
                }

                for (j=0; j<numModes; j++)
                {
                    if (strcmp(szModeArray[j], "None") == 0)
                    {
                        g_pEndpoints[n].pSecurityPolicies[i].uMessageSecurityModes |= OPCUA_ENDPOINT_MESSAGESECURITYMODE_NONE;
                    }
                    else if (strcmp(szModeArray[j], "Sign") == 0)
                    {
                        g_pEndpoints[n].pSecurityPolicies[i].uMessageSecurityModes |= OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGN;
                    }
                    else if (strcmp(szModeArray[j], "SignAndEncrypt") == 0)
                    {
                        g_pEndpoints[n].pSecurityPolicies[i].uMessageSecurityModes |= OPCUA_ENDPOINT_MESSAGESECURITYMODE_SIGNANDENCRYPT;
                    }
                    else
                    {
                        ualds_log(UALDS_LOG_WARNING, "Ignored invalid message security mode '%s'.", szModeArray[j]);
                    }
                }
                if (szModeArray)
                {
                    /* see note above */
                    free((void*)szModeArray);
                    szModeArray = 0;
                }
            }
        }
    }
    return ret;
}

static OpcUa_StatusCode ualds_delete_security_policies()
{
    OpcUa_UInt32 i;

    if (g_pEndpoints)
    {
        for (i=0; i<g_numEndpoints; i++)
        {
            ualds_endpoint_clear(&g_pEndpoints[i]);
        }
        OpcUa_Free(g_pEndpoints);
        g_pEndpoints = 0;
        g_numEndpoints = 0;
    }

    return OpcUa_Good;
}

static void ualds_datetime_from_time_t(time_t t, OpcUa_DateTime *pDate)
{
    OpcUa_UInt64 tmp = t;

    tmp += UINT64_C(11644473600);
    tmp *= 10000000;

    pDate->dwHighDateTime = tmp >> 32;
    pDate->dwLowDateTime = (OpcUa_UInt32)tmp;
}

#if HAVE_OPENSSL
static OpcUa_StatusCode ualds_create_selfsigned_certificates(OpcUa_Handle hCertificateStore)
{
    OpcUa_StatusCode ret = OpcUa_Good;
    OpcUa_CryptoProvider crypto;
    OpcUa_Int32 serial = (OpcUa_Int32)time(0);
    OpcUa_Crypto_NameEntry NameEntries[7];
    OpcUa_UInt numNameEntries = 7;
    OpcUa_Certificate pCert = OPCUA_BYTESTRING_STATICINITIALIZER;
    OpcUa_Key pubKey, prvKey;
    char szSubjectAltName[256] = {0};
    char szCommonName[50] = {0};
    char szOrganization[50] = {0};
    char szOrganizationUnit[50] = {0};
    char szLocality[50] = {0};
    char szState[50] = {0};
    char szCountry[5] = {0};
    int i = 0;
    UALDS_FILE* f;

    UALDS_UNUSED(hCertificateStore);

    ret = OpcUa_CryptoProvider_Create(OpcUa_SecurityPolicy_Basic128Rsa15, &crypto);
    OpcUa_ReturnErrorIfBad(ret);

    OpcUa_Key_Initialize(&pubKey);
    OpcUa_Key_Initialize(&prvKey);

    ret = OpcUa_Crypto_GenerateAsymmetricKeypair(
        &crypto,
        OpcUa_Crypto_Rsa_Id,
        2048,
        &pubKey,
        &prvKey
    );
    OpcUa_GotoErrorIfBad(ret);

    ualds_settings_begingroup("CertificateInfo");
    ualds_settings_readstring("CommonName", szCommonName, sizeof(szCommonName));
    ualds_settings_readstring("Organization", szOrganization, sizeof(szOrganization));
    ualds_settings_readstring("OrganizationUnit", szOrganizationUnit, sizeof(szOrganizationUnit));
    ualds_settings_readstring("Locality", szLocality, sizeof(szLocality));
    ualds_settings_readstring("State", szState, sizeof(szState));
    ualds_settings_readstring("Country", szCountry, sizeof(szCountry));
    ualds_settings_endgroup();

    i = 0;
    if (strlen(szOrganization) > 0) {
        NameEntries[i].key      = "O";  /* Organization */
        NameEntries[i++].value  = szOrganization;
    }
    if (strlen(szOrganizationUnit) > 0) {
        NameEntries[i].key      = "OU"; /* Organization Unit */
        NameEntries[i++].value  = szOrganizationUnit;
    }
    if (strlen(szLocality) > 0) {
        NameEntries[i].key      = "L";  /* Locality */
        NameEntries[i++].value  = szLocality;
    }
    if (strlen(szState) > 0) {
        NameEntries[i].key      = "ST"; /* State */
        NameEntries[i++].value  = szState;
    }
    if (strlen(szCountry) > 0) {
        NameEntries[i].key      = "C";  /* Country */
        NameEntries[i++].value  = szCountry;
    }
    if (strlen(szCommonName) > 0) {
        NameEntries[i].key      = "CN"; /* Common Name */
        NameEntries[i++].value  = szCommonName;
    }
    if (strlen(g_szHostname) > 0) {
        NameEntries[i].key      = "DC"; /* Domain Component */
        NameEntries[i++].value  = g_szHostname;
    }
    numNameEntries = i;

    snprintf(szSubjectAltName, sizeof(szSubjectAltName), "URI:%s, DNS:%s", g_szServerUri, g_szHostname);
    replace_string(szSubjectAltName, sizeof(szSubjectAltName), "[gethostname]", g_szHostname);
    ext_ent[0].value = szSubjectAltName;

    ret = OpcUa_Crypto_CreateCertificate(
        &crypto,
        serial,
        3 * SECONDS_PER_YEAR,
        NameEntries,
        numNameEntries,
        pubKey,
        ext_ent,
        EXT_COUNT,
        OPCUA_P_SHA_256,
        prvKey,
        &pCert);
    OpcUa_GotoErrorIfBad(ret);

    f = ualds_platform_fopen(g_szCertificateFile, "wb");
    if (f != NULL)
    {
        ualds_platform_fwrite(pCert.Data, 1, pCert.Length, f);
        ualds_platform_fclose(f);
    }
    else
    {
        ualds_log(UALDS_LOG_ERR, "ualds_create_selfsigned_certificates: Cloud not create self-signed certificate file. \"%s\"!", g_szCertificateFile);
    }

    f = ualds_platform_fopen(g_szCertificateKeyFile, "wb");
    if (f != NULL)
    {
        ualds_platform_fwrite(prvKey.Key.Data, 1, prvKey.Key.Length, f);
        ualds_platform_fclose(f);
    }
    else
    {
        ualds_log(UALDS_LOG_ERR, "ualds_create_selfsigned_certificates: Cloud not create self-signed certificate file. \"%s\"!", g_szCertificateKeyFile);
    }

Error:
    OpcUa_CryptoProvider_Delete(&crypto);
    OpcUa_ByteString_Clear(&pCert);
    OpcUa_Key_Clear(&prvKey);
    OpcUa_Key_Clear(&pubKey);

    return ret;
}
#endif /* HAVE_OPENSSL */

#ifdef _WIN32
static OpcUa_StatusCode ualds_override_validate_certificate(
    struct _OpcUa_PKIProvider*  pPKI,
    OpcUa_ByteString*           pCertificate,
    OpcUa_Void*                 pCertificateStore,
    OpcUa_Int*                  pValidationCode)
{
  OpcUa_StatusCode uStatus = g_PkiProvider.ValidateCertificate(pPKI, pCertificate, pCertificateStore, pValidationCode);

  if (uStatus == OpcUa_BadCertificateUntrusted && g_bAllowLocalRegistration)
  {
      ualds_log(UALDS_LOG_DEBUG, "ualds_override_validate_certificate: Ignoring BadCertificateUntrusted error, because AllowLocalRegistration is set to yes.");
      uStatus = OpcUa_Good;
  }

  // Certificate store paths have changed at some point. 
  // Make a check using the old standard paths, for backward compatibility
  if (uStatus == OpcUa_BadCertificateUntrusted)
  {
      OpcUa_StatusCode uStatusVerify = ualds_verify_cert_old_default_location(pCertificate, g_szCRLPath, g_szRejectedPath, pValidationCode);
      if (OpcUa_IsGood(uStatusVerify))
      {
          ualds_log(UALDS_LOG_DEBUG, "Verifying certificate in old default store succeeded.");
          uStatus = OpcUa_Good;
      }
  }

  // Certificate store paths have changed at some point. 
  // Make a check using the old edited paths, for backward compatibility
  if (uStatus == OpcUa_BadCertificateUntrusted)
  {
      OpcUa_StatusCode uStatusVerify = ualds_verify_cert_old_edited_location(pCertificate, g_szCRLPath, g_szRejectedPath, g_szTrustListPathOldEditedLocation, pValidationCode);
      if (OpcUa_IsGood(uStatusVerify))
      {
          ualds_log(UALDS_LOG_DEBUG, "Verifying certificate in old edited store succeeded.");
          uStatus = OpcUa_Good;
      }
  }

#if OPCUA_SUPPORT_PKI_WIN32
  if (uStatus == OpcUa_BadCertificateUntrusted && g_bWin32StoreCheck)
  {
    OpcUa_StatusCode uStatusVerify = ualds_verify_cert_win32(pCertificate);
    if (OpcUa_IsGood(uStatusVerify))
    {
      ualds_log(UALDS_LOG_DEBUG, "Verifying certificate in windows store succeeded.");
      uStatus = OpcUa_Good;
    }
  }
#endif /* OPCUA_SUPPORT_PKI_WIN32 */

  return uStatus;
}
#endif /* _WIN32 */

static OpcUa_StatusCode ualds_load_certificate(OpcUa_Handle hCertificateStore)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "ualds_load_certificate");

    /* load DER encoded server certificate */
    uStatus = g_PkiProvider.LoadCertificate(
        &g_PkiProvider,
        g_szCertificateFile,
        hCertificateStore,
        &g_server_certificate);

    if (OpcUa_IsBad(uStatus))
    {
        ualds_log(UALDS_LOG_ERR, "Failed to load server certificate \"%s\"! (0x%08X)", g_szCertificateFile, uStatus);
        OpcUa_GotoError;
    }

    OpcUa_Key_Initialize(&g_server_key);

    /* load DER encoded server certificate private key */
    uStatus = g_PkiProvider.LoadPrivateKeyFromFile(
        g_szCertificateKeyFile,
        OpcUa_Crypto_Encoding_DER,
        OpcUa_Null,
        OpcUa_Crypto_KeyType_Rsa_Private,
        &g_server_key);

    if (OpcUa_IsBad(uStatus))
    {
        /* load PEM encoded server certificate private key */
        uStatus = g_PkiProvider.LoadPrivateKeyFromFile(
            g_szCertificateKeyFile,
            OpcUa_Crypto_Encoding_PEM,
            OpcUa_Null,
            OpcUa_Crypto_KeyType_Rsa_Private,
            &g_server_key);
    }

    if (OpcUa_IsBad(uStatus))
    {
        ualds_log(UALDS_LOG_ERR, "Failed to load server private key \"%s\"! (0x%08X)", g_szCertificateKeyFile, uStatus);
        OpcUa_GotoError;
    }

    OpcUa_ReturnStatusCode;

    OpcUa_BeginErrorHandling;
    OpcUa_ByteString_Clear(&g_server_certificate);
    OpcUa_Key_Clear(&g_server_key);
    OpcUa_FinishErrorHandling;
}

static OpcUa_StatusCode ualds_security_initialize()
{
    OpcUa_Handle hCertificateStore = OpcUa_Null;
    char         szValue[10];

OpcUa_InitializeStatus(OpcUa_Module_Server, "ualds_security_initialize");

    ualds_settings_begingroup("PKI");
    int reCreateOwnCertificateOnError = 0;

    UALDS_SETTINGS_READSTRING(CertificateStorePath);
    if (g_szCertificateStorePath[0] == 0)
    {
        ualds_log(UALDS_LOG_WARNING, "Certificate store path (Section: 'PKI', Key: 'CertificateStorePath') is not set in the settings file!");
    }

    int szCertificateStorePathLen = strlen(g_szCertificateStorePath);

    // 50 characters are enough to complete the paths to subfolders. The subfolder structure is fixed.
    if (szCertificateStorePathLen > PATH_MAX - 50)
    {
        ualds_log(UALDS_LOG_WARNING, "Certificate store path is too long!");
        uStatus = OpcUa_Bad;
        OpcUa_GotoError;
    }

    //check if path ends with dir separator
    char* directory_separator = __ualds_plat_path_sep;
    if (szCertificateStorePathLen > 0)
    {
        if (g_szCertificateStorePath[szCertificateStorePathLen - 1] != *directory_separator)
        {
            strlcat(g_szCertificateStorePath, directory_separator, PATH_MAX);
        }
    }
        
    if (ualds_platform_mkpath(g_szCertificateStorePath) != 0)
    {
        g_szCertificateStorePath[0] = 0;
        ualds_platform_getcwd(g_szCertificateStorePath, sizeof(g_szCertificateStorePath));
        strlcat(g_szCertificateStorePath, __ualds_plat_path_sep "pki" __ualds_plat_path_sep, PATH_MAX);
        ualds_log(UALDS_LOG_ALERT, "Failed to create certificate store path - using %s as path...", g_szCertificateStorePath);
    }
    else
    {
        ualds_log(UALDS_LOG_NOTICE, "Using certificate store at %s...", g_szCertificateStorePath);
    }

    // The folder structure of the CertificateStore is specified in OPC-UA Spec 1.03, Part 12, Table 48
    strlcpy(g_szCertificateFile, g_szCertificateStorePath, PATH_MAX);
    strlcat(g_szCertificateFile, "own" __ualds_plat_path_sep "certs" __ualds_plat_path_sep, PATH_MAX);
    ualds_platform_mkpath(g_szCertificateFile);
    strlcat(g_szCertificateFile, "ualdscert.der", PATH_MAX);
    
    strlcpy(g_szCertificateKeyFile, g_szCertificateStorePath, PATH_MAX);
    strlcat(g_szCertificateKeyFile, "own" __ualds_plat_path_sep "private" __ualds_plat_path_sep, PATH_MAX);
    ualds_platform_mkpath(g_szCertificateKeyFile);
    strlcat(g_szCertificateKeyFile, "ualdskey.nopass.pem", PATH_MAX);

    strlcpy(g_szTrustListPath, g_szCertificateStorePath, PATH_MAX);
    strlcat(g_szTrustListPath, "trusted" __ualds_plat_path_sep "certs" __ualds_plat_path_sep, PATH_MAX);
    ualds_platform_mkpath(g_szTrustListPath);

    strlcpy(g_szCRLPath, g_szCertificateStorePath, PATH_MAX);
    strlcat(g_szCRLPath, "trusted" __ualds_plat_path_sep "crl" __ualds_plat_path_sep, PATH_MAX);
    ualds_platform_mkpath(g_szCRLPath);

    strlcpy(g_szRejectedPath, g_szCertificateStorePath, PATH_MAX);
    strlcat(g_szRejectedPath, "rejected" __ualds_plat_path_sep "certs" __ualds_plat_path_sep, PATH_MAX);
    ualds_platform_mkpath(g_szRejectedPath);

    strlcpy(g_szIssuerPath, g_szCertificateStorePath, PATH_MAX);
    strlcat(g_szIssuerPath, "issuer" __ualds_plat_path_sep "certs" __ualds_plat_path_sep, PATH_MAX);
    ualds_platform_mkpath(g_szIssuerPath);

    ualds_settings_readstring("TrustListPath", g_szTrustListPathOldEditedLocation, PATH_MAX);

#ifdef _WIN32
    if (ualds_settings_readstring("Win32StoreCheck", szValue, sizeof(szValue)) == 0)
    {
        if (strcmp(szValue, "yes") == 0)
        {
#if OPCUA_SUPPORT_PKI_WIN32
            ualds_log(UALDS_LOG_DEBUG, "Win32StoreCheck is enabled.");
            g_bWin32StoreCheck = 1;
#else /* OPCUA_SUPPORT_PKI_WIN32 */
            ualds_log(UALDS_LOG_ERR, "Could not enable Win32StoreCheck because UaStack was built without OPCUA_SUPPORT_PKI_WIN32.");
#endif /* OPCUA_SUPPORT_PKI_WIN32 */
        }
    }
#endif /* _WIN32 */

    /* Check if we should re-create the own certificate if we found an error */
    if (ualds_settings_readstring("ReCreateOwnCertificateOnError", szValue, sizeof(szValue)) == 0)
    {
        if (strcmp(szValue, "yes") == 0)
        {
            ualds_log(UALDS_LOG_INFO, "ReCreateOwnCertificateOnError is enabled.");
            reCreateOwnCertificateOnError = 1;
        }
    }

    ualds_settings_endgroup();

#if HAVE_OPENSSL
    g_PKIConfig.PkiType = OpcUa_OpenSSL_PKI;
    g_PKIConfig.CertificateTrustListLocation = g_szTrustListPath;
    g_PKIConfig.CertificateRevocationListLocation = g_szCRLPath;
    g_PKIConfig.CertificateUntrustedListLocation = g_szRejectedPath;
    g_PKIConfig.Flags = OPCUA_P_PKI_OPENSSL_USE_DEFAULT_CERT_CRL_LOOKUP_METHOD;
    g_PKIConfig.Override = OpcUa_Null;

    uStatus = OpcUa_PKIProvider_Create(&g_PKIConfig, &g_PkiProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = g_PkiProvider.OpenCertificateStore(&g_PkiProvider, &hCertificateStore);
    if (OpcUa_IsBad(uStatus))
    {
        ualds_log(UALDS_LOG_ERR, "Failed to open certificate store! (0x%08X)", uStatus);
        OpcUa_GotoError;
    }

    /* create self-signed certificates if no certificates are provided. */
    if (ualds_platform_fileexists(g_szCertificateFile) == 0 ||
        ualds_platform_fileexists(g_szCertificateKeyFile) == 0)
    {
        ualds_create_selfsigned_certificates(hCertificateStore);
    }
    else
    {
        if (reCreateOwnCertificateOnError)
        {
            int reCreateCert = 0;
            /* try to load certificate and check domain name */

            /* loads the server certificate */
            uStatus = ualds_load_certificate(hCertificateStore);
            if (OpcUa_IsBad(uStatus))
            {
                ualds_log(UALDS_LOG_ERR, "Failed to load server certificate! (0x%08X)", uStatus);
                reCreateCert = 1;
            }
            else
            {
                OpcUa_ByteString pSubjectDNS;
                uStatus = g_PkiProvider.ExtractCertificateData(&g_server_certificate, NULL, NULL, NULL, NULL, &pSubjectDNS, NULL, NULL, NULL);
                if (OpcUa_IsGood(uStatus))
                {
                    if (strcmp(pSubjectDNS.Data, g_szHostname) != 0)
                    {
                        ualds_log(UALDS_LOG_ERR, "Server certificate DNS entry (%s) and current hostname (%s) is NOT the same!", pSubjectDNS.Data, g_szHostname);
                        reCreateCert = 1;
                    }
                }
                else
                {
                    ualds_log(UALDS_LOG_ERR, "Failed to extract server certificate DNS subject! (0x%08X)", uStatus);
                    reCreateCert = 1;
                }
                OpcUa_ByteString_Clear(&pSubjectDNS);
            }

            if (reCreateCert)
            {
                ualds_log(UALDS_LOG_NOTICE, "Re-Creating server certificate!");

                /* clear a previously loaded certificate */
                OpcUa_ByteString_Clear(&g_server_certificate);
                OpcUa_Key_Clear(&g_server_key);

                /* create new self-signed certificate  */
                ualds_create_selfsigned_certificates(hCertificateStore);
            }
        }
    }


    /* check if certificate is already loaded (during the above check) */
    if (g_server_certificate.Data == OpcUa_Null)
    {
        /* loads the server certificate */
        uStatus = ualds_load_certificate(hCertificateStore);

        if (OpcUa_IsBad(uStatus))
        {
            ualds_log(UALDS_LOG_ERR, "Failed to load server private key \"%s\"! (0x%08X)", g_szCertificateKeyFile, uStatus);
            OpcUa_GotoError;
        }

    }

    g_PkiProvider.CloseCertificateStore(&g_PkiProvider, &hCertificateStore);
#else /* HAVE_OPENSSL */
    g_PKIConfig.PkiType = OpcUa_NO_PKI;
    g_PKIConfig.CertificateTrustListLocation = 0;
    g_PKIConfig.CertificateRevocationListLocation = 0;
    g_PKIConfig.Flags = 0;
    g_PKIConfig.Override = OpcUa_Null;
#endif /* HAVE_OPENSSL */

#ifdef _WIN32
    memset(&g_Win32Override, 0, sizeof(g_Win32Override));
    g_Win32Override.ValidateCertificate = ualds_override_validate_certificate;
    g_Win32Config = g_PKIConfig;
    g_Win32Config.PkiType = OpcUa_Override;
    g_Win32Config.Override = &g_Win32Override;
#endif /* _WIN32 */

    ualds_create_security_policies();

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    ualds_log(UALDS_LOG_ERR, "Could not initialize UALDS PKI infrastructure.");

    if (hCertificateStore != OpcUa_Null)
    {
        g_PkiProvider.CloseCertificateStore(&g_PkiProvider, &hCertificateStore);
    }

OpcUa_FinishErrorHandling;
}

static OpcUa_StatusCode ualds_security_uninitialize()
{
    OpcUa_ByteString_Clear(&g_server_certificate);
    OpcUa_Key_Clear(&g_server_key);

    OpcUa_PKIProvider_Delete(&g_PkiProvider);

    return ualds_delete_security_policies();
}

/** File filter for scandir. All entries where this functions return non-zero,
 * will be stored in scandir namelist.
 * This function filters for file ending with "*.der".
 */
static int ualds_certificate_filefilter(const struct ualds_dirent *entry)
{
    int len = strlen(entry->d_name);
    if (len < 4) return 0;
    return (strcmp(entry->d_name+len-4, ".der") == 0);
}

/** This function is used by ualds_platform_scandir which does a qsort() call
 * to sort the file entries.
 * We implement a sort filter here to sort all entries by creation date descending (youngest first).
 */
static int ualds_certificate_filesort(const struct ualds_dirent **a, const struct ualds_dirent **b)
{
    char szPathA[PATH_MAX];
    char szPathB[PATH_MAX];
    struct ualds_stat statA;
    struct ualds_stat statB;

    strlcpy(szPathA, g_szRejectedPath, PATH_MAX);
    strlcat(szPathA, "/", PATH_MAX);
    strlcat(szPathA, (*a)->d_name, PATH_MAX);
    strlcpy(szPathB, g_szRejectedPath, PATH_MAX);
    strlcat(szPathB, "/", PATH_MAX);
    strlcat(szPathB, (*b)->d_name, PATH_MAX);

    if (ualds_platform_stat(szPathA, &statA) == 0 &&
        ualds_platform_stat(szPathB, &statB) == 0)
    {
        return (statA.st_ctime < statB.st_ctime);
    }

    return 0;
}

static OpcUa_Void ualds_cleanup_rejected()
{
    int ret;
    struct ualds_dirent **namelist;
    struct ualds_stat statF;
    char szPath[PATH_MAX];
    int i;
    time_t now = time(0);

    /* get list of all certificate files, orderd by their creation date */
    ret = ualds_platform_scandir(
        g_szRejectedPath,
        &namelist,
        ualds_certificate_filefilter,
        ualds_certificate_filesort);

    if (ret != -1)
    {
        /* scan loop: find first certificate which should be removed. */
        for (i=0; i<ret; i++)
        {
            ualds_log(UALDS_LOG_DEBUG, "%s", namelist[i]->d_name);
            strlcpy(szPath, g_szRejectedPath, PATH_MAX);
            strlcat(szPath, "/", PATH_MAX);
            strlcat(szPath, namelist[i]->d_name, PATH_MAX);

            /* g_MaxRejectedCertificates-1 because we are actually adding a new one when this function gets called. */
            if (i >= (g_MaxRejectedCertificates-1))
            {
                ualds_log(UALDS_LOG_DEBUG, "Reached maximum number of rejected certificates.");
                break; /* start delete loop */
            }
            else
            {
                if (ualds_platform_stat(szPath, &statF) == 0)
                {
                    time_t age = now - statF.st_ctime;

                    age /= 86000; /* convert to days */
                    if (age > g_MaxAgeRejectedCertificates)
                    {
                        ualds_log(UALDS_LOG_DEBUG, "Reached maximum age of rejected certificates.");
                        break; /* start delete loop */
                    }
                }
            }
            free(namelist[i]);
        }
        /* delete loop: remove all certificates from here. */
        for (; i<ret; i++)
        {
            strlcpy(szPath, g_szRejectedPath, PATH_MAX);
            strlcat(szPath, "/", PATH_MAX);
            strlcat(szPath, namelist[i]->d_name, PATH_MAX);
            ualds_log(UALDS_LOG_DEBUG, "Removing certificate %s from rejected folder.", namelist[i]->d_name);
            ualds_platform_rm(szPath);
            free(namelist[i]);
        }
        free(namelist);
    }
}

static OpcUa_Void ualds_save_clientcertificate(OpcUa_ByteString* a_pbsClientCertificate)
{
#if OPCUA_SUPPORT_PKI
    unsigned char hash[20];
    char szHash[41];
    int i, j;
    unsigned char h, l;
    UALDS_FILE *f;
    char szPath[PATH_MAX];

    if (a_pbsClientCertificate == OpcUa_Null) return;
    if (a_pbsClientCertificate->Length <= 0) return;
    if (a_pbsClientCertificate->Data == OpcUa_Null) return;

    ualds_cleanup_rejected();

    /* compute SHA1 hash from certificate data */
    SHA1(a_pbsClientCertificate->Data, a_pbsClientCertificate->Length, hash);
    for (i = 0, j = 0; i < 20; i++)
    {
        h = (hash[i] >> 4);
        l = (hash[i] & 0xf);
        /* hex string conversion */
        szHash[j++] = h > 9 ? 'A' + h - 10 : '0' + h;
        szHash[j++] = l > 9 ? 'A' + l - 10 : '0' + l;
    }
    szHash[j++] = 0;

    strlcpy(szPath, g_szRejectedPath, sizeof(szPath));
    strlcat(szPath, "/", sizeof(szPath));
    strlcat(szPath, szHash, sizeof(szPath));
    strlcat(szPath, ".der", sizeof(szPath));

    f = ualds_platform_fopen(szPath, "wb");
    if (f)
    {
        ualds_platform_fwrite(a_pbsClientCertificate->Data, a_pbsClientCertificate->Length, 1, f);
        ualds_platform_fclose(f);
    }
    else
    {
        ualds_log(UALDS_LOG_ERR, "Failed to write certificate into rejected folder.");
    }
#endif /* OPCUA_SUPPORT_PKI */
}

static OpcUa_StatusCode ualds_endpoint_callback(
    OpcUa_Endpoint          hEndpoint,
    OpcUa_Void*             pvCallbackData,
    OpcUa_Endpoint_Event    eEvent,
    OpcUa_StatusCode        uStatus,
    OpcUa_UInt32            uSecureChannelId,
    OpcUa_ByteString*       pbsClientCertificate,
    OpcUa_String*           pSecurityPolicy,
    OpcUa_UInt16            uSecurityMode)
{
    int index = eEvent;
    UALDS_UNUSED(pvCallbackData);

    if (index < 0 || index > 5) index = 0;
    ualds_log(UALDS_LOG_INFO, "ualds_endpoint_callback called: Event=%s, SecureChanneldId=0x%08X, uStatus=0x%08X",
              g_szEndpointEventNames[index], uSecureChannelId, uStatus);
    switch (eEvent)
    {
    case eOpcUa_Endpoint_Event_SecureChannelOpened:
        {
            ualds_log(UALDS_LOG_DEBUG,
                                "ualds_endpoint_callback: SecureChannel 0x%08X opened with %s in mode %u status 0x%08X!",
                                uSecureChannelId,
                                (pSecurityPolicy)?OpcUa_String_GetRawString(pSecurityPolicy):"(not provided)",
                                uSecurityMode,
                                uStatus);
            if (uStatus == OpcUa_BadCertificateUntrusted)
            {
                /* save untrusted certificate in rejected folder */
                ualds_save_clientcertificate(pbsClientCertificate);
            }
            break;
        }
    case eOpcUa_Endpoint_Event_SecureChannelClosed:
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_endpoint_callback: SecureChannel 0x%08X closed with status 0x%08X!", uSecureChannelId, uStatus);
            break;
        }
    case eOpcUa_Endpoint_Event_SecureChannelRenewed:
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_endpoint_callback: SecureChannel 0x%08X renewed!", uSecureChannelId);
            break;
        }
    case eOpcUa_Endpoint_Event_UnsupportedServiceRequested:
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_endpoint_callback: SecureChannel 0x%08X received request for unsupported service!", uSecureChannelId);
            break;
        }
    case eOpcUa_Endpoint_Event_DecoderError:
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_endpoint_callback: SecureChannel 0x%08X received a request that could not be decoded! (0x%08X)", uSecureChannelId, uStatus);
            break;
        }
    case eOpcUa_Endpoint_Event_Invalid:
    default:
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_endpoint_callback: Unknown Endpoint event!");
            break;
        }
    }

    return uStatus;
}

static OpcUa_StatusCode ualds_create_endpoints()
{
    OpcUa_StatusCode ret = OpcUa_Good;
    ualds_endpoint *pEP = g_pEndpoints;
    OpcUa_UInt32 i;

    if (pEP == NULL)
    {
        ualds_log(UALDS_LOG_NOTICE, "No endpoints to open!");
        return OpcUa_Bad;
    }

    for (i=0; i<g_numEndpoints; i++, pEP++)
    {
        /* only opc.tcp endpoints allow RegisterServer */
        if (tolower(pEP->szUrl[0]) != 'o')
        {
            ret = OpcUa_Endpoint_Create(&pEP->hEndpoint, OpcUa_Endpoint_SerializerType_Binary, g_ServiceTableHttps);
            OpcUa_ReturnErrorIfBad(ret);
        }
        else
        {
            ret = OpcUa_Endpoint_Create(&pEP->hEndpoint, OpcUa_Endpoint_SerializerType_Binary, g_ServiceTable);
            OpcUa_ReturnErrorIfBad(ret);
        }

        ualds_log(UALDS_LOG_NOTICE, "Opening endpoint '%s'...", pEP->szUrl);

        ret = OpcUa_Endpoint_Open(
            pEP->hEndpoint,
            pEP->szUrl,
            OpcUa_True,
            ualds_endpoint_callback,
            OpcUa_Null,
            &g_server_certificate,
            &g_server_key,
#ifdef _WIN32
            &g_Win32Config,
#else
            &g_PKIConfig,
#endif /* _WIN32 */
            pEP->nNoOfSecurityPolicies,
            pEP->pSecurityPolicies);

        if (OpcUa_IsGood(ret))
        {
            ualds_log(UALDS_LOG_NOTICE, "Endpoint is open.");
        }
        else
        {
            ualds_log(UALDS_LOG_ERR, "Opening endpoint failed with error 0x%08X.", ret);
        }
    }

    return ret;
}

static OpcUa_StatusCode ualds_delete_endpoints()
{
    OpcUa_StatusCode ret = OpcUa_Good;
    OpcUa_UInt32 i;

    for (i=0; i<g_numEndpoints; i++)
    {
        ret = OpcUa_Endpoint_Close(g_pEndpoints[i].hEndpoint);
    }

    OpcUa_SocketManager_Delete(OpcUa_Null);

    for (i=0; i<g_numEndpoints; i++)
    {
        OpcUa_Endpoint_Delete(&g_pEndpoints[i].hEndpoint);
    }

    return ret;
}

static void ualds_initialize_proxystubconfig(OpcUa_ProxyStubConfiguration *pConfig)
{
    pConfig->bProxyStub_Trace_Enabled              = OpcUa_True;
    pConfig->uProxyStub_Trace_Level                = g_StackTraceLevel;
    pConfig->iSerializer_MaxAlloc                  = -1;
    pConfig->iSerializer_MaxStringLength           = -1;
    pConfig->iSerializer_MaxByteStringLength       = -1;
    pConfig->iSerializer_MaxArrayLength            = -1;
    pConfig->iSerializer_MaxMessageSize            = -1;
    pConfig->iSerializer_MaxRecursionDepth         = -1;
    pConfig->bSecureListener_ThreadPool_Enabled    = OpcUa_False;
    pConfig->iSecureListener_ThreadPool_MinThreads = -1;
    pConfig->iSecureListener_ThreadPool_MaxThreads = -1;
    pConfig->iSecureListener_ThreadPool_MaxJobs    = -1;
    pConfig->bSecureListener_ThreadPool_BlockOnAdd = OpcUa_True;
    pConfig->uSecureListener_ThreadPool_Timeout    = OPCUA_INFINITE;
    pConfig->iTcpListener_DefaultChunkSize         = -1;
    pConfig->iTcpConnection_DefaultChunkSize       = -1;
    pConfig->iTcpTransport_MaxMessageLength        = -1;
    pConfig->iTcpTransport_MaxChunkCount           = -1;
    pConfig->bTcpListener_ClientThreadsEnabled     = OpcUa_False;
    pConfig->bTcpStream_ExpectWriteToBlock         = OpcUa_True;
}

static OpcUa_Void OPCUA_DLLCALL ualds_stack_trace_hook(OpcUa_CharA* szMessage)
{
    ualds_log(UALDS_LOG_DEBUG, "[uastack] %.*s", strlen(szMessage)-1, szMessage);
}

int ualds_server_startup()
{
    int ret = EXIT_SUCCESS;
    OpcUa_ProxyStubConfiguration stackconfig;
    OpcUa_StatusCode status;
    int numServerNames = 0;
    char szExeFileName[PATH_MAX];
    char szValue[10];
    OpcUa_Handle pcalltab = OpcUa_Null;

#ifdef HAVE_HDS
    g_bEnableZeroconf = 1;
#endif

    /* Get fully qualified domain name */
    ualds_platform_getfqhostname(g_szHostname, sizeof(g_szHostname));
    ualds_log(UALDS_LOG_NOTICE, "Server startup complete. Host name is %s.", g_szHostname);

    /* Get executable filename for updating the semaphore file path .*/
    szExeFileName[0] = 0;
    ualds_platform_getapplicationpath(szExeFileName, sizeof(szExeFileName));

    ualds_settings_begingroup("Log");
    if (ualds_settings_readstring("StackTrace", szValue, sizeof(szValue)) == 0)
    {
        if (strcmp(szValue, "error") == 0) {
            g_StackTraceLevel = OPCUA_TRACE_OUTPUT_LEVEL_ERROR;
        }
        else if (strcmp(szValue, "warn") == 0) {
            g_StackTraceLevel = OPCUA_TRACE_OUTPUT_LEVEL_WARNING;
        }
        else if (strcmp(szValue, "info") == 0) {
            g_StackTraceLevel = OPCUA_TRACE_OUTPUT_LEVEL_INFO;
        }
        else if (strcmp(szValue, "debug") == 0) {
            g_StackTraceLevel = OPCUA_TRACE_OUTPUT_LEVEL_DEBUG;
        }
        else {
            g_StackTraceLevel = OPCUA_TRACE_OUTPUT_LEVEL_NONE;
        }
    }
    ualds_settings_endgroup();

    /* setup trace hook of uastack */
    g_OpcUa_P_TraceHook = ualds_stack_trace_hook;

    /* Initialize stack platform layer */
    status = OpcUa_P_Initialize(&pcalltab);
    if (OpcUa_IsBad(status)) return EXIT_FAILURE;

    /* Initialize OPC UA Stack */
    memset(&stackconfig, 0, sizeof(stackconfig));
    ualds_initialize_proxystubconfig(&stackconfig);
    status = OpcUa_ProxyStub_Initialize(pcalltab, &stackconfig);
    if (OpcUa_IsBad(status))
    {
        OpcUa_P_Clean(&pcalltab);
        return EXIT_FAILURE;
    }

    status = OpcUa_Mutex_Create(&g_mutex);
    if (OpcUa_IsBad(status))
    {
        OpcUa_P_Clean(&pcalltab);
        return EXIT_FAILURE;
    }

    /* read settings */
    ualds_settings_begingroup("General");
    UALDS_SETTINGS_READSTRING(ServerUri);
    ualds_settings_readint("ExpirationMaxAge", &g_ExpirationMaxAge);
    if (ualds_settings_readstring("AllowLocalRegistration", szValue, sizeof(szValue)) == 0)
    {
        if (strcmp(szValue, "yes") == 0)
        {
            ualds_log(UALDS_LOG_WARNING, "AllowLocalRegistration is enabled.");
            g_bAllowLocalRegistration = 1;
        }
    }
    ualds_settings_endgroup();

    ualds_settings_begingroup(g_szServerUri);
    UALDS_SETTINGS_READSTRING(ProductUri);
    ualds_settings_beginreadarray("ServerNames", &numServerNames);
    if (numServerNames > 0)
    {
        ualds_settings_setarrayindex(0);
        ualds_settings_readstring("Text", g_szApplicationName, sizeof(g_szApplicationName));
    }
    ualds_settings_endarray();
    if (strlen(szExeFileName) > 0)
    {
        ualds_settings_writestring("SemaphoreFilePath", szExeFileName);
    }
    ualds_settings_endgroup();

    /* Initialize OPC UA Security */
    status = ualds_security_initialize();
    if (OpcUa_IsBad(status))
    {
        OpcUa_ProxyStub_Clear();
        OpcUa_P_Clean(&pcalltab);
        return EXIT_FAILURE;
    }

#ifdef HAVE_HDS
    ualds_settings_begingroup("Zeroconf");
    if (ualds_settings_readstring("EnableZeroconf", szValue, sizeof(szValue)) == 0)
    {
        if (strcmp(szValue, "yes") != 0)
        {
            g_bEnableZeroconf = 0;
        }
    }
    ualds_settings_endgroup();

    ualds_log(UALDS_LOG_INFO, "Zeroconf is %s.", g_bEnableZeroconf == 0 ? "disabled" : "enabled");

    loadKnownTLD();

    if (g_bEnableZeroconf)
    {

#ifdef _WIN32
        /*Precondition: Bonjour Service running.*/
        BOOL successBonjourServiceStart = StartBonjourService();
        if (successBonjourServiceStart == FALSE)
        {
            ualds_log(UALDS_LOG_ERR, "Could not start Bonjour Service");
        }
#endif /* _WIN32 */

        status = ualds_zeroconf_start_registration();
        if (OpcUa_IsBad(status))
        {
            ualds_delete_endpoints();
            ualds_security_uninitialize();
            ualds_settings_cleanup(0);
            OpcUa_Mutex_Delete(&g_mutex);
            OpcUa_ProxyStub_Clear();
            OpcUa_P_Clean(&pcalltab);
            return EXIT_FAILURE;
        }

        /* start browsing for DNSServices in the background */
        status = ualds_findserversonnetwork_start_listening();
        if (OpcUa_IsBad(status))
        {
            ualds_zeroconf_stop_registration();
            ualds_delete_endpoints();
            ualds_security_uninitialize();
            ualds_settings_cleanup(0);
            OpcUa_Mutex_Delete(&g_mutex);
            OpcUa_ProxyStub_Clear();
            OpcUa_P_Clean(&pcalltab);
            return EXIT_FAILURE;
        }
    }
    else
    {
        ualds_zeroconf_loadOffline();
    }
#endif

    /* Open Endpoints */
    status = ualds_create_endpoints();
    if (OpcUa_IsBad(status))
    {
        ualds_delete_endpoints();

#ifdef HAVE_HDS
        if (g_bEnableZeroconf)
        {
            ualds_zeroconf_socketEventCallback(&g_shutdown);

            ualds_zeroconf_stop_registration();

            /* stop browsing for DNSServices in the background */
            ualds_findserversonnetwork_stop_listening();
        }
        else
        {
            ualds_zeroconf_cleanupOffline();
        }
#endif

        ualds_security_uninitialize();
        ualds_settings_cleanup(0);
        OpcUa_Mutex_Delete(&g_mutex);
        OpcUa_ProxyStub_Clear();
        OpcUa_P_Clean(&pcalltab);
        return EXIT_FAILURE;
    }

    ualds_settings_flush();

    while (!g_shutdown)
    {
#ifdef HAVE_HDS
        if (g_bEnableZeroconf)
        {
            ualds_zeroconf_socketEventCallback(&g_shutdown);
            ualds_findserversonnetwork_socketEventCallback(&g_shutdown);
        }
#endif
        ualds_platform_sleep(1);
    }

    ualds_delete_endpoints();

#ifdef HAVE_HDS
    if (g_bEnableZeroconf)
    {
        ualds_zeroconf_stop_registration();

        /* stop browsing for DNSServices in the background */
        ualds_findserversonnetwork_stop_listening();
    }
    else
    {
        ualds_zeroconf_cleanupOffline();
    }

#endif

    ualds_security_uninitialize();
    ualds_settings_cleanup(1);
    OpcUa_Mutex_Delete(&g_mutex);
    OpcUa_ProxyStub_Clear();
    OpcUa_P_Clean(&pcalltab);

    return ret;
}
/** Main loop of UA LDS service.
 * The main.c file calls this function to start the UA LDS server.
 * This function does not return until shutdown.
 * @see ualds_shutdown
 */
int ualds_server()
{
    int ret = EXIT_SUCCESS;

    ret = ualds_server_startup();
    if (ret != EXIT_SUCCESS)
    {
        // The LDS could not start with the current config file (ualds.ini/ualds.conf).
        // Althow the config file can be syntactically correct, different manual changes to it can make the LDS not startup.
        // Try to use the backup config file (ualds.ini.bak/ualds.conf.bak)

        char szConfigFile[PATH_MAX] = { 0 };
        ualds_platform_getconfigfile_path(szConfigFile, sizeof(szConfigFile));
        ualds_settings_open_from_backup(szConfigFile);

        ret = ualds_server_startup();
        if (ret != EXIT_SUCCESS)
        {
            // The LDS could not start with the backup config file(ualds.ini.bak/ualds.conf.bak).
            // Try default configuration values

            char szConfigFile[PATH_MAX] = { 0 };
            ualds_platform_getconfigfile_path(szConfigFile, sizeof(szConfigFile));
            ualds_settings_open_from_default(szConfigFile);

            ret = ualds_server_startup();
        }
    }

    return ret;
}

/** This functions sets the shutdown flag which forces the ualds_serve function to stop
 * the server, clean up all OPC UA resources and return.
 */
void ualds_shutdown()
{
    g_shutdown = 1;
}

/** Reloads the configuration.
 * Note that not all parameters take effect without restarting.
 */
void ualds_reload()
{
}

/** Iterates over all registered servers and removes all expired entries. */
void ualds_expirationcheck()
{
    int numServers = 0;
    int numRemoved = 0;
    int i, pos;
    char **szServerUriArray = 0;
    int  *RemovedServers = 0;
    time_t now, updatetime;
    char szSemaphoreFile[PATH_MAX];

    /* read list of all registered servers */
    ualds_settings_begingroup("RegisteredServers");
    ualds_settings_beginreadarray("Servers", &numServers);
    if (numServers < 1)
    {
        ualds_settings_endarray();
        ualds_settings_endgroup();
        return;
    }
    szServerUriArray = malloc(sizeof(char*) * numServers);
    if (szServerUriArray == 0)
    {
        ualds_log(UALDS_LOG_ERR, "malloc failed. Out of Memory.");
        ualds_settings_endarray();
        ualds_settings_endgroup();
        return;
    }
    RemovedServers = malloc(sizeof(int) * numServers);
    if (RemovedServers == 0)
    {
        free(szServerUriArray);
        ualds_log(UALDS_LOG_ERR, "malloc failed. Out of Memory.");
        ualds_settings_endarray();
        ualds_settings_endgroup();
        return;
    }
    for (i=0; i<numServers; i++)
    {
        szServerUriArray[i] = malloc(UALDS_CONF_MAX_URI_LENGTH);
        if (szServerUriArray[i])
        {
            ualds_settings_setarrayindex(i);
            ualds_settings_readstring("ServerUri", szServerUriArray[i], UALDS_CONF_MAX_URI_LENGTH);
        }
    }
    ualds_settings_endarray();
    ualds_settings_endgroup();

    /* check each server for expiration */
    for (i=0; i<numServers; i++)
    {
        RemovedServers[i] = 0;
        if (szServerUriArray[i])
        {
            szSemaphoreFile[0] = 0;
            updatetime = 0;
            ualds_settings_begingroup(szServerUriArray[i]);
            if (ualds_settings_readstring("SemaphoreFilePath", szSemaphoreFile, PATH_MAX) == ENOENT)
            {
                ualds_settings_endgroup();
                ualds_settings_removegroup(szServerUriArray[i]);
#ifdef HAVE_HDS
                ualds_zeroconf_removeRegistration(szServerUriArray[i]);
#endif
                RemovedServers[i] = 1;
                numRemoved++;
                continue;
            }
            ualds_settings_readtime_t("UpdateTime", &updatetime);
            ualds_settings_endgroup();

            if (szSemaphoreFile[0] != 0)
            {
                if (szSemaphoreFile[0] != '*' &&
                    !ualds_platform_fileexists(szSemaphoreFile))
                {
                    /* file does not exist, remove entry */
                    ualds_settings_removegroup(szServerUriArray[i]);
#ifdef HAVE_HDS
                    ualds_zeroconf_removeRegistration(szServerUriArray[i]);
#endif
                    RemovedServers[i] = 1;
                    numRemoved++;
                }
            }
            else
            {
                now = time(0);
                if ((now - updatetime) > g_ExpirationMaxAge)
                {
                    /* entry is expired, remove it */
                    ualds_settings_removegroup(szServerUriArray[i]);
#ifdef HAVE_HDS
                    ualds_zeroconf_removeRegistration(szServerUriArray[i]);
#endif
                    RemovedServers[i] = 1;
                    numRemoved++;
                }
            }
        }
    }

    /* write new list of all registered servers */
    ualds_settings_begingroup("RegisteredServers");
    ualds_settings_removearray("Servers");
    ualds_settings_beginwritearray("Servers", numServers-numRemoved);
    for (i=0, pos=0; i<numServers; i++)
    {
        if (RemovedServers[i] == 0)
        {
            ualds_settings_setarrayindex(pos);
            ualds_settings_writestring("ServerUri", szServerUriArray[i]);
            pos++;
        }
    }
    ualds_settings_endarray();
    ualds_settings_endgroup();

    /* cleanup */
    for (i=0; i<numServers; i++)
    {
        if (szServerUriArray[i])
        {
            free(szServerUriArray[i]);
            szServerUriArray[i] = 0;
        }
    }
    free(szServerUriArray);
    szServerUriArray = 0;
    
    free(RemovedServers);
    RemovedServers = 0;
}

int ualds_settings_cleanup(int flush)
{
    int status = 0;

    OpcUa_Mutex_Lock(g_mutex);
    status = ualds_settings_close(flush);
    OpcUa_Mutex_Unlock(g_mutex);

    return status;
}