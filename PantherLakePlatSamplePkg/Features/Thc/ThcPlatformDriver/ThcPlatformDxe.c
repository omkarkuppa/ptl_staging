/** @file
  Touch Host Controller QuickI2c Platform Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <PiDxe.h>
#include <Uefi/UefiSpec.h>

//
// Libraries
//
//#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Guid/EventGroup.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <SetupVariable.h>
#include <ThcPlatformDxe.h>

#define V_INTEL_VENDOR_ID     0x8086
#define MAX_SUPPORTED_DEVICES 5

/**
  Available modes
**/
typedef enum {
  Thc,        ///< Intel THC protocol
  HidOverSpi, ///< Hid Over SPI protocol
  HidOverI2c,  ///< Hid Over I2C protocol
  ThcModeNone
} THC_MODE;

/**
  Function to check if Device ID belongs to THC Hid Over I2c Controller

  @param[in]  VendorId        Vendor Id that will be compared to THCs VendorId
  @param[in]  DeviceId        Device ID that will be compared to THCs DeviceId
  @param[in]  Index           Device Index

  @retval  TRUE    Device ID belongs to THC
           FALSE   Device ID is not supported

**/
BOOLEAN
STATIC
QuickI2cIsDeviceIdSupported (
  IN  UINT16            VendorId,
  IN  UINT16            DeviceId,
  OUT UINT8             *IdIndex
  )
{
  THC_HID_OVER_I2C_DEVID  *ThcContreollerDevId;
  UINT8                   Index;

  ThcContreollerDevId = NULL;
  *IdIndex            = FALSE;

  if (VendorId != V_INTEL_VENDOR_ID) {
    return FALSE;
  }

  ThcContreollerDevId = (THC_HID_OVER_I2C_DEVID *) PcdGetPtr (PcdThcQuickI2cDevId);
  if (ThcContreollerDevId == NULL || ThcContreollerDevId->Count == 0) {
    DEBUG ((DEBUG_ERROR, "%a: Count is out of range or PcdThcQuickI2cDevId is NULL\n", __FUNCTION__));
    return FALSE;
  }

  if (ThcContreollerDevId->Count > ARRAY_SIZE (ThcContreollerDevId->ThcHidOverI2cDevId)) {
    DEBUG ((DEBUG_ERROR, "%a: Count exceeds array size\n", __FUNCTION__));
    return FALSE;
  }

  for (Index = 0; Index < (ThcContreollerDevId->Count); Index++) {
    if (DeviceId == (ThcContreollerDevId->ThcHidOverI2cDevId[Index].DevId)) {
      *IdIndex = (UINT8) ThcContreollerDevId->ThcHidOverI2cDevId[Index].Index;
      DEBUG ((DEBUG_INFO, "DeviceId matched: 0x%X at Index: %d\n", DeviceId, *IdIndex));
      return TRUE;
    }
  }
  return FALSE;
}


