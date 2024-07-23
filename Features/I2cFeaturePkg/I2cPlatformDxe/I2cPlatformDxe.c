/**@file
  Implements the EFI driver binding driver to produce I2cEnumerateProtocol and
  I2cBusConfigurationManagementProtocol which is later consumed by I2cBus driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <I2cPlatformDxe.h>
#include <Library/PcdLib.h>
#include <Library/DevicePathLib.h>

/**
  Checks if Device with given PciDeviceId is one of SerialIo I2C controllers
  If yes, its number is returned through I2cIndex parameter, otherwise I2cIndex is not updated

  @param[in]  PciDevId                  Device ID
  @param[out] I2cNumber                 Number of SerialIo I2C controller

  @retval TRUE                          yes it is a SerialIo I2C controller
  @retval FALSE                         no it isn't a SerialIo I2C controller
**/
BOOLEAN
IsSerialIoI2cDeviceIdCp (
  IN  UINT16    PciDevId,
  OUT UINT16    *I2cNumber
  )
{
  UINT16  Index;

  SERIAL_IO_I2C_DEVID *SerialIoI2cDevid;
  SerialIoI2cDevid = NULL;

  SerialIoI2cDevid = (SERIAL_IO_I2C_DEVID *) PcdGetPtr (PcdSerialIoI2cDevId);

  if (SerialIoI2cDevid == NULL) {
    return FALSE;
  }

  for (Index = 0; Index < (SerialIoI2cDevid->Count); Index++) {
    if (PciDevId == (SerialIoI2cDevid->SerialIoI2cDevId[Index])) {
      *I2cNumber = Index;
      return TRUE;
    }
  }
  return FALSE;
}

/*
  Set bus configuration
  Since our platforms support only a single configuration per i2c bus,
  the only thing this function needs to do is to configure i2c central for correct bus speed

  @param[in]      This                     Pointer to an EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL structure.
  @param[in]      I2cBusConfiguration      Index of an I2C bus configuration. All values in the range of zero to N-1 are valid where N is the total number of I2C bus configurations for an I2C bus.
  @param[in]      Event                    Event to signal when the transaction is complete
  @param[in]      I2cStatus                Buffer to receive the transaction status.

  @retval EFI_SUCCESS              The asynchronous bus configuration request was successfully started when Event is not NULL.
  @retval EFI_SUCCESS              The bus configuration request completed successfully when Event is NULL.
  @retval EFI_DEVICE_ERROR         The bus configuration failed.
  @retval EFI_DEVICE_ERROR         Invalid I2cBusConfiguration value
*/
EFI_STATUS
EFIAPI
SetConfiguration (
  IN CONST EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL *This,
  IN UINTN                                               I2cBusConfiguration,
  IN EFI_EVENT                                           Event      OPTIONAL,
  IN EFI_STATUS                                          *I2cStatus OPTIONAL
  )
{
  I2C_PLATFORM_CONTEXT     *Context;
  EFI_I2C_MASTER_PROTOCOL  *CentralProtocol;
  EFI_I2C_ENUMERATE_PROTOCOL *EnumerateProtocol;
  EFI_STATUS               Status;
  UINTN                    Frequency;

  Context = I2C_PLATFORM_CONTEXT_FROM_BUS_CFG_PROTOCOL (This);

  Status = gBS->OpenProtocol (
                  Context->ControllerHandle,
                  &gEfiI2cEnumerateProtocolGuid,
                  (VOID **) &EnumerateProtocol,
                  Context->ControllerHandle,
                  Context->ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG (( DEBUG_INFO, "SetConfiguration error @Enum, %r\n", Status ));
    return Status;
  }
  Status = EnumerateProtocol->GetBusFrequency(EnumerateProtocol, I2cBusConfiguration, &Frequency);
  gBS->CloseProtocol (
         Context->ControllerHandle,
         &gEfiI2cEnumerateProtocolGuid,
         Context->ControllerHandle,
         Context->ControllerHandle );

  Status = gBS->OpenProtocol (
                  Context->ControllerHandle,
                  &gEfiI2cMasterProtocolGuid,
                  (VOID **) &CentralProtocol,
                  Context->ControllerHandle,
                  Context->ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG (( DEBUG_INFO, "SetConfiguration error @Central, %r\n", Status ));
    return Status;
  }
  Status = CentralProtocol->SetBusFrequency(CentralProtocol, &Frequency);
  gBS->CloseProtocol (
         Context->ControllerHandle,
         &gEfiI2cMasterProtocolGuid,
         Context->ControllerHandle,
         Context->ControllerHandle );

  if (I2cStatus != NULL) {
    *I2cStatus = Status;
  };
  if (Event != NULL) {
    gBS->SignalEvent(Event);
  };
  return Status;
}

