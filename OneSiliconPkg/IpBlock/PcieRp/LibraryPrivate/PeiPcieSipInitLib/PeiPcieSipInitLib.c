/** @file
  This file contains common initialization code for PCIe SIP controller

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

#include <Library/PeiPcieSipInitLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Ppi/SiPolicy.h> // To import FORCE_ENABLE/DISABLE
#include <Register/PcieSipRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PchRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PerformanceLib.h>
#include <Register/PchPcieRpRegs.h>

#define LINK_RETRAIN_WAIT_TIME 1000 // microseconds
#define LINK_ACTIVE_POLL_INTERVAL 100     // in microseconds
#define LINK_ACTIVE_POLL_TIMEOUT  1000000 // in microseconds
#define S_PCIE_PCR_PCD_RP_FIELD   4       ///< 4 bits for each RP FN

#define DISABLE_TIMEOUT      5000


/**
  Configures PTM settings on the PCIe controller.

  @param[in] RpDev      Pointer to the root port device
  @param[in] PtmConfig  Pointer to the PTM configuration structure
**/
VOID
PcieSipConfigurePtm (
  IN PCIE_ROOT_PORT_DEV  *RpDev,
  IN PTM_CONFIGURATION   *PtmConfig
  )
{
  UINT32  PtmRegisterOffset;
  UINT32  PsdRegIndex;
  UINT32  MaxPsdReg;

  if (RpDev == NULL || PtmConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "RpDev or PtmConfig can't be NULL\n"));
    return;
  }

  if (RpDev->SipVersion < PcieSip14) {
    DEBUG ((DEBUG_WARN, "PCIe SIP < %d does not support PTM configuration\n", PcieSip14));
    return;
  }

  //
  // This function can be called when root port 0 has already been hidden. Because of that all accesses done by this function
  // have to be done through sideband access.
  //
  if (RpDev->SipVersion >= PcieSip17) {
    //
    // SIP 17 has moved all PTM registers into RCRB.
    //
    for (PsdRegIndex = 0; PsdRegIndex < MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17; PsdRegIndex++) {
      RpDev->PciSbiMsgMemAccess->Write32 (
        RpDev->PciSbiMsgMemAccess,
        R_PCIE_MEM_PTMPSDC1 + (4 * PsdRegIndex),
        PtmConfig->PtmPipeStageDelay[PsdRegIndex]
        );
    }
  } else {
    if (RpDev->SipVersion == PcieSip16) {
      MaxPsdReg = MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP16;
    } else {
      MaxPsdReg = MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP14;
    }

    for (PsdRegIndex = 0; PsdRegIndex < MaxPsdReg; PsdRegIndex++) {
      if (PsdRegIndex < 5) {
        PtmRegisterOffset = R_PCIE_MEM_PTMPSDC1 + (4 * PsdRegIndex);
      } else if (PsdRegIndex < 8) {
        PtmRegisterOffset = R_PCIE_MEM_PTMPSDC6 + ((PsdRegIndex - 6) * PsdRegIndex);
      } else {
        PtmRegisterOffset = R_PCIE_MEM_PTMPSDC9 + ((PsdRegIndex - 9) * PsdRegIndex);
      }

      RpDev->PciSbiMsgCfgAccess->Write32 (
        RpDev->PciSbiMsgCfgAccess,
        PtmRegisterOffset,
        PtmConfig->PtmPipeStageDelay[PsdRegIndex]
        );
    }
  }

  RpDev->PciSbiMsgCfgAccess->Write32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PTMECFG,
    PtmConfig->PtmConfig
    );
}

/**
  Configure 10-Bit Tag

  @param[in] RpDev               Pointer to the root port device
  @param[in] CompleterSupported  If TRUE it indicates 10-Bit Tag Completer is supported.
  @param[in] RequesterSupported  If TRUE it indicates 10-Bit Tag Requester is supported.

**/
VOID
PcieSipConfigure10BitTag (
  IN PCIE_ROOT_PORT_DEV      *RpDev,
  IN BOOLEAN                 CompleterSupported,
  IN BOOLEAN                 RequesterSupported
  )
{
  if (RpDev->SipVersion < PcieSip16) {
    DEBUG ((DEBUG_WARN, "PCIe SIP < %d does not support 10-bit tag\n", PcieSip16));
    return;
  }

  if (CompleterSupported == FALSE) {
    //
    // Program 10-Bit Tag Completer Supported PX10BTCS = 0x0
    //
    RpDev->PciCfgAccess->And32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCAP2, (UINT32)~B_PCIE_CFG_DCAP2_PX10BTCS);
  } else {
    //
    // Program 10-Bit Tag Completer Supported PX10BTCS = 0x1
    //
    RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCAP2, B_PCIE_CFG_DCAP2_PX10BTCS);
  }

  if (RequesterSupported == FALSE) {
    //
    // Program 10-Bit Tag Requester Enable PX10BTRE = 0x0
    // Program 10-Bit Tag Requester Supported PX10BTRS = 0x0
    //
    RpDev->PciCfgAccess->And32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCTL2, (UINT32)~B_PCIE_CFG_DCTL2_PX10BTRE);
    RpDev->PciCfgAccess->And32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCAP2, (UINT32)~B_PCIE_CFG_DCAP2_PX10BTRS);
    RpDev->PciCfgAccess->And32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCAP, (UINT32)~B_PCIE_DCAP_ETFS);
  } else {
    //
    // Program 10-Bit Tag Requester Enable PX10BTRE = 0x1
    // Program 10-Bit Tag Requester Supported PX10BTRS = 0x1
    //
    RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCTL2, B_PCIE_CFG_DCTL2_PX10BTRE);
    RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_DCAP2, B_PCIE_CFG_DCAP2_PX10BTRS);
  }

  //
  // SipPhyVersion SE32: Program Fabric 10-bit Tag Support Enable F10BTSE = 0x1
  // SipPhyVersion SC16: Program Fabric 10-bit Tag Support Enable F10BTSE = 0x0
  //
  if (RpDev->Integration == UpDmi) {
    RpDev->PciCfgAccess->AndThenOr32 (
                           RpDev->PciCfgAccess,
                           R_PCIE_CFG_ADVMCTRL,
                           (UINT32) ~(B_PCIE_CFG_ADVMCTRL_F10BTSE | B_PCIE_CFG_ADVMCTRL_CCBE | B_PCIE_CFG_ADVMCTRL_PMREQCWC | B_PCIE_CFG_ADVMCTRL_PMREQBLKPGRSPT),
                           (UINT32) ((0x6 << N_PCIE_CFG_ADVMCTRL_PMREQCWC) |
                                     (0x2 << N_PCIE_CFG_ADVMCTRL_PMREQBLKPGRSPT))
                           );
  } else {
    RpDev->PciCfgAccess->And32 (RpDev->PciCfgAccess, R_PCIE_CFG_ADVMCTRL, (UINT32)~B_PCIE_CFG_ADVMCTRL_F10BTSE);
  }
}

