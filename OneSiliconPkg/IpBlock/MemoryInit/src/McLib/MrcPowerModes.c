/** @file
  This module configures the memory controller power modes.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

///
/// Include files
///
#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcMcApi.h"
#include "MrcPowerModes.h"
#include "MrcDebugPrint.h"

/**
  This function configures the power down control register.

  @param[in]  - MrcData - The MRC global data.

  @retval - Nothing
**/
void
MrcPowerDownConfig (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput* Inputs;
  MrcOutput*      Outputs;
  INT64           GetSetVal;
  INT64           GetSetEn;
  INT64           DisCkett;
  INT64           TTIdleCounter;
  UINT32          Controller;
  UINT32          Channel;
  BOOLEAN         UlxUlt;
  BOOLEAN         IsLpddr;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;

  UlxUlt = Inputs->IsMcUlxUlt;
  IsLpddr = Outputs->IsLpddr;
  ExtInputs = Inputs->ExtInputs.Ptr;
  GetSetEn = 1;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        continue;
      }
      GetSetVal = ExtInputs->AllowOppRefBelowWriteThrehold;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccAllowOppRefBelowWrtThreshold, WriteToCache | PrintValue, &GetSetVal);

      GetSetVal = ExtInputs->WriteThreshold;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccWrtThreshold, WriteToCache | PrintValue, &GetSetVal);

      GetSetVal = (UlxUlt) ? PDWN_IDLE_COUNTER_ULX_ULT : PDWN_IDLE_COUNTER_DT_HALO;
      if (ExtInputs->PwdwnIdleCounter != 0) {  // 0 means AUTO
        GetSetVal = ExtInputs->PwdwnIdleCounter;
      }
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPdwnIdleCounterSubch0, WriteCached | PrintValue, &GetSetVal);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPdwnIdleCounterSubch1, WriteCached | PrintValue, &GetSetVal);

      DisCkett = IsLpddr ? ExtInputs->ThrtCkeMinDefeatLpddr : ExtInputs->ThrtCkeMinDefeat;
      TTIdleCounter = IsLpddr ? ExtInputs->ThrtCkeMinTmrLpddr : ExtInputs->ThrtCkeMinTmr;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccDisCkett, WriteToCache | PrintValue, &DisCkett);
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccTTIdleCounter, WriteToCache | PrintValue, &TTIdleCounter);

      switch (ExtInputs->PowerDownMode) {
        case pdmNoPowerDown:
          break;

        case pdmAPD:
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccAPD, WriteToCache | PrintValue, &GetSetEn);
          break;

        case pdmPPDDLLOFF:
        case pdmAuto:
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccPPD, WriteToCache | PrintValue, &GetSetEn);
          break;
      }

      GetSetVal = 0x3FF;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMaxPcit, WriteToCache | PrintValue, &GetSetVal);
      GetSetVal = 0x700;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMaxPdwnIdleCounter, WriteToCache | PrintValue, &GetSetVal);
      GetSetVal = 0x20;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMinPdwnIdleCounter, WriteToCache | PrintValue, &GetSetVal);
    }
  }

  if (IsLpddr && ExtInputs->IsWckIdleExitEnabled && !Inputs->IsDdrIoMbA0) {
    if (Outputs->Frequency >= f8533) {
      GetSetVal = 0;
      MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccPPD, WriteToCache | PrintValue, &GetSetVal);
    }
  }

  MrcFlushRegisterCachedData (MrcData);
}
