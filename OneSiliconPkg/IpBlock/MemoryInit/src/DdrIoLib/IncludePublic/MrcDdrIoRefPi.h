/** @file
  This file contains code related to initializing and configuring the DDRIO RefPi.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _MrcDdrIoRefPi_h_
#define _MrcDdrIoRefPi_h_

#include "CMrcInterface.h"  // for MrcParameters

// Defines
#define MRC_REFPI_DEFAULT       (0)
#define MRC_REFPI_XOVER_DIVIDER (256)
#define MRC_PRINT_CURRENT_TASK  (TRUE)
#define MRC_SKIP_PRINT          (FALSE)
#define MRC_REFPI_BAD_INPUT     (0xFFFFFFFF)

#define MRC_REFPI_CAL_LIST_LEN  (12)

typedef enum {
  MrcRefPiDefault       = 0,
  MrcRefPi2QClkDataCcc  = 1,
  MrcRefPi2XOverAlign   = 2
} MRC_REFPI_CAL_STAGE;

typedef enum {
  RefPiCccFub       = 1,
  RefPiXOverCcc     = 2,
  RefPiDataFub      = 4,
  RefPiXOverData    = 8,
  RefPiXOverDataDQS = 0x10,
  RefPiXOverDataWck = 0x20,
  RefPiXOverCccClk  = 0x40,
  AllRefPiOffsets   = (RefPiCccFub | RefPiDataFub),
  AllXOverOffsets   = (RefPiXOverCcc | RefPiXOverData | RefPiXOverDataDQS | RefPiXOverDataWck | RefPiXOverCccClk),
  AllCccInst        = (RefPiCccFub | RefPiXOverCcc | RefPiXOverCccClk)
} MRC_REFPI_PARTITION_OFFSETS;

typedef enum {
  RefPiAlignmentCentered,
  RefPiAlignmentMinHold,
  RefPiAlignmentMax
} MRC_REF_PI_ALIGNMENT;

typedef enum {
  MrcVccsaPt85,
  MrcVccsaPt65,
  MrcVccsaMax
} MRC_REFPI_VCCSA;

typedef enum {
  MrcRpData,
  MrcRpCcc,
  MrcRpMax
} MRC_REFPI_PAR_TYPE;

typedef struct {
  UINT8   RefPi;
  UINT8   RefPiOffset;
  UINT16  RefPi4Xover;
  UINT16  RefPi4XoverOffset;
  UINT16  XoverRefPiDelta;
  BOOLEAN IsPresent;
} MRC_REFPI_DATA_TYPE;

typedef struct {
  MRC_REFPI_PAR_TYPE ParType;
  UINT8   Instance;
  UINT8   Channel;
  INT16   Delay;
} MRC_REFPI_DELAY_TYPE;

typedef struct {
  MRC_REFPI_PAR_TYPE ParType;
  UINT8   Instance;
  UINT8   Channel;  ///< Only used by the Data Partitions as there are two "bytes" in 1 Data Instance
} MRC_REFPI_PAR_ORDER_TYPE;

/**
  This function executes the RefPI Calibration flow for the DDRIO.  In addition, the
  function checks to ensure the delays are monotonic, and the delays for RefPi and
  RefPi4Xover meet the requirements.  This function does not block the boot if the
  checks are violated.  The violations will be flagged in the log.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - The return value from MrcGetRefPiData() if it does not function correctly
                    - mrcSuccess otherwise.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcRefPiCalibration (
  IN  MrcParameters *const MrcData
  );

/**
  This function configures the RefPI FSM control configuration for POR operation after calibration is complete.

  @param[in]  MrcData     - Pointer to global MRC data

  @retval MrcStatus
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcFinalizeRefPiFsmControl (
  IN  MrcParameters *const MrcData
  );

/**
  This function initializes the RefPiFsmControl register for DDRIO.
  All partitions which have RefPiFsmControl have the same structure which allows for a single init function.

  @param[in] MrcData   - Pointer to global MRC data.
  @param[in] FsmStage  - FSM stage
  @param[in] Offset    - Partiton offset for restoring previous values
  @param[in] PrintTask - Qualifier to include print statemenets

  @return UINT32 - Initialized value of RefPiFsmControl Register
**/
MRC_IRAM0_FUNCTION
UINT32
MrcDdrIoInitRefPiFsmCtl (
  IN MrcParameters* const MrcData,
  IN MRC_REFPI_CAL_STAGE  FsmStage,
  IN UINT32               Offset,
  IN BOOLEAN              PrintTask
  );

