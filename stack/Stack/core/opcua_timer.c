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
#include <opcua_list.h>
#include <opcua_utilities.h>

#include <opcua_timer.h>

/*============================================================================
 * Delete A Timer
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_Timer_Delete(OpcUa_Timer* a_phTimer)
{
    return OPCUA_P_TIMER_DELETE(a_phTimer);
}


/*============================================================================
 * Create A Timer
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_Timer_Create(  OpcUa_Timer*            a_phTimer,
                                                    OpcUa_UInt32            a_msecInterval,
                                                    OpcUa_Timer_Callback*   a_fpTimerCallback,
                                                    OpcUa_Timer_Callback*   a_fpKillCallback,
                                                    OpcUa_Void*             a_pvCallbackData)
{
    return OPCUA_P_TIMER_CREATE(    a_phTimer,
                                    a_msecInterval,
                                    a_fpTimerCallback,
                                    a_fpKillCallback,
                                    a_pvCallbackData);
}

