/* Copyright (c) 1996-2020, OPC Foundation. All rights reserved.

The source code in this file is covered under a dual - license scenario :
- RCL: for OPC Foundation members in good - standing
- GPL V2: everybody else

RCL license terms accompanied with this source code.See http ://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code.See http ://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "findserversonnetwork.h"

/* system includes */
#include <time.h>
/* mdns includes */
#include <dns_sd.h>

/* uastack includes */
#include <opcua_serverstub.h>
#include <opcua_memory.h>
#include <opcua_string.h>
#include <opcua_datetime.h>
#include <opcua_socket.h>
#include <opcua_timer.h>
#include <opcua_list.h>
#include <opcua_core.h>
/* local includes */
#include "config.h"
#include "ualds.h"
#include "settings.h"
#include "utils.h"
/* local platform includes */
#include <platform.h>
#include <log.h>
#include <opcua_p_socket.h>

static OpcUa_List g_findServersSocketList;

typedef enum _ualds_contextType
{
    ContextType_Browse,
    ContextType_Resolve
} ualds_contextType;

typedef struct _ualds_genericContext
{
    /* generic context */
    ualds_contextType   contextType;
    DNSServiceRef       sdRef;
    OpcUa_Socket        uaSocket;
} ualds_genericContext;

typedef struct _ualds_browseContext
{
    /* generic context */
    ualds_contextType   contextType;
    DNSServiceRef       sdRef;
    OpcUa_Socket        uaSocket;

    /* browse context */
} ualds_browseContext;

typedef struct _ualds_resolveContext
{
    /* generic context */
    ualds_contextType   contextType;
    DNSServiceRef       sdRef;
    OpcUa_Socket        uaSocket;

    /* resolve context */
    OpcUa_ServerOnNetwork record;
} ualds_resolveContext;


static OpcUa_Timer              g_hBrowseTimer = OpcUa_Null;
#define                         g_noOfServiceTypes 3
static ualds_browseContext      g_browseContexts[g_noOfServiceTypes];
static const char              *g_pszServiceTypes[g_noOfServiceTypes] = {"_opcua-tcp._tcp",
                                                                         "_opcua-wss._tcp",
                                                                         "_opcua-https._tcp"};
static const char              *g_pszServiceSchemes[g_noOfServiceTypes] = {"opc.tcp://",
                                                                           "opc.wss://",
                                                                           "opc.https://"};
/* list of ualds_resolveContext used as internal DNSService cache */
static OpcUa_List               g_lstServers;
static OpcUa_DateTime           g_lastCounterResetTime = {0, 0};
static OpcUa_UInt32             g_currentRecordId = 0;

int string_ends_with(const char * str, const char * suffix)
{
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    int ret = strcmp(str + (str_len - suffix_len), suffix);

    return (str_len >= suffix_len) && (ret);
}


/* if pBrowseContext is one of the global browse contexts, the according browse call has been canceled.
   in this case, all according entries in g_lstServers have to be removed. */
void ualds_findserversonnetwork_removeServiceEntries(ualds_browseContext *pBrowseContext)
{
    OpcUa_UInt32 i;
    for (i = 0; i < g_noOfServiceTypes; i++)
    {
        if (pBrowseContext == &g_browseContexts[i])
        {
            ualds_resolveContext *pResolveContext = OpcUa_Null;

            ualds_log(UALDS_LOG_DEBUG, "ualds_findserversonnetwork_removeServiceEntries: remove all entries of type %s", g_pszServiceTypes[i]);

            OpcUa_List_Enter(&g_lstServers);
            OpcUa_List_ResetCurrent(&g_lstServers);
            pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
            while (pResolveContext)
            {
                if (OpcUa_StrnCmpA(g_pszServiceSchemes[i], OpcUa_String_GetRawString(&pResolveContext->record.DiscoveryUrl), OpcUa_StrLenA(g_pszServiceSchemes[i])) == 0)
                {
                    /* cancel all outstanding service calls belonging to this service record */
                    if (pResolveContext->sdRef != OpcUa_Null)
                    {
                        OpcUa_Socket_Close(pResolveContext->uaSocket);
                        DNSServiceRefDeallocate(pResolveContext->sdRef);
                        pResolveContext->uaSocket = OpcUa_Null;
                        pResolveContext->sdRef = OpcUa_Null;
                    }

                    OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
                    OpcUa_Free(pResolveContext);
                    OpcUa_List_DeleteCurrentElement(&g_lstServers);
                    pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
                }
                else
                {
                    pResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
                }
            }
            OpcUa_List_Leave(&g_lstServers);
            break;
        }
    }
}

void ualds_findserversonnetwork_socketEventCallback(int* shutdown)
{
    fd_set readFDs;
    struct timeval tv;
    int fd, status;
    ualds_genericContext* context;
    MulticastSocketCallbackStruct* socketCallbackStruct;

    if (*shutdown)
    {
        return;
    }

    /*
    TODO:
    This should be combined with zerconf socket callback into one "poll" loop that builds 
    pollfd or select bitmask from the socket cb list. This would then be driven from main.
    */

    /* Lock access to shared sdref/fd from server processing thread */
    OpcUa_Mutex_Lock(g_mutex);
    OpcUa_List_Enter(&g_findServersSocketList);
    OpcUa_List_ResetCurrent(&g_findServersSocketList);

    socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetCurrentElement(&g_findServersSocketList);
    while (socketCallbackStruct)
    {
        context = (ualds_genericContext*)socketCallbackStruct->context;
        if (*shutdown || context == OpcUa_Null || context->sdRef == OpcUa_Null)
        {
            /* browse or resolve failed or was stopped - remove socket from poll list */
            OpcUa_Free(socketCallbackStruct);
            OpcUa_List_DeleteCurrentElement(&g_findServersSocketList);
            socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetCurrentElement(&g_findServersSocketList);
            continue;
        }

        fd = DNSServiceRefSockFD(socketCallbackStruct->sdRef);
        FD_ZERO(&readFDs);
        FD_SET(fd, &readFDs);

        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        status = select(fd + 1, &readFDs, NULL, NULL, &tv);
        if (status > 0)
        {
            if (FD_ISSET(fd, &readFDs))
            {
                int error = DNSServiceProcessResult(socketCallbackStruct->sdRef);
                if (error != kDNSServiceErr_NoError)
                {
                    ualds_log(UALDS_LOG_ERR, "ualds_findserversonnetwork_socketEventCallback: DNSServiceProcessResult returned error %i", error);
                }
            }
        }
        socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetNextElement(&g_findServersSocketList);
    }

    OpcUa_List_Leave(&g_findServersSocketList);
    OpcUa_Mutex_Unlock(g_mutex);
}

