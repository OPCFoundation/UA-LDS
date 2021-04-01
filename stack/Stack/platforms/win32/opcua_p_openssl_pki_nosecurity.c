/* ========================================================================
 * Copyright (c) 2005-2018 The OPC Foundation, Inc. All rights reserved.
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

/* own headers */
#include <opcua_p_openssl_pki.h>

/*============================================================================
 * OpcUa_P_OpenSSL_CertificateStore_Open
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_OpenCertificateStore(
    OpcUa_PKIProvider*          a_pProvider,
    OpcUa_Void**                a_ppCertificateStore)
{
    OpcUa_ReferenceParameter(a_pProvider);
    OpcUa_ReferenceParameter(a_ppCertificateStore);

    return OpcUa_BadNotSupported;
}

/*============================================================================
 * OpcUa_P_OpenSSL_PKI_NoSecurity_CloseCertificateStore
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_CloseCertificateStore(
    OpcUa_PKIProvider*          a_pProvider,
    OpcUa_Void**                a_ppCertificateStore)
{
    OpcUa_ReferenceParameter(a_pProvider);
    OpcUa_ReferenceParameter(a_ppCertificateStore);

    return OpcUa_BadNotSupported;
}


/*============================================================================
 * OpcUa_P_OpenSSL_PKI_ValidateCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_ValidateCertificate(
    OpcUa_PKIProvider*          a_pProvider,
    OpcUa_ByteString*           a_pCertificate,
    OpcUa_Void*                 a_pCertificateStore,
    OpcUa_Int*                  a_pValidationCode)
{
    OpcUa_ReferenceParameter(a_pProvider);
    OpcUa_ReferenceParameter(a_pCertificateStore);
    OpcUa_ReferenceParameter(a_pCertificate);
    OpcUa_ReferenceParameter(a_pValidationCode);

    return OpcUa_BadNotSupported;
}

/*============================================================================
 * OpcUa_P_OpenSSL_PKI_NoSecurity_SaveCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_SaveCertificate(
    OpcUa_PKIProvider*          a_pProvider,
    OpcUa_ByteString*           a_pCertificate,
    OpcUa_Void*                 a_pCertificateStore,
    OpcUa_Void*                 a_pSaveHandle)
{
    OpcUa_ReferenceParameter(a_pProvider);
    OpcUa_ReferenceParameter(a_pCertificateStore);
    OpcUa_ReferenceParameter(a_pCertificate);
    OpcUa_ReferenceParameter(a_pSaveHandle);

    return OpcUa_BadNotSupported;
}

/*============================================================================
 * OpcUa_P_OpenSSL_PKI_NoSecurity_LoadCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_LoadCertificate(
    OpcUa_PKIProvider*          a_pProvider,
    OpcUa_Void*                 a_pLoadHandle,
    OpcUa_Void*                 a_pCertificateStore,
    OpcUa_ByteString*           a_pCertificate)
{
    OpcUa_ReferenceParameter(a_pProvider);
    OpcUa_ReferenceParameter(a_pCertificateStore);
    OpcUa_ReferenceParameter(a_pCertificate);
    OpcUa_ReferenceParameter(a_pLoadHandle);

    return OpcUa_BadNotSupported;
}

/*============================================================================
 * OpcUa_P_OpenSSL_PKI_NoSecurity_LoadCertificate
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_LoadPrivateKeyFromFile(
    OpcUa_StringA               a_privateKeyFile,
    OpcUa_P_FileFormat          a_fileFormat,
    OpcUa_StringA               a_password,
    OpcUa_UInt                  a_keyType,
    OpcUa_Key*                  a_pPrivateKey)
{
    OpcUa_ReferenceParameter(a_privateKeyFile);
    OpcUa_ReferenceParameter(a_fileFormat);
    OpcUa_ReferenceParameter(a_password);
    OpcUa_ReferenceParameter(a_keyType);
    OpcUa_ReferenceParameter(a_pPrivateKey);

    return OpcUa_BadNotSupported;
}

/*============================================================================
 * OpcUa_P_OpenSSL_PKI_NoSecurity_ExtractCertificateData
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_ExtractCertificateData(
    OpcUa_ByteString*           a_pCertificate,
    OpcUa_ByteString*           a_pIssuer,
    OpcUa_ByteString*           a_pSubject,
    OpcUa_ByteString*           a_pSubjectUri,
    OpcUa_ByteString*           a_pSubjectIP,
    OpcUa_ByteString*           a_pSubjectDNS,
    OpcUa_ByteString*           a_pCertThumbprint,
    OpcUa_UInt32*               a_pSubjectHash,
    OpcUa_UInt32*               a_pCertRawLength)
{
    OpcUa_ReferenceParameter(a_pCertificate);
    OpcUa_ReferenceParameter(a_pIssuer);
    OpcUa_ReferenceParameter(a_pSubject);
    OpcUa_ReferenceParameter(a_pSubjectUri);
    OpcUa_ReferenceParameter(a_pSubjectIP);
    OpcUa_ReferenceParameter(a_pSubjectDNS);
    OpcUa_ReferenceParameter(a_pCertThumbprint);
    OpcUa_ReferenceParameter(a_pSubjectHash);
    OpcUa_ReferenceParameter(a_pCertRawLength);

    return OpcUa_BadNotSupported;
}
