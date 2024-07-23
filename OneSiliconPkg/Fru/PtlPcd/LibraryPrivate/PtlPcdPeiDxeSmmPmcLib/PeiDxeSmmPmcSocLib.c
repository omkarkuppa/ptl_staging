/** @file
  PTL PCH PMC SoC library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PmcLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/SataSocLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PmcRegs.h>
#include <Fru/PtlPcd/IncludePrivate/Register/PmcRegsPtlPcd.h>
#include <Register/PchRegs.h>
#include <PmcSocConfig.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <IndustryStandard/Pci.h>
#include <Library/PmcSocLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/HostBridgeRegs.h>
#include <PcieRegs.h>

#define B_PMC_PWRM_IPC_WBUF0_D2D_LINK_FORCE_ON                BIT4
#define B_PMC_PWRM_IPC_WBUF0_D2D_LINK_REMOVE_FORCE_ON         BIT5
#define B_PMC_PWRM_IPC_WBUF0_IOE_DBG_CLK_EN                   BIT9
#define B_PMC_PWRM_IPC_WBUF0_IOE_DBG_CLK_DIS                  BIT10

/**
  This function checks if SCS UFS device is supported (not disabled by fuse)

  @param[in] UfsNum     SCS UFS Device

  @retval SCS device support state
**/
BOOLEAN
PmcIsScsUfsSupported (
  IN UINT32   UfsNum
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1) & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1_UFS) == 0);
}

/**
  This function checks RTC SCRATCH GCR register if SCS UFS is function disabled or not

  @retval SCS UFS disable state
**/
BOOLEAN
PmcIsScsUfsFunctionDisabled (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_RTC_SCRATCH_GCR_0) & B_PMC_PWRM_RTC_SCRATCH_GCR_0_DISABLE_UFS) == 1);
}

/**
  This function enables all SerailIo devices.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcEnableSerialIo (
  IN UINTN  PmcMmio
  )
{
  MmioAnd32 (PmcMmio + R_PMC_PWRM_ST_PG_FDIS_PMC_0_V2, (UINT32)~B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_0_LPSS);
}

/**
  This function disables (static power gating) all SerailIo devices.
  For SerialIo controllers they can be power gated only if all of them are to be disabled.
  They cannot be statically power gated separately.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @param[in] PmcMmio        PMC MMIO
**/
VOID
PmcStaticDisableSerialIo (
  IN UINTN  PmcMmio
  )
{
  MmioOr32 (PmcMmio + R_PMC_PWRM_ST_PG_FDIS_PMC_0_V2, B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_0_LPSS);
}

/**
  This function checks if all SerialIo devices are statically disabled (static power gating)

  @param[in] PmcMmio        PMC MMIO

  @retval SerialIo disable state
**/
BOOLEAN
PmcIsSerialIoStaticallyDisabled (
  IN UINTN  PmcMmio
  )
{
  return ((MmioRead32 (PmcMmio + R_PMC_PWRM_ST_PG_FDIS_PMC_0_V2) & B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_0_LPSS) == B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_0_LPSS);
}

/**
  This function checks if SerialIo device is supported (not disabled by fuse)

  @param[in] PmcMmio        PMC MMIO

  @retval SerialIo support state
**/
BOOLEAN
PmcIsSerialIoSupported (
  IN UINTN  PmcMmio
  )
{
  return ((MmioRead32 (PmcMmio + R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0) & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0_LPSS) == 0);
}

/**
  This function checks if SATA device is supported (not disabled by fuse)

  @param[in] SataCtrlIndex SATA controller index

  @retval SATA support state
**/
BOOLEAN
PmcIsSataSupported (
  UINT32  SataCtrlIndex
  )
{
  return FALSE;
}

/**
  This function checks if ISH is function disabled
  by static power gating

  @retval ISH device state
**/
BOOLEAN
PmcIsIshFunctionDisabled (
  VOID
  )
{
  return ((MmioRead32 ((PmcGetPwrmBase() + R_PMC_PWRM_ST_PG_FDIS_PMC_2_V2)) & B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_2_ISH) != 0);
}

/**
  This function checks if ISH device is supported (not disabled by fuse)

  @retval ISH support state
**/
BOOLEAN
PmcIsIshSupported (
  VOID
  )
{
  return ((MmioRead32 ((PmcGetPwrmBase() + R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD2)) & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD2_ISH) == 0);
}

/**
  This function checks if function disable (static and non-static power gating)
  configuration is locked

  @param[in] PmcMmio       PMC MMIO

  @retval lock state
**/
BOOLEAN
PmcIsFunctionDisableConfigLocked(
  IN UINTN  PmcMmio
)
{
  return ((MmioRead32 (PmcMmio + R_PMC_PWRM_GEN_PMCON_B) & B_PTL_PCD_P_PMC_PWRM_GEN_PMCON_B_ST_FDIS_LK) != 0);
}


