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
#include "MrcAmt.h"
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
**/
VOID
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
} // PprDispositionFailRange

/**
  Executes PPR flow on correctable failures in the row failure list

  @param [in]     MrcData             - Pointer to global MRC data.
  @param [in]     PprAmtData          - PPR and AMT data structure
  @param [in]     McChBitMask         - Memory Controller Channel Bit mask to update
  @param [in,out] *RepairDone         - Flag to indicate the repair was done
**/
VOID
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
  MrcInput            *Inputs;
  UINT32              DqMask[DQ_MASK_INDEX_MAX];
  UINT32              NumRowsError;
  UINT32              NumRepairsIssued;
  UINT32              NumRepairsSuccessful;
  UINT8               McChRowTestBitMask;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  MaxChDdr = Outputs->MaxChannels;
  MrcPprSetRefresh (MrcData, FALSE);
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
          if (Inputs->PprRepairType == NoRepair) {
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

                DqMask[0] = FailRangePtr->DqMask[0];
                DqMask[1] = FailRangePtr->DqMask[1];

                // Construct DRAM Mask
                ByteMask = 0;
                for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                  //
                  // Set byte mask bits based on failing DQ bits
                  //
                  if ((DqMask[Byte / 4] >> ((Byte * 8) % 32)) & 0xFF) {
                    ByteMask |= 1 << Byte;
                  }
                }
                //
                // If ByteMask bits are set, then execute a PPR repair flow
                //
                if (ByteMask) {

                  NumRowsError++;

                  RepairStatus = FALSE;

                  McChRowTestBitMask = MrcGetPprChBitMask (MrcData, Controller, Channel);

                  // Run clean-up before PPR execution
                  PprCleanup (MrcData, PprAmtData, McChRowTestBitMask);
                  // Test row
                  RowTestPpr(MrcData, PprAmtData, McChRowTestBitMask, PprBank, FailRangePtr->Addr.Bits.Row);
                  // Disable refresh
                  PprCleanup (MrcData, PprAmtData, McChRowTestBitMask);
                  MrcPprSetRefresh (MrcData, FALSE);

                  // Attempt PPR repair
                  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Calling PPR for Mc%d, Ch%d, Rank: 0x%x, BG: 0x%x, Bank: 0x%x Row: 0x%x, ByteMask = 0x%x\n",
                    Controller, Channel, PprAmtData->Rank, BankInterleaveIndex, Bank, FailRangePtr->Addr.Bits.Row, ByteMask);
                  Status = MrcPostPackageRepair (MrcData, Controller, Channel, PprAmtData->Rank, BankInterleaveIndex, Bank, FailRangePtr->Addr.Bits.Row, ByteMask);
                  ++NumRepairsIssued;
                  FailRangePtr->BankGroupAttempts[BankInterleaveIndex]++;

                  if (Status == mrcSuccess) {
                    RepairStatus = MrcPostPprAnalysis (MrcData, PprAmtData, Controller, Channel, Bank, FailRangePtr->Addr.Bits.Row);
                    NumRepairsSuccessful += RepairStatus;
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
        if (Inputs->PprRepairType == NoRepair) {
          MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "PPR repair flow is disabled via PprRepairType setup option.\n");
          break;
        }
      } // Bank pair
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "DispositionFailRangesWithPprFlow: Rank%d Mc%d Ch%d:\n  Number of rows with errors = %d\n  Number of repairs issued = %d\n  Number of repairs successful = %d\n",
        PprAmtData->Rank, Controller, Channel, NumRowsError, NumRepairsIssued, NumRepairsSuccessful);
      Outputs->PprNumSuccessfulRepairs += (UINT16) NumRepairsSuccessful;
    } // Ch
  } // Controller
} // DispositionFailRangesWithPprFlow

