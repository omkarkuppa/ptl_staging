/** @file
  This file is SampleCode for Intel PEI Platform Policy initialization in post-memory.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include "PeiPolicy.h"
#include <Library/PeiServicesLib.h>
#include <Library/PeiPolicyBoardConfigLib.h>
#include <Library/PeiPolicyAdvancedConfigLib.h>

VOID *
EFIAPI
SiliconPolicyUpdatePostMem (
  IN OUT VOID *Policy
  )
{
  EFI_STATUS             Status;
  SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi;
  SI_POLICY_PPI          *SiPolicyPpi;

  Policy = NULL;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status) || (SiPreMemPolicyPpi == NULL)) {
    return Policy;
  }

  Status = PeiServicesLocatePpi (&gSiPolicyPpiGuid, 0, NULL, (VOID **) &SiPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) || (SiPolicyPpi == NULL)) {
    return Policy;
  }

  //
  // Update essential policies by board configuration
  //
  UpdatePeiPolicyBoardConfig (SiPreMemPolicyPpi, SiPolicyPpi);

  //
  // Update advanced policies
  //
  SiliconPolicyUpdatePostMemAdvancedConfig (SiPreMemPolicyPpi, SiPolicyPpi);

  return Policy;
}
