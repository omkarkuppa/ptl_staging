/** @file
  This file contains common definitions, enums, and structs for CPGC 2.0.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _CPGC_20_H_
#define _CPGC_20_H_

/// Definitions

/// Bank and Bank Group definitions
//@todo confirm that these are correct
#define CPGC20_MAX_BANKS_PER_CHANNEL            (32)
#define CPGC20_BANK_GROUP_FIELD_OFFSET_DDR5     (2)
#define CPGC20_BANK_GROUP_FIELD_OFFSET_LPDDR5   (2)
#define CPGC20_BANK_FIELD_OFFSET                (0)
#define CPGC20_MAX_ADDRESS_INSTRUCTION          (4)
#define CPGC20_MAX_ALGORITHM_INSTRUCTION        (8)
#define CPGC20_MAX_COMMAND_INSTRUCTION          (24)
#define CPGC20_MAX_DATA_INSTRUCTION             (4)
#define CPGC22_BITS_PER_ERROR_STATUS            (4)
#define CPGC22_ECC_ERROR_STATUS                 (MRC_BIT16 | MRC_BIT17)
#define CPGC20_ECC_CLEAN_LC                     (4)

#define CPGC20_NUM_RASTER_REPO_CONTENT_ECC  (2)
#define CPGC20_NUM_RASTER_REPO_CONTENT_REG  (8)
#define CPGC20_MAX_RASTER_REPO_CONTENT      (10) // 8 Entries + 2 ECC
#define CPGC20_EVENT_WAIT_TIMER_DISABLE     (0xF)

#define CPGC_BASE_READ_SUBSEQ          (0) // 0000: Base read
#define CPGC_BASE_WRITE_SUBSEQ         (1) // 0001: Base Write

#define CPGC_NO_SUBSEQ1                (0xFF)  // do not use subseq1

/// CPGC Modes
typedef enum {
  CPGC20_IDLE_MODE,
  CPGC20_ACTIVE_MODE
} CPGC20_INIT_MODES;

/// CPGC Address Increment Order
typedef enum {
  CPGC20_ROW_COL_2_BANK_2_RANK,
  CPGC20_BANK_2_ROW_COL_2_RANK,
  CPGC20_ROW_COL_2_RANK_2_BANK,
  CPGC20_RANK_2_ROW_COL_2_BANK,
  CPGC20_BANK_2_RANK_2_ROW_COL,
  CPGC20_RANK_2_BANK_2_ROW_COL
} CPGC20_ADDRESS_INCREMENT_ORDER;

/// CPGC Address Direction
typedef enum {
  CPGC20_FAST_Y,
  CPGC20_DIAGONAL,
  CPGC20_FAST_X,
  CPGC20_DIAGONAL2,
  CPGC20_INVERSE_FAST_Y,
  CPGC20_INVERSE_DIAGONAL,
  CPGC20_INVERSE_FAST_X,
  CPGC20_INVERSE_DIAGONAL2
} CPGC20_ADDRESS_DIRECTION;

/// CPGC Algorithm Wait Clock Frequency
typedef enum {
  CPGC20_1GHZ_FREQ,
  CPGC20_1MHZ_FREQ,
  CPGC20_1KHZ_FREQ,
  CPGC20_NATIVE_DUNIT_FREQ
} CPGC20_ALGO_WAIT_CLOCK_FREQ;

#endif

