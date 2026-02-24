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

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

/* system includes */
#ifdef HAVE_OPCUA_STACK
#include <opcua_platformdefs.h>
#include <opcua_string.h>
#endif
/* local platform includes */
#include <platform.h>

int ualds_settings_open(const char *szFilename);
int ualds_settings_flush();
void ualds_settings_update_config_file();
int ualds_settings_close(int flush);
int ualds_settings_begingroup(const char *szGroup);
int ualds_settings_endgroup();
int ualds_settings_readstring(const char *szKey, char *szValue, int len);
int ualds_settings_setReadOnly(int readOnly);
#ifdef HAVE_OPCUA_STACK
int ualds_settings_readuastring(const char *szKey, OpcUa_String *pString);
#endif /* HAVE_OPCUA_STACK */
int ualds_settings_writestring(const char *szKey, const char *szValue);
int ualds_settings_readint(const char *szKey, int *piVal);
int ualds_settings_writeint(const char *szKey, int iVal);
int ualds_settings_readtime_t(const char *szKey, time_t *pTime);
int ualds_settings_writetime_t(const char *szKey, time_t time);
int ualds_settings_beginreadarray(const char *szArrayKey, int *pNumElements);
int ualds_settings_beginwritearray(const char *szArrayKey, int numElements);
int ualds_settings_setarrayindex(int index);
int ualds_settings_endarray();
int ualds_settings_addcomment(const char* szComment);
int ualds_settings_addemptyline();
int ualds_settings_removekey(const char *szKey);
int ualds_settings_removearray(const char *szKey);
int ualds_settings_removegroup(const char *szGroup);

int split_string(char *pszString, char cSep, const char ***pszStringArray);
int replace_string(char *pszString, int len, const char *pszSearch, const char *pszReplace);

int ualds_settings_open_from_backup(const char *szFilename);
int ualds_settings_open_from_default(const char *szFilename);

#ifdef _WIN32
#define __ualds_plat_path_sep "\\"
#else
#define __ualds_plat_path_sep "/"
#endif /* _WIN32 */

/** Helper macro for lazy programmers.
 * This macro reads a string parameter and stores it in a global string variable.
 * It assumes that a global variable like "char g_szPARAM[20]" was defined.
 * The advantage is that you don't need to type so much, and the \c len parameter is filled
 * automatically with the size of the variable. DON'T use string pointers! Otherwise sizeof()
 * will not work.
 * If you don't understand this, don't use it.
 */
#define UALDS_SETTINGS_READSTRING(param) ualds_settings_readstring(xstr(param), g_sz##param, sizeof(g_sz##param))


#endif /* __SETTINGS_H__ */
