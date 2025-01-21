/** @file
  Low Power Subsystem (LPSS) initialization library

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

#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/LpssInitLib.h>
#include <IndustryStandard/Pci.h>
#include <Register/PchRegs.h>
#include <Register/LpssRegs.h>
#include <PcrDefine.h>
#include <Library/IoLib.h>

/**
  Checks if higher than PCIe function zero are enabled.
  Used for Function 0 Lpss Device disabling

  @param[in] Seg    PCIe Segment
  @param[in] Bus    PCIe Bus
  @param[in] Dev    PCIe Devie

  @retval TRUE            At least one higher function device is enabled
          FALSE           Higher functions are disabled
**/
BOOLEAN
LpssHigherFunctionsEnabled (
  IN  UINT8   Seg,
  IN  UINT8   Bus,
  IN  UINT8   Dev
  )
{
  UINT8   FuncNum;
  UINT16  DevId;
  //
  // Check all other func devs(1 to 7) status except func 0.
  //
  for (FuncNum = 1; FuncNum <= PCI_MAX_FUNC; FuncNum++) {
    DevId = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (Seg, Bus, Dev, FuncNum, 0));
    if (DevId != 0xFFFF) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Print LPSS Device configuration data

  @param[in] LpssDev   Pointer to LPSS devices list
**/
STATIC
VOID
LpssPrintDeviceSettings (
  IN LPSS_DEVICE   *LpssDev
  )
{
  DEBUG ((DEBUG_INFO, "\tName:             %a\n",     LpssDev->Name));
  DEBUG ((DEBUG_INFO, "\tPCI Device:       %d\n",     LpssDev->PciDev));
  DEBUG ((DEBUG_INFO, "\tPCI Function:     %d\n",     LpssDev->PciFun));
  DEBUG ((DEBUG_INFO, "\tInterface Index:  %d\n",     LpssDev->InterfaceIndex));
  DEBUG ((DEBUG_INFO, "\tMode:             %d\n",     LpssDev->Mode));
  DEBUG ((DEBUG_INFO, "\tPciCfgCtrlOffset: 0x%04X\n", LpssDev->PciCfgCtrlOffset));

  DEBUG ((DEBUG_INFO, "\tInterruptPin:     %d\n",     LpssDev->InterruptPin));
  DEBUG ((DEBUG_INFO, "\tIrq:              %d\n",     LpssDev->Irq));
}

/**
  Enables ACPI mode for given Lpss device

  @param[in] PcrAccess  LPSS Subsystem regisers access protocol
  @param[in] LpssDev    LPSS Device
**/
STATIC
VOID
LpssAcpiModeEnable (
  IN REGISTER_ACCESS  *PcrAccess,
  IN LPSS_DEVICE      *LpssDev
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  if (LpssDev->Mode != ModeAcpi && LpssDev->Mode != ModeCom) {
    DEBUG ((DEBUG_ERROR, "ERROR: Invalid ACPI Mode for %a device!\n", LpssDev->Name));
    return;
  }

  // - Set PCI Config Space Disable bit
  // - Set ACPI Interrupt Enable bit
  PcrAccess->Or32 (PcrAccess, LpssDev->PciCfgCtrlOffset,
                  (B_LPSS_PCR_PCICFGCTR1_PCI_CFG_DIS1 |
                  B_LPSS_PCR_PCICFGCTR1_ACPI_INTR_EN1));
}

/**
  Enables PCI mode for given Lpss device

  @param[in] PcrAccess  LPSS Subsystem regisers access protocol
  @param[in] LpssDev    LPSS Device
**/
STATIC
VOID
LpssPciModeEnable (
  IN REGISTER_ACCESS  *PcrAccess,
  IN LPSS_DEVICE      *LpssDev
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  if (LpssDev->Mode != ModePci) {
    DEBUG ((DEBUG_ERROR, "ERROR: Invalid PCI Mode for %a device!\n", LpssDev->Name));
    return;
  }

  // - Clear PCI Config Space Disable bit
  // - Clear ACPI Interrupt Enable bit
  PcrAccess->And32 (PcrAccess, LpssDev->PciCfgCtrlOffset,
                  ~(UINT32)(B_LPSS_PCR_PCICFGCTR1_PCI_CFG_DIS1 |
                  B_LPSS_PCR_PCICFGCTR1_ACPI_INTR_EN1));
}

/**
  Enables COM mode for given Lpss UART device

  @param[in] LpssDev    LPSS Device
  @param[in] PcrAccess  LPSS Subsystem regisers access protocol
**/
STATIC
VOID
LpssComModeEnable (
  IN REGISTER_ACCESS  *PcrAccess,
  IN LPSS_DEVICE      *LpssDev
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  if (LpssDev->Mode != ModeCom) {
    DEBUG ((DEBUG_ERROR, "ERROR: Invalid COM Mode for %a device!\n", LpssDev->Name));
    return;
  }

  PcrAccess->Or32 (PcrAccess, R_LPSS_PCR_GEN_PVT_HIGH_REGRW3, (V_LPSS_PCR_GEN_PVT_HIGH_REGRW3_UART_BYTE_ADDR_EN << LpssDev->InterfaceIndex));
}

/**
  Enables Base Address Register 1 (MMIO BAR1) for Lpss device

  @param[in] LpssDev    LPSS Device
  @param[in] PcrAccess  LPSS Subsystem regisers access protocol
**/
VOID
LpssBar1Enable (
  IN REGISTER_ACCESS  *PcrAccess,
  IN LPSS_DEVICE      *LpssDev
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  // Clear BAR 1 Disable bit
  PcrAccess->And32 (PcrAccess, LpssDev->PciCfgCtrlOffset, ~(UINT32)B_LPSS_PCR_PCICFGCTR1_BAR1_DISABLE1);

  if (LpssDev->PsfBar1Enable != NULL) {
    DEBUG ((DEBUG_INFO, "%a: PSF BAR 1 Enable\n", __FUNCTION__));
    LpssDev->PsfBar1Enable (LpssDev->InterfaceIndex);
  }
}

/**
  Disables Base Address Register 1 (MMIO BAR1) for Lpss device

  @param[in] LpssDev    LPSS Device
  @param[in] PcrAccess  LPSS Subsystem regisers access protocol
**/
VOID
LpssBar1Disable (
  IN REGISTER_ACCESS  *PcrAccess,
  IN LPSS_DEVICE      *LpssDev
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  // Set BAR 1 Disable bit
  PcrAccess->Or32 (PcrAccess, LpssDev->PciCfgCtrlOffset, B_LPSS_PCR_PCICFGCTR1_BAR1_DISABLE1);
}

/**
  Configures Lpss  devices interrupt pin and IRQ assignment

  @param[in] LpssDev    LPSS Device
  @param[in] PcrAccess  LPSS Subsystem registers access protocol
**/
VOID
LpssInterruptSet (
  IN REGISTER_ACCESS  *PcrAccess,
  IN LPSS_DEVICE      *LpssDev
  )
{
  UINT32  DataAnd;
  UINT32  DataOr;

  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  DataAnd = ~(UINT32) (B_LPSS_PCR_PCICFGCTR1_IPIN1 | B_LPSS_PCR_PCICFGCTR1_PCI_IRQ1 | B_LPSS_PCR_PCICFGCTR1_ACPI_IRQ1);

  if (LpssDev->Mode == ModePci || LpssDev->Mode == ModeSkipInit || LpssDev->Mode == ModeDisabled) {
    DataOr = LpssDev->InterruptPin << N_LPSS_PCR_PCICFGCTR1_IPIN1 | LpssDev->Irq << N_LPSS_PCR_PCICFGCTR1_PCI_IRQ1;
  } else if (LpssDev->Mode == ModeAcpi || LpssDev->Mode == ModeCom) {
    DataOr = LpssDev->InterruptPin << N_LPSS_PCR_PCICFGCTR1_IPIN1 | LpssDev->Irq << N_LPSS_PCR_PCICFGCTR1_ACPI_IRQ1;
  } else {
    return;
  }
  PcrAccess->AndThenOr32 (PcrAccess, LpssDev->PciCfgCtrlOffset, DataAnd, DataOr);
}

/**
  Enables Power and Clock gating feature for LPSS subsystem

  In order for the Clocks, Clock Gating and Power Gating functions to operate properly,
  the BIOS must set the following CSR bits in the sequence outline below:
  1. 0x604 GEN_REGRW2 (MTL/MTP-S/LNL)(aka GPPRVRW2), 0x604 GEN_PVT_LOW_REGRW2 (PTL onwards)
      * bit 1 (LPSS_PGCB_FRC_CLK_CP_EN)
      * bit 11 (LPSS_CDC_SIDE_CFG_CLKREQ_CTL_EN)
  2. 0x1D0 PMCTL bits 5:0
  3. 0x0A0 D0I3MAXDEVPG]
      * bit 16 (PMCRE),
      * bit 17 (IE3),
      * bit 18 (PGE)
    Note:
        This step is done at IP layer, separately for each IP integrated in LPSS

  @param[in] LpssPcrAccess   LPSS Subsystem registers access protocol
**/
VOID
LpssEnablePowerAndClockGating (
  IN REGISTER_ACCESS  *LpssPcrAccess
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  LpssPcrAccess->Or32 (LpssPcrAccess, R_LPSS_PCR_GEN_PVT_LOW_REGRW2, V_LPSS_PCR_GEN_PVT_LOW_REGRW2_CLK_GATING);
  LpssPcrAccess->Or32 (LpssPcrAccess, R_LPSS_PCR_PMCTL, V_LPSS_PCR_PMCTL_PWR_GATING);
}

/**
  Forces to enable LPSS Clocks

  @param[in] LpssPcrAccess   LPSS Subsystem registers access protocol
**/
VOID
LpssForceClockOn (
  IN REGISTER_ACCESS  *LpssPcrAccess
  )
{
  DEBUG ((DEBUG_INFO, "%a\n", __FUNCTION__));

  //
  // Force Clocks on
  //
  LpssPcrAccess->Or32 (LpssPcrAccess, R_LPSS_PCR_GEN_PVT_LOW_REGRW4, V_LPSS_PCR_GEN_PVT_LOW_REGRW4_SLC_CLK_CGC);
}

/**
  Initializes LPSS subsystem
  Configure all devices integrated in the subsystem.

  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
**/
VOID
LpssInit (
  IN LPSS_SUBSYSTEM   *LpssSubsystem
  )
{
  LPSS_DEVICE  *Dev;
  UINT32       Index;

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  if (LpssSubsystem == NULL) {
    DEBUG ((DEBUG_WARN, "%a: LpssSubsystem is NULL pointer!\n", __FUNCTION__));
    return;
  }

  if (LpssSubsystem->Flags & LpssFlagForceClockOn) {
    LpssForceClockOn (LpssSubsystem->PcrAccess);
  }

  Dev = LpssSubsystem->Devices;
  Index = 0;
  while (Dev != NULL) {
    DEBUG ((DEBUG_INFO, "Initilize LPSS Device %d:\n", Index));
    LpssPrintDeviceSettings (Dev);
    switch (Dev->Mode) {
      case ModePci:
        LpssEnablePowerAndClockGating (LpssSubsystem->PcrAccess);
        LpssPciModeEnable (LpssSubsystem->PcrAccess, Dev);
        LpssBar1Disable (LpssSubsystem->PcrAccess, Dev);
        LpssInterruptSet (LpssSubsystem->PcrAccess, Dev);
        break;
      case ModeAcpi:
        LpssEnablePowerAndClockGating (LpssSubsystem->PcrAccess);
        LpssBar1Enable (LpssSubsystem->PcrAccess, Dev);
        LpssAcpiModeEnable (LpssSubsystem->PcrAccess, Dev);
        LpssInterruptSet (LpssSubsystem->PcrAccess, Dev);
        break;
      case ModeCom:
        LpssEnablePowerAndClockGating (LpssSubsystem->PcrAccess);
        LpssBar1Enable (LpssSubsystem->PcrAccess, Dev);
        LpssAcpiModeEnable (LpssSubsystem->PcrAccess, Dev);
        LpssInterruptSet (LpssSubsystem->PcrAccess, Dev);
        LpssComModeEnable (LpssSubsystem->PcrAccess, Dev);
        break;
      case ModeDisabled:
        LpssBar1Disable (LpssSubsystem->PcrAccess, Dev);
        break;
      case ModeSkipInit:
        LpssInterruptSet (LpssSubsystem->PcrAccess, Dev);
        break;
      default:
        break;
    }
    Index++;
    Dev = Dev->Next;
  }

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));
}