/**
  Get PCI Device ID from device pointed to by handle.
  If valid DeviceID can't be retrieved, 0xFFFF is returned

  @param[in] Handle   device handle

  retval     PCI Device ID
**/
UINT16
GetPciDevId (
  EFI_HANDLE Handle
  )
{
  EFI_STATUS          Status;
  EFI_PCI_IO_PROTOCOL *PciIo;
  UINT32              VenDevId;

  Status = gBS->OpenProtocol (
                  Handle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  Handle,
                  Handle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return 0xFFFF;
  }
  Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint32, 0, 1, &VenDevId);
  gBS->CloseProtocol (
         Handle,
         &gEfiPciIoProtocolGuid,
         Handle,
         Handle );
  if (EFI_ERROR(Status)) {
    return 0xFFFF;
  }
  return (VenDevId>>16);
}

/*
  GetBusFrequency
  For now, this function simply returns 100kHz because we only use one bus,
  single configuration, with a single device and it accepts just that speed

  @param[in]      This                     A pointer to the EFI_I2C_ENUMERATE_PROTOCOL instance.
  @param[in]      I2cBusConfiguration      A pointer to the I2cBusConfiguration
  @param[in/out]  BusClockHertz            Pointer is updated with right Serial I2C controller frequency

  @retval EFI_SUCCESS              BusClockHertz is returned with freq value
  @retval EFI_NOT_FOUND            End of device list
*/
EFI_STATUS
EFIAPI
GetBusFrequency (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN UINTN                            I2cBusConfiguration,
  OUT UINTN                           *BusClockHertz
  )
{
  I2C_FREQUENCY_DATA_TABLE *I2cFreqMap;
  I2C_PLATFORM_CONTEXT *Context;

  I2cFreqMap = NULL;

  // Get context to extract Controller Serial I2C controller number
  Context = I2C_PLATFORM_CONTEXT_FROM_ENUMERATION_PROTOCOL(This);

  I2cFreqMap = (I2C_FREQUENCY_DATA_TABLE *) PcdGetPtr (PcdI2cFrequencyDataMap);
  if (I2cFreqMap == NULL) {
    return EFI_NOT_FOUND;
  }
  *BusClockHertz = I2cFreqMap->I2cFrequency[Context->I2cNumber];

  return EFI_SUCCESS;
}

/*
  Enumerate the I2C devices

  This function enables the caller to traverse the set of I2C devices
  on an I2C bus.

  @param[in]  This              The platform data for the next device on
                                the I2C bus was returned successfully.
  @param[in, out] Device        Pointer to a buffer containing an
                                EFI_I2C_DEVICE structure.  Enumeration is
                                started by setting the initial EFI_I2C_DEVICE
                                structure pointer to NULL.  The buffer
                                receives an EFI_I2C_DEVICE structure pointer
                                to the next I2C device.

  @retval EFI_SUCCESS           The platform data for the next device on
                                the I2C bus was returned successfully.
  @retval EFI_INVALID_PARAMETER Device is NULL
  @retval EFI_NO_MAPPING        *Device does not point to a valid
                                EFI_I2C_DEVICE structure returned in a
                                previous call Enumerate().
  @retval EFI_NOT_FOUND         No any I2c devices on the I2C bus.
*/
EFI_STATUS
EFIAPI
Enumerate (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN OUT CONST EFI_I2C_DEVICE         **Device
  )
{
  I2C_PLATFORM_CONTEXT        *Context;
  UINTN                       Index;
  I2C_CONTROLLER_DEVICE_LIST  *I2cConDevList;
  EFI_I2C_DEVICE              *DeviceItem;

  DEBUG ((DEBUG_INFO, "%a - Enumerate I2C device\n", __FUNCTION__));

  if (Device == NULL) {
    DEBUG ((DEBUG_INFO, "EFI_INVALID_PARAMETER.\n"));
    return EFI_INVALID_PARAMETER;
  }

  Context       = I2C_PLATFORM_CONTEXT_FROM_ENUMERATION_PROTOCOL (This);
  I2cConDevList = Context->I2cConDevList;
  DeviceItem    = (VOID*) &(I2cConDevList->I2cControllerDeviceList[Context->I2cNumber].I2cDeviceList[0]);
  Index         = 0;

  DEBUG ((DEBUG_INFO, "[In]: Context->I2cNumber = %02x\n", Context->I2cNumber));
  DEBUG ((DEBUG_INFO, "[In]: DeviceItem = %x\n", DeviceItem));
  DEBUG ((DEBUG_INFO, "[In]: Device = %x\n", *Device));
  if (*Device != NULL) {
    DEBUG ((DEBUG_INFO, "[In]: DeviceIndex = %x\n", (**Device).DeviceIndex));
  }
  DEBUG ((DEBUG_INFO, "[In]: Count = %02x\n", I2cConDevList->I2cControllerDeviceList[Context->I2cNumber].Count));

  //
  // Don't exist any I2C device
  //
  // We have defined a DEVICE_END in the End of I2c devices list.
  // If the I2cConDevList.Count < 2 means there is no any devices in this I2c Bus.
  //
  if ((I2cConDevList->I2cControllerDeviceList[Context->I2cNumber].Count) < 2) {
    DEBUG ((DEBUG_INFO, "Device not found.\n"));
    *Device = NULL;
    return EFI_NOT_FOUND;
  }

  //
  // Enumeration is started by setting the initial EFI_I2C_DEVICE sturcture pointer to NULL
  //
  if (*Device == NULL) {
    *Device = DeviceItem;
    DEBUG ((DEBUG_INFO, "Enumerate the first one device. Device = %x\n", *Device));
    DEBUG ((DEBUG_INFO, "DeviceIndex = %x\n", (**Device).DeviceIndex));
    return EFI_SUCCESS;
  }

  //
  // Request next device
  //
  if (((**Device).DeviceGuid) != 0) {
    for (Index = 0; Index < ((I2cConDevList->I2cControllerDeviceList[Context->I2cNumber].Count) - 2); Index++) {
      DEBUG ((DEBUG_INFO, "Index = %02x\n", Index));
      if (*Device == (DeviceItem + Index)) {
        *Device = (DeviceItem + Index + 1);
        DEBUG ((DEBUG_INFO, "Assign next device. Device = %x\n", *Device));
        DEBUG ((DEBUG_INFO, "DeviceIndex = %x\n", (**Device).DeviceIndex));
        return EFI_SUCCESS;
      }
    }
  }

  //
  // Don't exist any I2C device
  //
  *Device = NULL;
  DEBUG((DEBUG_INFO, "Don't exist any I2c device\n"));
  return EFI_NO_MAPPING;
}

