/** @file
  This file contains functions to find and track DRAM Failing addresses for the
  Post Package Repair feature.

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

#include "MrcPpr.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcReset.h"
#include "MrcGeneral.h"

#ifdef MRC_DEBUG_PRINT
static const CHAR8 *PprRepairTypeStrings[] = {
  [NoRepair] = "NoRepair",
  [sPPR] = "sPPR",
  [hPPR] = "hPPR",
  [mPPR] = "mPPR",
};
#endif

#define PPR_ALL_DEVICES_SELECTED      (0x7)
#define DDR5_FULL_CHANNEL_BYTE_MASK   (0xF)
#define LPDDR5_FULL_CHANNEL_BYTE_MASK (0x3)

#define DDR5_CHANNEL_WIDTH   (32)
#define LPDDR5_CHANNEL_WIDTH (16)
/**
  Enter Post Package Repair (PPR) to attempt to repair detected failed row.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Controller  - Controller for detected fail row
  @param[in] Channel     - Channel for detected fail row
  @param[in] Rank        - Rank for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] ByteMask    - Byte mask to repair for fail row

  @retval MrcStatus
**/
MrcStatus
Ddr5PostPackageRepair (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               BankGroup,
  IN UINT32               BankAddress,
  IN UINT32               Row,
  IN UINT16               ByteMask
)
{
  MrcOutput                     *Outputs;
  MrcInput                      *Inputs;
  MRC_EXT_INPUTS_TYPE           *ExtInputs;
  MrcChannelOut                 *ChannelOut;
  MrcDimmOut                    *DimmOut;
  BOOLEAN                       Width16;
  MrcTiming                     *Timing;
  MrcDebug                      *Debug;
  MrcStatus                     Status;
  UINT32                        tMpcNck;
  UINT32                        tMpcNckFs;
  UINT32                        tMpcNs;
  UINT8                         DimmIdx;
  UINT8                         Byte;
  UINT8                         MrrByte;
  UINT32                        Index;
  UINT32                        tRCD;
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  DDR5_MODE_REGISTER_23_TYPE    MR23;
  MrcModeRegister               MRNumber;
  UINT8                         PprResource;
  const MrcSpd                  *SpdIn;
  const SPD5_MANUFACTURING_DATA *Ddr5ManufactureData;
  BOOLEAN                       IsSppr;
  PPR_MC_SETUP_SAVE             PprMcSaveData;

  Outputs     = &MrcData->Outputs;
  Inputs      = &MrcData->Inputs;
  ExtInputs   = Inputs->ExtInputs.Ptr;
  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
  Timing      = &Outputs->Timing[ExtInputs->MemoryProfile];
  Debug       = &Outputs->Debug;
  Status      = mrcSuccess;
  tMpcNck     = MrcGetTmod (MrcData, Timing->tCK);
  tMpcNckFs   = tMpcNck * Outputs->MemoryClock;
  tMpcNs      = DIVIDECEIL (tMpcNckFs, FEMTOSECONDS_PER_NANOSECOND);
  tRCD        = Timing->tRCDtRP;
  tRCD        *= Outputs->tCKps; // convert from nCK to ps
  tRCD        /= 1000; //convert from ps to ns
  IsSppr      = Inputs->PprRepairType == sPPR;

  // DDR5 PPR Beginning Sequence
  static const MR_DATA_STRUCT Ddr5PprGuardKeySequence[] = {
    { mrMR24, 0xCF },
    { mrMR24, 0x73 },
    { mrMR24, 0xBB },
    { mrMR24, 0x3B },
    { mrEndOfSequence, 0 }
  };

  DimmIdx = (UINT8) RANK_TO_DIMM_NUMBER (Rank);
  DimmOut = &ChannelOut->Dimm[DimmIdx];
  Width16 = (DimmOut->SdramWidth == 16);

  // Get MR associated with BankGroup
  if ((BankGroup == 0x0) || (BankGroup == 0x1)) {
    MRNumber = mrMR54;
  } else if ((BankGroup == 0x2) || (BankGroup == 0x3)) {
    MRNumber = mrMR55;
  } else if ((BankGroup == 0x4) || (BankGroup == 0x5)) {
    MRNumber = mrMR56;
  } else if ((BankGroup == 0x6) || (BankGroup == 0x7)) {
    MRNumber = mrMR57;
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "DDR5 PPR Error, incorrect parameters: BankGroup%d, BankAddress%d\n", BankGroup, BankAddress);
    return mrcWrongInputParameter;
  }

  MrcDdr5PprControllerSetup (MrcData, Controller, Channel, &PprMcSaveData);

  MrcIssueMrr (MrcData, Controller, Channel, Rank, MRNumber, MrrResult); // Check the correct MR for PPR Resources, based on BA/BG
  for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
    if (Byte == MRC_DDR5_ECC_BYTE) {
      continue;
    }

    if (Width16) {
      // Need combine 2 bytes for repair
      if ((Byte % 2) == 0) {
        // If either byte is set, set 2 bytes
        if (((0x3 << Byte) & ByteMask) != 0) {
          ByteMask |= 0x3 << Byte;
        }
      }
      // DRAM returns only the even byte
      MrrByte = Byte & 0xFE;
      } else {
      MrrByte = Byte;
    }

    MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "  MR%d Byte %d MrrByte %d: ", MRNumber, Byte, MrrByte);

    if (((1 << Byte) & ByteMask) == 0) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Disabled in ByteMask\n");
      continue;
    }
    MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "Value = 0x%X\n", MrrResult[MrrByte]);

    PprResource = MrrResult[MrrByte];
    if (BankGroup & 0x1) { // Odd number bank group
      PprResource = (PprResource & (MRC_BIT4 << BankAddress));
    } else { // Even number bank group
      PprResource = (PprResource & (MRC_BIT0 << BankAddress));
    }
    if (PprResource == 0) { // Resource is not available
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PPR Resource not available: BankGroup%d, BankAddress%d, Byte%d, removing from ByteMask\n", BankGroup, BankAddress, Byte);
      ByteMask &= ~(1 << Byte); // Remove current byte from ByteMask
    }
    }

  if (ByteMask == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No PPR Resources available: BankGroup%d, BankAddress%d, Exiting PPR\n", BankGroup, BankAddress);
    return mrcPPRBankRowUnavailable;
  }

  // Zero the contents of MR23 to ensure only one bit is set
  MR23.Data8 = 0;

  // Issue PREA using MPTU Dunit
  Status = MrcIssuePreaCmd (MrcData, Controller, Channel, Rank);

  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  if (IsSppr) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PprRepairType enables SOFT PPR, executing sPPR\n");
    MR23.Bits.Sppr = 1; // Setting bit OP[0] to high for sPPR
  } else {
    MR23.Bits.Hppr = 1; // Setting bit OP[0] to high for hPPR
  }

  Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR23, MR23.Data8, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tRCD * MRC_TIMER_1NS); // wait tRCD

  for (Index = 0; Ddr5PprGuardKeySequence[Index].MRnum != mrEndOfSequence; Index++) { // Pass PPR Guard Key
    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, Ddr5PprGuardKeySequence[Index].MRnum, Ddr5PprGuardKeySequence[Index].Data, TRUE);
    if (Status != mrcSuccess) {
      return Status;
    }
    MrcWait (MrcData, tRCD * MRC_TIMER_1NS); // wait tRCD
  }

  // Issue WRA using MPTU Dunit
  MrcIssueWraCmd (MrcData, Controller, Channel, Rank, BankGroup, BankAddress, Row, ByteMask);

  // Wait tPGMa/tPGMb for Repair to complete. tPGMa is used for x8, tPGMb for x16
  if (DimmOut->SdramWidth == 8) {
    MrcWait (MrcData, MRC_DDR5_tPGMa * MRC_TIMER_1MS);
  } else {
    MrcWait (MrcData, MRC_DDR5_tPGMb * MRC_TIMER_1MS);
  }

  // Issue Precharge PB
  MrcIssuePrepbCmd (MrcData, Controller, Channel, Rank, BankGroup, BankAddress);

  MrcWait (MrcData, tMpcNs);

  // Wait tPGM_Exit(min) = tRP = tRCD
  MrcWait (MrcData, tRCD * MRC_TIMER_1US);

  // Setting bits OP[1:0] to low to exit PPR
  MR23.Bits.Sppr = 0;
  MR23.Bits.Hppr = 0;
  Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR23, MR23.Data8, FALSE);
  if (Status != mrcSuccess) {
    return Status;
  }

  MrcWait (MrcData, MRC_DDR5_tPGMPST * MRC_TIMER_1US);

  IoReset (MrcData);

  MrcDdr5PprControllerTeardown (MrcData, Controller, Channel, &PprMcSaveData);
  MrcWait (MrcData, 20 * MRC_TIMER_1NS);

  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[DimmIdx].Spd.Data;
  Ddr5ManufactureData = &SpdIn->Ddr5.ManufactureInfo;
  if ((!IsSppr) &&
    ((Ddr5ManufactureData->DramIdCode.Data == 0xCE80) ||
      (Ddr5ManufactureData->ModuleId.IdCode.Data == 0xCE80))) {
    MrcResetSequence(MrcData);
  }

  return Status;
}

