/** @file
  This file contains code related to initializing and configuring the DDRIO Rcomp.

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

#ifndef _MrcDdrIoRcomp_h_
#define _MrcDdrIoRcomp_h_

#include "CMrcTypes.h"
#include "CMrcApi.h"

/**
  This function determines default RcompTargets based on CPU type and DDR Type

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Profile - MemoryProfile

  @retval Returns pointer to RcompTarget
**/
const UINT16 *
MrcGetDefaultRcompTarget (
  IN OUT MrcParameters *const MrcData,
  IN     MrcProfile Profile
  );

#endif // _MrcDdrIoRcomp_h_
