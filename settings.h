/* Copyright (c) 1996-2017, OPC Foundation. All rights reserved.

The source code in this file is covered under a dual - license scenario :
- RCL: for OPC Foundation members in good - standing
- GPL V2: everybody else

RCL license terms accompanied with this source code.See http ://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code.See http ://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

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
int ualds_settings_close(int flush);
int ualds_settings_begingroup(const char *szGroup);
int ualds_settings_endgroup();
int ualds_settings_readstring(const char *szKey, char *szValue, int len);
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
