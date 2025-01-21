/** @file
  PCH LPC Library

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
#ifndef _PEI_LPC_LIB_H_
#define _PEI_LPC_LIB_H_

/**
  @param[in]  *LpcPcrAccess       Instance of LPC sideband Register Access Protocol

  Configure LPC power management.
**/
VOID
LpcConfigurePm (
  REGISTER_ACCESS            *LpcPcrAccess
  );

/**
  Configure LPC device on early PEI.
**/
VOID
LpcOnEarlyPeiConfigure (
  VOID
  );

/**
  Configure LPC device on Policy callback.

  @param[in] SiPreMemPolicyPpi   The SI PreMem Policy PPI instance
**/
VOID
LpcOnPolicyConfigure (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  );

#endif //_PEI_LPC_LIB_H_
