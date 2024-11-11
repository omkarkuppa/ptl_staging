/**@file
  IP IGpu Display library.

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
#include <IGpuDisplayMem.h>
#include <IGpuDisplayRegs.h>
#include <IGpuCfg.h>

#define TOTAL_T12_TIME  500               ///< 500us
#define NO_DELAY        0                 ///< 0us
#define MAX_DELAY       0xFFFF

#ifndef DIVIDEROUND
#define DIVIDEROUND(a, b)  (((a) * (b) > 0) ? ((a) + (b) / 2) / (b) : ((a) - (b) / 2) / (b))
#endif

/**
  Calculates the memory Data Rate in units of (DataRate / 16.6666)

  @param[in] MemoryType          - The memory technology type
  @param[in] Tck                 - The minimum clock cycle time for the memory clock in femtoseconds

  @retval The scaled memory Data Rate in units of (DataRate / 16.6666)
**/
STATIC
UINT32
ScaleMemoryDataRate (
  UINT32  MemoryType,
  UINT32  Tck
  )
{
  UINT32  ConversionFactor;
  UINT32  ScaledMemDataRate;

  // Calculate the memory Data Rate in units of (DataRate / 16.6666)
  // using the reduced equation based on tCK in femtoseconds
  ConversionFactor = 120000000;
  if (MemoryType == MemoryTypeLpddr5) {
    // tCK is 4:1 of the double data rate clock period on LP5
    ConversionFactor *= 4;
  }

  ScaledMemDataRate = DIVIDEROUND (ConversionFactor, Tck);

  return ScaledMemDataRate;
}

/**
  Calculates the input timing in units of ns * (Data Rate / 1000)

  @param[in] MemoryType         - The memory technology type
  @param[in] NckTiming          - The timing to scale in units of tCK clocks

  @retval The scaled memory timing in units of ns * (Data Rate / 1000)
**/
STATIC
UINT32
ScaleMemoryTiming (
  MEMORY_DEVICE_TYPE  MemoryType,
  UINT32              NckTiming
  )
{
  UINT32  ScaledTiming;

  // Convert the input timing to units of ns * (Data Rate / 1000)
  // using the reduced requation based on the timing parameter in
  // units of memory clocks.
  ScaledTiming = NckTiming * 2;
  if (MemoryType == MemoryTypeLpddr5) {
    // tCK is 4:1 of the double data rate clock period on LP5
    ScaledTiming *= 4;
  }

  return ScaledTiming;
}

