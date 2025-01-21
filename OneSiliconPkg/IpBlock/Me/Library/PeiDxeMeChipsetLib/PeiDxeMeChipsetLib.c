/** @file
  Me Chipset Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#include <IndustryStandard/Pci22.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DebugLib.h>
#include <Library/MeInfoLib.h>
#include <Library/TimerLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Register/HeciRegs.h>
#include <Register/PchRegs.h>
#include <PcieRegs.h>
#include <IntelRcStatusCode.h>
#include <MeDefines.h>

/**
  Checks if the given PCIe ME Device Function is HECI Device Function

  @param[in]     Function  ME Device Function

  @retval TRUE   This is a HECI Device Function
  @retval FALSE  This is not a HECI Device Function
**/
BOOLEAN
IsHeciDeviceFunction (
  IN HECI_DEVICE Function
  )
{
  switch (Function) {
    case HECI1:
    case HECI2:
    case HECI3:
    case HECI4:
      return TRUE;

    default:
      return FALSE;
  }
}

/**
  Take ME device out of D0I3

  @param[in] Function  ME function where D0I3 is to be changed
**/
VOID
ClearD0I3Bit (
  IN  HECI_DEVICE   Function
  )
{
  UINT64       DevicePciCfgBase;
  UINT64       DeviceBar;
  UINT8        Cmd;
  UINTN        Timeout;
  UINT32       D0I3Ctrl;

  DEBUG ((DEBUG_INFO, "[HECI%d] Clearing D0I3 bit\n", HECI_DEVICE_TO_NUMBER (Function)));
  ///
  /// Get Device MMIO address
  ///
  DevicePciCfgBase = MeHeciPciCfgBase (Function);
  if (PciSegmentRead16 (DevicePciCfgBase + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "[HECI%d] Function is disabled, cannot clear D0I3 bit!\n", HECI_DEVICE_TO_NUMBER (Function)));
    return;
  }

  DeviceBar = PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  if ((PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    DeviceBar += LShiftU64 ((UINT64)PciSegmentRead32 (DevicePciCfgBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 32);
  }

  if (DeviceBar == 0) {
    DEBUG ((DEBUG_WARN, "MMIO Bar for HECI%d isn't programmed\n", HECI_DEVICE_TO_NUMBER (Function)));
    return;
  }

  ///
  /// Put CSME Device out of R_ME_MEM_DEVIDLEC
  /// (1) Poll R_ME_MEM_DEVIDLEC[0] CIP bit is 0 with timeout 5 seconds
  /// (2) Write R_ME_MEM_DEVIDLEC[2] = 0
  /// (3) Poll R_ME_MEM_DEVIDLEC[0] CIP bit is 0 with timeout 5 seconds
  ///
  Cmd = PciSegmentRead8 (DevicePciCfgBase + PCI_COMMAND_OFFSET);
  if ((Cmd & EFI_PCI_COMMAND_MEMORY_SPACE) != EFI_PCI_COMMAND_MEMORY_SPACE) {
    PciSegmentOr8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  D0I3Ctrl = MmioRead32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC);
  if ((D0I3Ctrl & B_ME_MEM_DEVIDLEC_DEVIDLE) == B_ME_MEM_DEVIDLEC_DEVIDLE) {
    ///
    /// (1) If entering D0I3 is in progress wait till it finishes. Let's give it 5000 ms timeout.
    ///
    Timeout = 5000;
    while ((D0I3Ctrl & B_ME_MEM_DEVIDLEC_CIP) == B_ME_MEM_DEVIDLEC_CIP && Timeout-- > 0) {
      MicroSecondDelay (1000);
      D0I3Ctrl = MmioRead32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC);
    }

    ///
    /// (2) Write R_ME_MEM_DEVIDLEC[2] = 0
    ///
    MmioWrite32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC, D0I3Ctrl & ~B_ME_MEM_DEVIDLEC_DEVIDLE);

    D0I3Ctrl = MmioRead32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC);
    ///
    /// (3) If exiting D0I3 is in progress wait till it finishes. Let's give it 5000 ms timeout.
    ///
    Timeout = 5000;
    while ((D0I3Ctrl & B_ME_MEM_DEVIDLEC_CIP) == B_ME_MEM_DEVIDLEC_CIP && Timeout-- > 0) {
      MicroSecondDelay (1000);
      D0I3Ctrl = MmioRead32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC);
    }
  }
  DEBUG ((DEBUG_INFO, "[HECI%d] R_ME_MEM_DEVIDLEC register = %08X\n", HECI_DEVICE_TO_NUMBER (Function), MmioRead32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC)));
  PciSegmentWrite8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, Cmd);
}


