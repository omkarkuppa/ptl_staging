/** @file
  This file contains functions to detect and record DRAM Failing addresses via
  Advanced Memory Test.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include "MrcAmt.h"
#include "BlueMrcAmt.h"
#include "MrcPpr.h"
#include "MrcCommon.h"
#include "MrcCpgcApi.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcSpdProcessingDefs.h"
#include "BlueMrcIoTest.h"
#include "MrcMemoryScrub.h"
#include "BlueTestDataEngine.h"


/**
  This function will convert the CPGC Bank address into the DRAM Bank Address and Bank Group bits based on DDR technology

  @param[in]  MrcData     - Global MRC data structure
  @param[in]  PprAmtData  - PPR and AMT data structure
  @param[in]  LogicalBank - CPGC Bank Number
  @param[out] BankAddress - Pointer to BankAddress value
  @param[out] BankGroup   - Pointer to BankGroup value
**/
VOID
MrcConvertLogicalBanktoBankBg (
  IN  MrcParameters *const          MrcData,
  IN  PPR_AMT_PARAMETER_DATA *const PprAmtData,
  IN  UINT32                        LogicalBank,
  OUT UINT8                         *BankAddress,
  OUT UINT8                         *BankGroup
  )
{
  MrcOutput         *Outputs;
  UINT8             DdrType;

  Outputs = &MrcData->Outputs;
  DdrType = Outputs->DdrType;

  switch (DdrType) {
    case MRC_DDR_TYPE_DDR5:
      if ((BankAddress != NULL) && (BankGroup != NULL)) {
        *BankGroup = (LogicalBank >> CPGC20_BANK_GROUP_FIELD_OFFSET_DDR5) & 0x7;
        *BankAddress = LogicalBank & 0x3;
      }
      break;
    case MRC_DDR_TYPE_LPDDR5:
      if (MrcGetBankBgOrg(MrcData, Outputs->Frequency) == MrcLp5BgMode) {
        if ((BankAddress != NULL) && (BankGroup != NULL)) {
          *BankGroup = (LogicalBank >> CPGC20_BANK_GROUP_FIELD_OFFSET_LPDDR5) & 0x3;  // 4 banks, 4 bank groups
          *BankAddress = LogicalBank & 0x3;
        }
        break;
      } // LP5 16-bank mode should fall through to the default case
    default:
      if (BankAddress != NULL) {
        *BankAddress = LogicalBank & MRC_UINT8_MAX;
      }
      break;
  }
}

/**
  This function will convert the DRAM Bank Address and Bank Group bits into the CPGC Bank address based on DDR technology

  @param[in]  MrcData     - Global MRC data structure
  @param[in]  PprAmtData  - PPR and AMT data structure
  @param[in]  BankAddress - Pointer to BankAddress value
  @param[in]  BankGroup   - Pointer to BankGroup value
  @param[out] LogicalBank - CPGC Bank Number
**/
VOID
MrcConvertBankBgtoLogicalBank (
  IN  MrcParameters *const          MrcData,
  IN  PPR_AMT_PARAMETER_DATA *const PprAmtData,
  IN  UINT32                        BankAddress,
  IN  UINT32                        BankGroup,
  OUT UINT32                        *LogicalBank
  )
{
  MrcOutput         *Outputs;
  UINT32            FirstController;
  UINT32            FirstChannel;
  UINT8             Rank;
  UINT8             FirstDimm;

  Outputs         = &MrcData->Outputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel    = Outputs->Controller[FirstController].FirstPopCh;
  FirstDimm       = 0;

  switch (Outputs->DdrType) {
    default:
      return;
    case MRC_DDR_TYPE_DDR5:
      if (LogicalBank != NULL) {
        *LogicalBank = ((BankGroup & 0x7) << CPGC20_BANK_GROUP_FIELD_OFFSET_DDR5) | (BankAddress & 0x3);
      }
      break;
    case MRC_DDR_TYPE_LPDDR5:
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (MrcRankExist (MrcData, FirstController, FirstChannel, Rank)) {
          FirstDimm = RANK_TO_DIMM_NUMBER (Rank);
          break;
        }
      }
      if (Outputs->Controller[FirstController].Channel[FirstChannel].Dimm[FirstDimm].BankGroups > 1) { // 4 Banks/4 Bank Groups Configuration
        if (LogicalBank != NULL) {
          *LogicalBank = ((BankGroup & 0x3) << CPGC20_BANK_GROUP_FIELD_OFFSET_LPDDR5) | (BankAddress & 0x3);
        }
      } else {
        switch (Outputs->Controller[FirstController].Channel[FirstChannel].Dimm[FirstDimm].Banks) {
          case 8:
            *LogicalBank = (BankAddress & 0x7);
            break;
          case 16:
            *LogicalBank = (BankAddress & 0xF);
            break;
        } // Switch (Banks)
      }
      break;
  } // Switch (DdrType)
}

/**
  This function programs the specified 64-bit Pattern provided, into CPGC ExtBuf registers.
  Data Pattern is shifted across each UI, if non-zero

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  PprAmtData              - PPR and AMT data structure
  @param[in]  McChBitMask             - Memory Controller Channel Bit mask to read status for.
  @param[in]  ErrInjMask16            - Bitmask of DQ lanes to inject error
**/
VOID
MrcProgramMATSPattern (
  IN  MrcParameters *const          MrcData,
  IN  PPR_AMT_PARAMETER_DATA *const PprAmtData,
  IN  UINT32                        McChBitMask,
  IN  UINT16                        ErrInjMask16
  )
{
  MRC_FUNCTION    *MrcCall;
  UINT32          Index;
  UINT32          CpgcExtBuf[CPGC_20_NUM_DPAT_EXTBUF];
  UINT64_STRUCT   PatternTemp;

  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcSetMem ((UINT8 *) CpgcExtBuf, sizeof (CpgcExtBuf), 0);

  if (PprAmtData->DataPatternDepth == 0) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR,
      "WARNING: MrcProgramMATSPattern PatternDepth set to 0. Using PatternDepth = 1 instead\n");
    PprAmtData->DataPatternDepth = 1;
  } else if (PprAmtData->DataPatternDepth > CPGC_20_NUM_DPAT_EXTBUF) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE,
      "Note: MrcProgramMATSPattern PatternDepth greater than %d, using PatternDepth = %d instead\n",
      CPGC_20_NUM_DPAT_EXTBUF, CPGC_20_NUM_DPAT_EXTBUF);
    PprAmtData->DataPatternDepth = CPGC_20_NUM_DPAT_EXTBUF;
  }

  for (Index = 0; Index < ARRAY_COUNT(CpgcExtBuf); Index++) {
    PatternTemp.Data = PprAmtData->DataPattern[Index % PprAmtData->DataPatternDepth];
    // Rotate pattern left for each UI
    PatternTemp.Data = MrcCall->MrcLeftShift64 (PatternTemp.Data, Index * PprAmtData->UiShl);
    CpgcExtBuf[Index] = ((PatternTemp.Data32.High & ~ErrInjMask16) | (~PatternTemp.Data32.High & ErrInjMask16));
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "ExtBuf[%d]: 0x%08x\n", Index,
      (PprAmtData->IsUseInvtPat) ? ~CpgcExtBuf[Index] : CpgcExtBuf[Index]);
  } // i loop
  Cpgc20WriteMATSFixedPattern (MrcData, CpgcExtBuf);
}

