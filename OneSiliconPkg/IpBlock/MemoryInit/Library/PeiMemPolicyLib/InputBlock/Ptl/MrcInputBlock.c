/** @file
  This file contains MrcInputBlock initial value.

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
#include "MrcInputBlock.h"
#include "CMrcApi.h"
#ifndef MRC_MINIBIOS_BUILD
#include <Library/BaseMemoryLib.h>
#else
#include "MrcOemPlatform.h"
#endif

#define MRC_INPUT_BINARY_BLOCK_SIG0                    (0x4943524D5F5F5F5FULL)
#define MRC_INPUT_BINARY_BLOCK_SIG1                    (0x5F5F5F5F5455504EULL)

#define MRC_EXT_INPUTS_INIT_VAL_Size                   (sizeof (MRC_EXT_INPUTS_TYPE))
#define MRC_EXT_INPUTS_INIT_VAL_HobBufferSize          (0)
#define MRC_EXT_INPUTS_INIT_VAL_MemoryProfile          (STD_PROFILE)

#define MRC_EXT_INPUTS_INIT_VAL_tCL                    (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRCDtRP                (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRAS                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_tWR                    (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRFC                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRRD                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_tWTR                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRTP                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_tFAW                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_tCWL                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_tREFI                  (0)
#define MRC_EXT_INPUTS_INIT_VAL_VddVoltage             (VDD_INVALID)
#define MRC_EXT_INPUTS_INIT_VAL_Vdd2Mv                 (0)

#define MRC_EXT_INPUTS_INIT_VAL_EccSupport              (0)
#define MRC_EXT_INPUTS_INIT_VAL_RsvdBit1                (0)
#define MRC_EXT_INPUTS_INIT_VAL_RemapEnable             (1)
#define MRC_EXT_INPUTS_INIT_VAL_ScramblerSupport        (1)
#define MRC_EXT_INPUTS_INIT_VAL_ProbelessTrace          (0)
#define MRC_EXT_INPUTS_INIT_VAL_PerBankRefresh          (1)
#define MRC_EXT_INPUTS_INIT_VAL_SimicsFlag              (0)
#define MRC_EXT_INPUTS_INIT_VAL_ExtendedBankHashing     (1)
#define MRC_EXT_INPUTS_INIT_VAL_SharedZqPin             (0)
#define MRC_EXT_INPUTS_INIT_VAL_UserThresholdEnable     (0)
#define MRC_EXT_INPUTS_INIT_VAL_RmtPerTask              (0)
#define MRC_EXT_INPUTS_INIT_VAL_TrainTrace              (0)
#define MRC_EXT_INPUTS_INIT_VAL_UserBudgetEnable        (0)
#define MRC_EXT_INPUTS_INIT_VAL_MsHashEnable            (0)
#define MRC_EXT_INPUTS_INIT_VAL_DisPgCloseIdleTimeout   (0)
#define MRC_EXT_INPUTS_INIT_VAL_Ibecc                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_IbeccParity             (0)
#define MRC_EXT_INPUTS_INIT_VAL_IbeccOperationMode      (2)
#define MRC_EXT_INPUTS_INIT_VAL_ChHashOverride          (0)
#define MRC_EXT_INPUTS_INIT_VAL_SimicsMinDbgMsg         (0)
#define MRC_EXT_INPUTS_INIT_VAL_RetrainOnFastFail       (1)
#define MRC_EXT_INPUTS_INIT_VAL_DvfsqEnabled            (0)
#define MRC_EXT_INPUTS_INIT_VAL_DvfscEnabled            (1)
#define MRC_EXT_INPUTS_INIT_VAL_VoltageReadout          (0)
#define MRC_EXT_INPUTS_INIT_VAL_CccHalfFrequency        (0)
#define MRC_EXT_INPUTS_INIT_VAL_Ddr5AutoPrechargeEnable (0)
#define MRC_EXT_INPUTS_INIT_VAL_Lp5SplitACTEnable       (2)
#define MRC_EXT_INPUTS_INIT_VAL_RetrainToWorkingChannel (0)
#define MRC_EXT_INPUTS_INIT_VAL_SubChHashOverride       (0)
#define MRC_EXT_INPUTS_INIT_VAL_DisableChannel          {{0,0,0,0}, {0,0,0,0}}
#define MRC_EXT_INPUTS_INIT_VAL_Ratio                   (0)

#define MRC_EXT_INPUTS_INIT_VAL_ChHashInterleaveBit    (2)
#define MRC_EXT_INPUTS_INIT_VAL_SmramMask              (0)

// TrainingEnables
#define MRC_EXT_INPUTS_INIT_VAL_ECT                    (1)  // BIT0
#define MRC_EXT_INPUTS_INIT_VAL_SOT                    (1)  // BIT1
#define MRC_EXT_INPUTS_INIT_VAL_ERDMPRTC2D             (1)  // BIT2
#define MRC_EXT_INPUTS_INIT_VAL_RDMPRT                 (0)  // BIT3
#define MRC_EXT_INPUTS_INIT_VAL_RCVET                  (1)  // BIT4
#define MRC_EXT_INPUTS_INIT_VAL_JWRL                   (1)  // BIT5
#define MRC_EXT_INPUTS_INIT_VAL_EWRTC2D                (1)  // BIT6
#define MRC_EXT_INPUTS_INIT_VAL_ERDTC2D                (1)  // BIT7
#define MRC_EXT_INPUTS_INIT_VAL_UNMATCHEDWRTC1D        (1)  // BIT8
#define MRC_EXT_INPUTS_INIT_VAL_WRTC1D                 (1)  // BIT9
#define MRC_EXT_INPUTS_INIT_VAL_WRVC1D                 (1)  // BIT10
#define MRC_EXT_INPUTS_INIT_VAL_RDTC1D                 (1)  // BIT11
#define MRC_EXT_INPUTS_INIT_VAL_RDVC1D                 (1)  // BIT12
#define MRC_EXT_INPUTS_INIT_VAL_WRTC2D                 (0)  // BIT13
#define MRC_EXT_INPUTS_INIT_VAL_RDTC2D                 (0)  // BIT14
#define MRC_EXT_INPUTS_INIT_VAL_WRVC2D                 (0)  // BIT15
#define MRC_EXT_INPUTS_INIT_VAL_RDVC2D                 (0)  // BIT16
#define MRC_EXT_INPUTS_INIT_VAL_WRDSEQT                (1)  // BIT17
#define MRC_EXT_INPUTS_INIT_VAL_DQSRF                  (1)  // BIT18
#define MRC_EXT_INPUTS_INIT_VAL_RDDQSODTT              (0)  // BIT19
#define MRC_EXT_INPUTS_INIT_VAL_RDEQT                  (1)  // BIT20
#define MRC_EXT_INPUTS_INIT_VAL_DUNITC                 (1)  // BIT21
#define MRC_EXT_INPUTS_INIT_VAL_CMDVC                  (1)  // BIT22
#define MRC_EXT_INPUTS_INIT_VAL_LCT                    (0)  // BIT23
#define MRC_EXT_INPUTS_INIT_VAL_RTL                    (0)  // BIT24
#define MRC_EXT_INPUTS_INIT_VAL_TAT                    (1)  // BIT25
#define MRC_EXT_INPUTS_INIT_VAL_RMT                    (1)  // BIT26
#define MRC_EXT_INPUTS_INIT_VAL_RMTEVENODD             (0)  // BIT27
#define MRC_EXT_INPUTS_INIT_VAL_ALIASCHK               (0)  // BIT28
#define MRC_EXT_INPUTS_INIT_VAL_RCVENC1D               (0)  // BIT29
#define MRC_EXT_INPUTS_INIT_VAL_RMC                    (0)  // BIT30
#define MRC_EXT_INPUTS_INIT_VAL_PRETRAIN               (1)  // BIT31

// TrainingEnables2
#define MRC_EXT_INPUTS_INIT_VAL_DCCPICODELUT           (1)  // BIT0
#define MRC_EXT_INPUTS_INIT_VAL_DIMMODTT               (1)  // BIT1
#define MRC_EXT_INPUTS_INIT_VAL_DIMMRONT               (0)  // BIT2
#define MRC_EXT_INPUTS_INIT_VAL_TXTCO                  (1)  // BIT3
#define MRC_EXT_INPUTS_INIT_VAL_CLKTCO                 (0)  // BIT4
#define MRC_EXT_INPUTS_INIT_VAL_CMDSR                  (0)  // BIT5
#define MRC_EXT_INPUTS_INIT_VAL_CMDDSEQ                (0)  // BIT6
#define MRC_EXT_INPUTS_INIT_VAL_DIMMODTCA              (0)  // BIT7
#define MRC_EXT_INPUTS_INIT_VAL_DDR5ODTTIMING          (1)  // BIT8
#define MRC_EXT_INPUTS_INIT_VAL_DBI                    (0)  // BIT9
#define MRC_EXT_INPUTS_INIT_VAL_DLLDCC                 (1)  // BIT10
#define MRC_EXT_INPUTS_INIT_VAL_DLLBWSEL               (1)  // BIT11
#define MRC_EXT_INPUTS_INIT_VAL_RDVREFDC               (0)  // BIT12
#define MRC_EXT_INPUTS_INIT_VAL_RDTCIDLE               (1)  // BIT13
#define MRC_EXT_INPUTS_INIT_VAL_RMTBIT                 (0)  // BIT14
#define MRC_EXT_INPUTS_INIT_VAL_DQDQSSWZ               (1)  // BIT15
#define MRC_EXT_INPUTS_INIT_VAL_REFPI                  (1)  // BIT16
#define MRC_EXT_INPUTS_INIT_VAL_DCCLP5READDCA          (1)  // BIT17
#define MRC_EXT_INPUTS_INIT_VAL_VCCCLKFF               (0)  // BIT18
#define MRC_EXT_INPUTS_INIT_VAL_FUNCDCCDQS             (0)  // BIT19
#define MRC_EXT_INPUTS_INIT_VAL_FUNCDCCCLK             (0)  // BIT20
#define MRC_EXT_INPUTS_INIT_VAL_FUNCDCCWCK             (0)  // BIT21
#define MRC_EXT_INPUTS_INIT_VAL_FUNCDCCDQ              (0)  // BIT22
#define MRC_EXT_INPUTS_INIT_VAL_DATAPILIN              (1)  // BIT23
#define MRC_EXT_INPUTS_INIT_VAL_DDR5XTALK              (0)  // BIT24
#define MRC_EXT_INPUTS_INIT_VAL_DCCLP5WCKDCA           (1)  // BIT25
#define MRC_EXT_INPUTS_INIT_VAL_RXUNMATCHEDCAL         (1)  // BIT26
#define MRC_EXT_INPUTS_INIT_VAL_WRTDIMMDFE             (1)  // BIT27
#define MRC_EXT_INPUTS_INIT_VAL_RMTLVR                 (0)  // BIT28
#define MRC_EXT_INPUTS_INIT_VAL_DCCDDR5READDCA         (1)  // BIT29
#define MRC_EXT_INPUTS_INIT_VAL_WREQT                  (0)  // BIT30
#define MRC_EXT_INPUTS_INIT_VAL_SimicsReservedBit      (0)  // BIT31

// TrainingEnables3
#define MRC_EXT_INPUTS_INIT_VAL_RXDQSDCC               (1)  // BIT0
#define MRC_EXT_INPUTS_INIT_VAL_DIMMNTODT              (0)  // BIT1
#define MRC_EXT_INPUTS_INIT_VAL_TXDQSDCC               (1)  // BIT2
#define MRC_EXT_INPUTS_INIT_VAL_RXVREFPERBIT           (0)  // BIT3
#define MRC_EXT_INPUTS_INIT_VAL_PPR                    (0)  // BIT4
#define MRC_EXT_INPUTS_INIT_VAL_LVRAUTOTRIM            (1)  // BIT5
#define MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit6          (0)  // BIT6
#define MRC_EXT_INPUTS_INIT_VAL_OPTIMIZECOMP           (0)  // BIT7
#define MRC_EXT_INPUTS_INIT_VAL_WRTRETRAIN             (1)  // BIT8
#define MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit9          (0)  // BIT9
#define MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit10         (0)  // BIT10
#define MRC_EXT_INPUTS_INIT_VAL_JEDECRESET             (1)  // BIT11
#define MRC_EXT_INPUTS_INIT_VAL_ROUNDTRIPMATCH         (1)  // BIT12
#define MRC_EXT_INPUTS_INIT_VAL_TLINECLKCAL            (1)  // BIT13
#define MRC_EXT_INPUTS_INIT_VAL_DCCPISERIALCAL         (1)  // BIT14
#define MRC_EXT_INPUTS_INIT_VAL_PHASECLKCAL            (1)  // BIT15
#define MRC_EXT_INPUTS_INIT_VAL_WCKPADDCCCAL           (1)  // BIT16
#define MRC_EXT_INPUTS_INIT_VAL_RDCTLET                (1)  // BIT17
#define MRC_EXT_INPUTS_INIT_VAL_RDDQODTT               (1)  // BIT18
#define MRC_EXT_INPUTS_INIT_VAL_EMPHASIS               (1)  // BIT19
#define MRC_EXT_INPUTS_INIT_VAL_DIMMRXOFFSET           (0)  // BIT20
#define MRC_EXT_INPUTS_INIT_VAL_VIEWPINCAL             (0)  // BIT21
#define MRC_EXT_INPUTS_INIT_VAL_QCLKDCC                (0)  // BIT22
#define MRC_EXT_INPUTS_INIT_VAL_WCKCLKPREDCC           (0)  // BIT23
#define MRC_EXT_INPUTS_INIT_VAL_DQSPADDCC              (0)  // BIT24
#define MRC_EXT_INPUTS_INIT_VAL_QCLKPHALIGN            (0)  // BIT25
#define MRC_EXT_INPUTS_INIT_VAL_RXDQSVOCC              (0)  // BIT26
#define MRC_EXT_INPUTS_INIT_VAL_ISENSERMT              (0)  // BIT27
#define MRC_EXT_INPUTS_INIT_VAL_WCKCLKRF               (0)  // BIT28
#define MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit29         (0)  // BIT29
#define MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit30         (0)  // BIT30
#define MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit31         (0)  // BIT31

#define MRC_EXT_INPUTS_INIT_VAL_MrcTimeMeasure         (0)
#define MRC_EXT_INPUTS_INIT_VAL_MrcFastBoot            (0)
#define MRC_EXT_INPUTS_INIT_VAL_DqPinsInterleaved      (0)
#define MRC_EXT_INPUTS_INIT_VAL_RankInterleave         (1)
#define MRC_EXT_INPUTS_INIT_VAL_EnhancedInterleave     (1)
#define MRC_EXT_INPUTS_INIT_VAL_MsHashOverride         (0)
#define MRC_EXT_INPUTS_INIT_VAL_ChHashEnable           (1)
#define MRC_EXT_INPUTS_INIT_VAL_EnablePwrDn            (1)
#define MRC_EXT_INPUTS_INIT_VAL_EnablePwrDnLpddr       (1)
#define MRC_EXT_INPUTS_INIT_VAL_SrefCfgEna             (1)
#define MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinDefeatLpddr  (1)
#define MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinDefeat       (1)
#define MRC_EXT_INPUTS_INIT_VAL_AutoSelfRefreshSupport (0)
#define MRC_EXT_INPUTS_INIT_VAL_ExtTemperatureSupport  (0)
#define MRC_EXT_INPUTS_INIT_VAL_AllowOppRefBelowWriteThrehold (0)
#define MRC_EXT_INPUTS_INIT_VAL_Force1Dpc              (0)
#define MRC_EXT_INPUTS_INIT_VAL_ForceSingleRank        (0)
#define MRC_EXT_INPUTS_INIT_VAL_DynamicMemoryBoost     (0)
#define MRC_EXT_INPUTS_INIT_VAL_RealtimeMemoryFrequency (0)
#define MRC_EXT_INPUTS_INIT_VAL_ExitOnFailure          (1)
#define MRC_EXT_INPUTS_INIT_VAL_ForceRetrainPath       (0)
#define MRC_EXT_INPUTS_INIT_VAL_SaGv                   (MRC_SAGV_DISABLED)
#define MRC_EXT_INPUTS_INIT_VAL_IsDdr5MR7WicaSupported (0)
#define MRC_EXT_INPUTS_INIT_VAL_VoltageCurrentSensor   (0)
#define MRC_EXT_INPUTS_INIT_VAL_DataInvertNibble       (0)
#define MRC_EXT_INPUTS_INIT_VAL_CccPinsInterleaved     (0)
#define MRC_EXT_INPUTS_INIT_VAL_EnPeriodicComp         (1)
#define MRC_EXT_INPUTS_INIT_VAL_AsyncOdtDis            (0)
#define MRC_EXT_INPUTS_INIT_VAL_OpportunisticRead      (1)
#define MRC_EXT_INPUTS_INIT_VAL_Disable2CycleBypass    (0)
#define MRC_EXT_INPUTS_INIT_VAL_OCSafeMode             (1)
#define MRC_EXT_INPUTS_INIT_VAL_HsleFlag               (0)

#define MRC_EXT_INPUTS_INIT_VAL_SrefCfgIdleTmr         (0x640)
#define MRC_EXT_INPUTS_INIT_VAL_ChHashMask             (0x830)
#define MRC_EXT_INPUTS_INIT_VAL_DdrFreqLimit           (0)
#define MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinTmr          (0)
#define MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinTmrLpddr     (0)
#define MRC_EXT_INPUTS_INIT_VAL_SaGvWpMask             (MRC_SAGV_MASK_0_1_2_3)
#define MRC_EXT_INPUTS_INIT_VAL_NModeSupport           (0)
#define MRC_EXT_INPUTS_INIT_VAL_McRefreshRate          (RefreshNORMAL)
#define MRC_EXT_INPUTS_INIT_VAL_PowerDownMode          (0xFF)
#define MRC_EXT_INPUTS_INIT_VAL_PwdwnIdleCounter       (0)
#define MRC_EXT_INPUTS_INIT_VAL_CmdRanksTerminated     (1)
#define MRC_EXT_INPUTS_INIT_VAL_MsHashMask             (0x2090)
#define MRC_EXT_INPUTS_INIT_VAL_Lp5CccConfig           (0xA2)
#define MRC_EXT_INPUTS_INIT_VAL_RMTLoopCount           (0)
#define MRC_EXT_INPUTS_INIT_VAL_MsHashInterleaveBit    (2)
#define MRC_EXT_INPUTS_INIT_VAL_GearRatio              (0)
#define MRC_EXT_INPUTS_INIT_VAL_DdrOneDpc              (3)
#define MRC_EXT_INPUTS_INIT_VAL_VddqVoltageOverride    (0)
#define MRC_EXT_INPUTS_INIT_VAL_VccIogVoltageOverride  (0)
#define MRC_EXT_INPUTS_INIT_VAL_VccClkVoltageOverride  (0)
#define MRC_EXT_INPUTS_INIT_VAL_LpddrRttWr             (0)
#define MRC_EXT_INPUTS_INIT_VAL_LpddrRttCa             (0)
#define MRC_EXT_INPUTS_INIT_VAL_SaGvFreq               {0,0,0,0}

#define MRC_EXT_INPUTS_INIT_VAL_SaGvGear                        {0,0,0,0}
#define MRC_EXT_INPUTS_INIT_VAL_IbeccProtectedRegionEnable      {0,0,0,0,0,0,0,0}
#define MRC_EXT_INPUTS_INIT_VAL_IbeccProtectedRegionBase        {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000}
#define MRC_EXT_INPUTS_INIT_VAL_IbeccProtectedRegionMask        {0x3fff,0x3fff,0x3fff,0x3fff,0x3fff,0x3fff,0x3fff,0x3fff}
#define MRC_EXT_INPUTS_INIT_VAL_MarginLimitL2                   (100)
#define MRC_EXT_INPUTS_INIT_VAL_WriteThreshold                  (0x00)
#define MRC_EXT_INPUTS_INIT_VAL_MarginLimitCheck                (0)
#define MRC_EXT_INPUTS_INIT_VAL_Lfsr0Mask                       (OneIn2To11)
#define MRC_EXT_INPUTS_INIT_VAL_Lfsr1Mask                       (OneIn2To11)
#define MRC_EXT_INPUTS_INIT_VAL_RefreshWm                       (1)
#define MRC_EXT_INPUTS_INIT_VAL_DramRfmMode                     (Rfm)
#define MRC_EXT_INPUTS_INIT_VAL_TargetedRowRefreshMode          (Ptrr)
#define MRC_EXT_INPUTS_INIT_VAL_DrfmBrc                         (DrfmBrc2)
#define MRC_EXT_INPUTS_INIT_VAL_CmdMirror                       (0)

#define MRC_EXT_INPUTS_INIT_VAL_SafeModeOverride                (0xFF)
#define MRC_EXT_INPUTS_INIT_VAL_DdrSafeMode                     (MRC_BIT2) // DDR_SAFE_DCC

#define MRC_EXT_INPUTS_INIT_VAL_McSafeMode                      (0)
#define MRC_EXT_INPUTS_INIT_VAL_LpMode                          (MRC_LPMODE_ALL)
#define MRC_EXT_INPUTS_INIT_VAL_LpMode4                         (MrcLpMode4_Enable)
#define MRC_EXT_INPUTS_INIT_VAL_PprTestType                     (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRFCpb                          (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRFC2                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRFC4                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRRD_L                          (0)
#define MRC_EXT_INPUTS_INIT_VAL_tRRD_S                          (0)
#define MRC_EXT_INPUTS_INIT_VAL_tWTR_L                          (0)
#define MRC_EXT_INPUTS_INIT_VAL_tCCD_L                          (0)
#define MRC_EXT_INPUTS_INIT_VAL_tWTR_S                          (0)
#define MRC_EXT_INPUTS_INIT_VAL_DimmOdt                         {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}
#define MRC_EXT_INPUTS_INIT_VAL_DFETAP1STEPSIZE                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_DFETAP2STEPSIZE                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_CsVrefLow                       (0)
#define MRC_EXT_INPUTS_INIT_VAL_CsVrefHigh                      (0)
#define MRC_EXT_INPUTS_INIT_VAL_CaVrefLow                       (0)
#define MRC_EXT_INPUTS_INIT_VAL_CaVrefHigh                      (0)
#define MRC_EXT_INPUTS_INIT_VAL_EccCorrectionMode               (0)
#define MRC_EXT_INPUTS_INIT_VAL_EccGranularity32BEn             (0)

// OffsetKnobs
#define MRC_EXT_INPUTS_INIT_VAL_CAVrefCtlOffset                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_VrefCtlOffset                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_ClkPiCodeOffset                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_RcvEnOffset                     (0)
#define MRC_EXT_INPUTS_INIT_VAL_RxDqsOffset                     (0)
#define MRC_EXT_INPUTS_INIT_VAL_TxDqOffset                      (0)
#define MRC_EXT_INPUTS_INIT_VAL_TxDqsOffset                     (0)
#define MRC_EXT_INPUTS_INIT_VAL_VrefOffset                      (0)
#define MRC_EXT_INPUTS_INIT_VAL_CntrlrMask                      (0x0)
#define MRC_EXT_INPUTS_INIT_VAL_ChMask                          (0x0)

#define MRC_EXT_INPUTS_INIT_VAL_LowerBasicMemTestSize           (0)
#define MRC_EXT_INPUTS_INIT_VAL_MCREGOFFSET                     (0)
#define MRC_EXT_INPUTS_INIT_VAL_PcuDdrVoltage                   (0xFF)

#define MRC_EXT_INPUTS_INIT_VAL_Trrsg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Trrdg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Trrdr                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Trrdd                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twrsg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twrdg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twrdr                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twrdd                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twwsg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twwdg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twwdr                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Twwdd                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Trwsg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Trwdg                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Trwdr                           (0)
#define MRC_EXT_INPUTS_INIT_VAL_Trwdd                           (0)

#define MRC_EXT_INPUTS_INIT_VAL_Interpreter                     (0)
#define MRC_EXT_INPUTS_INIT_VAL_IoOdtMode                       (MrcOdtModeDefault)
#define MRC_EXT_INPUTS_INIT_VAL_DynamicRangeLow                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_VddqVoltage                     (VDD_INVALID)
#define MRC_EXT_INPUTS_INIT_VAL_VppVoltage                      (VDD_INVALID)
#define MRC_EXT_INPUTS_INIT_VAL_RcompResistor                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_RcompTarget                     {0, 0, 0, 0, 0}
#define MRC_EXT_INPUTS_INIT_VAL_tCCD_L_WR                       (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_TYPE                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_RUN_ONCE                    (1)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_RUN_AT_FASTBOOT             (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_ERROR_INJECTION             (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_PHYSICAL_ADDRESS_LOW  (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_PHYSICAL_ADDRESS_HIGH (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_ROW                  (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_CONTROLLER           (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_CHANNEL              (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_DIMM                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_RANK                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_BANKGROUP            (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_BANK                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_PPR_FORCE_REPAIR                (0)
#define MRC_EXT_INPUTS_INIT_VAL_RunRefPiMaxVoltage              (0)
#define MRC_EXT_INPUTS_INIT_VAL_BOARDSTACKUP                    (0)
#define MRC_EXT_INPUTS_INIT_VAL_BOARDTOPOLOGY                   (0)
#define MRC_EXT_INPUTS_INIT_VAL_SingleVdd2Rail                  (0)
#define MRC_EXT_INPUTS_INIT_VAL_BRDRESERVED                     (0)
#define MRC_EXT_INPUTS_INIT_VAL_BRDRESERVED1                    (0)
#define MRC_EXT_INPUTS_INIT_VAL_SubChHashMask                   (0x838)
#define MRC_EXT_INPUTS_INIT_VAL_SubChHashInterleaveBit          (3)
#define MRC_EXT_INPUTS_INIT_VAL_ForceCkdBypass                  (0)

#define MRC_EXT_INPUTS_INIT_VAL_DprLock                  (0)
#define MRC_EXT_INPUTS_INIT_VAL_AllocateSharedMailbox    (0)
#define MRC_EXT_INPUTS_INIT_VAL_MimicWcDisaplayInIpq     (0)
#define MRC_EXT_INPUTS_INIT_VAL_FakeSagv                 (0)
#define MRC_EXT_INPUTS_INIT_VAL_DisableZq                (0)
#define MRC_EXT_INPUTS_INIT_VAL_ReplicateSagv            (0)
#define MRC_EXT_INPUTS_INIT_VAL_AdjustWckMode            (3)
#define MRC_EXT_INPUTS_INIT_VAL_TelemetryControl         (0)
#define MRC_EXT_INPUTS_INIT_VAL_SpineAndPhclkGateControl (0)
#define MRC_EXT_INPUTS_INIT_VAL_SpineGatePerLpmode       (0x7)//lpmode05=0x1,              lpmode2=0x1, lpmode3=0x1,lpmode4=0x0
#define MRC_EXT_INPUTS_INIT_VAL_PhclkGatePerLpmode       (0)  //lpmode05=0x0, lpmode1=0x0, lpmode2=0x0, lpmode3=0x0,lpmode4=0x0
#define MRC_EXT_INPUTS_INIT_VAL_DisableSwitchDfiToMc     (0)
#define MRC_EXT_INPUTS_INIT_VAL_DqLoopbackTest           (0)
#define MRC_EXT_INPUTS_INIT_VAL_DunitTatOptimization     (0)
#define MRC_EXT_INPUTS_INIT_VAL_Lp5SafeSpeed             (0)
#define MRC_EXT_INPUTS_INIT_VAL_SvReservedBits           (0)

#define MRC_EXT_INPUTS_INIT_VAL_RowPressEn                      (0)
#define MRC_EXT_INPUTS_INIT_VAL_WeaklockEn                      (MrcAuto)
#define MRC_EXT_INPUTS_INIT_VAL_RxDqsDelayCompEn                (MrcAuto)

#define MRC_EXT_INPUTS_INIT_VAL_RxDqsStepSizeLB          (1)
#define MRC_EXT_INPUTS_INIT_VAL_RxVrefStepSizeLB         (2)
#define MRC_EXT_INPUTS_INIT_VAL_TxStepSizeLB             (1)
#define MRC_EXT_INPUTS_INIT_VAL_ReservedBitsLB           (0)

#define MRC_EXT_INPUTS_INIT_VAL_MrcFailureOnUnsupportedDimm     (0)
#define MRC_EXT_INPUTS_INIT_VAL_WckModeOverride          (2)
#define MRC_EXT_INPUTS_INIT_VAL_Use1p5ReadPostamble      (0)

#define MRC_EXT_INPUTS_INIT_VAL_IBECCECCINJCONTROL              (0)
#define MRC_EXT_INPUTS_INIT_VAL_IBECCECCINJADDRBASE             (0x0)
#define MRC_EXT_INPUTS_INIT_VAL_IBECCECCINJCOUNT                (1)

#define MRC_EXT_INPUTS_INIT_VAL_IsWckIdleExitEnabled            (1)
#define MRC_EXT_INPUTS_INIT_VAL_Vdd2HVoltage                    (VDD_INVALID)
#define MRC_EXT_INPUTS_INIT_VAL_Vdd1Voltage                     (VDD_INVALID)
#define MRC_EXT_INPUTS_INIT_VAL_Vdd2LVoltage                    (VDD_INVALID)
#define MRC_EXT_INPUTS_INIT_VAL_Reserved344                     {0}

#ifdef MRC_MINIBIOS_BUILD
#define MRC_INT_INPUTS_INIT_VAL_LpFreqSwitch                            (0)
#define MRC_INT_INPUTS_INIT_VAL_ErrorCountForFail                       (MRC_BER_ERROR_COUNTER_FOR_FAILURE)
#define MRC_INT_INPUTS_INIT_VAL_BER                                     (1)
#define MRC_INT_INPUTS_INIT_VAL_TxtClean                                (0)
#define MRC_INT_INPUTS_INIT_VAL_EnablePda                               (1)
#define MRC_INT_INPUTS_INIT_VAL_Lp58BankMode                            (0)
#define MRC_INT_INPUTS_INIT_VAL_ReadMprVA                               (0)
#define MRC_INT_INPUTS_INIT_VAL_IsPmaMemConfigDoneSkip                  (0)
#define MRC_INT_INPUTS_INIT_VAL_PmaCceConfig                            (MrcCceDisable)
#define MRC_INT_INPUTS_INIT_VAL_IsCs2NRequested                         (0)
#define MRC_INT_INPUTS_INIT_VAL_MinAllowedNModeOvrd                     (CA_2_NMODE)
#define MRC_INT_INPUTS_INIT_VAL_SenseAtRxDll                            (1)
#define MRC_INT_INPUTS_INIT_VAL_LockUiDiv6Flow                          (0)
#define MRC_INT_INPUTS_INIT_VAL_MptuPropagationErrorFlow                (0)
#define MRC_INT_INPUTS_INIT_VAL_RloadTarget                             (1000)
#define MRC_INT_INPUTS_INIT_VAL_DiscardLvrAutoTrimResults               (0)
#define MRC_INT_INPUTS_INIT_VAL_PhClkSkipPhCorrection                   (0)
#define MRC_INT_INPUTS_INIT_VAL_PhClkCheckPhError                       (20)
#define MRC_INT_INPUTS_INIT_VAL_PhClkCheckDcError                       (10)
#endif

MRC_INPUT_BINARY_BLOCK_TYPE MainInputs = {
  /// SIGNATURE: ____MRCINPUT____
  {MRC_INPUT_BINARY_BLOCK_SIG0, MRC_INPUT_BINARY_BLOCK_SIG1},
  /// MrcExtInputs
  {
    MRC_EXT_INPUTS_INIT_VAL_Size,
    MRC_EXT_INPUTS_INIT_VAL_HobBufferSize,
    MRC_EXT_INPUTS_INIT_VAL_MemoryProfile,

    MRC_EXT_INPUTS_INIT_VAL_tCL,
    MRC_EXT_INPUTS_INIT_VAL_tRCDtRP,
    MRC_EXT_INPUTS_INIT_VAL_tRAS,
    MRC_EXT_INPUTS_INIT_VAL_tWR,
    MRC_EXT_INPUTS_INIT_VAL_tRFC,
    MRC_EXT_INPUTS_INIT_VAL_tRRD,
    MRC_EXT_INPUTS_INIT_VAL_tWTR,
    MRC_EXT_INPUTS_INIT_VAL_tRTP,
    MRC_EXT_INPUTS_INIT_VAL_tFAW,
    MRC_EXT_INPUTS_INIT_VAL_tCWL,
    MRC_EXT_INPUTS_INIT_VAL_tREFI,
    MRC_EXT_INPUTS_INIT_VAL_VddVoltage,
    MRC_EXT_INPUTS_INIT_VAL_Vdd2Mv,

    MRC_EXT_INPUTS_INIT_VAL_EccSupport,
    MRC_EXT_INPUTS_INIT_VAL_RsvdBit1,
    MRC_EXT_INPUTS_INIT_VAL_RemapEnable,
    MRC_EXT_INPUTS_INIT_VAL_ScramblerSupport,
    MRC_EXT_INPUTS_INIT_VAL_ProbelessTrace,
    MRC_EXT_INPUTS_INIT_VAL_PerBankRefresh,
    MRC_EXT_INPUTS_INIT_VAL_SimicsFlag,
    MRC_EXT_INPUTS_INIT_VAL_ExtendedBankHashing,
    MRC_EXT_INPUTS_INIT_VAL_SharedZqPin,
    MRC_EXT_INPUTS_INIT_VAL_UserThresholdEnable,
    MRC_EXT_INPUTS_INIT_VAL_RmtPerTask,
    MRC_EXT_INPUTS_INIT_VAL_TrainTrace,
    MRC_EXT_INPUTS_INIT_VAL_UserBudgetEnable,
    MRC_EXT_INPUTS_INIT_VAL_MsHashEnable,
    MRC_EXT_INPUTS_INIT_VAL_DisPgCloseIdleTimeout,
    MRC_EXT_INPUTS_INIT_VAL_Ibecc,
    MRC_EXT_INPUTS_INIT_VAL_IbeccParity,
    MRC_EXT_INPUTS_INIT_VAL_IbeccOperationMode,
    MRC_EXT_INPUTS_INIT_VAL_ChHashOverride,
    MRC_EXT_INPUTS_INIT_VAL_SimicsMinDbgMsg,
    MRC_EXT_INPUTS_INIT_VAL_RetrainOnFastFail,
    MRC_EXT_INPUTS_INIT_VAL_DvfsqEnabled,
    MRC_EXT_INPUTS_INIT_VAL_DvfscEnabled,
    MRC_EXT_INPUTS_INIT_VAL_VoltageReadout,
    MRC_EXT_INPUTS_INIT_VAL_CccHalfFrequency,
    MRC_EXT_INPUTS_INIT_VAL_Ddr5AutoPrechargeEnable,
    MRC_EXT_INPUTS_INIT_VAL_Lp5SplitACTEnable,
    MRC_EXT_INPUTS_INIT_VAL_RetrainToWorkingChannel,
    MRC_EXT_INPUTS_INIT_VAL_SubChHashOverride,
    MRC_EXT_INPUTS_INIT_VAL_DisableChannel,
    MRC_EXT_INPUTS_INIT_VAL_Ratio,

    MRC_EXT_INPUTS_INIT_VAL_ChHashInterleaveBit,
    MRC_EXT_INPUTS_INIT_VAL_SmramMask,
    // TrainingEnables
    {
      MRC_EXT_INPUTS_INIT_VAL_ECT,               // BIT0
      MRC_EXT_INPUTS_INIT_VAL_SOT,               // BIT1
      MRC_EXT_INPUTS_INIT_VAL_ERDMPRTC2D,        // BIT2
      MRC_EXT_INPUTS_INIT_VAL_RDMPRT,            // BIT3
      MRC_EXT_INPUTS_INIT_VAL_RCVET,             // BIT4
      MRC_EXT_INPUTS_INIT_VAL_JWRL,              // BIT5
      MRC_EXT_INPUTS_INIT_VAL_EWRTC2D,           // BIT6
      MRC_EXT_INPUTS_INIT_VAL_ERDTC2D,           // BIT7
      MRC_EXT_INPUTS_INIT_VAL_UNMATCHEDWRTC1D,   // BIT8
      MRC_EXT_INPUTS_INIT_VAL_WRTC1D,            // BIT9
      MRC_EXT_INPUTS_INIT_VAL_WRVC1D,            // BIT10
      MRC_EXT_INPUTS_INIT_VAL_RDTC1D,            // BIT11
      MRC_EXT_INPUTS_INIT_VAL_RDVC1D,            // BIT12
      MRC_EXT_INPUTS_INIT_VAL_WRTC2D,            // BIT13
      MRC_EXT_INPUTS_INIT_VAL_RDTC2D,            // BIT14
      MRC_EXT_INPUTS_INIT_VAL_WRVC2D,            // BIT15
      MRC_EXT_INPUTS_INIT_VAL_RDVC2D,            // BIT16
      MRC_EXT_INPUTS_INIT_VAL_WRDSEQT,           // BIT17
      MRC_EXT_INPUTS_INIT_VAL_DQSRF,             // BIT18
      MRC_EXT_INPUTS_INIT_VAL_RDDQSODTT,         // BIT19
      MRC_EXT_INPUTS_INIT_VAL_RDEQT,             // BIT20
      MRC_EXT_INPUTS_INIT_VAL_DUNITC,            // BIT21
      MRC_EXT_INPUTS_INIT_VAL_CMDVC,             // BIT22
      MRC_EXT_INPUTS_INIT_VAL_LCT,               // BIT23
      MRC_EXT_INPUTS_INIT_VAL_RTL,               // BIT24
      MRC_EXT_INPUTS_INIT_VAL_TAT,               // BIT25
      MRC_EXT_INPUTS_INIT_VAL_RMT,               // BIT26
      MRC_EXT_INPUTS_INIT_VAL_RMTEVENODD,        // BIT27
      MRC_EXT_INPUTS_INIT_VAL_ALIASCHK,          // BIT28
      MRC_EXT_INPUTS_INIT_VAL_RCVENC1D,          // BIT29
      MRC_EXT_INPUTS_INIT_VAL_RMC,               // BIT30
      MRC_EXT_INPUTS_INIT_VAL_PRETRAIN,          // BIT31
    },
    // TrainingEnables2
    {
      MRC_EXT_INPUTS_INIT_VAL_DCCPICODELUT,      // BIT0
      MRC_EXT_INPUTS_INIT_VAL_DIMMODTT,          // BIT1
      MRC_EXT_INPUTS_INIT_VAL_DIMMRONT,          // BIT2
      MRC_EXT_INPUTS_INIT_VAL_TXTCO,             // BIT3
      MRC_EXT_INPUTS_INIT_VAL_CLKTCO,            // BIT4
      MRC_EXT_INPUTS_INIT_VAL_CMDSR,             // BIT5
      MRC_EXT_INPUTS_INIT_VAL_CMDDSEQ,           // BIT6
      MRC_EXT_INPUTS_INIT_VAL_DIMMODTCA,         // BIT7
      MRC_EXT_INPUTS_INIT_VAL_DDR5ODTTIMING,     // BIT8
      MRC_EXT_INPUTS_INIT_VAL_DBI,               // BIT9
      MRC_EXT_INPUTS_INIT_VAL_DLLDCC,            // BIT10
      MRC_EXT_INPUTS_INIT_VAL_DLLBWSEL,          // BIT11
      MRC_EXT_INPUTS_INIT_VAL_RDVREFDC,          // BIT12
      MRC_EXT_INPUTS_INIT_VAL_RDTCIDLE,          // BIT13
      MRC_EXT_INPUTS_INIT_VAL_RMTBIT,            // BIT14
      MRC_EXT_INPUTS_INIT_VAL_DQDQSSWZ,          // BIT15
      MRC_EXT_INPUTS_INIT_VAL_REFPI,             // BIT16
      MRC_EXT_INPUTS_INIT_VAL_DCCLP5READDCA,     // BIT17
      MRC_EXT_INPUTS_INIT_VAL_VCCCLKFF,          // BIT18
      MRC_EXT_INPUTS_INIT_VAL_FUNCDCCDQS,        // BIT19
      MRC_EXT_INPUTS_INIT_VAL_FUNCDCCCLK,        // BIT20
      MRC_EXT_INPUTS_INIT_VAL_FUNCDCCWCK,        // BIT21
      MRC_EXT_INPUTS_INIT_VAL_FUNCDCCDQ,         // BIT22
      MRC_EXT_INPUTS_INIT_VAL_DATAPILIN,         // BIT23
      MRC_EXT_INPUTS_INIT_VAL_DDR5XTALK,         // BIT24
      MRC_EXT_INPUTS_INIT_VAL_DCCLP5WCKDCA,      // BIT25
      MRC_EXT_INPUTS_INIT_VAL_RXUNMATCHEDCAL,    // BIT26
      MRC_EXT_INPUTS_INIT_VAL_WRTDIMMDFE,        // BIT27
      MRC_EXT_INPUTS_INIT_VAL_RMTLVR,            // BIT28
      MRC_EXT_INPUTS_INIT_VAL_DCCDDR5READDCA,    // BIT29
      MRC_EXT_INPUTS_INIT_VAL_WREQT,             // BIT30
      MRC_EXT_INPUTS_INIT_VAL_SimicsReservedBit, // BIT31
    },
    // TrainingEnables3
    {
      MRC_EXT_INPUTS_INIT_VAL_RXDQSDCC,          // BIT0
      MRC_EXT_INPUTS_INIT_VAL_DIMMNTODT,         // BIT1
      MRC_EXT_INPUTS_INIT_VAL_TXDQSDCC,          // BIT2
      MRC_EXT_INPUTS_INIT_VAL_RXVREFPERBIT,      // BIT3
      MRC_EXT_INPUTS_INIT_VAL_PPR,               // BIT4
      MRC_EXT_INPUTS_INIT_VAL_LVRAUTOTRIM,       // BIT5
      MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit6,     // BIT6
      MRC_EXT_INPUTS_INIT_VAL_OPTIMIZECOMP,      // BIT7
      MRC_EXT_INPUTS_INIT_VAL_WRTRETRAIN,        // BIT8
      MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit9,     // BIT9
      MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit10,    // BIT10
      MRC_EXT_INPUTS_INIT_VAL_JEDECRESET,        // BIT11
      MRC_EXT_INPUTS_INIT_VAL_ROUNDTRIPMATCH,    // BIT12
      MRC_EXT_INPUTS_INIT_VAL_TLINECLKCAL,       // BIT13
      MRC_EXT_INPUTS_INIT_VAL_DCCPISERIALCAL,    // BIT14
      MRC_EXT_INPUTS_INIT_VAL_PHASECLKCAL,       // BIT15
      MRC_EXT_INPUTS_INIT_VAL_WCKPADDCCCAL,      // BIT16
      MRC_EXT_INPUTS_INIT_VAL_RDCTLET,           // BIT17
      MRC_EXT_INPUTS_INIT_VAL_RDDQODTT,          // BIT18
      MRC_EXT_INPUTS_INIT_VAL_EMPHASIS,          // BIT19
      MRC_EXT_INPUTS_INIT_VAL_DIMMRXOFFSET,      // BIT20
      MRC_EXT_INPUTS_INIT_VAL_VIEWPINCAL,        // BIT21
      MRC_EXT_INPUTS_INIT_VAL_QCLKDCC,           // BIT22
      MRC_EXT_INPUTS_INIT_VAL_WCKCLKPREDCC,      // BIT23
      MRC_EXT_INPUTS_INIT_VAL_DQSPADDCC,         // BIT24
      MRC_EXT_INPUTS_INIT_VAL_QCLKPHALIGN,       // BIT25
      MRC_EXT_INPUTS_INIT_VAL_RXDQSVOCC,         // BIT26
      MRC_EXT_INPUTS_INIT_VAL_ISENSERMT,         // BIT27
      MRC_EXT_INPUTS_INIT_VAL_WCKCLKRF,          // BIT28
      MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit29,    // BIT29
      MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit30,    // BIT30
      MRC_EXT_INPUTS_INIT_VAL_Reserved3Bit31,    // BIT31
    },
    MRC_EXT_INPUTS_INIT_VAL_MrcTimeMeasure,
    MRC_EXT_INPUTS_INIT_VAL_MrcFastBoot,
    MRC_EXT_INPUTS_INIT_VAL_DqPinsInterleaved,
    MRC_EXT_INPUTS_INIT_VAL_RankInterleave,
    MRC_EXT_INPUTS_INIT_VAL_EnhancedInterleave,
    MRC_EXT_INPUTS_INIT_VAL_MsHashOverride,
    MRC_EXT_INPUTS_INIT_VAL_ChHashEnable,
    MRC_EXT_INPUTS_INIT_VAL_EnablePwrDn,
    MRC_EXT_INPUTS_INIT_VAL_EnablePwrDnLpddr,
    MRC_EXT_INPUTS_INIT_VAL_SrefCfgEna,
    MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinDefeatLpddr,
    MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinDefeat,
    MRC_EXT_INPUTS_INIT_VAL_AutoSelfRefreshSupport,
    MRC_EXT_INPUTS_INIT_VAL_ExtTemperatureSupport,
    MRC_EXT_INPUTS_INIT_VAL_AllowOppRefBelowWriteThrehold,
    MRC_EXT_INPUTS_INIT_VAL_Force1Dpc,
    MRC_EXT_INPUTS_INIT_VAL_ForceSingleRank,
    MRC_EXT_INPUTS_INIT_VAL_DynamicMemoryBoost,
    MRC_EXT_INPUTS_INIT_VAL_RealtimeMemoryFrequency,
    MRC_EXT_INPUTS_INIT_VAL_ExitOnFailure,
    MRC_EXT_INPUTS_INIT_VAL_ForceRetrainPath,
    MRC_EXT_INPUTS_INIT_VAL_SaGv,
    MRC_EXT_INPUTS_INIT_VAL_IsDdr5MR7WicaSupported,
    MRC_EXT_INPUTS_INIT_VAL_VoltageCurrentSensor,
    MRC_EXT_INPUTS_INIT_VAL_DataInvertNibble,
    MRC_EXT_INPUTS_INIT_VAL_CccPinsInterleaved,
    MRC_EXT_INPUTS_INIT_VAL_EnPeriodicComp,
    MRC_EXT_INPUTS_INIT_VAL_AsyncOdtDis,
    MRC_EXT_INPUTS_INIT_VAL_OpportunisticRead,
    MRC_EXT_INPUTS_INIT_VAL_Disable2CycleBypass,
    MRC_EXT_INPUTS_INIT_VAL_OCSafeMode,
    MRC_EXT_INPUTS_INIT_VAL_HsleFlag,

    MRC_EXT_INPUTS_INIT_VAL_SrefCfgIdleTmr,
    MRC_EXT_INPUTS_INIT_VAL_ChHashMask,
    MRC_EXT_INPUTS_INIT_VAL_DdrFreqLimit,
    MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinTmr,
    MRC_EXT_INPUTS_INIT_VAL_ThrtCkeMinTmrLpddr,
    MRC_EXT_INPUTS_INIT_VAL_SaGvWpMask,
    MRC_EXT_INPUTS_INIT_VAL_NModeSupport,
    MRC_EXT_INPUTS_INIT_VAL_McRefreshRate,
    MRC_EXT_INPUTS_INIT_VAL_PowerDownMode,
    MRC_EXT_INPUTS_INIT_VAL_PwdwnIdleCounter,
    MRC_EXT_INPUTS_INIT_VAL_CmdRanksTerminated,
    MRC_EXT_INPUTS_INIT_VAL_MsHashMask,
    MRC_EXT_INPUTS_INIT_VAL_Lp5CccConfig,
    MRC_EXT_INPUTS_INIT_VAL_RMTLoopCount,
    MRC_EXT_INPUTS_INIT_VAL_MsHashInterleaveBit,
    MRC_EXT_INPUTS_INIT_VAL_GearRatio,
    MRC_EXT_INPUTS_INIT_VAL_DdrOneDpc,
    MRC_EXT_INPUTS_INIT_VAL_VddqVoltageOverride,
    MRC_EXT_INPUTS_INIT_VAL_VccIogVoltageOverride,
    MRC_EXT_INPUTS_INIT_VAL_VccClkVoltageOverride,
    MRC_EXT_INPUTS_INIT_VAL_LpddrRttWr,
    MRC_EXT_INPUTS_INIT_VAL_LpddrRttCa,
    MRC_EXT_INPUTS_INIT_VAL_SaGvFreq,
    MRC_EXT_INPUTS_INIT_VAL_SaGvGear,
    MRC_EXT_INPUTS_INIT_VAL_IbeccProtectedRegionEnable,
    MRC_EXT_INPUTS_INIT_VAL_IbeccProtectedRegionBase,
    MRC_EXT_INPUTS_INIT_VAL_IbeccProtectedRegionMask,
    MRC_EXT_INPUTS_INIT_VAL_MarginLimitL2,
    MRC_EXT_INPUTS_INIT_VAL_WriteThreshold,
    MRC_EXT_INPUTS_INIT_VAL_MarginLimitCheck,
    MRC_EXT_INPUTS_INIT_VAL_Lfsr0Mask,
    MRC_EXT_INPUTS_INIT_VAL_Lfsr1Mask,
    MRC_EXT_INPUTS_INIT_VAL_RefreshWm,
    MRC_EXT_INPUTS_INIT_VAL_DramRfmMode,
    MRC_EXT_INPUTS_INIT_VAL_TargetedRowRefreshMode,
    MRC_EXT_INPUTS_INIT_VAL_DrfmBrc,
    MRC_EXT_INPUTS_INIT_VAL_CmdMirror,
    MRC_EXT_INPUTS_INIT_VAL_SafeModeOverride,
    MRC_EXT_INPUTS_INIT_VAL_DdrSafeMode,
    MRC_EXT_INPUTS_INIT_VAL_McSafeMode,
    MRC_EXT_INPUTS_INIT_VAL_LpMode,
    MRC_EXT_INPUTS_INIT_VAL_LpMode4,
    MRC_EXT_INPUTS_INIT_VAL_PprTestType,
    MRC_EXT_INPUTS_INIT_VAL_tRFCpb,
    MRC_EXT_INPUTS_INIT_VAL_tRFC2,
    MRC_EXT_INPUTS_INIT_VAL_tRFC4,
    MRC_EXT_INPUTS_INIT_VAL_tRRD_L,
    MRC_EXT_INPUTS_INIT_VAL_tRRD_S,
    MRC_EXT_INPUTS_INIT_VAL_tWTR_L,
    MRC_EXT_INPUTS_INIT_VAL_tCCD_L,
    MRC_EXT_INPUTS_INIT_VAL_tWTR_S,
    MRC_EXT_INPUTS_INIT_VAL_DimmOdt,
    MRC_EXT_INPUTS_INIT_VAL_DFETAP1STEPSIZE,
    MRC_EXT_INPUTS_INIT_VAL_DFETAP2STEPSIZE,
    MRC_EXT_INPUTS_INIT_VAL_CsVrefLow,
    MRC_EXT_INPUTS_INIT_VAL_CsVrefHigh,
    MRC_EXT_INPUTS_INIT_VAL_CaVrefLow,
    MRC_EXT_INPUTS_INIT_VAL_CaVrefHigh,
    MRC_EXT_INPUTS_INIT_VAL_EccCorrectionMode,
    MRC_EXT_INPUTS_INIT_VAL_EccGranularity32BEn,
    // OffsetKnobs
    {
      MRC_EXT_INPUTS_INIT_VAL_CAVrefCtlOffset,
      MRC_EXT_INPUTS_INIT_VAL_VrefCtlOffset,
      MRC_EXT_INPUTS_INIT_VAL_ClkPiCodeOffset,
      MRC_EXT_INPUTS_INIT_VAL_RcvEnOffset,
      MRC_EXT_INPUTS_INIT_VAL_RxDqsOffset,
      MRC_EXT_INPUTS_INIT_VAL_TxDqOffset,
      MRC_EXT_INPUTS_INIT_VAL_TxDqsOffset,
      MRC_EXT_INPUTS_INIT_VAL_VrefOffset,
      MRC_EXT_INPUTS_INIT_VAL_CntrlrMask,
      MRC_EXT_INPUTS_INIT_VAL_ChMask,
    },

    MRC_EXT_INPUTS_INIT_VAL_LowerBasicMemTestSize,
    MRC_EXT_INPUTS_INIT_VAL_MCREGOFFSET,
    MRC_EXT_INPUTS_INIT_VAL_PcuDdrVoltage,

    MRC_EXT_INPUTS_INIT_VAL_Trrsg,
    MRC_EXT_INPUTS_INIT_VAL_Trrdg,
    MRC_EXT_INPUTS_INIT_VAL_Trrdr,
    MRC_EXT_INPUTS_INIT_VAL_Trrdd,
    MRC_EXT_INPUTS_INIT_VAL_Twrsg,
    MRC_EXT_INPUTS_INIT_VAL_Twrdg,
    MRC_EXT_INPUTS_INIT_VAL_Twrdr,
    MRC_EXT_INPUTS_INIT_VAL_Twrdd,
    MRC_EXT_INPUTS_INIT_VAL_Twwsg,
    MRC_EXT_INPUTS_INIT_VAL_Twwdg,
    MRC_EXT_INPUTS_INIT_VAL_Twwdr,
    MRC_EXT_INPUTS_INIT_VAL_Twwdd,
    MRC_EXT_INPUTS_INIT_VAL_Trwsg,
    MRC_EXT_INPUTS_INIT_VAL_Trwdg,
    MRC_EXT_INPUTS_INIT_VAL_Trwdr,
    MRC_EXT_INPUTS_INIT_VAL_Trwdd,

    MRC_EXT_INPUTS_INIT_VAL_Interpreter,
    MRC_EXT_INPUTS_INIT_VAL_IoOdtMode,
    MRC_EXT_INPUTS_INIT_VAL_DynamicRangeLow,
    MRC_EXT_INPUTS_INIT_VAL_VddqVoltage,
    MRC_EXT_INPUTS_INIT_VAL_VppVoltage,
    MRC_EXT_INPUTS_INIT_VAL_RcompResistor,
    MRC_EXT_INPUTS_INIT_VAL_RcompTarget,
    MRC_EXT_INPUTS_INIT_VAL_tCCD_L_WR,

    // PPR defaults
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_TYPE,
    MRC_EXT_INPUTS_INIT_VAL_PPR_RUN_ONCE,
    MRC_EXT_INPUTS_INIT_VAL_PPR_RUN_AT_FASTBOOT,
    MRC_EXT_INPUTS_INIT_VAL_PPR_ERROR_INJECTION,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_PHYSICAL_ADDRESS_LOW,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_PHYSICAL_ADDRESS_HIGH,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_ROW,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_CONTROLLER,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_CHANNEL,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_DIMM,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_RANK,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_BANKGROUP,
    MRC_EXT_INPUTS_INIT_VAL_PPR_REPAIR_BANK,
    MRC_EXT_INPUTS_INIT_VAL_PPR_FORCE_REPAIR,
    MRC_EXT_INPUTS_INIT_VAL_RunRefPiMaxVoltage,

    /// MrcBoardInputs
    {
      MRC_EXT_INPUTS_INIT_VAL_BOARDSTACKUP,                         ///< Offset 189 BIT0 - Board Stack Up: 0 - Typical, 1 - Frequency Limited
      MRC_EXT_INPUTS_INIT_VAL_BOARDTOPOLOGY,                        ///< Offset 189 BIT1 - Board Topology: 0 - Daisy Chain, 1 - Tee
      MRC_EXT_INPUTS_INIT_VAL_SingleVdd2Rail,                       ///< Offset 189 BIT2 - LP5x VDD2 rail: <b>0=Dual rail (E-DVFSC is possible)</b>, 1=Single rail(No E-DVFSC; VDD2L == VDD2H).
      MRC_EXT_INPUTS_INIT_VAL_BRDRESERVED,                          ///< Offset 189 BIT[7:3] - Reserved for future use
      {MRC_EXT_INPUTS_INIT_VAL_BRDRESERVED1},                       ///< Reserved bytes for struct DWORD alignment
    },
    MRC_EXT_INPUTS_INIT_VAL_SubChHashMask,
    MRC_EXT_INPUTS_INIT_VAL_SubChHashInterleaveBit,
    MRC_EXT_INPUTS_INIT_VAL_ForceCkdBypass,

    MRC_EXT_INPUTS_INIT_VAL_DprLock,
    MRC_EXT_INPUTS_INIT_VAL_AllocateSharedMailbox,
    MRC_EXT_INPUTS_INIT_VAL_MimicWcDisaplayInIpq,
    MRC_EXT_INPUTS_INIT_VAL_FakeSagv,
    MRC_EXT_INPUTS_INIT_VAL_DisableZq,
    MRC_EXT_INPUTS_INIT_VAL_ReplicateSagv,
    MRC_EXT_INPUTS_INIT_VAL_AdjustWckMode,
    MRC_EXT_INPUTS_INIT_VAL_TelemetryControl,
    MRC_EXT_INPUTS_INIT_VAL_SpineAndPhclkGateControl,
    MRC_EXT_INPUTS_INIT_VAL_SpineGatePerLpmode,
    MRC_EXT_INPUTS_INIT_VAL_PhclkGatePerLpmode,
    MRC_EXT_INPUTS_INIT_VAL_DisableSwitchDfiToMc,
    MRC_EXT_INPUTS_INIT_VAL_DqLoopbackTest,
    MRC_EXT_INPUTS_INIT_VAL_DunitTatOptimization,
    MRC_EXT_INPUTS_INIT_VAL_Lp5SafeSpeed,
    MRC_EXT_INPUTS_INIT_VAL_SvReservedBits,
    MRC_EXT_INPUTS_INIT_VAL_RowPressEn,
    MRC_EXT_INPUTS_INIT_VAL_WeaklockEn,
    MRC_EXT_INPUTS_INIT_VAL_RxDqsDelayCompEn,
    MRC_EXT_INPUTS_INIT_VAL_RxDqsStepSizeLB,
    MRC_EXT_INPUTS_INIT_VAL_RxVrefStepSizeLB,
    MRC_EXT_INPUTS_INIT_VAL_TxStepSizeLB,
    MRC_EXT_INPUTS_INIT_VAL_ReservedBitsLB,
    MRC_EXT_INPUTS_INIT_VAL_MrcFailureOnUnsupportedDimm,
    MRC_EXT_INPUTS_INIT_VAL_WckModeOverride,
    MRC_EXT_INPUTS_INIT_VAL_Use1p5ReadPostamble,

    MRC_EXT_INPUTS_INIT_VAL_IBECCECCINJCONTROL,
    MRC_EXT_INPUTS_INIT_VAL_IBECCECCINJADDRBASE,
    MRC_EXT_INPUTS_INIT_VAL_IBECCECCINJCOUNT,

    MRC_EXT_INPUTS_INIT_VAL_IsWckIdleExitEnabled,
    MRC_EXT_INPUTS_INIT_VAL_Vdd2HVoltage,
    MRC_EXT_INPUTS_INIT_VAL_Vdd1Voltage,
    MRC_EXT_INPUTS_INIT_VAL_Vdd2LVoltage,
    MRC_EXT_INPUTS_INIT_VAL_Reserved344,
  },
#ifdef MRC_MINIBIOS_BUILD
  /// MRC_INT_INPUTS_TYPE
  {
    MRC_INT_INPUTS_INIT_VAL_LpFreqSwitch,
    MRC_INT_INPUTS_INIT_VAL_ErrorCountForFail,
    MRC_INT_INPUTS_INIT_VAL_BER,
    MRC_INT_INPUTS_INIT_VAL_TxtClean,
    MRC_INT_INPUTS_INIT_VAL_EnablePda,
    MRC_INT_INPUTS_INIT_VAL_Lp58BankMode,
    MRC_INT_INPUTS_INIT_VAL_ReadMprVA,
    MRC_INT_INPUTS_INIT_VAL_IsPmaMemConfigDoneSkip,
    MRC_INT_INPUTS_INIT_VAL_PmaCceConfig,
    MRC_INT_INPUTS_INIT_VAL_IsCs2NRequested,
    MRC_INT_INPUTS_INIT_VAL_MinAllowedNModeOvrd,
    MRC_INT_INPUTS_INIT_VAL_SenseAtRxDll,
    MRC_INT_INPUTS_INIT_VAL_LockUiDiv6Flow,
    MRC_INT_INPUTS_INIT_VAL_MptuPropagationErrorFlow,
    MRC_INT_INPUTS_INIT_VAL_RloadTarget,
    MRC_INT_INPUTS_INIT_VAL_DiscardLvrAutoTrimResults,
    MRC_INT_INPUTS_INIT_VAL_PhClkSkipPhCorrection,
    MRC_INT_INPUTS_INIT_VAL_PhClkCheckPhError,
    MRC_INT_INPUTS_INIT_VAL_PhClkCheckDcError,
  },
#endif
  MRC_INPUT_BINARY_BLOCK_END
};

/**
  Copy over static data structure MainInputs to the pointer passed in the address of the ExternalInputs

  @param[out] ExternalInputsPtr - the address of the ExternalInputs.

**/
VOID
InitExternalInputsDefault (
  OUT MRC_EXT_INPUTS_TYPE *ExternalInputsPtr
  )
{
#ifndef MRC_MINIBIOS_BUILD
  CopyMem((UINT8 *)ExternalInputsPtr, (UINT8 *)&MainInputs.ExternalInputs, sizeof (MRC_EXT_INPUTS_TYPE));
#else
  MrcOemMemoryCpy ((UINT8 *)ExternalInputsPtr, (UINT8 *)&MainInputs.ExternalInputs, sizeof (MRC_EXT_INPUTS_TYPE));
#endif
}

