/** @file
  Implementation of LPDDR5 Specific Settings.

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
  LPDDR5 JEDEC Spec
**/

#include "MrcLpddr5.h"  // to check declarations against implementations here (public API)
#include "MrcLpddr5Settings.h"
#include "MrcHalRegisterAccess.h"
#include "CMrcApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdrIoUtils.h"
// AUTO-GENERATED LP5 TABLES START
// AUTO-GENERATED CODE
// Lp5 Parameters Table
#define MICRON_MANUFACTURE_ID                      (0xFF)
#define SAMSUNG_MANUFACTURE_ID                     (0x1)
#define SKHYNIX_MANUFACTURE_ID                     (0x6)

// Dfeq/ PdDrvStr/ SocOdt/ PreEmpDn/ PreEmpUp/ WckDcaWr/ WckDcaRd/ RttWr/ RttCa/ RttNT/
const NnFlexLpddr5Params NnFlexInitialSettingsLpddr5[] = {
  {   4,  60,  40,   2,   3,   1,  -2, 120, 240,   0 },
  {   4,  60,  40,   2,   3,   1,  -2, 120, 240,   0 },
  {   2,  40,  40,   2,   1,  -2,  -2,  80, 240,   0 },
  {   7,  48,  40,   3,   3,  -3,   2,  80, 240,   0 },
  {   2,  40,  40,   2,   1,  -2,  -2,  80, 240,   0 },
  {   7,  48,  40,   3,   3,  -3,   2,  80, 240,   0 },
};

const static DramPartMap NnFlexDramPartMap[MaxNnFlexDramPart] = {
  {DeviceDefault_key,        DramTypeDefault},
  {DeviceDefault1_key,       DramTypeDefault1},
  {Samsung_2R_8533_1_key,    Samsung_2R_8533_1},
  {Micron_2R_8533_1_key,     Micron_2R_8533_1},
  {Samsung_2R_8533_0_key,    Samsung_2R_8533_0},
  {Micron_2R_8533_0_key,     Micron_2R_8533_0},
};

typedef union {
  struct {
    UINT32 Vendor           :  3; // Bits 0:2   - Default = 0, Micron = 1, Samsung = 2, Hynix = 3
    UINT32 Rank             :  2; // Bits 3:4   -  0 = 1R, 1 = 2R
    UINT32 Freq             : 14; // Bits 5:18  - 2133 < Freq < 11000
    UINT32 Board            :  1; // Bit  19    - 0 = Non Typical Board (Type3), 1 = Typical Board (Type4)
    UINT32 ModulePartNumber :  4; // Bits 20:23 - Module Part Number
  } Bits;
  UINT32 Data;
} DRAM_TYPE_STRUCT;
// AUTO-GENERATED LP5 TABLES END

// LPDDR5 ODT Values
// ODT values are in this order: { RttWr, RttWck, RttCa, RttCa2RByteMode, RttCs, RttCs2RByteMode, RttNT}
// ODT tables will only be modified in Green MRC (JEDEC reset) during the cold boot.
TOdtValueLpddr Lpddr5OdtTableType3[MAX_DIMMS_IN_CHANNEL][MAX_ODT_VALUE] = {
/// 1DPC 1R,                    1DPC 2R
  {{120, 60, 240, 240, 80, 0, 0},  {120, 60, 240, 240, 80, 120, 0}},
};

TOdtValueLpddr Lpddr5OdtTableType4[MAX_DIMMS_IN_CHANNEL][MAX_ODT_VALUE] = {
/// 1DPC 1R,                    1DPC 2R
  {{120, 60, 240, 240, 80, 0, 0},  {120, 48, 240, 240, 80, 120, 0}},
};

