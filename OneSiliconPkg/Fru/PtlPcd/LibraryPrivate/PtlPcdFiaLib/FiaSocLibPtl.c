/** @file
  This file contains PTLPCD specific FIA routines

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Register/PchFiaRegs.h>
#include <Library/PeiPcieRpInitLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PchFia19.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PcdInfoLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

#define  R_PCD_FIA_PCR_PDSDM1                     0x18
#define  B_PCD_FIA_PCR_PDSDM1_USBXPDIPSLSD        (BIT0|BIT1)
#define  V_PCD_FIA_PCR_PDSDM1_USBXPDIPSLSD_3CLK   0x2
#define  N_PCD_FIA_PCR_PDSDM1_USBXPDIPSLSD        0
#define  B_PCD_FIA_PCR_PDSDM1_USBXPDIPSDM         (BIT2|BIT3)
#define  V_PCD_FIA_PCR_PDSDM1_USBXPDIPSDM_1       0
#define  N_PCD_FIA_PCR_PDSDM1_USBXPDIPSDM         2
#define  B_PCD_FIA_PCR_PDSDM1_GBEPDIPSLSD         (BIT20|BIT21)
#define  N_PCD_FIA_PCR_PDSDM1_GBEPDIPSLSD         20
#define  V_PCD_FIA_PCR_PDSDM1_GBEPDIPSLSD_3CLK    0X2
#define  B_PCD_FIA_PCR_PDSDM1_GBEPDIPSDM          (BIT22|BIT23)
#define  N_PCD_FIA_PCR_PDSDM1_GBEPDIPSDM          22
#define  V_PCD_FIA_PCR_PDSDM1_GBEPDIPSDM_1        0x0

#define  R_PCD_FIA_PCR_PDSDM2                       0x28
#define  B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD          (BIT0|BIT1)
#define  N_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD          0
#define  V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD_2CLK     0x1
#define  V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD_1CLK     0x0
#define  B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM           (BIT2|BIT3)
#define  N_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM           2
#define  V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM_4         0x1
#define  V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM_16        0x2
#define  B_PCD_FIA_PCR_PDSDM2_PXPBPDIPSLSD          (BIT4|BIT5)
#define  N_PCD_FIA_PCR_PDSDM2_PXPBPDIPSLSD          4
#define  V_PCD_FIA_PCR_PDSDM2_PXPBPDIPSLSD_1CLK     0x0
#define  B_PCD_FIA_PCR_PDSDM2_PXPBPDIPSDM           (BIT6|BIT7)
#define  N_PCD_FIA_PCR_PDSDM2_PXPBPDIPSDM           6
#define  V_PCD_FIA_PCR_PDSDM2_PXPBPDIPSDM_16        0x2
#define  B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD       (BIT16|BIT17)
#define  N_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD       16
#define  V_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD_3CLK  2
#define  V_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD_1CLK  0
#define  B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSDM        (BIT18|BIT19)
#define  N_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSDM        18
#define  V_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSDM_1      0

#define  R_PCD_FIA_PCR_RXSDM1                       0x1C
#define  B_PCD_FIA_PCR_RXSDM1_USBXRXIPSLSD          (BIT0|BIT1)
#define  V_PCD_FIA_PCR_RXSDM1_USBXRXIPSLSD_3CLK     0x2
#define  B_PCD_FIA_PCR_RXSDM1_USBXRXIPSDM           (BIT2|BIT3)
#define  N_PCD_FIA_PCR_RXSDM1_USBXRXIPSDM           2
#define  V_PCD_FIA_PCR_RXSDM1_USBXRXIPSDM_1         0

#define  R_PCD_FIA_PCR_RXSDM2                     0x2C
#define  B_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD        (BIT0|BIT1)
#define  N_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD        0
#define  V_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD_1CLK   0x0
#define  V_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD_2CLK   0x1
#define  B_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM         (BIT2|BIT3)
#define  N_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM         2
#define  V_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM_4       0x1
#define  V_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM_16      0x2
#define  B_PCD_FIA_PCR_RXSDM2_PXPBRXIPSLSD        (BIT4 | BIT5)
#define  N_PCD_FIA_PCR_RXSDM2_PXPBRXIPSLSD        4
#define  V_PCD_FIA_PCR_RXSDM2_PXPBRXIPSLSD_1CLK   0x0
#define  B_PCD_FIA_PCR_RXSDM2_PXPBRXIPSDM         (BIT6 | BIT7)
#define  N_PCD_FIA_PCR_RXSDM2_PXPBRXIPSDM         6
#define  V_PCD_FIA_PCR_RXSDM2_PXPBRXIPSDM_16      0x2

/**
  Returns the FIA Instance

  @param[in]   P2SbPid             Port id
  @param[out]  SbAccess            P2SB Register Access pointer
  @param[out]  P2SbController      P2SB controller pointer

  @return FIA Instance
**/
FIA_INSTANCE
FiaGetInstance (
  IN  P2SB_PORT_16_ID                P2SbPid,
  OUT P2SB_SIDEBAND_REGISTER_ACCESS  *SbAccess,
  OUT P2SB_CONTROLLER                *P2SbController
  )
{
  FIA_INSTANCE     FiaInst;
  EFI_STATUS       Status;

  Status = PtlPcdGetP2SbController (P2SbController, P2SbPid);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    FiaInst.RegisterAccess = NULL;
    return FiaInst;
  }

  BuildP2SbSidebandAccess (
      P2SbController,
      P2SbPid.PortId.LocalPid,
      0,
      P2SbMemory,
      P2SbMmioAccess,
      FALSE,
      SbAccess
      );
  FiaInst.RegisterAccess = (REGISTER_ACCESS *) SbAccess;

  return FiaInst;
}

