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

/******************************************************************************************************/
/* Platform Portability Layer                                                                         */
/* Modify the content of this file according to the socket implementation on your system.             */
/* Win32                                                                                              */
/******************************************************************************************************/

/* System Headers */
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

/* UA platform definitions */
#include <opcua_p_internal.h>
#include <opcua_p_utilities.h>

/* own headers */
#include <opcua_p_socket.h>

/*============================================================================
 * Initialize the platform network interface
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_InitializeNetwork(OpcUa_Void)
{
    WSADATA wsaData;
    int     apiResult;

    /* The return value is zero if the operation was successful.
       Otherwise, the value SOCKET_ERROR is returned, and a specific
       error number can be retrieved by calling WSAGetLastError. */
    apiResult = WSAStartup(0x202, &wsaData);

    if(apiResult == 0)
    {
        return OpcUa_Good;
    }
    return OpcUa_BadCommunicationError;
}

/*============================================================================
 * Clean the platform network interface up.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_CleanupNetwork(OpcUa_Void)
{
    WSACleanup();
    return OpcUa_Good;
}

/*============================================================================
 * Close Socket.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_Close(OpcUa_RawSocket a_RawSocket)
{
    SOCKET winSocket = (SOCKET)OPCUA_P_SOCKET_INVALID;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_Close");

    winSocket = (SOCKET)a_RawSocket;

    /* close socket */
    shutdown(winSocket, OPCUA_P_SOCKET_SD_BOTH);

    uStatus = closesocket(winSocket);

    /* check uStatus */
    if(uStatus == OPCUA_P_SOCKET_SOCKETERROR)
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Create Socket.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_Create(  OpcUa_RawSocket*    a_pRawSocket,
                                            OpcUa_Boolean       a_bNagleOff,
                                            OpcUa_Boolean       a_bKeepAliveOn)
{
    OpcUa_StatusCode    uStatus     = OpcUa_Good;
    int                 flag        = 1;
    OpcUa_Int           apiResult   = 0;
    OpcUa_Int           iBufferSize = OPCUA_P_TCPRCVBUFFERSIZE;
    SOCKET              WinSocket   = (SOCKET)OPCUA_P_SOCKET_INVALID;

    OpcUa_GotoErrorIfArgumentNull(a_pRawSocket);


    /* create socket through platform API */
    WinSocket = socket(AF_INET, SOCK_STREAM, 0);
    apiResult = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)WinSocket);

    /* check if socket creation was successful */
    if(     WinSocket == OPCUA_P_SOCKET_INVALID
        ||  apiResult != 0)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

    /* set socketoptions */
    if(a_bNagleOff)
    {
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)))
        {
            uStatus = OpcUa_BadCommunicationError;
            goto Error;
        }
    }
    if(a_bKeepAliveOn)
    {
        /* set socket options */
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, IPPROTO_TCP,  SO_KEEPALIVE, (char*)&flag, sizeof(int)))
        {
            uStatus = OpcUa_BadCommunicationError;
            goto Error;
        }
    }

#if OPCUA_P_SOCKET_SETTCPRCVBUFFERSIZE
#ifdef _WIN32_WCE
#else /* _WIN32_WCE */
    iBufferSize = OPCUA_P_TCPRCVBUFFERSIZE;
    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, SOL_SOCKET,  SO_RCVBUF, (char*)&iBufferSize, sizeof(int)))
    {
        /*int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)WinSocket);*/
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }
#endif /* _WIN32_WCE */
#endif /* OPCUA_P_SOCKET_SETTCPRCVBUFFERSIZE */

#if OPCUA_P_SOCKET_SETTCPSNDBUFFERSIZE
#ifdef _WIN32_WCE
#else /* _WIN32_WCE */
    iBufferSize = OPCUA_P_TCPSNDBUFFERSIZE;
    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, SOL_SOCKET,  SO_SNDBUF, (char*)&iBufferSize, sizeof(int)))
    {
        /*int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)WinSocket);*/
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }
#endif /* _WIN32_WCE */
#endif /* OPCUA_P_SOCKET_SETTCPSNDBUFFERSIZE */

    *a_pRawSocket = (OpcUa_RawSocket)WinSocket;

    return OpcUa_Good;

Error:

    if(WinSocket != OPCUA_P_SOCKET_INVALID)
    {
        OpcUa_P_RawSocket_Close((OpcUa_RawSocket)WinSocket);
        *a_pRawSocket = (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID;
    }

    return uStatus;
}

