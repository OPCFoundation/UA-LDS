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

#ifndef OpcUa_SecureListener_ChannelManager_
#define OpcUa_SecureListener_ChannelManager_

#ifdef OPCUA_HAVE_SERVERAPI

OPCUA_BEGIN_EXTERN_C

typedef OpcUa_Void (OPCUA_DLLCALL OpcUa_SecureListener_ChannelManager_ChannelRemovedCallback)(
    OpcUa_SecureChannel* pSecureChannel,
    OpcUa_Void*          pvCallbackData);

typedef struct _OpcUa_SecureListener_ChannelManager OpcUa_SecureListener_ChannelManager;

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_Create(
    OpcUa_SecureListener_ChannelManager_ChannelRemovedCallback* pChannelTimeoutCallback,
    OpcUa_Void*                                                 pvChannelTimeoutCallbackData,
    OpcUa_SecureListener_ChannelManager**                       ppChannelManager);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_Initialize(
    OpcUa_SecureListener_ChannelManager_ChannelRemovedCallback* pChannelTimeoutCallback,
    OpcUa_Void*                                                 pvChannelTimeoutCallbackData,
    OpcUa_SecureListener_ChannelManager*                        pChannelManager);

/* @brief */
OpcUa_Void OpcUa_SecureListener_ChannelManager_Clear(
    OpcUa_SecureListener_ChannelManager* pChannelManager);

/* @brief */
OpcUa_Void OpcUa_SecureListener_ChannelManager_Delete(
    OpcUa_SecureListener_ChannelManager** ppChannelManager);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_IsValidChannelID(
    OpcUa_SecureListener_ChannelManager* pChannelManager,
    OpcUa_UInt32                         uSecureChannelID);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_AddChannel(
    OpcUa_SecureListener_ChannelManager* pChannelManager,
    OpcUa_SecureChannel*                 pChannel);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_ReleaseChannel(
    OpcUa_SecureListener_ChannelManager* pChannelManager,
    OpcUa_SecureChannel**                ppSecureChannel);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_SetSecureChannelID(
    OpcUa_SecureListener_ChannelManager* pChannelManager,
    OpcUa_SecureChannel*                 pSecureChannel,
    OpcUa_UInt32                         uSecureChannelID);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_SetTransportConnection(
    OpcUa_SecureListener_ChannelManager* pChannelManager,
    OpcUa_SecureChannel*                 pSecureChannel,
    OpcUa_Handle                         hTransportConnection);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_GetChannelBySecureChannelID(
    OpcUa_SecureListener_ChannelManager* pChannelManager,
    OpcUa_UInt32                         uSecureChannelID,
    OpcUa_SecureChannel**                ppSecureChannel);

/* @brief */
OpcUa_StatusCode OpcUa_SecureListener_ChannelManager_GetChannelByTransportConnection(
    OpcUa_SecureListener_ChannelManager* pChannelManager,
    OpcUa_Handle                         hTransportConnection,
    OpcUa_SecureChannel**                ppSecureChannel);

OPCUA_END_EXTERN_C

#endif /* OPCUA_HAVE_SERVERAPI */

#endif
