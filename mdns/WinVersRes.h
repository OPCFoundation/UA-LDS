/* ========================================================================
 * Copyright (c) 2005-2026, OPC Federation AISBL, All rights reserved.
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

#ifndef WINRESVERS_H
#define WINRESVERS_H

#define MASTER_PROD_NAME	"Bonjour"

// Define the company name for mDNSResponder on Windows
#define MASTER_COMPANY_NAME   "Apple Inc."

// Define the product version for mDNSResponder on Windows
#define MASTER_PROD_VERS		3,0,0,2
#define MASTER_PROD_VERS_STR	"3,0,0,2"
#define MASTER_PROD_VERS_STR2	"3.0.0.2"
#define MASTER_PROD_VERS_STR3 "Explorer Plugin 3.0.0.2"

// Define the legal copyright
#define MASTER_LEGAL_COPYRIGHT "Copyright (C) 2003-2011 Apple Inc."

// Library name
#ifdef NDEBUG
#  define LIBRARY_NAME "dnssd.dll"
#else
#  define LIBRARY_NAME "dnssdd.dll"
#endif

#endif // WINRESVERS_H
