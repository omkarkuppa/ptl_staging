/** @file
  Prototype of the DxeAmtPolicyLib library.

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
#ifndef _DXE_AMT_POLICY_LIB_H_
#define _DXE_AMT_POLICY_LIB_H_

#include <Protocol/AmtPolicy.h>

/**
  This function prints the AMT DXE phase policy.

  @param[in] DxeAmtPolicy - AMT DXE Policy protocol
**/
VOID
AmtPrintPolicyProtocol (
  IN  AMT_POLICY_PROTOCOL             *DxeAmtPolicy
  );

/**
  AmtCreatePolicyDefaults creates the default setting of Amt Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[in, out] DxeAmtPolicy          The pointer to get Amt Policy protocol instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
AmtCreatePolicyDefaults (
  IN OUT  AMT_POLICY_PROTOCOL       **DxeAmtPolicy
  );

/**
  AmtInstallPolicyProtocol installs Amt Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.
  @param[in] DxeAmtPolicy               The pointer to Amt Policy Protocol instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
AmtInstallPolicyProtocol (
  IN  EFI_HANDLE                        ImageHandle,
  IN  AMT_POLICY_PROTOCOL               *DxeAmtPolicy
  );

#endif // _DXE_AMT_POLICY_LIB_H_
