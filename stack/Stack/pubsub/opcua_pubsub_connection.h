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

#ifndef _OpcUa_PubSub_Connection_H_
#define _OpcUa_PubSub_Connection_H_ 1

#if OPCUA_HAVE_PUBSUB

OPCUA_BEGIN_EXTERN_C

typedef OpcUa_Void* OpcUa_PubSubConnection;

/**
  @brief Creates a new PubSub connection.

  @param a_pConnection  [out] The new connection
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PubSubConnection_Create(
    OpcUa_PubSubConnection*                 a_pConnection);

/**
  @brief Disconnect a connection connected for publishing and releases all resources allocated to it.

  @param a_pConnection       [in] The connection to disconnect.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PubSubConnection_Shutdown(
    OpcUa_PubSubConnection    a_pConnection);

/**
  @brief Frees the connection structure.

  @param a_pConnection [in] The connection.
*/
OPCUA_EXPORT OpcUa_Void OpcUa_PubSubConnection_Delete(
    OpcUa_PubSubConnection* a_pConnection);

/**
  @brief Connects for publishing.

  @param a_pConnection        [in] The connection to connect.
  @param a_sAddress           [in] The connection string for the communication (e.g. "opc.udp://<machine>:<port>" or URL of the broker).
  @param a_sIpOfOwnInterface  [in] An IP address of the local interface to use for the connection (only for UDP connections).
  @param a_uTimeToLive        [in] The number of routers a datagram will pass (only for UDP connections).
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PubSubConnection_Connect(
    OpcUa_PubSubConnection                  a_pConnection,
    const OpcUa_String*                     a_sAddress,
    const OpcUa_String*                     a_sIpOfOwnInterface,
    OpcUa_Byte                              a_uTimeToLive);

/**
  @brief Opens for subscribing.

  @param a_pConnection        [in] The connection to open.
  @param a_sAddress           [in] The connection string for the communication (e.g. "opc.udp://<machine>:<port>" or URL of the broker).
  @param a_sIpOfOwnInterface  [in] An IP address of the local interface to use for the connection (only for UDP connections).
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PubSubConnection_Open(
    OpcUa_PubSubConnection                  a_pConnection,
    const OpcUa_String*                     a_sAddress,
    const OpcUa_String*                     a_sIpOfOwnInterface);

/**
  @brief Sends a publish message message to a connection.

  @param a_pConnection [in]  The connection.
  @param a_pBuffer     [in]  The buffer to be filled.
  @param a_BufferSize  [in]  The size of the buffer.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PubSubConnection_Send(
    OpcUa_PubSubConnection      a_pConnection,
    OpcUa_Byte*                 a_pBuffer,
    OpcUa_UInt32                a_BufferSize);

/**
  @brief Receives a publish message message to a connection.

  @param a_pConnection    [in]     The connection.
  @param a_pBuffer        [in/out] The buffer to be filled.
  @param a_BufferSize     [in]     The size of the buffer.
  @param a_pReceivedBytes [out]    The number of received bytes in the buffer.
*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_PubSubConnection_Receive(
    OpcUa_PubSubConnection      a_pConnection,
    OpcUa_Byte*                 a_pBuffer,
    OpcUa_UInt32                a_BufferSize,
    OpcUa_UInt32*               a_pReceivedBytes);

OPCUA_END_EXTERN_C

#endif /* OPCUA_HAVE_PUBSUB */
#endif /* _OpcUa_PubSub_Connection_H_ */
