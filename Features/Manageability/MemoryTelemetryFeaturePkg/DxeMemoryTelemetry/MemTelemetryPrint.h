/** @file
  Memory Telemetry Feature Table Print Function Definitons
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

@par Specification Reference:
**/

#ifndef _MEM_TELEMETRY_PRINT_H_
#define _MEM_TELEMETRY_PRINT_H_

#include "MemTelemetryTables.h"

/**
  Prints the received memory module telemetry data to serial debug log.

  @param[in] ModulesTable  Memory Telemetry Module Table to print
**/
VOID
PrintMemoryModuleTable (
  IN MEMORY_TELEMETRY_MODULE_TABLE *ModulesTable
  );

/**
  Prints the received SAGV telemetry data to serial debug log.

  @param[in] SagvTable  Pointer to the SaGv Table
**/
VOID
PrintSagvTable (
  IN MEMORY_TELEMETRY_SAGV_TABLE *SagvTable
  );

/**
  Prints the received AMT PPR telemetry data to serial debug log.

  @param[in] AmtPprTable  Pointer to the AMT PPR Table
**/
VOID
PrintAmtPprTable (
  IN MEMORY_TELEMETRY_AMT_PPR_TABLE *AmtPprTable
  );

/**
  Prints the received rank resource telemetry data to serial debug log.

  @param[in] RankResourceTable  Pointer to the Rank Resource Table
**/
VOID
PrintRankResourceTable (
  IN MEMORY_TELEMETRY_RANK_RESOURCE_TABLE *RankResourceTable
  );

#endif // _MEM_TELEMETRY_PRINT_H_
