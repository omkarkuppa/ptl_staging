/** @file
  PTL SoC root port initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <PchPcieRpConfig.h>
#include <PchPcieRpInfo.h>
#include <Library/PeiPcieSipInitLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PeiPcieRpLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PcdLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiPcieRpInitLib.h>
#include <Library/PcdInfoLib.h>
#include <Register/PchRegs.h>
#include <Register/PcieSipRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/PciSegmentLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPcieRpLib.h>
#include <Library/IoLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/HostBridgeRegs.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/PeiVtdInitFruLib.h>
#include <Library/PerformanceLib.h>

#include "PtlPcdInitPei.h"
#include <Library/PciExpressHelpersLib.h>

#define BUS_NUMBER_FOR_IMR 0x00

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID  gPtlPcdRpListPpiGuid = {0x55F621C1, 0xAEFE, 0x4712, {0x97, 0x96, 0xE3, 0xDC, 0x4D, 0xE4, 0x17, 0xC9}};

typedef struct {
  PCIE_ROOT_PORT_DEV             RpDev;
  PCI_CONFIG_SPACE_ACCESS        PcieConfigSpaceAccess;
  PCI_CONFIG_SPACE_ACCESS        EpPciRegAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbMsgCfgAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbMsgMemAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS  P2SbPcrAccess;
  P2SB_CONTROLLER                P2SbController;
  UINT32                         RpIndex;
} PCIE_ROOT_PORT_DEV_PRIVATE;

typedef struct {
  PCIE_ROOT_PORT_LIST         RpList;
  UINT32                      CurrentIndex;
  UINT32                      NumOfElements;
  PCIE_ROOT_PORT_DEV_PRIVATE  RootPorts[PCH_MAX_PCIE_ROOT_PORTS];
} PCIE_ROOT_PORT_LIST_PRIVATE;

EFI_STATUS
DomainPcieInit (
  IN  UINT32  MaxRootPortNum,
  IN BOOLEAN  PcieType
  );

/**
  Returns end point register access. Caller is responsible
  to make sure that end point is actually attached.

  @param[in]  RpDev        Root port device.
  @param[in]  Sbdf         SBDF of an endpoint device.
  @param[out] EpPciAccess  On output a pointer to the end point register access.

  @retval TRUE   End point access initialized successfuly.
  @retval FALSE  End point access failed to initialize.
**/
BOOLEAN
PtlPcdPcieRpGetPciAccess (
  IN  PCIE_ROOT_PORT_DEV  *RpDev,
  IN  PCIE_SBDF           *Sbdf,
  OUT REGISTER_ACCESS     **PciAccess
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;

  PciConfigAccessInit ((UINT8)Sbdf->Segment, (UINT8)Sbdf->Bus, (UINT8)Sbdf->Device, (UINT8)Sbdf->Function, &RpDevPrivate->EpPciRegAccess);
  *PciAccess = &RpDevPrivate->EpPciRegAccess.RegAccess;
  return TRUE;
}

/**
  Called when SIP library disables the PCIe root port.

  @param[in] RpDev  Pointer to root port device which is being disabled.
**/
VOID
PtlPcdPcieDisableRootPort (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;
  if (PtlPcdIsRpIocDecoded (RpDevPrivate->RpIndex)) {
    IocDisablePcieRootPort (RpDevPrivate->RpIndex);
  } else {
    PtlPcdPsfDisablePcieRootPort (RpDevPrivate->RpIndex);
  }
  DisableClock (PchClockUsagePchPcie0 + RpDevPrivate->RpIndex);
}

/**
  Called by SIP library to check if root port's lane
  is connected to PHY.

  @param[in] RpDev        Pointer to root port device.
  @param[in] RpLaneIndex  Index of the root port's lane which to check.

  @retval TRUE   PCIe lane connected to PHY.
  @retval FALSE  PCIe lane not connected to PHY.
**/
BOOLEAN
PtlPcdIsPcieLaneConnected (
  IN PCIE_ROOT_PORT_DEV  *RpDev,
  IN UINT32              RpLaneIndex
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;
  UINT32                      PcieLane;
  UINT32                      ControllerIndex;
  UINT8                       LaneNum;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;

  if (RpDevPrivate->RpIndex < 4) {
    ControllerIndex = PtlPcdRpIndexToControllerIndex ((UINTN)RpDevPrivate->RpIndex);
    PcieLane = (ControllerIndex * PCH_PCIE_CONTROLLER_PORTS) + RpLaneIndex;
    return PtlPcdFiaGetPcieLaneNum (PcieLane, &LaneNum);
  } else {
      return TRUE;
  }
}

/**
  Called by SIP library to enable port 80 decode.

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PtlPcdPcieEnablePort80Decode (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;

  PchIoPort80DecodeSet (RpDevPrivate->RpIndex);
}

/**
  Called by SIP library to check if CLKREQ signal is being pulled down.

  @param[in] RpDev  Pointer to root port device.
**/
BOOLEAN
PtlPcdPcieIsClkReqPulledDown (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT32                      RpIndex;
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;
  RpIndex = RpDevPrivate->RpIndex;

  if (!IsClkReqAssigned (PchClockUsagePchPcie0 + RpIndex)) {
    return FALSE;
  }
  if (RpDev->PcieRootPortConfig->PcieRpCommonConfig.ClkReqDetect &&
      EFI_ERROR (CheckClkReq (PchClockUsagePchPcie0 + RpIndex))) {
    DEBUG ((DEBUG_INFO, "CLKREQ is not Low, disabling power management for RP %d.\n", RpIndex));
    return FALSE;
  }
  return TRUE;
}

/**
  Called by SIP library to enable CLKREQ pin for root port.

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PtlPcdPcieEnableClkReq (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;

  EnableClkReq (PchClockUsagePchPcie0 + RpDevPrivate->RpIndex);
}

/**
  Called by SIP library when initializing NCC root port.

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PtlPcdPcieInitNccRootPort (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
}

/**
  Called by SIP library when swapping the root port function number.

  @param[in] RpDev        Pointer to root port device.
  @param[in] NewFunction  New function number.
**/
VOID
PtlPcdPcieSwapFunctionNum (
  IN PCIE_ROOT_PORT_DEV  *RpDev,
  IN UINT16              NewFunction
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;
  if (PtlPcdIsRpIocDecoded (RpDevPrivate->RpIndex)) {
    IocSetPcieFunction (RpDevPrivate->RpIndex, (UINT32) NewFunction);
  } else {
    PtlPcdPsfSetPcieRpFunction (RpDevPrivate->RpIndex, (UINT32) NewFunction);
  }
}

