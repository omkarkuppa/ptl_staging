/** @file
  Implementation of DDR5 Specific Settings.

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
  DDR5 JEDEC Spec
**/

#include "MrcDdr5.h"  // to check declarations against implementations here (public API)
#include "MrcDdr5Settings.h"
#include "MrcHalRegisterAccess.h"
#include "CMrcApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdrIoUtils.h"

///
/// Initialization Timing Parameters
///

//--------------- DDR5 -----------------------------------------
// DDR5 DQ ODT values are in this order: { RttWr, RTT_NOM_RD, RTT_NOM_WR, RttPark, RttParkDqs }
// Note: RttWr   0xFFFF denotes Hi-Z
//               0      denotes disabled
//
//       RttPark, RttNom may be disabled by setting 0, there is no Hi-Z value.
// 1DPC: RttDqs = RttWr
// 1DPC 2Rank: Write NT ODT = RTT_NOM_WR, Read NT ODT = RttPark (RttPark is based on frequency and is set properly during MR34 programming)

// 1DPC
const TOdtValueDqDdr5 Ddr5DqOdtTable1Dpc[MAX_DIMMS_IN_CHANNEL][MAX_DDR5_1SPC_ODT] = {
  /// 1DPC 1R,             1DPC 2R
  {{ 80, 60, 60, 60, 80 }, { 80, 60, 60, 60, 80 }}, // DIMM 0
};

// DDR5 CCC ODT values are in this order { RttCa, RttCs, RttCk }
// 1DPC
const TOdtValueCccDdr5 Ddr5CccOdtTable1Dpc[MAX_DIMMS_IN_CHANNEL][MAX_DDR5_1SPC_ODT][MAX_DDR5_ODT_STRAP] = {
  //  1DPC 1R,                      1DPC 2R
  //  Group A,    Group B           Group A,    Group B
  {{{ 480, 480, 0 },{ 40, 40, 80 }}, {{ 480, 480, 0 },{ 80, 40, 80 }}}, // DIMM 0
};

// DDR5 Dimm DFE values in this order { Tap1, Tap2, Tap3, Tap4 }
const TDFEValueDdr5 Ddr5DFETable[MAX_DDR5_CHANNEL][MAX_DIMMS_IN_CHANNEL] = {
  //DIMM 0
  {{ -12, -2, 0, 0 }}, // CHANNEL 0
  {{ -12, -2, 0, 0 }}, // CHANNEL 1
};

// AUTO-GENERATED DDR5 TABLES START
// AUTO-GENERATED CODE
const char* CardPartNumber[Card_Max] = {
  "HMCG66AHBVA312N",         // Card_230C
  "HMCG78AHBVA312N",         // Card_235A
  "M435R1GB4PB1-CCPSG",      // Card_240C
  "HMCGY8AKBVB318N",         // Card_256B
  "M435RZGB4PB1-CCPRC",      // Card_269C
  "MTC4C1016ZS1VC72BCZKFF",  // Card_276C
  "MTC4C10163S1VC72BHZ KFF", // Card_298C
  "M435R8JA3MB1-CJRLC",      // Card_311B
  "HMCG88AGBSA092N",         // Card_75B
  "none",                    // Card_default
};

#ifdef MRC_DEBUG_PRINT
const char* Ddr5OptParamNames[DDR5_NUM_OF_OPTS] = {
  "DDR5_DFE_TAP1",
  "DDR5_DFE_TAP2",
  "DDR5_RTT_WR",
  "DDR5_RTT_NOM_WR",
  "DDR5_RTT_PARK_RX",
  "DDR5_RON_UP",
  "DDR5_RON_DN",
};

const char* Ddr5CardToName[Card_NotFound] = {
  "240C_1R_6400",
  "256B_2R_6400",
  "256B_2R_7200",
  "269C_1R_6400",
  "276C_1R_6400",
  "276C_1R_7200",
  "298C_1R_6400",
  "298C_1R_7200",
  "311B_2R_7200",
  "311B_2R_6400",
  "230C_1R_6400",
  "235A_1R_6400",
  "75B_2R_6400",
  "DEFAULT_1R_6400",
  "DEFAULT_1R_7200",
  "DEFAULT_2R_6400",
  "DEFAULT_2R_7200",
};
#endif

