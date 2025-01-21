/** @file
  This file contains functions to find and track DRAM Failing addresses for the
  Post Package Repair feature.

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

#include "MrcPpr.h"
#include "MrcAmt.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcCpgcApi.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcReset.h"
#include "MrcGeneral.h"
#include "MrcMcSiSpecific.h"

// Verify at compile time that MAX_PATTERN_DEPTH is equal to CPGC_20_NUM_DPAT_EXTBUF
STATIC_ASSERT (MAX_PATTERN_DEPTH == CPGC_20_NUM_DPAT_EXTBUF, "Unexpected MAX_PATTERN_DEPTH value");

UINT64 AmtCacheLine1[8] = {
  0x5555555555555555, 0x5555555555555555,
  0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA,
  0x5555555555555555, 0x5555555555555555,
  0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA
};

UINT64 AmtCacheLine2[8] = {
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF
};

UINT64 AmtXMarchPattern[16] = {
  0x0000000000000000, 0x0000000000000000,
  0xFF00FF00FF00FF00, 0xFF00FF00FF00FF00,
  0xFFFF0000FFFF0000, 0xFFFF0000FFFF0000,
  0x0000000000000000, 0x0000000000000000,
  0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
  0xFF00FF00FF00FF00, 0xFF00FF00FF00FF00,
  0xFFFF0000FFFF0000, 0xFFFF0000FFFF0000,
  0x0000000000000000, 0x0000000000000000
};

UINT64 AmtXMarchGPattern[8] = {
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00,
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00,
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00,
  0xAAAAAA00AAAAAA00, 0xAAAAAA00AAAAAA00
};

UINT64 AmtMarchAltPattern[8] = {
  0x0000000000000000, 0x1111111111111111,
  0x2222222222222222, 0x3333333333333333,
  0x4444444444444444, 0x5555555555555555,
  0x6666666666666666, 0x7777777777777777
};

/**
  Cleans up registers and values set during PPR memory tests.

  @param[in] MrcData            - Global MRC data structure
  @param[in] PprAmtData         - PPR and AMT data structure
  @param[in] McChBitMask        - Bit masks of MC/Channels to clean settings.
**/
VOID
PprCleanup (
  IN MrcParameters *const           MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          McChBitMask
  )
{
  MrcOutput       *Outputs;
  UINT8           Controller;
  UINT8           Channel;
  INT64           DefaultRefreshMask;
  BOOLEAN         StopOnRaster;
  BOOLEAN         RasterRepoClear;
  UINT8           RasterRepoMode;
  UINT8           Mode3Banks;
  UINT32          Mode3Max;

  Outputs = &MrcData->Outputs;
  MrcGetMcConfigGroupLimits (MrcData, GsmMccRefreshRankMask, NULL, &DefaultRefreshMask, NULL);
  StopOnRaster = MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_StopOnRaster_DEF;
  RasterRepoClear = 1;
  RasterRepoMode = MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_RasterRepoMode_DEF;
  Mode3Banks = MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_MODE3_BANKS_DEF;
  Mode3Max = MC0_CH0_CR_CPGC2_RASTER_MODE3_MAX_FAIL_MAX_DEF;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      // Restore refreshes on all ranks to default value
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRefreshRankMask,  WriteCached, &DefaultRefreshMask);
    }
  }

  // Clear raster repo, set other fields to default
  Cpgc20RasterRepoConfig (MrcData, McChBitMask, &Mode3Banks, NULL, &StopOnRaster, &RasterRepoClear, &RasterRepoMode, (UINT8 *) &Mode3Max);

  // Clean values set in DPAT_BUF_CFG and DPAT_ALT_BUF_CFG
  // Cpgc20SetDpatBufCtl (MrcData, 0, 0, 0, 0);
  Cpgc20SetDpatAltBufCtl (MrcData, 0, 0, 0, 0);

  Cpgc20SetDataControl (MrcData, MC0_REQ0_CR_CPGC2_DATA_CONTROL_Data_Select_Rotation_Repeats_DEF, MC0_REQ0_CR_CPGC2_DATA_CONTROL_SPLIT_BACKGROUND_DEF);
  MrcSetLoopcount (MrcData, Outputs->McChBitMask, 0);
  AmtSetBaseAddressControl (MrcData, AdvMtWcMats8); // Reset fields to default (0)

  Outputs->PprPatBufShift = 0;
}

/**
  Check row failure if repairs are required

  @param[in] FailRangePtr         - Pointer to Row Fail
  @param[in] BankGroup            - Bank group

  @retval status - TRUE/FALSE
**/
BOOLEAN
IsRowFailureRepairRequired (
  ROW_FAIL_RANGE  *const FailRangePtr,
  UINT8                  BankGroup
  )
{
  // the bankgroup has failure and it's not repaired.
  return (FailRangePtr->BankGroupMask & (1 << BankGroup)) != 0
          && (FailRangePtr->BankGroupRepairedMask & (1 << BankGroup)) == 0;
}
/**
  Check row failure list and PPR resource list to determine if repairs are required

  @param[in] MrcData              - Global MRC data structure
  @param[in] BaseBits             - Number of bank bits in SW loop

  @retval status - TRUE/FALSE
**/
BOOLEAN
IsPprRepairRequired (
  MrcParameters  *const   MrcData,
  UINT8                   BaseBits
  )
{
  ROW_FAIL_RANGE      *FailRangePtr;
  UINT32              FailIndex;
  UINT8               Controller;
  UINT8               Ch;
  UINT8               Rank;
  UINT8               Bank;
  UINT8               BankInterleaveIndex;
  UINT8               Dram;
  UINT8               BytesPerDram;
  UINT32              RowFailMax;
  MrcDimmOut          *DimmOut;
  UINT8               MaxChDdr;
  MrcOutput           *Outputs;
  MrcChannelOut       *ChannelOut;
  UINT8               MSVx4; // device count

  Outputs         = &MrcData->Outputs;
  MSVx4           = Outputs->SdramCount;

  // Inspect test results for given rank, bank
  MaxChDdr = Outputs->MaxChannels;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Ch];

      // Was this channel part of the test group?
      if ((MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Ch, MaxChDdr) != 0)) {
        // Loop through all row failure ranges
        RowFailMax = AmtGetRowFailListSize (MrcData, Controller, Ch);
        for (FailIndex = 0; FailIndex < RowFailMax; FailIndex++) {
          FailRangePtr = AmtGetRowFailRange (MrcData, Controller, Ch, FailIndex);

          Rank = (UINT8) FailRangePtr->Addr.Bits.Rank;
          Bank = (UINT8) FailRangePtr->Addr.Bits.BankPair;

          DimmOut = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank)];
          for (BankInterleaveIndex = 0; BankInterleaveIndex < MAX_MEMTEST_BANK_INTERLEAVE_NUMBER; BankInterleaveIndex++) {
            if (IsRowFailureRepairRequired(FailRangePtr, BankInterleaveIndex)) {

              if (DimmOut->SdramWidth == 8) { // x8
                BytesPerDram = 1;
              } else { // x16
                BytesPerDram = 2;
              }
              for (Dram = 0; Dram < MSVx4; Dram += BytesPerDram) {
                if (GetPprResourceAvailable (MrcData, Controller, Ch, Rank, Dram, BankInterleaveIndex, Bank)) {
                  return 1;
                } // GetPprResourceAvailable
              } // for Dram
            } // if FailRangePtr->BankGroupMask
          } // for BankInterleaveIndex
        } // for FailIndex
      } // if MC_CH_MASK_CHECK
    } // for Ch
  } // for Controller

  return 0;
} // IsPprRepairRequired

