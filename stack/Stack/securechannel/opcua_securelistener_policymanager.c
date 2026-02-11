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

/* core */
#include <opcua.h>

#ifdef OPCUA_HAVE_SERVERAPI

#include <opcua_types.h>
#include <opcua_datetime.h>
#include <opcua_guid.h>
#include <opcua_list.h>

/* stackcore */
#include <opcua_securechannel.h>

/* security */
#include <opcua_tcpsecurechannel.h>
#include <opcua_securelistener.h>
#include <opcua_securelistener_policymanager.h>

/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_Create                                    */
/*==============================================================================*/
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_Create(
    OpcUa_SecureListener_PolicyManager** a_ppPolicyManager)
{
    OpcUa_SecureListener_PolicyManager* pPolicyMngr = OpcUa_Null;

OpcUa_InitializeStatus(OpcUa_Module_SecureListener, "PolicyManager_Create");

    OpcUa_ReturnErrorIfArgumentNull(a_ppPolicyManager);

    *a_ppPolicyManager = OpcUa_Null;

    pPolicyMngr = (OpcUa_SecureListener_PolicyManager*) OpcUa_Alloc(sizeof(OpcUa_SecureListener_PolicyManager));
    OpcUa_ReturnErrorIfAllocFailed(pPolicyMngr);

    uStatus = OpcUa_SecureListener_PolicyManager_Initialize(pPolicyMngr);
    OpcUa_GotoErrorIfBad(uStatus);

    if(pPolicyMngr->SecurityPolicies == OpcUa_Null)
    {
        OpcUa_SecureListener_PolicyManager_Delete(&pPolicyMngr);
    }

    *a_ppPolicyManager = pPolicyMngr;

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_Free(pPolicyMngr);
    pPolicyMngr = OpcUa_Null;

OpcUa_FinishErrorHandling;
}

/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_Initialize                                */
/*==============================================================================*/
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_Initialize(
    OpcUa_SecureListener_PolicyManager* a_pPolicyManager)
{
OpcUa_InitializeStatus(OpcUa_Module_SecureListener, "PolicyManager_Initialize");

    if (a_pPolicyManager == OpcUa_Null)
    {
        uStatus = OpcUa_BadInvalidArgument;
        OpcUa_GotoErrorIfBad(uStatus);
    }

    uStatus = OpcUa_List_Create(&(a_pPolicyManager->SecurityPolicies));
    OpcUa_ReturnErrorIfBad(uStatus);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
OpcUa_FinishErrorHandling;
}

/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_ClearAll                                  */
/*==============================================================================*/
OpcUa_Void OpcUa_SecureListener_PolicyManager_ClearAll(
    OpcUa_SecureListener_PolicyManager* a_pPolicyManager)
{
    /* OpcUa_UInt32    uModule = OpcUa_Module_SecureListener; */
    /* OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "SecureListener - PolicyManager_ClearAll\n"); */
    OpcUa_SecureListener_PolicyManager_ClearSecurityPolicyConfigurations(a_pPolicyManager);
}


/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_ClearSecurityPolicies                     */
/*==============================================================================*/
OpcUa_Void OpcUa_SecureListener_PolicyManager_ClearSecurityPolicyConfigurations(
    OpcUa_SecureListener_PolicyManager* a_pPolicyManager)
{
    if(a_pPolicyManager->SecurityPolicies)
    {
        OpcUa_SecureListener_SecurityPolicyConfiguration* pPolicyConfiguration = OpcUa_Null;

        OpcUa_List_Enter(a_pPolicyManager->SecurityPolicies);

        /* delete all elements */
        OpcUa_List_ResetCurrent(a_pPolicyManager->SecurityPolicies);
        pPolicyConfiguration = (OpcUa_SecureListener_SecurityPolicyConfiguration*)OpcUa_List_GetCurrentElement(a_pPolicyManager->SecurityPolicies);
        while(pPolicyConfiguration != OpcUa_Null)
        {
            OpcUa_List_DeleteCurrentElement(a_pPolicyManager->SecurityPolicies);
            OpcUa_String_Clear(&pPolicyConfiguration->sSecurityPolicy);
            OpcUa_Free(pPolicyConfiguration);

            pPolicyConfiguration = (OpcUa_SecureListener_SecurityPolicyConfiguration*)OpcUa_List_GetCurrentElement(a_pPolicyManager->SecurityPolicies);
        }

        OpcUa_List_Leave(a_pPolicyManager->SecurityPolicies);

        OpcUa_List_Delete(&(a_pPolicyManager->SecurityPolicies));
    }
}