//To-Do: Add all function prototypes not elsewhere declared from MrcDdrIoConfigNvl.c here

/**
  This function kicks off RefPI FSM in all the DDRIO partitions.  Each partition on a line is run in parallel
    DATA, CCC_SHARED
  Function has a 10 second timeout waiting for the FSM's to complete.

  @param[in]  MrcData        - Pointer to MRC global data.

  @retval MrcStatus - mrcDeviceBusy: A partition status bit did not clear before the timeout.
                    - mrcSuccess: FSM's executed with no problems.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcRunRefPiFsm (
  IN  MrcParameters *const MrcData
  );

/**
  This function accesses RefPi and RefP4Xover from the DDRIO based on the specific partition
  passed in ParInfo and stores the result in the output variable RefPiData.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[in]  ParInfo   - Pointer to the partition definition that should be accessed.
  @param[out] RefPiData - Pointer to the storage variable for RefPi and RefPi4Xover.
  @param[out] SaveData  - Pointer to the buffer to store the RefPI CR data.

  @retval MrcStatus - mrcWrongInputParameter if RefPiData or ParInfo is NULL;
                    - mrcFail if the ParType in ParInfo isn't supported
                    - mrcSuccess otherwise.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcGetRefPiData (
  IN        MrcParameters                 *MrcData,
  IN const  MRC_REFPI_PAR_ORDER_TYPE      *ParInfo,
  OUT       MRC_REFPI_DATA_TYPE           *RefPiData,
  OUT       MRC_REF_PI_FREQ_SWITCH_DATA   *SaveData
  );

/**
  This function checks to determine if the delays in a test list are valid based on acceptable limits
  for propagation between partitions.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  ParData     - Pointer to 1D array containing RefPi and RefPi4Xover data per partition.
  @param[in]  ParOrder    - Pointer to 1D array containing partition information and order.
  @param[in]  TestListIdx - Test index used to keep RefPiRelativeDelayLists in sync
  @param[in]  Length      - Pointer to 1D array containing the length of the test list.

  @retval BOOLEAN - TRUE if delay is valid, otherwise FALSE.
**/
MRC_IRAM0_FUNCTION
BOOLEAN
MrcRefPiDelayCheck (
  IN MrcParameters *MrcData,
  IN MRC_REFPI_DATA_TYPE ParData[],
  IN const  MRC_REFPI_PAR_ORDER_TYPE ParOrder[],
  IN UINT32 TestListIdx,
  IN UINT8 Length
  );

/**
  This function will iterate through Length number of lists of TestListLen to verify
  two formal requirements on RefPi and RefPi4Xover:
    For each partition - The delta between RefPi4Xover and RefPi should be within 20-100ps in terms of PI ticks.
    Across all paritions - The delta between the fastest and slowest partition for RefPi4Xover-RefPi should be <= 20ps.
    RefPi4Xover = (RefPi + RefPiOFfset + H-Tree) % MRC_REFPI_XOVER_DIVIDER

    @param[in]  MrcData     - Pointer to global MRC data.
    @param[in]  ListData    - Pointer to 2D array containing RefPi and RefPi4Xover data per partition.
    @param[in]  TestListLen - Pointer to 1D array containing the length of the test list.
    @param[in]  Length      - Number of test lists in ListData.

    @retval: MrcSuccess - mrcFail if either of the two constraints are violated, otherwise mrcSuccess.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcRefPi4XoverCheck (
  IN  MrcParameters *MrcData,
  IN  MRC_REFPI_DATA_TYPE ListData[][MRC_REFPI_CAL_LIST_LEN],
  IN  const UINT8 TestListLen[],
  IN  UINT8 Length
  );

/**
  This function formats and prints RefPi, RefPiOffset, RefPi4Xover, and the delta betwen the Xover and the source.
  The source is the sum of RefPi and RefPiOffset modulo MRC_REFPI_XOVER_DIVIDER.
  The format will be a 2D table of 4 columns:
    List i    RefPi RefPiOffset 4Xover  Delta
    Par1      w     x           y       z
    Par2      a     b           c       d

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  ListData    - Pointer to 2D array containing RefPi and RefPi4Xover data per partition.
  @param[in]  ParLists    - Pointer to 2D array containing partition information and order.
  @param[in]  TestListLen - Pointer to 1D array containing the length of the test list.
  @param[in]  Length      - Number of test lists in ListData.
  @param[in]  PrintLevel  - The MrcDebugMsgLevel to use for the debug prints of this function.

  @retval mrcSuccess if all existing RefPi / RefPi4Xover values are not zero otherwise mrcFail
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcRefPiPrint (
  IN  MrcParameters *MrcData,
  IN  MRC_REFPI_DATA_TYPE ListData[][MRC_REFPI_CAL_LIST_LEN],
  IN  const MRC_REFPI_PAR_ORDER_TYPE **ParLists,
  IN  const UINT8 TestListLen[],
  IN  UINT8 Length,
  IN  MrcDebugMsgLevel PrintLevel
  );

/**
  This function sets RefPiFsmControl to the correct value for input FSM stage.
  This is acieved via CR Multicast in the DDRIO.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  FsmStage     - FSM calibration stage
**/
MRC_IRAM0_FUNCTION
VOID
MrcDdrIoResetRefPiFsmCtl (
  IN  MrcParameters* const MrcData,
  IN  MRC_REFPI_CAL_STAGE  FsmStage
  );

