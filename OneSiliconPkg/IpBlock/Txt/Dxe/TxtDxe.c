/** @file
  This is the main DXE file for TXT. It represents an abstract outline of the
  steps required during DXE for enabling TXT. Each individual step is further
  abstracted behind a function call interface. This is intended to minimize
  the need to modify this file when porting TXT to future platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "TxtDxeLib.h"

GLOBAL_REMOVE_IF_UNREFERENCED TXT_DXE_LIB_CONTEXT mTxtDxeCtx;

/**
  This function gets registered as a callback to run the LockConfigCallback function
  from the TXT BIOS ACM as a result of Boot Events.

  @param[in]  Event    A pointer to the Event that triggered the callback.
  @param[in]  Context  A pointer to private data registered with the callback function.
  **/
VOID
EFIAPI
LockConfigCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS       Status;
  VOID             *ProtocolPointer;

  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }


  DEBUG ((DEBUG_INFO, "TXTDXE::Running of LockConfig\n"));
  DoLockConfig (&mTxtDxeCtx);

  ///
  /// Closed the event to avoid call twice when launch shell
  ///
  gBS->CloseEvent (Event);
}

/**
  Read CR4, clear SMXE (BIT14), and writeback CR4.

  @param[in] None

  @retval    None
**/

VOID
DisableCR4Smx (
  VOID
  )
{
  IA32_CR4    CR4;

  CR4.UintN = AsmReadCr4 ();
  CR4.Bits.SMXE = 0;
  AsmWriteCr4 (CR4.UintN);
}

/**

  TXT late init flow at thred scope

  @param  None

  @retval None

**/
VOID
EFIAPI
DisableCR4SmxOnAllThreads (
  VOID
  )
{
  EFI_STATUS                           Status;
  EFI_MP_SERVICES_PROTOCOL             *MpServices;

  //
  // Find the MpService Protocol to use with LT Config lock
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **)&MpServices
                  );

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status) || (MpServices == NULL)) {
    DEBUG ((DEBUG_ERROR, "\tMpService Protocol was not found\n"));
    return;
  }

  //
  // Thread scope TXT flow flow
  //
  DEBUG ((DEBUG_INFO, "Disabling CR4.SMXE...\n"));

  DisableCR4Smx ();

  Status = MpServices->StartupAllAPs(
     MpServices,
    (EFI_AP_PROCEDURE) DisableCR4Smx,
    FALSE,
    NULL,
    0,
    NULL,
    NULL
    );

  if (Status == EFI_NOT_STARTED) {
    DEBUG ((DEBUG_ERROR, "APs not started, continue...\n"));
  } else if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error clearing CR4.SMXE on all threads (%r), continuing..\n", Status));
  }
}

/**
  TxtDxeReadyToBoot: handle TXT-related flows before OS handoff.

  @param[in] Event                Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.

  @retval None
**/
VOID
EFIAPI
TxtDxeReadyToBoot (
  EFI_EVENT     Event,
  VOID         *Context
  )
{
  DEBUG ((DEBUG_INFO, "%a: Entry\n",__FUNCTION__));

  //Disable CR4.SMXE on all threads prior to OS-handoff.
  DisableCR4SmxOnAllThreads ();

  gBS->CloseEvent (Event);

  DEBUG((DEBUG_INFO, "%a: Exit\n", __FUNCTION__));
}

