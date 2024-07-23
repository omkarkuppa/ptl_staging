/** @file
  Prototype of the DxeITbtPolicyLib library.

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
#ifndef _DXE_ITBT_POLICY_LIB_H_
#define _DXE_ITBT_POLICY_LIB_H_

#include <Protocol/ITbtPolicy.h>
/**
  Print DXE ITBT Policy

  @param[in]  ITbtPolicy                The pointer to get iTBT DXE Protocol instance
**/
VOID
ITbtPrintDxePolicyConfig (
  IN  ITBT_POLICY_PROTOCOL              *ITbtPolicy
  );

/**
  CreateITbtDxeConfigBlocks generates the config blocksg of iTBT DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] ITbtPolicy                The pointer to get iTBT DXE Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateITbtDxeConfigBlocks (
  IN OUT  ITBT_POLICY_PROTOCOL          **ITbtPolicy
);

/**
  ITbtInstallPolicyProtocol installs iTBT Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] ITbtPolicy                 The pointer to TBT Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
ITbtInstallPolicyProtocol (
  IN  EFI_HANDLE                        ImageHandle,
  IN  ITBT_POLICY_PROTOCOL              *ITbtPolicy
  )
;

#endif // _DXE_ITBT_POLICY_LIB_H_
