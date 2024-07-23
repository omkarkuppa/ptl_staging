/** @file
  Initialize I3C Controllers.

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
#include <Library/LpssI3cPrivateLib.h>
#include <Library/IoLib.h>
#include <IndustryStandard/Pci.h>
#include <Register/LpssI3cRegs.h>
#include <LpssI3cHandle.h>

/**
  Places LPSS device in D3

  @param[in] I3cHandle         The LPSS I3C Handle instance

**/
VOID
LpssI3cSetD3 (
  IN LPSS_I3C_HANDLE  *I3cHandle
  )
{
  I3cHandle->Controller->PcieCfgAccess->Or32 (I3cHandle->Controller->PcieCfgAccess, R_LPSS_I3C_CFG_PMECTRLSTATUS, B_LPSS_I3C_CFG_PMECTRLSTATUS_POWERSTATE);

  //
  // Original code for reference:
  //
    // LPSS_PWRMG_CONTROL_AND_STATUS  PwrMgmControlStsReg;
    // if (PciCfgBase < LpssI3cGetReservedSerialIoMmio ()) {
    //   PwrMgmControlStsReg.Data32 = PciSegmentRead32 (PciCfgBase + R_LPSS_CFG_PMECTRLSTATUS);
    //   PwrMgmControlStsReg.Fields.PwrState = 0x3; // Place Device in D3 State
    //   PciSegmentWrite32 (PciCfgBase + R_LPSS_CFG_PMECTRLSTATUS, PwrMgmControlStsReg.Data32);
    // } else {
    //   PwrMgmControlStsReg.Data32 = MmioRead32 ((UINTN) PciCfgBase + R_LPSS_CFG_PMECTRLSTATUS);
    //   PwrMgmControlStsReg.Fields.PwrState = 0x3; // D3 State
    //   MmioWrite32 ((UINTN) PciCfgBase + R_LPSS_CFG_PMECTRLSTATUS, PwrMgmControlStsReg.Data32);
    //   //
    //   // Reading back value after write to ensure bridge observes the BAR1 write access
    //   //
    //   MmioRead8 ((UINTN) PciCfgBase + R_LPSS_CFG_PMECTRLSTATUS);
    // }
}

/**
  Configures LPSS I3C Controller

  @param[in] I3cHandle        The LPSS I3C Handle instance

  @retval     EFI_SUCCESS      Initialization completed successfully
              EFI_UNSUPPORTED  Usupported I3C controller mode
**/
EFI_STATUS
LpssI3cInit (
  IN LPSS_I3C_HANDLE    *I3cHandle
  )
{
  REGISTER_ACCESS             *I3cCfgAccess;

  I3cCfgAccess = I3cHandle->Controller->PcieCfgAccess;

  //
  // Do not modify a device that has already been disabled/hidden
  //
  if (I3cCfgAccess->Read16 (I3cCfgAccess, 0) == 0xFFFF) {
    return EFI_UNSUPPORTED;
  }

  //
  //  Configure Power gating
  //
  I3cCfgAccess->Or32 (I3cCfgAccess, R_LPSS_I3C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG,
                  B_LPSS_I3C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_PMCRE |
                  B_LPSS_I3C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_DEVIDLEN |
                  B_LPSS_I3C_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_D3HEN);

  switch (I3cHandle->I3cConfig->Mode) {
    case I3cDisabled:
      //
      //  Set Device to D3
      //
      LpssI3cSetD3 (I3cHandle);
      return EFI_SUCCESS;;
    case I3cPci:
      break;
    default:
      return EFI_UNSUPPORTED;
  }

  //
  // No usage in BIOS/PreOS - Setting D3
  //
  LpssI3cSetD3 (I3cHandle);

  //
  // No usage in BIOS/PreOS - Disable MMIO
  //
  I3cCfgAccess->And16 (I3cCfgAccess, PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
  I3cCfgAccess->Write32 (I3cCfgAccess, R_LPSS_I3C_CFG_BAR,  0x0);
  if (I3cCfgAccess->Read32 (I3cCfgAccess, R_LPSS_I3C_CFG_BAR) & BIT2) {
    I3cCfgAccess->Write32 (I3cCfgAccess, R_LPSS_I3C_CFG_BAR_HIGH, 0x0);
  }

  return EFI_SUCCESS;
}
