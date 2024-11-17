/** @file
  The functions in this file implement the memory controller registers that
  are not training specific. After these functions are executed, the
  memory controller will be ready to execute the timing training sequences.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
//
// Include files
//
#include "MrcAddressDecodeConfiguration.h"
#include "MrcPowerModes.h"
#include "MrcMcConfiguration.h"
#include "MrcMemoryApi.h"
#include "MrcLpddr5.h"
#include "MrcDdr5.h"
#include "MrcDdr5Registers.h"
#include "MrcCommon.h"
#include "Cpgc20.h"
#include "MrcCpgcOffsets.h"
#include "MrcCpgcApi.h"
#include "MrcChipApi.h"
#include "MrcDdrIoDefines.h"
#include "MrcPowerMetering.h"

/**
  This function provides disabling of periodic ZQ latch during runtime DVFSQ_LOW
  The purpose is to allow MC to disable ZQ-latch during runtime DVFSQ_LOW

  @param[in] MrcData - The MRC global data.
**/
VOID
DisablePeriodicZqLatch (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  UINT8         Controller;
  UINT8         Channel;
  UINT32        Offset;
  BOOLEAN       IsLpddr;
  UINT32        IpChannel;
  MC0_CH0_CR_MCMNTS_SPARE2_STRUCT  McMntsSpare2;

  Outputs   = &MrcData->Outputs;
  IsLpddr   = Outputs->IsLpddr;
  Debug     = &Outputs->Debug;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        continue; // For LPDDR5, only program registers on even channels.
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MCMNTS_SPARE2_REG, MC1_CH0_CR_MCMNTS_SPARE2_REG, Controller, MC0_CH1_CR_MCMNTS_SPARE2_REG, IpChannel);
      McMntsSpare2.Data = MrcReadCR (MrcData, Offset);
      McMntsSpare2.Bits.Spare_RW |= (MRC_BIT1);

      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Writing McMntsSpare2.Data: %d\n", McMntsSpare2.Data);
      MrcWriteCR (MrcData, Offset, McMntsSpare2.Data);
    }
  }
}

/**
  This function finalizes the Memory Controller configuration to prepare for normal operation.
  Some items are set to special values for training that need to be set to the POR value.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful
**/
MrcStatus
MrcMcFinalize (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput   *Inputs;
  MrcOutput        *Outputs;
  MrcDebug         *Debug;
  INT64            GetSetVal;
  INT64            GetSetEn;
  INT64            GetSetDis;
  UINT32           Controller;
  UINT32           Channel;
  BOOLEAN          IsLpddr;
  UINT32           IpChannel;
  UINT32           Offset;
  UINT32           Data32;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Debug = &MrcData->Outputs.Debug;
  IsLpddr = Outputs->IsLpddr;
  GetSetDis = 0;
  GetSetEn = 1;

  // Configure Power Down CR
  MrcPowerDownConfig (MrcData);

  // Configure DDR5 1DPC performance feature
  MrcMcOneDpcConfig (MrcData);

  // Configure LPModes
  MrcLpMode4Enable (MrcData);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!(MrcControllerExist (MrcData, Controller))) {
      continue;
    }

    MrcGetSetMc(MrcData, Controller, GsmMccSpineGateDis, WriteToCache, &GetSetDis);

    MrcGetSetMc(MrcData, Controller, GsmMccMainGlbDrvGateDis, WriteToCache, &GetSetDis);

    GetSetVal = ExtInputs->SrefCfgIdleTmr;
    MrcGetSetMc (MrcData, Controller, GsmMctSrIdleTimer, WriteToCache, &GetSetVal);

    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!(MrcChannelExist (MrcData, Controller, Channel))  || IS_MC_SUB_CH (IsLpddr, Channel)) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);

      if (ExtInputs->LpMode & MRC_LPMODE0P5) {
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccIdleEnable, WriteToCache | PrintValue, &GetSetEn);
      }

      MrcGetSetMcCh(MrcData, Controller, Channel, GsmMccSchedGlbDrvGateDis, WriteToCache, &GetSetDis);

      // Configure Safe Config values
      MrcMcApplySafeConfig (MrcData, Controller, Channel);

      // Program 1x Refresh per technology during cold path.
      if (Outputs->IsLpddr5) {
        Data32 = RANK_TEMPERATURE_1XREF_LPDDR5;
      } else { // DDR5
        Data32 = RANK_TEMPERATURE_1XREF_DDR5;
      }
      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_RANK_TEMPERATURE_REG, MC1_CH0_CR_RANK_TEMPERATURE_REG, Controller, MC0_CH1_CR_RANK_TEMPERATURE_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, Data32);

      GetSetVal = ExtInputs->DisPgCloseIdleTimeout;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteToCache, &GetSetVal);

      MrcConfigureDdr5EchoMask (MrcData, Controller, Channel);

      if (ExtInputs->McRefreshRate < RefreshMax) {
        GetSetVal = ExtInputs->McRefreshRate;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMinRefRate, WriteToCache, &GetSetVal);
        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_NOTE,
          "MC%u C%u:%s Refresh on\n",
          Controller,
          Channel,
          (char *[RefreshMax]){"Normal", ".5x", "1x", "2x", "4x"}[ExtInputs->McRefreshRate]
          );
      } else {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Inputs->McRefreshRate is not supported! \n");
      }

      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Enable Self Refresh %d: Idle Timer %d \n", ExtInputs->SrefCfgEna, ExtInputs->SrefCfgIdleTmr);
    } // Channel
  } // Controller

  MrcFlushRegisterCachedData (MrcData);

  if (ExtInputs->MimicWcDisaplayInIpq != 0) {
    MrcMimicWcDisplayPatternInIPQ(MrcData);
  }

  // Configuration for MC normal mode - mcmnts_spare2_0_0_0_mchbar.spare_rw[1].
  if (Outputs->IsDvfsqEnabled) {
    DisablePeriodicZqLatch (MrcData);
  }

  return mrcSuccess;
}

