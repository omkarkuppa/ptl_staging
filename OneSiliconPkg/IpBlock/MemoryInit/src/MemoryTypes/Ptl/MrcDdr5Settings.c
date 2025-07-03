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
  "none",                   // Card_default
  "M435R1GB4PB1-CCPSG",     // Card_240C
  "MTC4C1016ZS1VC72BCZKFF", // Card_246C
  "M435RZGB4PB1-CCPRC",     // Card_269C
  "MTC4C1016ZS1VC72BCZKFF", // Card_279C
  "MTC16C208XS1VC64BBZKCF", // Card_207G
  "M435R4GA3PB0-CCPDC",     // Card_241G
  "HMCGY8AKBVB318N",        // Card_256B
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
    case Card_207G:
      if (NumOfRanks == 2 && Frequency <= 6400) return Card_207G_2R_6400;
      break;
    case Card_240C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_240C_1R_6400;
      break;
    case Card_241G:
      if (NumOfRanks == 2 && Frequency <= 6400) return Card_241G_2R_6400;
      break;
    case Card_246C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_246C_1R_6400;
      if (NumOfRanks == 1 && Frequency <= 7200) return Card_246C_1R_7200;
      break;
    case Card_256B:
      if (NumOfRanks == 2 && Frequency <= 6400) return Card_256B_2R_6400;
      if (NumOfRanks == 2 && Frequency <= 7200) return Card_256B_2R_7200;
      break;
    case Card_269C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_269C_1R_6400;
      break;
    case Card_279C:
      if (NumOfRanks == 1 && Frequency <= 6400) return Card_279C_1R_6400;
      if (NumOfRanks == 1 && Frequency <= 7200) return Card_279C_1R_7200;
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

// ddr5_dfe_tap1/ ddr5_dfe_tap2/ ddr5_rtt_wr/ ddr5_rtt_nom_wr/ ddr5_rtt_park_rx/ ddr5_ron_up/ ddr5_ron_dn/
const NnFlexDdr5Params NnFlexInitialSettingsDdr5[] = {
  { -23, -5, 120, 60, 60, 40, 48 },
  { -23, -5, 120, 60, 60, 40, 48 },
  { -12, -2, 80, 60, 80, 40, 40 },
  { -12, -2, 80, 60, 80, 40, 40 },
  { -32, -5, 80, 60, 60, 34, 40 },
  { -23, -5, 80, 60, 60, 34, 40 },
  { -23, -5, 80, 60, 60, 34, 40 },
  { -21, 0, 120, 60, 60, 40, 48 },
  { -33, -15, 120, 60, 60, 34, 40 },
  { -33, -8, 80, 60, 60, 40, 40 },
  { -33, -8, 120, 60, 60, 48, 40 },
  { -40, 1, 120, 48, 34, 48, 40 },
  { -25, -5, 120, 80, 80, 40, 40 },
  { -25, -5, 120, 80, 48, 34, 34 },
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
GetCardEnumFromPartNumber (
  IN const char* ModulePartNumber
)
{
  BOOLEAN IsPartNumberFound;
  char  PartNumberSuffix;
  int   PartNumberIdx = 0;
  UINT8 Card;

  if (ModulePartNumber == NULL) {
    return Card_default;
  }

  for (Card = 0; Card < Card_Max; Card++) {
    IsPartNumberFound = TRUE;
    PartNumberIdx = 0;
    PartNumberSuffix = ModulePartNumber[0];

    while (ModulePartNumber[PartNumberIdx] != '\0' && CardPartNumber[Card][PartNumberIdx] != '\0') {
      if (ModulePartNumber[PartNumberIdx] != CardPartNumber[Card][PartNumberIdx]) { // break loop for non matching case.
        break;
      }
      PartNumberSuffix = ModulePartNumber[++PartNumberIdx];
    }

    if (!MRC_ISSPACE(PartNumberSuffix)) { // not a space char. both part numbers are different.
      IsPartNumberFound = FALSE;
    }
    else {
      PartNumberSuffix = '\0';
    }

    if (IsPartNumberFound && PartNumberSuffix == CardPartNumber[Card][PartNumberIdx]) {
      return (CardEnum)Card; // return matching index
    }
  }
  return Card_default;
}

