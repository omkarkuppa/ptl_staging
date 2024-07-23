/**@file
    Boot Services mock of the I2c Platform Dxe driver.
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

#ifndef UEFI_BOOT_SERVICES_TABLE_LIB_MOCK_H
#define UEFI_BOOT_SERVICES_TABLE_LIB_MOCK_H

#include <Uefi.h>

typedef struct {
  EFI_GUID     *Protocol;
  VOID         *Interface;
} PROTOCOLS_LIST;

/**
  Mock of Boot Services for OpenProtocol.
  @param  UserHandle             The handle to obtain the protocol interface on
  @param  Protocol               The ID of the protocol
  @param  Interface              The location to return the protocol interface
  @param  ImageHandle            The handle of the Image that is opening the
                                 protocol interface specified by Protocol and
                                 Interface.
  @param  ControllerHandle       The controller handle that is requiring this
                                 interface.
  @param  Attributes             The open mode of the protocol interface
                                 specified by Handle and Protocol.
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_SUCCESS            Get the protocol interface.
**/
EFI_STATUS
EFIAPI
MockBootServiceOpenProtocol (
  IN  EFI_HANDLE  UserHandle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface OPTIONAL,
  IN  EFI_HANDLE  ImageHandle,
  IN  EFI_HANDLE  ControllerHandle,
  IN  UINT32      Attributes
  );

/**
  Mock of Boot Services for LocateProtocol.
  @param  Protocol               The protocol to search for
  @param  Registration           Optional Registration Key returned from
                                 RegisterProtocolNotify()
  @param  Interface              Return the Protocol interface (instance).
  @retval EFI_SUCCESS            If a valid Interface is returned
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found
**/
EFI_STATUS
EFIAPI
MockBootServiceLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  );

/**
  Mock of Boot Services for CloseProtocol.
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
MockBootServiceCloseProtocol (
  IN  EFI_HANDLE  UserHandle,
  IN  EFI_GUID    *Protocol,
  IN  EFI_HANDLE  AgentHandle,
  IN  EFI_HANDLE  ControllerHandle
  );

/**
  Mock of Boot Services for InstallMultipleProtocolInterfaces.

  @param[in, out]  Handle       The pointer to a handle to install the new protocol interfaces on,
                                or a pointer to NULL if a new handle is to be allocated.
  @param  ...                   A variable argument list containing pairs of protocol GUIDs and protocol
                                interfaces.

  @retval EFI_SUCCESS           All the protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES  There was not enough memory in pool to install all the protocols.
  @retval EFI_ALREADY_STARTED   A Device Path Protocol instance was passed in that is already present in
                                the handle database.
  @retval EFI_INVALID_PARAMETER Handle is NULL.
  @retval EFI_INVALID_PARAMETER Protocol is already installed on the handle specified by Handle.

**/
EFI_STATUS
EFIAPI
MockBootServiceInstallMultipleProtocolInterfaces (
  IN OUT EFI_HANDLE           *Handle,
  ...
  );

/**
  Mock of Boot Services for UninstallMultipleProtocolInterfaces.

  @param  Handle                 The handle to uninstall the protocol interfaces
                                 from.
  @param  ...                    EFI_GUID followed by protocol instance. A NULL
                                 terminates the list. The pairs are the
                                 arguments to UninstallProtocolInterface(). All
                                 the protocols are added to Handle.

  @retval EFI_SUCCESS            if all protocol interfaces where uninstalled.
  @retval EFI_INVALID_PARAMETER  if any protocol interface could not be
                                 uninstalled and an attempt was made to
                                 reinstall previously uninstalled protocol
                                 interfaces.
**/
EFI_STATUS
EFIAPI
MockBootServiceUninstallMultipleProtocolInterfaces (
  IN OUT EFI_HANDLE           *Handle,
  ...
  );
/**
  Mock of Boot Services for LocateHandleBuffer.

  @param  SearchType             Specifies which handle(s) are to be returned.
  @param  Protocol               Provides the protocol to search by.    This
                                 parameter is only valid for SearchType
                                 ByProtocol.
  @param  SearchKey              Supplies the search key depending on the
                                 SearchType.
  @param  NumberHandles          The number of handles returned in Buffer.
  @param  Buffer                 A pointer to the buffer to return the requested
                                 array of  handles that support Protocol.

  @retval EFI_SUCCESS            The result array of handles was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the
                                 matching results.
  @retval EFI_INVALID_PARAMETER  One or more parameters are not valid.

**/
EFI_STATUS
EFIAPI
MockBootServiceLocateHandleBuffer (
  IN EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN EFI_GUID                *Protocol OPTIONAL,
  IN VOID                    *SearchKey OPTIONAL,
  IN OUT UINTN               *NumberHandles,
  OUT EFI_HANDLE             **Buffer
  );

