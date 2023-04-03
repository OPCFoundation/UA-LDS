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

#ifndef _OpcUa_Types_H_
#define _OpcUa_Types_H_ 1

#include <opcua_builtintypes.h>

OPCUA_BEGIN_EXTERN_C

struct _OpcUa_Encoder;
struct _OpcUa_Decoder;
struct _OpcUa_EncodeableType;
struct _OpcUa_EnumeratedType;

#ifndef OPCUA_EXCLUDE_ApplicationType
/*============================================================================
 * The ApplicationType enumeration.
 *===========================================================================*/
/** @brief The type of application. */
typedef enum _OpcUa_ApplicationType
{
    OpcUa_ApplicationType_Server          = 0,  /*!< The application is a Server. */
    OpcUa_ApplicationType_Client          = 1,  /*!< The application is a Client. */
    OpcUa_ApplicationType_ClientAndServer = 2,  /*!< The application is a Client and a Server. */
    OpcUa_ApplicationType_DiscoveryServer = 3   /*!< The application is a DiscoveryServer. */
#if OPCUA_FORCE_INT32_ENUMS
    ,_OpcUa_ApplicationType_MaxEnumerationValue = OpcUa_Int32_Max
#endif
}
OpcUa_ApplicationType;

#define OpcUa_ApplicationType_Clear(xValue) OpcUa_EnumeratedType_Clear(xValue, OpcUa_ApplicationType_Server)

#define OpcUa_ApplicationType_Initialize(xValue) OpcUa_EnumeratedType_Initialize(xValue, OpcUa_ApplicationType_Server)

OPCUA_IMEXPORT extern struct _OpcUa_EnumeratedType OpcUa_ApplicationType_EnumeratedType;
#endif

#ifndef OPCUA_EXCLUDE_ApplicationDescription
/*============================================================================
 * The ApplicationDescription structure.
 *===========================================================================*/
/** @brief Specifies an application that is available */
typedef struct _OpcUa_ApplicationDescription
{
    OpcUa_String          ApplicationUri;       /*!< The globally unique identifier for the application instance.
                                                * This URI is used as ServerUri in Services if the application is a Server*/
    OpcUa_String          ProductUri;           /*!< The globally unique identifier for the product. */
    OpcUa_LocalizedText   ApplicationName;      /*!< A localized descriptive name for the application. */
    OpcUa_ApplicationType ApplicationType;      /*!< The type of application.
                                                * This value is an enumeration with one of the following values:
                                                * SERVER_0 The application is a Server.
                                                * CLIENT_1 The application is a Client.
                                                * CLIENTANDSERVER_2 The application is a Client and a Server.
                                                * DISCOVERYSERVER_3 The application is a DiscoveryServer. */
    OpcUa_String          GatewayServerUri;     /*!< A URI that identifies the Gateway Server associated with the discoveryUrls.
                                                * This value is not specified if the Server can be accessed directly.
                                                * This field is not used if the applicationType is CLIENT_1. */
    OpcUa_String          DiscoveryProfileUri;  /*!< A URI that identifies the discovery profile supported by the URLs provided.
                                                * This field is not used if the applicationType is CLIENT_1.
                                                * If this value is not specified then the Endpoints shall support the Discovery Services. */
    OpcUa_Int32           NoOfDiscoveryUrls;    /*!< the Size of DiscoveryUrls list */
    OpcUa_String*         DiscoveryUrls;        /*!< A list of URLs for the discovery Endpoints provided by the application.
                                                * If the applicationType is CLIENT_1, this field shall contain an empty list. */
}
OpcUa_ApplicationDescription;