/**
  Called by SIP library when it wants to force enable the
  PCI config decode for given root port.

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PtlPcdPcieForceEnablePciConfig (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;
  PtlPcdPsfEnablePcieRootPort (RpDevPrivate->RpIndex);
}

/**
  Called by SIP library when PCI config access to root port
  should be disabled.

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PtlPcdPcieDisablePciConfig (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate;

  RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;
  if (PtlPcdIsRpIocDecoded (RpDevPrivate->RpIndex)) {
    IocDisablePcieRootPort (RpDevPrivate->RpIndex);
  } else {
    PtlPcdPsfDisablePcieRootPort (RpDevPrivate->RpIndex);
  }
}

/**
  Called by SIP library to enable PCIE LINK DOWN functionality
  on platform specifc GPIO
**/
VOID
PtlPcdPcieEnableGpioLinkDown (
  VOID
  )
{
  EFI_STATUS Status;
  GPIOV2_SERVICES *GpioServices;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a] Error getting GpioServices\n", __FUNCTION__));
    return;
  }

  PtlPcdLinkDownEnable (GpioServices);
}

/**
  Get project specific Gen4 preset configuration.

  @param[in/out] PlatformSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PtlPcdPcieGetProjectGen4PresetConfiguration (
  IN  PCIE_ROOT_PORT_DEV              *RpDev,
  OUT PCIE_LINK_EQ_PLATFORM_SETTINGS  *PlatformSettings
  )
{
 if ((RpDev->SipPhyVersion == SC16)) {
    PlatformSettings->Ph3PresetList[0] = 4;
    PlatformSettings->Ph3PresetList[1] = 5;
    PlatformSettings->Ph3PresetList[2] = 6;
    PlatformSettings->Ph3PresetList[3] = 0;
    PlatformSettings->Ph3PresetList[4] = 0;
    PlatformSettings->Ph3PresetList[5] = 0;
    PlatformSettings->Ph3PresetList[6] = 0;
    PlatformSettings->Ph3PresetList[7] = 0;
    PlatformSettings->Ph3PresetList[8] = 0;
    PlatformSettings->Ph3PresetList[9] = 0;
    PlatformSettings->Ph3PresetList[10] = 0;
    PlatformSettings->Ph3NoOfPresetOrCoeff = 3;
  } else if ((RpDev->SipPhyVersion == SE32)) {
    PlatformSettings->Ph3PresetList[0] = 4;
    PlatformSettings->Ph3PresetList[1] = 5;
    PlatformSettings->Ph3PresetList[2] = 6;
    PlatformSettings->Ph3PresetList[3] = 8;
    PlatformSettings->Ph3PresetList[4] = 9;
    PlatformSettings->Ph3PresetList[5] = 0;
    PlatformSettings->Ph3PresetList[6] = 0;
    PlatformSettings->Ph3PresetList[7] = 0;
    PlatformSettings->Ph3PresetList[8] = 0;
    PlatformSettings->Ph3PresetList[9] = 0;
    PlatformSettings->Ph3PresetList[10] = 0;
    PlatformSettings->Ph3NoOfPresetOrCoeff = 5;
  }
}

/**
  Get project specific Gen3 preset configuration.

  @param[in/out] PlatformSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PtlPcdPcieGetProjectGen3PresetConfiguration (
  IN  PCIE_ROOT_PORT_DEV              *RpDev,
  OUT PCIE_LINK_EQ_PLATFORM_SETTINGS  *PlatformSettings
  )
{
  if ((RpDev->SipPhyVersion == SC16)) {
    PlatformSettings->Ph3PresetList[0] = 4;
    PlatformSettings->Ph3PresetList[1] = 5;
    PlatformSettings->Ph3PresetList[2] = 6;
    PlatformSettings->Ph3PresetList[3] = 7;
    PlatformSettings->Ph3PresetList[4] = 8;
    PlatformSettings->Ph3PresetList[5] = 0;
    PlatformSettings->Ph3PresetList[6] = 0;
    PlatformSettings->Ph3PresetList[7] = 0;
    PlatformSettings->Ph3PresetList[8] = 0;
    PlatformSettings->Ph3PresetList[9] = 0;
    PlatformSettings->Ph3PresetList[10] = 0;
    PlatformSettings->Ph3NoOfPresetOrCoeff = 5;
  } else if ((RpDev->SipPhyVersion == SE32)) {
    PlatformSettings->Ph3PresetList[0] = 4;
    PlatformSettings->Ph3PresetList[1] = 5;
    PlatformSettings->Ph3PresetList[2] = 6;
    PlatformSettings->Ph3PresetList[3] = 8;
    PlatformSettings->Ph3PresetList[4] = 9;
    PlatformSettings->Ph3PresetList[5] = 0;
    PlatformSettings->Ph3PresetList[6] = 0;
    PlatformSettings->Ph3PresetList[7] = 0;
    PlatformSettings->Ph3PresetList[8] = 0;
    PlatformSettings->Ph3PresetList[9] = 0;
    PlatformSettings->Ph3PresetList[10] = 0;
    PlatformSettings->Ph3NoOfPresetOrCoeff = 5;
  }
}

/**
  Get project specific Gen5 preset configuration.

  @param[in/out] PlatformSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PtlPcdPcieGetProjectGen5PresetConfiguration (
  IN  PCIE_ROOT_PORT_DEV              *RpDev,
  OUT PCIE_LINK_EQ_PLATFORM_SETTINGS  *PlatformSettings
  )
{
  PlatformSettings->Ph3PresetList[0] = 4;
  PlatformSettings->Ph3PresetList[1] = 5;
  PlatformSettings->Ph3PresetList[2] = 7;
  PlatformSettings->Ph3PresetList[3] = 8;
  PlatformSettings->Ph3PresetList[4] = 0;
  PlatformSettings->Ph3PresetList[5] = 0;
  PlatformSettings->Ph3PresetList[6] = 0;
  PlatformSettings->Ph3PresetList[7] = 0;
  PlatformSettings->Ph3PresetList[8] = 0;
  PlatformSettings->Ph3PresetList[9] = 0;
  PlatformSettings->Ph3PresetList[10] = 0;
  PlatformSettings->Ph3NoOfPresetOrCoeff = 4;
}

/**
  Get project specific Gen4 preset to coefficient mapping configuration.

  @param[in]     RpDev              Pointer to the root port device
  @param[in/out] ProjectSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PtlPcdPcieGetProjectGen4PresetToCoefficientConfiguration (
  IN  PCIE_ROOT_PORT_DEV             *RpDev,
  OUT PCIE_LINK_EQ_PROJECT_SETTINGS  *ProjectSettings
  )
{
  if ((RpDev->SipPhyVersion == SC16)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 44;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 4;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  } else if ((RpDev->SipPhyVersion == SE32)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 14;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  }
}

/**
  Get project specific Gen3 preset to coefficient mapping configuration.

  @param[in]     RpDev              Pointer to the root port device
  @param[in/out] ProjectSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PtlPcdPcieGetProjectGen3PresetToCoefficientConfiguration (
  IN  PCIE_ROOT_PORT_DEV             *RpDev,
  OUT PCIE_LINK_EQ_PROJECT_SETTINGS  *ProjectSettings
  )
{
  if ((RpDev->SipPhyVersion == SC16)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 44;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 4;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  } else if ((RpDev->SipPhyVersion == SE32)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 14;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  }
}

/**
  Get project specific Gen5 preset to coefficient mapping configuration.

  @param[in/out] ProjectSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PtlPcdPcieGetProjectGen5PresetToCoefficientConfiguration (
  OUT PCIE_LINK_EQ_PROJECT_SETTINGS  *ProjectSettings
  )
{
  ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 34;
  ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 14;
  ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 38;
  ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 10;
  ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 36;
  ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 12;
  ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 40;
  ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 8;
  ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
  ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 42;
  ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 6;
  ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 40;
  ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 8;
  ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 32;
  ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 5;
  ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 11;
  ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 34;
  ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
  ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 8;
  ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 38;
  ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 10;
  ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
  ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 30;
  ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 2;
  ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
}

/**
  Get the project recommended equalization settings

  @param[in]  RpDev                          Pointer to the root port device
  @param[out] PcieGen3LinkEqSettings         Pointer to the equalization config
  @param[out] PcieGen4LinkEqSettings         Pointer to the equalization config
  @param[out] PcieGen5LinkEqSettings         Pointer to the equalization config
**/
VOID
PtlPcdPcieGetProjectDefaultEqConfiguration (
  IN  PCIE_ROOT_PORT_DEV     *RpDev,
  OUT PCIE_LINK_EQ_SETTINGS  *PcieGen3LinkEqSettings,
  OUT PCIE_LINK_EQ_SETTINGS  *PcieGen4LinkEqSettings,
  OUT PCIE_LINK_EQ_SETTINGS  *PcieGen5LinkEqSettings
  )
{
  PcieGen3LinkEqSettings->PlatformSettings.PcieLinkEqMethod = PcieLinkHardwareEq;
  PcieGen3LinkEqSettings->PlatformSettings.PcieLinkEqMode = PcieLinkEqPresetMode;
  PcieGen3LinkEqSettings->PlatformSettings.Ph1DpTxPreset = 7;
  if (RpDev->SipPhyVersion == SE32) {
    PcieGen3LinkEqSettings->PlatformSettings.Ph1UpTxPreset = 7;
  } else {
    PcieGen3LinkEqSettings->PlatformSettings.Ph1UpTxPreset = 4;
  }
  PcieGen3LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported = FALSE;
  PcieGen3LinkEqSettings->PlatformSettings.PCETTimer = 2;

  PcieGen3LinkEqSettings->ProjectSettings.FullSwingValue = 0x30;
  PcieGen3LinkEqSettings->ProjectSettings.LowFrequencyValue = 0x10;
  PcieGen3LinkEqSettings->PlatformSettings.EqPh3Bypass = 0;
  PcieGen3LinkEqSettings->PlatformSettings.EqPh23Bypass = 0;
  PcieGen3LinkEqSettings->PlatformSettings.TsLockTimer = 0;
  PtlPcdPcieGetProjectGen3PresetConfiguration (RpDev, &PcieGen3LinkEqSettings->PlatformSettings);
  PtlPcdPcieGetProjectGen3PresetToCoefficientConfiguration (RpDev, &PcieGen3LinkEqSettings->ProjectSettings);

  PcieGen4LinkEqSettings->PlatformSettings.PcieLinkEqMethod = PcieLinkHardwareEq;
  PcieGen4LinkEqSettings->PlatformSettings.PcieLinkEqMode = PcieLinkEqPresetMode;
  PcieGen4LinkEqSettings->PlatformSettings.Ph1DpTxPreset = 7;
  if (RpDev->SipPhyVersion == SE32) {
    PcieGen4LinkEqSettings->PlatformSettings.Ph1UpTxPreset = 7;
  } else {
    PcieGen4LinkEqSettings->PlatformSettings.Ph1UpTxPreset = 4;
  }
  PcieGen4LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported = FALSE;
  PcieGen4LinkEqSettings->PlatformSettings.PCETTimer = 2;

  PcieGen4LinkEqSettings->ProjectSettings.FullSwingValue = 0x30;
  PcieGen4LinkEqSettings->ProjectSettings.LowFrequencyValue = 0x10;
  PcieGen4LinkEqSettings->PlatformSettings.EqPh3Bypass = 0;
  PcieGen4LinkEqSettings->PlatformSettings.EqPh23Bypass = 0;
  PcieGen4LinkEqSettings->PlatformSettings.TsLockTimer = 0;
  PtlPcdPcieGetProjectGen4PresetConfiguration (RpDev, &PcieGen4LinkEqSettings->PlatformSettings);
  PtlPcdPcieGetProjectGen4PresetToCoefficientConfiguration (RpDev, &PcieGen4LinkEqSettings->ProjectSettings);

  PcieGen5LinkEqSettings->PlatformSettings.PcieLinkEqMethod = PcieLinkHardwareEq;
  PcieGen5LinkEqSettings->PlatformSettings.PcieLinkEqMode = PcieLinkEqPresetMode;
  PcieGen5LinkEqSettings->PlatformSettings.Ph1DpTxPreset = 5;
  PcieGen5LinkEqSettings->PlatformSettings.Ph1UpTxPreset = 5;
  PcieGen5LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported = FALSE;
  PcieGen5LinkEqSettings->PlatformSettings.PCETTimer = 2;

  PcieGen5LinkEqSettings->ProjectSettings.FullSwingValue = 0x30;
  PcieGen5LinkEqSettings->ProjectSettings.LowFrequencyValue = 0x10;
  PcieGen5LinkEqSettings->PlatformSettings.EqPh3Bypass = 0;
  PcieGen5LinkEqSettings->PlatformSettings.EqPh23Bypass = 0;
  PcieGen5LinkEqSettings->PlatformSettings.TsLockTimer = 0;
  PtlPcdPcieGetProjectGen5PresetConfiguration (RpDev, &PcieGen5LinkEqSettings->PlatformSettings);
  PtlPcdPcieGetProjectGen5PresetToCoefficientConfiguration (&PcieGen5LinkEqSettings->ProjectSettings);
}