/**
  Get max link width.

  @param[in] RpDev  Pointer to the root port device
  @retval           Max link width, 0 when failed
**/
UINT8
PcieSipGetMaxLinkWidth (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  )
{
  UINT8  LinkWidth;

  LinkWidth = (UINT8) ((RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCAP) & B_PCIE_LCAP_MLW) >> N_PCIE_LCAP_MLW);

  if (LinkWidth != 0x3F) {  // LCAP.MLW bits [9:4]
    return LinkWidth;
  } else {
    DEBUG ((DEBUG_ERROR, "Incorrect Link Width = %d\n", LinkWidth));
    return 0;
  }
}

/**
  Get PCIe port number for enabled port.
  @param[in] RpDev  Pointer to the root port device

  @retval Root Port number (1 based)
**/
UINT32
PcieSipGetPortNum (
  IN     PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  return RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCAP) >> N_PCIE_CFG_LCAP_PN;
}

/**
  Get the negotiated link width

  @param[in] RpDev  Pointer to the root port device

  @return negotiated link width
**/
UINT8
PcieSipGetNegotiatedLinkWidth (
  PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  return (UINT8) ((RpDev->PciCfgAccess->Read16 (RpDev->PciCfgAccess, R_PCIE_CFG_LSTS) & B_PCIE_LSTS_NLW) >> N_PCIE_LSTS_NLW);
}

/**
  Checks if lane reversal is enabled on a given Root Port

  @param[in] RpDev  Pointer to root port device.

  @retval TRUE if lane reversal is enbabled, FALSE otherwise
**/
BOOLEAN
PcieSipIsLaneReversalEnabled (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT32  Data32;

  if (RpDev->SipVersion >= PcieSip17) {
    //
    // From SIP17, LR bit is moved from PCIEDBG to STRPFUSECFG
    //
    Data32 = RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_STRPFUSECFG);
    return !! (Data32 & B_PCIE_CFG_STRPFUSECFG_LR);
  } else {
    Data32 = RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_PCIEDBG);
    return !! (Data32 & B_PCIE_CFG_PCIEDBG_DTCA);
  }
}

/**
  Returns the PCIe controller bifurcation

  @param[in] RpDev  Pointer to root port device.

  @retval PCIe controller bifurcation
**/
UINT8
PcieSipGetControllerBifurcation (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT32                      Data32;

  Data32 = RpDev->PciSbiMsgCfgAccess->Read32 (RpDev->PciSbiMsgCfgAccess, R_PCIE_CFG_STRPFUSECFG);
  if (Data32 == MAX_UINT32) {
    DEBUG ((DEBUG_ERROR, "Failed to get controller config, defaulting to Pcie4x1\n"));
    return V_PCH_PCIE_CFG_STRPFUSECFG_RPC_1_1_1_1;
  }

  return (UINT8)((Data32 & B_PCIE_CFG_STRPFUSECFG_RPC) >> N_PCIE_CFG_STRPFUSECFG_RPC);
}

/**
  Get max PCIe link speed supported by the root port.

  @param[in]  RpDev  Pointer to root port device.

  @retval Max link speed
**/
UINT32
PcieSipGetMaxLinkSpeed (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  return RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCAP) & B_PCIE_LCAP_MLS;
}

/**
  Enable PCIe root port in controller.

  @param[in] ControllerDev        Pointer to controller device.
  @param[in] RpIndexInController  Which root port in controller to disable.
**/
VOID
PcieSipEnableRootPortInController (
  IN PCIE_ROOT_PORT_DEV  *ControllerDev,
  IN UINT32              RpIndexInController
  )
{
  if (RpIndexInController >= 4) {
    return;
  }

  //
  // PCIe RP IOSF Sideband register offset 0x00[19:16], depending on the port that is Function Disabled
  // Access it by offset 0x02[4:0] to avoid RWO bit
  //
  ControllerDev->PciPcrAccess->AndThenOr8 (
    ControllerDev->PciPcrAccess,
    R_PCIE_PCR_PCD + 0x02,
    (UINT8) ~(1 << RpIndexInController),
    0x0
    );
}

