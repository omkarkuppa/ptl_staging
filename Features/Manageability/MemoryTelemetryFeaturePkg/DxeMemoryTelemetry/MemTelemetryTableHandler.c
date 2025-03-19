/** @file
  Dxe Memory Telemetry Feature Table Handler Functions

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
#include "MemoryDefinitions.h"
#include <MemInfoHob.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/SmBios.h>

/**
  Encodes the MRC memory type and device type to the form factor.
  Matching the SMBIOS type 17 form factor encoding style.

  @param[in]  ModuleType      Memory module type from MRC HOB
  @param[in]  DramDeviceType  DRAM device type from MRC HOB

  @retval     FormFactorVal   Form factor value encoded
**/
UINT8
GetFormFactor (
  IN UINT8  ModuleType,
  IN UINT8  DramDeviceType
  )
{
  if (DramDeviceType == DDR_DTYPE_LPDDR5 || DramDeviceType == DDR_DTYPE_LPDDR5X) {
    switch (ModuleType & DDR_MTYPE_SPD_MASK) {
      case DDR_MTYPE_LPDIMM_LP5:
      case DDR_MTYPE_CAMM:
        return MEMORY_FORM_FACTOR_CAM;
      case DDR_MTYPE_MEM_DOWN_LP5:
      default:
        return MemoryFormFactorRowOfChips;
    }
  }
  switch (ModuleType & DDR_MTYPE_SPD_MASK) {
    case DDR_MTYPE_SODIMM:
      ///
      /// Legacy non-JEDEC LPDDR3 SPD images use SODIMM module type which should be soldered down form factor.
      ///
      return (DramDeviceType == DDR_DTYPE_LPDDR3) ? MemoryFormFactorRowOfChips : MemoryFormFactorSodimm;
    case DDR_MTYPE_RDIMM:
      return (UINT8) MemoryFormFactorRimm;
    case DDR_MTYPE_CSODIMM:
      return MemoryFormFactorSodimm;
    case DDR_MTYPE_MEM_DOWN:
      return (UINT8) MemoryFormFactorRowOfChips;
    case DDR_MTYPE_UDIMM:
    case DDR_MTYPE_LR_DIMM:
    case DDR_MTYPE_CUDIMM:
    case DDR_MTYPE_MRDIMM:
    case DDR_MTYPE_DDIMM:
    default:
      return (UINT8) MemoryFormFactorDimm;
  }
}

