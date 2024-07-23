/** @file
  decode encode registers defines

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


#ifndef DECODER_REGISTERS_H
#define DECODER_REGISTERS_H

//Register field ranges
#define MAD_INTER_CHANNEL_DDR_TYPE        1,0
#define MAD_INTER_CHANNEL_ECC             2,2
#define MAD_INTER_CHANNEL_CH0_SIZE        15,8
#define MAD_INTER_CHANNEL_CH1_SIZE        23,16
#define MAD_INTRA_CHANNEL_SUBCH_0_DENSITY 3,0
#define MAD_INTRA_CHANNEL_SUBCH_0_WIDTH   6,6
#define MAD_INTRA_CHANNEL_SUBCH_0_RANKS   7,7
#define MAD_INTRA_CHANNEL_SUBCH_1_DENSITY 11,8
#define MAD_INTRA_CHANNEL_SUBCH_1_WIDTH   14,14
#define MAD_INTRA_CHANNEL_SUBCH_1_RANKS   15,15
#define MAD_MC_HASH_HASH_ENABLED          0,0
#define MAD_MC_HASH_HASH_LSB              3,1
#define MAD_MC_HASH_ZONE1_START           29,20
#define MAD_CHANNEL_HASH_HASH_MASK        19,6
#define MAD_CHANNEL_HASH_HASH_LSB         26,24
#define MAD_SUB_CHANNEL_HASH_HASH_MASK    19,6
#define MAD_SUB_CHANNEL_HASH_HASH_LSB     26,24
#define MAD_DRAM_HASH_EIM                 0,0
#define MAD_DRAM_HASH_EBH                 1,1
#define MAD_DRAM_HASH_BG0                 6,4
#define MAD_DRAM_HASH_BG1                 10,8
#define MAD_DRAM_HASH_BG2                 14,12

#define REMAP_BASE                        41,20
#define REMAP_LIMIT                       41,20
#define TOLUD                             41,20

#define MEMORY_SLICE_HASH_HASH_ENABLED    0,0
#define MEMORY_SLICE_HASH_HASH_LSB        3,1
#define MEMORY_SLICE_HASH_HASH_MASK       19,6
#define MEMORY_SLICE_HASH_ZONE1_START     29,20
#define MEMORY_SLICE_HASH_SLICE_L_ID      30,30

#endif //DECODER_REGISTERS_H