/**
  Checks row failure list for any failures, and checks whether PPR resource exists for any failures.
  If PPR resources exist, runs disposition flow for each failure.

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     PprAmtData          - PPR and AMT data structure
  @param [in,out] *RepairDone         - Flag to indicate the repair was done

  @retval status - MRC_STATUS_SUCCESS/MRC_STATUS_FAILURE
**/
MrcStatus
PprDispositionFailRange (
  IN     MrcParameters *const       MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN OUT UINT8                      *RepairDone
  )
{
  MrcOutput           *Outputs;
  UINT32              McChBitMask;
  UINT8               Channel;
  UINT8               Controller;
  UINT8               RowFailuresFound;
  MrcStatus           RetVal = mrcSuccess;
  BOOLEAN             PprRequired;
  UINT8               MaxChDdr;

  Outputs = &MrcData->Outputs;
  MaxChDdr = Outputs->MaxChannels;

  *RepairDone = 0;

  // Are there any failures in the list?
  RowFailuresFound = 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, MaxChDdr) == 0) {
        continue;
      }
      if (AmtGetRowFailListSize (MrcData, Controller, Channel) > 0) {
        RowFailuresFound = 1;
        break;
      }
    }
  }

  if (RowFailuresFound) {

     // Check if any failure is on a device with a PPR resource
     PprRequired = IsPprRepairRequired (MrcData, (UINT8) PprAmtData->BaseBits);
     if (PprRequired) {
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "PprDispositionFailRange: Row Failures found and PPR resources available - Execute PPR flow to repair row failures\n");
     }

    // Loop over all ranks
    for (PprAmtData->Rank = 0; PprAmtData->Rank < MAX_RANK_IN_CHANNEL; PprAmtData->Rank++) {
      // check if any rank enabled per controller/channel
      McChBitMask = 0;
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
          if (MrcRankExist (MrcData, Controller, Channel, PprAmtData->Rank)) {
            McChBitMask |= (1 << ((Controller * Outputs->MaxChannels) + Channel));
          }
        }
      }

      // skip to next rank if no controllers/channels enabled
      if (McChBitMask == 0) {
        continue;
      }
      // Search row fail list and apply PPR repairs as needed
      if (PprRequired) {
        DispositionFailRangesWithPprFlow(MrcData, PprAmtData, McChBitMask, RepairDone);
      }
    } // for Rank
  } // RowFailuresFound

  return RetVal;
} // PprDispositionFailRange

/**
  Executes PPR flow on correctable failures in the row failure list

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     PprAmtData          - PPR and AMT data structure
  @param [in]     McChBitMask         - Memory Controller Channel Bit mask to update
  @param [in,out] *RepairDone         - Flag to indicate the repair was done

  @retval status - MRC_STATUS_SUCCESS/MRC_STATUS_FAILURE

**/
MrcStatus
DispositionFailRangesWithPprFlow (
  IN     MrcParameters      *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN     UINT32                     McChBitMask,
  IN OUT UINT8                      *RepairDone
)
{
  ROW_FAIL_RANGE      *FailRangePtr;
  UINT8               Controller;
  UINT8               Channel;
  UINT8               Bank;
  UINT8               BankInterleaveIndex;
  UINT8               PprBank;
  UINT8               Byte;
  UINT16              ByteMask;
  UINT32              FailIndex;
  ROW_ADDR            CurAddr;
  MrcStatus           Status;
  BOOLEAN             RepairStatus = FALSE;
  UINT8               MaxChDdr;
  MrcOutput           *Outputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT32              DqMask[DQ_MASK_INDEX_MAX];
  UINT32              NumRowsError;
  UINT32              NumRepairsIssued;
  UINT32              NumRepairsSuccessful;
  UINT8               McChRowTestBitMask;
  INT64               DisableRefreshMask;

  Outputs = &MrcData->Outputs;
  ExtInputs = MrcData->Inputs.ExtInputs.Ptr;
  MaxChDdr = Outputs->MaxChannels;
  MrcGetMcConfigGroupLimits (MrcData, GsmMccRefreshRankMask, NULL, &DisableRefreshMask, NULL);
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "DispositionFailRangesWithPprFlow Starts:  McChBitMask=0x%x\n", McChBitMask);

  // Inspect test results for given rank and bank
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      // Was this channel part of the test group?
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChDdr) == 0) {
        continue;
      }
      NumRowsError = 0;
      NumRepairsIssued = 0;
      NumRepairsSuccessful = 0;

      // For each bank pair
      for (Bank = 0; Bank < (1 << PprAmtData->BaseBits); Bank++) {
        CurAddr.Data = 0;
        CurAddr.Bits.BankPair = Bank;
        CurAddr.Bits.Rank = PprAmtData->Rank;
        CurAddr.Bits.Valid = 1;

        // Loop through all row failure ranges
        for (FailIndex = 0; FailIndex < AmtGetRowFailListSize (MrcData, Controller, Channel); FailIndex++) {
          FailRangePtr = AmtGetRowFailRange (MrcData, Controller, Channel, FailIndex);
          if (ExtInputs->PprRepairType == NOREPAIR_PPR) {
            // Set RepairedMask
            if (FailRangePtr->BankGroupRepairedMask != FailRangePtr->BankGroupMask) {
              FailRangePtr->BankGroupRepairedMask = FailRangePtr->BankGroupMask;
              NumRowsError++;
            }
            continue;
          }

          // Check for address match, masking row number
          if (AmtCurrentAddrMatch (CurAddr, FailRangePtr->Addr)) {
            Status = mrcFail;
            for (BankInterleaveIndex = 0; BankInterleaveIndex < 8; BankInterleaveIndex++) {
              // If current BG has a failure.
              if (IsRowFailureRepairRequired(FailRangePtr, BankInterleaveIndex)) {

                // Get the failing bank address. The interleavingIndex contains the upper bank(bank group) address bit.
                PprBank = (BankInterleaveIndex << PprAmtData->BaseBits) + Bank;

                DqMask[0] = 0xFFFFFFFF;
                DqMask[1] = (Outputs->EccSupport) ? 0xFF : 0;

                // Construct DRAM Mask
                ByteMask = 0;
                for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                  //
                  // Set byte mask bits based on failing DQ bits
                  //
                  if ((DqMask[Byte / 4] >> (((Byte * 8) % 32))) & 0xFF) {
                    ByteMask |= 1 << Byte;
                  }
                }
                //
                // If ByteMask bits are set, then execute a PPR repair flow
                //
                if (ByteMask) {

                  NumRowsError++;

                  RepairStatus = FALSE;

                  // Test on paired channels for LPDDR
                  if (Outputs->IsLpddr) {
                    McChRowTestBitMask = 3 << ((Controller * Outputs->MaxChannels) + 2 * LP_IP_CH(Outputs->IsLpddr, Channel));
                  } else {
                    McChRowTestBitMask = 1 << MC_CH_IDX(Controller, Channel, Outputs->MaxChannels);
                  }
                  // Run clean-up before PPR execution
                  PprCleanup (MrcData, PprAmtData, McChRowTestBitMask);
                  // Test row
                  RowTestPpr(MrcData, PprAmtData, McChRowTestBitMask, PprBank, FailRangePtr->Addr.Bits.Row);
                  // Disable refresh
                  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRefreshRankMask,  WriteCached, &DisableRefreshMask);

                  // Attempt PPR repair
                  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Calling PPR for Mc%d, Ch%d, Rank: 0x%x, BG: 0x%x, Bank: 0x%x Row: 0x%x, ByteMask = 0x%x\n",
                    Controller, Channel, PprAmtData->Rank, BankInterleaveIndex, Bank, FailRangePtr->Addr.Bits.Row, ByteMask);
                  Status = MrcPostPackageRepair (MrcData, Controller, Channel, PprAmtData->Rank, BankInterleaveIndex, Bank, FailRangePtr->Addr.Bits.Row, ByteMask);
                  ++NumRepairsIssued;
                  FailRangePtr->BankGroupAttempts[BankInterleaveIndex]++;

                  // Post-repair analysis
                  if (Status == mrcSuccess && (RowTestPpr(MrcData, PprAmtData, McChRowTestBitMask, PprBank, FailRangePtr->Addr.Bits.Row) == mrcSuccess)) {
                    ++NumRepairsSuccessful;
                    RepairStatus = TRUE;
                  } else {
                    Outputs->PprRepairFails++;
                  }
                  // Clean up after PPR execution
                  PprCleanup (MrcData, PprAmtData, McChRowTestBitMask);
                } // if (ByteMask)

                if (RepairStatus) {
                  *RepairDone = 1;
                  FailRangePtr->BankGroupRepairedMask |= MRC_BIT0 << BankInterleaveIndex;
                  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "Repair Success\n");
                } else {
                  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "Repair Fail\n");
                }
              } // if ((FailRangePtr->BankGroupMask
            } // BankInterleaveIndex
          } // CurrentAddrMatch
        } // FailIndex
        if (ExtInputs->PprRepairType == NOREPAIR_PPR) {
          MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "PPR repair flow is disabled via PprRepairType setup option.\n");
          break;
        }
      } // Bank pair
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "DispositionFailRangesWithPprFlow: Rank%d Mc%d Ch%d:\n  Number of rows with errors = %d\n  Number of repairs issued = %d\n  Number of repairs successful = %d\n",
        PprAmtData->Rank, Controller, Channel, NumRowsError, NumRepairsIssued, NumRepairsSuccessful);
      Outputs->PprNumSuccessfulRepairs += (UINT16) NumRepairsSuccessful;
    } // Ch
  } // Controller

  return mrcSuccess;
} // DispositionFailRangesWithPprFlow

