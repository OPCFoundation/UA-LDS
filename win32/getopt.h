/******************************************************************************
**
** Copyright (C) 2005-2013 Unified Automation GmbH. All Rights Reserved.
** Web: http://www.unifiedautomation.com
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Project: OPC UA ANSI C SDK
**
** Description: Windows getopt() implementation
**
******************************************************************************/

#ifndef __WINGETOPT_H__
#define __WINGETOPT_H__

extern char           *optarg; /* Global argument pointer. */
extern int             optind; /* Global argv index.       */

int getopt(int argc, char *argv[], char *optstring);

#endif /* __WINGETOPT_H__ */
