/** @file
  This library initialize Silicon Policy for PostMemory in FSP API mode.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SiPolicyLib.h>

/**
  Performs silicon post-mem policy initialization.

  The returned data must be used as input data for SiliconPolicyDonePostMem (),
  and SiliconPolicyUpdateLib.SiliconPolicyUpdatePostMem ().

  @param[in, out] Policy       Pointer to policy.
  @return the initialized policy.
**/
VOID *
EFIAPI
SiliconPolicyInitPostMem (
  IN OUT VOID *Policy
  )
{
/**
  The implementation of silicon policy initialization was moved to SiliconPolicyUpdatePostMemLoadDefaultUpd
  and gets called in SiliconPolicyUpdatePostMem in bootloader for FSP API mode due to package dependency.
**/
  Policy = NULL;
  return Policy;
}

/**
  The silicon post-mem policy is finalized.
  Silicon code can do initialization based upon the policy data.

  The input Policy must be returned by SiliconPolicyInitPostMem().

  @param[in] Policy       Pointer to policy.
  @retval RETURN_SUCCESS The policy is handled consumed by silicon code.
**/
RETURN_STATUS
EFIAPI
SiliconPolicyDonePostMem (
  IN VOID *Policy
  )
{
  //
  // While installed PolicyReadyPpi, RC assumes the Policy is ready and finalized. So please
  // update and override any setting before calling this function.
  //
  SiInstallPolicyReadyPpi ();
  DEBUG ((DEBUG_INFO, "Silicon PEI Policy Initialization Done in Post-Memory\n"));
  return RETURN_SUCCESS;
}