/**
This function programs a specific pattern for the MMRW test into CPGC ExtBuf registers.

@param[in]  MrcData                 - Global MRC data structure
@param[in]  PprAmtData              - PPR and AMT data structure
@param[in]  McChBitMask             - Memory Controller Channel Bit mask to read status for.
@param[in]  ErrInjMask16            - Bitmask of DQ lanes to inject error
**/
VOID
MrcProgramDSPattern (
  IN  MrcParameters *const          MrcData,
  IN  PPR_AMT_PARAMETER_DATA *const PprAmtData,
  IN  UINT32                        McChBitMask,
  IN  UINT16                        ErrInjMask16
)
{
  MRC_FUNCTION    *MrcCall;
  UINT32          Index;
  UINT32          CpgcExtBuf[CPGC_20_NUM_DPAT_EXTBUF];
  UINT32          BasePattern[2];
  UINT32          PatternUI;
  UINT64_STRUCT   InputPattern;

  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcSetMem ((UINT8 *) CpgcExtBuf, sizeof (CpgcExtBuf), 0);

  switch (PprAmtData->DataPattern[0])
  {
  case AmtMmrwTestPattern1:
  case AmtMmrwTestPattern3:
    BasePattern[0] = 0xffffffff;
    BasePattern[1] = 0x00000000;
    break;
  case AmtMmrwTestPattern2:
    BasePattern[0] = 0xf0f0f0f0;
    BasePattern[1] = 0x0f0f0f0f;
    break;
  default:
    InputPattern.Data = PprAmtData->DataPattern[0];
    BasePattern[0] = InputPattern.Data32.High;
    break;
  }

  for (Index = 0; Index < ARRAY_COUNT(CpgcExtBuf); Index++) {
    // Select the current UI based on pattern type
    switch (PprAmtData->DataPattern[0]){
      case AmtMmrwTestPattern1:
        PatternUI = ((Index & 0x7) < 4) ? BasePattern[0] : BasePattern[1];
        break;
      case AmtMmrwTestPattern2:
      case AmtMmrwTestPattern3:
        PatternUI = BasePattern[Index % 2];
        break;
      default:
        PatternUI = BasePattern[0];
        break;
    }
    CpgcExtBuf[Index] = (PatternUI & ~ErrInjMask16) | (~PatternUI & ErrInjMask16);
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "ExtBuf[%d]: 0x%08x\n", Index,
      (PprAmtData->IsUseInvtPat) ? ~CpgcExtBuf[Index] : CpgcExtBuf[Index]);
  } // i loop
  Cpgc20WriteMATSFixedPattern (MrcData, CpgcExtBuf);
}

/**
  Selects channels to enable for the next memory test

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  ChanEnMask              - Bit mask of channels to be enabled

  @retval BitMask that was previously programmed (for save/restore purposes)
  **/
UINT32
AmtSelectChannel (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                ChanEnMask
  )
{
  // Not needed in CPGC
  return MRC_UINT32_MAX;
}

/**
  Cpgc command pattern setup for memory test

  @param[in] MrcData        - Pointer to MRC global data.
  @param[in] RwMode         - Cpgc read and write mode
  @param[in] UseSubSeq1     - Cpgc subseq1 usage
  @param[in] Direction      - Address direction during memory test
  @param[in] SeqDataInv     - Enables pattern inversion per subsequence
  @param[in] IsUseInvtPat   - Info to indicate whether or not patternQW is inverted by comparing original pattern

  @retval n/a
**/
VOID
MrcConfigurePprCmdPat (
  IN MrcParameters  *const MrcData,
  IN UINT8                 RwMode,
  IN BOOLEAN               UseSubSeq1,
  IN UINT8                 Direction,
  IN UINT8                 SeqDataInv[2],
  IN BOOLEAN               IsUseInvtPat
  )
{
  UINT8          CmdPat;
  UINT16         BurstWait;

  BurstWait = 0;

  if (UseSubSeq1 == CPGC_NO_SUBSEQ1) { // Only read or write
    if (RwMode == CPGC_BASE_READ_SUBSEQ) { // Read only
      if (IsUseInvtPat) {
        if (Direction == MT_ADDR_DIR_UP) { // ^(rI)
          CmdPat = PatRdUpInvt;
        } else { // v(rI)
          CmdPat = PatRdDownInvt;
        }
      } else {
        if (Direction == MT_ADDR_DIR_UP) { // ^(rD)
          CmdPat = PatRdUp;
        } else { // v(rD)
          CmdPat = PatRdDown;
        }
      }
    } else { // Write only
      if (IsUseInvtPat) {
        if (Direction == MT_ADDR_DIR_UP) { // ^(wI)
          CmdPat = PatWrUpInvt;
        } else { // v(wI)
          CmdPat = PatWrDownInvt;
        }
      } else {
        if (Direction == MT_ADDR_DIR_UP) { // ^(wD)
          CmdPat = PatWrUp;
        } else { // v(wD)
          CmdPat = PatWrDown;
        }
      }
    }
  } else { // Read and then Write
    if (IsUseInvtPat) {
      if (Direction == MT_ADDR_DIR_UP) {
        if (SeqDataInv[1] == 1) { // ^(rI, wD)
          CmdPat = PatRdWrUpInvtAlt;
        } else { // ^(rI, wI)
          CmdPat = PatRdWrUpInvt;
        }
      } else {
        if (SeqDataInv[1] == 1) { // v(rI, wD)
          CmdPat = PatRdWrDownInvtAlt;
        } else { // v(rI, wI)
          CmdPat = PatRdWrDownInvt;
        }
      }
    } else {
      if (Direction == MT_ADDR_DIR_UP) {
        if (SeqDataInv[1] == 1) { // ^(rD, wI)
          CmdPat = PatRdWrUpAlt;
        } else { // ^(rD, wD)
          CmdPat = PatRdWrUp;
        }
      } else {
        if (SeqDataInv[1] == 1) { // v(rD, wI)
          CmdPat = PatRdWrDownAlt;
        } else { // v(rD, wD)
          CmdPat = PatRdWrDown;
        }
      }
    }
  }
  Cpgc20SetCommandSequence(MrcData, CmdPat, FALSE, BurstWait, FALSE);
}

/**
  This function programs the Raster Repo Configuration register based on input values.

  @param[in]  MrcData         - Pointer to MRC global data.
  @param[in]  McChBitMask     - Bitmask of participating controllers / channels
  @param[in]  Mode3Banks      - Specifies which Bank mode to use in Raster Repo Mode 3 usage.
  @param[in]  UpperBanks      - Indicates the base Bank to store when using Raster Repo Mode 3
  @param[in]  StopOnRaster    - If TRUE, the test will stop after status condition is met in Raster Repo
  @param[in]  RasterRepoClear - If set, Raster Repo entries will be Reset (this bit auto-clears)
  @param[in]  RasterRepoMode  - Indicates the mode of operation for Raster Repo
  @param[in]  Mode3Max        - In Raster Repo Mode 3, sets maximum number of failing address before an additional failing address will set fail_excess for the respective Bank.
  **/
void
Cpgc20RasterRepoConfig (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT8         *const  Mode3Banks,
  IN  UINT8         *const  Mode3UpperBanks,
  IN  BOOLEAN       *const  StopOnRaster,
  IN  BOOLEAN       *const  RasterRepoClear,
  IN  UINT8         *const  RasterRepoMode,
  IN  UINT8         *const  Mode3Max
  )
{
  MrcOutput                                   *Outputs;
  UINT8                                       Controller;
  UINT8                                       Channel;
  UINT8                                       MaxChannels;
  UINT32                                      IpChannel;
  UINT32                                      Offset;
  MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_STRUCT  RasterRepoConfig;
  MC0_CH0_CR_CPGC2_RASTER_MODE3_MAX_STRUCT    Mode3MaxStruct;

  Outputs     = &MrcData->Outputs;
  MaxChannels = Outputs->MaxChannels;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannels) == 0) {
        continue;
      }
      IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);

      Offset = OFFSET_CALC_MC_CH (
        MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_REG,
        MC1_CH0_CR_CPGC2_RASTER_REPO_CONFIG_REG, Controller,
        MC0_CH1_CR_CPGC2_RASTER_REPO_CONFIG_REG, IpChannel);

      RasterRepoConfig.Data = MrcReadCR (MrcData, Offset);

      if (Mode3Banks != NULL) {
        RasterRepoConfig.Bits.MODE3_BANKS = *Mode3Banks;
      }
      if (Mode3UpperBanks != NULL) {
        RasterRepoConfig.Bits.UPPER_BANKS = *Mode3UpperBanks;
      }
      if (StopOnRaster != NULL) {
        RasterRepoConfig.Bits.StopOnRaster = *StopOnRaster;
      }
      if (RasterRepoClear != NULL) {
        RasterRepoConfig.Bits.RasterRepoClear = *RasterRepoClear;
      }
      if (RasterRepoMode != NULL) {
        RasterRepoConfig.Bits.RasterRepoMode = *RasterRepoMode;
      }

      MrcWriteCR (MrcData, Offset, RasterRepoConfig.Data);

      if (Mode3Max != NULL) {
        Offset = OFFSET_CALC_MC_CH(
          MC0_CH0_CR_CPGC2_RASTER_MODE3_MAX_REG,
          MC1_CH0_CR_CPGC2_RASTER_MODE3_MAX_REG, Controller,
          MC0_CH1_CR_CPGC2_RASTER_MODE3_MAX_REG, IpChannel);

        Mode3MaxStruct.Data = 0;
        Mode3MaxStruct.Bits.FAIL_MAX = *Mode3Max;

        MrcWriteCR (MrcData, Offset, Mode3MaxStruct.Data);
      }
    }
  }
}

