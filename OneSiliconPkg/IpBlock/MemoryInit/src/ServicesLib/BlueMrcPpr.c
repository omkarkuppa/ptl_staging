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
#include "MrcPprPrivate.h"
#include "MrcBluePprPrivate.h"
#include "MrcAmt.h"
#include "BlueMrcAmt.h"
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

#define FORCE_PPR_TEST_NUMBER (90)

// Verify at compile time that MAX_PATTERN_DEPTH is equal to CPGC_20_NUM_DPAT_EXTBUF
STATIC_ASSERT (MAX_PATTERN_DEPTH == CPGC_20_NUM_DPAT_EXTBUF, "Unexpected MAX_PATTERN_DEPTH value");

UINT64 AmtWcMats8Pattern[1] = {
  0x0101010101010101ULL,
};

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
  Configures the data pattern that will be written and read for the next memory test.

  @param[in] MrcData                    - Global MRC data structure
  @param[in] PprAmtData                 - PPR and AMT data structure
**/
VOID
AmtSetDataPattern (
  MrcParameters *const            MrcData,
  PPR_AMT_PARAMETER_DATA  *const  PprAmtData
  )
{
  const MRC_FUNCTION   *MrcCall;

  MrcCall         = MrcData->Inputs.Call.Func;

  switch (PprAmtData->TestType) {
    case PprTestTypeWcMats8:
      PprAmtData->DataPatternDepth = 1;
      PprAmtData->DataPattern[0] = MrcCall->MrcLeftShift64 (AmtWcMats8Pattern[0], PprAmtData->TestNumber);
      break;
    case PprTestTypeDataRetention:
      PprAmtData->DataPatternDepth = 8;
      switch (PprAmtData->TestNumber) {
        case 0:
          // Copy cacheline1 into Pattern
          MrcCall->MrcCopyMem((UINT8 *) PprAmtData->DataPattern, (UINT8 *) AmtCacheLine1, sizeof (AmtCacheLine1));
          break;
        case 1:
          // Copy cacheline2 into Pattern
          MrcCall->MrcCopyMem((UINT8 *) PprAmtData->DataPattern, (UINT8 *) AmtCacheLine2, sizeof (AmtCacheLine2));
          break;
      }
      break;
    case PprTestTypeXMarch:
#if AMT_USE_XMARCH_ALT == 1
      PprAmtData->DataPatternDepth = 1;
      MrcCall->MrcCopyMem ((UINT8*) PprAmtData->DataPattern, (UINT8*) &AmtMarchAltPattern[PprAmtData->TestNumber % 8], sizeof(PprAmtData->DataPattern[0]));
#else
      PprAmtData->DataPatternDepth = 16;
      MrcCall->MrcCopyMem ((UINT8*) PprAmtData->DataPattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));
#endif
      break;
    case PprTestTypeYMarchShort:
#if AMT_USE_YMARCH_SHORT_ALT == 1
      PprAmtData->DataPatternDepth = 1;
      MrcCall->MrcCopyMem ((UINT8*) PprAmtData->DataPattern, (UINT8*) &AmtMarchAltPattern[PprAmtData->TestNumber % 8], sizeof(PprAmtData->DataPattern[0]));
      break;
#endif
    case PprTestTypeYMarchLong:
      // YMarchShort will fall through to here if not using alt pattern
      PprAmtData->DataPatternDepth = 16;
      MrcCall->MrcCopyMem ((UINT8*) PprAmtData->DataPattern, (UINT8*) AmtXMarchPattern, sizeof(AmtXMarchPattern));
      break;
    case PprTestTypeXMarchG:
      PprAmtData->DataPatternDepth = 8;
      MrcCall->MrcCopyMem ((UINT8*) PprAmtData->DataPattern, (UINT8*) AmtXMarchGPattern, sizeof(AmtXMarchGPattern));
      break;
    case PprTestTypeMmrw:
      PprAmtData->DataPatternDepth = 1;
      switch (PprAmtData->TestNumber) {
        case 0:
          PprAmtData->DataPattern[0] = AmtMmrwTestPattern1;
          break;
        case 1:
          PprAmtData->DataPattern[0] = AmtMmrwTestPattern2;
          break;
        case 2:
          PprAmtData->DataPattern[0] = AmtMmrwTestPattern3;
          break;
      }
      break;
    default:
      break;
  }
}

