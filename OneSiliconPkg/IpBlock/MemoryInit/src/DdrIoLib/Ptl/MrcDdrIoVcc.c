/** @file
  This file contains code related to initializing and configuring the DDRIO VCC.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include "CMrcTypes.h"
#include "CMcAddress.h"
#include "MrcDdrIoDefines.h"
#include "MrcCommon.h"
#include "MrcDdrIoVcc.h"
#include "MrcDdrIoUtils.h"

/**
  Calculate VrefSel for VccDist according to the formula
  in PHY MRC User Guide Calibration Spec.

  @param[in] MrcData       - All the MRC global data.
  @param[in] SelVdd2Ladder - SelVdd2Ladder value that was programmed in WP register
  @param[in] VccTarg       - Target voltage.

  @retval VrefSel value
**/
UINT32
CalculateVrefSelVccDist (
  IN MrcParameters *const MrcData,
  IN UINT32        SelVdd2Ladder,
  IN UINT32        VccTarg
  )
{
  MrcOutput *Outputs;

  UINT32 VccRef, VrefSel;

  Outputs = &MrcData->Outputs;

  // VccRef = SELVDD2LADDER ? VccDD2 : 0.8
  // LVR Target Code = (2/3) * VccTarg * 192 / VccRef
  VccRef = SelVdd2Ladder ? Outputs->Vdd2Mv : BG_REF_MV;
  VrefSel = UDIVIDEROUND (2 * VccTarg * 192, 3 * VccRef);

  return VrefSel;
}

/**
  Calculate VrefSel for VccClk* and VccIog* according to the formula
  in PHY MRC User Guide Calibration Spec.

  @param[in] MrcData       - All the MRC global data.
  @param[in] SelVdd2Ladder - SelVdd2Ladder value that was programmed in WP register
  @param[in] VccTarg       - Target voltage.

  @retval VrefSel value
**/
UINT32
CalculateVrefSel (
  IN MrcParameters *const MrcData,
  IN UINT32        SelVdd2Ladder,
  IN UINT32        VccTarg
  )
{
  MrcOutput *Outputs;

  UINT32 VccRef, VrefSel;

  Outputs = &MrcData->Outputs;

  // VccRef = SELVDD2LADDER ? VccDD2 : 0.8
  // LVR Target Code = (4/5) * VccTarg * 192 / VccRef
  VccRef = SelVdd2Ladder ? Outputs->Vdd2Mv : BG_REF_MV;
  VrefSel = UDIVIDEROUND (4 * VccTarg * 192, 5 * VccRef);

  return VrefSel;
}