/**
  This function Reads the Repo Content registers, and returns those values in an input array.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  Controller   - Controller to work on
  @param[in]  Channel      - Channel to work on
  @param[out] RepoStatus   - Contains the data from Repo Content registers, for 8 normal entries and 2 ECC entries

  @retval nothing
**/
void
Cpgc20ReadRasterRepoContent (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  OUT UINT64_STRUCT         RepoStatus[CPGC20_MAX_RASTER_REPO_CONTENT]
  )
{
  MrcOutput                                       *Outputs;
  UINT8                                           McChMask;
  UINT8                                           MaxChannel;
  UINT32                                          IpChannel;
  UINT32                                          Offset;
  UINT8                                           RepoIndex;
  MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_0_STRUCT   RasterRepoContent;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;

  if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
    return;
  }
  IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);

  Offset = OFFSET_CALC_MC_CH (
    MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_0_REG,
    MC1_CH0_CR_CPGC2_RASTER_REPO_CONTENT_0_REG, Controller,
    MC0_CH1_CR_CPGC2_RASTER_REPO_CONTENT_0_REG, IpChannel);
  for (RepoIndex = 0; RepoIndex < CPGC20_NUM_RASTER_REPO_CONTENT_REG; RepoIndex++) {
    RasterRepoContent.Data = MrcReadCR64 (MrcData, Offset);
    Offset += (MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_1_REG - MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_0_REG);
    if (RepoStatus != NULL) {
      RepoStatus[RepoIndex].Data = RasterRepoContent.Data;
    }
  }

  Offset = OFFSET_CALC_MC_CH (
    MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_REG,
    MC1_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_REG, Controller,
    MC0_CH1_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_REG, IpChannel);
  for (RepoIndex = 0; RepoIndex < CPGC20_NUM_RASTER_REPO_CONTENT_ECC; RepoIndex++) {
    RasterRepoContent.Data = MrcReadCR64 (MrcData, Offset);
    Offset += (MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_1_REG - MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_REG);
    if (RepoStatus != NULL) {
      RepoStatus[CPGC20_NUM_RASTER_REPO_CONTENT_REG + RepoIndex].Data = RasterRepoContent.Data;
    }
  }
}

/**
  This function reads the Raster Repo Status register and parses the individual fields into the input pointers.

  @param[in]   MrcData        - Pointer to MRC global data.
  @param[in]   Controller     - Controller to work on
  @param[in]   Channel        - Channel to work on
  @param[out]  BitmapValid    - Optional pointer, if provided it will be updated with indication of valid bits in Repo Status
  @param[out]  SummaryValid   - Optional pointer, if provided it will be updated with indication that test summary in Raster Repo is valid
  @param[out]  FailExcessAll  - Optional pointer, if provided it will be updated with indication of any bank that observed a number of failures that exceeded MAX_BANKS setting
  @param[out]  FailAnyAll     - Optional pointer, if provided it will be updated with indication if any bank failed
  @param[out]  NumErrLogged   - Optional pointer, if provided it will be updated with number of errors logged
  @param[out]  RasterRepoFull - Optional pointer, if provided it will be updated with indication of RasterRepo being full

  @retval nothing
**/
void
Cpgc20ReadRasterRepoStatus (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Controller,
  IN  UINT8                 Channel,
  OUT UINT8         *const  BitmapValid,
  OUT UINT8         *const  SummaryValid,
  OUT UINT8         *const  FailExcessAll,
  OUT UINT8         *const  FailAnyAll,
  OUT UINT8         *const  NumErrLogged,
  OUT UINT8         *const  RasterRepoFull
  )
{
  MrcOutput                                   *Outputs;
  UINT8                                       McChMask;
  UINT8                                       MaxChannel;
  UINT32                                      IpChannel;
  UINT32                                      Offset;
  MC0_CH0_CR_CPGC2_RASTER_REPO_STATUS_STRUCT  RasterRepoStatus;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;

  if (MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) {
    return;
  }
  IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);

  Offset = OFFSET_CALC_MC_CH (
    MC0_CH0_CR_CPGC2_RASTER_REPO_STATUS_REG,
    MC1_CH0_CR_CPGC2_RASTER_REPO_STATUS_REG, Controller,
    MC0_CH1_CR_CPGC2_RASTER_REPO_STATUS_REG, IpChannel);
  RasterRepoStatus.Data = (UINT8) MrcReadCR (MrcData, Offset);

  if (BitmapValid != NULL) {
    *BitmapValid = RasterRepoStatus.Bits.BitmapValid;
  }
  if (SummaryValid != NULL) {
    *SummaryValid = RasterRepoStatus.Bits.SummaryValid;
  }
  if (FailExcessAll != NULL) {
    *FailExcessAll = RasterRepoStatus.Bits.FailExcessAll;
  }
  if (FailAnyAll != NULL) {
    *FailAnyAll = RasterRepoStatus.Bits.FailAnyAll;
  }
  if (NumErrLogged != NULL) {
    *NumErrLogged = RasterRepoStatus.Bits.NumErrLogged;
  }
  if (RasterRepoFull != NULL) {
    *RasterRepoFull = RasterRepoStatus.Bits.RasterRepoFull;
  }
}

/**
  This function returns the device that failed in the last memory test based on DQ lane errors.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Desired Memory Controller.
  @param[in]  Channel     - Desired Channel.

  @retval The device that failed.
**/
UINT8
AmtGetFailingDevice (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel
)
{
  MrcOutput     *Outputs;
  UINT8         Byte;
  UINT8         BitGroupErr[MRC_MRR_ARRAY_SIZE];  // Variable size needs to support ECC

  Outputs    = &MrcData->Outputs;

  Cpgc20GetBitGroupErrStatus (MrcData, Controller, Channel, BitGroupErr);

  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    if (BitGroupErr[Byte] != 0) {
      return Byte;
    }
  }

  return 0xFF;
}

/**
  This function configures the CPGC Data Control register.

  @param[in]  MrcData                   - Pointer to MRC global data.
  @param[in]  DataSelectRotationRepeats - Number of times to repeat a Data_Instruction with Data_Select_Rotation_Enable
  @param[in]  SplitBackground           - Background mode.

  @retval Nothing.
**/
void
Cpgc20SetDataControl (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                DataSelectRotationRepeats,
  IN  UINT32                SplitBackground
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     McChMask;
  UINT8     MaxChannel;
  BOOLEAN   IsLpddr;
  MC0_REQ0_CR_CPGC2_DATA_CONTROL_STRUCT DataControl;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  IsLpddr    = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      DataControl.Data = 0;
      DataControl.Bits.Data_Select_Rotation_Repeats = DataSelectRotationRepeats;
      DataControl.Bits.SPLIT_BACKGROUND = SplitBackground;
      Offset = OFFSET_CALC_MC_CH (
        MC0_REQ0_CR_CPGC2_DATA_CONTROL_REG,
        MC1_REQ0_CR_CPGC2_DATA_CONTROL_REG, Controller,
        MC0_REQ1_CR_CPGC2_DATA_CONTROL_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, DataControl.Data);
    }
  }
}

/**
  This function configures the CPGC Base Address Control register based on TestType.

  @param[in] MrcData        - Pointer to MRC global data.
  @param[in] TestType       - Memory test type that is currently being run

  @retval Nothing.
**/
void
AmtSetBaseAddressControl (
  IN  MrcParameters *const MrcData,
  IN PprTestTypeOffset     TestType
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpChannel;
  UINT32    Offset;
  UINT8     McChMask;
  UINT8     MaxChannel;
  BOOLEAN   IsLpddr;
  MC0_REQ0_CR_CPGC2_BASE_ADDRESS_CONTROL_STRUCT BaseAddressControl;

  Outputs    = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  McChMask   = Outputs->McChBitMask;
  IsLpddr    = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      BaseAddressControl.Data = 0;
      if (TestType == PprTestTypeYMarchLong) {
        BaseAddressControl.Bits.Block_Row_Move_One_Row = 1;
        BaseAddressControl.Bits.Row_Inc = 1;
      }
      Offset = OFFSET_CALC_MC_CH (
        MC0_REQ0_CR_CPGC2_BASE_ADDRESS_CONTROL_REG,
        MC1_REQ0_CR_CPGC2_BASE_ADDRESS_CONTROL_REG, Controller,
        MC0_REQ1_CR_CPGC2_BASE_ADDRESS_CONTROL_REG, IpChannel);
      MrcWriteCR (MrcData, Offset, BaseAddressControl.Data);
    }
  }
}

