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
#ifndef _MRC_LPDDR5_MODE_REGISTER_HANDLER_H_
#define _MRC_LPDDR5_MODE_REGISTER_HANDLER_H_

#include "CMrcTypes.h"
#include "MrcChipApi.h"

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
MRC_IRAM0_FUNCTION
MrcStatus
MrcLpddr5GetCmdType  (
  IN  MrcParameters *const MrcData,
  IN MrcModeRegister       CurMrAddr,
   OUT GmfCmdType          *CmdTypeOut
  );

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
MRC_IRAM0_FUNCTION
MrcStatus
MrcLpddr5IssueMrcommand (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank,
  IN  MrcModeRegister      CurMrAddr,
  IN  GmfCmdType           MrCmd
  );

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
MRC_IRAM0_FUNCTION
MrcStatus
MrcLpddr5GetMrCommandDelay (
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      CurMrAddr,
  IN  MrcModeRegister      NextMrAddr,
  IN  GmfCmdType           CurCmdType,
  IN  GmfCmdType           NextCmdType,
  OUT UINT16               *MrWaitNs
  );

#endif // _MRC_LPDDR5_MODE_REGISTER_HANDLER_H_