/**
  Enter Post Package Repair (PPR) to attempt to repair detected failed row.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] Controller  - Controller for detected fail row
  @param[in] Channel     - Channel for detected fail row
  @param[in] Rank        - Rank for detected fail row
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] Row         - Row for detected fail row

  @retval MrcStatus
**/

MrcStatus
LpDdr5PostPackageRepair (
  IN MrcParameters* const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT32               BankGroup,
  IN UINT32               BankAddress,
  IN UINT32               Row
  )
{
  MRC_EXT_INPUTS_TYPE             *ExtInputs;
  MrcOutput                       *Outputs;
  //MrcFrequency                    CurrentFrequency;
  MrcChannelOut                   *ChannelOut;
  MrcTiming                       *Timing;
  MrcStatus                       Status;
  UINT32                          tMpcNck;
  UINT32                          tMpcNckFs;
  UINT32                          tMpcNs;
  UINT8                           MrrResult[MRC_MRR_ARRAY_SIZE];
  //BOOLEAN                         IsFreqSwitchRequired;
  //UINT8                           RankMask;
  UINT32                          tCK;
  UINT32                          tMRD;
  UINT32                          tMRDns;
  LPDDR5_MODE_REGISTER_41_TYPE    Mr41;
  MRC_LP5_BANKORG                 BankMode;
  PPR_MC_SETUP_SAVE               PprMcSaveData;
  UINT32                          PprResource;

  ExtInputs             = MrcData->Inputs.ExtInputs.Ptr;
  Outputs               = &MrcData->Outputs;
  //IsFreqSwitchRequired  = FALSE;
  BankMode              = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  ChannelOut            = &Outputs->Controller[Controller].Channel[Channel];
  Timing                = &Outputs->Timing[ExtInputs->MemoryProfile];
  tMpcNck               = MrcGetTmod (MrcData, Timing->tCK);
  tMpcNckFs             = tMpcNck * Outputs->MemoryClock;
  tMpcNs                = DIVIDECEIL (tMpcNckFs, FEMTOSECONDS_PER_NANOSECOND);
  tCK                   = Outputs->MemoryClock;
  tMRD                  = MrcGetTmod (MrcData, tCK);
  tMRDns                = tMRD * tCK;
  tMRDns                = DIVIDECEIL (tMRDns, FEMTOSECONDS_PER_NANOSECOND);
  //RankMask              = 1 << Rank;

  // LPDDR5 PPR Beginning Sequence
  static const MR_DATA_STRUCT LpDdr5PprGuardKeySequence[] = {
    { mrMR42, 0xCF },
    { mrMR42, 0x73 },
    { mrMR42, 0xBB },
    { mrMR42, 0x3B },
    { mrEndOfSequence, 0 }
  };

  MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR29, MrrResult);
  switch (BankMode) {
    case MrcLp58Bank:
      PprResource = (MrrResult[0] & (MRC_BIT0 << BankAddress));
      break;
    case MrcLp516Bank:
      PprResource = (MrrResult[0] & (MRC_BIT0 << (BankAddress & 0x7)));
      break;
    case MrcLp5BgMode:
      if ((BankGroup % 2) == 0) { // Even number bank groups
        PprResource = (MrrResult[0] & (MRC_BIT0 << BankAddress));
      } else { // Odd number bank groups
        PprResource = (MrrResult[0] & (MRC_BIT4 << BankAddress));
      }
      break;
    default:
      return mrcWrongInputParameter;
      break;
  }
  if (PprResource == 0) { // Resource is not available
    return mrcPPRBankRowUnavailable;
  }

  // PPR resource for Bank Group/Address pair is available

  /*
  CurrentFrequency = Outputs->Frequency;
  // PPR must run at a frequency <= 6400MHz
  if (CurrentFrequency > f6400) {
    IsFreqSwitchRequired = TRUE;
    Status = MrcSetFspVrcg (MrcData, RankMask, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, LpFspOpPoint0);
    Status |= MrcFrequencySwitch (MrcData, Outputs->LowFrequency, MRC_PRINTS_OFF);
    if (Status != mrcSuccess) {
      return Status;
    }
  }
  */

  MrcLpddr5PprControllerSetup (MrcData, Controller, Channel, &PprMcSaveData);

  // Issue PREA
  Status = MrcIssuePreaCmd (MrcData, Controller, Channel, Rank);

  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  Mr41.Data8 = (UINT8) ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR[mrIndexMR41];
  Mr41.Bits.Ppre = 1; // Setting bit OP[0] to high for PPR
  MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR41, Mr41.Data8, FALSE);
  MrcWait (MrcData, tMRDns);

  for (UINT32 Index = 0; LpDdr5PprGuardKeySequence[Index].MRnum != mrEndOfSequence; Index++) { // Pass PPR Guard Key
    MrcIssueMrw (MrcData, Controller, Channel, Rank, LpDdr5PprGuardKeySequence[Index].MRnum, LpDdr5PprGuardKeySequence[Index].Data, FALSE);
    MrcWait (MrcData, tMRDns);
  }

  Status = MrcIssueActCmd (MrcData, Controller, Channel, Rank, BankGroup, BankAddress, Row);

  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  // Wait for PPR programming time
  MrcWait (MrcData, MRC_LPDDR5_tPGM * MRC_TIMER_1MS);

  // Issue PREA using MPTU Dunit
  Status = MrcIssuePreaCmd (MrcData, Controller, Channel, Rank);

  if (Status != mrcSuccess) {
    return Status;
  }
  MrcWait (MrcData, tMpcNs);

  // Wait PPR Exit time
  MrcWait (MrcData, MRC_LPDDR5_tPGM_EXIT * MRC_TIMER_1NS);

  Mr41.Bits.Ppre = 0; // Setting bit OP[0] to low to exit PPR
  MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR41, Mr41.Data8, FALSE);

  // Wait PPR new Address Setting time
  MrcWait (MrcData, MRC_LPDDR5_tPGMPST * MRC_TIMER_1US);

  /*
  if (IsFreqSwitchRequired) {
    // Set FSP-OP = 1, set High frequency
    Status = MrcLpddrSwitchToHigh (MrcData, MRC_PRINTS_OFF);
    if (Status != mrcSuccess) {
      return Status;
    }
  }
  */

  MrcResetSequence (MrcData);

  MrcLpddr5PprControllerTeardown (MrcData, Controller, Channel, &PprMcSaveData);

  return Status;
}

