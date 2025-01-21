/** @file
  Header file for PSMI Fru Init Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _PSMI_INIT_FRU_LIB_H_
#define _PSMI_INIT_FRU_LIB_H_

#include <Ppi/SiPolicy.h>
#include <PsmiDataHob.h>

/**
  Override policies relevant to CPU/PCH Trace Hub to enable PSMI debug in PreMem.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
PsmiTraceHubPolicyOverride (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  );

/**
  Returns the Base address of Trace Hub

  @retval Tracehub base address
**/
UINT32
GetTraceHubBaseAddress (
  VOID
  );

/**
  Returns the Base address of Trace Hub Mbt Bar

  @retval Tracehub Mtb Bar base address
**/
UINT32
GetTraceHubMtbBarBaseAddress (
  VOID
  );

#endif
