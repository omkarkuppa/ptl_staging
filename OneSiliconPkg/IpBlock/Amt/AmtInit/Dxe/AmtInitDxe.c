/** @file
  Amt Initialization Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <IndustryStandard/AlertStandardFormatTable.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/AslUpdateLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeInfoLib.h>
#include <Protocol/AsfProtocol.h>
#include <Protocol/AmtReadyToBoot.h>
#include <Register/HeciRegs.h>
#include <AmtConfig.h>
#include <MeBiosPayloadHob.h>
#include "AmtInitDxe.h"

/**
  ASF watchdog timer on AMT ready to boot event.
**/
VOID
EFIAPI
WatchdogOnReadyToBoot (
  VOID
  );

GLOBAL_REMOVE_IF_UNREFERENCED AMT_READY_TO_BOOT_PROTOCOL mAmtWatchdogReadyToBoot = {
  AMT_READY_TO_BOOT_PROTOCOL_REVISION,
  WatchdogOnReadyToBoot
};

/**
  Invoke a notification event to update ASF Table RMCP part

  @param[in]  Event                 Event whose notification function is being invoked.
  @param[in]  Context               The pointer to the notification function's context.

**/
VOID
EFIAPI
AsfTableRmcpOnReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                          Status;
  EFI_ACPI_DESCRIPTION_HEADER         *AcpiAsfTable;
  UINTN                               Handle;
  EFI_ACPI_TABLE_PROTOCOL             *AcpiTable;
  EFI_ACPI_ASF_RECORD_HEADER          *AsfRecord;
  EFI_ACPI_ASF_RMCP                   *AsfRmcp;
  ALERT_STANDARD_FORMAT_PROTOCOL      *Asf;

  ///
  /// Locate ACPI Table Protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Locate the ASF Table
  ///
  AcpiAsfTable = NULL;
  Status = LocateAcpiTableBySignature (
             EFI_ACPI_ASF_DESCRIPTION_TABLE_SIGNATURE,
             &AcpiAsfTable,
             &Handle
             );
  if (EFI_ERROR (Status) || (AcpiAsfTable == NULL)) {
    return;
  }

  ///
  /// Locate the RMCP Table
  ///
  AsfRecord = (EFI_ACPI_ASF_RECORD_HEADER *)((UINT8 *)AcpiAsfTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
  do {
    if ((AsfRecord->Type & ASF_RMCP_TYPE) == ASF_RMCP_TYPE) {
      break;
    }
    AsfRecord = (EFI_ACPI_ASF_RECORD_HEADER *)((UINT8 *)AsfRecord + AsfRecord->RecordLength);
  } while ((AsfRecord->Type & ASF_LAST_RECORD_TYPE) != ASF_LAST_RECORD_TYPE);

  if ((AsfRecord->Type & ASF_RMCP_TYPE) != ASF_RMCP_TYPE) {
    DEBUG ((DEBUG_INFO, "ASF_RMCP table NOT found\n"));
    FreePool (AcpiAsfTable);
    return;
  }

  //
  // Get Protocol for ASF
  //
  Status = gBS->LocateProtocol (
                  &gAlertStandardFormatProtocolGuid,
                  NULL,
                  (VOID **) &Asf
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Info : Error gettings ASF protocol -> %r\n", Status));
    FreePool (AcpiAsfTable);
    return;
  }

  AsfRmcp = (EFI_ACPI_ASF_RMCP *) AsfRecord;
  if (!EFI_ERROR (Status) && (Asf->AsfBootOptions.SpecialCommand != NOP)) {
    AsfRmcp->RMCPCompletionCode = ASF_RMCP_BOOT_OPTION_SUCCESS;
    CopyMem (
      &AsfRmcp->RMCPIANA,
      &Asf->AsfBootOptions.IanaId,
      sizeof (Asf->AsfBootOptions) -
      sizeof (Asf->AsfBootOptions.SubCommand) -
      sizeof (Asf->AsfBootOptions.Version)
      );
  }

  if (Handle != 0) {
    Status = AcpiTable->UninstallAcpiTable (AcpiTable, Handle);
    if (EFI_ERROR (Status)) {
      FreePool (AcpiAsfTable);
      return;
    }
  }

  ///
  /// Update the Acpi Asf table
  ///
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        AcpiAsfTable,
                        AcpiAsfTable->Length,
                        &Handle
                        );
  FreePool (AcpiAsfTable);
}

