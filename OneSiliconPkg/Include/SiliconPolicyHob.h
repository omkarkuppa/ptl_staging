/** @file
  The HOB definition for SiliconPolicyHob

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

#ifndef _SILICON_POLICY_HOB_H_
#define _SILICON_POLICY_HOB_H_

extern EFI_GUID gSiliconPolicyHobGuid;

typedef struct {
  /*
  SI PreMemory policy pointer
  */
  EFI_PHYSICAL_ADDRESS      SiPreMemPolicyPpi;
  /*
  SI PostMemory policy pointer
  */
  EFI_PHYSICAL_ADDRESS      SiPolicyPpi;
  /*
  PCH PreMemory policy pointer
  */
  EFI_PHYSICAL_ADDRESS      PchPreMemPolicyPpi;
  /*
  PCH PostMemory policy pointer
  */
  EFI_PHYSICAL_ADDRESS      PchPolicyPpi;
} SILICON_POLICY_HOB;

#endif
