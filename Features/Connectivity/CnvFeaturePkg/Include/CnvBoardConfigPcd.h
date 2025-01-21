/** @file
  CNV Board Config Structural PCD definition..

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _CNV_BOARD_PCD_H_
#define _CNV_BOARD_PCD_H_

#pragma pack(push, 1)

//
// CNV Board Configuration Structure. Data are Specific according to Board Design.
//
typedef struct _CNV_BOARD_CONFIG {
  UINT16    Revision;
  UINT8     WlanRootPortNumber;
  UINT8     DiscreteUsbBtPortNumber;
  UINT8     IntegratedUsbBtPortNumber;
  UINT32    WlanRfKillGpio;
  UINT32    WlanWakeGpio;
  UINT32    WlanRstGpio;
  UINT32    BtRfKillGpio;
  UINT32    BtIrqGpio;
} CNV_BOARD_CONFIG;

//
// CNV Board Configuration Structure PCD Interface
//
typedef struct _CNV_BOARD_CONFIG_PCD {
  CNV_BOARD_CONFIG    CnvBoardConfig;
} CNV_BOARD_CONFIG_PCD;

#pragma pack(pop)

#endif
