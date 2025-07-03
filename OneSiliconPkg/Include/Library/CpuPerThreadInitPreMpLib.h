/** @file

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

#ifndef __CPU_PER_THREAD_INIT_PRE_MP_LIB_H__
#define __CPU_PER_THREAD_INIT_PRE_MP_LIB_H__

#include <PiPei.h>

BOOLEAN
IsFusaCapSupported (
  VOID
  );

/**
  This function will do per thread init before MpInit, like setting Lockstep mode, enabling X2Apic mode and etc.
  This function must be called after gEfiPeiMemoryDiscoveredPpiGuid is installed and before MpInit.
**/
VOID
EFIAPI
CpuPerThreadInitPreMp (
  UINT16 ModuleLockstep
  );

#endif
