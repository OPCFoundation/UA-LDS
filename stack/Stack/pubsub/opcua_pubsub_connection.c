/* Copyright (c) 1996-2018, OPC Foundation. All rights reserved.

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

#include <opcua.h>

#if OPCUA_HAVE_PUBSUB

#include <opcua_pubsub_connection.h>
#include <opcua_socket.h>
#include <opcua_p_utilities.h>

/* A single UDP message without fragmentation can be up to 1500 bytes,
 * including 20 bytes IPv4 header and 8 bytes UDP header.
 * IPv6 has no fragmentation at all */
#define OPCUA_PUBSUB_MAX_UADP_MESSAGE_SIZE 1472

/*============================================================================
 * OpcUa_PubSubConnectionState
 *===========================================================================*/
 /** @brief The states a PubSubConnection can be in. */
typedef enum _OpcUa_PubSubConnectionState
{
    /** @brief Error state. */
    OpcUa_PubSubConnectionState_Invalid,
    /** @brief Connection is configured for publishing. */
    OpcUa_PubSubConnectionState_Connected,
    /** @brief Connection is configured for subscribing. */
    OpcUa_PubSubConnectionState_Open,
    /** @brief An error message was received and the connection is inactive. */
    OpcUa_PubSubConnectionState_Error
} OpcUa_PubSubConnectionState;


/**
  @brief A generic PubSub connection.
*/
typedef struct _OpcUa_PubSubConnection_Internal
{
    /** @brief Internal helper to verify instances. */
    OpcUa_UInt32                            SanityCheck;

    /** @brief The state of the connection to the server. */
    OpcUa_PubSubConnectionState             ConnectionState;

    /*! @brief The string identifying the address of this connection. */
    OpcUa_String                            Address;

    /*! @brief The string identifying the network interface used by this connection. */
    OpcUa_String                            NetworkInterface;

    /** @brief The socket holding the connection to the server. */
    OpcUa_Socket                            Socket;
} OpcUa_PubSubConnection_Internal;

/*============================================================================
 * OpcUa_PubSubConnection_SanityCheck
 *===========================================================================*/
#define OpcUa_PubSubConnection_SanityCheck 0x6EF9D725

/*============================================================================
 * OpcUa_ReturnErrorIfInvalidConnection
 *===========================================================================*/
#define OpcUa_ReturnErrorIfInvalidPubSubConnection(xConnection) \
if( (xConnection) == OpcUa_Null || \
    ((OpcUa_PubSubConnection_Internal *)(xConnection))->SanityCheck != OpcUa_PubSubConnection_SanityCheck) \
{\
    return OpcUa_BadInvalidArgument;\
}