/* async DNSService callback for browse result resolving */
void DNSSD_API ualds_DNSServiceResolveReply(DNSServiceRef           sdRef,
                                            DNSServiceFlags         flags,
                                            uint32_t                interfaceIndex,
                                            DNSServiceErrorType     errorCode,
                                            const char             *fullname,
                                            const char             *hosttarget,
                                            uint16_t                port,
                                            uint16_t                txtLen,
                                            const unsigned char    *txtRecord,
                                            void                   *context)
{
    ualds_resolveContext   *pResolveContext = (ualds_resolveContext*)context;
    OpcUa_ServerOnNetwork  *pRecord = &pResolveContext->record;
    uint16_t                hostOrderPort = ntohs(port);

    UALDS_UNUSED(sdRef);
    UALDS_UNUSED(flags);

    if (errorCode != kDNSServiceErr_NoError)
    {
        ualds_log(UALDS_LOG_ERR, "ualds_DNSServiceResolveReply: error %i", errorCode);
        goto Error;
    }

    /* debug traces */
    ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceResolveReply: resolved service:");
    ualds_log(UALDS_LOG_DEBUG, "                              interfaceIndex %u", interfaceIndex);
    ualds_log(UALDS_LOG_DEBUG, "                              fullname       %s", fullname);
    ualds_log(UALDS_LOG_DEBUG, "                              hosttarget     %s", hosttarget);
    ualds_log(UALDS_LOG_DEBUG, "                              port           %hu", hostOrderPort);

    /* fill results */

    OpcUa_String_StrnCat(&pRecord->DiscoveryUrl,
        OpcUa_String_FromCString((OpcUa_StringA)hosttarget),
        strlen(hosttarget));

    char szPort[40] = { 0 };
#if OPCUA_USE_SAFE_FUNCTIONS
    OpcUa_SnPrintfA(szPort, 40, 40, ":%hu", hostOrderPort);
#else
    OpcUa_SnPrintfA(szPort, 40, ":%hu", hostOrderPort);
#endif
    OpcUa_String_StrnCat(&pRecord->DiscoveryUrl,
        OpcUa_String_FromCString(szPort),
        OPCUA_STRING_LENDONTCARE);

    if (TXTRecordContainsKey(txtLen, txtRecord, "path"))
    {
        uint8_t lenPath = 0;
        const char *szPath = TXTRecordGetValuePtr(txtLen, txtRecord, "path", &lenPath);
        if (lenPath > 0 && szPath != OpcUa_Null)
        {
            OpcUa_String_StrnCat(&pRecord->DiscoveryUrl,
                OpcUa_String_FromCString((OpcUa_StringA)szPath),
                                 lenPath);
        }
    }

    if (TXTRecordContainsKey(txtLen, txtRecord, "caps"))
    {
        uint8_t lenCaps = 0;
        const char *szCaps = TXTRecordGetValuePtr(txtLen, txtRecord, "caps", &lenCaps);
        if (lenCaps > 0 && szCaps != OpcUa_Null && szCaps[0] != '\0')
        {
            const char *szLastPos = szCaps;
            int i = 0, iCap = 0;
            pRecord->NoOfServerCapabilities = 1;

            for (i = 0; i < lenCaps; i++)
            {
                if (szCaps[i] == ',' && szCaps[i+1] != ',' && i+1 != lenCaps) {pRecord->NoOfServerCapabilities++;}
            }
            pRecord->ServerCapabilities = (OpcUa_String*)OpcUa_Alloc(pRecord->NoOfServerCapabilities * sizeof(OpcUa_String));
            for (i = 0; i < lenCaps; i++)
            {
                if (szCaps[i] == ',' && szCaps[i+1] != ',' && i+1 != lenCaps)
                {
                    OpcUa_String_Initialize(&pRecord->ServerCapabilities[iCap]);
                    OpcUa_String_AttachToString((OpcUa_StringA)szLastPos,
                                                &szCaps[i] - szLastPos,
                                                &szCaps[i] - szLastPos,
                                                OpcUa_True,
                                                OpcUa_True,
                                                &pRecord->ServerCapabilities[iCap]);
                    iCap++;
                    szLastPos = &szCaps[i+1];
                }
            }
            OpcUa_String_Initialize(&pRecord->ServerCapabilities[iCap]);
            OpcUa_String_AttachToString((OpcUa_StringA)szLastPos,
                                        &szCaps[i] - szLastPos,
                                        &szCaps[i] - szLastPos,
                                        OpcUa_True,
                                        OpcUa_True,
                                        &pRecord->ServerCapabilities[iCap]);
        }
    }

Error:
    OpcUa_Socket_Close(pResolveContext->uaSocket);
    DNSServiceRefDeallocate(pResolveContext->sdRef);
    pResolveContext->uaSocket = OpcUa_Null;
    pResolveContext->sdRef = OpcUa_Null;
}

