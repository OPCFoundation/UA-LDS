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

/* UA platform definitions */
#include <opcua_p_internal.h>
#include <opcua_p_memory.h>
#include <opcua_p_string.h>

/* own */
#include <opcua_p_openssl_pki.h>
#include <opcua_p_pkifactory.h>

/*============================================================================
 * OpcUa_P_PKIFactory_CreatePKIProvider
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_PKIFactory_CreatePKIProvider(OpcUa_Void*         a_pCertificateStoreConfig,
                                                                    OpcUa_PKIProvider*  a_pPkiProvider)
{
    OpcUa_P_OpenSSL_CertificateStore_Config*    pCertificateStoreCfg    = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_P_PKIFactory, "CreatePKIProvider");

    OpcUa_ReturnErrorIfArgumentNull(a_pCertificateStoreConfig);
    OpcUa_ReturnErrorIfArgumentNull(a_pPkiProvider);

    pCertificateStoreCfg = (OpcUa_P_OpenSSL_CertificateStore_Config*)a_pCertificateStoreConfig;
    a_pPkiProvider->Handle = a_pCertificateStoreConfig;

    switch(pCertificateStoreCfg->PkiType)
    {
    case OpcUa_NO_PKI:
        {
            a_pPkiProvider->OpenCertificateStore    = OpcUa_P_OpenSSL_PKI_NoSecurity_OpenCertificateStore;
            a_pPkiProvider->CloseCertificateStore   = OpcUa_P_OpenSSL_PKI_NoSecurity_CloseCertificateStore;
            a_pPkiProvider->ValidateCertificate     = OpcUa_P_OpenSSL_PKI_NoSecurity_ValidateCertificate;
            a_pPkiProvider->LoadCertificate         = OpcUa_P_OpenSSL_PKI_NoSecurity_LoadCertificate;
            a_pPkiProvider->SaveCertificate         = OpcUa_P_OpenSSL_PKI_NoSecurity_SaveCertificate;
            a_pPkiProvider->LoadPrivateKeyFromFile  = OpcUa_P_OpenSSL_PKI_NoSecurity_LoadPrivateKeyFromFile;
            a_pPkiProvider->ExtractCertificateData  = OpcUa_P_OpenSSL_PKI_NoSecurity_ExtractCertificateData;
            break;
        }
#if OPCUA_SUPPORT_PKI
#if OPCUA_SUPPORT_PKI_OVERRIDE
    case OpcUa_Override:
        {
            /* check if replacement for default is set and use it instead of the default */
            OpcUa_PKIProvider* pOverride = (OpcUa_PKIProvider*)pCertificateStoreCfg->Override;

            OpcUa_GotoErrorIfArgumentNull(pOverride);

            if(pOverride->OpenCertificateStore == OpcUa_Null)
            {
                a_pPkiProvider->OpenCertificateStore = OpcUa_P_OpenSSL_PKI_OpenCertificateStore;
            }
            else
            {
                a_pPkiProvider->OpenCertificateStore = pOverride->OpenCertificateStore;
            }

            if(pOverride->CloseCertificateStore == OpcUa_Null)
            {
                a_pPkiProvider->CloseCertificateStore = OpcUa_P_OpenSSL_PKI_CloseCertificateStore;
            }
            else
            {
                a_pPkiProvider->CloseCertificateStore = pOverride->CloseCertificateStore;
            }

            if(pOverride->ValidateCertificate == OpcUa_Null)
            {
                a_pPkiProvider->ValidateCertificate = OpcUa_P_OpenSSL_PKI_ValidateCertificate;
            }
            else
            {
                a_pPkiProvider->ValidateCertificate = pOverride->ValidateCertificate;
            }

            if(pOverride->LoadCertificate == OpcUa_Null)
            {
                a_pPkiProvider->LoadCertificate = OpcUa_P_OpenSSL_PKI_LoadCertificate;
            }
            else
            {
                a_pPkiProvider->LoadCertificate = pOverride->LoadCertificate;
            }

            if(pOverride->SaveCertificate == OpcUa_Null)
            {
                a_pPkiProvider->SaveCertificate = OpcUa_P_OpenSSL_PKI_SaveCertificate;
            }
            else
            {
                a_pPkiProvider->SaveCertificate = pOverride->SaveCertificate;
            }

            if(pOverride->LoadPrivateKeyFromFile == OpcUa_Null)
            {
                a_pPkiProvider->LoadPrivateKeyFromFile = OpcUa_P_OpenSSL_PKI_LoadPrivateKeyFromFile;
            }
            else
            {
                a_pPkiProvider->LoadPrivateKeyFromFile = pOverride->LoadPrivateKeyFromFile;
            }

            if(pOverride->ExtractCertificateData == OpcUa_Null)
            {
                a_pPkiProvider->ExtractCertificateData = OpcUa_P_OpenSSL_PKI_ExtractCertificateData;
            }
            else
            {
                a_pPkiProvider->ExtractCertificateData = pOverride->ExtractCertificateData;
            }
            break;
        }
