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

#ifndef _OpcUa_ServerApi_H_
#define _OpcUa_ServerApi_H_ 1
#ifdef OPCUA_HAVE_SERVERAPI

#include <opcua_types.h>
#include <opcua_endpoint.h>

OPCUA_BEGIN_EXTERN_C

#ifndef OPCUA_EXCLUDE_FindServers
/*============================================================================
 * Synchronously calls the FindServers service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_FindServers(
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
 * Begins processing of a FindServers service request.
 *===========================================================================*/
OPCUA_EXPORT_SYNC_SERVER_API OpcUa_StatusCode OpcUa_Server_BeginFindServers(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType);
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetwork
/*============================================================================
 * Synchronously calls the FindServersOnNetwork service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_FindServersOnNetwork(
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
 * Begins processing of a FindServersOnNetwork service request.
 *===========================================================================*/
OPCUA_EXPORT_SYNC_SERVER_API OpcUa_StatusCode OpcUa_Server_BeginFindServersOnNetwork(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType);
#endif

#ifndef OPCUA_EXCLUDE_GetEndpoints
/*============================================================================
 * Synchronously calls the GetEndpoints service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_GetEndpoints(
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
 * Begins processing of a GetEndpoints service request.
 *===========================================================================*/
OPCUA_EXPORT_SYNC_SERVER_API OpcUa_StatusCode OpcUa_Server_BeginGetEndpoints(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType);
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer
/*============================================================================
 * Synchronously calls the RegisterServer service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_RegisterServer(
    OpcUa_Endpoint                hEndpoint,
    OpcUa_Handle                  hContext,
    const OpcUa_RequestHeader*    pRequestHeader,
    const OpcUa_RegisteredServer* pServer,
    OpcUa_ResponseHeader*         pResponseHeader);

/*============================================================================
 * Begins processing of a RegisterServer service request.
 *===========================================================================*/
OPCUA_EXPORT_SYNC_SERVER_API OpcUa_StatusCode OpcUa_Server_BeginRegisterServer(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType);
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2
/*============================================================================
 * Synchronously calls the RegisterServer2 service.
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerApi_RegisterServer2(
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
 * Begins processing of a RegisterServer2 service request.
 *===========================================================================*/
OPCUA_EXPORT_SYNC_SERVER_API OpcUa_StatusCode OpcUa_Server_BeginRegisterServer2(
    OpcUa_Endpoint        a_hEndpoint,
    OpcUa_Handle          a_hContext,
    OpcUa_Void**          a_ppRequest,
    OpcUa_EncodeableType* a_pRequestType);
#endif

OPCUA_END_EXTERN_C

#endif /* OPCUA_HAVE_SERVERAPI */
#endif /* _OpcUa_ServerApi_H_ */
/* This is the last line of an autogenerated file. */
