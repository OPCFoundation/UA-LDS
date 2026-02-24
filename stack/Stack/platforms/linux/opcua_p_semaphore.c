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

#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* UA platform definitions */
#include <opcua_p_internal.h>

/* additional UA dependencies */
#include <opcua_p_datetime.h>
#include <opcua_p_memory.h>
#include <opcua_utilities.h>

/* own headers */
#include <opcua_semaphore.h>
#include <opcua_p_semaphore.h>

/** Creates a new semaphore.
 * @param a_Semaphore Pointer to semaphore handle. This returns the newly created semaphore.
 * @param a_uInitalValue The initial value of the semaphore.
 * @param a_uMaxRange The maximum value of the semaphore. This has no effect on linux, because the system has no maximim limit. But the parameter is checked to make sense 0 &lt;= InitValue &lt; MaxRange.
 * @return OpcUa_Good if the semaphore could be created, OpcUa_BadInvalidArgument MaxRange is not plausible, or OpcUa_BadInternalError in case of a system call error.
 */
OpcUa_StatusCode    OpcUa_P_Semaphore_Create(OpcUa_Semaphore* a_Semaphore,
                                             OpcUa_UInt32   a_uInitalValue,
                                             OpcUa_UInt32   a_uMaxRange)
{
    sem_t* pInternalSemaphore = OpcUa_Null;

    if (a_uMaxRange == 0) return OpcUa_BadInvalidArgument;
    if (a_uMaxRange < a_uInitalValue) return OpcUa_BadInvalidArgument;

    *a_Semaphore = OpcUa_Null;

    pInternalSemaphore = (sem_t *)OpcUa_P_Memory_Alloc(sizeof(sem_t));

    if(pInternalSemaphore == OpcUa_Null)
    {
        return OpcUa_BadOutOfMemory;
    }

    if(sem_init(pInternalSemaphore, 0, (unsigned int)a_uInitalValue) != 0) /* 0 = process local semaphore */
    {
        OpcUa_P_Memory_Free(pInternalSemaphore);
        return OpcUa_BadInternalError;
    }

    *a_Semaphore = (OpcUa_Semaphore)pInternalSemaphore;

    return OpcUa_Good;
}

/** Deletes the semaphore. */
OpcUa_Void OpcUa_P_Semaphore_Delete(OpcUa_Semaphore* pRawSemaphore)
{
    sem_t*  pInternalSemaphore  = OpcUa_Null;

    if (pRawSemaphore == OpcUa_Null || *pRawSemaphore == OpcUa_Null) return;

    pInternalSemaphore = (sem_t*) *pRawSemaphore;

    sem_destroy(pInternalSemaphore);
    OpcUa_P_Memory_Free(pInternalSemaphore);
    *pRawSemaphore = OpcUa_Null;
}

/** Acquires a resource.
 * This function blocks until a resource could be acquired.
 * Use OpcUa_P_Semaphore_TimedWait if you don't want to block forever.
 * This function handles interruptions due to signals and automatically
 * restarts the wait operation.
 * @param RawSemaphore Handle to semaphore.
 * @return OpcUa_Good if the resource was successfully acquired,
 * OpcUa_BadInternalError in case of a system call error.
 */
OpcUa_StatusCode OpcUa_P_Semaphore_Wait(OpcUa_Semaphore RawSemaphore)
{
    sem_t* pInternalSemaphore = (sem_t*)RawSemaphore;
    int err;

    do
    {
        err = 0;
        if (sem_wait(pInternalSemaphore) == -1) err = errno;
    }
    while (err == EINTR);

    if(err != 0)
    {
        return OpcUa_BadInternalError;
    }

    return OpcUa_Good;
}

/** Acquires a resource.
 * This function behaves like OpcUa_P_Semaphore_Wait, but does not block forever.
 * In case of a timeout the function returns OpcUa_GoodNonCriticalTimeout.
 * @param RawSemaphore Handle to semaphore.
 * @param msecTimeout Maximum time to wait to acquire the resource.
 * @return OpcUa_Good if the resource was successfully acquired, OpcUa_GoodNonCriticalTimeout in case of a timeout,
 * OpcUa_BadInternalError in case of a system call error.
 */
OpcUa_StatusCode OpcUa_P_Semaphore_TimedWait(OpcUa_Semaphore RawSemaphore, OpcUa_UInt32 msecTimeout)
{
    sem_t*          pInternalSemaphore = (sem_t*)RawSemaphore;
    struct timespec Timeout;
    int err;

    if (msecTimeout == OpcUa_Infinite)
    {
        do
        {
            err = 0;
            if (sem_wait(pInternalSemaphore) == -1) err = errno;
        }
        while (err == EINTR);

        if (err != 0)
        {
            return OpcUa_BadInternalError;
        }
    }
    else
    {
        int sec  = msecTimeout / 1000;
        int nsec = (msecTimeout % 1000) * 1000000;

        if (clock_gettime(CLOCK_REALTIME, &Timeout) == -1)
        {
            return OpcUa_BadInternalError;
        }

        Timeout.tv_sec  += sec;
        Timeout.tv_nsec += nsec;
        if (Timeout.tv_nsec >= 1000000000)
        {
            Timeout.tv_nsec -= 1000000000;
            Timeout.tv_sec++;
        }

        do
        {
            err = 0;
            if (sem_timedwait(pInternalSemaphore, &Timeout) == -1) err = errno;
        }
        while (err == EINTR);

        if(err != 0)
        {
            if(errno == ETIMEDOUT)
            {
                return OpcUa_GoodNonCriticalTimeout;
            }
            else
            {
                return OpcUa_BadInternalError;
            }
        }
    }

    return OpcUa_Good;
}

/** Gives back a number of acquired resources.
 * This means it unblocks other blocking OpcUa_P_Semaphore_Wait or OpcUa_P_Semaphore_TimedWait calls.
 * @param RawSemaphore Handle to semaphore.
 * @param uReleaseCount Gives back uReleaseCount resources.
 * @return OpcUa_Good on success or OpcUa_BadInternalError in case of a system call error.
 */
OpcUa_StatusCode OpcUa_P_Semaphore_Post(OpcUa_Semaphore RawSemaphore,
                                        OpcUa_UInt32    uReleaseCount)
{
    sem_t* pInternalSemaphore = (sem_t*)RawSemaphore;

    if (uReleaseCount == 0) return OpcUa_BadInvalidArgument;

    while(uReleaseCount > 0)
    {
        if (sem_post(pInternalSemaphore) != 0)
        {
            return OpcUa_BadInternalError;
        }
        --uReleaseCount;
    }
    return OpcUa_Good;
}
