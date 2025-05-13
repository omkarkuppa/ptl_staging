/** @file
  This is the Fsp driver that initializes the Intel Silicon.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/EventGroup.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Pi/PiStatusCode.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DxeSaInitFruLib.h>
#include <Library/PerformanceLib.h>


/**
  This function gets registered as a callback to perform Silicon configuration on PCI enumeration complete.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SiOnPciEnumerationCompleteDxeFsp (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  This function is to perform silicon tasks on exit boot service.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
SiInitExitBootCallbackFsp (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Silicon Fsp Initialization.

  @param[in] ImageHandle             Handle for the image of this driver
  @param[in] SystemTable             Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SiInitEntryPointDxeFsp (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;
  VOID                      *Registration;
  EFI_EVENT                 Event;

  DEBUG ((DEBUG_INFO, "SiInitEntryPointDxeFsp Start\n"));

  PERF_INMODULE_BEGIN ("SiInitDxeFsp");

  ///
  /// Create PCI Enumeration Completed callback for Silicon
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    SiOnPciEnumerationCompleteDxeFsp,
    NULL,
    &Registration
    );

  ///
  /// Register ExitBootService() to before OS handoff
  ///
  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_CALLBACK,
                  SiInitExitBootCallbackFsp,
                  NULL,
                  &Event
                  );
  if (EFI_ERROR (Status)) {
    REPORT_STATUS_CODE_EX (
      (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
      (EFI_COMPUTING_UNIT_CHIPSET|EFI_CHIPSET_EC_DXE_NB_ERROR),
      0,
      &gSiFspErrorTypeCallerId,
      NULL,
      NULL,
      0
      );
  }
  ASSERT_EFI_ERROR (Status);

  PERF_INMODULE_END ("SiInitDxeFsp");

  DEBUG ((DEBUG_INFO, "SiInitEntryPointDxeFsp End\n"));

  return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to perform Silicon configuration on PCI enumeration complete

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SiOnPciEnumerationCompleteDxeFsp (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  return;
}

/**
  This function is to perform silicon tasks on exit boot service.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
SiInitExitBootCallbackFsp (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  DEBUG ((DEBUG_INFO, "SiInitExitBootCallbackFsp Start\n"));

  PamLock ();

  DEBUG ((DEBUG_INFO, "SiInitExitBootCallbackFsp End\n"));
}