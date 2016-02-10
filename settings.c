/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
**
** Project: OPC UA Local Discovery Server
**
** Author: Gerhard Gappmeier <gerhard.gappmeier@ascolab.com>
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** This file is based on uaserver_settings_filebackend.c from the
** ANSI C based OPC UA Server SDK / Toolkit from Unified Automation GmbH.
**
******************************************************************************/

/**
 * \addtogroup settings Settings Module
 * @{
 *
 *  This settings module can be used to load/modify/store configuration files without loosing meta information like
 *  comments and empty lines.
 *  \b Note that currently this implementation only supports to open one file at a time.
 *
 *  Example:
 *  @code
 *  const char szUserName[] = "john";
 *  time_t now = time(0);
 *  ualds_settings_open("/path/to/file.conf");
 *  ualds_settings_begingroup("Session");
 *  ualds_settings_writeint("Timeout", 1000);
 *  ualds_settings_writestring("User", szUserName, strlen(szUserName));
 *  ualds_settings_writetime_t("LastAccessTime", now);
 *  ualds_settings_endgroup();
 *  ualds_settings_close();
 *  @endcode
 */

/**
 * \file
 * Settings implementation file.
 * \author Gerhard Gappmeier <gerhard.gappmeier@ascolab.com>
 */

#include "settings.h"
/* system includes */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/* local includes */
#include "config.h"
/* local platform includes */
#include <log.h>

enum _EntryType
{
    Invalid = 0,
    KeyValuePair,
    CommentLine,
    EmptyLine,
    Section
};
typedef enum _EntryType EntryType;

struct _Entry
{
    EntryType type;
    char *pszKey;
    char *pszValue;
    int   parent; /* parent entry index, -1 if the entry has no parent. */
    int   line;   /* line no in config file */
};
typedef struct _Entry Entry;

struct _FileSettings
{
    char  *szPath;
    const char  *szArrayKey;
    int    iArrayIndex;
    Entry *pEntries;
    int    numEntries;
    int    EntrySize;
    int    index;
    int    CurrentGroup;
    int    modified;
};
typedef struct _FileSettings FileSettings;

#define ENTRY_STEP_SIZE 10

static FileSettings g_settings;

/**
 * @brief Splits \c pszString into a string array using the separator \c cSep.
 * This function will modify \c pszString and only stores pointer to the substrings
 * inside the string array. That's why this is an array of const strings.
 * You only need to free the string array, not the strings itself.
 *
 * @param pszString Pointer to string which should be split. Consider passing a copy
 * the string if you don't to modify it.
 * @param cSep Separator character.
 * @param pszStringArray Pointer to const string array that will be filled with substrings.
 * @return Returns the number of strings in the array or -1 if the operation fails.
 **/
int split_string(char *pszString, char cSep, const char ***pszStringArray)
{
    int num;
    char *pszSep;
    const char **szStringArray;
    int i;

    if (pszString == 0) return -1;
    if (pszStringArray == 0) return -1;

    num = 1;
    /* count strings */
    for (i=0; pszString[i] != 0; i++)
    {
        if (pszString[i] == cSep) num++;
    }

    /* allocate string array */
    szStringArray = (const char**)malloc(num * sizeof(char*));
    if (szStringArray == 0) return -1;

    /* now split the string and store pointers to the substrings in string array. */
    for (i=0; i<num && pszString; i++)
    {
        pszSep = strchr(pszString, cSep);
        if (pszSep) *pszSep = 0;

        szStringArray[i] = pszString;

        if (pszSep)
        {
            pszString = pszSep;
            pszString++;
            while (*pszString == ' ' || *pszString == '\t') pszString++; /* skip whitespaces */
        }
        else
        {
            pszString = pszSep;
        }
    }

    *pszStringArray = szStringArray;
    return num;
}

