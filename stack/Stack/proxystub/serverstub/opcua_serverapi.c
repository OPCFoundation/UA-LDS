/* ========================================================================
 * Copyright (c) 2005-2021 The OPC Foundation, Inc. All rights reserved.
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

/* base */
#include <opcua.h>

#ifdef OPCUA_HAVE_SERVERAPI

/* types */
#include <opcua_types.h>
#include <opcua_builtintypes.h>
#include <opcua_extensionobject.h>
#include <opcua_encodeableobject.h>
#include <opcua_identifiers.h>

/* server related */
#include <opcua_endpoint.h>
#include <opcua_servicetable.h>
#include <opcua_serverapi.h>

#ifndef OPCUA_EXCLUDE_FindServers
/*============================================================================
 * A pointer to a function that implements the FindServers service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnFindServers)(
    OpcUa_Endpoint                 hEndpoint,
    OpcUa_Handle                   hContext,
    const OpcUa_RequestHeader*     pRequestHeader,
    const OpcUa_String*            pEndpointUrl,
    OpcUa_Int32                    nNoOfLocaleIds,
    const OpcUa_String*            pLocaleIds,
    OpcUa_Int32                    nNoOfServerUris,
    const OpcUa_String*            pServerUris,
    OpcUa_ResponseHeader*          pResponseHeader,
    OpcUa_Int32*                   pNoOfServers,
    OpcUa_ApplicationDescription** pServers);

/*============================================================================
 * A stub method which implements the FindServers service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_FindServers(
    OpcUa_Endpoint                 a_hEndpoint,
    OpcUa_Handle                   a_hContext,
    const OpcUa_RequestHeader*     a_pRequestHeader,
    const OpcUa_String*            a_pEndpointUrl,
    OpcUa_Int32                    a_nNoOfLocaleIds,
    const OpcUa_String*            a_pLocaleIds,
    OpcUa_Int32                    a_nNoOfServerUris,
    const OpcUa_String*            a_pServerUris,
    OpcUa_ResponseHeader*          a_pResponseHeader,
    OpcUa_Int32*                   a_pNoOfServers,
    OpcUa_ApplicationDescription** a_pServers)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_FindServers");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pEndpointUrl);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfLocaleIds, a_pLocaleIds);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfServerUris, a_pServerUris);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfServers);
    OpcUa_ReturnErrorIfArgumentNull(a_pServers);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a FindServers service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginFindServers(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_FindServersRequest* pRequest = OpcUa_Null;
    OpcUa_FindServersResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnFindServers* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginFindServers");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_FindServersRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_FindServersRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        &pRequest->EndpointUrl,
        pRequest->NoOfLocaleIds,
        pRequest->LocaleIds,
        pRequest->NoOfServerUris,
        pRequest->ServerUris,
        &pResponse->ResponseHeader,
        &pResponse->NoOfServers,
        &pResponse->Servers);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_FindServersResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information FindServers service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_FindServers_ServiceType =
{
    OpcUaId_FindServersRequest,
    &OpcUa_FindServersResponse_EncodeableType,
    OpcUa_Server_BeginFindServers,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_FindServers
};
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetwork
/*============================================================================
 * A pointer to a function that implements the FindServersOnNetwork service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnFindServersOnNetwork)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_UInt32               nStartingRecordId,
    OpcUa_UInt32               nMaxRecordsToReturn,
    OpcUa_Int32                nNoOfServerCapabilityFilter,
    const OpcUa_String*        pServerCapabilityFilter,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_DateTime*            pLastCounterResetTime,
    OpcUa_Int32*               pNoOfServers,
    OpcUa_ServerOnNetwork**    pServers);

/*============================================================================
 * A stub method which implements the FindServersOnNetwork service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_FindServersOnNetwork(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_UInt32               a_nStartingRecordId,
    OpcUa_UInt32               a_nMaxRecordsToReturn,
    OpcUa_Int32                a_nNoOfServerCapabilityFilter,
    const OpcUa_String*        a_pServerCapabilityFilter,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_DateTime*            a_pLastCounterResetTime,
    OpcUa_Int32*               a_pNoOfServers,
    OpcUa_ServerOnNetwork**    a_pServers)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_FindServersOnNetwork");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nStartingRecordId);
    OpcUa_ReferenceParameter(a_nMaxRecordsToReturn);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfServerCapabilityFilter, a_pServerCapabilityFilter);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pLastCounterResetTime);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfServers);
    OpcUa_ReturnErrorIfArgumentNull(a_pServers);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a FindServersOnNetwork service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginFindServersOnNetwork(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_FindServersOnNetworkRequest* pRequest = OpcUa_Null;
    OpcUa_FindServersOnNetworkResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnFindServersOnNetwork* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginFindServersOnNetwork");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_FindServersOnNetworkRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_FindServersOnNetworkRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->StartingRecordId,
        pRequest->MaxRecordsToReturn,
        pRequest->NoOfServerCapabilityFilter,
        pRequest->ServerCapabilityFilter,
        &pResponse->ResponseHeader,
        &pResponse->LastCounterResetTime,
        &pResponse->NoOfServers,
        &pResponse->Servers);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_FindServersOnNetworkResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information FindServersOnNetwork service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_FindServersOnNetwork_ServiceType =
{
    OpcUaId_FindServersOnNetworkRequest,
    &OpcUa_FindServersOnNetworkResponse_EncodeableType,
    OpcUa_Server_BeginFindServersOnNetwork,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_FindServersOnNetwork
};
#endif

#ifndef OPCUA_EXCLUDE_GetEndpoints
/*============================================================================
 * A pointer to a function that implements the GetEndpoints service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnGetEndpoints)(
    OpcUa_Endpoint              hEndpoint,
    OpcUa_Handle                hContext,
    const OpcUa_RequestHeader*  pRequestHeader,
    const OpcUa_String*         pEndpointUrl,
    OpcUa_Int32                 nNoOfLocaleIds,
    const OpcUa_String*         pLocaleIds,
    OpcUa_Int32                 nNoOfProfileUris,
    const OpcUa_String*         pProfileUris,
    OpcUa_ResponseHeader*       pResponseHeader,
    OpcUa_Int32*                pNoOfEndpoints,
    OpcUa_EndpointDescription** pEndpoints);

/*============================================================================
 * A stub method which implements the GetEndpoints service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_GetEndpoints(
    OpcUa_Endpoint              a_hEndpoint,
    OpcUa_Handle                a_hContext,
    const OpcUa_RequestHeader*  a_pRequestHeader,
    const OpcUa_String*         a_pEndpointUrl,
    OpcUa_Int32                 a_nNoOfLocaleIds,
    const OpcUa_String*         a_pLocaleIds,
    OpcUa_Int32                 a_nNoOfProfileUris,
    const OpcUa_String*         a_pProfileUris,
    OpcUa_ResponseHeader*       a_pResponseHeader,
    OpcUa_Int32*                a_pNoOfEndpoints,
    OpcUa_EndpointDescription** a_pEndpoints)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_GetEndpoints");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pEndpointUrl);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfLocaleIds, a_pLocaleIds);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfProfileUris, a_pProfileUris);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfEndpoints);
    OpcUa_ReturnErrorIfArgumentNull(a_pEndpoints);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a GetEndpoints service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginGetEndpoints(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_GetEndpointsRequest* pRequest = OpcUa_Null;
    OpcUa_GetEndpointsResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnGetEndpoints* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginGetEndpoints");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_GetEndpointsRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_GetEndpointsRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        &pRequest->EndpointUrl,
        pRequest->NoOfLocaleIds,
        pRequest->LocaleIds,
        pRequest->NoOfProfileUris,
        pRequest->ProfileUris,
        &pResponse->ResponseHeader,
        &pResponse->NoOfEndpoints,
        &pResponse->Endpoints);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_GetEndpointsResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information GetEndpoints service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_GetEndpoints_ServiceType =
{
    OpcUaId_GetEndpointsRequest,
    &OpcUa_GetEndpointsResponse_EncodeableType,
    OpcUa_Server_BeginGetEndpoints,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_GetEndpoints
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer
/*============================================================================
 * A pointer to a function that implements the RegisterServer service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnRegisterServer)(
    OpcUa_Endpoint                hEndpoint,
    OpcUa_Handle                  hContext,
    const OpcUa_RequestHeader*    pRequestHeader,
    const OpcUa_RegisteredServer* pServer,
    OpcUa_ResponseHeader*         pResponseHeader);

/*============================================================================
 * A stub method which implements the RegisterServer service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_RegisterServer(
    OpcUa_Endpoint                a_hEndpoint,
    OpcUa_Handle                  a_hContext,
    const OpcUa_RequestHeader*    a_pRequestHeader,
    const OpcUa_RegisteredServer* a_pServer,
    OpcUa_ResponseHeader*         a_pResponseHeader)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_RegisterServer");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pServer);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a RegisterServer service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginRegisterServer(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_RegisterServerRequest* pRequest = OpcUa_Null;
    OpcUa_RegisterServerResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnRegisterServer* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginRegisterServer");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_RegisterServerRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_RegisterServerRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        &pRequest->Server,
        &pResponse->ResponseHeader);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_RegisterServerResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information RegisterServer service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_RegisterServer_ServiceType =
{
    OpcUaId_RegisterServerRequest,
    &OpcUa_RegisterServerResponse_EncodeableType,
    OpcUa_Server_BeginRegisterServer,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_RegisterServer
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2
/*============================================================================
 * A pointer to a function that implements the RegisterServer2 service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnRegisterServer2)(
    OpcUa_Endpoint                hEndpoint,
    OpcUa_Handle                  hContext,
    const OpcUa_RequestHeader*    pRequestHeader,
    const OpcUa_RegisteredServer* pServer,
    OpcUa_Int32                   nNoOfDiscoveryConfiguration,
    const OpcUa_ExtensionObject*  pDiscoveryConfiguration,
    OpcUa_ResponseHeader*         pResponseHeader,
    OpcUa_Int32*                  pNoOfConfigurationResults,
    OpcUa_StatusCode**            pConfigurationResults,
    OpcUa_Int32*                  pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**        pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the RegisterServer2 service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_RegisterServer2(
    OpcUa_Endpoint                a_hEndpoint,
    OpcUa_Handle                  a_hContext,
    const OpcUa_RequestHeader*    a_pRequestHeader,
    const OpcUa_RegisteredServer* a_pServer,
    OpcUa_Int32                   a_nNoOfDiscoveryConfiguration,
    const OpcUa_ExtensionObject*  a_pDiscoveryConfiguration,
    OpcUa_ResponseHeader*         a_pResponseHeader,
    OpcUa_Int32*                  a_pNoOfConfigurationResults,
    OpcUa_StatusCode**            a_pConfigurationResults,
    OpcUa_Int32*                  a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**        a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_RegisterServer2");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pServer);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfDiscoveryConfiguration, a_pDiscoveryConfiguration);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfConfigurationResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pConfigurationResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a RegisterServer2 service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginRegisterServer2(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_RegisterServer2Request* pRequest = OpcUa_Null;
    OpcUa_RegisterServer2Response* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnRegisterServer2* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginRegisterServer2");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_RegisterServer2Request, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_RegisterServer2Request*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        &pRequest->Server,
        pRequest->NoOfDiscoveryConfiguration,
        pRequest->DiscoveryConfiguration,
        &pResponse->ResponseHeader,
        &pResponse->NoOfConfigurationResults,
        &pResponse->ConfigurationResults,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_RegisterServer2Response*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information RegisterServer2 service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_RegisterServer2_ServiceType =
{
    OpcUaId_RegisterServer2Request,
    &OpcUa_RegisterServer2Response_EncodeableType,
    OpcUa_Server_BeginRegisterServer2,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_RegisterServer2
};
#endif

#ifndef OPCUA_EXCLUDE_Write
/*============================================================================
 * A pointer to a function that implements the Write service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnWrite)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_Int32                nNoOfNodesToWrite,
    const OpcUa_WriteValue*    pNodesToWrite,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Int32*               pNoOfResults,
    OpcUa_StatusCode**         pResults,
    OpcUa_Int32*               pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the Write service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_Write(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_Int32                a_nNoOfNodesToWrite,
    const OpcUa_WriteValue*    a_pNodesToWrite,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfResults,
    OpcUa_StatusCode**         a_pResults,
    OpcUa_Int32*               a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_Write");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfNodesToWrite, a_pNodesToWrite);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a Write service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginWrite(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_WriteRequest* pRequest = OpcUa_Null;
    OpcUa_WriteResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnWrite* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginWrite");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_WriteRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_WriteRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->NoOfNodesToWrite,
        pRequest->NodesToWrite,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_WriteResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information Write service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_Write_ServiceType =
{
    OpcUaId_WriteRequest,
    &OpcUa_WriteResponse_EncodeableType,
    OpcUa_Server_BeginWrite,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_Write
};
#endif

#ifndef OPCUA_EXCLUDE_HistoryUpdate
/*============================================================================
 * A pointer to a function that implements the HistoryUpdate service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnHistoryUpdate)(
    OpcUa_Endpoint               hEndpoint,
    OpcUa_Handle                 hContext,
    const OpcUa_RequestHeader*   pRequestHeader,
    OpcUa_Int32                  nNoOfHistoryUpdateDetails,
    const OpcUa_ExtensionObject* pHistoryUpdateDetails,
    OpcUa_ResponseHeader*        pResponseHeader,
    OpcUa_Int32*                 pNoOfResults,
    OpcUa_HistoryUpdateResult**  pResults,
    OpcUa_Int32*                 pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**       pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the HistoryUpdate service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_HistoryUpdate(
    OpcUa_Endpoint               a_hEndpoint,
    OpcUa_Handle                 a_hContext,
    const OpcUa_RequestHeader*   a_pRequestHeader,
    OpcUa_Int32                  a_nNoOfHistoryUpdateDetails,
    const OpcUa_ExtensionObject* a_pHistoryUpdateDetails,
    OpcUa_ResponseHeader*        a_pResponseHeader,
    OpcUa_Int32*                 a_pNoOfResults,
    OpcUa_HistoryUpdateResult**  a_pResults,
    OpcUa_Int32*                 a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**       a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_HistoryUpdate");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfHistoryUpdateDetails, a_pHistoryUpdateDetails);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a HistoryUpdate service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginHistoryUpdate(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_HistoryUpdateRequest* pRequest = OpcUa_Null;
    OpcUa_HistoryUpdateResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnHistoryUpdate* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginHistoryUpdate");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_HistoryUpdateRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_HistoryUpdateRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->NoOfHistoryUpdateDetails,
        pRequest->HistoryUpdateDetails,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_HistoryUpdateResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information HistoryUpdate service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_HistoryUpdate_ServiceType =
{
    OpcUaId_HistoryUpdateRequest,
    &OpcUa_HistoryUpdateResponse_EncodeableType,
    OpcUa_Server_BeginHistoryUpdate,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_HistoryUpdate
};
#endif

#ifndef OPCUA_EXCLUDE_Call
/*============================================================================
 * A pointer to a function that implements the Call service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnCall)(
    OpcUa_Endpoint                 hEndpoint,
    OpcUa_Handle                   hContext,
    const OpcUa_RequestHeader*     pRequestHeader,
    OpcUa_Int32                    nNoOfMethodsToCall,
    const OpcUa_CallMethodRequest* pMethodsToCall,
    OpcUa_ResponseHeader*          pResponseHeader,
    OpcUa_Int32*                   pNoOfResults,
    OpcUa_CallMethodResult**       pResults,
    OpcUa_Int32*                   pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**         pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the Call service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_Call(
    OpcUa_Endpoint                 a_hEndpoint,
    OpcUa_Handle                   a_hContext,
    const OpcUa_RequestHeader*     a_pRequestHeader,
    OpcUa_Int32                    a_nNoOfMethodsToCall,
    const OpcUa_CallMethodRequest* a_pMethodsToCall,
    OpcUa_ResponseHeader*          a_pResponseHeader,
    OpcUa_Int32*                   a_pNoOfResults,
    OpcUa_CallMethodResult**       a_pResults,
    OpcUa_Int32*                   a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**         a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_Call");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfMethodsToCall, a_pMethodsToCall);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a Call service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginCall(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_CallRequest* pRequest = OpcUa_Null;
    OpcUa_CallResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnCall* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginCall");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_CallRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_CallRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->NoOfMethodsToCall,
        pRequest->MethodsToCall,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_CallResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information Call service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_Call_ServiceType =
{
    OpcUaId_CallRequest,
    &OpcUa_CallResponse_EncodeableType,
    OpcUa_Server_BeginCall,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_Call
};
#endif

#ifndef OPCUA_EXCLUDE_CreateMonitoredItems
/*============================================================================
 * A pointer to a function that implements the CreateMonitoredItems service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnCreateMonitoredItems)(
    OpcUa_Endpoint                          hEndpoint,
    OpcUa_Handle                            hContext,
    const OpcUa_RequestHeader*              pRequestHeader,
    OpcUa_UInt32                            nSubscriptionId,
    OpcUa_TimestampsToReturn                eTimestampsToReturn,
    OpcUa_Int32                             nNoOfItemsToCreate,
    const OpcUa_MonitoredItemCreateRequest* pItemsToCreate,
    OpcUa_ResponseHeader*                   pResponseHeader,
    OpcUa_Int32*                            pNoOfResults,
    OpcUa_MonitoredItemCreateResult**       pResults,
    OpcUa_Int32*                            pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**                  pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the CreateMonitoredItems service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_CreateMonitoredItems(
    OpcUa_Endpoint                          a_hEndpoint,
    OpcUa_Handle                            a_hContext,
    const OpcUa_RequestHeader*              a_pRequestHeader,
    OpcUa_UInt32                            a_nSubscriptionId,
    OpcUa_TimestampsToReturn                a_eTimestampsToReturn,
    OpcUa_Int32                             a_nNoOfItemsToCreate,
    const OpcUa_MonitoredItemCreateRequest* a_pItemsToCreate,
    OpcUa_ResponseHeader*                   a_pResponseHeader,
    OpcUa_Int32*                            a_pNoOfResults,
    OpcUa_MonitoredItemCreateResult**       a_pResults,
    OpcUa_Int32*                            a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**                  a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_CreateMonitoredItems");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nSubscriptionId);
    OpcUa_ReferenceParameter(a_eTimestampsToReturn);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfItemsToCreate, a_pItemsToCreate);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a CreateMonitoredItems service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginCreateMonitoredItems(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_CreateMonitoredItemsRequest* pRequest = OpcUa_Null;
    OpcUa_CreateMonitoredItemsResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnCreateMonitoredItems* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginCreateMonitoredItems");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_CreateMonitoredItemsRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_CreateMonitoredItemsRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->SubscriptionId,
        pRequest->TimestampsToReturn,
        pRequest->NoOfItemsToCreate,
        pRequest->ItemsToCreate,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_CreateMonitoredItemsResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information CreateMonitoredItems service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_CreateMonitoredItems_ServiceType =
{
    OpcUaId_CreateMonitoredItemsRequest,
    &OpcUa_CreateMonitoredItemsResponse_EncodeableType,
    OpcUa_Server_BeginCreateMonitoredItems,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_CreateMonitoredItems
};
#endif

#ifndef OPCUA_EXCLUDE_ModifyMonitoredItems
/*============================================================================
 * A pointer to a function that implements the ModifyMonitoredItems service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnModifyMonitoredItems)(
    OpcUa_Endpoint                          hEndpoint,
    OpcUa_Handle                            hContext,
    const OpcUa_RequestHeader*              pRequestHeader,
    OpcUa_UInt32                            nSubscriptionId,
    OpcUa_TimestampsToReturn                eTimestampsToReturn,
    OpcUa_Int32                             nNoOfItemsToModify,
    const OpcUa_MonitoredItemModifyRequest* pItemsToModify,
    OpcUa_ResponseHeader*                   pResponseHeader,
    OpcUa_Int32*                            pNoOfResults,
    OpcUa_MonitoredItemModifyResult**       pResults,
    OpcUa_Int32*                            pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**                  pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the ModifyMonitoredItems service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_ModifyMonitoredItems(
    OpcUa_Endpoint                          a_hEndpoint,
    OpcUa_Handle                            a_hContext,
    const OpcUa_RequestHeader*              a_pRequestHeader,
    OpcUa_UInt32                            a_nSubscriptionId,
    OpcUa_TimestampsToReturn                a_eTimestampsToReturn,
    OpcUa_Int32                             a_nNoOfItemsToModify,
    const OpcUa_MonitoredItemModifyRequest* a_pItemsToModify,
    OpcUa_ResponseHeader*                   a_pResponseHeader,
    OpcUa_Int32*                            a_pNoOfResults,
    OpcUa_MonitoredItemModifyResult**       a_pResults,
    OpcUa_Int32*                            a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**                  a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_ModifyMonitoredItems");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nSubscriptionId);
    OpcUa_ReferenceParameter(a_eTimestampsToReturn);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfItemsToModify, a_pItemsToModify);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a ModifyMonitoredItems service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginModifyMonitoredItems(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_ModifyMonitoredItemsRequest* pRequest = OpcUa_Null;
    OpcUa_ModifyMonitoredItemsResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnModifyMonitoredItems* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginModifyMonitoredItems");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_ModifyMonitoredItemsRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_ModifyMonitoredItemsRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->SubscriptionId,
        pRequest->TimestampsToReturn,
        pRequest->NoOfItemsToModify,
        pRequest->ItemsToModify,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_ModifyMonitoredItemsResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information ModifyMonitoredItems service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_ModifyMonitoredItems_ServiceType =
{
    OpcUaId_ModifyMonitoredItemsRequest,
    &OpcUa_ModifyMonitoredItemsResponse_EncodeableType,
    OpcUa_Server_BeginModifyMonitoredItems,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_ModifyMonitoredItems
};
#endif

#ifndef OPCUA_EXCLUDE_SetMonitoringMode
/*============================================================================
 * A pointer to a function that implements the SetMonitoringMode service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnSetMonitoringMode)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_UInt32               nSubscriptionId,
    OpcUa_MonitoringMode       eMonitoringMode,
    OpcUa_Int32                nNoOfMonitoredItemIds,
    const OpcUa_UInt32*        pMonitoredItemIds,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Int32*               pNoOfResults,
    OpcUa_StatusCode**         pResults,
    OpcUa_Int32*               pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the SetMonitoringMode service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_SetMonitoringMode(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_UInt32               a_nSubscriptionId,
    OpcUa_MonitoringMode       a_eMonitoringMode,
    OpcUa_Int32                a_nNoOfMonitoredItemIds,
    const OpcUa_UInt32*        a_pMonitoredItemIds,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfResults,
    OpcUa_StatusCode**         a_pResults,
    OpcUa_Int32*               a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_SetMonitoringMode");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nSubscriptionId);
    OpcUa_ReferenceParameter(a_eMonitoringMode);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfMonitoredItemIds, a_pMonitoredItemIds);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a SetMonitoringMode service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginSetMonitoringMode(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_SetMonitoringModeRequest* pRequest = OpcUa_Null;
    OpcUa_SetMonitoringModeResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnSetMonitoringMode* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginSetMonitoringMode");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_SetMonitoringModeRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_SetMonitoringModeRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->SubscriptionId,
        pRequest->MonitoringMode,
        pRequest->NoOfMonitoredItemIds,
        pRequest->MonitoredItemIds,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_SetMonitoringModeResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information SetMonitoringMode service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_SetMonitoringMode_ServiceType =
{
    OpcUaId_SetMonitoringModeRequest,
    &OpcUa_SetMonitoringModeResponse_EncodeableType,
    OpcUa_Server_BeginSetMonitoringMode,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_SetMonitoringMode
};
#endif

#ifndef OPCUA_EXCLUDE_SetTriggering
/*============================================================================
 * A pointer to a function that implements the SetTriggering service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnSetTriggering)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_UInt32               nSubscriptionId,
    OpcUa_UInt32               nTriggeringItemId,
    OpcUa_Int32                nNoOfLinksToAdd,
    const OpcUa_UInt32*        pLinksToAdd,
    OpcUa_Int32                nNoOfLinksToRemove,
    const OpcUa_UInt32*        pLinksToRemove,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Int32*               pNoOfAddResults,
    OpcUa_StatusCode**         pAddResults,
    OpcUa_Int32*               pNoOfAddDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pAddDiagnosticInfos,
    OpcUa_Int32*               pNoOfRemoveResults,
    OpcUa_StatusCode**         pRemoveResults,
    OpcUa_Int32*               pNoOfRemoveDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pRemoveDiagnosticInfos);

/*============================================================================
 * A stub method which implements the SetTriggering service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_SetTriggering(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_UInt32               a_nSubscriptionId,
    OpcUa_UInt32               a_nTriggeringItemId,
    OpcUa_Int32                a_nNoOfLinksToAdd,
    const OpcUa_UInt32*        a_pLinksToAdd,
    OpcUa_Int32                a_nNoOfLinksToRemove,
    const OpcUa_UInt32*        a_pLinksToRemove,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfAddResults,
    OpcUa_StatusCode**         a_pAddResults,
    OpcUa_Int32*               a_pNoOfAddDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pAddDiagnosticInfos,
    OpcUa_Int32*               a_pNoOfRemoveResults,
    OpcUa_StatusCode**         a_pRemoveResults,
    OpcUa_Int32*               a_pNoOfRemoveDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pRemoveDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_SetTriggering");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nSubscriptionId);
    OpcUa_ReferenceParameter(a_nTriggeringItemId);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfLinksToAdd, a_pLinksToAdd);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfLinksToRemove, a_pLinksToRemove);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfAddResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pAddResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfAddDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pAddDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfRemoveResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pRemoveResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfRemoveDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pRemoveDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a SetTriggering service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginSetTriggering(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_SetTriggeringRequest* pRequest = OpcUa_Null;
    OpcUa_SetTriggeringResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnSetTriggering* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginSetTriggering");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_SetTriggeringRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_SetTriggeringRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->SubscriptionId,
        pRequest->TriggeringItemId,
        pRequest->NoOfLinksToAdd,
        pRequest->LinksToAdd,
        pRequest->NoOfLinksToRemove,
        pRequest->LinksToRemove,
        &pResponse->ResponseHeader,
        &pResponse->NoOfAddResults,
        &pResponse->AddResults,
        &pResponse->NoOfAddDiagnosticInfos,
        &pResponse->AddDiagnosticInfos,
        &pResponse->NoOfRemoveResults,
        &pResponse->RemoveResults,
        &pResponse->NoOfRemoveDiagnosticInfos,
        &pResponse->RemoveDiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_SetTriggeringResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information SetTriggering service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_SetTriggering_ServiceType =
{
    OpcUaId_SetTriggeringRequest,
    &OpcUa_SetTriggeringResponse_EncodeableType,
    OpcUa_Server_BeginSetTriggering,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_SetTriggering
};
#endif

#ifndef OPCUA_EXCLUDE_DeleteMonitoredItems
/*============================================================================
 * A pointer to a function that implements the DeleteMonitoredItems service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnDeleteMonitoredItems)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_UInt32               nSubscriptionId,
    OpcUa_Int32                nNoOfMonitoredItemIds,
    const OpcUa_UInt32*        pMonitoredItemIds,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Int32*               pNoOfResults,
    OpcUa_StatusCode**         pResults,
    OpcUa_Int32*               pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the DeleteMonitoredItems service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_DeleteMonitoredItems(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_UInt32               a_nSubscriptionId,
    OpcUa_Int32                a_nNoOfMonitoredItemIds,
    const OpcUa_UInt32*        a_pMonitoredItemIds,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfResults,
    OpcUa_StatusCode**         a_pResults,
    OpcUa_Int32*               a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_DeleteMonitoredItems");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nSubscriptionId);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfMonitoredItemIds, a_pMonitoredItemIds);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a DeleteMonitoredItems service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginDeleteMonitoredItems(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_DeleteMonitoredItemsRequest* pRequest = OpcUa_Null;
    OpcUa_DeleteMonitoredItemsResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnDeleteMonitoredItems* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginDeleteMonitoredItems");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_DeleteMonitoredItemsRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_DeleteMonitoredItemsRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->SubscriptionId,
        pRequest->NoOfMonitoredItemIds,
        pRequest->MonitoredItemIds,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_DeleteMonitoredItemsResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information DeleteMonitoredItems service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_DeleteMonitoredItems_ServiceType =
{
    OpcUaId_DeleteMonitoredItemsRequest,
    &OpcUa_DeleteMonitoredItemsResponse_EncodeableType,
    OpcUa_Server_BeginDeleteMonitoredItems,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_DeleteMonitoredItems
};
#endif

#ifndef OPCUA_EXCLUDE_CreateSubscription
/*============================================================================
 * A pointer to a function that implements the CreateSubscription service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnCreateSubscription)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_Double               nRequestedPublishingInterval,
    OpcUa_UInt32               nRequestedLifetimeCount,
    OpcUa_UInt32               nRequestedMaxKeepAliveCount,
    OpcUa_UInt32               nMaxNotificationsPerPublish,
    OpcUa_Boolean              bPublishingEnabled,
    OpcUa_Byte                 nPriority,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_UInt32*              pSubscriptionId,
    OpcUa_Double*              pRevisedPublishingInterval,
    OpcUa_UInt32*              pRevisedLifetimeCount,
    OpcUa_UInt32*              pRevisedMaxKeepAliveCount);

/*============================================================================
 * A stub method which implements the CreateSubscription service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_CreateSubscription(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_Double               a_nRequestedPublishingInterval,
    OpcUa_UInt32               a_nRequestedLifetimeCount,
    OpcUa_UInt32               a_nRequestedMaxKeepAliveCount,
    OpcUa_UInt32               a_nMaxNotificationsPerPublish,
    OpcUa_Boolean              a_bPublishingEnabled,
    OpcUa_Byte                 a_nPriority,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_UInt32*              a_pSubscriptionId,
    OpcUa_Double*              a_pRevisedPublishingInterval,
    OpcUa_UInt32*              a_pRevisedLifetimeCount,
    OpcUa_UInt32*              a_pRevisedMaxKeepAliveCount)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_CreateSubscription");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nRequestedPublishingInterval);
    OpcUa_ReferenceParameter(a_nRequestedLifetimeCount);
    OpcUa_ReferenceParameter(a_nRequestedMaxKeepAliveCount);
    OpcUa_ReferenceParameter(a_nMaxNotificationsPerPublish);
    OpcUa_ReferenceParameter(a_bPublishingEnabled);
    OpcUa_ReferenceParameter(a_nPriority);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pSubscriptionId);
    OpcUa_ReturnErrorIfArgumentNull(a_pRevisedPublishingInterval);
    OpcUa_ReturnErrorIfArgumentNull(a_pRevisedLifetimeCount);
    OpcUa_ReturnErrorIfArgumentNull(a_pRevisedMaxKeepAliveCount);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a CreateSubscription service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginCreateSubscription(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_CreateSubscriptionRequest* pRequest = OpcUa_Null;
    OpcUa_CreateSubscriptionResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnCreateSubscription* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginCreateSubscription");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_CreateSubscriptionRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_CreateSubscriptionRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->RequestedPublishingInterval,
        pRequest->RequestedLifetimeCount,
        pRequest->RequestedMaxKeepAliveCount,
        pRequest->MaxNotificationsPerPublish,
        pRequest->PublishingEnabled,
        pRequest->Priority,
        &pResponse->ResponseHeader,
        &pResponse->SubscriptionId,
        &pResponse->RevisedPublishingInterval,
        &pResponse->RevisedLifetimeCount,
        &pResponse->RevisedMaxKeepAliveCount);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_CreateSubscriptionResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information CreateSubscription service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_CreateSubscription_ServiceType =
{
    OpcUaId_CreateSubscriptionRequest,
    &OpcUa_CreateSubscriptionResponse_EncodeableType,
    OpcUa_Server_BeginCreateSubscription,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_CreateSubscription
};
#endif

#ifndef OPCUA_EXCLUDE_ModifySubscription
/*============================================================================
 * A pointer to a function that implements the ModifySubscription service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnModifySubscription)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_UInt32               nSubscriptionId,
    OpcUa_Double               nRequestedPublishingInterval,
    OpcUa_UInt32               nRequestedLifetimeCount,
    OpcUa_UInt32               nRequestedMaxKeepAliveCount,
    OpcUa_UInt32               nMaxNotificationsPerPublish,
    OpcUa_Byte                 nPriority,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Double*              pRevisedPublishingInterval,
    OpcUa_UInt32*              pRevisedLifetimeCount,
    OpcUa_UInt32*              pRevisedMaxKeepAliveCount);

/*============================================================================
 * A stub method which implements the ModifySubscription service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_ModifySubscription(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_UInt32               a_nSubscriptionId,
    OpcUa_Double               a_nRequestedPublishingInterval,
    OpcUa_UInt32               a_nRequestedLifetimeCount,
    OpcUa_UInt32               a_nRequestedMaxKeepAliveCount,
    OpcUa_UInt32               a_nMaxNotificationsPerPublish,
    OpcUa_Byte                 a_nPriority,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Double*              a_pRevisedPublishingInterval,
    OpcUa_UInt32*              a_pRevisedLifetimeCount,
    OpcUa_UInt32*              a_pRevisedMaxKeepAliveCount)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_ModifySubscription");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nSubscriptionId);
    OpcUa_ReferenceParameter(a_nRequestedPublishingInterval);
    OpcUa_ReferenceParameter(a_nRequestedLifetimeCount);
    OpcUa_ReferenceParameter(a_nRequestedMaxKeepAliveCount);
    OpcUa_ReferenceParameter(a_nMaxNotificationsPerPublish);
    OpcUa_ReferenceParameter(a_nPriority);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pRevisedPublishingInterval);
    OpcUa_ReturnErrorIfArgumentNull(a_pRevisedLifetimeCount);
    OpcUa_ReturnErrorIfArgumentNull(a_pRevisedMaxKeepAliveCount);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a ModifySubscription service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginModifySubscription(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_ModifySubscriptionRequest* pRequest = OpcUa_Null;
    OpcUa_ModifySubscriptionResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnModifySubscription* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginModifySubscription");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_ModifySubscriptionRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_ModifySubscriptionRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->SubscriptionId,
        pRequest->RequestedPublishingInterval,
        pRequest->RequestedLifetimeCount,
        pRequest->RequestedMaxKeepAliveCount,
        pRequest->MaxNotificationsPerPublish,
        pRequest->Priority,
        &pResponse->ResponseHeader,
        &pResponse->RevisedPublishingInterval,
        &pResponse->RevisedLifetimeCount,
        &pResponse->RevisedMaxKeepAliveCount);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_ModifySubscriptionResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information ModifySubscription service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_ModifySubscription_ServiceType =
{
    OpcUaId_ModifySubscriptionRequest,
    &OpcUa_ModifySubscriptionResponse_EncodeableType,
    OpcUa_Server_BeginModifySubscription,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_ModifySubscription
};
#endif

#ifndef OPCUA_EXCLUDE_SetPublishingMode
/*============================================================================
 * A pointer to a function that implements the SetPublishingMode service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnSetPublishingMode)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_Boolean              bPublishingEnabled,
    OpcUa_Int32                nNoOfSubscriptionIds,
    const OpcUa_UInt32*        pSubscriptionIds,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Int32*               pNoOfResults,
    OpcUa_StatusCode**         pResults,
    OpcUa_Int32*               pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the SetPublishingMode service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_SetPublishingMode(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_Boolean              a_bPublishingEnabled,
    OpcUa_Int32                a_nNoOfSubscriptionIds,
    const OpcUa_UInt32*        a_pSubscriptionIds,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfResults,
    OpcUa_StatusCode**         a_pResults,
    OpcUa_Int32*               a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_SetPublishingMode");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_bPublishingEnabled);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfSubscriptionIds, a_pSubscriptionIds);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a SetPublishingMode service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginSetPublishingMode(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_SetPublishingModeRequest* pRequest = OpcUa_Null;
    OpcUa_SetPublishingModeResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnSetPublishingMode* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginSetPublishingMode");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_SetPublishingModeRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_SetPublishingModeRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->PublishingEnabled,
        pRequest->NoOfSubscriptionIds,
        pRequest->SubscriptionIds,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_SetPublishingModeResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information SetPublishingMode service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_SetPublishingMode_ServiceType =
{
    OpcUaId_SetPublishingModeRequest,
    &OpcUa_SetPublishingModeResponse_EncodeableType,
    OpcUa_Server_BeginSetPublishingMode,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_SetPublishingMode
};
#endif

#ifndef OPCUA_EXCLUDE_Publish
/*============================================================================
 * A pointer to a function that implements the Publish service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnPublish)(
    OpcUa_Endpoint                           hEndpoint,
    OpcUa_Handle                             hContext,
    const OpcUa_RequestHeader*               pRequestHeader,
    OpcUa_Int32                              nNoOfSubscriptionAcknowledgements,
    const OpcUa_SubscriptionAcknowledgement* pSubscriptionAcknowledgements,
    OpcUa_ResponseHeader*                    pResponseHeader,
    OpcUa_UInt32*                            pSubscriptionId,
    OpcUa_Int32*                             pNoOfAvailableSequenceNumbers,
    OpcUa_UInt32**                           pAvailableSequenceNumbers,
    OpcUa_Boolean*                           pMoreNotifications,
    OpcUa_NotificationMessage*               pNotificationMessage,
    OpcUa_Int32*                             pNoOfResults,
    OpcUa_StatusCode**                       pResults,
    OpcUa_Int32*                             pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**                   pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the Publish service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_Publish(
    OpcUa_Endpoint                           a_hEndpoint,
    OpcUa_Handle                             a_hContext,
    const OpcUa_RequestHeader*               a_pRequestHeader,
    OpcUa_Int32                              a_nNoOfSubscriptionAcknowledgements,
    const OpcUa_SubscriptionAcknowledgement* a_pSubscriptionAcknowledgements,
    OpcUa_ResponseHeader*                    a_pResponseHeader,
    OpcUa_UInt32*                            a_pSubscriptionId,
    OpcUa_Int32*                             a_pNoOfAvailableSequenceNumbers,
    OpcUa_UInt32**                           a_pAvailableSequenceNumbers,
    OpcUa_Boolean*                           a_pMoreNotifications,
    OpcUa_NotificationMessage*               a_pNotificationMessage,
    OpcUa_Int32*                             a_pNoOfResults,
    OpcUa_StatusCode**                       a_pResults,
    OpcUa_Int32*                             a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**                   a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_Publish");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfSubscriptionAcknowledgements, a_pSubscriptionAcknowledgements);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pSubscriptionId);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfAvailableSequenceNumbers);
    OpcUa_ReturnErrorIfArgumentNull(a_pAvailableSequenceNumbers);
    OpcUa_ReturnErrorIfArgumentNull(a_pMoreNotifications);
    OpcUa_ReturnErrorIfArgumentNull(a_pNotificationMessage);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a Publish service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginPublish(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_PublishRequest* pRequest = OpcUa_Null;
    OpcUa_PublishResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnPublish* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginPublish");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_PublishRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_PublishRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->NoOfSubscriptionAcknowledgements,
        pRequest->SubscriptionAcknowledgements,
        &pResponse->ResponseHeader,
        &pResponse->SubscriptionId,
        &pResponse->NoOfAvailableSequenceNumbers,
        &pResponse->AvailableSequenceNumbers,
        &pResponse->MoreNotifications,
        &pResponse->NotificationMessage,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_PublishResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information Publish service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_Publish_ServiceType =
{
    OpcUaId_PublishRequest,
    &OpcUa_PublishResponse_EncodeableType,
    OpcUa_Server_BeginPublish,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_Publish
};
#endif

#ifndef OPCUA_EXCLUDE_Republish
/*============================================================================
 * A pointer to a function that implements the Republish service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnRepublish)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_UInt32               nSubscriptionId,
    OpcUa_UInt32               nRetransmitSequenceNumber,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_NotificationMessage* pNotificationMessage);

/*============================================================================
 * A stub method which implements the Republish service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_Republish(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_UInt32               a_nSubscriptionId,
    OpcUa_UInt32               a_nRetransmitSequenceNumber,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_NotificationMessage* a_pNotificationMessage)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_Republish");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_nSubscriptionId);
    OpcUa_ReferenceParameter(a_nRetransmitSequenceNumber);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNotificationMessage);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a Republish service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginRepublish(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_RepublishRequest* pRequest = OpcUa_Null;
    OpcUa_RepublishResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnRepublish* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginRepublish");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_RepublishRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_RepublishRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->SubscriptionId,
        pRequest->RetransmitSequenceNumber,
        &pResponse->ResponseHeader,
        &pResponse->NotificationMessage);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_RepublishResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information Republish service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_Republish_ServiceType =
{
    OpcUaId_RepublishRequest,
    &OpcUa_RepublishResponse_EncodeableType,
    OpcUa_Server_BeginRepublish,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_Republish
};
#endif

#ifndef OPCUA_EXCLUDE_TransferSubscriptions
/*============================================================================
 * A pointer to a function that implements the TransferSubscriptions service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnTransferSubscriptions)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_Int32                nNoOfSubscriptionIds,
    const OpcUa_UInt32*        pSubscriptionIds,
    OpcUa_Boolean              bSendInitialValues,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Int32*               pNoOfResults,
    OpcUa_TransferResult**     pResults,
    OpcUa_Int32*               pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the TransferSubscriptions service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_TransferSubscriptions(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_Int32                a_nNoOfSubscriptionIds,
    const OpcUa_UInt32*        a_pSubscriptionIds,
    OpcUa_Boolean              a_bSendInitialValues,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfResults,
    OpcUa_TransferResult**     a_pResults,
    OpcUa_Int32*               a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_TransferSubscriptions");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfSubscriptionIds, a_pSubscriptionIds);
    OpcUa_ReferenceParameter(a_bSendInitialValues);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a TransferSubscriptions service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginTransferSubscriptions(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_TransferSubscriptionsRequest* pRequest = OpcUa_Null;
    OpcUa_TransferSubscriptionsResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnTransferSubscriptions* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginTransferSubscriptions");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_TransferSubscriptionsRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_TransferSubscriptionsRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->NoOfSubscriptionIds,
        pRequest->SubscriptionIds,
        pRequest->SendInitialValues,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_TransferSubscriptionsResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information TransferSubscriptions service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_TransferSubscriptions_ServiceType =
{
    OpcUaId_TransferSubscriptionsRequest,
    &OpcUa_TransferSubscriptionsResponse_EncodeableType,
    OpcUa_Server_BeginTransferSubscriptions,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_TransferSubscriptions
};
#endif

#ifndef OPCUA_EXCLUDE_DeleteSubscriptions
/*============================================================================
 * A pointer to a function that implements the DeleteSubscriptions service.
 *===========================================================================*/