/**
  This function selects the ODT table according to the board type.

  @param[in] MrcData  - Include all the MRC general data.
  @param[in] Dimm     - selected DIMM.
  @param[in] OdtIndex - selected ODT index.

  @return TOdtValueLpddr * - Pointer to the relevant table or NULL if the table was not found.
**/
TOdtValueLpddr *
SelectTableLpddr5 (
  IN MrcParameters *const MrcData,
  IN const UINT32         Dimm,
  IN const TOdtIndex      OdtIndex
  )
{
  const MrcInput            *Inputs;
  const MrcBoardInputs      *BoardDetails;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcDebug                  *Debug;
  MrcOutput                 *Outputs;
  BOOLEAN                   IsTypicalBoard;
  TOdtValueLpddr            *OdtTable;

  Inputs          = &MrcData->Inputs;
  ExtInputs       = Inputs->ExtInputs.Ptr;
  BoardDetails    = &ExtInputs->BoardDetails;
  IsTypicalBoard  = BoardDetails->BoardStackUp == 0;
  Outputs         = &MrcData->Outputs;
  Debug           = &Outputs->Debug;
  OdtTable        = NULL;

  if ((OdtIndex >= 2) || (Dimm > 0)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Error: Lpddr5OdtTable array out of bounds! OdtIndex: %u, Dimm: %u\n", OdtIndex, Dimm);
    return NULL;
  }
  if (IsTypicalBoard) {
    OdtTable = (TOdtValueLpddr*) &Lpddr5OdtTableType4[Dimm][OdtIndex];
  } else {
    OdtTable = (TOdtValueLpddr*) &Lpddr5OdtTableType3[Dimm][OdtIndex];
  }

  if (ExtInputs->FlexibleAnalogSettings) {
    // Nn Flex RttWr/ RttCa/ RttNT settings for Lpddr5 - All Cards can have same default values
    OdtTable->RttWr = (ExtInputs->LpddrRttWr != 0) ? ExtInputs->LpddrRttWr : NnFlexInitialSettingsLpddr5[DramTypeDefault].RttWr;
    OdtTable->RttCa = (ExtInputs->LpddrRttCa != 0) ? ExtInputs->LpddrRttCa : NnFlexInitialSettingsLpddr5[DramTypeDefault].RttCa;
    OdtTable->RttNT = IS_NNFLEX_DRAM_VAR_EN (NnFlexMaskLpddr5RttNT) ? (UINT16) LpddrOdtDecode ((UINT16) ExtInputs->NnFlexLpddr5RttNT) : NnFlexInitialSettingsLpddr5[DramTypeDefault].RttNT;
  }

  return OdtTable;
}

/**
  This function returns the DFE Setting for MR24.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] Dfeql     - DFE Quantity for Lower Byte.
  @param[out] Dfequ     - DFE Quantity for Upper Byte.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcLp5GetDFE (
  IN  MrcParameters  *MrcData,
  OUT UINT8          *Dfeql,
  OUT UINT8          *Dfequ
  )
{
  const MrcInput      *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT8                DfeqVal;

  Inputs    = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  // DFE setting range is from -0.052V to -0.01V in step size of 7mV. Tap=1: -0.052V, Tap=7: -0.01V
  if ((Dfeql != NULL) && (Dfequ != NULL)) {
    if (ExtInputs->FlexibleAnalogSettings) { // Nn Flex initial settings for Lpddr5
      DfeqVal = IS_NNFLEX_DRAM_VAR_EN (NnFlexMaskLpddr5Dfeq) ?  (UINT8) ExtInputs->NnFlexLpddr5Dfeq : NnFlexInitialSettingsLpddr5[DramTypeDefault].Dfeq; // NN "Per Device" logic
      *Dfeql = DfeqVal;
      *Dfequ = DfeqVal;
    } else {
      *Dfeql = 4;
      *Dfequ = 4;
    }
  }
  return mrcSuccess;
}

/**
  Return the initial LPDDR5 DQ Vref (MR14/MR15)

  @param[in]  MrcData      - Pointer to global MRC data

  @retval Initial DQ Vref in [mV]
**/
UINT16
MrcLp5GetVrefDq (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput *Outputs;
  UINT32    DqVrefMv;

  Outputs = &MrcData->Outputs;

  // Based on design input we don't want it to be a function of RttWr and WrDS
  // Making it a function of SoC Vddq instead, plus additional offset for better initial centering

  DqVrefMv = Outputs->VccddqVoltage / 4;
  DqVrefMv += ((Outputs->Frequency == f9600) ? 45 : (Outputs->Frequency >= f6400) ? 60 : 30);

  if (Outputs->IsDvfscEnabled) {
    // DQ ODT is disabled when in E-DVFSC mode, and we need a higher DqVref
    DqVrefMv += 100;
  }

  return (UINT16) DqVrefMv;
}