/**
  Initializes private config of the root port.

  @param[in] RpDevPrivate       Pointer to the device
  @param[in] RpIndex            Root port index
**/
VOID
PtlPcdPcieInitRpPrivateConfig (
  IN PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate,
  IN UINT32                      RpIndex
  )
{
  PCIE_ROOT_PORT_PRIVATE_CONFIG  *PrivateConfig;
  PCIE_ROOT_PORT_DEV             *RpDev;

  RpDev = &RpDevPrivate->RpDev;
  PrivateConfig = &RpDevPrivate->RpDev.PrivateConfig;

  if (PtlIsPcdP ()) {
    if (RpIndex < 8) {
      PrivateConfig->RootPortIndexInController = RpIndex % PCH_PCIE_CONTROLLER_PORTS;
      PrivateConfig->RootPortsInController = 4;
      PrivateConfig->LanesPerRootPort = 1;
    } else if (RpIndex < 10) {
      PrivateConfig->RootPortIndexInController = RpIndex % 2;
      PrivateConfig->RootPortsInController = 2;
      PrivateConfig->LanesPerRootPort = 2;
    }
  } else {
    if (RpIndex < 8) {
      PrivateConfig->RootPortIndexInController = RpIndex % PCH_PCIE_CONTROLLER_PORTS;
      PrivateConfig->RootPortsInController = 4;
      PrivateConfig->LanesPerRootPort = 1;
    } else if (RpIndex < 10) {
      PrivateConfig->RootPortIndexInController = RpIndex % 2;
      PrivateConfig->RootPortsInController = 2;
      PrivateConfig->LanesPerRootPort = 2;
    } else if (RpIndex < 11) {
      PrivateConfig->RootPortIndexInController = 0;
      PrivateConfig->RootPortsInController = 1;
      PrivateConfig->LanesPerRootPort = 8;
    } else if (RpIndex < 12) {
      PrivateConfig->RootPortIndexInController = 0;
      PrivateConfig->RootPortsInController = 1;
      PrivateConfig->LanesPerRootPort = 4;
    }
  }
  if (PtlIsPcdH()) {
    PrivateConfig->MultiVcEnabled = FALSE;
    PrivateConfig->NumOfVcs       = 0;
  }
  if (PtlIsPcdP ()) {
    PrivateConfig->MultiVcEnabled = TRUE;
    PrivateConfig->NumOfVcs       = 0;
    if (RpDev->SipPhyVersion == SE32) {
      PrivateConfig->NumOfVcs       = 1;
    }
  }

  PrivateConfig->VnnRemovalSupported = FALSE;
  PrivateConfig->BusMin = PcdGet8 (PcdSiliconInitTempPciBusMin);
  PrivateConfig->BusMax = PcdGet8 (PcdSiliconInitTempPciBusMax);
  PrivateConfig->TPowerOn = 0x5;
  if ((RpDev->SipPhyVersion == SC16)) {
    PrivateConfig->PtmConfig.PtmPipeStageDelay[0]  = 0xFD5AFF3C;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[1]  = 0xA028EF54;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[2]  = 0x86249024;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[3]  = 0x4717471B;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[4]  = 0x4215;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[5]  = 0x0;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[6]  = 0x0;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[7]  = 0x0;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[8]  = 0x2F0D300F;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[9]  = 0x00002B0C;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[10] = 0x0;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[11] = 0x0;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[12] = 0x0;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[13] = 0x0;
  }

  if ((RpDev->SipPhyVersion == SE32)) {
    PrivateConfig->PtmConfig.PtmPipeStageDelay[0]  = 0xFF5EFF40;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[1]  = 0xAA2FEF60;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[2]  = 0x862C912C;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[3]  = 0x481D4A21;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[4]  = 0x0000431C;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[5]  = 0xE75FED60;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[6]  = 0x7F258328;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[7]  = 0x401A431B;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[8]  = 0x2E0F3111;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[9]  = 0x2B102C0E;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[10] = 0x00002C11;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[11] = 0x2F0D290E;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[12] = 0x2C0F2C0F;
    PrivateConfig->PtmConfig.PtmPipeStageDelay[13] = 0x00002C0F;
  }
  PrivateConfig->PtmConfig.PtmConfig            = 0x40452;
  PrivateConfig->L1StandardConfig.NoSnoopEnable = 0x1;
  PrivateConfig->L1StandardConfig.NoSnoopScale  = 0x2;
  PrivateConfig->L1StandardConfig.NoSnoopValue  = 0x41;
  PrivateConfig->L1StandardConfig.SnoopEnable   = 0x1;
  PrivateConfig->L1StandardConfig.SnoopScale    = 0x2;
  PrivateConfig->L1StandardConfig.SnoopValue    = 0x41;
  PrivateConfig->L1p1Config.NoSnoopEnable       = 0x1;
  PrivateConfig->L1p1Config.NoSnoopScale        = 0x2;
  PrivateConfig->L1p1Config.NoSnoopValue        = 0x41;
  PrivateConfig->L1p1Config.SnoopEnable         = 0x1;
  PrivateConfig->L1p1Config.SnoopScale          = 0x2;
  PrivateConfig->L1p1Config.SnoopValue          = 0x41;
  PrivateConfig->L1p2Config.NoSnoopEnable       = 0x1;
  PrivateConfig->L1p2Config.NoSnoopScale        = 0x2;
  PrivateConfig->L1p2Config.NoSnoopValue        = 0x82;
  PrivateConfig->L1p2Config.SnoopEnable         = 0x1;
  PrivateConfig->L1p2Config.SnoopScale          = 0x2;
  PrivateConfig->L1p2Config.SnoopValue          = 0x82;
  PrivateConfig->LtrSubL11Npg.NoSnoopEnable     = 0x1;
  PrivateConfig->LtrSubL11Npg.NoSnoopScale      = 0x2;
  PrivateConfig->LtrSubL11Npg.NoSnoopValue      = 0x2D;
  PrivateConfig->LtrSubL11Npg.SnoopEnable       = 0x1;
  PrivateConfig->LtrSubL11Npg.SnoopScale        = 0x2;
  PrivateConfig->LtrSubL11Npg.SnoopValue        = 0x2D;
  PrivateConfig->ConfigureAspmAtDpDmiInit       = TRUE;
  PrivateConfig->CompletionChaining             = TRUE;
  PrivateConfig->PmcRequestEnable               = TRUE;
  PrivateConfig->PresentDetectStatePolicy       = 0x1;
  PrivateConfig->ChainBitGenerationMode         = TRUE;
  PrivateConfig->Gen3ElasticBufferMode          = 0x0;
  PrivateConfig->Gen4ElasticBufferMode          = 0x0;
  PrivateConfig->Gen5ElasticBufferMode          = 0x0;
  PrivateConfig->ReceiveSbemCapDecodeEnable     = FALSE;
  PrivateConfig->MarginIndependentLeftRightTimingSupport = TRUE;
  PrivateConfig->MarginMaxTimingOffsetSupport            = 0x14;
  if (RpDev->SipPhyVersion == SC16) {
    PrivateConfig->MarginNumberOfTimingStepsSupport      = 0x9;
    PrivateConfig->MarginSamplingRateTimingSupport       = 0x3F;
  } else if (RpDev->SipPhyVersion == SE32) {
    PrivateConfig->MarginNumberOfTimingStepsSupport      = 0x8;
    PrivateConfig->MarginNumberOfVoltageStepsSupport     = 0x20;
    PrivateConfig->MarginSamplingRateTimingSupport       = 0x1F;
  }
  PrivateConfig->MarginMaxNumberOfLanesSupport           = 0x0;
  PrivateConfig->MarginSampleReportingMethod             = 0x1;
  PrivateConfig->ApmsL0sSupported                        = TRUE;
  PrivateConfig->DefaultVc0Mapping                       = 0xFF;
  PrivateConfig->ConfigureDynamicLinkThrottling          = TRUE;
  PrivateConfig->IncreaseChainTimersValue                = FALSE;
  PtlPcdPcieGetProjectDefaultEqConfiguration (RpDev, &PrivateConfig->Gen3EqSettings, &PrivateConfig->Gen4EqSettings, &PrivateConfig->Gen5EqSettings);
}

