/** @file
  Private functions supporting MrcModeRegisterHandler.c code

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
**/
#ifndef MRC_MODE_REGISTER_HANDLER_H_
#define MRC_MODE_REGISTER_HANDLER_H_

#include "CMrcInterface.h" // for MrcParameters

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcModeRegisterHandlerPrivate)

/**
  This function executes the MRH (Mode Register Handler).

  If the CADB mux is enabled, MRH is not executed.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] Controller        - Controller to run MRH.
  @param[in] Channel           - The channel to work on
  @param[in] Rank              - The rank to work on
  @param[in] Address           - MRW address
  @param[in] Data              - MRW Data
  @param[in] MrhCmd            - MRH command to execute
  @param[in] SpidWrCmdOverride - Force WrCmd on SPID_cmd_type
  @param[in] MrhGenericCommand - Optinal Generic MRH Command to execute. If NULL, the MrhCmd will be used.
  @param[in] DebugPrint        - When TRUE, will print debugging information

  @retval mrcSuccess              - MRW was sent successfully
  @retval mrcDeviceBusy           - Timed out waiting for MRH
  @retval mrcWrongInputParameter  - Invalid MRH Command was request for current DDR type.
  @retval mrcFail                 - If CADB Mux is enabled
**/
MrcStatus
WrappedMrcRunMrh (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  UINT8                 MrhCmd,
  IN  BOOLEAN               SpidWrCmdOverride,
  IN  VOID                  *MrhGenericCommand, OPTIONAL
  IN  BOOLEAN               DebugPrint
  );

#endif // MRC_MODE_REGISTER_HANDLER_H_
