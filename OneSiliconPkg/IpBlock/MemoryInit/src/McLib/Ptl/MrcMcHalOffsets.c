/** @file
  This file contains functions to get Memory Controller (MC) Offsets
  for timings, refresh, scheduler, and anything else MC specific.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include "CMcAddress.h"
#include "MrcCommon.h"


/**
  Function used to get the CR Offset for Memory Controller Timings.

  @param[in]  Group      - MC Timing Group being accessed.
  @param[in]  Socket     - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Controller - Memory controller in the processor socket (0-based).
  @param[in]  Channel    - DDR Channel Number within the memory controller (0-based).
  @param[in]  FreqIndex  - Index supporting multiple operating frequencies. (Not used in Client CPU's)

  @retval CR Offset.
**/
UINT32
MrcGetMcTimingRegOffset (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Socket,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32    Offset;
  UINT8     MaxChannel;

  Offset     = MRC_UINT32_MAX;
  MaxChannel = 2; // MAX_MC_CHANNEL


  switch (Group) {
    case GsmMctRP:
    case GsmMctRPab:
    case GsmMctRAS:
    case GsmMctnWRDiff:
    case GsmMctRDPRE:
    case GsmMctPPD:
    case GsmMctWRPRE:
    case GsmMctLpDeratingExt:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_PRE_REG, MC1_CH0_CR_TC_PRE_REG, Controller, MC0_CH1_CR_TC_PRE_REG, Channel);
      }
      break;

    case GsmMctRCD:
    case GsmMctRCDw:
    case GsmMctFAW:
    case GsmMctRRDsg:
    case GsmMctRRDdg:
    case GsmMctRDA2ACT:
    case GsmMctWRA2ACT:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_ACT_REG, MC1_CH0_CR_TC_ACT_REG, Controller, MC0_CH1_CR_TC_ACT_REG, Channel);
      }
      break;

    case GsmMctVrcgEnable:
    case GsmMctVrcgDisable:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_DVFSQ_CTL_REG, MC1_CH0_CR_DVFSQ_CTL_REG, Controller, MC0_CH1_CR_DVFSQ_CTL_REG, Channel);
      }
      break;

    case GsmtccdCasDelta:
    case GsmMctCL:
    case GsmMctCWL:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_CAS_REG, MC1_CH0_CR_TC_CAS_REG, Controller, MC0_CH1_CR_TC_CAS_REG, Channel);
      }
      break;

    case GsmMctRDRDsg:
    case GsmMctRDRDdg:
    case GsmMctRDRDdr:
    case GsmMctRDRDdd:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_RDRD_REG, MC1_CH0_CR_TC_RDRD_REG, Controller, MC0_CH1_CR_TC_RDRD_REG, Channel);
      }
      break;

    case GsmMctRDWRsg:
    case GsmMctRDWRdg:
    case GsmMctRDWRdr:
    case GsmMctRDWRdd:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_RDWR_REG, MC1_CH0_CR_TC_RDWR_REG, Controller, MC0_CH1_CR_TC_RDWR_REG, Channel);
      }
      break;

    case GsmMctWRRDsg:
    case GsmMctWRRDdg:
    case GsmMctWRRDdr:
    case GsmMctWRRDdd:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_WRRD_REG, MC1_CH0_CR_TC_WRRD_REG, Controller, MC0_CH1_CR_TC_WRRD_REG, Channel);
      }
      break;

    case GsmMctWRWRsg:
    case GsmMctWRWRdg:
    case GsmMctWRWRdr:
    case GsmMctWRWRdd:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_WRWR_REG, MC1_CH0_CR_TC_WRWR_REG, Controller, MC0_CH1_CR_TC_WRWR_REG, Channel);
      }
      break;

    case GsmMctCKE:
    case GsmMctXP:
    case GsmMctPRPDEN:
    case GsmMctRDPDEN:
    case GsmMctWRPDEN:
    case GsmMctCA2CS:
    case GsmMctCSL:
    case GsmMctCSH:
    case GsmMctCKCKEH:
    case GsmMctCPDED:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_PWRDN_REG, MC1_CH0_CR_TC_PWRDN_REG, Controller, MC0_CH1_CR_TC_PWRDN_REG, Channel);
      }
      break;

    case GsmMcttOSCO:
    case GsmMctWrOsclRuntime:
    case GsmMctRdOsclRuntime:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_RETRAINING_OSCL_REG, MC1_CH0_CR_TC_RETRAINING_OSCL_REG, Controller, MC0_CH1_CR_TC_RETRAINING_OSCL_REG, Channel);
      }
      break;

    case GsmMcttPREMRR:
    case GsmMcttMRR:
    case GsmMctMRD:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_MR_REG, MC1_CH0_CR_TC_MR_REG, Controller, MC0_CH1_CR_TC_MR_REG, Channel);
      }
      break;

    case GsmMcttCSLCK:
    case GsmMcttWckStop:
    case GSmMcttWckOff:
    case GsmMcttCkFspX:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_WCK_REG, MC1_CH0_CR_TC_WCK_REG, Controller, MC0_CH1_CR_TC_WCK_REG, Channel);
      }
      break;

    case GsmMctWckEnlFs:
    case GsmMctWckEnlRd:
    case GsmMctWckEnlWr:
    case GsmMctWckPreToggleFs:
    case GsmMctWckPreToggleRd:
    case GsmMctWckPreToggleWr:
    case GsmMctWckDfiStop:
    case GsmMctWckDfiOffset:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_WCK_CONFIG_REG, MC1_CH0_CR_WCK_CONFIG_REG, Controller, MC0_CH1_CR_WCK_CONFIG_REG, Channel);
      }
      break;

    case GsmMctXSDLL:
    case GsmMctXSR:
    case GsmMctLpMode4SrxDelay:
    case GsmMctSR:
    case GsmMctXSRDs:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_SREXITTP_REG, MC1_CH0_CR_TC_SREXITTP_REG, Controller, MC0_CH1_CR_TC_SREXITTP_REG, Channel);
      }
      break;

    case GsmMctMrhAfterCommandDelay:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH(MC0_CH0_CR_MRH_CONFIG_REG, MC1_CH0_CR_MRH_CONFIG_REG, Controller, MC0_CH1_CR_MRH_CONFIG_REG, Channel);
      }
      break;

    case GsmMctZQCAL:
    case GsmMcttZQLatch:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_ZQCAL_REG, MC1_CH0_CR_TC_ZQCAL_REG, Controller, MC0_CH1_CR_TC_ZQCAL_REG, Channel);
      }
      break;

    case GsmMctZQCalPeriod:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH(MC0_CH0_CR_ZQCAL_CONTROL_REG, MC1_CH0_CR_ZQCAL_CONTROL_REG, Controller, MC0_CH1_CR_ZQCAL_CONTROL_REG, Channel);
      }
      break;

    case GsmMctSrIdleTimer:
      Offset = OFFSET_CALC_CH (MC0_PM_OPP_SREF_IDLE_TIMER_REG, MC1_PM_OPP_SREF_IDLE_TIMER_REG, Controller);
      break;

    case GsmMctSrefDelayQsync:
      Offset = OFFSET_CALC_CH (MC0_MCDECS_SECOND_CBIT_REG, MC1_MCDECS_SECOND_CBIT_REG, Controller);
      break;

    case GsmMctRFCpb:
    case GsmMctREFI:
    case GsmMctRFC:
    case GsmMctREFSbRd:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_RFTP_REG, MC1_CH0_CR_TC_RFTP_REG, Controller, MC0_CH1_CR_TC_RFTP_REG, Channel);
      }
      break;

    case GsmMctRFM:
    case GsmMctDRFM:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_REFM_REG, MC1_CH0_CR_TC_REFM_REG, Controller, MC0_CH1_CR_TC_REFM_REG, Channel);
      }
      break;

    case GsmMcDdr5Rir:
    case GsmMctRefreshHpWm:
    case GsmMctRefreshPanicWm:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_RFP_REG, MC1_CH0_CR_RFP_REG, Controller, MC0_CH1_CR_RFP_REG, Channel);
      }
      break;

    case GsmMctCWLAdd:
    case GsmMctCWLDec:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_WR_DELAY_REG, MC1_CH0_CR_SC_WR_DELAY_REG, Controller, MC0_CH1_CR_SC_WR_DELAY_REG, Channel);
      }
      break;

    default:
      break;
  }

  return Offset;
}


