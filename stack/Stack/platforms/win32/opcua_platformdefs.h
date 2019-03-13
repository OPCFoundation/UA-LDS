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

/* This are the win32 platform definitions! */

#ifndef _OpcUa_PlatformDefs_H_
#define _OpcUa_PlatformDefs_H_ 1

/* System Headers */
#include <stdio.h>

#pragma warning (disable:4127) /* suppress "conditional expression is constant" in macros */

#ifdef __cplusplus
# define OPCUA_BEGIN_EXTERN_C extern "C" {
# define OPCUA_END_EXTERN_C }
#else
# define OPCUA_BEGIN_EXTERN_C
# define OPCUA_END_EXTERN_C
#endif

/*============================================================================
* Types and Mapping
*===========================================================================*/

OPCUA_BEGIN_EXTERN_C

#define LITTLE_ENDIAN

/* marks functions that need to be exported via functiontable */
#if defined(_UA_STACK_BUILD_DLL)
  #define OPCUA_EXPORT __declspec(dllexport)
#else
  #define OPCUA_EXPORT
#endif

#define OPCUA_EXPORT_SYNC_SERVER_API OPCUA_EXPORT

#if defined(_UA_STACK_USE_DLL)
  #define OPCUA_IMPORT __declspec(dllimport)
#else
  #define OPCUA_IMPORT
#endif

  #if defined(_UA_STACK_BUILD_DLL)
    #define OPCUA_IMEXPORT __declspec(dllexport)
  #elif defined(_UA_STACK_USE_DLL)
    #define OPCUA_IMEXPORT __declspec(dllimport)
  #else
    /* build/using static lib */
    #define OPCUA_IMEXPORT
  #endif

/* call exported functions by stdcall convention */
#ifdef _WIN32_WCE
  #define OPCUA_DLLCALL
#else
  #define OPCUA_DLLCALL __stdcall
#endif

/* calling convention used by stack functions that explicitly use cdecl */
#define OPCUA_CDECL __cdecl

/* used ie. for unlimited timespans */
#define OPCUA_INFINITE 0xFFFFFFFF

/* Some compilers support applying printf-style warnings. */
#define OPCUA_PRINTF_ARGS(formatIndex)
#define OPCUA_PRINTF_VALIST(formatIndex)

/*============================================================================
* Additional basic headers
*===========================================================================*/
#include <string.h>

/* configuration switches */
#include <opcua_config.h>

/* basic type mapping */
#include "opcua_p_types.h"

/**********************************************************************************/
/*/  Security Configuration section.                                             /*/
/**********************************************************************************/
#ifndef OPCUA_SUPPORT_SECURITYPOLICY_BASIC128RSA15
#define OPCUA_SUPPORT_SECURITYPOLICY_BASIC128RSA15  OPCUA_CONFIG_YES
#endif  /* OPCUA_SUPPORT_SECURITYPOLICY_BASIC128RSA15 */

#ifndef OPCUA_SUPPORT_SECURITYPOLICY_BASIC256
#define OPCUA_SUPPORT_SECURITYPOLICY_BASIC256       OPCUA_CONFIG_YES
#endif /* OPCUA_SUPPORT_SECURITYPOLICY_BASIC256 */

#ifndef OPCUA_SUPPORT_SECURITYPOLICY_BASIC256SHA256
#define OPCUA_SUPPORT_SECURITYPOLICY_BASIC256SHA256 OPCUA_CONFIG_YES
#endif /* OPCUA_SUPPORT_SECURITYPOLICY_BASIC256SHA256 */

#ifndef OPCUA_SUPPORT_SECURITYPOLICY_AES128SHA256RSAOAEP
#define OPCUA_SUPPORT_SECURITYPOLICY_AES128SHA256RSAOAEP OPCUA_CONFIG_YES
#endif /* OPCUA_SUPPORT_SECURITYPOLICY_AES128SHA256RSAOAEP */

#ifndef OPCUA_SUPPORT_SECURITYPOLICY_AES256SHA256RSAPSS
#define OPCUA_SUPPORT_SECURITYPOLICY_AES256SHA256RSAPSS OPCUA_CONFIG_YES
#endif /* OPCUA_SUPPORT_SECURITYPOLICY_AES256SHA256RSAPSS */

#ifndef OPCUA_SUPPORT_SECURITYPOLICY_NONE
#define OPCUA_SUPPORT_SECURITYPOLICY_NONE           OPCUA_CONFIG_YES
#endif  /* OPCUA_SUPPORT_SECURITYPOLICY_NONE */

