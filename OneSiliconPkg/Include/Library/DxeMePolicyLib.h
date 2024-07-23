/** @file
  Prototype of the DxeMePolicyLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _DXE_ME_POLICY_LIB_H_
#define _DXE_ME_POLICY_LIB_H_

#include <Protocol/MePolicy.h>

/**
  This function prints the ME DXE phase policy.

  @param[in] DxeMePolicy - ME DXE Policy protocol
**/
VOID
MePrintPolicyProtocol (
  IN  ME_POLICY_PROTOCOL             *DxeMePolicy
  );

/**
  MeCreatePolicyDefaults creates the default setting of ME Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in, out] DxeMePolicy           The pointer to get ME Policy protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
MeCreatePolicyDefaults (
  IN OUT  ME_POLICY_PROTOCOL            **DxeMePolicy
  );

/**
  MeInstallPolicyProtocol installs ME Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] DxeMePolicy                The pointer to ME Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
MeInstallPolicyProtocol (
  IN  EFI_HANDLE                    ImageHandle,
  IN  ME_POLICY_PROTOCOL            *DxeMePolicy
  );

#endif // _DXE_ME_POLICY_LIB_H_
