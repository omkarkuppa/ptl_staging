/** @file
  USB4 connection manager Dxe driver.

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

#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/ResetNotification.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/ITbtPolicy.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/Usb4PlatformInfo.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4HrInst.h>
#include <Library/Usb4PlatformHob.h>
#include <Library/Usb4CmMode.h>
#include <Usb4LogMappingTable.h>
#include <Library/Usb4DebugLib.h>
#include "Usb4CmDxe.h"
#include "PostPciUsb4Cm.h"
#include "Usb4Hr.h"

EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log = NULL;

/**
  USB4 CM EndOfDxe handler

  @param[in] Event   - Event handle.
  @param[in] Context - Context for ExitBootService handler.
**/
VOID
EFIAPI
Usb4CmEndOfDxe (
  EFI_EVENT           Event,
  VOID                *Context
  )
{
  EFI_STATUS          Status;
  USB4_HR_INSTANCE    *Usb4Hr;
  EFI_TPL             OldTpl;

  DEBUG ((DEBUG_INFO, "Usb4CmEndOfDxe entry - Context = %p\n", Context));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_EOF_DXE_ENTRY, 0, 0);

  Usb4Hr = (USB4_HR_INSTANCE *) Context;
  if (Usb4Hr != NULL) {
    //
    // Start Connection Mangers that have been stopped before PCI enumeration.
    //
    OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
    Status = PostPciCmStart (Usb4Hr);
    gBS->RestoreTPL (OldTpl);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4CmEndOfDxe: CM start failure, %r\n", Status));
      goto Exit;
    }

    //
    // Start the USB4 CM Monitor Timer
    //
    if (Usb4Hr->PollTimer == NULL) {
      DEBUG ((DEBUG_ERROR, "Usb4CmEndOfDxe: Null poll timer!\n"));
      goto Exit;
    }
    Status = gBS->SetTimer (Usb4Hr->PollTimer, TimerPeriodic, USB4_CM_MONITOR_TIMER_INTERVAL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4CmEndOfDxe: Set poll timer failure, %r\n", Status));
      goto Exit;
    }
  }

Exit:
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_EOF_DXE_EXIT, 0, 0);
  DEBUG ((DEBUG_INFO, "Usb4CmEndOfDxe exit\n"));
}

/**
  USB4 CM ExitBootService handler

  @param[in] Event   - Event handle.
  @param[in] Context - Context for ExitBootService handler.
**/
VOID
EFIAPI
Usb4CmExitBootService (
  EFI_EVENT    Event,
  VOID         *Context
  )

{
  USB4_HR_INSTANCE   *Usb4Hr;

  DEBUG ((DEBUG_INFO, "Usb4CmExitBootService entry - Context = %p\n", Context));

  Usb4Hr = (USB4_HR_INSTANCE *) Context;
  if (Usb4Hr != NULL) {
    //
    // Cancel Monitor Timer and close the event
    //
    if (Usb4Hr->PollTimer != NULL) {
      gBS->SetTimer (Usb4Hr->PollTimer, TimerCancel, 0);
      gBS->CloseEvent (Usb4Hr->PollTimer);
    }
    PostPciCmDestroy (Usb4Hr);
  }

  DEBUG ((DEBUG_INFO, "Usb4CmExitBootService exit\n"));
}

/**
   This function un-power TBT controller before system reset.

   @param[in] ResetType   - Type of reset to perform.
   @param[in] ResetStatus - Status code of the reset.
   @param[in] DataSize    - Size of ResetData, in bytes.
   @param[in] ResetData   - Optional Null-terminated string which can be used to introduce a platform specific reset.
**/
VOID
EFIAPI
Usb4PlatformResetSystem (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "Usb4PlatformResetSystem entry - ResetType = %x\n", ResetType));

  Usb4CmUnpowerHr ();
}

