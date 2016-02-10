/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
**
** Project: OPC UA Local Discovery Server
**
** Author: Hannes Mezger <hannes.mezger@ascolab.com>
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
******************************************************************************/
#include "certstore.h"

/* uastack includes */
#include <opcua_pkifactory.h>
/* local platform includes */
#include <../config.h>
#include <log.h>

/** Checks the configured windows store if the given certificate is trusted.
* Returns OpcUa_Good if the certificate is trusted, OpcUa_BadCertificateUntrusted otherwise.
*/
OpcUa_StatusCode ualds_verify_cert_win32(OpcUa_ByteString* pbsClientCertificate)
{
    OpcUa_CertificateStoreConfiguration win32PkiConfig;
    OpcUa_PKIProvider                   win32PkiProvider;
    OpcUa_Handle                        hCertificateStore = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Server, "ualds_verify_cert_win32");

    OpcUa_ReturnErrorIfArgumentNull(pbsClientCertificate);

    uStatus = OpcUa_BadCertificateUntrusted;

    OpcUa_CertificateStoreConfiguration_Initialize(&win32PkiConfig);

    /* store configuration, only strPkiType, uFlags and strTrustedCertificateListLocation need to be set */
    win32PkiConfig.strPkiType                           = OPCUA_P_PKI_TYPE_WIN32;
    win32PkiConfig.uFlags                               = UALDS_CONF_WIN32_STORE_LOCATION;
    win32PkiConfig.strTrustedCertificateListLocation    = UALDS_CONF_WIN32_STORE_NAME;

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
