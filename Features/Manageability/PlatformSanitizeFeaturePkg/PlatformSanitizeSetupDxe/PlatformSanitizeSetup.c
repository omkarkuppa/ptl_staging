/**@file
  Platform Sanitize Setup Driver

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

#include "PlatformSanitizeSetup.h"
#include <Guid/PlatformSanitizeVariable.h>

//
// Package List GUID containing all form and string packages
//
EFI_GUID mPlatformSanitizeFormsetGuid = PLATFORM_SANITIZE_FORMSET_GUID;

PLATFORM_ERASE_SETUP_DRIVER mPlatformEraseSetupDriver;
BOOLEAN  mRpeSupport;

HII_VENDOR_DEVICE_PATH mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    PLATFORM_SANITIZE_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

//
// Initialization for internal data structure
//
HII_CONFIG mStorageEraseHiiConfigTemplate = {
  {
    SANITIZE_BLOCK_ERASE,
    SANITIZE_CRYPTO_ERASE,
    SANITIZE_OVR_ERASE,
    SANITIZE_CLEAR
  },
  {0}
};

/**
  Check if Device is selected.

  @param[in] SnSize  Size of serial string number
  @param[in] Sn      Serial string number

  @retval 1          The device is selected.
  @retval 0          The device is not selected.
**/
UINT8
IsDeviceSelected (
  UINTN    SnSize,
  VOID     *Sn
  )
{
  STORAGE_ERASE_DEVICE_VARIABLE  *TempVariable;
  STORAGE_ERASE_DEVICE_VARIABLE  *Variable;
  UINTN                          VariableSize;
  VOID                           *SnInVariable;
  UINT8                          CheckFlag;
  EFI_STATUS                     Status;

  Variable = NULL;
  VariableSize = 0;
  CheckFlag = 1;

  if ((SnSize == 0) || (Sn == NULL)) {
    return CheckFlag;
  }

  Status = GetVariable2 (
             DEVICE_VARIABLE_NAME,
             &gPsStorageEraseVariableGuid,
             (VOID **)&Variable,
             &VariableSize
             );
  if (EFI_ERROR (Status)) {
    return CheckFlag;
  }

  if ((VariableSize > 0) && (Variable != NULL)) {
    DEBUG ((DEBUG_INFO, "VariableSize(%d): name %d, sn %d .\n", VariableSize, Variable->NameLength, Variable->SnLength));

    TempVariable = Variable;
    while ((TempVariable != NULL) &&
          (VariableSize >= sizeof (STORAGE_ERASE_DEVICE_VARIABLE))) {
      //
      // Compare if it is the same SN
      //
      SnInVariable = TempVariable->Sn;
      DEBUG ((DEBUG_INFO, "SnInVariable (%a), Sn (%a).\n", SnInVariable, Sn));
      if ( (SnSize <= SN_MAX_LEN) && (SnSize == TempVariable->SnLength) &&
          (CompareMem (SnInVariable, Sn, SnSize) == 0)) {
        CheckFlag = 0;
        break;
      }
      VariableSize -= sizeof (STORAGE_ERASE_DEVICE_VARIABLE);
      TempVariable ++;
    }
    FreePool (Variable);
  }
  return CheckFlag;
}

