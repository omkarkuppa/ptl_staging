/** @file
  Terminal Over Debug driver for Redirect Shell/Setup through DCI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "TerminalOverDebug.h"
#include <Defines/PcdPchBdfAssignment.h>

//
//DCI Mailbox Driver Global Variables
//

DCI_MB *gDciM = NULL;

EFI_DRIVER_BINDING_PROTOCOL gDciControllerDriver = {
  DciControllerDriverSupported,
  DciControllerDriverStart,
  DciControllerDriverStop,
  0xa,
  NULL,
  NULL
};

DCI_MB_PATH  mDciMailboxPath = {
  {
    MESSAGING_DEVICE_PATH,
    MSG_DCI_MB_DP,
    {
      (UINT8) (sizeof (DCI_MB_PATH)),
      (UINT8) ((sizeof (DCI_MB_PATH)) >> 8)
    }
  },
  0,
};

/**
  Check to see if this driver supports the given controller

  @param  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param  Controller           The handle of the controller to test.
  @param  RemainingDevicePath  A pointer to the remaining portion of a device path.

  @return EFI_SUCCESS          This driver can support the given controller

**/
EFI_STATUS
EFIAPI
DciControllerDriverSupported(
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  ) {
  EFI_STATUS                        Status = EFI_UNSUPPORTED;
  EFI_DEVICE_PATH_PROTOCOL          *ParentDevicePath;
  EFI_PCI_IO_PROTOCOL               *PciIo;
  PCI_TYPE00                        Pci;
  UINTN                             SegmentNumber;
  UINTN                             BusNumber;
  UINTN                             DeviceNumber;
  UINTN                             FunctionNumber;
  BOOLEAN                           msgdevpath= FALSE;

  //
  // Check for ExIDci MailBox Path in the RemainingDevicePath.
  //
  if ((RemainingDevicePath != NULL) && !IsDevicePathEnd (RemainingDevicePath)) {
    if (DevicePathType (RemainingDevicePath) == MESSAGING_DEVICE_PATH &&
        DevicePathSubType (RemainingDevicePath) == MSG_DCI_MB_DP) {
          msgdevpath= TRUE;
        }
  }

  if (!msgdevpath) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0,
                          sizeof (Pci) / sizeof (UINT32),
                          &Pci
                          );
    ASSERT_EFI_ERROR (Status);
    Status = PciIo->GetLocation (
                            PciIo,
                            &SegmentNumber,
                            &BusNumber,
                            &DeviceNumber,
                            &FunctionNumber
                            );
    ASSERT_EFI_ERROR (Status);
  } else {
    return EFI_UNSUPPORTED;
  }
  gBS->CloseProtocol (
           Controller,
           &gEfiPciIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );
  if ((DeviceNumber != PCI_DEVICE_NUMBER_PCH_XHCI) ||
      (FunctionNumber != PCI_FUNCTION_NUMBER_PCH_XHCI) ||
      (Pci.Hdr.ClassCode [2] != PCI_CLASS_SERIAL) ||
      (Pci.Hdr.ClassCode [1] != PCI_CLASS_SERIAL_USB) ||
      (Pci.Hdr.ClassCode [0] != PCI_IF_XHCI)) {
        return EFI_UNSUPPORTED;
  }
  //
  // Open the EFI Device Path protocol needed to perform the supported test
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Close protocol, don't use device path protocol in the Support() function
  //
  gBS->CloseProtocol (
        Controller,
        &gEfiDevicePathProtocolGuid,
        This->DriverBindingHandle,
        Controller
        );

  return Status;
}