/* async DNSService callback for browse results */
void DNSSD_API ualds_DNSServiceBrowseReply(DNSServiceRef        sdRef,
                                           DNSServiceFlags      flags,
                                           uint32_t             interfaceIndex,
                                           DNSServiceErrorType  errorCode,
                                           const char          *serviceName,
                                           const char          *regtype,
                                           const char          *replyDomain,
                                           void                *context)
{
    ualds_browseContext *pBrowseContext = (ualds_browseContext*)context;

    UALDS_UNUSED(sdRef);

    if (errorCode != kDNSServiceErr_NoError)
    {
        ualds_log(UALDS_LOG_ERR, "ualds_DNSServiceBrowseReply: error %i", errorCode);
        goto Error;
    }

    if (flags & kDNSServiceFlagsAdd)
    {
        /* existing service, resolve it */
        ualds_resolveContext   *pResolveContext = OpcUa_Null;
        DNSServiceErrorType     retDnssd = kDNSServiceErr_NoError;
        OpcUa_Boolean           bFound = OpcUa_False;
        char                    szDiscoveryUrl[UALDS_CONF_MAX_URI_LENGTH] = {0};
        OpcUa_UInt32            i = 0;

        OpcUa_Boolean           bFoundSchema = OpcUa_False;
        for (i = 0; i < g_noOfServiceTypes; i++)
        {
            if (strncmp(regtype, g_pszServiceTypes[i], strlen(g_pszServiceTypes[i])) == 0)
            {
                strlcat(szDiscoveryUrl, g_pszServiceSchemes[i], UALDS_CONF_MAX_URI_LENGTH);
                bFoundSchema = OpcUa_True;
                break;
            }
        }

        if (bFoundSchema == OpcUa_False)
        {
            ualds_log(UALDS_LOG_DEBUG, "ignoring invalid regtype %s", regtype);
            goto Error;
        }

        if (szDiscoveryUrl[0] == '\0')
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceBrowseReply: ignoring invalid regtype %s", regtype);
            goto Error;
        }

        /* check if record is already known; this can happen if a service is delivered for two different
           network interfaces */
        OpcUa_List_Enter(&g_lstServers);
        OpcUa_List_ResetCurrent(&g_lstServers);
        pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
        while (pResolveContext)
        {
            if (OpcUa_StrCmpA(serviceName, OpcUa_String_GetRawString(&pResolveContext->record.ServerName)) == 0 &&
                OpcUa_StrnCmpA(szDiscoveryUrl, OpcUa_String_GetRawString(&pResolveContext->record.DiscoveryUrl), strlen(szDiscoveryUrl)) == 0)
            {
                bFound = OpcUa_True;
                break;
            }

            pResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
        }
        OpcUa_List_Leave(&g_lstServers);

        if (bFound != OpcUa_False)
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceBrowseReply: ignoring known service %s / %s", serviceName, regtype);
            goto Error;
        }

        /* fill results */
        pResolveContext = (ualds_resolveContext*)OpcUa_Alloc(sizeof(ualds_resolveContext));
        if (pResolveContext)
        {
            OpcUa_StatusCode uStatus = OpcUa_Good;

            pResolveContext->contextType = ContextType_Resolve;
            pResolveContext->uaSocket = OpcUa_Null;
            OpcUa_ServerOnNetwork_Initialize(&pResolveContext->record);

            /* debug traces */
            ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceBrowseReply: received service:");
            ualds_log(UALDS_LOG_DEBUG, "                             interfaceIndex %u", interfaceIndex);
            ualds_log(UALDS_LOG_DEBUG, "                             serviceName    %s", serviceName);
            ualds_log(UALDS_LOG_DEBUG, "                             regtype        %s", regtype);
            ualds_log(UALDS_LOG_DEBUG, "                             replyDomain    %s", replyDomain);

            OpcUa_String_AttachToString((OpcUa_StringA)szDiscoveryUrl,
                                        strlen(szDiscoveryUrl),
                                        strlen(szDiscoveryUrl),
                                        OpcUa_True,
                                        OpcUa_True,
                                        &pResolveContext->record.DiscoveryUrl);

            /* set service name and type */
            OpcUa_String_AttachToString((OpcUa_StringA)serviceName,
                                        strlen(serviceName),
                                        strlen(serviceName),
                                        OpcUa_True,
                                        OpcUa_True,
                                        &pResolveContext->record.ServerName);

            /* set RecordId */
            pResolveContext->record.RecordId = g_currentRecordId++;
            if (g_currentRecordId == 0)
            {
                ualds_resolveContext *pExistingResolveContext = OpcUa_Null;

                g_lastCounterResetTime = OpcUa_DateTime_UtcNow();

                /* reassign RecordIds to entries after overflow */
                OpcUa_List_Enter(&g_lstServers);
                OpcUa_List_ResetCurrent(&g_lstServers);
                pExistingResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
                while (pExistingResolveContext)
                {
                    pExistingResolveContext->record.RecordId = g_currentRecordId++;
                    pExistingResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
                }
                pResolveContext->record.RecordId = g_currentRecordId++;
                OpcUa_List_Leave(&g_lstServers);
            }

            /* append pResolveContext to list of services */
            OpcUa_List_Enter(&g_lstServers);
            uStatus = OpcUa_List_AddElementToEnd(&g_lstServers, pResolveContext);
            OpcUa_List_Leave(&g_lstServers);
            if (OpcUa_IsNotGood(uStatus))
            {
                ualds_log(UALDS_LOG_ERR, "ualds_DNSServiceBrowseReply: could not add pResolveContext to list");
                goto Error;
            }

            ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceBrowseReply: Call DNSServiceResolve");

            retDnssd = DNSServiceResolve(&pResolveContext->sdRef,
                                         0,
                                         interfaceIndex,
                                         serviceName,
                                         regtype,
                                         replyDomain,
                                         ualds_DNSServiceResolveReply,
                                         pResolveContext);

            /* process results */
            if (retDnssd == kDNSServiceErr_NoError)
            {
                OpcUa_List_Enter(&g_lstServers);

                if (pResolveContext->sdRef)
                {
                    MulticastSocketCallbackStruct* socketCallbackStruct = OpcUa_Alloc(sizeof(MulticastSocketCallbackStruct));
                    socketCallbackStruct->sdRef = pResolveContext->sdRef;
                    socketCallbackStruct->context = pResolveContext;
   
                    /* Lock is not needed - called on poll thread so just add to end and be done with it... */
                    /* OpcUa_List_Enter(&g_findServersSocketList); */
                    uStatus = OpcUa_List_AddElementToEnd(&g_findServersSocketList, socketCallbackStruct);
                    /* OpcUa_List_Leave(&g_findServersSocketList); */
                }

                OpcUa_List_Leave(&g_lstServers);
            }
            else
            {
                ualds_log(UALDS_LOG_ERR, "ualds_DNSServiceBrowseReply: DNSServiceResolve returned error %i", retDnssd);
                goto Error;
            }
        }
        else
        {
            ualds_log(UALDS_LOG_ERR, "ualds_DNSServiceBrowseReply: could not allocate pResolveContext");
            goto Error;
        }
    }
    else
    {
        /* remove record as it it not longer valid */
        ualds_resolveContext *pResolveContext = OpcUa_Null;
        char                  szDiscoveryUrl[UALDS_CONF_MAX_URI_LENGTH] = {0};
        OpcUa_UInt32          i = 0;

        /* debug traces */
        ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceBrowseReply: remove service:");
        ualds_log(UALDS_LOG_DEBUG, "                             interfaceIndex %u", interfaceIndex);
        ualds_log(UALDS_LOG_DEBUG, "                             serviceName    %s", serviceName);
        ualds_log(UALDS_LOG_DEBUG, "                             regtype        %s", regtype);
        ualds_log(UALDS_LOG_DEBUG, "                             replyDomain    %s", replyDomain);

        for (i = 0; i < g_noOfServiceTypes; i++)
        {
            if (strncmp(regtype, g_pszServiceTypes[i], strlen(g_pszServiceTypes[i])) == 0)
            {
                strlcat(szDiscoveryUrl, g_pszServiceSchemes[i], UALDS_CONF_MAX_URI_LENGTH);
                break;
            }
        }
        if (szDiscoveryUrl[0] == '\0')
        {
            ualds_log(UALDS_LOG_DEBUG, "ualds_DNSServiceBrowseReply: ignoring invalid regtype %s", regtype);
            goto Error;
        }

        /* search for server and remove it */
        OpcUa_List_Enter(&g_lstServers);
        OpcUa_List_ResetCurrent(&g_lstServers);
        pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
        while (pResolveContext)
        {
            if (OpcUa_StrCmpA(serviceName, OpcUa_String_GetRawString(&pResolveContext->record.ServerName)) == 0 &&
                OpcUa_StrnCmpA(szDiscoveryUrl, OpcUa_String_GetRawString(&pResolveContext->record.DiscoveryUrl), strlen(szDiscoveryUrl)) == 0)
            {
                /* cancel all outstanding service calls belonging to this service record */
                if (pResolveContext->sdRef != OpcUa_Null)
                {
                    OpcUa_Socket_Close(pResolveContext->uaSocket);
                    DNSServiceRefDeallocate(pResolveContext->sdRef);
                    pResolveContext->uaSocket = OpcUa_Null;
                    pResolveContext->sdRef = OpcUa_Null;
                }

                OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
                OpcUa_Free(pResolveContext);
                OpcUa_List_DeleteCurrentElement(&g_lstServers);
                break;
            }

            pResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
        }
        OpcUa_List_Leave(&g_lstServers);
    }

