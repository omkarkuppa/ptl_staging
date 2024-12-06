/** @file
  This file contains code for MC Silicon W/As

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
#ifndef _MrcMc_h_
#define _MrcMc_h_

#include "MrcMcApi.h"

/**
  This function sets minimum time allowed in self refresh mode.
  @param[in]  MrcData           - Pointer to MRC global data.
**/
VOID
MrcMcLpmodeSelfRefreshLength (
  IN  MrcParameters *const  MrcData
  );

/**
  Disable Full Rank Block if TRR is used and DRFM is not enabled.

  @param[in] MrcData      - The MRC general data
  @param[in] Controller   - MC index
  @param[in] Channel      - Channel index
**/
VOID
MrcFullRankBlockOptimization (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel
  );

/**
  This function disables independent fast sleep option.

  @param[in]  MrcData  - Pointer to MRC global data.
**/
VOID
MrcMcRfFastSleepDisable (
  IN  MrcParameters *const  MrcData
  );

#endif //_MrcMc_h_