/**
  This function returns TRUE if MRC input PmaCceConfig requires the input Controller
  index CCE bit to be enabled in the MemSS PMA.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  Controller - The controller index to check for CCE enable request from PmaCceConfig

  @retval TRUE  - if the PmaCceConfig input requests CCE Enabled
  @retval FALSE - if the PmaCceConfig input requests CCE Disabled
**/
BOOLEAN
MrcIsCceEnabled (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller
  )
{
  MrcInput *Inputs;
  BOOLEAN  IsEnabled;

  Inputs  = &MrcData->Inputs;
  switch (Inputs->PmaCceConfig) {
  case MrcCceEnableAll:
    IsEnabled = TRUE;
    break;

  case MrcCceEnablePerMc:
    IsEnabled = MrcControllerExist (MrcData, Controller);
    break;

  case MrcCceEnable0:
    IsEnabled = (Controller == cCONTROLLER0);
    break;

  case MrcCceEnable1:
    IsEnabled = (Controller == cCONTROLLER1);
    break;

  case MrcCceEnSingle:
    // If both MC0 and MC1 are populated we want to set only CCE0 otherwise CCE1
    IsEnabled = (MrcControllerExist (MrcData, cCONTROLLER0)) ? (Controller == cCONTROLLER0) : (Controller == cCONTROLLER1);
    break;

  case MrcCceDisable:
  default:
    IsEnabled = FALSE;
    break;
  }

  return IsEnabled;
}

