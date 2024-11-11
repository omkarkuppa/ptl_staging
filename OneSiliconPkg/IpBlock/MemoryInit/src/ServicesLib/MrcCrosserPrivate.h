/**@file

  This file contains all private crosser training algorithm helpers.

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

@par Specification
**/

#include "MrcCommon.h"
#include "CMrcTypes.h"
#ifdef XTENSA
#include <CGreenMrcCommonTypes.h>
#else
#include <CBlueMrcCommonTypes.h>
#endif
#include "CMrcInterfaceGlobalTypes.h"
#include "MrcCrosser.h" // Included for MAX_BITS_FOR_OFFSET_TRAINING

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcCrosserPrivate)

#define SOT_SELECT_RX                    (2)            ///  Amount of TrainingIdx cases for SenseAmpOffset, for the Rise and Fall
#define RX_RISE                          (0)            ///< Training Case Idx for Rise
#define RX_FALL                          (1)            ///< Training Case Idx for Fall

#pragma pack (push,1)
typedef struct {
  INT64               OrigRxTap0;
  INT64               OrigRxTap1;
  INT64               OrigRxTap2;
  INT64               OrigRxTap3;
  INT64               RxDqVrefOrig;
  INT64               DataOdtModeOrig;
  INT64               DisableOdtStaticOrig;
  INT64               DrvStatLegEnOrig;
  INT64               RcompOdtDnOrig;
  INT64               RcompOdtUpOrig;
  INT64               RcompDrvDnOrig;
  INT64               RcompDrvUpOrig;
  INT64               InternalClockOnOrig;
  INT64               DqsRFTrainingModeValue;
  UINT32              VcmTargetRatio;
  UINT32              VrefEdge[SOT_SELECT_RX][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS_FOR_OFFSET_TRAINING]; // serves as an output for the Vref search results. Used mainly for UTing
  INT32               VocEdge[SOT_SELECT_RX][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS_FOR_OFFSET_TRAINING]; // serves as an output for the Voc search results
} SenseAmpTrainingStateConfig;
#pragma pack (pop)

typedef enum {
  SotVrefSearchStage,
  SotVocSearchStage
} SenseAmpTrainingLsFeedbackStage; // LocalStub feedback stages

/**
  1) Store current MRC settings pre-test for later restoration.
  2) Pad setup for SenseAmpOffset. Calculates Common Mode Percentages and configures comps.

  @param[in,out] MrcData - Include all MRC global data.
  @param[out] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval N/A
**/
VOID
SenseAmpOffsetTrainingSetup (
  IN OUT MrcParameters *const MrcData,
  IN OUT SenseAmpTrainingStateConfig *Config
  );

/**
  Restore previous MRC settings to pre SenseAmpOffset Training
  and clean up.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval N/A
**/
VOID
SenseAmpOffsetTrainingTeardown (
  IN OUT  MrcParameters* const MrcData,
  IN     SenseAmpTrainingStateConfig* Config
  );

/**
  Search for the Vref voltage per lane where the 0 to 1 transition for both Rx
  are equidistant from the center.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval mrcSuccess if successful calibration else mrcFail
**/
MrcStatus
SenseAmpOffsetVrefSearch (
  IN OUT MrcParameters* const MrcData,
  IN     SenseAmpTrainingStateConfig* Config
  );

/**
  Search for the offset code per lane for the 0 to 1 transition for both Rx

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Config - Include MRC settings senseamp changes and Odt Mode.

  @retval mrcSuccess if successful calibration else mrcFail
**/
MrcStatus
SenseAmpOffsetVocSearch (
  IN OUT MrcParameters* const MrcData,
  IN     SenseAmpTrainingStateConfig* Config
  );

/**
  Set the RxVref code for all lanes for the given Controller/Channel/Byte.
  If MultiCast is enabled, the RxVref code will be casted to all Controller/Channel/Byte.

  @param[in,out] MrcData  - Include all MRC global data.
  @param[in] Controller   - Controller to update
  @param[in] Channel      - Channel to update
  @param[in] Channel      - Byte to update
  @param[in] LaneValue    - Array containing the Vref code for each lane
  @param[in] MultiCast    - Bool flag to enable/disable multicasting


  @retval N/A
**/
VOID
SenseAmpOffsetSetRxVrefCodeAllLanes (
  IN OUT MrcParameters* const MrcData,
  IN     UINT8    Controller,
  IN     UINT8    Channel,
  IN     UINT8    Byte,
  IN     UINT32   LaneValue[MAX_BITS_FOR_OFFSET_TRAINING],
  IN     BOOLEAN  MultiCast
  );

/**
  Set the RxVoc (offset) codes for all lanes for the given Controller/Channel/Byte.
  If MultiCast is enabled, the RxVoc code will be casted to all Controller/Channel/Byte.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Controller   - Controller to update
  @param[in] Channel      - Channel to update
  @param[in] Channel      - Byte to update
  @param[in] LaneValue    - 2D Array containing the Voc code for both Rx per lane
  @param[in] MultiCast    - Bool flag to enable/disable multicasting

  @retval N/A
**/
VOID
SenseAmpOffsetSetRxVocCodeAllLanes (
  IN OUT MrcParameters* const MrcData,
  IN     UINT8    Controller,
  IN     UINT8    Channel,
  IN     UINT8    Rank,
  IN     UINT8    Byte,
  IN     INT32    LaneValues[MAX_BITS_FOR_OFFSET_TRAINING][SOT_SELECT_RX],
  IN     BOOLEAN  MultiCast
  );

/**
  Set the RxVoc codes for a specific lane.
  The offset code will be set for both receivers per lane.

  @param[in,out] MrcData - Include all MRC global data.
  @param[in] Controller   - Controller to update
  @param[in] Channel      - Channel to update
  @param[in] Channel      - Byte to update
  @param[in] VocCodes     - Voc codes to set for the two Rx
  @param[in] MultiCast    - Bool flag to enable/disable multicasting

  @retval N/A
**/
VOID
SenseAmpOffsetSetRxVocCodeLane (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8    Controller,
  IN     UINT8    Channel,
  IN     UINT8    Rank,
  IN     UINT8    Byte,
  IN     UINT8    Bit,
  IN     INT32    VocCodes[SOT_SELECT_RX],
  IN     BOOLEAN  MultiCast
);

/**
  Collect all samples from the DataTrainFeedback registers.

  @param[in,out] MrcData    - Include all MRC global data.
  @param[in,out] LaneResult - Pointer to the data structure that holds the DataTrainFeedback values for both Rx on a byte level

  @retval N/A
**/
VOID
SenseAmpOffsetGetRxOffsetResults (
  IN OUT MrcParameters *const MrcData,
  IN OUT UINT32   LaneResult[SOT_SELECT_RX][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM]
  );

/**
  Run comp if needed based on OptParam or if ForceComp is TRUE.

  @param[in] MrcData   - MRC global data.
  @param[in] OptParam  - Param type
  @param[in] ForceComp - Force the comp to run regardless of the param type

  @retval BOOLEAN - Whether a comp was performed or not
**/
BOOLEAN
ForceSystemRComp (
  IN MrcParameters *const MrcData,
  IN UINT8                OptParam,
  IN BOOLEAN              ForceComp
  );
