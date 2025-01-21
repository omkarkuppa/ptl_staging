/** @file
  Overclocking early post initialization null library.

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

@par Specification
**/

#include <Library/PeiOcLib.h>

/**
  Initializes pre-memory Overclocking settings in the processor.

  @param[in] SiPolicyPreMemPpi  Instance of SI_PREMEM_POLICY_PPI

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuOcInitPreMem (
  IN SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  return EFI_SUCCESS;
}

/**
  Initializes post-memory Overclocking settings in the processor.

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance
  @param[in] MpServices2Ppi            Pointer to this instance of the MpServices.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuOcInitPostMem (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig,
  IN EDKII_PEI_MP_SERVICES2_PPI   *MpServices2Ppi
  )
{
  return EFI_SUCCESS;
}
