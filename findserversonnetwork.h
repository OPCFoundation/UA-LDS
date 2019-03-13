/* Copyright (c) 1996-2019, OPC Foundation. All rights reserved.

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

#ifndef __FINDSERVERSONNETWORK_H__
#define __FINDSERVERSONNETWORK_H__

#include <opcua_proxystub.h>

OpcUa_StatusCode ualds_findserversonnetwork_start_listening();
void ualds_findserversonnetwork_stop_listening();

void ualds_findserversonnetwork_socketEventCallback(int *shutdown);

void ualds_zeroconf_registerOffline(const char *szServerUri);
void ualds_zeroconf_unregisterOffline(const char *szServerUri);
void ualds_zeroconf_loadOffline();
void ualds_zeroconf_cleanupOffline();

#endif /* __FINDSERVERSONNETWORK_H__ */