/**
  This function hooks system reset to allow performing correct reset.

  @param[in] Event   - A pointer to the Event that triggered the callback.
  @param[in] Context - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
Usb4OnResetHandlerInstall (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                        Status;
  EFI_RESET_NOTIFICATION_PROTOCOL   *ResetNotify;

  DEBUG ((DEBUG_INFO, "Usb4OnResetHandlerInstall entry\n"));
  Status = gBS->LocateProtocol (
                  &gEfiResetNotificationProtocolGuid,
                  NULL,
                  (VOID **) &ResetNotify
                  );

  if (!EFI_ERROR (Status)) {
    ResetNotify->RegisterResetNotify (ResetNotify, Usb4PlatformResetSystem);
    DEBUG ((DEBUG_INFO, "Usb4PlatformResetSystem() registered\n"));
    gBS->CloseEvent (Event);
  }
  DEBUG ((DEBUG_INFO, "Usb4OnResetHandlerInstall exit\n"));
}

/**
  Register connection manager callback functions for hot plug support.

  @param[out] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Register CM callback successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Create event error.
**/
EFI_STATUS
Usb4CmRegisterCallback (
  USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     EndOfDxeEvent;
  EFI_EVENT     ExitBootServiceEvent;

  DEBUG ((DEBUG_INFO, "Usb4CmRegisterCallback entry - Usb4Hr = %p\n", Usb4Hr));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4CmRegisterCallback: Null Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Create CM Monitor Timer
  //
  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  Usb4CmMonitor,
                  Usb4Hr,
                  &(Usb4Hr->PollTimer)
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4CmRegisterCallback: Create USB4 CM poll timer event failure, %r\n", Status));
    Usb4Hr->PollTimer = NULL;
    goto Exit;
  }

  //
  // Create an End of DXE event.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  Usb4CmEndOfDxe,
                  Usb4Hr,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4CmRegisterCallback: Create EndOfDxe event failure, %r\n", Status));
    goto Exit;
  }

  //
  //
  // Create event to stop the USB4 CM Monitor in the exit boot service.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  Usb4CmExitBootService,
                  Usb4Hr,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServiceEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4CmRegisterCallback: Create ExitBootService event failure, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4CmRegisterCallback exit\n"));
  return Status;
}

/**
  Register event callback for each USB4 host router instance in platform.
**/
VOID
RegisterCmCallback (
  VOID
  )
{
  UINT32    HrIndex;
  UINT32    HrCount;
  VOID      *Registration;

  DEBUG ((DEBUG_INFO, "Register reset hook for Usb4CmDxe\n"));
  EfiCreateProtocolNotifyEvent (
    &gEfiResetNotificationProtocolGuid,
    TPL_CALLBACK,
    Usb4OnResetHandlerInstall,
    NULL,
    &Registration
    );

  HrCount = (mUsb4HrCount >= USB4_HR_SUPPORT_MAX) ? USB4_HR_SUPPORT_MAX : mUsb4HrCount;

  for (HrIndex = 0; HrIndex < HrCount; HrIndex++) {
    Usb4CmRegisterCallback (mUsb4HrInst[HrIndex]);
  }
}