/**
  This function returns index NN Flex csv according to to Dimm/Rank/Freq.
  @param[in] Frequency   - Frequency config.
  @param[in] NumOfRanks  - Rank config - 1/2.
  @param[in] Card        - Dimm module card id.
  @returns corresponding card row index in NN Flex csv to struct table. default if not exists.
**/
Ddr5ParamIndex
GetDdr5ParamIndex (
  IN MrcFrequency Frequency,
  IN UINT32 NumOfRanks,
  IN CardEnum Card
)
{
  switch (Card) {
    case Card_230C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_230C_1R_6400;
      break;
    case Card_235A:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_235A_1R_6400;
      break;
    case Card_240C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_240C_1R_6400;
      break;
    case Card_256B:
      if (NumOfRanks == 2 && Frequency <= 6400) return Card_256B_2R_6400;
      if (NumOfRanks == 2 && Frequency <= 7200) return Card_256B_2R_7200;
      break;
    case Card_269C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_269C_1R_6400;
      break;
    case Card_276C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_276C_1R_6400;
      if (NumOfRanks == 1 && Frequency <= 7200) return Card_276C_1R_7200;
      break;
    case Card_298C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_298C_1R_6400;
      if (NumOfRanks == 1 && Frequency <= 7200) return Card_298C_1R_7200;
      break;
    case Card_311B:
      if (NumOfRanks == 2 && Frequency <= 6400) return Card_311B_2R_6400;
      if (NumOfRanks == 2 && Frequency <= 7200) return Card_311B_2R_7200;
      break;
    case Card_75B:
      if (NumOfRanks == 2 && Frequency <= 6400) return Card_75B_2R_6400;
      break;
    default:
      if (NumOfRanks == 1 && Frequency <= 6400) {
        return Card_default_1R_6400;
      } else if (NumOfRanks == 1 && Frequency > 6400) {
        return Card_default_1R_7200;
      } else if (NumOfRanks == 2 && Frequency <= 6400) {
        return Card_default_2R_6400;
      } else if (NumOfRanks == 2 && Frequency > 6400) {
        return Card_default_2R_7200;
      }
      break;
  }
  return Card_NotFound;
}

// Ddr5 Parameters Table

// DfeTap1/ DfeTap2/ RttWr/ RttNomWr/ RttNomRd/ RonUp/ RonDn/
const NnFlexDdr5Params NnFlexInitialSettingsDdr5[] = {
  { -32,  -5,  80,  60,  60,  34,  40 },
  { -25,  -5, 120,  80,  80,  40,  40 },
  { -25,  -5, 120,  80,  48,  34,  34 },
  { -21,   0, 120,  60,  60,  40,  48 },
  { -33, -15, 120,  60,  60,  34,  40 },
  { -33,  -8,  80,  60,  60,  40,  40 },
  { -35,   5,  80,  60,  60,  48,  48 },
  { -35,   5,  80,  60,  60,  48,  48 },
  { -23,  -5, 120,  80,  80,  40,  40 },
  { -23,  -5, 120,  80,  80,  40,  40 },
  { -13,   0, 120,  60,  60,  40,  48 },
  { -13,   0, 120,  60,  60,  40,  48 },
  { -23,  -5, 120,  48,  80,  40,  40 },
  { -23,  -5, 120,  60,  60,  40,  48 },
  { -23,  -5, 120,  60,  60,  40,  48 },
  { -23,  -5, 120,  48,  80,  40,  40 },
  { -23,  -5, 120,  48,  80,  40,  40 },
};
// AUTO-GENERATED DDR5 TABLES END

/**
  This function selects the CCC ODT table according to DIMM/Rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Controller      - selected Controller.
  @param[in] Channel         - selected Channel.
  @param[in] Dimm            - selected Dimm.

  @retval TOdtValueCccDdr5 * - Pointer to the relevant table or NULL if the table was not found.
**/
TOdtValueCccDdr5 *
SelectCccTableDdr5 (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm
  )
{
  MrcOutput           *Outputs;
  MrcChannelOut       *ChannelOut;
  TOdtIndex           OdtIndex;
  TOdtValueCccDdr5    *OdtTable;
  MrcDimmOut          *DimmOut;
  UINT8               RanksInDimm0;

  Outputs      = &MrcData->Outputs;
  ChannelOut   = &Outputs->Controller[Controller].Channel[Channel];
  DimmOut      = &ChannelOut->Dimm[dDIMM0];
  RanksInDimm0 = DimmOut[dDIMM0].RankInDimm;

  OdtIndex = (RanksInDimm0 == 2) ? oi1DPC2R : oi1DPC1R;

  OdtTable = (TOdtValueCccDdr5 *) &Ddr5CccOdtTable1Dpc[Dimm][OdtIndex][0];

  return OdtTable;
}

/**
  This function selects the DFE table according to DIMM/Rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Profile         - Memory Profile.
  @param[in] Controller      - selected Controller.
  @param[in] Channel         - selected Channel.
  @param[in] Dimm            - selected Dimm.

  @return Pointer to the relevant table or NULL if the table was not found.
**/
TDFEValueDdr5 *
SelectDfeTableDdr5 (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm
  )
{
  TDFEValueDdr5 *TDFEValueDdr5Index;

  TDFEValueDdr5Index = (TDFEValueDdr5*) &Ddr5DFETable[Channel][Dimm];

  return TDFEValueDdr5Index;
}

