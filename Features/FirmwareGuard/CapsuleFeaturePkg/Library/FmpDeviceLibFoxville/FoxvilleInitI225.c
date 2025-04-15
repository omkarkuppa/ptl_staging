/** @file
  FoxvilleDeviceLib instance to support Foxville Capsule update

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

  @par Specification Reference:
  Foxville Ethernet Controller External Architecture Specification, version 0.99

**/

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/FirmwareManagement.h>
#include "FoxvilleDeviceLib.h"
#include "FoxvilleNvmMap.h"
#include <I225.h>

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
  DEVICE_ID_I226_LMVP
};

FOXVILLE_CORE_INSTANCE PrivateData = {
  DEV_SIGNATURE,                                                  // Signature
  NULL,                                                           // PciIo
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },              // PciConfig
  {
    NULL,                                                         // Back
    NULL,                                                         // HwAddr
    NULL,                                                         // FlashAddr
    0,                                                            // VendorId
    0,                                                            // DeviceId
    FALSE,                                                        // clear_semaphore_once
    NULL,                                                         // FlashInfo
    0                                                             // FlashBankSize
  }
};

/**
  Initial the module private data structure.

  @param[in]   HandleCount   Pointer to the number of handles returned in buffer.
  @param[in]   Handles       Pointer to the buffer to return the requested
                             array of handles that support Protocol.
  @param[out]  DeviceHandle  Pointer to the handle of Foxville device instance.

  @retval  EFI_SUCCESS  Succeed to initial the private data structure.
  @retval  Others       Failed to initial the private data structure.

**/
EFI_STATUS
InitFoxvilleDeviceHandle (
  IN     UINTN       *HandleCount,
  IN     EFI_HANDLE  *Handles,
     OUT EFI_HANDLE  *DeviceHandle
  );

/**
  Get the string of firmware version.

  @note  This would convert the version from xyzw into x.yz-w in HEX.

  @param[out]  VersionString  Pointer to the string of version.

  @retval  EFI_SUCCESS  Succeed to get the string of firmware version.
  @retval  Others       Failed to get the string of firmware version.

**/
EFI_STATUS
GetVersionString (
  OUT CHAR16  **VersionString
  )
{
  EFI_STATUS                    Status;
  UINT32                        FwVersion;
  FOXVILLE_DEV_STARTER_VERSION  NvmVersion;

  FwVersion         = 0;
  NvmVersion.Uint16 = 0;

  Status = IgcGetImageVersion (&FwVersion);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  NvmVersion.Uint16 = (UINT16)(FwVersion & MAX_UINT16);

  *VersionString = (CHAR16 *)AllocateZeroPool (FOXVILLE_DEVICE_VERSION_STRING_SIZE);
  if (*VersionString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UnicodeSPrint (
    *VersionString,
    FOXVILLE_DEVICE_VERSION_STRING_SIZE,
    L"%X.%X%X-%X",
    (UINT8)((NvmVersion.Bits.UpperByte & 0xF0) >> 4),
    (UINT8)(NvmVersion.Bits.UpperByte & 0x0F),
    (UINT8)((NvmVersion.Bits.LowerByte & 0xF0) >> 4),
    (UINT8)(NvmVersion.Bits.LowerByte & 0x0F)
    );

  return EFI_SUCCESS;
}

/**
  Checks whether the device ID is supported by the driver.

  @param[in]  DeviceId  The Device ID of the evaluated handle.

  @retval  FALSE  The device ID is not supported.
  @retval  TRUE   The device ID is supported.

**/
BOOLEAN
IsSupportedDeviceId (
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
  Initial the Foxville device FMP.

  This function would do the actions below,
    (1) Find the supported device ID.
    (2) Initialize the hardware to firmware update process.
    (3) Initialize Foxville core instance and hardware structures.

  @param[out]  DeviceHandle  Pointer to the handle of Foxville device instance.

  @retval  EFI_SUCCESS  Succeed to initial the Foxville device for FMP.
  @retval  Others       Failed to initial the Foxville device for FMP.

**/
EFI_STATUS
EFIAPI
FoxvilleDeviceInit (
  OUT EFI_HANDLE  *DeviceHandle
  )
{
  EFI_STATUS          Status;
  EFI_HANDLE          *Handles;
  UINTN               HandleCount;

  DEBUG ((DEBUG_INFO, "Foxville NVM update %d.%d\n", MAJORVERSION, MINORVERSION));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "LocateHandleBuffer of gEfiPciIoProtocolGuid failed, Status = %r\n", Status));
    return Status;
  }

  PrivateData.Signature = DEV_SIGNATURE;
  Status = InitFoxvilleDeviceHandle (&HandleCount, Handles, DeviceHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InitFoxvilleDeviceHandle failed, Status=%r\n", Status));
  }

  return Status;
}

