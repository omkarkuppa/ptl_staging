/** @file
  Dxe Memory Health Insights Feature Implementation.

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

@par Specification Reference:
**/

#include "DxeMemoryHealthInsights.h"
#include "MhiTable.h"
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
  Prints the received memory health insights data to serial debug log.

  @param[in]  MhiDataTable    Memory Health Table stored from MRC
**/
VOID
PrintMhiTable (
  IN MHI_DATA_TABLE   *MhiDataTable
  )
{
  UINT8   Idxi;
  UINT8   Idxj;
  UINT8   Idxk;

  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
  DEBUG ((DEBUG_INFO, "|---------- Memory Health Insights Data Table ----------|\n"));
  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
  DEBUG ((DEBUG_INFO, "|  Memory Type                             0x%-10X |\n", MhiDataTable->MemoryType));
  for (Idxi = 0; Idxi < MAX_NODE; Idxi++) {
    DEBUG ((DEBUG_INFO, "|  Node          %d                                      |\n", Idxi));
    for (Idxj = 0; Idxj < MAX_CH; Idxj++) {
      DEBUG ((DEBUG_INFO, "|  |  Channel    %d                                      |\n", Idxj));
      for (Idxk = 0; Idxk < MAX_DIMM; Idxk++) {
        DEBUG ((DEBUG_INFO, "|  |  |  DIMM    %d                                      |\n", Idxk));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Manufacture ID                 0x%-10X |\n", MhiDataTable->MfgId[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Memory Speed                   0x%-10X |\n", MhiDataTable->Speed[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Rank In DIMM                   0x%-10X |\n", MhiDataTable->RankInDimm[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Bank In DIMM                   0x%-10X |\n", MhiDataTable->Banks[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Bank Groups In DIMM            0x%-10X |\n", MhiDataTable->BankGroups[Idxi][Idxj][Idxk]));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Form Factor                    0x%-10X |\n", MhiDataTable->FormFactor[Idxi][Idxj][Idxk]));
      }
      DEBUG ((DEBUG_INFO, "|  |  |  DIMM Count                        0x%-10X |\n", MhiDataTable->DimmCount[Idxi][Idxj]));
      DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
    }
  }
  for (Idxi = 0; Idxi < HOB_MAX_SAGV_POINTS; Idxi++) {
    DEBUG ((DEBUG_INFO, "|  Sa Gv Point - %d                                     |\n", Idxi+1));
    DEBUG ((DEBUG_INFO, "|  Data Rate                               0x%-10X |\n", MhiDataTable->DataRate[Idxi]));
    DEBUG ((DEBUG_INFO, "|  Bandwidth                               0x%-10X |\n", MhiDataTable->Bandwidth[Idxi]));
  }
  DEBUG ((DEBUG_INFO, "|  SaGvPointMask                           0x%-10X |\n", MhiDataTable->SaGvPointMask));
  DEBUG ((DEBUG_INFO, "|  Maximum Frequency                       0x%-10X |\n", MhiDataTable->MaxFrequency));
  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));

  // PPR
  DEBUG ((DEBUG_INFO, "|---------------------- AMT PPR  -----------------------|\n"));
  DEBUG ((DEBUG_INFO, "|  PprAvailable                            0x%-10X |\n", MhiDataTable->PprAvailable));
  DEBUG ((DEBUG_INFO, "|  AmtPprRanInLastBoot                     0x%-10X |\n", MhiDataTable->AmtPprRanInLastBoot));
  if (MhiDataTable->AmtPprRanInLastBoot.AmtRanLastBoot == 0x1) {
    if (MhiDataTable->AmtPprRanInLastBoot.PprRanLastBoot == 0x1) {
      DEBUG ((DEBUG_INFO, "|  Number of Successful Repairs            0x%-10X |\n", MhiDataTable->TotalPprRowRepairsSuccessful));
      DEBUG ((DEBUG_INFO, "|  Number of Uncorrectable Devices         0x%-10X |\n", MhiDataTable->TotalUncorrectableDevices));
    }
    DEBUG ((DEBUG_INFO, "|  Number of Detected Errors               0x%-10X |\n", MhiDataTable->TotalRowFailuresDiscovered));
    DEBUG ((DEBUG_INFO, "|  Error Repair Successful                 0x%-10X |\n", MhiDataTable->PprErrorInfo.PprRowRepairsSuccessful));
    DEBUG ((DEBUG_INFO, "|  Error Controller                        0x%-10X |\n", MhiDataTable->PprErrorInfo.Controller));
    DEBUG ((DEBUG_INFO, "|  Error Channel                           0x%-10X |\n", MhiDataTable->PprErrorInfo.Channel));
    DEBUG ((DEBUG_INFO, "|  Error Rank                              0x%-10X |\n", MhiDataTable->PprErrorInfo.Rank));
    DEBUG ((DEBUG_INFO, "|  Error Bank Group                        0x%-10X |\n", MhiDataTable->PprErrorInfo.BankGroup));
    DEBUG ((DEBUG_INFO, "|  Error Bank                              0x%-10X |\n", MhiDataTable->PprErrorInfo.Bank));
    DEBUG ((DEBUG_INFO, "|  Error Row                               0x%-10X |\n", MhiDataTable->PprErrorInfo.Row));
    for (Idxi = 0; Idxi < MAX_NODE; Idxi++) {
      DEBUG ((DEBUG_INFO, "|  Node          %d                                      |\n", Idxi));
      for (Idxj = 0; Idxj < MAX_CH; Idxj++) {
        DEBUG ((DEBUG_INFO, "|  |  Channel    %d                                      |\n", Idxj));
        for (Idxk = 0; Idxk < MAX_RANK_IN_CHANNEL; Idxk++) {
          DEBUG ((DEBUG_INFO, "|  |  |  Rank    %d                                      |\n", Idxk));
          for (UINT8 Idxl = 0; Idxl < MAX_SDRAM_IN_DIMM; Idxl++) {
            DEBUG ((DEBUG_INFO, "|  |  |  |  Device    %d                                    |\n", Idxl));
            DEBUG ((DEBUG_INFO, "|  |  |  |  |  Available Resources         0x%-10X |\n", MhiDataTable->AvailableResources[Idxi][Idxj][Idxk][Idxl]));
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
    case DDR_MTYPE_SO_UDIMM:
      /// Legacy non-JEDEC LPDDR3 SPD images use SODIMM module type which should be soldered down form factor.
      return (DramDeviceType == DDR_DTYPE_LPDDR3) ? MemoryFormFactorRowOfChips : MemoryFormFactorSodimm;
      break;

    case DDR_MTYPE_RDIMM:
    case DDR_MTYPE_MINI_RDIMM:
      return (UINT8) MemoryFormFactorRimm;
      break;

    case DDR_MTYPE_MEM_DOWN:
      return (UINT8) MemoryFormFactorRowOfChips;
      break;

    case DDR_MTYPE_UDIMM:
    case DDR_MTYPE_MICRO_DIMM:
    case DDR_MTYPE_MINI_UDIMM:
    default:
      return (UINT8) MemoryFormFactorDimm;
      break;
  }
}

/**
  Format MRC Memory Info Data structure for ACPI publishing.

  @param[in]      MemInfoData       Memory info data hob structure pointer
  @param[in, out] MhiDataTable      Configured data to MHI table and PHAT append

  @retval     EFI_SUCCESS           Data set successfully
  @retval     EFI_OUT_OF_RESOURCES  Could not allocate memory for MHI table
  @retval     Others                Failed to set data to new table
**/
EFI_STATUS
UpdateMemInfoToMhiPhat (
  IN      MEMORY_INFO_DATA_HOB        *MemInfoData,
  IN OUT  MHI_DATA_TABLE              *MhiDataTable
  )
{
  UINT8 Idxi;
  UINT8 Idxj;
  UINT8 Idxk;
  UINT8 ModuleType;
  UINT8 DramDeviceType;

  if ((MhiDataTable == NULL) || (MemInfoData == NULL) ) {
    return EFI_INVALID_PARAMETER;
  }

  MhiDataTable->MemoryType = MemInfoData->MemoryType;
  for (Idxi = 0; Idxi < MAX_NODE; Idxi++) {
    for (Idxj = 0; Idxj < MAX_CH; Idxj++) {
      for (Idxk = 0; Idxk < MAX_DIMM; Idxk++) {
        MhiDataTable->MfgId[Idxi][Idxj][Idxk]       = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].MfgId;
        MhiDataTable->Speed[Idxi][Idxj][Idxk]       = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].Speed;
        MhiDataTable->RankInDimm[Idxi][Idxj][Idxk]  = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].RankInDimm;
        MhiDataTable->Banks[Idxi][Idxj][Idxk]       = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].Banks;
        MhiDataTable->BankGroups[Idxi][Idxj][Idxk]  = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].BankGroups;

        // Set the encoding for memory form factor align with SMBIOS Type 17
        DramDeviceType = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].SpdDramDeviceType;
        ModuleType = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmInfo[Idxk].SpdModuleType;
        MhiDataTable->FormFactor[Idxi][Idxj][Idxk]  = GetFormFactor (ModuleType, DramDeviceType);
      }
      MhiDataTable->DimmCount[Idxi][Idxj]           = MemInfoData->Controller[Idxi].ChannelInfo[Idxj].DimmCount;
    }
  }

  for (Idxi = 0; Idxi < HOB_MAX_SAGV_POINTS; Idxi++) {
    MhiDataTable->DataRate[Idxi]  = MemInfoData->SagvConfigInfo.SaGvTiming[Idxi].DataRate;
    MhiDataTable->Bandwidth[Idxi] = MemInfoData->SagvConfigInfo.SaGvTiming[Idxi].MaxMemoryBandwidth;
  }
  MhiDataTable->SaGvPointMask     = MemInfoData->SagvConfigInfo.SaGvPointMask;
  MhiDataTable->MaxFrequency      = MemInfoData->MaximumMemoryClockSpeed;

  // PPR
  MhiDataTable->PprAvailable                            = (UINT8) FixedPcdGetBool (PcdPprCapability);
  MhiDataTable->AmtPprRanInLastBoot.AmtRanLastBoot      = (MemInfoData->PprRanInLastBoot & BIT1) >> 1;
  MhiDataTable->AmtPprRanInLastBoot.PprRanLastBoot      = MemInfoData->PprRanInLastBoot & BIT0;
  if (MhiDataTable->AmtPprRanInLastBoot.AmtRanLastBoot == 0x1) {
    if (MhiDataTable->AmtPprRanInLastBoot.PprRanLastBoot == 0x1) {
      MhiDataTable->TotalPprRowRepairsSuccessful        = MemInfoData->PprRepairsSuccessful;
      MhiDataTable->TotalUncorrectableDevices           = MemInfoData->PprRepairFails;
    }
    MhiDataTable->TotalRowFailuresDiscovered            = MemInfoData->PprDetectedErrors;
    CopyMem (&MhiDataTable->PprErrorInfo, &MemInfoData->PprErrorInfo, sizeof (MhiDataTable->PprErrorInfo));
    CopyMem (&MhiDataTable->AvailableResources, &MemInfoData->PprAvailableResources, sizeof (MhiDataTable->AvailableResources));
  }

  // Print the MHI data to serial debug log
  PrintMhiTable (MhiDataTable);

  return EFI_SUCCESS;
}