/**
  Initializes access and callbacks functions.

  @param[in] RpDevPrivate       Pointer to the device
  @param[in] RpIndex            Root port index
**/
VOID
PtlPcdPcieInitAccessAndCallbacks (
  IN PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate,
  IN UINT32                      RpIndex
  )
{
  PCIE_ROOT_PORT_DEV  *RpDev;
  P2SB_SIDEBAND_ACCESS_METHOD    AccessMethod;

  RpDev = &RpDevPrivate->RpDev;

  PciConfigAccessInit (
    (UINT8)RpDev->Sbdf.Segment, (UINT8)RpDev->Sbdf.Bus, (UINT8)RpDev->Sbdf.Device, (UINT8)RpDev->Sbdf.Function, &RpDevPrivate->PcieConfigSpaceAccess
    );
  RpDev->PciCfgAccess = &RpDevPrivate->PcieConfigSpaceAccess.RegAccess;

  BuildP2SbSidebandAccess (
    &RpDevPrivate->P2SbController,
    PtlPcdGetRpSbiPid ((UINTN)RpIndex),
    PtlPcdPcieRpGetSbiFid (RpIndex),
    P2SbPciConfig,
    P2SbMsgAccess,
    FALSE,
    &RpDevPrivate->P2SbMsgCfgAccess
    );
  RpDev->PciSbiMsgCfgAccess = &RpDevPrivate->P2SbMsgCfgAccess.Access;

  BuildP2SbSidebandAccess (
    &RpDevPrivate->P2SbController,
    PtlPcdGetRpSbiPid ((UINTN)RpIndex),
    PtlPcdPcieRpGetSbiFid (RpIndex),
    P2SbMemory,
    P2SbMsgAccess,
    TRUE,
    &RpDevPrivate->P2SbMsgMemAccess
    );
  RpDev->PciSbiMsgMemAccess = &RpDevPrivate->P2SbMsgMemAccess.Access;

  //
  // In PEI IOE root ports need to be accessed with
  // MSG access due to MMIO not being available in 32-bit
  // environment.
  //
  AccessMethod = P2SbMmioAccess;
  BuildP2SbSidebandAccess (
    &RpDevPrivate->P2SbController,
    PtlPcdGetRpSbiPid (RpIndex),
    PtlPcdPcieRpGetSbiFid (RpIndex),
    P2SbPrivateConfig,
    AccessMethod,
    FALSE,
    &RpDevPrivate->P2SbPcrAccess
    );
  RpDev->PciPcrAccess = &RpDevPrivate->P2SbPcrAccess.Access;

  RpDev->Callbacks.GetPciAccess = PtlPcdPcieRpGetPciAccess;
  RpDev->Callbacks.DisableRootPort = PtlPcdPcieDisableRootPort;
  RpDev->Callbacks.IsPcieLaneConnected = PtlPcdIsPcieLaneConnected;
  RpDev->Callbacks.EnablePort80Decode = PtlPcdPcieEnablePort80Decode;
  RpDev->Callbacks.IsClkReqPulledDown = PtlPcdPcieIsClkReqPulledDown;
  RpDev->Callbacks.EnableClkReq = PtlPcdPcieEnableClkReq;
  RpDev->Callbacks.InitNccPort = PtlPcdPcieInitNccRootPort;
  RpDev->Callbacks.SwapFunctionNumber = PtlPcdPcieSwapFunctionNum;
  RpDev->Callbacks.ForceEnablePciConfig = PtlPcdPcieForceEnablePciConfig;
  RpDev->Callbacks.DisablePciConfig = PtlPcdPcieDisablePciConfig;
  RpDev->Callbacks.EnableGpioLinkDown = PtlPcdPcieEnableGpioLinkDown;
}

