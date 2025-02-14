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

#include <IpWrapper.h>
#include <IpPcieConfig.h>
#include <IpPcieRegs.h>
#include <IpPcieDefines.h>
#include "IpPcieSipInitLib.h"

  #define LINK_RETRAIN_WAIT_TIME 1000 // microseconds
  #define LINK_ACTIVE_POLL_INTERVAL 100     // in microseconds
  #define LINK_ACTIVE_POLL_TIMEOUT  1000000 // in microseconds

  #define DISABLE_TIMEOUT      5000

/**
  Configure all LTR subtraction registers according to configuration

  @param[in] pInst                   *pInst
  @param[in] L1StandardConfig        Pointer to L1Standard LTR subtraction config
  @param[in] L1p1Config              Pointer to L1.1 LTR subtraction config
  @param[in] L1p2Config              Pointer to L1.2 LTR subtraction config
  @param[in] LtrSubL11Npg            Pointer to L1.1 No Power Gate LTR subtraction config
**/
void
SipConfigureLtrSubstraction (
  IP_PCIE_INST               *pInst,
  IP_LTR_SUBTRACTION_CONFIG  *L1StandardConfig,
  IP_LTR_SUBTRACTION_CONFIG  *L1p1Config,
  IP_LTR_SUBTRACTION_CONFIG  *L1p2Config,
  IP_LTR_SUBTRACTION_CONFIG  *LtrSubL11Npg
  )
{
  LTRSUBL1STD_PCIE_CFG_STRUCT  LtrSubL1Std;
  LTRSUBL11_PCIE_CFG_STRUCT    LtrSubL11;
  LTRSUBL12_PCIE_CFG_STRUCT    LtrSubL12;
  LTRSUBL11NPG_PCIE_CFG_STRUCT LtrSubL11npg;

  PRINT_LEVEL1 ("%s(%d) Start \n", __FUNCTION__, pInst->RpIndex);

  if (pInst->SipVersion < IpPcieSip16) {
    PRINT_WARNING ( "PCIe SIP < %d does not support LTR subtraction\n", IpPcieSip16);
    return;
  }

  /// Program LTR Subtraction for L1.Standard register 0x0C5C
  LtrSubL1Std.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTRSUBL1STD_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (L1StandardConfig->NoSnoopEnable) {
    LtrSubL1Std.Bits.ltrnslsuben = 1;
    LtrSubL1Std.Bits.ltrnslssubv = L1StandardConfig->NoSnoopScale;
    LtrSubL1Std.Bits.ltrnslsubv  = L1StandardConfig->NoSnoopValue;
  }

  if (L1StandardConfig->SnoopEnable) {
    LtrSubL1Std.Bits.ltrslsuben = 1;
    LtrSubL1Std.Bits.ltrslssubv = L1StandardConfig->SnoopScale;
    LtrSubL1Std.Bits.ltrslsubv  = L1StandardConfig->SnoopValue;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTRSUBL1STD_PCIE_CFG_REG, LtrSubL1Std.Data, IpWrRegFlagSize32Bits);

  /// Program LTR Subtraction for L1.1 Power Gate register 0x0C60
  LtrSubL11.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTRSUBL11_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (L1p1Config->NoSnoopEnable) {
    LtrSubL11.Bits.ltrnslsuben = 1;
    LtrSubL11.Bits.ltrnslssubv = L1p1Config->NoSnoopScale;
    LtrSubL11.Bits.ltrnslsubv  = L1p1Config->NoSnoopValue;
  }

  if (L1p1Config->SnoopEnable) {
    LtrSubL11.Bits.ltrslsuben = 1;
    LtrSubL11.Bits.ltrslssubv = L1p1Config->SnoopScale;
    LtrSubL11.Bits.ltrslsubv  = L1p1Config->SnoopValue;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTRSUBL11_PCIE_CFG_REG, LtrSubL11.Data, IpWrRegFlagSize32Bits);

  /// Program LTR Subtraction for L1.2 register 0x0C64
  LtrSubL12.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTRSUBL12_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (L1p2Config->NoSnoopEnable) {
    LtrSubL12.Bits.ltrnslsuben = 1;
    LtrSubL12.Bits.ltrnslssubv = L1p2Config->NoSnoopScale;
    LtrSubL12.Bits.ltrnslsubv  = L1p2Config->NoSnoopValue;
  }

  if (L1p2Config->SnoopEnable) {
    LtrSubL12.Bits.ltrslsuben = 1;
    LtrSubL12.Bits.ltrslssubv = L1p2Config->SnoopScale;
    LtrSubL12.Bits.ltrslsubv  = L1p2Config->SnoopValue;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTRSUBL12_PCIE_CFG_REG, LtrSubL12.Data, IpWrRegFlagSize32Bits);

  /// Program LTR Subtraction for L1.1 No Power Gate register 0x0C68
  LtrSubL11npg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTRSUBL11NPG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (LtrSubL11Npg->NoSnoopEnable) {
    LtrSubL11npg.Bits.ltrnslsuben = 1;
    LtrSubL11npg.Bits.ltrnslssubv = LtrSubL11Npg->NoSnoopScale;
    LtrSubL11npg.Bits.ltrnslsubv  = LtrSubL11Npg->NoSnoopValue;
  }

  if (LtrSubL11Npg->SnoopEnable) {
    LtrSubL11npg.Bits.ltrslsuben = 1;
    LtrSubL11npg.Bits.ltrslssubv = LtrSubL11Npg->SnoopScale;
    LtrSubL11npg.Bits.ltrslsubv  = LtrSubL11Npg->SnoopValue;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTRSUBL11NPG_PCIE_CFG_REG, LtrSubL11npg.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Configures PTM settings on the PCIe controller.

  @param[in] pInst      *pInst
  @param[in] PtmConfig  Pointer to the PTM configuration structure
**/
void
SipConfigurePtm (
  IP_PCIE_INST           *pInst,
  IP_PTM_CONFIGURATION   *PtmConfig
  )
{
  UINT32            PsdRegIndex;

  PRINT_LEVEL1 ("%s(%d) Start \n", __FUNCTION__, pInst->RpIndex);

  //
  // This function can be called when root port 0 has already been hidden. Because of that all accesses done by this function
  // have to be done through sideband access.
  // Program PTM Pipe Stage Delay Configuration 1 register 0x1920

  for (PsdRegIndex = 0; PsdRegIndex < IP_MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17; PsdRegIndex++) {
    IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PTMPSDC1_PCIE_MEM_RCRB_REG + (4 * PsdRegIndex), PtmConfig->PtmPipeStageDelay[PsdRegIndex], IpWrRegFlagSize32Bits);
  }

  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PTMECFG_PCIE_CFG_REG, PtmConfig->PtmConfig, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Enables retimer presence and two retimers presence detect on supported
  SIP versions.

  @param[in] pInst  Pointer to the IP_PCIE_INST
**/
void
SipConfigureRetimerSupport (
  IP_PCIE_INST  *pInst
  )
{
  LCAP2_PCIE_CFG_STRUCT       Lcap2;

  if (pInst->SipVersion < IpPcieSip16) {
    PRINT_LEVEL1 ("PCIe SIP < %d does not support retimer presence detect\n", IpPcieSip16);
    return;
  }

  /// Program Link Capabilities 2 register 0x006C
  Lcap2.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCAP2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Lcap2.Bits.rpds = 1;
  Lcap2.Bits.trpds = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCAP2_PCIE_CFG_REG, Lcap2.Data, IpWrRegFlagSize32Bits);
}

/**
  Configure Pcie Squelch Power Management.

  @param[in] pInst                *pInst
**/
void
SipConfigureSquelchPowerManagement (
  IP_PCIE_INST  *pInst
  )
{
  PCIECFG2_PCIE_CFG_STRUCT            PcieCfg2;
  PCIEDBG_PCIE_CFG_STRUCT             PcieDbg;
  LPHYCP4_PCIE_MEM_RCRB_STRUCT        LphyCp4;
  PWRCTL_PCIE_CFG_STRUCT              PwrCtl;
  PHYCTL3_PCIE_CFG_STRUCT             PhyClt3;
  RXL0SESQCP4_PCIE_MEM_RCRB_STRUCT    Rxlosesqcp4Cfg;
  TXCRSTOCTL_PCIE_MEM_RCRB_STRUCT     TxCrsToCtl;
  RXL0SESQCP5_PCIE_MEM_RCRB_STRUCT    Rxl0sesqcp5;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  //
  // Enable Squelch propagation control
  // Program Link Clock Domain Squelch Exit Debounce Timers to 0ns
  // Program PCI Express Debug And Configuration register 0x0324
  //
  PcieDbg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEDBG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieDbg.Bits.spce = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEDBG_PCIE_CFG_REG, PcieDbg.Data, IpWrRegFlagSize32Bits);

  //
  // Program Log Phy Control Policy 4 register 0x1434
  //
  LphyCp4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, LPHYCP4_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  LphyCp4.Bits.lgclksqexitdbtimers = V_LPHYCP4_LGCLKSQEXITDBTIMERS;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, LPHYCP4_PCIE_MEM_RCRB_REG, LphyCp4.Data, IpWrRegFlagSize32Bits);
  //
  // Disable RxElecIdle Sampling Policy (RSP) and Root Port Detect Squelch Polling
  // and program the Root Port L1 Squelch Polling.
  // Program Power Control register 0x00E8
  //
  PwrCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PWRCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PwrCtl.Bits.rpl1sqpol = 0;
  PwrCtl.Bits.rpdtsqpol = 0;
  PwrCtl.Bits.dlp       = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PWRCTL_PCIE_CFG_REG, PwrCtl.Data, IpWrRegFlagSize32Bits);

  //
  // Configure Squelch Direction settings
  // Program Physical Layer And AFE Control 3 register 0x00F6
  //
  PhyClt3.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PHYCTL3_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
  PhyClt3.Bits.sqdirovren = 0;
  PhyClt3.Bits.sqdirctrl = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PHYCTL3_PCIE_CFG_REG, PhyClt3.Data, IpWrRegFlagSize8Bits);

  //
  // Program PCI Express Debug And Configuration register 0x0324
  // Disable Squelch Off in L0
  //
  PcieDbg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEDBG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieDbg.Bits.sqol0 = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEDBG_PCIE_CFG_REG, PcieDbg.Data, IpWrRegFlagSize32Bits);

  //
  // Enable Low Bandwidth Squelch Settling Timer
  // Set this before enabling any of the squelch power management
  // Program PCI Express Configuration 2 register 0x0320
  //
  //@todo - revisit PCIECFG2.CROAOV, PCIECFG2.CROAOE
  PcieCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIECFG2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieCfg2.Bits.crsren = 1;
  PcieCfg2.Bits.rrcp = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIECFG2_PCIE_CFG_REG, PcieCfg2.Data, IpWrRegFlagSize32Bits);

  //
  // @todo need to revisit Un-Squelch Sampling Period programing
  //

  Rxlosesqcp4Cfg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, RXL0SESQCP4_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  if (pInst->SipPhyVersion == IP_HSPHY) {
    Rxlosesqcp4Cfg.Bits.g1lbwsst  = 0x0A;
    Rxlosesqcp4Cfg.Bits.g2lbwsst  = 0x0A;
  } else {
    Rxlosesqcp4Cfg.Bits.g1lbwsst  = 0x18;
    Rxlosesqcp4Cfg.Bits.g2lbwsst  = 0x0C;
  }

  Rxlosesqcp4Cfg.Bits.g3lbwsst  = 0x14;
  Rxlosesqcp4Cfg.Bits.g4lbwsst  = 0x04;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, RXL0SESQCP4_PCIE_MEM_RCRB_REG, Rxlosesqcp4Cfg.Data, IpWrRegFlagSize32Bits);

  Rxl0sesqcp5.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, RXL0SESQCP5_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Rxl0sesqcp5.Bits.g5lbwsst = 0x4;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, RXL0SESQCP5_PCIE_MEM_RCRB_REG, Rxl0sesqcp5.Data, IpWrRegFlagSize32Bits);

  TxCrsToCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, TXCRSTOCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  TxCrsToCtl.Bits.crs_to_dis = 0x1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, TXCRSTOCTL_PCIE_MEM_RCRB_REG, TxCrsToCtl.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Configures the dynamic clock gating

  @param[in] pInst  *pInst
**/
void
SipConfigureDynamicClockGating (
   IP_PCIE_INST  *pInst
)
{
  DCGM1_PCIE_MEM_RCRB_STRUCT    DcgM1;
  DCGM2_PCIE_MEM_RCRB_STRUCT    DcgM2;
  DCGM3_PCIE_MEM_RCRB_STRUCT    DcgM3;
  DCGEN1_PCIE_MEM_RCRB_STRUCT   DcgEn1;
  DCGEN2_PCIE_MEM_RCRB_STRUCT   DcgEn2;
  DCGEN3_PCIE_MEM_RCRB_STRUCT   DcgEn3;
  IPCLKCTR_PCIE_MEM_RCRB_STRUCT IpClkCtr;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  //
  // To enable the Unit Level Dynamic Clock Gating, BIOS is required to set the enable bits in DCGM1, 2, 3, 4 before setting any bits in DCGEN1, 2, 3, 4
  // Program DCG Mode 1 register 0x1358
  //
  DcgM1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, DCGM1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  DcgM1.Bits.pxttsuldcgm = 1;
  DcgM1.Bits.pxkguldcgm = 1;
  DcgM1.Bits.pxtrsuldcgm = 1;
  DcgM1.Bits.pxtruldcgm = 1;
  DcgM1.Bits.pxlsuldcgm = 1;
  DcgM1.Bits.pxliuldcgm = 1;
  DcgM1.Bits.pxltuldcgm = 1;
  DcgM1.Bits.pxlruldcgm = 1;
  DcgM1.Bits.pxculdcgm = 1;
  DcgM1.Bits.pxttuldcgm = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, DCGM1_PCIE_MEM_RCRB_REG, DcgM1.Data, IpWrRegFlagSize32Bits);

  //
  // Program DCG Mode 2 register 0x135c
  //
  DcgM2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, DCGM2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  DcgM2.Bits.pxfruldcgm = 1;
  DcgM2.Bits.pxftuldcgm = 1;
  DcgM2.Bits.pxpbuldcgm = 1;
  if (pInst->PrivateConfig.ClockDcgEnable) {
    DcgM2.Bits.pxpsuldcgm = 1;
  } else {
    DcgM2.Bits.pxpsuldcgm = 0;
  }
  DcgM2.Bits.pxpiuldcgm = 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, DCGM2_PCIE_MEM_RCRB_REG, DcgM2.Data, IpWrRegFlagSize32Bits);

  //
  // Program DCG Mode 3 register 0x1368
  //
  DcgM3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, DCGM3_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  DcgM3.Bits.pxttupdcgm    = 1;
  DcgM3.Bits.pxtoupdcgm    = 0;
  DcgM3.Bits.pxtrsupdcgm   = 1;
  DcgM3.Bits.pxtrupdcgm    = 1;
  DcgM3.Bits.pxliupdcgm    = 1;
  DcgM3.Bits.pxltupdcgm    = 1;
  DcgM3.Bits.pxlrupdcgm    = 1;
  DcgM3.Bits.pxsrussnrdcgm = 1;
  DcgM3.Bits.pxcupsnrdcgm  = 1;
  DcgM3.Bits.pxcupsnrdcgm  = 1;
  DcgM3.Bits.pxcupsrcdcgm  = 1;
  DcgM3.Bits.pxbupdcgm     = 1;
  DcgM3.Bits.pxeupdcgm     = 1;
   DcgM3.Bits.pxttsupdcgm  = 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, DCGM3_PCIE_MEM_RCRB_REG, DcgM3.Data, IpWrRegFlagSize32Bits);

  //
  // Program DCG Enable 1 register 0x1350
  //
  DcgEn1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, DCGEN1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  DcgEn1.Bits.pxttsuldcgen = 1;
  DcgEn1.Bits.pxkguldcgen = 1;
  DcgEn1.Bits.pxtrsuldcgen = 1;
  DcgEn1.Bits.pxtruldcgen = 1;
  DcgEn1.Bits.pxlsuldcgen = 1;
  DcgEn1.Bits.pxliuldcgen = 1;
  DcgEn1.Bits.pxlruldcgen = 1;
  DcgEn1.Bits.pxculdcgen = 1;
  DcgEn1.Bits.pxttuldcgen = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, DCGEN1_PCIE_MEM_RCRB_REG, DcgEn1.Data, IpWrRegFlagSize32Bits);

  //
  // Program DCG Enable 2 register 0x1354
  //
  DcgEn2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, DCGEN2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  DcgEn2.Bits.pxfruldcgen = 1;
  DcgEn2.Bits.pxftuldcgen = 1;
  DcgEn2.Bits.pxpbuldcgen = 1;
  DcgEn2.Bits.pxpsuldcgen = 1;
  DcgEn2.Bits.pxpiuldcgen = 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, DCGEN2_PCIE_MEM_RCRB_REG, DcgEn2.Data, IpWrRegFlagSize32Bits);

  //
  // Program DCG Enable 3 register 0x1360
  //
  DcgEn3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, DCGEN3_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  DcgEn3.Bits.pxttsupdcgen = 1;
  DcgEn3.Bits.pxttupdcgen = 1;
  DcgEn3.Bits.pxtoupdcgen = 1;
  DcgEn3.Bits.pxtrsupdcgen = 1;
  DcgEn3.Bits.pxtrupdcgen = 1;
  DcgEn3.Bits.pxliupdcgen = 1;
  DcgEn3.Bits.pxlrupdcgen = 1;
  DcgEn3.Bits.pxcupsnrdcgen = 0;
  DcgEn3.Bits.pxsrussnrdcgen = 1;
  DcgEn3.Bits.pxbupdcgen = 1;
  DcgEn3.Bits.pxeupdcgen = 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, DCGEN3_PCIE_MEM_RCRB_REG, DcgEn3.Data, IpWrRegFlagSize32Bits);

  //
  // IP Clock Control register 0x1370
  //
  IpClkCtr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, IPCLKCTR_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  IpClkCtr.Bits.mdpccen = 1;
  if (pInst->PcieRpCommonConfig.VisaClockGating) {
    IpClkCtr.Bits.pxdcge = 1;
  } else {
    IpClkCtr.Bits.pxdcge = 0;
  }
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, IPCLKCTR_PCIE_MEM_RCRB_REG, IpClkCtr.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Get max link width.

  @param[in]        pInst  *pInst
  @retval           Max link width, 0 when failed
