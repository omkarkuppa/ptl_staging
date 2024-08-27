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
#include <Library/Tpm2CommandLib.h>
#include <PsParameter.h>
#include <Library/PlatformSanitizeServices.h>
#include <Protocol/PsBoardOemProtocol.h>
#include <PchResetPlatformSpecific.h>
#include <Library/SanitizeUiLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/PlatformSanitizeVariable.h>
#include <Library/PsCsmeAccessLib.h>
#include <Library/PsCsmeAccessRpeLib.h>
#include <Library/PrintLib.h>
#include "PlatformSanitize.h"

extern GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_PARAMETERS              *mPsBootParameters;
extern GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_STORAGE_PARAMETERS      mPsBootStorageParameter;
extern GLOBAL_REMOVE_IF_UNREFERENCED PS_OEM_HOOK_TLV_DATA            mPsOemHookTlvData;

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR16 *mGenOpCodeStr[] = {
    L"Boot Parameters Received",
    L"Started Platform Sanitize",
    L"Cleared Reserved Parameters",
    L"Invalid Boot Parameters",
    L"All Setting Decremented",
  };

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR16 *mDevEraseOpCodeStr[] = {
    L"Started",
    L"Successful",
    L"Failed",
    L"Not Supported"
  };

/**
  Print Sanitize Alert Information.
  Additionally, In case of RPE, Send operation status using PET alerts.

  @param[in]    PsStatus           Operation status either in Progress or Failure
  @param[in]    PsEraseAction      Erase action details
  @param[in]    PsOperation        Erase operation details

  @retval EFI_SUCCESS               Function executed successfully
  @retval Others                    Return HECI status in case of RPE
**/
EFI_STATUS
PsPetAlert (
  IN PS_PET_STATUS                 PsStatus,
  IN PS_PET_ERASE_ACTION           PsEraseAction,
  IN PS_PET_OPCODE                 PsOperation
  )
{
  EFI_STATUS         Status;

  Status = EFI_SUCCESS;
  if (mPsBootParameters->TriggerSource == BootRpe) {
    Status = PsRpeAlertInfo (PsStatus, PsEraseAction, PsOperation);
  }

  DEBUG_CODE_BEGIN ();
  if (PsStatus == PsProgressInProgress) {
    DEBUG ((DEBUG_INFO, "Ps-Alert: In Progress\n"));
  } else if (PsStatus == PsProgressFailure) {
    DEBUG ((DEBUG_INFO, "Ps-Alert: Failed\n"));
  } else {
    return Status;
  }

  if (PsEraseAction == PsPetEraseGeneral && PsOperation >= PsGeneralOpcodeMax) {
    DEBUG ((DEBUG_INFO, "Invalid PET parameter for General Opration\n"));
    return EFI_UNSUPPORTED;
  } else if (PsEraseAction > PsPetEraseGeneral && (PsOperation < PsGeneralOpcodeMax || PsOperation >= PsEraseActionCodeMax)) {
    DEBUG ((DEBUG_INFO, "Invalid PET parameter for Erase Action Opration\n"));
    return EFI_UNSUPPORTED;
  }

  if (PsOperation >= PsGeneralOpcodeMax) {
    //
    // Switch to use DevEraseOpCodeStr to print erase action result.
    //
    PsOperation -= PsGeneralOpcodeMax;
  }

  switch (PsEraseAction) {
    case PsPetEraseGeneral:
      DEBUG ((DEBUG_INFO, "    %s\n", mGenOpCodeStr[PsOperation]));
      break;
    case PsPetEraseStorage:
      DEBUG ((DEBUG_INFO, "   Storage Erase %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseTpmClear:
      DEBUG ((DEBUG_INFO, "   TPM Clear %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseCustomAction:
      DEBUG ((DEBUG_INFO, "   Custom Action %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseClearNvm:
      DEBUG ((DEBUG_INFO, "   Clear All NVM Variable %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseReloadConfig:
      DEBUG ((DEBUG_INFO, "   BIOS Golden Config %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    case PsPetEraseCsmeUnconfig:
      DEBUG ((DEBUG_INFO, "   CSME Unconfigure %s\n", mDevEraseOpCodeStr[PsOperation]));
      break;
    default:
      break;
  }
  DEBUG_CODE_END ();

  return Status;
}

/**
  Function to trigger Reset.

  @param[in]  ResetType         The type of reset to perform.
**/
VOID
PsIssueReset (
  IN EFI_RESET_TYPE           ResetType
  )
{
  PCH_RESET_DATA ResetData;

  switch (ResetType) {
    case EfiResetPlatformSpecific:
      CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
      StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
      gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
      break;

    case EfiResetWarm:
    case EfiResetCold:
      gRT->ResetSystem (ResetType, EFI_SUCCESS, 0, NULL);
      break;

    default:
      ASSERT (FALSE);
      break;
  }

  // Control should not come here.
  CpuDeadLoop ();
}

/**
  Print Debug and Pop-Up Messages to display Sanitization Summary information
**/
VOID
PrintSanitizeSummary (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "\n*** Platform Sanitization Summary Report ***\n"));
  gST->ConOut->OutputString (gST->ConOut, L"\n                  *** Platform Sanitization Summary Report ***\n\r");

  if (mPsBootParameters->TriggerSource == BootLpe) {
    DEBUG ((DEBUG_INFO, "\nPlatform Sanitize Triggered from: BIOS Setup\n"));
    gST->ConOut->OutputString (gST->ConOut, L"\n  Platform Sanitize Triggered Source: BIOS Setup\n\r");
  } else {
    DEBUG ((DEBUG_INFO, "\nPlatform Sanitize Triggered from: Remote Access\n"));
    gST->ConOut->OutputString (gST->ConOut, L"\n  Platform Sanitize Triggered Source: Remote Access\n\r");
  }

  DEBUG ((DEBUG_INFO, "PS: Requested Sanitize Items = %x\n", mPsBootParameters->PsRequestedList));
  DEBUG ((DEBUG_INFO, "PS: Attempted Sanitize Items = %x\n", mPsBootParameters->PsStatus.PsAttempted));
  DEBUG ((DEBUG_INFO, "PS: Attempted Sanitize Result = %x\n", mPsBootParameters->PsStatus.PsAttemptResult));

  if (mPsBootParameters->PsRequestedList & PS_ERASE_STORAGE_MEDIA) {
    DEBUG ((DEBUG_INFO, "Storage Device - "));
    gST->ConOut->OutputString (gST->ConOut, L"  Storage Device - ");
    if (mPsBootParameters->PsStatus.PsAttemptResult & PS_ERASE_STORAGE_MEDIA) {
      if (mPsBootParameters->PsRequestedList & PS_VERIFY_STORAGE_MEDIA) {
        if (mPsBootParameters->PsStatus.PsAttemptResult & PS_VERIFY_STORAGE_MEDIA) {
          DEBUG ((DEBUG_INFO, "Successfully Erased & Verified.\n"));
          gST->ConOut->OutputString (gST->ConOut, L"Successfully Erased & Verified.\n\r");
        } else {
          DEBUG ((DEBUG_INFO, "Successfully Erased but Verification Failed.\n"));
          gST->ConOut->OutputString (gST->ConOut, L"Successfully Erased but Verification Failed.\n\r");
        }
      } else {
        DEBUG ((DEBUG_INFO, "Successfully Erased. (Verification - Not selected)\n"));
        gST->ConOut->OutputString (gST->ConOut, L"Successfully Erased. (Verification - Not selected).\n\r");
      }
    } else {
      DEBUG ((DEBUG_INFO, "Failed to Erase.\n"));
      gST->ConOut->OutputString (gST->ConOut, L"Failed to Erase.\n\r");
    }
  }

  if (mPsBootParameters->PsRequestedList & PS_TPM_CLEAR) {
    DEBUG((DEBUG_INFO, "TPM Clear -"));
    gST->ConOut->OutputString(gST->ConOut, L"  TPM Clear - ");
    if (mPsBootParameters->PsStatus.PsAttemptResult & PS_TPM_CLEAR) {
      DEBUG ((DEBUG_INFO, "Success\n"));
      gST->ConOut->OutputString (gST->ConOut, L"Success\n\r");
    } else {
      DEBUG ((DEBUG_INFO, "Failed\n"));
      gST->ConOut->OutputString (gST->ConOut, L"Failed\n\r");
    }
  }

  if (mPsBootParameters->PsRequestedList & PS_OEM_CUSTOM_ACTION) {
    DEBUG((DEBUG_INFO, "OEM Custom Action -"));
    gST->ConOut->OutputString(gST->ConOut, L"  OEM Custom Action - ");
    if (mPsBootParameters->PsStatus.PsAttemptResult & PS_OEM_CUSTOM_ACTION) {
      DEBUG ((DEBUG_INFO, "Success\n"));
      gST->ConOut->OutputString (gST->ConOut, L"Success\n\r");
    } else {
      DEBUG ((DEBUG_INFO, "Failed\n"));
      gST->ConOut->OutputString (gST->ConOut, L"Failed\n\r");
    }
  }

  if (mPsBootParameters->PsRequestedList & (PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION | PS_BIOS_RELOAD_GOLDEN_CONFIG | PS_CSME_UNCONFIGURE)) {
    DEBUG ((DEBUG_INFO, "Below Erase Actions will attempt after system reboot. (Result captured at Debug logs, PET alerts (only for RPE) and PSR logs (if enabled)).\n"));
    gST->ConOut->OutputString (gST->ConOut, L"  Below Erase Actions will attempt after system reboot.\n\r  (Result captured at Debug logs, PET alerts (only for RPE) and PSR logs (if enabled)).\n\r");
  }

  if (mPsBootParameters->PsRequestedList & PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION) {
    DEBUG ((DEBUG_INFO, "Clear All BIOS NVM Variable Region\n"));
    gST->ConOut->OutputString (gST->ConOut, L"    Clear All BIOS NVM Variable Region.\n\r");
  }

  if (mPsBootParameters->PsRequestedList & PS_BIOS_RELOAD_GOLDEN_CONFIG) {
    DEBUG ((DEBUG_INFO, "BIOS Reload Golden Config.\n"));
    gST->ConOut->OutputString (gST->ConOut, L"    BIOS Reload Golden Config.\n\r");
  }

  if (mPsBootParameters->PsRequestedList & PS_CSME_UNCONFIGURE) {
    DEBUG ((DEBUG_INFO, "CSME Unconfigure.\n"));
    gST->ConOut->OutputString (gST->ConOut, L"    CSME Unconfigure.\n\r");
  }

  DEBUG ((DEBUG_INFO, "\n*** Platform Sanitization End of the Report ***\n"));
  gST->ConOut->OutputString (gST->ConOut, L"\n                  *** Platform Sanitization End of the Report ***\n\r");
}

/**
  Storage erase report to send to CSME to store in AMT-WEB storage

  @param[in]  ReportNum     Report count.
  @param[in]  Report        Pointer to an array of report
**/
VOID
EFIAPI
PsStorageEraseReport (
  IN UINT8                   ReportNum,
  IN ERASE_DOCUMENTATION     *Report
  )
{
  EFI_STATUS            Status;
  UINT8                 Index;
  CHAR8                 FileName[MAX_FILE_NAME_LENGTH];

  ZeroMem (FileName, sizeof (FileName));

  //
  // Get erase report from array and send to CSME
  //
  if ((ReportNum > 0) && (Report != NULL) && (mPsBootParameters->TriggerSource == BootRpe)) {
    for (Index = 0; Index < ReportNum; Index++) {
      AsciiSPrint (FileName, sizeof (FileName), "Erase%d.json", Index + 1);
      Status = PsStorageEraseReportToAmt (FileName,  (Report + Index)->DataSize, (UINT8 *)((Report + Index)->Data));
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "PS: Failed to update Storage Erase report to CSME, Status = %r\n", Status));
      }
    }
  }

}

/**
  Console Output (GOP, Simple Text) callback function to display
  Platform Sanitize Summary information.

  @param[in]  Event         The Event this notify function registered to.
  @param[in]  Context       Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
PsGraphicsOutputCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                       Status;
  VOID                             *Interface;
  EFI_GRAPHICS_OUTPUT_PROTOCOL     *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *SimpleTextOut;
  EFI_RESET_TYPE                   ResetType;

  DEBUG ((DEBUG_INFO, "PS: %a Entry.\n", __FUNCTION__));

  if (mPsBootParameters == NULL) {
    return;
  }

  Interface = NULL;
  ResetType = EfiResetCold;

  //
  // Make sure GraphicsOutput and SimpleTextOut are installed to display messages,
  // check for its availability.
  //
  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID **) &GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    GraphicsOutput = NULL;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiSimpleTextOutProtocolGuid,
                  (VOID **) &SimpleTextOut
                  );
  if (EFI_ERROR (Status)) {
    SimpleTextOut = NULL;
  }

  if ((GraphicsOutput == NULL) || (SimpleTextOut == NULL)) {
    return;
  }

  gBS->CloseEvent (Event);

  InitUiLib ();

  if (mPsBootParameters->PsRequestedList & PS_CSME_UNCONFIGURE) {
    ResetType = EfiResetWarm;
  }

  PrintSanitizeSummary ();

  //
  // Set the flag and update to NVM Variable, so that on next boot will perform pending erase actions.
  //
  if (mPsBootParameters->PsPendingList != 0) {
    mPsBootParameters->IsSanitizeSummaryDisplayed = TRUE;
    PsStoreBootParametersAtNvm ();
  }

  gST->ConOut->OutputString (gST->ConOut, L"\n\rSystem will restart in 5 seconds...\n\r");
  MicroSecondDelay (5000000);
  PsIssueReset (ResetType);
}

/**
  Clear EraseAction Bit in the BootParameter pending list and update the status.

  @param[in] EraseActionBit          EraseAction Bit to be cleared.
  @param[in] Status                  EraseAction exit status.

**/
VOID
PsBootParameterStatusUpdate (
  IN UINT32                EraseActionBit,
  IN EFI_STATUS            Status
  )
{
  DEBUG ((DEBUG_INFO, "PS: %a () entry, EraseActionBit: %08x, Status: %r\n", __FUNCTION__, EraseActionBit, Status));

  if (EraseActionBit & PS_ERASE_STORAGE_MEDIA) {
    //
    // For Storage Erase action, additionally clear verification, document report bits and also
    // clear Storage erase password.
    //
    mPsBootParameters->PsPendingList &= ~(PS_ERASE_STORAGE_MEDIA | PS_VERIFY_STORAGE_MEDIA | PS_GENERATE_SANITIZE_REPORT);
    ZeroMem (&mPsBootStorageParameter.PsStorageDevPassword, STORAGE_DEV_PSW_MAX_LEN);

    // Update the status
    mPsBootParameters->PsStatus.PsAttempted |= (PS_ERASE_STORAGE_MEDIA | PS_GENERATE_SANITIZE_REPORT);
    if (mPsBootParameters->PsRequestedList & PS_VERIFY_STORAGE_MEDIA) {
      mPsBootParameters->PsStatus.PsAttempted |= PS_VERIFY_STORAGE_MEDIA;
    }

    if (Status == EFI_SUCCESS) {
      mPsBootParameters->PsStatus.PsAttemptResult |= (PS_ERASE_STORAGE_MEDIA | PS_GENERATE_SANITIZE_REPORT);
      if (mPsBootParameters->PsRequestedList & PS_VERIFY_STORAGE_MEDIA) {
        mPsBootParameters->PsStatus.PsAttemptResult |= PS_VERIFY_STORAGE_MEDIA;
      }
      PsPetAlert (PsProgressInProgress, EraseActionBit, PsOperationSuccessful);
    } else if (Status == EFI_UNSUPPORTED) {
      PsPetAlert (PsProgressFailure, EraseActionBit, PsOperationNotSupported);
    } else {
      PsPetAlert (PsProgressFailure, EraseActionBit, PsOperationFailed);
    }
  } else {
    //
    // All other erase actions can be treated commonly by clearing mask bit and updating
    // corresponding status bits
    //
    mPsBootParameters->PsPendingList &= ~EraseActionBit;
    mPsBootParameters->PsStatus.PsAttempted |= EraseActionBit;
    if (Status == EFI_SUCCESS) {
      mPsBootParameters->PsStatus.PsAttemptResult |= EraseActionBit;
      PsPetAlert (PsProgressInProgress, EraseActionBit, PsOperationSuccessful);
    } else if (Status == EFI_UNSUPPORTED) {
      PsPetAlert (PsProgressFailure, EraseActionBit, PsOperationNotSupported);
    } else {
      PsPetAlert (PsProgressFailure, EraseActionBit, PsOperationFailed);
    }
  }

  //
  // Update the PsBootParameters at NVM Variable.
  // Do not update in case of Clear All NVM
  //
  if (!(EraseActionBit & PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION)) {
    PsStoreBootParametersAtNvm ();
  }
}

/**
  Check if PS_BOOT_PARAMETERS exists in NVRAM from previous Sanitize Action Reboot.
  If 'PS_NVM_VARIABLE_BOOT_PARAM' received successfully means it was set in the
  previous boot and its continuation of sanitize process. So get boot parameter details.

  @retval TRUE       Previous Sanitize Continuation
  @retval FALSE      NVM variable doesn't exists, Sanitize requested freshly. (OR) Null Boot Parameter
**/
BOOLEAN
IsPreviousBootSanitizeContinuation (
  VOID
  )
{
  EFI_STATUS               Status;
  UINT32                   PsBootParametersAttr;
  UINTN                    Size;

  DEBUG ((DEBUG_INFO, "PS: %a Entry\n", __FUNCTION__));

  if (mPsBootParameters == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: PsBootParameters - NULL\n"));
    return FALSE;
  }

  Size = sizeof (PS_BOOT_PARAMETERS);
  Status = gRT->GetVariable (
                  PS_NVM_VARIABLE_BOOT_PARAM,
                  &gPsNvmBootParametersGuid,
                  &PsBootParametersAttr,
                  &Size,
                  mPsBootParameters
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PS: PsNvmBootParameters NVM Variable doesn't exists, Status = %r\n", Status));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "PS: PsNvmBootParameters NVM Variable exists with below values:\n"));
  DEBUG ((DEBUG_INFO, "PS: TriggerSource    0x%x \n", mPsBootParameters->TriggerSource));
  DEBUG ((DEBUG_INFO, "PS: PsRequestedList  0x%x \n", mPsBootParameters->PsRequestedList));
  DEBUG ((DEBUG_INFO, "PS: PsPendingList    0x%x \n", mPsBootParameters->PsPendingList));
  DEBUG ((DEBUG_INFO, "PS: IsSanitizeSummaryDisplayed   0x%x \n", mPsBootParameters->IsSanitizeSummaryDisplayed));
  DEBUG ((DEBUG_INFO, "PS: PsAttempted      0x%x \n", mPsBootParameters->PsStatus.PsAttempted));
  DEBUG ((DEBUG_INFO, "PS: PsAttemptResult  0x%x \n", mPsBootParameters->PsStatus.PsAttemptResult));

  return TRUE;
}

/**
  Store PS BootParameters structure to NVM.

  @retval EFI_SUCCESS                The firmware has successfully stored the variable and its data as
                                     defined by the Attributes.
  @retval EFI_INVALID_PARAMETER      An invalid combination of attribute bits, name, and GUID was supplied, or the
                                     DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER      VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES       Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR           The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED        The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED        The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION     The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                     but the AuthInfo does NOT pass the validation check carried out by the firmware.
  @retval EFI_NOT_FOUND              The variable trying to be updated or deleted was not found.
**/
EFI_STATUS
PsStoreBootParametersAtNvm (
  VOID
  )
{
  EFI_STATUS                           Status;

  DEBUG ((DEBUG_INFO, "PS: %a entry\n", __FUNCTION__));

  //
  // Create PsBootParameters NVRAM structure to track erase actions.
  //
  Status = gRT->SetVariable (
                  PS_NVM_VARIABLE_BOOT_PARAM,
                  &gPsNvmBootParametersGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (PS_BOOT_PARAMETERS),
                  mPsBootParameters
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS: SetVariable of PsBootParameters Failed, Status = %r\n", Status));
  }
  return Status;
}

/**
  Clear Boot option and all related NVM variables. So that next boot will be Normal Boot.
**/
VOID
PsClearBootOption (
  VOID
  )
{
  EFI_STATUS          Status;

  //
  // Get the trigger source and clear accordingly
  //
  if (mPsBootParameters->TriggerSource == BootLpe) {
    //
    // Disable Setup option
    // No action needed in case if Clear All Nvm is attempted
    //
    if (!(mPsBootParameters->PsStatus.PsAttempted & PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION)) {
      Status = PsLpeSetDisabled ();
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "PS: Failed to disable LPE, Status = %r\n", Status));
      }
    }
  } else if (mPsBootParameters->TriggerSource == BootRpe) {
    Status = PsClearRpeBoot ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PS: Failed to disabled LPE, Status = %r\n", Status));
    }
  }
  //
  // Delete Boot Parameter NVM Variable
  //
  Status = gRT->SetVariable (
                  PS_NVM_VARIABLE_BOOT_PARAM,
                  &gPsNvmBootParametersGuid,
                  0,
                  0,
                  NULL
                  );

}

/**
  End of DXE CallBack event to Perform sanitize actions as per requested list from Parameter structure.

  @param[in] Event      The event that triggered this notification function
  @param[in] Context    Pointer to the notification functions context
**/
VOID
EFIAPI
PerformSanitizeActions (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{
  EFI_STATUS              Status;
  UINTN                   Index;
  UINT32                  PsPendingItems;
  UINT32                  ActionItem;
  PS_BOARD_OEM_PROTOCOL   *BoardOemSanitize;
  BOOLEAN                 PerformCsmeUnconfig;
  VOID                    *Registration;

  DEBUG ((DEBUG_INFO, "PS-%a: Entry\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "PS: Requested Sanitize Items = %x\n", mPsBootParameters->PsRequestedList));
  DEBUG ((DEBUG_INFO, "PS: Pending Sanitize Items = %x\n", mPsBootParameters->PsPendingList));

  PsPendingItems = mPsBootParameters->PsPendingList;
  BoardOemSanitize = NULL;
  PerformCsmeUnconfig = FALSE;

  //
  // Get Board, OEM Sanitize Protocol in case if Board or OEM specific functions to be executed
  //
  if (PsPendingItems & (PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION | PS_BIOS_RELOAD_GOLDEN_CONFIG | PS_OEM_CUSTOM_ACTION)) {
    Status = gBS->LocateProtocol (
                    &gPsBoardOemProtocolGuid,
                    NULL,
                    (VOID **) &BoardOemSanitize
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "PS: Failed to locate Board OEM sanitize protocol (%r).\n", Status));
      DEBUG ((DEBUG_INFO, "PS: Sanitization will continue and only skip Board, OEM related erase tasks.\n"));
    }
  }

  //
  // Perform the action item as per the priority order.
  // As storage erase action treated separately, its not considered here.
  //
  for (Index = 0; Index < ARRAY_SIZE (mOrderOfOperation); Index++) {
    ActionItem = mOrderOfOperation[Index] & PsPendingItems;
    switch (ActionItem) {
      //
      // Perform TPM Clear operation
      //
      case PS_TPM_CLEAR:
        DEBUG ((DEBUG_INFO, "PS: Perform TPM Clear.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseTpmClear, PsOperationStarted);

        Status = TriggerTpmClear ();

        PsPendingItems &= ~PS_TPM_CLEAR;
        PsBootParameterStatusUpdate (PS_TPM_CLEAR, Status);
        break;

      //
      // Hook for OEM to add OEM Specific Erase actions
      //
      case PS_OEM_CUSTOM_ACTION:
        DEBUG ((DEBUG_INFO, "PS: Invoke OEM Custom Erase Action.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseCustomAction, PsOperationStarted);
        Status = EFI_NOT_FOUND;
        if (BoardOemSanitize != NULL) {
          Status = BoardOemSanitize->PsOemCustomAction (BoardOemSanitize, &mPsOemHookTlvData);
          DEBUG ((DEBUG_INFO, "PS: OEM Custom Action Status = %r\n", Status));
        }

        PsPendingItems &= ~PS_OEM_CUSTOM_ACTION;
        PsBootParameterStatusUpdate (PS_OEM_CUSTOM_ACTION, Status);
        break;

      //
      // 1. Performing 'BIOS Reload Golden Config' deletes all BIOS Setup variables, So that on next boot,
      // BIOS loads with default Setup configuration.
      // 2. Performing 'Clear All BIOS NVM Variable Region' erase the entire NVM variable region, Which also includes
      // Authenticated variables, BIOS setup variables, OS based variables.
      // So 'Reload Golden Config' also considered as part of 'clearing all NVM variables'.
      // Hence if both erase actions are set, then BIOS will peform Clear All NVM and update both the status.
      //
      // Note: If OEM choose other than default settings for 'Reload Golden Config', then below two cases should
      //       be treated separately.
      //
      case PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION:
      case PS_BIOS_RELOAD_GOLDEN_CONFIG:
        //Skip to show Pop-Up message with summary details.
        if (!mPsBootParameters->IsSanitizeSummaryDisplayed) {
          DEBUG ((DEBUG_INFO, "PS: NVM variable - Will attempt in next boot after Pop-Up message.\n"));
          break;
        }

        if ((PsPendingItems & PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION) &&
            (PsPendingItems & PS_BIOS_RELOAD_GOLDEN_CONFIG)) {
          //Both Clear All BIOS NVM Variable Region and BIOS Reload to Golden Config are requested, perform Clear All BIOS NVM Variable Region
          DEBUG ((DEBUG_INFO, "PS: Clear All BIOS NVM Variable Region and Reload Golden Config.\n"));
          PsPetAlert (PsProgressInProgress, PsPetEraseClearNvm, PsOperationStarted);
          PsPetAlert (PsProgressInProgress, PsPetEraseReloadConfig, PsOperationStarted);
          Status = ClearAllBiosNvmVariableRegion ();

          PsPendingItems &= ~(PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION | PS_BIOS_RELOAD_GOLDEN_CONFIG);
          PsBootParameterStatusUpdate (PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION, Status);
          PsBootParameterStatusUpdate (PS_BIOS_RELOAD_GOLDEN_CONFIG, Status);

          //
          // Invoke Post Erase All Bios NVM Variable Region function. (In case if OEM implemented the protocol to set additional variables like
          // to support UEFI Secure Boot, invoking this interface applies those settings)
          //
          Status = EFI_NOT_FOUND;
          if (BoardOemSanitize != NULL) {
            Status = BoardOemSanitize->PsPostEraseAllNvmVars (BoardOemSanitize);
            DEBUG ((DEBUG_INFO, "PS: Post Erase All NVM Vars Status = %r\n", Status));
          }
        } else if (PsPendingItems & PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION) {
          //Only Clear All BIOS NVM Region is requested, perform the same.
          DEBUG ((DEBUG_INFO, "PS: Clear All BIOS NVM Variable Region.\n"));
          PsPetAlert (PsProgressInProgress, PsPetEraseClearNvm, PsOperationStarted);
          Status = ClearAllBiosNvmVariableRegion ();

          PsPendingItems &= ~PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION;
          PsBootParameterStatusUpdate (PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION, Status);

          //
          // Invoke Post Erase All Bios NVM Variable Region function, incase if any additional action to be performed
          // like setting UEFI Secure Boot variables.
          // Note: OEM can implement this function as needed.
          //
          Status = EFI_NOT_FOUND;
          if (BoardOemSanitize != NULL) {
            Status = BoardOemSanitize->PsPostEraseAllNvmVars (BoardOemSanitize);
            DEBUG ((DEBUG_INFO, "PS: Post Erase All NVM Vars Status = %r\n", Status));
          }
        } else if (PsPendingItems & PS_BIOS_RELOAD_GOLDEN_CONFIG) {
          // Only BIOS Reload to Golden Config is requested, perform the same.
          DEBUG ((DEBUG_INFO, "PS: BIOS Reload to Golden Config.\n"));
          PsPetAlert (PsProgressInProgress, PsPetEraseReloadConfig, PsOperationStarted);
          Status = EFI_NOT_FOUND;
          if (BoardOemSanitize != NULL) {
            Status = BoardOemSanitize->PsNvmGoldenConfig (BoardOemSanitize);
          }

          PsPendingItems &= ~PS_BIOS_RELOAD_GOLDEN_CONFIG;
          PsBootParameterStatusUpdate (PS_BIOS_RELOAD_GOLDEN_CONFIG, Status);
        }
        break;

      //
      // Perform CSME Unconfiguration.
      // BIOS will only send command to CSME and its CSME to perform unconfituration
      // After sending CSME cmd via HECI, BIOS will not get control and CSME will restart the system.
      // After CSME Unconfiguration, system also disconnects from Network access (AMT get unconfigured)
      // so this is one of special action and will attempt at the end of the order list.
      // BIOS will set a Flag and update the status in the switch case and send a command later in this function.
      //
      case PS_CSME_UNCONFIGURE:
        //Skip to show Pop-Up message with summary details.
        if (!mPsBootParameters->IsSanitizeSummaryDisplayed) {
          DEBUG ((DEBUG_INFO, "PS: CSME Unconfig - Will attempt in next boot after Pop-Up message.\n"));
          break;
        }

        DEBUG ((DEBUG_INFO, "PS: Notify CSME to perform CSME-Unconfiguration.\n"));
        PerformCsmeUnconfig = TRUE;

        Status = EFI_SUCCESS;        //Keeping the status to Success,but actual result will be updated by CSME
        PsPendingItems &= ~PS_CSME_UNCONFIGURE;
        PsBootParameterStatusUpdate (PS_CSME_UNCONFIGURE, Status);
        break;

      default:
        DEBUG ((DEBUG_INFO, "PS: No action\n"));
        break;
    } // End of Switch Case
  } // End of For loop.

  if (mPsBootParameters->PsRequestedList & (PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION | PS_BIOS_RELOAD_GOLDEN_CONFIG | PS_CSME_UNCONFIGURE)) {
    if ((mPsBootParameters->IsSanitizeSummaryDisplayed) && (PsPendingItems == 0)) {
      DEBUG ((DEBUG_INFO, "PS: Sanitize Completed.\n"));
      DEBUG ((DEBUG_INFO, "PS: Requested Sanitize Items = %x\n", mPsBootParameters->PsRequestedList));
      DEBUG ((DEBUG_INFO, "PS: Attempted Sanitize Items = %x\n", mPsBootParameters->PsStatus.PsAttempted));
      DEBUG ((DEBUG_INFO, "PS: Attempted Sanitize Result = %x\n", mPsBootParameters->PsStatus.PsAttemptResult));
      DEBUG ((DEBUG_INFO, "PS: Pending Sanitize Items = %x\n", mPsBootParameters->PsPendingList));

      if (PerformCsmeUnconfig) {
        DEBUG ((DEBUG_INFO, "Note: The Above Sanitize Result is not consider for CSME-Unconfiguration\n"));
      }

      //
      // Clear RPE / LPE Boot Flag, so that next boot should be Normal Boot
      //
      PsPetAlert (PsProgressInProgress, PsPetEraseGeneral, PsAllSettingsDecremented);
      if ((mPsBootParameters->PsRequestedList & PS_PERFORM_ACTION_MASK) == mPsBootParameters->PsStatus.PsAttemptResult) {
        PsReportBiosStatus (PsSuccess);
      } else {
        PsReportBiosStatus (PsGeneralFailure);
      }

      PsSummaryReportToPsr (mPsBootParameters);

      PsClearBootOption ();

      if (PerformCsmeUnconfig) {
        Status = PsCsmeUnconfigure ();
        DEBUG ((DEBUG_INFO, "PS: PsCsmeUnconfigure Status = %r\n", Status));
      }

      //
      // Issue Cold Reset
      //
      PsIssueReset (EfiResetCold);
    }

  } else if (PsPendingItems == 0) {
    //
    // Clear RPE / LPE Boot Flag, so that next boot should be Normal Boot
    // System restart, will takes place after showing Pop-Up message.
    //
    PsPetAlert (PsProgressInProgress, PsPetEraseGeneral, PsAllSettingsDecremented);
    if ((mPsBootParameters->PsRequestedList & PS_PERFORM_ACTION_MASK) == mPsBootParameters->PsStatus.PsAttemptResult) {
      PsReportBiosStatus (PsSuccess);
    } else {
      PsReportBiosStatus (PsGeneralFailure);
    }

    PsSummaryReportToPsr (mPsBootParameters);

    PsClearBootOption ();
  }

  //
  // Register a console Output (GOP, Simple Text) callback function to display Platform Sanitize Summary information.
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiGraphicsOutputProtocolGuid,
    TPL_CALLBACK,
    PsGraphicsOutputCallback,
    NULL,
    &Registration
    );

  return;
}