/**
  Checks if root port is disabled in controller.

  @param[in] ControllerDev        Pointer to controller device.
  @param[in] RpIndexInController  Which root port in controller to disable.

  @retval TRUE   Root port is enabled.
  @retval FALSE  Root port is disabled.
**/
BOOLEAN
PcieSipIsRootPortEnabledInController (
  IN PCIE_ROOT_PORT_DEV  *ControllerDev,
  IN UINT32              RpIndexInController
  )
{
  UINT8  RpDisabled;

  if (RpIndexInController >= 4) {
    return FALSE;
  }

  RpDisabled = ControllerDev->PciPcrAccess->Read8 (ControllerDev->PciPcrAccess, R_PCIE_PCR_PCD + 0x02);
  return !((RpDisabled >> RpIndexInController) & 0x1);
}

/**
  Assign root port function number in PCIe controller.

  @param[in] ControllerDev              Pointer to controller device.
  @param[in] RpIndexInController        Which root port in controller to swap function for.
  @param[in] NewFunctionNumber          New function number.
  @param[in] SwapWithinController       If TRUE function swap is done within a single controller
  @param[in] SecondRpIndexInController  Second root port for which we want to change function number
  @param[in] SecondRpNewFunctionNumber  Second root port function number
**/
VOID
PcieSipAssignRootPortFunctionNumber (
  IN PCIE_ROOT_PORT_DEV  *ControllerDev,
  IN UINT32              RpIndexInController,
  IN UINT16              NewFunctionNumber,
  IN BOOLEAN             SwapWithinController,
  IN UINT32              SecondRpIndexInController,
  IN UINT16              SecondRpNewFunctionNumber
  )
{
  UINT32  Data32And;
  UINT32  Data32Or;

  if (RpIndexInController >= 4 || NewFunctionNumber >= 8) {
    DEBUG ((DEBUG_ERROR, "%a incorrect rp index(%d) or function number(%d)\n", ControllerDev, RpIndexInController, NewFunctionNumber));
    return;
  }

  if (SwapWithinController && (SecondRpIndexInController >= 4 || SecondRpNewFunctionNumber >= 8)) {
    DEBUG ((DEBUG_ERROR, "%a incorrect SecondRpIndex(%d) or SecondRpNewFuncNum(%d)\n", ControllerDev, SecondRpIndexInController, SecondRpNewFunctionNumber));
    return;
  }

  Data32And = (UINT32) ~(B_PCIE_PCR_PCD_RP0FN << (RpIndexInController * S_PCIE_PCR_PCD_RP_FIELD));
  Data32Or = NewFunctionNumber << (RpIndexInController * S_PCIE_PCR_PCD_RP_FIELD);
  if (SwapWithinController) {
    Data32And &= (UINT32) ~(B_PCIE_PCR_PCD_RP0FN << (SecondRpIndexInController * S_PCIE_PCR_PCD_RP_FIELD));
    Data32Or |= SecondRpNewFunctionNumber << (SecondRpIndexInController * S_PCIE_PCR_PCD_RP_FIELD);
  }

  ControllerDev->PciPcrAccess->AndThenOr32 (
    ControllerDev->PciPcrAccess,
    R_PCIE_PCR_PCD,
    Data32And,
    Data32Or
    );
}

/**
  Checks if given rootport has an endpoint connected

  @param[in]  RpDev  Pointer to root port device.

  @retval TRUE   if endpoint is connected
  @retval FALSE  if no endpoint was detected
**/
BOOLEAN
PcieSipIsEndpointConnected (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  return !!(RpDev->PciCfgAccess->Read16 (RpDev->PciCfgAccess, R_PCIE_CFG_SLSTS) & B_PCIE_SLSTS_PDS);
}