/**
  Returns the number of low order bank group and bank address bits that are not included in the BG interleave

  @param[in] MrcData                - Pointer to MrcData

  @retval BaseBits                  - Number of bank bits for software bank loop
**/
UINT32
GetBaseBits (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput           *Outputs;
  UINT8               BaseBits = 0;

  Outputs = &MrcData->Outputs;

  if (Outputs->DdrType == MRC_DDR_TYPE_DDR5) { // DDR5 has 32 banks (8 BG, 4 banks per BG)
    // BG_Interleave 8
    BaseBits = MrcLog2 (MrcData, MAX_BANKS / MAX_DDR5_BANK_GROUPS) - 1; // MrcLog2 returns +1 so subtract 1 from the result
  } else { // LPDDR
    if (MrcGetBankBgOrg (MrcData, Outputs->Frequency) == MrcLp5BgMode) {
      // BG_Interleave 4
      BaseBits = MrcLog2 (MrcData, MAX_LPDDR5_BANKS / MAX_LPDDR5_BANK_GROUPS) - 1; // MrcLog2 returns +1 so subtract 1 from the result
    } else {
      BaseBits = MrcLog2 (MrcData, MAX_LPDDR5_BANKS) - 1; // MrcLog2 returns +1 so subtract 1 from the result
    }
  }

  return BaseBits;
} // GetBaseBits

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
  AmtSetBaseAddressControl (MrcData, PprTestTypeWcMats8); // Reset fields to default (0)

  Outputs->PprPatBufShift = 0;
}

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
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT32               TotalTestTimeMarch = 0;
  const CHAR8          *TestName;
  UINT32               Controller;
  UINT32               Channel;
  UINT8                PatBufShift;


  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  switch (PprAmtData->TestType) {
    case PprTestTypeXMarch:
      TestName = "MemTestXMarch";
      break;
    case PprTestTypeXMarchG:
      TestName = "MemTestXMarchG";
      break;
    case PprTestTypeYMarchShort:
      TestName = "MemTestYMarchShort";
      break;
    case PprTestTypeYMarchLong:
      TestName = "MemTestYMarchLong";
      break;
    default:
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MemTestMarch: Unsupported test type %d\n", PprAmtData->TestType);
      return mrcFail;
  }

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start %s\n", TestName);

  for (PatBufShift = 0; (PatBufShift < 4) && (PprAmtData->TestStatus == mrcSuccess); PatBufShift++) {
    PprAmtData->TestNumber = PatBufShift;
    MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PatBufShift=%d\n", PatBufShift);
    Outputs->PprPatBufShift = PatBufShift << 3; // Shift by 8 * PatBufShift
    RepairDone = 1;
    RetryCount = 0;

    do {
      AmtSetDataPattern (MrcData, PprAmtData);
      PprAmtData->UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0;
      PprAmtData->SubSeqDataInv[1] = 1;
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;

      //1. Execute write data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, FALSE, MT_ADDR_DIR_UP, 1);

      if (Inputs->PprErrorInjection != 0) {
        if (PprAmtData->TestNumber == 0 && RetryCount == 1) {
          PprAmtData->Rank = 0;
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 5, 1, 1, 0x0F);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 4, 4, 1, 0x0C);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 27, 0x7, 1, 0x01);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 22, 0x7, 1, 0x02);
        }
      }

      //2. Execute read data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, FALSE, MT_ADDR_DIR_UP, 2);

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
    } while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++));
    if (PprAmtData->TestType == PprTestTypeXMarchG) {
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
  MrcStatus           Status;
  UINT32              Controller;
  UINT32              Channel;
  UINT32              IpChannel;
  UINT32              BankIndex;
  INT64               GetSetDis;
  INT64               GetSetVal;
  INT64               EnableSrSave;
  UINT32              SavedCpgc20Credits[MAX_CONTROLLER];
  BOOLEAN             IsUlxUlt;
  BOOLEAN             SaveCpgcGlobalStart;
  MRC_MC_AD_SAVE      MadSavedValues;
  PPR_AMT_PARAMETER_DATA  PprAmtData;
  MRC_BG_BANK_PAIR    BankMappingSave[MAX_CONTROLLER][MAX_CHANNEL][MAX_BANKS];
  BOOLEAN             IsTargetedPprRequested;

  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  MrcCall         = Inputs->Call.Func;
  GetSetDis       = 0;
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
  PprAmtData.UseStopOnError = FALSE;

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
  Status = mrcSuccess;
  for (Controller = 0; Controller < MAX_CONTROLLER && Status == mrcSuccess; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels && Status == mrcSuccess; Channel++) {
      Status = Cpgc20GetSetBankSequence (MrcData, Controller, Channel, BankMappingSave[Controller][Channel], MAX_BANKS, MRC_GET);
    }
  }
  // If function call fails for some reason, set BankMapping to default values
  if (Status != mrcSuccess) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        for (BankIndex = 0; BankIndex < MAX_BANKS; BankIndex++) {
          MrcConvertLogicalBanktoBankBg(MrcData, &PprAmtData, BankIndex, &BankMappingSave[Controller][Channel][BankIndex].Bank, &BankMappingSave[Controller][Channel][BankIndex].BankGroup);
        }
      }
    }
  }

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PprTestType=0x%02x\n", (UINT32) Inputs->PprTestType.Value);
  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "PprRepairType=0x%02x\n", (UINT32) Inputs->PprRepairType);

  MrcPprSetRefresh (MrcData, TRUE);

  MrcGetSetMc (MrcData, Outputs->FirstPopController, GsmMccOppSrefEnable, ReadNoCache, &EnableSrSave);  // Save state of Self Refresh
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccOppSrefEnable, WriteNoCache, &GetSetDis); // Disable Self Refresh

  MrcModifyMcAddressDecoderValues (MrcData);

  MrcCall->MrcSetMemDword (SavedCpgc20Credits, ARRAY_COUNT (SavedCpgc20Credits), RDCPL_CREDITS_SCRUB);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if ((MrcControllerExist (MrcData, Controller)) && IsUlxUlt) {
       MrcCpgcModifyReadCredits (MrcData, Controller, TRUE, &SavedCpgc20Credits[Controller]);
    }
  }

  IsTargetedPprRequested = MrcIsTargetedPprRequested (MrcData);
  if (IsTargetedPprRequested) {
    MrcRunPprTargeted (MrcData);
  }

  if (Inputs->PprTestType.Value & MRC_PPR_ADV_ALGORITHM_TEST_MASK) {
    Status = MrcRunPprAdvancedMemoryTests (MrcData, &PprAmtData);
    if (Status == mrcSuccess) {
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "All Tests Completed! TotalTime Elapsed for all Tests: %dms\n", PprAmtData.TotalTestTime);
    }
  }

  if (Inputs->PprForceRepair) {
    MrcRunPprForceRepair (MrcData, &PprAmtData);
  }


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
  Run PPR Advanced Memory Tets.

  @param[in] MrcData pointer to global MRC data.
  @param[in] PprAmtData pointer to PPR and AMT data structure.

  @returns MrcStatus
