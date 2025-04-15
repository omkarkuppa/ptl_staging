/** @file
  This is the DXE driver that supports Cnv Compatibilty Driver.

#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.

@par Specification Reference:
**/
#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleFileSystem.h>


EFI_EVENT  SimpleFileEvent;
VOID       *Registration;
EFI_HANDLE NewImageHandle;

BOOLEAN
IsUsbDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL *Node;

  for (Node = DevicePath; !IsDevicePathEnd(Node); Node = NextDevicePathNode(Node)) {
    if ((DevicePathType(Node) == MESSAGING_DEVICE_PATH) &&
        ((DevicePathSubType(Node) == MSG_USB_DP) || (DevicePathSubType(Node) == MSG_USB_CLASS_DP))) {
      DEBUG((DEBUG_INFO,"IsUsbDevicePath found\n"));
      return TRUE;
    }
  }
  DEBUG((DEBUG_INFO,"IsUsbDevicePath Not found\n"));
  return FALSE;
}


VOID
EFIAPI
SimpleFileCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                Status;
  UINTN                     HandleCount;
  EFI_HANDLE                *HandleBuffer;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     Index;
  EFI_BLOCK_IO_PROTOCOL     *BlkIo;

  DEBUG ((DEBUG_INFO, "SimpleFileCallback Entry \n"));

  // Locate all handles that have the Block I/O protocol
  Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &HandleCount, &HandleBuffer);
  if (EFI_ERROR(Status)) {
    return;
  }
  DEBUG ((DEBUG_INFO, "Locate SFS Protocol. - Status=%r NumSimpleFsHandles = %d\n", Status, HandleCount));
  for (Index = 0; Index < HandleCount; Index++) {
    // Get the device path for each handle
    Status = gBS->HandleProtocol(HandleBuffer[Index], &gEfiBlockIoProtocolGuid, (VOID**)&BlkIo);
    if (EFI_ERROR(Status)) {
      continue;
    }

    DevicePath   = DevicePathFromHandle (HandleBuffer[Index]);
    // Check if the device path corresponds to a USB device
    if (IsUsbDevicePath(DevicePath)) {
        NewImageHandle = NULL;
        Status = gBS->InstallProtocolInterface (
                          &NewImageHandle,
                          &gCnvSupportingProtocolGuid,
                          EFI_NATIVE_INTERFACE,
                          NULL
                          );
        ASSERT_EFI_ERROR (Status);
      gBS->CloseEvent(SimpleFileEvent);
    }
  }

  if (HandleBuffer != NULL) {
    FreePool(HandleBuffer);
  }
}

/**
  Cnv Support Entry Point

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver

**/
EFI_STATUS
EFIAPI
CnvSupportDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  // Create an event that is triggered when EFI_SIMPLE_FILE_SYSTEM_PROTOCOL is installed
  Status = gBS->CreateEvent(
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SimpleFileCallback,
                  NULL,
                  &SimpleFileEvent
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Register the event to be notified when EFI_SIMPLE_FILE_SYSTEM_PROTOCOL is installed
  Status = gBS->RegisterProtocolNotify(
                  &gEfiSimpleFileSystemProtocolGuid,
                  SimpleFileEvent,
                  &Registration
                  );
  if (EFI_ERROR(Status)) {
    gBS->CloseEvent(SimpleFileEvent);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
