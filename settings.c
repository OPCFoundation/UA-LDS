/* Copyright (c) 1996-2024, OPC Foundation. All rights reserved.
   Copyright (c) 2025 Pilz GmbH & Co. KG

The source code in this file is covered under a dual - license scenario :
-RCL : for OPC Foundation members in good - standing
- GPL V2 : everybody else

RCL license terms accompanied with this source code.See http ://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code.See http ://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

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
 *  ualds_settings_close(1);
 *  @endcode
 */

/**
 * \file
 * Settings implementation file.
 * \author
 */

#include "settings.h"

/* system includes */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
/* local includes */
#include "config.h"
/* local platform includes */
#include <opcua_p_crypto.h>
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
    char  *szPathBackup;
    const char  *szArrayKey;
    int    iArrayIndex;
    Entry *pEntries;
    int    numEntries;
    int    EntrySize;
    int    index;
    int    CurrentGroup;
    int    modified;
    int    readOnly;  // if this is 1, we never write to the configuration file!
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
    while (iLen >= 0 && (szText[iLen] == ' ' || szText[iLen] == '\t' || szText[iLen] == '\n' || szText[iLen] == '\r'))
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

    // Move all following entries to this entry...
    int i;
    for (i = index; i < pFS->numEntries - 1; i++)
    {
        Entry *pEntryFrom = &pFS->pEntries[i + 1];
        Entry *pEntryTo = &pFS->pEntries[i];

        *pEntryTo = *pEntryFrom;  // assign all values directly

        pEntryFrom->pszKey = 0;
        pEntryFrom->pszValue = 0;
        pEntryFrom->type = Invalid;
        pEntryFrom->parent = -1;
        pEntryFrom->line = 0;
    }

    pFS->numEntries--; // we have now one entry less...
    /*assert(*pFS->numEntries >= 0);*/

    // be sure the last entry is cleared
    Entry *pEntryToClear = &pFS->pEntries[pFS->numEntries];

    pEntryToClear->pszKey = 0;
    pEntryToClear->pszValue = 0;
    pEntryToClear->type = Invalid;
    pEntryToClear->parent = -1;
    pEntryToClear->line = 0;

    // now correct "parent" of all entries...
    int i2;
    for (i2 = 0; i2 < pFS->numEntries; i2++)
    {
        pEntry = &pFS->pEntries[i2];
        if (pEntry->parent != -1)
        {
            if (pEntry->parent == index)
            {
                // we just removed this parent...
                pEntry->parent = -1;
            }
            else if (pEntry->parent > index)
            {
                pEntry->parent--;  // This entry has moved; now one less...
            }
        }
    }
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

static int UaServer_FSBE_ParseConfigFile(char* path)
{
    FileSettings *pFS = &g_settings;
    UALDS_FILE *f = ualds_platform_fopen(path, "r");
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

        len = strlen(szLine);
        /* remove trailing whitespace and line breaks */
        while (len > 0)
        {
            if (szLine[len - 1] != '\r' &&
                szLine[len - 1] != '\n' &&
                szLine[len - 1] != '\t' &&
                szLine[len - 1] != ' ')
            {
                break;
            }
            szLine[len - 1] = 0;
            len--;
        }

        /* skip empty lines */
        if (len == 0)
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
            ualds_log(UALDS_LOG_WARNING, "The configuration file '%s' contains an invalid line in line %i", path, iLine);
        }
    }

    ualds_platform_fclose(f);

    /* reset current group */
    pFS->CurrentGroup = -1;

    return 0;
}

