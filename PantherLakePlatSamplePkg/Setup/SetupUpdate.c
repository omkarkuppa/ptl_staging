/** @file
  Update setup formset to Advanced menu.

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

#include <Library/DebugLib.h>
#include <Guid/MdeModuleHii.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Uefi.h>
#include "SetupUpdate.h"

/**
  Extract configuration data from form.

  @param Form     Form data to be parsed.
  @param Size     Form data size.
  @param Value    configuration data value.

  @retval TRUE    Found configuration data in form.
  @retval FALSE   No configuration data found.

**/
BOOLEAN
ExtractConfigFromForm (
  IN    UINT8           *Form,
  IN    UINT32          Size,
  OUT   HII_CONFIG_PCD  *Value
  )
{
  UINT8     *OpCodeData;
  UINT32    Offset;
  EFI_GUID  Guid;

  if ((Form == NULL) || (Value == NULL)) {
    return FALSE;
  }

  Offset = ((EFI_IFR_OP_HEADER *)Form)->Length;

  while (Offset < Size) {
    OpCodeData = Form + Offset;

    if (((EFI_IFR_OP_HEADER *)OpCodeData)->OpCode == EFI_IFR_GUID_OP) {
      CopyMem (&Guid, &((EFI_IFR_GUID *)OpCodeData)->Guid, sizeof (EFI_GUID));
      if (CompareGuid (&gEfiIfrFrontPageGuid, &Guid)) {
        *Value = *(HII_CONFIG_PCD *)(OpCodeData + sizeof (EFI_IFR_GUID));
        return TRUE;
      }
    } else {
      return FALSE;
    }

    Offset += ((EFI_IFR_OP_HEADER *)OpCodeData)->Length;
  }

  return FALSE;
}

/**
  Extract configuration data from formset package.

  @param Formset    Formset data to be parsed.
  @param Size       Formset data size.
  @param Value      Configuration data value.

  @retval  TRUE     Found configuration data in formset.
  @retval  FALSE    No configuration data found.

**/
BOOLEAN
ExtractConfigFromFormset (
  IN    UINT8           *Formset,
  IN    UINT32          Size,
  OUT   HII_CONFIG_PCD  *Value
  )
{
  UINT8   *OpCodeData;
  UINT32  Offset;

  if ((Formset == NULL) || (Value == NULL)) {
    return FALSE;
  }

  Offset = 0;

  while (Offset < Size) {
    OpCodeData = Formset + Offset;
    //
    // Assume configuration opcode is always in first form.
    //
    if (((EFI_IFR_OP_HEADER *)OpCodeData)->OpCode == EFI_IFR_FORM_OP) {
      if (ExtractConfigFromForm (OpCodeData, Size - Offset, Value)) {
        return TRUE;
      }

      return FALSE;
    }

    Offset += ((EFI_IFR_OP_HEADER *)OpCodeData)->Length;
  }

  return FALSE;
}

