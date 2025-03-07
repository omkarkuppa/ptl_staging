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

// LPDDR5 ODT Values
// ODT values are in this order: { RttWr, RttWck, RttCa, RttCa2RByteMode, RttCs, RttCs2RByteMode, RttNT}
const TOdtValueLpddr Lpddr5OdtTableType3[MAX_DIMMS_IN_CHANNEL][MAX_ODT_VALUE] = {
/// 1DPC 1R,                    1DPC 2R
  {{60, 60, 240, 240, 80, 0, 0},  {48, 60, 240, 240, 80, 120, 0}},
};

const TOdtValueLpddr Lpddr5OdtTableType4[MAX_DIMMS_IN_CHANNEL][MAX_ODT_VALUE] = {
/// 1DPC 1R,                    1DPC 2R
  {{48, 60, 240, 240, 80, 0, 0},  {60, 48, 240, 240, 80, 120, 0}},
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
  const MrcInput        *Inputs;
  const MrcBoardInputs  *BoardDetails;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  BOOLEAN               IsTypicalBoard;
  TOdtValueLpddr        *OdtTable;

  Inputs          = &MrcData->Inputs;
  BoardDetails    = &Inputs->ExtInputs.Ptr->BoardDetails;
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
  // EV Feedback - All configurations should have same value
  if ((Dfeql != NULL) && (Dfequ != NULL)) {
    *Dfeql = 4;
    *Dfequ = 4;
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
  DqVrefMv += ((Outputs->Frequency == f9600) ? 5 : (Outputs->Frequency >= f6400) ? 30 : 10);

  return (UINT16) DqVrefMv;
}
