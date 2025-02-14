/** @file
  Dxe Memory Telemetry Feature Implementation.

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
#include "MemoryTelemetryTable.h"
#include <MemInfoHob.h>
#include <Library/HobLib.h>
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
  Prints the received memory telemetry data to serial debug log.

  @param[in]  MemoryTelemetryDataTable    Memory Telemetry Table stored from MRC
**/
VOID
PrintMemoryTelemetryTable (
  IN MEMORY_TELEMETRY_DATA_TABLE   *MemoryTelemetryDataTable
  )
{
  UINT8   Idxi;
  UINT8   Idxj;
  UINT8   Idxk;

  DEBUG ((DEBUG_INFO, "|------------- Memory Telemetry Data Table -------------|\n"));
  DEBUG ((DEBUG_INFO, "| Memory Type    0x%X\n", MemoryTelemetryDataTable->MemoryType));
  for (Idxi = 0; Idxi < MAX_NODE; Idxi++) {
    DEBUG ((DEBUG_INFO,     "|\tNode          %d\n", Idxi));
    for (Idxj = 0; Idxj < MAX_CH; Idxj++) {
      DEBUG ((DEBUG_INFO,   "|\tChannel       %d\n", Idxj));
      for (Idxk = 0; Idxk < MAX_DIMM; Idxk++) {
        DEBUG ((DEBUG_INFO, "|\tDIMM          %d\n", Idxk));
        DEBUG ((DEBUG_INFO, "|\t\tManufacture ID        0x%X\n", MemoryTelemetryDataTable->MfgId[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|\t\tMemory Speed          0x%X\n", MemoryTelemetryDataTable->Speed[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|\t\tRank In DIMM          0x%X\n", MemoryTelemetryDataTable->RankInDimm[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|\t\tBank In DIMM          0x%X\n", MemoryTelemetryDataTable->Banks[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|\t\tBank Groups In DIMM   0x%X\n", MemoryTelemetryDataTable->BankGroups[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|\t\tForm Factor           0x%X\n", MemoryTelemetryDataTable->FormFactor[Idxi][Idxj][Idxk]));
      }
      DEBUG ((DEBUG_INFO, "|\tDIMM Count              0x%X\n", MemoryTelemetryDataTable->DimmCount[Idxi][Idxj]));
    }
  }
  for (Idxi = 0; Idxi < HOB_MAX_SAGV_POINTS; Idxi++) {
    DEBUG ((DEBUG_INFO, "| Sa Gv Point        %d\n", Idxi));
    DEBUG ((DEBUG_INFO, "|\tData Rate                   0x%X\n", MemoryTelemetryDataTable->DataRate[Idxi]));
    DEBUG ((DEBUG_INFO, "|\tBandwidth                   0x%X\n", MemoryTelemetryDataTable->Bandwidth[Idxi]));
  }
  DEBUG ((DEBUG_INFO, "| SaGvPointMask               0x%X\n", MemoryTelemetryDataTable->SaGvPointMask));
  DEBUG ((DEBUG_INFO, "| Maximum Frequency           0x%X\n", MemoryTelemetryDataTable->MaxFrequency));
  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
  DEBUG ((DEBUG_INFO, "|---------------------- AMT PPR  -----------------------|\n"));
  DEBUG ((DEBUG_INFO, "| PprAvailable                        0x%X\n", MemoryTelemetryDataTable->PprAvailable));
  DEBUG ((DEBUG_INFO, "| AmtPprRanInLastBoot                 0x%X\n", MemoryTelemetryDataTable->AmtPprRanInLastBoot));
  if (MemoryTelemetryDataTable->AmtPprRanInLastBoot.AmtRanLastBoot) {
    if (MemoryTelemetryDataTable->AmtPprRanInLastBoot.PprRanLastBoot) {
      DEBUG ((DEBUG_INFO, "| Number of Successful Repairs     0x%X\n", MemoryTelemetryDataTable->TotalPprRowRepairsSuccessful));
      DEBUG ((DEBUG_INFO, "| Number of Uncorrectable Devices  0x%X\n", MemoryTelemetryDataTable->TotalUncorrectableDevices));
    }
    DEBUG ((DEBUG_INFO, "| Number of Detected Errors          0x%X\n", MemoryTelemetryDataTable->TotalRowFailuresDiscovered));
    DEBUG ((DEBUG_INFO, "| Error Repair Successful            0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.PprRowRepairsSuccessful));
    DEBUG ((DEBUG_INFO, "| Error Controller                   0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.Controller));
    DEBUG ((DEBUG_INFO, "| Error Channel                      0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.Channel));
    DEBUG ((DEBUG_INFO, "| Error Rank                         0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.Rank));
    DEBUG ((DEBUG_INFO, "| Error Bank Group                   0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.BankGroup));
    DEBUG ((DEBUG_INFO, "| Error Bank                         0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.Bank));
    DEBUG ((DEBUG_INFO, "| Error Row                          0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.Row));
    DEBUG ((DEBUG_INFO, "| Error Device                       0x%X\n", MemoryTelemetryDataTable->PprErrorInfo.Device));
    for (Idxi = 0; Idxi < MAX_NODE; Idxi++) {
      DEBUG ((DEBUG_INFO, "| Node        %d\n", Idxi));
      for (Idxj = 0; Idxj < MAX_CH; Idxj++) {
        DEBUG ((DEBUG_INFO, "|\tChannel     %d\n", Idxj));
        for (Idxk = 0; Idxk < MAX_RANK_IN_CHANNEL; Idxk++) {
          DEBUG ((DEBUG_INFO, "|\tRank        %d\n", Idxk));
          for (UINT8 Idxl = 0; Idxl < MAX_SDRAM_IN_DIMM; Idxl++) {
            DEBUG ((DEBUG_INFO, "|\t\tDevice                         %d\n", Idxl));
            DEBUG ((DEBUG_INFO, "|\t\tAvailable Resources            0x%X\n", MemoryTelemetryDataTable->AvailableResources[Idxi][Idxj][Idxk][Idxl]));
          }
        }
      }
    }
  }
  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
}

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
  )
{
  switch (ModuleType & DDR_MTYPE_SPD_MASK) {
    case DDR_MTYPE_SODIMM:
      ///
      /// Legacy non-JEDEC LPDDR3 SPD images use SODIMM module type which should be soldered down form factor.
      ///
      return (DramDeviceType == DDR_DTYPE_LPDDR3) ? MemoryFormFactorRowOfChips : MemoryFormFactorSodimm;
      break;
    case DDR_MTYPE_RDIMM:
      return (UINT8) MemoryFormFactorRimm;
      break;
    case DDR_MTYPE_CSODIMM:
      return MemoryFormFactorSodimm;
      break;
    case DDR_MTYPE_MEM_DOWN:
      return (UINT8) MemoryFormFactorRowOfChips;
      break;
    case DDR_MTYPE_UDIMM:
    case DDR_MTYPE_LR_DIMM:
    case DDR_MTYPE_CUDIMM:
    case DDR_MTYPE_MRDIMM:
    case DDR_MTYPE_DDIMM:
    default:
      return (UINT8) MemoryFormFactorDimm;
      break;
  }
}

/**
  Format MRC Memory Info Data structure for ACPI publishing.

  @param[in]      MemInfoData       Memory info data hob structure pointer
  @param[in, out] MemoryTelemetryDataTable      Configured data to MEMORY_TELEMETRY table and PHAT append

  @retval     EFI_SUCCESS           Data set successfully
  @retval     EFI_OUT_OF_RESOURCES  Could not allocate memory for MEMORY_TELEMETRY table
  @retval     Others                Failed to set data to new table
**/
EFI_STATUS
UpdateMemInfoToMemoryTelemetryPhat (
  IN      MEMORY_INFO_DATA_HOB         *MemInfoData,
  IN OUT  MEMORY_TELEMETRY_DATA_TABLE  *MemoryTelemetryDataTable
  )
{
  UINT8 Idxi;
  UINT8 Idxj;
  UINT8 Idxk;
  UINT8 ModuleType;
  UINT8 DramDeviceType;

  if ((MemoryTelemetryDataTable == NULL) || (MemInfoData == NULL) ) {
    return EFI_INVALID_PARAMETER;
  }

  MemoryTelemetryDataTable->MemoryType = MemInfoData->MemoryType;
  for (Idxi = 0; Idxi < MAX_NODE; Idxi++) {
    for (Idxj = 0; Idxj < MAX_CH; Idxj++) {
      for (Idxk = 0; Idxk < MAX_DIMM; Idxk++) {
        MemoryTelemetryDataTable->MfgId[Idxi][Idxj][Idxk]       = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].MfgId;
        MemoryTelemetryDataTable->Speed[Idxi][Idxj][Idxk]       = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].Speed;
        MemoryTelemetryDataTable->RankInDimm[Idxi][Idxj][Idxk]  = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].RankInDimm;
        MemoryTelemetryDataTable->Banks[Idxi][Idxj][Idxk]       = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].Banks;
        MemoryTelemetryDataTable->BankGroups[Idxi][Idxj][Idxk]  = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].BankGroups;

        ///
        /// Set the encoding for memory form factor align with SMBIOS Type 17
        ///
        DramDeviceType = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].SpdDramDeviceType;
        ModuleType = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].SpdModuleType;
        MemoryTelemetryDataTable->FormFactor[Idxi][Idxj][Idxk]  = GetFormFactor (ModuleType, DramDeviceType);
      }
      MemoryTelemetryDataTable->DimmCount[Idxi][Idxj]           = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmCount;
    }
  }

  for (Idxi = 0; Idxi < HOB_MAX_SAGV_POINTS; Idxi++) {
    MemoryTelemetryDataTable->DataRate[Idxi]  = MemInfoData->SagvConfigInfo.SaGvTiming[Idxi].DataRate;
    MemoryTelemetryDataTable->Bandwidth[Idxi] = MemInfoData->SagvConfigInfo.SaGvTiming[Idxi].MaxMemoryBandwidth;
  }
  MemoryTelemetryDataTable->SaGvPointMask     = MemInfoData->SagvConfigInfo.SaGvPointMask;
  MemoryTelemetryDataTable->MaxFrequency      = MemInfoData->MaximumMemoryClockSpeed;

  ///
  /// PPR
  ///
  MemoryTelemetryDataTable->PprAvailable                            = (UINT8) FixedPcdGetBool (PcdPprCapability);
  MemoryTelemetryDataTable->AmtPprRanInLastBoot.AmtRanLastBoot      = (MemInfoData->PprRanInLastBoot & BIT1) >> 1;
  MemoryTelemetryDataTable->AmtPprRanInLastBoot.PprRanLastBoot      = MemInfoData->PprRanInLastBoot & BIT0;
  if (MemoryTelemetryDataTable->AmtPprRanInLastBoot.AmtRanLastBoot == 0x1) {
    if (MemoryTelemetryDataTable->AmtPprRanInLastBoot.PprRanLastBoot == 0x1) {
      MemoryTelemetryDataTable->TotalPprRowRepairsSuccessful        = MemInfoData->PprRepairsSuccessful;
      MemoryTelemetryDataTable->TotalUncorrectableDevices           = MemInfoData->PprRepairFails;
    }
    MemoryTelemetryDataTable->TotalRowFailuresDiscovered            = MemInfoData->PprDetectedErrors;
    CopyMem (&MemoryTelemetryDataTable->PprErrorInfo, &MemInfoData->PprErrorInfo, sizeof (PPR_RESULT_COLUMNS_HOB));
    CopyMem (&MemoryTelemetryDataTable->AvailableResources, &MemInfoData->PprAvailableResources, sizeof (MemoryTelemetryDataTable->AvailableResources));
  }

  ///
  /// Print the Memory Telemetry data to serial debug log
  ///
  PrintMemoryTelemetryTable (MemoryTelemetryDataTable);

  return EFI_SUCCESS;
}

/**
  Configures the PHAT MEMORY TELEMETRY ACPI table data for installation

  @param[in]     MemoryTelemetryDataTable    Pointer to the MRC MEMORY TELEMETRY data table
  @param[in,out] PhatMemoryTelemetryTable    Pointer to configured PHAT with MEMORY TELEMETRY data

  @retval EFI_SUCCESS            Successfully configured MEMORY TELEMETRY for PHAT
  @retval EFI_INVALID_PARAMETER  Received invalid MEMORY TELEMETRY or PHAT pointers
**/
EFI_STATUS
ConfigMemoryTelemetryPhatTable (
  IN MEMORY_TELEMETRY_DATA_TABLE                  *MemoryTelemetryDataTable,
  IN OUT PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE   *PhatMemoryTelemetryTable
  )
{
  if ((MemoryTelemetryDataTable == NULL) || (PhatMemoryTelemetryTable == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Add Memory Telemetry specific data
  ///
  PhatMemoryTelemetryTable->PlatformRecordType  = MEMORY_TELEMETRY_PHAT_RECORD_TYPE;
  PhatMemoryTelemetryTable->Revision            = MEMORY_TELEMETRY_PHAT_RECORD_VERSION;
  CopyMem (&PhatMemoryTelemetryTable->MemTelemetryData, MemoryTelemetryDataTable, sizeof (MEMORY_TELEMETRY_DATA_TABLE));
  PhatMemoryTelemetryTable->RecordLength        = sizeof (PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE);
  return EFI_SUCCESS;
}

/**
  Checks if PHAT is installed, if it is we will add MEMORY TELEMETRY data and update PHAT. If PHAT is not found then
  add data to PHAt and install table to ACPI.

  @param[in]  MemoryTelemetryDataTable       Memory telemetry data table configured from MRC

  @retval EFI_SUCCESS            Successfully posted MEMORY TELEMETRY data to existing or new PHAT ACPI table
  @retval other                  Failed to either install or update PHAT with MEMORY TELEMETRY data
**/
EFI_STATUS
PublishMemoryTelemetryToPhat (
  IN MEMORY_TELEMETRY_DATA_TABLE       *MemoryTelemetryDataTable
  )
{
  EFI_STATUS                              Status;
  PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE  *MemoryTelemetryPhatTable;

  if (MemoryTelemetryDataTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MemoryTelemetryPhatTable = AllocateZeroPool (sizeof (PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE));
  if (MemoryTelemetryPhatTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Configure the Memory Telemetry to ACPI data
  ///
  Status = ConfigMemoryTelemetryPhatTable (MemoryTelemetryDataTable, MemoryTelemetryPhatTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// Table exists, get current table
  ///
  Status = InstallPhatTable (MemoryTelemetryPhatTable, MemoryTelemetryPhatTable->RecordLength);

  return Status;
}

/**
  Entry point of the Dxe Memory Telemetry
  Checks for support, and begins the PHAT table information update.

  @param[in] ImageHandle        Handle for this drivers loaded image protocol.
  @param[in] SystemTable        EFI system table.

  @retval     EFI_SUCCESS       The protocol has installed successfully
  @retval     EFI_UNSUPPORTED   The protocol is not supported
  @retval     Others            The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
DxeMemoryTelemetryEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                   Status;
  MEMORY_INFO_DATA_HOB         *MemInfoData;
  MEMORY_TELEMETRY_DATA_TABLE  *MemoryTelemetryDataTable;

  MemInfoData  = NULL;
  MemoryTelemetryDataTable = NULL;

  DEBUG ((DEBUG_INFO, "[%a] Entering\n", __FUNCTION__));

  ///
  /// Manage AMT PPR variable, must execute even if memory telemetry table cannot allocate
  ///
  AmtPprVarHandler ();

  ///
  /// Allocate space for Memory Telemetry table
  ///
  MemoryTelemetryDataTable = (MEMORY_TELEMETRY_DATA_TABLE *) AllocateZeroPool (sizeof (MEMORY_TELEMETRY_DATA_TABLE));
  if (MemoryTelemetryDataTable == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  ///
  /// Get MRC Data Hob
  ///
  MemInfoData = GetMemInfo ();
  if (MemInfoData == NULL) {
    Status = EFI_ABORTED;
    goto Exit;
  }

  ///
  /// Set memory data to Memory Telemetry PHAT format
  ///
  Status = UpdateMemInfoToMemoryTelemetryPhat (MemInfoData, MemoryTelemetryDataTable);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  ///
  /// Post Memory telemetry information to PHAT ACPI table
  ///
  Status = PublishMemoryTelemetryToPhat (MemoryTelemetryDataTable);

  Exit:
  FreePool (MemoryTelemetryDataTable);
  DEBUG ((DEBUG_INFO, "[%a] Exit status: %r\n", __FUNCTION__, Status));
  return Status;
}
