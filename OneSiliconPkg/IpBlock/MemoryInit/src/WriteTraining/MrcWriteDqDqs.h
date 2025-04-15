/** @file
  The third stage of the write training is determining the PI setting for each
  byte strobe to make sure that data is sent at the optimal location.
  In order to do that a pattern of alternating zeros and ones is written to a block of the memory, and then read out.
  By identifying the location where it is farthest away from where errors are shown the DQS will be aligned to the
  center of the eye.

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
#ifndef _MrcWriteDqDqs_h_
#define _MrcWriteDqDqs_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcCommandTraining.h"
#include "MrcCommon.h"
#include "MrcCommon.h"

/*
 * During Early Write Timing Centering 2D training, first run
 * will check if smallest margin is 5. If it is < 5, run DFE
 * Tap1/Tap2 sweep to find a better margin.
 * Experiments show when margin is < 5, sytem might be not stable.
 */
#define EARLY_2D_OVERCLOCKING_MARGIN    (50)

// DataRetrainEn ([0]: Write DQ2DQS (aka WCK2DQI) Training, [1]: Read WCK2DQO Training)
#define WR_RETRAIN_EN     (MRC_BIT0)

/**
  this function executes the write timing centering in 2D.
    Final write timing centering using 2D algorithm and per bit optimization.

    @param[in] MrcData - Include all MRC global data.

    @retval MrcStatus -  if it succeded returns mrcSuccess

  **/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcWriteTimingCentering (
  IN     MrcParameters *const MrcData
  );

/**
  This function preforms write centering for Unmatched path
  Center Tx DQ/WCK-DQ using moderate pattern with 1D eye.

  @param[in] MrcData  - Include all MRC global data.

  @retval MrcStatus   - If it succeeded, returns mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcUnMatchedWriteTimingCentering (
  IN     MrcParameters *const MrcData
  );

/**
  This function executes the write Voltage centering.
  Center Tx DQ Vref using moderate pattern with 1D eye using PDA mode.

  @param[in] MrcData  - Include all MRC global data.

  @retval MrcStatus   - If it succeeded, returns mrcSuccess
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcWriteVoltageCentering (
  IN     MrcParameters *const MrcData
  );

/**
  this function executes the write timing centering in 2D.
    Final write timing centering using 2D algorithm and per bit optimization.

    @param[in] MrcData - Include all MRC global data.

    @retval MrcStatus -  if it succeded returns mrcSuccess

  **/
extern
MrcStatus
MrcWriteTimingCentering2D (
  IN     MrcParameters *const MrcData
  );

/**
  Measure margins across various parameters

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus -  mrcSuccess if succeded
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcRankMarginTool (
  IN OUT MrcParameters *const MrcData
  );

/**
  Measure margins for the specified parameter. The pattern used must be configured externally prior to invoking this function.

  @param[in, out] MrcData - Include all MRC global data
  @param[in]      Param -  specify parameter for which margin measured

  @retval MrcStatus -  mrcSuccess if succeded
**/
extern
MrcStatus
MrcCustomRankMarginTool (
  IN OUT    MrcParameters  *const  MrcData,
  IN        const MrcMarginTypes  Param
);

/**
  Measure even UI error and odd UI error margins across various RxDqs and TxDq

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus -  mrcSuccess if succeeded
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcRmtEvenOdd (
  IN OUT MrcParameters *const MrcData
  );

/**
  Measure margins across various parameters

  @param[in, out] MrcData - Include all MRC global data.

  @retval                   -  mrcSuccess if succeeded
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcRankMarginToolBit (
  IN OUT MrcParameters *const MrcData
  );

/**
  Perform Read Voltage Centering in 2D.
  Note: This function currently only supports param = WrV

  @param[in, out] MrcData       - Include all MRC global data.

  @retval MrcStatus -    if it succeded return mrcSuccess
  @todo - Need option for loopcount
**/
extern
MrcStatus
MrcWriteVoltageCentering2D (
  IN OUT MrcParameters *const MrcData
  );

/**
  Perform Write Timing Linear Centering in different Write Vref points.

  @param[in] MrcData             - Include all MRC global data.
  @param[in] Prints              - Debug prints enable/disable.
  @param[in] RunEyeSizeFailCheck - Fail for eyes that are too small.

  @retval MrcStatus - if it succeeds return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
EarlyWriteTimingCentering2D (
  IN     MrcParameters *const MrcData,
  IN     BOOLEAN              Prints,
  IN     BOOLEAN              RunEyeSizeFailCheck
  );

/**
  Perform Write Timing Linear Centering in different Vref points.

  @param[in] MrcData       - Include all MRC global data.

  @retval MrcStatus       - if it success return mrcSuccess
**/
extern
MRC_IRAM0_FUNCTION
MrcStatus
MrcEarlyWriteTimingCentering2D (
  IN     MrcParameters *const MrcData
  );

/**
  Perform Re-Training.

  @param[in] MrcData       - Include all MRC global data.

  @retval MrcStatus        - if it success return mrcSuccess
**/
extern
MrcStatus
MrcWriteDqDqsReTraining (
  IN     MrcParameters *const MrcData
  );

/**
  This procedure is meant to handle basic per bit timing centering, places strobe in the middle of the data eye,
  for both read and write DQ/DQS using a very robust, linear search algorithm.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     StepSize       - Step size
  @param[in]     loopcount      - loop count
  @param[in]     MsgPrint       - Show debug prints
  @param[in]     EarlyCentering - Execute as early centering routine

  @retval MrcStatus -  If succeeded, return mrcSuccess
**/
MRC_IRAM0_FUNCTION
MrcStatus
WriteTimingPerBit1DCentering (
  IN OUT MrcParameters *const MrcData,
  IN     const UINT8          StepSize,
  IN     const UINT8          loopcount,
  IN     UINT8                MsgPrintMsk,
  IN     BOOLEAN              EarlyCentering
  );

/**
  Perform Write Voltage Centering in 2D for DDR5.
  Note: This function currently only supports param = WrV

  @param[in, out] MrcData       - Include all MRC global data.

  @retval MrcStatus -    if it succeded return mrcSuccess
  @todo - Need option for loopcount
**/
extern
MrcStatus
MrcWriteVoltageCentering2D_DDR5 (
  IN OUT MrcParameters *const MrcData
  );

/**
  Perform Write Voltage Centering in 2D for DDR5.
  Note: This function currently only supports param = WrV

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in, out] MarginByte  - Margin results array for each byte
  @param[in]      McChBitMask - Channel bit mask.
  @param[in]      Rank        - The particular rank being centered
  @param[in]      LoopCount   - Loop counter.
  @param[in]      En2D        - Enable 2D.

  @retval MrcStatus -    if it succeded return mrcSuccess
**/
extern
MrcStatus
WriteVoltageCentering2D_DDR5 (
  IN OUT MrcParameters *const MrcData,
  IN OUT UINT16               MarginByte[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES],
  IN     UINT8                McChBitMask,
  IN     UINT8                Rank,
  IN     UINT8                LoopCount,
  IN     UINT8                En2D
  );

/**
  Revoke the DVFSQ configuration in DRAM Mode Registers back to 0.5v due to insufficient margins at 0.3v

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess if succeeded
**/
MrcStatus
MrcRevokeDvfsqConfiguration (
  IN OUT MrcParameters *const MrcData
  );

/**
  Check margins and fallback to 0.5V if below threshold.

  @param[in, out] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess if succeeded
**/
MrcStatus
MrcDvfsqCheckRmtResult (
  IN OUT MrcParameters *const MrcData
  );

#endif // _MrcWriteDqDqs_h_
