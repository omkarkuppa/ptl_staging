/** @file
  This file contains the memory scrubbing and alias checking functions.

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
**/

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcMemoryScrub.h"
#include "MrcGeneral.h"
#include "MrcCpgcApi.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "MrcMemoryApi.h"
#include "MrcDdrCommon.h"
#include "BlueMrcIoTest.h"
#include "MrcSpdProcessingDefs.h" // for SdramCapacityTable[]
#include "MrcDdr5.h"
#include "MrcReset.h"
#include "MrcMemoryScrub.h"
#include "MrcMcSiSpecific.h"

/**
  Zero out all of the memory.
  This function is used in the following cases:
  - ECC is enabled (to initialize ECC logic)
  - TXT library is asking to scrub the memory (instead of SCLEAN ACM)
  - Platform code is asking to scrub the memory due to MOR bit being set (Memory Override Request)

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess if the clean succeeded, otherwise an error status.
**/
MrcStatus
MrcEccClean (
  IN     MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcDimmOut            *DimmOut;
  MrcStatus             Status;
  INT64                 GetSetVal;
  INT64                 GetSetDis;
  INT64                 SrxWrRetrainSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64                 DisPageTableIdleTimerSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64                 PowerDownEnableSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64                 SelfRefreshEnableSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64                 MccIdleEnable[MAX_CONTROLLER][MAX_CHANNEL];
  INT64                 RetrainPeriodSave[MAX_CONTROLLER][MAX_CHANNEL];
  INT64                 MR4PeriodSave[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32                Offset;
  UINT32                tRFC;
  UINT8                 Rank;
  UINT8                 IpChannel;
  UINT8                 Channel;
  UINT8                 Controller;
  UINT8                 MaxChannels;
  UINT8                 McChBitMask;
  UINT8                 McChError;
  UINT8                 RankToDimm;
  BOOLEAN               TxtClean;
  BOOLEAN               IsLpddr;
  BOOLEAN               CapNotPowerOf2[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL];
  UINT32                SavedCpgc20Credits[MAX_CONTROLLER];
  MRC_ADDRESS          CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32                Pattern[CPGC_20_NUM_DPAT_EXTBUF/ CPGC_20_64b_CHUNCKS_IN_32b_ENTRIES][CPGC_20_64b_CHUNCKS_IN_32b_ENTRIES];
  MRC_MC_AD_SAVE        MadSavedValues;
  const MRC_FUNCTION    *MrcCall;
  const MRC_EXT_INPUTS_TYPE          *ExtInputs;
  MC0_CH0_CR_SC_PCIT_STRUCT          ScPcit;
  MC0_CH0_CR_SC_PCIT_STRUCT          ScPcitSave[MAX_CONTROLLER][MAX_CHANNEL];


  Inputs      = &MrcData->Inputs;
  ExtInputs   = Inputs->ExtInputs.Ptr;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  Status      = mrcSuccess;
  IsLpddr     = Outputs->IsLpddr;
  tRFC        = Outputs->Timing[ExtInputs->MemoryProfile].tRFC;
  TxtClean    = Inputs->TxtClean;
  McChError   = 0;
  GetSetDis   = 0;
  MaxChannels = Outputs->MaxChannels;

  if (!MrcIsTeMemoryScrubbingRequired (MrcData, MRC_PRINTS_ON)) {
    // Test engine based memory scrubbing is not required
    return mrcSuccess;
  }

  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcSaveEnum, &MadSavedValues);

  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcSetMem ((UINT8 *)Pattern, sizeof (Pattern), 0x0);
  MrcCall->MrcSetMemDword (SavedCpgc20Credits, ARRAY_COUNT (SavedCpgc20Credits), RDCPL_CREDITS_SCRUB);
  MrcCall->MrcSetMem ((UINT8 *) CapNotPowerOf2, sizeof (CapNotPowerOf2), 0);

  if (Inputs->BootMode == bmFast) {
    IoReset (MrcData);
  }

  Outputs->IsSkipIoReset = TRUE;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
          continue;
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%d.CH%d\n", Controller, Channel);

        IpChannel = LP_IP_CH (IsLpddr, Channel);
        if (IsLpddr) {
          // Disable the DQS Osillator for LP5.
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnSrxWrRetraining, ReadCached, &SrxWrRetrainSave[Controller][Channel]);
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnSrxWrRetraining, WriteToCache, &GetSetDis);
        }

        // Disable retraining period
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRetrainPeriod, ReadCached, &RetrainPeriodSave[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRetrainPeriod, WriteToCache, &GetSetDis);

        // Disable MR4 period
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMR4Period, ReadCached, &MR4PeriodSave[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMR4Period, WriteToCache, &GetSetDis);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, ReadCached, &DisPageTableIdleTimerSave[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteToCache, &GetSetDis);

        // Change PCIT to 0xFF
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_PCIT_REG, MC1_CH0_CR_SC_PCIT_REG, Controller, MC0_CH1_CR_SC_PCIT_REG, IpChannel);
        ScPcit.Data = MrcReadCR (MrcData, Offset);
        ScPcitSave[Controller][IpChannel] = ScPcit;
        ScPcit.Bits.PCIT_SUBCH0 = MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_MAX;
        ScPcit.Bits.PCIT_SUBCH1 = MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_MAX;
        MrcWriteCR (MrcData, Offset, ScPcit.Data);

        // Clear out SPID Low Power Ctl Enables during scrubbing
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPowerDownEnable, ReadCached, &PowerDownEnableSave[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSelfRefreshEnable, ReadFromCache, &SelfRefreshEnableSave[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccIdleEnable, ReadFromCache, &MccIdleEnable[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPowerDownEnable, WriteToCache, &GetSetDis);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSelfRefreshEnable, WriteToCache, &GetSetDis);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccIdleEnable, WriteToCache, &GetSetDis);
      }
      Status |= MrcCpgcModifyReadCredits (MrcData, Controller, TRUE, &SavedCpgc20Credits[Controller]);

    }
  }
  MrcFlushRegisterCachedData (MrcData);

  // Enable refreshes on MC before we start ECC scrubbing.
  // Refresh must be kept enabled from this point on, otherwise we might loose the ECC initialization.
  GetSetVal = 1;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteCached, &GetSetVal);

  MrcCkeOnProgramming (MrcData);

  MrcModifyMcAddressDecoderValues (MrcData);

  // Scrub per rank, on both channels in parallel
  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    if (((1 << Rank) & Outputs->ValidRankMask) == 0) {
      continue;
    }
    // Determine the Active Channels
    McChBitMask = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        RankToDimm = RANK_TO_DIMM_NUMBER (Rank);
        DimmOut = &ControllerOut->Channel[Channel].Dimm[RankToDimm];
        if (!IS_CAPACITY_POWER2_DENSITY_INDEX_CHECK (DimmOut->DensityIndex)) {
          CapNotPowerOf2[Rank][Controller][Channel] = TRUE;
        }
        McChBitMask |= SelectReutRanks (MrcData, Controller, Channel, 1 << Rank, FALSE);
        if (MrcChannelExist (MrcData, Controller, Channel) && (!IS_MC_SUB_CH (IsLpddr, Channel))) {
          EnableRefresh (MrcData, Controller, Channel, 1 << Rank);
        }
      }
    }

    // Update the Bank/Row/Col sizes per current rank population
    UpdateAddressForWholeRank (MrcData, CPGCAddressArray, BANK_2_ROW_COL_2_RANK, McChBitMask, Rank);

    SetupIOTestScrubCpgc (MrcData, CPGCAddressArray, McChBitMask, PatWrScrub, CapNotPowerOf2[Rank]);

    Cpgc20UpdateBaseRepeatsForWholeRankExtended (MrcData, Rank, McChBitMask, TRUE, CapNotPowerOf2);

    MrcUpdateL2PAllsBanksMapping (MrcData, Rank, McChBitMask, FALSE);

    // Run the test on both channels, don't check for errors - this is WR only test
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Scrubbing Rank%d: McChBitMask = 0x%X\n", Rank, McChBitMask);
    RunIOTest (MrcData, McChBitMask, MRC_IGNORE_ARG_8, Outputs->DQPat, 1, FALSE);
    McChError |= Cpgc20GetAllChannelsError (MrcData, McChBitMask);


    // Wait for MC to drain - CPGC test done indication comes earlier that all the WR's come out
    MrcWait (MrcData, ((Outputs->tCKps * 64 * Outputs->BurstLength) / 1000) * MRC_TIMER_1NS);
    MrcWait (MrcData, ((Outputs->tCKps * tRFC) / 1000) * MRC_TIMER_1NS);
  } // for Rank

  if (TxtClean) {
    MrcModifyRdRdTimings (MrcData, TRUE);

    // Perform Memory Read Test to Check Zeros per rank, on both channels in parallel
    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      if (((1 << Rank) & Outputs->ValidRankMask) == 0) {
        continue;
      }
      // Determine the Active Channels
      McChBitMask = 0;
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MaxChannels; Channel++) {
          McChBitMask |= SelectReutRanks (MrcData, Controller, Channel, 1 << Rank, FALSE);
          if (MrcChannelExist (MrcData, Controller, Channel) && (!IS_MC_SUB_CH (IsLpddr, Channel))) {
            EnableRefresh (MrcData, Controller, Channel, 1 << Rank);
          }
        }
      }
      // Update the Bank/Row/Col sizes per current rank population
      UpdateAddressForWholeRank (MrcData, CPGCAddressArray, BANK_2_ROW_COL_2_RANK, McChBitMask, Rank);

      SetupIOTestScrubCpgc (MrcData, CPGCAddressArray, McChBitMask, PatRd, CapNotPowerOf2[Rank]);

      Cpgc20UpdateBaseRepeatsForWholeRankExtended (MrcData, Rank, McChBitMask, TRUE, CapNotPowerOf2);

      MrcUpdateL2PAllsBanksMapping (MrcData, Rank, McChBitMask, FALSE);

      // Run the test on both channels, to check if it is all zeros
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Checking Zeros on Rank%d: McChBitMask = 0x%X\n", Rank, McChBitMask);
      RunIOTest (MrcData, McChBitMask, MRC_IGNORE_ARG_8, Outputs->DQPat, 1, FALSE);
      McChError |= Cpgc20GetAllChannelsError (MrcData, McChBitMask);
    } // for Rank

    MrcModifyRdRdTimings (MrcData, FALSE);
  } // for TxtClean

  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcRestoreEnum, &MadSavedValues);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
          continue;
        }
        IpChannel = LP_IP_CH (IsLpddr, Channel);
        // Restore PCIT value
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_PCIT_REG, MC1_CH0_CR_SC_PCIT_REG, Controller, MC0_CH1_CR_SC_PCIT_REG, IpChannel);
        MrcWriteCR (MrcData, Offset, ScPcitSave[Controller][IpChannel].Data);

        // Restore Page Table Idle Timer
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteToCache, &DisPageTableIdleTimerSave[Controller][Channel]);

        if (IsLpddr) {
          // Restore the DQS Oscillator value for LP5.
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnSrxWrRetraining, WriteToCache, &SrxWrRetrainSave[Controller][Channel]);
        }

        // Restore retraining period
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRetrainPeriod, WriteToCache, &RetrainPeriodSave[Controller][Channel]);

        // Restore MR4 period
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMR4Period, WriteToCache, &MR4PeriodSave[Controller][Channel]);

        // Restore SPID Low Power Ctl Enables after scrubbing
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPowerDownEnable, WriteToCache, &PowerDownEnableSave[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccSelfRefreshEnable, WriteToCache, &SelfRefreshEnableSave[Controller][Channel]);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccIdleEnable, WriteToCache, &MccIdleEnable[Controller][Channel]);
      }
      // Restore RD_CPL_CREDITS_INIT
      Status |= MrcCpgcModifyReadCredits (MrcData, Controller, FALSE, &SavedCpgc20Credits[Controller]);
    }
  }
  MrcFlushRegisterCachedData (MrcData);

  // Check the scrubbing result
  if (McChError != 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s: Memory is not Clean after Scrubbing, McChError = 0x%X\n", gWarnString, McChError);
  }

  if (TxtClean) {
    Outputs->TxtScrubSuccess = 0;
    if (McChError == 0) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Memory is Clean, McChError = 0x%X, Calling TxtLib ClearSecretsBit()\n", McChError);
      Outputs->TxtScrubSuccess = 1;
    }
  }

  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  Outputs->IsSkipIoReset = FALSE;

  return Status;
}