Error:
    if (errorCode != kDNSServiceErr_NoError)
    {
        OpcUa_Socket_Close(pBrowseContext->uaSocket);
        DNSServiceRefDeallocate(pBrowseContext->sdRef);
        pBrowseContext->uaSocket = OpcUa_Null;
        pBrowseContext->sdRef = OpcUa_Null;

        /* when cancelling a Browse call, remove according entries in service list */
        ualds_findserversonnetwork_removeServiceEntries(pBrowseContext);
    }
}

OpcUa_StatusCode OPCUA_DLLCALL ualds_findserversonnetwork_start_internal(OpcUa_Void*  pvCallbackData,
                                                                         OpcUa_Timer  hTimer,
                                                                         OpcUa_UInt32 msecElapsed)
{
    OpcUa_StatusCode        uStatus = OpcUa_Good;
    DNSServiceErrorType     retDnssd = kDNSServiceErr_NoError;
    unsigned int            i;

    UALDS_UNUSED(pvCallbackData);
    UALDS_UNUSED(hTimer);
    UALDS_UNUSED(msecElapsed);

    OpcUa_Mutex_Lock(g_mutex);

    for (i = 0; i < g_noOfServiceTypes; i++)
    {
        if (g_browseContexts[i].sdRef == OpcUa_Null)
        {
            g_browseContexts[i].contextType = ContextType_Browse;

            /* browse for services */
            ualds_log(UALDS_LOG_DEBUG, "ualds_findserversonnetwork_start_internal: Call DNSServiceBrowse for service type %s", g_pszServiceTypes[i]);
            retDnssd = DNSServiceBrowse(&g_browseContexts[i].sdRef,
                                        0,
                                        0,
                                        g_pszServiceTypes[i],
                                        0,
                                        ualds_DNSServiceBrowseReply,
                                        &g_browseContexts[i]);

            if (retDnssd == kDNSServiceErr_NoError)
            {
                if (g_browseContexts[i].sdRef)
                {
                    MulticastSocketCallbackStruct* socketCallbackStruct = OpcUa_Alloc(sizeof(MulticastSocketCallbackStruct));
                    socketCallbackStruct->sdRef = g_browseContexts[i].sdRef;
                    socketCallbackStruct->context = &g_browseContexts[i];

                    OpcUa_List_Enter(&g_findServersSocketList);
                    uStatus = OpcUa_List_AddElementToEnd(&g_findServersSocketList, socketCallbackStruct);
                    OpcUa_List_Leave(&g_findServersSocketList);
                }
            }
            else
            {
                ualds_log(UALDS_LOG_ERR, "ualds_findserversonnetwork_start_internal: DNSServiceBrowse returned error %i", retDnssd);
            }
        }
    }

    OpcUa_Mutex_Unlock(g_mutex);

    return uStatus;
}

OpcUa_StatusCode OPCUA_DLLCALL ualds_findserversonnetwork_stop_internal(OpcUa_Void*  pvCallbackData,
                                                                        OpcUa_Timer  hTimer,
                                                                        OpcUa_UInt32 msecElapsed)
{
    unsigned int            i;

    UALDS_UNUSED(pvCallbackData);
    UALDS_UNUSED(hTimer);
    UALDS_UNUSED(msecElapsed);

    OpcUa_Mutex_Lock(g_mutex);

    for (i = 0; i < g_noOfServiceTypes; i++)
    {
        /* release ref for stopping the browse */
        ualds_log(UALDS_LOG_DEBUG, "ualds_findserversonnetwork_stop_internal: Call DNSServiceRefDeallocate for browse for service type %s", g_pszServiceTypes[i]);
        OpcUa_Socket_Close(g_browseContexts[i].uaSocket);
        DNSServiceRefDeallocate(g_browseContexts[i].sdRef);
        g_browseContexts[i].uaSocket = OpcUa_Null;
        g_browseContexts[i].sdRef = OpcUa_Null;
    }

    OpcUa_Mutex_Unlock(g_mutex);

    return OpcUa_Good;
}

OpcUa_StatusCode ualds_findserversonnetwork_start_listening()
{
    OpcUa_StatusCode ret = OpcUa_BadNothingToDo;

    if (g_hBrowseTimer == OpcUa_Null)
    {
        int iBrowseCheckInterval = 10;

        OpcUa_Mutex_Lock(g_mutex);

        /* initialize */
        OpcUa_List_Initialize(&g_lstServers);
        OpcUa_List_Initialize(&g_findServersSocketList);
        g_lastCounterResetTime = OpcUa_DateTime_UtcNow();

        OpcUa_Mutex_Unlock(g_mutex);

        /* First ever browse - unlock mutex we internal browse will lock it again */
        ualds_findserversonnetwork_start_internal(OpcUa_Null, OpcUa_Null, 0);

        OpcUa_Mutex_Lock(g_mutex);

        /* get RegistrationInterval setting */
        ualds_settings_begingroup("Zeroconf");
        ualds_settings_readint("RegistrationInterval", &iBrowseCheckInterval);
        if (iBrowseCheckInterval < 5)
        {
            iBrowseCheckInterval = 5;
        }
        ualds_settings_endgroup();

        OpcUa_Mutex_Unlock(g_mutex);

        /* create timer for regular checking of registration */
        ualds_log(UALDS_LOG_INFO, "Create Zeroconf browse timer with interval %i", iBrowseCheckInterval);
        ret = OpcUa_Timer_Create(&g_hBrowseTimer,
                                 iBrowseCheckInterval * 1000,
                                 ualds_findserversonnetwork_start_internal,
                                 ualds_findserversonnetwork_stop_internal,
                                 OpcUa_Null);
    }

    return ret;
}