/**
  Verify whether the PCIe port does own all lanes according to the port width.

  @param[in] ControllerDev  Pointer to the controller dev.
  @param[in] RpDev          Pointer to root port device.

  @retval TRUE   All lanes are owned by PCIe controller.
  @retval FALSE  Not all lanes are owned by PCIe root port.
**/
BOOLEAN
PcieSipIsRpOwningLanes (
  IN PCIE_ROOT_PORT_DEV  *ControllerDev,
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT32  MaxLinkWidth;
  UINT32  RpLaneIndex;
  UINT32  LaneIndexInController;
  UINT32  FirstRpLaneInController;
  UINT32  TotalLanesInController;

  MaxLinkWidth = RpDev->PrivateConfig.LanesPerRootPort;
  for (RpLaneIndex = 0; RpLaneIndex < MaxLinkWidth; ++RpLaneIndex) {
    if (PcieSipIsLaneReversalEnabled (ControllerDev)) {
      TotalLanesInController = RpDev->PrivateConfig.RootPortsInController * RpDev->PrivateConfig.LanesPerRootPort;
      FirstRpLaneInController = TotalLanesInController - (RpDev->PrivateConfig.RootPortIndexInController * RpDev->PrivateConfig.LanesPerRootPort);
      // 1 is subtracted to make the PhysicalLaneIndex 0-based.
      LaneIndexInController = FirstRpLaneInController - RpLaneIndex - 1;
    } else {
      FirstRpLaneInController = (RpDev->PrivateConfig.RootPortIndexInController * RpDev->PrivateConfig.LanesPerRootPort);
      LaneIndexInController = FirstRpLaneInController + RpLaneIndex;
    }
    if (!RpDev->Callbacks.IsPcieLaneConnected (RpDev, LaneIndexInController)) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Configure PCIe power down mapping

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PcieSipConfigurePowerDownMapping (
  IN  PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT32                      Data32Or;
  UINT32                      Data32And;
  UINT32                      Data32;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  Data32Or = 0;
  Data32And = 0;
  if (RpDev->Integration == PchPcie) {
    Data32Or = (UINT32) ((0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D2PGPDCTL) |
                         (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D2NOPGPDCTL) |
                         (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1PGPGPDCTL) |
                         (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1PGNOPGPDCTL));
    if (RpDev->SipVersion > PcieSip17){
      if (RpDev->IpTrain == PcieIpTrainD2) {
        Data32Or |= (UINT32) ((0x3 << N_PCIE_MEM_PIPEPDCTL2_DUCFGPGPDCTL) |
                            (0x6 << N_PCIE_MEM_PIPEPDCTL2_L1D1UPUPGPDCTL) |
                            (0x6 << N_PCIE_MEM_PIPEPDCTL2_L1UPNOPGPDCTL));
      } else {
        Data32Or |= (UINT32) ((0xD << N_PCIE_MEM_PIPEPDCTL2_DUCFGPGPDCTL) |
                              (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1UPUPGPDCTL) |
                              (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1UPNOPGPDCTL));
      }
    } else {
      Data32Or |= (UINT32) ((0x3 << N_PCIE_MEM_PIPEPDCTL2_DUCFGPGPDCTL) |
                            (0x0 << N_PCIE_MEM_PIPEPDCTL2_L1D1UPUPGPDCTL) |
                            (0x6 << N_PCIE_MEM_PIPEPDCTL2_L1UPNOPGPDCTL));
    }
    Data32And = (UINT32) ~ (B_PCIE_MEM_PIPEPDCTLEXT_P2TP2TCD | B_PCIE_MEM_PIPEPDCTLEXT_P2TP2TP);
  } else if (RpDev->Integration == UpDmi3) {
    Data32Or = (UINT32) ((0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D2PGPDCTL) |
                         (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D2NOPGPDCTL) |
                         (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1PGPGPDCTL) |
                         (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1PGNOPGPDCTL) |
                         (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1UPUPGPDCTL) |
                         (0x6 << N_PCIE_MEM_PIPEPDCTL2_L1UPNOPGPDCTL));
    Data32And = (UINT32) ~(B_PCIE_MEM_PIPEPDCTLEXT_PDDPMRFM | B_PCIE_MEM_PIPEPDCTLEXT_P2TP2TCD | B_PCIE_MEM_PIPEPDCTLEXT_P2TP2TP);
  } else if (RpDev->Integration == UpDmi) {
    Data32Or = (UINT32)((0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D2PGPDCTL) |
                        (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D2NOPGPDCTL) |
                        (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1PGPGPDCTL) |
                        (0x4 << N_PCIE_MEM_PIPEPDCTL2_L1D1PGNOPGPDCTL) |
                        (0x6 << N_PCIE_MEM_PIPEPDCTL2_L1D1UPUPGPDCTL) |
                        (0x6 << N_PCIE_MEM_PIPEPDCTL2_L1UPNOPGPDCTL));
    Data32And = (UINT32) ~ (B_PCIE_MEM_PIPEPDCTLEXT_P2TP2TCD | B_PCIE_MEM_PIPEPDCTLEXT_P2TP2TP);
  }
  if (RpDev->SipVersion > PcieSip17) {
    if (RpDev->IpTrain == PcieIpTrainD2) {
      Data32 = 0x44333333;
    } else {
      Data32 = 0x44DDD3D3;
    }
  } else {
    Data32 = 0x44333333;
  }
  RpDev->PciSbiMsgMemAccess->Write32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PIPEPDCTL, Data32);
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PIPEPDCTL2, ((UINT32)~(B_PCIE_MEM_PIPEPDCTL2_L1UPNOPGPDCTL | B_PCIE_MEM_PIPEPDCTL2_L1D1UPUPGPDCTL)), Data32Or);
  RpDev->PciSbiMsgMemAccess->Or32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PIPEPDCTL3, 0x44);
  Data32Or = 0;
  if (RpDev->SipVersion > PcieSip17) {
    Data32And &= (UINT32) ~(B_PCIE_MEM_PIPEPDCTLEXT_LSDPMRFM | B_PCIE_MEM_PIPEPDCTLEXT_PDDPMRFM);
    Data32Or = (0xE << N_PCIE_MEM_PIPEPDCTLEXT_LSDPMRFM) | (0x0017 << N_PCIE_MEM_PIPEPDCTLEXT_PDDPMRFM);
  }
  if (RpDev->Integration == UpDmi3) {
    Data32Or |= (UINT32) ((0xFFFF << N_PCIE_MEM_PIPEPDCTLEXT_PDDPMRFM) |
                         (0xFF << N_PCIE_MEM_PIPEPDCTLEXT_LSDPMRFM));
  }
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PIPEPDCTLEXT, Data32And, Data32Or);
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  Perform reset prep programming

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PcieSipConfigureResetPrep (
  IN  PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (RpDev->Integration == UpDmi3) {
    RpDev->PciSbiMsgMemAccess->AndThenOr32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_RPR,
      B_PCIE_MEM_RPR_WRM,
      (UINT32) ((0x1 << N_PCIE_MEM_RPR_S5SMTO) |
                (0x1 << N_PCIE_MEM_RPR_S4SMTO) |
                (0x1 << N_PCIE_MEM_RPR_S3SMTO) |
                (0x1 << N_PCIE_MEM_RPR_CRMTO) |
                (0x1 << N_PCIE_MEM_RPR_WRMTO) |
                (0x1 << N_PCIE_MEM_RPR_DMTO) |
                (0x1 << N_PCIE_MEM_RPR_S5SM) |
                (0x1 << N_PCIE_MEM_RPR_S4SM) |
                (0x1 << N_PCIE_MEM_RPR_S3SM) |
                (0x1 << N_PCIE_MEM_RPR_CRM))
      );
    RpDev->PciSbiMsgMemAccess->AndThenOr32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_RPDEC1,
      (UINT32) 0x0,
      (UINT32) ((0x11 << N_PCIE_MEM_RPDEC1_RPCRERT) |
                (0x3 << N_PCIE_MEM_RPDEC1_RPWREPT) |
                (0x3 << N_PCIE_MEM_RPDEC1_RPCREPT) |
                (0x10 << N_PCIE_MEM_RPDEC1_RPWRERT))
      );

    RpDev->PciSbiMsgMemAccess->AndThenOr32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_RPDEC2,
      (UINT32) ~((0xFF << N_PCIE_MEM_RPDEC2_RPS3EPT) |
                (0xFF << N_PCIE_MEM_RPDEC2_RPS4EPT)),
      (UINT32) ((0x3 << N_PCIE_MEM_RPDEC2_RPS3EPT) |
                (0x3 << N_PCIE_MEM_RPDEC2_RPS4EPT) |
                (0x4 << N_PCIE_MEM_RPDEC2_RPS4ERT) |
                (0x3 << N_PCIE_MEM_RPDEC2_RPS3ERT))
      );

    RpDev->PciSbiMsgMemAccess->AndThenOr32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_RPDEC3,
      (UINT32) ~(0xFF << N_PCIE_MEM_RPDEC3_RPS5EPT),
      (UINT32) (0x3 << N_PCIE_MEM_RPDEC3_RPS5EPT |
                (0x5 << N_PCIE_MEM_RPDEC3_RPDH) |
                (0x5 << N_PCIE_MEM_RPDEC3_RPS5ERT))
      );
  } else {
    RpDev->PciSbiMsgMemAccess->Write32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_RPR,
      (UINT32) ((0x1 << N_PCIE_MEM_RPR_S5SMTO) |
                (0x1 << N_PCIE_MEM_RPR_S4SMTO) |
                (0x1 << N_PCIE_MEM_RPR_S3SMTO) |
                (0x1 << N_PCIE_MEM_RPR_CRMTO) |
                (0x1 << N_PCIE_MEM_RPR_WRMTO) |
                (0x1 << N_PCIE_MEM_RPR_DMTO) |
                (0x1 << N_PCIE_MEM_RPR_S5SM) |
                (0x1 << N_PCIE_MEM_RPR_S4SM) |
                (0x1 << N_PCIE_MEM_RPR_S3SM) |
                (0x1 << N_PCIE_MEM_RPR_CRM) |
                (0x1 << N_PCIE_MEM_RPR_WRM))
    );
    if (RpDev->IsController) {
      RpDev->PciSbiMsgMemAccess->AndThenOr32 (
        RpDev->PciSbiMsgMemAccess,
        R_PCIE_MEM_RPDEC1,
        (UINT32) ~((B_PCIE_MEM_RPDEC1_RPCRERT) |
                   (0xFF << N_PCIE_MEM_RPDEC1_RPWREPT) |
                   (0xFF << N_PCIE_MEM_RPDEC1_RPCREPT)),
        (UINT32) ((0x11 << N_PCIE_MEM_RPDEC1_RPCRERT) |
                  (0x10 << N_PCIE_MEM_RPDEC1_RPWRERT))
        );

      RpDev->PciSbiMsgMemAccess->AndThenOr32 (
        RpDev->PciSbiMsgMemAccess,
        R_PCIE_MEM_RPDEC2,
        (UINT32) ~((0xFF << N_PCIE_MEM_RPDEC2_RPS3EPT) |
                   (0xFF << N_PCIE_MEM_RPDEC2_RPS4EPT)),
        (UINT32) ((0x4 << N_PCIE_MEM_RPDEC2_RPS4ERT) |
                  (0x3 << N_PCIE_MEM_RPDEC2_RPS3ERT))
        );

      RpDev->PciSbiMsgMemAccess->AndThenOr32 (
        RpDev->PciSbiMsgMemAccess,
        R_PCIE_MEM_RPDEC3,
        (UINT32) ~(0xFF << N_PCIE_MEM_RPDEC3_RPS5EPT),
        (UINT32) ((0x5 << N_PCIE_MEM_RPDEC3_RPDH) |
                  (0x5 << N_PCIE_MEM_RPDEC3_RPS5ERT))
        );
    }

    RpDev->PciSbiMsgMemAccess->AndThenOr32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_FCTL2,
      (UINT32)~(B_PCIE_MEM_FCTL2_HRTCTL),
      (UINT32) ((0x0 << N_PCIE_MEM_FCTL2_HRTCTL) |
                (0x5 << N_PCIE_MEM_FCTL2_RPTOT))
      );
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  Program Rx primary Cycle Decode Registers for SIP17

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PcieSipRxPrimaryCycleDecode (
  IN  PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT32                      Data32Or;
  UINT32                      Data32And;

  DEBUG ((DEBUG_INFO, "%a for Rp %x\n", __FUNCTION__, RpDev->Id));

  ///
  /// Program (R_PCIE_MEM_DECCTL) 0X1904
  /// Program (ATSCE) 0X1904[1]                0x1
  /// Program (BUSNUMZCHK) 0X1904[2]           0x1
  /// Program (MTRXLTC) 0X1904[3]              0x0
  /// Program (OUTBEXPCPLCHKEN) 0X1904[4]      0x1
  /// Program (DROPCPLATNZCE) 0X1904[5]        0x1
  /// Program (LCRXINT) 0X1904[6]              0x1
  /// Program (VDMATAC) 0X1904[7]              0x1
  /// Program (URVDME16DW) 0X1904[8]           0x1
  /// Program (URRXUVDMINTELVID) 0X1904[9]     0x0
  /// Program (URRXUVDMUVID) 0X1904[10]        0x0
  /// Program (URRXURTRCVDM) 0X1904[11]        0x1
  /// Program (URRXULTVDM) 0X1904[12]          0x1
  /// Program (URRXURAVDM) 0X1904[13]          0x1
  /// Program (URRXURIDVDM) 0X1904[14]         0x1
  /// Program (URRXUORVDM) 0X1904[15]          0x1
  /// Program (URRXVMCTPBFRC) 0X1904[16]       0x1
  /// Program (ICHKINVREQRMSGRBID) 0X1904[17]  0x1
  /// Program (RXMCTPDECEN) 0X1904[18]         0x1
  /// Program (RXVDMDECE) 0X1904[19]           0x0
  /// Program (RXGPEDECEN) 0X1904[20]          0x1
  /// Program (RXSBEMCAPDECEN) 0X1904[21]      0x1 for SIP17
  /// Program (RXSBEMCAPDECEN) 0X1904[21]      0x0 for SipVersion > SIP17
  /// Program (RXADLEDDECEN) 0X1904[22]        0x1
  /// Program (RXLTRMDECH) 0X1904[23]          0x0
  /// Program (LCRXERRMSG) 0X1904[24]          0x1
  /// Program (RSVD_RW) 0X1904[25]             0x1
  /// Program (LCRXPTMREQ) 0X1904[26]          0x1
  /// Program (URRXUVDMRBFRC) 0X1904[27]       0x1
  /// Program (URRXUVDMRGRTRC) 0X1904[28]      0x1
  /// Program (RXMCTPBRCDECEN) 0X1904[29]      0x1
  /// Program (URRXMCTPNTCO) 0x1904[30]        0x1
  /// Program (RXIMDECEN) 0X1904[31]           0x0
  ///
  if (RpDev->Integration != UpDmi) {
    Data32And = (UINT32) ~( B_PCIE_MEM_DECCTL_RXSBEMCAPDECEN | B_PCIE_MEM_DECCTL_URRXUVDMINTELVID | B_PCIE_MEM_DECCTL_URRXUVDMUVID | B_PCIE_MEM_DECCTL_RXVDMDECE |
                         B_PCIE_MEM_DECCTL_RXIMDECEN | B_PCIE_MEM_DECCTL_RXLTRMDECH);
    Data32Or = (UINT32) (B_PCIE_MEM_DECCTL_MTRXLTC | B_PCIE_MEM_DECCTL_ATSCE | B_PCIE_MEM_DECCTL_BUSNUMZCHK | B_PCIE_MEM_DECCTL_OUTBEXPCPLCHKEN | B_PCIE_MEM_DECCTL_DROPCPLATNZCE |
                         B_PCIE_MEM_DECCTL_LCRXINT | B_PCIE_MEM_DECCTL_VDMATAC | B_PCIE_MEM_DECCTL_URVDME16DW | B_PCIE_MEM_DECCTL_URRXURTRCVDM |
                         B_PCIE_MEM_DECCTL_URRXULTVDM | B_PCIE_MEM_DECCTL_URRXURAVDM | B_PCIE_MEM_DECCTL_URRXURIDVDM | B_PCIE_MEM_DECCTL_URRXUORVDM |
                         B_PCIE_MEM_DECCTL_URRXVMCTPBFRC | B_PCIE_MEM_DECCTL_ICHKINVREQRMSGRBID | B_PCIE_MEM_DECCTL_RXMCTPDECEN | B_PCIE_MEM_DECCTL_RXGPEDECEN |
                         B_PCIE_MEM_DECCTL_RXADLEDDECEN | B_PCIE_MEM_DECCTL_LCRXERRMSG | BIT25 |
                         B_PCIE_MEM_DECCTL_LCRXPTMREQ | B_PCIE_MEM_DECCTL_URRXUVDMRBFRC | B_PCIE_MEM_DECCTL_URRXUVDMRGRTRC | B_PCIE_MEM_DECCTL_RXMCTPBRCDECEN |
                         B_PCIE_MEM_DECCTL_URRXMCTPNTCO);
  } else {
    Data32And = (UINT32)~(B_PCIE_MEM_DECCTL_URRXMCTPNTCO);
    Data32Or = 0u;
  }
  if (((RpDev->SipVersion == PcieSip17) && (RpDev->Integration != UpDmi)) || RpDev->PrivateConfig.ReceiveSbemCapDecodeEnable) {
    Data32Or |= B_PCIE_MEM_DECCTL_RXSBEMCAPDECEN;
  }
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_DECCTL, Data32And, Data32Or);

  ///
  /// Program (R_PCIE_MEM_PIDECCTL) 0X190C
  /// Program (CPLBNCHK) 0X190C[0]  0x1
  ///
  RpDev->PciSbiMsgMemAccess->Or32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PIDECCTL, (UINT32)(B_PCIE_MEM_PIDECCTL_CPLBNCHK));
}

