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

#ifndef _OpcUa_HttpsListener_ConnectionManager_H_
#define _OpcUa_HttpsListener_ConnectionManager_H_ 1

OPCUA_BEGIN_EXTERN_C

/*==============================================================================*/
/* OpcUa_HttpsListener_Connection                                                     */
/*==============================================================================*/
/**
* @brief Symbolizes a single client connection for the binary protocol layer.
*/
struct _OpcUa_HttpsListener_Connection
{
    /** @brief The socket to the client. */
    OpcUa_Socket                Socket;
    /** @brief Information about the peer. */
    OpcUa_CharA                 achPeerInfo[OPCUA_P_PEERINFO_MIN_SIZE];
    /** @brief The time when the connection was made. */
    OpcUa_UInt32                uConnectTime;
    /** @brief The time when the client disconnected. */
    OpcUa_UInt32                uDisconnectTime;
    /** @brief The last time data was received. */
    OpcUa_UInt32                uLastReceiveTime;
    /** @brief True, as long as the connection is established. */
    OpcUa_Boolean               bConnected;
    /** @brief True, if the client requested the connection to be kept alive. */
    OpcUa_Boolean               bKeepAlive;
    /** @brief Backlink to the listener which hosts the connection. */
    OpcUa_Void*                 pListenerHandle;
    /** @brief Holds a reference to a not fully received stream message. */
    OpcUa_InputStream*          pInputStream;
    /** @brief Holds a reference to a not fully sent stream message. */
    OpcUa_OutputStream*         pOutputStream;
    /** @brief Mutex for granting mutually exclusive access to the connection object */
    OpcUa_Mutex                 Mutex;
    /** @brief Number of request being issued over this connection. */
    OpcUa_UInt32                uNoOfRequestsTotal;
    /** @brief Security Policy provided by the client. */
    OpcUa_String                sSecurityPolicy;
    /** @brief ValidationCallback has been received. */
    OpcUa_Boolean               bCallbackPending;
    /** @brief hResult from ValidationCallback. */
    OpcUa_StatusCode            hValidationResult;
    /** @brief ClientCertificate from ValidationCallback. */
    OpcUa_ByteString            bsClientCertificate;
    /** @brief Counts number of variables pointing to this object. */
    OpcUa_Int32                 iReferenceCount;
    /** @brief The queued list of data blocks to be sent. */
    OpcUa_BufferList*           pSendQueue;
};

typedef struct _OpcUa_HttpsListener_Connection OpcUa_HttpsListener_Connection;

/** @brief Allocate and initialize an HttpListener_Connection */
OpcUa_StatusCode        OpcUa_HttpsListener_Connection_Create(              OpcUa_HttpsListener_Connection**   ppConnection);

/** @brief Clear and free an HttpListener_Connection */
OpcUa_Void              OpcUa_HttpsListener_Connection_Delete(              OpcUa_HttpsListener_Connection**   pValue);

/*==============================================================================*/
/* OpcUa_HttpsListener_ConnectionManager                                              */
/*==============================================================================*/

typedef struct _OpcUa_HttpsListener_ConnectionManager OpcUa_HttpsListener_ConnectionManager;

/**
* @brief Being part of a specific HttpListener, it manages the resources for all clients connected to an endpoint.
*/
struct _OpcUa_HttpsListener_ConnectionManager
{
    /** @brief A list with current connections of type OpcUa_HttpsListener_Connection */
    OpcUa_List*                             Connections;
    /** @brief Backlink to the listener to which the connection manager belongs to. */
    OpcUa_Listener*                         Listener;
};

/* @brief Create a new connection manager object. */
OpcUa_StatusCode    OpcUa_HttpsListener_ConnectionManager_Create(
    OpcUa_HttpsListener_ConnectionManager** ppConnectionManager);

/* @brief Initialize a new connection manager object. */
OpcUa_StatusCode    OpcUa_HttpsListener_ConnectionManager_Initialize(
    OpcUa_HttpsListener_ConnectionManager*  ppConnectionManager);

/* @brief Clear the connection manager object. */
OpcUa_Void          OpcUa_HttpsListener_ConnectionManager_Clear(
    OpcUa_HttpsListener_ConnectionManager*  ConnectionManager);

/* @brief Delete the connection manager object. */
OpcUa_Void          OpcUa_HttpsListener_ConnectionManager_Delete(
    OpcUa_HttpsListener_ConnectionManager** ppConnectionManager);


/* @brief Add a new connection object to the list of managed connections. */
OpcUa_StatusCode    OpcUa_HttpsListener_ConnectionManager_AddConnection(
    OpcUa_HttpsListener_ConnectionManager*  ConnectionManager,
    OpcUa_HttpsListener_Connection*         Connection);

/* @brief Retrieve the connection object identified by the socket. */
OpcUa_StatusCode    OpcUa_HttpsListener_ConnectionManager_GetConnectionBySocket(
    OpcUa_HttpsListener_ConnectionManager*  ConnectionManager,
    OpcUa_Socket                            Socket,
    OpcUa_HttpsListener_Connection**        Connection);

/* @brief Release reference to given connection. Pointer gets nulled on return. */
OpcUa_StatusCode OpcUa_HttpsListener_ConnectionManager_ReleaseConnection(
    OpcUa_HttpsListener_ConnectionManager*    a_pConnectionManager,
    OpcUa_HttpsListener_Connection**          a_ppConnection);

/* @brief Get the current number of managed connections. */
OpcUa_StatusCode    OpcUa_HttpsListener_ConnectionManager_GetConnectionCount(
    OpcUa_HttpsListener_ConnectionManager*  ConnectionManager,
    OpcUa_UInt32*                           pNoOfConnections);

OPCUA_END_EXTERN_C

#endif /* _OpcUa_HttpsListener_ConnectionManager_H_ */