/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_Delete                                    */
/*==============================================================================*/
OpcUa_Void OpcUa_SecureListener_PolicyManager_Delete(
    OpcUa_SecureListener_PolicyManager** a_ppPolicyManager)
{
    /* OpcUa_UInt32    uModule = OpcUa_Module_SecureListener; */
    /* OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "SecureListener - PolicyManager_Delete\n"); */

    if (a_ppPolicyManager != OpcUa_Null)
    {
        OpcUa_SecureListener_PolicyManager_ClearAll(*a_ppPolicyManager);

        OpcUa_Free(*a_ppPolicyManager);
        *a_ppPolicyManager = OpcUa_Null;
    }
}

/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_IsValidSecurityPolicy                     */
/*==============================================================================*/
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_IsValidSecurityPolicy(
    OpcUa_SecureListener_PolicyManager* a_pPolicyManager,
    OpcUa_String*                       a_pSecurityPolicyUri)
{
    OpcUa_SecureListener_SecurityPolicyConfiguration*   pTmpSecurityPolicyUri   = OpcUa_Null;
    OpcUa_Int32                                         cmpResult               = 1;

OpcUa_InitializeStatus(OpcUa_Module_SecureListener, "PolicyManager_IsValidSecurityPolicy");

    OpcUa_List_Enter(a_pPolicyManager->SecurityPolicies);

    uStatus = OpcUa_List_ResetCurrent(a_pPolicyManager->SecurityPolicies);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_BadSecurityPolicyRejected;

    pTmpSecurityPolicyUri = (OpcUa_SecureListener_SecurityPolicyConfiguration*)OpcUa_List_GetCurrentElement(a_pPolicyManager->SecurityPolicies);

    while(pTmpSecurityPolicyUri)
    {
        cmpResult = OpcUa_String_StrnCmp(   &pTmpSecurityPolicyUri->sSecurityPolicy,
                                            a_pSecurityPolicyUri,
                                            OPCUA_STRING_LENDONTCARE,
                                            OpcUa_False);

        if(cmpResult == 0)
        {
            OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "SecureListener - PolicyManager_IsValidSecurityPolicy: Searched security policy found!\n");
            uStatus = OpcUa_Good;
            break;
        }

        pTmpSecurityPolicyUri = (OpcUa_SecureListener_SecurityPolicyConfiguration*)OpcUa_List_GetNextElement(a_pPolicyManager->SecurityPolicies);
    }

    if(cmpResult != 0)
    {
        OpcUa_Trace(OPCUA_TRACE_LEVEL_DEBUG, "SecureListener - PolicyManager_IsValidSecurityPolicy: Searched security policy NOT found!\n");
    }

    OpcUa_List_Leave(a_pPolicyManager->SecurityPolicies);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;
    OpcUa_List_Leave(a_pPolicyManager->SecurityPolicies);
OpcUa_FinishErrorHandling;
}

/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_IsValidSecurityPolicy                     */
/*==============================================================================*/
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_IsValidSecurityPolicyConfiguration(
    OpcUa_SecureListener_PolicyManager*                 a_pPolicyManager,
    OpcUa_SecureListener_SecurityPolicyConfiguration*   a_pSecurityPolicyConfiguration)
{
    OpcUa_SecureListener_SecurityPolicyConfiguration*   pCurrentSecConfig   = OpcUa_Null;
    OpcUa_Int32                                         iCmpResult          = 0;

OpcUa_InitializeStatus(OpcUa_Module_SecureListener, "PolicyManager_IsValidSecurityPolicyConfiguration");

    OpcUa_List_Enter(a_pPolicyManager->SecurityPolicies);

    uStatus = OpcUa_List_ResetCurrent(a_pPolicyManager->SecurityPolicies);
    OpcUa_GotoErrorIfBad(uStatus);

    uStatus = OpcUa_BadSecurityPolicyRejected;

    pCurrentSecConfig = (OpcUa_SecureListener_SecurityPolicyConfiguration*)OpcUa_List_GetCurrentElement(a_pPolicyManager->SecurityPolicies);
    while(pCurrentSecConfig != OpcUa_Null)
    {
        iCmpResult = OpcUa_String_StrnCmp(  &pCurrentSecConfig->sSecurityPolicy,
                                            &a_pSecurityPolicyConfiguration->sSecurityPolicy,
                                            OPCUA_STRING_LENDONTCARE,
                                            OpcUa_False);

        if(iCmpResult == 0)
        {
            /* policy found, now compare message security modes */
            if((pCurrentSecConfig->uMessageSecurityModes & a_pSecurityPolicyConfiguration->uMessageSecurityModes) != 0 )
            {
                /* complete match */
                uStatus = OpcUa_Good;
                break;
            }
            else
            {
                /* message security mode not found */
                uStatus = OpcUa_BadSecurityModeRejected;
            }
        }

        pCurrentSecConfig = (OpcUa_SecureListener_SecurityPolicyConfiguration*)OpcUa_List_GetNextElement(a_pPolicyManager->SecurityPolicies);
    }

    OpcUa_List_Leave(a_pPolicyManager->SecurityPolicies);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_List_Leave(a_pPolicyManager->SecurityPolicies);

OpcUa_FinishErrorHandling;
}

