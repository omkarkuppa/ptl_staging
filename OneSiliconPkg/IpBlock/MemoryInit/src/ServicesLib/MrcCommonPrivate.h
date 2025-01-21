/** @file
  This file include all the private MRC common data helpers.

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
**/
#ifndef MRC_COMMON_PRIVATE_H_
#define MRC_COMMON_PRIVATE_H_

#include "CMrcTypes.h"

// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(MrcCommonPrivate)

/**
  This function is a helper function for ChangeMargin.
  The function examines the RankMask to determine if it specifies a single rank.
  If the RankMask corresponds to exactly one rank, the 0 based rank is returned.
  If the rank does not specify a single rank (i.e., it is zero, or specifies multiple ranks), MAX_RANK_IN_CHANNEL
  is returned.

  @param[in]     RankMask     - RankMask to convert to rank

  @return The single rank specified by RankMask or MAX_RANK_IN_CHANNEL if no rank or multiple ranks are specified.
**/
UINT8
GetRankFromRankMask (
  IN  const UINT8 RankMask
  );

#endif // MRC_COMMON_PRIVATE_H_