/**
  This function sets the PMA Configuration. This should only be run once by the first workpoint

  @param[in, out] MrcData - MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPmaConfig (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MrcStatus         Status;
  UINT32            Controller;
  INT64             GetSetVal;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  Status  = mrcSuccess;


  // Verifying if memory is unlocked
  Status = MrcPmaUnlockStatus (MrcData);

  // Configure IP Enables for MC and IBECC in MEMSS_PMA_CR_BIOS_MEM_CONFIG
  GetSetVal = 1;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      MrcGetSetMc (MrcData, Controller, GsmPmaEnableMc, WriteToCache | PrintValue, &GetSetVal);
      if (Inputs->ExtInputs.Ptr->Ibecc == TRUE) {
        MrcGetSetMc (MrcData, Controller, GsmPmaEnableIbecc, WriteToCache | PrintValue, &GetSetVal);
      }
    }
    if (MrcIsCceEnabled (MrcData, Controller)) {
      MrcGetSetMc (MrcData, Controller, GsmPmaEnableCce, WriteToCache | PrintValue, &GetSetVal);
    }
  }
  if (Inputs->TmeEnable) {
    MrcGetSetNoScope (MrcData, GsmPmaEnableTme, WriteToCache | PrintValue, &GetSetVal);
  }

  if (Outputs->IsDdr5) {
    GetSetVal = 1;
  } else {
    // LPDDR5
    GetSetVal = 2;
  }
  MrcGetSetNoScope (MrcData, GsmPmaDdrType, WriteToCache | PrintValue, &GetSetVal);

  MrcFlushRegisterCachedData (MrcData);

  return Status;
}

#define TXEQ_CONSTATNTZ    (1 << 4)
#define TXEQ_NODEEMPHASIS   0xC
#define TXDQ_PREDRV_SEL_VCCIOG   0
#define TXDQ_PREDRV_SEL_VDDQ     1
#define CCC_PREDRV_SEL_VDDQ      0
#define CCC_PREDRV_SEL_VCCIOG    1
#define DLL_PWRMUX_SEL_VCCDD2G   0
#define DLL_PWRMUX_SEL_VCCIOG    1

/**
  Use the roundtrip matching feature in the MC to align Roundtrip to the rddata_valid on SPID.
  Issue one Read (MRR/MPR) transaction and use FSM in the MC to find the roundtrip gap.

  @param[in] MrcData - all the global data

  @retval mrcSuccess if all is well
**/
MrcStatus
MrcRoundTripMatch (
  IN     MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION      *MrcCall;
  MrcOutput               *Outputs;
  MrcDebug                *Debug;
  UINT32                  Controller;
  UINT8                   Channel;
  UINT32                  Rank;
  INT64                   DisIosSbClkGateSave[MAX_CONTROLLER];
  INT64                   GetSetEn;
  INT64                   Trigger;
  INT64                   RoundtripDelta;
  INT64                   GapOverflow;
  INT64                   GapSign;
  UINT64                  Timeout;
  BOOLEAN                 Busy;
  UINT8                   MrrData[MRC_MRR_ARRAY_SIZE];

  MrcCall = MrcData->Inputs.Call.Func;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  GetSetEn = 1;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }
    MrcGetSetMc (MrcData, Controller, GsmMccDisIosfSbClkGate, ReadCached, &DisIosSbClkGateSave[Controller]);
    MrcGetSetMc (MrcData, Controller, GsmMccDisIosfSbClkGate, WriteCached, &GetSetEn);

    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        // Start rrd_fsm
        MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, GsmMccRrdValidTrigger, WriteCached | PrintValue, &GetSetEn);
        // Issue one MRR
        MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR5, MrrData);
        // Poll on FSM to complete
        Timeout = MrcCall->MrcGetCpuTime () + 10000;   // 10 seconds timeout
        do {
          MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, GsmMccRrdValidTrigger, ReadUncached | PrintValue, &Trigger);
          Busy = (Trigger == 1);
          if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag == 1) { // No Simics support for this FSM yet
            Busy = FALSE;
          }
        } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

        if (Busy) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Timed out waiting for RRDVALID FSM!\n", gErrString);
          return mrcDeviceBusy;
        }

        // Read FSM results
        MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, GsmMccRrdValidOverflow, ReadUncached | PrintValue, &GapOverflow);
        MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, GsmMccRrdValidValue,    ReadUncached | PrintValue, &RoundtripDelta);
        MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, GsmMccRrdValidSign,     ReadUncached | PrintValue, &GapSign);

        if (GapOverflow == 1) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s RRDVALID Overflow!\n", gErrString);
          return mrcFail;
        }

        // Apply FSM result to the roundtrip of this rank
        if (GapSign == 1) {
          RoundtripDelta = -RoundtripDelta;
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RoundtripDelta: %lld\n", RoundtripDelta);
        MrcGetSetMcChRnk (MrcData, Controller, Channel, Rank, RoundTripDelay, WriteOffsetCached | PrintValue, &RoundtripDelta);
      } // Rank
    } // Channel

    // Restore original value
    MrcGetSetMc (MrcData, Controller, GsmMccDisIosfSbClkGate, WriteCached, &DisIosSbClkGateSave[Controller]);

  } // Controller

  return mrcSuccess;
}

/**
  In order to stress more easily the display worst case access pattern from the memory we can suggest a change in behavior
  in the IPQ to mimic such case.
  Assumption on worst case pattern occurs due to unfortunate page allocation that does not utilize all the memory banks but
  a small subset of them (one in the extreme case)
  The idea is is to limit the IPQ to only allow operation with one bank at a time, yet still benefit from serving existing PH
  requests to the open bank as even if fewer numbers of banks are used, once a page is open the controller is going to serve
  all the existing PH requests and not just one.
  @param[in] MrcData - all the global data
**/
void
MrcMimicWcDisplayPatternInIPQ (
  IN     MrcParameters *const MrcData
  )
{
  MrcInput                    *Inputs;
  MrcOutput                   *Outputs;
  MrcDebug                    *Debug;
  UINT32                      Controller;
  UINT32                      Channel;
  BOOLEAN                     IsLpddr;
  UINT32                      IpChannel;
  UINT32                      Offset;
  MC0_CH0_CR_DFT_MISC_STRUCT  DftMiscMchbar;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsLpddr = Outputs->IsLpddr;


  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        IpChannel = LP_IP_CH (IsLpddr, Channel);
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_DFT_MISC_REG, MC1_CH0_CR_DFT_MISC_REG, Controller, MC0_CH1_CR_DFT_MISC_REG, IpChannel);

        DftMiscMchbar.Data = MrcReadCR (MrcData, Offset);
        DftMiscMchbar.Bits.ipq_wcdp_act_resources = Inputs->ExtInputs.Ptr->MimicWcDisaplayInIpq;

        MrcWriteCR (MrcData, Offset, DftMiscMchbar.Data);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Controller: %u, Channel: %u, dft_misc_0_0_0_mchbar.ipq_wcdp_act_resources=%08Xh\n", Controller, Channel, DftMiscMchbar.Bits.ipq_wcdp_act_resources);
      }
    } // Channel
  } // Controller
}

