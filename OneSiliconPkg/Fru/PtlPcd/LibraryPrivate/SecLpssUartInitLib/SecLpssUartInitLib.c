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

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/LpssUartRegs.h>
#include <Library/LpssUartLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/LpssRegs.h>
#include <Library/PciSegmentLib.h>
#include <Defines/PchReservedResources.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdSecGpioInitLib.h>
#include <Library/PchInfoLib.h>

/**
  Allows memory access

  @param[in] PciCfgBase       Pci Config Offset
  @param[in] Hidden           Mode that determines access type

**/
VOID
LpssEnableMse (
  IN UINT64                    PciCfgBase
  )
{
  PciSegmentOr16 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Gets UART out of reset

  @param[in]  MmioBaseAddress    Base address of UART MMIO space
**/
STATIC
VOID
SecLpssUartGetOutOfReset (
  IN UINTN            MmioBaseAddress
  )
{

  MmioOr8 (MmioBaseAddress + R_LPSS_UART_MEM_RESETS, B_LPSS_UART_MEM_RESETS_RESET_IP | B_LPSS_UART_MEM_RESETS_RESET_DMA);
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
  PciSegmentAnd32 (PciCfgBase + R_LPSS_UART_CFG_PMECTRLSTATUS,  ~(UINT32)B_LPSS_UART_CFG_PMECTRLSTATUS_POWERSTATE);
}

/**
  Configures LPSS to work on fixed address assignments
  Disables Pci Enumaration for given device in PSF

  @param[in] PciCfgBase          Pci Config Space Base
  @param[in] FixedBaseAddress    Fixed Base Address for BAR0
  @param[in] FixedPciCfgAddress  Fixed Pci Config Space for BAR1
  @param[in] PciConfgCtrAddr     Register offset for each LPSS
  @param[in] PsfPort             Psf Port data

**/
VOID
SecLpssPciSetFixedMmio (
  IN UINT64          PciCfgBase,
  IN UINT32          FixedBaseAddress
  )
{
  // Assign BAR0 and BAR1 (access to Pci Config Space)
  //
  PciSegmentWrite32 (PciCfgBase + R_LPSS_UART_CFG_BAR, FixedBaseAddress);
  PciSegmentWrite32 (PciCfgBase + R_LPSS_UART_CFG_BAR_HIGH, 0x0);
  // Enable MSE and set D0 before placing device in Hidden Mode, otherwise memory will not map
  //
  LpssSetD0 (PciCfgBase);
  LpssEnableMse (PciCfgBase);
}
/**
  Configures LPSS Controller with minimum configuration in ACPI mode

  @param[in] UartNumber        UART Number
  @param[in] UartDeviceConfig  LPSS UART Config
**/
VOID
SecLpssUartConfiguration (
  IN UINT8                        UartNumber,
  IN LPSS_UART_DEVICE_CONFIG      *UartDeviceConfig                     
  )
{
  //
  // This is to prevent from overflow of array access.
  //
  if (UartNumber >= GetMaxUartInterfacesNum()) {
    return;
  }

  SecLpssPciSetFixedMmio (LpssUartPciCfgBase (UartNumber), GetLpssUartFixedMmioAddress (UartNumber));
  SecLpssUartGetOutOfReset (GetLpssUartFixedMmioAddress (UartNumber));
  LpssUartSetAttributes (
    GetLpssUartFixedMmioAddress (UartNumber),
    UartDeviceConfig->Attributes.BaudRate,
    UartDeviceConfig->Attributes.Parity,
    UartDeviceConfig->Attributes.DataBits,
    UartDeviceConfig->Attributes.StopBits,
    UartDeviceConfig->Attributes.AutoFlow
  );
  PtlPcdSecLpssUartGpioConfigure (UartDeviceConfig, UartNumber);
}