typedef OpcUa_StatusCode (OpcUa_ServerApi_PfnDeleteSubscriptions)(
    OpcUa_Endpoint             hEndpoint,
    OpcUa_Handle               hContext,
    const OpcUa_RequestHeader* pRequestHeader,
    OpcUa_Int32                nNoOfSubscriptionIds,
    const OpcUa_UInt32*        pSubscriptionIds,
    OpcUa_ResponseHeader*      pResponseHeader,
    OpcUa_Int32*               pNoOfResults,
    OpcUa_StatusCode**         pResults,
    OpcUa_Int32*               pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     pDiagnosticInfos);

/*============================================================================
 * A stub method which implements the DeleteSubscriptions service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_DeleteSubscriptions(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_Int32                a_nNoOfSubscriptionIds,
    const OpcUa_UInt32*        a_pSubscriptionIds,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfResults,
    OpcUa_StatusCode**         a_pResults,
    OpcUa_Int32*               a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pDiagnosticInfos)
{
    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_ServerApi_DeleteSubscriptions");

    /* validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfSubscriptionIds, a_pSubscriptionIds);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

    uStatus = OpcUa_BadNotImplemented;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * Begins processing of a DeleteSubscriptions service request.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_Server_BeginDeleteSubscriptions(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType)
{
    OpcUa_DeleteSubscriptionsRequest* pRequest = OpcUa_Null;
    OpcUa_DeleteSubscriptionsResponse* pResponse = OpcUa_Null;
    OpcUa_EncodeableType* pResponseType = OpcUa_Null;
    OpcUa_ServerApi_PfnDeleteSubscriptions* pfnInvoke = OpcUa_Null;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "OpcUa_Server_BeginDeleteSubscriptions");

    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(*a_ppRequest);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestType);

    OpcUa_ReturnErrorIfTrue(a_pRequestType->TypeId != OpcUaId_DeleteSubscriptionsRequest, OpcUa_BadInvalidArgument);

    pRequest = (OpcUa_DeleteSubscriptionsRequest*)*a_ppRequest;

    /* create a context to use for sending a response */
    uStatus = OpcUa_Endpoint_BeginSendResponse(a_hEndpoint, a_hContext, (OpcUa_Void**)&pResponse, &pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    /* get the function that implements the service call. */
    uStatus = OpcUa_Endpoint_GetServiceFunction(a_hEndpoint, a_hContext, (OpcUa_PfnInvokeService**)&pfnInvoke);
    OpcUa_GotoErrorIfBad(uStatus);

    /* invoke the service */
    uStatus = pfnInvoke(
        a_hEndpoint,        a_hContext,
        &pRequest->RequestHeader,
        pRequest->NoOfSubscriptionIds,
        pRequest->SubscriptionIds,
        &pResponse->ResponseHeader,
        &pResponse->NoOfResults,
        &pResponse->Results,
        &pResponse->NoOfDiagnosticInfos,
        &pResponse->DiagnosticInfos);

    if (OpcUa_IsBad(uStatus))
    {
        OpcUa_Void* pFault = OpcUa_Null;
        OpcUa_EncodeableType* pFaultType = OpcUa_Null;

        /* create a fault */
        uStatus = OpcUa_ServerApi_CreateFault(
            &pRequest->RequestHeader,
            uStatus,
            &pResponse->ResponseHeader.ServiceDiagnostics,
            &pResponse->ResponseHeader.NoOfStringTable,
            &pResponse->ResponseHeader.StringTable,
            &pFault,
            &pFaultType);

        OpcUa_GotoErrorIfBad(uStatus);

        /* free the response */
        OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

        /* make the response the fault */
        pResponse = (OpcUa_DeleteSubscriptionsResponse*)pFault;
        pResponseType = pFaultType;
    }

    /* send the response */
    uStatus = OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, OpcUa_Good, pResponse, pResponseType);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* send an error response */
    OpcUa_Endpoint_EndSendResponse(a_hEndpoint, &a_hContext, uStatus, OpcUa_Null, OpcUa_Null);

    OpcUa_EncodeableObject_Delete(pResponseType, (OpcUa_Void**)&pResponse);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * The service dispatch information DeleteSubscriptions service.
 *===========================================================================*/