void ualds_findserversonnetwork_stop_listening()
{
    if (g_hBrowseTimer != OpcUa_Null)
    {
        ualds_resolveContext *pResolveContext = OpcUa_Null;

        /* delete browse timer, this automatically calls the unregister function */
        ualds_log(UALDS_LOG_INFO, "Delete Zeroconf browse timer");
        OpcUa_Timer_Delete(&g_hBrowseTimer);
        g_hBrowseTimer = OpcUa_Null;

        /* cleanup */
        OpcUa_List_Enter(&g_lstServers);
        OpcUa_List_ResetCurrent(&g_lstServers);
        pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
        while (pResolveContext)
        {
            /* cancel all outstanding service calls belonging to this service record */
            if (pResolveContext->sdRef != OpcUa_Null)
            {
                OpcUa_Socket_Close(pResolveContext->uaSocket);
                DNSServiceRefDeallocate(pResolveContext->sdRef);
                pResolveContext->uaSocket = OpcUa_Null;
                pResolveContext->sdRef = OpcUa_Null;
            }

            OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
            OpcUa_Free(pResolveContext);
            pResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
        }
        OpcUa_List_Leave(&g_lstServers);
        OpcUa_List_Clear(&g_lstServers);

        // remove element from g_findServersSocketList
        OpcUa_List_Enter(&g_findServersSocketList);
        OpcUa_List_ResetCurrent(&g_findServersSocketList);
        MulticastSocketCallbackStruct* socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetCurrentElement(&g_findServersSocketList);
        while (socketCallbackStruct)
        {
            OpcUa_Free(socketCallbackStruct);
            socketCallbackStruct = (MulticastSocketCallbackStruct*)OpcUa_List_GetNextElement(&g_findServersSocketList);
        }
        OpcUa_List_Leave(&g_findServersSocketList);

        OpcUa_List_Clear(&g_findServersSocketList);
    }
}