/**
  Returns the SIP version of the PCIe root port.

  @param[in] RpIndex  Index of the root port in PCH

  @return Version of the SIP
**/
PCIE_SIP_VERSION
PcieRpGetSipVer (
  IN UINT32  RpIndex
  )
{
  return PcieSip17_5;
}

/**
  Initializes PCIe root port structure

  @param[in] RpDevPrivate       Pointer to the device
  @param[in] RpIndex            Root port index
**/
VOID
PtlPcdPcieInitRpDev (
  IN PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate,
  IN UINT32                      RpIndex
  )
{
  PCIE_ROOT_PORT_DEV  *RpDev;

  RpDev = &RpDevPrivate->RpDev;

  RpDev->PcieConfig = NULL;
  RpDev->PcieRootPortConfig = NULL;

  RpDev->SipVersion = PcieSip17_5;
  RpDev->Integration = PchPcie;
  RpDev->IpTrain = PcieIpTrainD4;

  RpDev->SipPhyVersion = PcieRpGetSipPhyVer(RpIndex);

  RpDev->Id = RpIndex;
  if (PtlIsPcdP ()) {
    if (RpIndex == 0 || RpIndex == 4 || RpIndex == 8) {
      RpDev->IsController = TRUE;
    } else {
      RpDev->IsController = FALSE;
    }
  } else {
    if (RpIndex == 0 || RpIndex == 4 || RpIndex == 8 || RpIndex == 10 || RpIndex == 11) {
      RpDev->IsController = TRUE;
    } else {
      RpDev->IsController = FALSE;
    }
  }

  RpDev->Sbdf.Segment = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  RpDev->Sbdf.Bus = DEFAULT_PCI_BUS_NUMBER_PCH;
  PtlPcdGetPcieRpDevFun (RpIndex, &RpDev->Sbdf.Device, &RpDev->Sbdf.Function);

  PtlPcdGetPcieP2SbController (RpIndex, &RpDevPrivate->P2SbController);
  PtlPcdPcieInitRpPrivateConfig (RpDevPrivate, RpIndex);
  PtlPcdPcieInitAccessAndCallbacks (RpDevPrivate, RpIndex);
  RpDevPrivate->RpIndex = RpIndex;
}

/**
  Updates root port descriptor in pre-mem phase after pre-mem policy
  has been installed.

  @param[in] SiPreMemPolicyPpi  Pointer to Si pre mem policy
  @param[in] RpDevPrivate       Pointer to the device
  @param[in] RpIndex            Root port index
**/
VOID
PtlPcdPcieUpdateRpDevPreMem  (
  IN SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi,
  IN PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate,
  IN UINT32                      RpIndex
  )
{
  PCIE_ROOT_PORT_DEV  *RpDev;
  VTD_CONFIG          *VtdConfig;
  EFI_STATUS           Status;

  if (SiPreMemPolicyPpi == NULL) {
    return;
  }

  RpDev = &RpDevPrivate->RpDev;
  RpDev->PrivateConfig.ClkReqAssigned = IsClkReqAssigned (PchClockUsagePchPcie0 + RpIndex);

  VtdConfig = NULL;
#if FixedPcdGetBool (PcdVtdEnable) == 1
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &VtdConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
  }
#endif

  if (VtdConfig == NULL || IsVtdDisabled(VtdConfig)) {
      RpDev->PrivateConfig.VtdEnabled = FALSE;
  } else {
      RpDev->PrivateConfig.VtdEnabled = TRUE;
  }
}

