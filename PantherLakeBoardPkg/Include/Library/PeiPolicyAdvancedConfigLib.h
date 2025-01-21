/** @file
  Header file for PeiPolicyAdvancedConfig Library.

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

#ifndef _PEI_POLICY_ADVANCED_CONFIG_LIB_H_
#define _PEI_POLICY_ADVANCED_CONFIG_LIB_H_

/**
  This function hooks pre-memory advanced policy update.

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval Others                  The Policy is not successfully updated.
**/
VOID
EFIAPI
SiliconPolicyUpdatePreMemAdvancedConfigApi (
  VOID
  );

/**
  This function hooks pre-memory advanced policy update.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval Others                  The Policy is not successfully updated.
**/
VOID
EFIAPI
SiliconPolicyUpdatePreMemAdvancedConfig (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  This function hooks post-memory advanced policy update.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi          The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval Others                  The Policy is not successfully updated.
**/
VOID
EFIAPI
SiliconPolicyUpdatePostMemAdvancedConfigApi (
  VOID
  );

/**
  This function hooks post-memory advanced policy update.

  @param[in] SiPreMemPolicyPpi    The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi          The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval Others                  The Policy is not successfully updated.
**/
VOID
EFIAPI
SiliconPolicyUpdatePostMemAdvancedConfig (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  );
#endif