/** FindDnsServices Service implementation.
* @param hEndpoint OPC UA Endpoint handle.
* @param hContext  Service context.
* @param ppRequest Pointer to decoded service request structure.
* @param pRequestType The service type.
*
* @return OpcUa_StatusCode
*/
OpcUa_StatusCode ualds_findserversonnetwork(OpcUa_Endpoint         hEndpoint,
                                            OpcUa_Handle           hContext,
                                            OpcUa_Void           **ppRequest,
                                            OpcUa_EncodeableType  *pRequestType)
{
    OpcUa_FindServersOnNetworkRequest   *pRequest;
    OpcUa_FindServersOnNetworkResponse  *pResponse;
    OpcUa_EncodeableType                *pResponseType = 0;
    OpcUa_StatusCode                     uStatus = OpcUa_Good;
    OpcUa_UInt32                         i;
    OpcUa_Boolean                        bIncludeRecord;
    UALDS_UNUSED(pRequestType);

    OpcUa_ReturnErrorIfArgumentNull(ppRequest);
    pRequest = *ppRequest;

    uStatus = OpcUa_Endpoint_BeginSendResponse(hEndpoint,
                                               hContext,
                                               (OpcUa_Void**)&pResponse,
                                               &pResponseType);
    OpcUa_ReturnErrorIfBad(uStatus);

    if (pResponse)
    {
        if (OpcUa_IsGood(uStatus))
        {
            /* fill results */
            ualds_resolveContext *pResolveContext = OpcUa_Null;

            pResponse->LastCounterResetTime = g_lastCounterResetTime;

            OpcUa_List_Enter(&g_lstServers);
            OpcUa_List_ResetCurrent(&g_lstServers);
            pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
            while (pResolveContext)
            {
                // check if DiscoveryUrl is ready
                if ( (OpcUa_String_IsNull(&(pResolveContext->record.DiscoveryUrl)) == OpcUa_True) ||
                    (OpcUa_String_IsEmpty(&(pResolveContext->record.DiscoveryUrl)) == OpcUa_True))
                {
                    bIncludeRecord = OpcUa_False;
                }
                else 
                {
                    bIncludeRecord = OpcUa_True;

                    // check if DiscoveryUrl is not just the scheme, but has a resolved name...
                    for (i = 0; i < g_noOfServiceTypes; i++)
                    {
                        if (OpcUa_StrnCmpA(g_pszServiceSchemes[i], OpcUa_String_GetRawString(&pResolveContext->record.DiscoveryUrl),
                            OpcUa_String_StrLen(&pResolveContext->record.DiscoveryUrl)) == 0)
                        {
                            ualds_log(UALDS_LOG_DEBUG, "ualds_findserversonnetwork: skip record as discovery url is not set.");
                            bIncludeRecord = OpcUa_False;
                            break;
                        }
                    }
                }
                
                if (bIncludeRecord != OpcUa_False)
                {
                    if (pRequest->StartingRecordId != 0 &&
                        pResolveContext->record.RecordId <= pRequest->StartingRecordId)
                    {
                        ualds_log(UALDS_LOG_DEBUG, "ualds_findserversonnetwork: skip record as StartingRecordId is set (%u)", pRequest->StartingRecordId);
                        bIncludeRecord = OpcUa_False;
                    }
                    else if (pRequest->NoOfServerCapabilityFilter > 0)
                    {
                        OpcUa_Int32 iFilter = 0, iCap = 0;
                        for (iFilter = 0; iFilter < pRequest->NoOfServerCapabilityFilter; iFilter++)
                        {
                            OpcUa_Boolean bFoundCap = OpcUa_False;
                            OpcUa_String *pFilter = &pRequest->ServerCapabilityFilter[iFilter];
                            for (iCap = 0; iCap < pResolveContext->record.NoOfServerCapabilities; iCap++)
                            {
                                OpcUa_String *pCap = &pResolveContext->record.ServerCapabilities[iCap];
                                if (OpcUa_String_StrnCmp(pFilter, pCap, OPCUA_STRING_LENDONTCARE, OpcUa_True) == 0)
                                {
                                    bFoundCap = OpcUa_True;
                                    break;
                                }
                            }
                            if (bFoundCap == OpcUa_False)
                            {
                                // No caps match. Check if NA is requested and there are no caps available
                                if (pResolveContext->record.NoOfServerCapabilities == 0 &&
                                    (OpcUa_String_StrnCmp(pFilter, OpcUa_String_FromCString("NA"), OPCUA_STRING_LENDONTCARE, OpcUa_True) == 0))
                                {
                                    bFoundCap = OpcUa_True;
                                    break;
                                }
                                else
                                {
                                    bIncludeRecord = OpcUa_False;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (bIncludeRecord != OpcUa_False)
                {
                    OpcUa_ServerOnNetwork *pTmpServices = OpcUa_Null;

                    if (pRequest->MaxRecordsToReturn != 0 &&
                        (OpcUa_UInt32)pResponse->NoOfServers >= pRequest->MaxRecordsToReturn)
                    {
                        ualds_log(UALDS_LOG_DEBUG, "ualds_findserversonnetwork: max number of records to return reached (%i)", pResponse->NoOfServers);
                        break;
                    }

                    pResponse->NoOfServers++;
                    pTmpServices = (OpcUa_ServerOnNetwork*)OpcUa_ReAlloc(pResponse->Servers,
                                                                         pResponse->NoOfServers * sizeof(OpcUa_ServerOnNetwork));

                    if (pTmpServices)
                    {
                        OpcUa_UInt32 uPos = pResponse->NoOfServers-1;
                        pResponse->Servers = pTmpServices;

                        OpcUa_ServerOnNetwork_Initialize(&pResponse->Servers[uPos]);
                        pResponse->Servers[uPos].RecordId = pResolveContext->record.RecordId;
                        OpcUa_String_StrnCpy(&pResponse->Servers[uPos].ServerName, &pResolveContext->record.ServerName, OPCUA_STRING_LENDONTCARE);
                        OpcUa_String_StrnCpy(&pResponse->Servers[uPos].DiscoveryUrl, &pResolveContext->record.DiscoveryUrl, OPCUA_STRING_LENDONTCARE);
                        pResponse->Servers[uPos].NoOfServerCapabilities = pResolveContext->record.NoOfServerCapabilities;
                        if (pResolveContext->record.NoOfServerCapabilities > 0)
                        {
                            OpcUa_Int32 iCap = 0;
                            pResponse->Servers[uPos].ServerCapabilities = (OpcUa_String*)OpcUa_Alloc(pResolveContext->record.NoOfServerCapabilities * sizeof(OpcUa_String));
                            for (iCap = 0; iCap < pResolveContext->record.NoOfServerCapabilities; iCap++)
                            {
                                OpcUa_String_Initialize(&pResponse->Servers[uPos].ServerCapabilities[iCap]);
                                OpcUa_String_StrnCpy(&pResponse->Servers[uPos].ServerCapabilities[iCap], &pResolveContext->record.ServerCapabilities[iCap], OPCUA_STRING_LENDONTCARE);
                            }
                        }
                    }
                    else
                    {
                        OpcUa_Int32 i;
                        ualds_log(UALDS_LOG_ERR, "ualds_findserversonnetwork: Could not allocate memory for pResponse->Servers");
                        uStatus = OpcUa_BadOutOfMemory;
                        pResponse->NoOfServers--;
                        for (i = 0; i < pResponse->NoOfServers; i++)
                        {
                            OpcUa_ServerOnNetwork_Clear(&pResponse->Servers[i]);
                        }
                        pResponse->NoOfServers = 0;
                        break;
                    }
                }

                pResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
            }
            OpcUa_List_Leave(&g_lstServers);
        }

        UALDS_BUILDRESPONSEHEADER;

        /* Send response */
        OpcUa_Endpoint_EndSendResponse(
            hEndpoint,
            &hContext,
            uStatus,
            pResponse,
            pResponseType);

        /* free response */
        OpcUa_FindServersOnNetworkResponse_Clear(pResponse);
        OpcUa_Free(pResponse);

        /* we have sent a response, either with good or bad status, this does not matter.
         * we must return Good now, otherwise the stack will send an error message.
         */
        uStatus = OpcUa_Good;
    }

    return uStatus;
}

void ualds_zeroconf_register_offline(const char *szServerUri)
{
    ualds_settings_begingroup(szServerUri);

    /* get Server name */
    char szMDNSServerName[UALDS_CONF_MAX_URI_LENGTH];
    szMDNSServerName[0] = 0;
    ualds_settings_readstring("MdnsServerName", szMDNSServerName, UALDS_CONF_MAX_URI_LENGTH);
    if (szMDNSServerName[0] == 0)
    {
        int numNames = 0, i = 0;
        ualds_settings_beginreadarray("ServerNames", &numNames);
        for (i = 0; i < numNames; i++)
        {
            ualds_settings_setarrayindex(i);
            ualds_settings_readstring("Text", szMDNSServerName, UALDS_CONF_MAX_URI_LENGTH);
            if (szMDNSServerName[0] == 0)
            {
                break;
            }
        }
        ualds_settings_endarray();
    }

    // replace [gethostname] if necessary
    char szHostname[UALDS_CONF_MAX_URI_LENGTH];
    ualds_platform_getfqhostname(szHostname, UALDS_CONF_MAX_URI_LENGTH);
    replace_string(szMDNSServerName, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", szHostname);

    // get number of Capabilities
    int numCaps = 0;
    char** capabilities = OpcUa_Null;
    ualds_settings_beginreadarray("ServerCapabilities", &numCaps);
    if (numCaps > 0)
    {
        capabilities = OpcUa_Alloc(numCaps * sizeof(char*));
        OpcUa_MemSet(capabilities, 0, numCaps * sizeof(char*));

        int k = 0;
        for (k = 0; k < numCaps; k++)
        {
            // get capability
            char szCapability[UALDS_CONF_MAX_URI_LENGTH];
            ualds_settings_setarrayindex(k);
            ualds_settings_readstring("Capability", szCapability, UALDS_CONF_MAX_URI_LENGTH);
            ualds_settings_endarray();

            capabilities[k] = OpcUa_Alloc(UALDS_CONF_MAX_URI_LENGTH);
            strlcpy(capabilities[k], szCapability, UALDS_CONF_MAX_URI_LENGTH);
        }
    }

    // read number of DiscoveryURL
    int numDiscoveryUrls = 0;
    int j = 0;

    ualds_settings_beginreadarray("DiscoveryUrls", &numDiscoveryUrls);
    if (numDiscoveryUrls > 0)
    {
        for (j = 0; j < numDiscoveryUrls; j++)
        {
            ualds_resolveContext* pResolveContext = (ualds_resolveContext*)OpcUa_Alloc(sizeof(ualds_resolveContext));
            if (pResolveContext)
            {
                OpcUa_StatusCode uStatus = OpcUa_Good;
                uint16_t port = 0;

                pResolveContext->contextType = ContextType_Resolve;
                pResolveContext->uaSocket = OpcUa_Null;
                pResolveContext->sdRef = OpcUa_Null;
                OpcUa_ServerOnNetwork_Initialize(&pResolveContext->record);

                // get DiscoveryURL
                char szDiscoveryUrl[UALDS_CONF_MAX_URI_LENGTH];
                char szDiscoveryUrlFormated[UALDS_CONF_MAX_URI_LENGTH];
                ualds_settings_setarrayindex(j);
                ualds_settings_readstring("Url", szDiscoveryUrl, UALDS_CONF_MAX_URI_LENGTH);

                // replace [gethostname] if necessary
                replace_string(szDiscoveryUrl, UALDS_CONF_MAX_URI_LENGTH, "[gethostname]", szHostname);

                {
                    // split DiscoveryURL to individual components
                    // it will be reassembled with the expected format for "FindServersOnNetwork" service call
                    char *szScheme = OpcUa_Null;
                    char *tmpHostname = OpcUa_Null;
                    char *szPath = OpcUa_Null;
                    char hostName[UALDS_CONF_MAX_URI_LENGTH];
                    if (OpcUa_IsGood(ualds_parse_url(szDiscoveryUrl, &szScheme, &tmpHostname, &port, &szPath)))
                    {
                        strlcpy(hostName, tmpHostname, UALDS_CONF_MAX_URI_LENGTH);

                        /* If IP4 => convert it to hostname */
                        int isIP4 = is_Host_IP4Address(hostName);
                        if (isIP4 == 0)
                        {
                            ualds_log(UALDS_LOG_DEBUG, "ualds_zeroconf_registerOffline: Found IP4: '%s'. Converting it to hostname.",
                                hostName);
                            int convertSuccess = ualds_platform_convertIP4ToHostname(hostName, UALDS_CONF_MAX_URI_LENGTH);
                            if (convertSuccess != 0)
                            {
                                ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_registerOffline: Convert IP4 to hostname failed for '%s'",
                                    hostName);

                                // cleanup
                                OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
                                OpcUa_Free(pResolveContext);

                                int i = 0;
                                if (capabilities)
                                {
                                    for (i = 0; i<numCaps; i++)
                                    {
                                        if (capabilities[i]) OpcUa_Free(capabilities[i]);
                                    }
                                    OpcUa_Free(capabilities);
                                }
                                ualds_settings_endgroup();
                                return;
                            }
                        }
                    }
                    else
                    {
                        // cleanup
                        OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
                        OpcUa_Free(pResolveContext);

                        int i = 0;
                        if (capabilities)
                        {
                            for (i = 0; i<numCaps; i++)
                            {
                                if (capabilities[i]) OpcUa_Free(capabilities[i]);
                            }
                            OpcUa_Free(capabilities);
                        }
                        ualds_settings_endgroup();
                        return;
                    }

                    /* Make sure we pass hostname.local. or a FQDN as registered host or else register or resolve will fail */
                    /* Check whether there is a domain label in the name */
                    char* domain = strrchr(hostName, '.');

                    if (domain)
                    {
                        if (strcmp(domain, ".") == 0)
                        {
                            // hostname.subdomain.domain.
                            *domain = '\0';
                            domain = strrchr(hostName, '.');
                        }

                        if (domain)
                        {
                            int tldCheck = isTLD(domain);
                            if (tldCheck != 0)
                            {
                                /* Not a Top Level Domain */

                                /*
                                Strip the name down to just the node name and make it link local.
                                */
                                domain = strchr(hostName, '.');
                                *domain = '\0';
                                domain = NULL; /* This will make below add .local to the node name left in hostName */
                            }
                        }
                    }

                    if (!domain)
                    {
                        /* If no domain labels in hostname, make link local by adding .local domain */
                        strlcat(hostName, ".local.", UALDS_CONF_MAX_URI_LENGTH);
                    }

                    // reassembled formated DiscoveryURL
                    memset(szDiscoveryUrlFormated, 0, UALDS_CONF_MAX_URI_LENGTH);

                    strlcpy(szDiscoveryUrlFormated, szScheme, UALDS_CONF_MAX_URI_LENGTH);
                    strlcat(szDiscoveryUrlFormated, "://", UALDS_CONF_MAX_URI_LENGTH);

                    strlcat(szDiscoveryUrlFormated, hostName, UALDS_CONF_MAX_URI_LENGTH);
                   
                    char szPort[40] = { 0 };
#if OPCUA_USE_SAFE_FUNCTIONS
                    OpcUa_SnPrintfA(szPort, 40, 40, ":%hu", port);
#else
                    OpcUa_SnPrintfA(szPort, 40, ":%hu", port);
#endif
                    strlcat(szDiscoveryUrlFormated, szPort, UALDS_CONF_MAX_URI_LENGTH);

                    if (szPath != OpcUa_Null)
                    {
                        strlcat(szDiscoveryUrlFormated, szPath, UALDS_CONF_MAX_URI_LENGTH);
                    }
                }

                // set DiscoveryURL
                OpcUa_String_Initialize(&pResolveContext->record.DiscoveryUrl);
                OpcUa_String_AttachCopy(&pResolveContext->record.DiscoveryUrl, szDiscoveryUrlFormated);

                //set Server name
                OpcUa_String_AttachCopy(&pResolveContext->record.ServerName, szMDNSServerName);

                // set RecordId
                pResolveContext->record.RecordId = g_currentRecordId++;
                if (g_currentRecordId == 0)
                {
                    ualds_resolveContext *pExistingResolveContext = OpcUa_Null;

                    g_lastCounterResetTime = OpcUa_DateTime_UtcNow();

                    /* reassign RecordIds to entries after overflow */
                    OpcUa_List_Enter(&g_lstServers);
                    OpcUa_List_ResetCurrent(&g_lstServers);
                    pExistingResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
                    while (pExistingResolveContext)
                    {
                        pExistingResolveContext->record.RecordId = g_currentRecordId++;
                        pExistingResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
                    }
                    pResolveContext->record.RecordId = g_currentRecordId++;
                    OpcUa_List_Leave(&g_lstServers);
                }

                // set capabilities
                if (numCaps > 0)
                {
                    pResolveContext->record.NoOfServerCapabilities = numCaps;
                    pResolveContext->record.ServerCapabilities = (OpcUa_String*)OpcUa_Alloc(numCaps * sizeof(OpcUa_String));
                    int k = 0;
                    for (k = 0; k < numCaps; k++)
                    {
                        OpcUa_String_Initialize(&pResolveContext->record.ServerCapabilities[k]);
                        OpcUa_String_AttachCopy(&pResolveContext->record.ServerCapabilities[k], capabilities[k]);
                    }
                }
                else
                {
                    if (port == 4840)
                    {
                        pResolveContext->record.NoOfServerCapabilities = 1;
                        pResolveContext->record.ServerCapabilities = (OpcUa_String*)OpcUa_Alloc(sizeof(OpcUa_String));
                        char szCapability[UALDS_CONF_MAX_URI_LENGTH] = "LDS";
                        OpcUa_String_Initialize(&pResolveContext->record.ServerCapabilities[0]);
                        OpcUa_String_AttachCopy(&pResolveContext->record.ServerCapabilities[0], szCapability);
                    }
                    else
                    {
                        pResolveContext->record.NoOfServerCapabilities = 1;
                        pResolveContext->record.ServerCapabilities = (OpcUa_String*)OpcUa_Alloc(sizeof(OpcUa_String));
                        char szCapability[UALDS_CONF_MAX_URI_LENGTH] = "NA";
                        OpcUa_String_Initialize(&pResolveContext->record.ServerCapabilities[0]);
                        OpcUa_String_AttachCopy(&pResolveContext->record.ServerCapabilities[0], szCapability);
                    }
                }
            }

            /* append pResolveContext to list of services */
            OpcUa_StatusCode uStatus = OpcUa_Good;
            OpcUa_List_Enter(&g_lstServers);
            uStatus = OpcUa_List_AddElementToEnd(&g_lstServers, pResolveContext);
            OpcUa_List_Leave(&g_lstServers);
            if (OpcUa_IsNotGood(uStatus))
            {
                ualds_log(UALDS_LOG_ERR, "ualds_zeroconf_registerOffliney: could not add pResolveContext to list");
                
                // cleanup
                OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
                OpcUa_Free(pResolveContext);

                int i = 0;
                if (capabilities)
                {
                    for (i = 0; i < numCaps; i++)
                    {
                        if (capabilities[i]) OpcUa_Free(capabilities[i]);
                    }
                    OpcUa_Free(capabilities);
                }
                ualds_settings_endgroup();
                return;
            }
        }
        ualds_settings_endarray();
    }

    /* cleanup */
    int i = 0;
    if (capabilities)
    {
        for (i = 0; i<numCaps; i++)
        {
            if (capabilities[i]) OpcUa_Free(capabilities[i]);
        }
        OpcUa_Free(capabilities);
    }

    ualds_settings_endgroup();
}

void ualds_zeroconf_unregister_offline(const char *szServerUri)
{
    ualds_settings_begingroup(szServerUri);

    /* get Server name */
    char szMDNSServerName[UALDS_CONF_MAX_URI_LENGTH];
    szMDNSServerName[0] = 0;
    ualds_settings_readstring("MdnsServerName", szMDNSServerName, UALDS_CONF_MAX_URI_LENGTH);
    if (szMDNSServerName[0] == 0)
    {
        int numNames = 0, i = 0;
        ualds_settings_beginreadarray("ServerNames", &numNames);
        for (i = 0; i < numNames; i++)
        {
            ualds_settings_setarrayindex(i);
            ualds_settings_readstring("Text", szMDNSServerName, UALDS_CONF_MAX_URI_LENGTH);
            if (szMDNSServerName[0] == 0)
            {
                break;
            }
        }
        ualds_settings_endarray();
    }
    ualds_settings_endgroup();

    OpcUa_List_Enter(&g_lstServers);
    
    for (;;)
    {
        OpcUa_Boolean bFoundEntry = OpcUa_False;

        OpcUa_List_ResetCurrent(&g_lstServers);

        ualds_resolveContext* pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
        while (pResolveContext)
        {
            if (OpcUa_StrCmpA(szMDNSServerName, OpcUa_String_GetRawString(&pResolveContext->record.ServerName)) == 0)
            {
                OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
                OpcUa_Free(pResolveContext);
                OpcUa_List_DeleteCurrentElement(&g_lstServers);
                bFoundEntry = OpcUa_True;
                break;
            }

            pResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
        }

        if (bFoundEntry == OpcUa_False)
        {
            break;
        }
    }
    
    OpcUa_List_Leave(&g_lstServers);
}

void ualds_zeroconf_load_offline()
{
    char** szUriArray = 0;
    int numServers = 0;
    int i = 0;

    ualds_expirationcheck();

    ualds_settings_begingroup("RegisteredServers");
    ualds_settings_beginreadarray("Servers", &numServers);
    if (numServers > 0)
    {
        szUriArray = OpcUa_Alloc(numServers * sizeof(char*));
        if (szUriArray)
        {
            OpcUa_MemSet(szUriArray, 0, numServers * sizeof(char*));
            for (i = 0; i<numServers; i++)
            {
                char szTmpUrl[UALDS_CONF_MAX_URI_LENGTH];
                ualds_settings_setarrayindex(i);
                ualds_settings_readstring("ServerUri", szTmpUrl, UALDS_CONF_MAX_URI_LENGTH);

                szUriArray[i] = OpcUa_Alloc(UALDS_CONF_MAX_URI_LENGTH);
                if (szUriArray[i])
                {
                    strlcpy(szUriArray[i], szTmpUrl, UALDS_CONF_MAX_URI_LENGTH);
                }
            }
        }
    }

    ualds_settings_endarray();
    ualds_settings_endgroup();

    for (i = 0; i < numServers; i++)
    {
        ualds_zeroconf_register_offline(szUriArray[i]);
    }

    /* cleanup */
    if (szUriArray)
    {
        for (i = 0; i < numServers; i++)
        {
            if (szUriArray[i])
            {
                OpcUa_Free(szUriArray[i]);
            }
        }
        OpcUa_Free(szUriArray);
    }
}

void ualds_zeroconf_cleanup_offline()
{
    OpcUa_List_Enter(&g_lstServers);
    OpcUa_List_ResetCurrent(&g_lstServers);
    ualds_resolveContext* pResolveContext = (ualds_resolveContext*)OpcUa_List_GetCurrentElement(&g_lstServers);
    while (pResolveContext)
    {
        OpcUa_ServerOnNetwork_Clear(&pResolveContext->record);
        OpcUa_Free(pResolveContext);
        pResolveContext = (ualds_resolveContext*)OpcUa_List_GetNextElement(&g_lstServers);
    }
    OpcUa_List_Leave(&g_lstServers);
    OpcUa_List_Clear(&g_lstServers);
}
