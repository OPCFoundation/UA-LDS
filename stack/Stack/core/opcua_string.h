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

#ifndef _OpcUa_String_H_
#define _OpcUa_String_H_ 1

OPCUA_BEGIN_EXTERN_C

#define OpcUa_P_String_StrnCpy  OpcUa_ProxyStub_g_PlatformLayerCalltable->StrnCpy
#define OpcUa_P_String_StrnCat  OpcUa_ProxyStub_g_PlatformLayerCalltable->StrnCat
#define OpcUa_P_String_StrLen   OpcUa_ProxyStub_g_PlatformLayerCalltable->StrLen
#define OpcUa_P_String_StrnCmp  OpcUa_ProxyStub_g_PlatformLayerCalltable->StrnCmp
#define OpcUa_P_String_StrniCmp OpcUa_ProxyStub_g_PlatformLayerCalltable->StrniCmp

/**
 * @brief Used as length for n-operations to work with full length source strings.
 */
#define OPCUA_STRINGLENZEROTERMINATED   0xffffffff
#define OPCUA_STRING_LENDONTCARE        OPCUA_STRINGLENZEROTERMINATED

/**
 * @brief Cast a C string into a OpcUa_String.
 *
 * @param strCString [in]    Pointer to the C string.
 *
 * This function can be used to convert a C character string constant to
 * a OpcUa_String*. The empty string ("") is converted to a null OpcUa_String*,
 * otherwise this function is guaranteed to succeed.
 *
 * The returned OpcUa_String* is a temporary object that refers to the
 * original C string.
 * It is however necessary to ensure the lifetime of the original C string
 * is long enough.
 *
 * Correct code looks like this:
 *   uStatus = OpcUa_String_StrnCpy(str, OpcUa_String_FromCString("text"), OPCUA_STRING_LENDONTCARE);
 *   OpcUa_GotoErrorIfBad(uStatus);
 *
 * Important:
 *   Never cast a C string directly to a OpcUa_String* object, always use this
 *   function for that purpose.
 *
 * @return Pointer to a OpcUa_String
 */
OPCUA_EXPORT
OpcUa_String* OpcUa_String_FromCString(/* in */ const OpcUa_CharA* strCString);

/**
 * @brief Initializes a string structure.
 *
 * @param pString [out] Pointer to the new string.
 *
 * @return void
 */
OPCUA_EXPORT
OpcUa_Void OpcUa_String_Initialize(/* out */ OpcUa_String* pString);


/**
 * @brief Allocates memory for a string of uLength bytes length.
 *
 * @param strSource     [in]  The value of the created string.
 * @param uLength       [in]  Length of string to be created. (OPCUA_STRINGLENZEROTERMINATED Use length of source, must be zero terminated)
 * @param uBufferSize   [in]  Length of string buffer to be created. (useful if larger than uLength)
 * @param bDoCopy       [in]  OpcUa_True: Copy the source string; else: reference it.
 * @param bFreeOnClear  [in]  OpcUa_True: Free the Source string if string gets cleared (only effective, if bDoCopy == OpcUa_False!).
 * @param ppNewString   [out] Pointer to the new string.
 *
 * @return Status code; @see opcua_statuscodes.h
 */
OPCUA_EXPORT
OpcUa_StatusCode    OpcUa_String_CreateNewString(   /*  in */ OpcUa_StringA     strSource,
                                                    /*  in */ OpcUa_UInt32      uLength,
                                                    /*  in */ OpcUa_UInt32      uBufferSize,
                                                    /*  in */ OpcUa_Boolean     bDoCopy,
                                                    /*  in */ OpcUa_Boolean     bFreeOnClear,
                                                    /* out */ OpcUa_String**    ppNewString);

