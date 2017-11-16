/* Copyright (c) 1996-2017, OPC Foundation. All rights reserved.

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

#include "certstore.h"

/* uastack includes */
#include <opcua_pkifactory.h>
#include <opcua_core.h>
/* local platform includes */
#include <platform.h>
#include <../config.h>
#include <log.h>

/** Checks the configured windows store if the given certificate is trusted.
* Returns OpcUa_Good if the certificate is trusted, OpcUa_BadCertificateUntrusted otherwise.
*/
OpcUa_StatusCode ualds_verify_cert_win32(OpcUa_ByteString* pbsClientCertificate)
{
    OpcUa_P_OpenSSL_CertificateStore_Config win32PkiConfig;
    OpcUa_PKIProvider                   win32PkiProvider;
    OpcUa_Handle                        hCertificateStore = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Server, "ualds_verify_cert_win32");

    OpcUa_ReturnErrorIfArgumentNull(pbsClientCertificate);

    uStatus = OpcUa_BadCertificateUntrusted;

    memset(&win32PkiConfig, 0, sizeof(win32PkiConfig));

    /* store configuration, only strPkiType, uFlags and strTrustedCertificateListLocation need to be set */
    win32PkiConfig.PkiType                         = OpcUa_Win32_PKI;
    win32PkiConfig.Flags                           = UALDS_CONF_WIN32_STORE_LOCATION;
    win32PkiConfig.CertificateTrustListLocation    = UALDS_CONF_WIN32_STORE_NAME;

    uStatus = OpcUa_PKIProvider_Create(&win32PkiConfig, &win32PkiProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = win32PkiProvider.OpenCertificateStore(&win32PkiProvider, &hCertificateStore);
    if (OpcUa_IsBad(uStatus))
    {
        ualds_log(UALDS_LOG_ERR, "Failed to open windows certificate store! (0x%08X)", uStatus);
        OpcUa_GotoError;
    }

    /* validate certificate, we only need the validation result, not the windows specific pValidationCode */
    uStatus = win32PkiProvider.ValidateCertificate(&win32PkiProvider,
                                                   pbsClientCertificate,
                                                   hCertificateStore,
                                                   OpcUa_Null);

    win32PkiProvider.CloseCertificateStore(&win32PkiProvider, &hCertificateStore);

    ualds_log(UALDS_LOG_INFO, "Verifying certificate in windows store returned 0x%08x.", uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    ualds_log(UALDS_LOG_ERR, "Could not verify certificate in windows certificate store (0x%08x).", uStatus);

    if (hCertificateStore != OpcUa_Null)
    {
        win32PkiProvider.CloseCertificateStore(&win32PkiProvider, &hCertificateStore);
    }

OpcUa_FinishErrorHandling;
}


OpcUa_StatusCode ualds_verify_cert_old_defualt_location(OpcUa_ByteString* pbsClientCertificate, 
    char* newCertificateStorePathCrl, char* newCertificateStorePathRejected, OpcUa_Int* pValidationCode)
{
    OpcUa_P_OpenSSL_CertificateStore_Config oldPkiConfig;
    OpcUa_PKIProvider                       oldPkiProvider;
    OpcUa_Handle                            hCertificateStore = OpcUa_Null;
    char                                    oldTrustListPath[MAX_PATH];
    char *                                  root;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "ualds_verify_cert_old_defualt_location");

    OpcUa_ReturnErrorIfArgumentNull(pbsClientCertificate);

    uStatus = OpcUa_BadCertificateUntrusted;

    memset(&oldPkiConfig, 0, sizeof(oldPkiConfig));

    oldPkiConfig.PkiType = OpcUa_OpenSSL_PKI;

    root = getenv("ALLUSERSPROFILE");
    char rootFixedPath[MAX_PATH] = "C:\\ProgramData";
    if (!root)
    {
        /* Fall back to fixed path */
        root = rootFixedPath;
    }
    memset(oldTrustListPath, 0, PATH_MAX);
    strlcpy(oldTrustListPath, root, MAX_PATH);
    strlcat(oldTrustListPath, "\\OPC Foundation\\UA\\Discovery\\pki\\trusted\\certs", MAX_PATH);

    oldPkiConfig.CertificateTrustListLocation = oldTrustListPath;
    oldPkiConfig.CertificateRevocationListLocation = newCertificateStorePathCrl;
    oldPkiConfig.CertificateUntrustedListLocation = newCertificateStorePathRejected;

    oldPkiConfig.Flags = OPCUA_P_PKI_OPENSSL_USE_DEFAULT_CERT_CRL_LOOKUP_METHOD;
    oldPkiConfig.Override = OpcUa_Null;

    uStatus = OpcUa_PKIProvider_Create(&oldPkiConfig, &oldPkiProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = oldPkiProvider.OpenCertificateStore(&oldPkiProvider, &hCertificateStore);
    if (OpcUa_IsBad(uStatus))
    {
        ualds_log(UALDS_LOG_ERR, "Failed to open old defualt certificate store! (0x%08X)", uStatus);
        OpcUa_GotoError;
    }

    /* validate certificate, we only need the validation result, not the windows specific pValidationCode */
    uStatus = oldPkiProvider.ValidateCertificate(&oldPkiProvider,
        pbsClientCertificate,
        hCertificateStore,
        pValidationCode);

    OpcUa_GotoErrorIfBad(uStatus);

    oldPkiProvider.CloseCertificateStore(&oldPkiProvider, &hCertificateStore);

    ualds_log(UALDS_LOG_INFO, "Verifying certificate in old defualt certificate store returned 0x%08x.", uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&oldPkiProvider);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    ualds_log(UALDS_LOG_ERR, "Could not verify certificate in old defualt certificate store (0x%08x).", uStatus);

    if (hCertificateStore != OpcUa_Null)
    {
        oldPkiProvider.CloseCertificateStore(&oldPkiProvider, &hCertificateStore);
    }
    OpcUa_PKIProvider_Delete(&oldPkiProvider);

OpcUa_FinishErrorHandling;
}

OpcUa_StatusCode ualds_verify_cert_old_edited_location(OpcUa_ByteString* pbsClientCertificate, char* newCertificateStorePathCrl, 
    char* newCertificateStorePathRejected, char* trustListPathOldEditedLocation, OpcUa_Int* pValidationCode)
{
    OpcUa_P_OpenSSL_CertificateStore_Config oldPkiConfig;
    OpcUa_PKIProvider                       oldPkiProvider;
    OpcUa_Handle                            hCertificateStore = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "ualds_verify_cert_old_edited_location");

    OpcUa_ReturnErrorIfArgumentNull(pbsClientCertificate);

    uStatus = OpcUa_BadCertificateUntrusted;

    memset(&oldPkiConfig, 0, sizeof(oldPkiConfig));

    oldPkiConfig.PkiType = OpcUa_OpenSSL_PKI;

    if (strlen(trustListPathOldEditedLocation) == 0)
    {
        char *root = getenv("ALLUSERSPROFILE");
        char rootFixedPath[MAX_PATH] = "C:\\ProgramData";
        if (!root)
        {
            /* Fall back to fixed path */
            root = rootFixedPath;
        }
        memset(trustListPathOldEditedLocation, 0, PATH_MAX);
        strlcpy(trustListPathOldEditedLocation, root, PATH_MAX);
        strlcat(trustListPathOldEditedLocation, "\\OPC Foundation\\UA\\Discovery\\pki\\trusted\\certs", PATH_MAX);
    }

    oldPkiConfig.CertificateTrustListLocation = trustListPathOldEditedLocation;
    oldPkiConfig.CertificateRevocationListLocation = newCertificateStorePathCrl;
    oldPkiConfig.CertificateUntrustedListLocation = newCertificateStorePathRejected;

    oldPkiConfig.Flags = OPCUA_P_PKI_OPENSSL_USE_DEFAULT_CERT_CRL_LOOKUP_METHOD;
    oldPkiConfig.Override = OpcUa_Null;

    uStatus = OpcUa_PKIProvider_Create(&oldPkiConfig, &oldPkiProvider);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = oldPkiProvider.OpenCertificateStore(&oldPkiProvider, &hCertificateStore);
    if (OpcUa_IsBad(uStatus))
    {
        ualds_log(UALDS_LOG_ERR, "Failed to open old edited certificate store! (0x%08X)", uStatus);
        OpcUa_GotoError;
    }

    /* validate certificate, we only need the validation result, not the windows specific pValidationCode */
    uStatus = oldPkiProvider.ValidateCertificate(&oldPkiProvider,
        pbsClientCertificate,
        hCertificateStore,
        pValidationCode);
    OpcUa_GotoErrorIfBad(uStatus);

    oldPkiProvider.CloseCertificateStore(&oldPkiProvider, &hCertificateStore);

    ualds_log(UALDS_LOG_INFO, "Verifying certificate in old edited  certificate store returned 0x%08x.", uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&oldPkiProvider);

 OpcUa_ReturnStatusCode;
 OpcUa_BeginErrorHandling;

    ualds_log(UALDS_LOG_ERR, "Could not verify certificate in old edited certificate store (0x%08x).", uStatus);

    if (hCertificateStore != OpcUa_Null)
    {
        oldPkiProvider.CloseCertificateStore(&oldPkiProvider, &hCertificateStore);
    }
    OpcUa_PKIProvider_Delete(&oldPkiProvider);

 OpcUa_FinishErrorHandling;
}