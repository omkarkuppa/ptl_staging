/** @file
  This file contains code related to initializing and configuring the DDRIO Data Partition

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

#ifndef MRC_DDR_IO_PTL_H_
#define MRC_DDR_IO_PTL_H_


extern const INT8 RxRdEnSrepCntOffset[MAX_SYS_CHANNEL];

extern const INT8 DqRxEqTableDdr5[MAX_RANK_IN_DIMM][DFE_TAP_NUM];
extern const INT8 DqRxEqTableLp5Type3[MAX_RANK_IN_DIMM][DFE_TAP_NUM];
extern const INT8 DqRxEqTableLp5Type4[MAX_RANK_IN_DIMM][DFE_TAP_NUM];

#define MRC_DACCODE_HI   (1400)
#define MRC_DACCODE_TARG (1100)
#define MRC_DACCODE_LOW  (800)

/// Valid safemodes for PTL
#define VALID_DDR_SAFE_MODES_1           (DDR_SAFE_SPINEGATE | DDR_SAFE_DCC | DDR_SAFE_DCC_CLK | DDR_SAFE_PERIODIC_COMP | DDR_SAFE_COMP_CLK)
#define VALID_DDR_SAFE_MODES_2           (DDR_SAFE_CCC_PATH | DDR_SAFE_DQ_CLK | DDR_SAFE_DQ_RETRAIN | DDR_SAFE_RX_PATH | DDR_SAFE_TX_PATH)
#define VALID_DDR_SAFE_MODES_3           (DDR_SAFE_DIGCLK | DDR_SAFE_PERIODIC_RXDQSCOMP | DDR_SAFE_WAKEUP3 | DDR_SAFE_PHCLK)

#define VALID_DDR_SAFE_MODES             (VALID_DDR_SAFE_MODES_1 | VALID_DDR_SAFE_MODES_2 | VALID_DDR_SAFE_MODES_3)

typedef enum {
  SelLvrVccIog,
  SelLvrVccDdq,
  SelLvrVccClk,
  SelLvrVccMax
} SelLvrSetting;

#endif // MRC_DDR_IO_PTL_H_