/**
  This function sets the Pattern Generators' Alternate Dpat Buffer Control for use with AMT tests.

  @param[in]  MrcData             - Pointer to global MRC data.
  @param[in]  Controller          - Desired Memory Controller.
  @param[in]  Channel             - Desired Channel.
  @param[in]  Start               - Lane to start from
  @param[in]  Stop                - Lane to wrap ("stop") on
**/
VOID
AmtSetDpatAltBufCtl (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Controller,
  IN  UINT8                 Channel,
  IN  UINT8                 Start,
  IN  UINT8                 Stop
  )
{
  MrcOutput *Outputs;
  UINT32    IpChannel;
  UINT32    Offset;
  MC0_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_STRUCT CpgcDpatAltBufCfg;

  Outputs     = &MrcData->Outputs;
  CpgcDpatAltBufCfg.Data = 0;
  CpgcDpatAltBufCfg.Bits.ZERO_DATA_EN       = 0xFF;
  CpgcDpatAltBufCfg.Bits.BUF_PNTR_INC_MODE  = 2;
  CpgcDpatAltBufCfg.Bits.BUF_STRT_PNTR      = Start;
  CpgcDpatAltBufCfg.Bits.BUF_END_PNTR       = Stop;

  IpChannel = DDR5_IP_CH (Outputs->IsDdr5, Channel);
  Offset = OFFSET_CALC_MC_CH (
    MC0_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_REG,
    MC1_CH0_CR_CPGC_DPAT_ALT_BUF_CFG_REG, Controller,
    MC0_CH1_CR_CPGC_DPAT_ALT_BUF_CFG_REG, IpChannel);
  MrcWriteCR (MrcData, Offset, CpgcDpatAltBufCfg.Data);
}

/**
  This function programs Bank Logical to Physical mapping for single-bank tests and for LP5 16-bank mode.
  The register will be updated on all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Rank              - Rank to work on
  @param[in]  UseSingleBank     - Whether to program a single bank; if false, will program all banks in order without BG calculation
  @param[in]  CpgcBank          - Cpgc address of bank to work on, only valid if UseSingleBank = TRUE
**/
VOID
AmtUpdateL2PBankMappingWithoutBG (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Rank,
  IN  BOOLEAN               UseSingleBank,
  IN  UINT32                CpgcBank
)
{
  MRC_FUNCTION      *MrcCall;
  MrcOutput         *Outputs;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             MaxChannel;
  MrcDimmOut        *DimmOut;
  MRC_BG_BANK_PAIR  BankMapping[CPGC20_MAX_BANKS_PER_CHANNEL];
  UINT8             Index;
  UINT8             BankMapLength;
  UINT8             BgOffset;
  UINT8             BankMask;

  MrcCall = MrcData->Inputs.Call.Func;
  Outputs = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;

  MrcCall->MrcSetMem ((UINT8*) BankMapping, sizeof (BankMapping), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (Outputs->McChBitMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (Outputs->IsLpddr, Channel))) {
        continue;
      }
      if (UseSingleBank) {
        if (Outputs->IsDdr5 || (Outputs->IsLpddr && (MrcGetBankBgOrg(MrcData, Outputs->Frequency) == MrcLp5BgMode))) {
          BgOffset = (Outputs->IsDdr5) ? CPGC20_BANK_GROUP_FIELD_OFFSET_DDR5 : CPGC20_BANK_GROUP_FIELD_OFFSET_LPDDR5;
          BankMask = (1 << BgOffset) - 1;
          BankMapping[0].BankGroup = (UINT8) (CpgcBank >> BgOffset);
          BankMapping[0].Bank = (UINT8) (CpgcBank & (BankMask));
        } else {
          BankMapping[0].Bank = (UINT8) CpgcBank; // BG is 0 by default
        }
        BankMapLength = 1;
      } else {
        DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER (Rank)];
        // Program Bank Logical to Physical mapping for LP5 16 bank mode, to cover all banks in order
        for (Index = 0; Index < DimmOut->Banks; Index++) {
          BankMapping[Index].Bank = (UINT8) Index;
          BankMapping[Index].BankGroup = 0;
        }
        BankMapLength = DimmOut->Banks;
      }
      Cpgc20GetSetBankSequence (MrcData, Controller, Channel, BankMapping, BankMapLength, MRC_SET);
    }
  }
}

/**

  CPGC setup per rank for Advanced Memory test

  @param[in] MrcData                    - Global MRC data structure
  @param[in] PprAmtData                 - PPR and AMT data structure
  @param[in] McChBitMask                - Memory Controller Channel Bit mask to read results for.

  @retval Bit mask of channels enabled if rank exists

**/
UINT8
AdvancedMemTestRankSetupMATSRowRange (
  IN OUT MrcParameters  *const      MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          McChBitMask
  )
{
  MRC_FUNCTION          *MrcCall;
  MrcOutput             *Outputs;
  UINT8                 CpgcColAddressBits;
  UINT8                 CpgcTotalBanks;
  UINT32                RowStart;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 LocalMcChBitMask;
  UINT32                IpChannel;
  MRC_ADDRESS           CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8                 McChBitMaskEnabled;
  UINT8                 DpatAltStart;
  UINT8                 DpatAltStop;
  BOOLEAN               IsMarchAlgorithm;
  UINT32                BaseRepeats;
  BOOLEAN               IsMarchAlgoStartStopPerCh;

  MrcCall = MrcData->Inputs.Call.Func;
  Outputs = &MrcData->Outputs;
  McChBitMaskEnabled = 0;

  MrcCall->MrcSetMem ((UINT8 *) CPGCAddressArray, sizeof (CPGCAddressArray), 0);

  switch (PprAmtData->TestType) {
    case PprTestTypeXMarch:
    case PprTestTypeXMarchG:
    case PprTestTypeYMarchShort:
    case PprTestTypeYMarchLong:
      IsMarchAlgorithm = TRUE;
      break;
    default:
      IsMarchAlgorithm = FALSE;
      break;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) ||
          (PprAmtData->TestRowSize[Controller][IpChannel] == 0)) {
        continue;
      }
      IsMarchAlgoStartStopPerCh = IsMarchAlgorithm;
#if AMT_USE_XMARCH_ALT == 1
      // Use the same DpatAltStart / Stop for all channels when the test is XMarch and AMT_USE_XMARCH_ALT is enabled
      IsMarchAlgoStartStopPerCh = IsMarchAlgoStartStopPerCh && (PprAmtData->TestType != PprTestTypeXMarch);
#endif // AMT_USE_XMARCH_ALT
#if AMT_USE_YMARCH_SHORT_ALT == 1
      // Use the same DpatAltStart / Stop for all channels when the test is YMarchShort and AMT_USE_YMARCH_SHORT_ALT is enabled
      IsMarchAlgoStartStopPerCh = IsMarchAlgoStartStopPerCh && (PprAmtData->TestType != PprTestTypeYMarchShort);
