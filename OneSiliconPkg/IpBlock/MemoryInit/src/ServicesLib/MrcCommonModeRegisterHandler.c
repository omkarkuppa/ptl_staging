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
#include "MrcMemoryApi.h"
#include "MrcCommonModeRegisterHandler.h"
#include "MrcDdr5ModeRegisterHandler.h"
#include "MrcLpddr5ModeRegisterHandler.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"

/**
  This function issues the apporiate MR command based on MrCmd Input.
  The data driven by the MR command is based on the data within the MrData host structure

  This function assumes the inputs Controller, Channel, and Rank are all valid instances

   @param[in] MrcData    - Pointer to global MRC data.
   @param[in] Controller - Current Controller
   @param[in] Channel    - Current Channel
   @param[in] Rank       - Current Rank
   @param[in] CurMrAddr  - Current MR Address to issue command too
   @param[in] MrCmd      - Current MR Cmd type (MRW, MPC, Vref, ect) to issue

   @retval mrcSuccess Dram Command successfully sent
   @retval mrcFail Dram Command not successfully sent or DdrType not supported
**/
MrcStatus
MrcIssueMrCommand (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  MrcModeRegister       Address,
  IN  GmfCmdType            MrCmd
  )
{

   switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
        return MrcDdr5IssueMrCommand (MrcData, Controller, Channel, Rank, Address, MrCmd);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      return MrcLpddr5IssueMrcommand (MrcData, Controller, Channel, Rank, Address, MrCmd);
      break;

    default:
      return mrcFail;
      break;
  }
}

/**
  This function determines the correct delay based on JEDEC Spec that should be applied
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
   @param[out] MrWait     - Delay in Ns to wait after sending MR command

   @retval mrcSuccess Dram Command Delay successfully determined
   @retval mrcFail Dram Command Delay not successfully determined
**/
MrcStatus
MrcGetMrCommandDelay (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  MrcModeRegister       CurMrAddr,
  IN  MrcModeRegister       NextMrAddr,
  IN  GmfCmdType            CurCmdType,
  IN  GmfCmdType            NextCmdType,
  OUT UINT16                *MrWait
  )
{

   switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
        return MrcDdr5GetMrCommandDelay (MrcData, Controller, Channel, Rank, CurMrAddr, NextMrAddr, CurCmdType, NextCmdType, MrWait);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      return MrcLpddr5GetMrCommandDelay (MrcData, CurMrAddr, NextMrAddr, CurCmdType, NextCmdType, MrWait);
      break;

    default:
      return mrcFail;
      break;
  }
}

/**
  This function gets the MR command that is assoicated with the given MR address.

  This function assumes the inputs Controller, Channel, and Rank are all valid instances

   @param[in] MrcData      - Pointer to global MRC data.
   @param[in] Controller   - Current Controller
   @param[in] Channel      - Current Channel
   @param[in] Rank         - Current Rank
   @param[in] CurMrAddr    - Current MR Address to detect command for
   @param[out] CmdTypeOut  - MR Command based on MR Address (MRW, MPC, VREF)

   @retval mrcSuccess Dram Command type successfully determined
   @retval mrcFail Dram Command type not successfully determined
**/
MrcStatus
MrcGetJedecMrCmdType (
  IN MrcParameters* const MrcData,
  IN  UINT32              Controller,
  IN  UINT32              Channel,
  IN  UINT32              Rank,
  IN MrcModeRegister      CurMrAddr,
  OUT GmfCmdType          *CmdTypeOut
  )
{

   switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
        return MrcDdr5GetCmdType (MrcData, Controller, Channel, Rank, CurMrAddr, CmdTypeOut);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      return MrcLpddr5GetCmdType (MrcData, CurMrAddr, CmdTypeOut);
      break;

    default:
      return mrcFail;
      break;
  }
}

/**
  This function compiles the DRAM command needed to preform the specifc MR Cmd requested,
  with the given Data and address information

  @param[in] MrcData              - Include all MRC global data.
  @param[in] Address              - MRR / MRW address
  @param[in] Data                 - MRW Data or MPC Opcode
  @param[in] MRCmd                - Cmd Type: MRR / MRW / MPC / NOP
  @param[in] IsMulticast          - True if Dram cmd is to be written as multicast
  @param[out] DramCmdData         - Compiled Dram command value
  @param[in out] IsSingleCycleCmd - True if MPC sended as a single cycle command

  @retval mrcSuccess Dram Command successfully
  @retval mrcFail MrCmd not supported or DdrType not supported
**/
MrcStatus
MrcGetDramCommandMap (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  MrDramCmdType         MrCmd,
  IN  BOOLEAN               IsMultiCast,
  OUT UINT32                *DramCmdData,
  IN OUT BOOLEAN            *IsSingleCycleCmd
  )
{
  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
        return MrcDdr5GetDramCommandMap (MrcData, Address, Data, MrCmd, IsMultiCast, DramCmdData);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      return MrcLpddr5GetDramCommandMap (MrcData, Address, Data, MrCmd, IsMultiCast, DramCmdData, IsSingleCycleCmd);
      break;

    default:
      return mrcFail;
      break;
  }
}

/**
  This function executes the MR Command using DRAM Command Register.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Sequence   - Pointer to an array of MRs that will be programmed.

  @retval mrcSuccess    - MR Command was sent successfully
  @retval mrcFail       - Invalid MR Command Parameter.
**/
MrcStatus
MrcMrSequencer (
  IN MrcParameters* const MrcData,
  IN const MrcModeRegister* const Sequence
  )
{
  MrcStatus     Status;
  MrcOutput     *Outputs;
  UINT32        Channel;
  UINT32        Controller;
  UINT32        Rank;
  UINT32        Index;
  UINT16        MrWait;
  GmfCmdType    CurCmdType;
  GmfCmdType    NextCmdType;
  MrcModeRegister  CurMrAddr;
  MrcModeRegister  NextMrAddr;

  Outputs    = &MrcData->Outputs;
  Status     = mrcSuccess;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        for (Index = 0; Sequence[Index] != mrEndOfSequence; Index++) {
          CurMrAddr = Sequence[Index];

          // Get Command
          Status = MrcGetJedecMrCmdType (MrcData, Controller, Channel, Rank, CurMrAddr, &CurCmdType);
          if (Status != mrcSuccess) {
            return Status;
          }
          // GmfCmdIndexMax signals that the MR should be skipped
          if (CurCmdType == GmfCmdIndexMax) {
            continue;
          }

          // Send Command, Data sent is within MrData Host Struct
          Status = MrcIssueMrCommand (MrcData, Controller, Channel, Rank, CurMrAddr, CurCmdType);
          if (Status != mrcSuccess) {
            return Status;
          }

          if (MrcData->Inputs.IsApplyMrCommandDelays) {
            NextMrAddr = Sequence[Index+1];
            if (NextMrAddr != mrEndOfSequence) {
              MrcMrAddrToIndex (MrcData, &NextMrAddr);
              Status = MrcGetJedecMrCmdType (MrcData, Controller, Channel, Rank, NextMrAddr, &NextCmdType);
              if (Status != mrcSuccess) {
                return Status;
              }
            } else {
              NextCmdType = GmfCmdIndexMax;
            }
            // Wait
            Status = MrcGetMrCommandDelay (MrcData, Controller, Channel, Rank, CurMrAddr, NextMrAddr, CurCmdType, NextCmdType, &MrWait);

            MrcWait (MrcData, MrWait);
          }
        }
      }
    }
  }
  return Status;
}