/**
  Check and Force Vdd On based on the H/W Status

  @param[in]  pInst               A pointer to the IP instance to be used.
  @param[in]  BootMode            Current BootMode

  @return         A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuCheckAndForceVddOn (
  IP_IGPU_INST  *pInst,
  UINT32        BootMode
  )
{
  PP_CONTROL_DISPLAY_MEM_STRUCT    DisplayPpControl;
  PP_CONTROL_2_DISPLAY_MEM_STRUCT  DisplayPpControl2;
  UINT32                           DeltaT12TimeMicroSec;
  NDE_RSTWRN_OPT_DISP_STRUCT       RstWrnOpt;

  DeltaT12TimeMicroSec = 0;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (IpIGpuSupported (pInst) == FALSE) {
    PRINT_LEVEL1 ("Returning from %s since IGD is not present\n", __FUNCTION__);
    return IpCsiStsSuccess;
  }

  if ((pInst->IGpuPreMemConfig.PanelPowerEnable == 1) && (BootMode != BOOT_ON_S3_RESUME) && (pInst->IGpuPreMemConfig.InternalGraphics != 0)) {
    ///
    /// Check if GttMmAdr has been already assigned, initialize if not
    ///
    if (IpIGpuGetGttMmAdr (pInst) == 0) {
      IpIGpuProgramGttMmAdr (pInst);
      //
      // Write LMEM Bar
      //
      IpIGpuProgramLMemBar (pInst);
    }

    if (!IpIGpuCmdRegEnabled (pInst)) {
      ///
      /// Enable Bus Initiator and Memory access on 0:2:0
      ///
      IpIGpuEnableCmdReg (pInst);
    }

    ///
    /// Return if Display Engine is not present
    ///
    if (pInst->IGpuPrivateConfig.IsDisplayPresent == FALSE) {
      PRINT_LEVEL1 ("Returning from %s since Display Engine is not present.", __FUNCTION__);
      return IpCsiStsSuccess;
    }

    ///
    /// Skip if VDD Bit is already set
    ///
    DisplayPpControl.Data  = (UINT32)IpWrRegRead (pInst->MmioAccess, PP_CONTROL_DISPLAY_MEM_REG, IpWrRegFlagSize32Bits);
    DisplayPpControl2.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, PP_CONTROL_2_DISPLAY_MEM_REG, IpWrRegFlagSize32Bits);
    if (((pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortAConfig == IpIGpuDdiPortEdp) && (DisplayPpControl.Bits.vdd_override == FALSE)) || \
        ((pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortBConfig == IpIGpuDdiPortEdp) && (DisplayPpControl2.Bits.vdd_override == FALSE))
        )
    {
      if ((BootMode == BOOT_ASSUMING_NO_CONFIGURATION_CHANGES) || (pInst->IGpuPreMemConfig.OemT12DelayOverride)) {
        DeltaT12TimeMicroSec = (UINT32)pInst->IGpuPreMemConfig.DeltaT12PowerCycleDelay;
        if ((DeltaT12TimeMicroSec > NO_DELAY) && (DeltaT12TimeMicroSec < MAX_DELAY)) {
          PRINT_LEVEL1 ("Policy value based T12 Delay Added : %d us\n", DeltaT12TimeMicroSec);
          IpWrDelayFromTimeUs (pInst->TimeCntxt, 0, DeltaT12TimeMicroSec);
        } else if (DeltaT12TimeMicroSec == MAX_DELAY) {
          PRINT_LEVEL1 ("Fixed T12 Delay added after elapsed time : %d us\n", MAX_DELAY);
          IpWrDelayFromTimeUs (pInst->TimeCntxt, 0, TOTAL_T12_TIME);
        }
      }
    }

    //
    // Enable PCH Reset Handshake
    //
    if (pInst->XeVersion <= IpIGpuXe3) {
      RstWrnOpt.Data                      = (UINT32)IpWrRegRead (pInst->MmioAccess, NDE_RSTWRN_OPT_DISP_REG, IpWrRegFlagSize32Bits);
      RstWrnOpt.Bits.rst_pch_handshake_en = TRUE;
      IpWrRegWrite (pInst->MmioAccess, NDE_RSTWRN_OPT_DISP_REG, RstWrnOpt.Data, IpWrRegFlagSize32Bits);
      PRINT_LEVEL1 ("PCH Reset Handshake = 0x%x\n", IpWrRegRead (pInst->MmioAccess, NDE_RSTWRN_OPT_DISP_REG, IpWrRegFlagSize32Bits));
    }

    ///
    /// Enable Panel Power - VDD bit
    ///
    if (pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortAConfig == IpIGpuDdiPortEdp) {
      PRINT_LEVEL1 ("Enable Panel Power Bit for eDP on DDI-A\n");
      DisplayPpControl.Bits.vdd_override = TRUE;
      IpWrRegWrite (pInst->MmioAccess, PP_CONTROL_DISPLAY_MEM_REG, DisplayPpControl.Data, IpWrRegFlagSize32Bits);
    }

    if (pInst->IGpuPreMemConfig.DdiConfiguration.DdiPortBConfig == IpIGpuDdiPortEdp) {
      PRINT_LEVEL1 ("Enable Panel Power Bit for eDP on DDI-B\n");
      DisplayPpControl2.Bits.vdd_override = TRUE;
      IpWrRegWrite (pInst->MmioAccess, PP_CONTROL_2_DISPLAY_MEM_REG, DisplayPpControl2.Data, IpWrRegFlagSize32Bits);
    }
  }

  return IpCsiStsSuccess;
}

/**
  Initialize the Display Memory Subsystem Config registers.

  @param[in]  pInst     A pointer to the IP instance to be used.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuDisplayMemSsInit (
  IP_IGPU_INST  *pInst
  )
{
  MEM_SS_INFO_GLOBAL_DISP_STRUCT         MemSsInfoGlobalReg;
  UINT8                                  SaGvPoint;
  IP_HOB_SAGV_TIMING_OUT                 *SaGvTiming;
  UINT32                                 tCK;
  MEMORY_DEVICE_TYPE                     MemoryType;
  UINT32                                 Dclk;
  UINT32                                 TRP;
  UINT32                                 TRDPRE;
  UINT32                                 TRAS;
  MEM_SS_INFO_QGV_POINT_0_0_DISP_STRUCT  MemSSInfoQgvPoint00;
  MEM_SS_INFO_QGV_POINT_0_1_DISP_STRUCT  MemSSInfoQgvPoint01;
  MEM_SS_INFO_QGV_POINT_1_0_DISP_STRUCT  MemSSInfoQgvPoint10;
  MEM_SS_INFO_QGV_POINT_1_1_DISP_STRUCT  MemSSInfoQgvPoint11;
  MEM_SS_INFO_QGV_POINT_2_0_DISP_STRUCT  MemSSInfoQgvPoint20;
  MEM_SS_INFO_QGV_POINT_2_1_DISP_STRUCT  MemSSInfoQgvPoint21;
  MEM_SS_INFO_QGV_POINT_3_0_DISP_STRUCT  MemSSInfoQgvPoint30;
  MEM_SS_INFO_QGV_POINT_3_1_DISP_STRUCT  MemSSInfoQgvPoint31;

  PRINT_LEVEL1 ("%s Begin\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (IpIGpuSupported (pInst) == FALSE) {
    PRINT_LEVEL1 ("Returning from %s since IGD is not present\n", __FUNCTION__);
    return IpCsiStsSuccess;
  }

  if (IpIGpuGetGttMmAdr (pInst) == 0) {
    IpIGpuProgramGttMmAdr (pInst);
  }

  if (!IpIGpuCmdRegEnabled (pInst)) {
    ///
    /// Enable Bus Initiator and Memory access on 0:2:0
    ///
    IpIGpuEnableCmdReg (pInst);
  }

  ///
  /// Return if Display Engine is not present
  ///
  if (pInst->IGpuPrivateConfig.IsDisplayPresent == FALSE) {
    PRINT_LEVEL1 ("Returning from %s since Display Engine is not present.\n", __FUNCTION__);
    return IpCsiStsSuccess;
  }

  MemoryType              = pInst->IGpuPrivateConfig.MemoryInfoData.MemoryType;
  MemSsInfoGlobalReg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, MEM_SS_INFO_GLOBAL_DISP_REG, IpWrRegFlagSize32Bits);
  switch (MemoryType) {
    case MemoryTypeDdr5:
      MemSsInfoGlobalReg.Bits.ddr_type = MEM_SS_INFO_GLOBAL_DDRTYPE_DDR5;
      break;

    case MemoryTypeLpddr5:
      MemSsInfoGlobalReg.Bits.ddr_type = MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR5;
      break;

    case MemoryTypeLpddr4:
      MemSsInfoGlobalReg.Bits.ddr_type = MEM_SS_INFO_GLOBAL_DDRTYPE_LPDDR4;
      break;

    default:
      return IpCsiStsError;
  }

  MemSsInfoGlobalReg.Bits.number_of_populated_channels = pInst->IGpuPrivateConfig.MemoryInfoData.NumPopulatedChannels;
  MemSsInfoGlobalReg.Bits.number_of_enabled_qgv_points = pInst->IGpuPrivateConfig.MemoryInfoData.SagvConfigInfo.NumSaGvPointsEnabled;
  IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_GLOBAL_DISP_REG, MemSsInfoGlobalReg.Data, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 (
    "MEM_SS_INFO_GLOBAL\n DdrType = %d \n NumberOfPopulatedChannels = %d\n NumberOfEnabledQgvPoints = %d\n", \
    MemSsInfoGlobalReg.Bits.ddr_type,
    MemSsInfoGlobalReg.Bits.number_of_populated_channels, \
    MemSsInfoGlobalReg.Bits.number_of_enabled_qgv_points
    );

  for (SaGvPoint = 0; SaGvPoint < HOB_MAX_SAGV_POINTS; SaGvPoint++) {
    if ((pInst->IGpuPrivateConfig.MemoryInfoData.SagvConfigInfo.SaGvPointMask & (BIT0 << SaGvPoint)) == 0) {
      continue;
    }

    SaGvTiming = &pInst->IGpuPrivateConfig.MemoryInfoData.SagvConfigInfo.SaGvTiming[SaGvPoint];
    tCK        = SaGvTiming->JedecTiming.tCK;
    Dclk       = ScaleMemoryDataRate (MemoryType, tCK);
    TRP        = ScaleMemoryTiming (MemoryType, SaGvTiming->JedecTiming.tRCDtRP);
    TRDPRE     = ScaleMemoryTiming (MemoryType, SaGvTiming->IpTiming.tRDPRE);
    TRAS       = ScaleMemoryTiming (MemoryType, SaGvTiming->JedecTiming.tRAS);
    switch (SaGvPoint) {
      case 0:
        MemSSInfoQgvPoint00.Data                                  = 0;
        MemSSInfoQgvPoint01.Data                                  = 0;
        MemSSInfoQgvPoint00.Bits.dclk_in_multiples_of_16_6666_mhz = Dclk;
        MemSSInfoQgvPoint00.Bits.trp_in_dclks                     = TRP;
        MemSSInfoQgvPoint00.Bits.trcd_in_dclks                    = TRP;
        MemSSInfoQgvPoint01.Bits.trdpre_in_dclks                  = TRDPRE;
        MemSSInfoQgvPoint01.Bits.tras_in_dclks                    = TRAS;
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_0_0_DISP_REG, MemSSInfoQgvPoint00.Data, IpWrRegFlagSize32Bits);
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_0_1_DISP_REG, MemSSInfoQgvPoint01.Data, IpWrRegFlagSize32Bits);
        PRINT_LEVEL1 (
          "MEM_SS_INFO_QGV_POINT_%d\n"
          "  Dclk = %d\n"
          "  TRP = %d\n"
          "  TRCD = %d\n"
          "  TRDPRE = %d\n"
          "  TRAS = %d\n",
          SaGvPoint,
          MemSSInfoQgvPoint00.Bits.dclk_in_multiples_of_16_6666_mhz,
          MemSSInfoQgvPoint00.Bits.trp_in_dclks,
          MemSSInfoQgvPoint00.Bits.trcd_in_dclks,
          MemSSInfoQgvPoint01.Bits.trdpre_in_dclks,
          MemSSInfoQgvPoint01.Bits.tras_in_dclks
          );
        break;

      case 1:
        MemSSInfoQgvPoint10.Data                                  = 0;
        MemSSInfoQgvPoint11.Data                                  = 0;
        MemSSInfoQgvPoint10.Bits.dclk_in_multiples_of_16_6666_mhz = Dclk;
        MemSSInfoQgvPoint10.Bits.trp_in_dclks                     = TRP;
        MemSSInfoQgvPoint10.Bits.trcd_in_dclks                    = TRP;
        MemSSInfoQgvPoint11.Bits.trdpre_in_dclks                  = TRDPRE;
        MemSSInfoQgvPoint11.Bits.tras_in_dclks                    = TRAS;
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_1_0_DISP_REG, MemSSInfoQgvPoint10.Data, IpWrRegFlagSize32Bits);
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_1_1_DISP_REG, MemSSInfoQgvPoint11.Data, IpWrRegFlagSize32Bits);
        PRINT_LEVEL1 (
          "MEM_SS_INFO_QGV_POINT_%d\n"
          "  Dclk = %d\n"
          "  TRP = %d\n"
          "  TRCD = %d\n"
          "  TRDPRE = %d\n"
          "  TRAS = %d\n",
          SaGvPoint,
          MemSSInfoQgvPoint10.Bits.dclk_in_multiples_of_16_6666_mhz,
          MemSSInfoQgvPoint10.Bits.trp_in_dclks,
          MemSSInfoQgvPoint10.Bits.trcd_in_dclks,
          MemSSInfoQgvPoint11.Bits.trdpre_in_dclks,
          MemSSInfoQgvPoint11.Bits.tras_in_dclks
          );
        break;

      case 2:
        MemSSInfoQgvPoint20.Data                                  = 0;
        MemSSInfoQgvPoint21.Data                                  = 0;
        MemSSInfoQgvPoint20.Bits.dclk_in_multiples_of_16_6666_mhz = Dclk;
        MemSSInfoQgvPoint20.Bits.trp_in_dclks                     = TRP;
        MemSSInfoQgvPoint20.Bits.trcd_in_dclks                    = TRP;
        MemSSInfoQgvPoint21.Bits.trdpre_in_dclks                  = TRDPRE;
        MemSSInfoQgvPoint21.Bits.tras_in_dclks                    = TRAS;
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_2_0_DISP_REG, MemSSInfoQgvPoint20.Data, IpWrRegFlagSize32Bits);
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_2_1_DISP_REG, MemSSInfoQgvPoint21.Data, IpWrRegFlagSize32Bits);
        PRINT_LEVEL1 (
          "MEM_SS_INFO_QGV_POINT_%d\n"
          "  Dclk = %d\n"
          "  TRP = %d\n"
          "  TRCD = %d\n"
          "  TRDPRE = %d\n"
          "  TRAS = %d\n",
          SaGvPoint,
          MemSSInfoQgvPoint20.Bits.dclk_in_multiples_of_16_6666_mhz,
          MemSSInfoQgvPoint20.Bits.trp_in_dclks,
          MemSSInfoQgvPoint20.Bits.trcd_in_dclks,
          MemSSInfoQgvPoint21.Bits.trdpre_in_dclks,
          MemSSInfoQgvPoint21.Bits.tras_in_dclks
          );
        break;

      case 3:
      default:
        MemSSInfoQgvPoint30.Data                                  = 0;
        MemSSInfoQgvPoint31.Data                                  = 0;
        MemSSInfoQgvPoint30.Bits.dclk_in_multiples_of_16_6666_mhz = Dclk;
        MemSSInfoQgvPoint30.Bits.trp_in_dclks                     = TRP;
        MemSSInfoQgvPoint30.Bits.trcd_in_dclks                    = TRP;
        MemSSInfoQgvPoint31.Bits.trdpre_in_dclks                  = TRDPRE;
        MemSSInfoQgvPoint31.Bits.tras_in_dclks                    = TRAS;
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_3_0_DISP_REG, MemSSInfoQgvPoint30.Data, IpWrRegFlagSize32Bits);
        IpWrRegWrite (pInst->MmioAccess, MEM_SS_INFO_QGV_POINT_3_1_DISP_REG, MemSSInfoQgvPoint31.Data, IpWrRegFlagSize32Bits);
        PRINT_LEVEL1 (
          "MEM_SS_INFO_QGV_POINT_%d\n"
          "  Dclk = %d\n"
          "  TRP = %d\n"
          "  TRCD = %d\n"
          "  TRDPRE = %d\n"
          "  TRAS = %d\n",
          SaGvPoint,
          MemSSInfoQgvPoint30.Bits.dclk_in_multiples_of_16_6666_mhz,
          MemSSInfoQgvPoint30.Bits.trp_in_dclks,
          MemSSInfoQgvPoint30.Bits.trcd_in_dclks,
          MemSSInfoQgvPoint31.Bits.trdpre_in_dclks,
          MemSSInfoQgvPoint31.Bits.tras_in_dclks
          );
        break;
    }
  }

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);

  return IpCsiStsSuccess;
}

/**
  Program Flat CCS Base Address register in Display.

  @param[in]  pInst               A pointer to the IP instance to be used.

  @return     A status value of type IP_CSI_STATUS.
**/
IP_CSI_STATUS
IpIGpuProgramFlatCcsBaseForDisplay (
  IP_IGPU_INST  *pInst
  )
{
  FLATCCSBASEANDRANGE_0_DISP_STRUCT  FlatCcsRange0;
  FLATCCSBASEANDRANGE_1_DISP_STRUCT  FlatCcsRange1;

  PRINT_LEVEL1 ("%s Begin\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->XeVersion >= IpIGpuXe2) {
    if (IpIGpuSupported (pInst) == FALSE) {
      return IpCsiStsSuccess;
    }

    if (pInst->IGpuPreMemConfig.MemoryBandwidthCompression == FALSE) {
      PRINT_LEVEL1 ("Skip Flat CCS programming for Display\n");
      return IpCsiStsSuccess;
    }

    FlatCcsRange0.Data                   = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_0_DISP_REG, IpWrRegFlagSize32Bits);
    FlatCcsRange0.Bits.ccs_enable        = TRUE;
    FlatCcsRange0.Bits.flat_ccs_base_low = (UINT32)IpWrRShiftU64 (pInst->IGpuPrivateConfig.FlatCcsBaseAddr, FLATCCSBASEANDRANGE_0_DISP_FLAT_CCS_BASE_LOW_LSB);
    IpWrRegWrite (pInst->MmioAccess, FLATCCSBASEANDRANGE_0_DISP_REG, FlatCcsRange0.Data, IpWrRegFlagSize32Bits);

    FlatCcsRange1.Data                    = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_1_DISP_REG, IpWrRegFlagSize32Bits);
    FlatCcsRange1.Bits.flat_ccs_base_high = (UINT32)IpWrRShiftU64 (pInst->IGpuPrivateConfig.FlatCcsBaseAddr, 32);
    FlatCcsRange1.Bits.flat_ccs_size      = (pInst->IGpuPrivateConfig.FlatCcsMemSize / 2);
    IpWrRegWrite (pInst->MmioAccess, FLATCCSBASEANDRANGE_1_DISP_REG, FlatCcsRange1.Data, IpWrRegFlagSize32Bits);

    PRINT_LEVEL1 ("FLATCCSBASEANDRANGE_0_DISP_REG = 0x%llx\n", IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_0_DISP_REG, IpWrRegFlagSize64Bits));
  }

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return IpCsiStsSuccess;
}