/**
  Function configures LPSS devices disable and hide mode
  in Primary Scalable Fabric (PSF).
  The function uses upper layer (FRU/SOC) function pointer
  to access PSF registers.

  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
  @param[in] PciSegment      PCI Segment assigned for LPSS
  @param[in] PciBus          PCI Bus for assigned LPSS
**/
VOID
LpssFabricFunctionDisable (
  LPSS_SUBSYSTEM  *LpssSubsystem,
  UINT8           PciSegment,
  UINT8           PciBus
  )
{
  LPSS_DEVICE  *Dev;

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  if (LpssSubsystem == NULL) {
    DEBUG ((DEBUG_WARN, "%a: LpssSubsystem is NULL pointer!\n", __FUNCTION__));
    return;
  }

  //
  // Step 1: Disable or Hide all requested functions except function zero
  //
  Dev = LpssSubsystem->Devices;
  while (Dev != NULL) {
    if (Dev->Mode == ModeDisabled || Dev->Mode == ModeAcpi) {
      if (Dev->PciFun == 0) {
        Dev = Dev->Next;
        continue;
      }
      if (Dev->PsfDisable != NULL && Dev->Mode == ModeDisabled) {
        DEBUG ((DEBUG_INFO, "Disable LPSS %a Device in PSF\n", Dev->Name));
        Dev->PsfDisable (Dev->InterfaceIndex);
      }
      if (Dev->PsfPciCfgHide != NULL && Dev->Mode == ModeAcpi) {
        DEBUG ((DEBUG_INFO, "Hide PCI CFG LPSS %a Device in PSF\n", Dev->Name));
        Dev->PsfPciCfgHide (Dev->InterfaceIndex);
      }
    }
    Dev = Dev->Next;
  }

  //
  // Step 2: Disable or Hide all requested functions zero
  //
  Dev = LpssSubsystem->Devices;
  while (Dev != NULL) {
    if (Dev->Mode == ModeDisabled || Dev->Mode == ModeAcpi) {
      if (Dev->PciFun == 0) {
        if (LpssHigherFunctionsEnabled (PciSegment, PciBus, Dev->PciDev) == FALSE) {
          if (Dev->PsfDisable != NULL && Dev->Mode == ModeDisabled) {
            DEBUG ((DEBUG_INFO, "Disable LPSS %a Device in PSF\n", Dev->Name));
            Dev->PsfDisable (Dev->InterfaceIndex);
          }
          if (Dev->PsfPciCfgHide != NULL && Dev->Mode == ModeAcpi) {
            DEBUG ((DEBUG_INFO, "Hide PCI CFG LPSS %a Device in PSF\n", Dev->Name));
            Dev->PsfPciCfgHide (Dev->InterfaceIndex);
          }
        } else {
          // Don't disable the device in PSF if thare are other devices in multi-function PCIe device
          LpssInterruptSet (LpssSubsystem->PcrAccess, Dev);
          DEBUG ((DEBUG_INFO, "Keep LPSS %a Device PSF enabled (PCI Function 0 is required)\n", Dev->Name));
        }
      }
    }
    Dev = Dev->Next;
  }

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));
}
