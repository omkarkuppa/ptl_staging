/** @file
  Tbt PD Retimer Device Library Functions.

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

#include <Library/DebugLib.h>
#include <Protocol/UsbCRetimerProtocol.h>
#include <EcCommon.h>
#include <Library/EcTcssLib.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>

extern UINT8    gTotalCountOfPdController;

/**
  Change Tbt Retimer PD controller Fw Update Mode.

  @param[in]  PdControllerMode     Mode of PD Controller to SET.

  @retval  EFI_SUCCESS            Successfully Mode is Changed to FW Update Mode.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_UNSUPPORTED        This driver does not support.
  @retval  EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval  EFI_ALREADY_STARTED    This driver has been started. This is applicable for PD Fw Update Mode Entry.
  @retval  EFI_NOT_STARTED        This driver has not been started. This is applicable for PD Fw Update Mode Exit.
  @retval  EFI_TIMEOUT            Mode Change Command timeout Happen.
**/
EFI_STATUS
TbtPdRetimerFwUpdateModeChange (
  IN  UINT8  PdControllerMode
  )
{
  EFI_STATUS  Status;
  UINT8       DeviceMode;
  UINT8       EcPdTempBuffer;
  CHAR16      *String = NULL;

  String = ((PdControllerMode == RetimerFirmWareUpdateEnableMode) ? L"Enable" : L"Disable" );

  // Check if PD Controller available in board or not.
  if (gTotalCountOfPdController == 0) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: Retimer Capsule Update is not supported as no PD Controller available. Returning with Status = %r\n", String, Status));
    return Status;
  }

  // Check the PD Controller Mode
  DeviceMode = INVALID_DEVICE_MODE; // Initialize DeviceMode with Invalid Data before getting PD Controller Mode
  EcPdTempBuffer = 0;
  Status = GetPdControllerMode (&DeviceMode, &EcPdTempBuffer, gTotalCountOfPdController);
  if (PdControllerMode == RetimerFirmWareUpdateEnableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_FIRST_GET_MODE_EC_STATUS, (UINT32)EcPdTempBuffer, 0);
  }
  if (PdControllerMode == RetimerFirmWareUpdateDisableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_FIRST_GET_MODE_EC_STATUS, (UINT32)EcPdTempBuffer, 0);
  }

  // Check if any ERROR status on Get Mode
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller GET MODE Not Responding. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_FIRST_GET_MODE_FAIL, (UINT32)Status, 0);
    return Status;
  }
  // Check if PD controller is in or not in FW Update mode already.
  if (DeviceMode == PdControllerMode) {
    if (PdControllerMode == RetimerFirmWareUpdateEnableMode) {
      DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.Drive: PD Controller is already in FW Update Mode\n"));
      CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_ALREADY_IN_UPDATE_MODE, 0, 0);
      Status = EFI_ALREADY_STARTED;
    }
    if (PdControllerMode == RetimerFirmWareUpdateDisableMode) {
      DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.Restore: PD Controller is already exited FW Update Mode.\n"));
      CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_ALREADY_IN_UPDATE_MODE, 0, 0);
      Status = EFI_NOT_STARTED;
    }
    return Status;
  }
  // Send command to EC instruct to PD Controller to either enter or exit FW Update Mode
  Status = SetPdControllerMode (PdControllerMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller SET MODE Not Responding. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SET_MODE_FAIL, (UINT32)Status, 0);
    return Status;
  }
  DeviceMode = INVALID_DEVICE_MODE; // Initialize DeviceMode with Invalid Data before getting PD Controller Mode
  EcPdTempBuffer = 0;
  Status = GetPdControllerMode (&DeviceMode, &EcPdTempBuffer, gTotalCountOfPdController);
  if (PdControllerMode == RetimerFirmWareUpdateEnableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SECOND_GET_MODE_EC_STATUS, (UINT32)EcPdTempBuffer, 0);
  }
  if (PdControllerMode == RetimerFirmWareUpdateDisableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_SECOND_GET_MODE_EC_STATUS, (UINT32)EcPdTempBuffer, 0);
  }
  // Check if any ERROR status on Get Mode
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller GET MODE Not Responding. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SECOND_GET_MODE_FAIL, (UINT32)Status, 0);
    return Status;
  }
  // Check if any ERROR status on Set Mode
  if (DeviceMode != PdControllerMode) {
    Status = EFI_TIMEOUT;
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller SET MODE is not Completed. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SET_MODE_NOT_COMPLETE, (UINT32)Status, 0);
    return Status;
  }

  if (PdControllerMode == RetimerFirmWareUpdateEnableMode) {
    // Now PD Controller Successfully Enter Into FW Update Mode.
    DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.Drive: "));
    DEBUG ((DEBUG_INFO, "PD Controller Enter Into FW Update Mode with Status = %r\n", Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_ENTER_FW_UPDATE_MODE, 0, 0);
  }
  if (PdControllerMode == RetimerFirmWareUpdateDisableMode) {
    // Now PD Controller Successfully EXIT From FW Update Mode.
    DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.Restore: "));
    DEBUG ((DEBUG_INFO, "PD Controller Exit from FW Update Mode with Status = %r\n", Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_ENTER_FW_UPDATE_MODE, 0, 0);
  }

  return EFI_SUCCESS;
}