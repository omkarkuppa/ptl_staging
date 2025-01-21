/** @file
  decode encode tables

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


#ifndef DECODER_TABLES_H
#define DECODER_TABLES_H

#include <Ptl/ebh_tables.h>


//DRAM decode bit ranges (based on sub channel address)
#define RANGE_LPDDR5_RANK         15
#define RANGE_LPDDR5_BG0          12
#define RANGE_LPDDR5_BG1          11
#define RANGE_LPDDR5_BA0          13
#define RANGE_LPDDR5_BA1          14
#define RANGE_LPDDR5_1R_ROW_MSB   32
#define RANGE_LPDDR5_1R_ROW_LSB   15
#define RANGE_LPDDR5_2R_ROW_MSB   33
#define RANGE_LPDDR5_2R_ROW_LSB   16
#define RANGE_LPDDR5_COL_MSB      10
#define RANGE_LPDDR5_COL_LSB      1
#define RANGE_DDR5_X16_RANK       16
#define RANGE_DDR5_X8_RANK        17
#define RANGE_DDR5_BG0            12
#define RANGE_DDR5_BG1            13
#define RANGE_DDR5_X8_BG2         14
#define RANGE_DDR5_X16_BA0        14
#define RANGE_DDR5_X16_BA1        15
#define RANGE_DDR5_X8_BA0         15
#define RANGE_DDR5_X8_BA1         16
#define RANGE_DDR5_X16_1R_ROW_MSB 32
#define RANGE_DDR5_X16_1R_ROW_LSB 16
#define RANGE_DDR5_X16_2R_ROW_LSB 17
#define RANGE_DDR5_X16_2R_ROW_MSB 33
#define RANGE_DDR5_X8_1R_ROW_MSB  33
#define RANGE_DDR5_X8_1R_ROW_LSB  17
#define RANGE_DDR5_X8_2R_ROW_MSB  34
#define RANGE_DDR5_X8_2R_ROW_LSB  18
#define RANGE_DDR5_COL_MSB        11
#define RANGE_DDR5_COL_LSB        2

//EIM XORs
#define EIM_LPDDR5_1R_BA0         15
#define EIM_LPDDR5_1R_BA1         16
#define EIM_LPDDR5_2R_BA0         16
#define EIM_LPDDR5_2R_BA1         17
#define EIM_LPDDR5_RANK           18
#define EIM_DDR5_X16_1R_BA0       16
#define EIM_DDR5_X16_1R_BA1       17
#define EIM_DDR5_X16_2R_BA0       17
#define EIM_DDR5_X16_2R_BA1       18
#define EIM_DDR5_X16_RANK         19
#define EIM_DDR5_X8_1R_BA0        17
#define EIM_DDR5_X8_1R_BA1        18
#define EIM_DDR5_X8_2R_BA0        18
#define EIM_DDR5_X8_2R_BA1        19
#define EIM_DDR5_X8_RANK          20

#endif //DECODER_TABLES_H