**/
UINT8
SipGetMaxLinkWidth (
  IP_PCIE_INST      *pInst
  )
{
  LCAP_PCIE_CFG_STRUCT    Lcap;
  UINT8                   LinkWidth;

  /// Program Link Capabilities register 0x004C
  Lcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  LinkWidth = (UINT8) Lcap.Bits.mlw;
  if (LinkWidth != 0x3F) {
    return LinkWidth;
  } else {
    PRINT_ERROR ( "Incorrect Link Width = %d\n", LinkWidth);
    return 0;
  }
}

/**
  Get PCIe port number for enabled port.
  @param[in] pInst  *pInst

  @retval Root Port number (1 based)
**/
UINT32
SipGetPortNum (
   IP_PCIE_INST      *pInst
  )
{
  LCAP_PCIE_CFG_STRUCT    Lcap;

  /// Program Link Capabilities register 0x004C
  Lcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  return Lcap.Bits.pn;
}

/**
  Configure Pcie Phy Configuration.
  This function has to be executed before
  start of equalization.

  @param[in] pInst  *pInst
**/
void
SipConfigurePhyInit (
  IP_PCIE_INST      *pInst
  )
{
  LPHYCP1_PCIE_MEM_RCRB_STRUCT LphyCp1;
  LPHYCP4_PCIE_MEM_RCRB_STRUCT LphyCp4;
  CTRL2_PCIE_CFG_STRUCT     Ctrl2;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  /// Program Control 2 register 0x04A4
  Ctrl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CTRL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Ctrl2.Bits.pmetofd = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CTRL2_PCIE_CFG_REG, Ctrl2.Data, IpWrRegFlagSize16Bits);

  //
  // Program Log Phy Control Policy 1 register 0x1410
  //
  LphyCp1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, LPHYCP1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  LphyCp1.Bits.rxadpsvh = 0;
  LphyCp1.Bits.rxadprece = 0;
  LphyCp1.Bits.pipembip = 1;
  LphyCp1.Bits.rxeqfnevc = 0;
  LphyCp1.Bits.rxadphm = 0;
  if ((pInst->SipPhyVersion == IP_SC20) || (pInst->SipPhyVersion == IP_SC16)) {
    LphyCp1.Bits.rxadphm = 1;
    LphyCp1.Bits.rxeqfnevc = 1;
  }
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, LPHYCP1_PCIE_MEM_RCRB_REG, LphyCp1.Data, IpWrRegFlagSize32Bits);

  ///
  /// Program Log Phy Control Policy 4 register 0x1434
  /// LG Clock Un-Squelch Sampling Period [7:6] 0x1
  /// Target Link Speed Policy During Recovery Substates (TLSPDRS) 0x1434  [13] 0x1
  /// RxL0s Exit Gen 1/2 FTS Timeout Extension (RXL0SEG12FTSE) 0x1434 [25:24] 0x2
  /// Gen 1 Async RxElecidle Detector for RXL0s (G1AREDRL0S) 0x1434 [16] 0x1
  /// Gen 2 Async RxElecidle Detector for RXL0s (G2AREDRL0S) 0x1434 [17] 0x1
  /// Gen 4 Async RxElecidle Detector for RXL0s (G4AREDRL0S) 0x1434 [19] 0x1
  /// Gen 5 Async RxElecidle Detector for RXL0s (G5AREDRL0S) 0x1434 [20] 0x1
  /// RX Valid Trigger RXL0s Exit Suppression Period (RXVLDRXL0SSP) 0x1434 [28:26] 0x3 for SE32 and 0x1 for C16
  ///
  LphyCp4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, LPHYCP4_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  LphyCp4.Bits.dpmdndbfe = 1;
  LphyCp4.Bits.pllbusdrc = 1;
  LphyCp4.Bits.rxl0seg12ftse = V_LPHYCP4_RXL0SEG12FTSE;
  if (pInst->SipPhyVersion == IP_HSPHY) {
    LphyCp4.Bits.g1aredrl0s = 0;
    LphyCp4.Bits.g2aredrl0s = 0;
    LphyCp4.Bits.g3aredrl0s = 0;
    LphyCp4.Bits.g4aredrl0s = 0;
    LphyCp4.Bits.g5aredrl0s = 0;
  } else {
    LphyCp4.Bits.g1aredrl0s = 1;
    LphyCp4.Bits.g2aredrl0s = 1;
    LphyCp4.Bits.g3aredrl0s = 1;
    LphyCp4.Bits.g4aredrl0s = 1;
    LphyCp4.Bits.g5aredrl0s = 1;
  }
  LphyCp4.Bits.tlspdrs    = 1;
  LphyCp4.Bits.lgussp     = 1;
  LphyCp4.Bits.oscclksqexitdbtimers = 2;
  if ((pInst->SipPhyVersion == IP_SC20) || (pInst->SipPhyVersion == IP_SC16)) {
    LphyCp4.Bits.rxvldrxl0ssp = 1;
  } else {
    LphyCp4.Bits.rxvldrxl0ssp = 3;
  }
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, LPHYCP4_PCIE_MEM_RCRB_REG, LphyCp4.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Get max PCIe link speed supported by the root port.

  @param[in]  pInst  *pInst

  @retval Max link speed
**/
UINT32 SipGetMaxLinkSpeed (
  IP_PCIE_INST      *pInst
  )
{
  LCAP_PCIE_CFG_STRUCT        Lcap;

  /// Program Link Capabilities register 0x004C
  Lcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri,  LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  return Lcap.Bits.mls;
}

/**
  Determines whether PCIe link is active

  @param[in] pInst  *pInst

  @retval Link Active state
**/
BOOLEAN
SipIsLinkActive (
  IP_PCIE_INST      *pInst
  )
{
  LSTS_PCIE_CFG_STRUCT        Lsts;

  /// Link Status register 0x0052
  Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  return !! (Lsts.Bits.la);
}

/**
  Disable PCIe root port in controller.

  @param[in] pInst                *pInst
**/
void
SipDisableRootPortInController (
  IP_PCIE_INST       *pInst
  )
{
  PCD_PCIE_PRVT_STRUCT    Pcd;
  IP_PCIE_INST            *pInstController;

  if (pInst->PrivateConfig.RootPortIndexInController >= 4) {
    return;
  }

  pInstController = (IP_PCIE_INST *)(UINTN) pInst->ControllerInst;
  if (pInstController == NULL) {
    PRINT_ERROR ( "Invalid pInstController for RpIndex = %d\n", pInst->RpIndex);
    return;
  }

  //
  // PCIe RP IOSF Sideband register offset 0x00[19:16], depending on the port that is Function Disabled
  // Access it by offset 0x02[4:0] to avoid RWO bit
  // Program Port Configuration And Disable register 0x3E00
  //
  Pcd.Data = 0;
  Pcd.Data8[2]    = (UINT8) IpWrRegRead (pInstController->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG + 2, IpWrRegFlagSize8Bits);
  switch (pInst->PrivateConfig.RootPortIndexInController) {
    case 0:
      Pcd.Bits.p0d = 1;
      break;
    case 1:
      Pcd.Bits.p1d = 1;
      break;
    case 2:
      Pcd.Bits.p2d = 1;
      break;
    case 3:
      Pcd.Bits.p3d = 1;
      break;
    default:
      PRINT_ERROR ( "Invalid RootPortIndexInController\n");
      break;
  }
  IpWrRegWrite (pInstController->RegCntxt_Prvt_Cfg, PCD_PCIE_PRVT_REG + 2, Pcd.Data8[2], IpWrRegFlagSize8Bits);
}