**/
MrcStatus
MrcRunPprAdvancedMemoryTests (
  IN MrcParameters *const MrcData,
  PPR_AMT_PARAMETER_DATA  *const PprAmtData
  )
{
  MrcStatus Status = mrcSuccess;
  MrcInput *Inputs = &MrcData->Inputs;
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 PprTestType = (UINT32) Inputs->PprTestType.Value;

  BOOLEAN InvertedPassEn;
  UINT8 Test;
  // Run all memory tests
  for (Test = 0; Test <= PprTestTypeNumMemTests; Test++) {
#if POISON_ROW_FAIL_LIST
    PprAmtData.HasRowFailListBeenPoisonedOnThisTest = FALSE;
#endif
    if (PprTestType & (1 << Test)) {
      PprAmtData->TestType = (PprTestTypeOffset) Test;
      switch (Test) {
        case PprTestTypeWcMats8:
          InvertedPassEn = TRUE;
          Status = MemTestMATSN (MrcData, PprAmtData, InvertedPassEn);
          break;
        case PprTestTypeDataRetention:
          Status = MemTestDataRetention (MrcData, PprAmtData);
          break;
        case PprTestTypeXMarch:
#if AMT_USE_XMARCH_ALT == 1
          Status = MemTestMarchAlt (MrcData, PprAmtData);
          break;
#endif
        case PprTestTypeXMarchG:
        case PprTestTypeYMarchShort:
#if AMT_USE_YMARCH_SHORT_ALT == 1
          if (Test == PprTestTypeYMarchShort) {
            Status = MemTestMarchAlt (MrcData, PprAmtData);
            break;
          }
#endif
        case PprTestTypeYMarchLong:
          Status = MemTestMarch (MrcData, PprAmtData);
          break;
        case PprTestTypeMmrw:
          Status = MemTestMmrw(MrcData, PprAmtData);
          break;
      }
      PprCleanup (MrcData, PprAmtData, Outputs->McChBitMask);  // Clean up programming between different test setups
    }
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MrcPostPackageRepairEnable: Last memory test failed, skipping other tests\n");
      break;
    }
  }

  return Status;
}