/**
  Check if the formset with given HII handle will be displayed.

  @param Handle          The Hii handle.
  @param FormSetTitle    Formset title string.
  @param FormSetHelp     Formset help string.
  @param FormSetGuid     Displayed formset guid.
  @param HrchyConfig     Displayed formset configuration data value.

  @retval  TRUE          The formset for given HII handle will be displayed.
  @return  FALSE         The formset for given HII handle will not be displayed.

**/
BOOLEAN
ExtractDisplayedHiiFormFromHiiHandle (
  IN    EFI_HII_HANDLE  Handle,
  OUT   EFI_STRING_ID   *FormSetTitle,
  OUT   EFI_STRING_ID   *FormSetHelp,
  OUT   EFI_GUID        *FormSetGuid,
  OUT   HII_CONFIG_PCD  *HrchyConfig
  )
{
  EFI_STATUS                   Status;
  UINTN                        BufferSize;
  EFI_HII_PACKAGE_LIST_HEADER  *HiiPackageList;
  EFI_HII_PACKAGE_HEADER       *PackageHeader;
  UINT8                        *DataPtr;
  UINT32                       Offset;
  UINT32                       Offset2;
  UINT32                       PackageListLength;

  ASSERT (Handle != NULL);
  ASSERT (FormSetTitle != NULL);
  ASSERT (FormSetHelp != NULL);

  if ((Handle == NULL) || (FormSetTitle == NULL) || (FormSetHelp == NULL) || (FormSetHelp == NULL)) {
    return FALSE;
  }

  *FormSetTitle  = 0;
  *FormSetHelp   = 0;
  Offset         = 0;
  Offset2        = 0;
  BufferSize     = 0;
  HiiPackageList = NULL;

  Status = gHiiDatabase->ExportPackageLists (gHiiDatabase, Handle, &BufferSize, HiiPackageList);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  HiiPackageList = AllocatePool (BufferSize);
  ASSERT (HiiPackageList != NULL);

  Status = gHiiDatabase->ExportPackageLists (gHiiDatabase, Handle, &BufferSize, HiiPackageList);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Offset           += sizeof (EFI_HII_PACKAGE_LIST_HEADER);
  PackageListLength = ReadUnaligned32 (&HiiPackageList->PackageLength);

  while (Offset < PackageListLength) {
    PackageHeader = (EFI_HII_PACKAGE_HEADER *)((UINT8 *)HiiPackageList + Offset);

    if (PackageHeader->Type == EFI_HII_PACKAGE_FORMS) {
      Offset2 = sizeof (EFI_HII_PACKAGE_HEADER);
      while (Offset2 < PackageHeader->Length) {
        DataPtr = (UINT8 *)PackageHeader + Offset2;

        if (((EFI_IFR_OP_HEADER *)DataPtr)->OpCode == EFI_IFR_FORM_SET_OP) {
          CopyMem (FormSetTitle, &((EFI_IFR_FORM_SET *)DataPtr)->FormSetTitle, sizeof (EFI_STRING_ID));
          CopyMem (FormSetHelp, &((EFI_IFR_FORM_SET *)DataPtr)->Help, sizeof (EFI_STRING_ID));
          CopyMem (FormSetGuid, &((EFI_IFR_FORM_SET *)DataPtr)->Guid, sizeof (EFI_GUID));
          if (ExtractConfigFromFormset (DataPtr, PackageHeader->Length - sizeof (EFI_HII_PACKAGE_HEADER), HrchyConfig)) {
            FreePool (HiiPackageList);
            return TRUE;
          } else {
            FreePool (HiiPackageList);
            return FALSE;
          }
        }

        Offset2 += ((EFI_IFR_OP_HEADER *)DataPtr)->Length;
      }
    }

    Offset += PackageHeader->Length;
  }

  FreePool (HiiPackageList);
  return FALSE;
}