/**
  Updates root port descriptor in post-mem phase after post-mem policy
  has been installed.

  @param[in] SiPolicy           Pointer to Si policy
  @param[in] RpDevPrivate       Pointer to the device
  @param[in] RpIndex            Root port index
**/
VOID
PtlPcdPcieUpdateRpDevPostMem  (
  IN SI_POLICY_PPI               *SiPolicy,
  IN PCIE_ROOT_PORT_DEV_PRIVATE  *RpDevPrivate,
  IN UINT32                      RpIndex
  )
{
  PCIE_ROOT_PORT_DEV  *RpDev;
  EFI_STATUS          Status;

  if (SiPolicy == NULL) {
    return;
  }

  RpDev = &RpDevPrivate->RpDev;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &RpDev->PcieConfig);
  if (!EFI_ERROR (Status)) {
      RpDev->PcieRootPortConfig = &RpDev->PcieConfig->RootPort[RpIndex];
  }

  RpDev->PrivateConfig.InterruptPin = ItssGetDevIntPin (SiPolicy, (UINT8)RpDev->Sbdf.Device, (UINT8)RpDev->Sbdf.Function);

  if (RpDev->PcieConfig != NULL) {
    if (RpDev->PcieConfig->PcieCommonConfig.EnablePort8xhDecode && RpDev->PcieConfig->PchPciePort8xhDecodePortIndex == (RpIndex + 1)) {
        RpDev->PrivateConfig.EnablePort80Decode = TRUE;
    }
  }
  RpDev->PrivateConfig.AtomicOpEnabled = TRUE;
}

/**
  Get next root port in the list. When called this
  function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
EFI_STATUS
PtlPcdPcieGetNextRootPort (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;

  RpListPrivate = (PCIE_ROOT_PORT_LIST_PRIVATE*) RpList;

  RpListPrivate->CurrentIndex++;
  if (RpListPrivate->CurrentIndex < RpListPrivate->NumOfElements) {
    *RpDev = &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex].RpDev;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
  Get next controller in the list. When called this
  function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next controller device.

  @retval EFI_SUCCESS    Next controller device found.
  @retval EFI_NOT_FOUND  Didn't find next controller device.
**/
EFI_STATUS
PtlPcdPcieGetNextController (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  PCIE_ROOT_PORT_DEV           *CurrentRootPort;
  UINT32                       RemainingRootPorts;

  RpListPrivate   = (PCIE_ROOT_PORT_LIST_PRIVATE*) RpList;

  CurrentRootPort = (PCIE_ROOT_PORT_DEV*) &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex];
  RemainingRootPorts = CurrentRootPort->PrivateConfig.RootPortsInController - CurrentRootPort->PrivateConfig.RootPortIndexInController;
  RpListPrivate->CurrentIndex += RemainingRootPorts;

  if (RpListPrivate->CurrentIndex < RpListPrivate->NumOfElements) {
    *RpDev = &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex].RpDev;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
  Get next root port belonging to current controller.
  When called this function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
EFI_STATUS
PtlPcdPcieGetNextRootPortInThisController (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  PCIE_ROOT_PORT_DEV           *CurrentRootPort;

  RpListPrivate = (PCIE_ROOT_PORT_LIST_PRIVATE*) RpList;
  CurrentRootPort = (PCIE_ROOT_PORT_DEV*) &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex];
  //
  // This condition checks if current root port device is the last one in the controller.
  //
  if ((CurrentRootPort->PrivateConfig.RootPortIndexInController + 1) == CurrentRootPort->PrivateConfig.RootPortsInController) {
    return EFI_NOT_FOUND;
  }

  RpListPrivate->CurrentIndex++;
  if (RpListPrivate->CurrentIndex < RpListPrivate->NumOfElements) {
    *RpDev = &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex].RpDev;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
  Get controller device to which current root port
  belongs. When called this function does not
  progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to controller device.

  @retval EFI_SUCCESS    Controller device found.
  @retval EFI_NOT_FOUND  Didn't find controller device.