/**
  Put ME device into D0I3

  @param[in] Function          Select of Me device
**/
VOID
SetD0I3Bit (
  IN  HECI_DEVICE   Function
  )
{
  UINT64       DevicePciCfgBase;
  UINT64       DeviceBar;
  UINT8        Cmd;
  BOOLEAN      IsTempBar;

  IsTempBar = FALSE;

  DEBUG ((DEBUG_INFO, "[HECI%d] Setting D0I3 bit\n", HECI_DEVICE_TO_NUMBER (Function)));
  ///
  /// Get Device MMIO address
  ///
  DevicePciCfgBase = MeHeciPciCfgBase (Function);
  if (PciSegmentRead16 (DevicePciCfgBase + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "[HECI%d] Function is disabled, can't set D0I3 bit!\n", HECI_DEVICE_TO_NUMBER (Function)));
    return;
  }
  if ((PciSegmentRead8 (DevicePciCfgBase + R_ME_CFG_HIDM) & B_ME_CFG_HIDM_MODE) != V_ME_CFG_HIDM_MSI) {
    DEBUG ((DEBUG_WARN, "[HECI%d] HIDM is not legacy/MSI, do not set D0I3 bit!\n", HECI_DEVICE_TO_NUMBER (Function)));
    return;
  }

  DeviceBar = PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  if ((PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    DeviceBar += LShiftU64 ((UINT64)PciSegmentRead32 (DevicePciCfgBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 32);
  }

  if (DeviceBar == 0) {
    DEBUG ((DEBUG_WARN, "MMIO Bar for HECI%d isn't programmed, setting temporary BAR.\n", HECI_DEVICE_TO_NUMBER (Function)));
    IsTempBar = TRUE;
    DeviceBar = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
    PciSegmentWrite32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) DeviceBar);
    PciSegmentWrite32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + 0x4, 0);
  }

  ///
  /// Put CSME Device in D0I3
  ///
  Cmd = PciSegmentRead8 (DevicePciCfgBase + PCI_COMMAND_OFFSET);
  if ((Cmd & EFI_PCI_COMMAND_MEMORY_SPACE) != EFI_PCI_COMMAND_MEMORY_SPACE) {
    PciSegmentOr8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }
  MmioOr32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC, B_ME_MEM_DEVIDLEC_DEVIDLE);
  DEBUG ((DEBUG_INFO, "[HECI%d] R_ME_MEM_DEVIDLEC register = %08x\n", HECI_DEVICE_TO_NUMBER (Function), MmioRead32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC)));

  PciSegmentWrite8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, Cmd);

  if (IsTempBar) {
    PciSegmentWrite32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET, 0);
  }
}

/**
  Get D0I3 status bit

  @param[in] Function          Select of Me device

  @retval    TRUE              D0I3 status bit is enabled
  @retval    FALSE             D0I3 status bit is not enabled
**/
BOOLEAN
IsD0I3BitEnabled (
  IN  HECI_DEVICE   Function
  )
{
  UINT64       DevicePciCfgBase;
  UINT64       DeviceBar;
  UINT32       D0I3Ctrl;
  BOOLEAN      Status;
  UINT8        Cmd;

  Status = FALSE;

  ///
  /// Get Device MMIO address
  ///
  DevicePciCfgBase = MeHeciPciCfgBase (Function);
  if (PciSegmentRead16 (DevicePciCfgBase + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "[HECI%d] Function is disabled, unable to access D0I3 bit!\n", HECI_DEVICE_TO_NUMBER (Function)));
    return Status;
  }
  if ((PciSegmentRead8 (DevicePciCfgBase + R_ME_CFG_HIDM) & B_ME_CFG_HIDM_MODE) != V_ME_CFG_HIDM_MSI) {
    DEBUG ((DEBUG_WARN, "[HECI%d] HIDM is not legacy/MSI, unable to access D0I3 bit!\n", HECI_DEVICE_TO_NUMBER (Function)));
    return Status;
  }

  DeviceBar = PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  if ((PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    DeviceBar += LShiftU64 ((UINT64)PciSegmentRead32 (DevicePciCfgBase + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 32);
  }

  if (DeviceBar == 0) {
    DEBUG ((DEBUG_WARN, "MMIO Bar for HECI%d isn't programmed\n", HECI_DEVICE_TO_NUMBER (Function)));
    return Status;
  }

  ///
  /// Get D0I3 status bit
  ///
  Cmd = PciSegmentRead8 (DevicePciCfgBase + PCI_COMMAND_OFFSET);
  if ((Cmd & EFI_PCI_COMMAND_MEMORY_SPACE) != EFI_PCI_COMMAND_MEMORY_SPACE) {
    PciSegmentOr8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }

  D0I3Ctrl = MmioRead32 ((UINTN) DeviceBar + R_ME_MEM_DEVIDLEC);
  if ((D0I3Ctrl & B_ME_MEM_DEVIDLEC_DEVIDLE) == B_ME_MEM_DEVIDLEC_DEVIDLE) {
    DEBUG ((DEBUG_INFO, "[HECI%d] R_ME_MEM_DEVIDLEC bit is set.\n", HECI_DEVICE_TO_NUMBER (Function)));
    Status = TRUE;
  }

  PciSegmentWrite8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, Cmd);
  return Status;
}
