/**@file
  Source code for the board PCH configuration Pcd init functions for Pre-Memory Init phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Include/PlatformBoardId.h>
#include <PlatformOpenBoardConfig.h>
#include <Library/PcdLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchLimits.h>

/**
  Board Root Port Clock Info configuration init function for PEI pre-memory phase.

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PtlRootPortClkInfoInit (
  VOID
  )
{
  PCD64_BLOB Clock[PCH_MAX_PCIE_CLOCKS];
  UINT32 Index;

  PCIE_CLOCKS_USAGE *PcieClocks;
  PcieClocks = NULL;

  //
  //The default clock assignment will be NOT_USED, which corresponds to PchClockUsageNotUsed. This will prevent clocks drawing Power by default.
  //If Platform code doesn't contain port-clock map for a given board, the clocks will be NOT_USED, preventing PCIe devices not to operate.
  //To prevent this, remember to provide port-clock map for every board.
  //
  for (Index = 0; Index < PCH_MAX_PCIE_CLOCKS; Index++) {
    Clock[Index].PcieClock.ClkReqSupported = TRUE;
    Clock[Index].PcieClock.ClockUsage = NOT_USED;
  }

  ///
  /// Assign ClkReq signal to root port. (Base 0)
  /// For LP, Set 0 - 5
  /// For H,  Set 0 - 15
  /// For S,  Set 0 - 17
  /// Note that if GbE is enabled, ClkReq assigned to GbE will not be available for Root Port.
  ///

  PcieClocks = PcdGetPtr (VpdPcdPcieClkUsageMap);
  if (PcieClocks == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Clock[0].PcieClock.ClockUsage  = PcieClocks->ClockUsage[0];
  Clock[1].PcieClock.ClockUsage  = PcieClocks->ClockUsage[1];
  Clock[2].PcieClock.ClockUsage  = PcieClocks->ClockUsage[2];
  Clock[3].PcieClock.ClockUsage  = PcieClocks->ClockUsage[3];
  Clock[4].PcieClock.ClockUsage  = PcieClocks->ClockUsage[4];
  Clock[5].PcieClock.ClockUsage  = PcieClocks->ClockUsage[5];
  Clock[6].PcieClock.ClockUsage  = PcieClocks->ClockUsage[6];

  PcdSet64S (PcdPcieClock0,  Clock[ 0].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock1,  Clock[ 1].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock2,  Clock[ 2].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock3,  Clock[ 3].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock4,  Clock[ 4].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock5,  Clock[ 5].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock6,  Clock[ 6].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock7,  Clock[ 7].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock8,  Clock[ 8].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock9,  Clock[ 9].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock10, Clock[10].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock11, Clock[11].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock12, Clock[12].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock13, Clock[13].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock14, Clock[14].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  PcdSet64S (PcdPcieClock15, Clock[15].Blob); // @todo Those individual PCDs should probably be replaced with something like a VOID* that can be iterated over
  return EFI_SUCCESS;
}

/**
  Board USB related configuration init function for PEI pre-memory phase.

  @param[in]  VOID

  @retval EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
PtlUsbConfigInit (
  VOID
  )
{

  return EFI_SUCCESS;
}


/**
  Board GPIO Group Tier configuration init function for PEI pre-memory phase.
**/
VOID
PtlBoardGroupTierInit (
  VOID
  )
{
  return;
}