/**
  Perform some early Pcie Root port configuration to enable RCRB and program decoder control registers

  @param[in] RpList  Pointer to the root port list.
**/
VOID
PcieSipEarlyDecodeEnable (
  IN PCIE_ROOT_PORT_LIST  *RpList
 )
{
  PCIE_ROOT_PORT_DEV  *RpDev;
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "%a Start.\n", __FUNCTION__));
  for (Status = RpList->ResetToFirst (RpList, &RpDev); Status == EFI_SUCCESS; Status = RpList->GetNextRootPort (RpList, &RpDev)) {
    if (RpDev->SipVersion >= PcieSip17) {
      //
      // Set Completion Ordering Mode
      //
      RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_DC, B_PCIE_CFG_DC_COM);
      //
      // RX primary Cycle Decoding Programming
      //
      PcieSipRxPrimaryCycleDecode (RpDev);
    }
  }
  DEBUG ((DEBUG_INFO, "%a End.\n", __FUNCTION__));
}

/**
  This function determines whether root port is configured in non-common clock mode.
  Result is based on the NCC soft-strap setting.

  @param[in] RpBase      Root Port pci segment base address

  @retval TRUE           Port in NCC SSC mode.
  @retval FALSE          Port not in NCC SSC mode.
**/
BOOLEAN
PcieSipIsNcc (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  if (RpDev->PciCfgAccess->Read16 (RpDev->PciCfgAccess, R_PCIE_CFG_LSTS) & B_PCIE_LSTS_SCC) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Initialize non-common clock port for SIP 16 and SIP 17 controllers

  @param[in]  RpDev       Pointer to root port device.
**/
VOID
PcieSipInitNccPort (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  )
{
  UINT32     Data32Or;
  UINT32     Data32And;

  DEBUG ((DEBUG_INFO, "%a(%d)\n", __FUNCTION__, RpDev->Id));

  if (RpDev->SipVersion >= PcieSip17) {
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCAP, (UINT32) ~B_PCIE_LCAP_APMS, B_PCIE_LCAP_APMS_L1);
    Data32Or = B_PCIE_CFG_MPC2_ASPMCOEN | (0x2 << N_PCIE_CFG_MPC2_ASPMCO);
    RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_MPC2, Data32Or);
    RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_MPC, B_PCIE_CFG_MPC_UCEL);
    RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCAP, B_PCIE_LCAP_EL1);
    Data32And = (UINT32) ~(B_PCIE_LCAP2_LSOSRSS | B_PCIE_LCAP2_LSOSGSSV);
    RpDev->PciCfgAccess->AndThenOr32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCAP2, Data32And, 0xF << N_PCIE_LCAP2_LSOSGSSV);
  }
  RpDev->Callbacks.InitNccPort (RpDev);
  RpDev->PciCfgAccess->And32 (RpDev->PciCfgAccess, R_PCIE_CFG_PCIEALC, (UINT32) ~B_PCIE_CFG_PCIEALC_BLKDQDA);
}