#endif // AMT_USE_YMARCH_SHORT_ALT
      if (IsMarchAlgoStartStopPerCh) {
        switch (Channel) {
          case 0:
            DpatAltStart = 0;
            DpatAltStop = 0x7;
            break;
          case 1:
            DpatAltStart = 0x1C;
            DpatAltStop = 0x3;
            break;
          case 2:
            DpatAltStart = 0x18;
            DpatAltStop = 0x1F;
            break;
          case 3:
            DpatAltStart = 0x14;
            DpatAltStop = 0x1B;
            break;
          default:
            DpatAltStart = 0;
            DpatAltStop = 0;
            break;
        }
        DpatAltStart = (DpatAltStart + Outputs->PprPatBufShift) & 0x1F;
        DpatAltStop  = (DpatAltStop + Outputs->PprPatBufShift) & 0x1F;
      } else {
        // WCMATS8 and Data Retention
        // XMarchAlt and YMarchShortAlt
        DpatAltStart = 0;
        DpatAltStop = 0x7;
      }
      AmtSetDpatAltBufCtl (MrcData, Controller, Channel, DpatAltStart, DpatAltStop);

      // Programs CPGC to run on the selected physical ranks
      // If FromRowTestPpr, test must run on only one rank
      McChBitMaskEnabled |= SelectReutRanks (MrcData, Controller, Channel, 1 << PprAmtData->Rank, FALSE);
      EnableRefresh (MrcData, Controller, Channel, 1 << PprAmtData->Rank); // Enable refresh on all Ranks

      // Global bind disabled for PPR
      Cpgc20SetGlobalBind (MrcData, Controller, IpChannel, FALSE);
    }
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (MC_CH_MASK_CHECK (McChBitMaskEnabled, Controller, Channel, Outputs->MaxChannels) == 0) {
        continue;
      }
      LocalMcChBitMask = (UINT8) (MRC_BIT0 << ((Controller * Outputs->MaxChannels) + Channel));
      IpChannel = LP_IP_CH(Outputs->IsLpddr, Channel);

      if (PprAmtData->Direction == MT_ADDR_DIR_UP) {
        RowStart = PprAmtData->TestRowAddr[Controller][IpChannel];
      } else {
        // RowAddr contains the ending row for downward direction
        // For display purposes, print the top row (starting row) instead, but still program RowAddr to CPGC
        RowStart = PprAmtData->TestRowAddr[Controller][IpChannel] + PprAmtData->TestRowSize[Controller][IpChannel] - 1;
      }

      // Clear raster repo
      MrcCpgcClearRasterRepo (MrcData, Controller, Channel);

      // DDR5 Burst Length is 16
      if (Outputs->IsLpddr) {
        CpgcColAddressBits = PprAmtData->ColumnBits[Controller][Channel] - (MrcLog2 (MrcData, 32) - 1); // LPDDR BL=32 The log function return +1 so we subtract 1
      } else {
        CpgcColAddressBits = PprAmtData->ColumnBits[Controller][Channel] - (MrcLog2 (MrcData, Outputs->BurstLength * 2) - 1); // The log function return +1 so we subtract 1
      }

      // PprAmtData->Bank is a "don't care" value when FromRowTestPpr = FALSE
      if (PprAmtData->FromRowTestPpr) {
        CpgcTotalBanks = 1;
      } else {
        // Banks + BG
        CpgcTotalBanks = (UINT8) (1 << (PprAmtData->BankBits[Controller][Channel]));
      }

      if (PprAmtData->TestType == PprTestTypeYMarchShort || PprAmtData->TestType == PprTestTypeYMarchLong) {
        CPGCAddressArray[Controller][Channel].AddrDirection = 3;
      } else {
        CPGCAddressArray[Controller][Channel].AddrDirection = FAST_Y;
      }
#if AMT_USE_YMARCH_SHORT_ALT == 1
      if (PprAmtData->TestType == PprTestTypeYMarchShort) {
        // Alternate YMarch on patterns 2 and 3 (Rd/inverted Wr) should use Fast X ordering
        if (PprAmtData->PatternNumber == 2 || PprAmtData->PatternNumber == 3) {
          CPGCAddressArray[Controller][Channel].AddrDirection = 3;
        } else {
          CPGCAddressArray[Controller][Channel].AddrDirection = FAST_Y;
        }
      }
#endif
      CPGCAddressArray[Controller][Channel].AddrIncOrder = BANK_2_ROW_COL_2_RANK;
      CPGCAddressArray[Controller][Channel].BankSize = CpgcTotalBanks;  // Value is decremented by 1, so Block_Size_Max_Bank=0 when executing a single bank test
      CPGCAddressArray[Controller][Channel].ColSizeBits = CpgcColAddressBits;
      CPGCAddressArray[Controller][Channel].ColStart = 0;
      CPGCAddressArray[Controller][Channel].LastValidAddr = 0;
      CPGCAddressArray[Controller][Channel].RowSizeBits = (PprAmtData->FromRowTestPpr) ? 0 : PprAmtData->RowBits[Controller][Channel];
      CPGCAddressArray[Controller][Channel].RowStart = PprAmtData->TestRowAddr[Controller][IpChannel];

      Cpgc20AddressSetup (MrcData, CPGCAddressArray, FALSE, NULL);
      AmtSetBaseAddressControl (MrcData, PprAmtData->TestType);
      if ((Outputs->IsLpddr && (MrcGetBankBgOrg(MrcData, Outputs->Frequency) != MrcLp5BgMode)) || PprAmtData->FromRowTestPpr) {
        AmtUpdateL2PBankMappingWithoutBG (MrcData, PprAmtData->Rank, PprAmtData->FromRowTestPpr, PprAmtData->Bank);  // Use if LP5 16 bank mode or programming a single bank
      } else {
        MrcUpdateL2PAllsBanksMapping (MrcData, PprAmtData->Rank, LocalMcChBitMask, FALSE);
      }

      // Set base repeats according to bank groups, number of rows tested, and column bits.
      BaseRepeats = CpgcTotalBanks * PprAmtData->TestRowSize[Controller][IpChannel] * (1 << (CpgcColAddressBits));
      if (PprAmtData->TestType == PprTestTypeYMarchLong) {
        Cpgc20BaseRepeats (MrcData, LocalMcChBitMask, BaseRepeats >> 1, 1); // Num Ranks = 1
        MrcSetLoopcount (MrcData, LocalMcChBitMask, 1);
      } else {
        Cpgc20BaseRepeats (MrcData, LocalMcChBitMask, BaseRepeats, 1); // Num Ranks = 1
        MrcSetLoopcount (MrcData, LocalMcChBitMask, 0);
      }

      MRC_DEBUG_MSG (
        &Outputs->Debug,
        MSG_LEVEL_NOTE,
        "Mc%d.C%d: Row = 0x%08x, Size = 0x%08x, Dir = %d, ColBits = %d, RowBits = %d, BankBits = %d\n",
        Controller,
        Channel,
        RowStart,
        PprAmtData->TestRowSize[Controller][IpChannel],
        PprAmtData->Direction,
        PprAmtData->ColumnBits[Controller][Channel],
        PprAmtData->RowBits[Controller][Channel],
        PprAmtData->BankBits[Controller][Channel]
      );
      if (PprAmtData->FromRowTestPpr) {
        MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE,
          "  Single Bank: %d\n", PprAmtData->Bank);
      }
    } // for Channel
  } // for Controller

  // Set stop on error control (if we need Stop on Error)
  //MrcSetupTestErrCtl(MrcData, NTHSOE, 1); // Stop on first error

  return McChBitMaskEnabled;
}