/*============================================================================
 * Create IPv6 Socket.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_CreateV6(  OpcUa_RawSocket*    a_pRawSocket,
                                              OpcUa_Boolean       a_bNagleOff,
                                              OpcUa_Boolean       a_bKeepAliveOn,
                                              OpcUa_Boolean       a_bV6Only)
{
    OpcUa_StatusCode    uStatus     = OpcUa_Good;
    int                 flag        = 1;
    OpcUa_Int           apiResult   = 0;
    OpcUa_Int           iBufferSize = OPCUA_P_TCPRCVBUFFERSIZE;
    SOCKET              WinSocket   = OPCUA_P_SOCKET_INVALID;

    OpcUa_GotoErrorIfArgumentNull(a_pRawSocket);

    /* create socket through platform API */
    WinSocket = socket(AF_INET6, SOCK_STREAM, 0);
    apiResult = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)WinSocket);

    /* check if socket creation was successful */
    if(     WinSocket == OPCUA_P_SOCKET_INVALID
        ||  apiResult != 0)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

    /* set socketoptions */
    if(a_bNagleOff)
    {
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)))
        {
            uStatus = OpcUa_BadCommunicationError;
            goto Error;
        }
    }
    if(a_bKeepAliveOn)
    {
        /* set socket options */
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, SOL_SOCKET,  SO_KEEPALIVE, (char*)&flag, sizeof(int)))
        {
            uStatus = OpcUa_BadCommunicationError;
            goto Error;
        }
    }

#if OPCUA_P_SOCKET_SETTCPRCVBUFFERSIZE
#ifdef _WIN32_WCE
#else /* _WIN32_WCE */
    iBufferSize = OPCUA_P_TCPRCVBUFFERSIZE;
    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, SOL_SOCKET,  SO_RCVBUF, (char*)&iBufferSize, sizeof(int)))
    {
        /*int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)linSocket);*/
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }
#endif /* _WIN32_WCE */
#endif /* OPCUA_P_SOCKET_SETTCPRCVBUFFERSIZE */

#if OPCUA_P_SOCKET_SETTCPSNDBUFFERSIZE
#ifdef _WIN32_WCE
#else /* _WIN32_WCE */
    iBufferSize = OPCUA_P_TCPSNDBUFFERSIZE;
    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, SOL_SOCKET,  SO_SNDBUF, (char*)&iBufferSize, sizeof(int)))
    {
        /*int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)linSocket);*/
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }
#endif /* _WIN32_WCE */
#endif /* OPCUA_P_SOCKET_SETTCPSNDBUFFERSIZE */

    flag = a_bV6Only != OpcUa_False;
#ifdef IPV6_V6ONLY
    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(WinSocket, IPPROTO_IPV6,  IPV6_V6ONLY, (char*)&flag, sizeof(int)))
#endif
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

    *a_pRawSocket = (OpcUa_RawSocket)WinSocket;

    return OpcUa_Good;

Error:

    if(WinSocket != OPCUA_P_SOCKET_INVALID)
    {
        OpcUa_P_RawSocket_Close((OpcUa_RawSocket)WinSocket);
        *a_pRawSocket = (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID;
    }

    return uStatus;
}