/**
  Check to see if Retry is required after a PPR repair

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  RepairDone  - Indicator whether PPR repair was done
  @param[in]  RetryCount  - Count of retry attempts

  @retval TRUE if Retry is required; FALSE otherwise
**/
BOOLEAN
IsAmtRetryRequiredAfterRepair (
  IN MrcParameters *const MrcData,
  IN UINT8                RepairDone,
  IN UINT32               RetryCount
  )
{
  MrcDebug  *Debug;
  BOOLEAN   RetryRequired;

  Debug = &MrcData->Outputs.Debug;

  if (RetryCount == 0) {
    RetryRequired = TRUE;
  } else if ((RepairDone != 0) && (RetryCount < PPR_REPAIR_RETRY_LOOPS)) {
    RetryRequired = TRUE;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nPPR repair done so retry MemTest!\n");
  } else {
    RetryRequired = FALSE;
  }
  return RetryRequired;
}

/**

  Inject error for given Rank/Controller/Channel

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  PprAmtData    - PPR and AMT data structure
  @param[in]  Controller    - 0-based index to controller
  @param[in]  Channel       - 0-based index to channel
  @param[in]  Bank          - bank address number
  @param[in]  Row           - row address
  @param[in]  TestSize      - number of rows to test
  @param[in]  ErrInjMask16  - Bitmask of DQ lanes to inject error

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
InjectMemtestError (
  IN MrcParameters *const           MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         Bank,
  IN UINT32                         Row,
  IN UINT32                         TestSize,
  IN UINT16                         ErrInjMask16
  )
{
  MrcStatus         Status = mrcSuccess;
  MrcOutput         *Outputs;
  MrcDimmOut        *DimmOut;
  MRC_FUNCTION      *MrcCall;
  UINT8             LocalMcChBitMask;
  UINT8             RankMask;
  UINT32            ChannelLoop;
  UINT32            ControllerLoop;
  UINT32            CurrentRow[MAX_CONTROLLER][MAX_IP_CHANNEL];
  UINT32            TestSizeArray[MAX_CONTROLLER][MAX_IP_CHANNEL];
  UINT32            DeviceMask[MAX_CONTROLLER][MAX_CHANNEL][DQ_MASK_INDEX_MAX];
  UINT8             IpChannel;
  UINT32            Rank;

  Outputs         = &MrcData->Outputs;
  MrcCall         = MrcData->Inputs.Call.Func;
  Rank            = PprAmtData->Rank;

  MrcCall->MrcSetMem ((UINT8 *) DeviceMask, sizeof (DeviceMask), 0);
  MrcCall->MrcSetMem ((UINT8 *) CurrentRow, sizeof (CurrentRow), 0);
  MrcCall->MrcSetMem ((UINT8 *) PprAmtData->RowBits, sizeof (PprAmtData->RowBits), 0); // Only 1 row in test
  MrcCall->MrcSetMem ((UINT8 *) PprAmtData->BankBits, sizeof (PprAmtData->BankBits), 0); // Only 1 bank in test
  MrcCall->MrcSetMem ((UINT8 *) PprAmtData->ColumnBits, sizeof (PprAmtData->ColumnBits), 0);

  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "Error injection beginning\n");

  // Set FromRowTestPpr = TRUE since we need to program a single bank
  PprAmtData->FromRowTestPpr = TRUE;
  //manually inject error here (only one controller/channel?)
  //hardcode a bunch of dimm settings
  LocalMcChBitMask = 0; // Reset bitmask for new Rank

  RankMask = (1 << Rank);
  LocalMcChBitMask |= SelectReutRanks (MrcData, (UINT8)Controller, (UINT8)Channel, RankMask, FALSE);
  for (ControllerLoop = 0; ControllerLoop < MAX_CONTROLLER; ControllerLoop++) {
    for (ChannelLoop = 0; ChannelLoop < Outputs->MaxChannels; ChannelLoop++) {
      DimmOut = &Outputs->Controller[ControllerLoop].Channel[ChannelLoop].Dimm[RANK_TO_DIMM_NUMBER (Rank)];
      IpChannel = (UINT8) LP_IP_CH (Outputs->IsLpddr, ChannelLoop);
      if (MC_CH_MASK_CHECK (LocalMcChBitMask, ControllerLoop, ChannelLoop, Outputs->MaxChannels) == 0) {
        if (!IS_MC_SUB_CH (Outputs->IsLpddr, ChannelLoop)) {
          TestSizeArray[ControllerLoop][IpChannel] = 0;
        }
        continue;
      } else {
        CurrentRow[ControllerLoop][IpChannel] = Row;
        TestSizeArray[ControllerLoop][IpChannel] = TestSize;
        // Initialize per-Rank Column Row and Bank sizes
        // The MrcLog2 function returns +1 so we subtract 1
        PprAmtData->ColumnBits[ControllerLoop][ChannelLoop] = MrcLog2 (MrcData, DimmOut->ColumnSize) - 1;
        MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "ErrInj! Rank[%d]Cont[%d]Channel[%d]:\n\tColumnBits=%d\n\tRowBits=%d\n\tBankBits=%d\n",
          Rank, Controller, Channel, PprAmtData->ColumnBits[Controller][Channel],
          PprAmtData->RowBits[Controller][Channel], PprAmtData->BankBits[Controller][Channel]);
      }
    }
  }

  PprAmtData->Bank = (Outputs->IsLpddr) ? (Bank & 0xF) : Bank;  // For LP5, limit injected bank to between 0 and 15

  // Reset Some cpgc registers for AdvMtWcMats8
  SetupIOTestRetention (MrcData, PprAmtData, LocalMcChBitMask, 1);
  if (PprAmtData->TestType == AdvMtMmrw) {
    MrcProgramDSPattern (MrcData, LocalMcChBitMask, PprAmtData->DataPattern, ErrInjMask16);
  } else {
    MrcProgramMATSPattern (MrcData, LocalMcChBitMask, PprAmtData->DataPattern, PprAmtData->DataPatternDepth, PprAmtData->UiShl, ErrInjMask16);
  }
  Status = AmtExecuteRowRangeTest (MrcData, PprAmtData, LocalMcChBitMask, CurrentRow, TestSizeArray);

  return Status;
}

/**
  Wrapper function for the Advanced Memory test algorithm that tracks time and splits read/write patterns.

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] CmdPat         - Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  @param[in] Pattern        - Array of 64-bit Data Pattern for the test
  @param[in] IsUseInvtPat   - Info to indicate whether or not patternQW is inverted by comparing original pattern
  @param[in] Direction      - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DOWN
  @param[in] PatternNumber  - Which pattern in the MATS8 sequence is this test

  @retval None
**/
VOID
MrcRunPprDetection (
  IN MrcParameters *const           MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          CmdPat,
  IN UINT64                         Pattern[],
  IN BOOLEAN                        IsUseInvtPat,
  IN UINT8                          Direction,
  IN UINT8                          PatternNumber
  )
{
  const MRC_FUNCTION   *MrcCall;
  UINT64               StartTime;
  UINT32               ElapsedTime;

  MrcCall = (&MrcData->Inputs)->Call.Func;
  PprAmtData->IsUseInvtPat = IsUseInvtPat;
  PprAmtData->Direction = Direction;
  PprAmtData->PatternNumber = PatternNumber;
  PprAmtData->CmdPat = CmdPat;
  MrcCall->MrcSetMem((UINT8 *) PprAmtData->DataPattern, 0, sizeof (PprAmtData->DataPattern));
  MrcCall->MrcCopyMem((UINT8 *) PprAmtData->DataPattern, (UINT8 *) Pattern, sizeof (Pattern[0]) * PprAmtData->DataPatternDepth);

  if (PprAmtData->TestStatus == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Pattern #%d Begin\n", PatternNumber);
    StartTime = MrcCall->MrcGetCpuTime();
    if (PprAmtData->CmdPat == PatRdWr) {
      // Ensure all reads are done before any writes occur
      // Need to split Rd/Wr at this level to ensure SetupIOTestRetention configures correctly for PatRd and PatWr tests
      PprAmtData->CmdPat = PatRd;
      MrcAdvancedMemTest(MrcData, PprAmtData);
      if (PprAmtData->TestStatus != mrcFail) {
        PprAmtData->CmdPat = PatWr;
        // Invert write cmd if SubSequence 1 was set to be inverted
        PprAmtData->IsUseInvtPat ^= PprAmtData->SubSeqDataInv[1];
        MrcAdvancedMemTest(MrcData, PprAmtData);
      }
    } else {
      MrcAdvancedMemTest(MrcData, PprAmtData);
    }
    ElapsedTime = (UINT32)(MrcCall->MrcGetCpuTime() - StartTime);
    PprAmtData->CurTestTime += ElapsedTime;
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Pattern #%d End, ElapsedTime = %dms\n\n", PatternNumber, ElapsedTime);
  }
}

