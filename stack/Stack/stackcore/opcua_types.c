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

/* base */
#include <opcua_platformdefs.h>
#include <opcua_statuscodes.h>
#include <opcua_errorhandling.h>
#include <opcua_trace.h>

/* core */
#include <opcua_memory.h>
#include <opcua_string.h>
#include <opcua_guid.h>

/* stack */
#include <opcua_encoder.h>
#include <opcua_decoder.h>

/* types */
#include <opcua_identifiers.h>

/* self */
#include <opcua_types.h>

#ifndef OPCUA_EXCLUDE_ApplicationType
/*============================================================================
 * OpcUa_ApplicationType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_ApplicationType_EnumeratedValues[] =
{
    { "Server", 0 },
    { "Client", 1 },
    { "ClientAndServer", 2 },
    { "DiscoveryServer", 3 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_ApplicationType_EnumeratedType =
{
    "ApplicationType",
    g_OpcUa_ApplicationType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_ApplicationDescription
/*============================================================================
 * OpcUa_ApplicationDescription_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ApplicationDescription_Initialize(OpcUa_ApplicationDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, ApplicationUri);
        OpcUa_Field_Initialize(String, ProductUri);
        OpcUa_Field_Initialize(LocalizedText, ApplicationName);
        OpcUa_Field_InitializeEnumerated(OpcUa_ApplicationType, ApplicationType);
        OpcUa_Field_Initialize(String, GatewayServerUri);
        OpcUa_Field_Initialize(String, DiscoveryProfileUri);
        OpcUa_Field_InitializeArray(String, DiscoveryUrls);
    }
}

/*============================================================================
 * OpcUa_ApplicationDescription_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ApplicationDescription_Clear(OpcUa_ApplicationDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, ApplicationUri);
        OpcUa_Field_Clear(String, ProductUri);
        OpcUa_Field_Clear(LocalizedText, ApplicationName);
        OpcUa_Field_ClearEnumerated(OpcUa_ApplicationType, ApplicationType);
        OpcUa_Field_Clear(String, GatewayServerUri);
        OpcUa_Field_Clear(String, DiscoveryProfileUri);
        OpcUa_Field_ClearArray(String, DiscoveryUrls);
    }
}

/*============================================================================
 * OpcUa_ApplicationDescription_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ApplicationDescription_GetSize(OpcUa_ApplicationDescription* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ApplicationDescription_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, ApplicationUri);
    OpcUa_Field_GetSize(String, ProductUri);
    OpcUa_Field_GetSize(LocalizedText, ApplicationName);
    OpcUa_Field_GetSizeEnumerated(OpcUa_ApplicationType, ApplicationType);
    OpcUa_Field_GetSize(String, GatewayServerUri);
    OpcUa_Field_GetSize(String, DiscoveryProfileUri);
    OpcUa_Field_GetSizeArray(String, DiscoveryUrls);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ApplicationDescription_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ApplicationDescription_Encode(OpcUa_ApplicationDescription* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ApplicationDescription_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, ApplicationUri);
    OpcUa_Field_Write(String, ProductUri);
    OpcUa_Field_Write(LocalizedText, ApplicationName);
    OpcUa_Field_WriteEnumerated(OpcUa_ApplicationType, ApplicationType);
    OpcUa_Field_Write(String, GatewayServerUri);
    OpcUa_Field_Write(String, DiscoveryProfileUri);
    OpcUa_Field_WriteArray(String, DiscoveryUrls);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ApplicationDescription_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ApplicationDescription_Decode(OpcUa_ApplicationDescription* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ApplicationDescription_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ApplicationDescription_Initialize(a_pValue);

    OpcUa_Field_Read(String, ApplicationUri);
    OpcUa_Field_Read(String, ProductUri);
    OpcUa_Field_Read(LocalizedText, ApplicationName);
    OpcUa_Field_ReadEnumerated(OpcUa_ApplicationType, ApplicationType);
    OpcUa_Field_Read(String, GatewayServerUri);
    OpcUa_Field_Read(String, DiscoveryProfileUri);
    OpcUa_Field_ReadArray(String, DiscoveryUrls);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ApplicationDescription_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ApplicationDescription_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ApplicationDescription_EncodeableType =
{
    "ApplicationDescription",
    OpcUaId_ApplicationDescription,
    OpcUaId_ApplicationDescription_Encoding_DefaultBinary,
    OpcUaId_ApplicationDescription_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ApplicationDescription),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ApplicationDescription_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ApplicationDescription_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ApplicationDescription_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ApplicationDescription_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ApplicationDescription_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RequestHeader
/*============================================================================
 * OpcUa_RequestHeader_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RequestHeader_Initialize(OpcUa_RequestHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(NodeId, AuthenticationToken);
        OpcUa_Field_Initialize(DateTime, Timestamp);
        OpcUa_Field_Initialize(UInt32, RequestHandle);
        OpcUa_Field_Initialize(UInt32, ReturnDiagnostics);
        OpcUa_Field_Initialize(String, AuditEntryId);
        OpcUa_Field_Initialize(UInt32, TimeoutHint);
        OpcUa_Field_Initialize(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_RequestHeader_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RequestHeader_Clear(OpcUa_RequestHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(NodeId, AuthenticationToken);
        OpcUa_Field_Clear(DateTime, Timestamp);
        OpcUa_Field_Clear(UInt32, RequestHandle);
        OpcUa_Field_Clear(UInt32, ReturnDiagnostics);
        OpcUa_Field_Clear(String, AuditEntryId);
        OpcUa_Field_Clear(UInt32, TimeoutHint);
        OpcUa_Field_Clear(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_RequestHeader_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RequestHeader_GetSize(OpcUa_RequestHeader* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RequestHeader_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(NodeId, AuthenticationToken);
    OpcUa_Field_GetSize(DateTime, Timestamp);
    OpcUa_Field_GetSize(UInt32, RequestHandle);
    OpcUa_Field_GetSize(UInt32, ReturnDiagnostics);
    OpcUa_Field_GetSize(String, AuditEntryId);
    OpcUa_Field_GetSize(UInt32, TimeoutHint);
    OpcUa_Field_GetSize(ExtensionObject, AdditionalHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RequestHeader_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RequestHeader_Encode(OpcUa_RequestHeader* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RequestHeader_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(NodeId, AuthenticationToken);
    OpcUa_Field_Write(DateTime, Timestamp);
    OpcUa_Field_Write(UInt32, RequestHandle);
    OpcUa_Field_Write(UInt32, ReturnDiagnostics);
    OpcUa_Field_Write(String, AuditEntryId);
    OpcUa_Field_Write(UInt32, TimeoutHint);
    OpcUa_Field_Write(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RequestHeader_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RequestHeader_Decode(OpcUa_RequestHeader* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RequestHeader_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RequestHeader_Initialize(a_pValue);

    OpcUa_Field_Read(NodeId, AuthenticationToken);
    OpcUa_Field_Read(DateTime, Timestamp);
    OpcUa_Field_Read(UInt32, RequestHandle);
    OpcUa_Field_Read(UInt32, ReturnDiagnostics);
    OpcUa_Field_Read(String, AuditEntryId);
    OpcUa_Field_Read(UInt32, TimeoutHint);
    OpcUa_Field_Read(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RequestHeader_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RequestHeader_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RequestHeader_EncodeableType =
{
    "RequestHeader",
    OpcUaId_RequestHeader,
    OpcUaId_RequestHeader_Encoding_DefaultBinary,
    OpcUaId_RequestHeader_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RequestHeader),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RequestHeader_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RequestHeader_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RequestHeader_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RequestHeader_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RequestHeader_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ResponseHeader
/*============================================================================
 * OpcUa_ResponseHeader_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ResponseHeader_Initialize(OpcUa_ResponseHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(DateTime, Timestamp);
        OpcUa_Field_Initialize(UInt32, RequestHandle);
        OpcUa_Field_Initialize(StatusCode, ServiceResult);
        OpcUa_Field_Initialize(DiagnosticInfo, ServiceDiagnostics);
        OpcUa_Field_InitializeArray(String, StringTable);
        OpcUa_Field_Initialize(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_ResponseHeader_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ResponseHeader_Clear(OpcUa_ResponseHeader* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(DateTime, Timestamp);
        OpcUa_Field_Clear(UInt32, RequestHandle);
        OpcUa_Field_Clear(StatusCode, ServiceResult);
        OpcUa_Field_Clear(DiagnosticInfo, ServiceDiagnostics);
        OpcUa_Field_ClearArray(String, StringTable);
        OpcUa_Field_Clear(ExtensionObject, AdditionalHeader);
    }
}

/*============================================================================
 * OpcUa_ResponseHeader_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ResponseHeader_GetSize(OpcUa_ResponseHeader* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ResponseHeader_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(DateTime, Timestamp);
    OpcUa_Field_GetSize(UInt32, RequestHandle);
    OpcUa_Field_GetSize(StatusCode, ServiceResult);
    OpcUa_Field_GetSize(DiagnosticInfo, ServiceDiagnostics);
    OpcUa_Field_GetSizeArray(String, StringTable);
    OpcUa_Field_GetSize(ExtensionObject, AdditionalHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ResponseHeader_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ResponseHeader_Encode(OpcUa_ResponseHeader* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ResponseHeader_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(DateTime, Timestamp);
    OpcUa_Field_Write(UInt32, RequestHandle);
    OpcUa_Field_Write(StatusCode, ServiceResult);
    OpcUa_Field_Write(DiagnosticInfo, ServiceDiagnostics);
    OpcUa_Field_WriteArray(String, StringTable);
    OpcUa_Field_Write(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ResponseHeader_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ResponseHeader_Decode(OpcUa_ResponseHeader* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ResponseHeader_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ResponseHeader_Initialize(a_pValue);

    OpcUa_Field_Read(DateTime, Timestamp);
    OpcUa_Field_Read(UInt32, RequestHandle);
    OpcUa_Field_Read(StatusCode, ServiceResult);
    OpcUa_Field_Read(DiagnosticInfo, ServiceDiagnostics);
    OpcUa_Field_ReadArray(String, StringTable);
    OpcUa_Field_Read(ExtensionObject, AdditionalHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ResponseHeader_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ResponseHeader_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ResponseHeader_EncodeableType =
{
    "ResponseHeader",
    OpcUaId_ResponseHeader,
    OpcUaId_ResponseHeader_Encoding_DefaultBinary,
    OpcUaId_ResponseHeader_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ResponseHeader),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ResponseHeader_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ResponseHeader_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ResponseHeader_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ResponseHeader_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ResponseHeader_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_ServiceFault
/*============================================================================
 * OpcUa_ServiceFault_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ServiceFault_Initialize(OpcUa_ServiceFault* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_ServiceFault_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ServiceFault_Clear(OpcUa_ServiceFault* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_ServiceFault_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServiceFault_GetSize(OpcUa_ServiceFault* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServiceFault_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServiceFault_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServiceFault_Encode(OpcUa_ServiceFault* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServiceFault_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServiceFault_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServiceFault_Decode(OpcUa_ServiceFault* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServiceFault_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ServiceFault_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ServiceFault_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServiceFault_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ServiceFault_EncodeableType =
{
    "ServiceFault",
    OpcUaId_ServiceFault,
    OpcUaId_ServiceFault_Encoding_DefaultBinary,
    OpcUaId_ServiceFault_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ServiceFault),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ServiceFault_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ServiceFault_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ServiceFault_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ServiceFault_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ServiceFault_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServers
#ifndef OPCUA_EXCLUDE_FindServersRequest
/*============================================================================
 * OpcUa_FindServersRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersRequest_Initialize(OpcUa_FindServersRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(String, EndpointUrl);
        OpcUa_Field_InitializeArray(String, LocaleIds);
        OpcUa_Field_InitializeArray(String, ServerUris);
    }
}

/*============================================================================
 * OpcUa_FindServersRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersRequest_Clear(OpcUa_FindServersRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(String, EndpointUrl);
        OpcUa_Field_ClearArray(String, LocaleIds);
        OpcUa_Field_ClearArray(String, ServerUris);
    }
}

/*============================================================================
 * OpcUa_FindServersRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersRequest_GetSize(OpcUa_FindServersRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(String, EndpointUrl);
    OpcUa_Field_GetSizeArray(String, LocaleIds);
    OpcUa_Field_GetSizeArray(String, ServerUris);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersRequest_Encode(OpcUa_FindServersRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(String, EndpointUrl);
    OpcUa_Field_WriteArray(String, LocaleIds);
    OpcUa_Field_WriteArray(String, ServerUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersRequest_Decode(OpcUa_FindServersRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(String, EndpointUrl);
    OpcUa_Field_ReadArray(String, LocaleIds);
    OpcUa_Field_ReadArray(String, ServerUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersRequest_EncodeableType =
{
    "FindServersRequest",
    OpcUaId_FindServersRequest,
    OpcUaId_FindServersRequest_Encoding_DefaultBinary,
    OpcUaId_FindServersRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServersResponse
/*============================================================================
 * OpcUa_FindServersResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersResponse_Initialize(OpcUa_FindServersResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ApplicationDescription, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersResponse_Clear(OpcUa_FindServersResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ApplicationDescription, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersResponse_GetSize(OpcUa_FindServersResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ApplicationDescription, Servers);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersResponse_Encode(OpcUa_FindServersResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ApplicationDescription, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersResponse_Decode(OpcUa_FindServersResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ApplicationDescription, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersResponse_EncodeableType =
{
    "FindServersResponse",
    OpcUaId_FindServersResponse,
    OpcUaId_FindServersResponse_Encoding_DefaultBinary,
    OpcUaId_FindServersResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_ServerOnNetwork
/*============================================================================
 * OpcUa_ServerOnNetwork_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ServerOnNetwork_Initialize(OpcUa_ServerOnNetwork* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, RecordId);
        OpcUa_Field_Initialize(String, ServerName);
        OpcUa_Field_Initialize(String, DiscoveryUrl);
        OpcUa_Field_InitializeArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_ServerOnNetwork_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ServerOnNetwork_Clear(OpcUa_ServerOnNetwork* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, RecordId);
        OpcUa_Field_Clear(String, ServerName);
        OpcUa_Field_Clear(String, DiscoveryUrl);
        OpcUa_Field_ClearArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_ServerOnNetwork_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerOnNetwork_GetSize(OpcUa_ServerOnNetwork* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServerOnNetwork_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, RecordId);
    OpcUa_Field_GetSize(String, ServerName);
    OpcUa_Field_GetSize(String, DiscoveryUrl);
    OpcUa_Field_GetSizeArray(String, ServerCapabilities);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServerOnNetwork_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerOnNetwork_Encode(OpcUa_ServerOnNetwork* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServerOnNetwork_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, RecordId);
    OpcUa_Field_Write(String, ServerName);
    OpcUa_Field_Write(String, DiscoveryUrl);
    OpcUa_Field_WriteArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServerOnNetwork_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ServerOnNetwork_Decode(OpcUa_ServerOnNetwork* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ServerOnNetwork_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ServerOnNetwork_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, RecordId);
    OpcUa_Field_Read(String, ServerName);
    OpcUa_Field_Read(String, DiscoveryUrl);
    OpcUa_Field_ReadArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ServerOnNetwork_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ServerOnNetwork_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ServerOnNetwork_EncodeableType =
{
    "ServerOnNetwork",
    OpcUaId_ServerOnNetwork,
    OpcUaId_ServerOnNetwork_Encoding_DefaultBinary,
    OpcUaId_ServerOnNetwork_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ServerOnNetwork),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ServerOnNetwork_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ServerOnNetwork_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ServerOnNetwork_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ServerOnNetwork_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ServerOnNetwork_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetwork
#ifndef OPCUA_EXCLUDE_FindServersOnNetworkRequest
/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkRequest_Initialize(OpcUa_FindServersOnNetworkRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(UInt32, StartingRecordId);
        OpcUa_Field_Initialize(UInt32, MaxRecordsToReturn);
        OpcUa_Field_InitializeArray(String, ServerCapabilityFilter);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkRequest_Clear(OpcUa_FindServersOnNetworkRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(UInt32, StartingRecordId);
        OpcUa_Field_Clear(UInt32, MaxRecordsToReturn);
        OpcUa_Field_ClearArray(String, ServerCapabilityFilter);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_GetSize(OpcUa_FindServersOnNetworkRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(UInt32, StartingRecordId);
    OpcUa_Field_GetSize(UInt32, MaxRecordsToReturn);
    OpcUa_Field_GetSizeArray(String, ServerCapabilityFilter);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_Encode(OpcUa_FindServersOnNetworkRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(UInt32, StartingRecordId);
    OpcUa_Field_Write(UInt32, MaxRecordsToReturn);
    OpcUa_Field_WriteArray(String, ServerCapabilityFilter);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkRequest_Decode(OpcUa_FindServersOnNetworkRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersOnNetworkRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(UInt32, StartingRecordId);
    OpcUa_Field_Read(UInt32, MaxRecordsToReturn);
    OpcUa_Field_ReadArray(String, ServerCapabilityFilter);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersOnNetworkRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersOnNetworkRequest_EncodeableType =
{
    "FindServersOnNetworkRequest",
    OpcUaId_FindServersOnNetworkRequest,
    OpcUaId_FindServersOnNetworkRequest_Encoding_DefaultBinary,
    OpcUaId_FindServersOnNetworkRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersOnNetworkRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersOnNetworkRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersOnNetworkRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersOnNetworkRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersOnNetworkRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersOnNetworkRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_FindServersOnNetworkResponse
/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkResponse_Initialize(OpcUa_FindServersOnNetworkResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Initialize(DateTime, LastCounterResetTime);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_ServerOnNetwork, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_FindServersOnNetworkResponse_Clear(OpcUa_FindServersOnNetworkResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Clear(DateTime, LastCounterResetTime);
        OpcUa_Field_ClearEncodeableArray(OpcUa_ServerOnNetwork, Servers);
    }
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_GetSize(OpcUa_FindServersOnNetworkResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSize(DateTime, LastCounterResetTime);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_ServerOnNetwork, Servers);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_Encode(OpcUa_FindServersOnNetworkResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Write(DateTime, LastCounterResetTime);
    OpcUa_Field_WriteEncodeableArray(OpcUa_ServerOnNetwork, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_FindServersOnNetworkResponse_Decode(OpcUa_FindServersOnNetworkResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "FindServersOnNetworkResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_FindServersOnNetworkResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Read(DateTime, LastCounterResetTime);
    OpcUa_Field_ReadEncodeableArray(OpcUa_ServerOnNetwork, Servers);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_FindServersOnNetworkResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_FindServersOnNetworkResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_FindServersOnNetworkResponse_EncodeableType =
{
    "FindServersOnNetworkResponse",
    OpcUaId_FindServersOnNetworkResponse,
    OpcUaId_FindServersOnNetworkResponse_Encoding_DefaultBinary,
    OpcUaId_FindServersOnNetworkResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_FindServersOnNetworkResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_FindServersOnNetworkResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_FindServersOnNetworkResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_FindServersOnNetworkResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_FindServersOnNetworkResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_FindServersOnNetworkResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_MessageSecurityMode
/*============================================================================
 * OpcUa_MessageSecurityMode_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_MessageSecurityMode_EnumeratedValues[] =
{
    { "Invalid", 0 },
    { "None", 1 },
    { "Sign", 2 },
    { "SignAndEncrypt", 3 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_MessageSecurityMode_EnumeratedType =
{
    "MessageSecurityMode",
    g_OpcUa_MessageSecurityMode_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_UserTokenType
/*============================================================================
 * OpcUa_UserTokenType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_UserTokenType_EnumeratedValues[] =
{
    { "Anonymous", 0 },
    { "UserName", 1 },
    { "Certificate", 2 },
    { "IssuedToken", 3 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_UserTokenType_EnumeratedType =
{
    "UserTokenType",
    g_OpcUa_UserTokenType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_UserTokenPolicy
/*============================================================================
 * OpcUa_UserTokenPolicy_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_UserTokenPolicy_Initialize(OpcUa_UserTokenPolicy* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
        OpcUa_Field_InitializeEnumerated(OpcUa_UserTokenType, TokenType);
        OpcUa_Field_Initialize(String, IssuedTokenType);
        OpcUa_Field_Initialize(String, IssuerEndpointUrl);
        OpcUa_Field_Initialize(String, SecurityPolicyUri);
    }
}

/*============================================================================
 * OpcUa_UserTokenPolicy_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_UserTokenPolicy_Clear(OpcUa_UserTokenPolicy* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
        OpcUa_Field_ClearEnumerated(OpcUa_UserTokenType, TokenType);
        OpcUa_Field_Clear(String, IssuedTokenType);
        OpcUa_Field_Clear(String, IssuerEndpointUrl);
        OpcUa_Field_Clear(String, SecurityPolicyUri);
    }
}

/*============================================================================
 * OpcUa_UserTokenPolicy_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserTokenPolicy_GetSize(OpcUa_UserTokenPolicy* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserTokenPolicy_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);
    OpcUa_Field_GetSizeEnumerated(OpcUa_UserTokenType, TokenType);
    OpcUa_Field_GetSize(String, IssuedTokenType);
    OpcUa_Field_GetSize(String, IssuerEndpointUrl);
    OpcUa_Field_GetSize(String, SecurityPolicyUri);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserTokenPolicy_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserTokenPolicy_Encode(OpcUa_UserTokenPolicy* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserTokenPolicy_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);
    OpcUa_Field_WriteEnumerated(OpcUa_UserTokenType, TokenType);
    OpcUa_Field_Write(String, IssuedTokenType);
    OpcUa_Field_Write(String, IssuerEndpointUrl);
    OpcUa_Field_Write(String, SecurityPolicyUri);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserTokenPolicy_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserTokenPolicy_Decode(OpcUa_UserTokenPolicy* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserTokenPolicy_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_UserTokenPolicy_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);
    OpcUa_Field_ReadEnumerated(OpcUa_UserTokenType, TokenType);
    OpcUa_Field_Read(String, IssuedTokenType);
    OpcUa_Field_Read(String, IssuerEndpointUrl);
    OpcUa_Field_Read(String, SecurityPolicyUri);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UserTokenPolicy_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserTokenPolicy_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_UserTokenPolicy_EncodeableType =
{
    "UserTokenPolicy",
    OpcUaId_UserTokenPolicy,
    OpcUaId_UserTokenPolicy_Encoding_DefaultBinary,
    OpcUaId_UserTokenPolicy_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_UserTokenPolicy),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_UserTokenPolicy_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_UserTokenPolicy_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_UserTokenPolicy_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_UserTokenPolicy_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_UserTokenPolicy_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_EndpointDescription
/*============================================================================
 * OpcUa_EndpointDescription_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_EndpointDescription_Initialize(OpcUa_EndpointDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, EndpointUrl);
        OpcUa_Field_InitializeEncodeable(OpcUa_ApplicationDescription, Server);
        OpcUa_Field_Initialize(ByteString, ServerCertificate);
        OpcUa_Field_InitializeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Initialize(String, SecurityPolicyUri);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
        OpcUa_Field_Initialize(String, TransportProfileUri);
        OpcUa_Field_Initialize(Byte, SecurityLevel);
    }
}

/*============================================================================
 * OpcUa_EndpointDescription_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_EndpointDescription_Clear(OpcUa_EndpointDescription* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, EndpointUrl);
        OpcUa_Field_ClearEncodeable(OpcUa_ApplicationDescription, Server);
        OpcUa_Field_Clear(ByteString, ServerCertificate);
        OpcUa_Field_ClearEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Clear(String, SecurityPolicyUri);
        OpcUa_Field_ClearEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
        OpcUa_Field_Clear(String, TransportProfileUri);
        OpcUa_Field_Clear(Byte, SecurityLevel);
    }
}

/*============================================================================
 * OpcUa_EndpointDescription_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointDescription_GetSize(OpcUa_EndpointDescription* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointDescription_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, EndpointUrl);
    OpcUa_Field_GetSizeEncodeable(OpcUa_ApplicationDescription, Server);
    OpcUa_Field_GetSize(ByteString, ServerCertificate);
    OpcUa_Field_GetSizeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_GetSize(String, SecurityPolicyUri);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
    OpcUa_Field_GetSize(String, TransportProfileUri);
    OpcUa_Field_GetSize(Byte, SecurityLevel);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointDescription_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointDescription_Encode(OpcUa_EndpointDescription* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointDescription_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, EndpointUrl);
    OpcUa_Field_WriteEncodeable(OpcUa_ApplicationDescription, Server);
    OpcUa_Field_Write(ByteString, ServerCertificate);
    OpcUa_Field_WriteEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Write(String, SecurityPolicyUri);
    OpcUa_Field_WriteEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
    OpcUa_Field_Write(String, TransportProfileUri);
    OpcUa_Field_Write(Byte, SecurityLevel);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointDescription_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_EndpointDescription_Decode(OpcUa_EndpointDescription* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "EndpointDescription_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_EndpointDescription_Initialize(a_pValue);

    OpcUa_Field_Read(String, EndpointUrl);
    OpcUa_Field_ReadEncodeable(OpcUa_ApplicationDescription, Server);
    OpcUa_Field_Read(ByteString, ServerCertificate);
    OpcUa_Field_ReadEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Read(String, SecurityPolicyUri);
    OpcUa_Field_ReadEncodeableArray(OpcUa_UserTokenPolicy, UserIdentityTokens);
    OpcUa_Field_Read(String, TransportProfileUri);
    OpcUa_Field_Read(Byte, SecurityLevel);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_EndpointDescription_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_EndpointDescription_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_EndpointDescription_EncodeableType =
{
    "EndpointDescription",
    OpcUaId_EndpointDescription,
    OpcUaId_EndpointDescription_Encoding_DefaultBinary,
    OpcUaId_EndpointDescription_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_EndpointDescription),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_EndpointDescription_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_EndpointDescription_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_EndpointDescription_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_EndpointDescription_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_EndpointDescription_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_GetEndpoints
#ifndef OPCUA_EXCLUDE_GetEndpointsRequest
/*============================================================================
 * OpcUa_GetEndpointsRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsRequest_Initialize(OpcUa_GetEndpointsRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(String, EndpointUrl);
        OpcUa_Field_InitializeArray(String, LocaleIds);
        OpcUa_Field_InitializeArray(String, ProfileUris);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsRequest_Clear(OpcUa_GetEndpointsRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(String, EndpointUrl);
        OpcUa_Field_ClearArray(String, LocaleIds);
        OpcUa_Field_ClearArray(String, ProfileUris);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsRequest_GetSize(OpcUa_GetEndpointsRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(String, EndpointUrl);
    OpcUa_Field_GetSizeArray(String, LocaleIds);
    OpcUa_Field_GetSizeArray(String, ProfileUris);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsRequest_Encode(OpcUa_GetEndpointsRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(String, EndpointUrl);
    OpcUa_Field_WriteArray(String, LocaleIds);
    OpcUa_Field_WriteArray(String, ProfileUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsRequest_Decode(OpcUa_GetEndpointsRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_GetEndpointsRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(String, EndpointUrl);
    OpcUa_Field_ReadArray(String, LocaleIds);
    OpcUa_Field_ReadArray(String, ProfileUris);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_GetEndpointsRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_GetEndpointsRequest_EncodeableType =
{
    "GetEndpointsRequest",
    OpcUaId_GetEndpointsRequest,
    OpcUaId_GetEndpointsRequest_Encoding_DefaultBinary,
    OpcUaId_GetEndpointsRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_GetEndpointsRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_GetEndpointsRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_GetEndpointsRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_GetEndpointsRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_GetEndpointsRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_GetEndpointsRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_GetEndpointsResponse
/*============================================================================
 * OpcUa_GetEndpointsResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsResponse_Initialize(OpcUa_GetEndpointsResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_InitializeEncodeableArray(OpcUa_EndpointDescription, Endpoints);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_GetEndpointsResponse_Clear(OpcUa_GetEndpointsResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_ClearEncodeableArray(OpcUa_EndpointDescription, Endpoints);
    }
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsResponse_GetSize(OpcUa_GetEndpointsResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSizeEncodeableArray(OpcUa_EndpointDescription, Endpoints);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsResponse_Encode(OpcUa_GetEndpointsResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_WriteEncodeableArray(OpcUa_EndpointDescription, Endpoints);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_GetEndpointsResponse_Decode(OpcUa_GetEndpointsResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "GetEndpointsResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_GetEndpointsResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_ReadEncodeableArray(OpcUa_EndpointDescription, Endpoints);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_GetEndpointsResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_GetEndpointsResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_GetEndpointsResponse_EncodeableType =
{
    "GetEndpointsResponse",
    OpcUaId_GetEndpointsResponse,
    OpcUaId_GetEndpointsResponse_Encoding_DefaultBinary,
    OpcUaId_GetEndpointsResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_GetEndpointsResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_GetEndpointsResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_GetEndpointsResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_GetEndpointsResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_GetEndpointsResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_GetEndpointsResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_RegisteredServer
/*============================================================================
 * OpcUa_RegisteredServer_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisteredServer_Initialize(OpcUa_RegisteredServer* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, ServerUri);
        OpcUa_Field_Initialize(String, ProductUri);
        OpcUa_Field_InitializeArray(LocalizedText, ServerNames);
        OpcUa_Field_InitializeEnumerated(OpcUa_ApplicationType, ServerType);
        OpcUa_Field_Initialize(String, GatewayServerUri);
        OpcUa_Field_InitializeArray(String, DiscoveryUrls);
        OpcUa_Field_Initialize(String, SemaphoreFilePath);
        OpcUa_Field_Initialize(Boolean, IsOnline);
    }
}

/*============================================================================
 * OpcUa_RegisteredServer_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisteredServer_Clear(OpcUa_RegisteredServer* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, ServerUri);
        OpcUa_Field_Clear(String, ProductUri);
        OpcUa_Field_ClearArray(LocalizedText, ServerNames);
        OpcUa_Field_ClearEnumerated(OpcUa_ApplicationType, ServerType);
        OpcUa_Field_Clear(String, GatewayServerUri);
        OpcUa_Field_ClearArray(String, DiscoveryUrls);
        OpcUa_Field_Clear(String, SemaphoreFilePath);
        OpcUa_Field_Clear(Boolean, IsOnline);
    }
}

/*============================================================================
 * OpcUa_RegisteredServer_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisteredServer_GetSize(OpcUa_RegisteredServer* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisteredServer_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, ServerUri);
    OpcUa_Field_GetSize(String, ProductUri);
    OpcUa_Field_GetSizeArray(LocalizedText, ServerNames);
    OpcUa_Field_GetSizeEnumerated(OpcUa_ApplicationType, ServerType);
    OpcUa_Field_GetSize(String, GatewayServerUri);
    OpcUa_Field_GetSizeArray(String, DiscoveryUrls);
    OpcUa_Field_GetSize(String, SemaphoreFilePath);
    OpcUa_Field_GetSize(Boolean, IsOnline);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisteredServer_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisteredServer_Encode(OpcUa_RegisteredServer* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisteredServer_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, ServerUri);
    OpcUa_Field_Write(String, ProductUri);
    OpcUa_Field_WriteArray(LocalizedText, ServerNames);
    OpcUa_Field_WriteEnumerated(OpcUa_ApplicationType, ServerType);
    OpcUa_Field_Write(String, GatewayServerUri);
    OpcUa_Field_WriteArray(String, DiscoveryUrls);
    OpcUa_Field_Write(String, SemaphoreFilePath);
    OpcUa_Field_Write(Boolean, IsOnline);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisteredServer_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisteredServer_Decode(OpcUa_RegisteredServer* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisteredServer_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisteredServer_Initialize(a_pValue);

    OpcUa_Field_Read(String, ServerUri);
    OpcUa_Field_Read(String, ProductUri);
    OpcUa_Field_ReadArray(LocalizedText, ServerNames);
    OpcUa_Field_ReadEnumerated(OpcUa_ApplicationType, ServerType);
    OpcUa_Field_Read(String, GatewayServerUri);
    OpcUa_Field_ReadArray(String, DiscoveryUrls);
    OpcUa_Field_Read(String, SemaphoreFilePath);
    OpcUa_Field_Read(Boolean, IsOnline);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisteredServer_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisteredServer_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisteredServer_EncodeableType =
{
    "RegisteredServer",
    OpcUaId_RegisteredServer,
    OpcUaId_RegisteredServer_Encoding_DefaultBinary,
    OpcUaId_RegisteredServer_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisteredServer),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisteredServer_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisteredServer_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisteredServer_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisteredServer_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisteredServer_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer
#ifndef OPCUA_EXCLUDE_RegisterServerRequest
/*============================================================================
 * OpcUa_RegisterServerRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerRequest_Initialize(OpcUa_RegisterServerRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_InitializeEncodeable(OpcUa_RegisteredServer, Server);
    }
}

/*============================================================================
 * OpcUa_RegisterServerRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerRequest_Clear(OpcUa_RegisterServerRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_ClearEncodeable(OpcUa_RegisteredServer, Server);
    }
}

/*============================================================================
 * OpcUa_RegisterServerRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerRequest_GetSize(OpcUa_RegisterServerRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSizeEncodeable(OpcUa_RegisteredServer, Server);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerRequest_Encode(OpcUa_RegisterServerRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_WriteEncodeable(OpcUa_RegisteredServer, Server);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerRequest_Decode(OpcUa_RegisterServerRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServerRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_ReadEncodeable(OpcUa_RegisteredServer, Server);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServerRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServerRequest_EncodeableType =
{
    "RegisterServerRequest",
    OpcUaId_RegisterServerRequest,
    OpcUaId_RegisterServerRequest_Encoding_DefaultBinary,
    OpcUaId_RegisterServerRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServerRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServerRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServerRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServerRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServerRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServerRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServerResponse
/*============================================================================
 * OpcUa_RegisterServerResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerResponse_Initialize(OpcUa_RegisterServerResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_RegisterServerResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServerResponse_Clear(OpcUa_RegisterServerResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_RegisterServerResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerResponse_GetSize(OpcUa_RegisterServerResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerResponse_Encode(OpcUa_RegisterServerResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServerResponse_Decode(OpcUa_RegisterServerResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServerResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServerResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServerResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServerResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServerResponse_EncodeableType =
{
    "RegisterServerResponse",
    OpcUaId_RegisterServerResponse,
    OpcUaId_RegisterServerResponse_Encoding_DefaultBinary,
    OpcUaId_RegisterServerResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServerResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServerResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServerResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServerResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServerResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServerResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_MdnsDiscoveryConfiguration
/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_MdnsDiscoveryConfiguration_Initialize(OpcUa_MdnsDiscoveryConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, MdnsServerName);
        OpcUa_Field_InitializeArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_MdnsDiscoveryConfiguration_Clear(OpcUa_MdnsDiscoveryConfiguration* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, MdnsServerName);
        OpcUa_Field_ClearArray(String, ServerCapabilities);
    }
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_GetSize(OpcUa_MdnsDiscoveryConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MdnsDiscoveryConfiguration_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, MdnsServerName);
    OpcUa_Field_GetSizeArray(String, ServerCapabilities);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_Encode(OpcUa_MdnsDiscoveryConfiguration* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MdnsDiscoveryConfiguration_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, MdnsServerName);
    OpcUa_Field_WriteArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_MdnsDiscoveryConfiguration_Decode(OpcUa_MdnsDiscoveryConfiguration* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "MdnsDiscoveryConfiguration_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_MdnsDiscoveryConfiguration_Initialize(a_pValue);

    OpcUa_Field_Read(String, MdnsServerName);
    OpcUa_Field_ReadArray(String, ServerCapabilities);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_MdnsDiscoveryConfiguration_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_MdnsDiscoveryConfiguration_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_MdnsDiscoveryConfiguration_EncodeableType =
{
    "MdnsDiscoveryConfiguration",
    OpcUaId_MdnsDiscoveryConfiguration,
    OpcUaId_MdnsDiscoveryConfiguration_Encoding_DefaultBinary,
    OpcUaId_MdnsDiscoveryConfiguration_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_MdnsDiscoveryConfiguration),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_MdnsDiscoveryConfiguration_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_MdnsDiscoveryConfiguration_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_MdnsDiscoveryConfiguration_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_MdnsDiscoveryConfiguration_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_MdnsDiscoveryConfiguration_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2
#ifndef OPCUA_EXCLUDE_RegisterServer2Request
/*============================================================================
 * OpcUa_RegisterServer2Request_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Request_Initialize(OpcUa_RegisterServer2Request* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_InitializeEncodeable(OpcUa_RegisteredServer, Server);
        OpcUa_Field_InitializeArray(ExtensionObject, DiscoveryConfiguration);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Request_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Request_Clear(OpcUa_RegisterServer2Request* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_ClearEncodeable(OpcUa_RegisteredServer, Server);
        OpcUa_Field_ClearArray(ExtensionObject, DiscoveryConfiguration);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Request_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Request_GetSize(OpcUa_RegisterServer2Request* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Request_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSizeEncodeable(OpcUa_RegisteredServer, Server);
    OpcUa_Field_GetSizeArray(ExtensionObject, DiscoveryConfiguration);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Request_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Request_Encode(OpcUa_RegisterServer2Request* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Request_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_WriteEncodeable(OpcUa_RegisteredServer, Server);
    OpcUa_Field_WriteArray(ExtensionObject, DiscoveryConfiguration);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Request_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Request_Decode(OpcUa_RegisterServer2Request* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Request_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServer2Request_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_ReadEncodeable(OpcUa_RegisteredServer, Server);
    OpcUa_Field_ReadArray(ExtensionObject, DiscoveryConfiguration);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServer2Request_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Request_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServer2Request_EncodeableType =
{
    "RegisterServer2Request",
    OpcUaId_RegisterServer2Request,
    OpcUaId_RegisterServer2Request_Encoding_DefaultBinary,
    OpcUaId_RegisterServer2Request_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServer2Request),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServer2Request_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServer2Request_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServer2Request_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServer2Request_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServer2Request_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_RegisterServer2Response
/*============================================================================
 * OpcUa_RegisterServer2Response_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Response_Initialize(OpcUa_RegisterServer2Response* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_InitializeArray(StatusCode, ConfigurationResults);
        OpcUa_Field_InitializeArray(DiagnosticInfo, DiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Response_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_RegisterServer2Response_Clear(OpcUa_RegisterServer2Response* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_ClearArray(StatusCode, ConfigurationResults);
        OpcUa_Field_ClearArray(DiagnosticInfo, DiagnosticInfos);
    }
}

/*============================================================================
 * OpcUa_RegisterServer2Response_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Response_GetSize(OpcUa_RegisterServer2Response* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Response_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSizeArray(StatusCode, ConfigurationResults);
    OpcUa_Field_GetSizeArray(DiagnosticInfo, DiagnosticInfos);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Response_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Response_Encode(OpcUa_RegisterServer2Response* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Response_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_WriteArray(StatusCode, ConfigurationResults);
    OpcUa_Field_WriteArray(DiagnosticInfo, DiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Response_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_RegisterServer2Response_Decode(OpcUa_RegisterServer2Response* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "RegisterServer2Response_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_RegisterServer2Response_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_ReadArray(StatusCode, ConfigurationResults);
    OpcUa_Field_ReadArray(DiagnosticInfo, DiagnosticInfos);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_RegisterServer2Response_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_RegisterServer2Response_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_RegisterServer2Response_EncodeableType =
{
    "RegisterServer2Response",
    OpcUaId_RegisterServer2Response,
    OpcUaId_RegisterServer2Response_Encoding_DefaultBinary,
    OpcUaId_RegisterServer2Response_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_RegisterServer2Response),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_RegisterServer2Response_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_RegisterServer2Response_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_RegisterServer2Response_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_RegisterServer2Response_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_RegisterServer2Response_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_SecurityTokenRequestType
/*============================================================================
 * OpcUa_SecurityTokenRequestType_EnumeratedType
 *===========================================================================*/