/**
 * @brief Replaces all occurrences of the search string \c pszSearch in \c pszString with
 * \c pszReplace.
 *
 * Note that this function use an internal buffer of 256 bytes, so longer strings may be truncated.
 *
 * @param pszString Pointer to string to work on.
 * @param len maximum length of pszString
 * @param pszSearch substring to replace
 * @param pszReplace replace strings
 *
 * @return Returns zero on success.
 **/
int replace_string(char *pszString, int len, const char *pszSearch, const char *pszReplace)
{
    char szTmp[256] = { 0 };
    size_t lenInput = strlen(pszString);
    size_t lenSearch = strlen(pszSearch);
    size_t lenReplace = strlen(pszReplace);
    size_t i, pos = 0;

    for (i=0; i<lenInput && pos < sizeof(szTmp); i++)
    {
        /* test for substring match */
        if (strncmp(pszString+i, pszSearch, lenSearch) == 0)
        {
            /* replace string */
            strlcat(szTmp, pszReplace, sizeof(szTmp));
            /* skip further pszSearch characters */
            i += lenSearch - 1;
            pos += lenReplace;
        }
        else
        {
            /* copy character from pszString. */
            szTmp[pos++] = pszString[i];
        }
    }

    /* copy back result */
    strlcpy(pszString, szTmp, len);

    return 0;
}

/** Removes white spaces from start and end of string. */
static char *trim(char *szText)
{
    ssize_t iLen;

    /* skipping beginning white spaces */
    while (*szText != 0 && (*szText == ' ' || *szText == '\t'))
    {
        szText++;
    }

    iLen = strlen(szText);
    iLen--;

    /* remove ending white spaces */
    while (iLen >= 0 && (szText[iLen] == ' ' || szText[iLen] == '\t' || szText[iLen] == '\n'))
    {
        iLen--;
    }
    szText[iLen+1] = 0;

    /* unquote text if quoted */
    if (szText[0] == '"' && szText[iLen] == '"')
    {
        szText[iLen] = 0;
        szText++;
    }

    return szText;
}

static Entry* UaServer_FSBE_AddEntry(FileSettings *pFS)
{
    Entry *pRet = 0;
    if (pFS->EntrySize == pFS->numEntries)
    {
        void *tmp = realloc(pFS->pEntries, (pFS->EntrySize+ENTRY_STEP_SIZE)*sizeof(Entry));
        if (tmp)
        {
            pFS->pEntries = tmp;
            pFS->EntrySize += ENTRY_STEP_SIZE;
        }
        else
        {
            return 0;
        }
    }

    pRet = &pFS->pEntries[pFS->numEntries];
    pRet->type = Invalid;
    pRet->pszKey = 0;
    pRet->pszValue = 0;
    pRet->parent = -1;

    pFS->numEntries++;

    return pRet;
}

static void UaServer_FSBE_RemoveEntry(FileSettings *pFS, int index)
{
    Entry *pEntry = &pFS->pEntries[index];

    if (pEntry->pszKey)
    {
        free(pEntry->pszKey);
        pEntry->pszKey = 0;
    }
    if (pEntry->pszValue)
    {
        free(pEntry->pszValue);
        pEntry->pszValue = 0;
    }
    pEntry->type = Invalid;
    pEntry->parent = -1;
    pEntry->line = 0;
}

static Entry* UaServer_FSBE_AddKeyValuePair(FileSettings *pFS, const char *szKey, const char *szValue)
{
    Entry *pEntry = UaServer_FSBE_AddEntry(pFS);
    if (pEntry)
    {
        pEntry->type = KeyValuePair;
        pEntry->pszKey = strdup(szKey);
        pEntry->pszValue = (szValue == 0) ? strdup("(null)") : strdup(szValue);
    }
    return pEntry;
}

static Entry* UaServer_FSBE_AddComment(FileSettings *pFS, const char *szText)
{
    Entry *pEntry = UaServer_FSBE_AddEntry(pFS);
    if (pEntry)
    {
        pEntry->type = CommentLine;
        pEntry->pszKey = strdup(szText);
    }
    return pEntry;
}