#ifndef OPCUA_SUPPORT_PKI
#define OPCUA_SUPPORT_PKI                           OPCUA_CONFIG_YES
#endif  /* OPCUA_SUPPORT_PKI */

#if OPCUA_SUPPORT_PKI
#define OPCUA_SUPPORT_PKI_OVERRIDE                  OPCUA_CONFIG_YES
#define OPCUA_SUPPORT_PKI_OPENSSL                   OPCUA_CONFIG_YES
#ifdef _WIN32_WCE
#define OPCUA_SUPPORT_PKI_WIN32                     OPCUA_CONFIG_NO
#else
#define OPCUA_SUPPORT_PKI_WIN32                     OPCUA_CONFIG_YES
#endif
#endif /* OPCUA_SUPPORT_PKI */

/*============================================================================
 * Memory allocation functions.
 *
 * Note: Realloc and Free behave gracefully if passed a NULL pointer. Changing
 * these functions to a macro call to free will cause problems.
 *===========================================================================*/
 /* shortcuts for often used memory functions */


 /* shortcuts for often used memory functions */
#define OpcUa_Alloc(xSize)                              OpcUa_Memory_Alloc(xSize)
#define OpcUa_ReAlloc(xSrc, xSize)                      OpcUa_Memory_ReAlloc(xSrc, xSize)
#define OpcUa_Free(xSrc)                                OpcUa_Memory_Free(xSrc)
#define OpcUa_MemCpy(xDst, xDstSize, xSrc, xCount)      OpcUa_Memory_MemCpy(xDst, xDstSize, xSrc, xCount)

#define OpcUa_DestroySecretData(xDst, xSize)            OpcUa_Memory_DestroySecretData(xDst, xSize)

/* import prototype for direct mapping on memset */
#define OpcUa_MemSet(xDst, xValue, xDstSize)            memset(xDst, xValue, xDstSize)

#ifdef _DEBUG
#define OpcUa_MemSetD(xDst, xValue, xDstSize)
#else
#define OpcUa_MemSetD(xDst, xValue, xDstSize)           memset(xDst, xValue, xDstSize)
#endif

/* import prototype for direct mapping on memcmp */
#define OpcUa_MemCmp(xBuf1, xBuf2, xBufSize)            memcmp(xBuf1, xBuf2, xBufSize)

/*============================================================================
 * String handling functions.
 *===========================================================================*/

/* mapping of ansi string functions on lib functions: */
#define OpcUa_StrCmpA(xStr1, xStr2)                     strcmp(xStr1, xStr2)
#define OpcUa_StrnCmpA(xStr1, xStr2, xCount)            strncmp(xStr1, xStr2, xCount)
#define OpcUa_StriCmpA(xStr1, xStr2)                    stricmp(xStr1, xStr2)
#define OpcUa_StrinCmpA(xStr1, xStr2, xCount)           strnicmp(xStr1, xStr2, xCount)
#define OpcUa_StrLenA(xStr)                             (OpcUa_UInt32)strlen(xStr)
#define OpcUa_StrChrA(xString, xChar)                   strchr(xString, xChar)
#define OpcUa_StrrChrA(xString, xChar)                  strrchr(xString, xChar)
#define OpcUa_StrStrA(xString, xSubstring)              strstr(xString, xSubstring)
#define OpcUa_StrnCpyA(xDst, xDstSize, xSrc, xCount)    strncpy(xDst, xSrc, xCount)
#define OpcUa_StrnCatA(xDst, xDstSize, xSrc, xCount)    strncat(xDst, xSrc, xCount)

#if OPCUA_USE_SAFE_FUNCTIONS
#define OpcUa_SPrintfA                                  sprintf_s
#else
#define OpcUa_SPrintfA                                  sprintf
#endif

#if OPCUA_USE_SAFE_FUNCTIONS
#define OpcUa_SnPrintfA                                 _snprintf_s
#else
#define OpcUa_SnPrintfA                                 _snprintf
#endif

#if OPCUA_USE_SAFE_FUNCTIONS
#define OpcUa_SScanfA                                   sscanf_s
#else
#define OpcUa_SScanfA                                   sscanf
#endif

/* shortcuts to OpcUa_String functions */
#define OpcUa_StrLen(xStr)                              OpcUa_String_StrLen(xStr)

OPCUA_END_EXTERN_C

#endif /* _OpcUa_PlatformDefs_H_ */