/**
  Checks if a Post Package Repair succeded and updates internal outputs structure based on the results.

  @param[in] MrcData     - Global MRC data structure
  @param[in] PprAmtData  - PPR and AMT data structure
  @param[in] McChBitMask - Memory Controller Channel Bit mask to read results for.
  @param[in] Bank        - Current bank address
  @param[in] Row         - Row address to run test

  @retval True when PPR succeded.
  @retval False when PPR failed.
**/
BOOLEAN
MrcPostPprAnalysis (
  IN MrcParameters  *const          MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         Bank,
  IN UINT32                         Row
  )
{
  MrcOutput *const Outputs = &MrcData->Outputs;
  MrcInput *const Inputs = &MrcData->Inputs;

  UINT8 McChBitMask;
  MrcStatus Status;
  BOOLEAN IsTestSuccessful;

  McChBitMask = MrcGetPprChBitMask (MrcData, Controller, Channel);

  Status = RowTestPpr (MrcData, PprAmtData, McChBitMask, Bank, Row);
  IsTestSuccessful = (Status == mrcSuccess) ? TRUE : FALSE;

  if (!IsTestSuccessful                                                                      &&
      (Outputs->Controller[Controller].Channel[Channel].DimmCount == 1)                      &&
    ((Outputs->IsLpddr5 && (Inputs->PprRepairType == hPPR)                                 ) ||
     (Outputs->IsDdr5   && (Inputs->PprRepairType == hPPR || Inputs->PprRepairType == sPPR)))) {

    MRC_DEBUG_MSG (&MrcData->Outputs.Debug,
                   MSG_LEVEL_ERROR,
                  "MC%d Ch%d: Post PPR analysis indicates that PPR flow didn't fix the issue with the failing row.\n",
                  Controller,
                  Channel);
    Outputs->PprFailingChannelBitMask |= McChBitMask;
    Outputs->PprRepairFails++;
  }

  return IsTestSuccessful;
}

/**
  Retrieves the Post Package Repair (PPR) channel bit mask for a specified controller and channel.

  @param[in]  MrcData     - Pointer to the MRC global data structure
  @param[in]  Controller  - The controller index
  @param[in]  Channel     - The channel index

  @returns The PPR channel bit mask for the specified controller and channel.
**/
UINT8
MrcGetPprChBitMask (
  IN MrcParameters  *const          MrcData,
  IN UINT32                         Controller,
  IN UINT32                         Channel
) {
  MrcOutput *const Outputs = &MrcData->Outputs;
  UINT8 McChBitMask = 0;

  if (Outputs->IsLpddr5) {
    McChBitMask = 0b11 << ((Controller * Outputs->MaxChannels) + 2 * LP_IP_CH (Outputs->IsLpddr, Channel));
  } else if (Outputs->IsDdr5) {
    McChBitMask = 1 << MC_CH_IDX (Controller, Channel, Outputs->MaxChannels);
  }

  return McChBitMask;
}

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
  MrcInput  *Inputs = &MrcData->Inputs;
  return ((RepairDone != 0) && (RetryCount < Inputs->PprRetryLimit));
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
  UINT32            ChanEnMask;
  UINT32            ChanEnMaskSave;

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
  // Error injection does not have a pattern number. Set to invalid value to avoid rerunning initial setup such as SetupIOTest in SetupIOTestRetention
  PprAmtData->PatternNumber = MRC_UINT8_MAX;
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

  // Disable channels not selected for current memory test
  if (Outputs->IsLpddr5) {
    ChanEnMask = 1 << ((Controller * MAX_CHANNEL) + Channel);
  } else {
    ChanEnMask = 1 << ((Controller * MAX_CHANNEL) + (Channel * 2));
  }
  ChanEnMaskSave = AmtSelectChannel (MrcData, ChanEnMask);

  PprAmtData->Bank = (Outputs->IsLpddr) ? (Bank & 0xF) : Bank;  // For LP5, limit injected bank to between 0 and 15
  PprAmtData->ErrInjMask = ErrInjMask16;  // Set error injection mask

  // Reset Some registers for WcMats8
  SetupIOTestRetention (MrcData, PprAmtData, LocalMcChBitMask, 1);
  Status = AmtExecuteRowRangeTest (MrcData, PprAmtData, LocalMcChBitMask, CurrentRow, TestSizeArray);

  PprAmtData->LastWriteErrorInjection = TRUE;
  PprAmtData->ErrInjMask = 0; // Restore error injection mask

  // Restore channel enable
  AmtSelectChannel (MrcData, ChanEnMaskSave);

  return Status;
}

