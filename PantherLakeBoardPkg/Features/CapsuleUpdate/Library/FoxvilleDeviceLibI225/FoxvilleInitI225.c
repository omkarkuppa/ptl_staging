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
#include <IndustryStandard/Pci.h>
#include <Protocol/FirmwareManagement.h>
#include <Library/FoxvilleDeviceLib.h>
#include <I225.h>

#define MAX_STRING_SIZE  0x1000

const SupportedIDs SupportedDeviceId[] = {
  {0x15F2}, {0x15F3}, {0x3100}, {0x3101}, {0x5502}, {0x0D9F},
  {0x125B}, {0x125C}, {0x125D}, {0x3102}, {0x15FD}, {0x5503},{0}
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
  This function initializes the Hw and PrivateData structs

  @param[in]  HandleCount       - A pointer to the number of handles that were got from LocateHandleBuffer
  @param[in]  Handles           - A pointer to the buffer to return the PciIo protocol handles.

  @retval
  EFI_SUCCESS                   - If the structs were initialized successfully.

**/
EFI_STATUS
InitFoxvilleDeviceHandle (
  IN UINTN        *HandleCount,
  IN EFI_HANDLE   *Handles
 );

/**
  Converts a digit to it's char value.

  @param[in] Digit  - the given digit that needs to convert.
  @retval           The char hex value of the digit.

**/
CHAR16
DigitToHexChar (
  IN UINT8 Digit
 )
{
  return (Digit > 9) ? (Digit + 'A' - 10) : (Digit + '0');
}

/**
  Converts the version number to hex xyzw to x.yz-w string/

  @param[in]  Num       - the decimal version number that was read from the eeprom.
  @param[out] Version   - A pointer to a 7 sized string that gets the version name.

**/
VOID
NumToStringVersion (
  IN UINT16 Num,
  OUT CHAR16 Version[7]
 )
{
  Version[0] = DigitToHexChar ((Num & 0xF000) >> 12);
  Version[1] = '.';
  Version[2] = DigitToHexChar ((Num & 0x0F00) >> 8);
  Version[3] = DigitToHexChar ((Num & 0x00F0) >> 4);
  Version[4] = '-';
  Version[5] = DigitToHexChar (Num & 0x000F);
  Version[6] = '\0';
}

/**
  Checks whether the device ID is supported by the driver

  @param[in] DeviceId   - the Device ID of the evaluated handle

  @retval    TRUE if the device ID is supported, FALSE otherwise.

**/
BOOLEAN
CheckSupportedDeviceId (
  IN UINT16 DeviceId
 )
{
  SupportedIDs* Id;

  for (Id = (SupportedIDs*) SupportedDeviceId; Id->DeviceId; Id++) {
    if (DeviceId == Id->DeviceId) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  This function finds the supported device ID, initializes the hardware
  to firmware update process and initializes PrivateData and hardware structures

  @retval Status         The status for the FMP process.

**/
EFI_STATUS
EFIAPI
FoxvilleDeviceInit (
  VOID
 )
{
  EFI_STATUS          Status;
  EFI_HANDLE*         Handles;
  UINTN               HandleCount;

  DEBUG ((DEBUG_INFO, "Foxville NVM update %d.%d\n", MAJORVERSION, MINORVERSION));

  Status = gBS->LocateHandleBuffer(ByProtocol,
                                    &gEfiPciIoProtocolGuid,
                                    NULL,
                                    &HandleCount,
                                    &Handles);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "LocateHandleBuffer of gEfiPciIoProtocolGuid failed, Status=%r\n", Status));
    return Status;
  }

  PrivateData.Signature = DEV_SIGNATURE;
  Status = InitFoxvilleDeviceHandle (&HandleCount, Handles);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InitFoxvilleDeviceHandle failed, Status=%r\n", Status));
  }

  return Status;
}