/**
  This function programs the Vcc LVR registers

  @param[in, out] MrcData - All the MRC global data.
  @retval Returns none
**/
VOID
SetVccLvr (
  IN MrcParameters *const MrcData
 )
{
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MrcSaGvPoint      SaGvPoint;
  UINT32            Offset;
  UINT32            TempVar1;
  UINT32            TempVar2;
  UINT32            VrefSel;
  UINT32            VrefSelClk;
  UINT32            VrefSelIog;
  BOOLEAN           EnOverclockingForLVR;
  UINT32            DataParMax;
  UINT32            VccClk;
  UINT32            VccIog;
  UINT32            Index;
  UINT32            SBClock;
  BOOLEAN           IsLpddr5;
  UINT32            SelVdd2Ladder;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_STRUCT VccIogControl;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_STRUCT VccDdqControl;
  DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_STRUCT       RptChCrMisc;
  DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_STRUCT           WorkPoint0;
  DDRCCC_SBMEM_CR_PM_MISC_CTRL_STRUCT                 PmMiscCtrl;
  DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT            DllCbTune0;
  DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_STRUCT              PmaOvrd;
  DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_STRUCT               PmaOvrd3;
  DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT            DataVccClkWp0;
  DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT             CccVccClkWp0;
  DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT            DataVccIogWp0;
  DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT             CccVccIogWp0;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP0_STRUCT    CompVccIog40Wp0;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP0_STRUCT      CompVccIogWp0;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL1_STRUCT                LvrCtrl1;
  DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_STRUCT       PllFuseVirtual;
  DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT                 DdrCrVssHi;

  Inputs     = &MrcData->Inputs;
  Outputs    = &MrcData->Outputs;
  IsLpddr5   = Outputs->IsLpddr5;
  SaGvPoint  = Outputs->SaGvPoint;
  VccClk     = Outputs->VccClkVoltage;
  VccIog     = Outputs->VccIogVoltage;
  Offset     = 0;
  EnOverclockingForLVR = (Outputs->IsDdr5 && (Outputs->Frequency > f6400));
  SBClock              = 1000000 / 400;
  if ((Inputs->IsDdrIoDtHalo)) {
    DataParMax = MRC_DATA_DT_NUM;
  } else {
    // Mobile
    DataParMax = MRC_DATA_MOBILE_NUM;
  }

  SelVdd2Ladder = 0;
  VrefSelClk = CalculateVrefSel (MrcData, SelVdd2Ladder, VccClk);
  VrefSelIog = CalculateVrefSel (MrcData, SelVdd2Ladder, VccIog);
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "clklvr_vrefsel: %u\n, ioglvr_vrefsel: %u\n",  VrefSelClk, VrefSelIog);

  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP1_REG, SaGvPoint);
  VccIogControl.Data = MrcReadCR (MrcData, Offset);

  VccIogControl.Bits.Disable = 0;
  VccIogControl.Bits.RemoteSense = 0;
  VccIogControl.Bits.OCDivider   = EnOverclockingForLVR ? 1 : 0;
  // Min(383, Rnd( VccIog/(BGRef/386*IF(OCDivider,1.5,1)) -1) )
  TempVar1 = (VccIogControl.Bits.OCDivider ? 1500 : 1000);
  TempVar2 = UDIVIDEROUND (THOUSAND_MULTIPLIER * VccIog * MRC_COMP_VREF_CALCULATION_VALUE, TempVar1 * BG_REF_MV) - 1;
  VccIogControl.Bits.Target = MIN (383, TempVar2);
  VccIogControl.Bits.BiasAdj   = 7;
  VccIogControl.Bits.AdaptiveZeroAdj1 = 2;
  VccIogControl.Bits.AdaptiveZeroAdj2 = 12;
  TempVar1 = 80000 / (16 * SBClock);
  TempVar2 = MrcLog2Round (MrcData, TempVar1);
  VccIogControl.Bits.LockTimer = RANGE (TempVar2, 0, 7);
  VccIogControl.Bits.LvrPM = 1;
  MrcWriteCR (MrcData, Offset, VccIogControl.Data);

  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP1_REG, SaGvPoint);
  VccDdqControl.Data = MrcReadCR (MrcData, Offset);
  VccDdqControl.Bits.Disable = 0;
  //VccDDQ is always bypassed in DDR5 mode
  VccDdqControl.Bits.Bypass = (Outputs->IsDdr5) ? 1 : 0;
  VccDdqControl.Bits.RemoteSense = 0;
  VccDdqControl.Bits.OCDivider   = EnOverclockingForLVR ? 1 : 0;
  VccDdqControl.Bits.Target = GetVddqControlTarget (MrcData, Outputs->VccddqVoltage);
  VccDdqControl.Bits.BiasAdj = 7;
  VccDdqControl.Bits.AdaptiveZeroAdj1 = 2;
  VccDdqControl.Bits.AdaptiveZeroAdj2 = 12;
  TempVar1 = 80000 / (16 * SBClock);
  TempVar2 = MrcLog2Round (MrcData, TempVar1);
  VccDdqControl.Bits.LockTimer = RANGE (TempVar2, 0, 7);
  VccDdqControl.Bits.LvrPM = 1;
  MrcWriteCR (MrcData, Offset, VccDdqControl.Data);

  // Override Work Point update fields to let the settings propagate into LVR
  PmaOvrd.Data = MrcReadCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG);
  PmaOvrd.Bits.WorkPointUpdateOvrd = 1;
  PmaOvrd.Bits.WorkPointUpdateOvrdVal = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG, PmaOvrd.Data);

  PmaOvrd3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG);
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdEn = 1;
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdVal = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG, PmaOvrd3.Data);

  LvrCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG);
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrdEn = 1;
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrd = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG, LvrCtrl1.Data);

  // VccDist
  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_REG, DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT1_REG, SaGvPoint);
  WorkPoint0.Data = MrcReadCR (MrcData, Offset);

  WorkPoint0.Bits.DISTGLVRSelVdd2Ladder = 0; // 1: for Vccdd2_hv, 0: for Bgvref (0.8V)

  VrefSel = CalculateVrefSelVccDist (MrcData, SelVdd2Ladder, VccClk);
  WorkPoint0.Bits.DISTGLVRVrefSel = VrefSel + 4;  // GB 4
  WorkPoint0.Bits.Gear4 = Outputs->GearMode ? 1 : 0;
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "DISTGLVRVrefSel: %u\n", WorkPoint0.Bits.DISTGLVRVrefSel);
  MrcWriteCR (MrcData, Offset, WorkPoint0.Data);

  for (Index = 0; Index < DataParMax; Index++) {
    // VccClk Data
    Offset = OFFSET_CALC_MC_CH (DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG, DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP0_REG, Index,
                                DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP1_REG, SaGvPoint);
    DataVccClkWp0.Data = MrcReadCR (MrcData, Offset);
    DataVccClkWp0.Bits.clklvr_selvdd2_ladder = SelVdd2Ladder;
    DataVccClkWp0.Bits.clklvr_vrefsel = VrefSelClk;
    DataVccClkWp0.Bits.clklvr_rxdllb1vrefsel = VrefSelClk;
    DataVccClkWp0.Bits.clklvr_rxdllb0vrefsel = VrefSelClk;
    MrcWriteCR (MrcData, Offset, DataVccClkWp0.Data);

    // VccIog Data
    Offset = OFFSET_CALC_MC_CH (DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG, DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP0_REG, Index,
                                DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP1_REG, SaGvPoint);
    DataVccIogWp0.Data = MrcReadCR (MrcData, Offset);
    DataVccIogWp0.Bits.ioglvr_selvdd2_ladder = SelVdd2Ladder;
    DataVccIogWp0.Bits.ioglvr_vrefsel = VrefSelIog;
    MrcWriteCR (MrcData, Offset, DataVccIogWp0.Data);
  }

  for (Index = 0; Index < MRC_CCC_SHARED_NUM; Index++) {
    // VccClk CCC
    Offset = OFFSET_CALC_MC_CH (DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG, DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP0_REG, Index,
                                DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP1_REG, SaGvPoint);
    CccVccClkWp0.Data = MrcReadCR (MrcData, Offset);
    CccVccClkWp0.Bits.clklvr_selvdd2_ladder = SelVdd2Ladder;
    CccVccClkWp0.Bits.clklvr_vrefsel = VrefSelClk;
    MrcWriteCR (MrcData, Offset, CccVccClkWp0.Data);

    // VccIog CCC
    Offset = OFFSET_CALC_MC_CH (DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP0_REG, DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP0_REG, Index,
                                DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP0_REG, SaGvPoint);
    CccVccIogWp0.Data = MrcReadCR (MrcData, Offset);
    CccVccIogWp0.Bits.ioglvr_selvdd2_ladder = SelVdd2Ladder;
    CccVccIogWp0.Bits.ioglvr_vrefsel = VrefSelIog;
    MrcWriteCR (MrcData, Offset, CccVccIogWp0.Data);
  }

  // VccIog COMP
  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP1_REG, SaGvPoint);
  CompVccIogWp0.Data = MrcReadCR (MrcData, Offset);
  CompVccIogWp0.Bits.ioglvr_selvdd2_ladder = SelVdd2Ladder;
  CompVccIogWp0.Bits.ioglvr_vrefsel = VrefSelIog;
  MrcWriteCR (MrcData, Offset, CompVccIogWp0.Data);

  Offset = OFFSET_CALC_CH (DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP0_REG, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP1_REG, SaGvPoint);
  CompVccIog40Wp0.Data = MrcReadCR (MrcData, Offset);
  CompVccIog40Wp0.Bits.ioglvr_selvdd2_ladder = SelVdd2Ladder;
  CompVccIog40Wp0.Bits.ioglvr_vrefsel = VrefSelIog;
  MrcWriteCR (MrcData, Offset, CompVccIog40Wp0.Data);

  // Restore the overrides
  PmaOvrd.Data = MrcReadCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG);
  PmaOvrd.Bits.WorkPointUpdateOvrd = 0;
  PmaOvrd.Bits.WorkPointUpdateOvrdVal = 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG, PmaOvrd.Data);

  PmaOvrd3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG);
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdEn = 0;
  PmaOvrd3.Bits.IOGLVRWorkPointUpdateOvrdVal = 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG, PmaOvrd3.Data);

  LvrCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG);
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrdEn = 0;
  LvrCtrl1.Bits.VccClk_LVRWPUpdateOvrd = 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG, LvrCtrl1.Data);

  for (Index = 0; Index < MRC_CCC_SHARED_NUM; Index++) {
    Offset = OFFSET_CALC_CH (DDRCCC_SBMEM0_CR_PM_MISC_CTRL_REG, DDRCCC_SBMEM1_CR_PM_MISC_CTRL_REG, Index);
    PmMiscCtrl.Data = MrcReadCR (MrcData, Offset);
    PmMiscCtrl.Bits.EnIOGDDQGLpMode4Mask = 1;
    PmMiscCtrl.Bits.IOG_LVREn_MaskforLp4 = 1;
    PmMiscCtrl.Bits.IOG_LVRVrefEn_MaskforLp4 = 1;
    MrcWriteCR (MrcData, Offset, PmMiscCtrl.Data);
  }

  for (Index = 0; Index < DataParMax; Index++) {
    Offset = OFFSET_CALC_CH (DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG, DDRDATA_SBMEM1_CR_DDRCRDLLCBTUNE0_REG, Index);
    DllCbTune0.Data = MrcReadCR (MrcData, Offset);
    DllCbTune0.Bits.bw4 = 9;
    MrcWriteCR (MrcData, Offset, DllCbTune0.Data);

    Offset = OFFSET_CALC_CH (DDRDATA_SBMEM0_CR_DDRCRTXDLLCBTUNE0_REG, DDRDATA_SBMEM1_CR_DDRCRTXDLLCBTUNE0_REG, Index);
    DllCbTune0.Data = MrcReadCR (MrcData, Offset);
    DllCbTune0.Bits.bw4 = 9;
    MrcWriteCR (MrcData, Offset, DllCbTune0.Data);
  }

  for (Index = 0; Index < MRC_VCCCLK_MOBILE_NUM; Index++) {
    Offset = OFFSET_CALC_MC_CH (DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG, DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP0_REG, Index,
                                DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP1_REG, SaGvPoint);
    RptChCrMisc.Data = MrcReadCR (MrcData, Offset);
    RptChCrMisc.Bits.TLTermEn2TLClken = 4;
    RptChCrMisc.Bits.TLClken2TLTermEn = 4;
    MrcWriteCR (MrcData, Offset, RptChCrMisc.Data);
  }

  PllFuseVirtual.Data = MrcReadCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_REG);
  PllFuseVirtual.Bits.PLLEnLow = 0;
  PllFuseVirtual.Bits.LockCountP_M = 1;
  PllFuseVirtual.Bits.ColdLockTime = 0x100;
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_REG, PllFuseVirtual.Data);

  DdrCrVssHi.Data = 0;
  DdrCrVssHi.Bits.vsshilvr_vrefsel = IsLpddr5 ? 4 : 5;
  MrcWriteCrMulticast (MrcData, DDRCCC_SBMEM_CR_DDRCRVSSHI_REG, DdrCrVssHi.Data);
  MrcWriteCrMulticast (MrcData, DDRDATA_SBMEM_CR_DDRCRVSSHI_REG, DdrCrVssHi.Data);
  MrcWriteCrMulticast (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVSSHI_REG, DdrCrVssHi.Data);
}

