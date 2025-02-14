/** @file
  Pei Memory Telemetry Amt Ppr Library Definitions

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

@par Specification Reference:
**/

#ifndef _PEI_MEMORY_TELEMETRY_H_
#define _PEI_MEMORY_TELEMETRY_H_

#include <PiPei.h>

///
/// MRC Memory Types API matching definitions
///
#define NOREPAIR_PPR      0
#define HARD_PPR          2
#define PPR_TEST          6

///
/// Defined for variable settings
#define DISABLED          0
#define ENABLED           1
#define AMT_ONLY_ENABLED  2
#define AMT_PPR_ENABLED   3

/**
  Entry point of the Memory Telemetry PEIM
  Checks the AMT PPR enable variable status, will update memory config accordingly.

  @retval     EFI_SUCCESS       PEI Memory Telemetry executed as expected
  @retval     Others            PEI Memory Telemetry failed to get and set AmtPprEnable variable
**/
VOID
MemTelemetryAmtPprVarUpdate (
  VOID
  );
#endif // _PEI_MEMORY_TELEMETRY_H_