/**
  Get the current device firmware version.

  @param[out]  ImageVersion  Pointer to be filled by the NVM version from flash.

  @retval  EFI_SUCCESS       Succeed to get the device firmware version.
  @retval  EFI_DEVICE_ERROR  Failed to get the device firmware version.
  @retval  EFI_UNSUPPORTED   There do not have any supported device.

**/
EFI_STATUS
IgcGetImageVersion (
  OUT UINT32  *ImageVersion
  )
{
  IGC_STATUS                    Status;
  IGC_HW                        *Hw;
  FOXVILLE_DEV_STARTER_VERSION  NvmVersion;

  NvmVersion.Uint16 = 0;

  Hw = &PrivateData.Hw;
  if (Hw->Back == NULL) {
    DEBUG ((DEBUG_ERROR, "No Foxville device detected\n"));
    return EFI_UNSUPPORTED;
  }

  Status = IgcGetNvmVersion (Hw, &NvmVersion);
  if (Status != IGC_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }

  *ImageVersion = (UINT32)NvmVersion.Uint16;

  return EFI_SUCCESS;
}

/**
  Updates the firmware image of the device.
  This function updates the hardware with the new firmware image.
  This function returns EFI_UNSUPPORTED if the firmware image is not
  updatable.

  @param[in]  Image         -     Points to the new image.
  @param[in]  ImageSize     -     Size of the new image in bytes.

  @retval
  EFI_SUCCESS             -       The device was successfully updated with
                                  the new image.
  EFI_ABORTED             -       The operation is aborted.
  EFI_INVALID_PARAMETER   -       The Image was NULL.
  EFI_UNSUPPORTED         -       The operation is not supported.
  EFI_SECURITY_VIOLATION  -       The operation could not be performed due
                                  to an authentication failure.

**/
EFI_STATUS
EFIAPI
IgcSetImage (
  IN  CONST VOID             *Image,
  IN  UINTN                  ImageSize
 )
{
  IGC_STATUS      IgcStatus;
  EFI_STATUS      Status;
  UINT8           *ImageCopy;

  DEBUG ((DEBUG_INFO, "Flashing new NVM image\n"));

  ImageCopy = (UINT8*) AllocateZeroPool (ImageSize);
  if (ImageCopy == NULL) {
    DEBUG ((DEBUG_ERROR, "Couldn't allocate memory for ImageCopy\n"));
    Status = EFI_ABORTED;
    goto IgcSetImageExit;
  }

  CopyMem (ImageCopy, Image, ImageSize);
  if (PrivateData.Hw.DeviceId == 0x15FD){
    DEBUG((DEBUG_INFO, "Flashing over blank flash, please turn off the power after the update\n"));
    IgcStatus = WriteFlashRegion (&PrivateData.Hw, ImageCopy, (UINT32) ImageSize, 0);
    if (EFI_ERROR (IgcStatus)) {
      DEBUG((DEBUG_ERROR, "Flashing NVM FAILED\n"));
      Status = EFI_ABORTED;
      goto IgcSetImageExit;
    }
  } else {
    IgcStatus = WriteFlashImage (&PrivateData.Hw, ImageCopy, ImageSize, FALSE);
    if (EFI_ERROR (IgcStatus)) {
      DEBUG ((DEBUG_ERROR, "Flashing NVM FAILED\n"));
      Status = EFI_ABORTED;
      goto IgcSetImageExit;
    }
    /* reload to GPHY FW to complete the update */
    IgcStatus = IgcResetPHYFW (&PrivateData.Hw);
    if (EFI_ERROR (IgcStatus)) {
      DEBUG((DEBUG_ERROR, "Couldn't reload GPHY Firmware\n"));
      Status = EFI_ABORTED;
      goto IgcSetImageExit;
    }
  }

  DEBUG((DEBUG_INFO, "NVM image was updated successfully\n"));
  Status = EFI_SUCCESS;

IgcSetImageExit:
  if (ImageCopy != NULL) {
    FreePool (ImageCopy);
  }
  return Status;
}