/**
  This function creates SIP17 Capability and Extended Capability List

  @param[in]  IP_PCIE_INST       *pInst
**/
void
SipInitCapabilityList (
  IP_PCIE_INST  *pInst
  )
{
  UINT16                        NextCap;
  UINT32                        SipMaxLinkSpeed;
  LCAP_PCIE_CFG_STRUCT          Lcap;
  PMCS_PCIE_CFG_STRUCT          Pmcs;
  PTMCAPR_PCIE_CFG_STRUCT       PtmCapr;
  CAPP_PCIE_CFG_STRUCT          Capp;
  CLIST_PCIE_CFG_STRUCT         Clist;
  SVCAP_PCIE_CFG_STRUCT         SvCap;
  PL16MECH_PCIE_CFG_STRUCT      Pl16Mech;
  PL16GECH_PCIE_CFG_STRUCT      Pl16Gech;
  DLFECH_PCIE_CFG_STRUCT        DlFech;
  SPEECH_PCIE_CFG_STRUCT        Speech;
  PTMECH_PCIE_CFG_STRUCT        PtmEch;
  L1SECH_PCIE_CFG_STRUCT        L1Sech;
  L1SCAP_PCIE_CFG_STRUCT        L1Scap;
  L1SCTL1_PCIE_CFG_STRUCT       L1Sctl1;
  ACSECH_PCIE_CFG_STRUCT        AcsEch;
  ACSCAPR_PCIE_CFG_STRUCT       AcsCapr;
  AECH_PCIE_CFG_STRUCT          Aech;
  VCCH_PCIE_CFG_STRUCT          Vcch;
  G5ECH_PCIE_CFG_STRUCT         G5ech;
  PVCCR1_PCIE_CFG_STRUCT        Pvccr1;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  ///
  /// Build Capability linked list
  /// 1.  Read and write back to capability registers 34h, 41h, 81h and 91h using byte access.
  /// 2.  Program NSR, A4h[3] = 0b
  /// Program Capabilities List Pointer register 0x0034
  ///
  Capp.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CAPP_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
  PRINT_LEVEL1 ("Capp.Data = 0x%x\n", Capp.Data);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CAPP_PCIE_CFG_REG, Capp.Data, IpWrRegFlagSize8Bits);

  /// Program Capabilities List register 0x0040
  Clist.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CLIST_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CLIST_PCIE_CFG_REG, Clist.Data, IpWrRegFlagSize16Bits);

  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MID_PCIE_CFG_REG, 0x9805, IpWrRegFlagSize16Bits);

  /// Program Subsystem Vendor Capability register 0x0098
  SvCap.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SVCAP_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, SVCAP_PCIE_CFG_REG, SvCap.Data, IpWrRegFlagSize16Bits);

  /// Program PCI Power Management Control register 0x00A4
  Pmcs.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PMCS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Pmcs.Bits.nsr = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PMCS_PCIE_CFG_REG, Pmcs.Data, IpWrRegFlagSize32Bits);

  ///
  /// II.Build PCI Extended Capability linked list
  /// 0xEDC - PL16MECH(CAPID:0027h) Physical Layer 16.0 GT/s Margining Extended Capability Header
  /// 0xA9C - PL16GECH(CAPID:0026h) Physical Layer 16.0 GT/s Extended Capability Header
  /// 0xA90 - DLFECH  (CAPID:0025h) Data Link Feature Extended Capability Header
  /// 0xA30 - SPEECH  (CAPID:0019h) Secondary PCI Express Extended Capability Header
  /// 0xA00 - DPCECH  (CAPID:001Dh) DPC Extended Capability Header
  /// 0x280 - VCECH   (CAPID:0002h) VC capability
  /// 0x150 - PTMECH  (CAPID:001Fh) PTM Extended Capability Register
  /// 0x200 - L1SSECH (CAPID:001Eh) L1SS Capability register
  /// 0x220 - ACSECH  (CAPID:000Dh) ACS Capability register
  /// 0x100 - AECH    (CAPID:0001h) Advanced Error Reporting Capability
  ///
  /*
  a. NEXT_CAP = 0h
  */
  NextCap     = V_EXCAP_NCO_LISTEND;

  if ((pInst->SipPhyVersion == IP_HSPHY) || (pInst->SipPhyVersion == IP_SE32)) {
    /*
    b. Program [0xADC] Physical Layer 32.0 GT/s Extended Capability Header(G5ECH)
      1. Set Next Capability Offset, 0xADC[31:20] = NEXT_CAP
      2. Set Capability Version, 0xADC[19:16] = 1h
      3. Set Capability ID, 0xADC[15:0] = 002Ah
      4. NEXT_CAP = 0xADC
    */
    G5ech.Data      = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, G5ECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    G5ech.Bits.cv   = 1;
    G5ech.Bits.ecid =  V_EX_G5ECH_CID;
    G5ech.Bits.nco  = NextCap;
    NextCap = (UINT16) G5ECH_PCIE_CFG_REG;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, G5ECH_PCIE_CFG_REG, G5ech.Data, IpWrRegFlagSize32Bits);
    PRINT_LEVEL1 ("Physical Layer 32.0 GT/s Extended Capability Header\n");
    PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_G5ECH_CID);
  }

  /// Program Link Capabilities register 0x004C
  Lcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  SipMaxLinkSpeed = Lcap.Bits.mls;

  if (SipMaxLinkSpeed >= IpPcieGen4) {
    /*
    b. Program [0xEDC] Physical Layer 16.0 GT/s Margining Extended Capability Header(PL16MECH)
      1. Set Next Capability Offset, 0xEDC[31:20] = NEXT_CAP
      2. Set Capability Version, 0xEDC[19:16] = 1h
      3. Set Capability ID, 0xEDC[15:0] = 0027h
      4. NEXT_CAP = 0xEDC
    */
    Pl16Mech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PL16MECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Pl16Mech.Bits.cv      = 1;
    Pl16Mech.Bits.pciecid =  V_EX_PL16MECH_CID;
    Pl16Mech.Bits.nco     = NextCap;
    NextCap = (UINT16) PL16MECH_PCIE_CFG_REG;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PL16MECH_PCIE_CFG_REG, Pl16Mech.Data, IpWrRegFlagSize32Bits);
    PRINT_LEVEL1 ("Physical Layer 16.0 GT/s Margining Extended Capability Header\n");
    PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_PL16MECH_CID);

    /*
    c. Program [0xA9C] Physical Layer 16.0 GT/s Extended Capability Header(PL16GECH)
      1. Set Next Capability Offset, 0xA9C[31:20] = NEXT_CAP
      2. Set Capability Version, 0xA9C[19:16] = 1h
      3. Set Capability ID, 0xA9C[15:0] = 0026h
      4. NEXT_CAP = 0xA9C
    */
    Pl16Gech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PL16GECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Pl16Gech.Bits.cv      = 1;
    Pl16Gech.Bits.pciecid = V_EX_PL16GECH_CID;
    Pl16Gech.Bits.nco     = NextCap;
    NextCap = (UINT16) PL16GECH_PCIE_CFG_REG;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PL16GECH_PCIE_CFG_REG, Pl16Gech.Data, IpWrRegFlagSize32Bits);
    PRINT_LEVEL1 ("Physical Layer 16.0 GT/s Extended Capability Header\n");
    PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_PL16GECH_CID);
  } else {
    Pl16Mech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PL16MECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PL16MECH_PCIE_CFG_REG, Pl16Mech.Data, IpWrRegFlagSize32Bits);
    Pl16Gech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PL16GECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PL16GECH_PCIE_CFG_REG, Pl16Gech.Data, IpWrRegFlagSize32Bits);
  }
  /*
  d. Program [0xA90] Data Link Feature Extended Capability Header(DLFECH)
    1. Set Next Capability Offset, 0xA90[31:20] = NEXT_CAP
    2. Set Capability Version, 0xA90[19:16] = 1h
    3. Set Capability ID, 0xA90[15:0] = 0025h
    4.  NEXT_CAP = 0xA90
  */
  DlFech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DLFECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  DlFech.Bits.cv      = 1;
  DlFech.Bits.pciecid = V_EX_DLFECH_CID;
  DlFech.Bits.nco     = NextCap;
  NextCap = (UINT16) DLFECH_PCIE_CFG_REG;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DLFECH_PCIE_CFG_REG, DlFech.Data, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("Data Link Feature Extended Capability Header\n\n");
  PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_DLFECH_CID);

   /*
  e. If the RP is GEN3 capable (by fuse and BIOS policy), enable Secondary PCI Express Extended Capability [0XA30]
    1. Set Next Capability Offset,0xA30[31:20] = NEXT_CAP
    2. Set Capability Version, 0xA30[19:16] = 1h
    3. Set Capability ID,  0xA30[15:0] = 0019h
    4. NEXT_CAP = 0xA30
    ELSE, set 0xA30[31:0] = 0
  */
  Speech.Data       = 0;
  if (SipMaxLinkSpeed >= V_LCAP_MLS_GEN3) {
    Speech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SPEECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Speech.Bits.cv       = 1;
    Speech.Bits.pcieecid = V_EX_SPE_CID;
    Speech.Bits.nco      = NextCap;
    PRINT_LEVEL1 ("Secondary PCI Express Extended Capability\n");
    PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_SPE_CID);
    NextCap = (UINT16) SPEECH_PCIE_CFG_REG;
  } else {
    Speech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SPEECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, SPEECH_PCIE_CFG_REG, Speech.Data, IpWrRegFlagSize32Bits);


  /*
  h. If Virtual Channel capability is enabled, then
      1. Set Next Capability Offset, 0xA00[31:20] = NEXT_CAP
      2. Set Capability Version, 0xA00[19:16] = 1h
      3. Set Capability ID, 0xA00[15:0] = 002h
      4. NEXT_CAP = 0xA00
     ELSE, set 0xA00 [31:0] = 0
  */
  Vcch.Data = 0;
  if (pInst->PrivateConfig.MultiVcEnabled) {
    Vcch.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, VCCH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Vcch.Bits.cv  = 1;
    Vcch.Bits.cid = V_EX_VCCH_CID;
    Vcch.Bits.nco = (UINT16) NextCap;
    PRINT_LEVEL1 ("Virtual Channel Extended Capability Header\n");
    PRINT_LEVEL1 ( "NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_VCCH_CID);
    NextCap = (UINT16) VCCH_PCIE_CFG_REG;
    Pvccr1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PVCCR1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Pvccr1.Bits.evcc = pInst->PrivateConfig.NumOfVcs;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PVCCR1_PCIE_CFG_REG, Pvccr1.Data, IpWrRegFlagSize32Bits);
  } else {
    Vcch.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, VCCH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, VCCH_PCIE_CFG_REG, Vcch.Data, IpWrRegFlagSize32Bits);

  /*
  g. If support PTM
    1. Set Next Capability Offset, Dxx:Fn +150h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +150h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +150h[15:0] = 001Fh
    4. Read and write back to Dxx:Fn +144h
    5. NEXT_CAP = 150h
    ELSE, set Dxx:Fn +150h [31:0] = 0
    In both cases: read Dxx:Fn + 154h, set BIT1 and BIT2 then write it back
    Program PTM Extended Capability Header register 0x0150
  */
  PtmEch.Data = 0;
  if (pInst->PcieRpCommonConfig.PtmEnabled == TRUE) {
    PtmEch.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PTMECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PtmEch.Bits.cv  = 1;
    PtmEch.Bits.cid = V_EX_PTM_CID;
    PtmEch.Bits.nco = (UINT16) NextCap;
    PRINT_LEVEL1 ("PTM\n");
    PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_PTM_CID);
    NextCap = PTMECH_PCIE_CFG_REG;
  } else {
    PtmEch.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PTMECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PTMECH_PCIE_CFG_REG, PtmEch.Data, IpWrRegFlagSize32Bits);

  // Program PTM Capability Register register 0x0154
  PtmCapr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PTMCAPR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PtmCapr.Bits.ptmrc   = 1;
  PtmCapr.Bits.ptmrspc = 1;
  PtmCapr.Bits.ptmpdac = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PTMCAPR_PCIE_CFG_REG, PtmCapr.Data, IpWrRegFlagSize32Bits);

  /*
  h. If support L1 Sub-State
    1. Set Next Capability Offset, Dxx:Fn +200h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +200h[19:16] = 2h
    3. Set Capability ID, Dxx:Fn +200h[15:0] = 001Eh
    4. Read and write back to Dxx:Fn +204h
    5. Refer to section 8.3 for other requirements (Not implemented here)
    6. NEXT_CAP = 200h
    ELSE, set Dxx:Fn +200h [31:0] = 0, and read and write back to Dxx:Fn +204h
    Program L1 Sub-States Extended Capability Header register 0x0200
  */
  L1Sech.Data = 0;
  if (pInst->PrivateConfig.ClkReqEnable &&
    (pInst->PcieRpCommonConfig.L1Substates != IpPchPcieL1SubstatesDisabled)) {
    L1Sech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, L1SECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    L1Sech.Bits.cv     = 0x2;
    L1Sech.Bits.pcieec = V_EX_L1S_CID;
    L1Sech.Bits.nco    = NextCap;
    PRINT_LEVEL1 ("L1SS\n");
    PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_L1S_CID);
    NextCap = (UINT16) L1SECH_PCIE_CFG_REG;
  } else {
    L1Sech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, L1SECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, L1SECH_PCIE_CFG_REG, L1Sech.Data, IpWrRegFlagSize32Bits);

  // Program L1 Sub-States Capabilities register 0x0204
  L1Scap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, L1SCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  //
  // L1PM Substates Supported (L1PSS) 0x204[4]
  // ASPM L1.1 Supported      (AL11S) 0x204[3]
  // ASPM L1.2 Supported      (AL12S) 0x204[2]
  // PCI-PM L1.1 Supported   (PPL11S) 0x204[1]
  // PCI-PM L1.2 Supported   (PPL12S) 0x204[0]
  //
  if (pInst->PcieRpCommonConfig.L1Substates == IpPchPcieL1SubstatesDisabled) {
    L1Scap.Bits.l1pss  = 0;
    L1Scap.Bits.al11s  = 0;
    L1Scap.Bits.al12s  = 0;
    L1Scap.Bits.ppl11s = 0;
    L1Scap.Bits.ppl12s = 0;
  } else if (pInst->PcieRpCommonConfig.L1Substates == IpPchPcieL1SubstatesL1_1) {
    L1Scap.Bits.al12s  = 0;
    L1Scap.Bits.ppl12s = 0;
    L1Scap.Bits.l1pss  = 1;
    L1Scap.Bits.al11s  = 1;
    L1Scap.Bits.ppl11s = 1;
  } else if (pInst->PcieRpCommonConfig.L1Substates == IpPchPcieL1SubstatesL1_1_2) {
    L1Scap.Bits.l1pss  = 1;
    L1Scap.Bits.al11s  = 1;
    L1Scap.Bits.al12s  = 1;
    L1Scap.Bits.ppl11s = 1;
    L1Scap.Bits.ppl12s = 1;
  }
  L1Scap.Bits.ptv    = pInst->PrivateConfig.TPowerOn;
  L1Scap.Bits.ptpos  = V_EX_L1SCAP_PTPOS_10us;
  L1Scap.Bits.l1sses = 1;
  L1Scap.Bits.pcmrt  = 0x3C;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, L1SCAP_PCIE_CFG_REG, L1Scap.Data, IpWrRegFlagSize32Bits);

  // Program L1 Sub-States Control 1 register 0x0208
  L1Sctl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, L1SCTL1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  L1Sctl1.Bits.l1sseie = 1;
  L1Sctl1.Bits.cmrt    = V_EX_L1SCTL1_CMRT;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, L1SCTL1_PCIE_CFG_REG, L1Sctl1.Data, IpWrRegFlagSize32Bits);

  /*
  i. If support ACS
    1. Set Next Capability Offset, Dxx:Fn +220h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +220h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +220h[15:0] = 000Dh
    4. Read and write back to Dxx:Fn +224h
    5. NEXT_CAP = 220h
    ELSE, set Dxx:Fn +220h [31:0] = 0, and read and write back to Dxx:Fn +224h
    Program ACS Extended Capability Header register 0x0220
  */
  AcsEch.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, ACSECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (pInst->PcieRpCommonConfig.AcsEnabled == TRUE) {
    AcsEch.Bits.cv  = 1;
    AcsEch.Bits.cid = V_EX_ACS_CID;
    AcsEch.Bits.nco = NextCap;
    PRINT_LEVEL1 ("ACSECH\n");
    PRINT_LEVEL1 ("NextCap = %x \t Capability ID = %x\n", NextCap, V_EX_ACS_CID);
    NextCap = (UINT16) ACSECH_PCIE_CFG_REG;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, ACSECH_PCIE_CFG_REG, AcsEch.Data, IpWrRegFlagSize32Bits);

  // Program ACS Capability Register register 0x0224
  AcsCapr.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, ACSCAPR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, ACSCAPR_PCIE_CFG_REG, AcsCapr.Data, IpWrRegFlagSize32Bits);

  /*
  j. If support Advanced Error Reporting
    1. Set Next Capability Offset, Dxx:Fn +100h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +100h[19:16] = 1h
    3. Set Capability ID, Dxx:Fn +100h[15:0] = 0001h
    ELSE
    1. Set Next Capability Offset, Dxx:Fn +100h[31:20] = NEXT_CAP
    2. Set Capability Version, Dxx:Fn +100h[19:16]  = 0h
    3. Set Capability ID, Dxx:Fn +100h[15:10]  = 0000h
    Program Advanced Error Extended register 0x0100
  */
  Aech.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, AECH_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (pInst->PcieRpCommonConfig.AdvancedErrorReporting) {
    Aech.Bits.cv  = 1;
    Aech.Bits.cid = V_EX_AEC_CID;
  }
  Aech.Bits.nco = NextCap;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, AECH_PCIE_CFG_REG, Aech.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Checks if given rootport has an endpoint connected

  @param[in]  IP_PCIE_INST       *pInst

  @retval TRUE   if endpoint is connected
  @retval FALSE  if no endpoint was detected
**/
BOOLEAN
SipIsEndpointConnected (
  IP_PCIE_INST  *pInst
  )
{
  SLSTS_PCIE_CFG_STRUCT     Slsts;

  /// Program Slot Status register 0x005A
  Slsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SLSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  return !!(Slsts.Bits.pds);
}

