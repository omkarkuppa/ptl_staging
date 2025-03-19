/** @file
  Dxe Memory Telemetry Feature Implementation

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

#include "DxeMemoryTelemetry.h"
#include "MemoryDefinitions.h"
#include "MemTelemetryPrint.h"
#include "MemTelemetryTableHandler.h"
#include <MemInfoHob.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PhatAcpiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Acpi64.h>
#include <IndustryStandard/SmBios.h>

/**
  Returns a pointer to the Memory Info Data Hob

  @retval     Pointer to the MEMORY_INFO_DATA_HOB
  @retval     NULL, Failed to get memory info data
**/
STATIC
MEMORY_INFO_DATA_HOB *
GetMemInfo (
  VOID
  )
{
  EFI_HOB_GUID_TYPE   *GuidHob;

  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);

  if (GuidHob == NULL) {
    DEBUG((DEBUG_WARN, "[%a] Failed to get Si memory info data GUID Hob\n", __FUNCTION__));
    return NULL;
  }
  return (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
}

/**
  Format MRC Memory Info Data structure for ACPI publishing.

  @param[in]     MemInfoData  Memory info data hob structure pointer
  @param[in,out] MainTable    Configured data to MEMORY_TELEMETRY table and PHAT append

  @retval     EFI_SUCCESS     Data set successfully
  @retval     Others          Failed to set data to new table
**/
EFI_STATUS
MemoryTelemetryDataTableHandler (
  IN     MEMORY_INFO_DATA_HOB              *MemInfoData,
  IN OUT MEMORY_TELEMETRY_MAIN_DATA_TABLE  *MainTable
  )
{
  EFI_STATUS                            Status;
  UINT16                                TableOffset;
  MEMORY_TELEMETRY_MODULE_TABLE         *ModulesTable;
  MEMORY_TELEMETRY_SAGV_TABLE           *SagvTable;
  MEMORY_TELEMETRY_AMT_PPR_TABLE        *AmtPprTable;
  MEMORY_TELEMETRY_RANK_RESOURCE_TABLE  *RankResourceTable;

  ModulesTable      = NULL;
  SagvTable         = NULL;
  AmtPprTable       = NULL;
  RankResourceTable = NULL;

  if (MemInfoData == NULL || MainTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Set the table offset to the start of the first table
  TableOffset = sizeof (MEMORY_TELEMETRY_MAIN_DATA_TABLE);

  ///
  /// Fill the memory module table
  ///
  ModulesTable = (MEMORY_TELEMETRY_MODULE_TABLE *) ((UINT8 *) MainTable + TableOffset);
  Status = FillMemoryModuleTable (MemInfoData, ModulesTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Memory Module Table Handler failed with status - %r\n", Status));
  } else {
    TableOffset += ModulesTable->Header.TableLength;
  }
  PrintMemoryModuleTable (ModulesTable);

  ///
  /// Fill the SAGV table data
  ///
  SagvTable = (MEMORY_TELEMETRY_SAGV_TABLE *) ((UINT8 *) MainTable + TableOffset);
  Status = FillSagvTable (MemInfoData, SagvTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "SAGV Table Handler failed with status - %r\n", Status));
  } else {
    TableOffset += SagvTable->Header.TableLength;
  }
  PrintSagvTable (SagvTable);

  ///
  /// Add AMT PPR data to the Memory Telemetry Data Table
  ///
  AmtPprTable = (MEMORY_TELEMETRY_AMT_PPR_TABLE *) ((UINT8 *) MainTable + TableOffset);
  Status = FillAmtPprTable (MemInfoData, AmtPprTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "AMT PPR Table Handler failed with status - %r\n", Status));
  } else {
    TableOffset += AmtPprTable->Header.TableLength;
  }
  PrintAmtPprTable (AmtPprTable);

  ///
  /// Add Rank Resource to the Memory Telemetry Data Table
  ///
  if (AmtPprTable->AmtPprRunStatus.AmtRanLastBoot == RAN_LAST_BOOT) {
    RankResourceTable = (MEMORY_TELEMETRY_RANK_RESOURCE_TABLE *) ((UINT8 *) MainTable + TableOffset);
    Status = FillRankResourceTable (MemInfoData, RankResourceTable);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "Rank Resource Table Handler failed with status - %r\n", Status));
    } else {
      TableOffset += RankResourceTable->Header.TableLength;
    }
    PrintRankResourceTable (RankResourceTable);
  }

  // Update the main header's table length with offset
  MainTable->Header.TableID     = MEMORY_TELEMETRY_MAIN_TABLE_ID;
  MainTable->Header.Revision    = MEMORY_TELEMETRY_MAIN_TABLE_REVISION;
  MainTable->Header.TableLength = TableOffset;

  return Status;
}

