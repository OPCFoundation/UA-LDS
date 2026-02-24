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

#include <opcua.h>
#include <opcua_mutex.h>
#include <opcua_semaphore.h>
#include <opcua_connection.h>
#include <opcua_statuscodes.h>

/*============================================================================
 * OpcUa_Connection_Connect
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Connection_Connect(
    OpcUa_Connection*               a_pConnection,
    OpcUa_String*                   a_sUrl,
    OpcUa_ClientCredential*         a_pCredentials,
    OpcUa_UInt32                    a_uTimeout,
    OpcUa_Connection_PfnOnNotify*   a_pCallback,
    OpcUa_Void*                     a_pCallbackData)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->Connect);

    return a_pConnection->Connect(a_pConnection, a_sUrl, a_pCredentials, a_uTimeout, a_pCallback, a_pCallbackData);
}

/*============================================================================
 * OpcUa_Connection_Disconnect
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Connection_Disconnect(
    OpcUa_Connection*         a_pConnection,
    OpcUa_Boolean             a_bNotifyOnComplete)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->Disconnect);

    return a_pConnection->Disconnect(a_pConnection, a_bNotifyOnComplete);
}

/*============================================================================
 * OpcUa_Connection_BeginSendRequest
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Connection_BeginSendRequest(
    OpcUa_Connection*    a_pConnection,
    OpcUa_OutputStream** a_ppOstrm)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->BeginSendRequest);

    return a_pConnection->BeginSendRequest(a_pConnection, a_ppOstrm);
}

/*============================================================================
 * OpcUa_Connection_EndSendRequest
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Connection_EndSendRequest(
    OpcUa_Connection*               a_pConnection,
    OpcUa_OutputStream**            a_ppOstrm,
    OpcUa_UInt32                    a_uMsecTimeout,
    OpcUa_Connection_PfnOnResponse* a_pCallback,
    OpcUa_Void*                     a_pCallbackData)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->EndSendRequest);

    return a_pConnection->EndSendRequest(a_pConnection, a_ppOstrm, a_uMsecTimeout, a_pCallback, a_pCallbackData);
}

/*============================================================================
 * OpcUa_Connection_AbortSendRequest
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Connection_AbortSendRequest(
    OpcUa_Connection*           a_pConnection,
    OpcUa_StatusCode            a_uStatus,
    OpcUa_String*               a_psReason,
    OpcUa_OutputStream**        a_ppOstrm)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->AbortSendRequest);

    return a_pConnection->AbortSendRequest(a_pConnection, a_uStatus, a_psReason, a_ppOstrm);
}

/*============================================================================
 * OpcUa_Connection_AbortSendRequest
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Connection_GetReceiveBufferSize(
    OpcUa_Connection*         a_pConnection,
    OpcUa_UInt32*             a_pBufferSize)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->GetReceiveBufferSize);

    return a_pConnection->GetReceiveBufferSize(a_pConnection, a_pBufferSize);
}

/*============================================================================
 * OpcUa_Connection_Delete
 *===========================================================================*/
OpcUa_Void OpcUa_Connection_Delete(OpcUa_Connection** a_ppConnection)
{
    if (a_ppConnection != OpcUa_Null && *a_ppConnection != OpcUa_Null)
    {
        (*a_ppConnection)->Delete(a_ppConnection);
    }
}

/*============================================================================
 * OpcUa_Connection_AddToSendQueue
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Connection_AddToSendQueue(
    OpcUa_Connection*         a_pConnection,
    OpcUa_BufferList*         a_pBufferList,
    OpcUa_UInt32              a_uFlags)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->AddToSendQueue);

    return a_pConnection->AddToSendQueue(a_pConnection, a_pBufferList, a_uFlags);
}

/*============================================================================
 * OpcUa_Connection_CheckProtocolVersion
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_Connection_CheckProtocolVersion(
    OpcUa_Connection*         a_pConnection,
    OpcUa_UInt32              a_uProtocolVersion)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_Connection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection);
    OpcUa_ReturnErrorIfArgumentNull(a_pConnection->CheckProtocolVersion);

    return a_pConnection->CheckProtocolVersion(a_pConnection, a_uProtocolVersion);
}
