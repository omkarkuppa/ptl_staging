/** @file
  This file contains code related to initializing and configuring the Rcomp

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

#include "MrcCommon.h"
#include "MrcMemoryApi.h"
#include "MrcDdrIoUtils.h"
#include "MrcDdrCommon.h"

// RCOMP target values for { RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk } - per DDR Type
const UINT16 RcompTargetLpddr5Type3[MAX_RCOMP_TARGETS]    = { 40, 27, 30, 30, 30 };
const UINT16 RcompTargetLpddr5Type4_1R[MAX_RCOMP_TARGETS] = { 40, 27, 30, 30, 25 };
const UINT16 RcompTargetLpddr5Type4_2R[MAX_RCOMP_TARGETS] = { 40, 27, 30, 30, 30 };

const UINT16 RcompTargetPDdr5[MAX_RCOMP_TARGETS]          = { 60, 27, 25, 25, 25 };

/**
  This function determines default RcompTargets based on CPU type and DDR Type

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Profile - MemoryProfile

  @retval Returns pointer to RcompTarget
**/
const UINT16 *
MrcGetDefaultRcompTarget (
  IN OUT MrcParameters *const MrcData,
  IN     MrcProfile Profile
  )
{
  const MrcInput        *Inputs;
  const MrcBoardInputs  *BoardDetails;
  MrcOutput             *Outputs;
  MrcDebug              *Debug;
  BOOLEAN               IsTypicalBoard;
  const UINT16          *RcompTarget;

  Inputs          = &MrcData->Inputs;
  BoardDetails    = &Inputs->ExtInputs.Ptr->BoardDetails;
  IsTypicalBoard  = BoardDetails->BoardStackUp == 0;
  Outputs         = &MrcData->Outputs;
  Debug           = &Outputs->Debug;
  RcompTarget     = NULL;

  /*
   * Use DdrType as index to find the default RcompTarget
   */
  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      if (IsTypicalBoard) {
        RcompTarget = (Outputs->MaxRanks == 2) ? RcompTargetLpddr5Type4_2R : RcompTargetLpddr5Type4_1R;
      } else {
        RcompTarget = RcompTargetLpddr5Type3;
      }
      break;

    case MRC_DDR_TYPE_DDR5:
      RcompTarget = RcompTargetPDdr5;
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s: %d\n", gErrString, gUnsupportedTechnology, Outputs->DdrType);
      break;
  }

  return RcompTarget;
}