/*============================================================================
 * Connect Socket for Client.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_Connect( OpcUa_RawSocket a_RawSocket,
                                            OpcUa_Int16     a_nPort,
                                            OpcUa_StringA   a_sHost)
{
    socklen_t           intSize;
    SOCKET              winSocket = (SOCKET)OPCUA_P_SOCKET_INVALID;
    struct sockaddr     *pName;
    struct sockaddr_in  srv;
    char*               localhost = "127.0.0.1";

OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_Connect");

    OpcUa_GotoErrorIfArgumentNull(a_RawSocket);
    winSocket = (SOCKET)a_RawSocket;

    intSize = sizeof(struct sockaddr_in);
    OpcUa_MemSet(&srv, 0, intSize);

    if(!strcmp("localhost", a_sHost))
    {
        a_sHost = localhost;
    }

    srv.sin_addr.s_addr = inet_addr(a_sHost);

    if(srv.sin_addr.s_addr == INADDR_NONE)
    {
        return OpcUa_BadInvalidArgument;
    }

    srv.sin_port   = htons(a_nPort);
    srv.sin_family = AF_INET;

    pName = (struct sockaddr *) &srv;

    if(connect(winSocket, pName, intSize) == OPCUA_P_SOCKET_SOCKETERROR)
    {
        int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)winSocket);

        /* a connect takes some time and this "error" is common with nonblocking sockets */
        if(result == WSAEWOULDBLOCK || result == WSAEINPROGRESS)
        {
            uStatus = OpcUa_BadWouldBlock;
        }
        else
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_WARNING, "OpcUa_P_RawSocket_Connect: error 0x%08X\n", result);
            uStatus = OpcUa_BadCommunicationError;
        }
        goto Error;
    }

    uStatus = OpcUa_Good;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Connect IPv6 Socket for Client.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_ConnectV6( OpcUa_RawSocket a_RawSocket,
                                              OpcUa_Int16     a_nPort,
                                              OpcUa_StringA   a_sHost)
{
    socklen_t           intSize;
    SOCKET              winSocket = (SOCKET)OPCUA_P_SOCKET_INVALID;
    struct sockaddr     *pName;
    struct sockaddr_in6 srv;
    struct in6_addr     localhost = IN6ADDR_LOOPBACK_INIT;
    char                *pScopeId;
    int                 apiResult;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_ConnectV6");

    OpcUa_GotoErrorIfArgumentNull(a_RawSocket);
    winSocket = (SOCKET)a_RawSocket;

    intSize = sizeof(struct sockaddr_in6);
    OpcUa_MemSet(&srv, 0, intSize);

    if(!strcmp("localhost", a_sHost))
    {
        srv.sin6_addr = localhost;
    }
    else
    {
        pScopeId = strchr(a_sHost, '%');
#ifdef _WIN32_WCE
        if(pScopeId != NULL)
        {
            srv.sin6_scope_id = OpcUa_P_CharAToInt(pScopeId+1);
        }
        apiResult = OpcUa_SScanfA(a_sHost,
                                  "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
                                  &srv.sin6_addr.s6_words[0],
                                  &srv.sin6_addr.s6_words[1],
                                  &srv.sin6_addr.s6_words[2],
                                  &srv.sin6_addr.s6_words[3],
                                  &srv.sin6_addr.s6_words[4],
                                  &srv.sin6_addr.s6_words[5],
                                  &srv.sin6_addr.s6_words[6],
                                  &srv.sin6_addr.s6_words[7]);
        if(apiResult != 8)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
        while(apiResult --> 0)
        {
            srv.sin6_addr.s6_words[apiResult] = OpcUa_P_RawSocket_HToNS(srv.sin6_addr.s6_words[apiResult]);
        }
#else
        if(pScopeId != NULL)
        {
            srv.sin6_scope_id = OpcUa_P_CharAToInt(pScopeId+1);
            *pScopeId = 0;
        }
        apiResult = inet_pton(AF_INET6, a_sHost, (void*)&srv.sin6_addr);
        if(pScopeId != NULL)
        {
            *pScopeId = '%';
        }
        if(apiResult <= 0)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
#endif
    }

    srv.sin6_port       = htons(a_nPort);
    srv.sin6_family     = AF_INET6;
    pName               = (struct sockaddr*)&srv;

    if(connect(winSocket, pName, intSize) == OPCUA_P_SOCKET_SOCKETERROR)
    {
        int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)winSocket);

        /* a connect takes some time and this "error" is common with nonblocking sockets */
        if(result == WSAEWOULDBLOCK || result == WSAEINPROGRESS)
        {
            uStatus = OpcUa_BadWouldBlock;
        }
        else
        {
            uStatus = OpcUa_BadCommunicationError;
        }
        goto Error;
    }

    uStatus = OpcUa_Good;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Bind to Socket
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_Bind(    OpcUa_RawSocket a_RawSocket,
                                            OpcUa_Int16     a_nPort)
{
    socklen_t           intSize;
    SOCKET              winSocket  = (SOCKET)OPCUA_P_SOCKET_INVALID;
    struct sockaddr_in  srv;
    struct sockaddr     *pName;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_Bind");

    OpcUa_GotoErrorIfArgumentNull(a_RawSocket);
    winSocket = (SOCKET)a_RawSocket;

    intSize = sizeof(struct sockaddr_in);
    OpcUa_MemSet(&srv, 0, intSize);

    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port        = htons(a_nPort);
    srv.sin_family      = AF_INET;
    pName               = (struct sockaddr*)&srv;

    if(bind(winSocket, pName, intSize) == OPCUA_P_SOCKET_SOCKETERROR)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}


