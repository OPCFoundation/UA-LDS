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

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <memory.h>


#include <opcua_p_internal.h>
#include <opcua_p_thread.h>
#include <opcua_p_memory.h>
#include <opcua_p_openssl.h>


/*============================================================================
 * Port Layer Thread Main
 *===========================================================================*/

typedef struct _OpcUa_P_ThreadArg
{
    pthread_t                    hThread;
    OpcUa_PfnInternalThreadMain* pfnInternalThreadMain;
    OpcUa_Void*                  ThreadArgs;
} OpcUa_P_ThreadArg;

pthread_attr_t *opcua_p_thread_attr = NULL;

/**
* This is the function, the new thread starts with. The only thing to do here,
* is calling the InternalThreadMain from OpcUa_Thread.c and your internal stuff.
*/

static
void* pthread_start(void* args)
{
    OpcUa_P_ThreadArg*  p_P_ThreadArgs;
    OpcUa_Void*         pArgument;

    if(args == OpcUa_Null)
    {
        return OpcUa_Null;
    }

    p_P_ThreadArgs = (OpcUa_P_ThreadArg*)args;

    pArgument       = p_P_ThreadArgs->ThreadArgs;

    /* run stack thread! */
    p_P_ThreadArgs->pfnInternalThreadMain(pArgument);

#if OPCUA_REQUIRE_OPENSSL
    OpcUa_P_OpenSSL_Thread_Cleanup();
#endif

    pthread_exit(NULL);

    return OpcUa_Null;
}

/*============================================================================
 * Initialize Raw Thread.
 *===========================================================================*/
static
OpcUa_Void OpcUa_P_Thread_Initialize(OpcUa_RawThread RawThread)
{
    memset((OpcUa_Void*)RawThread, 0, sizeof(OpcUa_P_ThreadArg));
    return;
}

/*============================================================================
 * Create a platform thread
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_Thread_Create(OpcUa_RawThread* pRawThread)
{
    OpcUa_StatusCode uStatus = OpcUa_Good;
    *pRawThread = (OpcUa_RawThread)OpcUa_P_Memory_Alloc(sizeof(OpcUa_P_ThreadArg));
    OpcUa_ReturnErrorIfAllocFailed(*pRawThread);

    OpcUa_P_Thread_Initialize(*pRawThread);

    return uStatus;
}

/*============================================================================
 * Delete Raw Thread
 *===========================================================================*/
OpcUa_Void OpcUa_P_Thread_Delete(OpcUa_RawThread* pRawThread)
{
    pthread_join(*(pthread_t*)*pRawThread, NULL);
    OpcUa_P_Memory_Free(*pRawThread);
    *pRawThread = OpcUa_Null;
    return;
}

/*============================================================================
 * Create Thread
 *===========================================================================*/
OpcUa_StatusCode OpcUa_P_Thread_Start(  OpcUa_RawThread             pThread,
                                        OpcUa_PfnInternalThreadMain pfnStartFunction,
                                        OpcUa_Void*                 pArguments)
{
    OpcUa_Int32         apiResult;
    OpcUa_P_ThreadArg*  pThreadArguments;
    pthread_t*          phThread;

    if(pThread == OpcUa_Null)
    {
        return OpcUa_BadInvalidArgument;
    }

    pThreadArguments = (OpcUa_P_ThreadArg*)pThread;

    phThread = &pThreadArguments->hThread;
    pThreadArguments->pfnInternalThreadMain = pfnStartFunction;
    pThreadArguments->ThreadArgs            = pArguments;

    apiResult = pthread_create(phThread, opcua_p_thread_attr, pthread_start, pThreadArguments);

    switch(apiResult)
    {
    case EAGAIN:
        {
            return OpcUa_BadResourceUnavailable;
        }
    case 0:
        {
            return OpcUa_Good;
        }
    default:
        {
            return OpcUa_BadInternalError;
        }
    }
}

/*============================================================================
 * Send the thread to sleep.
 *===========================================================================*/
OpcUa_Void OpcUa_P_Thread_Sleep(OpcUa_UInt32 msecTimeout)
{
    usleep(msecTimeout * 1000);
}

/*============================================================================
 * Get Current Thread Id
 *===========================================================================*/
unsigned long OpcUa_P_Thread_GetCurrentThreadId(OpcUa_Void)
{
    return (unsigned long)pthread_self();
}
