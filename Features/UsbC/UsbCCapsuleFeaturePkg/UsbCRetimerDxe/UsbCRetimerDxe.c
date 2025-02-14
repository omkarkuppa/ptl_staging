/** @file
  Publish EFI_CAPSULE_DEBUG_PROTOCOL.

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
  - UEFI Specification 2.6, Section 22.3

**/

#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <Library/CapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogMappingTable.h>

///
/// Pointer to Capsule Debug Protocol instance
///
EFI_CAPSULE_DEBUG_PROTOCOL   *mRetimerCapsuleLog = NULL;
extern const CAPSULE_LOG_MAPPING_ENTRY  mUsbCCapsuleLogMappingTable [];
extern UINT32 mUsbCCapsuleLogMappingEntries;

/**
  Install USBC Capsule Debug Protocol.

**/
VOID
InstallCapsuleDebugProtocol (
  VOID
  )
{
  EFI_STATUS   Status;

  Status = InstallCapsuleDebugLibProtocol (
             &gEfiUsbCCapsuleDebugProtocolGuid,
             PcdGet8 (PcdUsbCCapsuleDebugLevel),
             mUsbCCapsuleLogMappingTable,
             mUsbCCapsuleLogMappingEntries,
             &mRetimerCapsuleLog
             );
  if (!EFI_ERROR (Status) && mRetimerCapsuleLog != NULL) {
    if (mRetimerCapsuleLog->LogWrite != NULL) {
      DEBUG ((DEBUG_INFO, "Install USBC Capsule Debug Protocol - Debug Level = %d\n", PcdGet8 (PcdUsbCCapsuleDebugLevel)));
    } else {
      DEBUG ((DEBUG_ERROR, "NULL Log Write function in USBC Capsule Debug Protocol\n"));
      mRetimerCapsuleLog = NULL;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "NULL USBC Capsule Debug Protocol is returned\n"));
    mRetimerCapsuleLog = NULL;
  }

}

/**
  UsbCRetimer DXE module entry point.

  @param[in]  ImageHandle  Not used.
  @param[in]  SystemTable  Global system service table.

  @retval  EFI_SUCCESS           Initialization complete.
  @retval  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UsbCRetimerDxeEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  InstallCapsuleDebugProtocol ();

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));

  return EFI_SUCCESS;
}