/**
  This function enables and runs Post Package Repair on detected fail rows.
  @param[in] MrcData     - Pointer to global MRC data.
  @param[in] Controller  - 0-based index to controller
  @param[in] Channel     - 0-based index to channel
  @param[in] Rank        - Physical rank index inside the dimm
  @param[in] BankGroup   - BankGroup for detected fail row
  @param[in] BankAddress - BankAddress for detected fail row
  @param[in] Row         - Row for detected fail row
  @param[in] ByteMask    - Byte mask to repair for fail row
  @retval MrcStatus - mrcSuccess if the DelayType is supported.
**/
MrcStatus
MrcPostPackageRepair (
  IN  MrcParameters *const MrcData,
  IN  UINT32         Controller,
  IN  UINT32         Channel,
  IN  UINT32         Rank,
  IN  UINT32         BankGroup,
  IN  UINT32         BankAddress,
  IN  UINT32         Row,
  IN  UINT16         ByteMask
  )
{
  MrcOutput      *Outputs;
  MrcInput       *Inputs;
  MrcDdrType     DdrType;
  MrcStatus      Status;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  DdrType = Outputs->DdrType;

  if (Inputs->PprRepairType == NoRepair) {
    return mrcSuccess;
  }

  Status      = mrcSuccess;
  if (DdrType == MRC_DDR_TYPE_DDR5) {
    Status = Ddr5PostPackageRepair (MrcData, Controller, Channel, Rank, BankGroup, BankAddress, Row, ByteMask);
  } else {
    if (Inputs->PprRepairType == hPPR) {
      Status = LpDdr5PostPackageRepair (MrcData, Controller, Channel, Rank, BankGroup, BankAddress, Row);
    }
  }

  // Updating MRC PPR Status
  MrcData->Outputs.MrcPprStatus = Status;
  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "MrcPostPackageRepair Status: %s\n", Status == mrcSuccess ? "Success" : "Failed");
  return Status;
}