/**
  This function sets up a test for the given MC channel mask, with specified data pattern.

  @param[in,out] MrcData           - Pointer to MRC global data.
  @param[in]     PprAmtData        - PPR and AMT data structure
  @param[in]     McChBitMask       - Memory Controller Channel Bit mask to test.
  @param[in]     LoopCount         - CPGC sequence loop count

  @retval mrcSuccess
**/
MrcStatus
SetupIOTestRetention (
  IN OUT   MrcParameters *const     MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN       UINT8                    McChBitMask,
  IN       UINT8                    LoopCount
  )
{
  MrcOutput       *Outputs;
  UINT8           RwMode;
  UINT8           UseSubSeq1;
  BOOLEAN         StopOnRaster;
  BOOLEAN         RasterRepoClear;
  UINT8           RasterRepoMode;
  UINT8           Mode3Banks;
  UINT8           Mode3Max;
  MRC_PATTERN_CTL PatCtl;
  MRC_ADDRESS     CpgcAddressArr[MAX_CONTROLLER][MAX_CHANNEL];
  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT       CpgcSeqCtl;
  BOOLEAN         SkipZqRestore;
  BOOLEAN         IsMarchAlgorithm;
  INT64           GetSetDis;
  UINT8           Channel;
  UINT8           Controller;

  MC0_REQ0_CR_CPGC2_DATA_INSTRUCTION_0_STRUCT           DataInstruct[1];
  MC0_REQ0_CR_CPGC2_COMMAND_INSTRUCTION_0_STRUCT        Cpgc2CmdInstruction[3]; // 3 instructions defined in this function
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_0_STRUCT      Cpgc2AlgoInstruction[1];
  MC0_REQ0_CR_CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_STRUCT AlgoInstructControl;
  MC0_REQ0_CR_CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_STRUCT AlgoWaitEventControl;

  PatCtl.IncRate = 0;
  PatCtl.Start = 0;
  PatCtl.Stop = 8;
  PatCtl.DQPat = StaticPattern;
  PatCtl.PatSource = MrcPatSrcStatic;
  PatCtl.EnableXor = FALSE;

  Outputs = &MrcData->Outputs;
  GetSetDis = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, Outputs->MaxChannels) == 0) || IS_MC_SUB_CH (Outputs->IsLpddr, Channel)) {
        continue;
      }
      // MC Controller Init - disable miscellaneous events
      // functional CKE logic, do not disable CKE powerdown,
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOverride,   WriteToCache, &GetSetDis);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCkeOn,         WriteToCache, &GetSetDis);

      MrcFlushRegisterCachedData (MrcData);
    }
  }

  switch (PprAmtData->TestType) {
    case PprTestTypeXMarch:
    case PprTestTypeXMarchG:
    case PprTestTypeYMarchShort:
    case PprTestTypeYMarchLong:
      IsMarchAlgorithm = TRUE;
      break;
    default:
      IsMarchAlgorithm = FALSE;
      break;
  }

  // Update the Bank/Row/Col sizes per current rank population
  UpdateAddressForWholeRank (MrcData, CpgcAddressArr, BANK_2_ROW_COL_2_RANK, McChBitMask, PprAmtData->Rank);

  Cpgc20UpdateBaseRepeatsForWholeRank (MrcData, PprAmtData->Rank, Outputs->McChBitMask, FALSE);

  SkipZqRestore = Outputs->SkipZq;
  Outputs->SkipZq = TRUE;
  SetupIOTestAmt (MrcData, McChBitMask, PprAmtData->CmdPat, PprAmtData->NumCL, LoopCount, CpgcAddressArr, NSOE, &PatCtl, 0);
  Outputs->SkipZq = SkipZqRestore;

  // Set subsequence read or write data based on requested CmdPat mode
  RwMode = CPGC_BASE_WRITE_SUBSEQ;
  UseSubSeq1 = CPGC_NO_SUBSEQ1;
  if (PprAmtData->CmdPat == PatRdWr || PprAmtData->CmdPat == PatRd) {
    RwMode = CPGC_BASE_READ_SUBSEQ;
  }
  if (PprAmtData->CmdPat == PatRdWr) {
    UseSubSeq1 = CPGC_BASE_WRITE_SUBSEQ;
  }

  // Setup WDB
  // If FromRowTestPpr, we are either injecting an error or running a single row test before PPR execution
  // If PatternNumber==1, we are programming a new data pattern at the beginning of a new TestNumber
  // If LastWriteErrorInjection, the data pattern needs to be restored after error injection
  if (PprAmtData->FromRowTestPpr || PprAmtData->PatternNumber == 1 || PprAmtData->LastWriteErrorInjection) {
    if (PprAmtData->TestType == PprTestTypeMmrw) {
      MrcProgramDSPattern(MrcData, PprAmtData, McChBitMask, PprAmtData->ErrInjMask);
    } else {
      MrcProgramMATSPattern(MrcData, PprAmtData, McChBitMask, PprAmtData->ErrInjMask);
    }
  }
  if (PprAmtData->LastWriteErrorInjection && PprAmtData->ErrInjMask == 0) {
    // Once the data pattern has been restored after error injection, do not need to modify it again until the next TestNumber
    PprAmtData->LastWriteErrorInjection = FALSE;
  }

  //Cpgc20SetDpatBufCtl (MrcData, 1, 0, PatCtl.Start, PatCtl.Stop);

  CpgcSeqCtl.Data = 0;
  CpgcSeqCtl.Bits.CLEAR_ERRORS = 1;
  CpgcSeqCtl.Bits.STOP_TEST    = 1;     // Stop previous test if any
  Cpgc20ControlRegWrite (MrcData, McChBitMask, CpgcSeqCtl);

  if (IsMarchAlgorithm) {
    switch (PprAmtData->TestType) {
      case PprTestTypeXMarch:
      case PprTestTypeYMarchShort:
      case PprTestTypeYMarchLong:
        DataInstruct[0].Data = 0;
        DataInstruct[0].Bits.Last = 1;
        Cpgc20DataInstructWrite (MrcData, &DataInstruct[0], 0x1);
        break;
      case PprTestTypeXMarchG:
        DataInstruct[0].Data = 0;
        DataInstruct[0].Bits.Data_Background = 0xD;
        DataInstruct[0].Bits.Background_Mode = 1;
        DataInstruct[0].Bits.Last = 1;
        Cpgc20DataInstructWrite (MrcData, &DataInstruct[0], 0x1);
        Cpgc20SetDataControl (MrcData, 0, 1);
        break;
      default:
        break;
    }

    // Write CMD [0]
    Cpgc2CmdInstruction[0].Data          = 0;
    Cpgc2CmdInstruction[0].Bits.WriteCmd = 1;
    Cpgc2CmdInstruction[0].Bits.Alternate_Data = 1;
    Cpgc2CmdInstruction[0].Bits.Last     = 1;

    // Read CMD [1]
    Cpgc2CmdInstruction[1].Data          = 0;
    Cpgc2CmdInstruction[1].Bits.Alternate_Data = 1;
    Cpgc2CmdInstruction[1].Bits.Last     = 1;

    Cpgc20CmdInstructWrite (MrcData, &Cpgc2CmdInstruction[0], ARRAY_COUNT (Cpgc2CmdInstruction));

    if (PprAmtData->CmdPat == PatWr) {
      Cpgc2AlgoInstruction[0].Data = 0;
      Cpgc2AlgoInstruction[0].Bits.Last = 1;
    } else {  // CmdPat == PatRd
      Cpgc2AlgoInstruction[0].Data = 0;
      Cpgc2AlgoInstruction[0].Bits.Command_Start_Pointer = 1;
      Cpgc2AlgoInstruction[0].Bits.Last = 1;
    }
    Cpgc2AlgoInstruction[0].Bits.Invert_Data = PprAmtData->IsUseInvtPat;

    AlgoInstructControl.Data = 0;
    AlgoWaitEventControl.Data = 0;

    Cpgc20AlgoInstructWrite (MrcData, &Cpgc2AlgoInstruction[0], AlgoInstructControl, AlgoWaitEventControl, 0x1);

  } else {
    MrcConfigurePprCmdPat (MrcData, RwMode, UseSubSeq1, PprAmtData->Direction, PprAmtData->SubSeqDataInv, PprAmtData->IsUseInvtPat);

    // Adjust programming of CMD instructions to use alternate data source
    // Write CMD [0]
    Cpgc2CmdInstruction[0].Data          = 0;
    Cpgc2CmdInstruction[0].Bits.WriteCmd = 1;
    Cpgc2CmdInstruction[0].Bits.Alternate_Data = 1;
    Cpgc2CmdInstruction[0].Bits.Last     = 1;

    // Read CMD [1]
    Cpgc2CmdInstruction[1].Data          = 0;
    Cpgc2CmdInstruction[1].Bits.Alternate_Data = 1;
    Cpgc2CmdInstruction[1].Bits.Last     = 1;

    Cpgc20CmdInstructWrite (MrcData, &Cpgc2CmdInstruction[0], 2);
  }

  if (Outputs->IsDdr5) {
    // DDR5 uses 32 banks with max 1 fail address per bank
    Mode3Banks = 2;
    Mode3Max = 1;
  } else {
    // LP5 uses 16 banks with max 2 fail addresses per bank
    Mode3Banks = 1;
    Mode3Max = 2;
  }
  RasterRepoClear = 1;
  RasterRepoMode = 3;
  StopOnRaster = 0;
  Cpgc20RasterRepoConfig (MrcData, McChBitMask, &Mode3Banks, NULL, &StopOnRaster, &RasterRepoClear, &RasterRepoMode, &Mode3Max); // Clear Raster Repo, disable StopOnRaster, set Raster Repo Mode 3

  Cpgc20SetPgInvDcEn (MrcData, 0, 0);

  Cpgc20SetDpatAltBufCtl (MrcData, 0, 0, 0, 0);

  //
  // Error Control Registers
  //  do not stop on error (configured above in SetupIOTest)
  //  configure monitor errors on all cachelines
  //  do not monitor errors on all chunks
  //
  MrcSetChunkAndClErrMsk (MrcData, McChBitMask, 0xFF, 0xFFFFFFFF); // LineMask
  MrcSetDataAndEccErrMsk (MrcData, McChBitMask, 0xFFFFFFFFFFFFFFFFULL, 0xFF);

  return mrcSuccess;
}

