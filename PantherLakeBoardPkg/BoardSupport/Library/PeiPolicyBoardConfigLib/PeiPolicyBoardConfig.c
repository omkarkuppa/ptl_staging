/** @file
 Intel PEI essential policy update by board configuration

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

#include "PeiPolicyBoardConfig.h"

/**
  This function performs PEI essential policy update by board configuration.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi          The RC POSTMEM Policy PPI instance

**/
EFI_STATUS
EFIAPI
UpdatePeiPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  //
  // Update policy by board configuration
  //
  UpdatePeiPchPolicyBoardConfig (SiPreMemPolicyPpi, SiPolicyPpi);
  UpdatePeiSaPolicyBoardConfig (SiPreMemPolicyPpi, SiPolicyPpi);
  UpdatePeiCpuPolicyBoardConfig (SiPreMemPolicyPpi, SiPolicyPpi);
#if FixedPcdGetBool(PcdAmtEnable) == 1
  UpdatePeiAmtPolicyBoardConfig ();
#endif
  UpdatePeiMePolicyBoardConfig ();
  UpdatePeiTbtPolicyBoardConfig ();
  return EFI_SUCCESS;
}

/**
  This function performs PEI essential policy update by board configuration.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance
**/
VOID
EFIAPI
UpdatePeiPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  //
  // Update policy by board configuration
  //
  UpdatePeiSaPolicyBoardConfigPreMem (SiPreMemPolicyPpi);
  UpdatePeiCpuPolicyBoardConfigPreMem (SiPreMemPolicyPpi);
}

/**
  This function performs PEI essential policy update by board configuration in FSP API mode.

**/
VOID
EFIAPI
UpdatePeiPolicyBoardConfigApi (
  VOID
  )
{
  //
  // Update policy by board configuration
  //
  UpdatePeiSiPolicyBoardConfig ();
  UpdatePeiPolicyBoardConfig (NULL, NULL);

}

/**
  This function performs PEI essential policy update by board configuration in FSP API mode.

**/
VOID
EFIAPI
UpdatePeiPolicyBoardConfigPreMemApi (
  VOID
  )
{

}