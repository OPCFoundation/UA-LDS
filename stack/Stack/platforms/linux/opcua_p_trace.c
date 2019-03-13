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

/* System Headers */
 #include <stdio.h>

/* UA platform definitions */
#include <opcua_p_internal.h>

/* additional UA dependencies */
#include <opcua_p_mutex.h>
#include <opcua_p_thread.h>

/* own headers */
#include <opcua_p_trace.h>
#include <opcua_p_datetime.h>

OPCUA_EXPORT OpcUa_P_TraceHook g_OpcUa_P_TraceHook;

/*============================================================================
 * Trace Initialize
 *===========================================================================*/
/**
* Initialize all resources needed for tracing.
*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_Trace_Initialize(OpcUa_Void)
{
    return OpcUa_Good;
}

/*============================================================================
 * Trace Clear
 *===========================================================================*/
/**
* Clear all resources needed for tracing.
*/
OpcUa_Void OPCUA_DLLCALL OpcUa_P_Trace_Clear(OpcUa_Void)
{
    return;
}

/*============================================================================
 * Tracefunction
 *===========================================================================*/
/**
 * Writes the given string to the trace device, if the given trace level is
 * activated in the header file.
 */

OpcUa_Void OPCUA_DLLCALL OpcUa_P_Trace(
#if OPCUA_TRACE_FILE_LINE_INFO
                                        OpcUa_UInt32 level,
                                        OpcUa_CharA* sFile,
                                        OpcUa_UInt32 line,
#endif
                                        OpcUa_CharA* a_sMessage)
{
#if OPCUA_TRACE_FILE_LINE_INFO
    OpcUa_ReferenceParameter(level);
    OpcUa_ReferenceParameter(sFile);
    OpcUa_ReferenceParameter(line);
#endif

    /* send to tracehook if registered */
    if(g_OpcUa_P_TraceHook != OpcUa_Null)
    {
        g_OpcUa_P_TraceHook(a_sMessage);
    }
    else /* send to console */
    {
        char dtbuffer[25];
        OpcUa_DateTime timestamp;

        timestamp = OpcUa_P_DateTime_UtcNow();
        OpcUa_P_DateTime_GetStringFromDateTime(timestamp, dtbuffer, 25);

        printf("|%ld| %s %s", OpcUa_P_Thread_GetCurrentThreadId(), &dtbuffer[11], a_sMessage);
    }
}
