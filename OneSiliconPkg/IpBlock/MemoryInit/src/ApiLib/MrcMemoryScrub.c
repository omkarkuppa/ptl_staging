/** @file
  This file contains the memory scrubbing and alias checking functions.

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

/**
  Programs REFRESH ctrl register to run refresh.
  @param[in] MrcData           - Pointer to MRC global data.
  @param[in] Controller        - Controller to enable.
  @param[in] Channel           - Channel to enable.
  @param[in] RankBitMask       - Bit mask of ranks to enable.
  @retval Nothing
**/
VOID
EnableRefresh (
  IN MrcParameters *const MrcData,
  IN const UINT8          Controller,
  IN const UINT8          Channel,
  IN UINT8                RankBitMask
  )
{
  MrcOutput   *Outputs;
  INT64       GetSetDis;

  GetSetDis = 0;
  Outputs   = &MrcData->Outputs;
  RankBitMask &= Outputs->Controller[Controller].Channel[Channel].ValidRankBitMask;

  if ((RankBitMask & ALL_RANK_MASK) != 0) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRefreshRankMask,  WriteCached, &GetSetDis);
  }
}

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
  UINT32                SdramCapacity;
  UINT8                 Rank;
  UINT8                 IpChannel;
  UINT8                 Channel;
  UINT8                 Controller;
  UINT8                 MaxChannels;
  UINT8                 McChBitMask;
  UINT8                 McChError;
  UINT8                 RankToDimm;
  UINT8                 LocalMcChBitMask;
  BOOLEAN               EccEnabled;
  BOOLEAN               Ibecc;
  BOOLEAN               TxtClean;
  BOOLEAN               CleanMemory;
  BOOLEAN               WarmReset;
  BOOLEAN               IsLpddr;
  BOOLEAN               CapNotPowerOf2[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL];
  UINT32                SavedCpgc20Credits[MAX_CONTROLLER];
  MRC_ADDRESS          CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32                Pattern[CPGC_20_NUM_DPAT_EXTBUF/ CPGC_20_64b_CHUNCKS_IN_32b_ENTRIES][CPGC_20_64b_CHUNCKS_IN_32b_ENTRIES];
  const MRC_FUNCTION    *MrcCall;
  const MRC_EXT_INPUTS_TYPE          *ExtInputs;
  MC0_CH0_CR_SC_PCIT_STRUCT          ScPcit;
  MC0_CH0_CR_SC_PCIT_STRUCT          ScPcitSave[MAX_CONTROLLER][MAX_CHANNEL];

#ifdef MRC_DEBUG_PRINT
  static const char *SourceStr[4] = { "ECC", "CleanMemory", "TXT", "IBECC" };
