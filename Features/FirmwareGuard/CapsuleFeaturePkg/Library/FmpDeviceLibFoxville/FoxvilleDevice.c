/** @file
  Internal file to support Foxville device.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleCommon.h"
#include "Defines/FoxvilleNvmMap.h"
#include "FoxvilleApi.h"
#include "FoxvilleDevice.h"

//
// Module Variables.
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST DEVICE_ID  mSupportedDeviceId[] = {
  //
  // I225 series.
  //
  DEVICE_ID_I225_LM,
  DEVICE_ID_I225_V,
  DEVICE_ID_I225_K,
  DEVICE_ID_I225_K2,
  DEVICE_ID_I225_LMVP,
  DEVICE_ID_I225_IT,
  DEVICE_ID_I225_BLANK_NVM,
  //
  // I226 series.
  //
  DEVICE_ID_I226_LM,
  DEVICE_ID_I226_V,
  DEVICE_ID_I226_IT,
  DEVICE_ID_I226_K,
  DEVICE_ID_I226_LMVP,
  DEVICE_ID_I226_BLANK_NVM
};

/**
  Checks if the device ID is supported.

  @param[in]  DeviceId  The device ID to be evaluated.

  @retval  FALSE  The device ID is not supported.
  @retval  TRUE   The device ID is supported.

**/
BOOLEAN
IsDeviceIdSupported (
  IN UINT16  DeviceId
  )
{
  UINTN  Idx;

  for (Idx = 0; Idx < (sizeof (mSupportedDeviceId) / sizeof (DEVICE_ID)); Idx++) {
    if (DeviceId == mSupportedDeviceId[Idx]) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get the vendor ID from PCI configuration space.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  VendorIdPtr       Pointer to return the vendor ID.

  @retval  EFI_SUCCESS            Succeed to get the vendor ID.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the vendor ID.

**/
EFI_STATUS
GetPciVendorId (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT16               *VendorIdPtr
  )
{
  EFI_STATUS  Status;
  UINT16      Value;

  if ((PciIoProtocolPtr == NULL) || (VendorIdPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PciIoProtocolPtr->Pci.Read (
                                   PciIoProtocolPtr,
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   sizeof (Value),
                                   (VOID *)&Value
                                   );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *VendorIdPtr = Value;

  return EFI_SUCCESS;
}

/**
  Get the device ID from PCI configuration space.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  VendorIdPtr       Pointer to return the device ID.

  @retval  EFI_SUCCESS            Succeed to get the device ID.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the device ID.

**/
EFI_STATUS
GetPciDeviceId (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT16               *DeviceIdPtr
  )
{
  EFI_STATUS  Status;
  UINT16      Value;

  if ((PciIoProtocolPtr == NULL) || (DeviceIdPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PciIoProtocolPtr->Pci.Read (
                                   PciIoProtocolPtr,
                                   EfiPciIoWidthUint16,
                                   PCI_DEVICE_ID_OFFSET,
                                   sizeof (Value),
                                   (VOID *)&Value
                                   );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *DeviceIdPtr = Value;

  return EFI_SUCCESS;
}

/**
  Check if the device handle is supported.

  @param[in]  DeviceHandlePtr  Pointer to the device handle.

  @retval  FALSE  Device handle is not supported.
  @retval  TRUE   Device handle is supported.

**/
BOOLEAN
EFIAPI
IsDeviceHandleSupported (
  IN EFI_HANDLE  *DeviceHandlePtr
  )
{
  EFI_STATUS           Status;
  EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr;
  PCI_TYPE00           PciConfig;

  PciIoProtocolPtr = NULL;

  if (DeviceHandlePtr == NULL) {
    return FALSE;
  }

  //
  // Locate the PCI IO protocol from the input handle.
  //
  Status = gBS->HandleProtocol (
                  DeviceHandlePtr,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&PciIoProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  //
  // Read the PCI configuration space.
  //
  Status = PciIoProtocolPtr->Pci.Read (
                                   PciIoProtocolPtr,
                                   EfiPciIoWidthUint8,
                                   0,
                                   sizeof (PciConfig),
                                   (VOID *)&PciConfig
                                   );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (PciConfig.Hdr.VendorId != VENDOR_ID_INTEL) {
    return FALSE;
  }

  if (!IsDeviceIdSupported (PciConfig.Hdr.DeviceId)) {
    return FALSE;
  }

  return TRUE;
}

/**
  Get the supported Foxville device handle.

  @param[out]  DeviceHandlePtr  Pointer to the device handle.

  @retval  EFI_SUCCESS            Succeed to get the supported Foxville device handle.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          Not found the supported Foxville device handle.
  @retval  Others                 Failed to get the supported Foxville device handle.

**/
EFI_STATUS
EFIAPI
GetFoxvilleDeviceHandle (
  OUT EFI_HANDLE  *DeviceHandlePtr
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *HandlesPtr;
  UINTN       HandleCount;
  UINTN       Index;

  HandlesPtr  = NULL;
  HandleCount = 0;
  Index       = 0;

  if (DeviceHandlePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Locate all the handles with PCI IO protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandlesPtr
                  );
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Find the supported device handle.
  //
  Status = EFI_NOT_FOUND;
  for (Index = 0; Index < HandleCount; Index++) {
    if (!IsDeviceHandleSupported (HandlesPtr[Index])) {
      continue;
    }

    Status           = EFI_SUCCESS;
    *DeviceHandlePtr = HandlesPtr[Index];
    break;
  }

Exit:
  if (HandlesPtr != NULL) {
    FreePool (HandlesPtr);
  }

  return Status;
}

/**
  Check if the device ID is blank flash.

  @note If the device owned the blank flash, there would not have valid NVM.
        The device ID would be changed after the update.

  @param[in]  DeviceId  The device ID to be evaluated.

  @retval  FALSE  The device ID is not indicated as blank flash.
  @retval  TRUE   The device ID is indicated as blank flash.

**/
BOOLEAN
EFIAPI
IsBlankFlashDeviceId (
  IN UINT16  DeviceId
  )
{
  BOOLEAN  IsBlank;

  IsBlank = FALSE;

  switch (DeviceId) {
    case DEVICE_ID_I225_BLANK_NVM:
    case DEVICE_ID_I226_BLANK_NVM:
      IsBlank = TRUE;
      break;
    default:
      IsBlank = FALSE;
  }

  return IsBlank;
}

/**
  Set the required Foxville PCI attributes.

  @note This function would set the attributes below,
          (1) Enable device MMIO.
          (2) Enable device port IO.
          (3) Enable device bus mastering.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to set the PCI attributes.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set the PCI attributes.

**/
EFI_STATUS
EFIAPI
SetFoxvillePciAttributes (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS  Status;
  UINT64      Attributes;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PciIoProtocolPtr->Attributes (
                               PciIoProtocolPtr,
                               EfiPciIoAttributeOperationSupported,
                               0,
                               &Attributes
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get the PCI attributes - %r\n", Status));
    return EFI_DEVICE_ERROR;
  }

  Attributes = Attributes & (EFI_PCI_DEVICE_ENABLE | EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE);
  Status     = PciIoProtocolPtr->Attributes (
                                   PciIoProtocolPtr,
                                   EfiPciIoAttributeOperationEnable,
                                   Attributes,
                                   NULL
                                   );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set the PCI attributes - %r\n", Status));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Initial the Foxville device information.

  @param[in]   PciIoProtocolPtr       Pointer to the PCI IO protocol instance.
  @param[out]  FoxvilleDeviceInfoPtr  Pointer to the Foxville device information.

  @retval  EFI_SUCCESS            Succeed to initialize the Foxville device information.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to initialize the Foxville device information.

**/
EFI_STATUS
EFIAPI
InitFoxvilleDeviceInfo (
  IN     EFI_PCI_IO_PROTOCOL   *PciIoProtocolPtr,
     OUT FOXVILLE_DEVICE_INFO  *FoxvilleDeviceInfoPtr
  )
{
  EFI_STATUS           Status;
  UINT16               VendorId;
  UINT16               DeviceId;
  UINT16               NvmVersion;
  FOXVILLE_FLASH_MODE  FlashMode;
  UINT32               FlashSize;

  if ((PciIoProtocolPtr == NULL) || (FoxvilleDeviceInfoPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetPciVendorId (PciIoProtocolPtr, &VendorId);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = GetPciDeviceId (PciIoProtocolPtr, &DeviceId);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = GetNvmVersionFromFlash (PciIoProtocolPtr, &NvmVersion);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = GetFlashMode (PciIoProtocolPtr, &FlashMode);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = GetFlashSize (PciIoProtocolPtr, &FlashSize);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Fill in the device information.
  //
  FoxvilleDeviceInfoPtr->IsValid    = TRUE;
  FoxvilleDeviceInfoPtr->VendorId   = VendorId;
  FoxvilleDeviceInfoPtr->DeviceId   = DeviceId;
  FoxvilleDeviceInfoPtr->NvmVersion = NvmVersion;
  FoxvilleDeviceInfoPtr->FlashMode  = FlashMode;
  FoxvilleDeviceInfoPtr->FlashSize  = FlashSize;

  return EFI_SUCCESS;
}
