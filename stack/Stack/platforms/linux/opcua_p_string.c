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
#include <string.h>

/* UA platform definitions */
#include <opcua_p_internal.h>

/* own headers */
#include <opcua_string.h>
#include <opcua_p_string.h>

/*============================================================================
 * Copy uintCount Characters from a OpcUa_StringA to another OpcUa_StringA
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_String_strncpy(OpcUa_StringA strDestination, OpcUa_UInt32 uiDestSize, OpcUa_StringA strSource, OpcUa_UInt32 uiLength)
{
    if(strncpy(strDestination, strSource, uiLength) != strDestination)
    {
        return OpcUa_Bad;
    }

    if(uiDestSize > uiLength)
    {
        strDestination[uiLength] = '\0';
    }

    return OpcUa_Good;
}

/*============================================================================
 * Append uintCount Characters from a OpcUa_String to another OpcUa_String.
 *===========================================================================*/
OpcUa_StatusCode OPCUA_DLLCALL OpcUa_P_String_strncat(OpcUa_StringA strDestination, OpcUa_UInt32 uiDestSize, OpcUa_StringA strSource, OpcUa_UInt32 uiLength)
{
    OpcUa_ReferenceParameter(uiDestSize);

    if(strncat(strDestination, strSource, uiLength) != strDestination)
    {
        return OpcUa_Bad;
    }

    return OpcUa_Good;
}

/*============================================================================
 * Get the Length from a OpcUa_String
 *===========================================================================*/
OpcUa_Int32 OPCUA_DLLCALL OpcUa_P_String_strlen(OpcUa_StringA a_pCString)
{
    return (OpcUa_Int32) strlen(a_pCString);
}

/*============================================================================
 * Compare two OpcUa_Strings Case Sensitive
 *===========================================================================*/
OpcUa_Int32 OPCUA_DLLCALL OpcUa_P_String_strncmp(OpcUa_StringA string1, OpcUa_StringA string2, OpcUa_UInt32 uiLength)
{
    return (OpcUa_Int32)strncmp(string1, string2, uiLength);
}

/*============================================================================
* Compare two OpcUa_Strings NOT Case Sensitive
*===========================================================================*/
OpcUa_Int32 OPCUA_DLLCALL OpcUa_P_String_strnicmp(OpcUa_StringA string1, OpcUa_StringA string2, OpcUa_UInt32 uiLength)
{
    return (OpcUa_Int32) strncasecmp(string1, string2, uiLength);
}

/*============================================================================
 * Write Values to a OpcUa_String
 *===========================================================================*/
OpcUa_Int32 OPCUA_DLLCALL OpcUa_P_String_vsnprintf(OpcUa_StringA sDest, OpcUa_UInt32 nCount, const OpcUa_StringA sFormat, varg_list vaList)
{
    OpcUa_Int   nRetval;

    nRetval = vsnprintf(sDest, nCount, sFormat, vaList);
    if (nRetval >= (OpcUa_Int)nCount)
    {
        /* In case of truncation, we do not follow C99, and return -1 instead. */
        nRetval = -1;
    }

    return (OpcUa_Int32)nRetval;
}


/*============================================================================
 * Write Values to a OpcUa_String
 *===========================================================================*/
/* OPCUA_DLLCALL with varglist won't work ... -> vsnprintf */
OpcUa_Int32 OPCUA_DLLCALL OpcUa_P_String_snprintf(  OpcUa_StringA a_sTarget,
                                                    OpcUa_UInt32  a_nCount,
                                                    OpcUa_StringA a_sFormat,
                                                    ...)
{
    OpcUa_Int   nRetval;
    va_list     vaList;

    va_start(vaList, a_sFormat);
    nRetval = vsnprintf(a_sTarget, a_nCount, a_sFormat, vaList);
    va_end( vaList );

    return (OpcUa_Int32)nRetval;
}
