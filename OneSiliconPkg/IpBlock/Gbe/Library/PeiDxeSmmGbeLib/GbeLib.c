/** @file
  Gbe Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SocInfoLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/GbeMdiLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PmcLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/GbeRegs.h>

/**
  Check whether GbE region is valid
  Check SPI region directly since GbE might be disabled in SW.

  @retval TRUE                    Gbe Region is valid
  @retval FALSE                   Gbe Region is invalid
**/
BOOLEAN
IsGbeRegionValid (
  VOID
  )
{
  return SpiIsGbeRegionValid ();
}

/**
  Check whether GBE controller is enabled in the platform.

  @retval TRUE                    GbE is enabled
  @retval FALSE                   GbE is disabled
**/
BOOLEAN
IsGbePresent (
  IN UINTN  PmcPwrmBase
  )
{
  //
  // Check PMC strap/fuse
  //
  if (!PmcIsGbeSupported (PmcPwrmBase)) {
    return FALSE;
  }
  //
  // Check GbE NVM
  //
  if (IsGbeRegionValid () == FALSE) {
    return FALSE;
  }

  return TRUE;
}

/**
  Verifies Gigabit Ethernet PCI Class Code

  @param [in]  GbePciCfgBase      GbE PCI Config Space Address

  @retval TRUE                    GbE Class Code match
  @retval FALSE                   GbE Class Code does not match
**/
BOOLEAN
STATIC
GbeCheckClassCode (
  UINT64 GbePciCfgBase
  )
{
  UINT8 BaseCode;
  UINT8 SubClassCode;

  SubClassCode  = PciSegmentRead8 (GbePciCfgBase + PCI_CLASSCODE_OFFSET + 1);
  BaseCode      = PciSegmentRead8 (GbePciCfgBase + PCI_CLASSCODE_OFFSET + 2);

  if ((BaseCode != PCI_CLASS_NETWORK) || (SubClassCode != PCI_CLASS_NETWORK_ETHERNET)) {
    return FALSE;
  }
  return TRUE;
}

/**
  Checks if Gbe is Enabled or Disabled

  @param [in]  GbePciCfgBase    GbE PCI Config Space Address

  @retval  BOOLEAN    TRUE if device is enabled, FALSE otherwise.
**/
BOOLEAN
IsGbeEnabled (
  IN UINT64 GbePciCfgBase
  )
{
  if (PciSegmentRead32 (GbePciCfgBase) != 0xFFFFFFFF) {
    return GbeCheckClassCode (GbePciCfgBase);
  }

  return FALSE;
}

/**
  Gets Phy Revision and Model Number

  @param [in]  GbePciCfgBase    GbE PCI Config Space Address
  @param [out] LanPhyRevision   Return Value

  @return EFI_SUCCESS           Phy Revision was obtained
          EFI_NOT_FOUND         Gbe is disabled or not present
          EFI_DEVICE_ERROR      Gbe class code check failed
          EFI_INVALID_PARAMETER When GbeBar is incorrect
                                When Phy register or address is out of bounds
**/
EFI_STATUS
GbeGetLanPhyRevision (
  IN   UINT64  GbePciCfgBase,
  OUT  UINT16  *LanPhyRevision
  )
{
  EFI_PCI_PMCSR   GbePciPmCsr;
  EFI_PCI_PMCSR   GbePciPmCsrTemp;
  UINT8           GbePciCmd;
  UINT32          GbeBar;
  EFI_STATUS      Status;

  if (PciSegmentRead32 (GbePciCfgBase) == 0xFFFFFFFF) {
    return EFI_NOT_FOUND;
  }

  if (!GbeCheckClassCode (GbePciCfgBase)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Put Gbe in D0
  //
  GbePciPmCsr.Data = PciSegmentRead16 (GbePciCfgBase + R_GBE_CFG_PM1);
  GbePciPmCsrTemp.Data = GbePciPmCsr.Data;
  GbePciPmCsrTemp.Bits.PowerState = 0;

  PciSegmentWrite16 (GbePciCfgBase + R_GBE_CFG_PM1, GbePciPmCsrTemp.Data);

  GbeBar = PciSegmentRead32 (GbePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFE0000;
  if (GbeBar == 0) {
    return EFI_DEVICE_ERROR;
  }
  GbePciCmd = PciSegmentRead8 (GbePciCfgBase + PCI_COMMAND_OFFSET);

  //
  // Overwrite previous state of the command register
  //
  PciSegmentWrite8 ((GbePciCfgBase + PCI_COMMAND_OFFSET), EFI_PCI_COMMAND_MEMORY_SPACE);

  Status = GbeMdiGetLanPhyRevision (GbeBar, LanPhyRevision);

  //
  // Restore command register
  //
  PciSegmentWrite8 ((GbePciCfgBase + PCI_COMMAND_OFFSET), GbePciCmd);

  //
  // Restore PMCS
  //
  PciSegmentWrite16 (GbePciCfgBase + R_GBE_CFG_PM1, GbePciPmCsr.Data);

  return Status;
}