/**
  Mock of Create event.

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
MockCreateEvent (
  IN  UINT32                       Type,
  IN  EFI_TPL                      NotifyTpl,
  IN  EFI_EVENT_NOTIFY             NotifyFunction,
  IN  VOID                         *NotifyContext,
  OUT EFI_EVENT                    *Event
  );

/**
  Mock of SetTimer.

  @param  UserEvent              The timer event that is to be signaled at the
                                 specified time
  @param  Type                   The type of time that is specified in
                                 TriggerTime
  @param  TriggerTime            The number of 100ns units until the timer
                                 expires

  @retval EFI_SUCCESS            The event has been set to be signaled at the
                                 requested time
  @retval EFI_INVALID_PARAMETER  Event or Type is not valid

**/
EFI_STATUS
EFIAPI
MockSetTimer (
  IN EFI_EVENT UserEvent,
  IN EFI_TIMER_DELAY Type,
  IN UINT64 TriggerTime
  );

/**  Mock of MicroSecondDelay.

  @param[in]  MicroSeconds  The minimum number of microseconds to delay.

  @return The value of MicroSeconds input.

**/
UINTN
EFIAPI
MockMicroSecondDelay (
  IN      UINTN                     MicroSeconds
  );
/**
  Mock of CloseEvent.

  @param  UserEvent              Event to close

  @retval EFI_INVALID_PARAMETER  Parameters are not valid.
  @retval EFI_SUCCESS            The event has been closed

**/
EFI_STATUS
EFIAPI
MockCloseEvent (
  IN EFI_EVENT  UserEvent
  );
/**
  Mock of SignalEvent.

  @param  UserEvent              The event to signal .

  @retval EFI_INVALID_PARAMETER  Parameters are not valid.
  @retval EFI_SUCCESS            The event was signaled.

**/
EFI_STATUS
EFIAPI
MockSignalEvent (
  IN EFI_EVENT  UserEvent
  );

/**
  Mock of Boot Services for HandleProtocol.

  @param  UserHandle             The handle being queried.
  @param  Protocol               The published unique identifier of the protocol.
  @param  Interface              Supplies the address where a pointer to the corresponding Protocol
                                 Interface is returned.

  @retval EFI_SUCCESS            The interface information for the specified protocol was returned.
  @retval EFI_UNSUPPORTED        The device does not support the specified protocol.
  @retval EFI_INVALID_PARAMETER  Handle is NULL..
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.

**/
EFI_STATUS
EFIAPI
MockBootServiceHandleProtocol (
  IN  EFI_HANDLE  UserHandle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface
  );

/**
  Mock of Boot Services for CreateEventEx: Creates an event in a group.

  @param  Type                   The type of event to create and its mode and
                                 attributes
  @param  NotifyTpl              The task priority level of event notifications
  @param  NotifyFunction         Pointer to the events notification function
  @param  NotifyContext          Pointer to the notification functions context;
                                 corresponds to parameter "Context" in the
                                 notification function
  @param  EventGroup             GUID for EventGroup if NULL act the same as
                                 gBS->CreateEvent().
  @param  Event                  Pointer to the newly created event if the call
                                 succeeds; undefined otherwise

  @retval EFI_SUCCESS            The event structure was created
  @retval EFI_INVALID_PARAMETER  One of the parameters has an invalid value
  @retval EFI_OUT_OF_RESOURCES   The event could not be allocated

**/
EFI_STATUS
EFIAPI
MockBootServiceCreateEventEx (
  IN  UINT32            Type,
  IN  EFI_TPL           NotifyTpl,
  IN  EFI_EVENT_NOTIFY  NotifyFunction  OPTIONAL,
  IN  CONST VOID        *NotifyContext  OPTIONAL,
  IN  CONST EFI_GUID    *EventGroup     OPTIONAL,
  OUT EFI_EVENT         *Event
  );

/**
  Mock of Boot Services for InstallProtocol.

  @param  UserHandle             The handle to install the protocol handler on, or NULL
                                 if a new handle is to be allocated
  @param  Protocol               The protocol to add to the handle
  @param  InterfaceType          Indicates whether Interface is supplied in native form.
  @param  Interface              The interface for the protocol being added

  @return Status code

**/
EFI_STATUS
EFIAPI
MockBootServiceInstallProtocolInterface (
  IN OUT EFI_HANDLE      *UserHandle,
  IN EFI_GUID            *Protocol,
  IN EFI_INTERFACE_TYPE  InterfaceType,
  IN VOID                *Interface
  );

#endif /*UEFI_BOOT_SERVICES_TABLE_LIB_MOCK_H*/