/**
  This function disables ISH device by static power gating.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @retval TRUE   Disabled ISH in PMC
  @retval FALSE  Failed to disable ISH in PMC
**/
BOOLEAN
PmcStaticDisableIsh (
  VOID
  )
{
  if (PmcIsFunctionDisableConfigLocked (PmcGetPwrmBase ())) {
    return FALSE;
  }

  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_2_V2, B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_2_ISH);
  return TRUE;
}

/**
  This function enables ISH device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableIsh (
  VOID
  )
{
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_2_V2, (UINT32) (~B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_2_ISH));
}

/**
  This function checks if GbE is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsGbeFunctionDisabled (
  VOID
  )
{
  return ((MmioRead32 ((PmcGetPwrmBase() + R_PMC_PWRM_ST_PG_FDIS_PMC_1_V2)) & B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE) != 0);
}

/**
  This function disables GbE device by static power gating and enables ModPHY SPD gating (PCH-LP only).
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableGbe (
  VOID
  )
{
  UINTN  PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  ASSERT (!PmcIsFunctionDisableConfigLocked (PmcGetPwrmBase ()));

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_ST_PG_FDIS_PMC_1_V2, B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE);
}

/**
  This function enables GbE device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableGbe (
  VOID
  )
{
  //
  // Before modifying LAN Disable bit, make sure it's not locked.
  //
  ASSERT (!PmcIsFunctionDisableConfigLocked (PmcGetPwrmBase ()));

  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1_V2, (UINT32) ~B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE);
}

/**
  This function checks if GbE device is supported (not disabled by fuse)

  @param[in] PmcMmio       PMC MMIO

  @retval GbE support state
**/
BOOLEAN
PmcIsGbeSupported (
  IN UINTN  PmcMmio
  )
{
  return ((MmioRead32 (PmcMmio + R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1) & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1_GBE) == 0);
}

/**
  This function checks if XDCI device is supported (not disabled by fuse)

  @retval XDCI support state
**/
BOOLEAN
PmcIsXdciSupported (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0) & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD0_XDCI) == 0);
}

/**
  This function checks if Cnvi device is supported (not disabled by fuse)

  @retval Cnvi support state
**/
BOOLEAN
PmcIsCnviSupported (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1) & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD1_CNVI) == 0);
}

/**
  This function checks if CNVi is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsCnviFunctionDisabled (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1_V2) & B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI) != 0);
}

/**
  This function enables CNVi device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableCnvi (
  VOID
  )
{
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1_V2, (UINT32) ~B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI);
}

/**
  This function disables CNVi device by static power gating.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableCnvi (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1_V2, B_PTL_PCD_P_PMC_PWRM_ST_PG_FDIS_PMC_1_CNVI);
}

/**
  Check if PMC Extended straps are supported

  @retval  Status of Extended Straps support
**/
BOOLEAN
PmcIsExtendedStrapsSupported (
  VOID
  )
{
  return FALSE;
}

/**
  Check whether GBETSN_DIS strap is enabled. Note that even
  though this function doesn't have anything to do with PMC it is
  historically a PMC lib responsibility to check whether IPs are fuse/strap
  disabled.

  @retval TRUE                    GBETSN_DIS strap is enabled
  @retval FALSE                   GBETSN_DIS strap is disabled
**/
VOID
PmcIsGbeTsnStrapDis (
  VOID
  )
{
}

/**
  Checks if the Intel Touch Host Controller is enabled.
  For S SKU we check the register in PMC. For P and M we
  have to check the flash descriptor as PMC doesn't forward this
  information.

  @param[in] ThcNumber            THC0/THC1 0 or 1

  @retval TRUE                    THC_DIS strap is 0
  @retval FALSE                   THC_DIS strap is 1
**/
BOOLEAN
PmcIsThcEnabled (
  IN UINT8  ThcNumber
  )
{
  UINT32  FuseReg;

  FuseReg = MmioRead32 (PmcGetPwrmBase () + R_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD3);
  if (ThcNumber == 0) {
    return ((FuseReg & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD3_THC0) == 0);
  } else if (ThcNumber == 1) {
    return ((FuseReg & B_PTL_PCD_P_PMC_PWRM_STPG_FUSE_SS_DIS_RD3_THC1) == 0);
  } else {
    DEBUG ((DEBUG_WARN, "Incorrect THC number\n"));
    return FALSE;
  }
}