/**
  This function selects the DQ ODT table according to DIMM/rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Profile         - Memory Profile.
  @param[in] Dimm            - selected DIMM.
  @param[in] OdtIndex        - selected ODT index.

  @retval TOdtValueDqDdr5 * - Pointer to the relevant table or NULL if the table was not found.
**/
TOdtValueDqDdr5 *
SelectTableDdr5 (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Dimm,
  IN const TOdtIndex      OdtIndex
  )
{
  MrcOutput *Outputs;
  TOdtValueDqDdr5  *OdtTable;

  Outputs = &MrcData->Outputs;

  OdtTable = NULL;
  if ((OdtIndex > oi1DPC2R) || (Dimm >= MAX_DIMMS_IN_CHANNEL)) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "%s SelectTableDdr5: array out of bounds! OdtIndex: %u, Dimm: %u\n", gErrString, OdtIndex, Dimm);
    return NULL;
  }

  OdtTable = (TOdtValueDqDdr5*) &Ddr5DqOdtTable1Dpc[Dimm][OdtIndex];

  return OdtTable;
}

/**
  This function returns index in ODT table according to to DIMM/rank population.

  @param[in] MrcData         - Include all the MRC general data.
  @param[in] Controller      - selected Controller.
  @param[in] Channel         - selected Channel.

  @retval TOdtIndex - Index in ODT table according to to DIMM/rank population.
**/
TOdtIndex
GetOdtIndexDdr5 (
  IN MrcParameters* const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  MrcOutput           *Outputs;
  MrcChannelOut       *ChannelOut;
  MrcDimmOut          *DimmOut;
  TOdtIndex           OdtIndex;
  UINT8               RanksInDimm0;

  Outputs      = &MrcData->Outputs;
  ChannelOut   = &Outputs->Controller[Controller].Channel[Channel];
  DimmOut      = &ChannelOut->Dimm[dDIMM0];
  RanksInDimm0 = DimmOut[dDIMM0].RankInDimm;

  OdtIndex = (RanksInDimm0 == 2) ? oi1DPC2R : oi1DPC1R;
  return OdtIndex;
}

/**
  Return the initial DDR5 DQ Vref (MR10)

  @param[in]  MrcData      - Pointer to global MRC data
  @param[in]  Controller   - Controller to work on
  @param[in]  Channel      - Channel to work on
  @param[out] VrefDqCalVal - DQ Vref in MR10 encoding

  @retval mrcSuccess              if VrefDqCalVal is not NULL
  @retval mrcWrongInputParameter  if VrefDqCalVal is NULL
**/
MrcStatus
MrcDdr5GetVrefDqCalibrationValue (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  OUT DDR5_MR10_VREF *const VrefDqCalVal
  )
{
  MrcOutput       *Outputs;
  DDR5_MR10_VREF  VrefVal;

  Outputs = &MrcData->Outputs;

  if (VrefDqCalVal == NULL) {
    return mrcWrongInputParameter;
  }

  // Assuming mobile 1DPC
  if (Outputs->Controller[Controller].Channel[Channel].ValidRankBitMask > 1) {
    VrefVal = (Outputs->Frequency > f6400) ? Ddr5Vref_76p5 : Ddr5Vref_75p0; // 2R
  } else {
    VrefVal = (Outputs->Frequency > f6400) ? Ddr5Vref_70p0 : Ddr5Vref_68p0; // 1R
  }
  *VrefDqCalVal = VrefVal;
  return mrcSuccess;
}

/**
  This function is used to get the corresponding card for a given dram part number.

  @param[in]  ModulePartNumber - Dram module part number from SPD.

  @returns - The corresponding card index.
**/
CardEnum
Ddr5GetCardEnumFromPartNumber (
  IN const CHAR8* ModulePartNumber
)
{
  BOOLEAN  IsPartNumberFound;
  UINT16   PartNumberIdx;
  UINT8    Card;
  CHAR8    PartNumberNoSpaces[SPD5_MODULE_PART_NUMBER_SIZE + 1]; // +1 for null terminator
  UINT16   PartNumberNoSpacesIdx;

  if (ModulePartNumber == NULL) {
    return Card_default;
  }

  // Remove whitespaces from ModulePartNumber, because
  // according to spec unsed digits are coded as ASCII blanks (0x20).
  for (PartNumberIdx = 0, PartNumberNoSpacesIdx = 0; ModulePartNumber[PartNumberIdx] && PartNumberIdx < SPD5_MODULE_PART_NUMBER_SIZE; PartNumberIdx++) {
    if (!MRC_ISSPACE(ModulePartNumber[PartNumberIdx])) {
      PartNumberNoSpaces[PartNumberNoSpacesIdx++] = ModulePartNumber[PartNumberIdx];
    }
  }
  PartNumberNoSpaces[PartNumberNoSpacesIdx] = '\0';

  // Find matching serial number in CardPartNumber
  IsPartNumberFound = FALSE;
  for (Card = 0; Card < Card_Max && IsPartNumberFound == FALSE; Card++) {

    PartNumberIdx = 0;
    while (PartNumberNoSpaces[PartNumberIdx] && (PartNumberNoSpaces[PartNumberIdx] == CardPartNumber[Card][PartNumberIdx])) {
      PartNumberIdx++;
    }

    if (PartNumberNoSpaces[PartNumberIdx] == CardPartNumber[Card][PartNumberIdx]) {
      IsPartNumberFound = TRUE;
      break;
    }
  }

  if (IsPartNumberFound) {
    return (CardEnum) Card;
  }

  return Card_default;
}