/**
  Callback function when EfiPciEnumerationCompleteProtocol is installed.

  @param[in]  Event                Event whose notification function is being invoked
  @param[in]  Context              Pointer to the notification functions context

**/
VOID
EFIAPI
QuickI2cEnumerateEndOfPciEnumerationEvent (
  IN EFI_EVENT           Event,
  IN VOID                *Context
  )
{
  EFI_STATUS             Status;
  UINTN                  Index;
  UINTN                  PciIoHandleCount;
  EFI_HANDLE             *PciIoHandleBuffer;
  EFI_PCI_IO_PROTOCOL    *PciIo;
  UINT8                  DeviceIndex;
  EFI_HANDLE             Handle;
  UINT32                 VendDevId;
  UINT16                 VendorId;
  UINT16                 DeviceId;
  PCH_SETUP              PchSetup;
  UINTN                  VariableSize;
  UINT32                 PchSetupAttr;
  VOID                   *Protocol;

  PciIoHandleCount  = 0;
  PciIoHandleBuffer = NULL;
  PciIo             = NULL;
  DeviceIndex       = 0;
  VendDevId         = 0;
  VariableSize      = sizeof (PCH_SETUP);
  ZeroMem (&PchSetup, VariableSize);

  DEBUG ((DEBUG_INFO, "%a() \n", __FUNCTION__));

  Status = gRT->GetVariable (
                   L"PchSetup",
                   &gPchSetupVariableGuid,
                   &PchSetupAttr,
                   &VariableSize,
                   &PchSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  DEBUG ((DEBUG_INFO, "%a() ThcAssignment[0]=0x%X, ThcAssignment[1]=0x%X, ThcMode[0]=0x%X, ThcMode[1]=0x%X\n",
          __FUNCTION__, PchSetup.ThcAssignment[0], PchSetup.ThcAssignment[1], PchSetup.ThcMode[0], PchSetup.ThcMode[1]));

  if ((PchSetup.ThcAssignment[0] == 0) ||
     ((PchSetup.ThcAssignment[0] != 0 && PchSetup.ThcMode[0] != HidOverI2c) || // THC Port 0 Enabled not for HidOverI2c mode
      (PchSetup.ThcAssignment[1] != 0 && PchSetup.ThcMode[1] != HidOverI2c))) { // THC Port 1 Enabled not for HidOverI2c mode
    DEBUG ((DEBUG_INFO, "%a() THC Assignment is 0 for both controllers or THC Mode is not HidOverI2c for both controllers, Skipping !!!\n", __FUNCTION__));
    return;
  }

  //
  // Check whether gEfiPciEnumerationCompleteProtocolGuid is installed, if not return
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Protocol
                );
  if (EFI_ERROR(Status)) {
    return;
  }

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
    Handle = PciIoHandleBuffer[Index];
    Status = gBS->OpenProtocol (
                    PciIoHandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo,
                    Handle,
                    Handle,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, 0, 1, &VendDevId);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to read PCI configuration space: %r\n", Status));
      continue;
    }
    VendorId = (UINT16) (VendDevId & 0xFFFF);
    DeviceId = (UINT16) (VendDevId >> 16);
    if (QuickI2cIsDeviceIdSupported (VendorId, DeviceId, &DeviceIndex)) {
      DEBUG ((DEBUG_INFO, "Connecting QuickI2c driver for DeviceId: 0x%X, DeviceIndex: %d\n", DeviceId, DeviceIndex));
      Status = gBS->ConnectController (Handle, NULL, NULL, TRUE);
      if (!EFI_ERROR (Status)) {
        if (PcdGet16 (PcdThc0DeviceSlaveAddress) != 0 || PcdGet16 (PcdThc1DeviceSlaveAddress) != 0) {
          //
          // Update PchSetup variable with new THC HID I2C device addresses
          //
          if (DeviceIndex == 0) {
            PchSetup.ThcHidI2cDeviceAddress[0] = PcdGet16 (PcdThc0DeviceSlaveAddress);
            PchSetup.ThcHidI2cDeviceDescriptorAddress[0] = PcdGet32 (PcdThc0DeviceDescriptorAddress);
            DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceAddress[0]=0x%X, ThcHidI2cDeviceDescriptorAddress[0]=0x%X\n",
              PchSetup.ThcHidI2cDeviceAddress[0], PchSetup.ThcHidI2cDeviceDescriptorAddress[0]));
          } else if (DeviceIndex == 1) {
            PchSetup.ThcHidI2cDeviceAddress[1] = PcdGet16 (PcdThc1DeviceSlaveAddress);
            PchSetup.ThcHidI2cDeviceDescriptorAddress[1] = PcdGet32 (PcdThc1DeviceDescriptorAddress);
            DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceAddress[1]=0x%X, ThcHidI2cDeviceDescriptorAddress[1]=0x%X\n",
              PchSetup.ThcHidI2cDeviceAddress[1], PchSetup.ThcHidI2cDeviceDescriptorAddress[1]));
          }

          Status = gRT->SetVariable (
                          L"PchSetup",
                          &gPchSetupVariableGuid,
                          PchSetupAttr,
                          sizeof (PCH_SETUP),
                          &PchSetup
                        );
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_INFO, "Updated PchSetup variable with new THC HID I2C device addresses status %r\n", Status));
          }
        }
      }
    }
  }
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  gBS->CloseProtocol (
    Handle,
    &gEfiPciIoProtocolGuid,
    Handle,
    Handle
  );
  gBS->FreePool (PciIoHandleBuffer);
}

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
ThcPlatformEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  VOID                  *Registration;

  DEBUG ((DEBUG_INFO, "%a() Entry \n", __FUNCTION__));

  // Register to enumerate when THC controller enumeration completes
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    QuickI2cEnumerateEndOfPciEnumerationEvent,
    NULL,
    &Registration
  );

  return EFI_SUCCESS;
}