/**
  Program controller power management settings.
  This settings are relevant to all ports including disabled ports.
  All registers are located in the first port of the controller.
  Use sideband access since primary may not be available.

  @param[in]  pInst             *pInst
**/
void
SipConfigureControllerBasePowerManagement (
   IP_PCIE_INST         *pInst
  )
{
  PCE_PCIE_CFG_STRUCT           Pce;
  CCFG_PCIE_CFG_STRUCT          Ccfg;
  HWSNR_PCIE_CFG_STRUCT         Hwsnr;
  ACRG3_PCIE_CFG_STRUCT         Acrg3;
  PWRCTL_PCIE_CFG_STRUCT        PwrCtl;
  PGCTRL_PCIE_CFG_STRUCT        PgCtrl;
  RPPGEN_PCIE_CFG_STRUCT        RppGen;
  RPDCGEN_PCIE_CFG_STRUCT       RpdCgen;
  AECR1G3_PCIE_CFG_STRUCT       Aecr1g3;
  ADVMCTRL_PCIE_CFG_STRUCT      AdvmCtrl;
  PHYCTL2_PCIE_CFG_STRUCT       PhyCtl2;
  IOSFSBCS_PCIE_CFG_STRUCT      IosfSbcs;
  PCIEPMECTL2_PCIE_CFG_STRUCT   PciePmeCtl2;
  PCIEPMECTL3_PCIE_CFG_STRUCT   PciePmeCtl3;

  PRINT_LEVEL1 ("%s(%d) Start \n", __FUNCTION__, pInst->RpIndex);

  if (pInst->PcieRpCommonConfig.ClockGating) {
    PRINT_LEVEL1 ("Clock gating is enabled!\n");

    // Program Channel Configuration register 0x00D0
    Ccfg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, CCFG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Ccfg.Bits.dcgeisma = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, CCFG_PCIE_CFG_REG, Ccfg.Data, IpWrRegFlagSize32Bits);
    //
    // Program Root Port Power Gating Enable register 0x00E2
    // Program Root Port Dynamic Clock Gate Enable register 0x00E1
    // Clock gating programming for the PCIE controller
    // Set E1h[5] to 1b to enable PCIe Link clock trunk gating.
    // Set E1h[4] to 1b to enable PCIe backbone clock trunk gating.
    // Set E1h[2] to 1b to enable shared resource dynamic backbone clock gating.
    // Set E1h[6] to 1b to enable Partition/Trunk Oscillator clock gating; if all ports on the controller support CLKREQ#.
    // Set E2h[4] to 1b to enable side clock gating.
    //
    RppGen.Data  = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, RPPGEN_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    RpdCgen.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, RPDCGEN_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    RpdCgen.Bits.lclkreqen  = 1;
    RpdCgen.Bits.bbclkreqen = 1;
    RpdCgen.Bits.srdbcgen   = 1;
    RpdCgen.Bits.ptocge     = 1;
    RppGen.Bits.seoscge     = 1;
    PRINT_LEVEL1 ("Setting PTOCGE\n");
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, RPPGEN_PCIE_CFG_REG, RppGen.Data, IpWrRegFlagSize8Bits);
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, RPDCGEN_PCIE_CFG_REG, RpdCgen.Data, IpWrRegFlagSize8Bits);
  }

  Aecr1g3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, AECR1G3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    if ((pInst->SipPhyVersion == IP_HSPHY) || (pInst->SipPhyVersion == IP_SE32)) {
      Aecr1g3.Bits.reqtmbof = 1;
    }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, AECR1G3_PCIE_CFG_REG, Aecr1g3.Data, IpWrRegFlagSize32Bits);

  if (pInst->PcieRpCommonConfig.PowerGating) {
    //
    // Program Advance Extended Control Register 1 Group 3 register 0x0C80
    // Enable Dynamic and Trunk Clock Gating Coupling Mode
    //
    Aecr1g3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, AECR1G3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Aecr1g3.Bits.dtcgcm = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, AECR1G3_PCIE_CFG_REG, Aecr1g3.Data, IpWrRegFlagSize32Bits);
    ///
    /// Progarm (R_PCIE_HWSNR) 5F0h
    /// Set 5F0h[3:0] to 0101b for Bank Enable Pulse Width.
    /// Set 5F0h[7:4] to 01b for Restore Enable Pulse Width. 01b = 2 clocks.
    /// Set 5F0h[9:8] to 10b for Entry and Exit hysteresis. 10b = 16clocks.
    ///
    Hwsnr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, HWSNR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Hwsnr.Bits.bepw = V_HWSNR_BEPW;
    Hwsnr.Bits.repw = V_HWSNR_REPW_2CLK;
    Hwsnr.Bits.eeh  = V_HWSNR_EEH_16CLK;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, HWSNR_PCIE_CFG_REG, Hwsnr.Data, IpWrRegFlagSize32Bits);

    ///
    /// Program (R_PCIE_PGCTRL) 5F4h
    /// Set 5F4h[2:0] to 101b for PM_REQ Block Response Time.
    /// 010b = 10 micro seconds for 1LM & 101b = 25 micro seconds for 2LM(PEG60).
    ///
    PgCtrl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PGCTRL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);

    PgCtrl.Bits.pmreqblkrspt = pInst->PrivateConfig.BlockChainTimer;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PGCTRL_PCIE_CFG_REG, PgCtrl.Data, IpWrRegFlagSize32Bits);

    //
    // Power gating programming for the PCIE controller
    //
    //
    // Program PCIe PM Extension Control 3 register 0x0434
    // Set PM Request Controller Power Gating Exit Hysteresis to 0us
    //
      PciePmeCtl3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      PciePmeCtl3.Bits.pmreqcpgexh = 0;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL3_PCIE_CFG_REG, PciePmeCtl3.Data, IpWrRegFlagSize32Bits);

    ///
    /// Program PCIe PM Extension Control 2 register 0x0424
    /// Set Controller Power Gating Exit Hysteresis to 0us.
    /// Set Controller Power Gating Entry Hysteresis to 0us.
    ///
      PciePmeCtl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      PciePmeCtl2.Bits.cpgexh = 0;
      PciePmeCtl2.Bits.cpgenh = 0;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL2_PCIE_CFG_REG, PciePmeCtl2.Data, IpWrRegFlagSize32Bits);

    ///
    /// Program (R_PCIE_CFG_ACRG3) 6CCh
    /// Set 6CCh[21] to 1 to enable Chain Bit Generation Mode
    ///
    Acrg3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, ACRG3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Acrg3.Bits.cbgm        = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, ACRG3_PCIE_CFG_REG, Acrg3.Data, IpWrRegFlagSize32Bits);

      ///
      /// Set E8h[17,15] to [1,1]
      /// Program Power Control register 0x00E8
      ///
      PwrCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PWRCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      PwrCtl.Bits.dbupi    = 1;
      PwrCtl.Bits.wpdmpgep = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PWRCTL_PCIE_CFG_REG, PwrCtl.Data, IpWrRegFlagSize32Bits);

    ///
    /// Set F5h[1:0] to 11b  (R_PCIE_CFG_PHYCTL2)
    /// Set F7h[3:2] = 00b   (R_PCIE_CFG_IOSFSBCS)
    ///
    IosfSbcs.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, IOSFSBCS_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    PhyCtl2.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PHYCTL2_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    PhyCtl2.Bits.pxpg2plloffen = 1;
    PhyCtl2.Bits.pxpg3plloffen = 1;
    IosfSbcs.Bits.siid         = 0;

    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, IOSFSBCS_PCIE_CFG_REG, IosfSbcs.Data, IpWrRegFlagSize8Bits);
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PHYCTL2_PCIE_CFG_REG, PhyCtl2.Data, IpWrRegFlagSize8Bits);

    ///
    /// Program PCIe PM Extension Control 2 register 0x0424
    /// Set 424h[11] to 1b
    ///
    PciePmeCtl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PciePmeCtl2.Bits.phyclpge = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL2_PCIE_CFG_REG, PciePmeCtl2.Data, IpWrRegFlagSize32Bits);
  }
  ///
  ///
  /// Set Hardware Autonomous Enable 428h[5] to 1b
  /// Set PMC Request Enable 428h[0] to 0b
  /// Set Sleep Enable to 1b for SIP version 17
  /// According to chasis requirement, HAE and PMCRE are mutual exclusive. Meaning BIOS must not program HAE = 1 if the PMCRE =1 and vice versa
  ///
  Pce.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCE_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
  if (pInst->IsController) {
    if (pInst->PcieRpCommonConfig.AutoPowerGating ) {
      if (pInst->PrivateConfig.VnnRemovalSupported) {
      Pce.Bits.pmcre = 0;
      Pce.Bits.hae   = 1;
      }else {
        Pce.Bits.pmcre = 1;
        Pce.Bits.hae   = 0;
      }
    } else {
      Pce.Bits.pmcre = 0;
      Pce.Bits.hae   = 0;
    }
  }

  Pce.Bits.se = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCE_PCIE_CFG_REG, Pce.Data, IpWrRegFlagSize8Bits);

    //
    // Program PCIe PM Extension Control 3 register 0x0434
    // Set L1.PG Auto Power Gate Enable 434h[4] to 0.
    //
    PciePmeCtl3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PciePmeCtl3.Bits.l1pgautopgen = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL3_PCIE_CFG_REG, PciePmeCtl3.Data, IpWrRegFlagSize32Bits);

    //
    // Set PM_REQ Block and Power Gate Response Time to 10us
    //
    AdvmCtrl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, ADVMCTRL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    AdvmCtrl.Bits.pmreqcwc       = V_ADVMCTRL_PMREQCWC_LNK_PRIM;
    AdvmCtrl.Bits.pmreqblkpgrspt = V_ADVMCTRL_PMREQBLKPGRSPT_10US;
    AdvmCtrl.Bits.ccbe       = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, ADVMCTRL_PCIE_CFG_REG, AdvmCtrl.Data, IpWrRegFlagSize32Bits);

    // Program Hardware Save And Restore register 0x05F0
    Hwsnr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, HWSNR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Hwsnr.Bits.eeh  = V_HWSNR_EEH_16CLK;
    Hwsnr.Bits.repw = V_HWSNR_REPW_2CLK;
    Hwsnr.Bits.bepw = V_HWSNR_BEPW;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, HWSNR_PCIE_CFG_REG, Hwsnr.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Configure ASPM settings which are applicable to both enabled and disabled ports.
  These settings are relevant to all ports including disabled ports.
  Use sideband access since primary may not be available.

  @param[in] pInst                *pInst
**/
void
SipConfigureASPM (
  IP_PCIE_INST        *pInst
  )
{
  PCIENFTS_PCIE_CFG_STRUCT        PcieNfts;
  G3L0SCTL_PCIE_CFG_STRUCT        G3L0sCtl;
  G4L0SCTL_PCIE_CFG_STRUCT        G4L0sCtl;
  G5L0SCTL_PCIE_MEM_RCRB_STRUCT   G5L0sCtl;
  LLPC_PCIE_MEM_RCRB_STRUCT       Llpc;

  PRINT_LEVEL1 ("%s(%d) Start \n", __FUNCTION__, pInst->RpIndex);

  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Configure PCI Express Number of Fast Training Sequence for each port
  /// 1.  Set Dxx:Fn + 314h [31:24, 23:16, 15:8, 7:0] to [7Eh, 70h, 3Fh, 38h]
  ///     Set Dxx:Fn + 314h [31:24, 23:16, 15:8, 7:0] to [7Eh, C8h, 3Fh, BCh] (SipVersion > SIP17)
  /// 2.  Set Dxx:Fn + 478h [31:24, 15:8, 7:0] to [28h, 3Dh, 2Ch]
  ///     Set Dxx:Fn + 478h [31:24, 15:8, 7:0] to [28h, 40h, 4Bh] (SipVersion > SIP17)
  ///
  PcieNfts.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIENFTS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (pInst->SipPhyVersion == IP_HSPHY) {
    PcieNfts.Bits.g1ccnfts = 0x98;
    PcieNfts.Bits.g1ucnfts = 0x98;
    PcieNfts.Bits.g2ccnfts = 0xCC;
    PcieNfts.Bits.g2ucnfts = 0xCC;
  } else {
    PcieNfts.Bits.g1ccnfts = 0xDC;
    PcieNfts.Bits.g1ucnfts = 0xDC;
    PcieNfts.Bits.g2ccnfts = 0xC8;
    PcieNfts.Bits.g2ucnfts = 0x7E;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIENFTS_PCIE_CFG_REG, PcieNfts.Data, IpWrRegFlagSize32Bits);

  G3L0sCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, G3L0SCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (pInst->SipPhyVersion == IP_HSPHY) {
    G3L0sCtl.Bits.g3ccnfts  = 0x4E;
    G3L0sCtl.Bits.g3ucnfts  = 0x4E;
  } else {
    G3L0sCtl.Bits.g3ccnfts  = 0x4B;
    G3L0sCtl.Bits.g3ucnfts  = 0x4B;
  }
  G3L0sCtl.Bits.g3asl0spl = 0x28;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, G3L0SCTL_PCIE_CFG_REG, G3L0sCtl.Data, IpWrRegFlagSize32Bits);

  ///
  /// Set Dxx.Fn + 310h [31:24, 23:22, 15:8, 7:0] to [0x28, 0x3, 0x80, 0x5B] (SIP17)
  /// Set Dxx.Fn + 310h [31:24, 23:22, 15:8, 7:0] to [0x28, 0x3, 0x80, 0x5E] (SipVersion > SIP17)
  ///
  G4L0sCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, G4L0SCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (pInst->SipPhyVersion == IP_HSPHY) {
    G4L0sCtl.Bits.g4ccnfts  = 0x81;
    G4L0sCtl.Bits.g4ucnfts  = 0x81;
  } else {
    G4L0sCtl.Bits.g4ccnfts  = 0x5E;
    G4L0sCtl.Bits.g4ucnfts  = 0x80;
  }
  G4L0sCtl.Bits.g4l0sic   = 0x3;
  G4L0sCtl.Bits.g4asl0spl = 0x28;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, G4L0SCTL_PCIE_CFG_REG, G4L0sCtl.Data, IpWrRegFlagSize32Bits);
  ///
  /// Set Dxx.Fn + 1E00h [15:8, 7:0] to [0xE6, 0xBC]
  ///
  if ((pInst->SipPhyVersion == IP_HSPHY) || (pInst->SipPhyVersion == IP_SE32)) {
    G5L0sCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, G5L0SCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
    if (pInst->SipPhyVersion == IP_HSPHY) {
      G5L0sCtl.Bits.g5ucnfts = 0xFF;
      G5L0sCtl.Bits.g5ccnfts = 0xE6;
    } else {
      G5L0sCtl.Bits.g5ucnfts = 0xE6;
      G5L0sCtl.Bits.g5ccnfts = 0xBC;
    }
    G5L0sCtl.Bits.g5asl0spl = 0x50;
    IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, G5L0SCTL_PCIE_MEM_RCRB_REG, G5L0sCtl.Data, IpWrRegFlagSize32Bits);
  }

  ///
  /// Set Dxx.Fn + 132Ch [6:6] to 1b (SipVersion > SIP17)
  /// for all Gen4/Gen5 capable controllers
  ///
  Llpc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, LLPC_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Llpc.Bits.al1ntp = 0x1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, LLPC_PCIE_MEM_RCRB_REG, Llpc.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Configure power management settings whcih are applicable to both enabled and disabled ports.
  This settings are relevant to all ports including disabled ports.
  Use sideband access since primary may not be available.

  @param[in]  pInst               *pInst