/**
  This function map between pre-defined Vendor,Freq,Rank,BoardType combination formatted kay
  to a table entry containing configuration values defined for that specific combination

  @param[in] DramKey - pre-defined Vendor,Freq,Rank,BoardType combination formatted kay

  @returns NnFlexDramPart - Table entry containing configuration values defined per dimm specific combination.
**/
NnFlexDramPart
GetNnFlexDramPart (
  IN UINT32 DramKey
)
{
  UINT8 Index;
  for (Index = 0; Index < MaxNnFlexDramPart; ++Index) {
    if (DramKey == NnFlexDramPartMap[Index].Key) {
      return NnFlexDramPartMap[Index].DevIndex;
    }
  }
  return DramTypeDefault;
}

/**
  This function returns DRAM type for NN Flex per device usage.

  @param[in] MrcData - Pointer to global MRC data.

  @returns NnFlexDramPart - Table entry containing configuration values defined per dimm specific combination.
**/
NnFlexDramPart
Lpddr5NnFlexGetDramType (
  IN MrcParameters* MrcData
)
{
  MrcInput*       Inputs;
  MrcOutput*      Outputs;
  MrcDebug*       Debug;
  MrcBoardInputs* BoardDetails;

  BOOLEAN     IsTypicalBoard;
  UINT32      FirstController;
  UINT32      FirstChannel;
  UINT16      Manufacture;
  UINT8       MrrResult[MRC_MRR_ARRAY_SIZE];

  NnFlexDramPart   DramType;
  DRAM_TYPE_STRUCT DramTypeStruct;
  LPDDR5_MODE_REGISTER_5_TYPE Mr5;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  BoardDetails    = &Inputs->ExtInputs.Ptr->BoardDetails;
  IsTypicalBoard  = BoardDetails->BoardStackUp == 0;
  FirstController = Outputs->FirstPopController;
  FirstChannel    = Outputs->Controller[FirstController].FirstPopCh;

  if (Outputs->Frequency < f8533) {
    return DramTypeDefault;
  }

  // Read Manufacture id
  MrcIssueMrr (MrcData, FirstController, FirstChannel, dDIMM0, mrMR5, MrrResult);
  Mr5.Data8   = MrrResult[0];
  Manufacture = Mr5.Data8;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MR5: %d\n", Manufacture);

  // compress/map ModulePartNumber into small enum,per limited relevant cases
  switch (Manufacture) {
    case MICRON_MANUFACTURE_ID:
      Manufacture = 1;
      break;
    case SAMSUNG_MANUFACTURE_ID:
      Manufacture = 2;
      break;
    case SKHYNIX_MANUFACTURE_ID:
      Manufacture = 3;
      break;
    default:
      Manufacture = 0;
  }

  DramTypeStruct.Bits.Vendor = (Manufacture & 0x7);
  DramTypeStruct.Bits.Rank   = Outputs->IsAny2Ranks;
  DramTypeStruct.Bits.Freq   = Outputs->Frequency;
  DramTypeStruct.Bits.Board  = IsTypicalBoard;
  DramTypeStruct.Bits.ModulePartNumber = 0; // No ModulePartNumber for LP

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DramTypeStruct.Bits.Vendor: %d\n", DramTypeStruct.Bits.Vendor);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DramTypeStruct.Bits.Rank: %d\n", DramTypeStruct.Bits.Rank);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DramTypeStruct.Bits.Freq: %d\n", DramTypeStruct.Bits.Freq);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DramTypeStruct.Bits.Board: %d\n", DramTypeStruct.Bits.Board);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DramTypeStruct.Bits.ModulePartNumber: %d\n", DramTypeStruct.Bits.ModulePartNumber);

  DramType = GetNnFlexDramPart (DramTypeStruct.Data);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DramType: %x\n", DramType);

  return DramType;
}