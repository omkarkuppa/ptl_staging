/** @file
  Starting point for the core memory reference code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#ifndef __MRC_StartMemoryConfiguration_h__
#define __MRC_StartMemoryConfiguration_h__

//
// Include files
//

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcDebugPrint.h"

#define POST_CODE_NO_OVR  ((1 << ((sizeof (MrcPostCode) * 8) - 1)) + ((1 << ((sizeof (MrcPostCode) * 8) - 1)) - 1))

typedef enum {
  MF_NULL,                ///< All policy flags turned off.
  MF_COLD     = (1 << 0), ///< Execute MRC function on cold reset.
  MF_FAST     = (1 << 1), ///< Execute MRC function on cold reset when S3 data is present.
  MF_WARM     = (1 << 2), ///< Execute MRC function on warm reset.
  MF_S3       = (1 << 3), ///< Execute MRC function on S3 exit.
  MF_RMT      = (1 << 4), ///< Execute RMT after this function.
  MF_GV_FIRST = (1 << 5), ///< Execute MRC function when in the First SA GV point.
  MF_GV_OTHER = (1 << 6), ///< Execute MRC function when in any SA GV point that is not first or last.
  MF_GV_LAST  = (1 << 7), ///< Execute MRC function when in the Last SA GV point.
  MF_TST      = (1 << 8), ///< Execute Trained State Tracing after this function.
  MF_SKIP_STATS = (1 << 9), ///< Stats framework must be paused for this function (blue/green transition)
  MF_DELIM    = MRC_UINT32_MAX
} MrcPolicyFlag;

#pragma pack(push, 1)

typedef struct {
  MrcStatus (*mrc_task) (MrcParameters * const MrcData);  ///< Ptr to function to execute, with parameter list.
  MrcPostCode   post_code_ovr;                            ///< BIOS post code output to the debug port if value <> 0.
  UINT32        oem_cmd;                                  ///< OEM function to execute prior to MRC function.
  BOOLEAN       DebugPrint;                               ///< Print debug log or not for this task
  MrcPolicyFlag policy_flag;                              ///< Call table flags
  MrcIteration  iteration;                                ///< Maximum number of CPU only resets.
  char          *String;                                  ///< Output string describing this task (potentially output to debug serial port).
#ifdef MRC_DEBUG_PRINT
  TrainedGroupTypesPrintIndex  GroupTypeArrayIndex;       ///< Index of trained params List for printing. keep 0 here if not used.
#endif // MRC_DEBUG_PRINT
} CallTableEntry;

typedef struct {
  // Begin Outputs Save Data
  MrcDdrType     OutDdrType;
  BOOLEAN        OutLpByteMode;
  BOOLEAN        OutEnhancedChannelMode;
  BOOLEAN        OutIsLpddr;
  BOOLEAN        OutIsLpddr5;
  BOOLEAN        OutIsDdr5;
  BOOLEAN        OutIsLP5Camm2;
  UINT32         OutSystemDimmCount;
  UINT32         OutVdd2Mv;
  UINT8          OutBurstLength;
  UINT8          OutMaxChannels;
  UINT8          OutMaxDimm;
  UINT8          OutMaxDqBits;
  UINT8          OutXmpProfileEnable;
  BOOLEAN        OutXmpConfigWarning;
  MrcVddSelect   OutVddVoltage[MAX_PROFILE];
  MrcVddSelect   OutVddqVoltage[MAX_PROFILE];
  MrcVddSelect   OutVppVoltage[MAX_PROFILE];
  MrcFrequency   OutMemFrequency[MAX_PROFILE];
  BOOLEAN        OutIsDimm1Enabled;
  BOOLEAN        OutIsAny2Dpc;
  BOOLEAN        OutIs1DPC1R;
  BOOLEAN        OutIs1DPC2R;
  BOOLEAN        OutIs2SPC0R1R;
  BOOLEAN        OutIs2SPC0R2R;
  BOOLEAN        OutIs2DPC1R1R;
  BOOLEAN        OutIs2DPC2R2R;
  BOOLEAN        OutIsSoDimm;
  BOOLEAN        OutIs2RankDdr5;
  MRC_CKD_BUFFER OutCkdBuffer[MAX_DIMMS_IN_SYSTEM];
  UINT8          Reserved[3];
  // Begin DimmOut Save Data
  MrcDimmSts     DimmOutStatus[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  MrcDdrType     DimmOutDdrType[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  MEMORY_PACKAGE DimmOutModuleType[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutEccSupport[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutAddressMirrored[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutSelfRefreshTemp[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutAutoSelfRefresh[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutPartialSelfRefresh[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutOnDieThermalSensor[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutExtendedTemperRange[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  BOOLEAN        DimmOutExtendedTemperRefresh[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT32         DimmOutDimmCapacity[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT32         DimmOutRowSize[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT16         DimmOutColumnSize[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT16         DimmOutCrc[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutRankInDimm[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutBanks[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutBankGroups[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutPrimaryBusWidth[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutDiePerSdramPackage[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutChannelsPerSdramPackage[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutNumLpSysChannel[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutSdramWidth[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutSdramWidthIndex[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutDensityIndex[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOuttMAC[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutReferenceRawCard[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutXmpSupport[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT32         ChannelOutDimmCount[MAX_CONTROLLER][MAX_CHANNEL];
  BOOLEAN        OutLpX;
  BOOLEAN        OutIsAny2Ranks;
  BOOLEAN        DimmOutCkdSupport[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
  UINT8          DimmOutDeviceDensity[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL];
} STATIC_SPD_DATA;

#pragma pack(pop)

extern const CallTableEntry MrcCallTable[];

/**
  Save Static SPD Data in a given structure.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      StaticSpdData - Static SPD Data storage pointer.

**/
VOID
MrcSaveStaticSpdData (
  IN OUT MrcParameters *const MrcData,
  IN OUT STATIC_SPD_DATA      *StaticSpdData
);

/**
  Restore Static SPD Data from a given structure.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      StaticSpdData - Static SPD Data storage pointer.

**/
VOID
MrcRestoreStaticSpdData (
  IN OUT MrcParameters *const MrcData,
  IN OUT STATIC_SPD_DATA      *StaticSpdData
);

/**
  this function use by the OEM to do dedicated task during the MRC.

  @param[in] MrcData           - include all the MRC data
  @param[in] OemStatusCommand  - A command that indicates the task to perform.

  @retval The status of the task.
**/
MrcStatus
MrcInternalCheckPoint (
  IN MrcParameters        *MrcData,
  IN MrcOemStatusCommand  OemStatusCommand
);

/**
@brief

  Get the number of call table entries.

  @retval The number of call table entries.

**/
extern
UINT16
MrcGetCallTableCount (
  VOID
);

/**
  Convert a MRC task post code to the corresponding call table index.

  @param[in] MrcData  - Pointer to the MRC global data area.
  @param[in] PostCode - The post code to convert.

  @retval The index on success, INVALID_TASK_INDEX on failure.
**/
UINT32
ConvertPostCodeToIndex (
  MrcParameters *MrcData,
  UINT32        PostCode
);

/**
  Given an index to a task in the call table, return its postcode.
  It is assumed the index is correct, thus no error handling.

  @param[in] MrcData  - Pointer to the MRC global data area.
  @param[in] Index    - The index to convert

  @retval mrcSuccess
**/
MrcPostCode
ConvertIndexToPostCode (
  MrcParameters *MrcData,
  UINT32        Index
);

#endif