/**
  Updates the main menu form.

  @param[in] Private  Pointer to the internal data structure

  @retval EFI_SUCCESS  Update the main form success.
**/
EFI_STATUS
HiiPopulateMainMenuForm (
  IN PLATFORM_ERASE_SETUP_DRIVER  *Private
  )
{
  VOID                *StartOpCodeHandle;
  VOID                *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL  *StartLabel;
  EFI_IFR_GUID_LABEL  *EndLabel;
  EFI_HII_HANDLE      HiiHandle;
  EFI_STRING_ID       Token;
  UINTN               Index;
  EFI_STATUS          Status;
  UINT8               CheckFlag;
  BOOLEAN             FirstCheck;
  UINT32              TotalSize;
  ERASE_DEVICE_INFO   *DeviceInfo;
  STORAGE_ERASE_NOTIFY_PROTOCOL     *StorageEraseNotify;


  StartOpCodeHandle = NULL;
  EndOpCodeHandle   = NULL;
  HiiHandle = Private->HiiHandle;

  DEBUG ((DEBUG_INFO, "%a - entry\n", __FUNCTION__));

  //
  // Allocate space for creation of UpdateData Buffer
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (StartOpCodeHandle != NULL);
  if (StartOpCodeHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  ASSERT (EndOpCodeHandle != NULL);
  if (EndOpCodeHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Create Hii Extend Label OpCode as the start opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  //
  // Create Hii Extend Label OpCode as the end opcode
  //
  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  StartLabel->Number = LABEL_START;
  EndLabel->Number   = LABEL_END;
  Index = 0;

  if (Private->DeviceList != NULL) {
    FirstCheck = FALSE;
    Status = EFI_SUCCESS;
  } else {
    //
    // Install platform erase notify protocol
    //
    Status = gBS->LocateProtocol (
                    &gStorageSanitizeNotifyProtocolGuid,
                    NULL,
                    (VOID **) &StorageEraseNotify
                    );
    if (!EFI_ERROR (Status)) {
      FirstCheck = TRUE;
      Status = StorageEraseNotify->GetDevice ((VOID**) &Private->DeviceList);
    } else {
      DEBUG ((DEBUG_INFO, "locate storage erase notify protocol failed(%r).\n", Status));
    }
  }
  //
  // Traverse the device to create SETUP option
  //
  if (!EFI_ERROR (Status) && (Private->DeviceList != NULL)) {
    TotalSize = Private->DeviceList->TotalSize;
    if (TotalSize > 0) {
      TotalSize -= sizeof (ERASE_DEVICE_LIST);
      DeviceInfo = (ERASE_DEVICE_INFO *)((UINTN)Private->DeviceList + sizeof (ERASE_DEVICE_LIST));
    } else {
      DeviceInfo = NULL;
    }

    Index = 0;
    while ((TotalSize > 0) && (DeviceInfo != NULL)) {
      DEBUG ((DEBUG_INFO, "Name size(%d), Sn size(%d).\n", DeviceInfo->NameSize, DeviceInfo->SnSize));
      if (TotalSize < sizeof (ERASE_DEVICE_INFO)) {
        break;
      }
      DEBUG ((DEBUG_INFO, "Name(%S), Sn(%a).\n", DeviceInfo->Name, DeviceInfo->Sn));
      Token = HiiSetString (HiiHandle, 0, DeviceInfo->Name, NULL);
      if (FirstCheck) {
        //
        // Get the unselect device from variable
        //
        CheckFlag = IsDeviceSelected (DeviceInfo->SnSize, DeviceInfo->Sn);
        Private->HiiCfgData.DiskList[Index] = CheckFlag;
      } else {
        CheckFlag = Private->HiiCfgData.DiskList[Index];
      }
      DEBUG ((DEBUG_INFO, "CheckFlag(%d).\n", CheckFlag));

      HiiCreateCheckBoxOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID) (DISK_LIST_QUESTION_ID + Index),
        STORAGE_ERASE_VARSTORE_ID,
        (UINT16) (DISK_LIST_VAR_OFFSET + Index),
        Token,
        0,
        EFI_IFR_FLAG_CALLBACK,
        CheckFlag,
        NULL
        );
      TotalSize -= sizeof (ERASE_DEVICE_INFO);
      DeviceInfo ++;
      Index ++;
    }
  }

  HiiUpdateForm (
    HiiHandle,
    &mPlatformSanitizeFormsetGuid,
    FORMID_STORAGE_ERASE_MENU,
    StartOpCodeHandle,
    EndOpCodeHandle
    );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  DEBUG ((DEBUG_INFO, "%a - exit\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Save Hii configuration to STORAGE_ERASE_DEVICE_VARIABLE

  @param[in] Private  Pointer to the internal data structure
**/
VOID
SaveDeviceVariable (
  IN PLATFORM_ERASE_SETUP_DRIVER  *Private
  )
{
  EFI_STATUS                     Status;
  UINT32                         Index;
  STORAGE_ERASE_DEVICE_VARIABLE  *Variable;
  UINTN                          VariableSize;
  STORAGE_ERASE_DEVICE_VARIABLE  *NewVariable;
  UINTN                          NewVariableSize;
  STORAGE_ERASE_DEVICE_VARIABLE  *TempVariable;
  UINT32                         TotalSize;
  ERASE_DEVICE_INFO              *DeviceInfo;

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  Variable = NULL;
  VariableSize = 0;
  Index = 0;

  if (Private->DeviceList == NULL) {
    return;
  }
  //
  // store the required information of unselect device to the variable,
  // Which will be consumed in next boot.
  //
  TotalSize = Private->DeviceList->TotalSize;
  if (TotalSize > 0) {
    TotalSize -= sizeof (ERASE_DEVICE_LIST);
    DeviceInfo = (ERASE_DEVICE_INFO *)((UINTN)Private->DeviceList + sizeof (ERASE_DEVICE_LIST));
  } else {
    DeviceInfo = NULL;
  }

  while ((TotalSize > 0) && (DeviceInfo != NULL)) {
    if (TotalSize < sizeof (ERASE_DEVICE_INFO)) {
      break;
    }
    //
    // We only need to store the device which is not selected
    //
    if (Private->HiiCfgData.DiskList[Index] == 0) {
      DEBUG ((DEBUG_INFO, "Disk[%d] - is unchecked.\n", Index));
      NewVariableSize = sizeof (STORAGE_ERASE_DEVICE_VARIABLE);
      NewVariable = AllocateZeroPool (NewVariableSize + VariableSize);
      if (NewVariable == NULL) {
        break;
      }
      if ((VariableSize != 0) && (Variable != NULL)) {
        CopyMem (NewVariable, Variable, VariableSize);
      }
      // ------------
      //  Name Length
      //  SN Length
      //  Name
      //  SN
      // ------------
      DEBUG ((DEBUG_INFO, "NewVariableSize[%d].\n", NewVariableSize));
      TempVariable = (STORAGE_ERASE_DEVICE_VARIABLE *)((UINTN)NewVariable + VariableSize);
      TempVariable->NameLength = DeviceInfo->NameSize;
      TempVariable->SnLength = DeviceInfo->SnSize;
      StrCpyS (TempVariable->Name, MN_MAX_LEN, DeviceInfo->Name);
      CopyMem (TempVariable->Sn, DeviceInfo->Sn , SN_MAX_LEN);

      if (Variable != NULL) {
        FreePool (Variable);
      }
      Variable = NewVariable;
      VariableSize += NewVariableSize;
    }
    Index ++;
    TotalSize -= (sizeof (ERASE_DEVICE_INFO));
    DeviceInfo ++;
  }

  Status = gRT->SetVariable (
                  DEVICE_VARIABLE_NAME,
                  (EFI_GUID *) &gPsStorageEraseVariableGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                  VariableSize,
                  Variable
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%S - variable set failed (%r)\n", DEVICE_VARIABLE_NAME, Status));
  }

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));
}

/**
  Construct Request String (L"&OFFSET=%x&WIDTH=%x") base on the input Offset and Width.
  If the input RequestString is not NULL, new request will be cat at the end of it. The full
  request string will be constructed and return. Caller is responsible to free it.

  @param[in] RequestString   Current request string.
  @param[in] Offset          Offset of data in Storage.
  @param[in] Width           Width of data.

  @return String             Request string with input Offset and Width.
**/
EFI_STRING
HiiConstructRequestString (
  IN EFI_STRING      RequestString, OPTIONAL
  IN UINTN           Offset,
  IN UINTN           Width
  )
{
  CHAR16             RequestElement[30];
  UINTN              StringLength;
  EFI_STRING         NewString;

  StringLength = UnicodeSPrint (
                   RequestElement,
                   sizeof (RequestElement),
                   L"&OFFSET=%x&WIDTH=%x",
                   Offset,
                   Width
                   );
  if (RequestString != NULL) {
    StringLength = StringLength + StrLen (RequestString);
  }
  NewString = AllocateZeroPool ((StringLength + 1) * sizeof (CHAR16));
  if (NewString == NULL) {
    return NULL;
  }
  if (RequestString != NULL) {
    StrCatS (NewString, ((StringLength + 1) * sizeof (CHAR16)) / sizeof (CHAR16), RequestString);
    FreePool (RequestString);
  }
  StrCatS (NewString, ((StringLength + 1) * sizeof (CHAR16)) / sizeof (CHAR16), RequestElement);
  return NewString;
}

/**
  This function updates LpeEraseAll based on other erase values enabled/disabled.
**/
VOID
LpeSetupOptionUpdate (
  VOID
  )
{
  LPE_SETUP                       LpeSetup;
  UINTN                           VariableSize;
  EFI_STRING                      RequestString;

  RequestString = NULL;

  DEBUG ((DEBUG_INFO, "LPE: %a enter.\n", __FUNCTION__));

  VariableSize = sizeof (LPE_SETUP);
  if (!HiiGetBrowserData (&gPsLpeSetupVariableGuid, L"LpeSetup", VariableSize, (UINT8 *) &LpeSetup)) {
    DEBUG ((DEBUG_INFO, "LPE: %a cannot get browser data.\n", __FUNCTION__));
    return;
  }

  if (LpeSetup.LpeEraseSSD == TRUE &&
      LpeSetup.LpeClearTPM == TRUE &&
      LpeSetup.LpeOemCustomAction == TRUE &&
      LpeSetup.LpeClearNvm == TRUE &&
      LpeSetup.LpeBiosReloadGoldenConfig == TRUE) {
    LpeSetup.LpeEraseAll = TRUE;
  } else {
    LpeSetup.LpeEraseAll = FALSE;
  }

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeEraseAll), sizeof (LpeSetup.LpeEraseAll));
  if (RequestString != NULL) {
    if (!HiiSetBrowserData (&gPsLpeSetupVariableGuid, L"LpeSetup", VariableSize, (UINT8 *) &LpeSetup, RequestString)) {
      DEBUG ((DEBUG_INFO, "LPE: %a cannot set browser data.\n", __FUNCTION__));
    }
    FreePool (RequestString);
  }

  return;
}

