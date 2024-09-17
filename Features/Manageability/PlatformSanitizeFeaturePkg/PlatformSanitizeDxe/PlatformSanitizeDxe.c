/**@file
  Platform Sanitization Dxe driver.

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PlatformSanitizeServices.h>
#include <Library/PcdLib.h>
#include <Library/PsCsmeAccessLib.h>
#include <Library/PsCsmeAccessRpeLib.h>
#include <PsParameter.h>
#include "PlatformSanitize.h"

GLOBAL_REMOVE_IF_UNREFERENCED UINT32                          mPsCapabilities;
GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_PARAMETERS              *mPsBootParameters;
GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_STORAGE_PARAMETERS      mPsBootStorageParameter;
GLOBAL_REMOVE_IF_UNREFERENCED PS_OEM_HOOK_TLV_DATA            mPsOemHookTlvData;

/**
  Check whether Platform Sanitize requested by LPE (From BIOS Setup) or RPE (From Remote access)
  or CSME Unconfigure from Remote access.

  @retval TRUE     Platform Sanitize requested
  @retval FALSE    Platform Sanitize Not requested
**/
BOOLEAN
IsPlatformSanitizeRequested (
  VOID
  )
{
  BOOLEAN  RpeTriggered;
  BOOLEAN  CsmeUnConfigViaCdr;
  BOOLEAN  RemoteCsmeUnConfigTriggered;

  RpeTriggered = FALSE;
  RemoteCsmeUnConfigTriggered = FALSE;
  CsmeUnConfigViaCdr = FALSE;
  mPsBootParameters->TriggerSource = BootNone;

  //
  // Check for RPE Triggered from remote access
  //
  if (mPsCapabilities & PS_RPE_SUPPORTED) {
    RpeTriggered = PsIsRpeEnabledAndTriggeredByAmt ();
  }

  //
  // Check for ConfigurationDataReset.
  // When this bit is set, CSME unconfiguration to be performed.
  //
  CsmeUnConfigViaCdr = IsConfigDataResetTriggered ();

  //
  // Given 1st priority to LPE as the user expected infront of the system.
  // So if LPE is triggered, ignore RPE / Remote CSME Unconfig but send PET alerts as notification.
  //
  if (PsIsLpeEnabled ()) {
    DEBUG ((DEBUG_INFO, "PS: LPE Triggered.\n"));
    mPsBootParameters->TriggerSource = BootLpe;

    if (RpeTriggered || RemoteCsmeUnConfigTriggered) {
      DEBUG ((DEBUG_INFO, "PS: RPE/CSME Unconfig Triggered when LPE is Trigered, Ignoring RPE, CSME Unconfig actions.\n"));
      PsPetAlert (PsProgressFailure, PsPetEraseGeneral, PsInvalidBootParameters);
      RpeTriggered = FALSE;
      CsmeUnConfigViaCdr = FALSE;
    }
  } else if (RpeTriggered && CsmeUnConfigViaCdr) {
    // If both RPE & CSME Unconfig are set, it is Invalid Trigger.
    DEBUG ((DEBUG_INFO, "PS: Both RPE and CSME Unconfig Triggered at the same time.\n"));
    DEBUG ((DEBUG_INFO, "PS: No Sanitize takes place.\n"));
    PsPetAlert (PsProgressFailure, PsPetEraseGeneral, PsInvalidBootParameters);
  } else if (RpeTriggered) {
    DEBUG ((DEBUG_INFO, "PS: RPE Triggered.\n"));
    mPsBootParameters->TriggerSource = BootRpe;
  } else if (CsmeUnConfigViaCdr) {
    DEBUG ((DEBUG_INFO, "PS: Remote CSME Unconfiguration Triggered.\n"));
    mPsBootParameters->TriggerSource = BootConfigDataReset;
  } else if (IsPreviousBootSanitizeContinuation()) {
      // Config Data Reset gets cleared on reset. Hence checking with Pending list.
      if (mPsBootParameters->PsPendingList & PS_CSME_UNCONFIGURE) {
        DEBUG((DEBUG_INFO, "PS: Remote CSME Unconfiguration Triggered.\n"));
        mPsBootParameters->TriggerSource = BootConfigDataReset;
      }
  } else {
    DEBUG ((DEBUG_INFO, "PS: Not Triggered.\n"));
  }

  if (mPsBootParameters->TriggerSource == BootNone) {
    return FALSE;
  }
  return TRUE;
}

