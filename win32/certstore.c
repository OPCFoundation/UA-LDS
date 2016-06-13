/* Copyright (c) 1996-2016, OPC Foundation. All rights reserved.

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