static Entry* UaServer_FSBE_AddEmptyLine(FileSettings *pFS)
{
    Entry *pEntry = UaServer_FSBE_AddEntry(pFS);
    if (pEntry)
    {
        pEntry->type = EmptyLine;
    }
    return pEntry;
}

static Entry* UaServer_FSBE_AddSection(FileSettings *pFS, const char *szSection)
{
    Entry *pEntry = UaServer_FSBE_AddEntry(pFS);
    if (pEntry)
    {
        pEntry->type = Section;
        pEntry->pszKey = strdup(szSection);
    }
    return pEntry;
}

static int UaServer_FSBE_FindKey(FileSettings *pFS, const char *szKey, int parent)
{
    int i;

    for (i=0; i<pFS->numEntries; i++)
    {
        if (pFS->pEntries[i].type == KeyValuePair
            && pFS->pEntries[i].parent == parent
            && pFS->pEntries[i].pszKey
            && strcmp(pFS->pEntries[i].pszKey, szKey) == 0)
        {
            return i;
        }
    }

    return -1;
}

static int UaServer_FSBE_FindSection(FileSettings *pFS, const char *szSection)
{
    int i;

    for (i=0; i<pFS->numEntries; i++)
    {
        if (pFS->pEntries[i].type == Section
            && pFS->pEntries[i].parent == -1
            && pFS->pEntries[i].pszKey
            && strcmp(pFS->pEntries[i].pszKey, szSection) == 0)
        {
            return i;
        }
    }

    return -1;
}

static int UaServer_FSBE_ParseConfigFile()
{
    FileSettings *pFS = &g_settings;
    FILE *f = fopen(pFS->szPath, "r");
    char szLine[4096];
    char *pszSep, *pszKey, *pszValue;
    int iLine = 0;
    size_t len;
    Entry *pEntry;

    if (f == 0) return ENOENT;

    while (fgets(szLine, sizeof(szLine), f))
    {
        szLine[4095] = 0; /* ensure that string is zero-terminated */

        iLine++;
        /* skip comments */
        if (szLine[0] == '#')
        {
            pEntry = UaServer_FSBE_AddComment(pFS, szLine);
            if (pEntry)
            {
                pEntry->parent = pFS->CurrentGroup;
                pEntry->line = iLine;
            }
            else
            {
                ualds_log(UALDS_LOG_ERR, "Could not create settings entry for comment line.");
                break;
            }
            continue;
        }
        /* skip empty lines */
        if (szLine[0] == '\n')
        {
            pEntry = UaServer_FSBE_AddEmptyLine(pFS);
            if (pEntry)
            {
                pEntry->parent = pFS->CurrentGroup;
                pEntry->line = iLine;
            }
            else
            {
                ualds_log(UALDS_LOG_ERR, "Could not create settings entry for empty line.");
                break;
            }
            continue;
        }

        len = strlen(szLine);
        /* remove trailing \n */
        if (szLine[len-1] == '\n')
        {
            szLine[len-1] = 0;
            len--;
        }

        /* parse section names */
        if (szLine[0] == '[' && szLine[len-1] == ']')
        {
            szLine[len-1] = 0;
            ualds_log(UALDS_LOG_DEBUG, "Parsed section '%s'", szLine+1);
            pEntry = UaServer_FSBE_AddSection(pFS, szLine+1);
            if (pEntry)
            {
                pEntry->line = iLine;
                pFS->CurrentGroup = pFS->numEntries-1;
            }
            else
            {
                ualds_log(UALDS_LOG_ERR, "Could not create settings entry for section '%s'.", szLine+1);
                break;
            }
            continue;
        }

        /* find key/value separator */
        pszSep = strchr(szLine, '=');

        if (pszSep)
        {
            /* split key/value */
            *pszSep = 0;
            pszSep++;
            pszKey = trim(szLine);
            pszValue = trim(pszSep);
            ualds_log(UALDS_LOG_DEBUG, "Parsed config option '%s' = '%s'", pszKey, pszValue);
            pEntry = UaServer_FSBE_AddKeyValuePair(pFS, pszKey, pszValue);
            if (pEntry)
            {
                pEntry->parent = pFS->CurrentGroup;
                pEntry->line = iLine;
            }
            else
            {
                ualds_log(UALDS_LOG_ERR, "Could not create settings entry for key/value pair '%s' '= '%s'.", pszKey, pszValue);
            }
        }
        else
        {
            ualds_log(UALDS_LOG_WARNING, "The configuration file '%s' contains an invalid line in line %i", pFS->szPath, iLine);
        }
    }

    fclose(f);

    /* reset current group */
    pFS->CurrentGroup = -1;

    return 0;
}

