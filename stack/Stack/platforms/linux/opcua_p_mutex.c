/* ========================================================================
 * Copyright (c) 2005-2018 The OPC Foundation, Inc. All rights reserved.
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

#include <opcua_platformdefs.h>
#include <opcua.h>

#include <opcua_mutex.h>
#include <opcua_p_mutex.h>
#include <opcua_p_memory.h>
#include <string.h>
#include <stdlib.h>

/* see opcua_platformdefs.h */
#include <pthread.h>

/*============================================================================
 * Initialize the mutex.
 *===========================================================================*/

/* add errorchecking */

static
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_Mutex_Initialize(OpcUa_Mutex hMutex)
{
    OpcUa_StatusCode    uStatus     = OpcUa_Good;
    pthread_mutex_t*    pPosixMutex = (pthread_mutex_t*)(hMutex);
    int                 result;
    pthread_mutexattr_t att;

    /* initialize and set mutex attribute object */
    result = pthread_mutexattr_init(&att);
    if(result != 0)
    {
        return OpcUa_Bad;
    }

    result = pthread_mutexattr_settype(&att, PTHREAD_MUTEX_RECURSIVE_NP);
    if(result != 0)
    {
        pthread_mutexattr_destroy(&att);
        return OpcUa_Bad;
    }

    /* initialize mutex with the attribute object */
    result = pthread_mutex_init(pPosixMutex, &att); /* code */
    if(result != 0)
    {
        uStatus = OpcUa_Bad;
    }

    /* delete the temporary attribute object */
    result = pthread_mutexattr_destroy(&att); /* code */
    if(result != 0)
    {
        uStatus = OpcUa_Bad;
    }

    return uStatus;
}

/*============================================================================
 * Allocate the mutex.
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_Mutex_Create(OpcUa_Mutex* a_phMutex)
{
    OpcUa_StatusCode uStatus        = OpcUa_Good;
    OpcUa_Mutex      hMutex;

    if(a_phMutex == OpcUa_Null)
    {
        return OpcUa_BadInvalidArgument;
    }

    hMutex = (OpcUa_Mutex)OpcUa_P_Memory_Alloc(sizeof(pthread_mutex_t));
    OpcUa_ReturnErrorIfAllocFailed(hMutex);

    uStatus = OpcUa_P_Mutex_Initialize(hMutex);

    if(OpcUa_IsBad(uStatus))
    {
        OpcUa_P_Memory_Free(hMutex);
    }
    else
    {
        *a_phMutex = hMutex;
    }

    return uStatus;
}

/*============================================================================
 * Clear the mutex.
 *===========================================================================*/
static
OpcUa_Void OPCUA_DLLCALL OpcUa_P_Mutex_Clear(OpcUa_Mutex hMutex)
{
    if(hMutex != OpcUa_Null)
    {
        pthread_mutex_t*   pPosixMutex = (pthread_mutex_t*)hMutex;

        pthread_mutex_destroy(pPosixMutex);
    }
}

/*============================================================================
 * Clear and free the mutex.
 *===========================================================================*/
OpcUa_Void OPCUA_DLLCALL OpcUa_P_Mutex_Delete(OpcUa_Mutex* a_phMutex)
{
    if( a_phMutex == OpcUa_Null || *a_phMutex == OpcUa_Null)
    {
        return;
    }
    else
    {
        OpcUa_P_Mutex_Clear(*a_phMutex);
        OpcUa_P_Memory_Free(*a_phMutex);
        *a_phMutex = OpcUa_Null;
    }
}

/*============================================================================
 * Lock the mutex.
 *===========================================================================*/
OpcUa_Void OPCUA_DLLCALL OpcUa_P_Mutex_Lock(OpcUa_Mutex hMutex)
{
    if(hMutex != OpcUa_Null)
    {
        pthread_mutex_t*    pPosixMutex = (pthread_mutex_t*)hMutex;

        pthread_mutex_lock(pPosixMutex);
    }
}

/*============================================================================
 * Unlock the mutex.
 *===========================================================================*/
OpcUa_Void OPCUA_DLLCALL OpcUa_P_Mutex_Unlock(OpcUa_Mutex hMutex)
{
    if(hMutex != OpcUa_Null)
    {
        pthread_mutex_t*    pPosixMutex = (pthread_mutex_t*)hMutex;

        pthread_mutex_unlock(pPosixMutex);
    }
}