**/
EFI_STATUS
PtlPcdPcieGetThisController (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  UINT32                       RpIndex;
  PCIE_ROOT_PORT_DEV           *CurrentRootPort;

  RpListPrivate = (PCIE_ROOT_PORT_LIST_PRIVATE*) RpList;
  CurrentRootPort = (PCIE_ROOT_PORT_DEV*) &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex];

  RpIndex = RpListPrivate->CurrentIndex - CurrentRootPort->PrivateConfig.RootPortIndexInController;

  if (RpIndex < RpListPrivate->NumOfElements) {
    *RpDev = &RpListPrivate->RootPorts[RpIndex].RpDev;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
  Reset the iteration to the first root port.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
EFI_STATUS
PtlPcdPcieResetToFirst (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;

  RpListPrivate = (PCIE_ROOT_PORT_LIST_PRIVATE*) RpList;

  RpListPrivate->CurrentIndex = 0;
  if (RpListPrivate->CurrentIndex < RpListPrivate->NumOfElements) {
    *RpDev = &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex].RpDev;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
  Get root port device located on next PCI device number.
  When called this function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
EFI_STATUS
PtlPcdPcieGetRootPortOnNextDevice (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  )
{
  UINT32                       RpIndex;
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  UINT16                       ThisDevNum;
  PCIE_ROOT_PORT_DEV           *RpDevTemp;
  BOOLEAN                      Found;

  RpListPrivate = (PCIE_ROOT_PORT_LIST_PRIVATE*) RpList;
  RpDevTemp = &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex].RpDev;
  ThisDevNum = RpDevTemp->Sbdf.Device;
  Found = FALSE;
  for (RpIndex = RpListPrivate->CurrentIndex; RpIndex < RpListPrivate->NumOfElements; RpIndex++) {
    RpDevTemp = &RpListPrivate->RootPorts[RpIndex].RpDev;
    if (ThisDevNum != RpDevTemp->Sbdf.Device) {
      Found = TRUE;
      break;
    }
  }

  if (Found && RpIndex < RpListPrivate->NumOfElements) {
    RpListPrivate->CurrentIndex = RpIndex;
    *RpDev = &RpListPrivate->RootPorts[RpIndex].RpDev;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
  Get root port device located on next PCI function number.
  When called this function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
EFI_STATUS
PtlPcdPcieGetRootPortOnNextFunction (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  )
{
  UINT32                       RpIndex;
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  UINT16                       ThisDevNum;
  UINT16                       ThisFuncNum;
  PCIE_ROOT_PORT_DEV           *RpDevTemp;
  BOOLEAN                      Found;

  RpListPrivate = (PCIE_ROOT_PORT_LIST_PRIVATE*) RpList;
  RpDevTemp = &RpListPrivate->RootPorts[RpListPrivate->CurrentIndex].RpDev;
  ThisDevNum = RpDevTemp->Sbdf.Device;
  ThisFuncNum = RpDevTemp->Sbdf.Function;
  Found = FALSE;
  for (RpIndex = RpListPrivate->CurrentIndex; RpIndex < RpListPrivate->NumOfElements; RpIndex++) {
    RpDevTemp = &RpListPrivate->RootPorts[RpIndex].RpDev;
    if (ThisDevNum == RpDevTemp->Sbdf.Device && ThisFuncNum != RpDevTemp->Sbdf.Function) {
      Found = TRUE;
      break;
    }
  }

  if (Found && RpIndex < RpListPrivate->NumOfElements) {
    RpListPrivate->CurrentIndex = RpIndex;
    *RpDev = &RpListPrivate->RootPorts[RpIndex].RpDev;
    return EFI_SUCCESS;
  } else {
    return EFI_NOT_FOUND;
  }
}

/**
  Initialize PCIe root port list.

  @param[out] RpListPrivate      Pointer to the list
**/
VOID
PtlPcdPcieInitRpList (
  OUT PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate
  )
{
  PCIE_ROOT_PORT_LIST *RpList;
  UINT32              RpIndex;

  ZeroMem (RpListPrivate, sizeof (PCIE_ROOT_PORT_LIST_PRIVATE));

  RpList = &RpListPrivate->RpList;

  RpList->GetNextRootPort = PtlPcdPcieGetNextRootPort;
  RpList->GetNextController = PtlPcdPcieGetNextController;
  RpList->GetThisController = PtlPcdPcieGetThisController;
  RpList->GetNextRootPortInThisController = PtlPcdPcieGetNextRootPortInThisController;
  RpList->GetRootPortOnNextDevice = PtlPcdPcieGetRootPortOnNextDevice;
  RpList->GetRootPortOnNextFunction = PtlPcdPcieGetRootPortOnNextFunction;
  RpList->ResetToFirst = PtlPcdPcieResetToFirst;

  RpListPrivate->NumOfElements = GetMaxPciePortNum ();
  for (RpIndex = 0; RpIndex < RpListPrivate->NumOfElements; RpIndex++) {
    PtlPcdPcieInitRpDev (&RpListPrivate->RootPorts[RpIndex], RpIndex);
  }
}

/**
  Updates PCIe root port list in pre-mem.

  @param[in]  SiPreMemPolicyPpi  Pointer to Si pre mem policy
  @param[out] RpListPrivate      Pointer to the list
**/
VOID
PtlPcdPcieUpdateListInPreMem (
  IN SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi,
  OUT PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate
  )
{
  UINT32              RpIndex;

  for (RpIndex = 0; RpIndex < RpListPrivate->NumOfElements; RpIndex++) {
    PtlPcdPcieUpdateRpDevPreMem (SiPreMemPolicyPpi, &RpListPrivate->RootPorts[RpIndex], RpIndex);
  }
}

/**
  Updates PCIe root port list in post-mem.

  @param[in]  SiPolicy           Pointer to Si policy
  @param[out] RpListPrivate      Pointer to the list
**/
VOID
PtlPcdPcieUpdateListInPostMem (
  IN SI_POLICY_PPI                 *SiPolicy,
  OUT PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate
  )
{
  UINT32              RpIndex;

  for (RpIndex = 0; RpIndex < RpListPrivate->NumOfElements; RpIndex++) {
    PtlPcdPcieUpdateRpDevPostMem (SiPolicy, &RpListPrivate->RootPorts[RpIndex], RpIndex);
  }
}

/**
  Configure PCIe Grant Counts
**/
VOID
PtlPcdPcieRpConfigureGrantCounts (
  IN PCIE_ROOT_PORT_LIST  *RpList
  )
{
   UINT32                      ControllerIndex;
   PSF_PCIE_CTRL_CONFIG        PsfPcieCtrlBifurcationArray[PCH_MAX_PCIE_CONTROLLERS];
   EFI_STATUS                  Status;
   PCIE_ROOT_PORT_DEV          *ControllerDev;
   PCIE_ROOT_PORT_DEV_PRIVATE  *ControllerDevPrivate;

  ControllerIndex = 0;
  for (Status = RpList->ResetToFirst (RpList, &ControllerDev); Status == EFI_SUCCESS; Status = RpList->GetNextController (RpList, &ControllerDev)) {
    if (ControllerIndex >= ARRAY_SIZE (PsfPcieCtrlBifurcationArray)) {
      break;
    }
    ControllerDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) ControllerDev;
    if (PtlPcdIsRpIocDecoded (ControllerDevPrivate->RpIndex)) {
      continue;
    }
    switch (PcieSipGetControllerBifurcation (ControllerDev)) {
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_1_1_1_1:
        PsfPcieCtrlBifurcationArray[ControllerIndex] = PsfPcieCtrl4xn;
        break;
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_2_1_1:
        PsfPcieCtrlBifurcationArray[ControllerIndex] = PsfPcieCtrl1x2n_2xn;
        break;
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_2_2:
        PsfPcieCtrlBifurcationArray[ControllerIndex] = PsfPcieCtrl2x2n;
        break;
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_4:
        PsfPcieCtrlBifurcationArray[ControllerIndex] = PsfPcieCtrl1x4n;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "Unsupported PCIe configuration\n"));
        ASSERT (FALSE);
    }
    ControllerIndex++;
  }
  if (ControllerIndex != 0) {
    PtlPcdGrantCountProgramming (PsfPcieCtrlBifurcationArray, ControllerIndex - 1);
  }
}

/**
  Perform Initialization of the Downstream Root Ports.

  @param[in] SiPolicy  The SI Policy PPI
**/
VOID
PtlPcdPcieRpInit (
  IN SI_POLICY_PPI     *SiPolicy
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  PCIE_ROOT_PORT_LIST          *RpList;
  PCIE_ROOT_PORT_DEV           *RpDev;
  EFI_STATUS                   Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;
  UINT32 MaxRootPortNum;

  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );

  PtlPcdFiaSetClockOutputDelay ();

  Status = PeiServicesLocatePpi (&gPtlPcdRpListPpiGuid, 0, NULL, (VOID **) &RpListPrivate);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to get RpList\n", __FUNCTION__));
    return;
  }
  PtlPcdPcieUpdateListInPostMem (SiPolicy, RpListPrivate);
  RpList = &RpListPrivate->RpList;

  MaxRootPortNum = GetMaxPciePortNum ();
  DomainPcieInit (MaxRootPortNum, EnumPcdPcie);

  IoWrite32 (
    PmcGetAcpiBase () + R_ACPI_IO_GPE0_STS_127_96,
    B_ACPI_IO_GPE0_STS_127_96_PCI_EXP_STS | B_ACPI_IO_GPE0_STS_127_96_HOT_PLUG_STS
    );

  //
  // If SCI is enabled in any port, Set BIOS_PCI_EXP_EN bit, PMC PCI offset A0h[10],
  // to globally enable the setting of the PCI_EXP_STS bit by a PCI Express* PME event.
  //
  RpList->ResetToFirst (RpList, &RpDev); // Init RpDev to the first device

  do {
    if (RpDev->PcieRootPortConfig->PcieRpCommonConfig.PmSci) {
      PmcEnablePciExpressPmeEvents (PmcGetPwrmBase ());
      break;
    }
  } while (RpList->GetNextRootPort (RpList, &RpDev) != EFI_NOT_FOUND);
  PtlPcdPsfEnablePcieRelaxedOrder ();
  DisableUnusedPcieClocks ();
  //
  // Configure PSF PCIe Grant Counts after PCIe Root Ports are initialized
  // and unused ports are disabled
  //
  PtlPcdPcieRpConfigureGrantCounts (RpList);
}