/**
  Returns if passed Fia instance is located on SoC.

  @param[in] FiaInst  FIA Instance

  @return TRUE if FIA Instance is on SoC, FALSE otherwise
**/
BOOLEAN
FiaInstanceIsOnSoc (
  IN  FIA_INSTANCE      FiaInst,
  IN  P2SB_PORT_16_ID   P2SbPid
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *FiaSbAccess;
  P2SB_CONTROLLER                SocP2sbController;
  EFI_STATUS                     Status;

  FiaSbAccess = (P2SB_SIDEBAND_REGISTER_ACCESS *) FiaInst.RegisterAccess;

  Status = PtlPcdGetP2SbController (&SocP2sbController, P2SbPid);
  ASSERT_EFI_ERROR (Status);

  if (FiaSbAccess->P2SbCtrl->PciCfgBaseAddr == SocP2sbController.PciCfgBaseAddr) {
    return TRUE;
  } else {
    DEBUG ((DEBUG_WARN, "Unknown FIA instance\n"));
    return FALSE;
  }
}

/**
  Returns a FIA lane number for a GbE controller

  @param[out] LaneNum  FIA lane number that is owned by GbE

  @retval TRUE   Found FIA lane assigned to GbE
  @retval FALSE  No lanes assigned to GbE
**/
BOOLEAN
PtlPcdFiaGetGbeLaneNum (
  OUT UINT8  *LaneNum
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;
    P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;
    FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
    *LaneNum = 0;

  return (PtlPcdFiaGetLaneOwner (FiaInst, *LaneNum) == PchFiaOwnerGbe);
}

/**
  Returns a FIA lane number for a given PCIe lane.

  @param[in]  PciePhysicalLane  Index of the PCIe lane
  @param[out] LaneNum           Pointer to the variable that stores lane number.
                                The output value is only valid if this function returns TRUE.

  @return TRUE if given PciePhysicalLane owns FIA lane, FALSE otherwise
**/
BOOLEAN
PtlPcdFiaGetPcieLaneNum (
  IN  UINT32  PciePhysicalLane,
  OUT UINT8   *LaneNum
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;

  if (PciePhysicalLane < 4) {
    *LaneNum = (UINT8)PciePhysicalLane;
    P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;
    FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  } else {
      return TRUE;
  }

  return (PtlPcdFiaGetLaneOwner (FiaInst, *LaneNum) == PchFiaOwnerPcie);
}