/**
  This function sets the default Erase values in the LPE Setup Menu enabled/disabled.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original exporting driver
                              so that it can identify the type of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original exporting driver.
  @param[Out] ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS         The function completed successfully
  @retval EFI_UNSUPPORTED     The Setup Menu was not updated
  @retval EFI_NOT_FOUND       There is an error in Locating protocols or retrieving browser data
**/
EFI_STATUS
LpeSetupDefaultsCallBack (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       KeyValue,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  LPE_SETUP                       LpeSetup;
  UINTN                           VariableSize;
  EFI_STRING                      RequestString;
  UINT8                           EraseAllProfile;

  RequestString = NULL;

  DEBUG ((DEBUG_INFO, "LPE: %a enter (value = %d).\n", __FUNCTION__, Value->u8));

  VariableSize = sizeof (LPE_SETUP);
  if (!HiiGetBrowserData (&gPsLpeSetupVariableGuid, L"LpeSetup", VariableSize, (UINT8 *) &LpeSetup)) {
    DEBUG ((DEBUG_INFO, "LPE: %a cannot get browser data.\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  EraseAllProfile = Value->u8;
  if (EraseAllProfile == 1) {
    LpeSetup.LpeEraseAll = TRUE;
    LpeSetup.LpeEraseSSD = TRUE;
    LpeSetup.LpeClearTPM = TRUE;
    LpeSetup.LpeOemCustomAction = TRUE;
    LpeSetup.LpeClearNvm = TRUE;
    LpeSetup.LpeBiosReloadGoldenConfig = TRUE;
  } else {
    LpeSetup.LpeEraseAll = FALSE;
    LpeSetup.LpeEraseSSD = FALSE;
    LpeSetup.LpeStorageEraseVerify = FALSE;
    LpeSetup.LpeClearTPM = FALSE;
    LpeSetup.LpeOemCustomAction = FALSE;
    LpeSetup.LpeClearNvm = FALSE;
    LpeSetup.LpeBiosReloadGoldenConfig = FALSE;
  }

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeEraseAll), sizeof (LpeSetup.LpeEraseAll));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeEraseSSD), sizeof (LpeSetup.LpeEraseSSD));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeClearTPM), sizeof (LpeSetup.LpeClearTPM));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeOemCustomAction), sizeof (LpeSetup.LpeOemCustomAction));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeClearNvm), sizeof (LpeSetup.LpeClearNvm));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeBiosReloadGoldenConfig), sizeof (LpeSetup.LpeBiosReloadGoldenConfig));

  if (RequestString != NULL) {
    if (!HiiSetBrowserData (&gPsLpeSetupVariableGuid, L"LpeSetup", VariableSize, (UINT8 *) &LpeSetup, RequestString)) {
      DEBUG((DEBUG_INFO, "LPE: %a cannot set browser data.\n", __FUNCTION__));
      FreePool (RequestString);
      return EFI_NOT_FOUND;
    }
    FreePool (RequestString);
  }

  return EFI_SUCCESS;
}

