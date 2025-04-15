/** @file
  FmpDeviceLib instance to support Foxville Capsule update

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

#ifndef FOXVILLE_DEVICE_LIB_H
#define FOXVILLE_DEVICE_LIB_H

#include <Uefi.h>

#define FOXVILLE_DEVICE_VERSION_STRING_FORMAT  L"#.##-#"
#define FOXVILLE_DEVICE_VERSION_STRING_SIZE    (sizeof (FOXVILLE_DEVICE_VERSION_STRING_FORMAT))

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
  );

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
  );

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
  );

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
  IN  CONST VOID                                       *Image,
  IN  UINTN                                            ImageSize
 );

/**
  Get the string of firmware version.

  @note  This would convert the version from xyzw into x.yz-w in HEX format.

  @param[out]  VersionString  Pointer to the string of version.

  @retval  EFI_SUCCESS  Succeed to get the string of firmware version.
  @retval  Others       Failed to get the string of firmware version.

**/
EFI_STATUS
GetVersionString (
  OUT CHAR16  **VersionString
  );

#endif /* FOXVILLE_DEVICE_LIB_H */