/**
  Callback function when EfiPciEnumerationCompleteProtocol is installed.

  @param[in]  Event                Event whose notification function is being invoked
  @param[in]  Context              Pointer to the notification functions context

**/
VOID
EFIAPI
I2cConnectEndOfPciEnumerationEvent (
  IN EFI_EVENT           Event,
  IN VOID                *Context
  )
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  UINTN                         PciIoHandleCount;
  EFI_HANDLE                    *PciIoHandleBuffer;
  UINT16                        Number;

  DEBUG ((DEBUG_INFO, "%a Connect I2C Protocol\n", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (EFI_ERROR (Status) || (PciIoHandleBuffer == NULL) || (PciIoHandleCount == 0)) {
    return;
  }

  for (Index = 0; Index < PciIoHandleCount; Index++) {
    if (!IsSerialIoI2cDeviceIdCp (GetPciDevId (PciIoHandleBuffer[Index]), &Number)) {
      continue;
    }
    gBS->ConnectController (PciIoHandleBuffer[Index], NULL, NULL, TRUE);
  }
  gBS->CloseEvent (Event);
  gBS->FreePool (PciIoHandleBuffer);
}

/**
  I2cPlatformDriverBindingSupported - Checks if this driver can install on device pointed to by Handle
  It will only install on I2c controllers

  Tests to see if this driver supports a given controller. If a child device is provided,
  it further tests to see if this driver supports creating a handle for the specified child device.

  This function checks to see if the driver specified by This supports the device specified by
  ControllerHandle. Drivers will typically use the device path attached to
  ControllerHandle and/or the services from the bus I/O abstraction attached to
  ControllerHandle to determine if the driver supports ControllerHandle. This function
  may be called many times during platform initialization. In order to reduce boot times, the tests
  performed by this function must be very small, and take as little time as possible to execute. This
  function must not change the state of any hardware devices, and this function must be aware that the
  device specified by ControllerHandle may already be managed by the same driver or a
  different driver. This function must match its calls to AllocatePages() with FreePages(),
  AllocatePool() with FreePool(), and OpenProtocol() with CloseProtocol().
  Since ControllerHandle may have been previously started by the same driver, if a protocol is
  already in the opened state, then it must not be closed with CloseProtocol(). This is required
  to guarantee the state of ControllerHandle is not modified by this function.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to test. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For bus drivers, if this parameter is not NULL, then
                                   the bus driver must determine if the bus controller specified
                                   by ControllerHandle and the child controller specified
                                   by RemainingDevicePath are both supported by this
                                   bus driver.

  @retval EFI_SUCCESS              The device specified by ControllerHandle and
                                   RemainingDevicePath is supported by the driver specified by This.
  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by the driver
                                   specified by This.
  @retval EFI_ACCESS_DENIED        The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by a different
                                   driver or an application that requires exclusive access.
                                   Currently not implemented.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the driver specified by This.
**/
EFI_STATUS
EFIAPI
I2cPlatformDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL *I2cBusConfigurationManagement;
  EFI_STATUS                                    Status;
  UINT16                                        Number;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cBusConfigurationManagementProtocolGuid,
                  (VOID **)&I2cBusConfigurationManagement,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (Status == EFI_SUCCESS) {
    return EFI_ALREADY_STARTED;
  }

  if (!IsSerialIoI2cDeviceIdCp (GetPciDevId (ControllerHandle), &Number)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Starts a device controller or a bus controller.

  The Start() function is designed to be invoked from the EFI boot service ConnectController().
  As a result, much of the error checking on the parameters to Start() has been moved into this
  common boot service. It is legal to call Start() from other locations,
  but the following calling restrictions must be followed or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE.
  2. If RemainingDevicePath is not NULL, then it must be a pointer to a naturally aligned
     EFI_DEVICE_PATH_PROTOCOL.
  3. Prior to calling Start(), the Supported() function for the driver specified by This must
     have been called with the same calling parameters, and Supported() must have returned EFI_SUCCESS.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For a bus driver, if this parameter is NULL, then handles
                                   for all the children of Controller are created by this driver.
                                   If this parameter is not NULL and the first Device Path Node is
                                   not the End of Device Path Node, then only the handle for the
                                   child device specified by the first Device Path Node of
                                   RemainingDevicePath is created by this driver.
                                   If the first Device Path Node of RemainingDevicePath is
                                   the End of Device Path Node, no child handle is created by this
                                   driver.

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
I2cPlatformDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  UINT16  I2cNumber;
  I2C_PLATFORM_CONTEXT  *Context;
  EFI_STATUS  Status;

  if (!IsSerialIoI2cDeviceIdCp (GetPciDevId (ControllerHandle), &I2cNumber)) {
    return EFI_UNSUPPORTED;
  }

  Context = AllocateZeroPool (sizeof (I2C_PLATFORM_CONTEXT));

  if (Context == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Context->Signature                                  = I2C_PLATFORM_SIGNATURE;
  Context->ControllerHandle                           = ControllerHandle;
  Context->I2cNumber                                  = I2cNumber;
  Context->Enumerate.Enumerate                        = Enumerate;
  Context->Enumerate.GetBusFrequency                  = GetBusFrequency;
  Context->BusConfiguration.EnableI2cBusConfiguration = SetConfiguration;
  Context->I2cConDevList                              = AllocateCopyPool (
                                                          sizeof (I2C_CONTROLLER_DEVICE_LIST),
                                                          PcdGetPtr (PcdI2cControllerDevList)
                                                          );


  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gEfiI2cEnumerateProtocolGuid,
                  &Context->Enumerate,
                  &gEfiI2cBusConfigurationManagementProtocolGuid,
                  &Context->BusConfiguration,
                  NULL
                  );

  return Status;
}

/**
  Stops this driver on device pointed to by Handle

  The Stop() function is designed to be invoked from the EFI boot service DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been moved
  into this common boot service. It is legal to call Stop() from other locations,
  but the following calling restrictions must be followed or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous call to this
     same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in this driver's
     Start() function, and the Start() function must have called OpenProtocol() on
     ControllerHandle with an Attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

  @param[in]  This              A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle  A handle to the device being stopped. The handle must
                                support a bus specific I/O protocol for the driver
                                to use to stop the device.
  @param[in]  NumberOfChildren  The number of child device handles in ChildHandleBuffer.
  @param[in]  ChildHandleBuffer An array of child handles to be freed. May be NULL
                                if NumberOfChildren is 0.

  @retval EFI_SUCCESS           The device was stopped.
  @retval EFI_DEVICE_ERROR      The device could not be stopped due to a device error.

**/
EFI_STATUS
EFIAPI
I2cPlatformDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  )
{
  return EFI_SUCCESS;
}

//
//  EFI_DRIVER_BINDING_PROTOCOL instance
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_DRIVER_BINDING_PROTOCOL gI2cPlatformDriverBinding = {
  I2cPlatformDriverBindingSupported,
  I2cPlatformDriverBindingStart,
  I2cPlatformDriverBindingStop,
  I2C_PLATFORM_DRIVER_VERSION,
  NULL,
  NULL
};

/**
  The user entry point for the I2c Platform module. The user code starts with
  this function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
I2cPlatformEntryPoint (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS    Status;
  VOID          *Registration;

  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    I2cConnectEndOfPciEnumerationEvent,
    NULL,
    &Registration
    );

  //
  // Install driver model protocol
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gI2cPlatformDriverBinding,
             ImageHandle,
             NULL,
             NULL
             );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}