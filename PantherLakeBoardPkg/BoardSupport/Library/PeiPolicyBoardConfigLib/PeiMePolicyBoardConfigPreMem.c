/** @file
 Intel PEI ME Pre-Memory Policy update by board configuration

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
#include "PeiPolicyBoardConfig.h"

/**
  This function performs PEI ME Pre-Memory Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicyBoardConfigPreMem (
  VOID
  )
{
  EFI_STATUS                         Status;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  SI_PREMEM_POLICY_PPI               *SiPreMemPolicyPpi;
  ME_PEI_PREMEM_CONFIG               *MePeiPreMemConfig;
#endif

  DEBUG ((DEBUG_INFO, "Updating ME Policy by board config in Pre-Mem\n"));

  Status = EFI_SUCCESS;

#if FixedPcdGet8(PcdFspModeSelection) == 0
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
#endif

  return Status;
}

