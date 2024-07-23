/** @file
  Private functions supporting TestDataEngine.c code

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/
#ifndef MRC_DDR_IO_UTILS_PRIVATE_H_
#define MRC_DDR_IO_UTILS_PRIVATE_H_

#include "CMrcInterface.h" // for MrcParameters
#include "MrcDdrIoDefines.h"  // for COMP_CYCLE_TYPE

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcDdrIoUtilsPrivate);


/**
  This function does Force Comp and Polls

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     CompType - Enum of Comp being forced

  @retval mrcSuccess if Poll returns expected otherwise mrcDeviceBusy.
**/
MrcStatus
WrappedForceRcomp (
  IN OUT MrcParameters *const MrcData,
  IN     COMP_CYCLE_TYPE      CompType
  );

#endif // MRC_DDR_IO_UTILS_PRIVATE_H_

