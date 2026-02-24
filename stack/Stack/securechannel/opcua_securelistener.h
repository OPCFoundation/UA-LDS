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

#ifndef _OpcUa_SecureListener_H_
#define _OpcUa_SecureListener_H_ 1

#ifdef OPCUA_HAVE_SERVERAPI

#include <opcua_listener.h>
#include <opcua_encoder.h>
#include <opcua_decoder.h>
#include <opcua_securechannel_types.h>

OPCUA_BEGIN_EXTERN_C

/**
 * @brief Types of events that can occur on secure channels.
*/
typedef enum eOpcUa_SecureListener_SecureChannelEvent
{
    eOpcUa_SecureListener_SecureChannelOpen,
    eOpcUa_SecureListener_SecureChannelClose,
    eOpcUa_SecureListener_SecureChannelRenew,
    eOpcUa_SecureListener_SecureChannelUnkown
} OpcUa_SecureListener_SecureChannelEvent;


/**
 * @brief Associates a supported security policy with message security modes.
 */
#ifndef OPCUA_ENDPOINT_SECURITYPOLICYCONFIGURATION_DEFINED
struct _OpcUa_Endpoint_SecurityPolicyConfiguration
{
    /** @brief The URI of a supported security policy. */
    OpcUa_String        sSecurityPolicy;
    /** @brief The message security modes allowed for the security policy. (bitmask) */
    OpcUa_UInt16        uMessageSecurityModes;
    /** @brief The client certificate, if provided. */
    OpcUa_ByteString*   pbsClientCertificate;
};
#define OPCUA_ENDPOINT_SECURITYPOLICYCONFIGURATION_DEFINED
#endif

typedef struct _OpcUa_Endpoint_SecurityPolicyConfiguration OpcUa_SecureListener_SecurityPolicyConfiguration;

/**
 * @brief Function, that needs to be implemented to receive notifications about secure channel events.
 *
 * @param uSecureChannelId      [in] The id assigned to the secure channel.
 * @param eEvent                [in] What type of event on the secure channel occurred.
 * @param uStatus               [in] The result of the operation.
 * @param pbsClientCertificate  [in] The certificate of the client.
 * @param sSecurityPolicy       [in] The security policy in case of open or renew.
 * @param eMessageSecurityMode  [in] What type of event on the secure channel occurred.
 * @param pCallbackData         [in] Data pointer received at creation.
 */
typedef OpcUa_StatusCode (OpcUa_SecureListener_PfnSecureChannelCallback)(
    OpcUa_UInt32                                        uSecureChannelId,
    OpcUa_SecureListener_SecureChannelEvent             eSecureChannelEvent,
    OpcUa_StatusCode                                    uStatus,
    OpcUa_ByteString*                                   pbsClientCertificate,
    OpcUa_String*                                       sSecurityPolicy,
    OpcUa_UInt16                                        uMessageSecurityModes,
    OpcUa_Void*                                         pCallbackData);

/**
  @brief Creates a new secure listener object.

  @param pInnerListener       [in]  The inner the listener is attached to.
  @param ppSecureListener     [out] The new listener.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureListener_Create(
    OpcUa_Listener*                                     pInnerListener,
    OpcUa_Decoder*                                      pDecoder,
    OpcUa_Encoder*                                      pEncoder,
    OpcUa_StringTable*                                  pNamespaceUris,
    OpcUa_EncodeableTypeTable*                          pKnownTypes,
    OpcUa_ByteString*                                   pServerCertificate,
    OpcUa_Key*                                          pServerPrivateKey,
    OpcUa_Void*                                         pPKIConfig,
    OpcUa_UInt32                                        nNoSecurityPolicies,
    OpcUa_SecureListener_SecurityPolicyConfiguration*   pSecurityPolicyConfigurations,
    OpcUa_SecureListener_PfnSecureChannelCallback*      pfSecureChannelCallback,
    OpcUa_Void*                                         pSecureChannelCallbackData,
    OpcUa_Listener**                                    ppSecureListener);


OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureListener_GetChannelId(
    OpcUa_Listener*                                     pListener,
    OpcUa_InputStream*                                  pIstrm,
    OpcUa_UInt32*                                       puChannelId);


OPCUA_EXPORT OpcUa_StatusCode OpcUa_SecureListener_GetSecureChannelSecurityPolicyConfiguration(
    OpcUa_Listener*                                     pListener,
    OpcUa_UInt32                                        uChannelId,
    OpcUa_SecureListener_SecurityPolicyConfiguration*   pSecurityPolicyConfiguration);

OpcUa_StatusCode OpcUa_SecureListener_GetPeerInfo(
    OpcUa_Listener*                                     a_pListener,
    OpcUa_UInt32                                        a_uChannelId,
    OpcUa_String*                                       a_pPeerInfo);

OPCUA_END_EXTERN_C

#endif /* OPCUA_HAVE_SERVERAPI */

#endif /* _OpcUa_SecureListener_H_ */