OPCUA_EXPORT OpcUa_Void OpcUa_ApplicationDescription_Initialize(OpcUa_ApplicationDescription* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_ApplicationDescription_Clear(OpcUa_ApplicationDescription* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ApplicationDescription_GetSize(OpcUa_ApplicationDescription* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ApplicationDescription_Encode(OpcUa_ApplicationDescription* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ApplicationDescription_Decode(OpcUa_ApplicationDescription* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_ApplicationDescription_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_RequestHeader
/*============================================================================
 * The RequestHeader structure.
 *===========================================================================*/
/** @brief Common parameters for all requests submitted on a Session. */
typedef struct _OpcUa_RequestHeader
{
    OpcUa_NodeId          AuthenticationToken;  /*!< The secret Session identifier used to verify that the request is associated with the Session. */
    OpcUa_DateTime        Timestamp;            /*!< The time the Client sent the request.
                                                * The parameter is only used for diagnostic and logging purposes in the server. */
    OpcUa_UInt32          RequestHandle;        /*!< A requestHandle associated with the request.
                                                * This client defined handle can be used to cancel the request.
                                                * It is also returned in the response. */
    OpcUa_UInt32          ReturnDiagnostics;    /*!< A bit mask that identifies the types of vendor-specific diagnostics to be returned in
                                                * diagnosticInfo response parameters.
                                                * The value of this parameter may consist of zero, one or more of the following values.
                                                * No value indicates that diagnostics are not to be returned.
                                                *
                                                * Bit Value Diagnostics to return
                                                * @li 0x0000 0001 ServiceLevel / SymbolicId
                                                * @li 0x0000 0002 ServiceLevel / LocalizedText
                                                * @li 0x0000 0004 ServiceLevel / AdditionalInfo
                                                * @li 0x0000 0008 ServiceLevel / Inner StatusCode
                                                * @li 0x0000 0010 ServiceLevel / Inner Diagnostics
                                                * @li 0x0000 0020 OperationLevel / SymbolicId
                                                * @li 0x0000 0040 OperationLevel / LocalizedText
                                                * @li 0x0000 0080 OperationLevel / AdditionalInfo
                                                * @li 0x0000 0100 OperationLevel / Inner StatusCode
                                                * @li 0x0000 0200 OperationLevel / Inner Diagnostics
                                                *
                                                * Each of these values is composed of two components, level and type, as described below.
                                                * If none are requested, as indicated by a 0 value, or if no diagnostic information was encountered
                                                * in processing of the request,
                                                * then diagnostics information is not returned.
                                                *
                                                * Level:
                                                * @li ServiceLevel return diagnostics in the diagnosticInfo of the Service.
                                                * @li OperationLevel return diagnostics in the diagnosticInfo defined for individual operations
                                                *     requested in the Service.
                                                *
                                                * Type:
                                                * @li SymbolicId return a namespace-qualified, symbolic identifier for an error or condition.
                                                *     The maximum length of this identifier is 32 characters.
                                                * @li LocalizedText return up to 256 bytes of localized text that describes the symbolic id.
                                                * @li AdditionalInfo return a byte string that contains additional diagnostic information, such as
                                                *     a memory image.
                                                *     The format of this byte string is vendor-specific, and may depend on the type of error or
                                                *     condition encountered.
                                                * @li InnerStatusCode return the inner StatusCode associated with the operation or Service.
                                                * @li InnerDiagnostics return the inner diagnostic info associated with the operation or Service.
                                                *     The contents of the inner diagnostic info structure are determined by other bits in the mask.
                                                *     Note that setting this bit could cause multiple levels of nested diagnostic info structures to
                                                *     be returned. */
    OpcUa_String          AuditEntryId;         /*!< An identifier that identifies the Client‟s security audit log entry associated with
                                                * this request.
                                                * An empty string value means that this parameter is not used.
                                                * The AuditEntryId typically contains who initiated the action and from where it was initiated.
                                                * The AuditEventId is included in the AuditEvent to allow human readers to correlate an Event with
                                                * the initiating action. */
    OpcUa_UInt32          TimeoutHint;          /*!< This timeout in milliseconds is used in the Client side Communication Stack to set the timeout
                                                * on a per-call base.
                                                * For a Server this timeout is only a hint and can be used to cancel long running operations to free
                                                * resources.
                                                * If the Server detects a timeout, he can cancel the operation by sending the Service
                                                * result Bad_Timeout.
                                                * The Server should wait at minimum the timeout after he received the request before cancelling
                                                * the operation.
                                                * The value of 0 indicates no timeout. */
    OpcUa_ExtensionObject AdditionalHeader;     /*!< Reserved for future use.
                                                * Applications that do not understand the header should ignore it. */
}
OpcUa_RequestHeader;

OPCUA_EXPORT OpcUa_Void OpcUa_RequestHeader_Initialize(OpcUa_RequestHeader* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_RequestHeader_Clear(OpcUa_RequestHeader* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RequestHeader_GetSize(OpcUa_RequestHeader* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RequestHeader_Encode(OpcUa_RequestHeader* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RequestHeader_Decode(OpcUa_RequestHeader* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_RequestHeader_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_ResponseHeader
/*============================================================================
 * The ResponseHeader structure.
 *===========================================================================*/
/** @brief Common parameters for all responses. */
typedef struct _OpcUa_ResponseHeader
{
    OpcUa_DateTime        Timestamp;            /*!< The time the Server sent the response. */
    OpcUa_UInt32          RequestHandle;        /*!< The requestHandle given by the Client to the request. */
    OpcUa_StatusCode      ServiceResult;        /*!< OPC UA-defined result of the Service invocation. */
    OpcUa_DiagnosticInfo  ServiceDiagnostics;   /*!< Diagnostic information for the Service invocation.
                                                * This parameter is empty if diagnostics information was not requested in the request header. */
    OpcUa_Int32           NoOfStringTable;      /*!< Size of StringTable list */
    OpcUa_String*         StringTable;          /*!< There is one string in this list for each unique namespace, symbolic identifier,
                                                * and localized text string contained in all of the diagnostics information parameters
                                                * contained in the response.
                                                * Each is identified within this table by its zero-based index. */
    OpcUa_ExtensionObject AdditionalHeader;     /*!< Reserved for future use.
                                                * Applications that do not understand the header should ignore it. */
}
OpcUa_ResponseHeader;

OPCUA_EXPORT OpcUa_Void OpcUa_ResponseHeader_Initialize(OpcUa_ResponseHeader* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_ResponseHeader_Clear(OpcUa_ResponseHeader* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ResponseHeader_GetSize(OpcUa_ResponseHeader* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ResponseHeader_Encode(OpcUa_ResponseHeader* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ResponseHeader_Decode(OpcUa_ResponseHeader* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_ResponseHeader_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_ServiceFault
/*============================================================================
 * The ServiceFault structure.
 *===========================================================================*/
/** @brief An error response sent when a service level error occurs. */
typedef struct _OpcUa_ServiceFault
{
    OpcUa_ResponseHeader ResponseHeader;    /*!< Common response parameters
                                            *
                                            * @see OpcUa_ResponseHeader */
}
OpcUa_ServiceFault;

OPCUA_EXPORT OpcUa_Void OpcUa_ServiceFault_Initialize(OpcUa_ServiceFault* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_ServiceFault_Clear(OpcUa_ServiceFault* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ServiceFault_GetSize(OpcUa_ServiceFault* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ServiceFault_Encode(OpcUa_ServiceFault* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ServiceFault_Decode(OpcUa_ServiceFault* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_ServiceFault_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_FindServers
#ifndef OPCUA_EXCLUDE_FindServersRequest
/*============================================================================
 * The FindServersRequest structure.
 *===========================================================================*/
typedef struct _OpcUa_FindServersRequest
{
    OpcUa_RequestHeader RequestHeader;
    OpcUa_String        EndpointUrl;
    OpcUa_Int32         NoOfLocaleIds;
    OpcUa_String*       LocaleIds;
    OpcUa_Int32         NoOfServerUris;
    OpcUa_String*       ServerUris;
}
OpcUa_FindServersRequest;

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersRequest_Initialize(OpcUa_FindServersRequest* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersRequest_Clear(OpcUa_FindServersRequest* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersRequest_GetSize(OpcUa_FindServersRequest* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersRequest_Encode(OpcUa_FindServersRequest* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersRequest_Decode(OpcUa_FindServersRequest* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_FindServersRequest_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_FindServersResponse
/*============================================================================
 * The FindServersResponse structure.
 *===========================================================================*/
typedef struct _OpcUa_FindServersResponse
{
    OpcUa_ResponseHeader          ResponseHeader;
    OpcUa_Int32                   NoOfServers;
    OpcUa_ApplicationDescription* Servers;
}
OpcUa_FindServersResponse;

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersResponse_Initialize(OpcUa_FindServersResponse* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersResponse_Clear(OpcUa_FindServersResponse* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersResponse_GetSize(OpcUa_FindServersResponse* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersResponse_Encode(OpcUa_FindServersResponse* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersResponse_Decode(OpcUa_FindServersResponse* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_FindServersResponse_EncodeableType;
#endif
#endif

#ifndef OPCUA_EXCLUDE_ServerOnNetwork
/*============================================================================
 * The ServerOnNetwork structure.
 *===========================================================================*/
typedef struct _OpcUa_ServerOnNetwork
{
    OpcUa_UInt32  RecordId;
    OpcUa_String  ServerName;
    OpcUa_String  DiscoveryUrl;
    OpcUa_Int32   NoOfServerCapabilities;
    OpcUa_String* ServerCapabilities;
}
OpcUa_ServerOnNetwork;

OPCUA_EXPORT OpcUa_Void OpcUa_ServerOnNetwork_Initialize(OpcUa_ServerOnNetwork* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_ServerOnNetwork_Clear(OpcUa_ServerOnNetwork* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ServerOnNetwork_GetSize(OpcUa_ServerOnNetwork* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ServerOnNetwork_Encode(OpcUa_ServerOnNetwork* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ServerOnNetwork_Decode(OpcUa_ServerOnNetwork* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_ServerOnNetwork_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetwork
#ifndef OPCUA_EXCLUDE_FindServersOnNetworkRequest
/*============================================================================
 * The FindServersOnNetworkRequest structure.
 *===========================================================================*/
typedef struct _OpcUa_FindServersOnNetworkRequest
{
    OpcUa_RequestHeader RequestHeader;
    OpcUa_UInt32        StartingRecordId;
    OpcUa_UInt32        MaxRecordsToReturn;
    OpcUa_Int32         NoOfServerCapabilityFilter;
    OpcUa_String*       ServerCapabilityFilter;
}
OpcUa_FindServersOnNetworkRequest;

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersOnNetworkRequest_Initialize(OpcUa_FindServersOnNetworkRequest* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersOnNetworkRequest_Clear(OpcUa_FindServersOnNetworkRequest* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_GetSize(OpcUa_FindServersOnNetworkRequest* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_Encode(OpcUa_FindServersOnNetworkRequest* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_Decode(OpcUa_FindServersOnNetworkRequest* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_FindServersOnNetworkRequest_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetworkResponse
/*============================================================================
 * The FindServersOnNetworkResponse structure.
 *===========================================================================*/
typedef struct _OpcUa_FindServersOnNetworkResponse
{
    OpcUa_ResponseHeader   ResponseHeader;
    OpcUa_DateTime         LastCounterResetTime;
    OpcUa_Int32            NoOfServers;
    OpcUa_ServerOnNetwork* Servers;
}
OpcUa_FindServersOnNetworkResponse;

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersOnNetworkResponse_Initialize(OpcUa_FindServersOnNetworkResponse* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_FindServersOnNetworkResponse_Clear(OpcUa_FindServersOnNetworkResponse* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_GetSize(OpcUa_FindServersOnNetworkResponse* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_Encode(OpcUa_FindServersOnNetworkResponse* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_Decode(OpcUa_FindServersOnNetworkResponse* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_FindServersOnNetworkResponse_EncodeableType;
#endif
#endif

#ifndef OPCUA_EXCLUDE_MessageSecurityMode
/*============================================================================
 * The MessageSecurityMode enumeration.
 *===========================================================================*/
/** @brief Define the possible message security modes, that can be used for communication. */
typedef enum _OpcUa_MessageSecurityMode
{
    OpcUa_MessageSecurityMode_Invalid        = 0,   /*!< An invalid value. */
    OpcUa_MessageSecurityMode_None           = 1,   /*!< No security mode. Messages are neither signed nor encrypted. */
    OpcUa_MessageSecurityMode_Sign           = 2,   /*!< Messages are only signed. */
    OpcUa_MessageSecurityMode_SignAndEncrypt = 3    /*!< Messages are signed and encrypted. */
#if OPCUA_FORCE_INT32_ENUMS
    ,_OpcUa_MessageSecurityMode_MaxEnumerationValue = OpcUa_Int32_Max
#endif
}
OpcUa_MessageSecurityMode;

#define OpcUa_MessageSecurityMode_Clear(xValue) OpcUa_EnumeratedType_Clear(xValue, OpcUa_MessageSecurityMode_Invalid)

#define OpcUa_MessageSecurityMode_Initialize(xValue) OpcUa_EnumeratedType_Initialize(xValue, OpcUa_MessageSecurityMode_Invalid)

OPCUA_IMEXPORT extern struct _OpcUa_EnumeratedType OpcUa_MessageSecurityMode_EnumeratedType;
#endif

#ifndef OPCUA_EXCLUDE_UserTokenType
/*============================================================================
 * The UserTokenType enumeration.
 *===========================================================================*/
/** @brief Enumeration of different user authentication types. */
typedef enum _OpcUa_UserTokenType
{
    OpcUa_UserTokenType_Anonymous   = 0,/*!< No authentication. The user is anonymous. */
    OpcUa_UserTokenType_UserName    = 1,/*!< Authentication via user name and password. */
    OpcUa_UserTokenType_Certificate = 2,/*!< Authentication via user specific certificates. */
    OpcUa_UserTokenType_IssuedToken = 3 /*!< Authentication via issued token. */
#if OPCUA_FORCE_INT32_ENUMS
    ,_OpcUa_UserTokenType_MaxEnumerationValue = OpcUa_Int32_Max
#endif
}
OpcUa_UserTokenType;

#define OpcUa_UserTokenType_Clear(xValue) OpcUa_EnumeratedType_Clear(xValue, OpcUa_UserTokenType_Anonymous)

#define OpcUa_UserTokenType_Initialize(xValue) OpcUa_EnumeratedType_Initialize(xValue, OpcUa_UserTokenType_Anonymous)

OPCUA_IMEXPORT extern struct _OpcUa_EnumeratedType OpcUa_UserTokenType_EnumeratedType;
#endif

#ifndef OPCUA_EXCLUDE_UserTokenPolicy
/*============================================================================
 * The UserTokenPolicy structure.
 *===========================================================================*/
/** @brief Specifies a UserIdentityToken that a Server will accept. */
typedef struct _OpcUa_UserTokenPolicy
{
    OpcUa_String        PolicyId;           /*!< An identifier for the UserTokenPolicy assigned by the Server.
                                            * The Client specifies this value when it constructs a UserIdentityToken that conforms to the policy.
                                            * This value is only unique within the context of a single Server. */
    OpcUa_UserTokenType TokenType;          /*!< The type of user identity token required.
                                            *
                                            * This value is an enumeration with one of the following values:
                                            * @li ANONYMOUS_0 No token is required.
                                            * @li USERNAME_1 A username/password token.
                                            * @li CERTIFICATE_2 An X509v3 certificate token.
                                            * @li ISSUEDTOKEN_3 Any WS-Security defined token.
                                            *
                                            * A tokenType of ANONYMOUS indicates that the Server does not require any user identification.
                                            * In this case the Client application instance Certificate is used as the user identification. */
    OpcUa_String        IssuedTokenType;    /*!< A URI for the type of token.
                                            * Part 6 defines URIs for common issued token types.
                                            * Vendors may specify their own token.
                                            * This field may only be specified if TokenType is ISSUEDTOKEN_3. */
    OpcUa_String        IssuerEndpointUrl;  /*!< An optional URL for the token issuing service.
                                            * The meaning of this value depends on the issuedTokenType. */
    OpcUa_String        SecurityPolicyUri;  /*!< The security policy to use when encrypting or signing the UserIdentityToken when it is
                                            * passed to the Server in the ActivateSession request.
                                            * The security policy for the SecureChannel is used if this value is omitted. */
}
OpcUa_UserTokenPolicy;

OPCUA_EXPORT OpcUa_Void OpcUa_UserTokenPolicy_Initialize(OpcUa_UserTokenPolicy* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_UserTokenPolicy_Clear(OpcUa_UserTokenPolicy* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_UserTokenPolicy_GetSize(OpcUa_UserTokenPolicy* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_UserTokenPolicy_Encode(OpcUa_UserTokenPolicy* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_UserTokenPolicy_Decode(OpcUa_UserTokenPolicy* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_UserTokenPolicy_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_EndpointDescription
/*============================================================================
 * The EndpointDescription structure.
 *===========================================================================*/
/** @brief Describes an Endpoint for a Server. */
typedef struct _OpcUa_EndpointDescription
{
    OpcUa_String                 EndpointUrl;               /*!< The URL for the Endpoint described. */
    OpcUa_ApplicationDescription Server;                    /*!< The description for the Server that the Endpoint belongs to. */
    OpcUa_ByteString             ServerCertificate;         /*!< The application instance Certificate issued to the Server. */
    OpcUa_MessageSecurityMode    SecurityMode;              /*!< The type of security to apply to the messages.
                                                            * A SecureChannel may have to be created even if the securityMode is NONE.
                                                            * The exact behavior depends on the mapping used. */
    OpcUa_String                 SecurityPolicyUri;         /*!< The URI for SecurityPolicy to use when securing messages. */
    OpcUa_Int32                  NoOfUserIdentityTokens;    /*!< Size of UserIdentityTokens list */
    OpcUa_UserTokenPolicy*       UserIdentityTokens;        /*!< The user identity tokens that the Server will accept.
                                                            * The Client shall pass one of the UserIdentityTokens in the ActivateSession request. */
    OpcUa_String                 TransportProfileUri;       /*!< The URI of the Transport Profile supported by the Endpoint. */
    OpcUa_Byte                   SecurityLevel;             /*!< A numeric value that indicates how secure the EndpointDescription is
                                                            * compared to other EndpointDescriptions for the same Server.
                                                            * A value of 0 indicates that the EndpointDescription is not recommended and is only
                                                            * supported for backward compatibility.
                                                            * A higher value indicates better security. */
}
OpcUa_EndpointDescription;

OPCUA_EXPORT OpcUa_Void OpcUa_EndpointDescription_Initialize(OpcUa_EndpointDescription* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_EndpointDescription_Clear(OpcUa_EndpointDescription* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_EndpointDescription_GetSize(OpcUa_EndpointDescription* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_EndpointDescription_Encode(OpcUa_EndpointDescription* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_EndpointDescription_Decode(OpcUa_EndpointDescription* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_EndpointDescription_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_GetEndpoints
#ifndef OPCUA_EXCLUDE_GetEndpointsRequest
/*============================================================================
 * The GetEndpointsRequest structure.
 *===========================================================================*/
typedef struct _OpcUa_GetEndpointsRequest
{
    OpcUa_RequestHeader RequestHeader;
    OpcUa_String        EndpointUrl;
    OpcUa_Int32         NoOfLocaleIds;
    OpcUa_String*       LocaleIds;
    OpcUa_Int32         NoOfProfileUris;
    OpcUa_String*       ProfileUris;
}
OpcUa_GetEndpointsRequest;

OPCUA_EXPORT OpcUa_Void OpcUa_GetEndpointsRequest_Initialize(OpcUa_GetEndpointsRequest* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_GetEndpointsRequest_Clear(OpcUa_GetEndpointsRequest* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_GetEndpointsRequest_GetSize(OpcUa_GetEndpointsRequest* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_GetEndpointsRequest_Encode(OpcUa_GetEndpointsRequest* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_GetEndpointsRequest_Decode(OpcUa_GetEndpointsRequest* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_GetEndpointsRequest_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_GetEndpointsResponse
/*============================================================================
 * The GetEndpointsResponse structure.
 *===========================================================================*/
typedef struct _OpcUa_GetEndpointsResponse
{
    OpcUa_ResponseHeader       ResponseHeader;
    OpcUa_Int32                NoOfEndpoints;
    OpcUa_EndpointDescription* Endpoints;
}
OpcUa_GetEndpointsResponse;

OPCUA_EXPORT OpcUa_Void OpcUa_GetEndpointsResponse_Initialize(OpcUa_GetEndpointsResponse* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_GetEndpointsResponse_Clear(OpcUa_GetEndpointsResponse* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_GetEndpointsResponse_GetSize(OpcUa_GetEndpointsResponse* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_GetEndpointsResponse_Encode(OpcUa_GetEndpointsResponse* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_GetEndpointsResponse_Decode(OpcUa_GetEndpointsResponse* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_GetEndpointsResponse_EncodeableType;
#endif
#endif

#ifndef OPCUA_EXCLUDE_RegisteredServer
/*============================================================================
 * The RegisteredServer structure.
 *===========================================================================*/
typedef struct _OpcUa_RegisteredServer
{
    OpcUa_String          ServerUri;
    OpcUa_String          ProductUri;
    OpcUa_Int32           NoOfServerNames;
    OpcUa_LocalizedText*  ServerNames;
    OpcUa_ApplicationType ServerType;
    OpcUa_String          GatewayServerUri;
    OpcUa_Int32           NoOfDiscoveryUrls;
    OpcUa_String*         DiscoveryUrls;
    OpcUa_String          SemaphoreFilePath;
    OpcUa_Boolean         IsOnline;
}
OpcUa_RegisteredServer;

OPCUA_EXPORT OpcUa_Void OpcUa_RegisteredServer_Initialize(OpcUa_RegisteredServer* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_RegisteredServer_Clear(OpcUa_RegisteredServer* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisteredServer_GetSize(OpcUa_RegisteredServer* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisteredServer_Encode(OpcUa_RegisteredServer* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisteredServer_Decode(OpcUa_RegisteredServer* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_RegisteredServer_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer
#ifndef OPCUA_EXCLUDE_RegisterServerRequest
/*============================================================================
 * The RegisterServerRequest structure.
 *===========================================================================*/
typedef struct _OpcUa_RegisterServerRequest
{
    OpcUa_RequestHeader    RequestHeader;
    OpcUa_RegisteredServer Server;
}
OpcUa_RegisterServerRequest;

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServerRequest_Initialize(OpcUa_RegisterServerRequest* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServerRequest_Clear(OpcUa_RegisterServerRequest* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServerRequest_GetSize(OpcUa_RegisterServerRequest* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServerRequest_Encode(OpcUa_RegisterServerRequest* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServerRequest_Decode(OpcUa_RegisterServerRequest* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_RegisterServerRequest_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_RegisterServerResponse
/*============================================================================
 * The RegisterServerResponse structure.
 *===========================================================================*/
typedef struct _OpcUa_RegisterServerResponse
{
    OpcUa_ResponseHeader ResponseHeader;
}
OpcUa_RegisterServerResponse;

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServerResponse_Initialize(OpcUa_RegisterServerResponse* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServerResponse_Clear(OpcUa_RegisterServerResponse* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServerResponse_GetSize(OpcUa_RegisterServerResponse* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServerResponse_Encode(OpcUa_RegisterServerResponse* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServerResponse_Decode(OpcUa_RegisterServerResponse* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_RegisterServerResponse_EncodeableType;
#endif
#endif

#ifndef OPCUA_EXCLUDE_MdnsDiscoveryConfiguration
/*============================================================================
 * The MdnsDiscoveryConfiguration structure.
 *===========================================================================*/
typedef struct _OpcUa_MdnsDiscoveryConfiguration
{
    OpcUa_String  MdnsServerName;
    OpcUa_Int32   NoOfServerCapabilities;
    OpcUa_String* ServerCapabilities;
}
OpcUa_MdnsDiscoveryConfiguration;

OPCUA_EXPORT OpcUa_Void OpcUa_MdnsDiscoveryConfiguration_Initialize(OpcUa_MdnsDiscoveryConfiguration* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_MdnsDiscoveryConfiguration_Clear(OpcUa_MdnsDiscoveryConfiguration* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_GetSize(OpcUa_MdnsDiscoveryConfiguration* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_Encode(OpcUa_MdnsDiscoveryConfiguration* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_Decode(OpcUa_MdnsDiscoveryConfiguration* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_MdnsDiscoveryConfiguration_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2
#ifndef OPCUA_EXCLUDE_RegisterServer2Request
/*============================================================================
 * The RegisterServer2Request structure.
 *===========================================================================*/
typedef struct _OpcUa_RegisterServer2Request
{
    OpcUa_RequestHeader    RequestHeader;
    OpcUa_RegisteredServer Server;
    OpcUa_Int32            NoOfDiscoveryConfiguration;
    OpcUa_ExtensionObject* DiscoveryConfiguration;
}
OpcUa_RegisterServer2Request;

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServer2Request_Initialize(OpcUa_RegisterServer2Request* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServer2Request_Clear(OpcUa_RegisterServer2Request* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServer2Request_GetSize(OpcUa_RegisterServer2Request* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServer2Request_Encode(OpcUa_RegisterServer2Request* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServer2Request_Decode(OpcUa_RegisterServer2Request* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_RegisterServer2Request_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2Response
/*============================================================================
 * The RegisterServer2Response structure.
 *===========================================================================*/
typedef struct _OpcUa_RegisterServer2Response
{
    OpcUa_ResponseHeader  ResponseHeader;
    OpcUa_Int32           NoOfConfigurationResults;
    OpcUa_StatusCode*     ConfigurationResults;
    OpcUa_Int32           NoOfDiagnosticInfos;
    OpcUa_DiagnosticInfo* DiagnosticInfos;
}
OpcUa_RegisterServer2Response;

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServer2Response_Initialize(OpcUa_RegisterServer2Response* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_RegisterServer2Response_Clear(OpcUa_RegisterServer2Response* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServer2Response_GetSize(OpcUa_RegisterServer2Response* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServer2Response_Encode(OpcUa_RegisterServer2Response* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_RegisterServer2Response_Decode(OpcUa_RegisterServer2Response* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_RegisterServer2Response_EncodeableType;
#endif
#endif

#ifndef OPCUA_EXCLUDE_SecurityTokenRequestType
/*============================================================================
 * The SecurityTokenRequestType enumeration.
 *===========================================================================*/
/** @brief The type of SecurityToken request */
typedef enum _OpcUa_SecurityTokenRequestType
{
    OpcUa_SecurityTokenRequestType_Issue = 0,   /*!< creates a new SecurityToken for a new SecureChannel. */
    OpcUa_SecurityTokenRequestType_Renew = 1    /*!< creates a new SecurityToken for an existing SecureChannel. */
#if OPCUA_FORCE_INT32_ENUMS
    ,_OpcUa_SecurityTokenRequestType_MaxEnumerationValue = OpcUa_Int32_Max
#endif
}
OpcUa_SecurityTokenRequestType;

#define OpcUa_SecurityTokenRequestType_Clear(xValue) OpcUa_EnumeratedType_Clear(xValue, OpcUa_SecurityTokenRequestType_Issue)

#define OpcUa_SecurityTokenRequestType_Initialize(xValue) OpcUa_EnumeratedType_Initialize(xValue, OpcUa_SecurityTokenRequestType_Issue)

OPCUA_IMEXPORT extern struct _OpcUa_EnumeratedType OpcUa_SecurityTokenRequestType_EnumeratedType;
#endif

#ifndef OPCUA_EXCLUDE_ChannelSecurityToken
/*============================================================================
 * The ChannelSecurityToken structure.
 *===========================================================================*/
/** @brief The ChannelSecurityToken structure. */
typedef struct _OpcUa_ChannelSecurityToken
{
    OpcUa_UInt32   ChannelId;       /*!< A unique identifier for the SecureChannel.
                                    * This is the identifier that shall be supplied whenever the SecureChannel is renewed. */
    OpcUa_UInt32   TokenId;         /*!< A unique identifier for a single SecurityToken within the channel.
                                    * This is the identifier that shall be passed with each Message secured with the SecurityToken. */
    OpcUa_DateTime CreatedAt;       /*!< The time when the SecurityToken was created. */
    OpcUa_UInt32   RevisedLifetime; /*!< The lifetime of the SecurityToken in milliseconds.
                                    * The UTC expiration time for the token may be calculated by adding the lifetime to the createdAt time. */
}
OpcUa_ChannelSecurityToken;

OPCUA_EXPORT OpcUa_Void OpcUa_ChannelSecurityToken_Initialize(OpcUa_ChannelSecurityToken* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_ChannelSecurityToken_Clear(OpcUa_ChannelSecurityToken* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ChannelSecurityToken_GetSize(OpcUa_ChannelSecurityToken* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ChannelSecurityToken_Encode(OpcUa_ChannelSecurityToken* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_ChannelSecurityToken_Decode(OpcUa_ChannelSecurityToken* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_ChannelSecurityToken_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_OpenSecureChannel
#ifndef OPCUA_EXCLUDE_OpenSecureChannelRequest
/*============================================================================
 * The OpenSecureChannelRequest structure.
 *===========================================================================*/
typedef struct _OpcUa_OpenSecureChannelRequest
{
    OpcUa_RequestHeader            RequestHeader;
    OpcUa_UInt32                   ClientProtocolVersion;
    OpcUa_SecurityTokenRequestType RequestType;
    OpcUa_MessageSecurityMode      SecurityMode;
    OpcUa_ByteString               ClientNonce;
    OpcUa_UInt32                   RequestedLifetime;
}
OpcUa_OpenSecureChannelRequest;

OPCUA_EXPORT OpcUa_Void OpcUa_OpenSecureChannelRequest_Initialize(OpcUa_OpenSecureChannelRequest* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_OpenSecureChannelRequest_Clear(OpcUa_OpenSecureChannelRequest* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_GetSize(OpcUa_OpenSecureChannelRequest* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_Encode(OpcUa_OpenSecureChannelRequest* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_Decode(OpcUa_OpenSecureChannelRequest* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_OpenSecureChannelRequest_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_OpenSecureChannelResponse
/*============================================================================
 * The OpenSecureChannelResponse structure.
 *===========================================================================*/
typedef struct _OpcUa_OpenSecureChannelResponse
{
    OpcUa_ResponseHeader       ResponseHeader;
    OpcUa_UInt32               ServerProtocolVersion;
    OpcUa_ChannelSecurityToken SecurityToken;
    OpcUa_ByteString           ServerNonce;
}
OpcUa_OpenSecureChannelResponse;

OPCUA_EXPORT OpcUa_Void OpcUa_OpenSecureChannelResponse_Initialize(OpcUa_OpenSecureChannelResponse* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_OpenSecureChannelResponse_Clear(OpcUa_OpenSecureChannelResponse* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_GetSize(OpcUa_OpenSecureChannelResponse* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_Encode(OpcUa_OpenSecureChannelResponse* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_Decode(OpcUa_OpenSecureChannelResponse* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_OpenSecureChannelResponse_EncodeableType;
#endif
#endif

#ifndef OPCUA_EXCLUDE_CloseSecureChannel
#ifndef OPCUA_EXCLUDE_CloseSecureChannelRequest
/*============================================================================
 * The CloseSecureChannelRequest structure.
 *===========================================================================*/
typedef struct _OpcUa_CloseSecureChannelRequest
{
    OpcUa_RequestHeader RequestHeader;
}
OpcUa_CloseSecureChannelRequest;

OPCUA_EXPORT OpcUa_Void OpcUa_CloseSecureChannelRequest_Initialize(OpcUa_CloseSecureChannelRequest* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_CloseSecureChannelRequest_Clear(OpcUa_CloseSecureChannelRequest* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_GetSize(OpcUa_CloseSecureChannelRequest* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_Encode(OpcUa_CloseSecureChannelRequest* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_Decode(OpcUa_CloseSecureChannelRequest* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_CloseSecureChannelRequest_EncodeableType;
#endif

#ifndef OPCUA_EXCLUDE_CloseSecureChannelResponse
/*============================================================================
 * The CloseSecureChannelResponse structure.
 *===========================================================================*/
typedef struct _OpcUa_CloseSecureChannelResponse
{
    OpcUa_ResponseHeader ResponseHeader;
}
OpcUa_CloseSecureChannelResponse;

OPCUA_EXPORT OpcUa_Void OpcUa_CloseSecureChannelResponse_Initialize(OpcUa_CloseSecureChannelResponse* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_CloseSecureChannelResponse_Clear(OpcUa_CloseSecureChannelResponse* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_GetSize(OpcUa_CloseSecureChannelResponse* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_Encode(OpcUa_CloseSecureChannelResponse* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_Decode(OpcUa_CloseSecureChannelResponse* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_CloseSecureChannelResponse_EncodeableType;
#endif
#endif

#ifndef OPCUA_EXCLUDE_UserIdentityToken
/*============================================================================
 * The UserIdentityToken structure.
 *===========================================================================*/
/** @brief The UserIdentityToken structure. */
typedef struct _OpcUa_UserIdentityToken
{
    OpcUa_String PolicyId;  /*!< An identifier for the UserTokenPolicy that the token conforms to. */
}
OpcUa_UserIdentityToken;

OPCUA_EXPORT OpcUa_Void OpcUa_UserIdentityToken_Initialize(OpcUa_UserIdentityToken* pValue);

OPCUA_EXPORT OpcUa_Void OpcUa_UserIdentityToken_Clear(OpcUa_UserIdentityToken* pValue);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_UserIdentityToken_GetSize(OpcUa_UserIdentityToken* pValue, struct _OpcUa_Encoder* pEncoder, OpcUa_Int32* pSize);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_UserIdentityToken_Encode(OpcUa_UserIdentityToken* pValue, struct _OpcUa_Encoder* pEncoder);

OPCUA_EXPORT OpcUa_StatusCode OpcUa_UserIdentityToken_Decode(OpcUa_UserIdentityToken* pValue, struct _OpcUa_Decoder* pDecoder);

OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType OpcUa_UserIdentityToken_EncodeableType;
#endif

/*============================================================================
 * Table of known types.
 *===========================================================================*/
OPCUA_IMEXPORT extern struct _OpcUa_EncodeableType** OpcUa_KnownEncodeableTypes;

OPCUA_END_EXTERN_C

#endif
/* This is the last line of an autogenerated file. */
