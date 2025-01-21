/** @file
  Initializes LPSS I2C Controllers.

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
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <IndustryStandard/Pci.h>
#include <Register/LpssI2cRegs.h>
#include <LpssI2cHandle.h>

/**
  Lpss Pci Device State structure.
  Used to preserve current information about the device when it is configured in Pci mode prior to Pch Initialization.
**/
typedef struct {
  UINT64 PciCfgBar0;       ///< Pci Config Space Base Address Register
  UINT8  PciCfgCommand;    ///< Pci Config Space Command Register
  UINT8  PciCfgPmeCtrlSts; ///< Pci Config Space Pme Control Status
  UINT8  PprReset;         ///< MMIO Proprietary Reset Register
} LPSS_I2C_PCI_DEVICE_STATE;

/**
  Enables Lpss I2C Power Gating

  @param[in] I2cCtrl    LPSS I2C Controller structure
**/
VOID
LpssI2cEnablePowerGating (
  IN LPSS_I2C_CONTROLLER *I2cCtrl
  )
{
  //
  // Pmc Request Enable
  // Device Idle Enable
  // D3-Hot Enable (D3HEN) / Power Gate Enable
  //
  I2cCtrl->PcieCfgAccess->Or32 (I2cCtrl->PcieCfgAccess, R_LPSS_I2C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG,
                              B_LPSS_I2C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_PMCRE |
                              B_LPSS_I2C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_DEVIDLEN |
                              B_LPSS_I2C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_D3HEN);
}

/**
  Places Lpss I2c device in D3

  @param[in] I2cCtrl    LPSS I2C Controller structure
**/
VOID
LpssI2cSetD3 (
  IN LPSS_I2C_CONTROLLER *I2cCtrl
  )
{
  //
  // Place Device in D3 State
  //
  I2cCtrl->PcieCfgAccess->Or32 (I2cCtrl->PcieCfgAccess, R_LPSS_I2C_CFG_PMECTRLSTATUS, B_LPSS_I2C_CFG_PMECTRLSTATUS_POWERSTATE);
}