/**
  Pause refreshes during Data Retention memory test

  @param[in] MrcData - Global MRC data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
AdvMemTestPauseDataRet (
  IN MrcParameters* const MrcData
)
{
  UINT32 DelayTimeNs;
  INT64  GetSetDis;
  INT64  GetSetEn;

  GetSetDis = 0;
  GetSetEn = 1;

  // If CTE, shorten wait time
  DelayTimeNs = 32000000;  // 32ms is 1x refresh interval for DDR5 and LP5

  // Ensure all pages closed and rows are refreshed
  MrcWait (MrcData, DelayTimeNs);

  // Disable refresh commands
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetDis);

  // Pause for given test delay
  MrcWait (MrcData, DelayTimeNs);

  // Enable refresh commands
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetEn);

  // Ensure all rows are refreshed
  MrcWait (MrcData, DelayTimeNs);

  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "AdvMemTestPauseDataRet = %d ms\n", DelayTimeNs / 1000000);

  return mrcSuccess;
} // AdvMemTestPauseDataRet

/**
  Runs Advanced Memory Tests based on the MATS + algorithm

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] InvertedPassEn - Whether to run MATS+ patterns 4-6, which are the same as patterns 1-3 but with inverted data

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMATSN (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN BOOLEAN                        InvertedPassEn
)
{
  const MRC_FUNCTION   *MrcCall;
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                Test;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               BasePattern;
  UINT64               Pattern[1];
  UINT64               InvPattern[1];
  UINT32               TotalTestTimeMATS8 = 0;
  UINT32               Controller;
  UINT32               Channel;

  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start MemTestMATS8\n");

  /**
  8-bit Pattern, where a bitshift of 1 is applied on each UI
  0101010101010101h
  0202020202020202h
  0404040404040404h
  0808080808080808h
  1010101010101010h
  2020202020202020h
  4040404040404040h
  8080808080808080h
  0101010101010101h
  **/
  BasePattern = 0x0101010101010101ULL;
  Controller = Outputs->FirstPopController;
  Channel = Outputs->Controller[Controller].FirstPopCh;
  for (Test = 0; Test < PPR_TEST_DETECTION_LOOPS; Test++) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);

    Pattern[0] = MrcCall->MrcLeftShift64 (BasePattern, Test);
    InvPattern[0] = ~(Pattern[0]);
    RepairDone = 1;
    RetryCount = 0;
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      PprAmtData->DataPatternDepth = 1;
      PprAmtData->UiShl = 1;  // Indicate a bit-shift of 1 for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0; // Indicates if CPGC should invert Data pattern for SubSequence 0
      PprAmtData->SubSeqDataInv[1] = 1; // Indicates if CPGC should invert Data pattern for SubSequence 1 (only applicable during PatRdWr tests, with 2 SubSequence commands)
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;
      //1. Write sliding data pattern to all of memory
      MrcRunPprDetection(MrcData, PprAmtData, PatWr, Pattern, FALSE, MT_ADDR_DIR_UP, 1);

      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        // Rank 0, Controller 0, Ch 0, Bank 5, Row 1, TestSize 1, ErrInjMask16 0x000F
        if (Test == 0 && RetryCount == 1) {
          PprAmtData->Rank = 0;
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 5, 1, 1, 0x0F);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 4, 7, 1, 0x0C);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 27, 0xE, 1, 0x01);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 22, 0xE, 1, 0x02);
        }
      }

      //2. Read and compare data pattern
      //   Write inverse pattern
      if (InvertedPassEn == TRUE) {
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, Pattern, FALSE, MT_ADDR_DIR_UP, 2);
      } else {
        // Modified step: Use down direction to pressure tWR on upper BG address
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, Pattern, FALSE, MT_ADDR_DIR_DOWN, 2);
      }

      //3. Read and compare inverse pattern
      //   Write original pattern
      if (InvertedPassEn == TRUE) {
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, InvPattern, FALSE, MT_ADDR_DIR_DOWN, 3);
      } else {
        // Modified step: Just read and compare the inverse pattern without writing the original pattern
        MrcRunPprDetection(MrcData, PprAmtData, PatRd, InvPattern, TRUE, MT_ADDR_DIR_DOWN, 3);
      }

      // Make another MATS+ pass with inverted data
      if (InvertedPassEn == TRUE) {
        //4. Write inverse pattern to all of memory
        MrcRunPprDetection(MrcData, PprAmtData, PatWr, InvPattern, FALSE, MT_ADDR_DIR_UP, 4);

        //5. Read and compare inverse pattern
        //   Write original pattern
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, InvPattern, FALSE, MT_ADDR_DIR_UP, 5);

        //6. Read and compare original pattern
        //   Write inverse pattern
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, Pattern, FALSE, MT_ADDR_DIR_DOWN, 6);
      } // InvertedPassEn

      TotalTestTimeMATS8 += PprAmtData->CurTestTime;
      if (PprAmtData->TestStatus == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (Test == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData, PprAmtData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
  }
#endif
        PprDispositionFailRange(MrcData, PprAmtData, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "MemTestMATS8: Test #%d Failed!\n\n", Test);
        break;
      }
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "End Test #%d, TotalTime Elapsed = %dms\n", Test, PprAmtData->CurTestTime);
    } // while
    if (PprAmtData->TestStatus != mrcSuccess) {
      break;
    }
  } // Test

  if (PprAmtData->TestStatus == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All MemTestMATS8 Tests Completed! TotalTime Elapsed for all MemTestMATS8 Tests: %dms\n", TotalTestTimeMATS8);
  }
