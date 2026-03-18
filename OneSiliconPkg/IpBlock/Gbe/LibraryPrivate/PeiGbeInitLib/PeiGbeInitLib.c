/** @file
  Initializes GBE Device in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MeInitLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/GbeLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PmcPrivateLib.h>
#include "PchResetPlatformSpecific.h"
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/GbeRegs.h>
#include <GbeHandle.h>

#define GBE_DEFAULT_MAC_RAL          0x88888888
#define GBE_DEFAULT_MAC_RAH          0x8887

//
//  GBE Controller Default (On-Reset) DID value.
//
#define V_GBE_CFG_DID_RESET_VALUE    0x15FC

/**
  Check whether GbE is enabled in PMC.

  @param[in]  GbeHandle      Pointer to GBE handle structure
**/
STATIC
BOOLEAN
PmcIsGbeEnabled (
  IN GBE_HANDLE          *GbeHandle
  )
{
  //
  // Is GbE disabled by PMC strap/fuse
  //
  if (GbeHandle->Callback->IsSupported) {
    if (!GbeHandle->Callback->IsSupported (GbeHandle->Controller)) {
      return FALSE;
    }
  } else {
    return FALSE;
  }
  //
  // Is GbE disabled by static power gating
  //
  if (GbeHandle->Callback->IsFunctionDisabled) {
    if (GbeHandle->Callback->IsFunctionDisabled (GbeHandle->Controller)) {
      return FALSE;
    }
  } else {
    return FALSE;
  }

  return TRUE;
}

/**
  Schedules global reset required to change GbE static power gating state.

  @param[in]  GbeHandle      Pointer to GBE handle structure
**/
STATIC
VOID
GbeGlobalReset (
  GBE_CONTROLLER             *GbeController
  )
{
  PCH_RESET_DATA             ResetData;

  DEBUG ((DEBUG_INFO, "GbeGlobalReset - Seg: %02x, Bus: %02x, Dev: %02x, Func: %02x\n",
      GbeController->Segment, GbeController->Bus, GbeController->Device, GbeController->Function));

  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);

  PmcEnableCf9GlobalReset ();
  SiScheduleResetSetType (EfiResetPlatformSpecific, &ResetData);
}

/**
  Disable the Internal GbE Controller

  BIOS has the ability to disable the LAN Controller functionality, in addition to
  configuring PMC to disable the LAN Controller BIOS need to set Disable bit in the
  LANDISCTRL register and then lock the LANDISCTRL register by setting the Lock bit in
  the LOCKLANDIS register.

  @param[in]  GbeHandle      Pointer to GBE handle structure
**/
STATIC
VOID
DisableGbe (
  IN GBE_HANDLE          *GbeHandle
  )
{
  UINT64  GbePciBase;

  DEBUG ((DEBUG_INFO, "Disabling internal LAN...\n"));

  GbePciBase = GbeHandle->Controller->PciCfgBaseAddr;

  ASSERT (PciSegmentRead32 (GbePciBase) != 0xFFFFFFFF);

  //
  // Put GbE into D3 PS, GbE PCI config + 0xCC bits[0:1] to 11
  //
  PciSegmentOr32 (GbePciBase + R_GBE_CFG_PM1, B_GBE_CFG_PM1_PWR_STATE);

  //
  // Set LANDISCTRL, GbE PCI config + 0xA0 bit[0] to 1
  //
  PciSegmentOr32 (GbePciBase + R_GBE_CFG_LANDISCTRL, B_GBE_CFG_LANDISCTRL_DISABLE);
  ASSERT (PciSegmentRead32 (GbePciBase) != 0xFFFFFFFF);

  //
  // Set LOCKLANDIS, GbE PCI config + 0xA4 bit[0] to 1
  //
  PciSegmentOr32 (GbePciBase + R_GBE_CFG_LOCKLANDIS, B_GBE_CFG_LOCKLANDIS_LOCK);

  //
  // LAN function is hidden now
  //
  ASSERT (PciSegmentRead32 (GbePciBase) == 0xFFFFFFFF);

  //
  // Before modifying LAN Disable bit, make sure it's not locked and not disabled yet
  //
  if (GbeHandle->Callback->IsFunctionDisabled &&
     !GbeHandle->Callback->IsFunctionDisabled (GbeHandle->Controller)) {
    //
    // Disable GbE Controller (static PG)
    //
    if (GbeHandle->Callback->Disable) {
      GbeHandle->Callback->Disable (GbeHandle->Controller);
    }
  }
}