#ifdef MRC_MINIBIOS_BUILD
/**
  Mrc internal inputs initilization

  @param Inputs - the address of the MrcInput data on the stack.

**/
VOID
MrcInternalInputsDefaultInit (
  OUT MrcInput *Inputs
  )
{
  Inputs->LpFreqSwitch              = MainInputs.InternalInputs.LpFreqSwitch;
  Inputs->ErrorCountForFail         = MainInputs.InternalInputs.ErrorCountForFail;
  Inputs->BER                       = MainInputs.InternalInputs.BER;
  Inputs->TxtClean                  = MainInputs.InternalInputs.TxtClean;
  Inputs->EnablePda                 = MainInputs.InternalInputs.EnablePda;
  Inputs->Lp58BankMode              = MainInputs.InternalInputs.Lp58BankMode;
  Inputs->ReadMprVA                 = MainInputs.InternalInputs.ReadMprVA;
  Inputs->IsPmaMemConfigDoneSkip    = MainInputs.InternalInputs.IsPmaMemConfigDoneSkip;
  Inputs->PmaCceConfig              = MainInputs.InternalInputs.PmaCceConfig;
  Inputs->IsCs2NRequested           = MainInputs.InternalInputs.IsCs2NRequested;
  Inputs->MinAllowedNModeOvrd       = MainInputs.InternalInputs.MinAllowedNModeOvrd;
  Inputs->SenseAtRxDll              = MainInputs.InternalInputs.SenseAtRxDll;
  Inputs->LockUiDiv6Flow            = MainInputs.InternalInputs.LockUiDiv6Flow;
  Inputs->MptuPropagationErrorFlow  = MainInputs.InternalInputs.MptuPropagationErrorFlow;
  Inputs->RloadTarget               = MainInputs.InternalInputs.RloadTarget;
  Inputs->DiscardLvrAutoTrimResults = MainInputs.InternalInputs.DiscardLvrAutoTrimResults;
  Inputs->PhClkSkipPhCorrection     = MainInputs.InternalInputs.PhClkSkipPhCorrection;
  Inputs->PhClkCheckPhError         = MainInputs.InternalInputs.PhClkCheckPhError;
  Inputs->PhClkCheckDcError         = MainInputs.InternalInputs.PhClkCheckDcError;
}

/**
  Copy over static data structure MainInputs to the pointer passed in the address of the MrcInput data on the stack

  @param Inputs - the address of the MrcInput data on the stack.

**/
VOID
MrcInputsDefaultInit (
  OUT MrcInput *Inputs
  )
{
  MrcInternalInputsDefaultInit (Inputs);
  InitExternalInputsDefault (Inputs->ExtInputs.Ptr);
}

/**
 * Print External Inputs as a binary dump for verification.
 *
 * @param[in] Inputs - the address of the External Inputs
 * @param[in] Debug  - Debug print
 */
VOID
ExtInputBinaryDump (
  IN MRC_EXT_INPUTS_TYPE *ExtInputs,
  IN MrcDebug            *Debug
  )
{
  const UINT8     *Buffer;
  UINT16          Index;

  Buffer = (UINT8 *) ExtInputs;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nDumping External Inputs:\n");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Index 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
  for (Index = 0; Index < sizeof (MRC_EXT_INPUTS_TYPE); Index++) {
    if (Index % 0x10 == 0) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n%04X: ", Index);
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%02X ", Buffer[Index]);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
}
#endif // MRC_MINIBIOS_BUILD