/**

  Get status whether PPR resource is available

  @param[in] MrcData        - Global MRC data structure
  @param[in] Controller     - Controller number to update
  @param[in] Channel        - Channel number to update
  @param[in] Rank           - Rank number
  @param[in] DeviceByte     - Byte number used to identify DRAM
  @param[in] BankGroup      - BankGroup number
  @param[in] BankAddress    - BankAddress number

  @retval TRUE if PPR resource available; FALSE if not available

**/
BOOLEAN
GetPprResourceAvailable (
  MrcParameters   *const    MrcData,
  UINT32                    Controller,
  UINT32                    Channel,
  UINT32                    Rank,
  UINT16                    DeviceByte,
  UINT8                     BankGroup,
  UINT8                     BankAddress
  )
{
  MrcDebug        *Debug;
  MrcOutput       *Outputs;
  BOOLEAN         PprResourceAvailable;
  UINT32          MRNumber;
  UINT8           MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT8           PprResource;
  MRC_LP5_BANKORG BankMode;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;
  PprResourceAvailable = FALSE;
  BankMode      = MrcGetBankBgOrg(MrcData, Outputs->Frequency);

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      if ((BankGroup == 0x0) || (BankGroup == 0x1)) {
        MRNumber = mrMR54;
      } else if ((BankGroup == 0x2) || (BankGroup == 0x3)) {
        MRNumber = mrMR55;
      } else if ((BankGroup == 0x4) || (BankGroup == 0x5)) {
        MRNumber = mrMR56;
      } else if ((BankGroup == 0x6) || (BankGroup == 0x7)) {
        MRNumber = mrMR57;
      } else {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "DDR5 PPR Error, incorrect parameters: BankGroup%d, BankAddress%d\n", BankGroup, BankAddress);
        return mrcWrongInputParameter;
      }

      MrcIssueMrr (MrcData, Controller, Channel, Rank, MRNumber, MrrResult); // Check the correct MR for PPR Resources, based on BA/BG
      PprResource = MrrResult[DeviceByte];
      if (BankGroup & 0x1) { // Odd number bank group
        PprResource = (PprResource & (MRC_BIT4 << BankAddress));
      } else { // Even number bank group
        PprResource = (PprResource & (MRC_BIT0 << BankAddress));
      }
      if (PprResource == 0) { // Resource is not available
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No PPR Resources available: BankGroup%d, BankAddress%d, Device%d\n", BankGroup, BankAddress, DeviceByte);
        PprResourceAvailable = FALSE;
      } else {
        PprResourceAvailable = TRUE;
      }
      break;
    case MRC_DDR_TYPE_LPDDR5:
      MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR29, MrrResult);
      PprResource = MrrResult[0];

      switch (BankMode) {
        case MrcLp58Bank:
          PprResource = (PprResource & (MRC_BIT0 << BankAddress));
          break;
        case MrcLp516Bank:
          PprResource = (PprResource & (MRC_BIT0 << (BankAddress & 0x7)));
          break;
        case MrcLp5BgMode:
          if ((BankGroup % 2) == 0) { // Even number bank groups
            PprResource = (PprResource & (MRC_BIT0 << BankAddress));
          } else { // Odd number bank groups
            PprResource = (PprResource & (MRC_BIT4 << BankAddress));
          }
          break;
        default:
          return mrcWrongInputParameter;
          break;
      }
      if (PprResource == 0) { // Resource is not available
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No PPR Resources available: BankGroup%d, BankAddress%d, Device%d\n", BankGroup, BankAddress, DeviceByte);
        PprResourceAvailable = FALSE;
      } else {
        PprResourceAvailable = TRUE;
      }
      break;
    default:
      break;
  }
  return PprResourceAvailable;
}