/**
  Get Sanitize parameters such as list of erase actions to be performed,
  Storage password in case of RPE, and update the global structure 'mPsBootParameters'.

  @retval EFI_SUCCESS             - Successfully received the parameters and updated the structure.
  @retval EFI_INVALID_PARAMETER   - Memory not allocated for the structure.
  @retval Others                  - Failed to receive Erase parameters.
**/
EFI_STATUS
GetSanitizeParameters (
  VOID
  )
{
  EFI_STATUS              Status;

  if (mPsBootParameters == NULL) {
    DEBUG ((DEBUG_ERROR, "PS-%a: NULL Parameter structure.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_NOT_FOUND;

  //
  // Check whether to continue Sanitize from previous boot.
  // In this case, NVM Variable does exists and maintains the track of ersae actions.
  //
  if (IsPreviousBootSanitizeContinuation ()) {
    DEBUG ((DEBUG_INFO, "PS: Continuation from previous boot Sanitize.\n"));
    //
    // Received PsBootParameters from BIOS NVM file.
    // If Storage Erase is pending and Trigger Source is RPE then Storage Erase password from AMT
    //
    if (mPsBootParameters->TriggerSource == BootRpe) {
      if ((mPsBootParameters->PsPendingList & PS_ERASE_STORAGE_MEDIA) ||
          (mPsBootParameters->PsPendingList & PS_OEM_CUSTOM_ACTION)) {
        PsGetRpeAdditionalParameters (mPsBootStorageParameter.PsStorageDevPassword, &mPsOemHookTlvData);
      }
    }
    return EFI_SUCCESS;
  }

  //
  // Sanitize triggered freshly, Get Boot parameters from triggered source.
  //
  switch (mPsBootParameters->TriggerSource) {
    case BootLpe:
      DEBUG ((DEBUG_INFO, "PS: Get Sanitize trigger information from Setup.\n"));
      Status = PsGetLpeSanitizeParameter (mPsBootParameters);
      break;

    case BootRpe:
      DEBUG ((DEBUG_INFO, "PS: Get Sanitize trigger information from CSME-AMT.\n"));
      Status = PsGetRpeSanitizeParameter (mPsBootParameters, mPsBootStorageParameter.PsStorageDevPassword, &mPsOemHookTlvData);
      break;

    case BootConfigDataReset:
      DEBUG ((DEBUG_INFO, "PS: Get Sanitize trigger information from Config Data Reset.\n"));
      Status = PsGetRpeConfigDataReset (mPsBootParameters);
      break;

    default:
      break;
  }

  //
  // Make sure only Capability support erase action bits are requested and
  // assign to PendingList.
  //
  mPsBootParameters->PsRequestedList &= PS_PERFORM_ACTION_MASK;
  mPsBootParameters->PsPendingList = mPsBootParameters->PsRequestedList;

  DEBUG((DEBUG_INFO, "PS: Boot Parameters list, after getting from triggered source:\n"));
  DEBUG((DEBUG_INFO, "PS: TriggerSource    0x%x \n", mPsBootParameters->TriggerSource));
  DEBUG((DEBUG_INFO, "PS: PsRequestedList  0x%x \n", mPsBootParameters->PsRequestedList));
  DEBUG((DEBUG_INFO, "PS: PsPendingList    0x%x \n", mPsBootParameters->PsPendingList));
  DEBUG((DEBUG_INFO, "PS: IsSanitizeSummaryDisplayed   0x%x \n", mPsBootParameters->IsSanitizeSummaryDisplayed));
  DEBUG((DEBUG_INFO, "PS: PsAttempted      0x%x \n", mPsBootParameters->PsStatus.PsAttempted));
  DEBUG((DEBUG_INFO, "PS: PsAttemptResult  0x%x \n", mPsBootParameters->PsStatus.PsAttemptResult));

  if (Status == EFI_SUCCESS) {
    // Update the PsBootParameters at NVM Variable.
    PsStoreBootParametersAtNvm ();
  } else {
    PsPetAlert (PsProgressFailure, PsPetEraseGeneral, PsBootParameterReceive);
  }

  return Status;
}

/**
  Entry point of the Intel Platform Sanitization DXE Driver

  @param[in] ImageHandle     The firmware allocated handle for the EFI image.
  @param[in] SystemTable     A pointer to the EFI System Table.

  @retval EFI_SUCCESS        Driver executed successfully, Checks for capability support,
                             if sanitize trigger, register EOD callback to perform and exit the driver.
  @retval Others             The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
PlatformSanitizeEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                   Status;

  DEBUG ((DEBUG_INFO, "PS: Platform Sanitization Driver EntryPoint.\n"));

  //
  // Get Sanitize Capabilities from PCD
  //
  mPsCapabilities = FixedPcdGet32 (PcdPsSupportCapabilities) & PS_CAPABILITY_MASK;

  //
  // Brand Identity Check for RPE support
  //
  if (mPsCapabilities & PS_RPE_SUPPORTED) {
    if (!IsBrandIdentityEnabledForRpe ()) {
      mPsCapabilities &= ~PS_RPE_SUPPORTED;     // Since Brand Identity not supported, Clear RPE bit in the Capability list.
    }
  }

  mPsBootParameters = AllocateZeroPool (sizeof (PS_BOOT_PARAMETERS));
  if (mPsBootParameters == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: mPsBootParameters EFI_OUT_OF_RESOURCES.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Check whether Sanitize requested.
  // It can be either from BIOS setup or Remote access
  //
  if (!IsPlatformSanitizeRequested ()) {
    DEBUG ((DEBUG_INFO, "PS: Platform Sanitize Not Requested, Exit.\n"));
    FreePool (mPsBootParameters);
    return EFI_SUCCESS;
  }

  // Begin the Platform Sanitization
  PsPetAlert (PsProgressInProgress, PsPetEraseGeneral, PsBeginPlatformSanitize);
  PsReportBiosStatus (PsInProgress);

  //
  // Get Platform Sanitize requested details
  //
  Status = GetSanitizeParameters ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS: Failed to get Sanitize Parameters, Exit.\n"));
    FreePool (mPsBootParameters );
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check for Simulation and if enabled then enter into Simulation mode
  //
#if FixedPcdGetBool(PcdPsSimulationSupport) == 1
  PsSimulation ();
#endif

  //
  // If storage erase is requested, then before End Of Dxe, below execution should takes place
  //    - PCD settings to take control over HDD's and OPAL's unlocking process and
  //    - Registering notify function when device start connecting
  // So Storage Sanitize should be treated separately and not to include with End Of Dxe Callback.
  //
  if (mPsBootParameters->PsPendingList & PS_ERASE_STORAGE_MEDIA) {
    //
    // Register SSD Erase Action as an event to be performed in BDS Phase.
    //
    CopyMem (&(mPsBootStorageParameter.PsBootParameters), mPsBootParameters, sizeof (PS_BOOT_PARAMETERS));

    Status = RegisterStorageEraseEvent (&mPsBootStorageParameter);
    DEBUG ((DEBUG_INFO, "PS: Storage Erase Register Event Status = %r\n", Status));
  } else {
    //
    // Perform Sanitize actions
    //
    PerformSanitizeActions ();
  }

  DEBUG ((DEBUG_INFO, "PS: Platform Sanitization Driver Exit.\n"));
  return EFI_SUCCESS;
}
