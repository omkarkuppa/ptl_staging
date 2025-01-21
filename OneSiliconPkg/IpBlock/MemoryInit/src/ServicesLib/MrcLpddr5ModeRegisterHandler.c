/** @file
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

#include "CMrcTypes.h"
#include "MrcChipApi.h"
#include "MrcLpddr5ModeRegisterHandler.h"
#include "MrcLpddr5.h"
#include "MrcDebugPrint.h"
#include "MrcCommon.h"


/**
  This function gets the LPDDR5 MR command that is assoicated with the given MR address.
  This function will return a CmdTypeOut of GmfCmdIndexMax if an MR should not be accessed due
  to system configuration (ex. ByteMode). It is up to the caller to approperiatly handle
  these cases. This function currently only supports MRWs but can be built up to support other
  MR commands in the future

   @param[in] MrcData     - Pointer to global MRC data.
   @param[in] CurMrAddr   - Current MR Address to detect command for
   @param[out] CmdTypeOut - MR Command based on MR Address (MRW, GmfCmdIndexMax)

   @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcLpddr5GetCmdType  (
  IN  MrcParameters *const MrcData,
  IN MrcModeRegister       CurMrAddr,
   OUT GmfCmdType          *CmdTypeOut
  )
{
  // Currently only MRWs are used for sequencer.
  if ((CurMrAddr == mrMR12b) && (!MrcData->Outputs.LpByteMode)) {
    *CmdTypeOut = GmfCmdIndexMax; // Skip MR in Sequence
  } else {
    *CmdTypeOut = GmfCmdMrw;
  }
  return mrcSuccess;
}

/**
  This function issues the apporiate MR command based on MrCmd Input.
  The data driven by the MR command is based on the data within the MrData host structure
  This function currently only supports MRWs but can be built up to support other MR commands in
  the future

  This function assumes the inputs Controller, Channel, and Rank are all valid instances

   @param[in] MrcData    - Pointer to global MRC data.
   @param[in] Controller - Current Controller
   @param[in] Channel    - Current Channel
   @param[in] Rank       - Current Rank
   @param[in] CurMrAddr  - Current MR Address to issue command too
   @param[in] MrCmd      - Current MR Cmd type (MRW, MPC, Vref, ect) to issue

   @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcLpddr5IssueMrcommand (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  MrcModeRegister      CurMrAddr,
  IN  GmfCmdType           MrCmd
  )
{
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  MrcStatus     Status;
  MrcChannelOut *ChannelOut;
  UINT32        Dimm;
  UINT32        DimmRank;
  UINT32        MrIndex;
  UINT8         Data;
  UINT8         *MrPtr;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status  = mrcSuccess;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  Dimm = RANK_TO_DIMM_NUMBER (Rank);
  DimmRank = Rank % MAX_RANK_IN_DIMM;
  MrPtr = ChannelOut->Dimm[Dimm].Rank[DimmRank].MR;
  MrIndex = MrcMrAddrToIndex (MrcData, &CurMrAddr);

  if (MrIndex >=  MAX_MR_IN_DIMM) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MR index(%d) exceeded MR array length(%d)\n", MrIndex, MAX_MR_IN_DIMM);
    Status = mrcWrongInputParameter;
    return Status;
  }

  Data = MrPtr[MrIndex];

  switch (MrCmd) {
    case GmfCmdMrw:
      Status |= MrcIssueMrw (MrcData, Controller, Channel, Rank, CurMrAddr, Data, MRC_PRINTS_OFF);
      break;
    default:
      return mrcFail;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, (!Outputs->JedecInitDone) ? "MC%d.C%d.R%d MR%d: 0x%X\n" : "", Controller, Channel, Rank, CurMrAddr, Data);
  return Status;
}

/**
  This function determines the correct delay based on LPDDR5 JEDEC Spec that should be applied
  following a specific MR command. This function takes into account the current MR cmd and the
  next MR cmd to ensure the optimal delay value is used. The NextMrAddr and NextCmdType are optional
  inputs which do not need to be used for standalone MR commands.

  This function assumes the inputs Controller, Channel, and Rank are all valid instances

   @param[in] MrcData     - Pointer to global MRC data.
   @param[in] CurMrAddr   - Current MR Address to detect delay for
   @param[in] NextMrAddr  - [Optional] Next MR address to be sent following current MR Cmd
   @param[in] CurCmdType  - Current MR Cmd type (MRW, MPC, ect) to detect delay for
   @param[in] NextCmdType - [Optional] Next MR Cmd type (MRW, MPC, ect) to be sent following current MR Cmd
   @param[out] MrWaitNs   - Delay in Ns to wait after sending MR command

   @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcLpddr5GetMrCommandDelay (
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      CurMrAddr,
  IN  MrcModeRegister      NextMrAddr,
  IN  GmfCmdType           CurCmdType,
  IN  GmfCmdType           NextCmdType,
  OUT UINT16               *MrWaitNs
  )
{
  MrcOutput          *Outputs;
  MrcDebug           *Debug;
  MrcStatus          Status;
  GmfLpddr5DelayType DelayType;
  UINT16             MrWaitNck;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status  = mrcSuccess;

  Lpddr5GmfDelayType (MrcData, CurMrAddr, &DelayType, (NextMrAddr == mrEndOfSequence));

  Status = Lpddr5GmfDelayTimings (MrcData, (GmfTimingIndex) DelayType, &MrWaitNck);
  *MrWaitNs = DIVIDECEIL (MrWaitNck * Outputs->tCKps, 1000);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, (!Outputs->JedecInitDone) ? "Apply Delay %d:  %dnS\n" : "", DelayType, *MrWaitNs);
  return Status;
}
