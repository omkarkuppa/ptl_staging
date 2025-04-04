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
#ifndef _MRC_COMMON_MODE_REGISTER_HANDLER_H_
#define _MRC_COMMON_MODE_REGISTER_HANDLER_H_

#include "CMrcTypes.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"

/**
  This function executes the MR Command using DRAM Command Register.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Sequence   - Pointer to an array of MRs that will be programmed.

  @retval mrcSuccess    - MR Command was sent successfully
  @retval mrcFail       - Invalid MR Command Parameter.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcMrSequencer (
  IN MrcParameters* const MrcData,
  IN const MrcModeRegister* const Sequence
  );

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
MRC_IRAM0_FUNCTION
MrcStatus
MrcGetDramCommandMap (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  MrDramCmdType         MrCmd,
  IN  BOOLEAN               IsMultiCast,
  OUT UINT32                *DramCmdData,
  IN OUT BOOLEAN            *IsSingleCycleCmd
  );

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
MRC_IRAM0_FUNCTION
MrcStatus
MrcIssueMrCommand (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  MrcModeRegister       Address,
  IN  GmfCmdType            MrCmd
  );

#endif // _MRC_COMMON_MODE_REGISTER_HANDLER_H_