/**
  Get status for all PPR resources and store number of resources available
  per Controller/Channel/Rank/Dimm into Outputs->PprAvailableResources.

  @param[in] MrcData        - Global MRC data structure
**/
VOID
GetAllPprResources (
  MrcParameters   *const    MrcData
  )
{
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  UINT8               Controller;
  UINT8               Ch;
  UINT8               Rank;
  UINT8               Dram;
  UINT8               BytesPerDram;
  MrcDimmOut          *DimmOut;
  UINT8               MaxChDdr;
  MrcChannelOut       *ChannelOut;
  UINT8               DeviceCount;
  UINT8               BankGroup;
  UINT32              MRNumber;
  UINT8               MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT8               PprResource;
  UINT8               Ddr5BgIdx;
  UINT8               Lp5BankIdx;


  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  MaxChDdr    = Outputs->MaxChannels;
  DeviceCount = Outputs->SdramCount;


  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Ch];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Ch, Rank)) {
          continue;
        }

        DimmOut = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER(Rank)];

        if (DimmOut->SdramWidth == 8) { // x8
          BytesPerDram = 1;
        } else { // x16
          BytesPerDram = 2;
        }

        switch (Outputs->DdrType) {
          case MRC_DDR_TYPE_DDR5:
            // Minimize the number of MRRs by reading the MrrResult for every device before moving to the next MR number
            for (BankGroup = 0; BankGroup < DimmOut->BankGroups; BankGroup += 2) {
              switch (BankGroup) {
                case 0x0:
                case 0x1:
                  MRNumber = mrMR54;
                  break;
                case 0x2:
                case 0x3:
                  MRNumber = mrMR55;
                  break;
                case 0x4:
                case 0x5:
                  MRNumber = mrMR56;
                  break;
                case 0x6:
                case 0x7:
                  MRNumber = mrMR57;
                  break;
                default:
                  MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Warning: BankGroup %d is outside of range 0-7\n", BankGroup);
                  continue;
              }
              MrcIssueMrr (MrcData, Controller, Ch, Rank, MRNumber, MrrResult); // Check the correct MR for PPR Resources, based on BA/BG

              for (Dram = 0; Dram < DeviceCount; Dram += BytesPerDram) {
                Outputs->PprAvailableResources[Controller][Ch][Rank][Dram] = 0;
                PprResource = MrrResult[Dram];
                for (Ddr5BgIdx = 0; Ddr5BgIdx < 2; Ddr5BgIdx++) {
                  // Ddr5BgIdx 0 => Even BG - take the logical AND of bytes 0-3
                  // Ddr5BgIdx 1 => Odd BG - take the logical AND of bytes 4-7
                  if (((PprResource >> (4 * Ddr5BgIdx)) & 0xF) == 0xF) {
                    Outputs->PprAvailableResources[Controller][Ch][Rank][Dram]++;
                  }
                }
              } // for Dram
            }
            break;
          case MRC_DDR_TYPE_LPDDR5:
            MrcIssueMrr (MrcData, Controller, Ch, Rank, mrMR29, MrrResult);
            for (Dram = 0; Dram < DeviceCount; Dram += BytesPerDram) {
              Outputs->PprAvailableResources[Controller][Ch][Rank][Dram] = 0;
              PprResource = MrrResult[Dram];
              for (Lp5BankIdx = 0; Lp5BankIdx < 8; Lp5BankIdx++) {
                if (PprResource & (1 << Lp5BankIdx)) {
                  Outputs->PprAvailableResources[Controller][Ch][Rank][Dram]++;
                }
              }
              // Available resources for LP5 are the number of banks with PPR resource available
              Outputs->PprAvailableResources[Controller][Ch][Rank][Dram] *= 2;
            }
            break;
          default:
            break;
        }
      } // for Rank
    } // for Ch
  } // for Controller
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
  This is a test function for direct testing of the Post Package Repair sequence.
  @param[in] MrcData     - Pointer to global MRC data.

  @return MrcStatus - mrcSuccess if no errors, otherwise return error status.
