/** @file
  Contains functions that are used outside of the PMA Library.

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
#ifndef MRC_PMA_API_H_
#define MRC_PMA_API_H_


/**
  This function set the MRC vertion to MCDECS_SPARE register.
  The function need to be call by the wrapper after MrcStartMemoryConfiguration function where the MC CLK enable.
  The function write:
  - Major number to bits 16-23
  - Minor number to bits 8-15
  - Build number to bits 0 - 7

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess if MrcRevision read successfully
**/
extern
MrcStatus
MrcSetMrcVersion (
  IN     MrcParameters *const MrcData
  );

/**
  This function reads the PMA capability register and sets the memory controller's capability.

  @param[in, out] MrcData - All the MRC global data.

  @retval mrcSuccess memory controller's capability has been determined
  @retval mrcFail memory controller's capability was not determined
**/
extern
MrcStatus
MrcMcCapability (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function reads the MEMSS_PMA_CR_CAPABILITIES_MISC register
  and determines if SAGV Input parameters is to be overwritten.

  @param[in, out] MrcData - All the MRC global data.
**/
VOID
MrcSaGvCapabilityCheck (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function sends PM messages to PHY.

  @param[in] MrcData - Include all MRC global data.
  @param[in] PmMsgCode - PM13 or PM14.

  @retval mrcSuccess PM message sending is successful
  @retval mrcDeviceBusy if poll InitComplete hits timeout
**/
MrcStatus
MrcPhyPmMessage (
  IN MrcParameters* const MrcData,
  IN UINT32               PmMsgCode
  );

/**
  This function locks both PHY and MC PLL by requesting memSS PMA

  @param[in] MrcData             - Include all MRC global data.
  @param[in] IsVoltageChangeOnly - FALSE for normal PM_MESSAGE_0. TRUE for PM_MESSAGE_0_VOLTAGE_ONLY, sent later on to change voltage after Green training is done.
  @param[in] DebugPrint          - Enable/disable debug printing

  @retval mrcSuccess PLL is locked successfully
  @retval other an unknown failure occurred
**/
MrcStatus
MrcPllLock (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              IsVoltageChangeOnly,
  IN BOOLEAN              DebugPrint
  );

/**
  This function sends a given PM message (PM0,PM6, PM8 PM13,PM14).

  @param[in] MrcData - Include all MRC global data.
  @param[in] PmMsgCode - PM0 or PM6 or PM8 or PM13 or PM14.
  @param[in] RequestTo - Which block is being requested, PHY or MC

  @retval mrcSuccess if poll InitComplete completed successfully
  @retval other an error code propagated from a called function
**/
MrcStatus
MrcSendPmMessage (
  IN MrcParameters* const MrcData,
  IN UINT32               PmMsgCode,
  IN BOOLEAN              RequestTo
  );

/**
  This function checks the DDRSZ filed.

  @param[in, out] MrcData - All the MRC global data.
  @param[in] PmaMcCapData - The value of MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT.

  @retval mrcSuccess memory controller's capability has been determined
  @retval mrcFail memory controller's capability was not determined
**/
MrcStatus
MrcMcCheckDDRSZ (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32         PmaMcCapData
  );

/**
  This function reads the PMA capacity register and sets the following values
  according to memory controller's capability and user input:
    Outputs->FreqMax
    Outputs->MemoryClockMax

  @param[in, out] MrcData - All the MRC global data.

  @retval mrcSuccess listed values has been set
  @retval mrcFail listed values was not set
**/
MrcStatus
MrcMcCapabilityPreSpd (
  IN OUT MrcParameters *const MrcData
  );

/**
  Sends PM14 message to MC using MemSS PMA.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess PM14 message sending is successful
  @retval mrcDeviceBusy if poll InitComplete hits timeout
**/
MrcStatus
MrcMcPm14Message (
  IN MrcParameters* const MrcData
  );

/**
  This function set PtrrSeed for pTRR/DRFM.
  @param[in] MrcData - Include all MRC global data.
**/

VOID MrcPmaSetPtrrSeed (
  IN     MrcParameters *const MrcData
  );

/**
  This function determines if the board design limits the POR frequency and updates Outputs->FreqMax as necessary

  @param[in, out] MrcData - All the MRC global data.

  @retval Returns mrcSuccess if the Board Details are supported otherwise mrcFail.
**/
MrcStatus
MrcBoardAndSkuFreqCheck (
  IN OUT MrcParameters *const  MrcData
  );

#endif //MRC_PMA_API_H_