static struct _OpcUa_EnumeratedValue g_OpcUa_SecurityTokenRequestType_EnumeratedValues[] =
{
    { "Issue", 0 },
    { "Renew", 1 },
    { OpcUa_Null, 0 }
};

struct _OpcUa_EnumeratedType OpcUa_SecurityTokenRequestType_EnumeratedType =
{
    "SecurityTokenRequestType",
    g_OpcUa_SecurityTokenRequestType_EnumeratedValues
};
#endif

#ifndef OPCUA_EXCLUDE_ChannelSecurityToken
/*============================================================================
 * OpcUa_ChannelSecurityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_ChannelSecurityToken_Initialize(OpcUa_ChannelSecurityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(UInt32, ChannelId);
        OpcUa_Field_Initialize(UInt32, TokenId);
        OpcUa_Field_Initialize(DateTime, CreatedAt);
        OpcUa_Field_Initialize(UInt32, RevisedLifetime);
    }
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_ChannelSecurityToken_Clear(OpcUa_ChannelSecurityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(UInt32, ChannelId);
        OpcUa_Field_Clear(UInt32, TokenId);
        OpcUa_Field_Clear(DateTime, CreatedAt);
        OpcUa_Field_Clear(UInt32, RevisedLifetime);
    }
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ChannelSecurityToken_GetSize(OpcUa_ChannelSecurityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ChannelSecurityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(UInt32, ChannelId);
    OpcUa_Field_GetSize(UInt32, TokenId);
    OpcUa_Field_GetSize(DateTime, CreatedAt);
    OpcUa_Field_GetSize(UInt32, RevisedLifetime);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ChannelSecurityToken_Encode(OpcUa_ChannelSecurityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ChannelSecurityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(UInt32, ChannelId);
    OpcUa_Field_Write(UInt32, TokenId);
    OpcUa_Field_Write(DateTime, CreatedAt);
    OpcUa_Field_Write(UInt32, RevisedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_ChannelSecurityToken_Decode(OpcUa_ChannelSecurityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "ChannelSecurityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_ChannelSecurityToken_Initialize(a_pValue);

    OpcUa_Field_Read(UInt32, ChannelId);
    OpcUa_Field_Read(UInt32, TokenId);
    OpcUa_Field_Read(DateTime, CreatedAt);
    OpcUa_Field_Read(UInt32, RevisedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_ChannelSecurityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_ChannelSecurityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_ChannelSecurityToken_EncodeableType =
{
    "ChannelSecurityToken",
    OpcUaId_ChannelSecurityToken,
    OpcUaId_ChannelSecurityToken_Encoding_DefaultBinary,
    OpcUaId_ChannelSecurityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_ChannelSecurityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_ChannelSecurityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_ChannelSecurityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_ChannelSecurityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_ChannelSecurityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_ChannelSecurityToken_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_OpenSecureChannel
#ifndef OPCUA_EXCLUDE_OpenSecureChannelRequest
/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelRequest_Initialize(OpcUa_OpenSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Initialize(UInt32, ClientProtocolVersion);
        OpcUa_Field_InitializeEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
        OpcUa_Field_InitializeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Initialize(ByteString, ClientNonce);
        OpcUa_Field_Initialize(UInt32, RequestedLifetime);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelRequest_Clear(OpcUa_OpenSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
        OpcUa_Field_Clear(UInt32, ClientProtocolVersion);
        OpcUa_Field_ClearEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
        OpcUa_Field_ClearEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
        OpcUa_Field_Clear(ByteString, ClientNonce);
        OpcUa_Field_Clear(UInt32, RequestedLifetime);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_GetSize(OpcUa_OpenSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_GetSize(UInt32, ClientProtocolVersion);
    OpcUa_Field_GetSizeEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
    OpcUa_Field_GetSizeEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_GetSize(ByteString, ClientNonce);
    OpcUa_Field_GetSize(UInt32, RequestedLifetime);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_Encode(OpcUa_OpenSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Write(UInt32, ClientProtocolVersion);
    OpcUa_Field_WriteEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
    OpcUa_Field_WriteEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Write(ByteString, ClientNonce);
    OpcUa_Field_Write(UInt32, RequestedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelRequest_Decode(OpcUa_OpenSecureChannelRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_OpenSecureChannelRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);
    OpcUa_Field_Read(UInt32, ClientProtocolVersion);
    OpcUa_Field_ReadEnumerated(OpcUa_SecurityTokenRequestType, RequestType);
    OpcUa_Field_ReadEnumerated(OpcUa_MessageSecurityMode, SecurityMode);
    OpcUa_Field_Read(ByteString, ClientNonce);
    OpcUa_Field_Read(UInt32, RequestedLifetime);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_OpenSecureChannelRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_OpenSecureChannelRequest_EncodeableType =
{
    "OpenSecureChannelRequest",
    OpcUaId_OpenSecureChannelRequest,
    OpcUaId_OpenSecureChannelRequest_Encoding_DefaultBinary,
    OpcUaId_OpenSecureChannelRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_OpenSecureChannelRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_OpenSecureChannelRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_OpenSecureChannelRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_OpenSecureChannelRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_OpenSecureChannelRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_OpenSecureChannelRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_OpenSecureChannelResponse
/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelResponse_Initialize(OpcUa_OpenSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Initialize(UInt32, ServerProtocolVersion);
        OpcUa_Field_InitializeEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
        OpcUa_Field_Initialize(ByteString, ServerNonce);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_OpenSecureChannelResponse_Clear(OpcUa_OpenSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
        OpcUa_Field_Clear(UInt32, ServerProtocolVersion);
        OpcUa_Field_ClearEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
        OpcUa_Field_Clear(ByteString, ServerNonce);
    }
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_GetSize(OpcUa_OpenSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_GetSize(UInt32, ServerProtocolVersion);
    OpcUa_Field_GetSizeEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
    OpcUa_Field_GetSize(ByteString, ServerNonce);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_Encode(OpcUa_OpenSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Write(UInt32, ServerProtocolVersion);
    OpcUa_Field_WriteEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
    OpcUa_Field_Write(ByteString, ServerNonce);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_OpenSecureChannelResponse_Decode(OpcUa_OpenSecureChannelResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "OpenSecureChannelResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_OpenSecureChannelResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    OpcUa_Field_Read(UInt32, ServerProtocolVersion);
    OpcUa_Field_ReadEncodeable(OpcUa_ChannelSecurityToken, SecurityToken);
    OpcUa_Field_Read(ByteString, ServerNonce);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_OpenSecureChannelResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_OpenSecureChannelResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_OpenSecureChannelResponse_EncodeableType =
{
    "OpenSecureChannelResponse",
    OpcUaId_OpenSecureChannelResponse,
    OpcUaId_OpenSecureChannelResponse_Encoding_DefaultBinary,
    OpcUaId_OpenSecureChannelResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_OpenSecureChannelResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_OpenSecureChannelResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_OpenSecureChannelResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_OpenSecureChannelResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_OpenSecureChannelResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_OpenSecureChannelResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_CloseSecureChannel
#ifndef OPCUA_EXCLUDE_CloseSecureChannelRequest
/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelRequest_Initialize(OpcUa_CloseSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_RequestHeader, RequestHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelRequest_Clear(OpcUa_CloseSecureChannelRequest* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_RequestHeader, RequestHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_GetSize(OpcUa_CloseSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelRequest_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_RequestHeader, RequestHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_Encode(OpcUa_CloseSecureChannelRequest* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelRequest_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_RequestHeader, RequestHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelRequest_Decode(OpcUa_CloseSecureChannelRequest* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelRequest_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_CloseSecureChannelRequest_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_RequestHeader, RequestHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_CloseSecureChannelRequest_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelRequest_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_CloseSecureChannelRequest_EncodeableType =
{
    "CloseSecureChannelRequest",
    OpcUaId_CloseSecureChannelRequest,
    OpcUaId_CloseSecureChannelRequest_Encoding_DefaultBinary,
    OpcUaId_CloseSecureChannelRequest_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_CloseSecureChannelRequest),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_CloseSecureChannelRequest_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_CloseSecureChannelRequest_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_CloseSecureChannelRequest_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_CloseSecureChannelRequest_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_CloseSecureChannelRequest_Decode
};
#endif

#ifndef OPCUA_EXCLUDE_CloseSecureChannelResponse
/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelResponse_Initialize(OpcUa_CloseSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_InitializeEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_CloseSecureChannelResponse_Clear(OpcUa_CloseSecureChannelResponse* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_ClearEncodeable(OpcUa_ResponseHeader, ResponseHeader);
    }
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_GetSize(OpcUa_CloseSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelResponse_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSizeEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_Encode(OpcUa_CloseSecureChannelResponse* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelResponse_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_WriteEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_CloseSecureChannelResponse_Decode(OpcUa_CloseSecureChannelResponse* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "CloseSecureChannelResponse_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_CloseSecureChannelResponse_Initialize(a_pValue);

    OpcUa_Field_ReadEncodeable(OpcUa_ResponseHeader, ResponseHeader);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_CloseSecureChannelResponse_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_CloseSecureChannelResponse_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_CloseSecureChannelResponse_EncodeableType =
{
    "CloseSecureChannelResponse",
    OpcUaId_CloseSecureChannelResponse,
    OpcUaId_CloseSecureChannelResponse_Encoding_DefaultBinary,
    OpcUaId_CloseSecureChannelResponse_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_CloseSecureChannelResponse),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_CloseSecureChannelResponse_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_CloseSecureChannelResponse_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_CloseSecureChannelResponse_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_CloseSecureChannelResponse_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_CloseSecureChannelResponse_Decode
};
#endif
#endif

#ifndef OPCUA_EXCLUDE_UserIdentityToken
/*============================================================================
 * OpcUa_UserIdentityToken_Initialize
 *===========================================================================*/
