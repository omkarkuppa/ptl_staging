/**@file
  Platform Sanitize library to support Storage device sanitizations.

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Protocol/StorageSanitizeNotifyProtocol.h>
#include <Library/PsStorageService.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SanitizeUiLib.h>
#include <Library/PlatformSanitizeServices.h>

/**
  Callback function for storage erase.

  @param[in] EraseStatus  Status return for erase
  @param[in] ReportNum    Count of the reports
  @param[in] Report       Pointer to an array of report
  @param[in] DeviceList   Pointer to device list which has erased

  @retval EFI_SUCCESS     Storage erase is successful.
  @retval Others          failed to erase all devices.
**/
VOID
EFIAPI
PsStorageEraseComplete (
  IN EFI_STATUS           EraseStatus,
  IN UINT8                ReportNum,
  IN ERASE_DOCUMENTATION  *Report,
  IN ERASE_DEVICE_LIST    *DeviceList
  )
{
  DEBUG ((DEBUG_INFO, "PS: %a Erase Status:%r ReportNum:%d\n", __FUNCTION__, EraseStatus, ReportNum));

  //
  // Update Boot parameter structure with status and clear its corresponding Mask bit
  //
  PsBootParameterStatusUpdate (PS_ERASE_STORAGE_MEDIA, EraseStatus);

  //
  // Upload the storage sanitize report
  //
  PsStorageEraseReport (ReportNum, Report);

  //
  // Print the summary of erase operation
  //
  InitUiLib ();
  SetScreenTitle ();
  PsStoragePrintSummary (EraseStatus);

  //
  // Other sanitize related actions/handshake will be performed during the reboot.
  // Note: If HDD password retry count is expired, system requires "cold reset" to reset counter.
  //
  PsIssueReset (EfiResetCold);
}

/**
  Perform Sanitization on Storage devices.

  @retval EFI_SUCCESS         Erase device succeed
  @retval EFI_UNSUPPORTED     The device is not supported
  @retval EFI_ACCESS_DENIED   User has entered wrong password too many times
  @retval EFI_ABORTED         The device is supported, but the system
                              has failed to erase it
**/
EFI_STATUS
SanitizeStorageDevices (
  IN PS_BOOT_STORAGE_PARAMETERS     *PsBootStorageParameters
  )
{
  EFI_STATUS                     Status;
  ERASE_CONFIGURATION            EraseConfiguration;

  EraseConfiguration.Value = 0;

  //
  // Start SSD Erase Operation
  //
  EraseConfiguration.b.Verification = (PsBootStorageParameters->PsBootParameters.PsPendingList & PS_VERIFY_STORAGE_MEDIA) ? 1 : 0;
  EraseConfiguration.b.ReportToUsb  = (PsBootStorageParameters->PsBootParameters.TriggerSource == BootLpe) ? 1 : 0;

  //
  // In case of RPE, Get the password from Boot Parameters (Pop UP will be shown if password is empty)
  // In case of LPE, Storage erase driver will Pop-Up a message and get the password directly from User
  //
  Status = PerformSsdErase (
             EraseConfiguration,
             PsBootStorageParameters->PsStorageDevPassword,
             PsStorageEraseComplete
             );
  DEBUG ((DEBUG_INFO, "SSD Erase Status = %r\n", Status));

  return EFI_SUCCESS;
}

/**
  This event gets control if the storage device is connecting.
  The storage sanitization starts from here.

  @param[in]  Event    The instance of EFI_EVENT.
  @param[in]  Context  The parameter passed in.
**/
VOID
EFIAPI
StorageSanitizeNotificationEvent (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  PS_BOOT_STORAGE_PARAMETERS     *PsBootStorageParameters;

  DEBUG ((DEBUG_INFO, "PS: %a () gets notified\n", __FUNCTION__));

  PsBootStorageParameters = (PS_BOOT_STORAGE_PARAMETERS *)Context;
  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "PS: Start performing Storage Sanitization\n"));
  SanitizeStorageDevices (PsBootStorageParameters);
}

/**
  Register Storage Erase Action Event

  @param[in]  PsBootStorageParameters  The parameter passed in.

  @retval EFI_SUCCESS                  Storage Erase event registered successfully to notify when device connecting.
**/
EFI_STATUS
RegisterStorageEraseEvent (
  IN PS_BOOT_STORAGE_PARAMETERS     *PsBootStorageParameters
  )
{
  EFI_EVENT         DeviceConnectingEvent;

  DEBUG ((DEBUG_INFO, "PS: %a Entry\n", __FUNCTION__));

  if (PsBootStorageParameters == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL Parameter Input.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Storage Erase is requested, so this module will take control over HDD's and OPAL's unlocking process
  //
  PcdSetBoolS (PcdSkipHddPasswordPrompt, TRUE);
  PcdSetBoolS (PcdSkipOpalPasswordPrompt, TRUE);
  PcdSetBoolS (PcdRpeOpalDriverEnable, TRUE);

  //
  // Register notify function when device start connecting.
  //
  gBS->CreateEventEx (
         EVT_NOTIFY_SIGNAL,
         TPL_CALLBACK,
         StorageSanitizeNotificationEvent,
         PsBootStorageParameters,
         &gPsStorageDeviceConnectNotifyGuid,
         &DeviceConnectingEvent
         );

  DEBUG ((DEBUG_INFO, "PS: %a Exit\n", __FUNCTION__));
  return EFI_SUCCESS;
}