/*============================================================================
 * OpcUa_PubSubConnection_Create
 *===========================================================================*/
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_PubSubConnection_Create(
    OpcUa_PubSubConnection* a_pConnection)
{
    OpcUa_PubSubConnection_Internal *pInternalConnection = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_PubSubConnection_Create");

    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);

    *a_pConnection = OpcUa_Null;
    pInternalConnection = (OpcUa_PubSubConnection_Internal *)OpcUa_Alloc(sizeof(OpcUa_PubSubConnection_Internal));
    OpcUa_GotoErrorIfAllocFailed(pInternalConnection);

    pInternalConnection->SanityCheck = OpcUa_PubSubConnection_SanityCheck;
    pInternalConnection->ConnectionState = OpcUa_PubSubConnectionState_Invalid;
    OpcUa_String_Initialize(&pInternalConnection->Address);
    OpcUa_String_Initialize(&pInternalConnection->NetworkInterface);
    pInternalConnection->Socket = OpcUa_Null;

    *a_pConnection = (OpcUa_PubSubConnection)pInternalConnection;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_PubSubConnection_Shutdown
 *===========================================================================*/
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_PubSubConnection_Shutdown(
    OpcUa_PubSubConnection    a_pConnection)
{
    OpcUa_PubSubConnection_Internal *pInternalConnection = (OpcUa_PubSubConnection_Internal*)a_pConnection;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_PubSubConnection_Shutdown");

    OpcUa_ReturnErrorIfInvalidPubSubConnection(a_pConnection);

    OpcUa_GotoErrorIfTrue(pInternalConnection->ConnectionState < OpcUa_PubSubConnectionState_Connected, OpcUa_BadConfigurationError);

    /* blind close without error checking */
    OPCUA_P_SOCKET_CLOSE(pInternalConnection->Socket);

    pInternalConnection->ConnectionState = OpcUa_PubSubConnectionState_Invalid;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_PubSubConnection_Delete
 *===========================================================================*/
OPCUA_EXPORT
OpcUa_Void OpcUa_PubSubConnection_Delete(
    OpcUa_PubSubConnection* a_pConnection)
{
    if(a_pConnection != OpcUa_Null && *a_pConnection != OpcUa_Null)
    {
        OpcUa_PubSubConnection_Internal *pInternalConnection = (OpcUa_PubSubConnection_Internal*)*a_pConnection;

        if(pInternalConnection->SanityCheck != OpcUa_PubSubConnection_SanityCheck)
        {
            return;
        }

        if(pInternalConnection->ConnectionState != OpcUa_PubSubConnectionState_Invalid)
        {
            OpcUa_PubSubConnection_Shutdown(*a_pConnection);
        }

        OpcUa_String_Clear(&pInternalConnection->Address);
        OpcUa_String_Clear(&pInternalConnection->NetworkInterface);
        pInternalConnection->SanityCheck = 0;

        OpcUa_Free(*a_pConnection);
        *a_pConnection = OpcUa_Null;
    }
}

/*============================================================================
 * OpcUa_PubSubConnection_Connect
 *===========================================================================*/
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_PubSubConnection_Connect(
    OpcUa_PubSubConnection                  a_Connection,
    const OpcUa_String*                     a_sAddress,
    const OpcUa_String*                     a_sIpOfOwnInterface,
    OpcUa_Byte                              a_uTimeToLive)
{
    OpcUa_PubSubConnection_Internal *pInternalConnection = (OpcUa_PubSubConnection_Internal*)a_Connection;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_PubSubConnection_Connect");

    OpcUa_ReturnErrorIfInvalidPubSubConnection(a_Connection);

    OpcUa_GotoErrorIfTrue(pInternalConnection->ConnectionState >= OpcUa_PubSubConnectionState_Connected, OpcUa_BadConfigurationError);

    uStatus = OpcUa_String_StrnCpy(&pInternalConnection->Address, a_sAddress, OPCUA_STRING_LENDONTCARE);
    OpcUa_GotoErrorIfBad(uStatus);
    uStatus = OpcUa_String_StrnCpy(&pInternalConnection->NetworkInterface, a_sIpOfOwnInterface, OPCUA_STRING_LENDONTCARE);
    OpcUa_GotoErrorIfBad(uStatus);

    if(!OpcUa_String_StrnCmp(&pInternalConnection->Address,
        OpcUa_String_FromCString("opc.udp:"),
        (OpcUa_UInt32)8,
        OpcUa_True))
    {
        OpcUa_StringA  sRemoteIp;
        OpcUa_UInt16   usRemotePort;
        uStatus = OpcUa_P_ParseUrl(OpcUa_String_GetRawString(&pInternalConnection->Address),
            &sRemoteIp,
            &usRemotePort);
        OpcUa_GotoErrorIfBad(uStatus);

        /* create a UDP connection */
        uStatus = OPCUA_P_SOCKETUDP_CREATESENDER(OpcUa_String_GetRawString(&pInternalConnection->NetworkInterface), sRemoteIp, usRemotePort, a_uTimeToLive, &pInternalConnection->Socket);
        OpcUa_Free(sRemoteIp);
    }
    /* the PubSub type is not supported */
    else
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadNotSupported);
    }

    pInternalConnection->ConnectionState = OpcUa_PubSubConnectionState_Connected;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_PubSubConnection_Open
 *===========================================================================*/
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_PubSubConnection_Open(
    OpcUa_PubSubConnection                  a_Connection,
    const OpcUa_String*                     a_sAddress,
    const OpcUa_String*                     a_sIpOfOwnInterface)
{
    OpcUa_PubSubConnection_Internal *pInternalConnection = (OpcUa_PubSubConnection_Internal*)a_Connection;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_PubSubConnection_Open");

    OpcUa_ReturnErrorIfInvalidPubSubConnection(a_Connection);

    OpcUa_GotoErrorIfTrue(pInternalConnection->ConnectionState >= OpcUa_PubSubConnectionState_Connected, OpcUa_BadConfigurationError);

    uStatus = OpcUa_String_StrnCpy(&pInternalConnection->Address, a_sAddress, OPCUA_STRING_LENDONTCARE);
    OpcUa_GotoErrorIfBad(uStatus);
    uStatus = OpcUa_String_StrnCpy(&pInternalConnection->NetworkInterface, a_sIpOfOwnInterface, OPCUA_STRING_LENDONTCARE);
    OpcUa_GotoErrorIfBad(uStatus);

    if(!OpcUa_String_StrnCmp(&pInternalConnection->Address,
        OpcUa_String_FromCString("opc.udp:"),
        (OpcUa_UInt32)8,
        OpcUa_True))
    {
        OpcUa_StringA  sRemoteIp;
        OpcUa_UInt16   usRemotePort;
        uStatus = OpcUa_P_ParseUrl(OpcUa_String_GetRawString(&pInternalConnection->Address),
            &sRemoteIp,
            &usRemotePort);
        OpcUa_GotoErrorIfBad(uStatus);

        /* create a UDP connection */
        uStatus = OPCUA_P_SOCKETUDP_CREATERECEIVER(OpcUa_String_GetRawString(&pInternalConnection->NetworkInterface), sRemoteIp, usRemotePort, &pInternalConnection->Socket);
        OpcUa_Free(sRemoteIp);
    }
    /* the PubSub type is not supported */
    else
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadNotSupported);
    }

    pInternalConnection->ConnectionState = OpcUa_PubSubConnectionState_Open;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_PubSubConnection_Send
 *===========================================================================*/
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_PubSubConnection_Send(
    OpcUa_PubSubConnection      a_pConnection,
    OpcUa_Byte*                 a_pBuffer,
    OpcUa_UInt32                a_BufferSize)
{
    OpcUa_PubSubConnection_Internal *pInternalConnection = (OpcUa_PubSubConnection_Internal*)a_pConnection;
    OpcUa_UInt32 dataSize;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_PubSubConnection_Send");

    OpcUa_ReturnErrorIfInvalidPubSubConnection(a_pConnection);

    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);

    /* Is the connection properly configured for sending? */
    OpcUa_GotoErrorIfTrue(pInternalConnection->ConnectionState != OpcUa_PubSubConnectionState_Connected, OpcUa_BadConfigurationError);

    dataSize = OPCUA_P_SOCKET_WRITE(pInternalConnection->Socket, a_pBuffer, a_BufferSize, OpcUa_False);
    if(dataSize != a_BufferSize)
    {
        uStatus = OpcUa_BadCommunicationError;
    }

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_PubSubConnection_Receive
 *===========================================================================*/
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_PubSubConnection_Receive(
    OpcUa_PubSubConnection      a_pConnection,
    OpcUa_Byte*                 a_pBuffer,
    OpcUa_UInt32                a_BufferSize,
    OpcUa_UInt32*               a_pReceivedBytes)
{
    OpcUa_PubSubConnection_Internal *pInternalConnection = (OpcUa_PubSubConnection_Internal*)a_pConnection;

    OpcUa_InitializeStatus(OpcUa_Module_PubSub, "OpcUa_PubSubConnection_Receive");

    OpcUa_ReturnErrorIfInvalidPubSubConnection(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);
    OpcUa_ReturnErrorIfArgumentNull(a_pReceivedBytes);

    /* Is the connection properly configured for receiving? */
    OpcUa_GotoErrorIfTrue(pInternalConnection->ConnectionState != OpcUa_PubSubConnectionState_Open, OpcUa_BadConfigurationError);

    uStatus = OPCUA_P_SOCKET_READ(pInternalConnection->Socket, a_pBuffer, a_BufferSize, a_pReceivedBytes);

    OpcUa_ReturnStatusCode;

    OpcUa_BeginErrorHandling;

    OpcUa_FinishErrorHandling;
}

#endif