**/
void
SipConfigurePortBasePowerManagement (
   IP_PCIE_INST        *pInst
  )
{
  FCTL2_PCIE_MEM_RCRB_STRUCT  Fctl2;
  RPDCGEN_PCIE_CFG_STRUCT     RpdCgen;
  IOSFSBCS_PCIE_CFG_STRUCT    IosfSbCs;
  ADVMCTRL_PCIE_CFG_STRUCT    AdvmCtrl;
  PGTHRES_PCIE_CFG_STRUCT     PgThres;
  VNNREMCTL_PCIE_CFG_STRUCT   VnnRemCtl;
  PCIEPMECTL_PCIE_CFG_STRUCT  PciePmeCtl;
  PCIEPMECTL2_PCIE_CFG_STRUCT PciePmeCtl2;
  PCIEPMECTL3_PCIE_CFG_STRUCT PciePmeCtl3;
  PCIEALC_PCIE_CFG_STRUCT     PcieAlc;
  PHYPG_PCIE_MEM_RCRB_STRUCT  Phypg;
  SPR_PCIE_CFG_STRUCT         Spr;

  PRINT_LEVEL1 ("%s(%d) Start \n", __FUNCTION__, pInst->RpIndex);

  ///
  /// Program (R_PCIE_SPR) E0hc
  /// E0h[0] - scb, clear this bit
  ///
  Spr.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, SPR_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
  Spr.Bits.scb = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, SPR_PCIE_CFG_REG, Spr.Data, IpWrRegFlagSize8Bits);

  if (pInst->PcieRpCommonConfig.Aspm != FALSE) {
    ///
    /// Section 4.4.2 ASPM
    ///
    SipConfigureASPM (pInst);
  }

  if (pInst->PcieRpCommonConfig.ClockGating) {
    ///
    /// Program Root Port Dynamic Clock Gate Enable register 0x00E1
    /// Set E1h[1:0] = 11b    (R_PCIE_CFG_RPDCGEN)
    ///
    RpdCgen.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, RPDCGEN_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    RpdCgen.Bits.rpdlcgen = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, RPDCGEN_PCIE_CFG_REG, RpdCgen.Data, IpWrRegFlagSize8Bits);
    RpdCgen.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, RPDCGEN_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    RpdCgen.Bits.rpdbcgen = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, RPDCGEN_PCIE_CFG_REG, RpdCgen.Data, IpWrRegFlagSize8Bits);
    ///
    /// Set F7h[6] to 1b     (R_PCIE_CFG_IOSFSBCS)
    ///
    IosfSbCs.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, IOSFSBCS_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    IosfSbCs.Bits.scptcge = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, IOSFSBCS_PCIE_CFG_REG, IosfSbCs.Data, IpWrRegFlagSize8Bits);

    //
    // Program PCIe PM Extension Control 3 register 0x0434
    // Set 0x434[3:2] to 1 to set Osc Clock Gate Hysterisis to 1us
    //
    PciePmeCtl3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PciePmeCtl3.Bits.osccgh      = V_PCIEPMECTL3_OSCCGH_1US;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL3_PCIE_CFG_REG, PciePmeCtl3.Data, IpWrRegFlagSize32Bits);

    SipConfigureDynamicClockGating (pInst);
  }

  //
  // Program Feature Control 2 register 0x1330
  // Set 0x1330[10] FCTL2.HPICTL         - 1b
  // Set 0x1330[27] FCTL2.RXCPPREALLOCEN - 1b
  //
  Fctl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, FCTL2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Fctl2.Bits.rxcppreallocen = 1;
  if (pInst->PcieRpCommonConfig.HotPlug) {
    Fctl2.Bits.hpictl = 0;
  } else {
    Fctl2.Bits.hpictl = 1;
  }
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, FCTL2_PCIE_MEM_RCRB_REG, Fctl2.Data, IpWrRegFlagSize32Bits);

  PcieAlc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEALC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieAlc.Bits.ssrld  = 0;
  PcieAlc.Bits.ssrrs  = 0;
  if (pInst->IsController) {
    PcieAlc.Bits.pdsp     = 1;
    PcieAlc.Bits.rtd3pdsp = 1;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEALC_PCIE_CFG_REG, PcieAlc.Data, IpWrRegFlagSize32Bits);

  if (pInst->PcieRpCommonConfig.PowerGating) {
    ///
    /// Set 420h[31] = 1b
    /// If CLKREQ# is supported or port is disabled set 420h[30,29] to 11b.
    /// 420h[29] (DLSULDLSD) and 420h[0] must be programmed if DLSULPPGE is set or PTOCGE is set.
    /// Assume that if PTOCGE is set CLKREQ is supported on this port.
    /// L1.LOW is disabled; if all conditions are met, it will be enabled later.
    /// Set 420h[0] = 0
    ///
    PciePmeCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PciePmeCtl.Bits.l1le       = 0;
    PciePmeCtl.Bits.l1snzcrewd = 0;
    PciePmeCtl.Bits.l1fsoe     = 0;
    PciePmeCtl.Bits.fdppge     = 1;
    PciePmeCtl.Bits.dlsulppge  = 1;
    //
    // As per 4.4.4 Squelch Power Management section
    // DLSULDLSD 420h[29] = 0 SIP17
    // POFFWT 420h[19:18] = 11b
    //
    PciePmeCtl.Bits.dlsuldlsd = 0;
    PciePmeCtl.Bits.poffwt    = V_PCIEPMECTL_POFFWT;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL_PCIE_CFG_REG, PciePmeCtl.Data, IpWrRegFlagSize32Bits);

    ///
    /// Program PCIe PM Extension Control 2 register 0x0424
    /// Set 424h[8,7] to 11b
    /// Disable Chassis PMC Save and Restore
    ///
    PciePmeCtl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      PciePmeCtl2.Bits.fdcpge   = 1;
      PciePmeCtl2.Bits.detscpge = 1;
      PciePmeCtl2.Bits.disscpge = 1;
      PciePmeCtl2.Bits.cpmcsre  = 1;
      PciePmeCtl2.Bits.cpgexh   = 0;
      PciePmeCtl2.Bits.cpgenh   = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PCIEPMECTL2_PCIE_CFG_REG, PciePmeCtl2.Data, IpWrRegFlagSize32Bits);

    if (pInst->SipVersion >= PCIE_SIP17) {
      ///
      /// Program (R_PCIE_ADVMCTRL) 5BCh
      /// Set 5BCh[22] to 1b for InRxL0 Control.
      /// Set 5BCh[21] to 0b for Accumulator Reset Mode
      /// Set 5BCh[20] to 1b for EIOS Disable DeSkew.
      /// Set 5BCh[19] to 1b for EIOS Mask Receiver Datapath.
      /// Set 5BCh[15] to 1b for RxL0 DeAssertion Control
      /// Set 5BCh[13] to 1b for Gen3 Short TLP Framing Error Reporting.
      /// Set 5BCh[11] to 0b for Reset Receiver Link Layer Common Logic.
      /// Set 5BCh[10] to 0b for Reset Link Layer in Gen1, 2 Recovery.
      /// Set 5BCh[23] to 0b for Completion Coalescing Break Event.
      ///
      AdvmCtrl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, ADVMCTRL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      AdvmCtrl.Bits.inrxl0ctrl = 1;
      AdvmCtrl.Bits.eiosdisds  = 1;
      AdvmCtrl.Bits.eiosmaskrx = 1;
      AdvmCtrl.Bits.rxl0dc     = 1;
      AdvmCtrl.Bits.g3stfer    = 1;
      AdvmCtrl.Bits.rrllcl     = 0;
      AdvmCtrl.Bits.rllg12r    = 0;
      AdvmCtrl.Bits.pmreqcwc   = 0x6;
      AdvmCtrl.Bits.pmreqblkpgrspt = V_ADVMCTRL_PMREQBLKPGRSPT_10US;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, ADVMCTRL_PCIE_CFG_REG, AdvmCtrl.Data, IpWrRegFlagSize32Bits);

      ///
      /// Program (R_PCIE_PGTHRES) 5C0h.
      /// Set 5C0h[0] to 1b for L1 power gating LTR Enable.
      /// Set 5C0h[31:29] to 10b for L1 power gating LTR Threshold latency Scale value. 010b = 1024nS
      /// Set 5C0h[25:16] to 110010b for L1 power gating LTR Threshold latency value. 110010b (0x32).
      /// @todo: Need to revisit the condition to enable L1 powergating
      ///
      PgThres.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PGTHRES_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      PgThres.Bits.l1pgltren   = 0x0;
      PgThres.Bits.l1pgltrtlsv = 0x2;
      PgThres.Bits.l1pgltrtlv  = 0x32;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PGTHRES_PCIE_CFG_REG, PgThres.Data, IpWrRegFlagSize32Bits);
    }
  }

  ///
  /// Vnn Removal register programming
  ///
  if (pInst->PrivateConfig.VnnRemovalSupported) {
    VnnRemCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, VNNREMCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    VnnRemCtl.Bits.fdvnnre    = 0;
    VnnRemCtl.Bits.hpvnnre    = 0;
    VnnRemCtl.Bits.dnpvnnre   = 0;
    VnnRemCtl.Bits.rtd3vnnre  = 0;
    VnnRemCtl.Bits.ldvnnre    = 0;
    VnnRemCtl.Bits.isplfvnnre = 0;
    VnnRemCtl.Bits.lrslfvnnre = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, VNNREMCTL_PCIE_CFG_REG, VnnRemCtl.Data, IpWrRegFlagSize32Bits);
  }

  //
  // Set PHYPG {4:0} to 0x1F
  //
  Phypg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PHYPG_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Phypg.Bits.detphypge   = 0x1;
  Phypg.Bits.disphypge   = 0x1;
  Phypg.Bits.l23phypge   = 0x1;
  Phypg.Bits.ducfgphypge = 0x1;
  Phypg.Bits.dlppgp      = 0x1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PHYPG_PCIE_MEM_RCRB_REG, Phypg.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Configure PCIe power down mapping

  @param[in] pInst                *pInst
**/
void
SipConfigurePowerDownMapping (
   IP_PCIE_INST            *pInst
  )
{
  PIPEPDCTL_PCIE_MEM_RCRB_STRUCT    PipePdCtl;
  PIPEPDCTL2_PCIE_MEM_RCRB_STRUCT   PipePdCtl2;
  PIPEPDCTL3_PCIE_MEM_RCRB_STRUCT   PipePdCtl3;
  PIPEPDCTLEXT_PCIE_MEM_RCRB_STRUCT PipePdCtlExt;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  //
  // Program PIPE Power Down Control register 0x1594
  //
  PipePdCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  if (pInst->SipPhyVersion == IP_HSPHY) {
    PipePdCtl.Bits.detpgpdctl   = V_PIPEPDCTL_DETPGPDCTL_PS3;
    PipePdCtl.Bits.l23pgpdctl   = V_PIPEPDCTL_L23PGPDCTL_PS3;
    PipePdCtl.Bits.disnopgpdctl = V_PIPEPDCTL_DISNOPGPDCTL_PS3;
    PipePdCtl.Bits.dispgpdctl   = V_PIPEPDCTL_DISPGPDCTL_PS3;
  } else {
    PipePdCtl.Bits.detpgpdctl   = V_PIPEPDCTL_DETPGPDCTL_PS13;
    PipePdCtl.Bits.l23pgpdctl   = V_PIPEPDCTL_L23PGPDCTL_PS13;
    PipePdCtl.Bits.disnopgpdctl = V_PIPEPDCTL_DISNOPGPDCTL_PS13;
    PipePdCtl.Bits.dispgpdctl   = V_PIPEPDCTL_DISPGPDCTL_PS13;
  }
  PipePdCtl.Bits.l23nopgpdctl = V_PIPEPDCTL_L23NOPGPDCTL;

  PipePdCtl.Bits.l1pgnopgpdctl = V_PIPEPDCTL_L1PGNOPGPDCTL;
  PipePdCtl.Bits.l1pgupgpdctl = V_PIPEPDCTL_L1PGUPGPDCTL;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTL_PCIE_MEM_RCRB_REG, PipePdCtl.Data, IpWrRegFlagSize32Bits);

  //
  // Program PIPE Power Down Control 2 register 0x1598
  //
  PipePdCtl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTL2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  PipePdCtl2.Bits.l1d2pgpdctl = V_PIPEPDCTL2_L1D2PGPDCTL;
  PipePdCtl2.Bits.l1d2nopgpdctl = V_PIPEPDCTL2_L1D2NOPGPDCTL;
  PipePdCtl2.Bits.l1d1pgpgpdctl = V_PIPEPDCTL2_L1D1PGPGPDCTL;
  PipePdCtl2.Bits.l1d1pgnopgpdctl = V_PIPEPDCTL2_L1D1PGNOPGPDCTL;
  PipePdCtl2.Bits.ducfgpgpdctl = V_PIPEPDCTL2_DUCFGPGPDCTL;
  PipePdCtl2.Bits.l1d1upupgpdctl = V_PIPEPDCTL2_L1D1UPUPGPDCTL;
  PipePdCtl2.Bits.l1upnopgpdctl = V_PIPEPDCTL2_L1UPNOPGPDCTL;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTL2_PCIE_MEM_RCRB_REG, PipePdCtl2.Data, IpWrRegFlagSize32Bits);

  //
  // Program PIPE Power Down Control 3 register 0x159c
  //
  PipePdCtl3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTL3_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  PipePdCtl3.Bits.l1dlownopgpdctl = V_PIPEPDCTL3_L1DLOWNOPGPDCTL;
  PipePdCtl3.Bits.l1dlowpgpdctl = V_PIPEPDCTL3_L1DLOWPGPDCTL;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTL3_PCIE_MEM_RCRB_REG, PipePdCtl3.Data, IpWrRegFlagSize32Bits);

  //
  // Program PIPE Power Down Control Extension register 0x15a0
  //
  PipePdCtlExt.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTLEXT_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  PipePdCtlExt.Bits.p2tp2tcd = 0;
  PipePdCtlExt.Bits.p2tp2tp  = 0;
  PipePdCtlExt.Bits.lsdpmrfm = V_PIPEPDCTLEXT_LSDPMRFM;
  PipePdCtlExt.Bits.pddpmrfm = V_PIPEPDCTLEXT_PDDPMRFM;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PIPEPDCTLEXT_PCIE_MEM_RCRB_REG, PipePdCtlExt.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Perform reset prep programming

  @param[in] pInst                *pInst
**/
void
SipConfigureResetPrep (
   IP_PCIE_INST            *pInst
  )
{
  FCTL2_PCIE_MEM_RCRB_STRUCT      Fctl2;
  RPR_PCIE_MEM_RCRB_STRUCT        Rpr;
  RPDEC1_PCIE_MEM_RCRB_STRUCT     RpDec1;
  RPDEC2_PCIE_MEM_RCRB_STRUCT     RpDec2;
  RPDEC3_PCIE_MEM_RCRB_STRUCT     RpDec3;
  TXCRSTOCTL_PCIE_MEM_RCRB_STRUCT TxCrstoCtl;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  //
  // Program Feature Control 2 register 0x1330
  //
  Fctl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, FCTL2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Fctl2.Bits.rptot = V_FCTL2_RPTOT;
  Fctl2.Bits.hrtctl = 0x0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, FCTL2_PCIE_MEM_RCRB_REG, Fctl2.Data, IpWrRegFlagSize32Bits);

  //
  // Program Reset Policy register 0x1334
  //
  Rpr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, RPR_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Rpr.Bits.wrm = 0x1;
  Rpr.Bits.crm = 0x1;
  Rpr.Bits.s3sm = 0x1;
  Rpr.Bits.s4sm = 0x1;
  Rpr.Bits.s5sm = 0x1;
  Rpr.Bits.dmto = 0x0;
  Rpr.Bits.wrmto = 0x0;
  Rpr.Bits.crmto = 0x0;
  Rpr.Bits.s3smto = 0x0;
  Rpr.Bits.s4smto = 0x0;
  Rpr.Bits.s5smto = 0x0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, RPR_PCIE_MEM_RCRB_REG, Rpr.Data, IpWrRegFlagSize32Bits);

  //
  // Program Reset Prep Decode 1 register 0x1780
  //
  RpDec1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, RPDEC1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  RpDec1.Bits.rpwrert = V_RPDEC1_RPWRERT;
  RpDec1.Bits.rpwrept = 0;
  RpDec1.Bits.rpcrert = V_RPDEC1_RPCRERT;
  RpDec1.Bits.rpcrept = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, RPDEC1_PCIE_MEM_RCRB_REG, RpDec1.Data, IpWrRegFlagSize32Bits);

  //
  // Program Reset Prep Decode 2 register 0x1784
  //
  RpDec2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, RPDEC2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  RpDec2.Bits.rps3ert = V_RPDEC2_RPS3ERT;
  RpDec2.Bits.rps3ept = 0;
  RpDec2.Bits.rps4ert = V_RPDEC2_RPS4ERT;
  RpDec2.Bits.rps4ept = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, RPDEC2_PCIE_MEM_RCRB_REG, RpDec2.Data, IpWrRegFlagSize32Bits);

  //
  // Program Reset Prep Decode 3 register 0x1788
  //
  RpDec3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, RPDEC3_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  RpDec3.Bits.rps5ert = V_RPDEC3_RPS5ERT;
  RpDec3.Bits.rps5ept = 0;
  RpDec3.Bits.rpdh = V_RPDEC3_RPDH;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, RPDEC3_PCIE_MEM_RCRB_REG, RpDec3.Data, IpWrRegFlagSize32Bits);

  //
  // Program Transmit Timeout and Configuration Retry Timeout register 0x1320
  //
  TxCrstoCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, TXCRSTOCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  TxCrstoCtl.Bits.txnpctodis = 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, TXCRSTOCTL_PCIE_MEM_RCRB_REG, TxCrstoCtl.Data, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Set SRL.

  @param[in] pInst  Pointer to IP_PCIE_INST
**/
static
void
SipSetSecureRegisterLock (
  IP_PCIE_INST   *pInst
  )
{
  LPCR_PCIE_CFG_STRUCT   Lpcr;
  LPCRE_PCIE_CFG_STRUCT  Lpcre;

  Lpcre.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LPCRE_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Lpcre.Bits.ipcl = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LPCRE_PCIE_CFG_REG, Lpcre.Data, IpWrRegFlagSize32Bits);

  ///Secure Register Lock (SRL) 0xC8C[0]: 0
  ///Secure Equalization Register Lock (SERL) 0XC8C[8]: 1
  ///LTR Configuration Lock (LTRCFGLOCK) 0xC8C[16]: 0
  ///Device ID Override Lock (DIDOVR_LOCK) 0xC8C[24]: 1
  Lpcr.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LPCR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Lpcr.Bits.serl = 1;
  Lpcr.Bits.didovr_lock = 1;
  Lpcr.Bits.srl = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LPCR_PCIE_CFG_REG, Lpcr.Data, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("R_PCIE_LPCR after setting SRL = %x\n", Lpcr.Data);
}