/**
  This function sets the default Erase values in the LPE Setup Menu when Erase all enabled/disabled.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original exporting driver
                              so that it can identify the type of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original exporting driver.
  @param[Out] ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS         The function completed successfully
  @retval EFI_UNSUPPORTED     The Setup Menu was not updated
  @retval EFI_NOT_FOUND       There is an error in Locating protocols or retrieving browser data
**/
EFI_STATUS
EraseAllCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       KeyValue,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  LPE_SETUP                       LpeSetup;
  UINTN                           VariableSize;
  EFI_STRING                      RequestString;
  UINT8                           EraseProfile;

  RequestString = NULL;

  DEBUG ((DEBUG_INFO, "LPE: %a enter (value = %d).\n", __FUNCTION__, Value->u8));

  VariableSize = sizeof (LPE_SETUP);
  if (!HiiGetBrowserData (&gPsLpeSetupVariableGuid, L"LpeSetup", VariableSize, (UINT8 *) &LpeSetup)) {
    DEBUG ((DEBUG_INFO, "LPE: %a cannot get browser data.\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  EraseProfile = Value->u8;
  if (EraseProfile == 1) {
    LpeSetup.LpeEraseSSD = TRUE;
    LpeSetup.LpeClearTPM = TRUE;
    LpeSetup.LpeOemCustomAction = TRUE;
    LpeSetup.LpeClearNvm = TRUE;
    LpeSetup.LpeBiosReloadGoldenConfig = TRUE;
  } else {
    LpeSetup.LpeEraseSSD = FALSE;
    LpeSetup.LpeClearTPM = FALSE;
    LpeSetup.LpeOemCustomAction = FALSE;
    LpeSetup.LpeClearNvm = FALSE;
    LpeSetup.LpeBiosReloadGoldenConfig = FALSE;
  }

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeEraseSSD), sizeof (LpeSetup.LpeEraseSSD));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeClearTPM), sizeof (LpeSetup.LpeClearTPM));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeOemCustomAction), sizeof (LpeSetup.LpeOemCustomAction));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeClearNvm), sizeof (LpeSetup.LpeClearNvm));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (LPE_SETUP, LpeBiosReloadGoldenConfig), sizeof (LpeSetup.LpeBiosReloadGoldenConfig));

  if (RequestString != NULL) {
    if (!HiiSetBrowserData (&gPsLpeSetupVariableGuid, L"LpeSetup", VariableSize, (UINT8 *) &LpeSetup, RequestString)) {
      DEBUG ((DEBUG_INFO, "LPE: %a cannot set browser data.\n", __FUNCTION__));
      FreePool (RequestString);
      return EFI_NOT_FOUND;
    }
    FreePool (RequestString);
  }

  return EFI_SUCCESS;
}

