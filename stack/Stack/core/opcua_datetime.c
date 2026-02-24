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
#include <opcua_datetime.h>

#define OpcUa_P_DateTime_UtcNow                 OpcUa_ProxyStub_g_PlatformLayerCalltable->UtcNow
#define OpcUa_P_DateTime_GetTimeOfDay           OpcUa_ProxyStub_g_PlatformLayerCalltable->GetTimeOfDay
#define OpcUa_P_DateTime_GetDateTimeFromString  OpcUa_ProxyStub_g_PlatformLayerCalltable->GetDateTimeFromString
#define OpcUa_P_DateTime_GetStringFromDateTime  OpcUa_ProxyStub_g_PlatformLayerCalltable->GetStringFromDateTime

/*============================================================================*/
OpcUa_StatusCode OpcUa_DateTime_GetTimeOfDay(OpcUa_TimeVal* a_pValue)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_DateTime);
    OpcUa_ReturnErrorIfArgumentNull(a_pValue);

    OpcUa_P_DateTime_GetTimeOfDay(a_pValue);

    return OpcUa_Good;
}

/*============================================================================*/
OpcUa_StatusCode OpcUa_DateTime_GetDateTimeFromString(  OpcUa_StringA   a_pchDateTimeString,
                                                        OpcUa_DateTime* a_pDateTime)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_DateTime);
    OpcUa_ReturnErrorIfArgumentNull(a_pchDateTimeString);
    OpcUa_ReturnErrorIfArgumentNull(a_pDateTime);

    return OpcUa_P_DateTime_GetDateTimeFromString(a_pchDateTimeString, a_pDateTime);
}

/*============================================================================*/
OpcUa_StatusCode OpcUa_DateTime_GetStringFromDateTime(  OpcUa_DateTime  a_dateTime,
                                                        OpcUa_StringA   a_pBuffer,
                                                        OpcUa_UInt32    a_uLength)
{
    OpcUa_DeclareErrorTraceModule(OpcUa_Module_DateTime);
    OpcUa_ReturnErrorIfArgumentNull(a_pBuffer);

    if(a_uLength < 25)
    {
        return OpcUa_BadInvalidArgument;
    }

    return OpcUa_P_DateTime_GetStringFromDateTime(a_dateTime, a_pBuffer, a_uLength);
}