/*============================================================================
 * Bind to Socket to a specific adapter
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_BindEx(  OpcUa_RawSocket a_RawSocket,
                                            OpcUa_StringA   a_IpAddress,
                                            OpcUa_Int16     a_nPort)
{
    socklen_t           intSize;
    SOCKET              winSocket  = (SOCKET)OPCUA_P_SOCKET_INVALID;
    struct sockaddr_in  srv;
    struct sockaddr     *pName;
    unsigned long       uIp = INADDR_ANY;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_BindEx");

    OpcUa_GotoErrorIfArgumentNull(a_RawSocket);
    winSocket = (SOCKET)a_RawSocket;

    intSize = sizeof(struct sockaddr_in);
    OpcUa_MemSet(&srv, 0, intSize);

    if(a_IpAddress != OpcUa_Null)
    {
        uIp = inet_addr(a_IpAddress);
    }

    srv.sin_addr.s_addr = uIp;
    srv.sin_port        = htons(a_nPort);
    srv.sin_family      = AF_INET;
    pName               = (struct sockaddr*)&srv;

    if(bind(winSocket, pName, intSize) == OPCUA_P_SOCKET_SOCKETERROR)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Bind to Socket to a specific adapter
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_BindV6(  OpcUa_RawSocket a_RawSocket,
                                            OpcUa_StringA   a_IpAddress,
                                            OpcUa_Int16     a_nPort)
{
    socklen_t           intSize;
    SOCKET              winSocket  = OPCUA_P_SOCKET_INVALID;
    struct sockaddr_in6 srv;
    struct sockaddr     *pName;
    char                *pScopeId;
    int                 apiResult;


OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_BindV6");
    OpcUa_GotoErrorIfArgumentNull(a_RawSocket);

    winSocket = (SOCKET)a_RawSocket;

    intSize = sizeof(struct sockaddr_in6);
    OpcUa_MemSet(&srv, 0, intSize);

    if(a_IpAddress != OpcUa_Null)
    {
        pScopeId = strchr(a_IpAddress, '%');
#ifdef _WIN32_WCE
        if(pScopeId != NULL)
        {
            srv.sin6_scope_id = OpcUa_P_CharAToInt(pScopeId+1);
        }
        apiResult = OpcUa_SScanfA(a_IpAddress,
                                  "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
                                  &srv.sin6_addr.s6_words[0],
                                  &srv.sin6_addr.s6_words[1],
                                  &srv.sin6_addr.s6_words[2],
                                  &srv.sin6_addr.s6_words[3],
                                  &srv.sin6_addr.s6_words[4],
                                  &srv.sin6_addr.s6_words[5],
                                  &srv.sin6_addr.s6_words[6],
                                  &srv.sin6_addr.s6_words[7]);
        if(apiResult != 8)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
        while(apiResult --> 0)
        {
            srv.sin6_addr.s6_words[apiResult] = OpcUa_P_RawSocket_HToNS(srv.sin6_addr.s6_words[apiResult]);
        }
#else
        if(pScopeId != NULL)
        {
            srv.sin6_scope_id = OpcUa_P_CharAToInt(pScopeId+1);
            *pScopeId = 0;
        }
        apiResult = inet_pton(AF_INET6, a_IpAddress, (void*)&srv.sin6_addr);
        if(pScopeId != NULL)
        {
            *pScopeId = '%';
        }
        if(apiResult <= 0)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
#endif
    }

    srv.sin6_port       = htons(a_nPort);
    srv.sin6_family     = AF_INET6;
    pName               = (struct sockaddr*)&srv;

    if(bind(winSocket, pName, intSize) == OPCUA_P_SOCKET_SOCKETERROR)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Bind to Socket and set to listen for Server.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_Listen(OpcUa_RawSocket a_RawSocket)
{
    SOCKET winSocket  = (SOCKET)OPCUA_P_SOCKET_INVALID;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_Listen");

    OpcUa_GotoErrorIfArgumentNull(a_RawSocket);
    winSocket = (SOCKET)a_RawSocket;

    if(listen(winSocket, SOMAXCONN) == OPCUA_P_SOCKET_SOCKETERROR)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Accept Socket connection from Client.
 *===========================================================================*/
OpcUa_RawSocket OpcUa_P_RawSocket_Accept(   OpcUa_RawSocket a_RawSocket,
                                            OpcUa_Boolean   a_bNagleOff,
                                            OpcUa_Boolean   a_bKeepAliveOn)
{
    int                 flag           = 1;
    SOCKET              winSocketServer= (SOCKET)OPCUA_P_SOCKET_INVALID;
    SOCKET              winSocketClient= (SOCKET)OPCUA_P_SOCKET_INVALID;

    OpcUa_Int           iBufferSize    = OPCUA_P_TCPRCVBUFFERSIZE;

    if(a_RawSocket == (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID)
    {
        return (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID;
    }
    winSocketServer = (SOCKET)a_RawSocket;

    winSocketClient = accept(winSocketServer, NULL, NULL);

    if(winSocketClient == OPCUA_P_SOCKET_INVALID)
    {
        /* accept failed */
        goto Error;
    }

    if(a_bNagleOff)
    {
        /* set socket options */
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocketClient, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)))
        {
            goto Error;
        }
    }

    if(a_bKeepAliveOn)
    {
        /* set socket options */
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt( winSocketClient, IPPROTO_TCP, SO_KEEPALIVE, (char*)&flag, sizeof(int)))
        {
            goto Error;
        }
    }

#if OPCUA_P_SOCKET_SETTCPRCVBUFFERSIZE
#ifdef _WIN32_WCE
#else /* _WIN32_WCE */
    iBufferSize = OPCUA_P_TCPRCVBUFFERSIZE;
    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocketClient, SOL_SOCKET,  SO_RCVBUF, (char*)&iBufferSize, sizeof(int)))
    {
        /*int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)winSocketClient);*/
        goto Error;
    }
#endif /* _WIN32_WCE */
#endif /* OPCUA_P_SOCKET_SETTCPRCVBUFFERSIZE */

#if OPCUA_P_SOCKET_SETTCPSNDBUFFERSIZE
#ifdef _WIN32_WCE
#else /* _WIN32_WCE */
    iBufferSize = OPCUA_P_TCPSNDBUFFERSIZE;
    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocketClient, SOL_SOCKET,  SO_SNDBUF, (char*)&iBufferSize, sizeof(int)))
    {
        /*int result = OpcUa_P_RawSocket_GetLastError((OpcUa_RawSocket)winSocketClient);*/
        goto Error;
    }