/**
  Programs all the key registers to define a CPGC test as per input mask and Outputs->McChBitMask.
  McChBitMask is initialized in MrcPretraining based on population, and used throughout internal CPGC structure.
  Modify McChBitMask to specify which MC/CH to program

  @param[in] MrcData       - Include all MRC global data.
  @param[in] McChBitMask   - Memory Controller Channel Bit mask for which test should be setup for.
  @param[in] CmdPat        - 0: PatWrRd (Standard Write/Read Loopback),
                             1: PatWr (Write Only),
                             2: PatRd (Read Only),
                             3: PatRdWrTA (ReadWrite Turnarounds),
                             4: PatWrRdTA (WriteRead Turnarounds)
  @param[in] NumCL         - Number of Cache lines transactions per algorithm instruction within 1 BlockRepeat.
                             Non-zero Inputs->NumCL will override this.
  @param[in] LcExp         - Loop Count exponent. Non-zero Inputs->LoopCount will override this.
  @param[in] AddressArray  - 2D Array of Structure that stores address related settings
  @param[in] SOE           - [0: Never Stop, 1: Stop on Any Lane, 2: Stop on All Byte, 3: Stop on All Lane]
  @param[in] PatCtlPtr     - Structure that stores start, Stop, IncRate and Dqpat for pattern.
  @param[in] SubSeqWait    - # of Dclks to stall at the end of a sub-sequence
**/
VOID
SetupIOTestAmt (
  IN MrcParameters *const         MrcData,
  IN const UINT8                  McChBitMask,
  IN const UINT8                  CmdPat,
  IN UINT32                       NumCL,
  IN UINT8                        LcExp,
  IN MRC_ADDRESS                  AddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN const MRC_TEST_STOP_TYPE     SOE,
  IN MRC_PATTERN_CTL *const       PatCtlPtr,
  IN UINT16                       SubSeqWait
)
{
  //static const UINT32 DataLfsrSeeds[MRC_NUM_MUX_SEEDS]  = {0xA10CA1, 0xEF0D08, 0xAD0A1E};
  //static const UINT32 DataStaticSeeds[MRC_NUM_MUX_SEEDS] = {0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0};
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  UINT32            ChunkMask;
  UINT8             Byte;
  //UINT8             Index;
  UINT8             LaneRotateRate;
  UINT32            BlockRepeats;
  //UINT32            *LfsrSeed;
  BOOLEAN           IsDdr5;
  BOOLEAN           IsPatSrcAllZeroes;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  IsDdr5      = Outputs->IsDdr5;
  IsPatSrcAllZeroes = (PatCtlPtr->PatSource == MrcPatSrcAllZeroes);

  if (Inputs->NumCL != 0) {
    NumCL = Inputs->NumCL;
  }
  if (Inputs->LoopCount != 0) {
    LcExp = Inputs->LoopCount;
  }

  BlockRepeats = MrcCalculateLoopCount (NumCL, LcExp);

  TestDataEngineSetLoopCount (MrcData, McChBitMask,BlockRepeats);
  //Set Number of CL transactions per algorithm instruction within 1 BlockRepeat
  Cpgc20BaseRepeats (MrcData, McChBitMask, MAX (NumCL, 1), 1);

  //###########################################################
  // Program Data Pattern Controls.  PGs are selected for Data
  //###########################################################
  if (!IsPatSrcAllZeroes) {
    // Setup the Pattern Generator for the test.
    //   StaticPattern: The caller programs the pattern

    // Write the LFSR seeds
    if (PatCtlPtr->DQPat == StaticPattern) {
      //LfsrSeed = (UINT32*) DataStaticSeeds;
      LaneRotateRate = 0;
    } else if (PatCtlPtr->DQPat == GaloisMprVa) {
      //LfsrSeed = (UINT32*) DataGaloisSeeds;
      LaneRotateRate = 0;
    } else {
      //LfsrSeed = (UINT32*) DataLfsrSeeds;
      LaneRotateRate = 2;
    }
    /*
    Cpgc20LfsrSeed (MrcData, (const UINT32*) LfsrSeed);

    // Setup CPGC in terms of LFSR selects, LFSR seeds, LMN constants and overall control
    for (Index = 0; Index < MRC_NUM_MUX_SEEDS; Index++) {
    Cpgc20DPatUsqCfg (MrcData, PatCtlPtr, Index);
    }
    */
    //@todo should LSFR Poly match previous code or documentation?
    Cpgc20LfsrCfg (MrcData, LaneRotateRate);
  }

  // Enable/Disable DC/Invert on all lanes, including ECC
  Cpgc20SetPgInvDcEn (MrcData, IsPatSrcAllZeroes ? ENABLE_DQ_LANE_MASK : DISABLE_DQ_LANE_MASK, IsPatSrcAllZeroes ? ENABLE_ECC_LANE_MASK : DISABLE_ECC_LANE_MASK);
  Cpgc20SetPgInvDcCfg (MrcData, IsPatSrcAllZeroes ? Cpgc20DcMode : Cpgc20InvertMode, 0, FALSE, 0, BASIC_STATIC_PATTERN);

  //Setup test command sequence
  Cpgc20SetCommandSequence(MrcData, CmdPat, FALSE, SubSeqWait, FALSE);

  //###########################################################
  // Program Cpgc Address
  //###########################################################

  Cpgc20AddressSetup (
    MrcData,
    AddressArray,
    FALSE,
    NULL
  );
  //###########################################################
  // Program Error Checking
  //###########################################################

  // Enable selective_error_enable_chunk and selective_error_enable_cacheline, mask later
  // the bits we don't want to check.
  // Burst Length: DDR5: 16, LP4/LP5: 32
  ChunkMask = (IsDdr5) ? 0x0000FFFF: 0xFFFFFFFF;
  Cpgc20SetupTestErrCtl  (MrcData, SOE, 1);
  MrcSetChunkAndClErrMsk (MrcData, McChBitMask, 0xFF, ChunkMask);

  // Setup Data and ECC Errors
  MrcSetDataAndEccErrMsk (MrcData, McChBitMask, 0xFFFFFFFFFFFFFFFFULL, 0xFF);
  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    MrcSetupErrCounterCtl (MrcData, Byte, ErrCounterCtlPerByte, 0); //Counts errors per Byte Group, 1 count per UI with an error across all lanes in the Byte Group, all UI
  }

  if (!Outputs->SkipZq) {
    MrcIssueZQ (MrcData);
  }
}

/**
  This function sets up a WR-only test for the given MC channel mask, with all-zero data.
  Used for driving DQ low for Post Package Repair after passing repair row address.

  @param[in,out] MrcData     - Pointer to MRC global data.
  @param[in]     Controller  - Targeted controller
  @param[in]     Channel     - Targeted channel
  @param[in]     Row         - Targeted row
  @param[in]     BankGroup   - Targeted bank group
  @param[in]     BankAddress - Targeted bank address
  @param[in]     ByteMask    - Targeted Bytes for DQ toggle

  @retval MrcStatus          - mrcSuccess if no errors encountered
**/
MrcStatus
SetupIOTestPPR (
  IN OUT MrcParameters* const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel,
  IN     const UINT32         Row,
  IN     const UINT32         BankGroup,
  IN     const UINT32         BankAddress,
  IN     const UINT16         *ByteMask     OPTIONAL
  )
{
  MRC_FUNCTION    *MrcCall;
  MrcOutput       *Outputs;
  MRC_PATTERN_CTL PatCtl;
  UINT8           NumCL;
  UINT8           LC;
  UINT8           Byte;
  UINT8           EccBitMask;
  UINT8           EccByte;
  UINT64          DqBitMask;
  UINT32          McChBitMask;
  UINT32          Bank;
  MRC_ADDRESS     CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL];
  BOOLEAN         SkipZqRestore;
  PPR_AMT_PARAMETER_DATA FakePprAmtData;

  Outputs    = &MrcData->Outputs;
  MrcCall    = MrcData->Inputs.Call.Func;
  MrcCall->MrcSetMem ((UINT8 *) CPGCAddressArray, sizeof (CPGCAddressArray), 0);
  CPGCAddressArray[Controller][Channel].AddrDirection = FAST_Y;
  CPGCAddressArray[Controller][Channel].AddrIncOrder = BANK_2_ROW_COL_2_RANK;
  CPGCAddressArray[Controller][Channel].BankSize = 0;
  CPGCAddressArray[Controller][Channel].ColSizeBits = 0;
  CPGCAddressArray[Controller][Channel].ColStart = 0;
  CPGCAddressArray[Controller][Channel].LastValidAddr = 0;
  CPGCAddressArray[Controller][Channel].RowSizeBits = 0;
  CPGCAddressArray[Controller][Channel].RowStart = Row;

  NumCL = 1;
  LC = 0;
  PatCtl.IncRate = 0;
  PatCtl.Start = 0;
  PatCtl.Stop = 0;
  PatCtl.DQPat = StaticPattern;
  PatCtl.PatSource = MrcPatSrcAllZeroes;  // DC zero
  PatCtl.EnableXor = FALSE;

  SkipZqRestore = Outputs->SkipZq;
  Outputs->SkipZq = TRUE;

  McChBitMask = 1 << MC_CH_IDX (Controller, Channel, Outputs->MaxChannels);

  SetupIOTestCpgc (MrcData, (UINT8) McChBitMask, PatWr, NumCL, LC, CPGCAddressArray, NSOE, &PatCtl, 0, NULL);

  // PprAmtData is not used in the blue version of this function
  MrcConvertBankBgtoLogicalBank (MrcData, &FakePprAmtData, BankAddress, BankGroup, &Bank);

  if (ByteMask != NULL) {
    EccByte    = (Outputs->DdrType == MRC_DDR_TYPE_DDR5) ? MRC_DDR5_ECC_BYTE : 0;
    DqBitMask  = 0;
    EccBitMask = 0;
    for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
      if (((1 << Byte) & *ByteMask) != 0) {
        if (Outputs->EccSupport && Byte == EccByte) {
          EccBitMask = 0xFF;
        } else if (Byte < EccByte) {
          DqBitMask |= MrcCall->MrcLeftShift64 (0xFF, 8 * Byte);
        }
      }
    }
    Cpgc20SetPgInvDcEn (MrcData, DqBitMask, EccBitMask);  // Enable DC on all lanes in Byte Mask
  }

  Outputs->SkipZq = SkipZqRestore;
  Outputs->DQPat = PatCtl.DQPat;

  return mrcSuccess;
}

