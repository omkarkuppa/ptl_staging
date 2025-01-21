/** @file
  PEIM to initialize FRU Early BIOS Guard.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/BiosGuardInit.h>
#include <Register/GenerationMsr.h>

/**
  Configure BIOS Guard MSRs MSR_PLAT_FRMW_PROT_HASH

  @param[in] BIOS_GUARD_CONFIG  - BIOS Guard Configuration pointer

**/
VOID
ConfigureBiosGuardHashMsrRegisters (
  IN BIOS_GUARD_CONFIG     *BiosGuardConfig
  )
{
  // Configure BIOS Guard Module Hashes Registers
  if (BiosGuardConfig != NULL) {
    AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_0, BiosGuardConfig->BgpdtHash[0]);
    AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_1, BiosGuardConfig->BgpdtHash[1]);
    AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_2, BiosGuardConfig->BgpdtHash[2]);
    AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_3, BiosGuardConfig->BgpdtHash[3]);
    AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_4, BiosGuardConfig->BgpdtHash[4]);
    AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_5, BiosGuardConfig->BgpdtHash[5]);
  }
}

