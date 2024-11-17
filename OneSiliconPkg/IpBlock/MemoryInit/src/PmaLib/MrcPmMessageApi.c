/** @file
  This file contains code related to initializing and configuring the PMA.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include "CMrcApi.h"
#include "MrcHalRegisterAccess.h"
#include "MrcPmaApi.h"
#include "MrcHalApi.h"

/**
  This function sends PM messages to PHY.

  @param[in] MrcData - Include all MRC global data.
  @param[in] PmMsgCode - PM13 or PM14 or PM0.

  @retval mrcSuccess PM14 message sending is successful
  @retval mrcDeviceBusy if poll InitComplete hits timeout
**/
MrcStatus
MrcPhyPmMessage (
  IN MrcParameters* const MrcData,
  IN UINT32               PmMsgCode
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcInput            *Inputs;
  MrcDebug            *Debug;
  UINT64              Timeout;
  INT64               GetSetVal;
  BOOLEAN             Busy;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &MrcData->Outputs.Debug;
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout

  // Issue PM message to PHY by directly writing to DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG
  GetSetVal = PmMsgCode;
  MrcGetSetNoScope (MrcData, GsmMiscPmMessage, WriteNoCache, &GetSetVal);
  GetSetVal = 1;
  MrcGetSetNoScope (MrcData, GsmMiscPmMessageRunBusy, WriteNoCache, &GetSetVal);

  // Poll on Run-Busy bit until it gets cleared by the hardware.
  // Hardware acknowledges that it received PM message when the run-busy bit is cleared.
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PHY PM%u: Waiting for ACK\n", PmMsgCode);
  do {
    Busy = 0;
    MrcGetSetNoScope (MrcData, GsmMiscPmMessageRunBusy, ReadNoCache, &GetSetVal);
    Busy = (GetSetVal == 1);
    // When PM is issued using PHY, Simics does not clear the RunBusy bit.
    if (Inputs->ExtInputs.Ptr->SimicsFlag == 1) {
      Busy = FALSE;
    }
  } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

  if (Busy) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s - %s\n", gErrString, gTimeout);
    return mrcDeviceBusy;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Done\n");
  return mrcSuccess;
}
