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

/* core */
#include <opcua.h>

/* own */
#include <opcua_pki.h>

/*============================================================================
 * OpcUa_PKIProvider_ValidateCertificate
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PKIProvider_ValidateCertificate(
    struct _OpcUa_PKIProvider*  a_pPKI,
    OpcUa_ByteString*           a_pCertificate,
    OpcUa_Void*                 a_pCertificateStore,
    OpcUa_Int*                  a_pValidationCode) /* Validation return codes from OpenSSL */
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_PkiProvider);
    OpcUa_ReturnErrorIfArgumentNull(a_pPKI);
    OpcUa_ReturnErrorIfNull(a_pPKI->ValidateCertificate, OpcUa_BadNotSupported);

    return a_pPKI->ValidateCertificate(a_pPKI, a_pCertificate, a_pCertificateStore, a_pValidationCode);
}

/*============================================================================
 * OpcUa_PKIProvider_OpenCertificateStore
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PKIProvider_OpenCertificateStore(
    struct _OpcUa_PKIProvider*  a_pPKI,
    OpcUa_Void**                a_ppCertificateStore)        /* type depends on store implementation */
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_PkiProvider);
    OpcUa_ReturnErrorIfArgumentNull(a_pPKI);
    OpcUa_ReturnErrorIfNull(a_pPKI->OpenCertificateStore, OpcUa_BadNotSupported);

    return a_pPKI->OpenCertificateStore(a_pPKI, a_ppCertificateStore);
}

/*============================================================================
 * OpcUa_PKIProvider_LoadCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_PKIProvider_LoadCertificate(
    struct _OpcUa_PKIProvider*  a_pPKI,
    OpcUa_Void*                 a_pLoadHandle,
    OpcUa_Void*                 a_pCertificateStore,
    OpcUa_ByteString*           a_pCertificate)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_PkiProvider);
    OpcUa_ReturnErrorIfArgumentNull(a_pPKI);
    OpcUa_ReturnErrorIfNull(a_pPKI->LoadCertificate, OpcUa_BadNotSupported);

    return a_pPKI->LoadCertificate(a_pPKI, a_pLoadHandle, a_pCertificateStore, a_pCertificate);
}

/*============================================================================
 * OpcUa_PKIProvider_SaveCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_PKIProvider_SaveCertificate(
    struct _OpcUa_PKIProvider*  a_pPKI,
    OpcUa_ByteString*           a_pCertificate,
    OpcUa_Void*                 a_pCertificateStore,
    OpcUa_Void*                 a_pSaveHandle)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_PkiProvider);
    OpcUa_ReturnErrorIfArgumentNull(a_pPKI);
    OpcUa_ReturnErrorIfNull(a_pPKI->SaveCertificate, OpcUa_BadNotSupported);

    return a_pPKI->SaveCertificate(a_pPKI, a_pCertificate, a_pCertificateStore, a_pSaveHandle);
}
/*============================================================================
 * OpcUa_PKIProvider_CloseCertificateStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_PKIProvider_CloseCertificateStore(
    struct _OpcUa_PKIProvider*   a_pPKI,
    OpcUa_Void**                 a_ppCertificateStore) /* type depends on store implementation */
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_PkiProvider);
    OpcUa_ReturnErrorIfArgumentNull(a_pPKI);
    OpcUa_ReturnErrorIfNull(a_pPKI->CloseCertificateStore, OpcUa_BadNotSupported);

    return a_pPKI->CloseCertificateStore(a_pPKI, a_ppCertificateStore);
}