#endif /* _WIN32_WCE */
#endif /* OPCUA_P_SOCKET_SETTCPSNDBUFFERSIZE */

    return (OpcUa_RawSocket)winSocketClient;

Error:

    if(winSocketClient != OPCUA_P_SOCKET_INVALID)
    {
        OpcUa_P_RawSocket_Close((OpcUa_RawSocket)winSocketClient);
    }

    return (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID;
}

/*============================================================================
 * Read Socket.
 *===========================================================================*/
OpcUa_Int32 OpcUa_P_RawSocket_Read( OpcUa_RawSocket a_RawSocket,
                                    OpcUa_Byte*     a_pBuffer,
                                    OpcUa_UInt32    a_nBufferSize)
{
    int     intBytesReceived    = 0;
    SOCKET  winSocket           = (SOCKET)OPCUA_P_SOCKET_INVALID;

    if(a_RawSocket == (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID)
    {
        return 0;
    }

    winSocket = (SOCKET)a_RawSocket;

    intBytesReceived = recv(winSocket, (char*)a_pBuffer, (int)a_nBufferSize, 0);

    return intBytesReceived;
}

/*============================================================================
 * Write Socket.
 *===========================================================================*/
OpcUa_Int32 OpcUa_P_RawSocket_Write(    OpcUa_RawSocket a_RawSocket,
                                        OpcUa_Byte*     a_pBuffer,
                                        OpcUa_UInt32    a_uBufferSize)
{
    int     intBytesSend    = 0;
    SOCKET  winSocket       = (SOCKET)OPCUA_P_SOCKET_INVALID;

    if(a_RawSocket == (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID)
    {
        return 0;
    }

    winSocket = (SOCKET)a_RawSocket;

    intBytesSend = send(winSocket, (char*)a_pBuffer, a_uBufferSize, 0);

    return intBytesSend;
}


/*============================================================================
 * Set socket to nonblocking mode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_SetBlockMode(    OpcUa_RawSocket a_RawSocket,
                                                    OpcUa_Boolean   a_bBlocking)
{
    int              apiResult   = 0;
    SOCKET           winSocket   = (SOCKET)OPCUA_P_SOCKET_INVALID;
    OpcUa_StatusCode uStatus     = OpcUa_Good;
    u_long           uNonBlocking= (a_bBlocking==OpcUa_False)?1:0;

    if(a_RawSocket == (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID)
    {
        return 0;
    }

    winSocket = (SOCKET)a_RawSocket;

    apiResult = ioctlsocket(winSocket, FIONBIO, &uNonBlocking);

    if(apiResult != 0)
    {
        uStatus = OpcUa_BadCommunicationError;
    }

    return uStatus;
}


/*============================================================================
 * Network Byte Order Conversion Helper Functions
 *===========================================================================*/
OpcUa_UInt32 OpcUa_P_RawSocket_NToHL(OpcUa_UInt32 netLong)
{
    OpcUa_UInt32 retval = ntohl(netLong);
    return retval;
}

OpcUa_UInt16 OpcUa_P_RawSocket_NToHS(OpcUa_UInt16 netShort)
{
    OpcUa_UInt16 retval = ntohs(netShort);
    return retval;
}

OpcUa_UInt32 OpcUa_P_RawSocket_HToNL(OpcUa_UInt32 hstLong)
{
    OpcUa_UInt32 retval = htonl(hstLong);
    return retval;
}

OpcUa_UInt16 OpcUa_P_RawSocket_HToNS(OpcUa_UInt16 hstShort)
{
    OpcUa_UInt16 retval = htons(hstShort);
    return retval;
}

/*============================================================================
 * Get address information about the peer
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_GetPeerInfo( OpcUa_RawSocket a_RawSocket,
                                                OpcUa_CharA*    a_achPeerInfoBuffer,
                                                OpcUa_UInt32    a_uiPeerInfoBufferSize)
{
    int                 apiResult       = 0;
    struct sockaddr_storage sockAddrIn;
    int                 sockAddrInLen   = sizeof(sockAddrIn);
    SOCKET              winSocket       = (SOCKET)OPCUA_P_SOCKET_INVALID;
    OpcUa_UInt32        PeerIp;
    OpcUa_UInt16        usPort;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "GetPeerInfo");

    /* initial parameter check */
    OpcUa_ReturnErrorIfTrue((a_RawSocket == (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID), OpcUa_BadInvalidArgument);
    OpcUa_ReturnErrorIfArgumentNull(a_achPeerInfoBuffer);
    OpcUa_ReturnErrorIfTrue((a_uiPeerInfoBufferSize < OPCUA_P_PEERINFO_MIN_SIZE), OpcUa_BadInvalidArgument);

    winSocket = (SOCKET)a_RawSocket;
    apiResult = getpeername(winSocket, (struct sockaddr*)&sockAddrIn, &sockAddrInLen);

    OpcUa_ReturnErrorIfTrue((apiResult != 0), OpcUa_BadInternalError);

    if(sockAddrIn.ss_family == AF_INET6)
    {
        struct sockaddr_in6* pAddr = (struct sockaddr_in6*)&sockAddrIn;

        /* Port */
        usPort = OpcUa_P_RawSocket_NToHS((OpcUa_UInt16)pAddr->sin6_port);

        /* IP */
        if(IN6_IS_ADDR_V4MAPPED(&pAddr->sin6_addr))
        {
            OpcUa_SPrintfA( a_achPeerInfoBuffer,
#if OPCUA_USE_SAFE_FUNCTIONS
                            a_uiPeerInfoBufferSize,
#endif /* OPCUA_USE_SAFE_FUNCTIONS */
                            "%u.%u.%u.%u:%u",
                            pAddr->sin6_addr.s6_addr[12],
                            pAddr->sin6_addr.s6_addr[13],
                            pAddr->sin6_addr.s6_addr[14],
                            pAddr->sin6_addr.s6_addr[15],
                            usPort);
        }
        else
        {
            OpcUa_SPrintfA( a_achPeerInfoBuffer,
#if OPCUA_USE_SAFE_FUNCTIONS
                            a_uiPeerInfoBufferSize,
#endif /* OPCUA_USE_SAFE_FUNCTIONS */
                            "%x:%x:%x:%x:%x:%x:%x:%x%%%u:%u",
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[0]),
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[1]),
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[2]),
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[3]),
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[4]),
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[5]),
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[6]),
                            OpcUa_P_RawSocket_NToHS(pAddr->sin6_addr.s6_words[7]),
                            pAddr->sin6_scope_id, usPort);
        }
    }
    else if(sockAddrIn.ss_family == AF_INET)
    {
        struct sockaddr_in* pAddr = (struct sockaddr_in*)&sockAddrIn;

        /* IP */
        PeerIp   = OpcUa_P_RawSocket_NToHL((OpcUa_UInt32)pAddr->sin_addr.s_addr);

        /* Port */
        usPort = OpcUa_P_RawSocket_NToHS((OpcUa_UInt16)pAddr->sin_port);

        OpcUa_SPrintfA( a_achPeerInfoBuffer,
#if OPCUA_USE_SAFE_FUNCTIONS
                        a_uiPeerInfoBufferSize,
#endif /* OPCUA_USE_SAFE_FUNCTIONS */
                        "%u.%u.%u.%u:%u",
                        (PeerIp >> 24) & 0xFF,
                        (PeerIp >> 16) & 0xFF,
                        (PeerIp >> 8) & 0xFF,
                        PeerIp & 0xFF,
                        usPort);
    }
    else
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadInternalError);
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Get IP Address and Port Number of the local connection
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_GetLocalInfo(    OpcUa_RawSocket a_RawSocket,
                                                    OpcUa_UInt16*   a_pPort)
{
    int                 apiResult     = 0;
    struct sockaddr_storage sockAddrIn;
    int                 sockAddrInLen = sizeof(sockAddrIn);
    SOCKET              winSocket     = (SOCKET)OPCUA_P_SOCKET_INVALID;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "GetLocalInfo");

    OpcUa_ReturnErrorIfTrue((a_RawSocket == (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID), OpcUa_BadInvalidArgument);
    OpcUa_ReturnErrorIfArgumentNull(a_pPort);

    winSocket = (SOCKET)a_RawSocket;

    apiResult = getsockname(winSocket, (struct sockaddr*)&sockAddrIn, &sockAddrInLen);

    OpcUa_ReturnErrorIfTrue((apiResult != 0), OpcUa_BadInternalError);

    if(sockAddrIn.ss_family == AF_INET6)
    {
        struct sockaddr_in6* pAddr = (struct sockaddr_in6*)&sockAddrIn;

        *a_pPort = OpcUa_P_RawSocket_NToHS((OpcUa_UInt16)pAddr->sin6_port);
    }
    else if(sockAddrIn.ss_family == AF_INET)
    {
        struct sockaddr_in* pAddr = (struct sockaddr_in*)&sockAddrIn;

        *a_pPort = OpcUa_P_RawSocket_NToHS((OpcUa_UInt16)pAddr->sin_port);
    }
    else
    {
        OpcUa_GotoErrorWithStatus(OpcUa_BadInternalError);
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}


/*============================================================================
 * Select usable socket. (maxfds ignored in win32)
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_Select(  OpcUa_RawSocket         a_MaxFds,
                                            OpcUa_P_Socket_Array*   a_pFdSetRead,
                                            OpcUa_P_Socket_Array*   a_pFdSetWrite,
                                            OpcUa_P_Socket_Array*   a_pFdSetException,
                                            OpcUa_UInt32            a_uTimeout)
{
    int                 apiResult  = 0;
    struct timeval      timeout;

OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_Select");

    OpcUa_ReferenceParameter(a_MaxFds);

    OpcUa_GotoErrorIfArgumentNull(a_pFdSetRead);
    OpcUa_GotoErrorIfArgumentNull(a_pFdSetWrite);
    OpcUa_GotoErrorIfArgumentNull(a_pFdSetException);

    timeout.tv_sec  = a_uTimeout / 1000;
    timeout.tv_usec = (a_uTimeout % 1000) * 1000;

    apiResult = select( 0,
                        (fd_set*)a_pFdSetRead,
                        (fd_set*)a_pFdSetWrite,
                        (fd_set*)a_pFdSetException,
                        &timeout);

    if(apiResult == OPCUA_P_SOCKET_SOCKETERROR)
    {
        uStatus   = OpcUa_BadCommunicationError;
    }

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Get last socket error.
 *===========================================================================*/
OpcUa_Int32 OpcUa_P_RawSocket_GetLastError( OpcUa_RawSocket a_RawSocket)
{
    int lastError = 0;
    OpcUa_ReferenceParameter(a_RawSocket); /* Not needed in this implementation. */

    lastError = WSAGetLastError();

    return (OpcUa_Int32)lastError;
}

/*============================================================================
* OpcUa_P_RawSocket_FD_Isset
*===========================================================================*/
OpcUa_Boolean OpcUa_P_RawSocket_FD_Isset(   OpcUa_RawSocket         a_RawSocket,
                                            OpcUa_P_Socket_Array*   a_pFdSet)
{
    SOCKET WinSocket = (SOCKET)a_RawSocket;

    if(FD_ISSET(WinSocket, (fd_set*)a_pFdSet) == TRUE)
    {
        return OpcUa_True;
    }
    else
    {
        return OpcUa_False;
    }
}

/*============================================================================
 * Create a UDP socket
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_CreateUdp(   OpcUa_RawSocket* a_pRawSocket,
                                                OpcUa_Boolean    a_bSender,
                                                OpcUa_StringA    a_LocalIpAddress,
                                                OpcUa_StringA    a_RemoteIpAddress,
                                                OpcUa_Int16      a_RemotePort,
                                                OpcUa_Byte       a_TimeToLive)
{
    SOCKET              winSocket   = (SOCKET)OPCUA_P_SOCKET_INVALID;
    int                 flag        = 1;

    OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_CreateUdp");
    OpcUa_GotoErrorIfArgumentNull(a_pRawSocket);
    OpcUa_GotoErrorIfArgumentNull(a_RemoteIpAddress);

    /* create socket through platform API */
    winSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* check if socket creation was successful */
    if(winSocket == OPCUA_P_SOCKET_INVALID)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, SOL_SOCKET,  SO_REUSEADDR, (char*)&flag, sizeof(int)))
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

    if(a_bSender)
    {
        /* A sender shall (try) to reduce receive buffer size - possible errors are ignored */
        setsockopt(winSocket, SOL_SOCKET,  SO_RCVBUF, (char*)&flag, sizeof(int));
        uStatus = OpcUa_P_RawSocket_BindEx((OpcUa_RawSocket)winSocket, a_LocalIpAddress, a_RemotePort);
        OpcUa_GotoErrorIfBad(uStatus);
        uStatus = OpcUa_P_RawSocket_Connect((OpcUa_RawSocket)winSocket, a_RemotePort, a_RemoteIpAddress);
        OpcUa_GotoErrorIfBad(uStatus);
    }
    else
    {
        uStatus = OpcUa_P_RawSocket_Bind((OpcUa_RawSocket)winSocket, a_RemotePort);
        OpcUa_GotoErrorIfBad(uStatus);
        uStatus = OpcUa_P_RawSocket_SetBlockMode((OpcUa_RawSocket)winSocket, OpcUa_False);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    if(a_bSender)
    {
        if(a_TimeToLive > 0)
        {
            flag = a_TimeToLive;
            if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&flag, sizeof(int)))
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
            }
        }
        if(a_LocalIpAddress != OpcUa_Null)
        {
            struct in_addr if_ip;
            if_ip.s_addr = inet_addr(a_LocalIpAddress);
            if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&if_ip, sizeof(if_ip)))
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
            }
        }
    }
    else
    {
        struct ip_mreq if_mr;
        if_mr.imr_multiaddr.s_addr = inet_addr(a_RemoteIpAddress);
        if_mr.imr_interface.s_addr = INADDR_ANY;
        if(a_LocalIpAddress != OpcUa_Null)
        {
            if_mr.imr_interface.s_addr = inet_addr(a_LocalIpAddress);
        }
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&if_mr, sizeof(if_mr)))
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
    }

    *a_pRawSocket = (OpcUa_RawSocket)winSocket;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    if(winSocket != OPCUA_P_SOCKET_INVALID)
    {
        OpcUa_P_RawSocket_Close((OpcUa_RawSocket)winSocket);
        *a_pRawSocket = (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID;
    }

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Create a UDPv6 socket
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_RawSocket_CreateUdpV6( OpcUa_RawSocket* a_pRawSocket,
                                                OpcUa_Boolean    a_bSender,
                                                OpcUa_StringA    a_LocalIpAddress,
                                                OpcUa_StringA    a_RemoteIpAddress,
                                                OpcUa_Int16      a_RemotePort,
                                                OpcUa_Byte       a_TimeToLive)
{
    SOCKET              winSocket   = (SOCKET)OPCUA_P_SOCKET_INVALID;
    int                 flag        = 1;
    char                *pScopeId;
    int                 apiResult;

    OpcUa_InitializeStatus(OpcUa_Module_Socket, "P_CreateUdpV6");
    OpcUa_GotoErrorIfArgumentNull(a_pRawSocket);
    OpcUa_GotoErrorIfArgumentNull(a_RemoteIpAddress);

    /* create socket through platform API */
    winSocket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    /* check if socket creation was successful */
    if(winSocket == OPCUA_P_SOCKET_INVALID)
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

    if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, SOL_SOCKET,  SO_REUSEADDR, (char*)&flag, sizeof(int)))
    {
        uStatus = OpcUa_BadCommunicationError;
        goto Error;
    }

    if(a_bSender)
    {
        /* A sender shall (try) to reduce receive buffer size - possible errors are ignored */
        setsockopt(winSocket, SOL_SOCKET,  SO_RCVBUF, (char*)&flag, sizeof(int));
        uStatus = OpcUa_P_RawSocket_BindV6((OpcUa_RawSocket)winSocket, a_LocalIpAddress, a_RemotePort);
        OpcUa_GotoErrorIfBad(uStatus);
        uStatus = OpcUa_P_RawSocket_ConnectV6((OpcUa_RawSocket)winSocket, a_RemotePort, a_RemoteIpAddress);
        OpcUa_GotoErrorIfBad(uStatus);
    }
    else
    {
        uStatus = OpcUa_P_RawSocket_BindV6((OpcUa_RawSocket)winSocket, OpcUa_Null, a_RemotePort);
        OpcUa_GotoErrorIfBad(uStatus);
        uStatus = OpcUa_P_RawSocket_SetBlockMode((OpcUa_RawSocket)winSocket, OpcUa_False);
        OpcUa_GotoErrorIfBad(uStatus);
    }

    pScopeId = strchr(a_RemoteIpAddress, '%');
    if(a_bSender)
    {
        if(a_TimeToLive > 0)
        {
            flag = a_TimeToLive - 1;
            if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, (char*)&flag, sizeof(int)))
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
            }
        }
        if(pScopeId != NULL)
        {
            flag = OpcUa_P_CharAToInt(pScopeId+1);
            if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char*)&flag, sizeof(int)))
            {
                OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
            }
        }
    }
    else
    {
        struct ipv6_mreq if_mr;
        if_mr.ipv6mr_interface = 0;
#ifdef _WIN32_WCE
        if(pScopeId != NULL)
        {
            if_mr.ipv6mr_interface = OpcUa_P_CharAToInt(pScopeId+1);
        }
        apiResult = OpcUa_SScanfA(a_RemoteIpAddress,
                                  "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
                                  &if_mr.ipv6mr_multiaddr.s6_words[0],
                                  &if_mr.ipv6mr_multiaddr.s6_words[1],
                                  &if_mr.ipv6mr_multiaddr.s6_words[2],
                                  &if_mr.ipv6mr_multiaddr.s6_words[3],
                                  &if_mr.ipv6mr_multiaddr.s6_words[4],
                                  &if_mr.ipv6mr_multiaddr.s6_words[5],
                                  &if_mr.ipv6mr_multiaddr.s6_words[6],
                                  &if_mr.ipv6mr_multiaddr.s6_words[7]);
        if(apiResult != 8)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
        while(apiResult --> 0)
        {
            if_mr.ipv6mr_multiaddr.s6_words[apiResult] = OpcUa_P_RawSocket_HToNS(if_mr.ipv6mr_multiaddr.s6_words[apiResult]);
        }
#else
        if(pScopeId != NULL)
        {
            if_mr.ipv6mr_interface = OpcUa_P_CharAToInt(pScopeId+1);
            *pScopeId = 0;
        }
        apiResult = inet_pton(AF_INET6, a_RemoteIpAddress, (void*)&if_mr.ipv6mr_multiaddr);
        if(pScopeId != NULL)
        {
            *pScopeId = '%';
        }
        if(apiResult <= 0)
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
#endif
        if(OPCUA_P_SOCKET_SOCKETERROR == setsockopt(winSocket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, (char*)&if_mr, sizeof(if_mr)))
        {
            OpcUa_GotoErrorWithStatus(OpcUa_BadCommunicationError);
        }
    }

    *a_pRawSocket = (OpcUa_RawSocket)winSocket;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    if(winSocket != OPCUA_P_SOCKET_INVALID)
    {
        OpcUa_P_RawSocket_Close((OpcUa_RawSocket)winSocket);
        *a_pRawSocket = (OpcUa_RawSocket)OPCUA_P_SOCKET_INVALID;
    }

    OpcUa_FinishErrorHandling;
}