OpcUa_Void OpcUa_UserIdentityToken_Initialize(OpcUa_UserIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Initialize(String, PolicyId);
    }
}

/*============================================================================
 * OpcUa_UserIdentityToken_Clear
 *===========================================================================*/
OpcUa_Void OpcUa_UserIdentityToken_Clear(OpcUa_UserIdentityToken* a_pValue)
{
    if (a_pValue != OpcUa_Null)
    {
        OpcUa_Field_Clear(String, PolicyId);
    }
}

/*============================================================================
 * OpcUa_UserIdentityToken_GetSize
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserIdentityToken_GetSize(OpcUa_UserIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder, OpcUa_Int32* a_pSize)
{
    OpcUa_Int32 iSize = 0;

    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserIdentityToken_GetSize");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);
    OpcUa_ReturnErrorIfArgumentNull(a_pSize);

    *a_pSize = -1;

    OpcUa_Field_GetSize(String, PolicyId);

    *a_pSize = iSize;

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    *a_pSize = -1;

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserIdentityToken_Encode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserIdentityToken_Encode(OpcUa_UserIdentityToken* a_pValue, OpcUa_Encoder* a_pEncoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserIdentityToken_Encode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pEncoder);

    OpcUa_Field_Write(String, PolicyId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    /* nothing to do */

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserIdentityToken_Decode
 *===========================================================================*/
