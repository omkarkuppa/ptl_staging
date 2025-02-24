/** @file
  THC Board Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#ifndef _THC_BOARD_DATA_H_
#define _THC_BOARD_DATA_H_

#define MAX_THC_PER_PORT_TABLE_INDEX        5
#define MAX_THC_PORT                        2

typedef struct {
  UINT8      Count;
  UINT8      ThcSlaveAddress [MAX_THC_PORT][MAX_THC_PER_PORT_TABLE_INDEX];
  UINT8      Reserved[4];
} VPD_THC_SLAVE_ADDRESS_TABLE;

#endif //_THC_BOARD_DATA_H_
