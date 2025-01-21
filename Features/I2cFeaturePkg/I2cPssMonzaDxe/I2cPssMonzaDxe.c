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

#include "I2cPssMonza.h"

///
/// Driver Binding Protocol instance
///
GLOBAL_REMOVE_IF_UNREFERENCED EFI_DRIVER_BINDING_PROTOCOL gPssMonzaDriverBinding = {
  PssMonzaDriverBindingSupported,
  PssMonzaDriverBindingStart,
  PssMonzaDriverBindingStop,
  PSS_MONZA_VERSION,
  NULL,
  NULL
};

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
PssMonzaDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS    Status;

  //
  // Install UEFI Driver Model protocol(s).
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gPssMonzaDriverBinding,
             ImageHandle,
             &gPssMonzaDriverComponentName,
             &gPssMonzaDriverComponentName2
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Looks for controllers which declare the EFI_I2C_IO_PROTOCOL and match the device path
  supplied by the silicon vendor or third party I2C driver writer to the platform integrator.

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
PssMonzaDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS                    Status;
  EFI_I2C_IO_PROTOCOL           *I2cIoProtocol;

  //
  // Open protocol by driver to obtain device guid
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **) &I2cIoProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Compare the known GUID with the GUID pointed to by the DeviceGuid field
  //
  if (CompareGuid (&gI2cPssMonzaDeviceGuid, I2cIoProtocol->DeviceGuid)) {
    DEBUG((DEBUG_INFO, "PSS Monza Supported: DeviceGuid = %g\n", I2cIoProtocol->DeviceGuid));
    Status = gBS->CloseProtocol (ControllerHandle, &gEfiI2cIoProtocolGuid, This->DriverBindingHandle, ControllerHandle);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    return EFI_SUCCESS;
  }

  gBS->CloseProtocol (ControllerHandle, &gEfiI2cIoProtocolGuid, This->DriverBindingHandle, ControllerHandle);
  return EFI_UNSUPPORTED;
}

/**
  Starts PSS Monza chip initialize by I2cIoProtocol.
  If initialization completes successfully, this driver will install gI2cPssProtocolGuid protocol
  on this device. This protocol can be further used to get serial number and MAC address from PSS chip.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support I2C protocol interface.
  @param[in]  RemainingDevicePath  ignored

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failded to start the device.

**/
EFI_STATUS
EFIAPI
PssMonzaDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  PSS_MONZA_PRIVATE_CONTEXT       *PssMonzaContext;
  EFI_I2C_IO_PROTOCOL             *I2cIoProtocol;
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  //
  // Allocate the driver context
  //
  PssMonzaContext = AllocateZeroPool (sizeof (PSS_MONZA_PRIVATE_CONTEXT));
  if (PssMonzaContext == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **) &I2cIoProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    FreePool(PssMonzaContext);
    return Status;
  }


  PssMonzaContext->ControllerHandle                   = ControllerHandle;
  PssMonzaContext->Signature                          = PSS_MONZA_SIGNATURE;
  PssMonzaContext->I2cIoProtocol                      = I2cIoProtocol;
  PssMonzaContext->PssProtocol.Revision               = I2C_PLATFORM_SECURE_STORAGE_PROTOCOL_REVISION;
  PssMonzaContext->PssProtocol.I2cPssGetSerialNumber  = PssMonzaGetSerialNumber;
  PssMonzaContext->PssProtocol.PssGetMacAddress       = PssMonzaGetMacAddress;

  AddUnicodeString2 (
    "eng",
    gPssMonzaDriverComponentName.SupportedLanguages,
    &PssMonzaContext->ControllerNameTable,
    L"I2C PSS Monza Driver",
    TRUE
    );

  AddUnicodeString2 (
    "en",
    gPssMonzaDriverComponentName2.SupportedLanguages,
    &PssMonzaContext->ControllerNameTable,
    L"I2C PSS Monza Driver",
    FALSE
    );

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gI2cPssProtocolGuid,
                  &PssMonzaContext->PssProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto ErrorExit;
  }

  return Status;

ErrorExit:
  DEBUG ((DEBUG_ERROR, "%a start exiting, %r\n", __FUNCTION__, Status));

  if (PssMonzaContext != NULL) {
    FreePool (PssMonzaContext);
  }

  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  return Status;
}

/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been moved
  into this common boot service. It is legal to call Stop() from other locations,
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
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
PssMonzaDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  )
{
  EFI_STATUS                           Status;
  I2C_PLATFORM_SECURE_STORAGE_PROTOCOL *PssProtocol;
  PSS_MONZA_PRIVATE_CONTEXT            *PssMonzaContext;

  //
  // Unload any higher level drivers
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gI2cPssProtocolGuid,
                  (VOID **) &PssProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER | EFI_OPEN_PROTOCOL_EXCLUSIVE
                  );


  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "INFO - PSS Monza driver still in use, Status: %r\n", Status));
    return EFI_SUCCESS;
  }

  //
  // Done with the upper level protocol
  //
  gBS->CloseProtocol (
         ControllerHandle,
         &gI2cPssProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ControllerHandle,
                  &gI2cPssProtocolGuid,
                  &PssProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR - PSS Monza failed to unload upper protocol, Status: %r\n", Status));
    return Status;
  }

  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  PssMonzaContext = PSS_MONZA_CONTEXT_FROM_PROTOCOL (PssProtocol);

  FreeUnicodeStringTable (PssMonzaContext->ControllerNameTable);
  FreePool (PssMonzaContext);

  return Status;
}
