/** @file
 Intel PEI PCH Policy update by board configuration

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
#include <ConfigBlock/PchGeneralConfig.h>

/**
  This function performs PEI PCH Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi          The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                         Status;
  PCH_GENERAL_CONFIG                 *PchGeneralConfig;

  DEBUG ((DEBUG_INFO, "Updating PCH Policy by board config in Post-Mem\n"));

  Status = EFI_SUCCESS;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