/**
  Set the final VccIog value into Outputs->VccIogVoltage

  @param[in, out] MrcData - All the MRC global data.
**/
VOID
SetVccIo (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  MrcDebug  *Debug;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  if (Outputs->Frequency <= f8533) {
    Outputs->VccIogVoltage = 750;
  } else {
    Outputs->VccIogVoltage = 850;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "VccIo(mV) = %u\n", Outputs->VccIogVoltage);
}

/**
  This function calculates the VccClk using frequency table. Sets the final value into Outputs->VccClkVoltage

  @param[in, out] MrcData - All the MRC global data.
  @retval Returns none
**/
VOID
SetVccClk (
  IN MrcParameters *const MrcData
)
{
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  MrcFrequency  DdrFrequency;
  UINT32        VccClk;
  BOOLEAN       IsDdr5;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  IsDdr5 = Outputs->IsDdr5;
  DdrFrequency = Outputs->Frequency;

  if (IsDdr5) {
    if (DdrFrequency <= f3200) {
      VccClk = 600;
    } else {
      VccClk = 750;
    }
  } else {
    if (DdrFrequency <= f3200) {
      VccClk = 600;
    } else if (DdrFrequency > f3200 && DdrFrequency <= f8533) {
      VccClk = 750;
    } else {
      VccClk = 850;
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "VccClk(mV) = %d\n", VccClk);
  Outputs->VccClkVoltage = VccClk;
}
