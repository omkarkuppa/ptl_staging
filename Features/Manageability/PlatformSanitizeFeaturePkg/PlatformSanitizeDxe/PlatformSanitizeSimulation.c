/**@file
  Platform Sanitize Simulation Dxe driver.

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
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/SanitizeUiLib.h>
#include <PsParameter.h>
#include <PlatformSanitize.h>
#include <Guid/PlatformSanitizeVariable.h>
#include <Library/PlatformSanitizeServices.h>
#include <Library/PsCsmeAccessRpeLib.h>

extern GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_PARAMETERS              *mPsBootParameters;
extern GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_STORAGE_PARAMETERS      mPsBootStorageParameter;

#define REPORT_DATA_MAX_LENGTH       3000
#define REPORT_TEMP_DATA_MAX_LENGTH  800

//
// Setting erase actions to execute in an order.
//
STATIC CONST UINT32 mOrderOfOperationSimulate[] = {
  PS_ERASE_STORAGE_MEDIA,
  PS_TPM_CLEAR,
  PS_OEM_CUSTOM_ACTION,
  PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION,
  PS_BIOS_RELOAD_GOLDEN_CONFIG,
  PS_CSME_UNCONFIGURE
};

/**
  Clear EraseAction Bit in the BootParameter pending list and update the status.

  @param[in] EraseActionBit          EraseAction Bit to be cleared.
  @param[in] Status                  EraseAction exit status.

**/
VOID
PsBootParameterStatusUpdateForSimulate (
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
}