#endif

  Inputs      = &MrcData->Inputs;
  ExtInputs   = Inputs->ExtInputs.Ptr;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  Status      = mrcSuccess;
  EccEnabled  = (Outputs->EccSupport == TRUE);
  IsLpddr     = Outputs->IsLpddr;
  tRFC        = Outputs->Timing[ExtInputs->MemoryProfile].tRFC;
  TxtClean    = Inputs->TxtClean;
  CleanMemory = (BOOLEAN) Inputs->CleanMemory;
  Ibecc       = (BOOLEAN) ((ExtInputs->Ibecc == TRUE) && ((ExtInputs->IbeccOperationMode == IbeccPartialProtect) || (ExtInputs->IbeccOperationMode == IbeccAllProtect)));
  WarmReset   = (Inputs->BootMode == bmWarm);
  McChError   = 0;
  GetSetDis   = 0;
  MaxChannels = Outputs->MaxChannels;

  if (!(((Ibecc || EccEnabled || CleanMemory) && (!WarmReset)) || TxtClean)) {
    // Memory clean is not needed if none of the (Ibecc || EccEnabled || CleanMemory || TxtClean) bit is set
    // In WarmRest, only TxtClean needs to preform this EccClean function, not required for EccEnabled or CleanMemory
    return mrcSuccess;
  } else if (Outputs->IsIbeccInitFsm && (!TxtClean)) {
    // IBECC scrubbing is done using HW FSM
    return mrcSuccess;
  }

  MrcModifyRdRdTimings (MrcData, TRUE);

  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Scrubbing Memory due to %s\n", EccEnabled ? SourceStr[0] : (Ibecc ? SourceStr[3] : (TxtClean ? SourceStr[2] : SourceStr[1])));

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
        SdramCapacity = SdramCapacityTable[DimmOut->DensityIndex] * 8;
        if ((SdramCapacity & (SdramCapacity - 1)) > 0) {
          CapNotPowerOf2[Rank][Controller][Channel] = TRUE;
        }
        McChBitMask |= SelectReutRanks (MrcData, Controller, Channel, 1 << Rank, FALSE);
        EnableRefresh (MrcData, Controller, Channel, 1 << Rank);
      }
    }

    // Update the Bank/Row/Col sizes per current rank population
    UpdateAddressForWholeRankExtended (MrcData, CPGCAddressArray, BANK_2_ROW_COL_2_RANK, McChBitMask, Rank, CapNotPowerOf2);

    SetupIOTestScrub (MrcData, CPGCAddressArray, McChBitMask, PatWrScrub);

    Cpgc20UpdateBaseRepeatsForWholeRankExtended (MrcData, Rank, McChBitMask, TRUE, CapNotPowerOf2);

    MrcUpdateL2PAllsBanksMapping (MrcData, Rank, McChBitMask);

    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
          continue;
        }
        LocalMcChBitMask = (UINT8) (MRC_BIT0 << ((Controller * Outputs->MaxChannels) + Channel));
        if (CapNotPowerOf2[Rank][Controller][Channel]) {
          MrcSetLoopcount (MrcData, LocalMcChBitMask, 2);
        }
      }
    }

    // Run the test on both channels, don't check for errors - this is WR only test
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Scrubbing Rank%d: McChBitMask = 0x%X\n", Rank, McChBitMask);
    RunIOTest (MrcData, McChBitMask, MRC_IGNORE_ARG_8, Outputs->DQPat, 1, FALSE);
    McChError |= Cpgc20GetAllChannelsError (MrcData, McChBitMask);


    // Wait for MC to drain - CPGC test done indication comes earlier that all the WR's come out
    MrcWait (MrcData, ((Outputs->tCKps * 64 * Outputs->BurstLength) / 1000) * MRC_TIMER_1NS);
    MrcWait (MrcData, ((Outputs->tCKps * tRFC) / 1000) * MRC_TIMER_1NS);
  } // for Rank

  if (TxtClean) {
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
          EnableRefresh (MrcData, Controller, Channel, 1 << Rank);
        }
      }
      // Update the Bank/Row/Col sizes per current rank population
      UpdateAddressForWholeRankExtended (MrcData, CPGCAddressArray, BANK_2_ROW_COL_2_RANK, McChBitMask, Rank, CapNotPowerOf2);

      SetupIOTestScrub (MrcData, CPGCAddressArray, McChBitMask, PatRd);

      Cpgc20UpdateBaseRepeatsForWholeRankExtended (MrcData, Rank, McChBitMask, TRUE, CapNotPowerOf2);

      MrcUpdateL2PAllsBanksMapping (MrcData, Rank, McChBitMask);

      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
          if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
            continue;
          }
          LocalMcChBitMask = (UINT8) (MRC_BIT0 << ((Controller * Outputs->MaxChannels) + Channel));
          if (CapNotPowerOf2[Rank][Controller][Channel]) {
            MrcSetLoopcount (MrcData, LocalMcChBitMask, 2);
          }
        }
      }

      // Run the test on both channels, to check if it is all zeros
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Checking Zeros on Rank%d: McChBitMask = 0x%X\n", Rank, McChBitMask);
      RunIOTest (MrcData, McChBitMask, MRC_IGNORE_ARG_8, Outputs->DQPat, 1, FALSE);
      McChError |= Cpgc20GetAllChannelsError (MrcData, McChBitMask);
    } // for Rank
  } // for TxtClean

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

  MrcModifyRdRdTimings (MrcData, FALSE);

  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  Outputs->IsSkipIoReset = FALSE;

  return Status;
}