/**
  Initializes ports with NonCommonClock and SSC configuration.
**/
VOID
PcieSipInitNccRootPorts (
  IN PCIE_ROOT_PORT_LIST  *RpList
  )
{
  PCIE_ROOT_PORT_DEV          *RpDev;
  EFI_STATUS                  Status;

  for (Status = RpList->ResetToFirst (RpList, &RpDev); Status == EFI_SUCCESS; Status = RpList->GetNextRootPort (RpList, &RpDev)) {
    if (PcieSipIsNcc (RpDev)) {
      PcieSipInitNccPort (RpDev);
    }
  }
}

/**
  Configure root port function number mapping.

  @param[in] RpList  Pointer to the root port list.
**/
VOID
PcieSipConfigureRpfnMapping (
  IN PCIE_ROOT_PORT_LIST  *RpList
  )
{
  PCIE_ROOT_PORT_DEV  *Func0RpDev;
  PCIE_ROOT_PORT_DEV  *Func0ControllerDev;
  PCIE_ROOT_PORT_DEV  *FirstEnabledFunctionRpDev;
  PCIE_ROOT_PORT_DEV  *FirstEnabledFunctionControllerDev;
  PCIE_ROOT_PORT_DEV  *RpDev;
  BOOLEAN             FunctionEnabled;
  UINT16              OldFunction;
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO,"%a () Start\n", __FUNCTION__));

  for (Status = RpList->ResetToFirst (RpList, &RpDev); Status == EFI_SUCCESS; Status = RpList->GetRootPortOnNextDevice (RpList, &RpDev)) {
    Func0RpDev = RpDev;
    Status = RpList->GetThisController (RpList, &Func0ControllerDev);
    if (EFI_ERROR (Status)) {
      continue;
    }
    FunctionEnabled = FALSE;

    for (; Status == EFI_SUCCESS; Status = RpList->GetRootPortOnNextFunction (RpList, &RpDev)) {
      //
      // In case of VMD device we need to recheck
      // again if it was not hidden by VMD code
      //
      if (RpDev->PciCfgAccess->Read16 (RpDev->PciCfgAccess, 0) != 0xFFFF) {
        FunctionEnabled = TRUE;
        break;
      }
    }

    if (!FunctionEnabled) {
      continue;
    }

    FirstEnabledFunctionRpDev = RpDev;
    DEBUG ((DEBUG_INFO,"%a (0) On Rp %d B %d D %d F %d \n", __FUNCTION__, RpDev->Id, RpDev->Sbdf.Bus, RpDev->Sbdf.Device, RpDev->Sbdf.Function));
    if (FirstEnabledFunctionRpDev->Sbdf.Function != 0) {
      DEBUG ((DEBUG_INFO,"%a (1) On Rp %d B %d D %d F %d \n", __FUNCTION__, RpDev->Id, RpDev->Sbdf.Bus, RpDev->Sbdf.Device, RpDev->Sbdf.Function));
      if (FirstEnabledFunctionRpDev->PcieConfig->PcieCommonConfig.RpFunctionSwap) {
        Status = RpList->GetThisController (RpList, &FirstEnabledFunctionControllerDev);
        if (EFI_ERROR (Status)) {
          continue;
        }
        OldFunction = FirstEnabledFunctionRpDev->Sbdf.Function;
        DEBUG ((DEBUG_INFO, "Swap Rp %d function 0 with Rp %d function %d\n", Func0RpDev->Id, FirstEnabledFunctionRpDev->Id, FirstEnabledFunctionRpDev->Sbdf.Function));
        if (FirstEnabledFunctionControllerDev->Id == Func0ControllerDev->Id) {
          //
          // Since function number fields in PCD register are RWO we need to write all of them at the same time
          // if both root ports are located in the same controller.
          //
          PcieSipAssignRootPortFunctionNumber (FirstEnabledFunctionControllerDev,
                                               FirstEnabledFunctionRpDev->PrivateConfig.RootPortIndexInController,
                                               0,
                                               TRUE,
                                               Func0RpDev->PrivateConfig.RootPortIndexInController,
                                               OldFunction);
        } else {
          PcieSipAssignRootPortFunctionNumber (FirstEnabledFunctionControllerDev,
                                               FirstEnabledFunctionRpDev->PrivateConfig.RootPortIndexInController,
                                               0,
                                               FALSE,
                                               0,
                                               0);
          PcieSipAssignRootPortFunctionNumber (Func0ControllerDev,
                                               Func0RpDev->PrivateConfig.RootPortIndexInController,
                                               OldFunction,
                                               FALSE,
                                               0,
                                               0);
        }
        FirstEnabledFunctionControllerDev->Callbacks.SwapFunctionNumber (FirstEnabledFunctionRpDev, 0);
        Func0RpDev->Callbacks.SwapFunctionNumber (Func0RpDev, OldFunction);
      } else {
        DEBUG ((DEBUG_INFO, "Force enable PCI config for Rp %d\n", Func0ControllerDev->Id));
        PcieSipEnableRootPortInController (Func0ControllerDev, Func0RpDev->PrivateConfig.RootPortIndexInController);
        Func0RpDev->Callbacks.ForceEnablePciConfig (Func0RpDev);
      }
    }
  }
}