/**
  Checks the results of the preceding advanced memory test

  @param[in]  MrcData                 - Global MRC data structure
  @param[in]  PprAmtData              - PPR and AMT data structure
  @param[in]  McChBitMask             - Memory Controller Channel Bit mask to check results for.

  @retval status - mrcSuccess / mrcFail
**/
MrcStatus
AmtCheckTestResults (
  IN  MrcParameters *const          MrcData,
  IN  PPR_AMT_PARAMETER_DATA *const PprAmtData,
  IN  UINT32                        McChBitMask
)
{
  MrcOutput               *Outputs;
  MrcStatus               Status = mrcSuccess;
  MRC_FUNCTION            *MrcCall;
  UINT8                   Channel;
  UINT8                   Controller;
  ERROR_STATUS_AMT        CpgcErrorStatus;
  UINT8                   MaxChannel;
  UINT8                   DeviceTemp[MAX_CONTROLLER][MAX_CHANNEL];
  UINT8                   Bank;
  UINT32                  Row;
  UINT32                  UpperRowBits;
  UINT8                   FailAnyAll;
  UINT64_STRUCT           RepoContent[CPGC20_MAX_RASTER_REPO_CONTENT];
  UINT8                   ContentIndex;
  UINT8                   DataIndex;
  UINT32                  DqMask[DQ_MASK_INDEX_MAX];
  BOOLEAN                 Overflow;
#ifdef MRC_DEBUG_PRINT
  MrcDebug                *Debug;
  const CHAR8             *DqText;

  Debug = &MrcData->Outputs.Debug;
  DqText = MrcData->Outputs.IsLpddr ? "DQ[15:0]" : "DQ[31:0]";
#endif

  Outputs = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;
  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcSetMem ((UINT8 *) DeviceTemp, sizeof (DeviceTemp), 0);
  MrcCall->MrcSetMem ((UINT8 *) &CpgcErrorStatus, sizeof (ERROR_STATUS_AMT), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChBitMask, Controller, Channel, MaxChannel)) == 0) {
        continue;
      }

      Cpgc20ReadRasterRepoStatus (MrcData, Controller, Channel, NULL, NULL, NULL, &FailAnyAll, NULL, NULL);
      if (!FailAnyAll) {
#ifdef MRC_DEBUG_PRINT
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR,
          "Mc%d.C%d: No errors found on last test run\n",
          Controller,
          Channel
        );
#endif
        // If no errors, skip checking RasterRepoContent
        continue;
      }

      // If any lane has errors, then read DIMM temperature
      DeviceTemp[Controller][Channel] = MrcReadDeviceTemperature (MrcData, Controller, Channel, PprAmtData->Rank);
      MrcPrintDeviceTemperature (MrcData, DeviceTemp[Controller][Channel]);

      MrcCall->MrcSetMem ((UINT8 *) RepoContent, sizeof (RepoContent), 0);
      Cpgc20ReadRasterRepoContent (MrcData, Controller, Channel, RepoContent);
      for (ContentIndex = 0; ContentIndex < CPGC20_NUM_RASTER_REPO_CONTENT_REG; ContentIndex++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Mc%d.Ch%d: RepoContent[%d] = 0x%llX\n", Controller, Channel, ContentIndex, RepoContent[ContentIndex].Data);
        for (DataIndex = 0; DataIndex < 4; DataIndex++) {
          Bank = ContentIndex * 4 + DataIndex;  // Stores an index that is later shifted right if needed to get the Cpgc bank
          // Check valid bit stored in lower 32 bits of ECC[1]
          if (RepoContent[CPGC20_MAX_RASTER_REPO_CONTENT - 1].Data32.Low & (1 << Bank)) {
            Overflow = RepoContent[CPGC20_MAX_RASTER_REPO_CONTENT - 1].Data32.High & (1 << Bank); // Determine if more than 1 fail happened on bank
            UpperRowBits = (RepoContent[CPGC20_NUM_RASTER_REPO_CONTENT_REG].Data16[Bank >> 3] >> (2 * (Bank % 8))) & 0x3; // 2 Upper row bits are stored in ECC[0]
            Row = (UpperRowBits << 16) | (RepoContent[ContentIndex].Data16[DataIndex] & MRC_UINT16_MAX);
            CpgcErrorStatus.errRow = Row;
            CpgcErrorStatus.errRank = PprAmtData->Rank;
            CpgcErrorStatus.overflow = Overflow;
            AmtCreateDqFailure (MrcData, DqMask);
            CpgcErrorStatus.errData = DqMask[0];
            CpgcErrorStatus.errEcc = DqMask[1];
            CpgcErrorStatus.device = AmtGetFailingDevice (MrcData, Controller, Channel);
            if (Outputs->IsLpddr) {
              // LP5 BG and 16 bank mode both use 16 bank programming
              Bank = Bank >> 1; // Bank number increases every 2 entries in Raster Repo Mode 3 16-bank mode; shift right to divide bank by 2
            }
            CpgcErrorStatus.errBank = Bank;

#ifdef MRC_DEBUG_PRINT
            MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR,
              "Error Found: Mc%d.C%d: Row = 0x%06x, Bank = 0x%02x, Rank%d\n",
              Controller,
              Channel,
              Row,
              Bank,
              PprAmtData->Rank
            );
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "  %s Error on strobe: 0x%02x",
              DqText,
              CpgcErrorStatus.errEcc & MRC_UINT8_MAX
            );
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%08x\n", CpgcErrorStatus.errData);
            if (CpgcErrorStatus.overflow) {
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "  Too many failures found on Bank 0x%02x, did not record excess\n", Bank);
            }
#endif
            if (!PprAmtData->FromRowTestPpr) {
              Status = AmtUpdateRowFailures (MrcData, PprAmtData, Controller, Channel, CpgcErrorStatus, DeviceTemp);
            } else {
              // Being here indicates that a failure was encountered, so return mrcFail to let RowTestPpr know there was a failure
              Status = mrcFail; // Single row test uses Status to determine whether test had bit errors
            }
          }
        }
      }
      for (ContentIndex = 0; ContentIndex < CPGC20_NUM_RASTER_REPO_CONTENT_ECC; ContentIndex++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Mc%d.Ch%d: EccContent[%d] = 0x%llX\n", Controller, Channel, ContentIndex, RepoContent[CPGC20_NUM_RASTER_REPO_CONTENT_REG + ContentIndex].Data);
      }
    }
  }

  return Status;
} // AmtCheckTestResults