/**
  Program Rx primary Cycle Decode Registers for SIP17

  @param[in] pInst   Pointer to IP_PCIE_INST
**/
void
SipRxMasterCycleDecode (
   IP_PCIE_INST            *pInst
  )
{
  DECCTL_PCIE_MEM_RCRB_STRUCT   Decctl;
  PIDECCTL_PCIE_MEM_RCRB_STRUCT PiDecctl;

  PRINT_LEVEL1 ("%s start\n", __FUNCTION__);

  //
  // Program Decoder Control Register 0x1904
  //
  Decctl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, DECCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Decctl.Bits.atsce = 1;
  Decctl.Bits.busnumzchk = 1;
  Decctl.Bits.mtrxltc = 1;
  Decctl.Bits.outbexpcplchken = 1;
  Decctl.Bits.dropcplatnzce = 1;
  Decctl.Bits.lcrxint = 1;
  Decctl.Bits.vdmatac = 1;
  Decctl.Bits.urvdme16dw = 1;
  Decctl.Bits.urrxurtrcvdm = 1;
  Decctl.Bits.urrxultvdm = 1;
  Decctl.Bits.urrxuravdm = 1;
  Decctl.Bits.urrxuridvdm = 1;
  Decctl.Bits.urrxuorvdm = 1;
  Decctl.Bits.urrxvmctpbfrc = 1;
  Decctl.Bits.ichkinvreqrmsgrbid = 1;
  Decctl.Bits.rxmctpdecen = 1;
  Decctl.Bits.rxgpedecen = 1;
  Decctl.Bits.rxadleddecen = 1;
  Decctl.Bits.lcrxerrmsg = 1;
  Decctl.Bits.lcrxptmreq = 1;
  Decctl.Bits.urrxuvdmrbfrc = 1;
  Decctl.Bits.urrxuvdmrgrtrc = 1;
  Decctl.Bits.rxmctpbrcdecen = 1;
  Decctl.Bits.urrxmctpntco = 1;
  Decctl.Bits.rxsbemcapdecen = 0;
  Decctl.Bits.urrxuvdmintelvid = 0;
  Decctl.Bits.urrxuvdmuvid = 0;
  Decctl.Bits.rxvdmdece = 0;
  Decctl.Bits.rximdecen = 0;
  Decctl.Bits.rxltrmdech = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, DECCTL_PCIE_MEM_RCRB_REG, Decctl.Data, IpWrRegFlagSize32Bits);

  //
  // Program Primary Interface Decoder Control register 0x190C
  // Program (CPLBNCHK) 0X190C[0]  0x1
  //
  PiDecctl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PIDECCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  PiDecctl.Bits.cplbnchk = 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PIDECCTL_PCIE_MEM_RCRB_REG, PiDecctl.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Perform some early Pcie Root port configuration to enable RCRB and program decoder control registers

  @param[in] pInst  Pointer to IP_PCIE_INST
**/
void
SipEarlyDecodeEnable (
   IP_PCIE_INST            *pInst
  )
{
  DC_PCIE_CFG_STRUCT    Dc;

  PRINT_LEVEL1 ("%s Start.\n", __FUNCTION__);

  //
  // Set Completion Ordering Mode
  //
  Dc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Dc.Bits.com = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DC_PCIE_CFG_REG, Dc.Data, IpWrRegFlagSize32Bits);
  //
  // RX primary Cycle Decoding Programming
  //
  SipRxMasterCycleDecode (pInst);

  PRINT_LEVEL1 ("%s End.\n", __FUNCTION__);
}

/**
  Configure Peer Disable

  @param[in] pInst               Pointer to the IP_PCIE_INST
  @param[in] CfgReads            TRUE/FALSE, enable/disable forwarding of Upstream Posted Memory Reads
  @param[in] CfgWrites           TRUE/FALSE, enable/disable forwarding of Upstream Posted Memory Writes
**/
void
SipConfigurePeerDisable (
  IP_PCIE_INST            *pInst,
  BOOLEAN                 CfgReads,
  BOOLEAN                 CfgWrites
  )
{
  CCFG_PCIE_CFG_STRUCT   Ccfg;


  if (pInst->SipVersion < IpPcieSip16) {
    PRINT_LEVEL1 ("PCIe SIP < %d does not support Peer Disable\n", IpPcieSip16);
    return;
  }

  /// Program Channel Configuration register 0x00D0
  Ccfg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CCFG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Ccfg.Bits.umrpd  = 0;
  Ccfg.Bits.upmwpd = 0;
  if (CfgReads) {
    Ccfg.Bits.umrpd = 1;
  }
  if (CfgWrites) {
    Ccfg.Bits.upmwpd = 1;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CCFG_PCIE_CFG_REG, Ccfg.Data, IpWrRegFlagSize32Bits);
}

/**
  Configure PCIe Completion Coalescing

  @param[in] RpDev               Pointer to the root port device
**/
void
SipConfigureCoalescing (
  IP_PCIE_INST            *pInst
  )
{
  COCTL_PCIE_CFG_STRUCT   CoCtl;

  if (pInst->SipVersion < IpPcieSip16) {
    PRINT_LEVEL1 ("PCIe SIP < %d does not support Completion Coalescing\n", IpPcieSip16);
    return;
  }

  CoCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, COCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);

  ///
  /// Program COCTL.PWCE       594h [0]     = 1b
  /// Program COCTL.DDCE       594h [1]     = 1b
  /// Program COCTL.CT         594h [9:2]   = 3h
  /// Program COCTL.CTE        594h [10]    = 1h
  /// Program COCTL.PCLM       594h [14:13] = 2b
  /// Program COCTL.NPCLM      594h [16:15] = 2b
  /// Program COCTL.CHAINBARBE 594h [18]    = 1b
  ///
  CoCtl.Bits.pwce       = 1;
  CoCtl.Bits.ddce       = 1;
  CoCtl.Bits.cte        = 1;
  CoCtl.Bits.chainbarbe = 1;
  CoCtl.Bits.roaop      = 0;
  CoCtl.Bits.pclm       = 0x2;
  CoCtl.Bits.npclm      = 0x2;
  CoCtl.Bits.ct         = pInst->PrivateConfig.ChainTimersValue;

  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, COCTL_PCIE_CFG_REG, CoCtl.Data, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("COCTL = 0x%x\n", CoCtl.Data);
  return;
}

/**
  Completion Timeout Range Supported

  @param[in] PCIE_ROOT_PORT_COMMON_CONFIG  Root port Common Config.
  @param[in] PcieCfgDcap2 DCAP2 data.
  @param[in] PcieCfgDctl2 DCTL2 data.

**/
void
IpCompletionTimeoutRangeSupported (
  IP_PCIE_INST           *pInst,
  DCAP2_PCIE_CFG_STRUCT  *Dcap2,
  DCTL2_PCIE_CFG_STRUCT  *Dctl2
  )
{

  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_50_100us || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_1_10ms) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_A) {
      Dctl2->Bits.ctv = pInst->PcieRpCommonConfig.CompletionTimeout;
    } else {
          pInst->PcieRpCommonConfig.CompletionTimeout = V_PCIE_DCTL2_CTV_16MS_55MS;
    }
  }
  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_16_55ms || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_65_210ms) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_B) {
      Dctl2->Bits.ctv= pInst->PcieRpCommonConfig.CompletionTimeout;
    } else {
          pInst->PcieRpCommonConfig.CompletionTimeout = V_PCIE_DCTL2_CTV_260MS_900MS;
    }
  }
  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_260_900ms || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_1_3P5s) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_C) {
      Dctl2->Bits.ctv = pInst->PcieRpCommonConfig.CompletionTimeout;
    } else {
      pInst->PcieRpCommonConfig.CompletionTimeout = V_PCIE_DCTL2_CTV_4S_13S;
    }
  }
  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_4_13s || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_17_64s) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_D) {
      Dctl2->Bits.ctv = pInst->PcieRpCommonConfig.CompletionTimeout;
    } else {
      Dctl2->Bits.ctd = TRUE;
    }
  }
}


/**
  Completion Timeout Max Range Supported

  @param[in] PCIE_ROOT_PORT_COMMON_CONFIG  Root port Common Config.
  @param[in] PcieCfgDcap2 DCAP2 data.
  @param[in] PcieCfgDctl2 DCTL2 data.

**/
void
IpCompletionTimeoutMaxRangeSupported (
  IP_PCIE_INST           *pInst,
  DCAP2_PCIE_CFG_STRUCT  *Dcap2,
  DCTL2_PCIE_CFG_STRUCT  *Dctl2
  )
{

  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_4_13s || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_17_64s) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_D) {
      Dctl2->Bits.ctv = pInst->PcieRpCommonConfig.CompletionTimeout;
    } else {
      pInst->PcieRpCommonConfig.CompletionTimeout = IpPcieCompletionTO_1_3P5s;
    }
  }

  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_260_900ms || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_1_3P5s) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_C) {
      Dctl2->Bits.ctv = pInst->PcieRpCommonConfig.CompletionTimeout;
    } else {
      pInst->PcieRpCommonConfig.CompletionTimeout = IpPcieCompletionTO_65_210ms;
    }
  }

  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_16_55ms || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_65_210ms) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_B) {
      Dctl2->Bits.ctv= pInst->PcieRpCommonConfig.CompletionTimeout;
    } else {
      pInst->PcieRpCommonConfig.CompletionTimeout = IpPcieCompletionTO_1_10ms;
    }
  }

  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_50_100us || pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_1_10ms) {
    if (Dcap2->Data & B_PCIE_DCTL2_RANGE_A) {
      Dctl2->Bits.ctv = pInst->PcieRpCommonConfig.CompletionTimeout;
    }
  }
}

