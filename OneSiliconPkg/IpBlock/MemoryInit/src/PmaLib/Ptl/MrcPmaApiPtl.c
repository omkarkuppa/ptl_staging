/** @file
  This file contains code related to initializing and configuring the PMA.

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
#include "MrcGeneral.h"
#include "CMrcInterface.h"

/**
  This function determines if the board design limits the POR frequency and updates Outputs->FreqMax as necessary

  @param[in, out] MrcData - All the MRC global data.

  @retval Returns mrcSuccess if the Board Details are supported otherwise mrcFail.
**/
MrcStatus
MrcBoardAndSkuFreqCheck (
  IN OUT MrcParameters *const  MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcBoardInputs  *BoardDetails;
  MrcOutput             *Outputs;
  MrcStatus             Status;
  MrcFrequency          FreqMax;
  BOOLEAN               IsLpddr5;
  BOOLEAN               IsDdr5;
  BOOLEAN               IsFreqLimitedBoard;
  MrcSkuType            SkuType;

  Inputs             = &MrcData->Inputs;
  BoardDetails       = &Inputs->ExtInputs.Ptr->BoardDetails;
  Outputs            = &MrcData->Outputs;
  IsLpddr5           = Outputs->IsLpddr5;
  IsDdr5             = Outputs->IsDdr5;
  FreqMax            = fInvalid;
  Status             = mrcSuccess;
  IsFreqLimitedBoard = (BoardDetails->BoardStackUp == 1);
  SkuType            = Inputs->SkuType;

  if (IsLpddr5) {
    if (IsFreqLimitedBoard) { // Type3
      // FreqMax = (Inputs->IsLp5SpeedLimited) ? f7467 : f8533; // For possible increase of Type3 to 8533
      FreqMax = f7467;
    } else {                  // Type4
      if (Inputs->IsLp5SpeedLimited) {
        FreqMax = f8533;
      }
    }
  }

  if (Inputs->Lpddr5Camm) {
    FreqMax = f7467;
  }

  // PTL-U and WCL are limited to 7467 (LP5) and 6400 (DDR5)
  if (SkuType == MrcSkuTypeU || Inputs->IsDdrphyx64) {
    FreqMax = IsDdr5 ? f6400 : f7467;
  }

  Outputs->FreqMax = MIN (Outputs->FreqMax, FreqMax);
  if (Outputs->FreqMax == 0) {
    Status = mrcFail;
  }

  return Status;
}
