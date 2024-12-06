/**@file
  IP IGpu Media library.

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
#include <IGpuMediaRegs.h>

/**
  "Poll Status" for GT/Media Readiness

  @param[in]  pInst       The instance
  @param[in] Offset          - MMIO Offset
  @param[in] Mask            - Mask
  @param[in] Result          - Value to wait for

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

typedef struct {
  UINT32    Offset;
  UINT32    Value;
} BOOT_SCRIPT_REGISTER_SETTING;

//
// XE2
//
BOOT_SCRIPT_REGISTER_SETTING  mMediaXe2ClockGatingRegisters[] = {
  //
  // BaseAddr  Offset  AndMask  OrMask
  //
  //
  // Unslice
  //
  { UNSLCGCTL9430_MEDIA_REG,      0x0 },
  { UNSLCGCTL9434_MEDIA_REG,      0x0 },
  { UNSLCGCTL9438_MEDIA_REG,      0x0 },
  { UNSLCGCTL9440_MEDIA_REG,      0x0 },
  { UNSLCGCTL9444_MEDIA_REG,      0x0 },
  //
  // Slice
  //
  { VDCGCTL3F0C_VDBOX0_MEDIA_REG, 0x0 },
  { VDCGCTL3F1C_VDBOX0_MEDIA_REG, 0x0 },
  { VDCGCTL3F0C_VDBOX2_MEDIA_REG, 0x0 },
  { VDCGCTL3F1C_VDBOX2_MEDIA_REG, 0x0 },
  { VDCGCTL3F0C_VDBOX4_MEDIA_REG, 0x0 },
  { VDCGCTL3F1C_VDBOX4_MEDIA_REG, 0x0 },
  { VDCGCTL3F0C_VDBOX6_MEDIA_REG, 0x0 },
  { VDCGCTL3F1C_VDBOX6_MEDIA_REG, 0x0 }
};

/**
  Initialize Media of IGPU

  @param[in]  pInst       A pointer to the IP instance to be used.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuMediaInit (
  IP_IGPU_INST  *pInst
  )
{
  RC6CTXBASE_RPM_MEDIA_0_MEDIA_STRUCT           MediaRc6CtxLowBase;
  RC6CTXBASE_RPM_MEDIA_1_MEDIA_STRUCT           MediaRc6CtxHighBase;
  RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_STRUCT        Rcp1Freq;
  RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_STRUCT       Rcp1BusyThresh;
  RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_STRUCT      Rcp1FreqChng;
  GCP_CLKDIV_CONTROL0_MEDIA_STRUCT              GcpClkDivControl0;
  GCP_CLKDIV_CONTROL1_MEDIA_STRUCT              GcpClkDivControl1;
  CONFIG0_RPM_MEDIA_MEDIA_STRUCT                Config0Rpm;
  CCCHKNREG1_I_SAM_MEDIA_STRUCT                 ChickenCCReg;
  MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_STRUCT         MiscCtrl0GpmGrp;
  ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_STRUCT         EcoBusrstGpmGrp;
  UINT32                                        RegOffset;
  UINT8                                         LoopCounter;
  UINT32                                        Value;
  FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_STRUCT  FlatCcsBaseGamReq;
  FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_STRUCT  FlatCssLowBase;
  FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_STRUCT  FlatCcsHighBase;
  CCLOCK_I_SAM_MEDIA_STRUCT                     CClockMedia;
  ECOCHK_GAMCTRL_MEDIA_MEDIA_STRUCT             EcochkGamCtrlMedia;
  L3BANKLOCK_LBCF_MEDIA_MEDIA_STRUCT            L3lockMedia;
  GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_STRUCT      GamCntrl;
  GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_STRUCT        GamCtrlLock;
  MIRROR_GMD_ID_RPM_MEDIA_MEDIA_STRUCT          GmdIdStruct;
  BOOLEAN                                       IsFlatCcsCompressionEnable;

  PRINT_LEVEL1 ("%s Begin\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  IsFlatCcsCompressionEnable = FALSE;

  if (IpIGpuSupported (pInst) == FALSE) {
    PRINT_LEVEL1 ("Returning from %s since IGPU is not present\n", __FUNCTION__);
    return IpCsiStsSuccess;
  }

  GmdIdStruct.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, MIRROR_GMD_ID_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("Media GMD ID = 0x%x \n", GmdIdStruct.Data);

  ///
  /// 1aa. Enable all GTI-Uncore clock gating
  ///
  IpWrRegWrite (pInst->MmioAccess, INFCGCTL9560_MEDIA_REG, 0x0, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, NULL, INFCGCTL9560_MEDIA_REG, 0x0, (UINT32)IpWrRegRead (pInst->MmioAccess, INFCGCTL9560_MEDIA_REG, IpWrRegFlagSize32Bits));

  ///
  /// 1c. Media RC6CTXBASE
  /// Get BdsmBase in MB
  ///
  MediaRc6CtxLowBase.Data            = (((UINT32)pInst->IGpuPrivateConfig.BdsmBase & ~(BIT0)) + (pInst->IGpuPrivateConfig.DsmSize * SIZE_1MB) - (GCD_WOPCM_SIZE_2_MB * SIZE_1MB) - SIZE_64KB);
  MediaRc6CtxLowBase.Bits.rc6memlock = TRUE;
  MediaRc6CtxHighBase.Data           = 0;
  IpWrRegWrite (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_1_MEDIA_REG, MediaRc6CtxHighBase.Data, IpWrRegFlagSize32Bits);
  IpWrRegWrite (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_0_MEDIA_REG, MediaRc6CtxLowBase.Data, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, "RC6CTXBASE HIGH", RC6CTXBASE_RPM_MEDIA_1_MEDIA_REG, MediaRc6CtxHighBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_1_MEDIA_REG, IpWrRegFlagSize32Bits));
  IpIGpuPrintRegData (pInst, "RC6CTXBASE LOW", RC6CTXBASE_RPM_MEDIA_0_MEDIA_REG, MediaRc6CtxLowBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_0_MEDIA_REG, IpWrRegFlagSize32Bits));

  if (pInst->IGpuPrivateConfig.IsMediaPresent == FALSE) {
    PRINT_LEVEL1 ("%s : Media is not present\n", __FUNCTION__);
    return IpCsiStsSuccess;
  }

  if (((UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_0_MEDIA_REG, IpWrRegFlagSize32Bits) != MediaRc6CtxLowBase.Data) || \
      ((UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_1_MEDIA_REG, IpWrRegFlagSize32Bits) != MediaRc6CtxHighBase.Data))
  {
    PRINT_ERROR ("Not able to configure Media Rc6CtxBase due to some other agent is configuring and locking it before BIOS does.\n");
    return IpCsiStsError;
  }

  if (pInst->IGpuConfig.PeiMediaConfig.ConfigureMedia) {
    PRINT_LEVEL1 ("Starting Register programming for Media Init.\n");
    ///
    /// 1h. Set and lock RC1p frequency to 16.667Mhz.
    ///
    Rcp1Freq.Data               = (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    Rcp1Freq.Bits.rc1p_req_freq = 0x1;
    Rcp1Freq.Bits.lock          = TRUE;
    IpWrRegWrite (pInst->MmioAccess, RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_REG, Rcp1Freq.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_REG, Rcp1Freq.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 1i. Set RC1p hysteresis to 4096 SBclocks, 10us.
    ///
    Rcp1BusyThresh.Data                      = (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    Rcp1BusyThresh.Bits.rc1p_busy_hyst_count = 0x1000;
    IpWrRegWrite (pInst->MmioAccess, RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_REG, Rcp1BusyThresh.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_REG, Rcp1BusyThresh.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 1j. Enable or disable RC1p.
    ///
    Rcp1FreqChng.Data             = (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    Rcp1FreqChng.Bits.rc1p_enable = FALSE;
    if (pInst->IGpuConfig.PeiMediaConfig.RC1pMediaFreqEnable == 1) {
      Rcp1FreqChng.Bits.rc1p_enable = TRUE;
    }

    IpWrRegWrite (pInst->MmioAccess, RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_REG, Rcp1FreqChng.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_REG, Rcp1FreqChng.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 1k.Set Lock Bit for GCP CLOCK DIV CONTROL Register.
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      GcpClkDivControl0.Data                = (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL0_MEDIA_REG, IpWrRegFlagSize32Bits);
      GcpClkDivControl0.Bits.clkdiv_lockbit = TRUE;
      IpWrRegWrite (pInst->MmioAccess, GCP_CLKDIV_CONTROL0_MEDIA_REG, GcpClkDivControl0.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, GCP_CLKDIV_CONTROL0_MEDIA_REG, GcpClkDivControl0.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL0_MEDIA_REG, IpWrRegFlagSize32Bits));

      GcpClkDivControl1.Data                = (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL1_MEDIA_REG, IpWrRegFlagSize32Bits);
      GcpClkDivControl1.Bits.clkdiv_lockbit = TRUE;
      IpWrRegWrite (pInst->MmioAccess, GCP_CLKDIV_CONTROL1_MEDIA_REG, GcpClkDivControl1.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, GCP_CLKDIV_CONTROL1_MEDIA_REG, GcpClkDivControl1.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GCP_CLKDIV_CONTROL1_MEDIA_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 1l. Programming Crystal Clock.
    /// Programm [2:1] = 11b if [5:3] is 001 (indicate ref clock is 19.2Mhz)
    /// Programm [2:1] = 10b if [5:3] is 000/010/011 (indicate ref clock is 24Mhz/38.4MHz/25MHz)
    ///
    Config0Rpm.Data      = (UINT32)IpWrRegRead (pInst->MmioAccess, CONFIG0_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    Config0Rpm.Bits.lock = TRUE;
    if (Config0Rpm.Bits.crysclock == 0x1) {
      Config0Rpm.Bits.ctc_shift_param = 0x3;
    } else if ((Config0Rpm.Bits.crysclock == 0) || (Config0Rpm.Bits.crysclock == 2) || (Config0Rpm.Bits.crysclock == 3)) {
      Config0Rpm.Bits.ctc_shift_param = 0x2;
    }

    IpWrRegWrite (pInst->MmioAccess, CONFIG0_RPM_MEDIA_MEDIA_REG, Config0Rpm.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, CONFIG0_RPM_MEDIA_MEDIA_REG, Config0Rpm.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, CONFIG0_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 2. Enable Force Wake
    ///
    IpIGpuForceWakeupMedia (pInst);

    ///
    /// 3. Disable 32B Request
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      ChickenCCReg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, CCCHKNREG1_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits);
      if (pInst->IGpuPrivateConfig.IsIbeccEnabled == TRUE) {
        ChickenCCReg.Bits._32bwrdisable = TRUE;
      } else {
        ChickenCCReg.Bits._32bwrdisable = FALSE;
      }

      ChickenCCReg.Bits.encompperffix = FALSE;
      ChickenCCReg.Bits.l3cmpctrl     = TRUE;
      IpWrRegWrite (pInst->MmioAccess, CCCHKNREG1_I_SAM_MEDIA_REG, ChickenCCReg.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, CCCHKNREG1_I_SAM_MEDIA_REG, ChickenCCReg.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, CCCHKNREG1_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 4. Program GT GUC WOPCM OFFSET and SIZE
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      IpWrRegWrite (pInst->MmioAccess, DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_REG, 0x504002, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_REG, 0x504002, (UINT32)IpWrRegRead (pInst->MmioAccess, DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_REG, IpWrRegFlagSize32Bits));
    }

    IpWrRegWrite (pInst->MmioAccess, GUC_WOPCM_SIZE_GUCM_REG_MEDIA_REG, 0xD0000, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GUC_WOPCM_SIZE_GUCM_REG_MEDIA_REG, 0xD0000, (UINT32)IpWrRegRead (pInst->MmioAccess, GUC_WOPCM_SIZE_GUCM_REG_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5a. GPM Control
    ///
    MiscCtrl0GpmGrp.Data = 0x80200000;
    IpWrRegWrite (pInst->MmioAccess, MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_REG, MiscCtrl0GpmGrp.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_REG, MiscCtrl0GpmGrp.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5b. ECOBUS Disable Fence writes
    ///
    EcoBusrstGpmGrp.Data                            = (UINT32)IpWrRegRead (pInst->MmioAccess, ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    EcoBusrstGpmGrp.Bits.gacfg_fence_c6status_write = TRUE;
    EcoBusrstGpmGrp.Bits.cpd_block_mgsr_disable     = TRUE;
    EcoBusrstGpmGrp.Bits.cpd_bypass_go_msg_strms    = TRUE;
    EcoBusrstGpmGrp.Bits.lock                       = TRUE;
    IpWrRegWrite (pInst->MmioAccess, ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_REG, EcoBusrstGpmGrp.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_REG, EcoBusrstGpmGrp.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5c. Enable Unit Level Clock Gating
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      for (LoopCounter = 0; LoopCounter < ARRAY_SIZE (mMediaXe2ClockGatingRegisters); ++LoopCounter) {
        RegOffset = mMediaXe2ClockGatingRegisters[LoopCounter].Offset;
        Value     = mMediaXe2ClockGatingRegisters[LoopCounter].Value;
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
    FlatCcsBaseGamReq.Data               = (UINT32)IpWrRegRead (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    FlatCcsBaseGamReq.Bits.flat_ccs_lock = TRUE;
    if (IsFlatCcsCompressionEnable) {
      FlatCcsBaseGamReq.Bits.flat_ccs_en = TRUE;
    }

    IpWrRegWrite (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_REG, FlatCcsBaseGamReq.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_REG, FlatCcsBaseGamReq.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 5f. Program Flat CCS Base address.
    ///
    if (IsFlatCcsCompressionEnable) {
      FlatCssLowBase.Data                                = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits);
      FlatCssLowBase.Bits.ccsenable                      = TRUE;
      FlatCssLowBase.Bits.hwviewofflatccsbaseatcclevello = (UINT32)IpWrRShiftU64 (pInst->IGpuPrivateConfig.FlatCcsBaseAddr, FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_HWVIEWOFFLATCCSBASEATCCLEVELLO_LSB);
      IpWrRegWrite (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_REG, FlatCssLowBase.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, "FlatCssLowBase", FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_REG, FlatCssLowBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits));

      FlatCcsHighBase.Data                       = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits);
      FlatCcsHighBase.Bits.swviewofflatccsbasehi = (UINT32)IpWrRShiftU64 (pInst->IGpuPrivateConfig.FlatCcsBaseAddr, 32);
      FlatCcsHighBase.Bits.flatccsize            = (pInst->IGpuPrivateConfig.FlatCcsMemSize / 2);
      IpWrRegWrite (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_REG, FlatCcsHighBase.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, "FlatCcsHighBase", FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_REG, FlatCcsHighBase.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits));
    }

    //
    // 5g. Program CCLOCK register
    //
    if (pInst->XeVersion >= IpIGpuXe2) {
      CClockMedia.Data                          = (UINT32)IpWrRegRead (pInst->MmioAccess, CCLOCK_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits);
      CClockMedia.Bits.ccchknreg1_lock          = TRUE;
      CClockMedia.Bits.flatccsbaseandrange_lock = TRUE;
      CClockMedia.Bits.cclock_lock              = TRUE;
      IpWrRegWrite (pInst->MmioAccess, CCLOCK_I_SAM_MEDIA_REG, CClockMedia.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, CCLOCK_I_SAM_MEDIA_REG, CClockMedia.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, CCLOCK_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 6. Set all *CS/GuC Idle Max Count (hysteresis)
    /// 7. Enable Idle Messages from all *CS
    ///
    if (pInst->XeVersion >= IpIGpuXe2) {
      IpWrRegWrite (pInst->MmioAccess, GUC_PM_MAXCNT_GUCM_REG_MEDIA_REG, 0xA, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, GUC_PM_MAXCNT_GUCM_REG_MEDIA_REG, 0xA, (UINT32)IpWrRegRead (pInst->MmioAccess, GUC_PM_MAXCNT_GUCM_REG_MEDIA_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// 8a. Program GT Normal Frequency Request
    ///
    IpWrRegWrite (pInst->MmioAccess, RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_REG, 0x06000000, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_REG, 0x06000000, (UINT32)IpWrRegRead (pInst->MmioAccess, RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    ///
    /// 8b. RP Control.
    ///
    IpWrRegWrite (pInst->MmioAccess, RP_CTRL_GPM_GRP_MEDIA_MEDIA_REG, 0x00000400, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, RP_CTRL_GPM_GRP_MEDIA_MEDIA_REG, 0x00000400, (UINT32)IpWrRegRead (pInst->MmioAccess, RP_CTRL_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    //
    // 9a. TLB register Programming
    //
    GamCntrl.Data                 = (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    GamCntrl.Bits.skip_cflush_ack = FALSE;
    IpWrRegWrite (pInst->MmioAccess, GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_REG, GamCntrl.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_REG, GamCntrl.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    //
    // 9b. Disable ATS replay mechanism
    //
    EcochkGamCtrlMedia.Data                   = (UINT32)IpWrRegRead (pInst->MmioAccess, ECOCHK_GAMCTRL_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    EcochkGamCtrlMedia.Bits.en_security_chk   = TRUE;
    EcochkGamCtrlMedia.Bits.dis_ats_replay    = TRUE;
    EcochkGamCtrlMedia.Bits.lock_security_chk = TRUE;
    IpWrRegWrite (pInst->MmioAccess, ECOCHK_GAMCTRL_MEDIA_MEDIA_REG, EcochkGamCtrlMedia.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, ECOCHK_GAMCTRL_MEDIA_MEDIA_REG, EcochkGamCtrlMedia.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, ECOCHK_GAMCTRL_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    //
    // 9c. Lock GAM CTRL Register
    //
    GamCtrlLock.Data               = (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    GamCtrlLock.Bits.lock_reg_cf08 = TRUE;
    GamCtrlLock.Bits.lock_reg_cf54 = TRUE;
    GamCtrlLock.Bits.gamctrllock_l = TRUE;
    IpWrRegWrite (pInst->MmioAccess, GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG, GamCtrlLock.Data, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, NULL, GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG, GamCtrlLock.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

    //
    // Program L3BANKLOCK register for Media.
    //
    if (pInst->XeVersion >= IpIGpuXe3) {
      L3lockMedia.Data                     = (UINT32)IpWrRegRead (pInst->MmioAccess, L3BANKLOCK_LBCF_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
      L3lockMedia.Bits.lbcflockmsgreg_lock = TRUE;
      IpWrRegWrite (pInst->MmioAccess, L3BANKLOCK_LBCF_MEDIA_MEDIA_REG, L3lockMedia.Data, IpWrRegFlagSize32Bits);
      IpIGpuPrintRegData (pInst, NULL, L3BANKLOCK_LBCF_MEDIA_MEDIA_REG, L3lockMedia.Data, (UINT32)IpWrRegRead (pInst->MmioAccess, L3BANKLOCK_LBCF_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));
    }

    //
    // 9. Enabling to enter MC6 state in idle mode.
    //
    IpIGpuIdleMedia (pInst);
  }

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);

  return IpCsiStsSuccess;
}

/**
  Enable Force Wake Up for media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuForceWakeupMedia (
  IP_IGPU_INST  *pInst
  )
{
  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    PRINT_WARNING ("Invalid parameters for %s\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->IGpuPrivateConfig.IsMediaPresent == FALSE) {
    PRINT_LEVEL1 ("Exiting since Media is not present in the sku.\n");
    return IpCsiStsSuccess;
  }

  ///
  /// 2a. Enable Force Wake
  ///
  IpWrRegWrite (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REG, 0x00010001, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, NULL, FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REG, 0x00010001, (UINT32)IpWrRegRead (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

  ///
  /// 2b. Poll to verify Force Wake Acknowledge Bit
  ///
  PRINT_LEVEL1 ("Poll to verify Force Wake Acknowledge Bit, Result = 1\n");
  IpIGpuPollReady (pInst, DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_REG, BIT0, 1);

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return IpCsiStsSuccess;
}

/**
  Enable Idle mode for Media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuIdleMedia (
  IP_IGPU_INST  *pInst
  )
{
  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    PRINT_WARNING ("Invalid parameters for %s\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->IGpuPrivateConfig.IsMediaPresent == FALSE) {
    PRINT_LEVEL1 ("Exiting since Media is not present in the sku.\n");
    return IpCsiStsSuccess;
  }

  ///
  /// Enabling to enter MC6 state in idle mode.
  ///
  if (pInst->IGpuConfig.MediaStandby) {
    IpWrRegWrite (pInst->MmioAccess, RC_CTRL1_GPM_GRP_MEDIA_MEDIA_REG, 0x00040000, IpWrRegFlagSize32Bits);
    IpIGpuPrintRegData (pInst, "Entered MC6 state in idle mode", RC_CTRL1_GPM_GRP_MEDIA_MEDIA_REG, 0x00040000, (UINT32)IpWrRegRead (pInst->MmioAccess, RC_CTRL1_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));
  }

  ///
  /// 10a. Clear offset 0xA188 [31:0] to clear the force wake enable
  ///
  IpWrRegWrite (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REG, 0x00010000, IpWrRegFlagSize32Bits);
  IpIGpuPrintRegData (pInst, NULL, FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REG, 0x00010000, (UINT32)IpWrRegRead (pInst->MmioAccess, FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits));

  ///
  /// 10b. Poll until clearing is cleared to verify the force wake acknowledge.
  ///
  PRINT_LEVEL1 ("Poll to verify Force Wake Acknowledge Bit, Result = 0\n");
  IpIGpuPollReady (pInst, DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_REG, BIT0, 0);

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return IpCsiStsSuccess;
}