/**
  Reads the current Image version from the flash

  @param[out] ImageVersion                - Pointer to be filled by the read version

  @retval     EFI_STATUS                  - Image version was read successfully

**/
EFI_STATUS
IgcGetImageVersion (
  OUT UINT32   *ImageVersion
 )
{
  IGC_HW            *Hw;
  IGC_STATUS        Status;

  Hw = &PrivateData.Hw;
  if (Hw->Back == NULL) {
    DEBUG ((DEBUG_ERROR, "No Foxville device detected\n"));
    return EFI_UNSUPPORTED;
  }

  Status = IgcGetNVMVersion (Hw, (UINT16*) ImageVersion);
  return Status;
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
  This function allows firmware update application to
  validate the firmware image without invoking the SetImage() first.

  @param[in, out]  Image         -     Points to the new image.
  @param[in, out]  ImageSize     -     Size of the new image in bytes.
  @param[out]      ImageUpdatable -    Can image be updated.

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
IgcCheckImage (
  IN  CONST VOID                        *Image,
  IN  UINTN                             ImageSize,
  OUT UINT32                            *ImageUpdatable
 )
{
  UINT16      *ImageToVerify;
  IGC_HW      *Hw;
  UINT16      CurrentNvmVersion;
  IGC_STATUS  Status;

  DEBUG ((DEBUG_INFO, "IgcCheckImage Entry\n"));

  ImageToVerify   = (UINT16*) Image;
  *ImageUpdatable = IMAGE_UPDATABLE_VALID;
  Hw              = &PrivateData.Hw;
  CurrentNvmVersion = 0;

  // Check whether Foxville private data is initialized
  if (Hw->Back == NULL) {
    DEBUG ((DEBUG_ERROR, "Foxville Device is not initialized!\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
    return EFI_DEVICE_ERROR;
  }

  /* Checking for a fit in the flash size and the image size */
  if (ImageSize != PrivateData.Hw.FlashInfo->FlashSize) {
    DEBUG ((DEBUG_ERROR, "ERROR: The image size is different than the flash size %x!\n", PrivateData.Hw.FlashInfo->FlashSize));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
  }
  //blank device ID
  if (ImageToVerify[IGC_DEVICE_ID_WORD] != Hw->DeviceId) {
    DEBUG ((DEBUG_ERROR, "ERROR: The device ID in the image is different than the current device ID\n"));
    *ImageUpdatable = IMAGE_UPDATABLE_INVALID_TYPE;
  }

  Status = IgcGetNVMVersion (Hw, &CurrentNvmVersion);

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
  This function gets all the system handles from system map,
  and searches for a supported device and vendor Ids.

  @param[in]  HandleCount       - A pointer to the number of handles that were got from LocateHandleBuffer
  @param[in]  Handles           - A pointer to the buffer to return the PciIo protocol handles.
  @param[out] PrivateData       - A pointer to the PrivateData instance.
  @param[out] PciConfigHeader   - A pointer to the pci config header to be filled in case of success.

  @retval
  EFI_SUCCESS                   - If a supported PCI device was found.
  EFI_NOT_FOUND                 - If none of the PCI devices are supported by the capsule.

**/
EFI_STATUS
FindFoxvilleDeviceHandle (
  IN UINTN                 *HandleCount,
  IN EFI_HANDLE            *Handles,
  OUT PCI_CONFIG_HEADER    *PciConfigHeader
 )
{
  EFI_STATUS Status;

  for (UINT32 i = 0; i < *HandleCount; i++) {
    Status = gBS->HandleProtocol (
                    Handles[i],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PrivateData.PciIo
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

    PciConfigHeader = (PCI_CONFIG_HEADER*) PrivateData.PciConfig;
    PrivateData.Hw.VendorId = PciConfigHeader->VendorID;
    PrivateData.Hw.DeviceId = PciConfigHeader->DeviceID;
    PrivateData.Hw.HwAddr = (UINT8*) (UINTN) PciConfigHeader->BaseAddressReg_0;

    if (PrivateData.Hw.VendorId != 0x8086)
      continue;

    if (CheckSupportedDeviceId (PrivateData.Hw.DeviceId)) {
      DEBUG ((DEBUG_INFO, "Found supported device ID: 0x%x\n", PrivateData.Hw.DeviceId));
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  This function initializes the Hw and PrivateData structs

  @param[in]  HandleCount       - A pointer to the number of handles that were got from LocateHandleBuffer
  @param[in]  Handles           - A pointer to the buffer to return the PciIo protocol handles.

  @retval
  EFI_SUCCESS                   - If the structs were initialized successfully.

**/
EFI_STATUS
InitFoxvilleDeviceHandle (
  IN  UINTN               *HandleCount,
  IN  EFI_HANDLE          *Handles
 )
{
  EFI_STATUS           Status;
  PCI_CONFIG_HEADER    PciConfigHeader;
  UINT64               Attributes;
  UINT32               Flop;

  Status = FindFoxvilleDeviceHandle (HandleCount, Handles, &PciConfigHeader);

  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "No GBE Supported device was found\n"));
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