/*==============================================================================*/
/* OpcUa_SecureListener_PolicyManager_AddSecurityPolicyConfiguration            */
/*==============================================================================*/
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_AddSecurityPolicyConfiguration(
    OpcUa_SecureListener_PolicyManager*                 a_pPolicyManager,
    OpcUa_SecureListener_SecurityPolicyConfiguration*   a_pPolicyConfiguration)
{
    OpcUa_SecureListener_SecurityPolicyConfiguration*   pPolicyConfiguration    = OpcUa_Null;
    OpcUa_StringA                                       sPolicyUri              = OpcUa_Null;
    OpcUa_UInt32                                        uPolicyUriLength        = 0;

OpcUa_InitializeStatus(OpcUa_Module_SecureListener, "OpcUa_SecureListener_PolicyManager_AddSecurityPolicyConfiguration");

    OpcUa_ReturnErrorIfArgumentNull(a_pPolicyManager);
    OpcUa_ReturnErrorIfArgumentNull(a_pPolicyManager->SecurityPolicies);
    OpcUa_ReturnErrorIfArgumentNull(a_pPolicyConfiguration);

    pPolicyConfiguration = (OpcUa_SecureListener_SecurityPolicyConfiguration*)OpcUa_Alloc(sizeof(OpcUa_SecureListener_SecurityPolicyConfiguration));
    OpcUa_ReturnErrorIfAllocFailed(pPolicyConfiguration);
    OpcUa_MemSet(pPolicyConfiguration, 0, sizeof(OpcUa_SecureListener_SecurityPolicyConfiguration));
    OpcUa_String_Initialize(&pPolicyConfiguration->sSecurityPolicy);

    OpcUa_List_Enter(a_pPolicyManager->SecurityPolicies);

    sPolicyUri         = OpcUa_String_GetRawString(&a_pPolicyConfiguration->sSecurityPolicy);
    uPolicyUriLength   = OpcUa_String_StrLen(&a_pPolicyConfiguration->sSecurityPolicy);

    uStatus = OpcUa_String_AttachToString(  sPolicyUri,
                                            uPolicyUriLength,
                                            0,
                                            OpcUa_True,
                                            OpcUa_True,
                                            &pPolicyConfiguration->sSecurityPolicy);
    OpcUa_GotoErrorIfBad(uStatus);

    pPolicyConfiguration->uMessageSecurityModes = a_pPolicyConfiguration->uMessageSecurityModes;

    uStatus = OpcUa_List_AddElement(    a_pPolicyManager->SecurityPolicies,
                                        (OpcUa_Void*)pPolicyConfiguration);
    OpcUa_GotoErrorIfBad(uStatus);

    OpcUa_List_Leave(a_pPolicyManager->SecurityPolicies);

OpcUa_ReturnStatusCode;
OpcUa_BeginErrorHandling;

    OpcUa_List_Leave(a_pPolicyManager->SecurityPolicies);
    OpcUa_String_Clear(&pPolicyConfiguration->sSecurityPolicy);
    OpcUa_Free(pPolicyConfiguration);

OpcUa_FinishErrorHandling;
}

#endif /* OPCUA_HAVE_SERVERAPI */
