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

/* These are the Linux platform definitions! */

#ifndef _OpcUa_PlatformDefs_H_
#define _OpcUa_PlatformDefs_H_ 1

/* Detect endianness of this unix system */
#if defined(__sun) || defined(sun)
# define BIG_ENDIAN 4321
# define LITTLE_ENDIAN 1234
# if defined(_BIG_ENDIAN)
#  define BYTE_ORDER BIG_ENDIAN
# else
#  define BYTE_ORDER LITTLE_ENDIAN
# endif
#else
# include <endian.h>
#endif
#include <stdio.h>

#define OPCUA_EXPORT
#define OPCUA_IMPORT
#define OPCUA_DLLCALL
#define OPCUA_IMEXPORT

#define OPCUA_EXPORT_SYNC_SERVER_API OPCUA_EXPORT

/* calling convention used by stack functions that explicitly use cdecl */
#define OPCUA_CDECL

/* used ie. for unlimited timespans */
#define OPCUA_INFINITE 0xFFFFFFFF

/* Some compilers support applying printf-style warnings. */
#if defined(__GNUC__)
#define OPCUA_PRINTF_ARGS(formatIndex) __attribute__((__format__(__printf__, (formatIndex), (formatIndex) + 1)))
#define OPCUA_PRINTF_VALIST(formatIndex) __attribute__((__format__(__printf__, (formatIndex), 0)))
#else
#define OPCUA_PRINTF_ARGS(formatIndex)
#define OPCUA_PRINTF_VALIST(formatIndex)
#endif

/*============================================================================
* Additional basic headers
*===========================================================================*/
/* configuration switches */
#include <opcua_config.h>
/* basic type mapping */
#include <opcua_p_types.h>
#include <memory.h>
#include <string.h>
/**********************************************************************************/
/*   Check configuration.                                                         */
/**********************************************************************************/
#if OPCUA_USE_SAFE_FUNCTIONS
  #error "Use of _s functions limited to windows."
#endif /* OPCUA_USE_SAFE_FUNCTIONS */

/**********************************************************************************/
/*   Security Configuration section.                                              */
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
#endif /* OPCUA_SUPPORT_PKI */

/*============================================================================
* Types and mapping.
*===========================================================================*/
#ifdef __cplusplus
# define OPCUA_BEGIN_EXTERN_C extern "C" {
# define OPCUA_END_EXTERN_C }
#else
# define OPCUA_BEGIN_EXTERN_C
# define OPCUA_END_EXTERN_C
#endif

OPCUA_BEGIN_EXTERN_C

/*============================================================================
 * Memory allocation functions.
 *
 * Note: Realloc and Free behave gracefully if passed a null pointer. Changing
 * these functions to a macro call to free will cause problems.
 *===========================================================================*/

/*OpcUa_Void* memset(OpcUa_Void* Dst, OpcUa_Int Val, OpcUa_UInt Size);*/
#define OpcUa_MemSet(xDst, xValue, xDstSize)        memset(xDst, xValue, xDstSize)

#ifdef _DEBUG
#define OpcUa_MemSetD(xDst, xValue, xDstSize)
#else
#define OpcUa_MemSetD(xDst, xValue, xDstSize)   memset(xDst, xValue, xDstSize)
#endif

/*OpcUa_Int memcmp(const OpcUa_Void* Buf1, const OpcUa_Void* Buf2, OpcUa_UInt Size);*/
#define OpcUa_MemCmp(xBuf1, xBuf2, xBufSize)        memcmp(xBuf1, xBuf2, xBufSize)

/*OpcUa_Int memcpy(OpcUa_Void* Buf1, const OpcUa_Void* Buf2, OpcUa_UInt Size);*/
#define OpcUa_MemCpy(xDst, xDstSize, xSrc, xCount)  OpcUa_Memory_MemCpy(xDst, xDstSize, xSrc, xCount)