/**
 * @brief Allocates memory for a string of uLength bytes length when a_bDoCopy is true.
 *
 * It is important to remember that the pString parameter needs to be initialized with
 * OpcUa_String_Initialize before calling this function.
 * This function can create Strings that are not zero terminated or contain embedded
 * NUL characters.
 *
 * @param strSource    [in] The value of the created string.
 * @param uLength      [in] Length of string to be created. (OPCUA_STRINGLENZEROTERMINATED Use length of source, must be zero terminated)
 * @param uBufferSize  [in] Length of string buffer to be created. (useful if larger than uLength)
 * @param bDoCopy      [in] OpcUa_True: Copy the source string; else: reference it (mind the next parameter!).
 * @param bFreeOnClear [in] OpcUa_True: Free the Source string if string gets cleared (only effective, if bDoCopy == OpcUa_False!).
 * @param pString      [bi] Pointer to the manipulated string.
 *
 * @return Status code; @see opcua_statuscodes.h
 */
OPCUA_EXPORT
OpcUa_StatusCode    OpcUa_String_AttachToString(  /*  in */ OpcUa_StringA strSource,
                                                  /*  in */ OpcUa_UInt32  uLength,
                                                  /*  in */ OpcUa_UInt32  uBufferSize,
                                                  /*  in */ OpcUa_Boolean bDoCopy,
                                                  /*  in */ OpcUa_Boolean bFreeOnClear,
                                                  /*  bi */ OpcUa_String* pString);


/**
 * @brief Deletes all resources occupied by a OpcUa_String including the OpcUa_String itself.
 *
 * @param ppString [bi] Pointer to the OpcUa_String to be deleted. Set to OpcUa_Null after the call.
 */
OPCUA_EXPORT
OpcUa_Void OpcUa_String_Delete(/* bi */ OpcUa_String** ppString);


/**
 * @brief Frees all internal resources occupied by a OpcUa_String.
 *
 * @param pString [bi] Pointer to the OpcUa_String to be cleared.
 */
OPCUA_EXPORT
OpcUa_Void OpcUa_String_Clear(/* bi */ OpcUa_String* pString);

/**
 * @brief Get a direct pointer to the internal UTF-8 string.
 *
 * Do not call this function if pString is OpcUa_Null.
 * The returned C string value may be NULL if the OpcUa_String object is a null string,
 * or not zero terminated if the OpcUa_String object was created by OpcUa_String_AttachToSring above.
 * Use OpcUa_String_StrSize to get the raw string size.
 *
 * @param pString [in] Pointer to the OpcUa_String to get the raw content from.
 */
OPCUA_EXPORT
OpcUa_CharA* OpcUa_String_GetRawString(/* in */ const OpcUa_String* pString);

/**
 * @brief Determine if the string is empty (length zero).
 *
 * pSrcString is allowed to be OpcUa_Null, the function returns OpcUa_False in this case.
 *
 * @param pString [in] Pointer to the OpcUa_String to be checked.
 */
OPCUA_EXPORT
OpcUa_Boolean OpcUa_String_IsEmpty(/* in */ const OpcUa_String* pString);

/**
 * @brief Determine if the string is null (pString is OpcUa_Null or refers
 * to a null string object).
 *
 * pSrcString is allowed to be OpcUa_Null, the function returns OpcUa_True in this case.
 *
 * @param pString [in] Pointer to the OpcUa_String to be checked.
 */
OPCUA_EXPORT
OpcUa_Boolean OpcUa_String_IsNull(/* in */ const OpcUa_String* pString);

/* C-String Conversion */

/**
 * @brief Copies a string (maximum bytes of the given len).
 *
 * It is important to remember that the pDestString parameter needs to be initialized with
 * OpcUa_String_Initialize before calling this function.
 * To simulate a strcpy() pass OPCUA_STRING_LENDONTCARE as a_uLength.
 * pSrcString is allowed to be OpcUa_Null or a null string object.
 *
 * @param pDestString   [bi]     The copy of the string (must be OpcUa_String).
 * @param pSrcString    [in]     The string to copy (can be from OpcUa_String_FromCString).
 * @param uLength       [in]     The number of characters to copy.
 */
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_String_StrnCpy(/* bi */ OpcUa_String*       pDestString,
                                      /* in */ const OpcUa_String* pSrcString,
                                      /* in */ OpcUa_UInt32        uLength);