/**
  Function used to get the CR Offset for Memory Controller Configuration Settings.

  @param[in]  Group     - PMA Group being accessed.

  @retval CR Offset.
**/
UINT32
MrcGetPmaRegOffset (
  IN  MrcParameters *MrcData,
  IN  GSM_GT        Group,
  IN  UINT32        FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32    Offset;

  Offset     = MRC_UINT32_MAX;

  switch (Group) {
    case GsmPmaEnableMc:
    case GsmPmaEnableIbecc:
    case GsmPmaEnableCce:
    case GsmPmaEnableTme:
    case GsmPmaDdrType:
      Offset = MEMSS_PMA_CR_BIOS_MEM_CONFIG_REG;
      break;

    case GsmPmaMaxBandwidth:
      switch (FreqIndex) {
        case MrcSaGvPoint0:
          Offset = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_0_REG;
          break;

        case MrcSaGvPoint1:
          Offset = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_1_REG;
          break;

        case MrcSaGvPoint2:
          Offset = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_2_REG;
          break;

        case MrcSaGvPoint3:
          Offset = MEMSS_PMA_CR_BIOS_MAX_BANDWIDTH_3_REG;
          break;
      }
      break;

    case GsmPmaQclkRatioReq:
    case GsmPmaGearTypeReq:
    case GsmPmaDvfsVddq:
    case GsmPmaVddqChangeOnly:
    case GsmPmaMaxBwGbps:
    case GsmPmaQclkWpIdx:
    case GsmPmaRunBusy:
      Offset = MEMSS_PMA_CR_BIOS_REQ_REG;
      VolatileMask->Data = MEMSS_PMA_CR_BIOS_REQ_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmPmaMrcSave:
    case GsmPmaMemConfigDone:
    case GsmPmaMemConfigDoneAck:
      Offset = MEMSS_PMA_CR_INIT_STATE_REG;
      VolatileMask->Data = MEMSS_PMA_CR_INIT_STATE_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmPmaQclkRatioData:
    case GsmPmaGearTypeData:
      Offset = MEMSS_PMA_CR_BIOS_DATA_REG;
      VolatileMask->Data = MEMSS_PMA_CR_BIOS_DATA_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmPmaErrorCode:
      Offset = MEMSS_PMA_CR_BIOS_ERROR_STATUS_REG;
      VolatileMask->Data = MEMSS_PMA_CR_BIOS_ERROR_STATUS_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmPmaBiosMailboxReserved:
      Offset = MEMSS_PMA_CR_BIOS_MAILBOX_REG;
      break;

    default:
      break;
  }
  return Offset;
}


