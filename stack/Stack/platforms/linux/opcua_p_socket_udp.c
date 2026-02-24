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

/* System Headers */
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

/* UA platform definitions */
#include <opcua_p_internal.h>

/* platform layer includes */
#include <opcua_p_mutex.h>
#include <opcua_p_pkifactory.h>

/* own headers */
#include <opcua_p_socket.h>
#include <opcua_p_memory.h>
#include <opcua_p_socket_internal.h>
#include <opcua_p_socket_interface.h>
#include <opcua_p_socket_udp.h>

/*============================================================================
 * The UDP Socket Type
 *===========================================================================*/

/**
* Internal representation for a logical socket (client and server). Includes
* beside the system socket additional information for handling.
*/
struct _OpcUa_InternalUdpSocket
{
    OpcUa_SocketServiceTable*        pSocketServiceTable;/* socket service table */
    OpcUa_RawSocket                  rawSocket;          /* underlying system socket */
};

typedef struct _OpcUa_InternalUdpSocket OpcUa_InternalUdpSocket;

/*============================================================================
 * Read UDP Socket.
 *===========================================================================*/
static OpcUa_StatusCode OpcUa_P_SocketService_UdpRead( OpcUa_Socket    a_pSocket,
                                                       OpcUa_Byte*     a_pBuffer,
                                                       OpcUa_UInt32    a_nBufferSize,
                                                       OpcUa_UInt32*   a_pBytesRead)
{
    OpcUa_InternalUdpSocket*   pInternalSocket     = (OpcUa_InternalUdpSocket*)a_pSocket;
    OpcUa_Int32                api_result;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "UdpRead");

    OpcUa_GotoErrorIfArgumentNull(a_pSocket);
    OpcUa_GotoErrorIfArgumentNull(a_pBuffer);
    OpcUa_GotoErrorIfArgumentNull(a_pBytesRead);

    *a_pBytesRead = 0;

    api_result = OpcUa_P_RawSocket_Read(pInternalSocket->rawSocket, a_pBuffer, a_nBufferSize);
    if(api_result >= 0)
    {
        *a_pBytesRead = (OpcUa_UInt32)api_result;
    }
    else
    {
        api_result = OpcUa_P_RawSocket_GetLastError(pInternalSocket->rawSocket);
        switch(api_result)
        {
            case EWOULDBLOCK:
                uStatus = OpcUa_BadWouldBlock;
                break;
            default:
                uStatus = OpcUa_BadCommunicationError;
                break;
        }
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Write UDP Socket.
 *===========================================================================*/
/* returns number of bytes written to the socket */
static OpcUa_Int32 OpcUa_P_SocketService_UdpWrite( OpcUa_Socket    a_pSocket,
                                                   OpcUa_Byte*     a_pBuffer,
                                                   OpcUa_UInt32    a_uBufferSize,
                                                   OpcUa_Boolean   a_bBlock)
{
    OpcUa_InternalUdpSocket*    pInternalSocket     = (OpcUa_InternalUdpSocket*)a_pSocket;
    OpcUa_Int32                 result;

    OpcUa_ReturnErrorIfNull(a_pSocket, OPCUA_SOCKET_ERROR);
    OpcUa_ReturnErrorIfNull(a_pBuffer, OPCUA_SOCKET_ERROR);
    OpcUa_ReferenceParameter(a_bBlock);

    result = OpcUa_P_RawSocket_Write (pInternalSocket->rawSocket, a_pBuffer, a_uBufferSize);

    return result;
}

/*============================================================================
 * Close UDP Socket.
 *===========================================================================*/
static OpcUa_StatusCode OpcUa_P_SocketService_UdpClose(OpcUa_Socket a_pSocket)
{
    OpcUa_InternalUdpSocket* pInternalSocket = (OpcUa_InternalUdpSocket*)a_pSocket;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "UdpClose");

    OpcUa_GotoErrorIfArgumentNull(a_pSocket);

    OpcUa_P_RawSocket_Close(pInternalSocket->rawSocket);
    OpcUa_P_Memory_Free(pInternalSocket);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Get IP Address and Port Number of the Peer
 *===========================================================================*/
static OpcUa_StatusCode OpcUa_P_SocketService_UdpGetPeerInfo(  OpcUa_Socket  a_pSocket,
                                                               OpcUa_CharA*  a_achPeerInfoBuffer,
                                                               OpcUa_UInt32  a_uiPeerInfoBufferSize)
{
OpcUa_InitializeStatus(OpcUa_Module_Socket, "UdpGetPeerInfo");

    OpcUa_GotoErrorIfArgumentNull(a_pSocket);
    OpcUa_GotoErrorIfArgumentNull(a_achPeerInfoBuffer);
    OpcUa_ReferenceParameter(a_uiPeerInfoBufferSize);

    OpcUa_GotoErrorWithStatus(OpcUa_BadNotImplemented);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Get last socket error
 *===========================================================================*/
static OpcUa_StatusCode OpcUa_P_SocketService_UdpGetLastError(OpcUa_Socket a_pSocket)
{
    OpcUa_InternalUdpSocket* pInternalSocket = (OpcUa_InternalUdpSocket*)a_pSocket;
    int                      err;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "UdpGetLastError");

    OpcUa_GotoErrorIfArgumentNull(a_pSocket);
    
    err = OpcUa_P_RawSocket_GetLastError(pInternalSocket->rawSocket);
    switch(err)
    {
        case EWOULDBLOCK:
            uStatus = OpcUa_BadWouldBlock;
            break;
        default:
            uStatus = OpcUa_BadCommunicationError;
            break;
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Set socket user data
 *===========================================================================*/
static OpcUa_StatusCode OpcUa_P_SocketService_UdpSetUserData(OpcUa_Socket a_pSocket,
                                                             OpcUa_Void*  a_pvUserData)
{
OpcUa_InitializeStatus(OpcUa_Module_Socket, "UdpSetUserData");

    OpcUa_GotoErrorIfArgumentNull(a_pSocket);
    OpcUa_ReferenceParameter(a_pvUserData);

    OpcUa_GotoErrorWithStatus(OpcUa_BadNotImplemented);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Initialize Socket Service Table
 *===========================================================================*/
static OpcUa_SocketServiceTable OpcUa_UdpSocketServiceTable =
{
  OpcUa_P_SocketService_UdpRead,
  OpcUa_P_SocketService_UdpWrite,
  OpcUa_P_SocketService_UdpClose,
  OpcUa_P_SocketService_UdpGetPeerInfo,
  OpcUa_P_SocketService_UdpGetLastError,
  OpcUa_P_SocketService_UdpSetUserData
};

/*============================================================================
 * Create a UDP sender socket
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_SocketUdp_CreateSender(  OpcUa_StringA    a_LocalIpAddress,
                                                                OpcUa_StringA    a_RemoteIpAddress,
                                                                OpcUa_Int16      a_RemotePort,
                                                                OpcUa_Byte       a_TimeToLive,
                                                                OpcUa_Socket*    a_pSocket)
{
    OpcUa_InternalUdpSocket* pInternalSocket = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "CreateUdpSender");

    OpcUa_ReturnErrorIfArgumentNull(a_pSocket);

    *a_pSocket = OpcUa_Null;

    OpcUa_GotoErrorIfArgumentNull(a_RemoteIpAddress);

    if(a_RemotePort == 0)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadInvalidArgument);
    }

    pInternalSocket = (OpcUa_InternalUdpSocket*)OpcUa_P_Memory_Alloc(sizeof(OpcUa_InternalUdpSocket));
    OpcUa_GotoErrorIfAllocFailed(pInternalSocket);

    pInternalSocket->pSocketServiceTable      = &OpcUa_UdpSocketServiceTable;

    if(strchr(a_RemoteIpAddress, ':'))
    {
        uStatus = OpcUa_P_RawSocket_CreateUdpV6(&pInternalSocket->rawSocket,
                                                OpcUa_True,
                                                a_LocalIpAddress,
                                                a_RemoteIpAddress,
                                                a_RemotePort,
                                                a_TimeToLive);
        OpcUa_GotoErrorIfBad(uStatus);
    }
    else
    {
        uStatus = OpcUa_P_RawSocket_CreateUdp(&pInternalSocket->rawSocket,
                                              OpcUa_True,
                                              a_LocalIpAddress,
                                              a_RemoteIpAddress,
                                              a_RemotePort,
                                              a_TimeToLive);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    *a_pSocket = pInternalSocket;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    if(pInternalSocket != OpcUa_Null)
    {
        OpcUa_P_Memory_Free(pInternalSocket);
    }

OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Create a UDP receiver socket
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_SocketUdp_CreateReceiver(  OpcUa_StringA    a_LocalIpAddress,
                                                                  OpcUa_StringA    a_RemoteIpAddress,
                                                                  OpcUa_Int16      a_RemotePort,
                                                                  OpcUa_Socket*    a_pSocket)
{
    OpcUa_InternalUdpSocket* pInternalSocket = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "CreateUdpReceiver");

    OpcUa_ReturnErrorIfArgumentNull(a_pSocket);

    *a_pSocket = OpcUa_Null;

    OpcUa_GotoErrorIfArgumentNull(a_RemoteIpAddress);

    if(a_RemotePort == 0)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadInvalidArgument);
    }

    pInternalSocket = (OpcUa_InternalUdpSocket*)OpcUa_P_Memory_Alloc(sizeof(OpcUa_InternalUdpSocket));
    OpcUa_GotoErrorIfAllocFailed(pInternalSocket);

    pInternalSocket->pSocketServiceTable      = &OpcUa_UdpSocketServiceTable;

    if(strchr(a_RemoteIpAddress, ':'))
    {
        uStatus = OpcUa_P_RawSocket_CreateUdpV6(&pInternalSocket->rawSocket,
                                                OpcUa_False,
                                                a_LocalIpAddress,
                                                a_RemoteIpAddress,
                                                a_RemotePort,
                                                0);
        OpcUa_GotoErrorIfBad(uStatus);
    }
    else
    {
        uStatus = OpcUa_P_RawSocket_CreateUdp(&pInternalSocket->rawSocket,
                                              OpcUa_False,
                                              a_LocalIpAddress,
                                              a_RemoteIpAddress,
                                              a_RemotePort,
                                              0);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    *a_pSocket = pInternalSocket;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    if(pInternalSocket != OpcUa_Null)
    {
        OpcUa_P_Memory_Free(pInternalSocket);
    }

OpcUa_FinishErrorHandling;
}