/**
  Configures the PHAT MHI ACPI table data for installation

  @param[in]     MhiDataTable    Pointer to the MRC MHI data table
  @param[in,out] PhatMhiTable    Pointer to configured PHAT with MHI data

  @retval EFI_SUCCESS            Successfully configured MHI for PHAT
  @retval EFI_INVALID_PARAMETER  Received invalid MHI or PHAT pointers
**/
EFI_STATUS
ConfigMhiPhatTable (
  IN MHI_DATA_TABLE                                     *MhiDataTable,
  IN OUT PHAT_MEMORY_HEALTH_INSIGHTS_RECORD_STRUCTURE   *PhatMhiTable
  )
{
  // Check for valid pointer input
  if ((MhiDataTable == NULL) || (PhatMhiTable == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Add MHI specific data
  PhatMhiTable->PlatformRecordType  = MHI_PHAT_RECORD_TYPE;
  PhatMhiTable->Revision            = MHI_PHAT_RECORD_VERSION;
  CopyMem (&PhatMhiTable->MemHealthInsightData, MhiDataTable, sizeof (MHI_DATA_TABLE));
  PhatMhiTable->RecordLength        = sizeof (PHAT_MEMORY_HEALTH_INSIGHTS_RECORD_STRUCTURE);
  return EFI_SUCCESS;
}

/**
  Checks if PHAT is installed, if it is we will add MHI data and update PHAT. If PHAT is not found then
  add data to PHAt and install table to ACPI.

  @param[in]  MhiDataTable       Memory health insights data table configured from MRC

  @retval EFI_SUCCESS            Successfully posted MHI data to existing or new PHAT ACPI table
  @retval other                  Failed to either install or update PHAT with MHI data
**/
EFI_STATUS
PublishMhiToPhat (
  IN MHI_DATA_TABLE       *MhiDataTable
  )
{
  EFI_STATUS                                    Status;
  PHAT_MEMORY_HEALTH_INSIGHTS_RECORD_STRUCTURE  *MhiPhatTable;

  // Check the input for validity
  if (MhiDataTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MhiPhatTable = AllocateZeroPool (sizeof (PHAT_MEMORY_HEALTH_INSIGHTS_RECORD_STRUCTURE));
  if (MhiPhatTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Configure the MHI to ACPI data
  Status = ConfigMhiPhatTable (MhiDataTable, MhiPhatTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Table exists, get current table
  Status = InstallPhatTable (MhiPhatTable, MhiPhatTable->RecordLength);

  return Status;
}

/**
  Entry point of the Dxe Memory Health Insights
  Checks for support, and begins the PHAT table information update.

  @param[in] ImageHandle        Handle for this drivers loaded image protocol.
  @param[in] SystemTable        EFI system table.

  @retval     EFI_SUCCESS       The protocol has installed successfully
  @retval     EFI_UNSUPPORTED   The protocol is not supported
  @retval     Others            The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
DxeMemoryHealthInsightsEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  MEMORY_INFO_DATA_HOB    *MemInfoData;
  MHI_DATA_TABLE          *MhiDataTable;

  MemInfoData  = NULL;
  MhiDataTable = NULL;

  // Manage AMT PPR variable, must execute even if MHI table cannot allocate
  MhiAmtPprMain ();

  // Allocate space for MHI table
  MhiDataTable = (MHI_DATA_TABLE *) AllocateZeroPool (sizeof (MHI_DATA_TABLE));
  if (MhiDataTable == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  // Get MRC Data Hob
  MemInfoData = GetMemInfo ();
  if (MemInfoData == NULL) {
    Status = EFI_ABORTED;
    goto Exit;
  }

  // Set memory data to MHI PHAT format
  Status = UpdateMemInfoToMhiPhat (MemInfoData, MhiDataTable);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  // Post Memory health information to PHAT ACPI table
  Status = PublishMhiToPhat (MhiDataTable);

  Exit:
  FreePool (MhiDataTable);
  DEBUG ((DEBUG_INFO, "[%a] Exit status: %r\n", __FUNCTION__, Status));
  return Status;
}