/**
  This function returns the offset for the MRS FSM Control register.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  Controller  - Controller index.
  @param[in]  Channel     - Channel index.

  @retval Register Offset.
**/
UINT32
MrcGetMrsFsmCtlOffset (
  IN  MrcParameters *MrcData,
  IN  UINT32        Controller,
  IN  UINT32        Channel
  )
{
  UINT32 Offset;

  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_FSM_CONTROL_REG, MC1_CH0_CR_MRS_FSM_CONTROL_REG, Controller, MC0_CH1_CR_MRS_FSM_CONTROL_REG, Channel);

  return Offset;
}


/**
  This function returns the offset for MRS run FSM Control register.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  Controller  - Controller index.
  @param[in]  Channel     - Channel index.

  @retval Register Offset.
**/
UINT32
MrcGetMrsRunFsmOffset (
  IN  MrcParameters *MrcData,
  IN  UINT32        Controller,
  IN  UINT32        Channel
  )
{
  UINT32 Offset;

  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_FSM_RUN_REG, MC1_CH0_CR_MRS_FSM_RUN_REG, Controller, MC0_CH1_CR_MRS_FSM_RUN_REG, Channel);

  return Offset;
}


typedef enum {
  MrrLowerBytes,
  MrrUpperBytes,
  MrrEccByte,
  MrrResultByteMax
} MrrResultByte;


static const UINT32 MrrResultOffset[MrrResultByteMax][MAX_SUB_CHANNEL][3] = { //3 Byte groups (Lower, Upper, Ecc), 2 Sub Channels, 3 Offset groups
  { {MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG, MC1_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG, MC0_CH1_CR_SCH0_DDR_MR_RESULT_LOW_REG},
    {MC0_CH0_CR_SCH1_DDR_MR_RESULT_LOW_REG, MC1_CH0_CR_SCH1_DDR_MR_RESULT_LOW_REG, MC0_CH1_CR_SCH1_DDR_MR_RESULT_LOW_REG}},
  { {MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG, MC1_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG, MC0_CH1_CR_SCH0_DDR_MR_RESULT_HIGH_REG},
    {MC0_CH0_CR_SCH1_DDR_MR_RESULT_HIGH_REG, MC1_CH0_CR_SCH1_DDR_MR_RESULT_HIGH_REG, MC0_CH1_CR_SCH1_DDR_MR_RESULT_HIGH_REG}},
  { {MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG, MC1_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG, MC0_CH1_CR_SCH0_DDR_MR_RESULT_ECC_REG},
    {MC0_CH0_CR_SCH1_DDR_MR_RESULT_ECC_REG, MC1_CH0_CR_SCH1_DDR_MR_RESULT_ECC_REG, MC0_CH1_CR_SCH1_DDR_MR_RESULT_ECC_REG}}
  };