#if POISON_ROW_FAIL_LIST
  AmtPrintTotalRowFailures(MrcData);
#endif

  PprAmtData->TotalTestTime += TotalTestTimeMATS8;
  return PprAmtData->TestStatus;
}

/**
  Runs Advanced Memory Test targeting Data retention between refresh

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestDataRetention (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
  )
{
  const MRC_FUNCTION   *MrcCall;
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                Test;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               Pattern[8];
  UINT32               TotalTestTimeDataRet = 0;
  UINT32               Controller;
  UINT32               Channel;

  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start MemTestDataRetention\n");
  for (Test = 0; Test < 2; Test++) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);
    switch (Test) {
      case 0:
        // Copy cacheline1 into Pattern
        MrcCall->MrcCopyMem((UINT8 *) Pattern, (UINT8 *) AmtCacheLine1, sizeof (AmtCacheLine1));
        break;
      case 1:
        // Copy cacheline2 into Pattern
        MrcCall->MrcCopyMem((UINT8 *) Pattern, (UINT8 *) AmtCacheLine2, sizeof (AmtCacheLine2));
        break;
    }

    RepairDone = 1;
    RetryCount = 0;
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      PprAmtData->DataPatternDepth = 8;
      PprAmtData->UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0;
      PprAmtData->SubSeqDataInv[1] = 0;
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;

      //1. Write data pattern to all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, Pattern, FALSE, MT_ADDR_DIR_UP, 1);

      if (PprAmtData->TestStatus == mrcSuccess) {
        PprAmtData->TestStatus = AdvMemTestPauseDataRet (MrcData);
      }

      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        // Rank 0, Controller 0, Ch 0, Bank 0, Row 1, TestSize 1, ErrInjMask16 0x000F
        if (Test == 0 && RetryCount == 1) {
          PprAmtData->Rank = 0;
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 5, 1, 1, 0x0F);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 4, 4, 1, 0x0C);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 27, 0x7, 1, 0x01);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 22, 0x7, 1, 0x02);
        }
      }

      //2. Read and compare data pattern
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, Pattern, FALSE, MT_ADDR_DIR_UP, 2);

      TotalTestTimeDataRet += PprAmtData->CurTestTime;
      // Verify that the last test was successful
      if (PprAmtData->TestStatus == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (Test == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData, PprAmtData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
        }
#endif
        // Disposition any failures
        PprDispositionFailRange(MrcData, PprAmtData, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "MemTestDataRetention: Test #%d Failed!\n\n", Test);
        break;
      }
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "End Test #%d, TotalTime Elapsed = %dms\n", Test, PprAmtData->CurTestTime);
    } // while
    if (PprAmtData->TestStatus != mrcSuccess) {
      break;
    }
  } // Test

  if (PprAmtData->TestStatus == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All MemTestDataRetention Tests Completed! TotalTime Elapsed for all MemTestDataRetention Tests: %dms\n", TotalTestTimeDataRet);
  }

  PprAmtData->TotalTestTime += TotalTestTimeDataRet;

  return PprAmtData->TestStatus;
} // MemTestDataRetention

/**
  Runs Advanced Memory Test march pattern

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
**/
MrcStatus
MemTestMarch (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
)
{
  const MRC_FUNCTION   *MrcCall;
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               Pattern[16];
  UINT8                PatternDepth;
  UINT32               TotalTestTimeMarch = 0;
  const CHAR8          *TestName;
  UINT32               Controller;
  UINT32               Channel;
  UINT8                PatBufShift;


  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  // Copy cacheline1 into Pattern
  switch (PprAmtData->TestType) {
    case AdvMtXMarch:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));
      PatternDepth = 16;
      TestName = "MemTestXMarch";
      break;
    case AdvMtXMarchG:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchGPattern, sizeof(AmtXMarchGPattern));
      PatternDepth = 8;
      TestName = "MemTestXMarchG";
      break;
    case AdvMtYMarchShort:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));  // Y March Short pattern is the same as X March
      PatternDepth = 16;
      TestName = "MemTestYMarchShort";
      break;
    case AdvMtYMarchLong:
      MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));  // Y March Long pattern is the same as X March
      PatternDepth = 16;
      TestName = "MemTestYMarchLong";
      break;
    default:
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MemTestMarch: Unsupported test type %d\n", PprAmtData->TestType);
      return mrcFail;
  }

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start %s\n", TestName);

  for (PatBufShift = 0; (PatBufShift < 4) && (PprAmtData->TestStatus == mrcSuccess); PatBufShift++) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PatBufShift=%d\n", PatBufShift);
    Outputs->PprPatBufShift = PatBufShift << 3; // Shift by 8 * PatBufShift
    RepairDone = 1;
    RetryCount = 0;
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      PprAmtData->DataPatternDepth = PatternDepth;
      PprAmtData->UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0;
      PprAmtData->SubSeqDataInv[1] = 1;
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;

      //1. Execute write data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, Pattern, FALSE, MT_ADDR_DIR_UP, 1);

      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        if (RetryCount == 1) {
          PprAmtData->Rank = 0;
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 5, 1, 1, 0x0F);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 4, 4, 1, 0x0C);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 27, 0x7, 1, 0x01);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 22, 0x7, 1, 0x02);
        }
      }

      //2. Execute read data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, Pattern, FALSE, MT_ADDR_DIR_UP, 2);

      TotalTestTimeMarch += PprAmtData->CurTestTime;
      // Verify that the last test was successful
      if (PprAmtData->TestStatus == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (PatBufShift == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData, PprAmtData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
        }
#endif
        // Disposition any failures
        PprDispositionFailRange(MrcData, PprAmtData, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "%s: Test Failed!\n\n", TestName);
        break;
      }
    } // while
    if (PprAmtData->TestType == AdvMtXMarchG) {
      break;  // XMarchG has one data instruction and does not need to shift pattern
    } else if (PatBufShift < 3) {
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Rerun with next PatBufShift=%d (different data pattern)\n", PatBufShift + 1);
    }
  } // for

  Outputs->PprPatBufShift = 0;

  if (PprAmtData->TestStatus == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All %s Tests Completed! TotalTime Elapsed: %dms\n", TestName, TotalTestTimeMarch);
    PprAmtData->TotalTestTime += TotalTestTimeMarch;
  }
  return PprAmtData->TestStatus;
} // MemTestMarch

  /**
  Runs Advanced Memory Test march pattern with different data pattern and test flow

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
  **/
