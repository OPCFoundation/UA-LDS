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
    OpcUa_Null
};

#endif /* OPCUA_HAVE_SERVERAPI */
/* This is the last line of an autogenerated file. */