/**
  Add the header of the report

  @param[in,out] Data  Target data buffer
  @param[in] TempData  Temp buffer
  @param[in] Length    Size in byte of temp buffer

  @return EFI_SUCCESS Add report header successfully.
  @return Others      Fail.

**/
EFI_STATUS
AddReportHeader (
  IN OUT CHAR16  *Data,
  IN CHAR16      *TempData,
  IN UINTN       Length
  )
{
  UINTN   StrLength;
  UINT16  BomValue = 0xFEFF;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  if ((Data == NULL) || (TempData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  StrLength = 0;
  CopyMem (Data, &BomValue, sizeof (UINT16));

  SetMem (TempData, Length, 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                Length,
                "%a%a%a%a%a%a%a%a",
                "{\r\n\t\"Information\": {",
                "\r\n\t\t\"Purpose\": \"Media Sanitization metadata generation in support of NIST 800-88r1 documentation requirements as listed in Section 4.8\",",
                "\r\n\t\t\"Reference Documentation\": {",
                "\r\n\t\t\t\"NIST Specification\": \"NIST 800-88r1\",",
                "\r\n\t\t\t\"NIST Document Name\": \"Guidelines for Media Sanitization\",",
                "\r\n\t\t\t\"NIST Document URL\": \"https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-88r1.pdf\"\r\n\t\t},",
                "\r\n\t\t\"Disclaimer\": \"\",",
                "\r\n\t\t\"Support\": \"https://www.acme.com/content/www/us/en/support.html\"\r\n\t},"
                );

  DEBUG((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Add erase report of device

  @param[in,out] Data  Target data buffer
  @param[in] TempData  Temp buffer
  @param[in] Length    Size in byte of temp buffer

  @return EFI_SUCCESS  Add device information to report successfully.
  @return Others       Fail.

**/
EFI_STATUS
AppendSimulationReport (
  IN OUT CHAR16  *Data,
  IN CHAR16      *TempData,
  IN UINTN       Length
  )
{
  UINTN     StrLength;
  CHAR8     TimeStr[100];
  EFI_TIME  Time;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  if ((Data == NULL) || (TempData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  StrLength = 0;

  // Content
  SetMem (TempData, Length, 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                Length,
                "%a%a%a%a%a%a%a%a%a%a%a%a%a%a",
                "\r\n\t\"Content\": {",
                "\r\n\t\t\"Manufacturer\": \"\",",
                "\r\n\t\t\"Model\": \"",
                "\",\r\n\t\t\"Serial Number\": \"",
                "\",\r\n\t\t\"Media Type\": \"",
                "\",\r\n\t\t\"Media Source\": \"\",",
                "\r\n\t\t\"Pre-Sanitization Confidentiality Categorization\": \"\",",
                "\r\n\t\t\"Sanitization Description\": \"",
                "Simulation",
                "\",\r\n\t\t\"Method used\": \"",
                "Simulation",
                "\",\r\n\t\t\"Method Used Detail\": \"\",",
                "\r\n\t\t\"Tool Used\": \"\",",
                "\r\n\t\t\"Verification Method\": \"Quick Sample 10% Pseudorandom\","
                );
  DEBUG ((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  // Sanitization Person
  SetMem (TempData, Length, 0x0);
  gRT->GetTime (&Time, NULL);
  ZeroMem (TimeStr, sizeof (TimeStr));
  AsciiSPrint (TimeStr, sizeof (TimeStr), "\r\n\t\t\t\"Date\": \"%d-%d-%dT%d:%d:%d\",", Time.Year, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Second);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                Length,
                "%a%a%a%a%a%a",
                "\r\n\t\t\"Sanitization Person\": {",
                "\r\n\t\t\t\"Name\": \"\",",
                "\r\n\t\t\t\"Position/Title\": \"\",",
                TimeStr,
                "\r\n\t\t\t\"Location\": \"\",",
                "\r\n\t\t\t\"Contact information\": \"\"\r\n\t\t},"
                );
  DEBUG ((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  // Verification Person
  SetMem (TempData, Length, 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                Length,
                "%a%a%a%a%a%a",
                "\r\n\t\t\"Verification Person\": {",
                "\r\n\t\t\t\"Name\": \"\",",
                "\r\n\t\t\t\"Position/Title\": \"\",",
                TimeStr,
                "\r\n\t\t\t\"Location\": \"\",",
                "\r\n\t\t\t\"Contact information\": \"\"\r\n\t\t}\r\n\t}"
                );
  DEBUG((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Add the footer of the report

  @param[in,out] Data  Target data buffer
  @param[in] TempData  Temp buffer
  @param[in] Length    Size in byte of temp buffer

  @return EFI_SUCCESS Add report header successfully.
  @return Others      Fail.

**/
EFI_STATUS
AppendReportFooter (
  IN OUT CHAR16  *Data,
  IN CHAR16      *TempData,
  IN UINTN       Length
  )
{
  UINTN   StrLength;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  if ((Data == NULL) || (TempData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  StrLength = 0;

  SetMem (TempData, Length, 0x0);
  StrLength = UnicodeSPrintAsciiFormat (
                TempData,
                Length,
                "%a",
                "\r\n}"
                );

  DEBUG((DEBUG_INFO, "length = %d.\nTempData =\n %s\n", StrLength, TempData));
  StrCatS (Data, REPORT_DATA_MAX_LENGTH, TempData);

  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Create report data with JSON format

  @param[in,out] ReportDoc  Pointer to report document

  @return EFI_SUCCESS Create file and write data status.
  @return Others      Fail.

**/
EFI_STATUS
CreateReport (
  ERASE_DOCUMENTATION * ReportDoc
  )
{
  EFI_STATUS    Status;
  CHAR16        *ReportData;
  UINTN         StrLengthAll;
  CHAR16        *TempData;

  DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));

  ReportData   = NULL;
  StrLengthAll = 0;

  if (ReportDoc == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Create a new documentation
  //
  DEBUG ((DEBUG_INFO, "Create new report: \n"));
  ReportData  = AllocateZeroPool (REPORT_DATA_MAX_LENGTH * sizeof (CHAR16));
  if (ReportData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  TempData = AllocateZeroPool (sizeof (CHAR16) * REPORT_TEMP_DATA_MAX_LENGTH);
  if (TempData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Add header
  //
  Status = AddReportHeader (ReportData, TempData, sizeof (CHAR16) * REPORT_TEMP_DATA_MAX_LENGTH);
  if (EFI_ERROR (Status)) {
    FreePool (ReportData);
    FreePool (TempData);
    return Status;
  }

  //
  // Add report info to the report
  //
  Status = AppendSimulationReport (ReportData, TempData, sizeof (CHAR16) * REPORT_TEMP_DATA_MAX_LENGTH);
  if (EFI_ERROR (Status)) {
    FreePool (ReportData);
    FreePool (TempData);
    return Status;
  }

  //
  // Add footer
  //
  Status = AppendReportFooter (ReportData, TempData, sizeof (CHAR16) * REPORT_TEMP_DATA_MAX_LENGTH);
  if (EFI_ERROR (Status)) {
    FreePool (ReportData);
    FreePool (TempData);
    return Status;
  }

  StrLengthAll = StrLen (ReportData);
  ReportDoc->Data = ReportData;
  ReportDoc->DataSize = (UINT32) (StrLengthAll * sizeof (CHAR16));
  DEBUG ((DEBUG_INFO, "Report : size = %d bytes, data address = 0x%08x\n", ReportDoc->DataSize, ReportDoc->Data));

  FreePool (TempData);
  DEBUG ((DEBUG_INFO, "%a - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Simulate sanitize actions as per requested list from Parameter structure.
  This function actually do not go beyond this level to perform erase actions,
  instead this function prints the debug log information and send
  the PET Alerts for each erase action indicating that the sanitize is done successfully.
  Finally clears the boot option and issues system reset to make next boot as normal boot.

**/
VOID
PsSimulateSanitizeActions (
  VOID
  )
{
  EFI_STATUS    Status;
  UINTN         Index;
  UINT32        PsPendingItems;
  UINT32        ActionItem;

#if FixedPcdGetBool(PcdPsSimulationReportEnable) == 1
  ERASE_DOCUMENTATION *ReportList;
  ReportList = NULL;
#endif

  DEBUG ((DEBUG_INFO, "PS-%a: Entry\n", __FUNCTION__));

  if (mPsBootParameters == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: PsBootParameters - NULL\n"));
    return;
  }

  DEBUG ((DEBUG_INFO, "PS: Requested Sanitize Items = %x\n", mPsBootParameters->PsRequestedList));
  DEBUG ((DEBUG_INFO, "PS: Pending Sanitize Items = %x\n", mPsBootParameters->PsPendingList));

  PsPendingItems = mPsBootParameters->PsPendingList;

  //
  // Perform the simulation action item as per the priority order.
  //
  for (Index = 0; Index < ARRAY_SIZE (mOrderOfOperationSimulate); Index++) {
    ActionItem = mOrderOfOperationSimulate[Index] & PsPendingItems;
    switch (ActionItem) {
      case PS_ERASE_STORAGE_MEDIA:
        DEBUG ((DEBUG_INFO, "PS: Perform Storage Sanitize.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseStorage, PsOperationStarted);

        Status = EFI_SUCCESS;

        PsPendingItems &= ~PS_ERASE_STORAGE_MEDIA;
        PsBootParameterStatusUpdateForSimulate (PS_ERASE_STORAGE_MEDIA, Status);
#if FixedPcdGetBool(PcdPsSimulationReportEnable) == 1
        ReportList = (ERASE_DOCUMENTATION *)AllocateZeroPool (sizeof (ERASE_DOCUMENTATION));
        if (ReportList != NULL) {
          if (CreateReport (ReportList) == EFI_SUCCESS) {
            PsStorageEraseReport (1, ReportList);
            if (ReportList->Data != NULL) {
              FreePool (ReportList->Data);
              ReportList->Data = NULL;
            }
          }
          FreePool (ReportList);
        }
#endif
        break;

      case PS_TPM_CLEAR:
        DEBUG ((DEBUG_INFO, "PS: Perform TPM Clear.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseTpmClear, PsOperationStarted);

        Status = EFI_SUCCESS;

        PsPendingItems &= ~PS_TPM_CLEAR;
        PsBootParameterStatusUpdateForSimulate (PS_TPM_CLEAR, Status);
        break;

      case PS_OEM_CUSTOM_ACTION:
        DEBUG ((DEBUG_INFO, "PS: Invoke OEM Custom Erase Action.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseCustomAction, PsOperationStarted);

        Status = EFI_SUCCESS;

        PsPendingItems &= ~PS_OEM_CUSTOM_ACTION;
        PsBootParameterStatusUpdateForSimulate (PS_OEM_CUSTOM_ACTION, Status);
        break;

      case PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION:
        DEBUG ((DEBUG_INFO, "PS: Perform Clear NVM Variables.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseClearNvm, PsOperationStarted);

        Status = EFI_SUCCESS;

        PsPendingItems &= ~PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION;
        PsBootParameterStatusUpdateForSimulate (PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION, Status);
        break;

      case PS_BIOS_RELOAD_GOLDEN_CONFIG:
        DEBUG ((DEBUG_INFO, "PS: Perform Reload BIOS Golden Config Action.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseReloadConfig, PsOperationStarted);

        Status = EFI_SUCCESS;

        PsPendingItems &= ~PS_BIOS_RELOAD_GOLDEN_CONFIG;
        PsBootParameterStatusUpdateForSimulate (PS_BIOS_RELOAD_GOLDEN_CONFIG, Status);
        break;

      case PS_CSME_UNCONFIGURE:
        DEBUG ((DEBUG_INFO, "PS: Perform CSME Unconfigure.\n"));
        PsPetAlert (PsProgressInProgress, PsPetEraseCsmeUnconfig , PsOperationStarted);

        Status = EFI_SUCCESS;

        PsPendingItems &= ~PS_CSME_UNCONFIGURE;
        PsBootParameterStatusUpdateForSimulate (PS_CSME_UNCONFIGURE, Status);
        break;

      default:
        DEBUG ((DEBUG_INFO, "PS: No action\n"));
        break;
    } // End of Switch Case
  } // End of For loop.

  DEBUG ((DEBUG_INFO, "PS: Sanitize Completed.\n"));
  DEBUG ((DEBUG_INFO, "PS: Requested Sanitize Items = %x\n", mPsBootParameters->PsRequestedList));
  DEBUG ((DEBUG_INFO, "PS: Attempted Sanitize Items = %x\n", mPsBootParameters->PsStatus.PsAttempted));
  DEBUG ((DEBUG_INFO, "PS: Attempted Sanitize Result = %x\n", mPsBootParameters->PsStatus.PsAttemptResult));
  DEBUG ((DEBUG_INFO, "PS: Pending Sanitize Items = %x\n", mPsBootParameters->PsPendingList));

  //
  // Clear RPE / LPE Boot Flag, so that next boot should be Normal Boot
  //
  PsPetAlert (PsProgressInProgress, PsPetEraseGeneral, PsAllSettingsDecremented);
  if (((mPsBootParameters->PsRequestedList) & PS_PERFORM_ACTION_MASK) == mPsBootParameters->PsStatus.PsAttemptResult) {
    PsReportBiosStatus (PsSuccess);
  } else {
    PsReportBiosStatus (PsGeneralFailure);
  }

  //
  // Clear the boot option accordingly
  //
  if (mPsBootParameters->TriggerSource == BootLpe) {
    //
    // Disable Setup option
    //
    Status = PsLpeSetDisabled ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PS: Failed to disabled LPE, Status = %r\n", Status));
    }
  } else if (mPsBootParameters->TriggerSource == BootRpe) {
    Status = PsClearRpeBoot ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PS: Failed to disabled LPE, Status = %r\n", Status));
    }
  }

  //
  // Issue Cold Reset
  //
  PsIssueReset (EfiResetCold);
}

/**
  Check if Platform Sanitize Simulation is enabled in Setup.

  @retval TRUE     Simulation is Enabled
  @retval FALSE    Simulation is NOT Enabled
**/
BOOLEAN
PsIsSimulationEnabled (
  VOID
  )
{
  EFI_STATUS               Status;
  PLATFORM_ERASE_VARIABLE  Variable;
  UINTN                    VariableSize;

  VariableSize = sizeof (PLATFORM_ERASE_VARIABLE);
  Status = gRT->GetVariable (
                  PLATFORM_ERASE_VARIABLE_NAME,
                  &gPsPlatformEraseVariableGuid,
                  NULL,
                  &VariableSize,
                  &Variable
                  );
  if (!EFI_ERROR (Status) && Variable.PlatformEraseSimulation) {
    DEBUG ((DEBUG_INFO, "PS: PsIsSimulationEnabled TRUE\n"));
    return TRUE;
  }
  DEBUG ((DEBUG_INFO, "PS: PsIsSimulationEnabled FALSE\n"));

  return FALSE;
}

/**
  Function to trigger Platform Sanitize Simulation.
**/
VOID
PsSimulation (
  VOID
  )
{
  if (!PsIsSimulationEnabled ()) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PS: Entering into Simulation Mode.\n"));
  PsSimulateSanitizeActions ();
}
