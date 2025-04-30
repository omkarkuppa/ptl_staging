/** @file
  This file declares the interface between the MRC and the different
  types of Test Data Engines

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#ifndef TEST_DATA_ENGINE_H__
#define TEST_DATA_ENGINE_H__

/**
  This function activates and initializes the Test Data Engine.
  The McChBitMask and ChAssign/CpgcChAssign variables will be initialized
  based on the populated channels.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
TestDataEngineInit (
  IN  MrcParameters *const  MrcData
  );

/**
  Get number of column address bits accounting for burst length of the memory technology.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Targeted controller
  @param[in]  Channel           - Targeted channel
  @param[in]  Rank              - Targeted rank

  @retval Number of column address bits
**/
UINT8
TestDataEngineGetColAddressBits (
  IN MrcParameters  *const          MrcData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         Rank
  );

/**
  Get number of columns accounting for burst length of the memory technology.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  Controller        - Targeted controller
  @param[in]  Channel           - Targeted channel
  @param[in]  Rank              - Targeted rank

  @retval Number of columns
**/
UINT16
TestDataEngineGetNumColumns (
  IN MrcParameters  *const          MrcData,
  IN UINT32                         Controller,
  IN UINT32                         Channel,
  IN UINT32                         Rank
  );

#endif //TEST_DATA_ENGINE_H__
