/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <PrivateMock.h>
#include <Library/MemoryAllocationLib.h>
#include <I2cTouchPanel.h>

/**
  Event used for first read HID Descriptor transaction from Touch Device

  @param[in]  Event    Event called by I2C HOST on the I2C request completion
  @param[in]  Context  Context passed by the event creator
**/
extern
VOID
EFIAPI
HidDescriptorReceivedEvent (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  );

/**
  Mock of CoreDisconnectController.

  @param  ControllerHandle                      ControllerHandle The handle of
                                                the controller from which
                                                driver(s)  are to be
                                                disconnected.
  @param  DriverImageHandle                     DriverImageHandle The driver to
                                                disconnect from ControllerHandle.
  @param  ChildHandle                           ChildHandle The handle of the
                                                child to destroy.

  @retval EFI_SUCCESS                           One or more drivers were
                                                disconnected from the controller.
  @retval EFI_SUCCESS                           On entry, no drivers are managing
                                                ControllerHandle.
  @retval EFI_SUCCESS                           DriverImageHandle is not NULL,
                                                and on entry DriverImageHandle is
                                                not managing ControllerHandle.
  @retval EFI_INVALID_PARAMETER                 ControllerHandle is NULL.
  @retval EFI_INVALID_PARAMETER                 DriverImageHandle is not NULL,
                                                and it is not a valid EFI_HANDLE.
  @retval EFI_INVALID_PARAMETER                 ChildHandle is not NULL, and it
                                                is not a valid EFI_HANDLE.
  @retval EFI_OUT_OF_RESOURCES                  There are not enough resources
                                                available to disconnect any
                                                drivers from ControllerHandle.
  @retval EFI_DEVICE_ERROR                      The controller could not be
                                                disconnected because of a device
                                                error.

**/
EFI_STATUS
EFIAPI
MockBootServiceDisconnectController (
  IN  EFI_HANDLE  ControllerHandle,
  IN  EFI_HANDLE  DriverImageHandle  OPTIONAL,
  IN  EFI_HANDLE  ChildHandle        OPTIONAL
  )
{
  return mock_type (EFI_STATUS);
}
/**
  Guid is transformed to array.

  @param[IN]   Guid                  ProtocolGuid pointer.

  @param[OUT]  GuidToString          Guid is transformed to array and return to function.


  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GuidToString (
  IN  EFI_GUID    *Guid,
  OUT UINT32      **GuidToString
  )
{
  UINT32               *GuidArray;

  GuidArray = AllocateZeroPool (sizeof (UINT32) * 4);
  GuidArray[0] = Guid->Data1;
  GuidArray[1] = ((Guid->Data2 << 16) | Guid->Data3);
  GuidArray[2] = ((Guid->Data4[0] << 24) | (Guid->Data4[1] << 16) | (Guid->Data4[2] << 8) | (Guid->Data4[3]));
  GuidArray[3] = ((Guid->Data4[4] << 24) | (Guid->Data4[5] << 16) | (Guid->Data4[6] << 8) | (Guid->Data4[7]));

  *GuidToString = GuidArray;
  return EFI_SUCCESS;
}
/**
  Guid list is transformed to array.

  @param[IN]   ProtocolList                  Protocol list pointer.

  @param[IN]   ProtocolNumber                Number of Protocols.

  @param[OUT]  GuidListToString              Guid list is transformed to array and return to function.


  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GuidListToString (
  IN  PROTOCOLS_LIST    *ProtocolList,
  IN  UINT8             ProtocolNumber,
  OUT UINT32            **GuidListToString
  )
{
  UINT8                Index;
  UINT8                ProtocolListIndex;
  UINT8                ArraySize;
  UINT32               *GuidArray;

  // Array size equal to number of Protocols multiplied by 4.
  ArraySize = ProtocolNumber * 4;
  ProtocolListIndex = 0;
  GuidArray = AllocateZeroPool (sizeof (UINT32) * ArraySize);
  for (Index = 0; Index < ArraySize; Index++) {
    switch (Index % 4) {
    case 0:
      GuidArray[Index] = ProtocolList[ProtocolListIndex].Protocol->Data1;
      break;
    case 1:
      GuidArray[Index] = ((ProtocolList[ProtocolListIndex].Protocol->Data2 << 16) | ProtocolList[ProtocolListIndex].Protocol->Data3);
      break;
    case 2:
      GuidArray[Index] = ((ProtocolList[ProtocolListIndex].Protocol->Data4[0] << 24) | (ProtocolList[ProtocolListIndex].Protocol->Data4[1] << 16) | (ProtocolList[ProtocolListIndex].Protocol->Data4[2] << 8) | (ProtocolList[ProtocolListIndex].Protocol->Data4[3]));
      break;
    case 3:
      GuidArray[Index] = ((ProtocolList[ProtocolListIndex].Protocol->Data4[4] << 24) | (ProtocolList[ProtocolListIndex].Protocol->Data4[5] << 16) | (ProtocolList[ProtocolListIndex].Protocol->Data4[6] << 8) | (ProtocolList[ProtocolListIndex].Protocol->Data4[7]));
      ProtocolListIndex++;
      break;
      }
    }
    *GuidListToString = GuidArray;
  return EFI_SUCCESS;
}
/**
  Check Protocol is included in Protocol list.

  @param[IN]    GuidListString            Protocol list array pointer.

  @param[IN]    GuidString                Protocol array pointer.

  @retval TRUE  Protocol is included in Protocol list.

  @retval FALSE Protocol isn't included in Protocol list.
**/
BOOLEAN
EFIAPI
GuidStringCmp (
  IN UINT32             *GuidListString,
  IN UINT32             *GuidString
  )
{
  UINT8             Index;
  BOOLEAN           CmpStatus;

  for (Index = 0; Index < 4; Index++) {
    if (GuidListString[Index] == GuidString[Index]) {
      CmpStatus = TRUE;
    }   else if ((GuidListString[Index + 4] == GuidString[Index]) || (GuidListString[Index + 8] == GuidString[Index])) {
      CmpStatus = TRUE;
    } else {
      CmpStatus = FALSE;
    }
  }
  return CmpStatus;
}
/**
  Mock of I2cBusQueueRequest.

  @param[in] This               Pointer to an EFI_I2C_IO_PROTOCOL structure.
  @param[in] SlaveAddressIndex  Index value into an array of slave addresses
                                for the I2C device.  The values in the array
                                are specified by the board designer, with the
                                third party I2C device driver writer providing
                                the slave address order.

                                For devices that have a single slave address,
                                this value must be zero.  If the I2C device
                                uses more than one slave address then the
                                third party (upper level) I2C driver writer
                                needs to specify the order of entries in the
                                slave address array.

                                \ref ThirdPartyI2cDrivers "Third Party I2C
                                Drivers" section in I2cMaster.h.
  @param[in] Event              Event to signal for asynchronous transactions,
                                NULL for synchronous transactions
  @param[in] RequestPacket      Pointer to an EFI_I2C_REQUEST_PACKET structure
                                describing the I2C transaction
  @param[out] I2cStatus         Optional buffer to receive the I2C transaction
                                completion status

  @retval EFI_SUCCESS           The asynchronous transaction was successfully
                                queued when Event is not NULL.
  @retval EFI_SUCCESS           The transaction completed successfully when
                                Event is NULL.
  @retval EFI_BAD_BUFFER_SIZE   The RequestPacket->LengthInBytes value is too
                                large.
  @retval EFI_DEVICE_ERROR      There was an I2C error (NACK) during the
                                transaction.
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL
  @retval EFI_NO_MAPPING        The EFI_I2C_HOST_PROTOCOL could not set the
                                bus configuration required to access this I2C
                                device.
  @retval EFI_NO_RESPONSE       The I2C device is not responding to the slave
                                address selected by SlaveAddressIndex.
                                EFI_DEVICE_ERROR will be returned if the
                                controller cannot distinguish when the NACK
                                occurred.
  @retval EFI_OUT_OF_RESOURCES  Insufficient memory for I2C transaction
  @retval EFI_UNSUPPORTED       The controller does not support the requested
                                transaction.

**/
EFI_STATUS
EFIAPI
MockI2cBusQueueRequest (
  IN CONST EFI_I2C_IO_PROTOCOL  *This,
  IN UINTN                      SlaveAddressIndex,
  IN EFI_EVENT                  Event               OPTIONAL,
  IN EFI_I2C_REQUEST_PACKET     *RequestPacket,
  OUT EFI_STATUS                *I2cStatus          OPTIONAL
  )
{
  EFI_I2C_REQUEST_PACKET     **pRequestPacket;

  check_expected (SlaveAddressIndex);
  pRequestPacket = mock_ptr_type (EFI_I2C_REQUEST_PACKET **);
  *pRequestPacket = RequestPacket;

  return mock_type (EFI_STATUS);
}
/**
  Mock of CreateEvent for GetHidDescriptor().

  @param[in]   Type             The type of event to create and its mode and attributes.
  @param[in]   NotifyTpl        The task priority level of event notifications, if needed.
  @param[in]   NotifyFunction   The pointer to the event's notification function, if any.
  @param[in]   NotifyContext    The pointer to the notification function's context; corresponds to parameter
                                Context in the notification function.
  @param[out]  Event            The pointer to the newly created event if the call succeeds; undefined
                                otherwise.

  @retval EFI_SUCCESS           The event structure was created.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The event could not be allocated.
**/

