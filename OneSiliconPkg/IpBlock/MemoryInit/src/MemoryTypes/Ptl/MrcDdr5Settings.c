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
  {{ -8, -2, 0, 0 }}, // CHANNEL 0
  {{ -8, -2, 0, 0 }}, // CHANNEL 1
};

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
  @param[in] Rank            - selected Rank.

  @return Pointer to the relevant table or NULL if the table was not found.
**/
TDFEValueDdr5 *
SelectDfeTableDdr5 (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Rank
  )
{
  UINT32        Dimm;
  TDFEValueDdr5 *TDFEValueDdr5Index;

  Dimm = RANK_TO_DIMM_NUMBER (Rank);

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
