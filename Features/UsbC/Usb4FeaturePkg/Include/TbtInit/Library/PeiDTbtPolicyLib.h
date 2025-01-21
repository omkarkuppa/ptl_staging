/** @file
  Prototype of the PeiDTbtPolicyLib library.

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

#ifndef _PEI_DTBT_POLICY_LIB_H_
#define _PEI_DTBT_POLICY_LIB_H_

/**
  Install DTbt Policy

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallPeiDTbtPolicy (
  VOID
  );

/**
  Print PEI DTBT Policy
**/
VOID
EFIAPI
DTbtPrintPeiPolicyConfig (
  VOID
  );
#endif // _PEI_DTBT_POLICY_LIB_H_
