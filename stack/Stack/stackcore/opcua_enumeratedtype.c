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
#include <opcua_enumeratedtype.h>

/*============================================================================
 * OpcUa_EnumeratedType_FindName
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EnumeratedType_FindName(
    OpcUa_EnumeratedType* a_pType,
    OpcUa_Int32           a_nValue,
    OpcUa_StringA*        a_pName)
{
    OpcUa_UInt32 ii = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumeratedType_FindName");

    OpcUa_ReturnErrorIfArgumentNull(a_pType);
    OpcUa_ReturnErrorIfArgumentNull(a_pName);

    *a_pName = OpcUa_Null;

    for (ii = 0; a_pType->Values[ii].Name != OpcUa_Null; ii++)
    {
        if (a_pType->Values[ii].Value == a_nValue)
        {
            *a_pName = a_pType->Values[ii].Name;
            break;
        }
    }

    OpcUa_GotoErrorIfTrue(a_pType->Values[ii].Name == OpcUa_Null, OpcUa_BadInvalidArgument);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EnumeratedType_FindValue
 *===========================================================================*/
OPCUA_EXPORT OpcUa_StatusCode OpcUa_EnumeratedType_FindValue(
    OpcUa_EnumeratedType* a_pType,
    OpcUa_StringA         a_sName,
    OpcUa_Int32*          a_pValue)
{
    OpcUa_UInt32 ii = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EnumeratedType_FindValue");

    OpcUa_ReturnErrorIfArgumentNull(a_pType);
    OpcUa_ReturnErrorIfArgumentNull(a_sName);
    OpcUa_ReturnErrorIfArgumentNull(a_pValue);

    *a_pValue = 0;

    for (ii = 0; a_pType->Values[ii].Name != OpcUa_Null; ii++)
    {
        if (OpcUa_StrCmpA(a_pType->Values[ii].Name, a_sName) == 0)
        {
            *a_pValue = a_pType->Values[ii].Value;
            break;
        }
    }

    OpcUa_GotoErrorIfTrue(a_pType->Values[ii].Name == OpcUa_Null, OpcUa_BadInvalidArgument);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}