/**
  Wrapper function for the Advanced Memory test algorithm that tracks time and splits read/write patterns.

  @param[in] MrcData        - Global MRC data structure
  @param[in] PprAmtData     - PPR and AMT data structure
  @param[in] CmdPat         - Type of sequence PatWr, PatRd, or PatRdWr
  @param[in] IsUseInvtPat   - Info to indicate whether or not data pattern is inverted by comparing original pattern
  @param[in] Direction      - Sequential address direction AMT_ADDR_DIR_UP, AMT_ADDR_DIR_DOWN
  @param[in] PatternNumber  - Which pattern in the MATS8 sequence is this test

  @retval None
**/
VOID
MrcRunPprDetection (
  IN MrcParameters *const           MrcData,
  IN PPR_AMT_PARAMETER_DATA *const  PprAmtData,
  IN UINT8                          CmdPat,
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
**/
VOID
AdvMemTestPauseDataRet (
  IN MrcParameters* const MrcData
)
{
  UINT32 DelayTimeNs;

  // If CTE, shorten wait time
  DelayTimeNs = 32000000;  // 32ms is 1x refresh interval for DDR5 and LP5

  // Ensure all pages closed and rows are refreshed
  MrcWait (MrcData, DelayTimeNs);

  // Disable refresh commands
  MrcPprSetRefresh (MrcData, FALSE);

  // Pause for given test delay
  MrcWait (MrcData, DelayTimeNs);

  // Enable refresh commands
  MrcPprSetRefresh (MrcData, TRUE);

  // Ensure all rows are refreshed
  MrcWait (MrcData, DelayTimeNs);

  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "AdvMemTestPauseDataRet = %d ms\n", DelayTimeNs / 1000000);
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
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                Test;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT32               TotalTestTimeMATS8 = 0;
  UINT32               Controller;
  UINT32               Channel;

  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;

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
  Controller = Outputs->FirstPopController;
  Channel = Outputs->Controller[Controller].FirstPopCh;
  for (Test = 0; Test < PPR_TEST_DETECTION_LOOPS; Test++) {
    PprAmtData->TestNumber = Test;
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);

    RepairDone = 1;
    RetryCount = 0;

    do {
      AmtSetDataPattern (MrcData, PprAmtData);
      PprAmtData->UiShl = 1;  // Indicate a bit-shift of 1 for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0; // Indicates if we should invert Data pattern for SubSequence 0
      PprAmtData->SubSeqDataInv[1] = 1; // Indicates if we should invert Data pattern for SubSequence 1 (only applicable during PatRdWr tests, with 2 SubSequence commands)
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;
      //1. Write sliding data pattern to all of memory
      MrcRunPprDetection(MrcData, PprAmtData, PatWr, FALSE, AMT_ADDR_DIR_UP, 1);

      if (Inputs->PprErrorInjection != 0) {
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
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, FALSE, AMT_ADDR_DIR_UP, 2);
      } else {
        // Modified step: Use down direction to pressure tWR on upper BG address
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, FALSE, AMT_ADDR_DIR_DOWN, 2);
      }

      //3. Read and compare inverse pattern
      //   Write original pattern
      if (InvertedPassEn == TRUE) {
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, TRUE, AMT_ADDR_DIR_DOWN, 3);
      } else {
        // Modified step: Just read and compare the inverse pattern without writing the original pattern
        MrcRunPprDetection(MrcData, PprAmtData, PatRd, TRUE, AMT_ADDR_DIR_DOWN, 3);
      }

      // Make another MATS+ pass with inverted data
      if (InvertedPassEn == TRUE) {
        //4. Write inverse pattern to all of memory
        MrcRunPprDetection(MrcData, PprAmtData, PatWr, TRUE, AMT_ADDR_DIR_UP, 4);

        //5. Read and compare inverse pattern
        //   Write original pattern
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, TRUE, AMT_ADDR_DIR_UP, 5);

        //6. Read and compare original pattern
        //   Write inverse pattern
        MrcRunPprDetection(MrcData, PprAmtData, PatRdWr, FALSE, AMT_ADDR_DIR_DOWN, 6);
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
    } while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++));

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
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                Test;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT32               TotalTestTimeDataRet = 0;
  UINT32               Controller;
  UINT32               Channel;

  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start MemTestDataRetention\n");
  for (Test = 0; Test < 2; Test++) {
    PprAmtData->TestNumber = Test;
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);

    RepairDone = 1;
    RetryCount = 0;

    do {
      AmtSetDataPattern (MrcData, PprAmtData);
      PprAmtData->UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0;
      PprAmtData->SubSeqDataInv[1] = 0;
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;

      //1. Write data pattern to all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, FALSE, AMT_ADDR_DIR_UP, 1);

      if (PprAmtData->TestStatus == mrcSuccess) {
        AdvMemTestPauseDataRet (MrcData);
      }

      if (Inputs->PprErrorInjection != 0) {
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
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, FALSE, AMT_ADDR_DIR_UP, 2);

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
    } while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++));
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
  MrcOutput            *Outputs;
  MrcInput             *Inputs;
  UINT8                RepairDone;
  UINT8                RetryCount;
  UINT32               TotalTestTimeMarch = 0;
  const CHAR8          *TestName;
  UINT32               Controller;
  UINT32               Channel;
  UINT8                Test;


  PprAmtData->TestStatus = mrcSuccess;
  Outputs         = &MrcData->Outputs;
  Inputs          = &MrcData->Inputs;
  Controller      = Outputs->FirstPopController;
  Channel         = Outputs->Controller[Controller].FirstPopCh;

  switch (PprAmtData->TestType) {
    case PprTestTypeXMarch:
      TestName = "MemTestXMarchAlt";
      break;
    case PprTestTypeYMarchShort:
      TestName = "MemTestYMarchShortAlt";
      break;
    case PprTestTypeYMarchLong:
      TestName = "MemTestYMarchLongAlt";
      break;
    default:
      MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "MemTestMarchAlt: Unsupported test type %d\n", PprAmtData->TestType);
      return mrcFail;
  }

  MRC_DEBUG_MSG(&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Start %s\n", TestName);

  for (Test = 0; (Test < 8) && (PprAmtData->TestStatus == mrcSuccess); Test++) {
    PprAmtData->TestNumber = Test;
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);
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
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, FALSE, AMT_ADDR_DIR_UP, 1);

      if (Inputs->PprErrorInjection != 0) {
        if (Test == 0 && RetryCount == 1) {
          PprAmtData->Rank = 0;
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 5, 1, 1, 0x0F);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 4, 4, 1, 0x0C);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 27, 0x7, 1, 0x01);
          InjectMemtestError(MrcData, PprAmtData, Controller, Channel, 22, 0x7, 1, 0x02);
        }
      }

      //2. Execute read data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, FALSE, AMT_ADDR_DIR_UP, 2);

      //3. Execute inverse write data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatWr, TRUE, AMT_ADDR_DIR_UP, 3);

      //4. Execute inverse read data pattern over all of memory
      MrcRunPprDetection (MrcData, PprAmtData, PatRd, TRUE, AMT_ADDR_DIR_UP, 4);

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
    } while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++));
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
    PprAmtData->TestNumber = Test;
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Begin Test %d\n", Test);

    RepairDone = 1;
    RetryCount = 0;

    do {
      AmtSetDataPattern (MrcData, PprAmtData);
      PprAmtData->UiShl = 0;  // Do not bit-shift for all UIs of the Data Pattern
      PprAmtData->SubSeqDataInv[0] = 0;
      PprAmtData->SubSeqDataInv[1] = 1;
      PprAmtData->NumCL = 1;  // Use 1 cacheline transaction per algorithm instruction
      PprAmtData->CurTestTime = 0;

      //1. Write data pattern to all of memory
      MrcRunPprDetection(MrcData, PprAmtData, PatWr, FALSE, AMT_ADDR_DIR_UP, 1);

      if (Inputs->PprErrorInjection != 0) {
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
      MrcRunPprDetection(MrcData, PprAmtData, PatRd, FALSE, AMT_ADDR_DIR_UP, 2);

      //3. Write inverse pattern
      MrcRunPprDetection(MrcData, PprAmtData, PatWr, TRUE, AMT_ADDR_DIR_UP, 3);

      //4. Read and compare inverse pattern
      MrcRunPprDetection(MrcData, PprAmtData, PatRd, TRUE, AMT_ADDR_DIR_UP, 4);

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
    } while (IsAmtRetryRequiredAfterRepair (MrcData, RepairDone, RetryCount++));
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