#define OpcUa_Alloc(xSize)                          OpcUa_Memory_Alloc(xSize)
#define OpcUa_ReAlloc(xSrc, xSize)                  OpcUa_Memory_ReAlloc(xSrc, xSize)
#define OpcUa_Free(xSrc)                            OpcUa_Memory_Free(xSrc)

#define OpcUa_DestroySecretData(xDst, xSize)        OpcUa_Memory_DestroySecretData(xDst, xSize)

/* shortcuts to utility functions */
#define OpcUa_qSort(xBase, xNum, xWidth, xCmp, xCtx)            OpcUa_QSort(xBase, xNum, xWidth, xCmp, xCtx)
#define OpcUa_bSearch(xKey, xBase, xNum, xWidth, xCmp, xCtx)    OpcUa_BSearch(xKey, xBase, xNum, xWidth, xCmp, xCtx)

/*============================================================================
 * String handling functions.
 *===========================================================================*/

/* mapping of ansi char functions on lib functions: */
/*#define OpcUa_CharAToInt(xChar)                       OpcUa_P_CharAToInt(xChar)*/

/* mapping of ansi string functions on lib functions: */
/*OpcUa_Int strcmp(OpcUa_StringA s1, OpcUa_StringA s2);*/
#define OpcUa_StrCmpA(xStr1, xStr2)                   strcmp(xStr1, xStr2)
#define OpcUa_StrnCmpA(xStr1, xStr2, xCount)          strncmp(xStr1, xStr2, xCount)
#define OpcUa_StriCmpA(xStr1, xStr2)                  stricmp(xStr1, xStr2)
#define OpcUa_StrinCmpA(xStr1, xStr2, xCount)         strnicmp(xStr1, xStr2, xCount)
#define OpcUa_StrLenA(xStr)                           (OpcUa_UInt32)strlen(xStr)
#define OpcUa_StrChrA(xString, xChar)                 strchr(xString, xChar)
#define OpcUa_StrrChrA(xString, xChar)                strrchr(xString, xChar)
#define OpcUa_StrStrA(xString, xSubstring)            strstr(xString, xSubstring)
#define OpcUa_StrnCpyA(xDst, xDstSize, xSrc, xCount)  strncpy(xDst, xSrc, xCount)
#define OpcUa_StrnCatA(xDst, xDstSize, xSrc, xCount)  strncat(xDst, xSrc, xCount)

/* import prototype for direct mapping on sprintf (TODO has to go) */
/* OpcUa_Int sprintf(char *buffer, const char *format, ...); */
#define OpcUa_SPrintfA                                sprintf

/* import prototype for direct mapping on sscanf (TODO has to go) */
/* OpcUa_Int sscanf(const OpcUa_CharA *buffer, const OpcUa_CharA *format, ... ); */
#define OpcUa_SScanfA                                 sscanf

OpcUa_Int32 OpcUa_P_String_snprintf(OpcUa_StringA sTarget, OpcUa_UInt32 nCount, OpcUa_StringA sFormat, ...) OPCUA_PRINTF_ARGS(3);
#define OpcUa_SnPrintfA                               OpcUa_P_String_snprintf

/* shortcuts to OpcUa_String functions */
#define OpcUa_StrLen(xStr)                            OpcUa_String_StrLen(xStr)

#define OpcUa_StrCpy(xDst, xSrc)                      OpcUa_String_StrCpy(xDst, xSrc, OPCUA_STRING_LENDONTCARE, OpcUa_True)
#define OpcUa_StrnCpy(xDst, xDstLength, xSrc, xCount) OpcUa_String_StrnCpy(xDst, xDstLength, xSrc, xCount)

#define OpcUa_StrCat(xDst, xSrc)                      OpcUa_String_StrCat(xDst, xSrc, OPCUA_STRING_LENDONTCARE)
#define OpcUa_StrnCat(xDst, xDstLength, xSrc, xCount) OpcUa_String_StrnCat(xDst, xDstLength, xSrc, xCount)

OPCUA_END_EXTERN_C

#endif /* _OpcUa_PlatformDefs_H_ */