struct _OpcUa_ServiceType OpcUa_DeleteSubscriptions_ServiceType =
{
    OpcUaId_DeleteSubscriptionsRequest,
    &OpcUa_DeleteSubscriptionsResponse_EncodeableType,
    OpcUa_Server_BeginDeleteSubscriptions,
    (OpcUa_PfnInvokeService*)OpcUa_ServerApi_DeleteSubscriptions
};
#endif

/*============================================================================
 * Table of standard services.
 *===========================================================================*/
OpcUa_ServiceType* OpcUa_SupportedServiceTypes[] =
{
    #ifndef OPCUA_EXCLUDE_FindServers
    &OpcUa_FindServers_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_FindServersOnNetwork
    &OpcUa_FindServersOnNetwork_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_GetEndpoints
    &OpcUa_GetEndpoints_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisterServer
    &OpcUa_RegisterServer_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisterServer2
    &OpcUa_RegisterServer2_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_Write
    &OpcUa_Write_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_HistoryUpdate
    &OpcUa_HistoryUpdate_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_Call
    &OpcUa_Call_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_CreateMonitoredItems
    &OpcUa_CreateMonitoredItems_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_ModifyMonitoredItems
    &OpcUa_ModifyMonitoredItems_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_SetMonitoringMode
    &OpcUa_SetMonitoringMode_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_SetTriggering
    &OpcUa_SetTriggering_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_DeleteMonitoredItems
    &OpcUa_DeleteMonitoredItems_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_CreateSubscription
    &OpcUa_CreateSubscription_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_ModifySubscription
    &OpcUa_ModifySubscription_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_SetPublishingMode
    &OpcUa_SetPublishingMode_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_Publish
    &OpcUa_Publish_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_Republish
    &OpcUa_Republish_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_TransferSubscriptions
    &OpcUa_TransferSubscriptions_ServiceType,
    #endif
    #ifndef OPCUA_EXCLUDE_DeleteSubscriptions
    &OpcUa_DeleteSubscriptions_ServiceType,
    #endif
    OpcUa_Null
};

#endif /* OPCUA_HAVE_SERVERAPI */
/* This is the last line of an autogenerated file. */
