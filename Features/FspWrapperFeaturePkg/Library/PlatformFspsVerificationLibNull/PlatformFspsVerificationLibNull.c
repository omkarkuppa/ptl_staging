/** @file
  Provide NULL implementation for FSP-S verification. Platform should provide
  its' own solution.

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

@par Specification
**/

#include <Library/PlatformFspsVerificationLib.h>

/**
  Platform specific method to verify FSP-S, which should be executed after FSP-S is
  extracted onto permarnant mermory.
  This instance provides a NULL implementation and always returns EFI_SUCCESS.

  @param[in]   FspsImageBase      FSP-S image base in memory.

  @retval EFI_SUCCESS             Verification Pass or verification can be skipped.
  @retval EFI_ACCESS_DENIED       Verification Fail.

**/
EFI_STATUS
EFIAPI
PlatformVerifyFsps (
  IN UINTN        FspsImageBase
  )
{
  return EFI_SUCCESS;
}