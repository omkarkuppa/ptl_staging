/** @file
  The header file includes the common header files, defines
  internal structure and functions used by TccPolicyLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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


#ifndef _TCC_POLICY_LIB_H_
#define _TCC_POLICY_LIB_H_

/**
  Returns if TCC mode is enabled

  @return TRUE  TCC mode is enabled
  @return FALSE TCC mode is disabled
**/
BOOLEAN
IsTccModeEnabled (
  SETUP_DATA *SetupData
  );

/**
  Gets the TCC Mode policy configuration.

  @param  CpuSetup   CPU setup policies with TCC Mode defaults
  @param  PchSetup   PCH setup policies with TCC Mode defaults
  @param  SaSetup    SA setup policies with TCC Mode defaults
  @param  SetupData  System setup policies with TCC Mode defaults

**/
VOID
GetTccModeConfigPolicy (
  CPU_SETUP  *CpuSetup,
  PCH_SETUP  *PchSetup,
  SA_SETUP   *SaSetup,
  SETUP_DATA *SetupData
  );

#endif
