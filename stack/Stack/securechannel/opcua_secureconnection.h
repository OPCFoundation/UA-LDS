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

#ifndef _OpcUa_SecureConnection_H_
#define _OpcUa_SecureConnection_H_ 1

#ifdef OPCUA_HAVE_CLIENTAPI

#include <opcua_connection.h>
#include <opcua_encoder.h>
#include <opcua_decoder.h>

OPCUA_BEGIN_EXTERN_C

/**
  @brief Creates a new secure connection object.

  A secure connection is always layered on top of a regular connection.

  @param pInnerConnection [in]  The connection to layer on top of.
  @param pEncoder         [in]  The encoder used by the channel.
  @param pDecoder         [in]  The decoder used by the channel.
  @param ppConnection     [out] The new connection.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureConnection_Create(
    OpcUa_Connection*               pInnerConnection,
    OpcUa_Encoder*                  pEncoder,
    OpcUa_Decoder*                  pDecoder,
    OpcUa_StringTable*              pNamespaceUris,
    OpcUa_EncodeableTypeTable*      pKnownTypes,
    OpcUa_Connection**              ppConnection);

/**
  @brief returns the securechannelid of an existing connection.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureConnection_GetChannelId(
    OpcUa_Connection*       pConnection,
    OpcUa_UInt32*           pSecureChannelId);

/**
  @brief returns the current tokenId of an existing connection.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureConnection_GetCurrentTokenId(
    OpcUa_Connection*       pConnection,
    OpcUa_UInt32*           pCurrentTokenId);


/**
  @brief returns the securechannelid of an existing connection.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureConnection_SetClientPrivateKey(
    OpcUa_Connection*       pConnection,
    OpcUa_ByteString*       pClientPrivateKey);

/**
  @brief sets the client certificate for an existing connection.
*/
OpcUa_StatusCode OpcUa_SecureConnection_SetClientCertificate(
    OpcUa_Connection*       pConnection,
    OpcUa_ByteString*       pClientCertificate);

/**
  @brief sets the client certificate for an existing connection.

*/
OpcUa_StatusCode OpcUa_SecureConnection_SetServerCertificate(
    OpcUa_Connection*       pConnection,
    OpcUa_ByteString*       pServerCertificate);

/**
  @brief returns the client certificate for an existing connection.
*/
OpcUa_StatusCode OpcUa_SecureConnection_GetClientCertificate(
    OpcUa_Connection*       pConnection,
    OpcUa_ByteString*       pClientCertificate);

/**
  @brief sets the pki provider of a connection.
*/
OpcUa_StatusCode OpcUa_SecureConnection_SetClientPKIProvider(
    OpcUa_Connection*       pConnection,
    OpcUa_PKIProvider*      pClientClientPKIProvider);

/**
  @brief
  @param pConnection [in]
  @param ppOstrm     [out]
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureConnection_EndSendOpenSecureChannelRequest(
    OpcUa_Connection*               a_pConnection,
    OpcUa_OutputStream**            a_ppOstrm,
    OpcUa_Void*                     a_pRequest,
    OpcUa_UInt32                    a_uTimeout,
    OpcUa_Connection_PfnOnResponse* a_pCallback,
    OpcUa_Void*                     a_pCallbackData);

/**
  @brief
  @param pConnection [in]
  @param ppOstrm     [out]
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureConnection_EndSendCloseSecureChannelRequest(
    OpcUa_Connection*               pConnection,
    OpcUa_OutputStream**            ppOstrm,
    OpcUa_UInt32                    uSecureChannelId,
    OpcUa_UInt32                    timeout,
    OpcUa_Connection_PfnOnResponse* pCallback,
    OpcUa_Void*                     pCallbackData);

/**
  @brief
  @param pConnection [in]
  @param ppOstrm     [out]
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureConnection_GetSecurityToken(
    OpcUa_Connection*               a_pConnection,
    OpcUa_ChannelSecurityToken**    a_pChannelSecurityToken);

OPCUA_END_EXTERN_C

#endif /* OPCUA_HAVE_CLIENTAPI */

#endif /* _OpcUa_SecureConnection_H_ */
