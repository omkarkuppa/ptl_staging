/** @file
  Memory Telemetry Feature Table Print Functions

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

#include "MemTelemetryTableHandler.h"
#include <MemInfoHob.h>
#include <Library/DebugLib.h>

/**
  Prints the received memory module telemetry data to serial debug log.

  @param[in] ModulesTable  Memory Telemetry Module Table to print
**/
VOID
PrintMemoryModuleTable (
  IN MEMORY_TELEMETRY_MODULE_TABLE *ModulesTable
  )
{
  UINT32              Index;
  MEMORY_MODULE_ENTRY *Entry;

  if (ModulesTable == NULL) {
    DEBUG ((DEBUG_WARN, "Memory Module Table is NULL\n"));
    return;
  }

  DEBUG ((DEBUG_INFO, "Memory Module Table:\n"));
  DEBUG ((DEBUG_INFO, "Table ID: %d\n", ModulesTable->Header.TableID));
  DEBUG ((DEBUG_INFO, "Revision: %d\n", ModulesTable->Header.Revision));
  DEBUG ((DEBUG_INFO, "Table Length: %d\n", ModulesTable->Header.TableLength));
  DEBUG ((DEBUG_INFO, "Module Entry Count: %d\n", ModulesTable->ModuleEntryCount));
  DEBUG ((DEBUG_INFO, "Module Entry Length: %d\n", ModulesTable->ModuleEntryLength));

  Entry = (MEMORY_MODULE_ENTRY *) ((UINT8 *)ModulesTable + sizeof (MEMORY_TELEMETRY_MODULE_TABLE));
  for (Index = 0; Index < ModulesTable->ModuleEntryCount; Index++) {
    DEBUG ((DEBUG_INFO, "Entry %d:\n", Index));
    DEBUG ((DEBUG_INFO, "  Node: %d\n", Entry->Location.Node));
    DEBUG ((DEBUG_INFO, "  Channel: %d\n", Entry->Location.Channel));
    DEBUG ((DEBUG_INFO, "  Dimm: %d\n", Entry->Location.Dimm));
    DEBUG ((DEBUG_INFO, "  Memory Type: %d\n", Entry->MemoryType));
    DEBUG ((DEBUG_INFO, "  MfgId: %d\n", Entry->MfgId));
    DEBUG ((DEBUG_INFO, "  Speed: %d\n", Entry->Speed));
    DEBUG ((DEBUG_INFO, "  Rank In Dimm: %d\n", Entry->RankInDimm));
    DEBUG ((DEBUG_INFO, "  Banks: %d\n", Entry->Banks));
    DEBUG ((DEBUG_INFO, "  Bank Groups: %d\n", Entry->BankGroups));
    DEBUG ((DEBUG_INFO, "  Form Factor: %d\n", Entry->FormFactor));
    Entry++;
  }
}

/**
  Prints the received SAGV telemetry data to serial debug log.

  @param[in] SagvTable  Pointer to the SaGv Table
**/
VOID
PrintSagvTable (
  IN MEMORY_TELEMETRY_SAGV_TABLE *SagvTable
  )
{
  UINT16      Index;
  SAGV_ENTRY  *Entry;

  if (SagvTable == NULL) {
    DEBUG ((DEBUG_WARN, "SAGV Table is NULL\n"));
    return;
  }

  DEBUG ((DEBUG_INFO, "SAGV Table:\n"));
  DEBUG ((DEBUG_INFO, "Table ID: %d\n", SagvTable->Header.TableID));
  DEBUG ((DEBUG_INFO, "Revision: %d\n", SagvTable->Header.Revision));
  DEBUG ((DEBUG_INFO, "Table Length: %d\n", SagvTable->Header.TableLength));
  DEBUG ((DEBUG_INFO, "SAGV Entry Count: %d\n", SagvTable->SagvEntryCount));
  DEBUG ((DEBUG_INFO, "SAGV Entry Length: %d\n", SagvTable->SagvEntryLength));
  DEBUG ((DEBUG_INFO, "SAGV Point Mask: %d\n", SagvTable->SagvPointMask));
  DEBUG ((DEBUG_INFO, "Max Frequency: %d\n", SagvTable->MaxFrequency));

  Entry = (SAGV_ENTRY *) ((UINT8 *)SagvTable + sizeof (MEMORY_TELEMETRY_SAGV_TABLE));
  for (Index = 0; Index < SagvTable->SagvEntryCount; Index++) {
    DEBUG ((DEBUG_INFO, "Entry %d:\n", Index));
    DEBUG ((DEBUG_INFO, "  Data Rate: %d\n", Entry->DataRate));
    DEBUG ((DEBUG_INFO, "  Bandwidth: %d\n", Entry->Bandwidth));
    Entry++;
  }
}

