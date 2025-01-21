/** @file
  Prototype of the DxePchPolicyLib library.

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
#ifndef _DXE_PCH_POLICY_LIB_H_
#define _DXE_PCH_POLICY_LIB_H_

#include <Protocol/PchPolicy.h>

/**
  This function prints the DXE phase policy.

  @param[in] PchPolicy    - PCH DXE Policy protocol
**/
VOID
PchPrintPolicyProtocol (
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  );

/**
  This function prints the DXE phase policy.

  @param[in] PchPolicy    - PCH DXE Policy protocol
**/
VOID
AttachedPchPrintPolicyProtocol (
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  );

/**
  CreatePchDxeConfigBlocks generates the config blocksg of PCH DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] PchPolicy                 The pointer to get PCH Policy Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreatePchDxeConfigBlocks(
  IN OUT  PCH_POLICY_PROTOCOL      **PchPolicy
  );

/**
  CreateAttachedPchDxeConfigBlocks generates the config blocksg of PCH DXE Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] PchPolicy                 The pointer to get PCH Policy Protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
CreateAttachedPchDxeConfigBlocks(
  IN OUT  PCH_POLICY_PROTOCOL      **PchPolicy
  );

/**
  PchInstallPolicyProtocol installs PCH Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] PchPolicy                  The pointer to PCH Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
PchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  );

/**
  AttachedPchInstallPolicyProtocol installs PCH Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] PchPolicy                  The pointer to PCH Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
AttachedPchInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle,
  IN  PCH_POLICY_PROTOCOL         *PchPolicy
  );

#endif // _DXE_PCH_POLICY_LIB_H_