/**
  This function restores RefPi

  @param[in] MrcData  - Include all MRC global data.
**/
MRC_IRAM0_FUNCTION
VOID
RefPiUpdate (
  IN MrcParameters *const MrcData
  );

/**
  This function organizes input values to calulate RefPi and XOver offset values. Per spec, the same
  channel values are used for all partitions

  @param[in] MrcData     - Include all MRC global data
  @param[in] Partition   - Current partition to operate on
  @param[in] Channel     - Current channel within the partition

  @return                        Return a UINT32 for the calculated offset
  @return MRC_REFPI_BAD_INPUT    Invalid partition supplied
**/
MRC_IRAM0_FUNCTION
UINT32
MrcGetRefPiOffset (
  IN MrcParameters* const        MrcData,
  IN MRC_REFPI_PARTITION_OFFSETS Partition,
  IN UINT32                      Channel
  );

/**
  This function calculates the values for RefPi/XOver offset

  @param[in] MrcData      - Include all MRC global data.
  @param[in] Delay        - Current delay value in ps
  @param[in] AddedDelayPi - Additional delay offset in PI
  @param[in] Partition    - PHY Partition

  @return - Return a calculated offset in PI
**/
MRC_IRAM0_FUNCTION
UINT32
MrcCalcRefPiOffsetMarginCenter (
  IN MrcParameters* const        MrcData,
  IN INT32                       Delay,
  IN INT32                       AddedDelayPi,
  IN MRC_REFPI_PARTITION_OFFSETS Partition
  );

/**
  This function accesses RefPi and RefP4Xover from the DDRIO based on the specific partition
  passed in ParInfo and stores the result in the output variable RefPiData.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] RefPiData - Pointer to the storage variable for RefPi and RefPi4Xover.
  @param[out] SaveCrPtr - Pointer to the storage variable for saved CR.
  @param[in]  Offset    - The register address offset to be read.

  @retval MrcStatus - mrcWrongInputParameter if RefPiData or ParInfo is NULL;
                    - mrcFail if the ParType in ParInfo isn't supported
                    - mrcSuccess otherwise.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcReadUpdateRefPiData (
  IN        MrcParameters             *MrcData,
  OUT       MRC_REFPI_DATA_TYPE       *RefPiData,
  OUT       UINT32                    *SaveCrPtr,
  IN        UINT32                     Offset
  );

/**
  Wrap around the RefPi offset values if they fall outside the permitted range

  @param[in, out] RefPiOffset       - pointer to RefPiOffset value
  @param[in, out] RefPi4XoverOffset - pointer to RefPi4XoverOffset value
**/
MRC_IRAM0_FUNCTION
VOID
MrcRefPiOffsetWrapAround (
  IN OUT INT32  *RefPiOffset,
  IN OUT INT32  *RefPi4XoverOffset
  );

#endif // _MrcDdrIoRefPi_h_
