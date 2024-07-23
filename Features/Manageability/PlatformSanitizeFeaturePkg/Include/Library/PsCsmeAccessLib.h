/** @file
  Platform Sanitize Structure definitions Header file.

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
**/

#ifndef _PS_CSME_ACCESS_LIB_H_
#define _PS_CSME_ACCESS_LIB_H_

/**
  Perform Unconfigure CSME without password and try to wait for completion.

  @retval EFI_SUCCESS  Successfully sent Command to CSME to perform Unconfiguration
  @retval EFI_ABORTED  Unconfiguration error
  @retval EFI_TIMEOUT  Unconfiguration is still in progress after time elapsed
**/
EFI_STATUS
EFIAPI
PsCsmeUnconfigure (
  VOID
  );

/**
  Send Platform Sanitization Report to CSME to update PSR Record

  @param[in] PS_BOOT_PARAMETERS         Pointer to Boot Parameter structure.

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_NOT_FOUND                 Command Failed
**/
EFI_STATUS
EFIAPI
PsSummaryReportToPsr (
  IN PS_BOOT_PARAMETERS      *PsBootParameters
  );

#endif //PS_RPE_LIB_H_
