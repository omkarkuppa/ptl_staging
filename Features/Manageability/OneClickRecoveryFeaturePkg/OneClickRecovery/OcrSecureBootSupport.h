/** @file
  Intel One Click Recovery Secure Boot Support functional implementation.

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

#ifndef _OCR_SECUREBOOT_SUPPORT_H_
#define _OCR_SECUREBOOT_SUPPORT_H_

#include <PiDxe.h>
#include <AsfMsgs.h>

/**
  This routine disables Secure Boot if requested for One Click Recovery.

  @param[in] AsfBootOptions ASF provided boot command options

  @retval EFI_SUCCESS       Successful
  @retval EFI_ABORTED       Unable to get Secure Boot EFI Variable
**/
EFI_STATUS
OcrDisableSecureBootState (
  IN ASF_BOOT_OPTIONS         AsfBootOptions
  );

/**
  This routine restores Secure Boot for One Click Recovery.

  @retval EFI_SUCCESS     Successful
  @retval EFI_ABORTED     Unable to get Secure Boot EFI Variable
**/
EFI_STATUS
OcrRestoreSecureBootState (
  VOID
  );

#endif // _OCR_SECUREBOOT_SUPPORT_H_