/**
  Hides root ports that have been disabled by straps
  or by the controller config.

  @param[in] RpList  Pointer to the root port list.
**/
VOID
PcieSipHideDisableRootPorts (
  IN PCIE_ROOT_PORT_LIST  *RpList
  )
{
  PCIE_ROOT_PORT_DEV  *RpDev;
  PCIE_ROOT_PORT_DEV  *ControllerDev;
  EFI_STATUS          Status;
  UINT8               RpDisableMask;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  for (Status = RpList->ResetToFirst (RpList, &ControllerDev); Status == EFI_SUCCESS; Status = RpList->GetNextController (RpList, &ControllerDev)) {
    switch (PcieSipGetControllerBifurcation (ControllerDev)) {
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_4:
        RpDisableMask = 0xE;
        break;
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_2_2:
        if (ControllerDev->Id >= 8) {
          RpDisableMask = 0xC;
        } else {
          RpDisableMask = 0xA;
        }
        break;
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_2_1_1:
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_16:
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_8:
        RpDisableMask = 0x2;
        break;
      case V_PCH_PCIE_CFG_STRPFUSECFG_RPC_1_1_1_1:
      default:
        RpDisableMask = 0x0;
        break;
    }
    for (RpDev = ControllerDev; Status == EFI_SUCCESS; Status = RpList->GetNextRootPortInThisController (RpList, &RpDev)) {
      if (!PcieSipIsRootPortEnabledInController (ControllerDev, RpDev->PrivateConfig.RootPortIndexInController) ||
          (RpDisableMask & (1 << RpDev->PrivateConfig.RootPortIndexInController)) != 0) {
        DEBUG ((DEBUG_INFO, "Disabling Rp %d\n", RpDev->Id));
        RpDev->Callbacks.DisablePciConfig (RpDev);
      }
    }
  }
}
