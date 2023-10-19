/* Copyright (c) 1996-2023, OPC Foundation. All rights reserved.

The source code in this file is covered under a dual-license scenario:
- RCL: for OPC Foundation members in good-standing
- GPL V2: everybody else

RCL license terms accompanied with this source code. See http://opcfoundation.org/License/RCL/1.00/

GNU General Public License as published by the Free Software Foundation;
version 2 of the License are accompanied with this source code. See http://opcfoundation.org/License/GPLv2

This source code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef __CERTSTORE_H__
#define __CERTSTORE_H__

#include <opcua_proxystub.h>

OpcUa_StatusCode ualds_verify_cert_win32(OpcUa_ByteString* pbsClientCertificate);

// the below checks are for backward compatibility woth older LDS versions
OpcUa_StatusCode ualds_verify_cert_old_default_location(OpcUa_ByteString* pbsClientCertificate, char* newCertificateStorePathCrl, 
    char* newCertificateStorePathRejected, OpcUa_Int* pValidationCode);
OpcUa_StatusCode ualds_verify_cert_old_edited_location(OpcUa_ByteString* pbsClientCertificate, char* newCertificateStorePathCrl, 
    char* newCertificateStorePathRejected, char* trustListPathOldEditedLocation, OpcUa_Int* pValidationCode);

#endif /* __CERTSTORE_H__ */
