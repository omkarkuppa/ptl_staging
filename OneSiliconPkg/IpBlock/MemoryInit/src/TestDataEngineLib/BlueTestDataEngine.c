/** @file
  This file implements an interface between the MRC and the different
  test data engines.

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
#include "MrcCpgcOffsets.h"
#include "MrcCommon.h"
#include "MrcCpgcApi.h"
#include "Cpgc20.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "BlueTestDataEngine.h"
#include "CMrcStatsTracker.h" // for MrcStatsStartTimer()


///
/// External Function Definitions
///

/**
  This function activates and initializes the CPGC Engine.
  The McChBitMask and CpgcChAssign variables will be initialized
  based on the populated channels.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
TestDataEngineInit (
  IN  MrcParameters *const  MrcData
) {
  return Cpgc20Init(MrcData);
}

/**
  This function programs the test loop count.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  LoopCount     - Number of sequence iterations

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
TestDataEngineSetLoopCount (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                LoopCount
  )
{
  Cpgc20SetLoopCount (MrcData, McChBitMask, &LoopCount);
  return mrcSuccess;
}

/**
  This function Starts Test on all populated channels.

  @param[in]  MrcData      - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineTestStart (
  IN  MrcParameters  *MrcData,
  IN  UINT8          McChBitMask
  )
{
  MrcStatsStartTimer (MrcData, MRC_CPGC_TIME);
  Cpgc20StartTest (MrcData, McChBitMask);
}

/**
  This function Stops Test on all populated channels.

  @param[in]  MrcData      - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineTestStop (
  IN  MrcParameters  *MrcData,
  IN  UINT8          McChBitMask
  )
{
  Cpgc20StopTest (MrcData, McChBitMask);
  MrcStatsEndTimer (MrcData, MRC_CPGC_TIME);
}

/**
  This function Starts a Non-Stop Test on all populated channels.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineStartEndlessTest (
  IN  MrcParameters *const  MrcData,
  IN  UINT8             McChBitMask
  )
{
  TestDataEngineTestStart (MrcData, McChBitMask);
}

/**
  This function Stops a Non-Stop Test on all populated channels.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineStopEndlessTest (
  IN  MrcParameters *const MrcData,
  IN  UINT8                McChBitMask
  )
{
  TestDataEngineTestStop (MrcData, McChBitMask);
}

/**
  This function returns success if test is done.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if Test is Done, else mrcFail.
**/
MrcStatus
TestDataEngineTestDone (
  IN  MrcParameters  *MrcData,
  IN  UINT8          McChBitMask
  )
{
  MrcOutput *Outputs;
  UINT8  Controller;
  UINT8  Channel;
  UINT8  MaxChannel;
  UINT8  CurMcChBitMask;
  BOOLEAN    IsLpddr;
  MrcStatus  Status;

  Status      = mrcSuccess;
  Outputs     = &MrcData->Outputs;
  IsLpddr     = Outputs->IsLpddr;
  MaxChannel  = Outputs->MaxChannels;

  // Wait till test is done on all participating channels
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      CurMcChBitMask = (1 << ((Controller * MaxChannel) + Channel));
      if (((McChBitMask & CurMcChBitMask) != 0) && !(IS_MC_SUB_CH(IsLpddr, Channel))) {
        // polls Cpgc registers to see if a test is done - mrcSuccess (=0) is returned
        // else mrcFail (=1) is returned and ther accumulated status will be mrcFail
        // Cpgc20TestDone sets 10 seconds for timeout.
        Status |= Cpgc20TestDone (MrcData, Controller, Channel);
        if (Status != mrcSuccess) {
          MRC_DEBUG_ASSERT(FALSE, &Outputs->Debug, "CPGC test exceeds timeout value\n");
        }
      }
    }
  }

  MrcStatsEndTimer (MrcData, MRC_CPGC_TIME);

  return Status;
}
