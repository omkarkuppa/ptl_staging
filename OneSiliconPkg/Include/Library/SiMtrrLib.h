/** @file
  Header file for Silicon code Mtrr Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _SI_MTRR_LIB_H_
#define _SI_MTRR_LIB_H_

/**
  Function attempts to transfer MTRRs default WriteBack
  and update MTRRs Setting.

  @param[out]  MtrrSetting      - A buffer holding all MTRRs content.

  @retval EFI_SUCCESS           - The function completed successfully.
          EFI_UNSUPPORTED       - Mtrr is not supported.
          EFI_INVALID_PARAMETER - MtrrSetting is NULL.
**/
EFI_STATUS
EFIAPI
MtrrTransfer2DefaultWB (
  OUT MTRR_SETTINGS  *MtrrSetting
  );

#endif
