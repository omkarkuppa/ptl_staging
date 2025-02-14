/** @file
  Publish USBC_PD_BRIDGE_PROTOCOL to support Intel PD Bridge.

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
  - UEFI Specification 2.6, Section 22.3

**/

#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/EcLib.h>
#include <Library/EcTcssLib.h>
#include <Library/EcMiscLib.h>
#include <Protocol/UsbCPdBridgeProtocol.h>
#include <UsbCPdSetup.h>

GLOBAL_REMOVE_IF_UNREFERENCED USBC_PD_BRIDGE_PROTOCOL  mUsbCPdBridgeProtocol;

/**
  The function to get PD Bridge version via EC command

  @param[in]  PdCntrlIndex     PD controller index (1-based).
  @param[in]  PdBridgeVersion  A Pointer to PD Bridge version

  @retval EFI_SUCCESS          Get PD Bridge Version successfully
  @retval others               Get PD Bridge Version failed

**/
EFI_STATUS
GetPdBridgeVersion (
  IN  UINT8   PdCntrlIndex,
  IN  UINT64  *PdBridgeVersion
  )
{
  EFI_STATUS     Status;
  UINT8          DataBuffer[8];
  USBC_PD_SETUP  UsbCPdSetup;
  UINT32         VarAttributes;
  UINTN          VarSize;

  if (PdBridgeVersion == NULL ) {
    DEBUG ((DEBUG_ERROR, "PdBridgeVersion is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (DataBuffer, sizeof (DataBuffer));
  Status = GetPDFwVersion (PdCntrlIndex , DataBuffer);
  if (!EFI_ERROR (Status)) {
    *PdBridgeVersion = LShiftU64 (DataBuffer[7], 56) + LShiftU64 (DataBuffer[6], 48) \
                     + LShiftU64 (DataBuffer[5], 40) + LShiftU64 (DataBuffer[4], 32) \
                     + LShiftU64 (DataBuffer[3], 24) + LShiftU64 (DataBuffer[2], 16) \
                     + LShiftU64 (DataBuffer[1], 8) + DataBuffer[0];
  } else {
    DEBUG ((DEBUG_ERROR, "Get PD%d FW version failed with status:%r\n", PdCntrlIndex + 1, Status));
    return Status;
  }

  ZeroMem (&UsbCPdSetup, sizeof (USBC_PD_SETUP));
  VarSize = sizeof (USBC_PD_SETUP);
  Status = gRT->GetVariable (
                  L"UsbCPdSetup",
                  &gUsbCPdSetupGuid,
                  &VarAttributes,
                  &VarSize,
                  &UsbCPdSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get UsbCPdSetup variable with return Status = (%r).\n", Status));
    return Status;
  }

  switch (PdCntrlIndex) {
    case 0:
      UsbCPdSetup.UsbCPd1Version = *PdBridgeVersion;
      break;
    case 1:
      UsbCPdSetup.UsbCPd2Version = *PdBridgeVersion;
      break;
    case 2:
      UsbCPdSetup.UsbCPd3Version = *PdBridgeVersion;
      break;
    case 3:
      UsbCPdSetup.UsbCPd4Version = *PdBridgeVersion;
      break;
  }

  VarAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE;
  Status = gRT->SetVariable (
                  USBC_PD_NAME,
                  &gUsbCPdSetupGuid,
                  VarAttributes,
                  VarSize,
                  &UsbCPdSetup
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Send Command to EC to lock/unlock EC-PD regular communication

  @param[in] Lock             Lock(0x01) or unlock(0x00).

  @retval EFI_SUCCESS         Lock/Unlock EC-PD regular communication successfully
  @retval EFI_ACCESS_DENIED   Already Locked or Unlocked
  @retval others              Lock/Unlock EC-PD regular communication failed

**/
EFI_STATUS
EcPdLockCommunication (
  IN  UINT8  Lock
  )
{
  //
  // Control the Ec Debug Info Print Level to DEBUG_VERBOSE before executing any command for PD Bridge
  // And revert back to DEBUG_INFO after executing the command
  //
  if (Lock == 1) {
    gEcDebugInfoPrintLevel = PcdGet32 (VpdPcdPdBridgeDebugInfoPrintLevel);
  } else {
    gEcDebugInfoPrintLevel =  (UINT32) DEBUG_INFO;
  }

  return LockEcPdI2cTarget (Lock);
}

/**
  UsbCPdBridgeSupport DXE module entry point.

  @param[in] ImageHandle         Handle for the image of this driver
  @param[in] SystemTable         Pointer to the EFI System Table

  @retval  EFI_SUCCESS           Initialization complete.
  @retval  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UsbCPdBridgeSupportEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  mUsbCPdBridgeProtocol.Revision              = USBC_PD_BRIDGE_PROTOCOL_REVISION;
  mUsbCPdBridgeProtocol.GetVersion            = GetPdBridgeVersion;
  mUsbCPdBridgeProtocol.Lock                  = EcPdLockCommunication;
  mUsbCPdBridgeProtocol.ExecuteVendorCmd      = EcPdExecuteVendorCommand;
  ///
  /// Install PD Bridge Protocol
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gUsbCPdBridgeProtocolGuid,
                  &mUsbCPdBridgeProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UsbCPdBridgeSupport: Failed to Install PD Bridge Protocol with Status = %r\n", Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "UsbCPdBridgeSupport: gUsbCPdBridgeProtocolGuid is installed Successfully.\n"));

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));
  return Status;
}