/**
  Performs mandatory Root Port Initialization.
  This function is silicon-specific and configures proprietary registers.

  @param[in] pInst                   *pInst
**/
void
SipInitRootPort (
  IP_PCIE_INST    *pInst
  )
{
  IP_PCIE_SPEED                      RootPortSpeed;
  UINT32                             Cls;
  UINT32                             RpMaxPayloadCapability;
  BOOLEAN                            CompletionTimeoutDefault;
  LSTS_PCIE_CFG_STRUCT               Lsts;
  PCIEDBG_PCIE_CFG_STRUCT            PcieDbg;
  PCIECFG2_PCIE_CFG_STRUCT           PcieCfg2;
  MPC2_PCIE_CFG_STRUCT               Mpc2;
  CCFG_PCIE_CFG_STRUCT               Ccfg;
  DCAP_PCIE_CFG_STRUCT               Dcap;
  LCTL_PCIE_CFG_STRUCT               Lctl;
  CMD_PCIE_CFG_STRUCT                Cmd;
  FCTL_PCIE_MEM_RCRB_STRUCT          Fctl;
  MPHYCAPCFG_PCIE_MEM_RCRB_STRUCT    MPhyCapCfg;
  TXMDEC1_PCIE_MEM_RCRB_STRUCT       TxmDec1;
  PWRCTL_PCIE_CFG_STRUCT             PwrCtl;
  MPC_PCIE_CFG_STRUCT                Mpc;
  LTROVR2_PCIE_CFG_STRUCT            Ltrovr2;
  LCAP_PCIE_CFG_STRUCT               Lcap;
  PCIEPMECTL_PCIE_CFG_STRUCT         PciePmeCtl;
  PCIEPMECTL2_PCIE_CFG_STRUCT        PciePmeCtl2;
  PHYCTL2_PCIE_CFG_STRUCT            PhyCtl2;
  DCTL_PCIE_CFG_STRUCT               Dctl;
  DCTL2_PCIE_CFG_STRUCT              Dctl2;
  SLCAP_PCIE_CFG_STRUCT              Slcap;
  G5CAP_PCIE_CFG_STRUCT              G5cap;
  G5CTL_PCIE_CFG_STRUCT              G5ctl;
  DPCCAPR_PCIE_CFG_STRUCT            Dpccapr;
  ACGR3S2_PCIE_CFG_STRUCT            Acgr3s2;
  RCTL_PCIE_CFG_STRUCT               Rctl;
  SLSTS_PCIE_CFG_STRUCT              Slsts;
  SLCTL_PCIE_CFG_STRUCT              Slstl;
  UEM_PCIE_CFG_STRUCT                Uem;
  BCTRL_PCIE_CFG_STRUCT              Bctrl;
  STRPFUSECFG_PCIE_CFG_STRUCT        StrpFuseCfg;
  TNPT_PCIE_CFG_STRUCT               Tnpt;
  EINJCTL_PCIE_CFG_STRUCT            EinjCtl;
  RXMC1_PCIE_CFG_STRUCT              Rxmc1;
  PL16MPCPSB01_PCIE_CFG_STRUCT       Pl16Mpcpsb01;
  IPCS_PCIE_CFG_STRUCT               Ipcs;
  V0CTL_PCIE_CFG_STRUCT              V0Ctl;
  V1CTL_PCIE_CFG_STRUCT              V1Ctl;
  DCAP2_PCIE_CFG_STRUCT              Dcap2;
  LTROVR_PCIE_CFG_STRUCT             Ltrovr;
  ADVMCTRL_PCIE_CFG_STRUCT           AdvmCtrl;
  AECR1G3_PCIE_CFG_STRUCT            Aecr1g3;
  PCIEL0SC_PCIE_CFG_STRUCT           PcieL0Sc;
  PCIERTP1_PCIE_CFG_STRUCT           PcieRtp;
  AECR2G3_PCIE_CFG_STRUCT            Aecr2g3;
  GDR_PCIE_CFG_STRUCT                Gdr;
  IORCCP1_PCIE_MEM_RCRB_STRUCT       Iorccp1;
  IPSCE_PCIE_MEM_RCRB_STRUCT         IpsceCfg;

  PRINT_LEVEL1 ("%s Start (%d)\n", __FUNCTION__, pInst->RpIndex);

  RootPortSpeed = IpPcieGen1;
  /// Link Status register 0x0052
  Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Cls       = Lsts.Bits.cls;

  if(Cls > V_CLS_GEN2) {
    IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
  }

  /// PCH BIOS Spec Section 8.2.10 Completion Retry Status Replay Enable
  /// Following reset it is possible for a device to terminate the
  /// configuration request but indicate that it is temporarily unable to process it,
  /// but in the future. The device will return the Configuration Request Retry Status.
  /// By setting the Completion Retry Status Replay Enable, Dxx:Fn + 320h[22],
  /// the RP will re-issue the request on receiving such status.
  /// The BIOS shall set this bit before first configuration access to the endpoint.
  /// Program PCI Express Configuration 2 register 0x0320
  PcieCfg2.Data   = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIECFG2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieCfg2.Bits.crsren = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIECFG2_PCIE_CFG_REG, PcieCfg2.Data, IpWrRegFlagSize32Bits);

  //
  // Set speed capability in rootport
  //
  Mpc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Mpc.Bits.pciesd = 0;
  switch (pInst->PcieRpCommonConfig.PcieSpeed) {
    case IpPcieGen1:
      Mpc.Bits.pciesd = 1;
      break;
    case IpPcieGen2:
      Mpc.Bits.pciesd = 2;
      break;
    case IpPcieGen3:
      Mpc.Bits.pciesd = 3;
      break;
    case IpPcieGen4:
      Mpc.Bits.pciesd = 4;
      break;
    case IpPcieAuto:
      break;
    default:
      PRINT_ERROR ( "Invalid PCIe Speed\n");
      break;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, Mpc.Data, IpWrRegFlagSize32Bits);

  RootPortSpeed = (IP_PCIE_SPEED) SipGetMaxLinkSpeed (pInst);
  Ccfg.Data     = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CCFG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  ///
  /// If only 128B max payload is supported set CCFG.UNRS to 0.
  ///
  /// and program all the PCH Root Ports such that upstream posted writes and upstream non-posted requests
  /// are split at 128B boundary by setting CCFG fields: UPSD to 0, CCFG.UPRS to 000b and UNSD to 0, UNRS to 000b
  ///

  ///
  /// Set UPRS and UNRS to 64B for SipVersion > 17.
  /// Program Channel Configuration register 0x00D0
  ///
  Ccfg.Data    = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CCFG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);

  RpMaxPayloadCapability = pInst->PcieRpCommonConfig.MaxPayload;
  ///
  /// Set B0:Dxx:Fn + D0h [24] to 0b
  ///
  Ccfg.Bits.uprs = pInst->PrivateConfig.PayLoadConfig.Uprs;
  Ccfg.Bits.unrs = pInst->PrivateConfig.PayLoadConfig.Unrs;

  Ccfg.Bits.unrd = 0;
  Ccfg.Bits.upsd = pInst->PrivateConfig.PayLoadConfig.Upsd;
  Ccfg.Bits.unsd = pInst->PrivateConfig.PayLoadConfig.Unsd;

  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CCFG_PCIE_CFG_REG, Ccfg.Data, IpWrRegFlagSize32Bits);
  /// Program Device Capabilities register 0x0044
  Dcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Dcap.Bits.mps= RpMaxPayloadCapability;
  Dcap.Bits.etfs = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCAP_PCIE_CFG_REG, Dcap.Data, IpWrRegFlagSize32Bits);

  //
  // Set IOSF max payload size For root ports with SIP version > 17 set 64 byte aligned offset.
  // Set IOSF max read request size For root ports with SIP version > 17 set 256 byte aligned offset.
  //
  Ipcs.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, IPCS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Ipcs.Bits.imps = pInst->PrivateConfig.PayLoadConfig.Imps;
  Ipcs.Bits.imrs = pInst->PrivateConfig.PayLoadConfig.Imrs;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, IPCS_PCIE_CFG_REG, Ipcs.Data, IpWrRegFlagSize32Bits);

  V0Ctl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, V0CTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  V0Ctl.Bits.tvm   = 0x7F;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, V0CTL_PCIE_CFG_REG, V0Ctl.Data, IpWrRegFlagSize32Bits);

  V1Ctl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, V1CTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  V1Ctl.Bits.id = V_VC1CTL_ID_ONE;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, V1CTL_PCIE_CFG_REG, V1Ctl.Data, IpWrRegFlagSize32Bits);

  Dcap2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCAP2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if (pInst->PcieRpCommonConfig.Aspm != FALSE) {
    ///
    /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
    /// If B0:Dxx:Fn + 400h is programmed, BIOS will also program B0:Dxx:Fn + 404h [1:0] = 11b,
    /// to enable these override values.
    /// - Fn refers to the function number of the root port that has a device attached to it.
    /// - Default override value for B0:Dxx:Fn + 400h should be 880F880Fh
    /// - Also set 404h[2] to lock down the configuration
    /// - Refer to table below for the 404h[3] policy bit behavior.
    /// Done in PcieSetPm()
    ///
    /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
    /// Program B0:Dxx:Fn + 64h [11] = 1b
    ///
    if (pInst->PcieRpCommonConfig.LtrEnable == TRUE) {
      Dcap2.Bits.ltrms = 1;
    } else {
      Dcap2.Bits.ltrms = 0;
    }
    ///
    /// Optimized Buffer Flush/Fill (OBFF) is not supported.
    /// Program B0:Dxx:Fn + 64h [19:18] = 0h
    ///
    Dcap2.Bits.obffs = 0;

    ///
    /// Latency Tolerance Reporting Override (LTROVR) 0x400  31:0 0x88428842
    ///
    Ltrovr.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTROVR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Ltrovr.Bits.ltrslovrv   = 0x42;
    Ltrovr.Bits.ltrslsovrv  = 0x2;
    Ltrovr.Bits.ltrsrovr    = 0x1;
    Ltrovr.Bits.ltrnslovrv  = 0x42;
    Ltrovr.Bits.ltrnslsovrv = 0x2;
    Ltrovr.Bits.ltrnsrovr   = 0x1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTROVR_PCIE_CFG_REG, Ltrovr.Data, IpWrRegFlagSize32Bits);
    ///
    /// LTR LTR Override Policy (LTROVRPLCY) 0x404 3 0
    /// LTR Non-Snoop Override Enable (LTRNSOVREN) 0x404  1 1
    /// LTR Snoop Override Enable (LTRSOVREN) 0x404  0 1
    ///
    Ltrovr2.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LTROVR2_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
    Ltrovr2.Bits.ltrovrplcy = 0;
    Ltrovr2.Bits.ltrnsovren = 1;
    Ltrovr2.Bits.ltrsovren  = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LTROVR2_PCIE_CFG_REG, Ltrovr2.Data, IpWrRegFlagSize8Bits);

    ///
    /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
    /// Program B0:Dxx:Fn + 68h [10] = 1b
    ///
    Dctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    if (pInst->PcieRpCommonConfig.LtrEnable == TRUE) {
      Dctl2.Bits.ltren = 1;
    } else {
      Dctl2.Bits.ltren = 0;
    }
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, Dctl2.Data, IpWrRegFlagSize16Bits);
  }

  ///
  /// Program registers to enable Atomics and 10-Bit Requester Supported
  ///
  if (pInst->PrivateConfig.AtomicOpEnabled) {
    Dcap2.Bits.ars      = 1;
    Dcap2.Bits.ac32bs   = 1;
    Dcap2.Bits.ac64bs   = 1;
    Dcap2.Bits.ac128bs  = 1;
  } else {
    Dcap2.Bits.ars      = 0;
    Dcap2.Bits.ac32bs   = 0;
    Dcap2.Bits.ac64bs   = 0;
    Dcap2.Bits.ac128bs  = 0;
  }
  Dcap2.Bits.px10btcs = 1;
  Dcap2.Bits.px10btrs = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCAP2_PCIE_CFG_REG, Dcap2.Data, IpWrRegFlagSize32Bits);

  //
  // Squelch Power Management
  //
  SipConfigureSquelchPowerManagement (pInst);

  //
  // Configure Phy Settings
  //
  SipConfigurePhyInit (pInst);

  //
  // Program ARE and AEB
  //
  Dctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Dctl2.Bits.aeb = 0;
  Dctl2.Bits.are = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, Dctl2.Data, IpWrRegFlagSize16Bits);

  //
  // Program 10-Bit Tag Requester Enable (PX10BTRE) 0x68[12] = 0x0
  //
  Dctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Dctl2.Bits.px10btre = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, Dctl2.Data, IpWrRegFlagSize16Bits);

  AdvmCtrl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, ADVMCTRL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if ((pInst->SipPhyVersion == IP_HSPHY) || (pInst->SipPhyVersion == IP_SE32)) {
    AdvmCtrl.Bits.f10btse = 1;
  } else {
    AdvmCtrl.Bits.f10btse = 0;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, ADVMCTRL_PCIE_CFG_REG, AdvmCtrl.Data, IpWrRegFlagSize32Bits);

  ///
  /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
  /// Step 3 done in PciExpressHelpersLibrary.c ConfigureLtr
  ///
  PcieRtp.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIERTP1_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
  PcieRtp.Bits.g1x4 = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIERTP1_PCIE_CFG_REG, PcieRtp.Data, IpWrRegFlagSize8Bits);

  //
  // Elastic buffer and simplified retimer mode programming. Needs to be done before equalization.
  //
    //
    // Set Dxx:Fn + 0xC50 [5, 6, 7, 8, 9]     to [1, 1, 1, 1, 1] for SIP17
    // Set Dxx:Fn + 0xC50 [5, 6, 7, 8, 9, 20] to [1, 1, 1, 0, 0, 0] for SipVersion > SIP17
    //
  Acgr3s2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, ACGR3S2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Acgr3s2.Bits.g1ebm = 1;
  Acgr3s2.Bits.g2ebm = 1;
  Acgr3s2.Bits.srt   = 1;
  Acgr3s2.Bits.g3ebm = 0;
  Acgr3s2.Bits.g4ebm = 0;
  Acgr3s2.Bits.g5ebm = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, ACGR3S2_PCIE_CFG_REG, Acgr3s2.Data, IpWrRegFlagSize32Bits);

  /// Program Advance Extended Control Register 1 Group 3 register 0x0C80
  Aecr1g3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, AECR1G3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Aecr1g3.Bits.l1offrdyhewten = 0;
  Aecr1g3.Bits.l1offrdyhewt   = 0;
  Aecr1g3.Bits.tpse = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, AECR1G3_PCIE_CFG_REG, Aecr1g3.Data, IpWrRegFlagSize32Bits);

  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Set Dxx:Fn + 318h [31:16] = 1414h (Gen2 and Gen1 Active State L0s Preparation Latency)
  ///
  PcieL0Sc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEL0SC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieL0Sc.Bits.g1asl0spl = 0x14;
  PcieL0Sc.Bits.g2asl0spl = 0x14;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEL0SC_PCIE_CFG_REG, PcieL0Sc.Data, IpWrRegFlagSize32Bits);

  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// 1.  Program Dxx:Fn + E8h[20] to [1]
  /// Program Power Control register 0x00E8
  ///
  PwrCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PWRCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PwrCtl.Bits.lifecf = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PWRCTL_PCIE_CFG_REG, PwrCtl.Data, IpWrRegFlagSize32Bits);

  ///
  /// 2.  Program Dxx:Fn + 320h[27] to [0]
  /// Enable PCIe Relaxed Order to always allow downstream completions to pass posted writes,
  /// 3.  Set B0:Dxx:Fn:320h[24:23] = 11b
  /// Set PME timeout to 10ms, by
  /// 4.  Set B0:Dxx:Fn:320h[21:20] = 01b
  /// Program PCI Express Configuration 2 register 0x0320
  ///

  PcieCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIECFG2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieCfg2.Bits.rllg3r = 0;
  PcieCfg2.Bits.pmet   = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIECFG2_PCIE_CFG_REG, PcieCfg2.Data, IpWrRegFlagSize32Bits);

  ///
  /// RXROM/TXROM programming update.
  ///
  IpsceCfg.Data = (UINT32) IpWrRegRead(pInst->RegCntxt_Mem_Rcrb, IPSCE_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  IpsceCfg.Bits.txrom = 0x2;
  IpsceCfg.Bits.rxrom = 0x0;
  IpWrRegWrite(pInst->RegCntxt_Mem_Rcrb, IPSCE_PCIE_MEM_RCRB_REG, IpsceCfg.Data, IpWrRegFlagSize32Bits);

  ///
  /// Program PCI Express Debug And Configuration register 0x0324
  /// Enable Completion Time-Out Non-Fatal Advisory Error, Dxx:Fn + 324h[14] = 1b
  /// Set LDSWQRP Dxx:Fn + 324h[13] = 0
  ///
  PcieDbg.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEDBG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieDbg.Bits.ldswqrp = 0;
  PcieDbg.Bits.ctonfae = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEDBG_PCIE_CFG_REG, PcieDbg.Data, IpWrRegFlagSize32Bits);

  ///
  /// Program PCIe PM Extension Control 2 register 0x0424
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Program Dxx:Fn + 424h [6, 5, 4] = [1, 1, 1]
  ///
  PciePmeCtl2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEPMECTL2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PciePmeCtl2.Bits.l1scpge     = 1;
  PciePmeCtl2.Bits.l23rdyscpge = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEPMECTL2_PCIE_CFG_REG, PciePmeCtl2.Data, IpWrRegFlagSize32Bits);

  ///
  /// Program Link Capabilities register 0x004C
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// If Dxx:Fn + F5h[0] = 1b or step 3 is TRUE, set Dxx:Fn + 4Ch[17:15] = 4h
  /// Else set Dxx:Fn + 4Ch[17:15] = 010b
  ///
  Lcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Lcap.Bits.el1 = 0x6;

  ///
  /// Set LCAP APMS according to platform policy.
  ///
  if (pInst->PcieRpCommonConfig.Aspm < IpPchPcieAspmAutoConfig) {
    Lcap.Bits.apms = pInst->PcieRpCommonConfig.Aspm;;
  } else {
    PRINT_LEVEL1 ("Programming LCAP to L0S and L1\n");
    Lcap.Bits.apms = 0x3;
  }
  //
  // Program Link Capabilities register 0x004C
  // The EL1, ASPMOC and APMS of LCAP are RWO, must program all together.
  //
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCAP_PCIE_CFG_REG, Lcap.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("LCAP = 0x%x \n", Lcap.Data);

  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Set MPC.IRRCE, Dxx:Fn + D8h[25] = 1b
  /// For system that support MCTP over PCIE set
  /// Set PCIE RP PCI offset D8h[27] = 1b
  /// Set PCIE RP PCI offset D8h[21] = 1b
  /// Set PCIE RP PCI offset D8h[3] = 1b
  /// Set Flow Control Update Policy D8[6:4] = 0x4 for SIP Version > 17
  ///
  Mpc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Mpc.Bits.fcdl1e = 1;
  Mpc.Bits.irrce  = 0;
  Mpc.Bits.mmbnce = 1;
  if (!pInst->PrivateConfig.VtdEnabled) {
    Mpc.Bits.irrce = 1;
  }
  Mpc.Bits.mctpse = 1;
  Mpc.Bits.fcp    = 0x4;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, Mpc.Data, IpWrRegFlagSize32Bits);

  ///
  /// Peer Disable. Starting from SIP16
  ///
  if (pInst->PcieRpCommonConfig.EnablePeerMemoryWrite) {
    SipConfigurePeerDisable (pInst, TRUE, FALSE);
  } else {
    SipConfigurePeerDisable (pInst, TRUE, TRUE);
  }
  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Set PCIE RP PCI offset F5h[7:4] = 0000b
  ///
  PhyCtl2.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PHYCTL2_PCIE_CFG_REG, IpWrRegFlagSize8Bits);
  PhyCtl2.Bits.tdft         = 0;
  PhyCtl2.Bits.txcfgchgwait = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PHYCTL2_PCIE_CFG_REG, PhyCtl2.Data, IpWrRegFlagSize8Bits);


  ///
  /// Configure Completion Timeout
  ///
  Dctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Dcap2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCAP2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  CompletionTimeoutDefault = FALSE;
  if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_Disabled) {
    if(Dcap2.Bits.ctds) {
      Dctl2.Bits.ctd = 1;
    } else if(Dcap2.Bits.ctrs) {
      pInst->PcieRpCommonConfig.CompletionTimeout = IpPcieCompletionTO_4_13s;
      IpCompletionTimeoutMaxRangeSupported(pInst, &Dcap2, &Dctl2);
    }
  } else if (pInst->PcieRpCommonConfig.CompletionTimeout == IpPcieCompletionTO_Default) {
    CompletionTimeoutDefault = TRUE;
  } else {
    if(Dcap2.Bits.ctrs) {
      IpCompletionTimeoutRangeSupported(pInst, &Dcap2, &Dctl2);
    }
  }

  if (pInst->PcieRpCommonConfig.EnableDtr) {
    PRINT_LEVEL1 ("DTR: prepare to programe CTV = %d on Port %d\n", Dctl2.Bits.ctv, pInst->RpIndex);
    if (Dctl2.Bits.ctv <= IpPcieCompletionTO_1_10ms)
    {
      //DTR not need since ctv < range B
      PRINT_LEVEL1 ("DTR Not Need: Port %d CTRS < Range B\n", pInst->RpIndex);
      IpPcieSetDtrStat(pInst, IpPcieDtrNotNeed);
    }
  }

  if (!CompletionTimeoutDefault) {
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCTL2_PCIE_CFG_REG, Dctl2.Data, IpWrRegFlagSize16Bits);
  }

  SipConfigureCoalescing (pInst);

  ///
  /// For Root Port Slots Numbering on the CRBs.
  ///
  Slcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SLCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Slcap.Bits.slv = 0;
  Slcap.Bits.sls = 0;
  Slcap.Bits.psn = 0;
  ///
  /// PCH BIOS Spec section 8.8.2.1
  /// Note: If Hot Plug is supported, then write a 1 to the Hot Plug Capable (bit6) and Hot Plug
  /// Surprise (bit5) in the Slot Capabilities register, PCIE RP PCI offset 54h. Otherwise,
  /// write 0 to the bits PCIe Hot Plug SCI Enable
  ///
  Slcap.Bits.hpc = 0;
  Slcap.Bits.hps = 0;
  if (pInst->PcieRpCommonConfig.HotPlug) {
    Slcap.Bits.hpc = 1;
    Slcap.Bits.hps = 1;
  }
  ///
  /// Get the width from LCAP
  /// Slot Type  X1  X2/X4/X8/X16
  /// Default     10W   25W
  /// The slot power consumption and allocation is platform specific. Please refer to the
  /// "PCI Express* Card Electromechanical (CEM) Spec" for details.
  ///
  if (pInst->PcieRpCommonConfig.SlotPowerLimitValue != 0) {
    Slcap.Bits.slv = pInst->PcieRpCommonConfig.SlotPowerLimitValue;
    Slcap.Bits.sls = pInst->PcieRpCommonConfig.SlotPowerLimitScale;
  } else {
    if (SipGetMaxLinkWidth (pInst) == 1) {
      Slcap.Bits.slv = 100;
      Slcap.Bits.sls = 1;
    } else if (SipGetMaxLinkWidth (pInst) >= 2) {
      Slcap.Bits.slv = 250;
      Slcap.Bits.sls = 1;
    }
  }

  ///
  /// Program Slot Capabilities register 0x0054
  /// PCH BIOS Spec section 8.2.4
  /// Initialize Physical Slot Number for Root Ports
  ///
  Slcap.Bits.psn = pInst->PcieRpCommonConfig.PhysicalSlotNumber;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, SLCAP_PCIE_CFG_REG, Slcap.Data, IpWrRegFlagSize32Bits);

  ///
  /// Physical Layer 32.0 GT/s Capability
  ///
  if((pInst->SipPhyVersion == IP_HSPHY) || (pInst->SipPhyVersion == IP_SE32)){
    G5cap.Data = (UINT32) IpWrRegRead(pInst->RegCntxt_Cfg_Pri, G5CAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    G5ctl.Data = (UINT32) IpWrRegRead(pInst->RegCntxt_Cfg_Pri, G5CTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    G5cap.Bits.eqbypsup = 1;
    G5ctl.Bits.eqbypdis = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, G5CAP_PCIE_CFG_REG, G5cap.Data, IpWrRegFlagSize32Bits);
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, G5CTL_PCIE_CFG_REG, G5ctl.Data, IpWrRegFlagSize32Bits);
  }

  Dpccapr.Data = (UINT16)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DPCCAPR_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Dpccapr.Bits.rpefdpc = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DPCCAPR_PCIE_CFG_REG, Dpccapr.Data, IpWrRegFlagSize16Bits);

  SipInitCapabilityList (pInst);

  //
  // All actions involving LinkDisable must finish before anything is programmed on endpoint side
  // because LinkDisable resets endpoint
  //

  ///
  /// Perform equalization for Gen3 capable ports
  ///
  if (RootPortSpeed >= 3) {
    //
    // Link EQ has to be performed with CCC set if supported.
    //
    //IpPcieSipEnableCommonClock (pInst);
    SipLinkEqualize (pInst,
                     &pInst->PrivateConfig.Gen3EqSettings,
                     &pInst->PrivateConfig.Gen4EqSettings,
                     &pInst->PrivateConfig.Gen5EqSettings,
                     RootPortSpeed);
  }
  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Step 29 If Transmitter Half Swing is enabled, program the following sequence
  /// a. Ensure that the link is in L0.
  /// b. Program the Link Disable bit (0x50[4]) to 1b.
  /// c. Program the Analog PHY Transmitter Voltage Swing bit (0xE8[13]) to set the transmitter swing to half/full swing
  /// d. Program the Link Disable bit (0x50[4]) to 0b.
  /// BIOS can only enable this on SKU where GEN3 capability is disabled on that port

  if (RootPortSpeed < V_LCAP_MLS_GEN3 && pInst->PcieRpCommonConfig.TransmitterHalfSwing) {
    /// PCI Express Additional Link Control register 0x0050
    Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Lctl.Bits.ld = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
    while (SipIsLinkActive (pInst)) {
      // wait until link becomes inactive before changing swing
    }
    /// Program Power Control register 0x00E8
    PwrCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PWRCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PwrCtl.Bits.txswing = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PWRCTL_PCIE_CFG_REG, PwrCtl.Data, IpWrRegFlagSize32Bits);

    /// PCI Express Additional Link Control register 0x0050
    Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Lctl.Bits.ld = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);
  }

  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Set "Poisoned TLP Non-Fatal Advisory Error Enable", Dxx:Fn + D4h[12] to 1
  /// Set "L1 Substate Exit SCI Enable (L1SSESE)", Dxx:Fn + D4[30] to 1
  /// Set "Disable PLL Early Wake on L1 Substate Exit (DISPLLEWL1SE)", Dxx:Fn + D4[16] to 1
  ///
  Mpc2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Mpc2.Bits.ptnfae  = 1;
  Mpc2.Bits.l1ssese = 0;
  Mpc2.Bits.displlewl1se = 0;
  if (pInst->SipPhyVersion == IP_HSPHY) {
    Mpc2.Bits.orce = 1;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC2_PCIE_CFG_REG, Mpc2.Data, IpWrRegFlagSize32Bits);

  //
  // L1LOW LTR threshold latency value
  //
  PciePmeCtl.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PCIEPMECTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PciePmeCtl.Bits.l1ltrtlsv = 2;
  PciePmeCtl.Bits.l1ltrtlv = V_PCIEPMECTL_L1LTRTLV;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PCIEPMECTL_PCIE_CFG_REG, PciePmeCtl.Data, IpWrRegFlagSize32Bits);
  ///
  /// Additional configurations
  ///
  ///
  /// Configure Error Reporting policy in the Device Control Register
  ///
  Dctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, DCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Dctl.Bits.ure = 0;
  Dctl.Bits.fee = 0;
  Dctl.Bits.nfe = 0;
  Dctl.Bits.cee = 0;

  if (pInst->PcieRpCommonConfig.UnsupportedRequestReport) {
    Dctl.Bits.ure = 1;
  }

  if (pInst->PcieRpCommonConfig.FatalErrorReport) {
    Dctl.Bits.fee = 1;
  }

  if (pInst->PcieRpCommonConfig.NoFatalErrorReport) {
    Dctl.Bits.nfe = 1;
  }

  if (pInst->PcieRpCommonConfig.CorrectableErrorReport) {
    Dctl.Bits.cee = 1;
  }

  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, DCTL_PCIE_CFG_REG, Dctl.Data, IpWrRegFlagSize16Bits);
  ///
  /// Configure Interrupt / Error reporting in R_PCIE_CFG_RCTL
  ///
  Rctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Rctl.Bits.sfe = 0;
  Rctl.Bits.sne = 0;
  Rctl.Bits.sce = 0;

  if (pInst->PcieRpCommonConfig.SystemErrorOnFatalError) {
    Rctl.Bits.sfe = 1;
  }

  if (pInst->PcieRpCommonConfig.SystemErrorOnNonFatalError) {
    Rctl.Bits.sne = 1;
  }

  if (pInst->PcieRpCommonConfig.SystemErrorOnCorrectableError) {
    Rctl.Bits.sce = 1;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RCTL_PCIE_CFG_REG, Rctl.Data, IpWrRegFlagSize16Bits);
  ///
  /// Root PCI-E Powermanagement SCI Enable
  ///
  if (pInst->PcieRpCommonConfig.PmSci) {
    ///
    /// PCH BIOS Spec section 8.7.3 BIOS Enabling of Intel PCH PCI Express* PME SCI Generation
    /// Step 1
    /// Make sure that PME Interrupt Enable bit, Dxx:Fn:Reg 5Ch[3] is cleared
    ///
    Rctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Rctl.Bits.pie = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RCTL_PCIE_CFG_REG, Rctl.Data, IpWrRegFlagSize16Bits);

    ///
    /// Step 2
    /// Program Misc Port Config (MPC) register at PCI config space offset
    /// D8h as follows:
    /// Set Power Management SCI Enable bit Dxx:Fn:Reg D8h[31]
    /// Clear Power Management SMI Enable bit, Dxx:Fn:Reg D8h[0]
    /// Use Byte Access to avoid RWO bit
    ///
    Mpc.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Mpc.Bits.pmme = 0;
    Mpc.Bits.pmce = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, Mpc.Data, IpWrRegFlagSize32Bits);
  }
  if (pInst->PcieRpCommonConfig.HotPlug) {
    ///
    /// PCH BIOS Spec section 8.8.2.1
    /// Step 1
    /// Clear following status bits, by writing 1b to them, in the Slot
    /// Status register at offset 1Ah of PCI Express Capability structure:
    /// Presence Detect Changed (bit3)
    ///
    Slsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SLSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Slsts.Bits.pdc = 0;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, SLSTS_PCIE_CFG_REG, Slsts.Data, IpWrRegFlagSize16Bits);

    ///
    /// Step 2
    /// Program the following bits in Slot Control register at offset 18h
    /// of PCI Express* Capability structure:
    /// Presence Detect Changed Enable (bit3) = 1b
    /// Hot Plug Interrupt Enable (bit5) = 0b
    ///
    Slstl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, SLCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    Slstl.Bits.hpe = 0;
    Slstl.Bits.pde = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, SLCTL_PCIE_CFG_REG, Slstl.Data, IpWrRegFlagSize16Bits);
    ///
    /// Step 3
    /// Program Misc Port Config (MPC) register at PCI config space offset
    /// D8h as follows:
    /// Hot Plug SCI Enable (HPCE, bit30) = 0b
    /// Use byte access to avoid premature locking BIT23, SRL
    ///
    Mpc.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Mpc.Bits.hpce = 0;
    Mpc.Bits.hpme = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, Mpc.Data, IpWrRegFlagSize32Bits);
    ///
    /// PCH BIOS Spec section 8.9
    /// BIOS should mask the reporting of Completion timeout (CT) errors by setting
    /// the uncorrectable Error Mask register PCIE RP PCI offset 108h[14].
    ///
    Uem.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, UEM_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Uem.Bits.ct = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, UEM_PCIE_CFG_REG, Uem.Data, IpWrRegFlagSize32Bits);

  }
  //
  // Set UEM.PTLPEBM 108h[26] to 1 for Gen5 Capable controller
  //
  Uem.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, UEM_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  if ((pInst->SipVersion > IpPcieSip17_4) && (((pInst->SipPhyVersion == IP_HSPHY) || (pInst->SipPhyVersion == IP_SE32)))) {
    Uem.Bits.ptlpebm = 1;
  } else {
    Uem.Bits.ptlpebm = 0;
  }
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, UEM_PCIE_CFG_REG, Uem.Data, IpWrRegFlagSize32Bits);

  ///
  /// PCH BIOS Spec Section 8.10 PCI Bus Emulation & Port80 Decode Support
  /// The I/O cycles within the 80h-8Fh range can be explicitly claimed
  /// by the PCIe RP by setting MPC.P8XDE, PCI offset D8h[26] = 1 (using byte access)
  /// BIOS must also configure the corresponding DMI registers GCS.RPR and GCS.RPRDID
  /// to enable DMI to forward the Port8x cycles to the corresponding PCIe RP
  ///
  if (pInst->PrivateConfig.EnablePort80Decode) {
    Mpc.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Mpc.Bits.p8xde = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, Mpc.Data, IpWrRegFlagSize32Bits);
  }

  //
  // Initialize R/WO Registers that described in PCH BIOS Spec
  //
  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Set Common Clock Exit Latency,      Dxx:Fn + D8h[17:15] = 4h (SipVersion < SIP17)
  /// Set Common Clock Exit Latency,      Dxx:Fn + D8h[17:15] = 6h (SipVersion > SIP17)
  /// Set Non-common Clock Exit Latency,  Dxx:Fn + D8h[20:18] = 7h
  ///
  Mpc.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Mpc.Bits.ucel = 0x7;
  Mpc.Bits.ccel = 0x6;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MPC_PCIE_CFG_REG, Mpc.Data, IpWrRegFlagSize32Bits);
  //
  // Set Dxx:Fn +0x04[8]=1
  // Set Dxx:Fn +0x3E[1]=1
  //
  Cmd.Data = (UINT16)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CMD_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Cmd.Bits.see = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CMD_PCIE_CFG_REG, Cmd.Data, IpWrRegFlagSize16Bits);

  Bctrl.Data = (UINT16)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, BCTRL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Bctrl.Bits.se = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, BCTRL_PCIE_CFG_REG, Bctrl.Data, IpWrRegFlagSize16Bits);

  /// Program Strap And Fuse Configuration register 0x00FC
  StrpFuseCfg.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Sb, STRPFUSECFG_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  StrpFuseCfg.Bits.pxip = pInst->PrivateConfig.InterruptPin;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, STRPFUSECFG_PCIE_CFG_REG, StrpFuseCfg.Data, IpWrRegFlagSize32Bits);

  if (pInst->PcieRpCommonConfig.LinkDownGpios) {
    /// Set Gpio Assertion on Link Down
    Gdr.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, GDR_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    Gdr.Bits.gpioald = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, GDR_PCIE_CFG_REG, Gdr.Data, IpWrRegFlagSize32Bits);
  }
  ///
  /// Dynamic Link Throttling
  ///
  Tnpt.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, TNPT_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Tnpt.Bits.tp = V_TNPT_TP_5_MS;
  Tnpt.Bits.drxlte = 0;
  Tnpt.Bits.dtxlte = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, TNPT_PCIE_CFG_REG, Tnpt.Data, IpWrRegFlagSize32Bits);

  SipConfigureRetimerSupport (pInst);

  //
  // Program Feature Control register 0x1300
  // Set Completion Timer Timeout Policy (CRSPSEL) to 0
  //
  Fctl.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, FCTL_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Fctl.Bits.crspsel = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, FCTL_PCIE_MEM_RCRB_REG, Fctl.Data, IpWrRegFlagSize32Bits);

  ///
  /// Program register AECR2G3 - BIT9 to 0x1
  ///
  Aecr2g3.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, AECR2G3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Aecr2g3.Bits.rsvd_rw2 = 0x2;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, AECR2G3_PCIE_CFG_REG, Aecr2g3.Data, IpWrRegFlagSize32Bits);

  ///
  /// Set Error Injection Disable (EINJDIS) to 1
  ///
  EinjCtl.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EINJCTL_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EinjCtl.Bits.einjdis = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EINJCTL_PCIE_CFG_REG, EinjCtl.Data, IpWrRegFlagSize32Bits);
  ///
  /// Program R_PCIE_RXMC1[C90]
  /// Set C90[14:13] TMSLOP to 0x1
  /// Set C90[12:11] VMSLOP to 0x1
  /// Set C90[8:4] MMNOLS to 0x0
  ///
  Rxmc1.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, RXMC1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Rxmc1.Bits.mmnols = V_RXMC1_MMNOLS;
  Rxmc1.Bits.vmslop = V_RXMC1_VMSLOP;
  Rxmc1.Bits.tmslop = V_RXMC1_TMSLOP;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, RXMC1_PCIE_CFG_REG, Rxmc1.Data, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1("RXMC1 : %x\n", Rxmc1.Data);

  ///
  /// Set CE4[0] MARGINDRISW to 0x0
  ///
  Pl16Mpcpsb01.Data = (UINT16)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, PL16MPCPSB01_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Pl16Mpcpsb01.Bits.margindrisw = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, PL16MPCPSB01_PCIE_CFG_REG, Pl16Mpcpsb01.Data, IpWrRegFlagSize16Bits);

  //
  // Program MAC PHY Capability Configurations register 0x15a8
  //
  MPhyCapCfg.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, MPHYCAPCFG_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  MPhyCapCfg.Bits.mlsosrssvcc = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, MPHYCAPCFG_PCIE_MEM_RCRB_REG, MPhyCapCfg.Data, IpWrRegFlagSize32Bits);

  //
  // Program TX Master Decode 1 register 0x17a0
  //
  TxmDec1.Data = (UINT8) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, TXMDEC1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  TxmDec1.Bits.cridbncd = 0;
  TxmDec1.Bits.mrbidbncd = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, TXMDEC1_PCIE_MEM_RCRB_REG, TxmDec1.Data, IpWrRegFlagSize32Bits);

  Iorccp1.Data = (UINT32)IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, IORCCP1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Iorccp1.Bits.disorcl12rec = 0x1;
  Iorccp1.Bits.disorcrodi   = 0x1;
  Iorccp1.Bits.g1orrrxecc   = 0x1;
  Iorccp1.Bits.g2orrrxecc   = 0x1;
  Iorccp1.Bits.g3orrrxecc   = 0x1;
  Iorccp1.Bits.g4orrrxecc   = 0x1;
  Iorccp1.Bits.g5orrrxecc   = 0x1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, IORCCP1_PCIE_MEM_RCRB_REG, Iorccp1.Data, IpWrRegFlagSize32Bits);
    SipSetSecureRegisterLock (pInst);
  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
}