/**
  Programs Safe mode values for specific register fields
  Register values are updated depending on the MC Safe Mode bitmask
  This function does not contain all the MC registers that need to be programmed
    with safe mode values. Some are dispersed throughout the code to not disrupt code flow

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Current Controller
  @param[in] Channel    - Current Channel

**/
VOID
MrcMcApplySafeConfig (
  IN MrcParameters* MrcData,
  IN UINT32         Controller,
  IN UINT32         Channel
  )
{
  const MrcInput   *Inputs;
  INT64     GetSetDis;
  UINT8     McSafeMode;

  Inputs = &MrcData->Inputs;
  McSafeMode = Inputs->ExtInputs.Ptr->McSafeMode;
  GetSetDis = 0;

  if (McSafeMode & MC_SAFE_OPP_SR) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccOppSrefEnable, WriteToCache, &GetSetDis);
  }

  MrcFlushRegisterCachedData (MrcData);
}

/**
  Configures registers to enable LpMode4 features.

  @param[in] MrcData - all the global data
**/
VOID
MrcLpMode4Enable (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  UINT32    SrxDelay;
  INT64     GetSetVal;
  INT64     GetSetEn;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT8     Controller;
  UINT32    PmOppSrPolicyOffset0;
  UINT32    PmOppSrPolicyOffset1;
  MC0_PM_OPP_SR_POLICY_0_STRUCT PmOppSrPolicy;

  Outputs   = &MrcData->Outputs;
  ExtInputs = MrcData->Inputs.ExtInputs.Ptr;
  GetSetEn  = 0x1;

  if (ExtInputs->LpMode4 != 0 && Outputs->IsLpddr5) {
    MrcGetSetNoScope (MrcData, GsmIocEnableLpMode4, WriteCached | PrintValue, &GetSetEn);
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }

    PmOppSrPolicyOffset0 = OFFSET_CALC_CH (MC0_PM_OPP_SR_POLICY_0_REG, MC1_PM_OPP_SR_POLICY_0_REG, Controller);
    PmOppSrPolicyOffset1 = OFFSET_CALC_CH (MC0_PM_OPP_SR_POLICY_1_REG, MC1_PM_OPP_SR_POLICY_1_REG, Controller);
    // PM_OPP_SR_POLICY_0_0_0_MCHBAR[0] (light)
    //  READ_DELAY=0x0
    //  READ_REQ=0x1
    //  WRITE_REQ=0x1
    //  LPMODE4_EN=0x0
    //  DELAY_LP_VC1=0x0
    // PM_OPP_SR_POLICY_0_0_0_MCHBAR[1] (aggressive) - keep at default values
    PmOppSrPolicy.Data = 0;
    PmOppSrPolicy.Bits.read_delay   = 0;
    PmOppSrPolicy.Bits.read_req     = 1;
    PmOppSrPolicy.Bits.write_req    = 1;
    PmOppSrPolicy.Bits.LPMode4_EN   = 0;
    PmOppSrPolicy.Bits.delay_lp_vc1 = 0;
    MrcWriteCR (MrcData, PmOppSrPolicyOffset0, PmOppSrPolicy.Data);

    PmOppSrPolicy.Data = MrcReadCR (MrcData, PmOppSrPolicyOffset1);
    PmOppSrPolicy.Bits.read_delay = 1;
    PmOppSrPolicy.Bits.read_req   = 1;
    PmOppSrPolicy.Bits.write_req  = 8;
    PmOppSrPolicy.Bits.LPMode4_EN = Outputs->IsLpddr5 ? ExtInputs->LpMode4 : 0;
    MrcWriteCR (MrcData, PmOppSrPolicyOffset1, PmOppSrPolicy.Data);
  }

  // Configuring SrxDelay according to timing specified in mip-master
  SrxDelay = DIVIDECEIL (MRC_LPMODE4_SRX_DELAY_PS, Outputs->Dclkps);
  GetSetVal = SrxDelay;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMctLpMode4SrxDelay, WriteCached | PrintValue, &GetSetVal);
}
