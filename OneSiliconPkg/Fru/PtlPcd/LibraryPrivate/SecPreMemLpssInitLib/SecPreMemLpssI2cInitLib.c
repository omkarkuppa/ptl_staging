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

#include <Register/LpssI2cRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdSecGpioInitLib.h>
#include <Library/PchInfoLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/IoLib.h>


VOID
LpssEnableMse (
  IN UINT64                    PciCfgBase
  )
{
  PciSegmentOr16 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Gets I2C out of reset

  @param[in]  MmioBaseAddress    Base address of I2C MMIO space
**/
STATIC
VOID
LpssI2cGetOutOfReset (
  IN UINTN                    MmioBaseAddress
  )
{

  MmioOr8 (MmioBaseAddress + R_LPSS_I2C_MEM_RESETS, B_LPSS_I2C_MEM_RESETS_RESET_IP | B_LPSS_I2C_MEM_RESETS_RESET_DMA);
}

/**
  Places LPSS device in D0

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
LpssSetD0 (
  IN UINT64                    PciCfgBase
  )
{
  PciSegmentAnd32 (PciCfgBase + R_LPSS_I2C_CFG_PMECTRLSTATUS,  ~(UINT32)B_LPSS_I2C_CFG_PMECTRLSTATUS_POWERSTATE);
}

/**
  Configures LPSS to work on fixed address assignments

  @param[in] PciCfgBase          Pci Config Space Base
  @param[in] FixedBaseAddress    Fixed Base Address for BAR0
**/
VOID
SecLpssI2cPciSetFixedMmio (
  IN UINT64            PciCfgBase,
  IN UINTN             FixedBaseAddress
  )
{
  // Assign BAR0 and BAR1 (access to Pci Config Space)
  //
  PciSegmentWrite32 ((UINTN) PciCfgBase + R_LPSS_I2C_CFG_BAR, (UINT32) FixedBaseAddress);
  PciSegmentWrite32 ((UINTN) PciCfgBase + R_LPSS_I2C_CFG_BAR_HIGH, 0x0);
}

/**
  Configures I2c LPSS Controller with minimum configuration

  @param[in] I2cNumber         I2c Number
  @param[in] I2cDeviceConfig  LPSS I2c Config
**/
VOID
SecLpssI2cConfiguration (
  IN UINT8                         I2cNumber,
  IN LPSS_I2C_CONTROLLER_CONFIG    *I2cDeviceConfig,
  IN UINTN                         LpssI2cMmioBase
  )
{
  UINT64                           LpssI2cPciBase;
  UINT16                           VendorId;
  if (I2cNumber >= GetMaxI2cInterfacesNum()) {
    return;
  }

  LpssI2cPciBase  = LpssI2cPciCfgBase (I2cNumber);
  VendorId        = PciSegmentRead16 ((UINTN)(LpssI2cPciBase + PCI_VENDOR_ID_OFFSET));
  if (VendorId == 0xFFFF) {
    return;
  }

  SecLpssI2cPciSetFixedMmio (LpssI2cPciBase, LpssI2cMmioBase);
  LpssSetD0 (LpssI2cPciBase);
  LpssEnableMse (LpssI2cPciBase);
  LpssI2cGetOutOfReset (LpssI2cMmioBase);
  PtlPcdPreMemLpssI2cGpioConfigure (I2cNumber, I2cDeviceConfig);
}
