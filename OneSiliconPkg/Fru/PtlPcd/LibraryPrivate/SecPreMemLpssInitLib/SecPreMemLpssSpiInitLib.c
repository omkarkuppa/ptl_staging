/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Register/LpssSpiRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdSecGpioInitLib.h>
#include <Library/PchInfoLib.h>
#include <IndustryStandard/Pci30.h>
#include <LpssSpiConfig.h>
#include <Library/IoLib.h>
#include <Library/LpssSpiBaseLib.h>

VOID
LpssSpiEnableMse (
  IN UINT64                    PciCfgBase
  )
{
  PciSegmentOr16 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Gets SPI out of reset

  @param[in]  MmioBaseAddress    Base address of SPI MMIO space
**/
STATIC
VOID
LpssSpiGetOutOfReset (
  IN UINTN                    MmioBaseAddress
  )
{

  MmioOr8 (MmioBaseAddress + R_LPSS_SPI_MEM_RESETS, B_LPSS_SPI_MEM_RESETS_RESET_IP | B_LPSS_SPI_MEM_RESETS_RESET_DMA);
}

/**
  Places LPSS device in D0

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
LpssSpiSetD0 (
  IN UINT64                    PciCfgBase
  )
{
  PciSegmentAnd32 (PciCfgBase + R_LPSS_SPI_CFG_PMECTRLSTATUS,  ~(UINT32)B_LPSS_SPI_CFG_PMECTRLSTATUS_POWERSTATE);
}
/**
  Configures LPSS to work on fixed address assignments

  @param[in] PciCfgBase          Pci Config Space Base
  @param[in] FixedBaseAddress    Fixed Base Address for BAR0
**/
VOID
LpssSpiPciSetFixedMmio (
  IN UINT64            PciCfgBase,
  IN UINTN             FixedBaseAddress
  )
{
  // Assign BAR0 and BAR1 (access to Pci Config Space)
  //
  PciSegmentWrite32 ((UINTN) PciCfgBase + R_LPSS_SPI_CFG_BAR, (UINT32) FixedBaseAddress);
  PciSegmentWrite32 ((UINTN) PciCfgBase + R_LPSS_SPI_CFG_BAR_HIGH, 0x0);
}

/**
  Configures Spi LPSS Controller with minimum configuration

  @param[in] SpiNumber         I2c Number
  @param[in] SpiDeviceConfig   A pointer to LPSS Spi Config
  @param[in] LpssSpiMmioBase   Lpss Spi Mmio base
**/
VOID
SecLpssSpiConfiguration (
  IN UINT8                     SpiNumber,
  IN LPSS_SPI_DEVICE_CONFIG    *SpiDeviceConfig,
  IN UINTN                     LpssSpiMmioBase
  )
{ 
  UINT64                       PciCfgBase;
  UINT16                       VendorId;

  if (SpiNumber >= GetMaxSpiInterfacesNum()) {
    return;
  }
  PciCfgBase      = LpssSpiPciCfgBase (SpiNumber);
  VendorId        = PciSegmentRead16 ((UINTN)(PciCfgBase + PCI_VENDOR_ID_OFFSET));
  if (VendorId == 0xFFFF) {
    return;
  }

  LpssSpiPciSetFixedMmio (PciCfgBase, LpssSpiMmioBase);
  LpssSpiSetD0 (PciCfgBase);
  LpssSpiEnableMse (PciCfgBase);
  LpssSpiGetOutOfReset (LpssSpiMmioBase);
  LpssSpiChipSelectConfig (
      PciCfgBase,
      SpiDeviceConfig->DefaultCsOutput,
      SpiDeviceConfig->CsMode,
      SpiDeviceConfig->CsState,
      (SpiDeviceConfig->CsEnable [0] |
      (SpiDeviceConfig->CsEnable [1] << 1)),
      (SpiDeviceConfig->CsPolarity [0] |
      (SpiDeviceConfig->CsPolarity [1] << 1))
      );

  PtlPcdPreMemLpssSpiGpioConfigure (SpiNumber, SpiDeviceConfig);
}