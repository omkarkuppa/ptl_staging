/** @file
    This file contains all the turnaround training algorithm definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _MrcTurnaround_h_
#define _MrcTurnaround_h_

//
// Include files
//
#include "MrcCommon.h"

#define DQFEEDBACKMASK   (0xFF)
#define DQSFEEDBACKMASK  (0x200)
#define PI_STEP_SIZE     (128)
#define PI_PER_TCK       (128)
#define PHCLK_DIVIDEND   (2000000)

#define TAT_COUNT        (16)
#define TAT_SG_DG_MAX    (8)
#define MC_INIT_SAFE_OFFSET_DR (8)
#define MC_INIT_SAFE_OFFSET_SR (4)
#define ODT_TYPE_MAX     (2)

/// CK2CK (pS)
/// Gaurdband any flight time between 2 ranks
#define MRC_LP5_TCK2CK (50)

/// Structure to store turnaround timings
typedef struct {
  UINT32 tRdRdsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdRddg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrdg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrdg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRdsg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRddg[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdRddr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdRddd[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrdr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrWrdd[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrdr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tRdWrdd[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRddr[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32 tWrRddd[MAX_CONTROLLER][MAX_CHANNEL];
} McTurnAroundTimings;

/**
  This function calculates the Turnaround timings based on training results and DRAM Spec

  @param[in] MrcData - Include all MRC global data.
  @param[in] IsMcInit - TRUE is calculating TAT values for Mc Init, FALSE if TAT Optimization

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
MrcStatus
SetTurnAroundTiming (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsMcInit
  );

/**
  This function implements Turn Around Timing Optimization.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
extern
MrcStatus
MrcTurnAroundTimingOptimization (
  IN MrcParameters * const MrcData
  );

/**
  Updates the value for following OptParamCliff variables:
  drrd2rd=0, ddrd2rd=1, drwr2wr=2, ddwr2wr=3, drrd2wr=4, ddrd2wr=5, drwr2rd=6, ddwr2rd=7,
  rdodtd=8, wrodtd=9, mcodts=10, mcodtd=11, rtl=12}

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in]     Controller - Controller to update the specified parameter.
  @param[in]     Channel    - Channel to update the specified parameter.
  @param[in]     Byte       - Byte to update the specified parameter.
  @param[in]     OptParam   - Parameter to update.
  @param[in]     Off        - Value to offset the current setting.
  @param[in]     UpdateHost - Switch to update the host structure with the new value.
  @param[in]     SkipPrint  - Switch to skip debug prints.
  @param[in]     RankMask   - Bit mask of Ranks to update.

  @retval Nothing
**/
extern
void
UpdateTAParamOffset (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT32         Controller,
  IN     const UINT32         Channel,
  IN     const UINT8          Byte,
  IN     const UINT8          OptParam,
  IN     const INT8           Off,
  IN     const UINT8          UpdateHost,
  IN     const UINT8          SkipPrint,
  IN     const UINT8          RankMask
  );

/**
  This function implements Turn Around Timing training.
  Optimize TA ODT Delay and Duration

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
extern
MrcStatus
MrcTurnAroundTiming (
  IN MrcParameters * const MrcData
  );

#endif // _MrcTurnAround_h_
