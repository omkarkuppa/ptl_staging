/** @file
  Component Update Libaray
  Called by each FmpDeviceLib instance to do capsule update.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/PlatformFlashAccessLib.h>

/**
  Perform ME firmware update.

  @param[in] WriteReq                Request information for update flash.
  @param[in] Progress                A function used report the progress of the
                                     firmware update.  This is an optional parameter
                                     that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.
  @param[in] EndPercentage           The end completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.

  @retval EFI_SUCCESS                ME FW is updated successfully.
  @retval Others                     The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateMeFirmware (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

/**
  Update BIOS region.

  @param[in] WriteReq                    Request information for update flash.
  @param[in] Progress                    A function used report the progress of the
                                         firmware update.  This is an optional parameter
                                         that may be NULL.
  @param[in] StartPercentage             The start completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.
  @param[in] EndPercentage               The end completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.

  @retval EFI_SUCCESS                    The flash region is updated successfully.
  @retval EFI_INVALID_PARAMETER          The input buffer is invalid.
  @retval Others                         The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateBiosFirmware (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

/**
  Update EC region.

  @param[in] WriteReq                    Request information for update flash.
  @param[in] Progress                    A function used report the progress of the
                                         firmware update.  This is an optional parameter
                                         that may be NULL.
  @param[in] StartPercentage             The start completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.
  @param[in] EndPercentage               The end completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.

  @retval EFI_SUCCESS                    The flash region is updated successfully.
  @retval EFI_INVALID_PARAMETER          The input buffer is invalid.
  @retval Others                         The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateEcFirmware (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

/**
  Update GbE region.

  @param[in] WriteReq                    Request information for update flash.
  @param[in] Progress                    A function used report the progress of the
                                         firmware update.  This is an optional parameter
                                         that may be NULL.
  @param[in] StartPercentage             The start completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.
  @param[in] EndPercentage               The end completion percentage value that may
                                         be used to report progress during the flash
                                         write operation.

  @retval EFI_SUCCESS                    The flash region is updated successfully.
  @retval EFI_INVALID_PARAMETER          The input buffer is invalid.
  @retval Others                         The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateGbERegion (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );

/**
  Perform ISH PDT configuration update

  @param[in] WriteReq                Request information for update flash.
  @param[in] Progress                A function used report the progress of the
                                     firmware update.  This is an optional parameter
                                     that may be NULL.
  @param[in] StartPercentage         The start completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.
  @param[in] EndPercentage           The end completion percentage value that may
                                     be used to report progress during the flash
                                     write operation.

  @retval EFI_SUCCESS                PDT is updated successfully.
  @retval Others                     The update operation fails.

**/
EFI_STATUS
EFIAPI
UpdateIshPdt (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  );
