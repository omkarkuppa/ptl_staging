/** @file
  Implements functions for handling Mode Register routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef __MrcModeRegisterHandler_h__
#define __MrcModeRegisterHandler_h__

///
/// Mode Register Handler Commands
///
#define MRC_MRH_CMD_MRR             (0)
#define MRC_MRH_CMD_MRW             (1)
#define MRC_MRH_CMD_MPC             (2)
#define MRC_MRH_CMD_DDR5_VREF_CMD   (3)
// Note : For MultiCyc MRH NOP command use "Multicyccs" field to specify the number of NOPs.
#define MRC_MRH_CMD_DDR5_NOP        (4)
#define MRC_MRH_CMD_DDR5_MAX        (MRC_MRH_CMD_DDR5_NOP)
#define MRC_MRH_CMD_LPDDR5_CAS      (3)
#define MRC_MRH_CMD_LPDDR5_MAX      (MRC_MRH_CMD_LPDDR5_CAS)

#define MRC_MRH_DDR5_SPID_CMD_WR_CMD (5)
#define MRC_MRH_DDR5_SPID_MR46       (1)
#define MRC_MRH_DDR5_SPID_MR47       (2)
#define MRC_MRH_LPDDR5_SPID_MR35     (1)
#define MRC_MRH_LPDDR5_SPID_MR36     (2)
#define MRC_MRH_LPDDR5_SPID_MR38     (3)
#define MRC_MRH_LPDDR5_SPID_MR39     (4)

///
/// WCK2CK SYNC OFF (CAS)
///
#define MRC_LP5_CAS_WCK2CK_SYNC_OFF (0x7)
///
/// WCK2CK FAST SYNC (CAS)
///
#define MRC_LP5_CAS_WCK2CK_FAST_SYNC (0x1)

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
  @param[in] DebugPrint        - When TRUE, will print debugging information

  @retval mrcSuccess              - MRW was sent successfully
  @retval mrcDeviceBusy           - Timed out waiting for MRH
  @retval mrcWrongInputParameter  - Invalid MRH Command was request for current DDR type.
  @retval mrcFail                 - If CADB Mux is enabled
**/
MrcStatus
MrcRunMrh (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  UINT8                 MrhCmd,
  IN  BOOLEAN               SpidWrCmdOverride,
  IN  BOOLEAN               DebugPrint
  );

#endif // __MrcModeRegisterHandler_h__