/**
  Execute PPR force repair routine.

  @param[in] MrcData pointer to global MRC data.
  @param[in] PprAmtData pointer to PPR and AMT data structure.

  @returns MrcStatus
**/
MrcStatus
MrcRunPprForceRepair (
  IN MrcParameters *const MrcData,
  PPR_AMT_PARAMETER_DATA  *const PprAmtData
  )
{
  MrcOutput *const Outputs = &MrcData->Outputs;

  MrcStatus Status = mrcSuccess;

  MrcChannelOut *ChannelOut;
  UINT8 FirstChannel;
  UINT8 Rank;
  UINT8 Test;
  UINT8 BG;

  for (Test = 0; Test < FORCE_PPR_TEST_NUMBER; Test++) {
    PprCleanup (MrcData, PprAmtData, Outputs->McChBitMask);
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
    Status |= MrcPostPackageRepair(MrcData, Outputs->FirstPopController, FirstChannel, Rank, BG, 2, 0xAB, 0xF);
    MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "Ppr Status = %d\n", Status);
  }
  Outputs->PprForceRepairStatus = Status;

  return Status;
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

/**
  Initial function to execute MBIST/mPPR

  @param[in]  MrcData           - Global MRC data structure

  @retval MrcStatus - mrcSuccess/mrcFail

**/
MrcStatus
RunMbistMppr (
  IN  MrcParameters             *const MrcData
  )
{
  return mrcSuccess;
}

/**
  Enable or disable refreshes in the test engine

  @param[in]  MrcData           - Pointer to global MRC data.
  @param[in]  RefreshEnabled    - Whether refreshes should be enabled.
**/
VOID
MrcPprSetRefresh (
  IN  MrcParameters *const MrcData,
  IN  BOOLEAN              RefreshEnabled
)
{
  INT64 GetSetVal;

  if (RefreshEnabled) {
    GetSetVal = 1;
    MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetVal);
  } else {
    GetSetVal = 0;
    MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, WriteNoCache, &GetSetVal);
  }
}
