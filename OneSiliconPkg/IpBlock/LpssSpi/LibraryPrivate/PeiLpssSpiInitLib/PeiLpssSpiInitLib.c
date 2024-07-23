/** @file

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

@par Specification Reference:
**/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/LpssSpiLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/LpssSpiRegs.h>
#include <LpssSpiConfig.h>
#include <LpssSpiHandle.h>

/**
  Enables Power Gating

  @param[in] LpssSpiController        Pointer to Lpss Spi Controller
**/
STATIC
VOID
LpssSpiEnablePowerGating (
  LPSS_SPI_CONTROLLER       *LpssSpiController
  )
{
  // Pmc Request Enable
  // Device Idle Enable
  // D3-Hot Enable (D3HEN) / Power Gate Enable
  LpssSpiController->PcieCfgAccess->Or32 (LpssSpiController->PcieCfgAccess, R_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG,
                B_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_PMCRE |
                B_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_DEVIDLEN |
                B_LPSS_SPI_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_D3HEN);
}

/**
  Configures Lpss Spi Controller in Pci Mode

  @param[in] LpssSpiHandle     The LPSS SPI Handle instance
**/
STATIC
VOID
LpssSpiPciModeInit (
  IN LPSS_SPI_HANDLE   *LpssSpiHandle
  )
{
  LPSS_SPI_CONTROLLER                *LpssSpiController;
  LPSS_SPI_DEVICE_CONFIG             *LpssSpiDeviceConfig;

  if (LpssSpiHandle == NULL) {
    return;
  }

  LpssSpiController         = LpssSpiHandle->Controller;
  LpssSpiDeviceConfig       = LpssSpiHandle->SpiConfig;

  //
  // Clear MSE
  //
  LpssSpiController->PcieCfgAccess->And16 (LpssSpiController->PcieCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Clear Bar1
  //
  LpssSpiController->PcieCfgAccess->Write32 (LpssSpiController->PcieCfgAccess, R_LPSS_SPI_CFG_BAR1, 0x0);
  if (LpssSpiController->PcieCfgAccess->Read32 (LpssSpiController->PcieCfgAccess, R_LPSS_SPI_CFG_BAR1) & BIT2) {
    LpssSpiController->PcieCfgAccess->Write32 (LpssSpiController->PcieCfgAccess, R_LPSS_SPI_CFG_BAR1_HIGH, 0x0);
  }

  //
  // Enable MMIO access
  //
  LpssSpiController->PcieCfgAccess->Write32 (LpssSpiController->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET, (UINT32) LpssSpiController->MmioTmpBar);
  if (LpssSpiController->PcieCfgAccess->Read32 (LpssSpiController->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    LpssSpiController->PcieCfgAccess->Write32 (LpssSpiController->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (LpssSpiController->MmioTmpBar, 32));
  }
  LpssSpiController->PcieCfgAccess->Or16 (LpssSpiController->PcieCfgAccess, PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Set D0
  //
  LpssSpiController->PcieCfgAccess->And32 (LpssSpiController->PcieCfgAccess, R_LPSS_SPI_CFG_PMECTRLSTATUS, ~(UINT32)B_LPSS_SPI_CFG_PMECTRLSTATUS_POWERSTATE);

  //
  // Get controller out of reset
  //
  MmioOr32 (LpssSpiController->MmioTmpBar + R_LPSS_SPI_MEM_RESETS, B_LPSS_SPI_MEM_RESETS_RESET_IP | B_LPSS_SPI_MEM_RESETS_RESET_DMA);

  //
  // Chip select configuration
  //
  LpssSpiChipSelectConfig (
    LpssSpiController->MmioTmpBar,
    LpssSpiDeviceConfig->DefaultCsOutput,
    LpssSpiDeviceConfig->CsMode,
    LpssSpiDeviceConfig->CsState,
    (LpssSpiDeviceConfig->CsEnable[0] |
    (LpssSpiDeviceConfig->CsEnable[1] << 1)),
    (LpssSpiDeviceConfig->CsPolarity[0] |
    (LpssSpiDeviceConfig->CsPolarity[1] << 1))
    );

  //
  // D3
  //
  LpssSpiController->PcieCfgAccess->Or32 (LpssSpiController->PcieCfgAccess, R_LPSS_SPI_CFG_PMECTRLSTATUS, B_LPSS_SPI_CFG_PMECTRLSTATUS_POWERSTATE);

  //
  // Disable MMIO access
  //
  LpssSpiController->PcieCfgAccess->Write32 (LpssSpiController->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET, (UINT32) 0x0);
  if (LpssSpiController->PcieCfgAccess->Read32 (LpssSpiController->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    LpssSpiController->PcieCfgAccess->Write32 (LpssSpiController->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) 0x0);
  }
  LpssSpiController->PcieCfgAccess->And16 (LpssSpiController->PcieCfgAccess, PCI_COMMAND_OFFSET, (UINT16)~EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Configures LPSS SPI Controller

  @param[in] LpssSpiHandle    The Lpss Spi Handle instance
**/
EFI_STATUS
LpssSpiInit (
  IN LPSS_SPI_HANDLE         *LpssSpiHandle
  )
{
  LPSS_SPI_CONTROLLER                *LpssSpiController;
  LPSS_SPI_DEVICE_CONFIG             *LpssSpiDeviceConfig;

  LpssSpiController         = LpssSpiHandle->Controller;
  LpssSpiDeviceConfig       = LpssSpiHandle->SpiConfig;

  LpssSpiEnablePowerGating (LpssSpiController);

  //
  // Check access to LPSS SPI over BDF
  //
  if (LpssSpiController->PcieCfgAccess->Read16 (LpssSpiController->PcieCfgAccess, 0) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "LPSS SPI not present - initialization aborted!\n"));
    return EFI_ABORTED;
  }

  switch (LpssSpiDeviceConfig->Mode) {
    case LpssSpiDisabled:
      LpssSpiController->PcieCfgAccess->Or32 (LpssSpiController->PcieCfgAccess, R_LPSS_SPI_CFG_PMECTRLSTATUS, B_LPSS_SPI_CFG_PMECTRLSTATUS_POWERSTATE);
      break;
    case LpssSpiPci:
      LpssSpiPciModeInit (LpssSpiHandle);
      break;
    default:
      DEBUG ((DEBUG_INFO, "Lpss Spi mode: %d not supported!\n%a () - End.\n", LpssSpiDeviceConfig->Mode, __FUNCTION__));
      return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