MrcStatus
MemTestMarchAlt (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
)
{
  const MRC_FUNCTION   *MrcCall;
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               Pattern[1];
  UINT64               InvPattern[1];
  UINT32               TotalTestTimeMarch = 0;
  const CHAR8          *TestName;
  UINT32               Controller;
  UINT32               Channel;
  UINT8                Test;


  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  switch (PprAmtData->TestType) {
    case AdvMtXMarch:
      TestName = "MemTestXMarchAlt";
      break;
    case AdvMtYMarchShort:
      TestName = "MemTestYMarchShortAlt";
      break;
    default:
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MemTestMarchAlt: Unsupported test type %d\n", PprAmtData->TestType);
      return mrcFail;
  }

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start %s\n", TestName);

  for (Test = 0; (Test < 8) && (PprAmtData->TestStatus == mrcSuccess); Test++) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);
    // Copy cacheline1 into Pattern
    MrcCall->MrcCopyMem ((UINT8*) Pattern, (UINT8*) &AmtMarchAltPattern[Test], sizeof(Pattern));
    InvPattern[0] = ~(Pattern[0]);
    RepairDone = 1;
    RetryCount = 0;
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      PprAmtData->DataPatternDepth = 1;
      PprAmtData->UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0;
      PprAmtData->SubSeqDataInv[1] = 1;
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;

      //1. Execute write data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, Pattern, FALSE, MT_ADDR_DIR_UP, 1);

      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        if (RetryCount == 1) {
          PprAmtData->Rank = 0;
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 5, 1, 1, 0x0F);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 4, 4, 1, 0x0C);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 27, 0x7, 1, 0x01);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 22, 0x7, 1, 0x02);
        }
      }

      //2. Execute read data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, Pattern, FALSE, MT_ADDR_DIR_UP, 2);

      //3. Execute inverse write data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, InvPattern, FALSE, MT_ADDR_DIR_UP, 3);

      //4. Execute inverse read data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, InvPattern, FALSE, MT_ADDR_DIR_UP, 4);

      TotalTestTimeMarch += PprAmtData->CurTestTime;
      // Verify that the last test was successful
      if (PprAmtData->TestStatus == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (Test == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData, PprAmtData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
        }
#endif
        // Disposition any failures
        PprDispositionFailRange(MrcData, PprAmtData, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "%s: Test Failed!\n\n", TestName);
        break;
      }
    } // while
  } // for

  if (PprAmtData->TestStatus == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All %s Tests Completed! TotalTime Elapsed: %dms\n", TestName, TotalTestTimeMarch);
    PprAmtData->TotalTestTime += TotalTestTimeMarch;
  }
  return PprAmtData->TestStatus;
} // MemTestMarchAlt

  /**
  Runs Advanced Memory Test write/read test using multiple types of alternating data patterns

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure

  @retval MrcStatus - mrcSuccess/mrcFail
  **/
MrcStatus
MemTestMmrw (
  IN MrcParameters          *const  MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData
)
{
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                Test;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT64               Pattern[2];
  UINT32               TotalTestTimeMmrw = 0;
  UINT32               Controller;
  UINT32               Channel;

  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start MemTestMmrw\n");
  for (Test = 0; Test < NUM_AMT_MMRW_TESTS; Test++) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);
    switch (Test) {
      case 0:
        Pattern[0] = AmtMmrwTestPattern1;
        break;
      case 1:
        Pattern[0] = AmtMmrwTestPattern2;
        break;
      case 2:
        Pattern[0] = AmtMmrwTestPattern3;
        break;
    }

    RepairDone = 1;
    RetryCount = 0;
    // Current retry limit is 10
    while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++)) {
      PprAmtData->DataPatternDepth = 2;
      PprAmtData->UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0;
      PprAmtData->SubSeqDataInv[1] = 1;
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;

      //1. Write data pattern to all of memory
      Pattern[1] = 0; // Do not invert data pattern
      MrcRunPprDetection(MrcData, PprAmtData, PatWr, Pattern, FALSE, MT_ADDR_DIR_UP, 1 + (Test * NUM_AMT_MMRW_PATTERNS_PER_TEST));

      if (Inputs->ExtInputs.Ptr->PprErrorInjection != 0) {
        // Rank 0, Controller 0, Ch 0, Bank 0, Row 1, TestSize 1, ErrInjMask16 0x000F
        if (Test == 0 && RetryCount == 1) {
          PprAmtData->Rank = 0;
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 5, 1, 1, 0x0F);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 4, 4, 1, 0x0C);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 27, 0x7, 1, 0x01);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 22, 0x7, 1, 0x02);
        }
      }

      //2. Read and compare data pattern
      MrcRunPprDetection(MrcData, PprAmtData, PatRd, Pattern, FALSE, MT_ADDR_DIR_UP, 2 + (Test * NUM_AMT_MMRW_PATTERNS_PER_TEST));

      //3. Write inverse pattern
      Pattern[1] = 1; // Invert data pattern
      MrcRunPprDetection(MrcData, PprAmtData, PatWr, Pattern, FALSE, MT_ADDR_DIR_UP, 3 + (Test * NUM_AMT_MMRW_PATTERNS_PER_TEST));

      //4. Read and compare inverse pattern
      MrcRunPprDetection(MrcData, PprAmtData, PatRd, Pattern, FALSE, MT_ADDR_DIR_UP, 4 + (Test * NUM_AMT_MMRW_PATTERNS_PER_TEST));

      TotalTestTimeMmrw += PprAmtData->CurTestTime;
      // Verify that the last test was successful
      if (PprAmtData->TestStatus == mrcSuccess) {
#if POISON_ROW_FAIL_LIST
        if (Test == 0) { // only poison first test
          AmtPoisonRowFailList(MrcData, PprAmtData);
          MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_NOTE, "Finished poisoning row fail list, print TotalRowFailures\n");
          AmtPrintTotalRowFailures(MrcData);
        }
