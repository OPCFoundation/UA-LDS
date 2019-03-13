/* ========================================================================
 * Copyright (c) 2005-2016 The OPC Foundation, Inc. All rights reserved.
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
 
/* serverstub (basic includes for implementing a server based on the stack) */
#include <opcua_serverstub.h>
#include <opcua_string.h>
#include <opcua_memory.h>
#include <opcua_trace.h>
#include "addressspace.h"
#include "browseservice.h"
#include "mytrace.h"
#include "general_header.h"







/*============================================================================
 * A stub method which implements the BrowseNext service.
 *===========================================================================*/
OpcUa_StatusCode my_BrowseNext(
    OpcUa_Endpoint             a_hEndpoint,
    OpcUa_Handle               a_hContext,
    const OpcUa_RequestHeader* a_pRequestHeader,
    OpcUa_Boolean              a_bReleaseContinuationPoints,
    OpcUa_Int32                a_nNoOfContinuationPoints,
    const OpcUa_ByteString*    a_pContinuationPoints,
    OpcUa_ResponseHeader*      a_pResponseHeader,
    OpcUa_Int32*               a_pNoOfResults,
    OpcUa_BrowseResult**       a_pResults,
    OpcUa_Int32*               a_pNoOfDiagnosticInfos,
    OpcUa_DiagnosticInfo**     a_pDiagnosticInfos)
{
	extern _my_continuationpoint_	Continuation_Point_Data;
	OpcUa_Int               m;
	extern OpcUa_UInt32		securechannelId;
	extern OpcUa_UInt32		session_flag;
	extern OpcUa_Double		msec_counter;
	extern OpcUa_String*	p_user_name;

    OpcUa_InitializeStatus(OpcUa_Module_Server, "my_BrowseNext");

    /* Validate arguments. */
    OpcUa_ReturnErrorIfArgumentNull(a_hEndpoint);
    OpcUa_ReturnErrorIfArgumentNull(a_hContext);
    OpcUa_ReturnErrorIfArgumentNull(a_pRequestHeader);
    OpcUa_ReferenceParameter(a_bReleaseContinuationPoints);
    OpcUa_ReturnErrorIfArrayArgumentNull(a_nNoOfContinuationPoints, a_pContinuationPoints);
    OpcUa_ReturnErrorIfArgumentNull(a_pResponseHeader);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pResults);
    OpcUa_ReturnErrorIfArgumentNull(a_pNoOfDiagnosticInfos);
    OpcUa_ReturnErrorIfArgumentNull(a_pDiagnosticInfos);

	*a_pNoOfDiagnosticInfos=0;
	*a_pDiagnosticInfos=OpcUa_Null;

	RESET_SESSION_COUNTER

#ifndef NO_DEBUGGING_
	MY_TRACE("\n\n\nBROWSENEXT SERVICE=========================================\n");
	if(p_user_name!=OpcUa_Null)
		MY_TRACE("\nUser:%s\n",OpcUa_String_GetRawString(p_user_name)); 
#endif /*_DEBUGGING_*/


	if(OpcUa_IsBad(session_flag))
	{
		/* Tell client that session is not active. */
#ifndef NO_DEBUGGING_
		MY_TRACE("\nSession not activated\n"); 
#endif /*_DEBUGGING_*/
		uStatus=OpcUa_BadSessionNotActivated;
		OpcUa_GotoError;
	}

	uStatus=check_authentication_token(a_pRequestHeader);
	if(OpcUa_IsBad(uStatus))
	{
#ifndef NO_DEBUGGING_
		MY_TRACE("\nInvalid Authentication Token.\n"); 
#endif /*_DEBUGGING_*/
		OpcUa_GotoError;
	}

	if(a_bReleaseContinuationPoints==OpcUa_False)
	{
		*a_pResults=OpcUa_Memory_Alloc(a_nNoOfContinuationPoints*sizeof(OpcUa_BrowseResult));
		OpcUa_GotoErrorIfAllocFailed((*a_pResults))

		*a_pNoOfResults=a_nNoOfContinuationPoints;

		for(m=0;m<a_nNoOfContinuationPoints;m++)
		{
			OpcUa_BrowseResult_Initialize((*a_pResults+m));
			if((a_pContinuationPoints+m)->Data!=OpcUa_Null && ((a_pContinuationPoints+m)->Length>=sizeof(OpcUa_Int)))
			{
				OpcUa_Int cpid=*(OpcUa_Int*)(a_pContinuationPoints+m)->Data;
				if(cpid==Continuation_Point_Data.Cont_Point_Identifier)
				{
					#ifndef NO_DEBUGGING_
						MY_TRACE("\nContinuationPoint (Identifier:%d) was provided.\n",cpid); 
					#endif /*_DEBUGGING_*/

					/* Clear continuation point in case we need to continue again. */
					Continuation_Point_Data.Cont_Point_Identifier=0;

					(*a_pResults+m)->StatusCode=browse(&Continuation_Point_Data.NodeToBrowse,(*a_pResults+m),Continuation_Point_Data.Current_Ref);
					continue;
				}
			}
			(*a_pResults+m)->StatusCode=OpcUa_BadContinuationPointInvalid;
			#ifndef NO_DEBUGGING_
				MY_TRACE("\n%d. Invalid ContinuationPoint!!!\n", m); 
			#endif /*_DEBUGGING_*/
		}
	}
	else
	{
		Continuation_Point_Data.Cont_Point_Identifier=0;
		OpcUa_BrowseDescription_Clear(&Continuation_Point_Data.NodeToBrowse);
		#ifndef NO_DEBUGGING_
			MY_TRACE("\nBrowseNext called, deleting Cont.Point");
			MY_TRACE("\n..................ContinuationPoint deleted.\n");
		#endif /*_DEBUGGING_*/
	}
	uStatus = response_header_fill(a_pResponseHeader,a_pRequestHeader,uStatus);
	if(OpcUa_IsBad(uStatus))
	{
       a_pResponseHeader->ServiceResult=OpcUa_BadInternalError;
	}
#ifndef NO_DEBUGGING_
	MY_TRACE("\nSERVICE===END============================================\n\n\n"); 
#endif /*_DEBUGGING_*/

	RESET_SESSION_COUNTER

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    
	uStatus = response_header_fill(a_pResponseHeader,a_pRequestHeader,uStatus);
	if(OpcUa_IsBad(uStatus))
	{
       a_pResponseHeader->ServiceResult=OpcUa_BadInternalError;
	}
#ifndef NO_DEBUGGING_
	MY_TRACE("\nSERVICE END (WITH ERROR)===========\n\n\n"); 
#endif /*_DEBUGGING_*/
	
	RESET_SESSION_COUNTER
    OpcUa_FinishErrorHandling;
}