/**
  Check if PMC IPC1 command for CPU straps is supported.

  @retval  Status of CPU Straps support
**/
BOOLEAN
PmcIsCpuStrapsIpcCommandSupported (
  VOID
  )
{
  return TRUE;
}


/**
  Disables the root port using DEV_EN bit.

  @param[in] RpIndex            Root Port Number

**/
VOID
PmcDisablePcieRootPort (
  IN UINT32   RpIndex
  )
{
}

/**
  Check if SOC PMC SSRAM device is enabled

  @retval FALSE - device is disabled
  @retval TRUE  - device is enabled
**/
BOOLEAN
IsPmcSsramDeviceEnabled (
  VOID
  )
{
  UINT64    PmcSsramBaseAddress;

  PmcSsramBaseAddress = PmcSsramPciCfgBase ();
  if (PciSegmentRead16 (PmcSsramBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return FALSE;
  }

  return TRUE;
}

/**
  Init SOC PMC SSRAM MMIO Bar

  @param[out] SavedStsCmd - the saved value of PMC SSRAM device status and command register

  @retval FALSE           - PMC SSRAM MMIO Bar already asigned
  @retval TRUE            - PMC SSRAM MMIO Bar asigned by this routine
**/
BOOLEAN
InitPmcSsramMmioBar (
  OUT UINT16    *SavedStsCmd
  )
{
  UINT64    PmcSsramBaseAddress;
  UINT32    PmcSsramBar0;
  UINT64    Data64;
  BOOLEAN   Return;

  PmcSsramBaseAddress = PmcSsramPciCfgBase ();

  Data64 = GetPmcSsramMmioBar();

  if ((Data64 & 0xFFFFFFFFFFFFFFF0) == 0) {
    //
    // Program BAR 0 if the BAR is not programmed
    //
    PmcSsramBar0 = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
    DEBUG ((DEBUG_INFO, "InitPmcSsramMmioBar Bar 0x%x = 0x%lx!\n", PCI_BASE_ADDRESSREG_OFFSET, PmcSsramBar0));
    PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, PmcSsramBar0);
    PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
    Return = TRUE;
  } else {
    Return = FALSE;
  }

  //
  // Save and enable command register memory space decoding
  //
  *SavedStsCmd = PciSegmentRead16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET);
  DEBUG ((DEBUG_INFO, "InitPmcSsramMmioBar SavedStsCmd = 0x%x!\n", *SavedStsCmd));
  PciSegmentOr16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, (UINT16) (EFI_PCI_COMMAND_MEMORY_SPACE));

  return Return;
}

/**
  Clear SOC PMC SSRAM MMIO Bar and restore the PMC SSRAM device status and command register value

  @param[in] SavedStsCmd  - the restore value of PMC SSRAM device status and command register
  @param[in] ClearBar     - if TRUE need to clear the bar, else don't
**/
VOID
ClearPmcSsramMmioBar (
  IN UINT16     SavedStsCmd,
  IN BOOLEAN    ClearBar
  )
{
  UINT64    PmcSsramBaseAddress;

  PmcSsramBaseAddress = PmcSsramPciCfgBase ();

  //
  // Disable PMC SSRAM MMIO
  //
  PciSegmentAnd16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_IO_SPACE));

  //
  // Only clear it when it is not pre-assigned
  //
  if (ClearBar) {
    PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET, 0);
    PciSegmentWrite32 (PmcSsramBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + 4, 0);
  }

  DEBUG ((DEBUG_INFO, "ClearPmcSsramMmioBar SavedStsCmd = 0x%x!\n", SavedStsCmd));
  PciSegmentWrite16 (PmcSsramBaseAddress + PCI_COMMAND_OFFSET, SavedStsCmd);
}

/**
  Get PMC SSRAM MMIO BAR

  @retval SOC PMC SSRAM MMIO base address
**/
UINT64
GetPmcSsramMmioBar (
  VOID
  )
{
  UINT64    PmcSsramPciCfgAddr;
  UINT64    PmcSsramBar;

  PmcSsramPciCfgAddr = PmcSsramPciCfgBase ();

  PmcSsramBar = (UINT64) PciSegmentRead32 (PmcSsramPciCfgAddr + PCI_BASE_ADDRESSREG_OFFSET);
  if ((PmcSsramBar & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    PmcSsramBar |= ((UINT64) LShiftU64 (PciSegmentRead32 (PmcSsramPciCfgAddr + PCI_BASE_ADDRESSREG_OFFSET + 4), 32));
  }

  PmcSsramBar &= 0xFFFFFFFFFFFFFFF0;

  return PmcSsramBar;
}