#endif
        // Disposition any failures
        PprDispositionFailRange(MrcData, PprAmtData, &RepairDone);
      } else {
        Outputs->PprNumDetectedErrors = PPR_MAX_DETECTED_ERRORS; // Too many errors
        MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "MemTestMmrw: Test #%d Failed!\n\n", Test);
        break;
      }
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "End Test #%d, TotalTime Elapsed = %dms\n", Test, PprAmtData->CurTestTime);
    } // while
    if (PprAmtData->TestStatus != mrcSuccess) {
      break;
    }
  } // Test

  if (PprAmtData->TestStatus == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All MemTestMmrw Tests Completed! TotalTime Elapsed for all MemTestMmrw Tests: %dms\n", TotalTestTimeMmrw);
  }

  PprAmtData->TotalTestTime += TotalTestTimeMmrw;

  return PprAmtData->TestStatus;
} // MemTestMmrw

/**
  Top-level PPR function that determines which memory tests should be run.
  Executes Post Package Repair flow to repair failures found during memory test.

  @param[in]  MrcData      - Pointer to global MRC data.

  @retval MrcStatus - mrcSuccess if the entire memory test/PPR flow executes, otherwise mrcFail if it is cut short
**/
MrcStatus
MrcPostPackageRepairEnable (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcOutput           *Outputs;
  MrcInput            *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcStatus           Status;
  UINT8               Test;
  UINT32              Controller;
  UINT32              Channel;
  UINT32              IpChannel;
  UINT32              BankIndex;
  INT64               GetSetDis;
  BOOLEAN             InvertedPassEn;
  INT64               GetSetVal;
  INT64               EnableSrSave;
  UINT32              SavedCpgc20Credits[MAX_CONTROLLER];
  BOOLEAN             IsUlxUlt;
  BOOLEAN             SaveCpgcGlobalStart;
  UINT8               BG;
  UINT8               FirstChannel;
  MrcChannelOut       *ChannelOut;
  UINT8               Rank;
  MRC_MC_AD_SAVE      MadSavedValues;
  PPR_AMT_PARAMETER_DATA  PprAmtData;
  MRC_BG_BANK_PAIR    BankMappingSave[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS];

  Status          = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  ExtInputs       = Inputs->ExtInputs.Ptr;
  MrcCall         = Inputs->Call.Func;
  GetSetDis       = 0;
  InvertedPassEn  = TRUE;
  IsUlxUlt        = Inputs->IsDdrIoUlxUlt;
  SaveCpgcGlobalStart = MrcData->Save.Data.CpgcGlobalStart;
  MrcData->Save.Data.CpgcGlobalStart = FALSE;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);
      Cpgc20SetGlobalBind (MrcData, Controller, IpChannel, FALSE);
    }
  }
  // PprAmtData TotalTestTime is initialized to 0
  MrcCall->MrcSetMem ((UINT8 *) &PprAmtData, sizeof(PprAmtData), 0);
  PprAmtData.BaseBits = GetBaseBits (MrcData);

  /*
   *  Run PPR at:
   *  1) Coldboot
   *  2) Fastboot and users config PprRunAtFastboot to TRUE
   */
  if ((Inputs->BootMode == bmFast) && (ExtInputs->PprRunAtFastboot == 0)) {
    return mrcSuccess;
  }

  MrcModifyRdRdTimings (MrcData, TRUE);

  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcSaveEnum, &MadSavedValues);

  GetSetVal = 1;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  Outputs->PprRunningState = PPR_IS_RUNNING;

  Outputs->PprNumDetectedErrors = 0;
  Outputs->PprRepairFails = 0;
  Outputs->PprForceRepairStatus = mrcSuccess;

  // Store all Bank L2P mapping data so it can be restored later
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      Status = Cpgc20GetSetBankSequence (MrcData, Controller, Channel, BankMappingSave[Controller][Channel], MAX_BANKS, MRC_GET);
      if (Status != mrcSuccess) {
        break;
      }
    }

    if (Status != mrcSuccess) {
      break;
    }
  }
  // If function call fails for some reason, set BankMapping to default values
  if (Status != mrcSuccess) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        for (BankIndex = 0; BankIndex < MAX_BANKS; BankIndex++) {
          MrcConvertCpgcBanktoBankAddress(MrcData, BankIndex, &BankMappingSave[Controller][Channel][BankIndex].Bank, &BankMappingSave[Controller][Channel][BankIndex].BankGroup);
        }
      }
    }
  }

  Status = mrcSuccess;


  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PprTestType=0x%02x\n", ExtInputs->PprTestType);
  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PprRepairType=0x%02x\n", ExtInputs->PprRepairType);

  GetSetVal = 1;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetVal);

  MrcGetSetMc (MrcData, Outputs->FirstPopController, GsmMccOppSrefEnable, ReadNoCache, &EnableSrSave);  // Save state of Self Refresh
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccOppSrefEnable, WriteNoCache, &GetSetDis); // Disable Self Refresh

  MrcModifyMcAddressDecoderValues (MrcData);

  MrcCall->MrcSetMemDword (SavedCpgc20Credits, ARRAY_COUNT (SavedCpgc20Credits), RDCPL_CREDITS_SCRUB);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if ((MrcControllerExist (MrcData, Controller)) && IsUlxUlt) {
       MrcCpgcModifyReadCredits (MrcData, Controller, TRUE, &SavedCpgc20Credits[Controller]);
    }
  }

  if ((ExtInputs->PprRepairPhysicalAddrLow != 0) || (ExtInputs->PprRepairPhysicalAddrHigh != 0)) {
    Status = mrcFail;
    if (ExtInputs->PprRepairController < MAX_CONTROLLER) {
      // Assume BIOS menu passes correct memory address
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Repair Physical address: 0x%x%08x\n", ExtInputs->PprRepairPhysicalAddrHigh, ExtInputs->PprRepairPhysicalAddrLow);
      Status = MrcPostPackageRepair(MrcData, ExtInputs->PprRepairController, ExtInputs->PprRepairChannel, (UINT16)(MAX_RANK_IN_DIMM * ExtInputs->PprRepairDimm + ExtInputs->PprRepairRank), ExtInputs->PprRepairBankGroup, ExtInputs->PprRepairBank, ExtInputs->PprRepairRow, (1 << Outputs->SdramCount) - 1);
    }
    if (Status != mrcSuccess) {
      Outputs->PprRepairFails++;
    }
    goto Done;
  }

  // If no memory tests are enabled, print a warning message
  if ((ExtInputs->PprTestType & ((1 << AdvMtNumMemTests) - 1)) == 0) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MrcPostPackageRepairEnable: Warning - No memory tests are enabled, detection and repair flow will not occur\n");
  }

  Status = mrcSuccess;
  // Run all memory tests
  for (Test = 0; Test < AdvMtNumMemTests; Test++) {
#if POISON_ROW_FAIL_LIST
    PprAmtData.HasRowFailListBeenPoisonedOnThisTest = FALSE;
#endif
    if (ExtInputs->PprTestType & (1 << Test)) {
      PprAmtData.TestType = (MRC_ADVANCED_MEM_TEST_TYPE) Test;
      switch (Test) {
        case PPR_MEMTEST_WCMATS8_BIT:
          Status |= MemTestMATSN(MrcData, &PprAmtData, InvertedPassEn);
          break;
        case PPR_MEMTEST_DATA_RETENTION_BIT:
          Status |= MemTestDataRetention(MrcData, &PprAmtData);
          break;
        case PPR_MEMTEST_XMARCH_BIT:
#if AMT_USE_XMARCH_ALT == 1
          Status |= MemTestMarchAlt(MrcData, &PprAmtData);
          break;
#endif
        case PPR_MEMTEST_XMARCHG_BIT:
        case PPR_MEMTEST_YMARCHSHORT_BIT:
#if AMT_USE_YMARCH_SHORT_ALT == 1
          // If statement is necessary due to fallthrough
          if (Test == PPR_MEMTEST_YMARCHSHORT_BIT) {
            Status |= MemTestMarchAlt(MrcData, &PprAmtData);
            break;
          }
#endif
        case PPR_MEMTEST_YMARCHLONG_BIT:
          Status |= MemTestMarch(MrcData, &PprAmtData);
          break;
        case PPR_MEMTEST_MMRW_BIT:
          Status |= MemTestMmrw(MrcData, &PprAmtData);
          break;
      }
    }
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MrcPostPackageRepairEnable: Last memory test failed, skipping other tests\n");
      break;
    }
    PprCleanup (MrcData, &PprAmtData, Outputs->McChBitMask);  // Clean up programming between different test setups
  }

  if (ExtInputs->PprForceRepair) {  // Force repair sequence
    BG      = 0;
    Status  = mrcSuccess;
    for (Test = 0; Test < 90; Test++) {
      PprCleanup (MrcData, &PprAmtData, Outputs->McChBitMask);
      if (Outputs->IsLpddr && (MrcGetBankBgOrg(MrcData, Outputs->Frequency) != MrcLp5BgMode)) {
        BG = 0;
      } else {
        BG = 3;
      }
      FirstChannel = Outputs->Controller[Outputs->FirstPopController].FirstPopCh;
      ChannelOut = &Outputs->Controller[Outputs->FirstPopController].Channel[FirstChannel];
      // Get first rank
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if ((1 << Rank) & ChannelOut->ValidRankBitMask) {
          break;
        }
      } // Rank
#ifdef MRC_DEBUG_PRINT
      Status |= MrcPostPackageRepair(MrcData, Outputs->FirstPopController, FirstChannel, Rank, BG, 2, 0xAB, 0xF);
      MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "Ppr Status = %d\n", Status);