/**
  ASF watchdog timer on EndOfDxe event.

  @param[in] Event          Event whose notification function is being invoked
  @param[in] Context        Context passed to this function.
**/
VOID
EFIAPI
WatchdogOnEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  ///
  /// Stop ASF BIOS watchdog timer before any third party option ROM.
  ///
  AsfStopWatchDog ();
  gBS->CloseEvent (Event);
}

/**
  ASF watchdog timer on AMT ready to boot event.
**/
VOID
EFIAPI
WatchdogOnReadyToBoot (
  VOID
  )
{
  ///
  /// In the end of Ready To Boot phase (before sending End Of Post message),
  /// when BIOS is about to pass control to OS load, BIOS should stop
  /// the watchdog to avoid false watchdog expiration
  ///
  AsfStopWatchDog ();
  ///
  /// Start ASF OS watchdog timer if the corresponding option policy is true
  /// with the non-zero value in the OS timeout setting
  ///
  AsfStartWatchDog (ASF_START_OS_WDT);
}

/**
  Initialize Watchdog Timer.
  If it is corporate IFWI:
  1. BIOS must stop ASF watchdog timer when normal POST after DID message. The reason is because BIOS
     doesn't know what the state of ASF watchdog timer was in the previous boot.
  2. Start BIOS watchdog timer only if Watchdog Timer is enabled.
  3. BIOS registers an EndOfDxe event to stop ASF BIOS watchdog timer after starting it. This way BIOS
     can send Stop ASF BIOS Watchdog Timer command before any third party Option ROM. Restart BIOS
     watchdog timer after third party Option ROM is complete.
  4. In the end of Ready To Boot phase (before sending End Of Post message), when BIOS is about to pass
     control to OS load, BIOS shall stop BIOS watchdog timer to avoid false watchdog expiration in
     case the user boot to OS. Then start OS watchdog timer if the corresponding option policy is true
     with the non-zero value in the OS timeout setting.
**/
VOID
WatchdogInit (
  VOID
  )
{
  EFI_STATUS         Status;
  EFI_HOB_GUID_TYPE  *GuidHob;
  AMT_PEI_CONFIG     *AmtPeiConfig;
  EFI_EVENT          EndOfDxeEvent;
  EFI_HANDLE         Handle;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  AsfStopWatchDog ();

  Handle = NULL;
  GuidHob = GetFirstGuidHob (&gAmtPolicyHobGuid);
  if (GuidHob == NULL) {
    return;
  }

  AmtPeiConfig = (AMT_PEI_CONFIG *) GET_GUID_HOB_DATA (GuidHob);
  if (!AmtPeiConfig->WatchDogEnabled) {
    return;
  }

  AsfStartWatchDog (ASF_START_BIOS_WDT);
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  WatchdogOnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to create event for watchdog on EndOfDxe\n"));
  }

  ///
  /// Install an AMT ready to boot protocol.
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gAmtReadyToBootProtocolGuid,
                  &mAmtWatchdogReadyToBoot,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  The driver entry point - initialize PET alert, progress message and KVM query functions.

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval EFI_SUCCESS             The driver installed without error.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.
**/
EFI_STATUS
EFIAPI
AmtInitDxeEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS             Status;
  ME_BIOS_PAYLOAD_HOB    *MbpHob;
  EFI_EVENT              ReadyToBoot;
  VOID                   *AsfRegistration;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  if (PciSegmentRead32 (MeHeciPciCfgBase (HECI1)) == 0xFFFFFFFF) {
    return EFI_UNSUPPORTED;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No MBP Data Protocol available\n"));
    return EFI_UNSUPPORTED;
  }

  if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw) {
    return EFI_UNSUPPORTED;
  }

  ///
  /// Watchdog function initialization
  ///
  WatchdogInit ();

  ///
  /// KVM function initialization
  ///
  EfiCreateProtocolNotifyEvent (
    &gAlertStandardFormatProtocolGuid,
    TPL_CALLBACK,
    KvmInit,
    NULL,
    &AsfRegistration
    );

  ///
  /// Register a Ready to boot event to config PCIE power management setting after OPROM executed
  ///
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             AsfTableRmcpOnReadyToBoot,
             NULL,
             &ReadyToBoot
             );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

