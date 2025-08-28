/** @file
  Telemetry hook to get firmware version infomation.

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

#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <TelemetryCommon.h>
#include <Protocol/DevicePath.h>
#include <Library/DxeMeLib.h>
#include <Guid/ComponentId.h>
#include <MeBiosPayloadHob.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DevicePathLib.h>

/**
  Get the full partition table for telemetry reporting.

  @param[out] PartitionTable       Pointer to receive partition table data pointe
  @param[out] NumOfModules         Pointer to number of modules/partitions

  @retval EFI_SUCCESS              Successfully retrieved partition table
  @retval EFI_OUT_OF_RESOURCES     Failed to allocate memory
  @retval Others                   HECI call failed
**/
EFI_STATUS
EFIAPI
GetFullFviPartitionTable (
  OUT VOID      **PartitionTable,
  OUT UINT32    *NumOfModules
  )
{
  EFI_STATUS                  Status;
  UINT32                      LocalNumOfModules;
  FLASH_PARTITION_DATA        *AllPartitionsData;

  *PartitionTable = NULL;
  *NumOfModules = 0;
  LocalNumOfModules = 0;

  AllPartitionsData = (FLASH_PARTITION_DATA *) AllocateZeroPool (sizeof (FLASH_PARTITION_DATA) * MAX_NUM_OF_PARTITIONS);
  if (AllPartitionsData == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to allocate memory for partition data\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Call HECI to get partition table
  //
  Status = HeciGetImageFwVersionMsg (Csme, FPT_PARTITION_NAME_UNDEFINED, (UINT32 *) &LocalNumOfModules, AllPartitionsData);
  if (EFI_ERROR (Status) || NumOfModules == 0 || AllPartitionsData == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a] HECI call failed with number of modules %d and status: %r\n", __FUNCTION__, LocalNumOfModules, Status));

    if (AllPartitionsData != NULL) {
      FreePool (AllPartitionsData);
    }

    *PartitionTable = NULL;
    *NumOfModules = 0;
    return Status;
  }

  // Return pointer to the partition data blob and the count
  *PartitionTable = AllPartitionsData;
  *NumOfModules = LocalNumOfModules;
  return Status;
}

/**
  Convert Fvi data from Smbios to Acpi.

  @param[in]  ComponentId          ComponentId for the FVI entry.
  @param[out] Version              A pointer to version value

  @retval EFI_SUCCESS              Update Telemetry firmware version successfully
  @retval EFI_INVALID_PARAMETER    Invalid input parameters
  @retval Others                   Fail to update Telemetry firmware version.
**/
EFI_STATUS
EFIAPI
TelemetryFirmwareVersionUpdate (
  IN  EFI_GUID  ComponentId,
  OUT UINT64    *Version
  )
{
  if (CompareGuid (&ComponentId, &gMeFirmwareVersionComponentId)) {
    ME_BIOS_PAYLOAD_HOB         *MbpHob;

    MbpHob = NULL;
    MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
    if (MbpHob != NULL) {
      *Version = (LShiftU64 ((UINT64) MbpHob->MeBiosPayload.FwVersionName.MajorVersion,  VERSION_MAJOR_VERSION_SHIFT) | \
                 LShiftU64 ((UINT64) MbpHob->MeBiosPayload.FwVersionName.MinorVersion,  VERSION_MINOR_VERSION_SHIFT) | \
                 LShiftU64 ((UINT64) MbpHob->MeBiosPayload.FwVersionName.HotfixVersion, VERSION_REVISION_SHIFT)      | \
                           ((UINT16) MbpHob->MeBiosPayload.FwVersionName.BuildVersion));
    } else {
      return EFI_LOAD_ERROR;
    }
  } else {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