#else
      MrcPostPackageRepair(MrcData, Outputs->FirstPopController, FirstChannel, Rank, BG, 2, 0xAB, 0xF);
#endif
    }
    Outputs->PprForceRepairStatus |= Status;
    Status = mrcSuccess;
  }

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All Tests Completed! TotalTime Elapsed for all Tests: %dms\n", PprAmtData.TotalTestTime);
  }

Done:

  PprCleanup (MrcData, &PprAmtData, Outputs->McChBitMask);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      // Restore Bank L2P Mapping
      Cpgc20GetSetBankSequence (MrcData, Controller, Channel, BankMappingSave[Controller][Channel], MAX_BANKS, MRC_SET);
    }
  }
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if ((MrcControllerExist(MrcData, Controller)) && IsUlxUlt) {
      MrcCpgcModifyReadCredits(MrcData, Controller, FALSE, &SavedCpgc20Credits[Controller]);
    }
  }
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetDis); // Cleanup, disable refreshes
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccOppSrefEnable, WriteNoCache, &EnableSrSave); // Restore Self Refresh

  // Restore CpgcGlobalStart
  MrcData->Save.Data.CpgcGlobalStart = SaveCpgcGlobalStart;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);
      Cpgc20SetGlobalBind (MrcData, Controller, IpChannel, SaveCpgcGlobalStart);
    }
  }

  MrcModifyRdRdTimings (MrcData, FALSE);

  MrcMcAddressDecoderValuesSaveRestore (MrcData, MrcRestoreEnum, &MadSavedValues);

  GetSetVal = 0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCpgcInOrder, WriteCached, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisAllCplInterleave, WriteCached, &GetSetVal);

  // Get status of PPR resources for MHI report
  GetAllPprResources (MrcData);


  Outputs->PprRunningState = PPR_IS_DONE;

  /*
   * Always return mrcSuccess:
   * 1) BIOS menu will show PPR stat;
   * 2) Avoid brick
   */
  return mrcSuccess;
}

/**
  Set up the controller for Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the current state of the controller.

  @retval None
**/
void
MrcDdr5PprControllerSetup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  INT64 GetSetEn = 1;

  // Save value of page-table idle timer
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, ReadCached, &SaveData->SaveVal0);
  // Disabling page-table idle timer, to allow custom wait time before PRE issued
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &GetSetEn);
}


/**
  Cleanup the MC configuration after Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the controller state prior to PPR.

  @retval None
**/
void
MrcDdr5PprControllerTeardown (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  // Restoring page-table idle timer
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &SaveData->SaveVal0);
}

/**
  Set up the controller for Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the current state of the controller.

  @retval None
**/
void
MrcLpddr5PprControllerSetup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  MrcOutput *Outputs;
  INT64  WckMode;
  INT64  SavetWckStop;
  INT64  tWckStop;
  INT64  tWckOff;
  INT64  RdWckAsync;
  UINT32 WckDelay;
  INT64  GetSetVal;
  INT64  GetSetEn = 1;

  Outputs = &MrcData->Outputs;

  // Save value of page-table idle timer
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, ReadCached, &SaveData->SaveVal0);
  // Disabling page-table idle timer, to allow custom wait time before PRE issued
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &GetSetEn);

  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccLp5WckMode, ReadCached, &WckMode);
  SaveData->SaveVal1 = WckMode;
  if (WckMode == MrcLp5WckSafe) {
    // If in Wck Safe Mode, Extend out twckstop and disable Wck on all channels
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttWckStop, ReadCached, &SavetWckStop);
    SaveData->SaveVal2 = SavetWckStop;
    GetSetVal = MC0_CH0_CR_TC_WCK_tWCKSTOP_MAX;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMcttWckStop, WriteCached, &GetSetVal);
    MrcIssueCas (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MrhCasWck2CkSyncOff, FALSE);

  } else if (WckMode == MrcLp5WckManual) {
    // If Wck is in manual mode ensure Wck has stopped before moving on
    MrcGetSetMcCh (MrcData, Controller, Channel, GSmMcttWckOff, ReadCached, &tWckOff);
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRdWckAsyncGap, ReadFromCache, &RdWckAsync);
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMcttWckStop, ReadFromCache, &tWckStop);
    WckDelay = (UINT32) tWckOff + (UINT32) RdWckAsync + (UINT32) tWckStop;
    WckDelay *= Outputs->Wckps;
    WckDelay /= 1000;

    // Wait for Wck to stop
    MrcWait (MrcData, WckDelay);
  }
}

/**
  Cleanup the MC configuration after Post-Package Repair (PPR).

  @param[in]  MrcData    - Include all MRC global data.
  @param[in]  Controller - the controller to work on
  @param[in]  Channel    - The channel to work on
  @param[out] SaveData   - A data buffer to save the controller state prior to PPR.

  @retval None
**/
void
MrcLpddr5PprControllerTeardown (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN PPR_MC_SETUP_SAVE    *SaveData
  )
{
  INT64  WckMode;
  INT64  SavetWckStop;

  WckMode = SaveData->SaveVal1;
  SavetWckStop = SaveData->SaveVal2;

  // Restoring page-table idle timer
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPageIdleTimerDisable, WriteCached, &SaveData->SaveVal0);

  if (WckMode == MrcLp5WckSafe) {
    // Start WCK on all Channels
    MrcIssueCas (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MrhCasWckFastSync, FALSE);

    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMcttWckStop, WriteCached, &SavetWckStop);
  }
}
