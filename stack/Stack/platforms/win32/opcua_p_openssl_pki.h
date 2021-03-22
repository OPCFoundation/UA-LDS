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

#ifndef _OpcUa_P_OpenSSL_PKI_H_
#define _OpcUa_P_OpenSSL_PKI_H_ 1

OPCUA_BEGIN_EXTERN_C

/**
  @brief Creates a certificate store object.

  @param pProvider                  [in]  The crypto provider handle.

  @param ppCertificateStore         [out] The handle to the certificate store. Type depends on store implementation.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_OpenCertificateStore(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_Void**                ppCertificateStore);

/**
  @brief frees a certificate store object.

  @param pProvider             [in]  The crypto provider handle.
  @param ppCertificateStore    [in] The certificate store object. Type depends on store implementation.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_CloseCertificateStore(
    OpcUa_PKIProvider*       pProvider,
    OpcUa_Void**             ppCertificateStore);

/**
  @brief Validates a given X509 certificate object.

   Validation:
   - Subject/Issuer
   - Path
   - Certificate Revocation List (CRL)
   - Certificate Trust List (CTL)

  @param pProvider                [in]  The crypto provider handle.
  @param pCertificate             [in]  The certificate that should be validated.
  @param pCertificateStore        [in]  The certificate store that validates the passed in certificate.

  @param pValidationCode          [out] The validation code, that gives information about the validation result. Validation return codes from OpenSSL are used.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_ValidateCertificate(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_ByteString*           pCertificate,
    OpcUa_Void*                 pCertificateStore,
    OpcUa_Int*                  pValidationCode);

/**
  @brief imports a given certificate into given certificate store.

  @param pProvider                [in]  The crypto provider handle.
  @param pCertificateStore        [in]  The certificate store that should store the passed in certificate.
  @param pCertificate             [in]  The certificate that should be stored in the certificate store.
  @param pSaveHandle              [in]  The index that indicates the store location of the certificate within the certificate store.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_SaveCertificate(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_ByteString*           pCertificate,
    OpcUa_Void*                 pCertificateStore,
    OpcUa_Void*                 pSaveHandle);

/**
  @brief exports a certain certificate from a given certificate store.

  @param pProvider                [in]  The crypto provider handle.
  @param pLoadHandle              [in]  The index that indicates the store location of the certificate within the certificate store.
  @param ppCertificateStore       [in]  The certificate store that contains the desired certificate.

  @param pCertificate             [out] The exported certificate.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_LoadCertificate(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_Void*                 pLoadHandle,
    OpcUa_Void*                 pCertificateStore,
    OpcUa_ByteString*           pCertificate);

/**
  @brief loads a private key object, usually from an encrypted file.

  Note: fileFormat == OpcUa_Crypto_Encoding_DER needs a keyType != OpcUa_Crypto_KeyType_Any
        e.g. OpcUa_Crypto_KeyType_Rsa_Private.
        Other formats can use keyType == OpcUa_Crypto_KeyType_Any as a wildcard.

  @param privateKeyFile           [in]  The file name.
  @param fileFormat               [in]  The file format.
  @param password                 [in]  The encryption password.
  @param keyType                  [in]  The expected key type.

  @param pPrivateKey              [out] The private key (in DER format).
  */
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_LoadPrivateKeyFromFile(
    OpcUa_StringA           privateKeyFile,
    OpcUa_P_FileFormat      fileFormat,
    OpcUa_StringA           password,
    OpcUa_UInt              keyType,
    OpcUa_Key*              pPrivateKey);

/**
  @brief Extracts data from a certificate.

  @param pCertificate          [in] The certificate to examine.
  @param pIssuer               [out, optional] The issuer name of the certificate.
  @param pSubject              [out, optional] The subject name of the certificate.
  @param pSubjectUri           [out, optional] The subject's URI of the certificate.
  @param pSubjectIP            [out, optional] The subject's IP of the certificate.
  @param pSubjectDNS           [out, optional] The subject's DNS name of the certificate.
  @param pCertThumbprint       [out, optional] The thumbprint of the certificate.
  @param pSubjectHash          [out, optional] The hash code of the certificate.
  @param pCertRawLength        [out, optional] The length of the DER encoded data. Can be smaller than the total length of pCertificate in case of chain certificate or garbage follow.
  @param pNotBefore            [out, optional] The the date on which a certificate becomes valid.
  @param pNotAfter             [out, optional] The date in after which the certificate is no longer valid.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_ExtractCertificateData(
    OpcUa_ByteString*           pCertificate,
    OpcUa_ByteString*           pIssuer,
    OpcUa_ByteString*           pSubject,
    OpcUa_ByteString*           pSubjectUri,
    OpcUa_ByteString*           pSubjectIP,
    OpcUa_ByteString*           pSubjectDNS,
    OpcUa_ByteString*           pCertThumbprint,
    OpcUa_UInt32*               pSubjectHash,
    OpcUa_UInt32*               pCertRawLength,
    OpcUa_Int64*                a_pNotBefore,
    OpcUa_Int64*                a_pNotAfter);

/* NoSecurity functions */