/**
  Returns a FIA lane number for a given USB3 port.

  @param[in]  Usb3PortIndex  USB3 port index
  @param[out] LaneNum        Pointer to the variable that stores lane number.
                             The output value is only valid if this function returns TRUE.

  @return TRUE if given USB3 port owns FIA lane, FALSE otherwise
**/
BOOLEAN
PtlPcdFiaGetUsb3LaneNum (
  IN  UINT32  Usb3PortIndex,
  OUT UINT8   *LaneNum
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;

  ASSERT (Usb3PortIndex < GetPchXhciMaxUsb3PortNum ());
  *LaneNum = (UINT8) Usb3PortIndex;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_U;
  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);

  return (PtlPcdFiaGetLaneOwner (FiaInst, *LaneNum) == PchFiaOwnerUsb3);
}

/**
  Returns number of FIA lanes

  @param[in] FiaInst  FIA Instance

  @return Number of FIA lanes
**/
UINT8
PtlPcdFiaGetMaxLaneNumEx (
  IN  FIA_INSTANCE FiaInst OPTIONAL
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *FiaSbAccess;
  P2SB_PORT_16_ID                P2SBPid;

  FiaSbAccess = (P2SB_SIDEBAND_REGISTER_ACCESS *) FiaInst.RegisterAccess;
  switch (FiaSbAccess->P2SbPid) {
    case GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_FIA_PG):
      P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;
      if (FiaInstanceIsOnSoc (FiaInst, P2SBPid)) {
        return 4;
      }
      break;
    case GET_P2SB_LOCAL_PID (PTL_SID_F2_PID_FIA_X):
      P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_X;
      if (FiaInstanceIsOnSoc (FiaInst, P2SBPid)) {
          return 4;
      }
      break;
    case GET_P2SB_LOCAL_PID (PTL_P_H_SID_F2_PID_FIA_P5X4):
      P2SBPid.Pid16bit = PTL_P_H_SID_F2_PID_FIA_P5X4;
      if (FiaInstanceIsOnSoc (FiaInst, P2SBPid)) {
        return 4;
      }
      break;
    case GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_FIA_U):
      P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_U;
      if (FiaInstanceIsOnSoc (FiaInst, P2SBPid)) {
        return 2;
      }
      break;
    case GET_P2SB_LOCAL_PID(PTL_H_SID_F2_PID_FIA_P5X8):
      P2SBPid.Pid16bit = PTL_H_SID_F2_PID_FIA_P5X8;
      if (FiaInstanceIsOnSoc (FiaInst, P2SBPid)) {
        return 8;
      }
      break;
    default:
      DEBUG ((DEBUG_INFO, "Invalid FIA Instance \n"));
      ASSERT (FALSE);
      return 0;
  }
  return 0;
}

/**
  Return FIA lane owner.

  @param[in] FiaInst  FIA Instance
  @param[in] LaneNum  FIA lane number

  @return  Code of the FIA lane owner, PchFiaOwnerInvalid if lane number wasn't valid
**/
PCH_FIA_LANE_OWNER
PtlPcdFiaGetLaneOwner (
  IN  FIA_INSTANCE FiaInst,
  IN  UINT8        LaneNum
  )
{
  if (LaneNum >= PtlPcdFiaGetMaxLaneNumEx (FiaInst)) {
    ASSERT (FALSE);
    return PchFiaOwnerInvalid;
  }
  return PchFia19GetLaneOwner (FiaInst, LaneNum);
}

