/** @file
  Header file for the PeiPolicyBoardConfig Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _PEI_POLICY_BOARD_CONFIG_LIB_H_
#define _PEI_POLICY_BOARD_CONFIG_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  This function performs PEI CPU Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  This function performs PEI ME Pre-Memory Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicyBoardConfigPreMem (
  VOID
  );

/**
  This function performs PEI PCH Pre-Memory Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyBoardConfigPreMem (
  VOID
  );

/**
  This function performs PEI SA Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  This function performs PEI AMT Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiAmtPolicyBoardConfig (
  VOID
  );

/**
  This function performs PEI CPU Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  );

/**
  This function performs PEI ME Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiMePolicyBoardConfig (
  VOID
  );

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
  );

/**
  This function performs PEI SA Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  );

/**
  This function performs PEI TBT Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiTbtPolicyBoardConfig (
  VOID
  );

/**
  This function performs PEI SI Policy update by board configuration.

  @retval EFI_SUCCESS             The SI Policy is successfully updated.
  @retval Others                  The SI Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicyBoardConfig (
  VOID
  );

/**
  This function performs PEI essential Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi          The RC POSTMEM Policy PPI instance

**/
EFI_STATUS
EFIAPI
UpdatePeiPolicyBoardConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  );

/**
  This function performs PEI essential Policy update by board configuration.

**/
VOID
EFIAPI
UpdatePeiPolicyBoardConfigApi (
  VOID
  );

/**
  This function performs PEI essential Pre-Memory Policy update by board configuration.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance

**/
VOID
EFIAPI
UpdatePeiPolicyBoardConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  This function performs PEI essential Pre-Memory Policy update by board configuration.

**/
VOID
EFIAPI
UpdatePeiPolicyBoardConfigPreMemApi (
  VOID
  );
#endif