static void UaServer_FSBE_WriteConfigFile_Descriptor(FileSettings *pFS, UALDS_FILE *f)
{
    int i, j;
    if (f)
    {
        /* write all global keys */
        for (i = 0; i<pFS->numEntries; i++)
        {
            if (pFS->pEntries[i].parent != -1) continue;

            switch (pFS->pEntries[i].type)
            {
            case KeyValuePair:
                ualds_platform_fprintf(f, "%s = %s\n", pFS->pEntries[i].pszKey, pFS->pEntries[i].pszValue);
                break;
            case CommentLine:
                ualds_platform_fprintf(f, "%s", pFS->pEntries[i].pszKey);
                break;
            case EmptyLine:
                ualds_platform_fprintf(f, "\n");
                break;
            default:
                break;
            }
        }

        /* write all sections */
        for (i = 0; i<pFS->numEntries; i++)
        {
            if (pFS->pEntries[i].type == Section)
            {
                ualds_platform_fprintf(f, "[%s]\n", pFS->pEntries[i].pszKey);

                /* write all entries for this section */
                for (j = 0; j<pFS->numEntries; j++)
                {                    
                    if (pFS->pEntries[j].parent != i) continue;

                    switch (pFS->pEntries[j].type)
                    {
                    case KeyValuePair:
                        ualds_platform_fprintf(f, "%s = %s\n", pFS->pEntries[j].pszKey, pFS->pEntries[j].pszValue);
                        break;
                    case CommentLine:
                        ualds_platform_fprintf(f, "%s", pFS->pEntries[j].pszKey);
                        break;
                    case EmptyLine:
                        ualds_platform_fprintf(f, "\n");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

static int UaServer_FSBE_WriteConfigFile(void)
{
    FileSettings *pFS = &g_settings;
    if (pFS->readOnly) {
        return EPERM;
    }

    UALDS_FILE *f_backup = ualds_platform_fopen(pFS->szPathBackup, "w");
    if (f_backup)
    {
        UaServer_FSBE_WriteConfigFile_Descriptor(pFS, f_backup);
        ualds_platform_fclose(f_backup);
    }

    UALDS_FILE *f_master = ualds_platform_fopen(pFS->szPath, "w");
    if (f_master)
    {
        UaServer_FSBE_WriteConfigFile_Descriptor(pFS, f_master);
        ualds_platform_fclose(f_master);
    }
    
    return 0;
}

/* It will write to settings to the disk. 
   Usecase: when the config file was corrupt, it will update it with the correct values.
*/
void ualds_settings_update_config_file(void)
{
    FileSettings *pFS = &g_settings;

    UALDS_FILE *f_master = ualds_platform_fopen(pFS->szPath, "w");
    if (f_master)
    {
        UaServer_FSBE_WriteConfigFile_Descriptor(pFS, f_master);
        ualds_platform_fclose(f_master);
    }
}

/* It will check if teh configuration is corrupt.
   Return: 0 if OK, -1 if error.
*/
static int checkConfigConsistency(void)
{
    char tmpString[4096];
    int tmpVal = 0;
    int i,j = 0;
    int NoOfSecurityPolicies = 0;
    const char **szPolicyArray = 0;

    // ---------------------------------------------------------

    // GENERAL
    int retCode = ualds_settings_begingroup("General");
    if (retCode != 0)
    {
        return -1;
    }

    // GENERAL/ServerUri
    retCode = ualds_settings_readstring("ServerUri", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // GENERAL/ExpirationMaxAge
    retCode = ualds_settings_readint("ExpirationMaxAge", &tmpVal);
    if (retCode != 0)
    {
        return -1;
    }
    if (tmpVal <= 0)
    {
        return -1;
    }

    // GENERAL/Endpoints
    retCode = ualds_settings_beginreadarray("Endpoints", &tmpVal);
    if (retCode != 0)
    {
        return -1;
    }
    if (tmpVal <= 0)
    {
        ualds_settings_endarray();
        return -1;
    }
    for (i = 0; i < tmpVal; i++)
    {
        retCode = ualds_settings_setarrayindex(i);
        if (retCode != 0)
        {
            return -1;
        }

        // GENERAL/Endpoints/[i]/Url
        retCode = ualds_settings_readstring("Url", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            return -1;
        }

        // GENERAL/Endpoints/[i]/SecurityPolicies
        retCode = ualds_settings_readstring("SecurityPolicies", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            return -1;
        }

        NoOfSecurityPolicies = split_string(tmpString, ',', &szPolicyArray);
        if (NoOfSecurityPolicies < 1)
        {
            return -1;
        }
    }

    ualds_settings_endarray();

    // GENERAL/ReadOnlyCfg
    retCode = ualds_settings_readint("ReadOnlyCfg", &tmpVal);
    if (retCode == 0)
    {
        if (tmpVal < 0)
        {
            return -1;
        }
    }

    // GENERAL
    retCode = ualds_settings_endgroup();
    if (retCode != 0)
    {
        return -1;
    }

    // ---------------------------------------------------------

    for (i = 0; i < NoOfSecurityPolicies; i++)
    {
        // SECURITYPOLICY_XYZ
        retCode = ualds_settings_begingroup(szPolicyArray[i]);
        if (retCode != 0)
        {
            return -1;
        }

        char tmpUrl[4095];
        // SECURITYPOLICY_XYZ/Url
        retCode = ualds_settings_readstring("Url", tmpUrl, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            return -1;
        }

        // check if it is one of the accepted options: None, Basic128Rsa15, Basic256, Aes128Sha256RsaOaep, Basic256Sha256, Aes256Sha256RsaPss
        if (strcmp(tmpUrl, OpcUa_SecurityPolicy_None) != 0 &&
            strcmp(tmpUrl, OpcUa_SecurityPolicy_Basic128Rsa15) != 0 &&
            strcmp(tmpUrl, OpcUa_SecurityPolicy_Basic256) != 0 &&
			strcmp(tmpUrl, OpcUa_SecurityPolicy_Aes128Sha256RsaOaep) != 0 &&
			strcmp(tmpUrl, OpcUa_SecurityPolicy_Basic256Sha256) != 0 &&
			strcmp(tmpUrl, OpcUa_SecurityPolicy_Aes256Sha256RsaPss) != 0
            )
        {
            return -1;
        }

        // SECURITYPOLICY_XYZ/MessageSecurity
        retCode = ualds_settings_readstring("MessageSecurity", tmpUrl, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            return -1;
        }

        // check if it is one of the accepted options: None, Sign, SignAndEncrypt.
        const char **szModeArray = 0;
        int numModes = split_string(tmpUrl, ',', &szModeArray);
        for (j = 0; j<numModes; j++)
        {
            if ((strcmp(szModeArray[j], "None") != 0) &&
                (strcmp(szModeArray[j], "Sign") != 0) && 
                (strcmp(szModeArray[j], "SignAndEncrypt") != 0)
                )
            {
                if (szModeArray)
                {
                    /* see note above */
                    free((void*)szModeArray);
                    szModeArray = 0;
                }

                return -1;
            }
        }
        if (szModeArray)
        {
            /* see note above */
            free((void*)szModeArray);
            szModeArray = 0;
        }

        // SECURITYPOLICY_XYZ
        retCode = ualds_settings_endgroup();
        if (retCode != 0)
        {
            return -1;
        }
    }

    free((void*)szPolicyArray);
    szPolicyArray = 0;

    // ---------------------------------------------------------

    // CertificateInfo
    retCode = ualds_settings_begingroup("CertificateInfo");
    if (retCode != 0)
    {
        return -1;
    }

    // CertificateInfo/CommonName
    retCode = ualds_settings_readstring("CommonName", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // CertificateInfo/Organization
    retCode = ualds_settings_readstring("Organization", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // CertificateInfo/OrganizationUnit
    retCode = ualds_settings_readstring("OrganizationUnit", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }

    // CertificateInfo/Locality
    retCode = ualds_settings_readstring("Locality", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // CertificateInfo/State
    retCode = ualds_settings_readstring("State", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // CertificateInfo/Country
    retCode = ualds_settings_readstring("Country", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // CertificateInfo
    retCode = ualds_settings_endgroup();
    if (retCode != 0)
    {
        return -1;
    }

    // ---------------------------------------------------------

    // PKI
    retCode = ualds_settings_begingroup("PKI");
    if (retCode != 0)
    {
        return -1;
    }

    // PKI/CertificateStorePath
    retCode = ualds_settings_readstring("CertificateStorePath", tmpString, PATH_MAX);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0 || strlen(tmpString) > (PATH_MAX - 50)) // 50 characters are enough to complete the paths to subfolders. The subfolder structure is fixed.
    {
        return -1;
    }

    // PKI/MaxRejectedCertificates
    retCode = ualds_settings_readint("MaxRejectedCertificates", &tmpVal);
    if (retCode != 0)
    {
        return -1;
    }
    if (tmpVal <= 0)
    {
        return -1;
    }

    // PKI/MaxAgeRejectedCertificates
    retCode = ualds_settings_readint("MaxAgeRejectedCertificates", &tmpVal);
    if (retCode != 0)
    {
        return -1;
    }
    if (tmpVal <= 0)
    {
        return -1;
    }

    // PKI/Win32StoreCheck
    retCode = ualds_settings_readstring("Win32StoreCheck", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if ((strcmp(tmpString, "yes") != 0) &&
        (strcmp(tmpString, "no") != 0)
        )
    {
        return -1;
    }
     
    // PKI/ReCreateOwnCertificateOnError[optinal]
    retCode = ualds_settings_readstring("ReCreateOwnCertificateOnError", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode == 0)
    {
        if ((strcmp(tmpString, "yes") != 0) &&
            (strcmp(tmpString, "no") != 0)
            )
        {
            return -1;
        }
    }

    // PKI/TrustListPath[optinal]
    retCode = ualds_settings_readstring("TrustListPath", tmpString, PATH_MAX);
    if (retCode == 0)
    {
        if (strlen(tmpString) <= 0)
        {
            return -1;
        }
    }

    // PKI/RejectedPath[optinal]
    retCode = ualds_settings_readstring("RejectedPath", tmpString, PATH_MAX);
    if (retCode == 0)
    {
        if (strlen(tmpString) <= 0)
        {
            return -1;
        }
    }

    // PKI/IssuerPath[optinal]
    retCode = ualds_settings_readstring("IssuerPath", tmpString, PATH_MAX);
    if (retCode == 0)
    {
        if (strlen(tmpString) <= 0)
        {
            return -1;
        }
    }

    // PKI/CRLPath[optinal]
    retCode = ualds_settings_readstring("CRLPath", tmpString, PATH_MAX);
    if (retCode == 0)
    {
        if (strlen(tmpString) <= 0)
        {
            return -1;
        }
    }

    // PKI/CertificateFile[optinal]
    retCode = ualds_settings_readstring("CertificateFile", tmpString, PATH_MAX);
    if (retCode == 0)
    {
        if (strlen(tmpString) <= 0)
        {
            return -1;
        }
    }

    // PKI/CertificateKeyFile[optinal]
    retCode = ualds_settings_readstring("CertificateKeyFile", tmpString, PATH_MAX);
    if (retCode == 0)
    {
        if (strlen(tmpString) <= 0)
        {
            return -1;
        }
    }

    // PKI
    retCode = ualds_settings_endgroup();
    if (retCode != 0)
    {
        return -1;
    }

    // ---------------------------------------------------------

    // Log
    retCode = ualds_settings_begingroup("Log");
    if (retCode != 0)
    {
        return -1;
    }

    // Log/LogSystem
    retCode = ualds_settings_readstring("LogSystem", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if ((strcmp(tmpString, "syslog") != 0) &&
        (strcmp(tmpString, "file") != 0)
        )
    {
        return -1;
    }

    // Log/LogFile [optinal]
    if (strcmp(tmpString, "file") == 0)
    {
        retCode = ualds_settings_readstring("LogFile", tmpString, PATH_MAX);
        if (retCode != 0)
        {
            return -1;
        }
        if (strlen(tmpString) <= 0 || strlen(tmpString) > PATH_MAX - 50) // 50 characters are enough to complete the rotate/backup file names
        {
            return -1;
        }
    }

    // Log/LogLevel
    retCode = ualds_settings_readstring("LogLevel", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // Log/LogFileSize [optinal]
    retCode = ualds_settings_readint("LogFileSize", &tmpVal);
    if (retCode == 0)
    {
         if (tmpVal <= 0)
         {
            return -1;
         }
    }


    // Log/StackTrace
    retCode = ualds_settings_readstring("StackTrace", tmpString, UALDS_CONF_MAX_URI_LENGTH);
    if (retCode != 0)
    {
        return -1;
    }
    if (strlen(tmpString) <= 0)
    {
        return -1;
    }

    // Log/LogRotateCount
    retCode = ualds_settings_readint("LogRotateCount", &tmpVal);
    if (retCode == 0)
    {
        if (tmpVal < 0 || tmpVal > 9999)
        {
            return -1;
        }
    }

    // Log
    retCode = ualds_settings_endgroup();
    if (retCode != 0)
    {
        return -1;
    }

    // ---------------------------------------------------------

    // Zeroconf [optinal]
    retCode = ualds_settings_begingroup("Zeroconf");
    if (retCode == 0)
    {
        // Zeroconf/EnableZeroconf [optinal]
        retCode = ualds_settings_readstring("EnableZeroconf", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode == 0)
        {
            if ((strcmp(tmpString, "yes") != 0) &&
                (strcmp(tmpString, "no") != 0)
                )
            {
                return -1;
            }
        }

        // Zeroconf/RegistrationInterval [optinal]
        retCode = ualds_settings_readint("RegistrationInterval", &tmpVal);
        if (retCode == 0)
        {
            if (tmpVal <= 0)
            {
                return -1;
            }
        }
    }

    // Zeroconf
    retCode = ualds_settings_endgroup();
    if (retCode != 0)
    {
        return -1;
    }

    // ---------------------------------------------------------

    // RegisteredServers
    retCode = ualds_settings_begingroup("RegisteredServers");
    if (retCode != 0)
    {
        return -1;
    }

    // RegisteredServers/Servers
    retCode = ualds_settings_beginreadarray("Servers", &tmpVal);
    if (retCode != 0)
    {
        return -1;
    }
    if (tmpVal <= 0)
    {
        ualds_settings_endarray();
        return -1;
    }

    char **szUriArray = 0;
    szUriArray = malloc(tmpVal * sizeof(char*));
    if (szUriArray == NULL)
    {
        return -1;
    }
    
    memset(szUriArray, 0, tmpVal * sizeof(char*));

    for (i = 0; i < tmpVal; i++)
    {
        retCode = ualds_settings_setarrayindex(i);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            ualds_settings_endarray();
            return -1;
        }

        // RegisteredServers/[i]/ServerUri
        retCode = ualds_settings_readstring("ServerUri", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            ualds_settings_endarray();
            return -1;
        }
        szUriArray[i] = malloc(UALDS_CONF_MAX_URI_LENGTH);
        if (szUriArray[i])
        {
            strlcpy(szUriArray[i], tmpString, UALDS_CONF_MAX_URI_LENGTH);
        }
    }

    ualds_settings_endarray();

    for (i = 0; i < tmpVal; i++)
    {
        char* uri = szUriArray[i];
        // ServerUri_XYZ
        retCode = ualds_settings_begingroup(uri);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }

        // ServerUri_XYZ/ProductUri
        retCode = ualds_settings_readstring("ProductUri", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }

        int tmpVal2 = 0;
        // ServerUri_XYZ/ServerNames
        retCode = ualds_settings_beginreadarray("ServerNames", &tmpVal2);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }
        if (tmpVal2 <= 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }

        retCode = ualds_settings_setarrayindex(0);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                 {
                     if (szUriArray[i]) free(szUriArray[i]);
                 }
                 free(szUriArray);
            }
            return -1;
        }

        // ServerUri_XYZ/ServerNames/[0]/Locale
        retCode = ualds_settings_readstring("Locale", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }

        // ServerUri_XYZ/ServerNames/[0]/Text
        retCode = ualds_settings_readstring("Text", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
             if (szUriArray)
             {
                 for (i = 0; i<tmpVal; i++)
                 {
                     if (szUriArray[i]) free(szUriArray[i]);
                 }
                 free(szUriArray);
             }
             return -1;
        }
        ualds_settings_endarray();

        // ServerUri_XYZ/ServerType
        retCode = ualds_settings_readint("ServerType", &tmpVal2);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }
        if (tmpVal2 < 0 || tmpVal2 > 3)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }

        // ServerUri_XYZ/GatewayServerUri
        retCode = ualds_settings_readstring("GatewayServerUri", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }

        // ServerUri_XYZ/DiscoveryUrls
        retCode = ualds_settings_beginreadarray("DiscoveryUrls", &tmpVal2);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }
        if (tmpVal2 <= 0)
        {
            ualds_settings_endarray();
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }
        for (j = 0; j < tmpVal2; j++)
        {
            retCode = ualds_settings_setarrayindex(j);
            if (retCode != 0)
            {
                if (szUriArray)
                {
                    for (i = 0; i<tmpVal; i++)
                    {
                        if (szUriArray[i]) free(szUriArray[i]);
                    }
                    free(szUriArray);
                }
                return -1;
            }

            // ServerUri_XYZ/DiscoveryUrls/[i]/Url
            retCode = ualds_settings_readstring("Url", tmpString, UALDS_CONF_MAX_URI_LENGTH);
            if (retCode != 0)
            {
                if (szUriArray)
                {
                    for (i = 0; i<tmpVal; i++)
                    {
                        if (szUriArray[i]) free(szUriArray[i]);
                    }
                    free(szUriArray);
                }
                return -1;
            }
            if (strlen(tmpString) <= 0)
            {
                if (szUriArray)
                {
                    for (i = 0; i<tmpVal; i++)
                    {
                        if (szUriArray[i]) free(szUriArray[i]);
                    }
                    free(szUriArray);
                }
                return -1;
            }
        }
        ualds_settings_endarray();

        // ServerUri_XYZ/DiscoveryUrls/[i]/SemaphoreFilePath
        retCode = ualds_settings_readstring("SemaphoreFilePath", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode != 0)
        {
            if (szUriArray)
            {
                for (i = 0; i<tmpVal; i++)
                {
                    if (szUriArray[i]) free(szUriArray[i]);
                }
                free(szUriArray);
            }
            return -1;
        }

        // ServerUri_XYZ/UpdateTime [optinal]
        retCode = ualds_settings_readint("UpdateTime", &tmpVal2);
        if (retCode == 0)
        {
            if (tmpVal2 <= 0)
            {
                if (szUriArray)
                {
                    for (i = 0; i<tmpVal; i++)
                    {
                        if (szUriArray[i]) free(szUriArray[i]);
                    }
                    free(szUriArray);
                }
                return -1;
            }
        }

        // ServerUri_XYZ/MdnsServerName [optional]
        retCode = ualds_settings_readstring("MdnsServerName", tmpString, UALDS_CONF_MAX_URI_LENGTH);
        if (retCode == 0)
        {
            if (strlen(tmpString) <= 0)
            {
                if (szUriArray)
                {
                    for (i = 0; i<tmpVal; i++)
                    {
                        if (szUriArray[i]) free(szUriArray[i]);
                    }
                    free(szUriArray);
                }
                return -1;
            }
        }

        // ServerUri_XYZ/ServerCapabilities [optional]
        retCode = ualds_settings_beginreadarray("ServerCapabilities", &tmpVal2);
        if (retCode == 0)
        {
            if (tmpVal2 <= 0)
            {
                ualds_settings_endarray();
                if (szUriArray)
                {
                    for (i = 0; i<tmpVal; i++)
                    {
                        if (szUriArray[i]) free(szUriArray[i]);
                    }
                    free(szUriArray);
                }
                return -1;
            }

            for (j = 0; j < tmpVal2; j++)
            {
                retCode = ualds_settings_setarrayindex(j);
                if (retCode != 0)
                {
                    if (szUriArray)
                    {
                        for (i = 0; i<tmpVal; i++)
                        {
                            if (szUriArray[i]) free(szUriArray[i]);
                        }
                        free(szUriArray);
                    }
                    return -1;
                }

                // ServerUri_XYZ/ServerCapabilities/[j]/Capability
                retCode = ualds_settings_readstring("Capability", tmpString, UALDS_CONF_MAX_URI_LENGTH);
                if (retCode == 0)
                {
                    if (strlen(tmpString) <= 0)
                    {
                        if (szUriArray)
                        {
                            for (i = 0; i<tmpVal; i++)
                            {
                                if (szUriArray[i]) free(szUriArray[i]);
                            }
                            free(szUriArray);
                        }
                        return -1;
                    }
                }
            }
        }
        ualds_settings_endarray();
        ualds_settings_endgroup();
    }

    if (szUriArray)
    {
        for (i = 0; i<tmpVal; i++)
        {
            if (szUriArray[i]) free(szUriArray[i]);
        }
        free(szUriArray);
    }

    // RegisteredServers
    retCode = ualds_settings_endgroup();
    if (retCode != 0)
    {
        return -1;
    }

    // ---------------------------------------------------------

    return 0;
}

static void loadDefaultSettings(void)
{
    // General
    int retCode = ualds_settings_begingroup("General");

    // General/ServerUri
    retCode = ualds_settings_addcomment("# The OPCUA application uri used by the LDS.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writestring("ServerUri", "urn:[gethostname]:UALocalDiscoveryServer");
    retCode = ualds_settings_addemptyline();

    // General/ExpirationMaxAge
    retCode = ualds_settings_addcomment("# maximum age of registered server entries before they expire in seconds. (Default=600)");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("ExpirationMaxAge", 600);
    retCode = ualds_settings_addemptyline();

    retCode = ualds_settings_addcomment("# AllowLocalRegistration: (default=no) this will disable certificate trust checks for connections comming from");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# localhost. Note that this is a security risk. Only enable this feature if you are aware of the full implications.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("#AllowLocalRegistration = yes");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addemptyline();

    retCode = ualds_settings_addcomment("# Endpoint configuration");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# Number of available endpoints");
    retCode = ualds_settings_addemptyline();

    // General/Endpoints
    retCode = ualds_settings_beginwritearray("Endpoints", 1);
    retCode = ualds_settings_setarrayindex(0);

    // General/Url
    retCode = ualds_settings_writestring("Url", "opc.tcp://[gethostname]:4840");

    // General/SecurityPolicies
    retCode = ualds_settings_writestring("SecurityPolicies", "SecurityPolicy_None, SecurityPolicy_Basic128Rsa15, "
		"SecurityPolicy_Basic256, SecurityPolicy_Basic256Sha256, SecurityPolicy_Aes256Sha256RsaPss, SecurityPolicy_Aes128Sha256RsaOaep");
    retCode = ualds_settings_endarray();

    retCode = ualds_settings_addemptyline();

    // General/ReadOnlyCfg
    retCode = ualds_settings_addcomment("# Defines if this configuration file is never written by the LDS");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# ReadOnly (default) > 0; Write = 0.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("ReadOnlyCfg", 1);
    retCode = ualds_settings_addemptyline();

    // General
    retCode = ualds_settings_endgroup();
   
    // ----------------------------------------------------------------

    // SecurityPolicy_None
    retCode = ualds_settings_begingroup("SecurityPolicy_None");

    // SecurityPolicy_None/Url
    retCode = ualds_settings_writestring("Url", OpcUa_SecurityPolicy_None);
    // SecurityPolicy_None/MessageSecurity
    retCode = ualds_settings_writestring("MessageSecurity", "None");

    retCode = ualds_settings_addemptyline();

    // SecurityPolicy_None
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

    // SecurityPolicy_Basic128Rsa15
    retCode = ualds_settings_begingroup("SecurityPolicy_Basic128Rsa15");

    // SecurityPolicy_Basic128Rsa15/Url
    retCode = ualds_settings_writestring("Url", OpcUa_SecurityPolicy_Basic128Rsa15);
    // SecurityPolicy_Basic128Rsa15/MessageSecurity
    retCode = ualds_settings_writestring("MessageSecurity", "Sign, SignAndEncrypt");

    retCode = ualds_settings_addemptyline();

    // SecurityPolicy_Basic128Rsa15
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

    // SecurityPolicy_Basic256
    retCode = ualds_settings_begingroup("SecurityPolicy_Basic256");

    // SecurityPolicy_Basic256/Url
    retCode = ualds_settings_writestring("Url", OpcUa_SecurityPolicy_Basic256);
    // SecurityPolicy_Basic256/MessageSecurity
    retCode = ualds_settings_writestring("MessageSecurity", "Sign, SignAndEncrypt");

    retCode = ualds_settings_addemptyline();

    // SecurityPolicy_Basic256
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

	// SecurityPolicy_Aes128Sha256RsaOaep
	retCode = ualds_settings_begingroup("SecurityPolicy_Aes128Sha256RsaOaep");

	// SecurityPolicy_Aes128Sha256RsaOaep/Url
	retCode = ualds_settings_writestring("Url", OpcUa_SecurityPolicy_Aes128Sha256RsaOaep);
	// SecurityPolicy_Aes128Sha256RsaOaep/MessageSecurity
	retCode = ualds_settings_writestring("MessageSecurity", "Sign, SignAndEncrypt");

	retCode = ualds_settings_addemptyline();

	// SecurityPolicy_Aes128Sha256RsaOaep
	retCode = ualds_settings_endgroup();

	// ----------------------------------------------------------------

	// SecurityPolicy_Basic256Sha256
	retCode = ualds_settings_begingroup("SecurityPolicy_Basic256Sha256");

	// SecurityPolicy_Basic256Sha256/Url
	retCode = ualds_settings_writestring("Url", OpcUa_SecurityPolicy_Basic256Sha256);
	// SecurityPolicy_Basic256Sha256/MessageSecurity
	retCode = ualds_settings_writestring("MessageSecurity", "Sign, SignAndEncrypt");

	retCode = ualds_settings_addemptyline();

	// SecurityPolicy_Basic256Sha256
	retCode = ualds_settings_endgroup();

	// ----------------------------------------------------------------

	// SecurityPolicy_Aes256Sha256RsaPss
	retCode = ualds_settings_begingroup("SecurityPolicy_Aes256Sha256RsaPss");

	// SecurityPolicy_Aes256Sha256RsaPss/Url
	retCode = ualds_settings_writestring("Url", OpcUa_SecurityPolicy_Aes256Sha256RsaPss);
	// SecurityPolicy_Aes256Sha256RsaPss/MessageSecurity
	retCode = ualds_settings_writestring("MessageSecurity", "Sign, SignAndEncrypt");

	retCode = ualds_settings_addemptyline();

	// SecurityPolicy_Aes256Sha256RsaPss
	retCode = ualds_settings_endgroup();

	// ----------------------------------------------------------------

    // CertificateInfo
    retCode = ualds_settings_begingroup("CertificateInfo");
    retCode = ualds_settings_addcomment("# Certificate information for creating self-signed certificates on startup");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# This is only used if no certificate exists in the configured path.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# On machine name change, the own certificate should be re-created by uninstalling/reinstalling LDS OR by deleting the actual certificate and restarting the service.");
    retCode = ualds_settings_addemptyline();

    // CertificateInfo/CommonName
    retCode = ualds_settings_writestring("CommonName", "UA Local Discovery Server");
    // CertificateInfo/Organization
    retCode = ualds_settings_writestring("Organization", "OPC Foundation");
    // CertificateInfo/OrganizationUnit
    retCode = ualds_settings_writestring("OrganizationUnit", "");
    // CertificateInfo/Locality
    retCode = ualds_settings_writestring("Locality", "16101 N. 82nd Street, Scottsdale");
    // CertificateInfo/State
    retCode = ualds_settings_writestring("State", "Arizona");
    // CertificateInfo/Country
    retCode = ualds_settings_writestring("Country", "US");

    retCode = ualds_settings_addemptyline();

    // CertificateInfo
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

    // PKI
    retCode = ualds_settings_begingroup("PKI");

    // PKI/CertificateStorePath
    retCode = ualds_settings_addcomment("#Certificate Store Path");
    retCode = ualds_settings_addemptyline();
    char certifFolder[PATH_MAX];
    getDefaultCertificateFolder(certifFolder, PATH_MAX);
    retCode = ualds_settings_writestring("CertificateStorePath", certifFolder);
    // PKI/MaxRejectedCertificates
    retCode = ualds_settings_addcomment("# Maximum number of rejected certificates. This prevents malicious programs from filling up available disk space by");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# calling OpenSecureChannel with different certificates.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("MaxRejectedCertificates", 50);
    // PKI/MaxAgeRejectedCertificates
    retCode = ualds_settings_addcomment("# MaxAge for rejected certificates: Rejected certificates will be deleted automatically after this time in days.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("MaxAgeRejectedCertificates", 1);
    // PKI/Win32StoreCheck
    retCode = ualds_settings_addcomment("# Windows certificate store (only for backward compatibility to old LDS store)");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# Enable certificate validation using the given windows certificate store (yes / no)");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writestring("Win32StoreCheck", "yes");
    // PKI/ReCreateOwnCertificateOnError
    retCode = ualds_settings_addcomment("# If this is 'yes' then the certificate will be automatically re-created if the machine name has changed");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# Default behavior is \"no\".");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writestring("ReCreateOwnCertificateOnError", "no");

    // old keys for backward compatibility
    retCode = ualds_settings_addcomment("# ReadOnly information for 3rd party applications");
    retCode = ualds_settings_addemptyline();

    char* directory_separator = __ualds_plat_path_sep;

    // PKI/CertificateFile
    char certifFile[PATH_MAX];
    memset(certifFile, 0, PATH_MAX);
    strlcpy(certifFile, certifFolder, PATH_MAX);
    strlcat(certifFile, directory_separator, PATH_MAX);
    strlcat(certifFile, "own", PATH_MAX);
    strlcat(certifFile, directory_separator, PATH_MAX);
    strlcat(certifFile, "certs", PATH_MAX);
    strlcat(certifFile, directory_separator, PATH_MAX);
    strlcat(certifFile, "ualdscert.der", PATH_MAX);
    retCode = ualds_settings_writestring("CertificateFile", certifFile);
    // PKI/CertificateKeyFile
    char certifFileKey[PATH_MAX];
    memset(certifFileKey, 0, PATH_MAX);
    strlcpy(certifFileKey, certifFolder, PATH_MAX);
    strlcat(certifFileKey, directory_separator, PATH_MAX);
    strlcat(certifFileKey, "own", PATH_MAX);
    strlcat(certifFileKey, directory_separator, PATH_MAX);
    strlcat(certifFileKey, "private", PATH_MAX);
    strlcat(certifFileKey, directory_separator, PATH_MAX);
    strlcat(certifFileKey, "ualdskey.nopass.pem", PATH_MAX);
    retCode = ualds_settings_writestring("CertificateKeyFile", certifFileKey);
    // PKI/CRLPath
    char cRLPath[PATH_MAX];
    memset(cRLPath, 0, PATH_MAX);
    strlcpy(cRLPath, certifFolder, PATH_MAX);
    strlcat(cRLPath, directory_separator, PATH_MAX);
    strlcat(cRLPath, "trusted", PATH_MAX);
    strlcat(cRLPath, directory_separator, PATH_MAX);
    strlcat(cRLPath, "crl", PATH_MAX);
    retCode = ualds_settings_writestring("CRLPath", cRLPath);
    // PKI/TrustListPath
    char trustListPath[PATH_MAX];
    memset(trustListPath, 0, PATH_MAX);
    strlcpy(trustListPath, certifFolder, PATH_MAX);
    strlcat(trustListPath, directory_separator, PATH_MAX);
    strlcat(trustListPath, "trusted", PATH_MAX);
    strlcat(trustListPath, directory_separator, PATH_MAX);
    strlcat(trustListPath, "certs", PATH_MAX);
    retCode = ualds_settings_writestring("TrustListPath", trustListPath);
    // PKI/IssuerPath
    char issuerPath[PATH_MAX];
    memset(issuerPath, 0, PATH_MAX);
    strlcpy(issuerPath, certifFolder, PATH_MAX);
    strlcat(issuerPath, directory_separator, PATH_MAX);
    strlcat(issuerPath, "issuer", PATH_MAX);
    strlcat(issuerPath, directory_separator, PATH_MAX);
    strlcat(issuerPath, "certs", PATH_MAX);
    retCode = ualds_settings_writestring("IssuerPath", issuerPath);
    // PKI/RejectedPath
    char rejectedPath[PATH_MAX];
    memset(rejectedPath, 0, PATH_MAX);
    strlcpy(rejectedPath, certifFolder, PATH_MAX);
    strlcat(rejectedPath, directory_separator, PATH_MAX);
    strlcat(rejectedPath, "rejected", PATH_MAX);
    strlcat(rejectedPath, directory_separator, PATH_MAX);
    strlcat(rejectedPath, "certs", PATH_MAX);
    retCode = ualds_settings_writestring("RejectedPath", rejectedPath);

    retCode = ualds_settings_addemptyline();

    // PKI
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

    // Log
    retCode = ualds_settings_begingroup("Log");

    // Log/LogSystem
    retCode = ualds_settings_addcomment("# Log System: syslog (system log), file (custom log file)");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writestring("LogSystem", "file");
    // Log/LogFile
    retCode = ualds_settings_addcomment("# LogFile: path to logfile. This is only required for LogSystem=file.");
    retCode = ualds_settings_addemptyline();
    char logFilePath[1024];
    getDefaultLogFilePath(logFilePath, 1024);
    retCode = ualds_settings_writestring("LogFile", logFilePath);
    // Log/LogLevel
    retCode = ualds_settings_addcomment("# Log Level: error, warn, info, debug");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writestring("LogLevel", "error");
    // Log/LogFileSize
    retCode = ualds_settings_addcomment("# Maximum logfile size in MB. This is only required for LogSystem=file.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("LogFileSize", 100);
    // Log/StackTrace
    retCode = ualds_settings_addcomment("# Activate UaStack trace: off, error, warn, info, debug");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writestring("StackTrace", "error");
    // Log/LogRotateCount
    retCode = ualds_settings_addcomment("# LogRotateCount: Maximum number of logfiles. This is optional for LogSystem=file. Default is '0' (no restriction in logfiles).");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_writeint("LogRotateCount", 2);

    retCode = ualds_settings_addemptyline();

    // Log
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

    // RegisteredServers
    retCode = ualds_settings_begingroup("RegisteredServers");

    retCode = ualds_settings_addcomment("# This section contains all registered server entries. The first entry is always the LDS itself.\n");
    retCode = ualds_settings_addcomment("# Servers/size is the number of entries.\n");
    retCode = ualds_settings_addcomment("# Servers/0/ServerUri is the first entry.\n");
    retCode = ualds_settings_addcomment("# Servers/1/ServerUri is the second entry and so on.\n");
    retCode = ualds_settings_addcomment("# For each ServerUri there must exist a section with the same name\n");
    retCode = ualds_settings_addcomment("# which contains the detailed information of the server entry.\n");

    retCode = ualds_settings_beginwritearray("Servers", 1);
    retCode = ualds_settings_endarray();

    // RegisteredServers/Servers
    retCode = ualds_settings_beginwritearray("Servers", 1);
    // RegisteredServers/[0]
    retCode = ualds_settings_setarrayindex(0);
    // RegisteredServers/[0]ServerUri
    retCode = ualds_settings_writestring("ServerUri", "urn:[gethostname]:UALocalDiscoveryServer");
    // RegisteredServers/[0]
    retCode = ualds_settings_endarray();

    retCode = ualds_settings_addemptyline();

    // RegisteredServers
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

    // urn:[gethostname]:UALocalDiscoveryServer
    retCode = ualds_settings_begingroup("urn:[gethostname]:UALocalDiscoveryServer");

    retCode = ualds_settings_addcomment("# The Local Discovery Server entry. This is always the first entry.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# ProductUri: The ProductUri from the RegisteredServer structure as defined in the");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# OPC UA Specification - Part 4, Chapter 5.4.4 RegisterServer");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# ServerNames Array: Array of LocalizedText entries");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# ServerType: 0=Server, 1=Client, 2=ClientAndServer, 3=DiscoveryServer");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# GatewayServerUri:");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# DiscoveryUrls: Array of Discovery Urls");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# SemaphoreFilePath: The semaphore file makes this entry a static entry. This entry will only be removed when the configured");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# semaphore file does not exist anymore. '*' is used to mark the entry to live forever.");
    retCode = ualds_settings_addemptyline();
    retCode = ualds_settings_addcomment("# UpdateTime: time_t when the server entry was updated the last time. Only used for dynamic registration when SemaphoreFilePath is empty.");
    retCode = ualds_settings_addemptyline();
    // urn:[gethostname]:UALocalDiscoveryServer/ProductUri
    retCode = ualds_settings_writestring("ProductUri", "http://opcfoundation.org/UA/LocalDiscoveryServer");
    // urn:[gethostname]:UALocalDiscoveryServer/ServerNames
    retCode = ualds_settings_beginwritearray("ServerNames", 1);
    // urn:[gethostname]:UALocalDiscoveryServer/ServerNames/[0]
    retCode = ualds_settings_setarrayindex(0);
    // urn:[gethostname]:UALocalDiscoveryServer/ServerNames/[0]/Locale
    retCode = ualds_settings_writestring("Locale", "en-US");
    // urn:[gethostname]:UALocalDiscoveryServer/ServerNames/[0]/Text
    retCode = ualds_settings_writestring("Text", "UA Local Discovery Server");
    // urn:[gethostname]:UALocalDiscoveryServer/ServerNames
    retCode = ualds_settings_endarray();
    retCode = ualds_settings_addcomment("# ServerType: 0=Server, 1=Client, 2=ClientAndServer, 3=DiscoveryServer");
    retCode = ualds_settings_addemptyline();
    // urn:[gethostname]:UALocalDiscoveryServer/ServerType
    retCode = ualds_settings_writeint("ServerType", 3);
    // urn:[gethostname]:UALocalDiscoveryServer/GatewayServerUri
    retCode = ualds_settings_writestring("GatewayServerUri", "");
    // urn:[gethostname]:UALocalDiscoveryServer/DiscoveryUrls
    retCode = ualds_settings_beginwritearray("DiscoveryUrls", 1);
    // urn:[gethostname]:UALocalDiscoveryServer/DiscoveryUrls/[0]
    retCode = ualds_settings_setarrayindex(0);
    // urn:[gethostname]:UALocalDiscoveryServer/DiscoveryUrls/[0]/Url
    retCode = ualds_settings_writestring("Url", "opc.tcp://[gethostname]");
    // urn:[gethostname]:UALocalDiscoveryServer/DiscoveryUrls
    retCode = ualds_settings_endarray();
    // urn:[gethostname]:UALocalDiscoveryServer/SemaphoreFilePath
    retCode = ualds_settings_writestring("SemaphoreFilePath", "*");
    // urn:[gethostname]:UALocalDiscoveryServer/MdnsServerName
    retCode = ualds_settings_writestring("MdnsServerName", "UA Local Discovery Server on [gethostname]");
    // urn:[gethostname]:UALocalDiscoveryServer/ServerCapabilities
    retCode = ualds_settings_beginwritearray("ServerCapabilities", 1);
    // urn:[gethostname]:UALocalDiscoveryServer/ServerCapabilities/[0]
    retCode = ualds_settings_setarrayindex(0);
    // urn:[gethostname]:UALocalDiscoveryServer/ServerCapabilities/[0]/Capability
    retCode = ualds_settings_writestring("Capability", "LDS");
    // urn:[gethostname]:UALocalDiscoveryServer/ServerCapabilities
    retCode = ualds_settings_endarray();

    retCode = ualds_settings_addemptyline();

    // urn:[gethostname]:UALocalDiscoveryServer
    retCode = ualds_settings_endgroup();

    // ----------------------------------------------------------------

    // Zeroconf
    retCode = ualds_settings_begingroup("Zeroconf");
    retCode = ualds_settings_addcomment("# Enable Zeroconf functionality in the LDS (default=yes)");
    retCode = ualds_settings_addemptyline();
    // Zeroconf/EnableZeroconf
    retCode = ualds_settings_writestring("EnableZeroconf", "yes");
    retCode = ualds_settings_addcomment("# Interval in which the LDS checks the registrations at the local Zeroconf service (minimum/default: 5)");
    retCode = ualds_settings_addemptyline();
    // Zeroconf/EnableZeroconf
    retCode = ualds_settings_writeint("RegistrationInterval", 5);

    retCode = ualds_settings_addemptyline();

    // Zeroconf
    retCode = ualds_settings_endgroup();

    (void)retCode;
}

/** Opens the given settings file \c szFilename. */
int ualds_settings_open(const char *szFilename)
{
    g_settings.szPath = strdup(szFilename);
    g_settings.szPathBackup = malloc(1024 * sizeof(char*));
    strlcpy(g_settings.szPathBackup, g_settings.szPath, PATH_MAX);
    strlcat(g_settings.szPathBackup, ".bak", PATH_MAX);

    g_settings.CurrentGroup = -1;
    UaServer_FSBE_ParseConfigFile(g_settings.szPath);

    // check if everything is ok
    int ret = checkConfigConsistency();
    if (ret != 0)
    {
        ualds_settings_close(0);

        ualds_settings_open_from_backup(szFilename);

        ret = checkConfigConsistency();
        if (ret != 0)
        {
            // create default values
            ualds_settings_close(0);
            
            ualds_settings_open_from_default(szFilename);
        }

        ualds_settings_update_config_file();
    }

    return 0;
}

int ualds_settings_open_from_backup(const char *szFilename)
{
    g_settings.szPath = strdup(szFilename);
    g_settings.szPathBackup = malloc(1024 * sizeof(char*));
    strlcpy(g_settings.szPathBackup, g_settings.szPath, PATH_MAX);
    strlcat(g_settings.szPathBackup, ".bak", PATH_MAX);

    g_settings.CurrentGroup = -1;
    UaServer_FSBE_ParseConfigFile(g_settings.szPathBackup);

    return 0;
}

int ualds_settings_open_from_default(const char *szFilename)
{
    g_settings.szPath = strdup(szFilename);
    g_settings.szPathBackup = malloc(1024 * sizeof(char*));
    strlcpy(g_settings.szPathBackup, g_settings.szPath, PATH_MAX);
    strlcat(g_settings.szPathBackup, ".bak", PATH_MAX);

    loadDefaultSettings();

    return 0;
}

/** This writes all changes to file.
 * You can use this function to force changes to be written to the file
 * without closing it.
 */
int ualds_settings_flush(void)
{
    UaServer_FSBE_WriteConfigFile();
    return 0;
}

/** Closes the file that was previously opened by ualds_settings_open.
 * This will atomically flush any changes.
 // param flush
 //          != 0 => flush to disk
 //          == 0 => don't flush to disk
 */
int ualds_settings_close(int flush)
{
    FileSettings *pFS = &g_settings;
    int ret = 0;
    int i;

    if (pFS->modified && flush != 0)
    {
        ualds_settings_flush();
    }

    free(pFS->szPathBackup);
    pFS->szPathBackup = 0;

    free(pFS->szPath);
    pFS->szPath = 0;
    
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
int ualds_settings_endgroup(void)
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

    if (strcmp("(null)", pFS->pEntries[index].pszValue) != 0)
    {
        strlcpy(szValue, pFS->pEntries[index].pszValue, len);
    }
    else if (len > 0)
    {
        szValue[0] = '\0';
    }

    return 0;
}

/** Sets the read only flag. */
int ualds_settings_setReadOnly(int readOnly)
{
    FileSettings *pFS = &g_settings;
    pFS->readOnly = readOnly;
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
 *  ualds_settings_close(1);
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
 *  ualds_settings_close(1);
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
int ualds_settings_endarray(void)
{
    FileSettings *pFS = &g_settings;
    pFS->szArrayKey = 0;

    return 0;
}

int ualds_settings_addcomment(const char* szComment)
{
    FileSettings *pFS = &g_settings;
    Entry *pEntry = UaServer_FSBE_AddComment(pFS, szComment);
    if (pEntry)
    {
        pEntry->parent = pFS->CurrentGroup;
        return 0;
    }

    return -1;
}

int ualds_settings_addemptyline(void)
{
    FileSettings *pFS = &g_settings;
    Entry *pEntry = UaServer_FSBE_AddEmptyLine(pFS);
    if (pEntry)
    {
        pEntry->parent = pFS->CurrentGroup;
        return 0;
    }

    return -1;
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
    for (i=0; i<pFS->numEntries; i++)
    {
        pEntry = &pFS->pEntries[i];
        if (pFS->CurrentGroup == pEntry->parent &&
            (pEntry->type == EmptyLine ||
             pEntry->pszKey == 0 ||
             strncmp(pEntry->pszKey, szKey, len) == 0))
        {
            UaServer_FSBE_RemoveEntry(pFS, i);
            i--;  // decrement index, so we start using the next entry, because we really removed the entry!
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
                if (index > i)  // this normally does not happen, because the section is first created and then the childs..
                {
                    index--;
                }
                i--;  // decrement index, so we start using the next entry, because we really removed the entry!
            }
        }
        /* remove the section */
        UaServer_FSBE_RemoveEntry(pFS, index);
    }

    pFS->modified = 1;

    return 0;
}

void ualds_settings_dump(char* pText)
{
    FileSettings *pFS = &g_settings;
    int i, j;
    char szTemp[1024];
    /* write all global keys */
    for (i = 0; i<pFS->numEntries; i++)
    {
        if (pFS->pEntries[i].parent != -1) continue;

        switch (pFS->pEntries[i].type)
        {
        case KeyValuePair:
            sprintf(szTemp, "%s = %s\n", pFS->pEntries[i].pszKey, pFS->pEntries[i].pszValue);
            strcat(pText, szTemp);
            break;
        case CommentLine:
            sprintf(szTemp, "%s", pFS->pEntries[i].pszKey);
            strcat(pText, szTemp);
            break;
        case EmptyLine:
            sprintf(szTemp, "\n");
            strcat(pText, szTemp);
            break;
        default:
            break;
        }
    }

    /* write all sections */
    for (i = 0; i<pFS->numEntries; i++)
    {
        if (pFS->pEntries[i].type == Section)
        {
            sprintf(szTemp, "[%s]\n", pFS->pEntries[i].pszKey);
            strcat(pText, szTemp);

            /* write all entries for this section */
            for (j = 0; j<pFS->numEntries; j++)
            {
                if (pFS->pEntries[j].parent != i) continue;

                switch (pFS->pEntries[j].type)
                {
                case KeyValuePair:
                    sprintf(szTemp, "%s = %s\n", pFS->pEntries[j].pszKey, pFS->pEntries[j].pszValue);
                    strcat(pText, szTemp);
                    break;
                case CommentLine:
                    sprintf(szTemp, "%s", pFS->pEntries[j].pszKey);
                    strcat(pText, szTemp);
                    break;
                case EmptyLine:
                    sprintf(szTemp, "\n");
                    strcat(pText, szTemp);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void ualds_settings_clear(void)
{
    int i;
    FileSettings *pFS = &g_settings;
    if (pFS->pEntries)
    {
        for (i = 0; i < pFS->numEntries; i++)
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
        pFS->EntrySize = 0;
        pFS->modified = 0;
    }
}


/**
 * @}
 */

