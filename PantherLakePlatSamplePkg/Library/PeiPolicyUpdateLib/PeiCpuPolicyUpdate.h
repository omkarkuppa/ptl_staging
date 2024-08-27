/** @file
  Header file for PEI CpuPolicyUpdate.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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
#ifndef _PEI_CPU_POLICY_UPDATE_H_
#define _PEI_CPU_POLICY_UPDATE_H_

#include <PiPei.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/Wdt.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiPlatformLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <PlatformBoardId.h>
#include <Library/BaseCryptLib.h>
#include <SetupVariable.h>
#include <CrashLogVariable.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Ppi/MasterBootMode.h>
#include <Library/PeiServicesLib.h>
#include "PeiPchPolicyUpdate.h"
#include <Library/CpuPlatformLib.h>

#define MAX_MICROCODE_PATCH_SIZE      0x20000
#define SEAMLDR_SE_SVN_NV_BYTE_OFFSET 1
#define SEAMLDR_TPM_NV_INDEX_SIZE     8
#define SEAMLDR_SE_SVN_DEFAULT_VALUE  0xFF
#define OS_BIOS_ACM_HANDOFF           0x1C10104


#endif
