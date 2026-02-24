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

/******************************************************************************************************/
/* Platform Portability Layer                                                                         */
/* Modify the content of this file according to the semaphore implementation on your system.          */
/* This is the pthreads implementation.                                                               */
/* For Information about pthread see http://sourceware.org/pthreads-win32/.                           */
/* The waits are affected by thread cancelation!                                                      */
/******************************************************************************************************/

/* System Headers */
#include <windows.h>
#include <time.h>

/* UA platform definitions */
#include <opcua_p_internal.h>

/* additional UA dependencies */
#include <opcua_datetime.h>
#include <opcua_utilities.h>

/* own headers */
#include <opcua_semaphore.h>
#include <opcua_p_semaphore.h>


/*============================================================================
 * Create a Semaphore
 *===========================================================================*/
OpcUa_StatusCode    OPCUA_DLLCALL OpcUa_P_Semaphore_Create( OpcUa_Semaphore* a_Semaphore,
                                                            OpcUa_UInt32     a_uInitalValue,
                                                            OpcUa_UInt32     a_uMaxRange)
{
    HANDLE InternalSemaphore = NULL;

    *a_Semaphore = OpcUa_Null;

    if((a_uMaxRange == 0) || (a_uMaxRange < a_uInitalValue))
    {
        return OpcUa_BadInvalidArgument;
    }


    InternalSemaphore = CreateSemaphore(    NULL,
                                            a_uInitalValue,
                                            a_uMaxRange,
                                            (LPCTSTR)NULL);

    if(InternalSemaphore == NULL)
    {
        return OpcUa_BadInternalError;
    }

    *a_Semaphore = (OpcUa_Semaphore)InternalSemaphore;

    return OpcUa_Good;
}

/*============================================================================
 * Delete the semaphore.
 *===========================================================================*/
OpcUa_Void OPCUA_DLLCALL OpcUa_P_Semaphore_Delete(OpcUa_Semaphore* pRawSemaphore)
{
    if(pRawSemaphore == OpcUa_Null || *pRawSemaphore == OpcUa_Null)
    {
        return;
    }

    if(CloseHandle((HANDLE)*pRawSemaphore) != 0)
    {
        /* CloseHandle succeeded if it returned non-zero */
        *pRawSemaphore = OpcUa_Null;
    }
    else
    {
        /* CloseHandle failed if it returned zero */
    }
}

/*============================================================================
 * Wait
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_Semaphore_Wait(OpcUa_Semaphore RawSemaphore)
{
    HANDLE  InternalSemaphore = (HANDLE)RawSemaphore;
    DWORD   dwResult;

    dwResult = WaitForSingleObject(InternalSemaphore, INFINITE);

    if(dwResult == WAIT_TIMEOUT)
    {
        return OpcUa_BadInternalError;
    }
/* TODO: find return value */
/*
    else if(dwResult == WAIT_ABANDONED)
    {
    }
*/
    else if(dwResult == WAIT_OBJECT_0)
    {
        return OpcUa_Good;
    }
    else /*dwResult == WAIT_FAILED*/
    {
        return OpcUa_BadInternalError;
    }
}

/*============================================================================
 * Wait for a maximum of seconds.
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_Semaphore_TimedWait( OpcUa_Semaphore a_RawSemaphore,
                                                            OpcUa_UInt32    a_msecTimeout)
{
    HANDLE  InternalSemaphore = (HANDLE)a_RawSemaphore;
    DWORD   dwResult;

    dwResult = WaitForSingleObject(InternalSemaphore, a_msecTimeout);

    if(dwResult == WAIT_TIMEOUT)
    {
        return OpcUa_GoodNonCriticalTimeout;
    }
    else if(dwResult == WAIT_OBJECT_0)
    {
        return OpcUa_Good;
    }
    else /*dwResult == WAIT_FAILED*/
    {
        return OpcUa_BadInternalError;
    }
}

/*============================================================================
 * Post (free)
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_Semaphore_Post(  OpcUa_Semaphore RawSemaphore,
                                                        OpcUa_UInt32    uReleaseCount)
{
    HANDLE InternalSemaphore = (HANDLE)RawSemaphore;

    if(uReleaseCount < 1)
    {
        return OpcUa_BadInvalidArgument;
    }

    if(ReleaseSemaphore(InternalSemaphore, uReleaseCount, NULL))
    {
        return OpcUa_Good;
    }
    else
    {
        DWORD dwLastError = GetLastError();

        switch(dwLastError)
        {
        case ERROR_TOO_MANY_POSTS:
            {
                return OpcUa_BadTooManyPosts;
            }
        default:
            {
                return OpcUa_BadInternalError;
            }
        }
    }
}
