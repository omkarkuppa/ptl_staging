/** @file
  Header file for the PeiCpuPolicyLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _PEI_TXT_POLICY_LIBRARY_H_
#define _PEI_TXT_POLICY_LIBRARY_H_

#include <PiPei.h>
#include <CpuRegs.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/SiPolicy.h>
#include <BiosGuard.h>
#include <IndustryStandard/SmBios.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TxtLib.h>
#include <Register/Cpuid.h>
#include <Library/PcdLib.h>

//
// TXT configuration defines
//
#define TXT_SINIT_MEMORY_SIZE         0x50000
#define TXT_HEAP_MEMORY_SIZE          0xF0000
#define TXT_LCP_PD_BASE               0x0         ///< Platform default LCP
#define TXT_LCP_PD_SIZE               0x0         ///< Platform default LCP
#define TXT_TGA_MEMORY_SIZE           0x0         ///< Maximum 512K of TGA memory (aperture)

#endif // _PEI_TXT_POLICY_LIBRARY_H_