/**
  @brief Creates a certificate store object.

  @param pProvider                  [in]  The crypto provider handle.

  @param ppCertificateStore         [out] The handle to the certificate store. Type depends on store implementation.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_OpenCertificateStore(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_Void**                ppCertificateStore);

/**
  @brief frees a certificate store object.

  @param pProvider             [in]  The crypto provider handle.
  @param ppCertificateStore    [in] The certificate store object. Type depends on store implementation.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_CloseCertificateStore(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_Void**                ppCertificateStore);

/**
  @brief Validates a given X509 certificate object.

   Validation:
   - Subject/Issuer
   - Path
   - Certificate Revocation List (CRL)
   - Certificate Trust List (CTL)

  @param pProvider                [in]  The crypto provider handle.
  @param pCertificate             [in]  The certificate that should be validated.
  @param pCertificateStore        [in]  The certificate store that validates the passed in certificate.

  @param pValidationCode          [out] The validation code, that gives information about the validation result. Validation return codes from OpenSSL are used.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_ValidateCertificate(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_ByteString*           pCertificate,
    OpcUa_Void*                 pCertificateStore,
    OpcUa_Int*                  pValidationCode /* Validation return codes from OpenSSL */);

/**
  @brief imports a given certificate into given certificate store.

  @param pProvider                [in]  The crypto provider handle.
  @param pCertificateStore        [in]  The certificate store that should store the passed in certificate.
  @param pCertificate             [in]  The certificate that should be stored in the certificate store.
  @param pSaveHandle              [in]  The index that indicates the store location of the certificate within the certificate store.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_SaveCertificate(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_ByteString*           pCertificate,
    OpcUa_Void*                 pCertificateStore,
    OpcUa_Void*                 pSaveHandle);

/**
  @brief exports a certain certificate from a given certificate store.

  @param pProvider                [in]  The crypto provider handle.
  @param pLoadHandle              [in]  The index that indicates the store location of the certificate within the certificate store.
  @param ppCertificateStore       [in]  The certificate store that contains the desired certificate.

  @param pCertificate             [out] The exported certificate.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_LoadCertificate(
    OpcUa_PKIProvider*          pProvider,
    OpcUa_Void*                 pLoadHandle,
    OpcUa_Void*                 pCertificateStore,
    OpcUa_ByteString*           pCertificate);

/**
  @brief loads a private key object, usually from an encrypted file.

  Note: fileFormat == OpcUa_Crypto_Encoding_DER needs a keyType != OpcUa_Crypto_KeyType_Any
        e.g. OpcUa_Crypto_KeyType_Rsa_Private.
        Other formats can use keyType == OpcUa_Crypto_KeyType_Any as a wildcard.

  @param privateKeyFile           [in]  The file name.
  @param fileFormat               [in]  The file format.
  @param password                 [in]  The encryption password.
  @param keyType                  [in]  The expected key type.

  @param pPrivateKey              [out] The private key (in DER format).
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_LoadPrivateKeyFromFile(
    OpcUa_StringA               privateKeyFile,
    OpcUa_P_FileFormat          fileFormat,
    OpcUa_StringA               password,
    OpcUa_UInt                  keyType,
    OpcUa_Key*                  pPrivateKey);

/**
  @brief Extracts data from a certificate store object.

  @param pCertificate          [in] The certificate to examine.
  @param pIssuer               [out, optional] The issuer name of the certificate.
  @param pSubject              [out, optional] The subject name of the certificate.
  @param pSubjectUri           [out, optional] The subject's URI of the certificate.
  @param pSubjectIP            [out, optional] The subject's IP of the certificate.
  @param pSubjectDNS           [out, optional] The subject's DNS name of the certificate.
  @param pCertThumbprint       [out, optional] The thumbprint of the certificate.
  @param pSubjectHash          [out, optional] The hash code of the certificate.
  @param pCertRawLength        [out, optional] The length of the DER encoded data. Can be smaller than the total length of pCertificate in case of chain certificate or garbage follow.
  @param pNotBefore            [out, optional] The the date on which a certificate becomes valid.
  @param pNotAfter             [out, optional] The date in after which the certificate is no longer valid.
*/
OpcUa_StatusCode OpcUa_P_OpenSSL_PKI_NoSecurity_ExtractCertificateData(
    OpcUa_ByteString*           pCertificate,
    OpcUa_ByteString*           pIssuer,
    OpcUa_ByteString*           pSubject,
    OpcUa_ByteString*           pSubjectUri,
    OpcUa_ByteString*           pSubjectIP,
    OpcUa_ByteString*           pSubjectDNS,
    OpcUa_ByteString*           pCertThumbprint,
    OpcUa_UInt32*               pSubjectHash,
    OpcUa_UInt32*               pCertRawLength,
    OpcUa_Int64*                pNotBefore,
    OpcUa_Int64*                pNotAfter);

OPCUA_END_EXTERN_C

#endif /* _OpcUa_Crypto_OpenSsl_H_ */
