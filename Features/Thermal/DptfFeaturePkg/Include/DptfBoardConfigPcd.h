/** @file
  Dptf Board Config Structural PCD definition..

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

  This software and the related documents are Intel copyrighted materials, and your
  use of them is governed by the express license under which they were provided to
  you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related
  documents without Intel's prior written permission.

  This software and the related documents are provided as is, with no express or
  implied warranties, other than those that are expressly stated in the License.

@par Specification Reference:
**/

#ifndef _DPTF_BOARD_PCD_H_
#define _DPTF_BOARD_PCD_H_

#pragma pack(push, 1)

//
// CNV Board Configuration Structure. Data are Specific according to Board Design.
//
typedef struct _DPTF_BOARD_CONFIG {
  UINT16                          PChrg;
  UINT8                           PUpper;
  UINT8                           PLower;
  UINT16                          PRate;
  UINT8                           hEol;
  UINT16                          TNormal;
} DPTF_BOARD_CONFIG;

//
// CNV Board Configuration Structure PCD Interface
//
typedef struct _DPTF_BOARD_CONFIG_PCD {
  DPTF_BOARD_CONFIG                DptfBoardConfig;
} DPTF_BOARD_CONFIG_PCD;

#pragma pack(pop)

#endif
