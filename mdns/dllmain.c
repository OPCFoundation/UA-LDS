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

#include <windows.h>
#include <DebugServices.h>
#include <stdlib.h>

BOOL APIENTRY	DllMain( HANDLE inModule, DWORD inReason, LPVOID inReserved )
{
	(void) inModule;
	(void) inReserved;
	
	switch( inReason )
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
    return( TRUE );
}


BOOL
IsSystemServiceDisabled()
{
	ENUM_SERVICE_STATUS	*	lpService = NULL;
	SC_HANDLE					sc;
	BOOL							ret = FALSE;
	BOOL							ok;
	DWORD							bytesNeeded = 0;
	DWORD							srvCount;
	DWORD							resumeHandle = 0;
	DWORD							srvType;
	DWORD							srvState;
	DWORD							dwBytes = 0;
	DWORD							i;
	OSStatus						err;

	sc = OpenSCManager( NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE );
	err = translate_errno( sc, GetLastError(), kUnknownErr );
	require_noerr( err, exit );

	srvType		=	SERVICE_WIN32;
	srvState		=	SERVICE_STATE_ALL;

	for ( ;; )
	{
		// Call EnumServicesStatus using the handle returned by OpenSCManager

		ok = EnumServicesStatus ( sc, srvType, srvState, lpService, dwBytes, &bytesNeeded, &srvCount, &resumeHandle );

		if ( ok || ( GetLastError() != ERROR_MORE_DATA ) )
		{
			break;
		}

		if ( lpService )
		{
			free( lpService );
		}

		dwBytes = bytesNeeded;

		lpService = ( ENUM_SERVICE_STATUS* ) malloc( dwBytes );
		require_action( lpService, exit, ret = FALSE );
	}

	err = translate_errno( ok, GetLastError(), kUnknownErr );
	require_noerr( err, exit );

	for ( i = 0; i < srvCount; i++ )
	{
		if ( strcmp( lpService[i].lpServiceName, "Bonjour Service" ) == 0 )
		{
			if ( ( lpService[i].ServiceStatus.dwCurrentState == SERVICE_PAUSED ) || ( lpService[i].ServiceStatus.dwCurrentState == SERVICE_STOPPED ) )
			{
				ret = TRUE;
			}

			break;
		}
	}

exit:

	if ( lpService )
	{
		free( lpService );
	}

	if ( sc )
	{
		CloseServiceHandle ( sc );
	}

	return ret;
}
