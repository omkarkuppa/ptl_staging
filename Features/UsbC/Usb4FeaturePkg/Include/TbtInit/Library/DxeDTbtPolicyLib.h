/** @file
  Prototype of the DxeDTbtPolicyLib library.

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

@par Specification
**/

#ifndef _DXE_DTBT_POLICY_LIB_H_
#define _DXE_DTBT_POLICY_LIB_H_

/**
  Print DXE DTBT Policy
**/
VOID
DTbtPrintDxePolicyConfig (
  VOID
);

/**
  DTbtInstallPolicyProtocol installs DTBT Policy.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] ImageHandle                Image handle of this driver.

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
DTbtInstallPolicyProtocol (
  IN  EFI_HANDLE                  ImageHandle
  );
#endif // _DXE_DTBT_POLICY_LIB_H_