#endif /* OPCUA_SUPPORT_PKI_OVERRIDE */
#if OPCUA_SUPPORT_PKI_OPENSSL
    case OpcUa_OpenSSL_PKI:
        {
            a_pPkiProvider->OpenCertificateStore    = OpcUa_P_OpenSSL_PKI_OpenCertificateStore;
            a_pPkiProvider->CloseCertificateStore   = OpcUa_P_OpenSSL_PKI_CloseCertificateStore;
            a_pPkiProvider->ValidateCertificate     = OpcUa_P_OpenSSL_PKI_ValidateCertificate;
            a_pPkiProvider->LoadCertificate         = OpcUa_P_OpenSSL_PKI_LoadCertificate;
            a_pPkiProvider->SaveCertificate         = OpcUa_P_OpenSSL_PKI_SaveCertificate;
            a_pPkiProvider->LoadPrivateKeyFromFile  = OpcUa_P_OpenSSL_PKI_LoadPrivateKeyFromFile;
            a_pPkiProvider->ExtractCertificateData  = OpcUa_P_OpenSSL_PKI_ExtractCertificateData;
            break;
        }
#endif /* OPCUA_SUPPORT_PKI_OPENSSL */
#endif /* OPCUA_SUPPORT_PKI */
    default:
        {
            uStatus = OpcUa_BadNotSupported;
        }
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_P_PKIFactory_DeletePKIProvider
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_PKIFactory_DeletePKIProvider(OpcUa_PKIProvider* a_pProvider)
{
OpcUa_InitializeStatus(OpcUa_Module_P_PKIFactory, "DeletePKIProvider");

    OpcUa_ReturnErrorIfArgumentNull(a_pProvider);

    a_pProvider->Handle = OpcUa_Null;
    a_pProvider->OpenCertificateStore = OpcUa_Null;
    a_pProvider->CloseCertificateStore = OpcUa_Null;
    a_pProvider->LoadCertificate = OpcUa_Null;
    a_pProvider->LoadPrivateKeyFromFile = OpcUa_Null;
    a_pProvider->SaveCertificate = OpcUa_Null;
    a_pProvider->ValidateCertificate = OpcUa_Null;
    a_pProvider->ExtractCertificateData = OpcUa_Null;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_P_PKIFactory_GetDHParamFileName
 *===========================================================================*/
OpcUa_StringA OpcUa_P_PKIFactory_GetDHParamFileName(OpcUa_Void* a_pPKIConfig)
{
    OpcUa_P_OpenSSL_CertificateStore_Config* pCertificateStoreCfg;

    pCertificateStoreCfg = (OpcUa_P_OpenSSL_CertificateStore_Config*)a_pPKIConfig;

    if(pCertificateStoreCfg != OpcUa_Null)
    {
#if OPCUA_SUPPORT_PKI
#if OPCUA_SUPPORT_PKI_OPENSSL
        if(pCertificateStoreCfg->PkiType == OpcUa_OpenSSL_PKI &&
           pCertificateStoreCfg->Flags & OPCUA_P_PKI_OPENSSL_OVERRIDE_IS_DHPARAM_FILE)
        {
            return (OpcUa_StringA)pCertificateStoreCfg->Override;
        }
#endif /* OPCUA_SUPPORT_PKI_OPENSSL */
#if OPCUA_SUPPORT_PKI_OVERRIDE
        if(pCertificateStoreCfg->PkiType == OpcUa_Override &&
           pCertificateStoreCfg->Flags & OPCUA_P_PKI_OVERRIDE_HANDLE_IS_DHPARAM_FILE)
        {
            OpcUa_PKIProvider* pOverride = (OpcUa_PKIProvider*)pCertificateStoreCfg->Override;
            if(pOverride != OpcUa_Null)
            {
                return (OpcUa_StringA)pOverride->Handle;
            }
        }
#endif /* OPCUA_SUPPORT_PKI_OVERRIDE */
#endif /* OPCUA_SUPPORT_PKI */
    }

    return OpcUa_Null;
}
