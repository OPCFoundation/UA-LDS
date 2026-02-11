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
#include <opcua_utilities.h>

#define OPCUA_P_QSORT           OpcUa_ProxyStub_g_PlatformLayerCalltable->qSort
#define OPCUA_P_BSEARCH         OpcUa_ProxyStub_g_PlatformLayerCalltable->bSearch
#define OPCUA_P_GETTICKCOUNT    OpcUa_ProxyStub_g_PlatformLayerCalltable->UtilGetTickCount
#define OPCUA_P_GETLASTERROR    OpcUa_ProxyStub_g_PlatformLayerCalltable->UtilGetLastError


/*============================================================================
 * Quick Sort
 *===========================================================================*/
OpcUa_StatusCode OpcUa_QSort(   OpcUa_Void*       a_pElements,
                                OpcUa_UInt32      a_nElementCount,
                                OpcUa_UInt32      a_nElementSize,
                                OpcUa_PfnCompare* a_pfnCompare,
                                OpcUa_Void*       a_pContext)
{
    if(     a_pElements     == OpcUa_Null
        ||  a_pfnCompare    == OpcUa_Null
        ||  a_nElementCount == 0
        ||  a_nElementSize  == 0)
    {
        return OpcUa_BadInvalidArgument;
    }

    OPCUA_P_QSORT(    a_pElements,
                      a_nElementCount,
                      a_nElementSize,
                      a_pfnCompare,
                      a_pContext);

    return OpcUa_Good;
}

/*============================================================================
 * Binary Search on sorted array
 *===========================================================================*/
OpcUa_Void* OpcUa_BSearch(  OpcUa_Void*       a_pKey,
                            OpcUa_Void*       a_pElements,
                            OpcUa_UInt32      a_nElementCount,
                            OpcUa_UInt32      a_nElementSize,
                            OpcUa_PfnCompare* a_pfnCompare,
                            OpcUa_Void*       a_pContext)
{
    if(     a_pElements     == OpcUa_Null
        ||  a_pKey          == OpcUa_Null
        ||  a_pfnCompare    == OpcUa_Null
        ||  a_nElementCount == 0
        ||  a_nElementSize  == 0)
    {
        return OpcUa_Null;
    }

    return OPCUA_P_BSEARCH( a_pKey,
                            a_pElements,
                            a_nElementCount,
                            a_nElementSize,
                            a_pfnCompare,
                            a_pContext);
}

/*============================================================================
 * Access to errno
 *===========================================================================*/
OpcUa_UInt32 OpcUa_GetLastError(OpcUa_Void)
{
    return OPCUA_P_GETLASTERROR();
}

/*============================================================================
 * OpcUa_GetTickCount
 *===========================================================================*/
OpcUa_UInt32 OpcUa_GetTickCount(OpcUa_Void)
{
    return OPCUA_P_GETTICKCOUNT();
}
