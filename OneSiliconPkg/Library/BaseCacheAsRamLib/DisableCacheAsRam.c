/** @file
  Disable Cache As Ram

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Library/BaseLib.h>
#include <Register/Msr.h>
#include <CpuRegs.h>
#include <Library/CpuInfoFruLib.h>
#include <Register/GenerationMsr.h>
#include <Library/CpuPlatformLib.h>
#include <Library/MsrFruLib.h>

VOID
CacheInvd (
  VOID
  );

/**
  Disable NEM (No Eviction Mode).

  @param[in] DisableCar       TRUE means use Cache INVD, FALSE means use WBINVD
**/
VOID
EFIAPI
DisableCacheAsRam (
  IN BOOLEAN DisableCar
  )
{
  UINT32                     Index;
  MSR_IA32_MCG_CAP_REGISTER  MsrMcgCap;
  MSR_NEM_REGISTER           MsrNem;
  MSR_MTRRDEFAULT_REGISTER   MsrMtrrDefault;

  //
  // Disable MTRRs by clearing the IA32_MTRR_DEF_TYPE MSR E flag
  //
  MsrMtrrDefault.Uint64 = AsmReadMsr64 (MSR_MTRRDEFAULT);
  MsrMtrrDefault.Bits.En = 0;
  AsmWriteMsr64 (MSR_MTRRDEFAULT, MsrMtrrDefault.Uint64);

  if (DisableCar) {
    CacheInvd ();
  } else {
    AsmWbinvd();
  }

  MsrNem.Uint64 = AsmReadMsr64 (MSR_NEM);

  //
  // Disable No-Eviction Mode Run State by clearing MSR_NEM MSR 2E0h bit [1] = 0
  //
  MsrNem.Bits.Run = 0;
  AsmWriteMsr64 (MSR_NEM, MsrNem.Uint64);

  //
  // Can't combine this MSR write with previous MSR write.
  // Disable No-Eviction Mode Setup State by clearing MSR_NEM MSR 2E0h bit [0] = 0
  //
  MsrNem.Bits.Setup = 0;
  AsmWriteMsr64 (MSR_NEM, MsrNem.Uint64);

  //
  //Dump all MCA bank status in debug build
  //
  MsrMcgCap.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);
  for (Index = 0; Index < (UINT32) MsrMcgCap.Bits.Count; Index++) {
      DEBUG ((DEBUG_INFO, "MCA status for bank %d is is 0x%lx\n",
              Index,
              AsmReadMsr64 (MSR_IA32_MC0_STATUS + Index * 4)));
  }
  McaBankStatusClear ();

  //
  // Enable MTRRs for better performance
  //
  MsrMtrrDefault.Bits.En = 1;
  AsmWriteMsr64 (MSR_MTRRDEFAULT, MsrMtrrDefault.Uint64);
}
