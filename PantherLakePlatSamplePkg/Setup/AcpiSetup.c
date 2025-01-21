/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#include <Base.h>
#include <Library/AslUpdateLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <SetupPrivate.h>

/**
  This function initializes the Acpi related setup option values

  @param[in] HiiHandle  HiiHandle Handle to HII database
  @param[in] Class      Indicates the setup class

**/
VOID
InitAcpiStrings (
  IN EFI_HII_HANDLE      HiiHandle,
  IN UINT16              Class
  )
{
  EFI_STATUS                    Status;
  CHAR8                         *FadtVersionStr;
  UINTN                         Handle;
  EFI_ACPI_DESCRIPTION_HEADER   *Table;

  DEBUG ((DEBUG_INFO, "<InitAcpiStrings>"));
  FadtVersionStr = "1.0";

  if (Class == ADVANCED_FORM_SET_CLASS) {
    ///
    /// Locate table with matching ID
    ///
    Handle = 0;
    Table  = NULL;
    Status = LocateAcpiTableBySignature (
               EFI_ACPI_6_5_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE,
               (EFI_ACPI_DESCRIPTION_HEADER **) &Table,
               &Handle
               );
    if (EFI_ERROR (Status) || (Table == NULL)) {
      return;
    }

    switch (Table->Revision) {
      case EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE_REVISION:
        FadtVersionStr = "2.0";
        break;
      case EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_REVISION:
        FadtVersionStr = "3.0";
        break;
      case EFI_ACPI_5_0_FIXED_ACPI_DESCRIPTION_TABLE_REVISION:
        FadtVersionStr = "5.0";
        break;
      case EFI_ACPI_6_5_FIXED_ACPI_DESCRIPTION_TABLE_REVISION:  // Major Revision: 6
        FadtVersionStr = "6.5";
        break;
      default:
        break;
    }
  }

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_ACPI_VERSION_VALUE),
    L"%a",
    FadtVersionStr
  );

#ifdef ACPI_DEBUG_FLAG
  InitString (
    HiiHandle,
    STRING_TOKEN (STR_ACPI_DEBUG_ADDRESS_VALUE),
    L"0x%x",
    PcdGet32 (PcdAcpiDebugAddress)
  );
#endif //ACPI_DEBUG_FLAG
}

//
// Callback routine for LowPowerS0Idle Enable
//
EFI_STATUS
EFIAPI
LowPowerS0IdleEnableCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  SETUP_DATA              SetupData;
  UINTN                   VarSize;
  EFI_STATUS              Status;
  EFI_STRING              RequestString;

  RequestString = NULL;
  Status = EFI_SUCCESS;

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  ASSERT (KeyValue == KEY_LOW_POWER_S0_IDLE);
  DEBUG ((DEBUG_INFO, "LowPowerS0IdleEnableCallback()\n"));

  VarSize = sizeof (SETUP_DATA);
  if (!HiiGetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status) && (SetupData.LowPowerS0Idle == 1)) {
    SetupData.HidEventFilterDriverEnable = 1;
    RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SETUP_DATA, HidEventFilterDriverEnable), sizeof (SetupData.HidEventFilterDriverEnable));

    VarSize = sizeof (SETUP_DATA);
    if (!HiiSetBrowserData (&gSetupVariableGuid, L"Setup", VarSize, (UINT8 *) &SetupData, RequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);

    FreePool (RequestString);
  }

  return EFI_SUCCESS;
}