OpcUa_StatusCode OpcUa_UserIdentityToken_Decode(OpcUa_UserIdentityToken* a_pValue, OpcUa_Decoder* a_pDecoder)
{
    OpcUa_InitializeStatus(OpcUa_Module_Serializer, "UserIdentityToken_Decode");

    OpcUa_ReturnErrorIfArgumentNull(a_pValue);
    OpcUa_ReturnErrorIfArgumentNull(a_pDecoder);

    OpcUa_UserIdentityToken_Initialize(a_pValue);

    OpcUa_Field_Read(String, PolicyId);

    OpcUa_ReturnStatusCode;
    OpcUa_BeginErrorHandling;

    OpcUa_UserIdentityToken_Clear(a_pValue);

    OpcUa_FinishErrorHandling;
}

/*============================================================================
 * OpcUa_UserIdentityToken_EncodeableType
 *===========================================================================*/
struct _OpcUa_EncodeableType OpcUa_UserIdentityToken_EncodeableType =
{
    "UserIdentityToken",
    OpcUaId_UserIdentityToken,
    OpcUaId_UserIdentityToken_Encoding_DefaultBinary,
    OpcUaId_UserIdentityToken_Encoding_DefaultXml,
    OpcUa_Null,
    sizeof(OpcUa_UserIdentityToken),
    (OpcUa_EncodeableObject_PfnInitialize*)OpcUa_UserIdentityToken_Initialize,
    (OpcUa_EncodeableObject_PfnClear*)OpcUa_UserIdentityToken_Clear,
    (OpcUa_EncodeableObject_PfnGetSize*)OpcUa_UserIdentityToken_GetSize,
    (OpcUa_EncodeableObject_PfnEncode*)OpcUa_UserIdentityToken_Encode,
    (OpcUa_EncodeableObject_PfnDecode*)OpcUa_UserIdentityToken_Decode
};
#endif