/**
  Print FIA LOS registers.
**/
VOID
PtlPcdFiaPrintLosRegisters (
  VOID
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;

  DEBUG ((DEBUG_INFO, "FIA_U: \n"));
  P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_U;
  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  PchFia19PrintLosRegisters (FiaInst, PtlPcdFiaGetMaxLaneNumEx (FiaInst));
    DEBUG ((DEBUG_INFO, "FIA_PG: \n"));
    P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;
  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  PchFia19PrintLosRegisters (FiaInst, PtlPcdFiaGetMaxLaneNumEx (FiaInst));
    DEBUG ((DEBUG_INFO, "FIA_X: \n"));
    P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_X;
  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  PchFia19PrintLosRegisters (FiaInst, PtlPcdFiaGetMaxLaneNumEx (FiaInst));

    DEBUG ((DEBUG_INFO, "P5X4: \n"));
    P2SBPid.Pid16bit = PTL_P_H_SID_F2_PID_FIA_P5X4;
    FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
    PchFia19PrintLosRegisters (FiaInst, PtlPcdFiaGetMaxLaneNumEx (FiaInst));

  if (PtlIsPcdH ()) {
    DEBUG ((DEBUG_INFO, "P5X8: \n"));
    P2SBPid.Pid16bit = PTL_H_SID_F2_PID_FIA_P5X8;
    FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
    PchFia19PrintLosRegisters (FiaInst, PtlPcdFiaGetMaxLaneNumEx (FiaInst));
  }
}

/**
  Assigns CLKREQ# to PCH PCIe ports

  @param[in] ClkReqMap      Mapping between PCH PCIe ports and CLKREQ#
  @param[in] ClkReqMapSize  Size of the map
**/
VOID
PchFiaAssignPchPciePortsClkReq (
  IN UINT8  *ClkReqMap,
  IN UINT8  ClkReqMapSize
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;
  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  PchFia19AssignPchPciePortsClkReq (FiaInst, ClkReqMap, ClkReqMapSize);
  PchFia19PrintPciePortsClkReq (P2SBPid.PortId.LocalPid, FiaInst);
}

/**
  Disables ClkReq mapping for PCIe root port

  @param[in] FiaInst  FIA Instance
  @param[in] RpIndex  Root port index
**/
VOID
PtlPcdFiaDisablePchPciePortClkReq (
  IN UINT32  RpIndex
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;
  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  PchFia19DisablePchPciePortClkReq (FiaInst, RpIndex);
}

/**
  Assigns CLKREQ# to GbE

  @param[in]  ClkReqNum  CLKREQ# number
**/
VOID
PchFiaAssignGbeClkReq (
  IN UINT8  ClkReqNum
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;
    P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;

  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  PchFia19AssignGbeClkReq (FiaInst, ClkReqNum);
}

/**
  Configures lower bound of delay between ClkReq assertion and driving RefClk.
**/
VOID
PtlPcdFiaSetClockOutputDelay (
  VOID
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  P2SB_PORT_16_ID                P2SBPid;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_FIA_PG;
  FiaInst = FiaGetInstance (P2SBPid, &FiaSbAccess, &P2SbController);
  PchFia19SetClockOutputDelay (FiaInst, FiaClkOutputDelay5us);
}

typedef struct {
  P2SB_PORT_16_ID Pid;
  BOOLEAN         OnIoe;
} PTL_FIA_PID;