EFI_STATUS
EFIAPI
MockCreateEventforGetHidDescriptor (
  IN  UINT32                       Type,
  IN  EFI_TPL                      NotifyTpl,
  IN  EFI_EVENT_NOTIFY             NotifyFunction,
  IN  VOID                         *NotifyContext,
  OUT EFI_EVENT                    *Event
  )
{
  check_expected (Type);
  check_expected (NotifyTpl);
  check_expected (NotifyFunction);

  //
  // HidDescriptorReceivedEvent() and CheckDataAvailableCallback() need to modify NotifyContext.
  //
  if (NotifyFunction == HidDescriptorReceivedEvent) {
    *((BOOLEAN *) NotifyContext) = mock_type (BOOLEAN);
  } else if (NotifyFunction == CheckDataAvailableCallback){
    ((ABSOLUTE_POINTER_DEV *) NotifyContext)->HidDescriptor.HIDDescLengthLsb = 1;
  }

  return mock_type (EFI_STATUS);
}
/**
  Mock of loseProtocol for TouchPanelDriverDriverBindingStart().
  @param  UserHandle             The handle for the protocol interface that was
                                 previously opened with OpenProtocol(), and is
                                 now being closed.
  @param  Protocol               The published unique identifier of the protocol.
                                 It is the caller's responsibility to pass in a
                                 valid GUID.
  @param  AgentHandle            The handle of the agent that is closing the
                                 protocol interface.
  @param  ControllerHandle       If the agent that opened a protocol is a driver
                                 that follows the EFI Driver Model, then this
                                 parameter is the controller handle that required
                                 the protocol interface. If the agent does not
                                 follow the EFI Driver Model, then this parameter
                                 is optional and may be NULL.
  @retval EFI_SUCCESS            The protocol instance was closed.
  @retval EFI_INVALID_PARAMETER  Handle, AgentHandle or ControllerHandle is not a
                                 valid EFI_HANDLE.
  @retval EFI_NOT_FOUND          Can not find the specified protocol or
                                 AgentHandle.
**/
EFI_STATUS
EFIAPI
MockBootServiceCloseProtocolForStart (
  IN  EFI_HANDLE  UserHandle,
  IN  EFI_GUID    *Protocol,
  IN  EFI_HANDLE  AgentHandle,
  IN  EFI_HANDLE  ControllerHandle
  )
{
  // CloseProtocol in TouchPanelDriverDriverBindingStart() don't recevie status.
  // return "mock_type (EFI_STATUS);" cause Unit Test build error.
  return EFI_SUCCESS;
}
/**
  Mock of CloseProtocol for TouchPanelDriverDriverBindingStart().
  Because CloseProtocol in some functions don't recevie status from gBS->CloseProtocol.
  return "mock_type (EFI_STATUS);" cause Unit Test build error.

  @param  UserHandle             The handle for the protocol interface that was
                                 previously opened with OpenProtocol(), and is
                                 now being closed.
  @param  Protocol               The published unique identifier of the protocol.
                                 It is the caller's responsibility to pass in a
                                 valid GUID.
  @param  AgentHandle            The handle of the agent that is closing the
                                 protocol interface.
  @param  ControllerHandle       If the agent that opened a protocol is a driver
                                 that follows the EFI Driver Model, then this
                                 parameter is the controller handle that required
                                 the protocol interface. If the agent does not
                                 follow the EFI Driver Model, then this parameter
                                 is optional and may be NULL.
  @retval EFI_SUCCESS            The protocol instance was closed.
  @retval EFI_INVALID_PARAMETER  Handle, AgentHandle or ControllerHandle is not a
                                 valid EFI_HANDLE.
  @retval EFI_NOT_FOUND          Can not find the specified protocol or
                                 AgentHandle.
**/
EFI_STATUS
EFIAPI
MockBootServiceCloseProtocolNonReturnStatus (
  IN  EFI_HANDLE  UserHandle,
  IN  EFI_GUID    *Protocol,
  IN  EFI_HANDLE  AgentHandle,
  IN  EFI_HANDLE  ControllerHandle
  )
{
  return EFI_SUCCESS;
}
/**
   Mock of CloseEvent for no need to return parameter.
   Because CloseEvent in some functions don't recevie status from gBS->CloseEvent.
   return "mock_type (EFI_STATUS);" cause Unit Test build error.
  @param  UserEvent              Event to close

  @retval EFI_INVALID_PARAMETER  Parameters are not valid.
  @retval EFI_SUCCESS            The event has been closed

**/
EFI_STATUS
EFIAPI
MockCloseEventNonReturnStatus (
  IN EFI_EVENT UserEvent
  )
{
  return EFI_SUCCESS;
}
/**
  Mock of FreeUnicodeStringTable for no need to return parameter..

  If UnicodeStringTable is NULL, then EFI_SUCCESS is returned.
  Otherwise, each language code, and each Unicode string in the Unicode string
  table are freed, and EFI_SUCCESS is returned.

  @param  UnicodeStringTable  A pointer to the table of Unicode strings.

  @retval EFI_SUCCESS         The Unicode string table was freed.

**/
EFI_STATUS
EFIAPI
FreeUnicodeStringTable (
  IN EFI_UNICODE_STRING_TABLE  *UnicodeStringTable
  )
{
  return EFI_SUCCESS;
}
/**
  Mock I2cBusQueueRequest and return RequestPacket.

  @param[in] This               Pointer to an EFI_I2C_IO_PROTOCOL structure.
  @param[in] SlaveAddressIndex  Index value into an array of slave addresses
                                for the I2C device.  The values in the array
                                are specified by the board designer, with the
                                third party I2C device driver writer providing
                                the slave address order.

                                For devices that have a single slave address,
                                this value must be zero.  If the I2C device
                                uses more than one slave address then the
                                third party (upper level) I2C driver writer
                                needs to specify the order of entries in the
                                slave address array.

                                \ref ThirdPartyI2cDrivers "Third Party I2C
                                Drivers" section in I2cMaster.h.
  @param[in] Event              Event to signal for asynchronous transactions,
                                NULL for synchronous transactions
  @param[in] RequestPacket      Pointer to an EFI_I2C_REQUEST_PACKET structure
                                describing the I2C transaction
  @param[out] I2cStatus         Optional buffer to receive the I2C transaction
                                completion status

  @retval EFI_SUCCESS           The asynchronous transaction was successfully
                                queued when Event is not NULL.
  @retval EFI_SUCCESS           The transaction completed successfully when
                                Event is NULL.
  @retval EFI_BAD_BUFFER_SIZE   The RequestPacket->LengthInBytes value is too
                                large.
  @retval EFI_DEVICE_ERROR      There was an I2C error (NACK) during the
                                transaction.
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL
  @retval EFI_NO_MAPPING        The EFI_I2C_HOST_PROTOCOL could not set the
                                bus configuration required to access this I2C
                                device.
  @retval EFI_NO_RESPONSE       The I2C device is not responding to the slave
                                address selected by SlaveAddressIndex.
                                EFI_DEVICE_ERROR will be returned if the
                                controller cannot distinguish when the NACK
                                occurred.
  @retval EFI_OUT_OF_RESOURCES  Insufficient memory for I2C transaction
  @retval EFI_UNSUPPORTED       The controller does not support the requested
                                transaction.

**/
EFI_STATUS
EFIAPI
MockI2cBusQueueRequestReturnRequestPacket (
  IN CONST EFI_I2C_IO_PROTOCOL  *This,
  IN UINTN                      SlaveAddressIndex,
  IN EFI_EVENT                  Event               OPTIONAL,
  IN EFI_I2C_REQUEST_PACKET     *RequestPacket,
  OUT EFI_STATUS                *I2cStatus          OPTIONAL
  )
{
  UINT8                    BufferSize;
  UINT8                    Index;

  BufferSize = mock_type (UINT8);
  for (Index = 0; Index < BufferSize; Index++) {
    RequestPacket->Operation[0].Buffer[Index] = mock_type (UINT8);
  }
  return mock_type (EFI_STATUS);
}