/**
  Entry point of the Dxe Memory Telemetry
  Checks for support, and begins the PHAT table information update.

  @param[in] ImageHandle      Handle for this drivers loaded image protocol.
  @param[in] SystemTable      EFI system table.

  @retval   EFI_SUCCESS       The protocol has installed successfully
  @retval   EFI_UNSUPPORTED   The protocol is not supported
  @retval   Others            The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
DxeMemoryTelemetryEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                              Status;
  MEMORY_INFO_DATA_HOB                    *MemInfoData;
  MEMORY_TELEMETRY_MAIN_DATA_TABLE        *MemoryTelemetryDataTable;
  PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE  *PhatTable;

  PhatTable    = NULL;
  MemInfoData  = NULL;

  DEBUG ((DEBUG_INFO, "[%a] Entering\n", __FUNCTION__));

  ///
  /// Manage AMT PPR variable, must execute even if memory telemetry table cannot allocate
  ///
  AmtPprVarHandler ();

  ///
  /// Get MRC Data Hob
  ///
  MemInfoData = GetMemInfo ();
  if (MemInfoData == NULL) {
    Status = EFI_ABORTED;
    goto Exit;
  }

  ///
  /// Allocate the PHAT Memory Telemetry Table
  ///
  PhatTable = (PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE *) AllocateZeroPool (sizeof (PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE) + \
                                                                           sizeof (MEMORY_TELEMETRY_MAIN_DATA_TABLE) + \
                                                                           sizeof (MEMORY_TELEMETRY_MODULE_TABLE) + MAX_NODE * MAX_CH * MAX_DIMM * sizeof (MEMORY_MODULE_ENTRY) + \
                                                                           sizeof (MEMORY_TELEMETRY_SAGV_TABLE) + HOB_MAX_SAGV_POINTS * sizeof (SAGV_ENTRY) + \
                                                                           sizeof (MEMORY_TELEMETRY_AMT_PPR_TABLE) + MAX_RESULTS_HOB_COUNT * sizeof (PPR_RESULT_COLUMNS_HOB) + \
                                                                           sizeof (MEMORY_TELEMETRY_RANK_RESOURCE_TABLE) + MAX_NODE * MAX_CH * _MAX_RANK_IN_CHANNEL * sizeof (RANK_RESOURCE_ENTRY));
  if (PhatTable == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  ///
  /// Initialize table length to header size
  ///
  PhatTable->RecordLength = sizeof (PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE);

  ///
  /// Set memory data to Memory Telemetry PHAT format
  ///
  MemoryTelemetryDataTable = (MEMORY_TELEMETRY_MAIN_DATA_TABLE *) ((UINT8 *) PhatTable + sizeof (PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE));
  Status = MemoryTelemetryDataTableHandler (MemInfoData, MemoryTelemetryDataTable);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  ///
  /// Update the PHAT header
  ///
  PhatTable->PlatformRecordType = MEMORY_TELEMETRY_PHAT_RECORD_TYPE;
  PhatTable->Revision           = MEMORY_TELEMETRY_PHAT_RECORD_VERSION;
  PhatTable->RecordLength       += MemoryTelemetryDataTable->Header.TableLength;

  ///
  /// Table exists, get current table
  ///
  Status = InstallPhatTable (PhatTable, PhatTable->RecordLength);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to install PHAT table with status - %r\n", Status));
  }

  Exit:
  if (PhatTable != NULL) {
    FreePool (PhatTable);
  }
  DEBUG ((DEBUG_INFO, "[%a] Exit status: %r\n", __FUNCTION__, Status));
  return Status;
}