/**
  This function displays a Pop Up message when SSD Erase is enabled from Setup menu.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original exporting driver
                              so that it can identify the type of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS         The function completed successfully
**/
EFI_STATUS
EraseSSDCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_INPUT_KEY                   Key;

  CreatePopUp (
    EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
    &Key,
    L"Selecting this Option to Sanitize Storage Media.",
    L"Press Y to Save.",
    L"Press any other key to cancel and exit.",
    NULL
    );

  if (Key.UnicodeChar == L'Y' || Key.UnicodeChar == L'y') {
    Value->b = TRUE;
  } else {
    Value->b = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This function displays a Pop Up message when TPM Clear is enabled from Setup menu.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original exporting driver
                              so that it can identify the type of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS         The function completed successfully
  @retval EFI_UNSUPPORTED     The Setup Menu was not updated
  @retval EFI_NOT_FOUND       There is an error in Locating protocols or retrieving browser data
**/
EFI_STATUS
TpmClearCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       KeyValue,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  EFI_INPUT_KEY  Key;

  CreatePopUp (
    EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
    &Key,
    L"Selecting this Option Clears TPM.",
    L"Press Y to Save.",
    L"Press any other key to cancel and exit.",
    NULL
    );

  if (Key.UnicodeChar == L'Y' || Key.UnicodeChar == L'y') {
    Value->b = TRUE;
  } else {
    Value->b = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This function displays a Pop Up message when NVM Clear is enabled from Setup menu.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original exporting driver
                              so that it can identify the type of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS         The function completed successfully
  @retval EFI_UNSUPPORTED     The Setup Menu was not updated
  @retval EFI_NOT_FOUND       There is an error in Locating protocols or retrieving browser data
**/
EFI_STATUS
ClearNvmCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       KeyValue,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  EFI_INPUT_KEY  Key;

  CreatePopUp (
    EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
    &Key,
    L"Selecting this Option Clears All NVM variables.",
    L"Press Y to Save.",
    L"Press any other key to cancel and exit.",
    NULL
    );

  if (Key.UnicodeChar == L'Y' || Key.UnicodeChar == L'y') {
    Value->b = TRUE;
  } else {
    Value->b = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This function displays a Pop Up message when Reload BIOS Golden Config is enabled from Setup menu.

  @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in] Action           Specifies the type of action taken by the browser.
  @param[in] QuestionId       A unique value which is sent to the original exporting driver
                              so that it can identify the type of data to expect.
  @param[in] Type             The type of value for the question.
  @param[in] Value            A pointer to the data being sent to the original exporting driver.
  @param[out] ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS         The function completed successfully
  @retval EFI_UNSUPPORTED     The Setup Menu was not updated
  @retval EFI_NOT_FOUND       There is an error in Locating protocols or retrieving browser data
**/
EFI_STATUS
BiosReloadConfigCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       KeyValue,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  EFI_INPUT_KEY  Key;

  CreatePopUp (
    EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
    &Key,
    L"Selecting this Option Reloads bios default Config.",
    L"Press Y to Save.",
    L"Press any other key to cancel and exit.",
    NULL
    );

  if (Key.UnicodeChar == L'Y' || Key.UnicodeChar == L'y') {
    Value->b = TRUE;
  } else {
    Value->b = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This function displays a Pop Up message  when user tries to modify
  RPE Enable/Disable option when remote session is opened.

  @retval EFI_SUCCESS - The function completed successfully
  @retval EFI_ABORTED - Must not modify the change.
**/

EFI_STATUS
RpeWarningMsg (
  VOID
  )
{
  EFI_INPUT_KEY  Key;

  if (!AmtIsRemoteSessionEnabled ()) {
    return EFI_SUCCESS;
  }

  do {
    CreatePopUp (
      EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
      &Key,
      L"Warning: RPE option cannot be modified while remote session is opened.",
      L"Press ENTER to Exit.",
      NULL
      );
  } while (Key.UnicodeChar != CHAR_CARRIAGE_RETURN);

  return EFI_ABORTED;
}

/**
  This function processes the results of changes in configuration.

  @param[in]  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action                 Specifies the type of action taken by the browser.
  @param[in]  QuestionId             A unique value which is sent to the original
                                     exporting driver so that it can identify the type
                                     of data to expect.
  @param[in]  Type                   The type of value for the question.
  @param[in]  Value                  A pointer to the data being sent to the original
                                     exporting driver.
  @param[out] ActionRequest          On return, points to the action requested by the
                                     callback function.

  @retval EFI_SUCCESS                The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES       Not enough storage is available to hold the
                                     variable and its data.
  @retval EFI_DEVICE_ERROR           The variable could not be saved.
  @retval EFI_UNSUPPORTED            The specified Action is not supported by the callback.

**/
EFI_STATUS
EFIAPI
DriverCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       QuestionId,
  IN UINT8                                 Type,
  IN EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  )
{
  PLATFORM_ERASE_SETUP_DRIVER  *Private;

  if (ActionRequest != NULL) {
    *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;
  } else {
    return EFI_INVALID_PARAMETER;
  }

  Private = PLATFORM_ERASE_SETUP_DRIVER_FROM_THIS (This);

  if (Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    return EFI_UNSUPPORTED;
  }

  if (Action == EFI_BROWSER_ACTION_FORM_OPEN) {
    if (QuestionId == FORMID_STORAGE_ERASE_MENU) {
      DEBUG ((DEBUG_INFO, "Platform storage erase form opened\n"));
      return HiiPopulateMainMenuForm (Private);
    }
  } else if (Action == EFI_BROWSER_ACTION_CHANGING) {
    if (QuestionId == KEY_LPE_ERASE_ALL) {
      return EraseAllCallBackFunction (This, Action, QuestionId, Type, Value, ActionRequest);
    } else if (QuestionId == KEY_LPE_ERASE_SSD) {
      return EraseSSDCallBackFunction (This, Action, QuestionId, Type, Value, ActionRequest);
    } else if (QuestionId == KEY_LPE_TPM_CLEAR) {
      return TpmClearCallBackFunction (This, Action, QuestionId, Type, Value, ActionRequest);
    } else if (QuestionId == KEY_LPE_CLEAR_NVM) {
      return ClearNvmCallBackFunction (This, Action, QuestionId, Type, Value, ActionRequest);
    } else if (QuestionId == KEY_LPE_RELOAD_BIOS) {
      return BiosReloadConfigCallBackFunction (This, Action, QuestionId, Type, Value, ActionRequest);
    } else if (QuestionId == KEY_LPE_PLATFORM_ENABLE) {
      return LpeSetupDefaultsCallBack (This, Action, QuestionId, Type, Value, ActionRequest);
    } else if (QuestionId == KEY_RPE_ENABLE) {
      return RpeWarningMsg ();
    }
  } else if (Action == EFI_BROWSER_ACTION_CHANGED) {
    if ((QuestionId >= DISK_LIST_QUESTION_ID) && (QuestionId < DISK_LIST_QUESTION_ID + DISK_COUNT_MAX)) {
      Private->HiiCfgData.DiskList[QuestionId - DISK_LIST_QUESTION_ID] = Value->u8;
      DEBUG ((DEBUG_INFO, "Platform storage erase form changed : set device index[%d] -> %x\n",
        QuestionId - DISK_LIST_QUESTION_ID, Private->HiiCfgData.DiskList[QuestionId - DISK_LIST_QUESTION_ID]));
    }

    LpeSetupOptionUpdate ();
  }

  return EFI_SUCCESS;
}

/**
  This function processes the results of changes in configuration.

  @param[in]  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Configuration          A null-terminated Unicode string in <ConfigResp> format.
  @param[out] Progress               A pointer to a string filled in with the offset of
                                     the most recent '&' before the first failing
                                     name/value pair (or the beginning of the string if
                                     the failure is in the first name/value pair) or
                                     the terminating NULL if all was successful.

  @retval EFI_SUCCESS                The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER      Configuration is NULL.
  @retval EFI_NOT_FOUND              Routing data doesn't match any storage in this driver.
**/
EFI_STATUS
EFIAPI
RouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  )
{
  PLATFORM_ERASE_SETUP_DRIVER  *Private;

  if ((Configuration == NULL) || (Progress == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  //
  // Check routing data in <ConfigHdr>.
  // Note: there is no name for Name/Value storage, only GUID will be checked
  //
  if (HiiIsConfigHdrMatch (Configuration, &gPsStorageEraseVariableGuid, HII_CFG_NAME)) {
    //
    // Set variable
    //
    Private = PLATFORM_ERASE_SETUP_DRIVER_FROM_THIS (This);
    SaveDeviceVariable (Private);

  } else {
    return EFI_NOT_FOUND;
  }
  *Progress = Configuration + StrLen (Configuration);
  return EFI_SUCCESS;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param[in]  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Request                A null-terminated Unicode string in <ConfigRequest> format.
  @param[out] Progress               On return, points to a character in the Request
                                     string. Points to the string's null terminator if
                                     request was successful. Points to the most recent
                                     '&' before the first failing name/value pair (or
                                     the beginning of the string if the failure is in
                                     the first name/value pair) if the request was not
                                     successful.
  @param[out] Results                A null-terminated Unicode string in
                                     <ConfigAltResp> format which has all values filled
                                     in for the names in the Request string. String to
                                     be allocated by the called function.

  @retval EFI_SUCCESS                The Results is filled with the requested values.
  @retval EFI_OUT_OF_RESOURCES       Not enough memory to store the results.
  @retval EFI_INVALID_PARAMETER      Request is illegal syntax, or unknown name.
  @retval EFI_NOT_FOUND              Routing data doesn't match any storage in this driver.
**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Request,
  OUT EFI_STRING                           *Progress,
  OUT EFI_STRING                           *Results
  )
{
  EFI_STATUS            Status;
  UINTN                 BufferSize;
  PLATFORM_ERASE_SETUP_DRIVER  *Private;
  EFI_STRING            ConfigRequestHdr;
  EFI_STRING            ConfigRequest;
  BOOLEAN               AllocatedRequest;
  UINTN                 Size;
  EFI_GUID              Guid;
  CHAR16                *Name;
  UINTN                 Length;

  DEBUG ((DEBUG_INFO,  "%a - entry\n", __FUNCTION__));

  if ((Progress == NULL) || (Results == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Name = NULL;
  *Progress = Request;
  BufferSize = 0;
  Status = EFI_SUCCESS;

  ZeroMem (&Guid, sizeof (EFI_GUID));
  if ((Request != NULL)) {
    if (HiiIsConfigHdrMatch (Request, &gPsStorageEraseVariableGuid, HII_CFG_NAME)) {
      BufferSize = sizeof (HII_CONFIG);
      CopyMem (&Guid, &gPsStorageEraseVariableGuid, sizeof (gPsStorageEraseVariableGuid));
      Length = StrSize (HII_CFG_NAME);
      Name = AllocatePool (Length);
      if (Name == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      StrCpyS (Name, Length / sizeof (CHAR16), HII_CFG_NAME);
    } else {
      return EFI_NOT_FOUND;
    }
  }

  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  AllocatedRequest = FALSE;
  Size             = 0;

  Private = PLATFORM_ERASE_SETUP_DRIVER_FROM_THIS (This);
  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  ConfigRequest = Request;
  if ((Request == NULL) || (StrStr (Request, L"OFFSET") == NULL)) {
    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    BufferSize = sizeof (HII_CONFIG);
    CopyMem (&Guid, &gPsStorageEraseVariableGuid, sizeof (gPsStorageEraseVariableGuid));
    Length = StrSize (HII_CFG_NAME);
    Name = AllocatePool (Length);
    if (Name == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }
    StrCpyS (Name, Length / sizeof (CHAR16), HII_CFG_NAME);
    ConfigRequestHdr = HiiConstructConfigHdr (&Guid, Name, Private->Handle);
    if (ConfigRequestHdr == NULL) {
      if (Name) {
        FreePool (Name);
      }
    return EFI_NOT_FOUND;
    }
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    ASSERT (ConfigRequest != NULL);
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
  }

  if (CompareGuid (&Guid, &gPsStorageEraseVariableGuid)) {
    Status = gHiiConfigRouting->BlockToConfig (
                                  gHiiConfigRouting,
                                  ConfigRequest,
                                  (UINT8 *)&Private->HiiCfgData,
                                  BufferSize,
                                  Results,
                                  Progress
                                  );
  }

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }
  if (Name) {
    FreePool (Name);
  }
  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
}

/**
  Get SETUP settings from variable and update HII_CONFIG data.

  @retval EFI_SUCCESS  Initialize HII configuration successfully.
  @retval others       Operation is fail due to some error.

**/
EFI_STATUS
InitHiiConfigFromVariable (
  VOID
  )
{
  EFI_STATUS                     Status;
  PLATFORM_ERASE_VARIABLE        *Variable;
  UINTN                          VariableSize;
  UINT32                         VariableAttr;
  UINT32                         Index;
  BOOLEAN                        VariableUpdate;
  RPE_SETUP                      RpeSetup;
  UINTN                          RpeVariableSize;
  UINT32                         RpeVariableAttr;
  ME_BIOS_PAYLOAD_HOB            *MbpHob = NULL;

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  CopyMem (&mPlatformEraseSetupDriver.HiiCfgData, &mStorageEraseHiiConfigTemplate, sizeof (HII_CONFIG));

  VariableSize   = 0;
  Variable       = NULL;
  VariableAttr   = 0;
  VariableUpdate = FALSE;

  Status = GetVariable3 (
             PLATFORM_ERASE_VARIABLE_NAME,
             &gPsPlatformEraseVariableGuid,
             (VOID **) &Variable,
             &VariableSize,
             &VariableAttr
             );
  if (EFI_ERROR (Status) || Variable == NULL) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "VariableAttr : %x \n", VariableAttr));
  ZeroMem (&RpeSetup, sizeof (RPE_SETUP));
  RpeVariableAttr = 0;
  RpeVariableSize = sizeof (RPE_SETUP);
  Status = gRT->GetVariable (
                  RPE_SETUP_VARIABLE_NAME,
                  &gPsRpeSetupVariableGuid,
                  &RpeVariableAttr,
                  &RpeVariableSize,
                  &RpeSetup
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Update HII_CONFIG from variable : %S \n", PLATFORM_ERASE_VARIABLE_NAME));
  CopyMem (&mPlatformEraseSetupDriver.HiiCfgData.SanitizeOrder, &Variable->SanitizeOrder, sizeof (Variable->SanitizeOrder));

  //
  // Do not display RPE option if Brand Identity for RPE is not supported.
  //
  if ((IsBrandIdentityEnabledForRpe ()) && (Variable->RpeOptionExposure == 0)) {
    Variable->RpeOptionExposure = 1;
    VariableUpdate = TRUE;
    if (RpeSetup.RpeStateChanged) {
      RpeSetup.RpeEnable = RpeSetup.PreviousRpeState;
      RpeSetup.RpeStateChanged = FALSE;
      }
  } else if ((!IsBrandIdentityEnabledForRpe ()) && ((Variable->RpeOptionExposure == 1) || (RpeSetup.RpeEnable == 1))) {
    Variable->RpeOptionExposure = 0;
    if (RpeSetup.RpeStateChanged == FALSE) {
      RpeSetup.PreviousRpeState = RpeSetup.RpeEnable;
      RpeSetup.RpeStateChanged = TRUE;
    }
    RpeSetup.RpeEnable = 0;
    VariableUpdate = TRUE;
  }

  if (Variable->RpeOptionExposure == 1) {
    Variable->RpeOptionGrayOut = AmtIsRemoteSessionEnabled () ? 1 : 0;
    VariableUpdate = TRUE;
  }

  //
  // Get Mbp Data HOB
  //
  MbpHob = GetFirstGuidHob(&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    if ((MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType == IntelMeCorporateFw) && (Variable->CsmeUnconfigureExposure == 0)) {
      Variable->CsmeUnconfigureExposure = 1;
      VariableUpdate = TRUE;
    } else if ((MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw) && (Variable->CsmeUnconfigureExposure == 1)) {
      Variable->CsmeUnconfigureExposure = 0;
      VariableUpdate = TRUE;
    }
  }

  if (VariableUpdate) {
    Status = gRT->SetVariable (
                    PLATFORM_ERASE_VARIABLE_NAME,
                    &gPsPlatformEraseVariableGuid,
                    VariableAttr,
                    VariableSize,
                    Variable
                    );
    ASSERT_EFI_ERROR (Status);

    Status = gRT->SetVariable(
                    RPE_SETUP_VARIABLE_NAME,
                    &gPsRpeSetupVariableGuid,
                    RpeVariableAttr,
                    RpeVariableSize,
                    &RpeSetup
                    );
    ASSERT_EFI_ERROR (Status);
  }

  DEBUG ((DEBUG_INFO, "Sanitize priority order: "));
  for (Index = 0; Index < SANITIZE_COUNT; Index ++) {
    DEBUG ((DEBUG_INFO, "0x%x >", mPlatformEraseSetupDriver.HiiCfgData.SanitizeOrder[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Main entry for this driver.

  @param[in] ImageHandle     Image Handle this driver.
  @param[in] SystemTable     Pointer to SystemTable.

  @retval EFI_SUCCESS    This function always complete successfully.
**/
EFI_STATUS
EFIAPI
PlatformSanitizeSetupEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE*  SystemTable
  )
{
  EFI_STATUS      Status;
  UINT32          PsCapabilities;

  DEBUG ((DEBUG_INFO, "PlatformSanitizeSetupEntryPoint\n"));
  //
  // Get Sanitize Capabilities from PCD
  //
  PsCapabilities = PcdGet32 (PcdPsSupportCapabilities) & PS_CAPABILITY_MASK;

  //
  // Brand Identity Check for RPE support
  //
  mRpeSupport = FALSE;
  if (PsCapabilities & PS_RPE_SUPPORTED) {
    if (!IsBrandIdentityEnabledForRpe ()) {
      PsCapabilities &= ~PS_RPE_SUPPORTED;     // Since Brand Identity not supported, Clear RPE bit in the Capability list.
      mRpeSupport = TRUE;
    }
  }

  ZeroMem (&mPlatformEraseSetupDriver, sizeof (PLATFORM_ERASE_SETUP_DRIVER));
  mPlatformEraseSetupDriver.Handle = ImageHandle;

  InitHiiConfigFromVariable ();

  //
  // Populate the config access protocol with the three functions we are publishing
  //
  mPlatformEraseSetupDriver.HiiConfigAccess.ExtractConfig = ExtractConfig;
  mPlatformEraseSetupDriver.HiiConfigAccess.RouteConfig   = RouteConfig;
  mPlatformEraseSetupDriver.HiiConfigAccess.Callback      = DriverCallback;

  //
  // Associate the required protocols with our driver handle
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mPlatformEraseSetupDriver.HiiConfigAccess,      // HII callback
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,        // required for HII callback allow all disks to be shown in same hii
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Publish the HII form and HII string packages
  //
  mPlatformEraseSetupDriver.HiiHandle = HiiAddPackages (
                                          &mPlatformSanitizeFormsetGuid,
                                          ImageHandle,
                                          PlatformSanitizeSetupDxeStrings,
                                          PlatformSanitizeSetupVfrBin,
                                          (VOID*)NULL
                                          );

  //
  // Make sure the packages installed successfully
  //
  if (mPlatformEraseSetupDriver.HiiHandle == NULL) {
    DEBUG ((DEBUG_INFO, "Platform Sanitize - HiiAddPackages failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}