/**
  This function performs a memory alias check.

  @param[in] MrcData - The global host structure

  @retval mrcSuccess or error value.
**/
MrcStatus
MrcAliasCheck (
  IN OUT MrcParameters *const MrcData
)
{
  const MRC_FUNCTION            *MrcCall;
  MrcDebug                      *Debug;
  MrcInput                      *Inputs;
  MrcOutput                     *Outputs;
  MrcControllerOut              *ControllerOut;
  MrcDimmOut                    *DimmOut;
  MrcStatus                     Status;
  INT64                         RefreshSave[MAX_CONTROLLER];
  INT64                         GetSetVal;
  INT64                         GetSetDis;
  INT64                         SaveSrxWrRetrain[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32                        SdramAddressingCapacity;
  UINT32                        CrOffset;
  UINT32                        SdramCapacity;
  UINT8                         Rank;
  UINT8                         RankToDimm;
  UINT8                         Controller;
  UINT8                         Channel;
  UINT8                         IpChannel;
  UINT8                         MaxChannels;
  UINT8                         ActiveChBitMask;
  UINT32                        SavedCpgc20Credits[MAX_CONTROLLER];
  BOOLEAN                       InvalidSpdAddressingCapacity;
  MRC_REUTAddress               ReutAddress;
  MC0_MAD_INTER_CHANNEL_STRUCT  MadIntraOrig[MAX_CONTROLLER];
  MC0_MAD_INTER_CHANNEL_STRUCT  MadIntra;
  BOOLEAN                       IsLpddr;
  MC0_CH0_CR_SC_PCIT_STRUCT     ScPcit;
  // INT64                         IocEccEn;
  MC0_CH0_CR_SC_PCIT_STRUCT     ScPcitOrg[MAX_CONTROLLER][MAX_CHANNEL];

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;
  Status  = mrcSuccess;
  IsLpddr = Outputs->IsLpddr;
  MaxChannels = Outputs->MaxChannels;
  GetSetDis = 0;
  InvalidSpdAddressingCapacity = FALSE;

  MrcCall->MrcSetMemDword (SavedCpgc20Credits, ARRAY_COUNT (SavedCpgc20Credits), RDCPL_CREDITS_SCRUB);

  // Check to see if the SDRAM Addressing * Primary Bus Width == SDRAM capacity.
  // If not, report an alias and exit.
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank += MAX_RANK_IN_DIMM) {
        if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
          RankToDimm = RANK_TO_DIMM_NUMBER (Rank);
          DimmOut = &ControllerOut->Channel[Channel].Dimm[RankToDimm];
          SdramAddressingCapacity = (DimmOut->ColumnSize * DimmOut->RowSize);
          // Since the minimum number of row and column bits are 12 and 9 respectively,
          // we can shift by 20 to get the result in Mb before multiplying by the bus width.
          SdramAddressingCapacity = SdramAddressingCapacity >> 20;
          SdramAddressingCapacity *= DimmOut->Banks;
          SdramAddressingCapacity *= DimmOut->BankGroups;
          SdramAddressingCapacity *= DimmOut->SdramWidth;
          if (IsLpddr && !Outputs->IsLP5Camm2) {
            // SPD Density is per die, but SdramWidth is per LP4 channel, so need to multiple by number of channels per die
            SdramAddressingCapacity *= DimmOut->ChannelsPerSdramPackage;
          }
          SdramCapacity = SdramCapacityTable[DimmOut->DensityIndex] * 8;
          // Check if density is power of 2, if non power of 2, reduce the addressing by 25%
          if ((SdramCapacity & (SdramCapacity - 1)) > 0) {
            SdramAddressingCapacity = SdramAddressingCapacity * 3 / 4;
          }
          if (SdramCapacity != SdramAddressingCapacity) {
            InvalidSpdAddressingCapacity = TRUE;
            MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_ERROR,
              "ERROR: MC %d Channel %d Dimm %d SPD SDRAM Adressing Capacity(0x%xMb) does not match SDRAM Capacity(0x%xMb)\nPlease verify:\n",
              Controller,
              Channel,
              RankToDimm,
              SdramAddressingCapacity,
              SdramCapacity
            );
            MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_ERROR,
              " Capacity: 0x%x\n RowSize: 0x%x\n ColumnSize: 0x%x\n Banks: 0x%x\n Bank Groups: 0x%x\n Device Width: 0x%x\n",
              SdramCapacity,
              DimmOut->RowSize,
              DimmOut->ColumnSize,
              DimmOut->Banks,
              DimmOut->BankGroups,
              DimmOut->SdramWidth
            );
            break;
          }
        }
      }
    }
  }
  //
  // Since we will not hang the system, signal that an Alias could exist and return mrcSuccess.
  //
  if (TRUE == InvalidSpdAddressingCapacity) {
    MRC_DEBUG_ASSERT (
      (InvalidSpdAddressingCapacity == FALSE),
      Debug,
      "...Memory Alias detected - Invalid Spd Addressing Capacity...\n"
    );
    return Status;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Performing Alias Test\n");
  MrcCall->MrcSetMem ((UINT8 *) &ReutAddress, sizeof (ReutAddress), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    // Determine if we are ECC enabled.  If so, disable ECC since the ECC scrub has yet to occur.
    if (Outputs->EccSupport == TRUE) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ECC enabled.  Disabling ECC for the test.  Must scrub after this!!!\n");

      CrOffset = OFFSET_CALC_CH (MC0_MAD_INTER_CHANNEL_REG, MC1_MAD_INTER_CHANNEL_REG, Controller);
      MadIntraOrig[Controller].Data = MrcReadCR (MrcData, CrOffset);
      MadIntra.Data = MadIntraOrig[Controller].Data;
      MadIntra.Bits.ECC = emNoEcc;
      MrcWriteCR (MrcData, CrOffset, MadIntra.Data);
    }

    if (MrcControllerExist (MrcData, Controller)) {
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
          continue;
        }
        IpChannel = LP_IP_CH (IsLpddr, Channel);

        if (IsLpddr) {
          // Disable the DQS Osillator for LP5.
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnSrxWrRetraining, ReadCached, &SaveSrxWrRetrain[Controller][Channel]);
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnSrxWrRetraining, WriteToCache, &GetSetDis);
          MrcFlushRegisterCachedData (MrcData);

        }
        // Change PCIT to 0xFF
        CrOffset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_PCIT_REG, MC1_CH0_CR_SC_PCIT_REG, Controller, MC0_CH1_CR_SC_PCIT_REG, IpChannel);
        ScPcit.Data = MrcReadCR (MrcData, CrOffset);
        ScPcitOrg[Controller][IpChannel] = ScPcit;
        ScPcit.Bits.PCIT_SUBCH0 = MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_MAX;
        ScPcit.Bits.PCIT_SUBCH1 = MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_MAX;
        MrcWriteCR (MrcData, CrOffset, ScPcit.Data);
      }
      Status |= MrcCpgcModifyReadCredits (MrcData, Controller, TRUE, &SavedCpgc20Credits[Controller]);

      // Enable Refreshes. Save previous state.
      MrcGetSetMc (MrcData, Controller, GsmMccEnableRefresh, ReadFromCache, &RefreshSave[Controller]);
      GetSetVal = 1;
      MrcGetSetMc (MrcData, Controller, GsmMccEnableRefresh, WriteCached, &GetSetVal);
    }
  }

  // Run test Per Dimm
  //
  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank += MAX_RANK_IN_DIMM) {
    if ((MRC_BIT0 << Rank) & Outputs->ValidRankMask) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Testing Dimm %d\n", Rank / 2);
      //
      // Determine Active Channels
      //
      ActiveChBitMask = 0;
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
          ActiveChBitMask |= SelectReutRanks (MrcData, Controller, Channel, MRC_BIT0 << Rank, FALSE);
        }
      }

      //
      // Run the test
      //
      //@todo      Status = MrcRunMemoryScrub (MrcData, ActiveChBitMask, TRUE);
      if (Status != mrcSuccess) {
        break;
      }
    }
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      for (Channel = 0; Channel < MaxChannels; Channel++) {
        if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
          continue;
        }
        IpChannel = LP_IP_CH (IsLpddr, Channel);
        // Restore PCIT value
        CrOffset = OFFSET_CALC_MC_CH (MC0_CH0_CR_SC_PCIT_REG, MC1_CH0_CR_SC_PCIT_REG, Controller, MC0_CH1_CR_SC_PCIT_REG, IpChannel);
        MrcWriteCR (MrcData, CrOffset, ScPcitOrg[Controller][IpChannel].Data);
        if (IsLpddr) {
          // Restore the DQS Oscillator value for LP5.
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEnSrxWrRetraining, WriteToCache, &SaveSrxWrRetrain[Controller][Channel]);
          MrcFlushRegisterCachedData (MrcData);
        }
      }

      // ReEnable ECC logic.
      if (Outputs->EccSupport == TRUE) {
        CrOffset = OFFSET_CALC_CH (MC0_MAD_INTER_CHANNEL_REG, MC1_MAD_INTER_CHANNEL_REG, Controller);
        MrcWriteCR (MrcData, CrOffset, MadIntraOrig[Controller].Data);
      }

      // Restore RD_CPL_CREDITS_INIT
      Status |= MrcCpgcModifyReadCredits (MrcData, Controller, FALSE, &SavedCpgc20Credits[Controller]);

      // Restore Refreshes.
      MrcGetSetMc (MrcData, Controller, GsmMccEnableRefresh, WriteCached, &RefreshSave[Controller]);
    }
  }

  // Wait 4 usec after enabling the ECC IO, needed by HW
  MrcWait (MrcData, 4 * MRC_TIMER_1US);

  if (mrcSuccess != Status) {
    MRC_DEBUG_ASSERT (
      (Status == mrcSuccess),
      Debug,
      "%s Alias Detected! See REUT Error above. ***\n Error Status : %x \n", gErrString, Status);
    Status = mrcSuccess;
  }

  return Status;
}