/**
  Function to create and fill memory telemetry module table data

  @param[in]     MemInfoData     Pointer to the Memory Info Data HOB
  @param[in,out] ModulesTable    Pointer to the Memory Module Table

  @retval     EFI_SUCCESS     Table created and filled successfully
  @retval     Others          Failed to create or fill table
**/
EFI_STATUS
FillMemoryModuleTable (
  IN     MEMORY_INFO_DATA_HOB           *MemInfoData,
  IN OUT MEMORY_TELEMETRY_MODULE_TABLE  *ModulesTable
  )
{
  UINT8               NodeIdx;
  UINT8               ChannelIdx;
  UINT8               DimmIdx;
  UINT8               ModuleType;
  UINT8               DramDeviceType;
  UINT8               MemoryNodeIndex;
  MEMORY_MODULE_ENTRY *MemoryData;

  MemoryNodeIndex = 0;

  if (MemInfoData == NULL || ModulesTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Set the data table for modules to the end of the header
  MemoryData = (MEMORY_MODULE_ENTRY *) ((UINT8 *) ModulesTable + sizeof (MEMORY_TELEMETRY_MODULE_TABLE));

  // Fill the memory module table
  for (NodeIdx = 0; NodeIdx < MAX_NODE; NodeIdx++) {
    if (MemInfoData->Controller[NodeIdx].Status == CONTROLLER_NOT_PRESENT) {
      continue;
    }
    for (ChannelIdx = 0; ChannelIdx < MAX_CH; ChannelIdx++) {
      if (MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].Status == CHANNEL_NOT_PRESENT) {
        continue;
      }
      for (DimmIdx = 0; DimmIdx < MAX_DIMM; DimmIdx++) {
        if (MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].Status == DIMM_NOT_PRESENT) {
          continue;
        }
        // Record location of memory device
        MemoryData[MemoryNodeIndex].Location.Node    = NodeIdx;
        MemoryData[MemoryNodeIndex].Location.Channel = ChannelIdx;
        MemoryData[MemoryNodeIndex].Location.Dimm    = DimmIdx;

        // Record memory data
        MemoryData[MemoryNodeIndex].MemoryType  = MemInfoData->MemoryType;
        MemoryData[MemoryNodeIndex].MfgId       = MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].MfgId;
        MemoryData[MemoryNodeIndex].Speed       = MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].Speed;
        MemoryData[MemoryNodeIndex].RankInDimm  = MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].RankInDimm;
        MemoryData[MemoryNodeIndex].Banks       = MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].Banks;
        MemoryData[MemoryNodeIndex].BankGroups  = MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].BankGroups;

        // Set the encoding for memory form factor align with SMBIOS Type 17
        DramDeviceType = MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].SpdDramDeviceType;
        ModuleType = MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[DimmIdx].SpdModuleType;
        MemoryData[MemoryNodeIndex].FormFactor  = GetFormFactor(ModuleType, DramDeviceType);
        MemoryNodeIndex++;
      }
    }
  }

  // Assign the filled ModulesTable to the MainTable
  ModulesTable->Header.TableID      = MEMORY_TELEMETRY_MODULE_TABLE_ID;
  ModulesTable->Header.Revision     = MEMORY_TELEMETRY_MODULE_TABLE_REVISION;
  ModulesTable->Header.TableLength  = (UINT16) sizeof (MEMORY_TELEMETRY_MODULE_TABLE) + MemoryNodeIndex * sizeof (MEMORY_MODULE_ENTRY);
  ModulesTable->ModuleEntryCount    = MemoryNodeIndex;
  ModulesTable->ModuleEntryLength   = sizeof (MEMORY_MODULE_ENTRY);

  return EFI_SUCCESS;
}

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
  )
{
  UINT16      Idx;
  UINT16      NumSaGvPointsEnabled;
  SAGV_ENTRY  *SagvData;

  if (MemInfoData == NULL || SagvTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Number of points to add to the table
  NumSaGvPointsEnabled = (UINT16) MemInfoData->SagvConfigInfo.NumSaGvPointsEnabled;

  // Set data to end of header
  SagvData = (SAGV_ENTRY *) ((UINT8 *) SagvTable + sizeof (MEMORY_TELEMETRY_SAGV_TABLE));

  // Fill the SaGv table
  SagvTable->SagvPointMask      = MemInfoData->SagvConfigInfo.SaGvPointMask;
  SagvTable->MaxFrequency       = MemInfoData->MaximumMemoryClockSpeed;

  for (Idx = 0; Idx < NumSaGvPointsEnabled; Idx++) {
    SagvData[Idx].DataRate      = MemInfoData->SagvConfigInfo.SaGvTiming[Idx].DataRate;
    SagvData[Idx].Bandwidth     = MemInfoData->SagvConfigInfo.SaGvTiming[Idx].MaxMemoryBandwidth;
  }

  // Set Header data
  SagvTable->Header.TableID     = MEMORY_TELEMETRY_SAGV_TABLE_ID;
  SagvTable->Header.Revision    = MEMORY_TELEMETRY_SAGV_TABLE_REVISION;
  SagvTable->Header.TableLength = sizeof (MEMORY_TELEMETRY_SAGV_TABLE) + NumSaGvPointsEnabled * sizeof (SAGV_ENTRY);
  SagvTable->SagvEntryCount     = (UINT8) NumSaGvPointsEnabled;
  SagvTable->SagvEntryLength    = sizeof (SAGV_ENTRY);

  return EFI_SUCCESS;
}

/**
  Function to create and fill MEMORY_TELEMETRY_AMT_PPR_TABLE

  @param[in]     MemInfoData     Pointer to the Memory Info Data HOB
  @param[in,out] AmtPprTable     Pointer to the AMT PPR Table

  @retval EFI_SUCCESS     Table created and filled successfully
  @retval Others          Failed to create or fill table
**/
EFI_STATUS
FillAmtPprTable (
  IN     MEMORY_INFO_DATA_HOB           *MemInfoData,
  IN OUT MEMORY_TELEMETRY_AMT_PPR_TABLE *AmtPprTable
  )
{
  UINT16    NumOfPprResultsHobs;

  NumOfPprResultsHobs   = 0;

  if (MemInfoData == NULL || AmtPprTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Fill the AMT PPR table
  AmtPprTable->PprAvailable                        = (UINT8) FixedPcdGetBool (PcdPprCapability);
  AmtPprTable->AmtPprRunStatus.AmtRanLastBoot      = (MemInfoData->PprRanInLastBoot & BIT1) >> 1;
  AmtPprTable->AmtPprRunStatus.PprRanLastBoot      = MemInfoData->PprRanInLastBoot & BIT0;
  if (AmtPprTable->AmtPprRunStatus.AmtRanLastBoot == RAN_LAST_BOOT) {
    if (AmtPprTable->AmtPprRunStatus.PprRanLastBoot == RAN_LAST_BOOT) {
      AmtPprTable->TotalPprRowRepairsSuccessful    = MemInfoData->PprRepairsSuccessful;
      AmtPprTable->TotalUncorrectableDevices       = MemInfoData->PprRepairFails;
    }
    AmtPprTable->TotalRowFailuresDiscovered        = MemInfoData->PprDetectedErrors;

    // Get the number of PPR Results Hobs if greater than 0 append data to end of table
    NumOfPprResultsHobs = 1;
    CopyMem ((UINT8 *)AmtPprTable + sizeof (MEMORY_TELEMETRY_AMT_PPR_TABLE), &MemInfoData->PprErrorInfo, NumOfPprResultsHobs * sizeof (PPR_RESULT_COLUMNS_HOB));
  }

  // Set Header data
  AmtPprTable->Header.TableID           = MEMORY_TELEMETRY_AMT_PPR_TABLE_ID;
  AmtPprTable->Header.Revision          = MEMORY_TELEMETRY_AMT_PPR_TABLE_REVISION;
  AmtPprTable->Header.TableLength       = sizeof (MEMORY_TELEMETRY_AMT_PPR_TABLE) + NumOfPprResultsHobs * sizeof (PPR_RESULT_COLUMNS_HOB);
  AmtPprTable->PprResultsHobCount       = (UINT8) NumOfPprResultsHobs;
  AmtPprTable->PprResultsHobLength      = sizeof (PPR_RESULT_COLUMNS_HOB);

  return EFI_SUCCESS;
}

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
  )
{
  UINT8               NodeIdx;
  UINT8               ChannelIdx;
  UINT8               RankIdx;
  UINT8               ChipIdx;
  UINT8               TableCount;
  RANK_RESOURCE_ENTRY *RankResourceData;

  TableCount = 0;

  if (MemInfoData == NULL || RankResourceTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Set data to end of header
  RankResourceData = (RANK_RESOURCE_ENTRY *) ((UINT8 *) RankResourceTable + sizeof (MEMORY_TELEMETRY_RANK_RESOURCE_TABLE));

  // Fill the memory module table
  for (NodeIdx = 0; NodeIdx < MAX_NODE; NodeIdx++) {
    if (MemInfoData->Controller[NodeIdx].Status == CONTROLLER_NOT_PRESENT) {
      continue;
    }
    for (ChannelIdx = 0; ChannelIdx < MAX_CH; ChannelIdx++) {
      if (MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].Status == CHANNEL_NOT_PRESENT) {
        continue;
      }
      for (RankIdx = 0; RankIdx < _MAX_RANK_IN_CHANNEL; RankIdx++) {
        if (MemInfoData->Controller[NodeIdx].ChannelInfo[ChannelIdx].DimmInfo[RankIdx/2].Status == DIMM_NOT_PRESENT) {
          continue;
        }
        RankResourceData[TableCount].Location.Node    = NodeIdx;
        RankResourceData[TableCount].Location.Channel = ChannelIdx;
        RankResourceData[TableCount].Location.Dimm    = RankIdx / 2;
        RankResourceData[TableCount].Location.Rank    = RankIdx;

        // if _MAX_SDRAM_IN_DIMM > size of ChipResource, then update the ChipResource size
        for (ChipIdx = 0; ChipIdx < _MAX_SDRAM_IN_DIMM; ChipIdx++) {
          RankResourceData[TableCount].ChipResource[ChipIdx] = MemInfoData->PprAvailableResources[NodeIdx][ChannelIdx][RankIdx][ChipIdx];
        }
        RankResourceData[TableCount].ChipCount = _MAX_SDRAM_IN_DIMM;
        TableCount++;
      }
    }
  }

  // Set Header data
  RankResourceTable->Header.TableID       = MEMORY_TELEMETRY_RANK_RESOURCE_TABLE_ID;
  RankResourceTable->Header.Revision      = MEMORY_TELEMETRY_RANK_RESOURCE_TABLE_REVISION;
  RankResourceTable->Header.TableLength   = sizeof (MEMORY_TELEMETRY_RANK_RESOURCE_TABLE) + TableCount * sizeof (RANK_RESOURCE_ENTRY);
  RankResourceTable->ResourceEntryCount   = TableCount;
  RankResourceTable->ResourceEntryLength  = sizeof (RANK_RESOURCE_ENTRY);

  return EFI_SUCCESS;
}
