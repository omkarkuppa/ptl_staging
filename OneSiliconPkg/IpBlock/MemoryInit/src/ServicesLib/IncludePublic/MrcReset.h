/** @file
  The LP5 reset sequence definitions.

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

#ifndef MRC_RESET_H_
#define MRC_RESET_H_

#include "CMrcInterface.h"  // for MrcParameters

/**
  Reset the IO module. The hardware will handle creating the 20 dclk pulse
  after the bit is set and will also clear the bit.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess IO Reset was done successfully
  @retval mrcDeviceBusy Timed out waiting for the IO to clear the bit
**/
MrcStatus
IoReset(
  IN MrcParameters *const MrcData
  );

/**
  Perform full JEDEC reset and init sequence.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess Perform JEDEC reset sequence and init sequence successfully
  @retval mrcFail Failed to perform Jeddec reset and init sequence
  //@TODO (HSD_14016775949) Remove after MCIP testing is done
**/
extern
MrcStatus
MrcResetSequence (
  IN MrcParameters *const MrcData
  );
#endif // MRC_RESET_H_
