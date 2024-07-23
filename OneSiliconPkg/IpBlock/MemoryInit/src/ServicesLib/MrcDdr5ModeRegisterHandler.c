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
#include "MrcDebugPrint.h"
#include "MrcDdr5.h"
#include "MrcDdr5ModeRegisterHandler.h"

/**
  This function gets the DDR5 MR command that is assoicated with the given MR address.
  This function will return a CmdTypeOut of GmfCmdIndexMax if an MR should not be accessed due
  to system configuration (ex. x8 Device). It is up to the caller to approperiatly handle
  these cases

  This function assumes the inputs Controller, Channel, and Rank are all valid instances

   @param[in] MrcData      - Pointer to global MRC data.
   @param[in] Controller   - Current Controller
   @param[in] Channel      - Current Channel
   @param[in] Rank         - Current Rank
   @param[in] CurMrAddr    - Current MR Address to detect command for
   @param[out] CmdTypeOut  - MR Command based on MR Address (MRW, MPC, VREF)

   @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcDdr5GetCmdType (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN MrcModeRegister       CurMrAddr,
  OUT GmfCmdType           *CmdTypeOut
  )
{
  MrcChannelOut *ChannelOut;
  UINT32        Dimm;
  BOOLEAN       Isx8DramWidth;

  ChannelOut = &MrcData->Outputs.Controller[Controller].Channel[Channel];
  Dimm = RANK_TO_DIMM_NUMBER (Rank);
  Isx8DramWidth = (ChannelOut->Dimm[Dimm].SdramWidth == 8) ? TRUE : FALSE;

  if (Isx8DramWidth && ((CurMrAddr >= mrMR193) && (CurMrAddr <= mrMR252))) {
    *CmdTypeOut = GmfCmdIndexMax; // Skip MR in Sequence
  } else if ((!MrcData->Inputs.ExtInputs.Ptr->IsDdr5MR7WicaSupported) && (CurMrAddr == mrMR7)) {
    // Skip when DRAM Device does not support MR7
    *CmdTypeOut = GmfCmdIndexMax; // Skip MR in Sequence
  } else {
    return MrcDdr5GetGmfAttributes (MrcData, CurMrAddr, CmdTypeOut, NULL);
  }

  return mrcSuccess;
}

/**
  This function issues the apporiate MR command based on MrCmd Input. If the CurMrAddr is to be issued
  with PDA, this function will handle enumerating the PDA IDs and issuing unique data for each device.
  The data driven by the MR command is based on the data within the MrData host structure

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
MrcDdr5IssueMrCommand (
  IN  MrcParameters *const MrcData,
  IN  UINT32              Controller,
  IN  UINT32              Channel,
  IN  UINT32              Rank,
  IN  MrcModeRegister     CurMrAddr,
  IN  GmfCmdType          MrCmd
   )
{
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  MrcStatus     Status;
  MrcChannelOut *ChannelOut;
  MrcDimmOut    *DimmOut;
  UINT32        DimmIdx;
  UINT32        DimmRank;
  UINT32        MrIndex;
  UINT8         MrPdaData[MAX_PDA_MR_IN_CHANNEL];
  UINT8         NumDevices;
  UINT8         DeviceIdx;
  UINT8         *MrPtr;
  BOOLEAN       IsPdaMr;

  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  Status     = mrcSuccess;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  DimmIdx    = RANK_TO_DIMM_NUMBER(Rank);
  DimmOut    = &ChannelOut->Dimm[DimmIdx];
  DimmRank   = Rank % MAX_RANK_IN_DIMM;
  MrPtr      = ChannelOut->Dimm[DimmIdx].Rank[DimmRank].MR;

  MrIndex = MrcMrAddrToIndex (MrcData, &CurMrAddr);

  if (MrIndex >=  MAX_MR_IN_DIMM) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MR index(%d) exceeded MR array length(%d)\n", MrIndex, MAX_MR_IN_DIMM);
    Status = mrcWrongInputParameter;
    return Status;
  }

  MrPdaData[0] = MrPtr[MrIndex];
  NumDevices = 1;

  IsPdaMr = MrcMrIsPda (MrcData, Controller, Channel, CurMrAddr);

  if (IsPdaMr) {
    Status |= MrcDdr5GetPdaData (MrcData, Controller, Channel, Rank, CurMrAddr, MrPdaData);
    // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
    NumDevices = DimmOut->PrimaryBusWidth / DimmOut->SdramWidth;
    if (DimmOut->EccSupport) {
      NumDevices += 1;
    }
  }

  for (DeviceIdx = 0; DeviceIdx < NumDevices; DeviceIdx++) {
    if (IsPdaMr) {
      // A sequence which has PDA MRs, will only go back to writing to all devices, once mpcSelectAllPDA is added to the sequence.
      Status |= MrcPdaSelect (MrcData, Controller, Channel, Rank, DeviceIdx, MRC_PRINTS_OFF);
    }
    switch (MrCmd) {
      case GmfCmdMrw:
        Status |= MrcIssueMrw (MrcData, Controller, Channel, Rank, CurMrAddr, MrPdaData[DeviceIdx], MRC_PRINTS_OFF);
        break;
      case GmfCmdMpc:
         Status |= MrcIssueMpc (MrcData, Controller, Channel, Rank, MrPdaData[DeviceIdx], MRC_PRINTS_OFF);
        break;
      case GmfCmdVref:
        if (CurMrAddr == mrMR11) {
          Status |= MrcIssueVrefCmd (MrcData, Controller, Channel, Rank, DDR5_VREFCA (MrPdaData[DeviceIdx]), MRC_PRINTS_OFF);
        } else if (CurMrAddr == mrMR12) {
          Status |= MrcIssueVrefCmd (MrcData, Controller, Channel, Rank, DDR5_VREFCS (MrPdaData[DeviceIdx]), MRC_PRINTS_OFF);
        }  else {
          return mrcFail;
        }
        break;
      default:
        return mrcFail;
        break;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, (!Outputs->JedecInitDone) ? "MC%d.C%d.R%d.D%d MR%d: 0x%X\n" : "", Controller, Channel, Rank, DeviceIdx, CurMrAddr, MrPdaData[DeviceIdx]);
  } // for DeviceIdx

  if (IsPdaMr) {
    Status |= MrcPdaSelect (MrcData, Controller, Channel, Rank, 0xF, MRC_PRINTS_OFF);
  }
  return Status;
}

/**
  This function determines the correct delay based on DDR5 JEDEC Spec that should be applied
  following a specific MR command. This function takes into account the current MR cmd and the
  next MR cmd to ensure the optimal delay value is used. The NextMrAddr and NextCmdType are optional
  inputs which do not need to be used for standalone MR commands.

  This function assumes the inputs Controller, Channel, and Rank are all valid instances

   @param[in] MrcData    - Pointer to global MRC data.
   @param[in] Controller - Current Controller
   @param[in] Channel    - Current Channel
   @param[in] Rank       - Current Rank
   @param[in] CurMrAddr  - Current MR Address to detect delay for
   @param[in] NextMrAddr  - [Optional] Next MR address to be sent following current MR Cmd
   @param[in] CurCmdType  - Current MR Cmd type (MRW, MPC, Vref, ect) to detect delay for
   @param[in] NextCmdType - [Optional] Next MR Cmd type (MRW, MPC, Vref, ect) to be sent following current MR Cmd
   @param[out] MrWaitNs   - Delay in Ns to wait after sending MR command

   @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcDdr5GetMrCommandDelay (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  MrcModeRegister      CurMrAddr,
  IN  MrcModeRegister      NextMrAddr, OPTIONAL
  IN  GmfCmdType           CurCmdType,
  IN  GmfCmdType           NextCmdType, OPTIONAL
  OUT UINT16               *MrWaitNs
   )
{
  MrcOutput          *Outputs;
  MrcDebug           *Debug;
  MrcStatus         Status;
  MrcChannelOut     *ChannelOut;
  GmfDdr5DelayType  DelayType;
  UINT32            Dimm;
  BOOLEAN           Isx8DramWidth;
  UINT16            MrWaitNck;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status        = mrcSuccess;
  ChannelOut    = &MrcData->Outputs.Controller[Controller].Channel[Channel];
  Dimm          = RANK_TO_DIMM_NUMBER (Rank);
  Isx8DramWidth = (ChannelOut->Dimm[Dimm].SdramWidth == 8) ? TRUE : FALSE;

  // Based on MR, detect the default delay type
  Status = MrcDdr5GetGmfAttributes (MrcData, CurMrAddr, NULL, &DelayType);

  // Adjust Delay type based on MR Sequence
  if (CurCmdType == GmfCmdMrw) {
    Status = MrcDdr5MrwGetTiming (MrcData, CurMrAddr, NextMrAddr, NextCmdType, Isx8DramWidth, &DelayType);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  // Convert delay type in to actual Ns value
  Status = Ddr5GmfDelayTimings (MrcData, (GmfTimingIndex) DelayType, &MrWaitNck);
  *MrWaitNs = DIVIDECEIL (MrWaitNck * Outputs->tCKps, 1000);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, (!Outputs->JedecInitDone) ? "Apply Delay %d:  %dnS\n" : "", DelayType, *MrWaitNs);

  return Status;
}

/**
  This function returns through MrPdaData the unique data per device for MRs that are
  accessed through PDA

  This function assumes the inputs Controller, Channel, and Rank are all valid instances

   @param[in] MrcData    - Pointer to global MRC data.
   @param[in] Controller - Current Controller
   @param[in] Channel    - Current Channel
   @param[in] Rank       - Current Rank
   @param[in] CurMrAddr  - Current PDA MR Address
   @param[out] MrPdaData - Array to hold PDA Data per device

   @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcDdr5GetPdaData (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  MrcModeRegister      CurMrAddr,
  IN  UINT8                MrPdaData[MAX_PDA_MR_IN_CHANNEL]
  )
{
  MrcChannelOut *ChannelOut;
  MrcDimmOut    *DimmOut;
  MrcRankOut    *RankOut;
  UINT32        RankMod2;
  UINT32        DimmIdx;
  UINT8         DeviceIdx;
  UINT8         NumDevices;

  ChannelOut = &MrcData->Outputs.Controller[Controller].Channel[Channel];
  DimmIdx = RANK_TO_DIMM_NUMBER(Rank);
  RankMod2 = Rank % MAX_RANK_IN_DIMM;
  DimmOut = &ChannelOut->Dimm[DimmIdx];
  RankOut = &DimmOut->Rank[RankMod2];

  // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
  NumDevices = DimmOut->PrimaryBusWidth / DimmOut->SdramWidth;
  if (DimmOut->EccSupport) {
    NumDevices += 1;
  }

  // Get the data for all ranks and devices
  for (DeviceIdx = 0; DeviceIdx < NumDevices; DeviceIdx++) {
      switch (CurMrAddr) {
        case mrMR3:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr3[DeviceIdx];
          break;
        case mrMR7:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr7[DeviceIdx];
          break;
        case mrMR10:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->DdrPdaVrefDq[DeviceIdx];
          break;
        case mrMR11:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->DdrPdaVrefCmd[DeviceIdx];
          break;
        case mrMR48:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr48[DeviceIdx];
          break;
        case mrMR129: // DFE PDA Tap1 DQL[0]
        case mrMR137:
        case mrMR145:
        case mrMR153:
        case mrMR161:
        case mrMR169:
        case mrMR177:
        case mrMR185:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr129[DeviceIdx];
          break;
        case mrMR130: // DFE PDA Tap2 DQL[0]
        case mrMR138:
        case mrMR146:
        case mrMR154:
        case mrMR162:
        case mrMR170:
        case mrMR178:
        case mrMR186:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr130[DeviceIdx];
          break;
        case mrMR131: // DFE PDA Tap3 DQL[0]
        case mrMR139:
        case mrMR147:
        case mrMR155:
        case mrMR163:
        case mrMR171:
        case mrMR179:
        case mrMR187:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr131[DeviceIdx];
          break;
        case mrMR132: // DFE PDA Tap4 DQL[0]
        case mrMR140:
        case mrMR148:
        case mrMR156:
        case mrMR164:
        case mrMR172:
        case mrMR180:
        case mrMR188:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr132[DeviceIdx];
          break;
        case mrMR193: // DFE PDA Tap1 DQU[0]
        case mrMR201:
        case mrMR209:
        case mrMR217:
        case mrMR225:
        case mrMR233:
        case mrMR241:
        case mrMR249:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr193[DeviceIdx];
          break;
        case mrMR194: // DFE PDA Tap2 DQU[0]
        case mrMR202:
        case mrMR210:
        case mrMR218:
        case mrMR226:
        case mrMR234:
        case mrMR242:
        case mrMR250:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr194[DeviceIdx];
          break;
        case mrMR195: // DFE PDA Tap3 DQU[0]
        case mrMR203:
        case mrMR211:
        case mrMR219:
        case mrMR227:
        case mrMR235:
        case mrMR243:
        case mrMR251:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr195[DeviceIdx];
          break;
        case mrMR196: // DFE PDA Tap4 DQU[0]
        case mrMR204:
        case mrMR212:
        case mrMR220:
        case mrMR228:
        case mrMR236:
        case mrMR244:
        case mrMR252:
          MrPdaData[DeviceIdx] = (UINT8)RankOut->Ddr5PdaMr196[DeviceIdx];
          break;
        default:
          return mrcFail;
          break;
      }
  } // for DeviceIdx
  return mrcSuccess;
}
