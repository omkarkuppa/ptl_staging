/** @file
  Intel Dxe Memory Telemetry Data Subtable Handler Definitions.

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

#ifndef _MEM_TELEMETRY_TABLE_HANDLER_H_
#define _MEM_TELEMETRY_TABLE_HANDLER_H_

#include <PiDxe.h>
#include "MemTelemetryTables.h"

/**
  Encodes the MRC memory type and device type to the form factor.
  Matching the SMBIOS type 17 form factor encoding style.

  @param[out] ModuleType      Memory module type from MRC HOB
  @param[out] DramDeviceType  DRAM device type from MRC HOB

  @retval     Form Factor     Form factor value encoded
**/
UINT8
GetFormFactor (
  IN UINT8  ModuleType,
  IN UINT8  DramDeviceType
  );

/**
  Function to create and fill MEMORY_TELEMETRY_MODULE_TABLE

  @param[in]  MemInfoData     Pointer to the Memory Info Data HOB
  @param[in,out] ModulesTable    Pointer to the Memory Module Table

  @retval     EFI_SUCCESS     Table created and filled successfully
  @retval     Others          Failed to create or fill table
**/
EFI_STATUS
FillMemoryModuleTable (
  IN     MEMORY_INFO_DATA_HOB           *MemInfoData,
  IN OUT MEMORY_TELEMETRY_MODULE_TABLE  *ModulesTable
  );

/**
  Function to create and fill MEMORY_TELEMETRY_SAGV_TABLE

  @param[in]     MemInfoData     Pointer to the Memory Info Data HOB
  @param[in,out] SagvTable       Pointer to the SaGv Table

  @retval EFI_SUCCESS     Table created and filled successfully
  @retval Others          Failed to create or fill table
**/
EFI_STATUS
FillSagvTable (
  IN     MEMORY_INFO_DATA_HOB         *MemInfoData,
  IN OUT MEMORY_TELEMETRY_SAGV_TABLE  *SagvTable
  );

/**
  Function to create and fill MEMORY_TELEMETRY_AMT_PPR_TABLE

  @param[in]  MemInfoData     Pointer to the Memory Info Data HOB
  @param[in,out] AmtPprTable     Pointer to the AMT PPR Table

  @retval EFI_SUCCESS     Table created and filled successfully
  @retval Others          Failed to create or fill table
**/
EFI_STATUS
FillAmtPprTable (
  IN     MEMORY_INFO_DATA_HOB           *MemInfoData,
  IN OUT MEMORY_TELEMETRY_AMT_PPR_TABLE *AmtPprTable
  );

/**
  Function to create and fill MEMORY_TELEMETRY_RANK_RESOURCE_TABLE

  @param[in]     MemInfoData       Pointer to the Memory Info Data HOB
  @param[in,out] RankResourceTable Pointer to the Rank Resource Table

  @retval EFI_SUCCESS     Table created and filled successfully
  @retval Others          Failed to create or fill table
**/
EFI_STATUS
FillRankResourceTable (
  IN     MEMORY_INFO_DATA_HOB                 *MemInfoData,
  IN OUT MEMORY_TELEMETRY_RANK_RESOURCE_TABLE *RankResourceTable
  );

#endif // _MEM_TELEMETRY_TABLE_HANDLER_H_
