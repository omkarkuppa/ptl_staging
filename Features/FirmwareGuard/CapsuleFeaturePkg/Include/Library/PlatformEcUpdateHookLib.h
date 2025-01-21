/** @file
  Platform hook to get EC information on platform or perfrom platform specific actions.

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

@par Specification Reference:

**/

#ifndef __PLATFORM_EC_UPDATE_HOOK_LIB_H__
#define __PLATFORM_EC_UPDATE_HOOK_LIB_H__

/**
  Returns the offset of EC recovery image within EC image.

**/
UINT32
EFIAPI
PlatformGetEcRecoveryImageOffset (
  VOID
  );

/**
  Returns the Null-terminated Unicode string that is used to fill in the
  VersionName field of the EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is
  returned by the GetImageInfo() service of the Firmware Management Protocol.
  The returned string must be allocated using EFI_BOOT_SERVICES.AllocatePool().

  @note It is recommended that all firmware devices support a method to report
        the VersionName string from the currently stored firmware image.

  @param[out] VersionString  The version string retrieved from the currently
                             stored firmware image.

  @retval EFI_SUCCESS            The version string of currently stored
                                 firmware image was returned in Version.
  @retval EFI_INVALID_PARAMETER  VersionString is NULL.
  @retval EFI_UNSUPPORTED        The firmware device does not support a method
                                 to report the version string of the currently
                                 stored firmware image.
  @retval EFI_DEVICE_ERROR       An error occurred attempting to retrieve the
                                 version string of the currently stored
                                 firmware image.
  @retval EFI_OUT_OF_RESOURCES   There are not enough resources to allocate the
                                 buffer for the version string of the currently
                                 stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetEcVersionString (
  OUT CHAR16  **VersionString
  );

/**
  Returns EC FMP version on the platform

  Returns the value used to fill in the Version field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.  If EFI_SUCCESS is returned, then
  the firmware device supports a method to report the Version value from the
  currently stored firmware image.  If the value can not be reported for the
  firmware image currently stored in the firmware device, then EFI_UNSUPPORTED
  must be returned.  EFI_DEVICE_ERROR is returned if an error occurs attempting
  to retrieve the LowestSupportedVersion value for the currently stored firmware
  image.

  @note It is recommended that all firmware devices support a method to report
        the Version value from the currently stored firmware image.

  @param[out] Version  The version value retrieved from the currently stored
                       firmware image.

  @retval EFI_SUCCESS       The version of currently stored firmware image was
                            returned in Version.
  @retval EFI_UNSUPPORTED   The firmware device does not support a method to
                            report the version of the currently stored firmware
                            image.
  @retval EFI_DEVICE_ERROR  An error occurred attempting to retrieve the version
                            of the currently stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetEcVersion (
  OUT UINT32  *Version
  );

/**
  A function used to check if the EC image is compatible to current platform.

  @param[in]  Image           Points to a new EC image.
  @param[in]  ImageSize       Size, in bytes, of the new EC image.
  @param[out] ImageUpdatable  Indicates if a EC firmware image is valid for current platform.

**/
EFI_STATUS
EFIAPI
PlatformFmpCheckEcCompatibility (
  IN  CONST VOID  *EcImage,
  IN  UINTN       EcImageSize,
  OUT BOOLEAN     *EcImageUpdatable
  );

/**
  A function used to perform platform related task before EC update.

  @retval EFI_SUCCESS           Platform required actions are perfromed successfully.
  @retval Others                Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpEcUpdatePreHook (
  VOID
  );

/**
  A function used to perform platform related task after EC update.

  @param[in]   UpdateStatus     The status of EC update.

  @retval EFI_SUCCESS           Platform required actions are perfromed successfully.
  @retval Others                Platform required actions are failed to execute.

**/
EFI_STATUS
EFIAPI
PlatformFmpEcUpdatePostHook (
  IN EFI_STATUS            UpdateStatus
  );

#endif
