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

#ifndef _OpcUa_Credentials_H_
#define _OpcUa_Credentials_H_ 1

OPCUA_BEGIN_EXTERN_C

/*============================================================================
 * OpcUa_ActualCredential
 *
 * An actually used credential.
 *===========================================================================*/
typedef struct _OpcUa_ActualCredential
{
    OpcUa_ByteString*               pClientCertificate;
    OpcUa_Key*                      pClientPrivateKey;
    OpcUa_ByteString*               pServerCertificate;
    OpcUa_Void*                     pkiConfig;
    OpcUa_String*                   pRequestedSecurityPolicyUri;
    OpcUa_Int32                     nRequestedLifetime;
    OpcUa_MessageSecurityMode       messageSecurityMode;
}
OpcUa_ActualCredential;

/*============================================================================
 * OpcUa_ClientCredential
 *
 * A union of all possible credential types.
 *
 *===========================================================================*/
typedef struct _OpcUa_ClientCredential
{
    union
    {
    OpcUa_ActualCredential   TheActuallyUsedCredential;
    }
    Credential;
}
OpcUa_ClientCredential;

OPCUA_END_EXTERN_C

#endif /* _OpcUa_Credentials_H_ */
