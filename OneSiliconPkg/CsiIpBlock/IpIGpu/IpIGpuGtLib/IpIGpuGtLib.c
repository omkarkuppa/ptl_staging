/**@file
  IP IGpu GT library.

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

#include <IpWrapper.h>
#include <IpWrapper64Bit.h>
#include <IpIGpu.h>
#include <IpIGpuGtReg.h>
#include <IpIGpuDefines.h>

BOOT_SCRIPT_REGISTER_IGPU_SETTING  mGtXe2ClockGatingRegisters[] = {
  //
  // BaseAddr  Offset  AndMask  OrMask
  //
  //
  // Unslice
  //
  { UNSLCGCTL9430_3D_GT_REG, 0x0 },
  { UNSLCGCTL9434_3D_GT_REG, 0x0 },
  { UNSLCGCTL9438_3D_GT_REG, 0x0 },
  { UNSLCGCTL9440_3D_GT_REG, 0x0 },
  { UNSLCGCTL9444_3D_GT_REG, 0x0 },
  //
  // Slice
  //
  { SCCGCTL94D0_3D_GT_REG,   0x0 },
  { SCCGCTL94D4_3D_GT_REG,   0x0 },
  { SCCGCTL94D8_3D_GT_REG,   0x0 },
  { SCCGCTL94DC_3D_GT_REG,   0x0 },
  { SCCGCTL94E0_3D_GT_REG,   0x0 },
  { SCCGCTL94E4_3D_GT_REG,   0x0 },
  //
  // Subslice
  //
  { SSMCGCTL9520_3D_GT_REG,  0x0 },
  { SSMCGCTL9524_3D_GT_REG,  0x0 }
};

/**
  "Poll Status" for GT/Media Readiness
  @param[in]  pInst            The instance
  @param[in]  Offset          - MMIO Offset
  @param[in]  Mask            - Mask
  @param[in]  Result          - Value to wait for
  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuPollReady (
  IP_IGPU_INST  *pInst,
  UINT32        Offset,
  UINT32        Mask,
  UINT32        Result
  );

/**
  Initialize GT of SystemAgent.

  @param[in]  pInst             A pointer to the IP instance to be used.

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuGtInit (
  IP_IGPU_INST  *pInst
  )
{
  RC6CTXBASE_RPM_GCD_0_GT_STRUCT             GcdRc6CtxLowBase;
  RC6CTXBASE_RPM_GCD_1_GT_STRUCT             GcdRc6CtxHighBase;
  GCP_CLKDIV_CONTROL1_3D_GT_STRUCT           Gcpclkctrl1div;
  RC1P_FREQ_VALUE_RPM_GCD_GT_STRUCT          Rc1pfreq;
  RC1P_BUSY_THRESH_RPM_GCD_GT_STRUCT         Rc1pBusyThresh;
  RC1P_FREQ_CHNG_EN_RPM_GCD_GT_STRUCT        Rc1pFreqChngEn;
  CONFIG0_RPM_GCD_GT_STRUCT                  Config0Rpm;
  MISC_CTRL0_GPM_GRP_GCD_GT_STRUCT           Miscctrl;
  ECO_BUSRST_GPM_GRP_GCD_GT_STRUCT           Ecobusrst;
  GCP_CLKDIV_CONTROL0_3D_GT_STRUCT           Gcpclkctrl0div;
  CCCHKNREG1_I_GCD_GT_STRUCT                 CcChickenGtStruct;
  UINT32                                     RegOffset;
  UINT8                                      LoopCounter;
  UINT32                                     Value;
  FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_STRUCT     FlatCcsGamReg;
  FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_STRUCT  FlatCssLowBase;
  FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_STRUCT  FlatCcsHighBase;
  CCLOCK_I_GCD_GT_STRUCT                     CClockGt;
  ECOCHK_GAMCTRL_3D_GT_STRUCT                EcochkGamCtrl3d;
  L3BANKLOCK_LBCF_GFX_GT_STRUCT              L3lockGt;
  MIRROR_GMD_ID_RPM_GCD_GT_STRUCT            GmdIdStruct;
  GAMCNTRL_CTRL_GAMCTRL_3D_GT_STRUCT         GamCntrl;
  GAMCTRLLOCK_GAMCTRL_3D_GT_STRUCT           GamCtrlLock;
  BOOLEAN                                    IsFlatCcsCompressionEnable;
  GMD_ID_STRUCT                              GmdId;

  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (IpIGpuGetGttMmAdr (pInst) == 0) {
    PRINT_WARNING ("Invalid parameters for GtInit\n");
    return IpCsiStsError;
  }

  IsFlatCcsCompressionEnable = FALSE;

  GmdIdStruct.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, MIRROR_GMD_ID_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("GT GMD ID = 0x%x \n", GmdIdStruct.Data);

  ///
  /// 1aa. Enable all GTI-Uncore clock gating
  ///
  IpWrRegWrite (pInst->MmioAccess, INFCGCTL9560_3D_GT_REG, 0x0, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, NULL, INFCGCTL9560_3D_GT_REG, 0x0, (UINT32)IpWrRegRead (pInst->MmioAccess, INFCGCTL9560_3D_GT_REG, IpWrRegFlagSize32Bits));

  ///
  /// 1c. GT RC6CTXBASE
  /// Get BdsmBase in MB
  ///
  GcdRc6CtxLowBase.Data            = (((UINT32)pInst->IGpuPrivateConfig.BdsmBase & ~(BIT0)) + (pInst->IGpuPrivateConfig.DsmSize * SIZE_1MB) - SIZE_64KB);
  GcdRc6CtxLowBase.Bits.rc6memlock = TRUE;
  GcdRc6CtxHighBase.Data           = 0;

  IpWrRegWrite (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_1_GT_REG, GcdRc6CtxHighBase.Data, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, "RC6CTXBASE HIGH", RC6CTXBASE_RPM_GCD_1_GT_REG, GcdRc6CtxHighBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_1_GT_REG, IpWrRegFlagSize32Bits));
  IpWrRegWrite (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_0_GT_REG, GcdRc6CtxLowBase.Data, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, "RC6CTXBASE LOW", RC6CTXBASE_RPM_GCD_0_GT_REG, GcdRc6CtxLowBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_0_GT_REG, IpWrRegFlagSize32Bits));

  if (pInst->IGpuPrivateConfig.IsGtPresent == FALSE) {
    PRINT_LEVEL1 ("Exiting GT Init since GT is not present in the sku.\n");
    return IpCsiStsSuccess;
  }

  ///
  /// 1d. Readback GT RC6CTXBASE and check.
  ///
  if (((UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_0_GT_REG, IpWrRegFlagSize32Bits) != GcdRc6CtxLowBase.Data) || \
      ((UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_1_GT_REG, IpWrRegFlagSize32Bits) != GcdRc6CtxHighBase.Data))
  {
    PRINT_ERROR ("Not able to configure GT Rc6CtxBase due to some other agent is configuring and locking it before BIOS does.\n");
    return IpCsiStsError;
  }

  if (pInst->IGpuConfig.PeiGtConfig.ConfigureGT) {
    PRINT_LEVEL1 ("Starting Register programming for GT Init.\n");
    ///
    /// 1g. Set Lock Bit for GCP CLOCK DIV CONTROL Registers.
    ///

    Gcpclkctrl0div.Data                = (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL0_3D_GT_REG, IpWrRegFlagSize32Bits);
    Gcpclkctrl0div.Bits.clkdiv_lockbit = TRUE;
    IpWrRegWrite (pInst->MmioAccess, GCP_CLKDIV_CONTROL0_3D_GT_REG, Gcpclkctrl0div.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GCP_CLKDIV_CONTROL0_3D_GT_REG, Gcpclkctrl0div.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL0_3D_GT_REG, IpWrRegFlagSize32Bits));

    Gcpclkctrl1div.Data                = (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL1_3D_GT_REG, IpWrRegFlagSize32Bits);
    Gcpclkctrl1div.Bits.clkdiv_lockbit = TRUE;
    IpWrRegWrite (pInst->MmioAccess, GCP_CLKDIV_CONTROL1_3D_GT_REG, Gcpclkctrl1div.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GCP_CLKDIV_CONTROL1_3D_GT_REG, Gcpclkctrl1div.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL1_3D_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 1h. Set and lock RC1p frequency to 16.667Mhz.
    ///
    Rc1pfreq.Data               = (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_VALUE_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits);
    Rc1pfreq.Bits.rc1p_req_freq = 0x1;
    Rc1pfreq.Bits.lock          = TRUE;
    IpWrRegWrite (pInst->MmioAccess, RC1P_FREQ_VALUE_RPM_GCD_GT_REG, Rc1pfreq.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RC1P_FREQ_VALUE_RPM_GCD_GT_REG, Rc1pfreq.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_VALUE_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 1i. Set RC1p hysteresis to 4096 SBclocks, 10us.
    ///
    Rc1pBusyThresh.Data                      = (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_BUSY_THRESH_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits);
    Rc1pBusyThresh.Bits.rc1p_busy_hyst_count = 0x1000;
    IpWrRegWrite (pInst->MmioAccess, RC1P_BUSY_THRESH_RPM_GCD_GT_REG, Rc1pBusyThresh.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RC1P_BUSY_THRESH_RPM_GCD_GT_REG, Rc1pBusyThresh.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_BUSY_THRESH_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 1j. Enable or disable RC1p.
    ///
    Rc1pFreqChngEn.Data             = (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_CHNG_EN_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits);
    Rc1pFreqChngEn.Bits.rc1p_enable = FALSE;
    if (pInst->IGpuConfig.PeiGtConfig.RC1pGtFreqEnable == 1) {
      Rc1pFreqChngEn.Bits.rc1p_enable = TRUE;
    }

    IpWrRegWrite (pInst->MmioAccess, RC1P_FREQ_CHNG_EN_RPM_GCD_GT_REG, Rc1pFreqChngEn.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RC1P_FREQ_CHNG_EN_RPM_GCD_GT_REG, Rc1pFreqChngEn.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_CHNG_EN_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 1l. Programming Crystal Clock.
    /// Programm [2:1] = 11b if [5:3] is 001 (indicate ref clock is 19.2Mhz)
    /// Programm [2:1] = 10b if [5:3] is 000/010/011 (indicate ref clock is 24Mhz/38.4MHz/25MHz)
    ///
    Config0Rpm.Data      = (UINT32)IpWrRegRead (pInst->MmioAccess, CONFIG0_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits);
    Config0Rpm.Bits.lock = TRUE;
    if (Config0Rpm.Bits.crysclock == 1) {
      Config0Rpm.Bits.ctc_shift_param = 0X3;
    } else if ((Config0Rpm.Bits.crysclock == 0) || (Config0Rpm.Bits.crysclock == 2) || (Config0Rpm.Bits.crysclock == 3)) {
      Config0Rpm.Bits.ctc_shift_param = 0x2;
    }

    IpWrRegWrite (pInst->MmioAccess, CONFIG0_RPM_GCD_GT_REG, Config0Rpm.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, CONFIG0_RPM_GCD_GT_REG, Config0Rpm.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, CONFIG0_RPM_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 2a. Enable Force Wake
    ///
    IpIGpuForceWakeupGt (pInst);

    ///
    /// 3. Disable 32B Request
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      CcChickenGtStruct.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, CCCHKNREG1_I_GCD_GT_REG, IpWrRegFlagSize32Bits);
      if (pInst->IGpuPrivateConfig.IsIbeccEnabled == TRUE) {
        CcChickenGtStruct.Bits._32bwrdisable = TRUE;
      } else {
        CcChickenGtStruct.Bits._32bwrdisable = FALSE;
      }
      CcChickenGtStruct.Bits.encompperffix = FALSE;
      CcChickenGtStruct.Bits.l3cmpctrl     = TRUE;
      GmdId.Data = GmdIdStruct.Data;
      if (GmdId.Bits.GMDArchitecture == 30) {
        CcChickenGtStruct.Bits.l2dyncompctrl = FALSE;
      }
      if (GmdIdStruct.Data == GMD_GT_30_0) {
        CcChickenGtStruct.Data |= BIT30;
      }
      IpWrRegWrite (pInst->MmioAccess, CCCHKNREG1_I_GCD_GT_REG, CcChickenGtStruct.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, CCCHKNREG1_I_GCD_GT_REG, CcChickenGtStruct.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, CCCHKNREG1_I_GCD_GT_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 4. Program GT GUC WOPCM OFFSET and SIZE
    ///
    PRINT_LEVEL1 ("Programming GT GUC WOPCM Offset and Size\n");

    if (pInst->XeVersion >= IpIGpuXe2) {
      IpWrRegWrite (pInst->MmioAccess, DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_REG, 0x600002, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_REG, 0x600002, (UINT32)IpWrRegRead (pInst->MmioAccess, DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_REG, IpWrRegFlagSize32Bits));
    }

    IpWrRegWrite (pInst->MmioAccess, GUC_WOPCM_SIZE_GUC_REG_GT_REG, 0xD0000, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GUC_WOPCM_SIZE_GUC_REG_GT_REG, 0xD0000, (UINT32)IpWrRegRead (pInst->MmioAccess, GUC_WOPCM_SIZE_GUC_REG_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5a. GPM Control
    ///
    Miscctrl.Data = 0x80200000;
    IpWrRegWrite (pInst->MmioAccess, MISC_CTRL0_GPM_GRP_GCD_GT_REG, Miscctrl.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, MISC_CTRL0_GPM_GRP_GCD_GT_REG, Miscctrl.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, MISC_CTRL0_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5b. ECOBUS Disable Fence writes
    ///
    Ecobusrst.Data                            = (UINT32)IpWrRegRead (pInst->MmioAccess, ECO_BUSRST_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits);
    Ecobusrst.Bits.gacfg_fence_c6status_write = TRUE;
    Ecobusrst.Bits.cpd_block_mgsr_disable     = TRUE;
    Ecobusrst.Bits.cpd_bypass_go_msg_strms    = TRUE;
    Ecobusrst.Bits.lock                       = TRUE;
    IpWrRegWrite (pInst->MmioAccess, ECO_BUSRST_GPM_GRP_GCD_GT_REG, Ecobusrst.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, ECO_BUSRST_GPM_GRP_GCD_GT_REG, Ecobusrst.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, ECO_BUSRST_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5c. Enable Unit Level Clock Gating
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      for (LoopCounter = 0; LoopCounter < ARRAY_SIZE (mGtXe2ClockGatingRegisters); ++LoopCounter) {
        RegOffset = mGtXe2ClockGatingRegisters[LoopCounter].Offset;
        Value     = mGtXe2ClockGatingRegisters[LoopCounter].Value;
        IpWrRegWrite (pInst->MmioAccess, RegOffset, Value, IpWrRegFlagSize32Bits);
        IpIGpuPrintRegData (pInst, NULL, RegOffset, Value, (UINT32)IpWrRegRead (pInst->MmioAccess, RegOffset, IpWrRegFlagSize32Bits));
      }
    }

    if (pInst->IGpuPreMemConfig.MemoryBandwidthCompression == 1) {
      if (pInst->XeVersion >= IpIGpuXe2) {
        IsFlatCcsCompressionEnable = TRUE;
      }
    }

    ///
    /// 5e. Enable Flat CCS Base address register
    ///
    FlatCcsGamReg.Data               = (UINT32)IpWrRegRead (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_REG, IpWrRegFlagSize32Bits);
    FlatCcsGamReg.Bits.flat_ccs_lock = TRUE;
    if (IsFlatCcsCompressionEnable) {
      FlatCcsGamReg.Bits.flat_ccs_en = TRUE;
    }

    IpWrRegWrite (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_REG, FlatCcsGamReg.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_REG, FlatCcsGamReg.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5f. Program Flat CCS Base address.
    ///
    if (IsFlatCcsCompressionEnable) {
      FlatCssLowBase.Data                                = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_REG, IpWrRegFlagSize32Bits);
      FlatCssLowBase.Bits.ccsenable                      = TRUE;
      FlatCssLowBase.Bits.hwviewofflatccsbaseatcclevello = (UINT32)IpWrRShiftU64 (pInst->IGpuPrivateConfig.FlatCcsBaseAddr, FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_HWVIEWOFFLATCCSBASEATCCLEVELLO_LSB);
      IpWrRegWrite (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_REG, FlatCssLowBase.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, "FlatCssLowBase", FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_REG, FlatCssLowBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_REG, IpWrRegFlagSize32Bits));

      FlatCcsHighBase.Data                       = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_REG, IpWrRegFlagSize32Bits);
      FlatCcsHighBase.Bits.swviewofflatccsbasehi = (UINT32)IpWrRShiftU64 (pInst->IGpuPrivateConfig.FlatCcsBaseAddr, 32);
      FlatCcsHighBase.Bits.flatccsize            = (pInst->IGpuPrivateConfig.FlatCcsMemSize / 2);
      IpWrRegWrite (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_REG, FlatCcsHighBase.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, "FlatCcsHighBase", FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_REG, FlatCcsHighBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_REG, IpWrRegFlagSize32Bits));
    }

    //
    // 5g. Program CCLOCK register
    //
    if (pInst->XeVersion >= IpIGpuXe2) {
      CClockGt.Data                          = (UINT32)IpWrRegRead (pInst->MmioAccess, CCLOCK_I_GCD_GT_REG, IpWrRegFlagSize32Bits);
      CClockGt.Bits.ccchknreg1_lock          = TRUE;
      CClockGt.Bits.flatccsbaseandrange_lock = TRUE;
      CClockGt.Bits.cclock_lock              = TRUE;
      IpWrRegWrite (pInst->MmioAccess, CCLOCK_I_GCD_GT_REG, CClockGt.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, CCLOCK_I_GCD_GT_REG, CClockGt.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, CCLOCK_I_GCD_GT_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 6. Set all *CS/GuC Idle Max Count (hysteresis)
    /// 7. Enable Idle Messages from all *CS
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      IpWrRegWrite (pInst->MmioAccess, GUC_PM_MAXCNT_GUC_REG_GT_REG, 0xA, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, GUC_PM_MAXCNT_GUC_REG_GT_REG, 0xA, (UINT32)IpWrRegRead (pInst->MmioAccess, GUC_PM_MAXCNT_GUC_REG_GT_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 8a. Program GT Normal Frequency Request
    ///
    IpWrRegWrite (pInst->MmioAccess, RP_FREQ_NORMAL_GPM_GRP_GCD_GT_REG, 0x06000000, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RP_FREQ_NORMAL_GPM_GRP_GCD_GT_REG, 0x06000000, (UINT32)IpWrRegRead (pInst->MmioAccess, RP_FREQ_NORMAL_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    /// 8b. RP Control.
    ///
    IpWrRegWrite (pInst->MmioAccess, RP_CTRL_GPM_GRP_GCD_GT_REG, 0x00000400, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RP_CTRL_GPM_GRP_GCD_GT_REG, 0x00000400, (UINT32)IpWrRegRead (pInst->MmioAccess, RP_CTRL_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits));

    //
    // 9a. TLB register Programming
    //
    GamCntrl.Data                 = (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCNTRL_CTRL_GAMCTRL_3D_GT_REG, IpWrRegFlagSize32Bits);
    GamCntrl.Bits.skip_cflush_ack = FALSE;
    IpWrRegWrite (pInst->MmioAccess, GAMCNTRL_CTRL_GAMCTRL_3D_GT_REG, GamCntrl.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GAMCNTRL_CTRL_GAMCTRL_3D_GT_REG, GamCntrl.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCNTRL_CTRL_GAMCTRL_3D_GT_REG, IpWrRegFlagSize32Bits));

    //
    // 9b. Disable ATS replay mechanism
    //
    EcochkGamCtrl3d.Data                   = (UINT32)IpWrRegRead (pInst->MmioAccess, ECOCHK_GAMCTRL_3D_GT_REG, IpWrRegFlagSize32Bits);
    EcochkGamCtrl3d.Bits.lock_security_chk = TRUE;
    EcochkGamCtrl3d.Bits.en_security_chk   = TRUE;
    EcochkGamCtrl3d.Bits.dis_ats_replay    = TRUE;
    IpWrRegWrite (pInst->MmioAccess, ECOCHK_GAMCTRL_3D_GT_REG, EcochkGamCtrl3d.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, ECOCHK_GAMCTRL_3D_GT_REG, EcochkGamCtrl3d.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, ECOCHK_GAMCTRL_3D_GT_REG, IpWrRegFlagSize32Bits));

    //
    // 9c. Lock GAM CTRL Register
    //
    GamCtrlLock.Data               = (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCTRLLOCK_GAMCTRL_3D_GT_REG, IpWrRegFlagSize32Bits);
    GamCtrlLock.Bits.lock_reg_cf08 = TRUE;
    GamCtrlLock.Bits.lock_reg_cf54 = TRUE;
    GamCtrlLock.Bits.gamctrllock_l = TRUE;
    IpWrRegWrite (pInst->MmioAccess, GAMCTRLLOCK_GAMCTRL_3D_GT_REG, GamCtrlLock.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GAMCTRLLOCK_GAMCTRL_3D_GT_REG, GamCtrlLock.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCTRLLOCK_GAMCTRL_3D_GT_REG, IpWrRegFlagSize32Bits));

    //
    // Program L3BANKLOCK register for GT.
    //
    if (pInst->XeVersion >= IpIGpuXe3) {
      L3lockGt.Data                     = (UINT32)IpWrRegRead (pInst->MmioAccess, L3BANKLOCK_LBCF_GFX_GT_REG, IpWrRegFlagSize32Bits);
      L3lockGt.Bits.lbcflockmsgreg_lock = TRUE;
      IpWrRegWrite (pInst->MmioAccess, L3BANKLOCK_LBCF_GFX_GT_REG, L3lockGt.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, L3BANKLOCK_LBCF_GFX_GT_REG, L3lockGt.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, L3BANKLOCK_LBCF_GFX_GT_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 10. Enabling to enter RC6 state in idle mode.
    ///
    IpIGpuIdleGt (pInst);
  }

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return IpCsiStsSuccess;
}

/**
  Enable Force Wake Up for GT.

  @param[in]  pInst             A pointer to the IP instance to be used.

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuForceWakeupGt (
  IP_IGPU_INST  *pInst
  )
{
  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    PRINT_WARNING ("Invalid parameters for %s\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->IGpuPrivateConfig.IsGtPresent == FALSE) {
    PRINT_LEVEL1 ("Exiting since GT is not present in the sku.\n");
    return IpCsiStsSuccess;
  }

  ///
  /// 2a. Enable Force Wake
  ///
  IpWrRegWrite (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_GCD_GT_REG, 0x00010001, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, NULL, FORCE_WAKE_GPM_GRP_GCD_GT_REG, 0x00010001, (UINT32)IpWrRegRead (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits));
  ///
  /// 2b. Poll to verify Force Wake Acknowledge Bit
  ///
  PRINT_LEVEL1 ("Poll to verify Force Wake Acknowledge Bit, Result = 1\n");
  IpIGpuPollReady (pInst, DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_REG, BIT0, 1);

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return IpCsiStsSuccess;
}

/**
  Enable Idle mode for GT.

  @param[in]  pInst             A pointer to the IP instance to be used.

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuIdleGt (
  IP_IGPU_INST  *pInst
  )
{
  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    PRINT_WARNING ("Invalid parameters for %s\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->IGpuPrivateConfig.IsGtPresent == FALSE) {
    PRINT_LEVEL1 ("Exiting since GT is not present in the sku.\n");
    return IpCsiStsSuccess;
  }

  ///
  /// Enabling to enter RC6 state in idle mode.
  ///
  if (pInst->IGpuConfig.RenderStandby) {
    IpWrRegWrite (pInst->MmioAccess, RC_CTRL1_GPM_GRP_GCD_GT_REG, 0x00040000, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, "Entered RC6 state in idle mode", RC_CTRL1_GPM_GRP_GCD_GT_REG, 0x00040000, (UINT32)IpWrRegRead (pInst->MmioAccess, RC_CTRL1_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits));
  }

    ///
    ///  11a. Clear offset 0xA188 [31:0] to clear the force wake enable
    ///
    IpWrRegWrite (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_GCD_GT_REG, 0x00010000, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, FORCE_WAKE_GPM_GRP_GCD_GT_REG, 0x00010000, (UINT32)IpWrRegRead (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_GCD_GT_REG, IpWrRegFlagSize32Bits));

    ///
    ///  11b. Poll until clearing is cleared to verify the force wake acknowledge.
    ///
    IpIGpuPollReady (pInst, DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_REG, BIT0, 0);

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return IpCsiStsSuccess;
}
