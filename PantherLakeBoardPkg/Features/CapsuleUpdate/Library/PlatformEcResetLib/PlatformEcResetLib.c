/** @file
  Platform EC reset library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Library/PlatformEcResetLib.h>
#include <Library/EcMiscLib.h>

/**
  Perform EC reset on platform

  @retval EFI_SUCCESS  The operation succeed.
  @retval Other        The operation failed.

**/
EFI_STATUS
EFIAPI
PlatformEcReset (
  VOID
  )
{
  return LpcEcResetEcInNormalMode ();
}