/**
  Initialize PAVP features.
  @param[in]  pInst       A pointer to the IP instance to be used.

  @retval IpCsiStsSuccess - PAVP initialization complete
  @retval EFI_UNSUPPORTED - IGPU is not present so PAVP not supported
**/
IP_CSI_STATUS
IpIGpuPavpInit (
  IP_IGPU_INST  *pInst
  )
{
  UINT32                     PcmBase;
  PAVPC0_REG_IGPU_STRUCT     DiplayPavpc;
  DEPAVPC_REG_0_DISP_STRUCT  DisplayMirrorPavpc;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (IpIGpuGetGttMmAdr (pInst) == 0) {
    IpIGpuProgramGttMmAdr (pInst);
  }

  if (IpIGpuSupported (pInst) == FALSE) {
    return IpCsiStsErrorNotAllowed;
  }

  DisplayMirrorPavpc.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, DEPAVPC_REG_0_DISP_REG, IpWrRegFlagSize32Bits);
  DiplayPavpc.Data        = (UINT32)IpWrRegRead (pInst->MmioAccess, PAVPC0_REG_IGPU_REG, IpWrRegFlagSize32Bits);

  if (!IpIGpuCmdRegEnabled (pInst)) {
    ///
    /// Enable Bus Initiator and Memory access on 0:2:0
    ///
    IpIGpuEnableCmdReg (pInst);
  }

  ///
  /// If device 0:2:0 (Internal Graphics Device, or GT) is not enabled, skip PAVP
  ///
  PRINT_LEVEL1 ("Initializing PAVP\n");

  PcmBase = 0;
  if (pInst->XeVersion >= IpIGpuXe2) {
    PcmBase = ((UINT32)IpWrRShiftU64 (pInst->IGpuPrivateConfig.BdsmBase, 20) + pInst->IGpuPrivateConfig.DsmSize - WOPCM_TOTAL_SIZE_8_MB);
  }

  PRINT_LEVEL1 ("Initializing PcmBase %x\n", PcmBase);

  DiplayPavpc.Bits.wopcmbase_0 = PcmBase;
  if (pInst->IGpuConfig.PavpEnable == 1) {
    DiplayPavpc.Bits.pavpe = TRUE;
  }

  DiplayPavpc.Bits.pcme = TRUE;

  DiplayPavpc.Bits.kcr_reset_fix = TRUE;


  //
  // Copy PAVP data into DE mirror register and skip copying WOPCM total and SA Media size.
  //
  DisplayMirrorPavpc.Data = DiplayPavpc.Data;

  if (pInst->XeVersion >= IpIGpuXe2) {
    //
    // Program SA Media WOPCM size = 6MB (BIT[13:11] = 0x4) and total WOPCM size = 8MB (BIT[9:7] = 0x3)
    //   WOPCM size is 000b = 1MB, 001b = 2MB, 010b = 4MB, 011b = 8MB, 100b = Reserved, 101b = 16MB, 110b = 32MB
    //   SA Media WOPCM size is 000b = 1MB, 001b = 2MB, 010b = 4MB, 011b = 8MB, 100b = 6MB, 101b = 16MB, 110b = Reserved.
    //
    DiplayPavpc.Bits.sa_media_wopcm_size = 0x4;
    DiplayPavpc.Bits.wopcm_size          = 0x3;
  }

  ///
  /// Lock PAVPC Register
  ///
    DiplayPavpc.Bits.lock        = TRUE;
    DisplayMirrorPavpc.Bits.lock = TRUE;

  //
  // Program GT MMIO PAVPC Mirror register
  //
  IpWrRegWrite (pInst->MmioAccess, PAVPC0_REG_IGPU_REG, DiplayPavpc.Data, IpWrRegFlagSize32Bits);
  if (pInst->XeVersion >= IpIGpuXe2) {
    IpWrRegWrite (pInst->MmioAccess, DEPAVPC_REG_0_DISP_REG, DisplayMirrorPavpc.Data, IpWrRegFlagSize32Bits);
  }

  return IpCsiStsSuccess;
}
