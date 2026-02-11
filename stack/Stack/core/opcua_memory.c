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
#include <opcua_trace.h>

#include <opcua_memory.h>

#define OPCUA_P_MEMORY_ALLOC    OpcUa_ProxyStub_g_PlatformLayerCalltable->MemAlloc
#define OPCUA_P_MEMORY_REALLOC  OpcUa_ProxyStub_g_PlatformLayerCalltable->MemReAlloc
#define OPCUA_P_MEMORY_FREE     OpcUa_ProxyStub_g_PlatformLayerCalltable->MemFree
#define OPCUA_P_MEMORY_MEMCPY   OpcUa_ProxyStub_g_PlatformLayerCalltable->MemCpy

/*============================================================================
 * OpcUa_Memory_Alloc
 *===========================================================================*/
OpcUa_Void* OPCUA_DLLCALL OpcUa_Memory_Alloc(OpcUa_UInt32 nSize)
{
    return OPCUA_P_MEMORY_ALLOC(nSize);
}

/*============================================================================
 * OpcUa_Memory_ReAlloc
 *===========================================================================*/
OpcUa_Void* OPCUA_DLLCALL OpcUa_Memory_ReAlloc(   OpcUa_Void*     a_pBuffer,
                                                  OpcUa_UInt32    a_nSize)
{
    return OPCUA_P_MEMORY_REALLOC(  a_pBuffer,
                                    a_nSize);
}

/*============================================================================
 * OpcUa_Memory_Free
 *===========================================================================*/
OpcUa_Void OPCUA_DLLCALL OpcUa_Memory_Free(OpcUa_Void* a_pBuffer)
{
    if(a_pBuffer != OpcUa_Null)
    {
        OPCUA_P_MEMORY_FREE(a_pBuffer);
    }
}

/*============================================================================
 * OpcUa_Memory_MemCpy
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_Memory_MemCpy(   OpcUa_Void*     a_pBuffer,
                                        OpcUa_UInt32    a_nSizeInBytes,
                                        OpcUa_Void*     a_pSource,
                                        OpcUa_UInt32    a_nCount)
{

    return OPCUA_P_MEMORY_MEMCPY(   a_pBuffer,
                                    a_nSizeInBytes,
                                    a_pSource,
                                    a_nCount);
}

/*============================================================================
 * OpcUa_Memory_DestroySecretData
 *===========================================================================*/
OpcUa_Void OPCUA_DLLCALL OpcUa_Memory_DestroySecretData(OpcUa_Void*  a_pData,
                                                        OpcUa_UInt32 a_nBytes)
{
    if(OpcUa_ProxyStub_g_PlatformLayerCalltable->DestroySecretData != OpcUa_Null)
    {
        OpcUa_ProxyStub_g_PlatformLayerCalltable->DestroySecretData(a_pData,
                                                                    a_nBytes);
    }
}