/**
  Query platform CM support information.

  @param[out] PlatformUsb4HrInfo - Pointer of pointer to the USB4 host router information in the platform.

  @retval EFI_SUCCESS           - Query platform CM support information successfully.
  @retval EFI_UNSUPPORTED       - Fail to query platform CM support information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
QueryPlatformUsb4Support (
  OUT USB4_PLATFORM_INFO    **Usb4PlatformInfo
  )
{
  EFI_STATUS               Status;
  VOID                     *HobPtr;
  USB4_PLATFORM_HOB        *Usb4PlatformHob;
  USB4_PLATFORM_INFO       *HrInfo;

  DEBUG ((DEBUG_INFO, "QueryPlatformUsb4Support entry\n"));

  if (Usb4PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryPlatformUsb4Support: Null Usb4PlatformInfo\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Find USB4 Hob to get platform USB4 host router information
  //
  HobPtr = GetFirstGuidHob (&gUsb4PlatformHobGuid);
  if (HobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "QueryPlatformUsb4Support: Unable to find USB4 platform hob!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  Usb4PlatformHob = GET_GUID_HOB_DATA (HobPtr);
  HrInfo = &(Usb4PlatformHob->Usb4PlatformInfo);

  *Usb4PlatformInfo = HrInfo;

  DEBUG ((DEBUG_INFO, "USB4 HR count = %d, HrMask = 0x%0X, CM mode = 0x%0x\n", HrInfo->Usb4HrCount, HrInfo->Usb4HrMask, HrInfo->CmMode));
  Status = EFI_SUCCESS;
Exit:
  DEBUG ((DEBUG_INFO, "QueryPlatformUsb4Support exit\n"));
  return Status;
}

/**
  USB4 Connection Manager Dxe entry.

  @param[in] ImageHandle  ImageHandle of the loaded driver
  @param[in] SystemTable  Pointer to the System Table

  @retval EFI_SUCCESS - USB4 CM Dxe is executed successfully.
**/
EFI_STATUS
EFIAPI
Usb4CmDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS            Status;
  USB4_PLATFORM_INFO    *Usb4PlatformInfo;

  DEBUG ((DEBUG_INFO, "Usb4CmDxeEntryPoint entry\n"));

  //
  // Query platform USB4 support information
  //
  Status = QueryPlatformUsb4Support (&Usb4PlatformInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Query platform USB4 support failure, %r\n", Status));
    goto Exit;
  }

  //
  // Check if USB4 Debug Capability Protocol is enabled
  //
  if (Usb4PlatformInfo->Usb4DebugLevel != USB4_DBG_DISABLED) {
    //
    // Install USB4 Debug Protocol
    //
    Status = InstallUsb4DebugProtocol (
                 &gEfiUsb4DebugProtocolGuid,
                 Usb4PlatformInfo->Usb4DebugLevel,
                 mUsb4LogMappingTable,
                 mUsb4LogMappingEntries,
                 &mUsb4Log
                 );
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Install USB4 Debug Protocol - Debug Level = %d\n", Usb4PlatformInfo->Usb4DebugLevel));
    } else {
      //
      // Debug protocol installation failure, but USB4 CM DXE execution should continue
      //
      DEBUG ((DEBUG_ERROR, "Install USB4 Debug Protocol failure, %r\n", Status));
      mUsb4Log = NULL;
    }
  } else {
    mUsb4Log = NULL;
  }

  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_CM_DXE_ENTRY, 0, 0);
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_CM_MODE_INFO, Usb4PlatformInfo->CmModeOption, Usb4PlatformInfo->CmMode);
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_HR_COUNT, Usb4PlatformInfo->Usb4HrCount, 0);

  //
  // Skip SW CM execution if the applied CM mode is not SW CM or CM debug option is selected
  //
  if ((Usb4PlatformInfo->CmMode != USB4_CM_MODE_SW_CM) || (Usb4PlatformInfo->CmModeOption == USB4_SETUP_CM_MODE_DEBUG)) {
    DEBUG ((DEBUG_INFO, "SW CM option is not enabled, skip USB4 SW CM execution!\n"));
    goto Exit;
  }

  //
  // SKip SW CM execution if no USB4 Host Router is enabled in platform
  //
  if (Usb4PlatformInfo->Usb4HrCount == 0) {
    DEBUG ((DEBUG_INFO, "No USB4 Host Router enabled in platform, skip USB4 SW CM execution!\n"));
    goto Exit;
  }

  //
  // Create and execute CM for all USB4 host routers in the platform.
  //
  Status = Usb4CmExecute (ImageHandle, Usb4PlatformInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "USB4 CM execution failure, %r\n", Status));
    goto Exit;
  }

  //
  // Stop CM to release PCI resources and DMA buffer.
  // PCI resources will be assigned by PCI bus driver after PCI enumeration.
  // DMA buffer should be allocated by PCI I/O protocol.
  // USB4 router and DP hot plug will be supported after PCI enumeration.
  //
  Usb4CmStop ();

  //
  // Register callback for hot plug support.
  //
  RegisterCmCallback ();

Exit:
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_CM_DXE_EXIT, 0, 0);
  DEBUG ((DEBUG_INFO, "Usb4CmDxeEntryPoint exit\n"));
  return EFI_SUCCESS;
}
