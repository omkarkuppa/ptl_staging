/** @file
  Board Info definition.

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
#ifndef __BOARD_INFO_H__
#define __BOARD_INFO_H__

typedef union {
  UINT16    Raw;
  struct {
    UINT16  BoardId        :6;     // [5:0]     - BOARD_IDx
    UINT16  BomId          :3;     // [8:6]     - BOM_IDx
    UINT16  FabId          :2;     // [10:9]    - REV_FAB_IDx
    UINT16  SpdPresent     :1;     // [11]      - SPD_PRSNT
    UINT16  DdiAConfig     :1;     // [12]      - DDI_A_CONFIG 0: EDP, 1: MIPI
    UINT16  DdiBConfig     :2;     // [14:13]   - DDI_B_CONFIG 2'b00: EDP, 2'b01: MIPI, 2'b10: DP, 2'b11: HDMI
    UINT16  Reserved       :1;     // [15]      - RESERVED
  } RvpFields;
} BOARD_ID_INFO;

#endif