/**
  Start this driver on ControllerHandle.

  @param  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param  Controller           The handle of the controller to test.
  @param  RemainingDevicePath  A pointer to the remaining portion of a device path.

  @return EFI_SUCCESS   Driver is started successfully
**/
EFI_STATUS
EFIAPI
DciControllerDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  )
{
  EFI_STATUS                         Status = EFI_SUCCESS;
  EFI_DEVICE_PATH_PROTOCOL           *ParentDevicePath;

  //
  // Get the Parent Device Path
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &ParentDevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  gDciM = AllocateZeroPool (sizeof (DCI_MB));
  if (gDciM == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gDciM->Signature = DCI_MB_SIGNATURE;
  gDciM->Handle = NULL;
  gDciM->DciIo.Revision = DCI_IO_INTERFACE_REVISION;
  gDciM->DciIo.Reset = DciMbReset;
  gDciM->DciIo.SetAttributes = DciMbSetAttributes;
  gDciM->DciIo.SetControl = DciMbSetControl;
  gDciM->DciIo.GetControl = DciMbGetControl;
  gDciM->DciIo.Read = DciMbRead;
  gDciM->DciIo.Write = DciMbWrite;

  gDciM->DciIo.Mode = &(gDciM->DciMode);

  gDciM->ParentDevicePath = ParentDevicePath;
  gDciM->DciMailboxPath = mDciMailboxPath;


  gDciM->DevicePath = AppendDevicePathNode (
                                 ParentDevicePath,
                                 (EFI_DEVICE_PATH_PROTOCOL *) &gDciM->DciMailboxPath
                                 );
  if (gDciM->DevicePath == NULL) {
    Status = EFI_DEVICE_ERROR;
    goto Error;
  }
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &gDciM->Handle,
                  &gEfiDevicePathProtocolGuid, gDciM->DevicePath,
                  &gEfiSerialIoProtocolGuid, &gDciM->DciIo,
                  NULL
                  );
  DEBUG((DEBUG_INFO, "TerminalOverDebug: Installing DevicePath and SerialIO Protocol %r\n", Status));
  if (EFI_ERROR (Status)) {
    goto Error;
  }
  return Status;
  Error:
    if (EFI_ERROR (Status)) {
      gBS->CloseProtocol (
            Controller,
            &gEfiDevicePathProtocolGuid,
            This->DriverBindingHandle,
            Controller
            );
      if (gDciM != NULL) {
        FreePool (gDciM);
      }
    }
    return Status;
}

/**
  Disconnect this driver with the controller, uninstall related protocol instance

  @param  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param  Controller            The handle of the controller to test.
  @param  NumberOfChildren      Number of child device.
  @param  ChildHandleBuffer     A pointer to the remaining portion of a device path.

  @retval EFI_SUCCESS           Operation successfully
  @retval EFI_DEVICE_ERROR      Cannot stop the driver successfully

**/
EFI_STATUS
EFIAPI
DciControllerDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  )

{
  EFI_STATUS                          Status;
  EFI_DCI_IO_PROTOCOL                 *DciIo;

  gBS->CloseProtocol (
           Controller,
           &gEfiDevicePathProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );

  Status = gBS->OpenProtocol (
                    Controller,
                    &gEfiSerialIoProtocolGuid,
                    (VOID **) &DciIo,
                    This->DriverBindingHandle,
                    Controller,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
  if (!EFI_ERROR (Status)) {
    gBS->UninstallMultipleProtocolInterfaces (
                        Controller,
                        &gEfiDevicePathProtocolGuid, gDciM->DevicePath,
                        &gEfiSerialIoProtocolGuid,&gDciM->DciIo,
                        NULL
                        );
  }

  gBS->CloseProtocol (
           Controller,
           &gEfiSerialIoProtocolGuid,
           This->DriverBindingHandle,
           Controller
           );

  if (gDciM != NULL) {
    FreePool(gDciM);
  }

  return EFI_SUCCESS;
}

/**
  The user Entry Point for Terminal Over Debug Driver. The user code starts with this function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
TerminalOverDebugDriverEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;

  //
  // Install driver model protocol(s).
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gDciControllerDriver,
             ImageHandle,
             NULL,
             NULL
             );

  ASSERT_EFI_ERROR (Status);

  return Status;
}