/**
  Performs FIA programming required at the end of configuration and locks lockable FIA registers
**/
VOID
PtlPcdFiaFinalizeConfigurationAndLock (
  VOID
  )
{
  FIA_INSTANCE                   FiaInst;
  P2SB_SIDEBAND_REGISTER_ACCESS  FiaSbAccess;
  P2SB_CONTROLLER                P2SbController;
  UINT32                         Data32Or;
  UINT8                          Index;
  PTL_FIA_PID                    *FiaPids;
  UINT8                          FiaPidSize;
  UINT32                         DataAnd32;
  UINT32                         DataOr32;

  PTL_FIA_PID FiaPidsH [] = {
    { {.Pid16bit = PTL_SID_F2_PID_FIA_U},    FALSE},
    { {.Pid16bit = PTL_SID_F2_PID_FIA_PG},   FALSE},
    { {.Pid16bit = PTL_SID_F2_PID_FIA_X},    FALSE},
    { {.Pid16bit = PTL_P_H_SID_F2_PID_FIA_P5X4}, FALSE},
    { {.Pid16bit = PTL_H_SID_F2_PID_FIA_P5X8}, FALSE}
  };

  PTL_FIA_PID FiaPidsP [] = {
    { {.Pid16bit = PTL_SID_F2_PID_FIA_U},    FALSE},
    { {.Pid16bit = PTL_SID_F2_PID_FIA_PG},   FALSE},
    { {.Pid16bit = PTL_SID_F2_PID_FIA_X},    FALSE},
    { {.Pid16bit = PTL_P_H_SID_F2_PID_FIA_P5X4}, FALSE}
  };

  if (PtlIsPcdH ()) {
    FiaPids = FiaPidsH;
    FiaPidSize = ARRAY_SIZE (FiaPidsH);
  }
  else {
    FiaPids = FiaPidsP;
    FiaPidSize = ARRAY_SIZE (FiaPidsP);
  }
  for (Index = 0;  Index < FiaPidSize; Index++) {
    if (FiaPids[Index].OnIoe == TRUE) {
      continue;
    }
    FiaInst = FiaGetInstance (FiaPids[Index].Pid, &FiaSbAccess, &P2SbController);

    switch (FiaPids[Index].Pid.Pid16bit) {
      case PTL_SID_F2_PID_FIA_U:
        //
        // Program PCR[FIA] + 18h bit [1:0, 3:2] to [10, 00]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_PDSDM1_USBXPDIPSLSD | B_PCD_FIA_PCR_PDSDM1_USBXPDIPSDM);
        DataOr32 = V_PCD_FIA_PCR_PDSDM1_USBXPDIPSLSD_3CLK;
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_PDSDM1,
          DataAnd32,
          DataOr32
        );

        //
        // Program PCR[FIA] + 1Ch bit [1:0] to [02]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_RXSDM1_USBXRXIPSLSD | B_PCD_FIA_PCR_RXSDM1_USBXRXIPSDM);
        DataOr32 = V_PCD_FIA_PCR_RXSDM1_USBXRXIPSLSD_3CLK;
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_RXSDM1,
          DataAnd32,
          DataOr32
        );

        //
        // Program PCR[FIA] + 28h bit [17:16, 19:18] to [10, 00]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD | B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSDM);
        DataOr32 = (V_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD_3CLK << N_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD);
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_PDSDM2,
          DataAnd32,
          DataOr32
        );
        break;
      case PTL_SID_F2_PID_FIA_PG:
        //
        // Program PCR[FIA] + 20h bit [30:28] to [000]
        //
        FiaInst.RegisterAccess->And32(
          FiaInst.RegisterAccess,
          R_PCH_FIA_PCR_PLLCTL,
          (UINT32)~(B_PCH_FIA_PCR_PLLCTL_PLLCLKVADT)
        );

          //
          // Program PCR[FIA] + 18h bit [21:20] to [10], [22:23] to [00]
          //
          DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_PDSDM1_GBEPDIPSLSD | B_PCD_FIA_PCR_PDSDM1_GBEPDIPSDM);
          DataOr32 = (V_PCD_FIA_PCR_PDSDM1_GBEPDIPSLSD_3CLK << N_PCD_FIA_PCR_PDSDM1_GBEPDIPSLSD);
          FiaInst.RegisterAccess->AndThenOr32(
            FiaInst.RegisterAccess,
            R_PCD_FIA_PCR_PDSDM1,
            DataAnd32,
            DataOr32
          );

        //
        // Program PCR[FIA] + 28h bit [1:0, 3:2] to [01, 01]
        // Program PCR[FIA] + 28h bit [17:16, 19:18] to [00, 00]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM | B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD |
                             B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD | B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSDM);
        DataOr32 = ((V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM_4 << N_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM) |
                    V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD_2CLK);
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_PDSDM2,
          DataAnd32,
          DataOr32
        );

        //
        // Program PCR[FIA] + 2Ch bit [1:0, 3:2] to [01, 01]
        //
        DataAnd32 = (UINT32)~ (B_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD | B_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM);
        DataOr32 = ((V_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM_4 << N_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM) |
          V_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD_2CLK);
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_RXSDM2,
          DataAnd32,
          DataOr32
        );
        break;
      case PTL_SID_F2_PID_FIA_X:
        //
        // Program PCR[FIA] + 28h bit [1:0, 3:2] to [01, 01]
        // Program PCR[FIA] + 28h bit [17:16, 19:18] to [00, 00]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM | B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD |
          B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD | B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSDM);
        DataOr32 = ((V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM_4 << N_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM) |
          V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD_2CLK);
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_PDSDM2,
          DataAnd32,
          DataOr32
        );

        //
        // Program PCR[FIA] + 2Ch bit [1:0, 3:2] to [01, 01]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD | B_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM);
        DataOr32 = ((V_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM_4 << N_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM) |
          V_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD_2CLK);
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_RXSDM2,
          DataAnd32,
          DataOr32
        );
        break;

      default:
        //
        // Program PCR[FIA] + 28h bit [1:0, 3:2] to [00, 10]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM | B_PCD_FIA_PCR_PDSDM2_PCIEPDIPSLSD |
                     B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSLSD | B_PCD_FIA_PCR_PDSDM2_PWRPWRREQIPSDM);
        DataOr32 = (V_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM_16 << N_PCD_FIA_PCR_PDSDM2_PCIEPDIPSDM);
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_PDSDM2,
          DataAnd32,
          DataOr32
        );

        //
        // Program PCR[FIA] + 2Ch bit [1:0, 3:2] to [00, 10]
        //
        DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_RXSDM2_PCIERXIPSLSD | B_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM);
        DataOr32 = (V_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM_16 << N_PCD_FIA_PCR_RXSDM2_PCIERXIPSDM);
        FiaInst.RegisterAccess->AndThenOr32(
          FiaInst.RegisterAccess,
          R_PCD_FIA_PCR_RXSDM2,
          DataAnd32,
          DataOr32
        );

        if (FiaPids[Index].Pid.Pid16bit == PTL_H_SID_F2_PID_FIA_P5X8) {
          //
          // Program PCR[FIA] + 20h bit [23:21, 20:18, 17:15, 14:12] to [001, 001, 001, 001]
          //
          DataAnd32 = (UINT32) ~(B_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT | B_PCH_FIA_PCR_PLLCTL_CL1PLLWAIT |
                                 B_PCH_FIA_PCR_PLLCTL_CL2PLLWAIT | B_PCH_FIA_PCR_PLLCTL_CL3PLLWAIT);
          Data32Or  = (UINT32) ((1 << N_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT) | (1 << N_PCH_FIA_PCR_PLLCTL_CL1PLLWAIT) |
                                 (1 << N_PCH_FIA_PCR_PLLCTL_CL2PLLWAIT) | (1 << N_PCH_FIA_PCR_PLLCTL_CL3PLLWAIT));
          FiaInst.RegisterAccess->AndThenOr32 (
            FiaInst.RegisterAccess,
            R_PCH_FIA_PCR_PLLCTL,
            DataAnd32,
            Data32Or
          );

          //
          // Program PCR[FIA] + 28h bit [4:5, 6:7] to [00, 10]
          // Program PCR[FIA] + 28h bit [17:16, 19:18] to [00, 00]
          //
          DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_PDSDM2_PXPBPDIPSLSD | B_PCD_FIA_PCR_PDSDM2_PXPBPDIPSDM);
          DataOr32 = (V_PCD_FIA_PCR_PDSDM2_PXPBPDIPSDM_16 << N_PCD_FIA_PCR_PDSDM2_PXPBPDIPSDM);
          FiaInst.RegisterAccess->AndThenOr32(
            FiaInst.RegisterAccess,
            R_PCD_FIA_PCR_PDSDM2,
            DataAnd32,
            DataOr32
          );

          //
          // Program PCR[FIA] + 2Ch bit [4:5, 6:7] to [00, 10]
          //
          DataAnd32 = (UINT32)~(B_PCD_FIA_PCR_RXSDM2_PXPBRXIPSLSD | B_PCD_FIA_PCR_RXSDM2_PXPBRXIPSDM);
          DataOr32 = (V_PCD_FIA_PCR_RXSDM2_PXPBRXIPSDM_16 << N_PCD_FIA_PCR_RXSDM2_PXPBRXIPSDM);
          FiaInst.RegisterAccess->AndThenOr32(
            FiaInst.RegisterAccess,
            R_PCD_FIA_PCR_RXSDM2,
            DataAnd32,
            DataOr32
          );
        }
        break;
    }

    switch (FiaPids[Index].Pid.Pid16bit) {
      case PTL_SID_F2_PID_FIA_U:
      case PTL_H_SID_F2_PID_FIA_P5X8:
        break;

      default:
        //
        // Program PCR[FIA] + 20h bit [17:15, 14:12] to [001, 001]
        //
        DataAnd32 = (UINT32) ~(B_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT | B_PCH_FIA_PCR_PLLCTL_CL1PLLWAIT);
        Data32Or  = (UINT32) ((1 << N_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT) | (1 << N_PCH_FIA_PCR_PLLCTL_CL1PLLWAIT));
        FiaInst.RegisterAccess->AndThenOr32 (
          FiaInst.RegisterAccess,
          R_PCH_FIA_PCR_PLLCTL,
          DataAnd32,
          Data32Or
          );
        break;
    }

    //
    // Set PCR[FIA] + 0h bit [16, 15] to [1, 1]
    //
    FiaInst.RegisterAccess->AndThenOr32 (
      FiaInst.RegisterAccess,
      R_PCH_FIA_PCR_CC,
      ~0u,
      B_PCH_FIA_PCR_CC_OSCDCGE | B_PCH_FIA_PCR_CC_SCPTCGE
      );

    //
    // Set PCR[FIA] + 40h bit [3] to [1]
    //
    Data32Or = B_PCH_FIA_PCR_PMC_PRDPGE;
    FiaInst.RegisterAccess->AndThenOr32 (
      FiaInst.RegisterAccess,
      R_PCH_FIA_PCR_PMC,
      ~0u,
      Data32Or
      );

    //
    // Set PCR[FIA] + 48h bit [0] to [0]
    // Set PCR[FIA] + 48h bit [11:8] to 4b1000
    //
    FiaInst.RegisterAccess->AndThenOr32 (
      FiaInst.RegisterAccess,
      R_PCH_FIA_PCR_PGCUC,
      (UINT32) ~(B_PCH_FIA_PCR_PGCUC_ACC_CLKGATE_DISABLED | B_PCH_FIA_PCR_PGCUC_T_CLKGATE),
      (V_PCH_FIA_PCR_PGCUC_T_CLKGATE << N_PCH_FIA_PCR_PGCUC_T_CLKGATE)
      );

    //
    // Set PCR[FIA] + 50h bit [0] to [0]
    //
    FiaInst.RegisterAccess->AndThenOr32 (
      FiaInst.RegisterAccess,
      R_PCH_FIA_PCR_PGCUCSOC,
      (UINT32) ~(B_PCH_FIA_PCR_PGCUCSOC_ACC_SOC_CLKGATE_DISABLED),
      0
      );
  }
}

/**
  Checks if a given USB3 port is assigned any FIA lane

  @param[in]  Usb3PortIndex  USB3 port index

  @return TRUE if given USB3 port is assigned a FIA lane, FALSE otherwise
**/
BOOLEAN
PtlPcdFiaIsUsb3PortConnected (
  IN UINT32  Usb3PortIndex
  )
{
  UINT8 LaneNum;
  return PtlPcdFiaGetUsb3LaneNum (Usb3PortIndex, &LaneNum);
}