/**
 * @brief Appends a string to another string.
 *
 * It is important to remember that the pDestString parameter needs to be initialized with
 * OpcUa_String_Initialize before calling this function.
 * To simulate a strcat() pass OPCUA_STRING_LENDONTCARE as a_uLength
 * pSrcString is allowed to be OpcUa_Null or a null string object.
 *
 * @param pDestString       [bi]     The string to append to (must be OpcUa_String).
 * @param pSrcString        [in]     The string to append (can be from OpcUa_String_FromCString).
 * @param uLength           [in]     The maximum number of characters to copy.
 */
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_String_StrnCat(/* bi */ OpcUa_String*       pDestString,
                                      /* in */ const OpcUa_String* pSrcString,
                                      /* in */ OpcUa_UInt32        uLength);

/**
 * @brief Returns the size in bytes of the given OpcUa_String.
 *
 * pSrcString is allowed to be OpcUa_Null or a null string object, the function
 * returns 0 in this case.
 *
 * @param pString [in] The string calculate the length of.
 */
OPCUA_EXPORT
OpcUa_UInt32 OpcUa_String_StrSize(/* in */ const OpcUa_String* pString);

/**
 * @brief Compares two strings.
 *
 * @return 0 if the strings are the same. <0 if pString2 is less than pString2. >0 if pString2 is greater than pString2.
 * This function does not do a lexicographic comparison.
 *
 * @param pString1    [in] The first string for comparison.
 * @param pString2    [in] The second string for comparison.
 * @param uLength     [in] Number of characters to compare.
 * @param bIgnoreCase [in] True if no case-sensitive comparison is needed.
 */
OPCUA_EXPORT
OpcUa_Int32 OpcUa_String_StrnCmp(/* in */ const OpcUa_String* pString1,
                                 /* in */ const OpcUa_String* pString2,
                                 /* in */ OpcUa_UInt32        uLength,
                                 /* in */ OpcUa_Boolean       bIgnoreCase);

/** The following functions are convenience functions for easy use only and call the above API only. **/

/**
 * @brief Attaches a readonly reference for a string constant to a string object.
 *
 * It is important to remember that the pDst parameter needs to be initialized with
 * OpcUa_String_Initialize before calling this function.
 * This call is generally used to assign static string constants to string objects.
 * The function is guaranteed to succeed.
 *
 * @param pDst [bi]  The string object.
 * @param pSrc [in]  The string being referenced.
 *
 * @return Status code; @see opcua_statuscodes.h
 */
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_String_AttachReadOnly(/* bi */ OpcUa_String* pDst,
                                             /* in */ const OpcUa_CharA* pSrc);

/**
 * @brief Attaches a copy of a string to a string object.
 *
 * It is important to remember that the pDst parameter needs to be initialized with
 * OpcUa_String_Initialize before calling this function.
 *
 * @param pDst [bi]  The string object.
 * @param pSrc [in]  The string being copied.
 *
 * @return Status code; @see opcua_statuscodes.h
 */
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_String_AttachCopy(/* bi */ OpcUa_String*       pDst,
                                         /* in */ const OpcUa_CharA*  pSrc);

/**
 * @brief Attaches a string to a string object.
 *
 * It is important to remember that the pDst parameter needs to be initialized with
 * OpcUa_String_Initialize before calling this function.
 * The string must be allocated with OpcUa_Alloc. The string object will free the string.
 * The function is guaranteed to succeed.
 *
 * @param pDst [bi]  The string object.
 * @param pSrc [in]  The string being referenced and freed on clear.
 *
 * @return Status code; @see opcua_statuscodes.h
 */
OPCUA_EXPORT
OpcUa_StatusCode OpcUa_String_AttachWithOwnership(/* bi */ OpcUa_String* pDst,
                                                  /* in */ OpcUa_StringA pSrc);


/**
 * @brief Returns the length in character of the given OpcUa_String
 *
 * pSrcString is allowed to be OpcUa_Null or a null string object, the function
 * returns 0 in this case.
 *
 * @param pString [in] The string calculate the length of.
 */
#define OpcUa_String_StrLen(x) OpcUa_String_StrSize(x)

OPCUA_END_EXTERN_C

#endif /* _OpcUa_String_H_ */

