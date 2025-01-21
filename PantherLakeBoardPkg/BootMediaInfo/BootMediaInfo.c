/** @file
  Identify Boot Medium Information

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include "BootMediaInfo.h"

/**
  @param  ImageHandle  A handle for the image that is initializing this driver
  @param  SystemTable  A pointer to the EFI system table

  @retval EFI_SUCCESS             The entry point is executed successfully.
  @retval EFI_NOT_FOUND           The firmware boot media type could not be found.

**/
EFI_STATUS
EFIAPI
BootMediaDriverEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                Status;
  BOOT_DEVICE_SETUP         BootDeviceInfo;
  UINTN                     VariableSize;
  VariableSize = 0;
  Status = gRT->GetVariable (
                  BOOT_DEVICE_INFO_VARIABLE_NAME,
                  &gBootDeviceInfoGuid,
                  NULL,
                  &VariableSize,
                  &BootDeviceInfo
                  );
  if (Status == RETURN_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_INFO, "FirmwareBootMediaInfo variable has already existed.\n"));
    return EFI_SUCCESS;
  }
  DEBUG ((DEBUG_INFO, "FwBootMediaType is SPI\n"));
  BootDeviceInfo.Version = BOOT_DEVICE_INFO_REVISION;
  BootDeviceInfo.BootDeviceType = FwBootMediaSpi;
  BootDeviceInfo.Partition = 0;
  VariableSize = sizeof (BootDeviceInfo);
  Status = gRT->SetVariable (
                  BOOT_DEVICE_INFO_VARIABLE_NAME,
                  &gBootDeviceInfoGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  VariableSize,
                  &BootDeviceInfo
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "BootMediaInfo set variable successfully.\n"));
  return EFI_SUCCESS;
}
