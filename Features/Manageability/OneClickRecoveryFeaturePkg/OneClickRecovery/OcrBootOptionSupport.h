/** @file
  Intel One Click Recovery Boot Option Support functional definitions.

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

#ifndef __OCR_BOOT_OPTION_SUPPORT_H__
#define __OCR_BOOT_OPTION_SUPPORT_H__

#include <PiDxe.h>
#include <Uefi.h>
#include <OneClickRecoveryConfig.h>
#include <Protocol/OneClickRecoveryProtocol.h>

//
// One Click Recovery String
//
#define OCR_STRING            L"One Click Recovery"
#define OCR_HTTPS_STRING      L"One Click Recovery HTTPS"
#define WINRE_OPTIONAL_DATA   L"/RecoveryBCD"

/**
  Creates OCR Description Title

  @param[in]  Description            Title to Append to boot option
  @param[out] OcrDescription         OCR Description
  @param[in]  MaxOcrDescriptionSize  Max OCR Description Size

  @retval  EFI_SUCCESS               Created OCR Description
  @retval  EFI_INVALID_PARAMETER     Pointers are null
  @retval  EFI_BUFFER_TOO_SMALL      Description will be larger than max size
**/
EFI_STATUS
CreateOcrBootOptionDescription (
  IN  CHAR16       *Description,
  OUT CHAR16       *OcrDescription,
  IN  UINT32       MaxOcrDescriptionSize
  );

/**
  Get OCR Boot Setting EFI Variable

  @retval OCR_BOOT_SETTINGS  OCR boot Setting
**/
OCR_BOOT_SETTINGS
GetOcrBootSettings (
  VOID
  );

/**
  Set UEFI Boot Options State

  @param[in] OcrConfig  OCR Setup Configuration
  @param[in] OcrCap     System OCR Capabilities

  @retval EFI_SUCCESS   Set AMT UEFI boot options states
**/
EFI_STATUS
SetUefiBootOptionsState (
  IN OCR_CONFIG               OcrConfig,
  IN ONE_CLICK_RECOVERY_CAP   OcrCap
  );

/**
  Adds OCR Boot Option to EFI Boot Manager

  @param[in] BootOption        Boot Option

  @retval EFI_SUCCESS          Added OCR Boot Option
  @retval EFI_INVALID          OCR Boot Option is NULL
  @retval EFI_NOT_FOUND        OCR Boot Device Path not found
  @retval EFI_UNSUPPORTED      OCR Boot Option is not supported
 **/
EFI_STATUS
AddOcrBootOption (
  IN OCR_BOOT_OPTION    *BootOption
  );

/**
  Delete One Click Recovery Boot Options if they exist
**/
VOID
DeleteOcrBootOption (
  VOID
  );

/**
  Is the current Boot Option the One Click Recovery Boot Option

  @param[in,out]  OcrBootOption   Boot option variable

  @retval TRUE   Current Boot Option is One Click Recovery Boot Option
  @retval False  Current Boot Option is not One Click Recovery Boot Option
**/
BOOLEAN
IsCurrentBootOcrBootOption (
  IN OUT OCR_BOOT_OPTION    *OcrBootOption
  );

/**
  Setup OCR Boot Option if requested from AMT

  @param[in]      OcrConfig            OCR Setup Configuration
  @param[in]      AmtUefiBootOption    OCR AMT provided boot options
  @param[out]     OcrBootOption        OCR boot options

  @retval EFI_SUCCESS             OCR Boot Option is setup.
  @retval EFI_NOT_FOUND           OCR Boot was not requested.
  @retval EFI_OUT_OF_RESOURCES    Ran out of resources
  @retval EFI_ABORTED             Boot Option requested is not valid
  @retval EFI_SECURITY_VIOLATION  Secure Boot needs to be enable for OCR WinRe and PBA
**/
EFI_STATUS
OcrBootOptionRequest (
  IN     OCR_CONFIG                   OcrConfig,
  IN     UEFI_BOOT_OPTION_PARAMETER   *AmtUefiBootOption,
  OUT    OCR_BOOT_OPTION              *OcrBootOption
  );

#endif // __OCR_BOOT_OPTION_SUPPORT_H__
