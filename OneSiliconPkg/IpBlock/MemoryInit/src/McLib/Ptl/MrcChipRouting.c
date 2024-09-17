/** @file
  This file contains code related to Chip Library Routing.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include "MrcChipRouting.h"
#include "MrcCommon.h"
#include "CMcAddress.h"

/**
  static_pma1 sequence - platform specific registers

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcStaticPma1PlatformSpecific (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_STRUCT PhyPmovrd4;
  DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_STRUCT PhyPmOvrd3;

  Outputs = &MrcData->Outputs;

  PhyPmovrd4.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_REG);
  PhyPmovrd4.Bits.LPM4ClkEnOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4CrResetOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetCriClkEnOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetSRSBClkDisOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetRestoreOvrdEn = 1;
  PhyPmovrd4.Bits.LPM4RetCtrlOvrdEn = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_REG, PhyPmovrd4.Data);

  PhyPmOvrd3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG);
  PhyPmOvrd3.Bits.pgddr5mode = Outputs->IsDdr5 ? 1 : 0;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG, PhyPmOvrd3.Data);
}

/**
  static_pma1 sequence.

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcStaticPma1 (
  IN OUT MrcParameters *const MrcData
  )
{
  DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_STRUCT   UgbControl;
  DDRPHY_MISC_SAUG_CR_PG_CTRL1_STRUCT         PgCtrl1;
  DDRPHY_MISC_SAUG_CR_PG_CTRL2_STRUCT         PgCtrl2;
  DDRPHY_MISC_SAUG_CR_PG_CTRL3_STRUCT         PgCtrl3;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL0_STRUCT        LvrCtrl0;
  DDRPHY_MISC_SAUG_CR_LVR_CTRL1_STRUCT        LvrCtrl1;
  DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_STRUCT     PchDfiCtrl;

  UgbControl.Data = MrcReadCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_REG);
  UgbControl.Bits.UGBRampUpDelay  = 0;
  UgbControl.Bits.BGBiasTrim      = 0;
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_REG, UgbControl.Data);

  PgCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL1_REG);
  PgCtrl1.Bits.VccDDQG_prampictl  = 6;
  PgCtrl1.Bits.VccDD2G_prampictl  = 6;
  PgCtrl1.Bits.VccSAG_prampictl   = 4;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL1_REG, PgCtrl1.Data);

  PgCtrl2.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL2_REG);
  PgCtrl2.Bits.LVRPwrGoodFSMCntDelay = 2;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL2_REG, PgCtrl2.Data);

  PgCtrl3.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL3_REG);
  PgCtrl3.Bits.VccSAXG_prampictl = 4;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PG_CTRL3_REG, PgCtrl3.Data);

  LvrCtrl0.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL0_REG);
  LvrCtrl0.Bits.IOG_LVRPwrGoodOnWhileRelockOvrd = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL0_REG, LvrCtrl0.Data);

  LvrCtrl1.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG);
  LvrCtrl1.Bits.VccClk_LVRPwrGoodOnWhileRelockOvrd = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG, LvrCtrl1.Data);

  PchDfiCtrl.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_REG);
  PchDfiCtrl.Bits.LpMode4IntPropDelay = 2;
  PchDfiCtrl.Bits.LPM4DisSAGPwrDn = 1;
  MrcWriteCR (MrcData, DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_REG, PchDfiCtrl.Data);

  MrcStaticPma1PlatformSpecific (MrcData);
}