/**

  Initialize HiiFormList Head.

  @param   HiiFormListHead          HiiFormList.

  @retval  EFI_SUCCESS              Initialization is successful.
  @retval  EFI_INVALID_PARAMETER    Parameter is not valid.
  @retval  EFI_OUT_OF_RESOURCES     Run out of memory.

**/
EFI_STATUS
InitializeHiiFormList (
  IN OUT HII_FORM_RECORD  **HiiFormListHead
  )
{
  if (HiiFormListHead == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *HiiFormListHead = AllocateZeroPool (sizeof (HII_FORM_RECORD));
  if (*HiiFormListHead == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  (*HiiFormListHead)->GotoRecord = AllocateZeroPool (sizeof (HII_GOTO_RECORD));
  if ((*HiiFormListHead)->GotoRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  InitializeListHead (&((*HiiFormListHead)->HiiFormEntry));

  return EFI_SUCCESS;
}

/**
  Add an entry to the ordered list.

  @param   ListHead               Head of the ordered list.
  @param   InsertedEntry          New entry to insert.

  @retval  EFI_SUCCESS            Entry is successfully added.
  @retval  EFI_INVALID_PARAMETER  Parameter is not valid.

**/
EFI_STATUS
InsertOrderedList (
  IN  LIST_ENTRY  *ListHead,
  IN  LIST_ENTRY  *InsertedEntry
  )
{
  if ((ListHead == NULL) || (InsertedEntry == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  LIST_ENTRY       *ListEntry;
  HII_GOTO_RECORD  *ListData;
  HII_GOTO_RECORD  *InsertedData;

  InsertedData = HII_GOTO_RECORD_FROM_LINK (InsertedEntry);
  for (ListEntry = GetFirstNode (ListHead);
       !IsNull (ListHead, ListEntry);
       ListEntry = GetNextNode (ListHead, ListEntry))
  {
    ListData = HII_GOTO_RECORD_FROM_LINK (ListEntry);
    if (InsertedData->Order <= ListData->Order) {
      break;
    }
  }

  InsertedEntry->ForwardLink       = ListEntry;
  InsertedEntry->BackLink          = ListEntry->BackLink;
  ListEntry->BackLink->ForwardLink = InsertedEntry;
  ListEntry->BackLink              = InsertedEntry;

  return EFI_SUCCESS;
}

/**
  Insert a new HiiGotoRecord to HiiFormList based on formid and order.

  @param HiiFormListHead          HiiFormList head.
  @param Formid                   Form id to add HiiGotoRecord.
  @param HiiGotoRecord            New HiiGotoRecord to be added to the HiiFormList.

  @retval  EFI_SUCCESS            HiiGotoRecord is successfully added.
  @retval  EFI_OUT_OF_RESOURCES   Run out of memory.

**/
EFI_STATUS
AddHiiGotoRecord (
  IN  HII_FORM_RECORD  *HiiFormListHead,
  IN  UINT16           Formid,
  IN  HII_GOTO_RECORD  *HiiGotoRecord
  )
{
  LIST_ENTRY       *Link;
  HII_FORM_RECORD  *HiiFormRecord;

  HiiFormRecord = NULL;

  for (Link = GetFirstNode (&HiiFormListHead->HiiFormEntry);
       !IsNull (&HiiFormListHead->HiiFormEntry, Link);
       Link = GetNextNode (&HiiFormListHead->HiiFormEntry, Link))
  {
    HiiFormRecord = HII_FORM_RECORD_FROM_LINK (Link);
    if (HiiFormRecord->Formid == Formid) {
      InsertOrderedList (&(HiiFormRecord->GotoRecord->GotoEntry), &HiiGotoRecord->GotoEntry);
      return EFI_SUCCESS;
    }
  }

  HiiFormRecord = AllocateZeroPool (sizeof (HII_FORM_RECORD));
  if (HiiFormRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  HiiFormRecord->Formid     = Formid;
  HiiFormRecord->GotoRecord = AllocateZeroPool (sizeof (HII_GOTO_RECORD));
  if (HiiFormRecord->GotoRecord == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  InitializeListHead (&(HiiFormRecord->GotoRecord->GotoEntry));
  InsertTailList (&HiiFormListHead->HiiFormEntry, &HiiFormRecord->HiiFormEntry);
  InsertOrderedList (&(HiiFormRecord->GotoRecord->GotoEntry), &HiiGotoRecord->GotoEntry);

  return EFI_SUCCESS;
}

/**
  Free memory of HiiFormList.

  @param HiiFormListHead    HiiFormList head.

**/
VOID
FreeHiiFormList (
  IN  HII_FORM_RECORD  *HiiFormListHead
  )
{
  HII_FORM_RECORD  *HiiFormRecord;
  LIST_ENTRY       *HiiFormLink;
  HII_GOTO_RECORD  *HiiGotoListHead;
  HII_GOTO_RECORD  *HiiGotoRecord;
  LIST_ENTRY       *HiiGotoLink;

  if (HiiFormListHead != NULL) {
    HiiFormLink = GetFirstNode (&HiiFormListHead->HiiFormEntry);

    while (!IsNull (&HiiFormListHead->HiiFormEntry, HiiFormLink)) {
      HiiFormRecord   = HII_FORM_RECORD_FROM_LINK (HiiFormLink);
      HiiGotoListHead = HiiFormRecord->GotoRecord;
      HiiGotoLink     = GetFirstNode (&HiiGotoListHead->GotoEntry);

      while (!IsNull (&HiiGotoListHead->GotoEntry, HiiGotoLink)) {
        HiiGotoRecord = HII_GOTO_RECORD_FROM_LINK (HiiGotoLink);
        HiiGotoLink   = RemoveEntryList (HiiGotoLink);
        FreePool (HiiGotoRecord);
      }

      FreePool (HiiGotoListHead);
      HiiFormLink = RemoveEntryList (HiiFormLink);
      FreePool (HiiFormRecord);
    }

    FreePool (HiiFormListHead);
  }
}

/**
  Extract string from displayed Hii Handle and Update to Advanced Hii Handle.

  @param AdvancedHiiHandle        Advanced Hii handle.
  @param DisplayedHiiHandle       Displayed Hii handle.
  @param StringId                 String id in DisplayedHiiHandle Hii handle.

  @retval Token                   String id in Advanced Hii handle.

**/
EFI_STRING_ID
UpdateStringToAdvancedHiiHandle (
  IN  EFI_HII_HANDLE  AdvancedHiiHandle,
  IN  EFI_HII_HANDLE  DisplayedHiiHandle,
  IN  EFI_STRING_ID   StringId
  )
{
  EFI_STRING     String;
  EFI_STRING_ID  Token;

  String = HiiGetString (DisplayedHiiHandle, StringId, NULL);
  if (String == NULL) {
    Token = STRING_TOKEN (STR_MISSING_STRING);
  } else {
    Token = HiiSetString (AdvancedHiiHandle, 0, String, NULL);
    FreePool (String);
  }

  return Token;
}

/**
  Extract displayed formset, create goto opcode and build HiiFormList.

  @param AdvancedHiiHandle        Advanced form Hii handle.
  @param HiiFormListHead          HiiFormList head.

  @retval  EFI_SUCCESS            HiiFormList is successfully built.
  @retval  EFI_NOT_FOUND          Not found any Hii handles
  @retval  EFI_OUT_OF_RESOURCES   Run out of memory.

**/
EFI_STATUS
BuildHiiFormList (
  IN  EFI_HII_HANDLE   AdvancedHiiHandle,
  IN  HII_FORM_RECORD  *HiiFormListHead
  )
{
  EFI_STATUS       Status;
  EFI_HII_HANDLE   *AllHiiHandles;
  UINTN            Index;
  EFI_STRING_ID    FormSetTitle;
  EFI_STRING_ID    FormSetHelp;
  EFI_STRING_ID    Token;
  EFI_STRING_ID    TokenHelp;
  EFI_GUID         FormsetGuid;
  HII_CONFIG_PCD   HrchyConfig;
  HII_GOTO_RECORD  *HiiGotoRecord;

  AllHiiHandles = HiiGetHiiHandles (NULL);
  if (AllHiiHandles == NULL) {
    return EFI_NOT_FOUND;
  }

  for (Index = 0; AllHiiHandles[Index] != NULL; Index++) {
    if (!ExtractDisplayedHiiFormFromHiiHandle (
           AllHiiHandles[Index],
           &FormSetTitle,
           &FormSetHelp,
           &FormsetGuid,
           &HrchyConfig
           ))
    {
      continue;
    }

    DEBUG ((DEBUG_INFO, "%a extracts formset %g\n", __FUNCTION__, &FormsetGuid));
    if (!HrchyConfig.Show) {
      DEBUG ((DEBUG_INFO, "Formset %g is hidden\n", &FormsetGuid));
      continue;
    }

    Token     = UpdateStringToAdvancedHiiHandle (AdvancedHiiHandle, AllHiiHandles[Index], FormSetTitle);
    TokenHelp = UpdateStringToAdvancedHiiHandle (AdvancedHiiHandle, AllHiiHandles[Index], FormSetHelp);

    HiiGotoRecord = AllocateZeroPool (sizeof (HII_GOTO_RECORD));
    if (HiiGotoRecord == NULL) {
      FreePool (AllHiiHandles);
      return EFI_OUT_OF_RESOURCES;
    }

    HiiGotoRecord->Token     = Token;
    HiiGotoRecord->TokenHelp = TokenHelp;
    HiiGotoRecord->Order     = (UINT8)HrchyConfig.Order;
    CopyGuid (&HiiGotoRecord->FormsetGuid, &FormsetGuid);
    Status = AddHiiGotoRecord (HiiFormListHead, (UINT16)HrchyConfig.Formid, HiiGotoRecord);
    if (EFI_ERROR (Status)) {
      FreePool (AllHiiHandles);
      return Status;
    }
  }

  FreePool (AllHiiHandles);
  return EFI_SUCCESS;
}

/**
  Update goto opcode to Advanced formset package.

  @param AdvancedHiiHandle        Advanced form Hii handle.
  @param HiiFormListHead          HiiFormList head.

  @retval  EFI_SUCCESS            Update Advanced form successfully.
  @retval  EFI_OUT_OF_RESOURCES   Run out of memory.
  @retval  EFI_NOT_FOUND          The updated form or Hii handle is not found.

**/
EFI_STATUS
UpdateAdvancedFormSet (
  IN  EFI_HII_HANDLE   AdvancedHiiHandle,
  IN  HII_FORM_RECORD  *HiiFormListHead
  )
{
  EFI_STATUS          Status;
  EFI_STATUS          Result;
  LIST_ENTRY          *HiiFormLink;
  HII_FORM_RECORD     *HiiFormRecord;
  LIST_ENTRY          *HiiGotoLink;
  HII_GOTO_RECORD     *HiiGotoListHead;
  HII_GOTO_RECORD     *HiiGotoRecord;
  VOID                *StartOpCodeHandle;
  EFI_IFR_GUID_LABEL  *StartLabel;
  UINT16              QuestionId;

  Result     = EFI_SUCCESS;
  QuestionId = ADVANCED_QUESTION_OFFSET;

  for (HiiFormLink = GetFirstNode (&HiiFormListHead->HiiFormEntry);
       !IsNull (&HiiFormListHead->HiiFormEntry, HiiFormLink);
       HiiFormLink = GetNextNode (&HiiFormListHead->HiiFormEntry, HiiFormLink))
  {
    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    if (StartOpCodeHandle == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    StartLabel = (EFI_IFR_GUID_LABEL *)HiiCreateGuidOpCode (
                                         StartOpCodeHandle,
                                         &gEfiIfrTianoGuid,
                                         NULL,
                                         sizeof (EFI_IFR_GUID_LABEL)
                                         );
    if (StartLabel == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
    StartLabel->Number       = LABEL_FORM_START;

    HiiFormRecord   = HII_FORM_RECORD_FROM_LINK (HiiFormLink);
    HiiGotoListHead = HiiFormRecord->GotoRecord;
    for (HiiGotoLink = GetFirstNode (&HiiGotoListHead->GotoEntry);
         !IsNull (&HiiGotoListHead->GotoEntry, HiiGotoLink);
         HiiGotoLink = GetNextNode (&HiiGotoListHead->GotoEntry, HiiGotoLink))
    {
      HiiGotoRecord = HII_GOTO_RECORD_FROM_LINK (HiiGotoLink);
      HiiCreateGotoExOpCode (
        StartOpCodeHandle,
        0,
        HiiGotoRecord->Token,
        HiiGotoRecord->TokenHelp,
        0,
        (EFI_QUESTION_ID)(QuestionId ++),
        0,
        &(HiiGotoRecord->FormsetGuid),
        0
        );
    }

    Status = HiiUpdateForm (
               AdvancedHiiHandle,
               &AdvancedGuid,
               AUTO_ID (HiiFormRecord->Formid),
               StartOpCodeHandle,
               NULL
               );

    DEBUG ((DEBUG_INFO, "Advanced menu Form %d update status - %r\n", HiiFormRecord->Formid, Status));

    if (StartOpCodeHandle != NULL) {
      HiiFreeOpCodeHandle (StartOpCodeHandle);
    }

    Result |= Status;
  }

  return Result;
}

/**
  Search for formset and update formset to Advanced menu.

  @param Event      Event to be triggered.
  @param Context    Context saved.

**/
VOID
EFIAPI
UpdateAdvancedMenu (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS       Status;
  VOID             *Protocol;
  EFI_HII_HANDLE   *AdvancedHiiHandle;
  HII_FORM_RECORD  *HiiFormListHead;

  HiiFormListHead = NULL;
  Status = gBS->LocateProtocol (
                  &gSetupEnterGuid,
                  NULL,
                  &Protocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  AdvancedHiiHandle = HiiGetHiiHandles (&AdvancedGuid);
  if (AdvancedHiiHandle == NULL) {
    Status = EFI_NOT_FOUND;
    goto EXIT;
  }

  Status          = InitializeHiiFormList (&HiiFormListHead);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  Status = BuildHiiFormList (AdvancedHiiHandle[0], HiiFormListHead);
  if (EFI_ERROR (Status)) {
    goto EXIT;
  }

  Status = UpdateAdvancedFormSet (AdvancedHiiHandle[0], HiiFormListHead);

EXIT:

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Advanced menu update fails\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Advanced menu update success\n"));
  }

  if (AdvancedHiiHandle != NULL) {
    FreePool (AdvancedHiiHandle);
  }

  FreeHiiFormList (HiiFormListHead);

  gBS->CloseEvent (Event);
}