/**
  Perform pre-mem initialization of the root port.

  @param[in] SiPreMemPolicyPpi  Pointer to pre-mem SI policy.
**/
VOID
PtlPcdPcieRpPreMemInit (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                   Status;
  PCH_PCIE_RP_PREMEM_CONFIG    *PcieRpPreMemConfig;
  UINT32                       RpIndex;
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PcieRpPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (&gPtlPcdRpListPpiGuid, 0, NULL, (VOID **) &RpListPrivate);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to get RpList\n", __FUNCTION__));
    return;
  }

  PtlPcdPcieUpdateListInPreMem (SiPreMemPolicyPpi, RpListPrivate);

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex++) {
    if ((PcieRpPreMemConfig->RpEnabledMask & (UINT32) (1 << RpIndex)) == 0) {
      if (PtlPcdIsRpIocDecoded (RpIndex)) {
        IocDisablePcieRootPort (RpIndex);
      } else {
        PtlPcdPsfDisablePcieRootPort (RpIndex);
      }
    }
  }
  PcieSipInitNccRootPorts (&RpListPrivate->RpList);
}

/**
  Updates function pointers in PtlPcdRpList after memory migration.

  @param[in] PeiServices       Pointer to PEI services
  @param[in] NotifyDescriptor  Pointer to notify descriptor
  @param[in] Ppi               Pointer to PPI.

  @retval EFI_SUCCESS  Updated RpList
  @retval Others       Failed to update RpList
**/
EFI_STATUS
EFIAPI
PtlPcdPcieUpdateListOnMemoryDiscovered (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  UINT32                       RpIndex;
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  EFI_STATUS                   Status;

  PERF_INMODULE_BEGIN ("PcieUpdateListOnMemDisc");

  Status = PeiServicesLocatePpi (&gPtlPcdRpListPpiGuid, 0, NULL, (VOID **) &RpListPrivate);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to get RpList\n", __FUNCTION__));
    return Status;
  }

  for (RpIndex = 0; RpIndex < RpListPrivate->NumOfElements; RpIndex++) {
    PtlPcdPcieInitAccessAndCallbacks  (&RpListPrivate->RootPorts[RpIndex], RpIndex);
  }

  PERF_INMODULE_END ("PcieUpdateListOnMemDisc");

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mPtlPcdemoryDiscoveredNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  PtlPcdPcieUpdateListOnMemoryDiscovered
};

/**
  Perform pre-policy initialization of a root port.
**/
VOID
PtlPcdPciePrePolicyInit (
  VOID
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  EFI_PEI_PPI_DESCRIPTOR       *PtlPcdRpListPpi;
  EFI_STATUS                   Status;

  PtlPcdRpListPpi = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  RpListPrivate = (PCIE_ROOT_PORT_LIST_PRIVATE*) AllocateZeroPool (sizeof (PCIE_ROOT_PORT_LIST_PRIVATE));
  if (PtlPcdRpListPpi == NULL || RpListPrivate == NULL) {
    DEBUG ((DEBUG_WARN, "%a out of memory for RpList\n", __FUNCTION__));
    return;
  }

  PtlPcdRpListPpi->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PtlPcdRpListPpi->Guid  = &gPtlPcdRpListPpiGuid;
  PtlPcdRpListPpi->Ppi   = RpListPrivate;
  Status = PeiServicesInstallPpi (PtlPcdRpListPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to install rp list PPI\n", __FUNCTION__));
  }

  Status = PeiServicesNotifyPpi (&mPtlPcdemoryDiscoveredNotify);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to install notify", __FUNCTION__));
  }

  PtlPcdPcieInitRpList (RpListPrivate);
  PcieSipHideDisableRootPorts (&RpListPrivate->RpList);
  PcieSipEarlyDecodeEnable (&RpListPrivate->RpList);
}

/**
  Configure root port function number mapping

  @param[in] SiPolicy  Pointer to silicon policy
**/
VOID
PtlPcdPcieRpConfigureRpfnMapping (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  EFI_STATUS                   Status;

  Status = PeiServicesLocatePpi (&gPtlPcdRpListPpiGuid, 0, NULL, (VOID **) &RpListPrivate);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to get RpList\n", __FUNCTION__));
    return;
  }

  PcieSipConfigureRpfnMapping (&RpListPrivate->RpList);
}

/**
  Update PCIe portion of the info hob.

  @param[in, out] PchInfoHob  PCH info HOB to fill in.
**/
VOID
BuildPcieInfoHob (
  IN OUT PCH_INFO_HOB *PchInfoHob
  )
{
  PCIE_ROOT_PORT_LIST_PRIVATE  *RpListPrivate;
  PCIE_ROOT_PORT_LIST          *RpList;
  PCIE_ROOT_PORT_DEV           *RpDev;
  PCIE_ROOT_PORT_DEV           *ControllerDev;
  PCIE_ROOT_PORT_DEV_PRIVATE   *RpDevPrivate;
  EFI_STATUS                   Status;
  UINT32                       ControllerIndex;

  Status = PeiServicesLocatePpi (&gPtlPcdRpListPpiGuid, 0, NULL, (VOID **) &RpListPrivate);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a failed to get RpList\n", __FUNCTION__));
    return;
  }

  PchInfoHob->PciePortFuses       = 0;
  PchInfoHob->PciePortLaneEnabled = 0;

  RpList = &RpListPrivate->RpList;
  for (Status = RpList->ResetToFirst (RpList, &ControllerDev); Status == EFI_SUCCESS; Status = RpList->GetNextController (RpList, &ControllerDev)) {
    RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) ControllerDev;
    ControllerIndex = PtlPcdRpIndexToControllerIndex ((UINTN)RpDevPrivate->RpIndex);
    if (ControllerIndex >= ARRAY_SIZE (PchInfoHob->PcieControllerBifurcation)) {
      break;
    }
    PchInfoHob->PcieControllerBifurcation[ControllerIndex] = PcieSipGetControllerBifurcation (ControllerDev);
    for (RpDev = ControllerDev; Status == EFI_SUCCESS; Status = RpList->GetNextRootPortInThisController (RpList, &RpDev)) {
      RpDevPrivate = (PCIE_ROOT_PORT_DEV_PRIVATE*) RpDev;
      if (PcieSipIsRpOwningLanes (ControllerDev, RpDev)) {
        PchInfoHob->PciePortLaneEnabled |= BIT0 << RpDevPrivate->RpIndex;
      }
      if (!PcieSipIsRootPortEnabledInController (ControllerDev, RpDev->PrivateConfig.RootPortIndexInController)) {
        PchInfoHob->PciePortFuses |= BIT0 << (RpDevPrivate->RpIndex);
      }
    }
  }
}