**/
MrcStatus
MrcPostPackageRepairTest (
  IN  MrcParameters *const MrcData
  )
{
  MrcInput *Inputs = &MrcData->Inputs;
  MrcStatus Status = mrcSuccess;
  BOOLEAN IsMbist  = (((UINT32) Inputs->PprTestType.Value) & (1 << PprTestTypeMbist)) ? 1 : 0;

  if (MrcIsTargetedPprRequested (MrcData)) {
    Status = MrcRunPprTargeted (MrcData);
  } else if (IsMbist) {
    Status = RunMbistMppr (MrcData);
  }

  return Status;
}

/**
  Checks if a specific PPR Entry Address exists in the system.

  @param[in] MrcData global MRC data structure.
  @param[in] PprEntryAddress PPR Entry Address to check.

  @returns TRUE if address exists, otherwise FALSE.
**/
BOOLEAN
MrcIsPprEntryAddressExists (
  IN MrcParameters *const MrcData,
  IN MRC_PPR_ENTRY_ADDRESS PprEntryAddress
  )
{
  MrcOutput *const Outputs = &MrcData->Outputs;
  MrcDimmOut *DimmOut;

  UINT32 Controller = (UINT32) PprEntryAddress.Controller;
  UINT32 Channel    = (UINT32) PprEntryAddress.Channel;
  UINT32 Rank       = (UINT32) PprEntryAddress.Rank;
  UINT8  ChannelWidth;
  UINT8  MaxSdramDevice;

  if (!MrcControllerExist (MrcData, Controller)          ||
      !MrcChannelExist (MrcData, Controller, Channel)) {
    return FALSE;
  }

  if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
    return FALSE;
  }

  DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER(Rank)];
  if (PprEntryAddress.BankGroup >= DimmOut->BankGroups ||
      PprEntryAddress.Bank >= DimmOut->Banks           ||
      PprEntryAddress.Row >= DimmOut->RowSize          ) {
    return FALSE;
  }

  if (PprEntryAddress.Device != PPR_ALL_DEVICES_SELECTED) {
    ChannelWidth = Outputs->DdrType == MRC_DDR_TYPE_DDR5 ? DDR5_CHANNEL_WIDTH : LPDDR5_CHANNEL_WIDTH;
    MaxSdramDevice = ChannelWidth / DimmOut->SdramWidth;
    if (PprEntryAddress.Device >= MaxSdramDevice) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Run Targeted PPR if requested.
  @param[in] MrcData pointer to global MRC data.
  @param[in] PprAmtData pointer to PPR and AMT data structure.

  @returns MrcStatus
**/
MrcStatus
MrcRunPprTargeted (
  IN MrcParameters *const MrcData
  )
{
  MrcInput *const Inputs = &MrcData->Inputs;
  MrcOutput *const Outputs = &MrcData->Outputs;
  MrcDebug *Debug;
  MrcStatus Status = mrcSuccess;

  Debug = &Outputs->Debug;

  MrcDimmOut *DimmOut;
  UINT8 Entry;

  UINT32 Controller;
  UINT32 Channel;
  UINT32 Rank;
  UINT32 BankGroup;
  UINT32 BankAddress;
  UINT32 Row;
  UINT32 Device;
  UINT16 ByteMask;

  UINT8 BytesNumInDevice;
  UINT8 Byte;

  for (Entry = 0; Entry < ARRAY_COUNT (Inputs->PprEntryInfo); Entry++) {
    if (Inputs->PprEntryInfo[Entry].PprValid) {

      Controller = (UINT32) Inputs->PprEntryAddress[Entry].Controller;
      Channel = (UINT32) Inputs->PprEntryAddress[Entry].Channel;
      Rank = (UINT32) Inputs->PprEntryAddress[Entry].Rank;
      DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER(Rank)];

      BankGroup = (UINT32) Inputs->PprEntryAddress[Entry].BankGroup;
      BankAddress = (UINT32) Inputs->PprEntryAddress[Entry].Bank;
      Row = (UINT32) Inputs->PprEntryAddress[Entry].Row;
      Device = (UINT32) Inputs->PprEntryAddress[Entry].Device;

      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PPR Entry %d: MC%d CH%d R%d BG: %d, Bank: %d, Row: %d, Device: %d\n",
        Entry, Controller, Channel, Rank, BankGroup, BankAddress, Row, Device);

      if (!MrcIsPprEntryAddressExists (MrcData, Inputs->PprEntryAddress[Entry])) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "PPR Entry %d: Invalid address was requested.\n", Entry);
        continue;
      }

      if (Device == PPR_ALL_DEVICES_SELECTED) {
        ByteMask = Outputs->DdrType == MRC_DDR_TYPE_DDR5 ? DDR5_FULL_CHANNEL_BYTE_MASK : LPDDR5_FULL_CHANNEL_BYTE_MASK;
      } else {
        ByteMask = 0;
        BytesNumInDevice = DimmOut->SdramWidth / 8;
        for (Byte = 0; Byte < BytesNumInDevice; Byte++) {
          ByteMask |= (1 << (Device * BytesNumInDevice + Byte));
        }
      }

      Outputs->PprNumDetectedErrors++;
      Status = MrcPostPackageRepair (MrcData, Controller, Channel, Rank, BankGroup, BankAddress, Row, ByteMask);
      if (Status == mrcSuccess) {
        Outputs->PprNumSuccessfulRepairs++;
      } else {
        Outputs->PprRepairFails++;
      }
    }
  }

  return Status;
}

