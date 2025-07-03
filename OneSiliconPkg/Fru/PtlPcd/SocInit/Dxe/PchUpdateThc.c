/** @file
  Update THC ACPI data

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

#include "PchInit.h"
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <ThcConfigHob.h>
#include <Library/SocInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Update ASL definitions for Touch Host Controller devices.

**/
EFI_STATUS
OnReadyToBootUpdateThcAcpiData (
  VOID
  )
{
  EFI_STATUS               Status;
  PCH_NVS_AREA_PROTOCOL    PchNvsAreaProtocol;

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));

  if (PcdGet16 (PcdThc0DeviceSlaveAddress) == 0 && PcdGet16 (PcdThc1DeviceSlaveAddress) == 0) {
    DEBUG ((DEBUG_ERROR, "%a: Invalid THC device slave address\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Locate PCH NVS protocol\n"));

  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (PCH_NVS_AREA), (VOID **) &PchNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *) PchNvsAreaProtocol.Area, sizeof (PCH_NVS_AREA));
  Status = gBS->LocateProtocol (
                  &gPchNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &PchNvsAreaProtocol
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: PchNvsAreaProtocol LocateProtocol Failed Status: %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  if (PcdGet16 (PcdThc0DeviceSlaveAddress) > 0) {
    PchNvsAreaProtocol.Area->ThcHidI2cDeviceAddress[0] = PcdGet16 (PcdThc0DeviceSlaveAddress);
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceAddress[0] set to 0x%X\n", PchNvsAreaProtocol.Area->ThcHidI2cDeviceAddress[0]));
  } else {
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceAddress[0] not set due to invalid slave address\n"));
  }

  if (PcdGet32 (PcdThc0DeviceDescriptorAddress) > 0) {
    PchNvsAreaProtocol.Area->ThcHidI2cDeviceDescriptorAddress[0] = (UINT16) PcdGet32 (PcdThc0DeviceDescriptorAddress);
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceDescriptorAddress[0] set to 0x%X\n", PchNvsAreaProtocol.Area->ThcHidI2cDeviceDescriptorAddress[0]));
  } else {
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceDescriptorAddress[0] not set due to invalid slave address\n"));
  }

  if (PcdGet16 (PcdThc1DeviceSlaveAddress) > 0) {
    PchNvsAreaProtocol.Area->ThcHidI2cDeviceAddress[1] = PcdGet16 (PcdThc1DeviceSlaveAddress);
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceAddress[1] set to 0x%X\n", PchNvsAreaProtocol.Area->ThcHidI2cDeviceAddress[1]));
  } else {
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceAddress[1] not set due to invalid slave address\n"));
  }

  if (PcdGet32 (PcdThc1DeviceDescriptorAddress) > 0) {
    PchNvsAreaProtocol.Area->ThcHidI2cDeviceDescriptorAddress[1] = (UINT16) PcdGet32 (PcdThc1DeviceDescriptorAddress);
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceDescriptorAddress[1] set to 0x%X\n", PchNvsAreaProtocol.Area->ThcHidI2cDeviceDescriptorAddress[1]));
  } else {
    DEBUG ((DEBUG_INFO, "ThcHidI2cDeviceDescriptorAddress[1] not set due to invalid slave address\n"));
  }

  //
  // Patch PchNvsArea Address
  //
  PatchPchNvsAreaAddress ((UINT32) (UINTN) PchNvsAreaProtocol.Area);

  return EFI_SUCCESS;
}