static int UaServer_FSBE_WriteConfigFile()
{
    FileSettings *pFS = &g_settings;
    FILE *f = fopen(pFS->szPath, "w");
    int i, j;

    if (f)
    {
        /* write all global keys */
        for (i=0; i<pFS->numEntries; i++)
        {
            if (pFS->pEntries[i].parent != -1) continue;

            switch (pFS->pEntries[i].type)
            {
            case KeyValuePair:
                fprintf(f, "%s = %s\n", pFS->pEntries[i].pszKey, pFS->pEntries[i].pszValue);
                break;
            case CommentLine:
                fprintf(f, "%s", pFS->pEntries[i].pszKey);
                break;
            case EmptyLine:
                fprintf(f, "\n");
                break;
            default:
                break;
            }
        }

        /* write all sections */
        for (i=0; i<pFS->numEntries; i++)
        {
            if (pFS->pEntries[i].type == Section)
            {
                fprintf(f, "[%s]\n", pFS->pEntries[i].pszKey);

                /* write all entries for this section */
                for (j=0; j<pFS->numEntries; j++)
                {
                    if (pFS->pEntries[j].parent != i) continue;

                    switch (pFS->pEntries[j].type)
                    {
                    case KeyValuePair:
                        fprintf(f, "%s = %s\n", pFS->pEntries[j].pszKey, pFS->pEntries[j].pszValue);
                        break;
                    case CommentLine:
                        fprintf(f, "%s", pFS->pEntries[j].pszKey);
                        break;
                    case EmptyLine:
                        fprintf(f, "\n");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        fclose(f);
    }

    return 0;
}

/** Opens the given settings file \c szFilename. */
int ualds_settings_open(const char *szFilename)
{
    g_settings.szPath = strdup(szFilename);
    g_settings.CurrentGroup = -1;
    return UaServer_FSBE_ParseConfigFile();
}

/** This writes all changes to file.
 * You can use this function to force changes to be written to the file
 * without closing it.
 */
int ualds_settings_flush()
{
    UaServer_FSBE_WriteConfigFile();
    return 0;
}

/** Closes the file that was previously opened by ualds_settings_open.
 * This will atomically flush any changes.
 */
int ualds_settings_close()
{
    FileSettings *pFS = &g_settings;
    int ret = 0;
    int i;

    if (pFS->modified)
    {
        ualds_settings_flush();
    }

    if (pFS->pEntries)
    {
        for (i=0; i<pFS->numEntries; i++)
        {
            switch (pFS->pEntries[i].type)
            {
            case Invalid:
                break;
            case KeyValuePair:
                free(pFS->pEntries[i].pszKey);
                free(pFS->pEntries[i].pszValue);
                break;
            case CommentLine:
            case Section:
                free(pFS->pEntries[i].pszKey);
                break;
            case EmptyLine:
                break;
            }
        }

        free(pFS->pEntries);
        pFS->pEntries = 0;
        pFS->numEntries = 0;
    }
    else
    {
        ret = EINVAL;
    }

    free(pFS->szPath);
    pFS->szPath = 0;
    pFS->szArrayKey = 0;
    pFS->iArrayIndex = 0;
    pFS->EntrySize = 0;
    pFS->index = 0;
    pFS->CurrentGroup = 0;
    pFS->modified = 0;

    return ret;
}

/** Creates a section.
 * Currently no nesting of groups is supported.
 */
int ualds_settings_begingroup(const char *szGroup)
{
    FileSettings *pFS = &g_settings;
    int index = UaServer_FSBE_FindSection(pFS, szGroup);
    Entry *pEntry;

    if (index == -1)
    {
        /* add new section */
        pEntry = UaServer_FSBE_AddSection(pFS, szGroup);
        if (pEntry)
        {
            index = pFS->numEntries-1;
        }
        else
        {
            return -1;
        }
    }

    pFS->CurrentGroup = index;

    return 0;
}

/** Closes the group opened by ualds_settings_begingroup. */
int ualds_settings_endgroup()
{
    FileSettings *pFS = &g_settings;
    pFS->CurrentGroup = -1;
    return 0;
}

/** Returns the value for the setting \c szKey as string.
 * @param szKey The key name
 * @param szValue Pointer to preallocated string.
 * @param len Length of szValue in bytes.
 *
 * This function will zero-terminate the string if it needs to be truncated.
 */
int ualds_settings_readstring(const char *szKey, char *szValue, int len)
{
    FileSettings *pFS = &g_settings;
    int index;
    char szTmpKey[UALDS_CONF_MAX_KEY_LENGTH];
    const char *pszKey = 0;

    if (pFS->szArrayKey)
    {
        /* create array key */
        snprintf(szTmpKey, UALDS_CONF_MAX_KEY_LENGTH, "%s/%i/%s", pFS->szArrayKey, pFS->iArrayIndex, szKey);
        szTmpKey[UALDS_CONF_MAX_KEY_LENGTH-1] = 0;
        pszKey = szTmpKey;
    }
    else
    {
        /* use input key as-is */
        pszKey = szKey;
    }

    index = UaServer_FSBE_FindKey(pFS, pszKey, pFS->CurrentGroup);
    if (index == -1) return ENOENT;

    if (strcmp("(null)", pFS->pEntries[index].pszValue))
    {
        strlcpy(szValue, pFS->pEntries[index].pszValue, len);
    }
    else if (len > 0)
    {
        szValue[0] = '\0';
    }

    return 0;
}

#ifdef HAVE_OPCUA_STACK
/** Convenience function for ualds_settings_readstring which fills an OPC UA String. */
int ualds_settings_readuastring(const char *szKey, OpcUa_String *pString)
{
    char szTmp[256];
    int ret;

    ret = ualds_settings_readstring(szKey, szTmp, sizeof(szTmp));
    OpcUa_String_AttachCopy(pString, szTmp);

    return ret;
}
#endif /* HAVE_OPCUA_STACK */

/** Sets the string value of setting \c szKey to \c szValue. */
int ualds_settings_writestring(const char *szKey, const char *szValue)
{
    FileSettings *pFS = &g_settings;
    int index;
    Entry *pEntry;
    char szTmpKey[UALDS_CONF_MAX_KEY_LENGTH];
    const char *pszKey = 0;

    if (pFS->szArrayKey)
    {
        /* create array key */
        snprintf(szTmpKey, UALDS_CONF_MAX_KEY_LENGTH, "%s/%i/%s", pFS->szArrayKey, pFS->iArrayIndex, szKey);
        szTmpKey[UALDS_CONF_MAX_KEY_LENGTH-1] = 0;
        pszKey = szTmpKey;
    }
    else
    {
        /* use input key as-is */
        pszKey = szKey;
    }

    index = UaServer_FSBE_FindKey(pFS, pszKey, pFS->CurrentGroup);
    if (index == -1)
    {
        /* add new key */
        pEntry = UaServer_FSBE_AddKeyValuePair(pFS, pszKey, szValue);
        if (pEntry)
        {
            pEntry->parent = pFS->CurrentGroup;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        /* update key value */
        free(pFS->pEntries[index].pszValue);
        pFS->pEntries[index].pszValue = (szValue == 0) ? strdup("(null)") : strdup(szValue);
    }

    pFS->modified = 1;

    return 0;
}

/** Returns the value for the setting \c szKey as integer. */
int ualds_settings_readint(const char *szKey, int *piVal)
{
    int ret;
    char szValue[20];

    ret = ualds_settings_readstring(szKey, szValue, sizeof(szValue));
    if (ret == 0)
    {
        *piVal = strtol(szValue, 0, 10);
    }

    return ret;
}

/** Sets the integer value \c iVal for setting \c szKey. */
int ualds_settings_writeint(const char *szKey, int iVal)
{
    char szValue[20];

    snprintf(szValue, 20, "%i", iVal);
    szValue[19] = 0;

    return ualds_settings_writestring(szKey, szValue);
}

/** Returns the value for the setting \c szKey as time_t. */
int ualds_settings_readtime_t(const char *szKey, time_t *pTime)
{
    int ret;
    char szValue[20];

    ret = ualds_settings_readstring(szKey, szValue, sizeof(szValue));
    if (ret == 0)
    {
        *pTime = strtol(szValue, 0, 10);
    }

    return ret;
}

/** Sets the time_t value \c time for setting \c szKey. */
int ualds_settings_writetime_t(const char *szKey, time_t time)
{
    char szValue[20];

    /* it is not so easy to print time_t in a portable way.
     * According to the C standard time_t can be any arithmetic type,
     * it can also be a floating point, although most systems will use an integer type.
     * However, typically the size of time_t is 8 bytes on 64bit systems,
     * and 4 bytes on 32bit system (Y2038 problem).
     */
    snprintf(szValue, 20, "%"PRITT, (TTINTTYPE)time);
    szValue[19] = 0;

    return ualds_settings_writestring(szKey, szValue);
}

/** Starts reading an array of values.
 *
 *  If you have many occurrences of a certain set of keys, you can use arrays to make your life easier.
 *  For example let's suppose that you want to save a variable-length list of user names and passwords.
 *  You could write:
 *
 *  @code
 *  struct User {
 *      char szUserName[50];
 *      char szPassword[50];
 *  }
 *  int numUsers = 0;
 *  struct User *pUsers = 0;
 *  int i;
 *
 *  ualds_settings_open("/path/to/file.conf");
 *  ualds_settings_beginreadarray("logins", &numUsers);
 *  pUsers = malloc(numUsers * sizeof(struct User));
 *  for (i=0; i<numUsers; i++) {
 *      ualds_settings_setarrayindex(i);
 *      ualds_settings_readstring("userName", pUser[i].szUserName, 50);
 *      ualds_settings_readstring("password", pUser[i].szPassword, 50);
 *  }
 *  ualds_settings_endarray();
 *  ualds_settings_close();
 *  @endcode
 */
int ualds_settings_beginreadarray(const char *szArrayKey, int *pNumElements)
{
    int ret = 0;
    char szKey[UALDS_CONF_MAX_KEY_LENGTH];
    FileSettings *pFS = &g_settings;

    strlcpy(szKey, szArrayKey, UALDS_CONF_MAX_KEY_LENGTH);
    strlcat(szKey, "/size", UALDS_CONF_MAX_KEY_LENGTH);

    ret = ualds_settings_readint(szKey, pNumElements);
    if (ret != 0) return ret;

    pFS->szArrayKey = szArrayKey;

    return 0;
}

/** Starts writing an array of values.
 *
 *  If you have many occurrences of a certain set of keys, you can use arrays to make your life easier.
 *  For example let's suppose that you want to save a variable-length list of user names and passwords.
 *  You could write:
 *
 *  @code
 *  struct User {
 *      char szUserName[50];
 *      char szPassword[50];
 *  }
 *  int numUsers = 0;
 *  struct User *pUsers = GetUsers(&numUsers);
 *  int i;
 *
 *  ualds_settings_open("/path/to/file.conf");
 *  ualds_settings_beginwritearray("logins", numUsers);
 *  for (i=0; i<numUsers; i++) {
 *      ualds_settings_setarrayindex(i);
 *      ualds_settings_writestring("userName", pUser[i].szUserName);
 *      ualds_settings_writestring("password", pUser[i].szPassword);
 *  }
 *  ualds_settings_endarray();
 *  ualds_settings_close();
 *  @endcode
 */
int ualds_settings_beginwritearray(const char *szArrayKey, int numElements)
{
    int ret = 0;
    char szKey[UALDS_CONF_MAX_KEY_LENGTH];
    FileSettings *pFS = &g_settings;

    strlcpy(szKey, szArrayKey, UALDS_CONF_MAX_KEY_LENGTH);
    strlcat(szKey, "/size", UALDS_CONF_MAX_KEY_LENGTH);

    ret = ualds_settings_writeint(szKey, numElements);
    if (ret != 0) return ret;

    pFS->szArrayKey = szArrayKey;

    return ret;
}

/** Sets the current array index.
 * Calls to any read or write function will operate on this index.
 *
 * You must call ualds_settings_beginreadarray or ualds_settings_beginwritearray before you can call this function.
 */
int ualds_settings_setarrayindex(int index)
{
    FileSettings *pFS = &g_settings;
    pFS->iArrayIndex = index;
    return 0;
}

/** Closes an array opened with ualds_settings_beginreadarray or ualds_settings_beginwritearray. */
int ualds_settings_endarray()
{
    FileSettings *pFS = &g_settings;
    pFS->szArrayKey = 0;

    return 0;
}

int ualds_settings_addcomment(const char* szComment)
{
    FileSettings *pFS = &g_settings;
    Entry *pEntry = UaServer_FSBE_AddComment(pFS, szComment);
    if (pEntry) return 0;
    return -1;
}

int ualds_settings_addemptyline()
{
    FileSettings *pFS = &g_settings;
    Entry *pEntry = UaServer_FSBE_AddEmptyLine(pFS);
    if (pEntry) return 0;
    return -1;
}

/** Removes the settings named \c szKey from the file. */
int ualds_settings_removekey(const char *szKey)
{
    FileSettings *pFS = &g_settings;
    int index;

    index = UaServer_FSBE_FindKey(pFS, szKey, pFS->CurrentGroup);
    if (index == -1)
    {
        return -1;
    }
    else
    {
        UaServer_FSBE_RemoveEntry(pFS, index);
    }

    pFS->modified = 1;

    return 0;
}

/** Removes the array named \c szArray and all sub-keys from the file. */
int ualds_settings_removearray(const char *szArray)
{
    FileSettings *pFS = &g_settings;
    Entry *pEntry;
    char szKey[UALDS_CONF_MAX_KEY_LENGTH];
    int i;
    size_t len;

    strlcpy(szKey, szArray, UALDS_CONF_MAX_KEY_LENGTH);
    len = strlcat(szKey, "/", UALDS_CONF_MAX_KEY_LENGTH);

    /* remove all keys in array */
    for (i=0; i<g_settings.numEntries; i++)
    {
        pEntry = &pFS->pEntries[i];
        if (pFS->CurrentGroup == pEntry->parent &&
            (pEntry->type == EmptyLine ||
             pEntry->pszKey == 0 ||
             strncmp(pEntry->pszKey, szKey, len) == 0))
        {
            UaServer_FSBE_RemoveEntry(pFS, i);
        }
    }

    pFS->modified = 1;

    return 0;
}

/** Removes the group named \c szGroup and all sub-keys from the file. */
int ualds_settings_removegroup(const char *szGroup)
{
    FileSettings *pFS = &g_settings;
    int index;
    int i;

    index = UaServer_FSBE_FindSection(pFS, szGroup);
    if (index == -1)
    {
        return -1;
    }
    else
    {
        /* remove all keys in section */
        for (i=0; i<g_settings.numEntries; i++)
        {
            if (g_settings.pEntries[i].parent == index)
            {
                UaServer_FSBE_RemoveEntry(pFS, i);
            }
        }
        /* remove the section */
        UaServer_FSBE_RemoveEntry(pFS, index);
    }

    pFS->modified = 1;

    return 0;
}

/**
 * @}
 */