/**
  Checks if RAL/RAH contain default Mac address.

  @param[in]  GbeMemBar      GbE MMMIO Address

  @retval TRUE    Default Mac Address
  @retval FALSE   Custom Mac Address
**/
STATIC
BOOLEAN
GbeIsDefaultMacAddress (
  IN   UINTN          GbeMemBar
  )
{
  if ((MmioRead32 (GbeMemBar + R_GBE_MEM_RAL_0) == GBE_DEFAULT_MAC_RAL) &&
      (((MmioRead32 (GbeMemBar + R_GBE_MEM_RAH_0) & 0xFFFF) == GBE_DEFAULT_MAC_RAH))) {
    return TRUE;
  }
  return FALSE;
}

/**
  Assign MMIO resource to GbE controller and enable memory space decoding.

  @param[in]  GbePciBase   GbE PCI Base Address
  @param[in]  GbeMemBar    GbE MMIO Address
**/
STATIC
VOID
SetGbeMmioBase (
  IN UINT64  GbePciBase,
  IN UINTN   GbeMemBar
  )
{
  PciSegmentAnd16 (GbePciBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (GbePciBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) GbeMemBar);
  if (PciSegmentRead32 (GbePciBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (GbePciBase + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (GbeMemBar, 32));
  }
  PciSegmentOr16 (GbePciBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Disable memory space decoding and clear GbE MMIO BAR.

  @param[in]  GbePciBase   GbE PCI Base Address
**/
STATIC
VOID
ClearGbeMmioBase (
  IN UINT64  GbePciBase
  )
{
  PciSegmentAnd16 (GbePciBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentWrite32 (GbePciBase + PCI_BASE_ADDRESSREG_OFFSET, 0);
  if (PciSegmentRead32 (GbePciBase + PCI_COMMAND_OFFSET) & BIT2) {
    PciSegmentWrite32 (GbePciBase + PCI_COMMAND_OFFSET + 4, 0);
  }
}

/**
  Sets GbE Lock Write Flash

  @param[in]  GbePciBase     GbE PCI Base Address
  @param[in]  GbeMemBar      GbE MMIO Address
**/
VOID
GbeLockWriteFlash (
  IN   UINT64         GbePciBase,
  IN   UINTN          GbeMemBar
  )
{
  EFI_STATUS      Status;
  EFI_BOOT_MODE   BootMode;
  BOOLEAN         MacValid;

  SetGbeMmioBase (GbePciBase, GbeMemBar);
  MacValid = FALSE;

  //
  // Lock write Flash when all 3 conditions are met:
  // 1. GbE Mac Assigned
  // 2. Out of Manufacturing Mode
  // 3. No capsule update
  //
  if (( PciSegmentRead16 (GbePciBase + R_GBE_CFG_DID)  != V_GBE_CFG_DID_RESET_VALUE)) {
    MacValid = !GbeIsDefaultMacAddress (GbeMemBar);
    DEBUG ((DEBUG_INFO, "%a: MAC address valid: %d\n", __FUNCTION__, MacValid));
  } else {
    // If BIOS was not able to read mac address it assumes it is valid
    MacValid = TRUE;
  }

  if (MacValid == TRUE) {
    Status = PeiServicesGetBootMode (&BootMode);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: Failed to get the current boot mode.\n", __FUNCTION__));
      ClearGbeMmioBase (GbePciBase);
      return;
    }
    if ((BootMode != BOOT_ON_FLASH_UPDATE) && IsEom ()) {
      MmioOr32 (GbeMemBar + R_GBE_MEM_FEXTNVM12, B_GBE_MEM_FEXTNVM12_LOCK_WRITE_TO_FLASH);
      DEBUG ((DEBUG_INFO, "%a: Write Flash is locked.\n", __FUNCTION__));
    }
  }

  DEBUG ((DEBUG_INFO, "%a: FEXTNVM12 Offset: 0x%X Value: 0x%X.\n", __FUNCTION__, GbeMemBar + R_GBE_MEM_FEXTNVM12, MmioRead32 (GbeMemBar + R_GBE_MEM_FEXTNVM12)));
  ClearGbeMmioBase (GbePciBase);
}

/**
  Configure LAN device registers based on policy settings.

  @param[in]  GbeHandle      Pointer to GBE handle structure
**/
STATIC
VOID
ConfigureGbeRegisters (
  IN  GBE_HANDLE          *GbeHandle
  )
{
  UINT64          GbePciBase;
  UINTN           GbeMemBar;
  GBE_CONFIG      *GbeConfig;
  UINT8           InterruptPin;

  DEBUG ((DEBUG_INFO, "ConfigureGbeRegisters\n"));

  GbeConfig = GbeHandle->GbeConfig;
  GbePciBase = GbeHandle->Controller->PciCfgBaseAddr;

  ASSERT (GbeConfig->Enable);
  ASSERT (PciSegmentRead32 (GbePciBase) != 0xFFFFFFFF);

  //
  // Set GbE PCI config + 0x80 (CPCE) bit[5] (HAE) = 1
  //
  PciSegmentOr32 (GbePciBase + R_GBE_CFG_CPCE, B_GBE_CFG_CPCE_HW_AUTONOM_EN);

  //
  // Set GbE PCI config + 0x94 (clock control of GBE) to 0x1F
  // To avoid blocking PCH floor power measurement when GbE enabled.
  //
  PciSegmentWrite8 (GbePciBase + R_GBE_CFG_CLCTL, 0x1F);

  //
  // Set LOCKLANDIS, GbE PCI config + 0xA4 bit[0]
  //
  PciSegmentOr32 (GbePciBase + R_GBE_CFG_LOCKLANDIS, B_GBE_CFG_LOCKLANDIS_LOCK);

  //
  //
  // LAN LTR Programming
  // The maximum snoop/non-snoop platform latency values to 3ms
  // Program GbE controller's PCI LTR capability register at D31:F6:Reg 0A8h
  //
  if (GbeConfig->LtrEnable == TRUE) {
    PciSegmentWrite32 (GbePciBase + R_GBE_CFG_LTRC, 0x10031003);
  }

  ///
  /// Configure GbE interrupt
  ///
  if (GbeHandle->PrivateConfig->InterruptPinSupported) {
    InterruptPin = GbeHandle->PrivateConfig->InterruptPin;

    PciSegmentWrite8 (GbePciBase + PCI_INT_PIN_OFFSET, InterruptPin);
  }

  if (!GbeHandle->PrivateConfig->SleEnabled) {
    //
    // Assign MMIO resource to Gbe controller
    //
    GbeMemBar = GbeHandle->Mmio;
    SetGbeMmioBase (GbePciBase, GbeMemBar);

    //
    // Additional GbE Controller Configurations for WOL Support
    // System BIOS requires to program the registers listed below for internal GbE.
    // 1. Set MBARA + 2Ch[31] = 1b
    // 2. If WOL is enabled set MBARA + Offset 2Ch[30] = 1b
    //    Else if disabled set MBARA + 2Ch[30] = 0b
    //
    if (GbeHandle->PrivateConfig->WolEnableOverride) {
      MmioOr32 (GbeMemBar + R_GBE_MEM_FEXT, B_GBE_MEM_FEXT_WOL_ENABLE | B_GBE_MEM_FEXT_WOL_VALID);
    } else {
      MmioAndThenOr32 (GbeMemBar + R_GBE_MEM_FEXT, (UINT32) ~(B_GBE_MEM_FEXT_WOL_ENABLE), B_GBE_MEM_FEXT_WOL_VALID);
    }

    if (GbeHandle->PrivateConfig->HasClkReq) {
      MmioOr32 (GbeMemBar + R_GBE_MEM_FEXTNVM6, B_GBE_MEM_FEXTNVM6_K1_OFF_EN);
      DEBUG((DEBUG_INFO, "K1Off enabled.\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "Error: K1Off requires CLKREQ# support -- disabled!\n"));
    }

    //
    // Enable internal ECC for Gbe Controller
    // Set GbeMemBar + 0x100c [16] to 1b
    // Set GbeMemBar + 0 [19] to 1b
    //
    MmioOr32 (GbeMemBar + R_GBE_MEM_PBECCSTS, B_GBE_MEM_PBECCSTS_ECC_EN);
    MmioOr32 (GbeMemBar + R_GBE_MEM_CTRL, B_GBE_MEM_CTRL_MEHE);

    //
    // Disable memory space decoding
    //
    ClearGbeMmioBase (GbePciBase);
  }
}


/**
  Initialize LAN device.

  Global reset may be perfomed due to static power gating requirements.

  @param[in] GbeHandle             The SI Policy PPI instance

**/
VOID
GbeInit (
  IN GBE_HANDLE          *GbeHandle
  )
{
  GBE_CONTROLLER                  *GbeController;
  GBE_CONFIG                      *GbeConfig;
  GBE_CALLBACK                    *GbeCallback;

  DEBUG ((DEBUG_INFO, "GbeInit() Start\n"));

  GbeController = GbeHandle->Controller;
  GbeConfig = GbeHandle->GbeConfig;
  GbeCallback = GbeHandle->Callback;

  DEBUG ((DEBUG_INFO, "GbePmcEnabled:  %d\n",  PmcIsGbeEnabled (GbeHandle)));
  if (GbeHandle->PrivateConfig->LaneNum != 0xFF) {
    DEBUG ((DEBUG_INFO, "GbeLaneNum: %d\n", GbeHandle->PrivateConfig->LaneNum));
  } else {
    DEBUG ((DEBUG_INFO, "GbeLaneNum: No lanes assigned\n"));
  }
  DEBUG ((DEBUG_INFO, "GbeRegionValid: %d\n",  IsGbeRegionValid ()));
  DEBUG ((DEBUG_INFO, "GbePresent:     %d\n",  IsGbePresent (GbeHandle->PwrmBase)));

  if (PmcIsGbeEnabled (GbeHandle)) {
    if (GbeConfig->Enable && IsGbePresent (GbeHandle->PwrmBase)) {
      //
      // Apply GbE settings
      //
      ConfigureGbeRegisters (GbeHandle);
    } else {
      //
      // GbE is not power gated by PMC but is not available in the platform or is disabled via policy
      // Perform GbE disabling flow
      //
      DisableGbe (GbeHandle);
      //
      // Global reset is required for GbE to enter static power gating
      //
      GbeGlobalReset(GbeController);
    }
  } else {
    if (GbeConfig->Enable && IsGbePresent (GbeHandle->PwrmBase)) {
      //
      // GbE is power gated but available in the platform and enabled with policy.
      // Perform enabling flow.
      //
      DEBUG ((DEBUG_INFO, "Gbe found disabled and requested to be enabled - clear PG bit in PMC and issue reset!\n"));
      DEBUG ((DEBUG_INFO, "Enabling internal LAN...\n"));

      //
      // Enable the Internal GbE Controller
      //
      if (GbeHandle->Callback->Enable) {
        GbeHandle->Callback->Enable (GbeHandle->Controller);
      }
      //
      // Global reset is required to get GbE out of static power gating
      //
      GbeGlobalReset (GbeController);
    } else {
      if (GbeCallback->DisableClk) {
        GbeCallback->DisableClk (GbeController);
      }
      if (GbeCallback->ModPhyPowerGating) {
        GbeCallback->ModPhyPowerGating (GbeController);
      }

      DEBUG ((DEBUG_INFO, "LAN disabled\n"));
    }
  }
  DEBUG ((DEBUG_INFO, "GbeInit() End\n"));
}

