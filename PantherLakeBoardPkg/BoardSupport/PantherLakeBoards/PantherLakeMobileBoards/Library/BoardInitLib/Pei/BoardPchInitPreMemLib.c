/** @file
 Source code for the board PCH configuration Pcd init functions for Pre-Memory Init phase.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BoardConfigLib.h>

#include <Library/PreSiliconEnvDetectLib.h>
#include <Include/PlatformBoardId.h>
#include <PlatformBoardConfig.h>
#include <Library/PcdLib.h>
#include <PlatformBoardId.h>
#include <Library/PchInfoLib.h>
#include <Library/PlatformUsbConfigLib.h>
#include <Library/PcdInfoLib.h>
#include <SetupVariable.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>

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

  PcieClocks = NULL;

      if (PtlIsPcdP ()) {
        PcieClocks = PcdGetPtr (VpdPcdPPcieClkUsageMap);
      } else {
          PcieClocks = PcdGetPtr (VpdPcdPcieClkUsageMap); // PTL-H
      }
      if (PcieClocks == NULL) {
        return EFI_INVALID_PARAMETER;
      }

  DEBUG ((DEBUG_INFO, "Max Pcie Clock Number = %d\n", GetPchMaxPcieClockNum ()));
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    Clock[Index].PcieClock.ClockUsage = PcieClocks->ClockUsage[Index];
  }

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
  //
  // USB2 PHY settings.
  //
  PcdSet64S (PcdUsb2PhyTuningTable, (UINTN) PcdGetPtr (VpdPcdUsb2PhyTuningTable));

  if (GetUsbConnectorHobData () == NULL) {
    BuildUsbConnectorHob ();
  }

  if (GetUsbCConnectorHobData () == NULL) {
    BuildUsbCConnectorHob ();
  }

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
  VPD_GPIO_PAD *GpioVpd;

  GpioVpd = NULL;

  //
  // GPIO Group Tier Setting. If there is no value updated in Vpd, it would leave Tier1 as default.
  //

  GpioVpd = PcdGetPtr (VpdPcdGpioGroupToGpeDw0);
  PcdSet32S (PcdGpioGroupToGpeDw0, GpioVpd->GpioPad);

  GpioVpd = PcdGetPtr (VpdPcdGpioGroupToGpeDw1);
  PcdSet32S (PcdGpioGroupToGpeDw1, GpioVpd->GpioPad);

  GpioVpd = PcdGetPtr (VpdPcdGpioGroupToGpeDw2);
  PcdSet32S (PcdGpioGroupToGpeDw2, GpioVpd->GpioPad);
  return;
}

/**
  GPIO init function for PEI pre-memory phase.

  @param[in]  BoardId   An unsigned integrer represent the board id.

  @retval EFI_SUCCESS   The function completed successfully.
**/
VOID
PtlGpioTablePreMemInit (
  VOID
  )
{
  GPIOV2_INIT_CONFIG  *GpioTable = (GPIOV2_INIT_CONFIG*)NULL;
  UINT16            GpioCount = 0;
  VPD_GPIO_PAD      *GpioVpd;

  GpioVpd = NULL;

  SETUP_DATA                       Setup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI* VariableServices;
  EFI_STATUS                       Status;
  UINTN                            VariableSize;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,  // GUID
             0,                                 // INSTANCE
             NULL,                              // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices        // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &Setup
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get setup variable \n"));
  }

  //
  // Configure WWAN Full Card Power Off and reset pins
  //
  GpioVpd = PcdGetPtr (VpdPcdWwanFullCardPowerOffGpio);
  PcdSet32S (PcdWwanFullCardPowerOffGpio, GpioVpd->GpioPad);
  GpioVpd = PcdGetPtr (VpdPcdWwanBbrstGpio);
  PcdSet32S (PcdWwanBbrstGpio, GpioVpd->GpioPad);
  GpioVpd = PcdGetPtr (VpdPcdWwanPerstGpio);
  PcdSet32S (PcdWwanPerstGpio, GpioVpd->GpioPad);
  GpioVpd = PcdGetPtr (VpdPcdWwanWakeGpio);
  PcdSet32S (PcdWwanWakeGpio, GpioVpd->GpioPad);
  PcdSetBoolS (PcdWwanPerstGpioPolarity, PcdGetBool (VpdPcdWwanPerstGpioPolarity));
  PcdSetBoolS (PcdWwanFullCardPowerOffGpioPolarity, PcdGetBool (VpdPcdWwanFullCardPowerOffGpioPolarity));
  PcdSetBoolS (PcdWwanBbrstGpioPolarity, PcdGetBool (VpdPcdWwanBbrstGpioPolarity));
  // When WWAN is enabled in setup power enable pin use WWAN FCP pin
  GpioVpd = PcdGetPtr (VpdPcdWwanFullCardPowerOffGpio);
  PcdSet32S (PcdWwanFullCardPowerOffGpio, GpioVpd->GpioPad);

  //
  // GPIO Table Init, Update M80Wwan PreMem GPIO table to PcdBoardGpioTableM80WwanOnEarlyPreMem
  //
  GpioTable = (GPIOV2_INIT_CONFIG*)NULL;
  GpioTable = PcdGetPtr (VpdPcdBoardGpioTableM80WwanOnEarlyPreMem);
  PcdSet64S (PcdBoardGpioTableM80WwanOnEarlyPreMem, (UINTN)GpioTable);
  GpioCount = 0;
  GetGpioTableSize (GpioTable, &GpioCount);
  PcdSet16S (PcdBoardGpioTableM80WwanOnEarlyPreMemSize, GpioCount);

  //
  // GPIO Table Init, Update enable BT Audio offload PreMem GPIO table to PcdBoardGpioTableEnableDiscreteAudioOffloadPreMem
  //
  GpioTable = (GPIOV2_INIT_CONFIG*) NULL;
  GpioTable = PcdGetPtr (VpdPcdBoardGpioTableEnableDiscreteAudioOffloadPreMem);
  PcdSet64S (PcdBoardGpioTableEnableDiscreteAudioOffloadPreMem, (UINT64) GpioTable);
  GpioCount = 0;
  GetGpioTableSize (GpioTable, &GpioCount);
  PcdSet16S (PcdBoardGpioTableEnableDiscreteAudioOffloadPreMemSize, GpioCount);
  //
  // GPIO Table Init, Update disable BT Audio offload PreMem GPIO table to PcdBoardGpioTableDisableDiscreteAudioOffloadPreMem
  //
  GpioTable = (GPIOV2_INIT_CONFIG*) NULL;
  GpioTable = PcdGetPtr (VpdPcdBoardGpioTableDisableDiscreteAudioOffloadPreMem);
  PcdSet64S (PcdBoardGpioTableDisableDiscreteAudioOffloadPreMem, (UINT64) GpioTable);
  GpioCount = 0;
  GetGpioTableSize (GpioTable, &GpioCount);
  PcdSet16S (PcdBoardGpioTableDisableDiscreteAudioOffloadPreMemSize, GpioCount);

  //
  // GPIO Table Init, Update PreMem and Early Premem GPIO table to PcdBoardGpioTablePreMem and PcdBoardGpioTableEarlyPreMem
  //
  GpioTable = (GPIOV2_INIT_CONFIG *)PcdGetPtr (VpdPcdBoardGpioTablePreMem);
  ConfigureGpioTabletoPCD (GpioTable , PRE_MEM);

  GpioTable = (GPIOV2_INIT_CONFIG *)PcdGetPtr (VpdPcdBoardGpioTableEarlyPreMem);
  ConfigureGpioTabletoPCD (GpioTable, EARLY_PRE_MEM);

  return;
}
