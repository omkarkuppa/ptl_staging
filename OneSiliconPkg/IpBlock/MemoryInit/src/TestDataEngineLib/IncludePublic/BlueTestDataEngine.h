/** @file
  This file declares the interface between the MRC and the different
  types of Test Data Engines

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
#ifndef BLUE_TEST_DATA_ENGINE_H__
#define BLUE_TEST_DATA_ENGINE_H__

#include "TestDataEngine.h" // for definitions common to blue and green

 /**
  This function programs the test loop count.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  LoopCount     - Number of sequence iterations.

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
TestDataEngineSetLoopCount (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                LoopCount
  );

/**
  This function Starts Test on all populated channels.

  @param[in]  MrcData      - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineTestStart (
  IN  MrcParameters  *MrcData,
  IN  UINT8          McChBitMask
  );

/**
  This function Stops Test on all populated channels.

  @param[in]  MrcData      - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineTestStop (
  IN  MrcParameters  *MrcData,
  IN  UINT8             McChBitMask
  );

/**
  This function Starts a Non-Stop Test on all populated channels.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineStartEndlessTest (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask

  );

/**
  This function Stops a Non-Stop Test on all populated channels.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval Nothing.
**/
void
TestDataEngineStopEndlessTest (
  IN  MrcParameters *const  MrcData,
  IN  UINT8             McChBitMask
  );

/**
  This function polls Mtg registers to see if a test is done. it sets 10 seconds for timeout.

  @param[in]  MrcData       - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if Test is Done, else mrcFail.
**/
MrcStatus
TestDataEngineTestDone (
  IN     MrcParameters  *MrcData,
  IN  UINT8             McChBitMask
  );

#endif //BLUE_TEST_DATA_ENGINE_H__
