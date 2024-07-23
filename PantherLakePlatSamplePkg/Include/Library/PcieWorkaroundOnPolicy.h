/** @file
  Header file for the PCIE Workaround on Policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _PCIE_WORKAROUND_ON_POLICY_PEI_H_
#define _PCIE_WORKAROUND_ON_POLICY_PEI_H_

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <CpuRegs.h>
#include <Library/TimerLib.h>
#include <HybridGraphicsInfo.h>

//
// Functions
//

/**
  PcieCardResetWorkAround performs PCIe Card reset on root port

  @param[in out] SiPreMemPolicyPpi       - SI_PREMEM_POLICY_PPI

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
EFIAPI
PcieCardResetWorkAround (
  VOID
  );
#endif
