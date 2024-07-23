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

/**
  This function finds the supported device ID, initializes the hardware to
  firmware update process and initializes Foxville core instance and hardware structures

  @retval Status              The status for the FMP process.

**/
EFI_STATUS
EFIAPI
FoxvilleDeviceInit (
  VOID
 );

/**
  Reads the current Image version from the flash

  @param[out] ImageVersion                - Pointer to be filled by the read version

  @retval     EFI_STATUS                  - Image version was read successfully

**/
EFI_STATUS
IgcGetImageVersion (
  OUT UINT32   *ImageVersion
 );

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

#endif /* FOXVILLE_DEVICE_LIB_H */
