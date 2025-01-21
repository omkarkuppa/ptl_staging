/**@file
  Defines Platform BoardIds

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

@par Specification
**/
#ifndef _PLATFORM_BOARD_ID_H_
#define _PLATFORM_BOARD_ID_H_


//
// PantherLake Board Id 0x00 - 0x3F
//

// BoardId Reserved for PCD Default SkuId usage
// SkuId = PcdDisplayId << 24 | PcdBoardBomId << 16 | PcdBoardRev << 8 | PcdBoardId
#define SkuIdPtlSimics                        0x00000031
#define SkuIdPtlUHLp5Rvp1                     0x00000001


#define BoardIdPtlSimics                      0x31
#define BoardIdPtlUHLp5Rvp1                   0x01


#endif // _PLATFORM_BOARD_ID_H_