/*============================================================================
 * Table of known types.
 *===========================================================================*/
static OpcUa_EncodeableType* g_OpcUa_KnownEncodeableTypes[] =
{
    #ifndef OPCUA_EXCLUDE_ApplicationDescription
    &OpcUa_ApplicationDescription_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RequestHeader
    &OpcUa_RequestHeader_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ResponseHeader
    &OpcUa_ResponseHeader_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ServiceFault
    &OpcUa_ServiceFault_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_FindServers
    &OpcUa_FindServersRequest_EncodeableType,
    &OpcUa_FindServersResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ServerOnNetwork
    &OpcUa_ServerOnNetwork_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_FindServersOnNetwork
    &OpcUa_FindServersOnNetworkRequest_EncodeableType,
    &OpcUa_FindServersOnNetworkResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_UserTokenPolicy
    &OpcUa_UserTokenPolicy_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_EndpointDescription
    &OpcUa_EndpointDescription_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_GetEndpoints
    &OpcUa_GetEndpointsRequest_EncodeableType,
    &OpcUa_GetEndpointsResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisteredServer
    &OpcUa_RegisteredServer_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisterServer
    &OpcUa_RegisterServerRequest_EncodeableType,
    &OpcUa_RegisterServerResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_MdnsDiscoveryConfiguration
    &OpcUa_MdnsDiscoveryConfiguration_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_RegisterServer2
    &OpcUa_RegisterServer2Request_EncodeableType,
    &OpcUa_RegisterServer2Response_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_ChannelSecurityToken
    &OpcUa_ChannelSecurityToken_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_OpenSecureChannel
    &OpcUa_OpenSecureChannelRequest_EncodeableType,
    &OpcUa_OpenSecureChannelResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_CloseSecureChannel
    &OpcUa_CloseSecureChannelRequest_EncodeableType,
    &OpcUa_CloseSecureChannelResponse_EncodeableType,
    #endif
    #ifndef OPCUA_EXCLUDE_UserIdentityToken
    &OpcUa_UserIdentityToken_EncodeableType,
    #endif
    OpcUa_Null
};

OpcUa_EncodeableType** OpcUa_KnownEncodeableTypes = g_OpcUa_KnownEncodeableTypes;
/* This is the last line of an autogenerated file. */
