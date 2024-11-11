/** @file
  Discrete Thunderbolt (TBT) Software Connection Manager Wrapper for DTBT 0

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "DTbtSwcmWrapper.h"

#undef DTBT_NHI
#undef DTBT_IDX
#undef DTBT_RPS
#undef DTBT_RPT
#undef DTBT_WAK

// NHI ACPI device name
#define DTBT_NHI    NHI0_DEV
// DTBT index
#define DTBT_IDX    0
// Root port number selected in DTBT RP ACPI name
#define DTBT_RPS    \RPI0
// Root port type of DTBT
#define DTBT_RPT    \RPT0
// Wake GPIO pin of DTBT
#define DTBT_WAK    \DWP0

//
// Common ACPI implementation of DTBT for SW CM
//
Include ("DTbtNhiCommon.asl")