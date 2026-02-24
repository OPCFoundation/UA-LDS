/* ========================================================================
* Copyright (c) 2005-2026 The OPC Foundation, Inc. All rights reserved.
*
* OPC Foundation MIT License 1.00
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* The complete license agreement can be found here:
* http://opcfoundation.org/License/MIT/1.00/
* ======================================================================*/

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
        ualds_log(UALDS_LOG_DEBUG, "Failed to open windows certificate store! (0x%08X)", uStatus);
        OpcUa_GotoError;
    }

    /* validate certificate, we only need the validation result, not the windows specific pValidationCode */
    uStatus = win32PkiProvider.ValidateCertificate(&win32PkiProvider,
                                                   pbsClientCertificate,
                                                   hCertificateStore,
                                                   OpcUa_Null);

    win32PkiProvider.CloseCertificateStore(&win32PkiProvider, &hCertificateStore);

    ualds_log(UALDS_LOG_DEBUG, "Verifying certificate in windows store returned 0x%08x.", uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    ualds_log(UALDS_LOG_DEBUG, "Could not verify certificate in windows certificate store (0x%08x).", uStatus);

    if (hCertificateStore != OpcUa_Null)
    {
        win32PkiProvider.CloseCertificateStore(&win32PkiProvider, &hCertificateStore);
    }

OpcUa_FinishErrorHandling;
}


OpcUa_StatusCode ualds_verify_cert_old_default_location(OpcUa_ByteString* pbsClientCertificate, 
    char* newCertificateStorePathCrl, char* newCertificateStorePathRejected, OpcUa_Int* pValidationCode)
{
    OpcUa_P_OpenSSL_CertificateStore_Config oldPkiConfig;
    OpcUa_PKIProvider                       oldPkiProvider;
    OpcUa_Handle                            hCertificateStore = OpcUa_Null;
    char                                    oldTrustListPath[MAX_PATH];
    char *                                  root;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "ualds_verify_cert_old_default_location");

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
        ualds_log(UALDS_LOG_DEBUG, "Failed to open old default certificate store! (0x%08X)", uStatus);
        OpcUa_GotoError;
    }

    /* validate certificate, we only need the validation result, not the windows specific pValidationCode */
    uStatus = oldPkiProvider.ValidateCertificate(&oldPkiProvider,
        pbsClientCertificate,
        hCertificateStore,
        pValidationCode);

    OpcUa_GotoErrorIfBad(uStatus);

    oldPkiProvider.CloseCertificateStore(&oldPkiProvider, &hCertificateStore);

    ualds_log(UALDS_LOG_INFO, "Verifying certificate in old default certificate store returned 0x%08x.", uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&oldPkiProvider);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    ualds_log(UALDS_LOG_DEBUG, "Could not verify certificate in old default certificate store (0x%08x).", uStatus);

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
        ualds_log(UALDS_LOG_DEBUG, "Failed to open old edited certificate store! (0x%08X)", uStatus);
        OpcUa_GotoError;
    }

    /* validate certificate, we only need the validation result, not the windows specific pValidationCode */
    uStatus = oldPkiProvider.ValidateCertificate(&oldPkiProvider,
        pbsClientCertificate,
        hCertificateStore,
        pValidationCode);
    OpcUa_GotoErrorIfBad(uStatus);

    oldPkiProvider.CloseCertificateStore(&oldPkiProvider, &hCertificateStore);

    ualds_log(UALDS_LOG_DEBUG, "Verifying certificate in old edited  certificate store returned 0x%08x.", uStatus);

    uStatus = OpcUa_PKIProvider_Delete(&oldPkiProvider);

 OpcUa_ReturnStatusCode;
 OpcUa_BeginErrorHandling;

    ualds_log(UALDS_LOG_DEBUG, "Could not verify certificate in old edited certificate store (0x%08x).", uStatus);

    if (hCertificateStore != OpcUa_Null)
    {
        oldPkiProvider.CloseCertificateStore(&oldPkiProvider, &hCertificateStore);
    }
    OpcUa_PKIProvider_Delete(&oldPkiProvider);

 OpcUa_FinishErrorHandling;
}