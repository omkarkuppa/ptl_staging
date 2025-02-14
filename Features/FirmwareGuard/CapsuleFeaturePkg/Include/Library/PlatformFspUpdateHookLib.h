/** @file
  Library to get FSP information or perform specific action on platform.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef __PLATFORM_FSP_UPDATE_HOOK_LIB_H__
#define __PLATFORM_FSP_UPDATE_HOOK_LIB_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

/**
  Get the version value of currently stored firmware image.

  @param[out] VersionPtr  A pointer to the version of currently stored firmware image.

  @retval  EFI_SUCCESS            Succeed to get the version value of currently
                                  stored firmware image.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported to get the version value of currently
                                  stored firmware image.
  @retval  Others                 Failed to get the version value of currently
                                  stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetFspVersion (
  OUT UINT32  *VersionPtr
  );

/**
  Get the Null-terminated Unicode string used to fill in the VersionName field.

  @param[out]  VersionStringPtr  Double pointer to the version string retrieved of
                                 currently stored firmware image.

  @retval  EFI_SUCCESS            Succeed to get the version string of currently
                                  stored firmware image.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported to get the version sting of currently
                                  stored firmware image.
  @retval  Others                 Failed to get the version sting of currently
                                  stored firmware image.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetFspVersionString (
  OUT CHAR16  **VersionStringPtr
  );

/**
  A function used to perform platform related task before FSP update.

  @retval  EFI_SUCCESS  Succeed to perform platform required actions.
  @retval  Others       Failed to perform platform required actions.

**/
EFI_STATUS
EFIAPI
PlatformFmpFspUpdatePreHook (
  VOID
  );

/**
  A function used to perform platform related task after FSP update.

  @param[in]  UpdateStatus  The status of BIOS update.

  @retval  EFI_SUCCESS  Succeed to perform platform required actions.
  @retval  Others       Failed to perform platform required actions.

**/
EFI_STATUS
EFIAPI
PlatformFmpFspUpdatePostHook (
  IN EFI_STATUS  UpdateStatus
  );

#endif
