/** @file
  This file contains the tests for the SecureMemoryMapConfiguration bit

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "HstiIhvDxe.h"
#include <Library/SpiAccessLib.h>
#include <Register/Hpet.h>
#include <Library/PchPciBdfLib.h>
#include <Library/HostBridgeInfoLib.h>
#include <Library/HostBridgeDataLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Defines/PchReservedResources.h>
#include <Protocol/MpService.h>
#include <Defines/HostBridgeDefines.h>

#define LOCAL_APIC_MSR_MASK           (0x00000000FFFFF000)

typedef struct {
  UINT64   Base;
  UINT64   End;
} MEMORY_RANGE;

typedef enum {
  LockableMemoryRangeLtSpace,
  LockableMemoryRangeHighBios,
  LockableMemoryRangeLowDram,
  LockableMemoryRangeHighDram,
  LockableMemoryRangeMchBar,
  LockableMemoryRangeVtdPvc0Bar,
  LockableMemoryRangeRegBar,
  LockableMemoryRangeCpuRsvd1Bar,
  LockableMemoryRangeCpuRsvd2Bar,
  LockableMemoryRangeSafBar,
  LockableMemoryRangeLocalApic,
  LockableMemoryRangeIoApic,
  LockableMemoryRangeHpet,
  LockableMemoryRangeP2sbBar,
  LockableMemoryRangeP2sb2Bar,
  LockableMemoryRangePcieCfg,
  LockableMemoryRangeExtBiosDecodeBar,
  LockableMemoryRangeMax,
} LOCKABLE_MEMORY_RANGE;

MEMORY_RANGE  mLockableMemoryRange[LockableMemoryRangeMax] = {
  // 0. LT space (0xFED20000 - 0xFED7FFFF)
  {0xFED20000, 0xFED7FFFF},
  // 1. High BIOS
  {0x0, SIZE_4GB - 1},
  // 2. Low DRAM (0 - TOLUD)
  {0x0, 0},
  // 3. High DRAM (4GB - TOUUD)
  {SIZE_4GB, 0},
  // 4. MCHBAR
  {0, 0},
  // 5. VTDBAR (GFX + Non-GFX)
  {0, 0},
  // 6. REGBAR
  {0, 0},
  // 7. CPU Reserved space: 0xFEB00000 to 0xFEB0FFFF
  {0xFEB00000, 0xFEB0FFFF},
  // 8. CPU Reserved space: 0xFEB80000 to 0xFEB8FFFF
  {0xFEB80000, 0xFEB8FFFF},
  // 9. SAFBAR
  {0, 0},
  // 10. Local APIC
  {0, 0},
  // 11. I/O APIC Range
  {0, 0},
  // 12. HPET
  {0, 0},
  // 13. P2SB BAR
  {0, 0},
  // 14. P2SB 2 BAR
  {0, 0},
  // 15. PCI Express CFG Range
  {0, 0},
  // 16. Extended Bios Decode
  {0, 0}
};

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

PCH_SPI_PROTOCOL         *mSpiProtocol;
EFI_MP_SERVICES_PROTOCOL *mMpProtocol;

/**
  Check for overlaps in single range array

  @param[in] Range     - Pointer to Range array
  @param[in] Count     - Number of Enties

  @retval BOOLEAN - Overlap Exists
**/
BOOLEAN
CheckOverlap (
  IN MEMORY_RANGE *Range,
  IN UINTN        Count
  )
{
  UINTN  Index;
  UINTN  SubIndex;

  for (Index = 0; Index < Count - 1; Index++) {
    if ((Range[Index].Base == 0) && (Range[Index].End == 0)) {
      continue;
    }
    for (SubIndex = Index + 1; SubIndex < Count; SubIndex++) {
      if ((Range[SubIndex].Base == 0) && (Range[SubIndex].End == 0)) {
        continue;
      }
      if (((Range[Index].Base >= Range[SubIndex].Base) && (Range[Index].Base <= Range[SubIndex].End)) ||
          ((Range[SubIndex].Base >= Range[Index].Base) && (Range[SubIndex].Base <= Range[Index].End))) {
        DEBUG ((DEBUG_ERROR, "        OVERLAP: \n"));
        DEBUG ((DEBUG_ERROR, "          0x%016lx - 0x%016lx\n", Range[Index].Base, Range[Index].End));
        DEBUG ((DEBUG_ERROR, "          0x%016lx - 0x%016lx\n", Range[SubIndex].Base, Range[SubIndex].End));
        return TRUE;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "        CheckOverlap: PASS\n"));
  return FALSE;
}

/**
  Check for overlaps between two arrays of memory ranges

  @param[in] Range1     - Pointer to Range1 array
  @param[in] Count1     - Number of Enties
  @param[in] Range2     - Pointer to Range2 array
  @param[in] Count2     - Number of Enties

  @retval BOOLEAN - Overlap Exists
**/
BOOLEAN
CheckOverlap2 (
  IN MEMORY_RANGE *Range1,
  IN UINTN        Count1,
  IN MEMORY_RANGE *Range2,
  IN UINTN        Count2
  )
{
  UINTN  Index1;
  UINTN  Index2;

  for (Index1 = 0; Index1 < Count1; Index1++) {
    if ((Range1[Index1].Base == 0) && (Range1[Index1].End == 0)) {
      continue;
    }
    for (Index2 = 0; Index2 < Count2; Index2++) {
      if ((Range2[Index2].Base == 0) && (Range2[Index2].End == 0)) {
        continue;
      }
      if (((Range1[Index1].Base >= Range2[Index2].Base) && (Range1[Index1].Base <= Range2[Index2].End)) ||
          ((Range2[Index2].Base >= Range1[Index1].Base) && (Range2[Index2].Base <= Range1[Index1].End))) {
        DEBUG ((DEBUG_ERROR, "        OVERLAP2: \n"));
        DEBUG ((DEBUG_ERROR, "          0x%016lx - 0x%016lx\n", Range1[Index1].Base, Range1[Index1].End));
        DEBUG ((DEBUG_ERROR, "          0x%016lx - 0x%016lx\n", Range2[Index2].Base, Range2[Index2].End));
        return TRUE;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "        CheckOverlap2: PASS\n"));
  return FALSE;
}

/**
  Dumps Ranges to Serial

  @param[in] Range     - Pointer to Range array
  @param[in] Count     - Number of Enties

**/
VOID
DumpRange (
  IN MEMORY_RANGE *Range,
  IN UINTN        Count
  )
{
  UINTN  Index;

  for (Index = 0; Index < Count; Index ++) {
    DEBUG ((DEBUG_INFO, "        [%02d] 0x%016lx - 0x%016lx\n", Index, Range[Index].Base, Range[Index].End));
  }
}


/**
  Run tests for SecureMemoryMapConfiguration bit
**/
VOID
CheckSecureMemoryMapConfiguration (
  VOID
  )
{
  EFI_STATUS        Status;
  BOOLEAN           Result;
  UINT64            SaMchBar;
  UINT64            SaSafBar;
  UINT32            BaseAddr;
  UINT32            BiosRegionSize;
  UINT64            ExtBiosDecodeBaseAddress;
  UINT64            ExtBiosDecodeLimitAddress;
  HPET_GENERAL_CAPABILITIES_ID_REGISTER HpetGeneralCapabilities;
  UINT64_STRUCT     Data64;

  if ((mFeatureImplemented[1] & HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;

  ExtBiosDecodeBaseAddress = 0;
  ExtBiosDecodeLimitAddress = 0;
  SaMchBar        = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  DEBUG ((DEBUG_INFO, "      SaMchBar =  0x%016lx\n", SaMchBar));
  DEBUG ((DEBUG_INFO, "      Lockable/Fixed Memory Ranges Overlap Test\n"));

  //
  // LT Space Fixed Memory Range
  //

  //
  // Locate the SPI protocol.
  //
  Status = gBS->LocateProtocol (
                  &gPchSpiProtocolGuid,
                  NULL,
                  (VOID **) &mSpiProtocol
                  );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_ERROR,"  Failed to locate gPchSpiProtocolGuid Protocol\n"));
    DEBUG ((DEBUG_ERROR,"    Unable to compute High BIOS Memory Size [3], it may incorrectly overlap\n"));
    BiosRegionSize = 0;
  } else {
    mSpiProtocol->GetRegionAddress (mSpiProtocol, FlashRegionBios, &BaseAddr, &BiosRegionSize);
  }

  //
  // High BIOS Memory Range
  // BIOS Direct Read range cannot be greater than 16 MB
  // Memory BIOS can access via direct read is set by BIOS Decode Enable Register (BDE)
  // which just enables segments in addresses 4G -16MB to 4G
  //
  DEBUG ((DEBUG_INFO, "        High BIOS Flash Region Size = 0x%x\n", BiosRegionSize));
  if (BiosRegionSize < SIZE_16MB) {
    mLockableMemoryRange[LockableMemoryRangeHighBios].Base  = SIZE_4GB - BiosRegionSize;
  } else {
    mLockableMemoryRange[LockableMemoryRangeHighBios].Base  = SIZE_4GB - SIZE_16MB;
  }
  mLockableMemoryRange[LockableMemoryRangeLowDram].End = ((UINT32) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase)) - 1;
  //
  // High DRAM Memory Range
  //
  mLockableMemoryRange[LockableMemoryRangeHighDram].End = GetHostBridgeRegisterData (HostBridgeTouud, HostBridgeTouudBase) - 1;
  //
  // MCHBAR (SA) Memory Range
  //
  mLockableMemoryRange[LockableMemoryRangeMchBar].Base = SaMchBar;
  mLockableMemoryRange[LockableMemoryRangeMchBar].End  = mLockableMemoryRange[LockableMemoryRangeMchBar].Base + SIZE_32KB - 1;

  //
  // VTDBAR (Gfx + Non-Gfx) Memory Range (VTDPVC0BAR)
  //
  if (IsVtdEngineEnabled (GLOBAL_VTD)) {
    mLockableMemoryRange[LockableMemoryRangeVtdPvc0Bar].Base = ReadVtdBar ();
    mLockableMemoryRange[LockableMemoryRangeVtdPvc0Bar].End  = mLockableMemoryRange[LockableMemoryRangeVtdPvc0Bar].Base + SIZE_128KB - 1;
  }

  //
  // REGBAR Memory Range
  //
  Data64.Data = MmioRead64 (SaMchBar + GetRegBar ());
  if ((Data64.Data & BIT0) != 0) {
    mLockableMemoryRange[LockableMemoryRangeRegBar].Base = Data64.Data & B_SA_MCHBAR_REGBAR_MASK_64B;
    mLockableMemoryRange[LockableMemoryRangeRegBar].End = mLockableMemoryRange[LockableMemoryRangeRegBar].Base + GetRegBarSize () - 1;
  }
  //
  // SAFBAR Memory Range Fixed
  //
  SaSafBar = GetHostBridgeRegisterData (HostBridgeSafBar, SafBarCfgBase);
  mLockableMemoryRange[LockableMemoryRangeSafBar].Base = SaSafBar;
  mLockableMemoryRange[LockableMemoryRangeSafBar].End = mLockableMemoryRange[LockableMemoryRangeSafBar].Base + V_SA_SAFBAR_REGBAR_SIZE - 1;

  //
  // Local APIC Memory Range
  //
  mLockableMemoryRange[LockableMemoryRangeLocalApic].Base = AsmReadMsr64 (MSR_IA32_APIC_BASE) & LOCAL_APIC_MSR_MASK;
  mLockableMemoryRange[LockableMemoryRangeLocalApic].End = mLockableMemoryRange[LockableMemoryRangeLocalApic].Base + SIZE_4KB - 1;

  //
  // I/O APIC Memory Range
  //
  mLockableMemoryRange[LockableMemoryRangeIoApic].Base = PcdGet32 (PcdSiIoApicBaseAddress);
  mLockableMemoryRange[LockableMemoryRangeIoApic].End = mLockableMemoryRange[LockableMemoryRangeIoApic].Base + SIZE_4KB - 1;

  //
  // HPET Memory Range
  //
  HpetGeneralCapabilities.Uint64  = MmioRead32 (PcdGet32 (PcdSiHpetBaseAddress) + HPET_GENERAL_CAPABILITIES_ID_OFFSET);
  mLockableMemoryRange[LockableMemoryRangeHpet].Base = PcdGet32 (PcdSiHpetBaseAddress);
  mLockableMemoryRange[LockableMemoryRangeHpet].End = mLockableMemoryRange[LockableMemoryRangeHpet].Base + HpetGeneralCapabilities.Bits.NumberOfTimers * SIZE_4KB - 1;

  //
  // SOC P2SB Memory Range
  //
  mLockableMemoryRange[LockableMemoryRangeP2sbBar].Base = PCH_PCR_BASE_ADDRESS;
  mLockableMemoryRange[LockableMemoryRangeP2sbBar].End = mLockableMemoryRange[LockableMemoryRangeP2sbBar].Base + PCH_PCR_MMIO_SIZE - 1;

  //
  // SOC P2SB 2 Memory Range
  //
  mLockableMemoryRange[LockableMemoryRangeP2sb2Bar].Base = PCH_PCR_SECOND_BASE_ADDRESS;
  mLockableMemoryRange[LockableMemoryRangeP2sb2Bar].End = mLockableMemoryRange[LockableMemoryRangeP2sb2Bar].Base + PCH_PCR_MMIO_SIZE - 1;

  //
  // PCI Express CFG Memory Range
  //
  mLockableMemoryRange[LockableMemoryRangePcieCfg].Base = PcdGet64 (PcdSiPciExpressBaseAddress);
  mLockableMemoryRange[LockableMemoryRangePcieCfg].End = mLockableMemoryRange[LockableMemoryRangePcieCfg].Base + PcdGet32 (PcdTemporaryPciExpressRegionLength) - 1;

  //
  // Extended Bios Decode Memory Range
  //
  if (SpiIsExtendedBiosRegionEnabled () == TRUE) {
    IocGetExtendedBiosRegionRange (&ExtBiosDecodeBaseAddress, &ExtBiosDecodeLimitAddress);
    mLockableMemoryRange[LockableMemoryRangeExtBiosDecodeBar].Base = ExtBiosDecodeBaseAddress;
    mLockableMemoryRange[LockableMemoryRangeExtBiosDecodeBar].End  = ExtBiosDecodeLimitAddress;
  }

  //
  // Check mLockableMemoryRange for overlaps
  //
  DEBUG ((DEBUG_INFO, "        INFO: Lockable Memory Ranges:\n"));
  DumpRange (mLockableMemoryRange, LockableMemoryRangeMax);
  if (CheckOverlap (mLockableMemoryRange, LockableMemoryRangeMax)) {
  DEBUG((DEBUG_INFO, "        Unexpected Status: Fixed MMIO Regions Overlap\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_CODE_2,
      HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_UNEXP_STATUS_STRING_2
      );
    Result = FALSE;
  }

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               1,
               HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION
               );
    CheckStatusForHstiLibSet (Status);
  }

  return;
}