/**
  Used to preserve current information about the device when it is configured in Pci mode in Pch Initialization.

  @param[in] I2cCtrl    LPSS I2C Controller structure
  @param[in/out] State         Pointer to state settings container

**/
VOID
LpssI2cPciSave (
  IN      LPSS_I2C_CONTROLLER       *I2cCtrl,
  IN OUT LPSS_I2C_PCI_DEVICE_STATE   *State
  )
{

  State->PciCfgBar0 = (UINT64) (I2cCtrl->PcieCfgAccess->Read32 (I2cCtrl->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFF000);
  State->PciCfgBar0 |= LShiftU64 (I2cCtrl->PcieCfgAccess->Read32 (I2cCtrl->PcieCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4), 32);

  if (State->PciCfgBar0 == 0x0) {
    return; // No save/restore required
  }
  State->PciCfgPmeCtrlSts = I2cCtrl->PcieCfgAccess->Read8 (I2cCtrl->PcieCfgAccess, R_LPSS_I2C_PCR_PMECTRLSTATUS);
  State->PciCfgCommand    = I2cCtrl->PcieCfgAccess->Read8 (I2cCtrl->PcieCfgAccess, PCI_COMMAND_OFFSET);
  State->PprReset         = MmioRead8 ((UINTN) (State->PciCfgBar0 + R_LPSS_I2C_MEM_RESETS));
}

/**
  Enable Lpss I2C controller in PCI Mode
  - MMIO decoding
  - DO state
  - Out of reset

  @param[in] I2cCtrl          LPSS I2C Controller structure

  @retval TRUE                Base address was obtained successfully
          FALSE               Failed to obtain Base Address Register address
**/
BOOLEAN
LpssI2cPciModeEnable (
  IN LPSS_I2C_CONTROLLER  *I2cCtrl
  )
{
  if (I2cCtrl->PcieCfgAccess->Read32 (I2cCtrl->PcieCfgAccess, PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return FALSE;
  }

  I2cCtrl->PcieCfgAccess->And16 (I2cCtrl->PcieCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  I2cCtrl->PcieCfgAccess->Write32 (I2cCtrl->PcieCfgAccess, R_LPSS_I2C_CFG_BAR, (UINT32) I2cCtrl->MmioTmpBar);
  if (I2cCtrl->PcieCfgAccess->Read32 (I2cCtrl->PcieCfgAccess, R_LPSS_I2C_CFG_BAR) & BIT2) {
    I2cCtrl->PcieCfgAccess->Write32 (I2cCtrl->PcieCfgAccess, R_LPSS_I2C_CFG_BAR_HIGH, (UINT32) RShiftU64 (I2cCtrl->MmioTmpBar, 32));
  }

  //
  // Enable MSE
  //
  I2cCtrl->PcieCfgAccess->Or16 (I2cCtrl->PcieCfgAccess, PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Set Device to D0 state
  //
  I2cCtrl->PcieCfgAccess->And32 (I2cCtrl->PcieCfgAccess, R_LPSS_I2C_PCR_PMECTRLSTATUS, ~(UINT32)B_LPSS_I2C_PCR_PMECTRLSTATUS_POWERSTATE);

  //
  // Get controller out of reset:
  // - Reset I2C Host Controller
  // - Reset DMA
  MmioOr32 (I2cCtrl->MmioTmpBar + R_LPSS_I2C_MEM_RESETS, B_LPSS_I2C_MEM_RESETS_RESET_IP | B_LPSS_I2C_MEM_RESETS_RESET_DMA);

  return TRUE;
}

/**
  Configures Lpss I2c Controller

  @param[in] I2cHandle        The Lpss I2c Handle instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       Usupported I2C controller mode
              EFI_DEVICE_ERROR      Unable to enable I2C PCI Mode
**/
EFI_STATUS
LpssI2cInit (
  IN LPSS_I2C_HANDLE    *I2cHandle
  )
{
  LPSS_I2C_PCI_DEVICE_STATE   PciState;
  REGISTER_ACCESS             *I2cCfgAccess;

  I2cCfgAccess = I2cHandle->Controller->PcieCfgAccess;

  LpssI2cEnablePowerGating (I2cHandle->Controller);

  switch (I2cHandle->I2cConfig->Mode) {
    case LpssI2cDisabled:
      LpssI2cSetD3 (I2cHandle->Controller);
      return EFI_SUCCESS;
    case LpssI2cPci:
      LpssI2cPciSave (I2cHandle->Controller, &PciState);
      I2cCfgAccess->Write32 (I2cCfgAccess, R_LPSS_I2C_CFG_BAR1, 0x0);
      I2cCfgAccess->Write32 (I2cCfgAccess, R_LPSS_I2C_CFG_BAR1_HIGH, 0x0);
      if (LpssI2cPciModeEnable (I2cHandle->Controller) == FALSE) {
        DEBUG ((DEBUG_ERROR, "ERROR: Unable to enable PCI Mode for I2C!\n"));
        return EFI_DEVICE_ERROR;
      }
      if (PciState.PciCfgBar0 == 0x0) {
        LpssI2cSetD3 (I2cHandle->Controller);
        //
        // Disable MSE
        //
        I2cCfgAccess->And16 (I2cCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
        I2cCfgAccess->Write32 (I2cCfgAccess, R_LPSS_I2C_CFG_BAR,  0x0);
        I2cCfgAccess->Write32 (I2cCfgAccess, R_LPSS_I2C_CFG_BAR_HIGH,  0x0);
      } else {
        //
        // Restore PCI state saved earlier this makes sure that I2C configuration done during SEC phase
        // remains intact.
        //
        MmioWrite8 ((UINTN) (PciState.PciCfgBar0 + R_LPSS_I2C_MEM_RESETS), PciState.PprReset);
        I2cCfgAccess->Write32 (I2cCfgAccess, PCI_BASE_ADDRESSREG_OFFSET, (UINT32) (PciState.PciCfgBar0 & 0xFFFFF000));
        if (I2cCfgAccess->Read32 (I2cCfgAccess, PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
          I2cCfgAccess->Write32 (I2cCfgAccess, PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (PciState.PciCfgBar0, 32));
        }
        I2cCfgAccess->Write8 (I2cCfgAccess, R_LPSS_I2C_PCR_PMECTRLSTATUS, PciState.PciCfgPmeCtrlSts);
        I2cCfgAccess->Write8 (I2cCfgAccess, PCI_COMMAND_OFFSET, PciState.PciCfgCommand);
      }
      break;
    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
