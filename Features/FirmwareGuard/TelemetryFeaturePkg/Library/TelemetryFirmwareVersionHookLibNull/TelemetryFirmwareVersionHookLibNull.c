/** @file
  @This file implements the functions to get Telemetry FW version.

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

  @par Specification
**/

/**
  Update Fvi data from Smbios to Acpi.

  @param[in]  ComponentId          ComponentId for the Fvi entry.
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
  return  EFI_SUCCESS;
}