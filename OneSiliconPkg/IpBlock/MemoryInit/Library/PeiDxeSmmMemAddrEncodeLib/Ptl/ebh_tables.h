/** @file
  Extended Bank Hashing (EBH) tables

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


#ifndef EBH_TABLES_H
#define EBH_TABLES_H

//PTL EBH XOR masks
#define EBH_DDR5_X16_1R_BG0       0x0444440000
#define EBH_DDR5_X16_1R_BG1       0x0888880000
#define EBH_DDR5_X16_1R_BA0       0x0111100000
#define EBH_DDR5_X16_1R_BA1       0x0222200000
#define EBH_DDR5_X16_2R_BG0       0x0111100000
#define EBH_DDR5_X16_2R_BG1       0x0222200000
#define EBH_DDR5_X16_2R_BA0       0x0444400000
#define EBH_DDR5_X16_2R_BA1       0x0888800000
#define EBH_DDR5_X8_1R_BG0        0x0421080000
#define EBH_DDR5_X8_1R_BG1        0x0842100000
#define EBH_DDR5_X8_1R_BG2        0x0084200000
#define EBH_DDR5_X8_1R_BA0        0x0108400000
#define EBH_DDR5_X8_1R_BA1        0x0210800000
#define EBH_DDR5_X8_2R_BG0        0x0084200000
#define EBH_DDR5_X8_2R_BG1        0x0108400000
#define EBH_DDR5_X8_2R_BG2        0x0210800000
#define EBH_DDR5_X8_2R_BA0        0x0421000000
#define EBH_DDR5_X8_2R_BA1        0x0842000000
#define EBH_LPDDR5_1R_BG0         0x0222220000
#define EBH_LPDDR5_1R_BG1         0x0444440000
#define EBH_LPDDR5_1R_BA0         0x0888880000
#define EBH_LPDDR5_1R_BA1         0x0111100000
#define EBH_LPDDR5_2R_BG0         0x0888880000
#define EBH_LPDDR5_2R_BG1         0x0111100000
#define EBH_LPDDR5_2R_BA0         0x0222200000
#define EBH_LPDDR5_2R_BA1         0x0444400000

#endif //EBH_TABLES_H
