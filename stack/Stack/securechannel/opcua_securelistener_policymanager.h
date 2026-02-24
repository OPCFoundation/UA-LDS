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

#ifndef OpcUa_SecureListener_PolicyManager_
#define OpcUa_SecureListener_PolicyManager_ 1

#ifdef OPCUA_HAVE_SERVERAPI

OPCUA_BEGIN_EXTERN_C

/************************************************************************************/

/**
* @brief Being part of a specific SecureListener, it manages the secure channel and connections.
*/
struct _OpcUa_SecureListener_PolicyManager
{
    /* @brief A list with current SecurityPolicies (OpcUa_String) */
    OpcUa_List*     SecurityPolicies;
};

typedef struct _OpcUa_SecureListener_PolicyManager OpcUa_SecureListener_PolicyManager;

/************************************************************************************/

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_Create(
    OpcUa_SecureListener_PolicyManager** ppPolicyManager);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_Initialize(
    OpcUa_SecureListener_PolicyManager* pPolicyManager);

/* @brief */
OpcUa_Void OpcUa_SecureListener_PolicyManager_Delete(
    OpcUa_SecureListener_PolicyManager** ppPolicyManager);

/* @brief */
OpcUa_Void OpcUa_SecureListener_PolicyManager_ClearAll(
    OpcUa_SecureListener_PolicyManager* pPolicyManager);

/************************************************************************************/

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_IsValidSecurityPolicy(
    OpcUa_SecureListener_PolicyManager* pPolicyManager,
    OpcUa_String*                       pSecurityPolicyUri);

/* @brief */
OpcUa_Void OpcUa_SecureListener_PolicyManager_ClearSecurityPolicyConfigurations(
    OpcUa_SecureListener_PolicyManager* pPolicyManager);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_IsValidSecurityPolicyConfiguration(
    OpcUa_SecureListener_PolicyManager*                 pPolicyManager,
    OpcUa_SecureListener_SecurityPolicyConfiguration*   pSecurityPolicyConfiguration);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_AddSecurityPolicyConfiguration(
    OpcUa_SecureListener_PolicyManager*                 a_pPolicyManager,
    OpcUa_SecureListener_SecurityPolicyConfiguration*   a_pPolicyConfiguration);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_PolicyManager_GetAllSecurityPolicies(
    OpcUa_SecureListener_PolicyManager* pPolicyManager,
    OpcUa_UInt16*                       pNoOfSecurityPolicies,
    OpcUa_String**                      ppSecurityPolicyUris);

OPCUA_END_EXTERN_C

#endif /* OPCUA_HAVE_SERVERAPI */

#endif