/**
  Function used to get the CR Offset for Memory Controller Configuration Settings.

  @param[in]  Group      - MC Timing Group being accessed.
  @param[in]  Socket     - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Controller - Memory controller in the processor socket (0-based).
  @param[in]  Channel    - DDR Channel Number within the memory controller (0-based).
  @param[in]  Rank       - Rank number within a channel (0-based).
  @param[in]  Strobe     - Strobe number within a channel (0-based).
  @param[in]  FreqIndex  - Index supporting multiple operating frequencies. (Not used in Client CPU's)

  @retval CR Offset.
**/
UINT32
MrcGetMcConfigRegOffset (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Socket,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  )
{
  UINT32    Offset;
  UINT8     MaxChannel;
  MrcOutput *Outputs;
  BOOLEAN   IsLpddr;
  UINT32    IpChannel;
  UINT32    SubCh;
  MrrResultByte MrrByte;

  Outputs = &MrcData->Outputs;
  IsLpddr   = Outputs->IsLpddr;
  Offset     = MRC_UINT32_MAX;
  MaxChannel = 2; // MAX_MC_CHANNEL
  // Channel is already an IP channel for most of the MC Config items.
  // There are few exceptions that use a system-level channel, see MrcTranslateSystemToIp.
  // For example, RrdValid and Deswizzle registers.
  // IpChannel variable is used for those.
  IpChannel = LP_IP_CH (IsLpddr, Channel);

  switch (Group) {
    case GsmMccMaskCs:
    case GsmMccIgnoreCke:
    case GsmMccBlockXarb:
    case GsmMccBlockCke:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_GS_CFG_TRAINING_REG, MC1_CH0_CR_SC_GS_CFG_TRAINING_REG , Controller, MC0_CH1_CR_SC_GS_CFG_TRAINING_REG, Channel);
      }
      break;

    case GsmMccDramType:
    case GsmMccOneDpc:
    case GsmMccCpgcInOrder:
    case GsmMccCmdStretch:
    case GsmMccAddrMirror:
    case GsmMccFreqPoint:
    case GsmMccEnableOdtMatrix:
    case GsmMccGearMode:
    case GsmMccWCKDiffLowInIdle:
    case GsmMccMultiCycCmd:
    case GsmMccLp5BankMode:
    case GsmMccEnCsGearDown:
    case GsmMccDdr5CkdEnable:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_GS_CFG_REG, MC1_CH0_CR_SC_GS_CFG_REG, Controller, MC0_CH1_CR_SC_GS_CFG_REG, Channel);
      }
      break;

    case GsmMccCmdTriStateDis:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH(MC0_CH0_CR_SC_GS_CFG_UARCH_REG, MC1_CH0_CR_SC_GS_CFG_UARCH_REG, Controller, MC0_CH1_CR_SC_GS_CFG_UARCH_REG, Channel);
      }
      break;

    case GsmMccPageOpenPolicyMaxCount:
    case GsmMccPageOpenPolicyEn:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH(MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_REG, MC1_CH0_CR_PAGEOPEN_POLICY_LIMIT_REG, Controller, MC0_CH1_CR_PAGEOPEN_POLICY_LIMIT_REG, Channel);
      }
      break;

    case GsmMccLp5WckMode:
    case GsmMccLp5WckFastMode:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_WCK_CONFIG_REG, MC1_CH0_CR_WCK_CONFIG_REG, Controller, MC0_CH1_CR_WCK_CONFIG_REG, Channel);
      }
      break;

    case GsmMccDisLpddr5RdwrInterleaving:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MCSCHEDS_SPARE_REG, MC1_CH0_CR_MCSCHEDS_SPARE_REG, Controller, MC0_CH1_CR_MCSCHEDS_SPARE_REG, Channel);
      }
      break;

    case GsmMccEccCorrectionDisable:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_ECC_DFT_REG, MC1_CH0_CR_ECC_DFT_REG, Controller, MC0_CH1_CR_ECC_DFT_REG, Channel);
      }
      break;

    case GsmMccMcMntsSpareRw:
    case GsmMccMcMntsRfFastSleepDisable:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MCMNTS_SPARE_REG, MC1_CH0_CR_MCMNTS_SPARE_REG, Controller, MC0_CH1_CR_MCMNTS_SPARE_REG, Channel);
      }
      break;

    case GsmDisVc1CplInterleave:
    case GsmDisAllCplInterleave:
      Offset = OFFSET_CALC_CH (MC0_MC_CFI_CONTROL_REG, MC1_MC_CFI_CONTROL_REG, Controller);
      break;

    case GsmMccRhLfsr0:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_RH_TRR_LFSR0_REG, MC1_CH0_CR_RH_TRR_LFSR0_REG, Controller, MC0_CH1_CR_RH_TRR_LFSR0_REG, Channel);
      }
      break;

    case GsmMccRhLfsr1:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_RH_TRR_LFSR1_REG, MC1_CH0_CR_RH_TRR_LFSR1_REG, Controller, MC0_CH1_CR_RH_TRR_LFSR1_REG, Channel);
      }
      break;

    case GsmMccRhTRRDimmEnable:
    case GsmMccRhDrfmEnable:
    case GsmMccRhLfsr0Mask:
    case GsmMccRhLfsr1Mask:
    case GsmMccRhMA1Swizzle:
    case GsmMccRhMA2Swizzle:
    case GsmMccMicronFSwizzle:
    case GsmMccMicronRSwizzle:
    case GsmMccBrc:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_RH_TRR_CONTROL_REG, MC1_CH0_CR_RH_TRR_CONTROL_REG, Controller, MC0_CH1_CR_RH_TRR_CONTROL_REG, Channel);
      }
      break;

    case GsmMccRhEnableDimm0:
    case GsmMccRhEnableDimm1:
    case GsmMccRhHighWM:
    case GsmMccRhLowWM:
    case GsmMccRhNormalRefSub:
    case GsmMccRhRefmSub:
    case GsmMccRhRefmEn:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_ROWHAMMER_CTL_REG, MC1_CH0_CR_ROWHAMMER_CTL_REG, Controller, MC0_CH1_CR_ROWHAMMER_CTL_REG, Channel);
      }
      break;

    case GsmMccOppSrefEnable:
      Offset = OFFSET_CALC_CH (MC0_PM_OPP_SREF_ENABLE_REG, MC1_PM_OPP_SREF_ENABLE_REG, Controller);
      VolatileMask->Data = MC0_PM_OPP_SREF_ENABLE_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccForceSRState:
      Offset = OFFSET_CALC_CH (MC0_NORMALMODE_CFG_REG, MC1_NORMALMODE_CFG_REG, Controller);
      VolatileMask->Data = MC0_NORMALMODE_CFG_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccVC0ReadEntries:
    case GsmMccVC1ReadEntries:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MC_RDB_CREDITS_REG, MC1_CH0_CR_MC_RDB_CREDITS_REG, Controller, MC0_CH1_CR_MC_RDB_CREDITS_REG, Channel);
      }
      break;

    case GsmMccSpineGateDis:
      Offset = OFFSET_CALC_CH(MC0_CLOCK_SPINE_GATE_CFG_REG, MC1_CLOCK_SPINE_GATE_CFG_REG, Controller);
      break;

   case GsmMccMainGlbDrvGateDis:
     Offset = OFFSET_CALC_CH(MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG, MC1_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG, Controller);
     break;

   case GsmMccSchedGlbDrvGateDis:
    if (Channel < MaxChannel) {
      Offset = OFFSET_CALC_MC_CH(MC0_CH0_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_REG, MC1_CH0_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_REG, Controller, MC0_CH1_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_REG, Channel);
    }
    break;

    case GsmMccRdWckAsyncGap:
    case GsmMccWrWckAsyncGap:
    case GsmMccCasStopAdditionalGap:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TC_WCK_REG, MC1_CH0_CR_TC_WCK_REG, Controller, MC0_CH1_CR_TC_WCK_REG, Channel);
      }
      break;

    case GsmMccCpgcActive:
      Offset =  OFFSET_CALC_CH (MC0_MC_CPGC_CONTROL_REG, MC1_MC_CPGC_CONTROL_REG, Controller);
      break;

    case GsmMccInOrderIngress:
      Offset = OFFSET_CALC_CH (MC0_MC_CPGC_CONTROL_UARCH_REG, MC1_MC_CPGC_CONTROL_UARCH_REG, Controller);
      break;

    case GsmMccEccMode:
    case GsmMccAddrDecodeDdrType:
    case GsmMccCh0Size:
    case GsmMccCh1Size:
    // No Broadcast supported for this.
      Offset = OFFSET_CALC_CH (MC0_MAD_INTER_CHANNEL_REG, MC1_MAD_INTER_CHANNEL_REG, Controller);
      break;

    case GsmMccEnhancedInterleave:
    case GsmMccExtendedBankHash:
    case GsmMccBg0Hash:
      // No Broadcast supported for this.
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_MAD_DRAM_HASH_0_REG, MC1_MAD_DRAM_HASH_0_REG, Controller, MC0_MAD_DRAM_HASH_1_REG, Channel);
      }
      break;

    case GsmMccSubch0RankCnt:
    case GsmMccSubch1RankCnt:
    case GsmMccSubch1SdramWidth:
    case GsmMccSubch0SdramWidth:
    case GsmMccSubch0Density:
    case GsmMccSubch1Density:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH ( MC0_MAD_INTRA_CHANNEL_0_REG,  MC1_MAD_INTRA_CHANNEL_0_REG, Controller,  MC0_MAD_INTRA_CHANNEL_1_REG, Channel);
      }
      break;

    case GsmMccHashEnabled:
    case GsmMccHashLsb:
    case GsmMccHashZone1Start:
      Offset =  OFFSET_CALC_CH (MC0_MAD_MC_HASH_REG, MC1_MAD_MC_HASH_REG, Controller);
      break;

    case GsmMccEnableRefresh:
    case GsmMccEnableDclk:
    case GsmMccPureSrx:
    case GsmMccSrState:
      Offset = OFFSET_CALC_CH (MC0_MC_INIT_STATE_G_REG, MC1_MC_INIT_STATE_G_REG, Controller);
      VolatileMask->Data = MC0_MC_INIT_STATE_G_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccRankOccupancy:
    case GsmMccMcSrx:
    case GsmMccLpddrCurrentFsp:
    case GsmMccLpddrCurrentFspTrk:
      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MC_INIT_STATE_REG, MC1_CH0_CR_MC_INIT_STATE_REG, Controller, MC0_CH1_CR_MC_INIT_STATE_REG, Channel);
      VolatileMask->Data = MC0_CH0_CR_MC_INIT_STATE_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccEnRefTypeDisplay:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MC_REFRESH_STAGGER_REG, MC1_CH0_CR_MC_REFRESH_STAGGER_REG, Controller, MC0_CH1_CR_MC_REFRESH_STAGGER_REG, Channel);
      }
      break;

    case GsmMccHashMask:
    case GsmMccLsbMaskBit:
      Offset = OFFSET_CALC_CH (MC0_MAD_CHANNEL_HASH_REG, MC1_MAD_CHANNEL_HASH_REG, Controller);
      break;

    case GsmMccCmdBgfAlwaysOn:
    case GsmMccDisStarvedPriorityOnNewReq:
    case GsmMccDisAsyncOdt:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCHED_SECOND_CBIT_REG, MC1_CH0_CR_SCHED_SECOND_CBIT_REG, Controller, MC0_CH1_CR_SCHED_SECOND_CBIT_REG, Channel);
      }
      break;

    case GsmMccPbrDis:
    case GsmMccRefreshAbrRelease:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_RFP_REG, MC1_CH0_CR_RFP_REG, Controller, MC0_CH1_CR_RFP_REG, Channel);
      }
      break;

    case GsmMccCkeOverride:
    case GsmMccCkeOn:
    case GsmMccCsOverride0:
    case GsmMccCsOverrideVal0:
    case GsmMccCsOverride1:
    case GsmMccCsOverrideVal1:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_REG, MC1_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_REG, Controller, MC0_CH1_CR_REUT_CH_MISC_CKE_CS_CTRL_REG, Channel);
      }
      break;

    case GsmMccRrdValidTrigger:
    case GsmMccRrdValidOverflow:
      // Note: using system-level channel here, because the register fields are per subch in LP5 (like RoundTripDelay)
      if (IpChannel < MAX_CHANNEL_SHARE_REGS) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TR_RRDVALID_CTRL_REG, MC1_CH0_CR_TR_RRDVALID_CTRL_REG, Controller, MC0_CH1_CR_TR_RRDVALID_CTRL_REG, IpChannel);
      }
      VolatileMask->Data = MC0_CH0_CR_TR_RRDVALID_CTRL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccRrdValidValue:
    case GsmMccRrdValidSign:
      // Note: using system-level channel here, because the register fields are per subch in LP5 (like RoundTripDelay)
      if (IpChannel < MAX_CHANNEL_SHARE_REGS) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_TR_RRDVALID_DATA_REG, MC1_CH0_CR_TR_RRDVALID_DATA_REG, Controller, MC0_CH1_CR_TR_RRDVALID_DATA_REG, IpChannel);
      }
      VolatileMask->Data = MC0_CH0_CR_TR_RRDVALID_DATA_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccMinRefRate:
    case GsmMccMaxRefRate:
    case GsmMccMR4Period:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_PM_CONFIG_THERM_STATUS_REG, MC1_CH0_CR_PM_CONFIG_THERM_STATUS_REG, Controller, MC0_CH1_CR_PM_CONFIG_THERM_STATUS_REG, Channel);
      }
      break;

    case GsmMccZqSerialize:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_ZQCAL_CONTROL_REG, MC1_CH0_CR_ZQCAL_CONTROL_REG, Controller, MC0_CH1_CR_ZQCAL_CONTROL_REG, Channel);
      }
      break;

    case GsmMccDeswizzleByte:
       // Note: Channel is a system-level channel here, because we need the per-subch resolution for LP5
      if (IsLpddr) {
        if ((Channel == 0) || (Channel == 2)) {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG, MC1_CH0_CR_SCH0_BYTE_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH0_BYTE_DESWIZZLE_REG, IpChannel);
        } else {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH1_BYTE_DESWIZZLE_REG, MC1_CH0_CR_SCH1_BYTE_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH1_BYTE_DESWIZZLE_REG, IpChannel);
        }
      } else { // DDR5
        if (Rank < 2) {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG, MC1_CH0_CR_SCH0_BYTE_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH0_BYTE_DESWIZZLE_REG, IpChannel);
        } else {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH1_BYTE_DESWIZZLE_REG, MC1_CH0_CR_SCH1_BYTE_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH1_BYTE_DESWIZZLE_REG, IpChannel);
        }
      }
      break;

    case GsmMccDeswizzleBit:
        // Note: Channel is a system-level channel here, because we need the per-subch resolution for LP5
      if (IsLpddr) {
        if ((Channel == 0) || (Channel == 2)) {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG, MC1_CH0_CR_SCH0_BIT_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH0_BIT_DESWIZZLE_REG, IpChannel);
        } else {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH1_BIT_DESWIZZLE_REG, MC1_CH0_CR_SCH1_BIT_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH1_BIT_DESWIZZLE_REG, IpChannel);
        }
      } else { // DDR5
        if (Rank < 2) {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG, MC1_CH0_CR_SCH0_BIT_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH0_BIT_DESWIZZLE_REG, IpChannel);
        } else {
          Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCH1_BIT_DESWIZZLE_REG, MC1_CH0_CR_SCH1_BIT_DESWIZZLE_REG, Controller, MC0_CH1_CR_SCH1_BIT_DESWIZZLE_REG, IpChannel);
        }
      }
      break;

    case GsmMccMrrResult:
      // Note: Channel is a system-level channel here, because we need the per-subch resolution for LP5
      if (IsLpddr) {
        SubCh = Channel % MAX_SUB_CHANNEL;
      } else { // DDR5
        SubCh = Rank / MAX_RANK_IN_DIMM;
      }
      if (Strobe < 2) {
        MrrByte = MrrLowerBytes;
      } else if (Strobe < 4) {
        MrrByte = MrrUpperBytes;
      } else {
        MrrByte = MrrEccByte;
      }
      Offset = OFFSET_CALC_MC_CH (MrrResultOffset[MrrByte][SubCh][0], MrrResultOffset[MrrByte][SubCh][1], Controller, MrrResultOffset[MrrByte][SubCh][2], IpChannel);
      break;

    case GsmMccRefreshRankMask:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_REG, MC1_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_REG, Controller, MC0_CH1_CR_REUT_CH_MISC_REFRESH_CTRL_REG, Channel);
      }
      break;

    case GsmMccAllowRHDebtSR:
      Offset = OFFSET_CALC_CH (MC0_MCDECS_SECOND_CBIT_REG, MC1_MCDECS_SECOND_CBIT_REG, Controller);
      break;

    case GsmMccOdtMatrixRd:
    case GsmMccOdtMatrixWr:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_ODT_MATRIX_REG, MC1_CH0_CR_SC_ODT_MATRIX_REG, Controller, MC0_CH1_CR_SC_ODT_MATRIX_REG, Channel);
      }
      break;

    case GsmMccMainDisClkGate:
    case GsmMccMainDisRegClkGate:
    case GsmMccDisGlbDrvClkGate:
    case GsmMccDisIosfSbClkGate:
      Offset = OFFSET_CALC_CH (MC0_MCDECS_CBIT_REG, MC1_MCDECS_CBIT_REG, Controller);
      VolatileMask->Data = MC0_MCDECS_CBIT_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccPtrSepOspid:
    case GsmMccPtrSepIspid:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MC2PHY_BGF_CTRL_REG, MC1_CH0_CR_MC2PHY_BGF_CTRL_REG, Controller, MC0_CH1_CR_MC2PHY_BGF_CTRL_REG, Channel);
      }
      VolatileMask->Data = MC0_CH0_CR_MC2PHY_BGF_CTRL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccAllowOppRefBelowWrtThreshold:
    case GsmMccDisCkett:
    case GsmMccTTIdleCounter:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_REG, MC1_CH0_CR_PM_PDWN_CONFIG_UARCH_REG, Controller, MC0_CH1_CR_PM_PDWN_CONFIG_UARCH_REG, Channel);
      }
      VolatileMask->Data = MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccWrtThreshold:
    case GsmMccPdwnIdleCounterSubch0:
    case GsmMccPdwnIdleCounterSubch1:
    case GsmMccPdwnLpmodeEnable:
    case GsmMccAPD:
    case GsmMccPPD:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_PM_PDWN_CONFIG_REG, MC1_CH0_CR_PM_PDWN_CONFIG_REG, Controller, MC0_CH1_CR_PM_PDWN_CONFIG_REG, Channel);
      }
      VolatileMask->Data = MC0_CH0_CR_PM_PDWN_CONFIG_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccEnSrxTempRead:
    case GsmMccSrxZqcal:
    case GsmMccForceSreWithFreqChange:
    case GsmMccEnSrxWrRetraining:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_PM_SREF_CONTROL_REG, MC1_CH0_CR_PM_SREF_CONTROL_REG, Controller, MC0_CH1_CR_PM_SREF_CONTROL_REG, Channel);
      }
      break;

    case GsmMccEnWrRetraining:
    case GsmMccEnRdRetraining:
    case GsmMccRetrainPeriod:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_RETRAINING_OSCL_PARAMS_REG, MC1_CH0_CR_RETRAINING_OSCL_PARAMS_REG, Controller, MC0_CH1_CR_RETRAINING_OSCL_PARAMS_REG, Channel);
      }
      break;

    case GsmMccSchedDisClkGateLocal:
    case GsmMccDis2cByp:
    case GsmMccPageIdleTimerDisable:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCHED_CBIT_REG, MC1_CH0_CR_SCHED_CBIT_REG, Controller, MC0_CH1_CR_SCHED_CBIT_REG, Channel);
      }
      break;

    case GsmMccDisSchedsClkGate:
    case GsmMccDisMntClkGate:
    case GsmMccDisWdbClkGate:
    case GsmMccDisableSplitAct:
    case GsmMccMcCbRowPressDisAutoPre:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SCHED_THIRD_CBIT_REG, MC1_CH0_CR_SCHED_THIRD_CBIT_REG, Controller, MC0_CH1_CR_SCHED_THIRD_CBIT_REG, Channel);
      }
      break;

    case GsmMccVc0LatencyGaurdTimerx16:
    case GsmMccLatencySensitiveTimerx16:
    case GsmMccVc1LatencyGuardTimerx8:
    case GsmMccVc1IsocLatencyGuardTimerx8:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_PR_CNT_CONFIG_REG, MC1_CH0_CR_SC_PR_CNT_CONFIG_REG, Controller, MC0_CH1_CR_SC_PR_CNT_CONFIG_REG, Channel);
      }
      break;

    case GsmMccSelfRefreshEnable:
    case GsmMccSelfRefreshLength:
    case GsmMccPowerDownEnable:
    case GsmMccIdleEnable:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SPID_LOW_POWER_CTL_REG, MC1_CH0_CR_SPID_LOW_POWER_CTL_REG, Controller, MC0_CH1_CR_SPID_LOW_POWER_CTL_REG, Channel);
      }
      VolatileMask->Data = MC0_CH0_CR_SPID_LOW_POWER_CTL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccLpMode4En:
      Offset = OFFSET_CALC_CH (MC0_PM_OPP_SR_POLICY_0_REG, MC1_PM_OPP_SR_POLICY_0_REG, Controller);
      break;

    case GsmMccCmiSourceId0:
    case GsmMccCmiSourceId1:
    case GsmMccCmiSourceId2:
    case GsmMccCmiSourceId3:
      Offset = OFFSET_CALC_CH (MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_REG, MC1_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_REG, Controller);
      break;

    case GsmMccDelayPMAckCycles:
      Offset = OFFSET_CALC_CH (MC0_PM_CONTROL_REG, MC1_PM_CONTROL_REG, Controller);
      VolatileMask->Data = MC0_PM_CONTROL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccEcDis:
    case GsmMccEccGranularity:
      Offset = OFFSET_CALC_CH (MC0_IBECC_CONTROL_REG, MC1_IBECC_CONTROL_REG, Controller);
      VolatileMask->Data = MC0_IBECC_CONTROL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccAutoPrechargeEn:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_AUTO_PRE_CONTROL_REG, MC1_CH0_CR_AUTO_PRE_CONTROL_REG, Controller, MC0_CH1_CR_AUTO_PRE_CONTROL_REG, Channel);
      }
      VolatileMask->Data = MC0_CH0_CR_AUTO_PRE_CONTROL_VOLATILE_BITFIELDS_MSK;
      break;

    case GsmMccCfiE2eParityEnRdata:
    case GsmMccCfiE2eParityEnRdCmplHeader:
    case GsmMccCfiE2eParityEn:
      Offset = OFFSET_CALC_CH (MC0_PARITY_CONTROL_REG, MC1_PARITY_CONTROL_REG, Controller);
      break;

    case GsmScPbrEcsRefabEnable:
    case GsmScPbrEcsRefabPeriod:
      if (Channel < MaxChannel) {
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_PBR_REG, MC1_CH0_CR_SC_PBR_REG, Controller, MC0_CH1_CR_SC_PBR_REG, Channel);
      }
      break;

    default:
      break;
  }

  return Offset;
}

