/** @file
  This module configures the memory controller timing parameters.

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
#ifndef _MrcTimingConfiguration_h_
#define _MrcTimingConfiguration_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcCrosser.h"

/**
  This function configures the memory controller timings.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
extern
void
MrcTimingConfiguration (
  IN MrcParameters *const MrcData
  );

#endif // _MrcTimingConfiguration_h_

