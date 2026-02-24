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

#ifndef _OpcUa_Endpoint_Internal_H_
#define _OpcUa_Endpoint_Internal_H_ 1

OPCUA_BEGIN_EXTERN_C

/* @brief Return invalid state if endpoint is not open. */
#define OPCUA_ENDPOINT_CHECKOPEN(xEndpoint) \
        OpcUa_ReturnErrorIfTrue((((OpcUa_EndpointInternal*)xEndpoint)->State != eOpcUa_Endpoint_State_Open), OpcUa_BadInvalidState);

/*============================================================================
 * OpcUa_Endpoint_State
 *===========================================================================*/
/**
 * @brief Describes the current state of the endpoint object.
 */
enum _OpcUa_Endpoint_State
{
    eOpcUa_Endpoint_State_Invalid,
    eOpcUa_Endpoint_State_Open,
    eOpcUa_Endpoint_State_Closed
};
typedef enum _OpcUa_Endpoint_State OpcUa_Endpoint_State;

/*============================================================================
 * OpcUa_EndpointInternal
 *===========================================================================*/
/**
 * @brief Manages an endpoint for a server. (internal representation)
 */
typedef struct _OpcUa_EndpointInternal
{
    /*! @brief An opaque handle associated with the endpoint. */
    OpcUa_Handle Handle;

    /*! @brief The current state of the endpoint. */
    OpcUa_Endpoint_State State;

    /*! @brief The url for the endpoint. */
    OpcUa_String Url;

    /*! @brief The type of encoding to use with the endpoint. */
    OpcUa_EncoderType EncoderType;

    /*! @brief The secure channel callback from application layer. */
    OpcUa_Endpoint_PfnEndpointCallback* pfEndpointCallback;

    /*! @brief The data to pass to the applications callback. */
    OpcUa_Void* pvEndpointCallbackData;

    /*! @brief The listener for the transport layer. */
    struct _OpcUa_Listener* TransportListener;

    /*! @brief The listener for the security layer. */
    struct _OpcUa_Listener* SecureListener;

    /*! @brief The encoder used for messages sent via the endpoint. */
    struct _OpcUa_Encoder* Encoder;

    /*! @brief The decoder used for messages received via the endpoint. */
    struct _OpcUa_Decoder* Decoder;

    /*! @brief The services supported by the endpoint. */
    OpcUa_ServiceTable SupportedServices;

    /*! @brief A mutex used to synchronize access to the endpoint structure. */
    OpcUa_Mutex Mutex;

    /*! @brief The current status of the endpoint. */
    OpcUa_StatusCode Status;
} OpcUa_EndpointInternal;

OPCUA_END_EXTERN_C

#endif /* _OpcUa_Endpoint_Internal_H_ */