/**
  Checks if the firmware image is valid for the device.

  @param[in, out]  Image           Points to the new image.
  @param[in, out]  ImageSize       Size of the new image in bytes.
  @param[out]      ImageUpdatable  Indicate the new image update ability.

  @retval  EFI_SUCCESS  Succeed to check the image to be updated.
  @retval  Others       Failed to check image to be updated.

**/
EFI_STATUS
EFIAPI
IgcCheckImage (
  IN     CONST VOID    *Image,
  IN           UINTN   ImageSize,
     OUT       UINT32  *ImageUpdatable
  )
{
  EFI_STATUS  Status;
  UINT16      *ImageToVerify;
  UINT32      CurrentNvmVersion;
  IGC_HW      *Hw;

  *ImageUpdatable   = IMAGE_UPDATABLE_VALID;
  ImageToVerify     = (UINT16 *)Image;
  CurrentNvmVersion = 0;
  Hw                = &PrivateData.Hw;

  DEBUG ((DEBUG_INFO, "IgcCheckImage Entry\n"));

  // Check whether Foxville private data is initialized.
  if (Hw->Back == NULL) {
    DEBUG ((DEBUG_ERROR, "Foxville Device is not initialized!\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    return EFI_DEVICE_ERROR;
  }

  // Checking for a fit in the flash size and the image size.
  if (ImageSize != PrivateData.Hw.FlashInfo->FlashSize) {
    DEBUG ((DEBUG_ERROR, "ERROR: The image size is different than the flash size %x!\n", PrivateData.Hw.FlashInfo->FlashSize));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
  }
  // Blank device ID.
  if (ImageToVerify[IGC_DEVICE_ID_WORD] != Hw->DeviceId) {
    DEBUG ((DEBUG_ERROR, "ERROR: The device ID in the image is different than the current device ID\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID_TYPE;
  }

  Status = IgcGetImageVersion (&CurrentNvmVersion);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR: Unable to get NVM version!\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    return EFI_ABORTED;
  }

  if (ImageToVerify[IGC_DEV_STARTER_WORD] < CurrentNvmVersion) {
    DEBUG((DEBUG_ERROR, "ERROR: Current version is newer than the one in the image, rollback is not supported\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID_OLD;
  }
  return EFI_SUCCESS;
}

/**
  Search a supported device and vendor ID from all the system handles.

  @param[in]   HandleCount   Pointer to the number of handles returned in buffer.
  @param[in]   Handles       Pointer to the buffer to return the requested
                             array of handles that support Protocol.
  @param[out]  DeviceHandle  Pointer to the handle of Foxville device instance.

  @retval  EFI_SUCCESS    Succeed to find a supported PCI device.
  @retval  EFI_NOT_FOUND  Failed to find a supported PCI device.

**/
EFI_STATUS
FindFoxvilleDeviceHandle (
  IN     UINTN       *HandleCount,
  IN     EFI_HANDLE  *Handles,
     OUT EFI_HANDLE  *DeviceHandle
  )
{
  EFI_STATUS  Status;
  PCI_TYPE00  *PciConfigHeader;

  for (UINT32 i = 0; i < *HandleCount; i++) {
    Status = gBS->HandleProtocol (
                    Handles[i],
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PrivateData.PciIo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HandleProtocol of gEfiPciIoProtocolGuid failed, Status=%r\n", Status));
      continue;
    }

    Status = PrivateData.PciIo->Pci.Read (
                                  PrivateData.PciIo,
                                  EfiPciIoWidthUint32,
                                  0,
                                  MAX_PCI_CONFIG_LEN,
                                  PrivateData.PciConfig
                                  );
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "Pci.Read failed, Status=%r\n", Status));
      continue;
    }

    //
    // Fill the PCI configuration in private data.
    //
    PciConfigHeader = (PCI_TYPE00 *)PrivateData.PciConfig;
    if ((PciConfigHeader->Hdr.HeaderType & HEADER_LAYOUT_CODE) != HEADER_TYPE_DEVICE) {
      DEBUG ((DEBUG_INFO, "Unexpected PCI header type - [0x%X]\n", PciConfigHeader->Hdr.HeaderType));
      continue;
    }

    PrivateData.Hw.VendorId = PciConfigHeader->Hdr.VendorId;
    PrivateData.Hw.DeviceId = PciConfigHeader->Hdr.DeviceId;
    PrivateData.Hw.HwAddr   = (UINT8 *)(UINTN)PciConfigHeader->Device.Bar[0];
    if (PrivateData.Hw.VendorId != VENDOR_ID_INTEL) {
      DEBUG ((DEBUG_INFO, "Unexpected vendor ID - [0x%04X]\n", PrivateData.Hw.VendorId));
      continue;
    }

    if (IsSupportedDeviceId (PrivateData.Hw.DeviceId)) {
      DEBUG ((DEBUG_INFO, "Found supported device ID: 0x%x\n", PrivateData.Hw.DeviceId));
      *DeviceHandle = Handles[i];
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Initial the module private data structure.

  @param[in]   HandleCount   Pointer to the number of handles returned in buffer.
  @param[in]   Handles       Pointer to the buffer to return the requested
                             array of handles that support Protocol.
  @param[out]  DeviceHandle  Pointer to the handle of Foxville device instance.

  @retval  EFI_SUCCESS  Succeed to initial the private data structure.
  @retval  Others       Failed to initial the private data structure.

**/
EFI_STATUS
InitFoxvilleDeviceHandle (
  IN     UINTN       *HandleCount,
  IN     EFI_HANDLE  *Handles,
     OUT EFI_HANDLE  *DeviceHandle
  )
{
  EFI_STATUS  Status;
  UINT64      Attributes;
  UINT32      Flop;

  Status = FindFoxvilleDeviceHandle (HandleCount, Handles, DeviceHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "No GBE Supported device was found\n"));
    return Status;
  }

  PrivateData.Hw.Back = &PrivateData;

  /* Get the PCI Command options that are supported by this controller */
  Status = PrivateData.PciIo->Attributes (
                                PrivateData.PciIo,
                                EfiPciIoAttributeOperationSupported,
                                0,
                                &Attributes
                                );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Attributes get failed, Status=%r\n", Status));
    return Status;
  }

  /*
    Set the PCI Command options to enable device memory mapped IO,
    port IO, and bus mastering.
  */
  Status = PrivateData.PciIo->Attributes (
                                PrivateData.PciIo,
                                EfiPciIoAttributeOperationEnable,
                                Attributes & (EFI_PCI_DEVICE_ENABLE | EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE),
                                NULL
                                );

  PrivateData.Hw.FlashInfo->FlashSize = GetFlashSize (&PrivateData.Hw);
  PrivateData.Hw.FlashInfo->FlashMode = GetFlashProgrammingMode (&PrivateData.Hw);
  DEBUG ((DEBUG_INFO, "Init: Flash Mode = %d\n", PrivateData.Hw.FlashInfo->FlashMode));
  Flop = ((IGC_S_SECT_ERASE << 8) | IGC_CMD_ERASE);
  WriteMacReg (&PrivateData.Hw, IGC_FLASHOP, Flop);

  return Status;
}