/**
  Prints the received AMT PPR telemetry data to serial debug log.

  @param[in] AmtPprTable  Pointer to the AMT PPR Table
**/
VOID
PrintAmtPprTable (
  IN MEMORY_TELEMETRY_AMT_PPR_TABLE *AmtPprTable
  )
{
  UINT16                  Index;
  PPR_RESULT_COLUMNS_HOB  *Entry;

  if (AmtPprTable == NULL) {
    DEBUG ((DEBUG_WARN, "AMT PPR Table is NULL\n"));
    return;
  }

  DEBUG ((DEBUG_INFO, "AMT PPR Table:\n"));
  DEBUG ((DEBUG_INFO, "Table ID: %d\n", AmtPprTable->Header.TableID));
  DEBUG ((DEBUG_INFO, "Revision: %d\n", AmtPprTable->Header.Revision));
  DEBUG ((DEBUG_INFO, "Table Length: %d\n", AmtPprTable->Header.TableLength));
  DEBUG ((DEBUG_INFO, "PPR Results Hob Count: %d\n", AmtPprTable->PprResultsHobCount));
  DEBUG ((DEBUG_INFO, "PPR Results Hob Length: %d\n", AmtPprTable->PprResultsHobLength));
  DEBUG ((DEBUG_INFO, "PPR Available: %d\n", AmtPprTable->PprAvailable));
  DEBUG ((DEBUG_INFO, "AMT Ran Last Boot: %d\n", AmtPprTable->AmtPprRunStatus.AmtRanLastBoot));
  DEBUG ((DEBUG_INFO, "PPR Ran Last Boot: %d\n", AmtPprTable->AmtPprRunStatus.PprRanLastBoot));
  DEBUG ((DEBUG_INFO, "Total PPR Row Repairs Successful: %d\n", AmtPprTable->TotalPprRowRepairsSuccessful));
  DEBUG ((DEBUG_INFO, "Total Uncorrectable Devices: %d\n", AmtPprTable->TotalUncorrectableDevices));
  DEBUG ((DEBUG_INFO, "Total Row Failures Discovered: %d\n", AmtPprTable->TotalRowFailuresDiscovered));

  Entry = (PPR_RESULT_COLUMNS_HOB *) ((UINT8 *)AmtPprTable + sizeof (MEMORY_TELEMETRY_AMT_PPR_TABLE));
  for (Index = 0; Index < AmtPprTable->PprResultsHobCount; Index++) {
    DEBUG ((DEBUG_INFO, "Entry %d:\n", Index));
    DEBUG ((DEBUG_INFO, "  PPR Row Repairs Successful: %d\n", Entry->PprRowRepairsSuccessful));
    DEBUG ((DEBUG_INFO, "  Controller: %d\n", Entry->Controller));
    DEBUG ((DEBUG_INFO, "  Channel: %d\n", Entry->Channel));
    DEBUG ((DEBUG_INFO, "  Rank: %d\n", Entry->Rank));
    DEBUG ((DEBUG_INFO, "  Bank Group: %d\n", Entry->BankGroup));
    DEBUG ((DEBUG_INFO, "  Bank: %d\n", Entry->Bank));
    DEBUG ((DEBUG_INFO, "  Row: %d\n", Entry->Row));
    DEBUG ((DEBUG_INFO, "  Device: %d\n", Entry->Device));
    Entry++;
  }
}

/**
  Prints the received rank resource telemetry data to serial debug log.

  @param[in] RankResourceTable  Pointer to the Rank Resource Table
**/
VOID
PrintRankResourceTable (
  IN MEMORY_TELEMETRY_RANK_RESOURCE_TABLE *RankResourceTable
  )
{
  UINT8               Index;
  UINT8               ChipIdx;
  RANK_RESOURCE_ENTRY *Entry;

  if (RankResourceTable == NULL) {
    DEBUG ((DEBUG_WARN, "Rank Resource Table is NULL\n"));
    return;
  }

  DEBUG ((DEBUG_INFO, "Rank Resource Table:\n"));
  DEBUG ((DEBUG_INFO, "Table ID: %d\n", RankResourceTable->Header.TableID));
  DEBUG ((DEBUG_INFO, "Revision: %d\n", RankResourceTable->Header.Revision));
  DEBUG ((DEBUG_INFO, "Table Length: %d\n", RankResourceTable->Header.TableLength));
  DEBUG ((DEBUG_INFO, "Resource Entry Count: %d\n", RankResourceTable->ResourceEntryCount));
  DEBUG ((DEBUG_INFO, "Resource Entry Length: %d\n", RankResourceTable->ResourceEntryLength));

  Entry = (RANK_RESOURCE_ENTRY *) ((UINT8 *)RankResourceTable + sizeof (MEMORY_TELEMETRY_RANK_RESOURCE_TABLE));
  for (Index = 0; Index < RankResourceTable->ResourceEntryCount; Index++) {
    DEBUG ((DEBUG_INFO, "Entry %d:\n", Index));
    DEBUG ((DEBUG_INFO, "  Node: %d\n", Entry->Location.Node));
    DEBUG ((DEBUG_INFO, "  Channel: %d\n", Entry->Location.Channel));
    DEBUG ((DEBUG_INFO, "  Dimm: %d\n", Entry->Location.Dimm));
    DEBUG ((DEBUG_INFO, "  Rank: %d\n", Entry->Location.Rank));
    DEBUG ((DEBUG_INFO, "  Chip Count: %d\n", Entry->ChipCount));
    for (ChipIdx = 0; ChipIdx < Entry->ChipCount; ChipIdx++) {
      DEBUG ((DEBUG_INFO, "    Chip Resource %d: %d\n", ChipIdx, Entry->ChipResource[ChipIdx]));
    }
    Entry++;
  }
}