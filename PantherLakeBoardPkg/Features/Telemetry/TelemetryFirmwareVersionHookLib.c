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
  Convert Fvi data from Smbios to Acpi.

  @param[in]  ComponentId          ComponentId for the FVI entry.
  @param[out] Version              A pointer to version value

  @retval EFI_SUCCESS              Update Telemetry firmware version successfully
  @retval Others                   Fail to update Telemetry firmware version.
**/
EFI_STATUS
EFIAPI
TelemeteryFirmwawreVersionUpdate (
  OUT EFI_GUID  ComponentId,
  OUT UINT64    *Version
  )
{
  EFI_STATUS                  Status;
  FLASH_PARTITION_DATA        PartitionIdData;
  ME_BIOS_PAYLOAD_HOB         *MbpHob;


  if (CompareGuid (&ComponentId, &gMeFirmwareVersionComponentId)) {
    DEBUG ((DEBUG_INFO, "Enter Update ME FW Version.\n" ));
    MbpHob = NULL;
    MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
    if (MbpHob != NULL) {
      *Version = (LShiftU64 ((UINT64) MbpHob->MeBiosPayload.FwVersionName.MajorVersion,  VERSION_MAJOR_VERSION_SHIFT) | \
                 LShiftU64 ((UINT64) MbpHob->MeBiosPayload.FwVersionName.MinorVersion,  VERSION_MINOR_VERSION_SHIFT) | \
                 LShiftU64 ((UINT64) MbpHob->MeBiosPayload.FwVersionName.HotfixVersion, VERSION_REVISION_SHIFT)      | \
                           ((UINT16) MbpHob->MeBiosPayload.FwVersionName.BuildVersion));
    } else {
      DEBUG ((DEBUG_ERROR, "Failed to get Me firmware version from hob.\n" ));
      return EFI_LOAD_ERROR;
    }
  } else if (CompareGuid (&ComponentId, &gMeSseVersionComponentId)) {
    DEBUG ((DEBUG_INFO, "Enter Update SSE FW Version.\n" ));
    Status = HeciGetImageFwVersionMsg (FPT_PARTITION_NAME_EFWP, &PartitionIdData);
    if (!EFI_ERROR (Status)) {
      *Version = (LShiftU64 ((UINT64) PartitionIdData.Version.Major,  VERSION_MAJOR_VERSION_SHIFT) | \
                 LShiftU64 ((UINT64) PartitionIdData.Version.Minor,  VERSION_MINOR_VERSION_SHIFT) | \
                 LShiftU64 ((UINT64) PartitionIdData.Version.Hotfix, VERSION_REVISION_SHIFT)      | \
                           ((UINT16) PartitionIdData.Version.Build));
    } else {
      DEBUG ((DEBUG_ERROR, "Failed to get SSE version through Heci. Status: %r\n", Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

