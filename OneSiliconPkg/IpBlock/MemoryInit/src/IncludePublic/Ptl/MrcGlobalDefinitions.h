/** @file
  This file includes definitions for PTL supported configurations

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

#ifndef MRC_GLOBAL_DEFINITIONS_H_
#define MRC_GLOBAL_DEFINITIONS_H_

#define MAX_RANK_IN_CHANNEL       (2)         ///< The maximum number of ranks per channel. PTL Supports 1DPC Only
#define MAX_SDRAM_IN_DIMM         (4)         ///< The maximum number of SDRAMs per DIMM.
#define MAX_DIMMS_IN_CHANNEL      (2)         ///< The maximum number of DIMMs per channel
#define MAX_BYTE_IN_DDR5_CHANNEL  (4)         ///< Max number of Bytes in a DDR5 Channel
#define SUPPORT_ECC               UNSUPPORT   ///< SUPPORT means ECC is suppported, else UNSUPPORT.
// DDR5 CA Training Guardband
#define MRC_CA_PARITY_DDR5_GUARDBAND (4)
#define MRC_DATA_NUM                 MRC_DATA_MOBILE_NUM
#define MRC_DATA_SHARED_NUM          (8)         ///< # of DATA_SHARED Partitions
#define NUM_PHASES                   (8)
#define PH_DCC_SCALE_NUMERATOR       1
#define PH_DCC_SCALE_DENOMINATOR     1
#define PH_DCC_SAMPLES               (3)
#define TICK_CODE_DELTA              31
#define DCC_INIT_FIRST               -16
#define DCC_INIT_SECOND              15
#endif
