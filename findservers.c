/* Copyright (c) 1996-2018, OPC Foundation. All rights reserved.

The source code in this file is covered under a dual - license scenario :
-RCL : for OPC Foundation members in good - standing
- GPL V2 : everybody else

RCL license terms accompanied with this source code.See http ://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code.See http ://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* system includes */
#include <time.h>
/* uastack includes */
#include <opcua_serverstub.h>
#include <opcua_memory.h>
#include <opcua_string.h>
#include <opcua_core.h>
#include <opcua_datetime.h>
/* local includes */
#include "config.h"
#include "ualds.h"
#include "settings.h"
/* local platform includes */
#include <platform.h>
#include <log.h>

/** FindServer Service implementation.
* @param hEndpoint OPC UA Endpoint handle.
* @param hContext  Service context.
* @param ppRequest Pointer to decoded service request structure.
* @param pRequestType The service type.
*
* @return OpcUa_StatusCode
*/
OpcUa_StatusCode ualds_findservers(
    OpcUa_Endpoint        hEndpoint,
    OpcUa_Handle          hContext,
    OpcUa_Void          **ppRequest,
    OpcUa_EncodeableType *pRequestType)
{
    OpcUa_FindServersRequest  *pRequest;
    OpcUa_FindServersResponse *pResponse;
    OpcUa_EncodeableType      *pResponseType = 0;
    OpcUa_StatusCode           uStatus = OpcUa_Good;
    char                     **szUriArray = 0;
    char                       szTmpUrl[UALDS_CONF_MAX_URI_LENGTH];
    int i, j;
    char                       szHostname[50];
    int numServers = 0;
    int numServerNames = 0;
    int numDiscoveryUrls = 0;
    int tmpInt;

    UALDS_UNUSED(pRequestType);

    OpcUa_ReturnErrorIfArgumentNull(ppRequest);
    pRequest = *ppRequest;

    ualds_platform_getfqhostname(szHostname, 50);
    
    uStatus = OpcUa_Endpoint_BeginSendResponse(
        hEndpoint,
        hContext,
        (OpcUa_Void**)&pResponse,
        &pResponseType);
    OpcUa_ReturnErrorIfBad(uStatus);

    if ( pResponse )
    {
		OpcUa_Mutex_Lock(g_mutex);

        ualds_expirationcheck();

        ualds_settings_begingroup("RegisteredServers");
        ualds_settings_beginreadarray("Servers", &numServers);
        if (numServers > 0)
        {
            szUriArray = OpcUa_Alloc(numServers * sizeof(char*));
            if (szUriArray)
            {
                OpcUa_MemSet(szUriArray, 0, numServers * sizeof(char*));

                tmpInt = 0;
                for (i=0; i<numServers; i++)
                {
                    OpcUa_Boolean bFilteredOut = OpcUa_False;

                    ualds_settings_setarrayindex(i);
                    ualds_settings_readstring("ServerUri", szTmpUrl, UALDS_CONF_MAX_URI_LENGTH);

                    /* filter by ServerUri */
                    if (pRequest->NoOfServerUris > 0)
                    {
                        bFilteredOut = OpcUa_True;

                        for (j = 0; j < pRequest->NoOfServerUris; j++)
                        {
                            if (OpcUa_String_StrnCmp(&pRequest->ServerUris[j], OpcUa_String_FromCString(szTmpUrl), OPCUA_STRING_LENDONTCARE, OpcUa_False) == 0)
                            {
                                bFilteredOut = OpcUa_False;
                                break;
                            }
                        }
                    }
                    if (bFilteredOut == OpcUa_False)
                    {
                        szUriArray[tmpInt] = OpcUa_Alloc(UALDS_CONF_MAX_URI_LENGTH);
                        if (szUriArray[tmpInt])
                        {
                            strlcpy(szUriArray[tmpInt], szTmpUrl, UALDS_CONF_MAX_URI_LENGTH);
                        }
                        tmpInt++;
                    }
                }
                numServers = tmpInt;
            }
            else
            {
                UALDS_BUILDRESPONSEHEADER;

                pResponse->ResponseHeader.ServiceResult = OpcUa_BadOutOfMemory;

                /* Send response */
                OpcUa_Endpoint_EndSendResponse(
                    hEndpoint,
                    &hContext,
                    OpcUa_Good,
                    pResponse,
                    pResponseType);

                ualds_settings_endarray();
                ualds_settings_endgroup();

				OpcUa_Mutex_Unlock(g_mutex);

                return OpcUa_Good;
            }
        }
        ualds_settings_endarray();
        ualds_settings_endgroup();

        pResponse->NoOfServers = numServers;
        pResponse->Servers = OpcUa_Alloc(sizeof(OpcUa_ApplicationDescription) * pResponse->NoOfServers);
        if (pResponse->Servers)
        {
            for (i=0; i<pResponse->NoOfServers; i++)
            {
                OpcUa_ApplicationDescription_Initialize(&pResponse->Servers[i]);
                if (szUriArray[i] == 0) continue;
                ualds_settings_begingroup(szUriArray[i]);
                ualds_settings_readuastring("ProductUri", &pResponse->Servers[i].ProductUri);
                ualds_settings_beginreadarray("ServerNames", &numServerNames);
                if (numServerNames > 0)
                {
                    ualds_settings_setarrayindex(0);
                    ualds_settings_readuastring("Locale", &pResponse->Servers[i].ApplicationName.Locale);
                    ualds_settings_readuastring("Text", &pResponse->Servers[i].ApplicationName.Text);
                }
                ualds_settings_endarray();
                ualds_settings_readint("ServerType", &tmpInt);
                pResponse->Servers[i].ApplicationType = (OpcUa_ApplicationType)tmpInt;
                ualds_settings_readuastring("GatewayServerUri", &pResponse->Servers[i].GatewayServerUri);
                ualds_settings_beginreadarray("DiscoveryUrls", &numDiscoveryUrls);
                if (numDiscoveryUrls > 0)
                {
                    pResponse->Servers[i].NoOfDiscoveryUrls = numDiscoveryUrls;
                    pResponse->Servers[i].DiscoveryUrls = OpcUa_Alloc(sizeof(OpcUa_String) * numDiscoveryUrls);
                    if (pResponse->Servers[i].DiscoveryUrls)
                    {
                        for (j=0; j<numDiscoveryUrls; j++)
                        {
                            ualds_settings_setarrayindex(j);
                            ualds_settings_readstring("Url", szTmpUrl, UALDS_CONF_MAX_URI_LENGTH);
                            replace_string(szTmpUrl, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", szHostname);
                            OpcUa_String_Initialize(&pResponse->Servers[i].DiscoveryUrls[j]);
                            OpcUa_String_AttachCopy(&pResponse->Servers[i].DiscoveryUrls[j], szTmpUrl);
                        }
                    }
                }
                ualds_settings_endarray();
                ualds_settings_endgroup();

                /* finally replace [gethostname] in ServerUri and copy to ApplicationUri */
                replace_string(szUriArray[i], UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", szHostname);
                OpcUa_String_AttachCopy(&pResponse->Servers[i].ApplicationUri, szUriArray[i]);
            }
        }
        else
        {
            uStatus = OpcUa_BadOutOfMemory;
        }


		OpcUa_Mutex_Unlock(g_mutex);

        UALDS_BUILDRESPONSEHEADER;

        /* Send response */
        OpcUa_Endpoint_EndSendResponse(
            hEndpoint,
            &hContext,
            uStatus,
            pResponse,
            pResponseType);

        /* free response */
        OpcUa_FindServersResponse_Clear(pResponse);
        OpcUa_Free(pResponse);

        /* cleanup */
        if (szUriArray)
        {
            for (i=0; i<numServers; i++)
            {
                if (szUriArray[i]) OpcUa_Free(szUriArray[i]);
            }
            OpcUa_Free(szUriArray);
        }

        /* we have send a response, either with good or bad status, this does not matter.
         * we must return Good now, otherwise the stack will send an error message.
         */
        uStatus = OpcUa_Good;
    }

    return uStatus;
}

/** @} */