/**
  Checks if Targeted PPR is requested.

  @param[in] MrcData global MRC data structure.

  @returns TRUE if Targeted PPR is requested, otherwise FALSE.
**/
BOOLEAN
MrcIsTargetedPprRequested (
  IN MrcParameters *const MrcData
  )
{
  MrcInput *Inputs = &MrcData->Inputs;
  BOOLEAN IsTargetedPprRequested = FALSE;
  UINT8 EntryPprIndex;

  for (EntryPprIndex = 0; EntryPprIndex < PPR_REQUEST_MAX; EntryPprIndex++) {
    if (Inputs->PprEntryInfo[EntryPprIndex].PprValid) {
      IsTargetedPprRequested = TRUE;
      break;
    }
  }

  return IsTargetedPprRequested;
}


/**
  Get status whether PPR is enabled or disabled based on supported usecases.

  @param[in] MrcData global MRC data structure.

  @retval TRUE if PPR is enabled and a supported usecase is detected.
  @retval FALSE if PPR is disabled.
**/
BOOLEAN
MrcIsPprEnabled (
  IN MrcParameters *const MrcData
  )
{
  MrcInput *Inputs = &MrcData->Inputs;

  UINT32  PprTestType    = (UINT32)  Inputs->PprTestType.Value;
  BOOLEAN IsMbist        = (((UINT32) Inputs->PprTestType.Value) & (1 << PprTestTypeMbist)) ? 1 : 0;
  BOOLEAN IsTestDisabled = (((UINT32) Inputs->PprTestType.Value) & (1 << PprTestTypeTestDisabled)) ? 1 : 0;

  BOOLEAN IsTargetedPprRequested;

  MrcDebug *Debug;
  Debug = &MrcData->Outputs.Debug;

  IsTargetedPprRequested = MrcIsTargetedPprRequested (MrcData);

  // Use Case 1: Algorithm + hPPR/sPPR Enabled
  // Use Case 2: Algorithm + PPR Disabled
  if ((PprTestType & MRC_PPR_ADV_ALGORITHM_TEST_MASK)               &&
       Inputs->PprRepairType != mPPR                                &&
      !IsTargetedPprRequested && !IsMbist
    ) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PPR memory test enabled run with PPR Repair Type: %s\n", PprRepairTypeStrings[Inputs->PprRepairType]);
    return TRUE;
  } else if ((PprTestType & MRC_PPR_ADV_ALGORITHM_TEST_MASK)) {
    // Debug messages for host when multiple confiting options are enabled
    if (Inputs->PprRepairType == mPPR) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PPR memory test cannot be run with %s\n", PprRepairTypeStrings[mPPR]);
    }
  }

  // Use Case 3: mBIST + mPPR
  // Use Case 4: mBIST only
  if ( IsMbist                                                                      &&
      (Inputs->PprRepairType == mPPR || Inputs->PprRepairType == NoRepair)          &&
      (PprTestType & MRC_PPR_ADV_ALGORITHM_TEST_MASK) == 0                          &&
      !Inputs->PprErrorInjection && !Inputs->PprForceRepair && !IsTargetedPprRequested) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "mBIST %senabled\n", Inputs->PprRepairType == mPPR ? "with mPPR " : "");
    return TRUE;
  } else if (IsMbist) {
    // Debug messages for host when multiple confiting options are enabled
    if (Inputs->PprRepairType != mPPR && Inputs->PprRepairType != NoRepair) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "mBIST cannot be run with %s\n", PprRepairTypeStrings[Inputs->PprRepairType]);
    } else if (Inputs->PprErrorInjection) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "mBIST cannot be run with Error Injection\n");
    } else if (Inputs->PprForceRepair) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "mBIST cannot be run with Force Repair\n");
    }
  }

  // Use Case 5: Targeted PPR
  if ( IsTargetedPprRequested && IsTestDisabled                          &&
      (Inputs->PprRepairType == hPPR || Inputs->PprRepairType == sPPR)   &&
      !IsMbist
    ) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Targeted PPR enabled\n");
    return TRUE;
  } else if (IsTargetedPprRequested) {
    // Debug messages for host when multiple confiting options are enabled
    if (Inputs->PprRepairType != hPPR && Inputs->PprRepairType != sPPR) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Targeted PPR cannot be run with PPR Repair Type: %s\n", PprRepairTypeStrings[Inputs->PprRepairType]);
    }
  }

  if (IsTargetedPprRequested && IsMbist) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Targeted PPR cannot be run with mBIST\n");
  }

  if ((PprTestType & MRC_PPR_ADV_ALGORITHM_TEST_MASK) && IsTargetedPprRequested) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PPR memory test cannot be run with Targeted PPR\n");
  }

  if ((PprTestType & MRC_PPR_ADV_ALGORITHM_TEST_MASK) && IsMbist) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PPR memory test cannot be run with mBIST\n");
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PPR is disabled\n");

  return FALSE;
}