/**
  This is the entry point to the TXT DXE Driver.  This routine checks to see if
  the platform should be configured for TXT and if so, configures the platform
  by reserving and initializing TXT Configuration Space and TXT Device Memory and
  registering a callback to run LockConfigCallback from the TXT BIOS ACM prior to boot.

  If the platform should not be configured for TXT, this routine checks the
  establishment bit in the TPM and resets it if it is asserted.

  @param[in]  ImageHandle       A handle for this module
  @param[in]  SystemTable       A pointer to the EFI System Table

  @retval     EFI_SUCCESS       If TXT initialization succeed
  @retval     EFI_UNSUPPORTED   If TXT criterias are not met
**/
EFI_STATUS
EFIAPI
DriverEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS  Status;
  void        *Registration;
  EFI_EVENT   ReadyToBootEvent;
  IA32_CR4    CR4;

  ReadyToBootEvent = 0;
  //
  // Note: SMX feature can be enabled independently of TXT.
  // Therefore check and register for SMXE disable, before
  // checking TXT config status / exiting the driver.
  //
  CR4.UintN = AsmReadCr4 ();
  if (CR4.Bits.SMXE == 1) {
    DEBUG((DEBUG_INFO, "TXTDXE::CR4.SMX Enabled. Calling EfiCreateEventReadyToBootEx\n"));
    if ((EfiCreateEventReadyToBootEx (TPL_CALLBACK, TxtDxeReadyToBoot, NULL, &ReadyToBootEvent)) != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "TXTDXE::SMX Disable Ready To Boot Event creation failure\n"));
    }
  }

  ///
  /// Initialize the platform specific code
  ///
  Status = InitializeTxtDxeLib (ImageHandle, SystemTable, &mTxtDxeCtx);
  ///
  /// If failure - assume TXT is not enabled
  ///
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TXTDXE::InitializeTxtDxeLib failed.... Unloading\n"));
    return EFI_UNSUPPORTED;
  }
  ///
  /// If ESTS.TXTRESET bit is set, skip all other functions since
  /// attempt to execute GETSEC will hang system. Skipping allows to
  /// boot to OS and let MLE assess situation.
  ///
  if (IsTxtResetSet (&mTxtDxeCtx)) {
    DEBUG ((DEBUG_ERROR, "TXTDXE::TXT_RESET bit is set.... Unloading\n"));
    return EFI_UNSUPPORTED;
  }
  ///
  /// If TXT is enabled, configure platform appropriately.
  /// Code assumes that if TXT is enabled by CPU driver than all checks
  /// are passed, i.e. TPM is present, CPU and CS are TXT capable.
  ///
  ///
  /// Add to check CPU TXT capable in case CPU drivers do not check additional requirements
  ///
  if ((mTxtDxeCtx.TxtInfoData->ChipsetIsTxtCapable) && (IsTxtProcessor ()) && (IsTxtEnabled (&mTxtDxeCtx))) {
    DEBUG ((DEBUG_INFO, "TXTDXE::TXT Enabled\n"));

    ///
    /// Allocate and Initialize TXT Device Memory
    ///
    Status = SetupTxtDeviceMemory (&mTxtDxeCtx);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "TXTDXE::SetupTxtDeviceMemory failed.... Unloading\n"));
      return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "Initializing TXT SSDT table\n"));
    InitializeTxtAcpi (ImageHandle);

    ///
    /// Create callback to run LcokConfig; this should execute before global SMI enable is locked
    ///
    EfiCreateProtocolNotifyEvent (
      &gEfiPciEnumerationCompleteProtocolGuid,
      TPL_CALLBACK,
      LockConfigCallback,
      NULL,
      &Registration
      );

  } else {
    ///
    /// TXT is not enabled, so make sure TPM Establishment
    /// bit is de-asserted
    ///
    DEBUG ((DEBUG_INFO, "TXTDXE::TXT Disabled\n"));

    if (IsTxtEstablished (&mTxtDxeCtx)) {
      ///
      /// We can invoke BIOS ACM function only if CS and CPU are TXT
      /// capable
      ///
      if ((mTxtDxeCtx.TxtInfoData->ChipsetIsTxtCapable) &&
          (IsTxtProcessor ()) &&
          !(mTxtDxeCtx.TxtInfoData->Flags & TPM_INIT_FAILED)
          ) {
        DEBUG ((DEBUG_INFO, "TXTDXE::Resetting TPM Establishment bit\n"));
        ResetTpmEstBit (&mTxtDxeCtx);
      }
    }
    ///
    /// Reset AUX
    ///
    Status = ResetTpmAux (&mTxtDxeCtx);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

/**
  This is the entry point to the TXT DXE Driver for installing SSDT ACPI table

  @param[in]  ImageHandle       A handle for this module
  @param[in]  SystemTable       A pointer to the EFI System Table

  @retval     EFI_SUCCESS       If SSDT installation succeed
**/
EFI_STATUS
EFIAPI
InitializeTxtAcpi (
  IN EFI_HANDLE       ImageHandle
  )
{
  EFI_STATUS Status;

  Status = InstallSsdtAcpiTable (gTxtAcpiTableStorageGuid, SIGNATURE_64 ('T','x','t','S','s','d','t',0));

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}