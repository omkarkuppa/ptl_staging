/** @file
  Code which parses the HII database and then generates BIOS
  knobs sections in XML.

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

#include <XmlCliComLib.h>
#include <Protocol/HiiDatabase.h>
#include <Library/HiiLib.h>
#include <Protocol/HiiFont.h>
#include <Protocol/HiiImage.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiImageEx.h>
#include <Protocol/HiiConfigKeyword.h>


#define MAX_FRONT_PAGE_FORM                                  (0x60)
#define ASCII_ORPHANED_SETUP_PAGE                            (0x2F3F3F3F)  // equivalent of `???/`
//--------- Private Data struct definition copied from HiiDatabaseDxe\HiiDatabase.h Starts here ---------
// String Package definitions
#define HII_STRING_PACKAGE_XML_SIGNATURE                     SIGNATURE_32 ('h','i','s','p')
#define HII_DATABASE_RECORD_XML_SIGNATURE                    SIGNATURE_32 ('h','i','d','r')
#define HII_DATABASE_PRIVATE_DATA_XML_SIGNATURE              SIGNATURE_32 ('H', 'i', 'D', 'p')
#define HII_DATABASE_DATABASE_PRIVATE_DATA_FROM_THIS_XML(a)  CR (a, HII_DATABASE_PRIVATE_DATA_XML, HiiDatabase, HII_DATABASE_PRIVATE_DATA_XML_SIGNATURE)


typedef struct _HII_STRING_PACKAGE_INSTANCE_XML {
  UINTN                       Signature;
  EFI_HII_STRING_PACKAGE_HDR  *StringPkgHdr;
  UINT8                       *StringBlock;
  LIST_ENTRY                  StringEntry;
  LIST_ENTRY                  FontInfoList;  // local font info list
  UINT8                       FontId;
  EFI_STRING_ID               MaxStringId;   // record StringId
} HII_STRING_PACKAGE_INSTANCE_XML;

// A package list can contain only one or less than one device path package.
// This rule also applies to image package since ImageId can not be duplicate.
typedef struct _HII_DATABASE_PACKAGE_LIST_INSTANCE_XML {
  EFI_HII_PACKAGE_LIST_HEADER  PackageListHdr;
  LIST_ENTRY                   GuidPkgHdr;
  LIST_ENTRY                   FormPkgHdr;
  LIST_ENTRY                   KeyboardLayoutHdr;
  LIST_ENTRY                   StringPkgHdr;
  LIST_ENTRY                   FontPkgHdr;
  VOID                         *ImagePkg;
  LIST_ENTRY                   SimpleFontPkgHdr;
  UINT8                        *DevicePathPkg;
} HII_DATABASE_PACKAGE_LIST_INSTANCE_XML;


typedef struct _HII_DATABASE_RECORD_XML {
  UINTN                                   Signature;
  HII_DATABASE_PACKAGE_LIST_INSTANCE_XML  *PackageList;
  EFI_HANDLE                              DriverHandle;
  EFI_HII_HANDLE                          Handle;
  LIST_ENTRY                              DatabaseEntry;
} HII_DATABASE_RECORD_XML;

typedef struct _HII_DATABASE_PRIVATE_DATA_XML {
  UINTN                                Signature;
  LIST_ENTRY                           DatabaseList;
  LIST_ENTRY                           DatabaseNotifyList;
  EFI_HII_FONT_PROTOCOL                HiiFont;
  EFI_HII_IMAGE_PROTOCOL               HiiImage;
  EFI_HII_IMAGE_EX_PROTOCOL            HiiImageEx;
  EFI_HII_STRING_PROTOCOL              HiiString;
  EFI_HII_DATABASE_PROTOCOL            HiiDatabase;
  EFI_HII_CONFIG_ROUTING_PROTOCOL      ConfigRouting;
  EFI_CONFIG_KEYWORD_HANDLER_PROTOCOL  ConfigKeywordHandler;
  LIST_ENTRY                           HiiHandleList;
  INTN                                 HiiHandleCount;
  LIST_ENTRY                           FontInfoList;  // global font info list
  UINTN                                Attribute;     // default system color
  EFI_GUID                             CurrentLayoutGuid;
  EFI_HII_KEYBOARD_LAYOUT              *CurrentLayout;
} HII_DATABASE_PRIVATE_DATA_XML;


//------------------------------ Private Data struct definition ends here ------------------------------

SHARED_MAILBOX_TYPE DRAMSharedMailBoxTable[] = {
  { LEG_MAILBOX_SIG,     LEG_MAILBOX_OFFSET,    LEG_MAILBOX_SIZE,   SHARED_MEMORY_FLAG_MEMORY_TYPE },
  { CLI_REQ_SIG,         CLI_REQ_BUFFER_OFFSET, CLI_BUFFER_SIZE,    SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { CLI_RES_SIG,         CLI_RES_BUFFER_OFFSET, CLI_BUFFER_SIZE,    SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { SHARED_MB_LAST_SIG,  SHARED_MB_LAST_SIG,    SHARED_MB_LAST_SIG, SHARED_MB_LAST_SIG}
};

typedef struct {
  EFI_GUID  HiiProtocolGuid;
  UINT32    HiiDatabaseSize;
} EFI_HII_PACKAGE_EXPORT_HEADER;


const CHAR8*  NullString = "";


/**
  Get String Length

  @param[in] String  Input EFI_STRING for which length to be found.

  @retval  UINTN  Length of String
**/
UINTN
StrLenUnaligned (
  IN EFI_STRING  String
  )
{
  UINTN  Length;
  for (Length = 0; *String != L'\0'; String++, Length++){
    ;
  }
  return Length;
}

/**
  Convert Unicode String to Ascii

  @param[in]  Source       Character pointer for Unicode string
  @param[out] Destination  Character pointer for ASCII string
  @param[in]  DestMax      Length of expected ASCII Output string

  @retval  VOID
**/
VOID
UnicodeToAsciiUnaligned (
  IN  CHAR16  *Source,
  OUT CHAR8   *Destination,
  IN  UINTN   DestMax
  )
{
  UINTN Length=0;

  while (*Source != '\0') {
    if (Length >= DestMax) {
      break;
    }
    *(Destination++) = (CHAR8)*(Source++);
    Length++;
  }
  *Destination = '\0';
}

/**
  Allocated Memory for given buffer pointer

  @param[in]  PoolType       EFI_MEMORY_TYPE Type of Memory Pool to be allocated
  @param[in]  PoolSize       Size of Memory to be allocated
  @param[out] BufferPointer  Buffer Pointer to allocate string

  @retval  VOID
**/
VOID
MyAllocateZeroPool (
  IN  EFI_MEMORY_TYPE  PoolType,
  IN  UINTN            PoolSize,
  OUT VOID             **BufferPointer
  )
{
  EFI_STATUS  Status;
  Status = gBS->AllocatePool(PoolType, PoolSize, BufferPointer);
  ASSERT_EFI_ERROR(Status);
  ASSERT(*BufferPointer != NULL);
  ZeroMem(*BufferPointer, PoolSize);
}

/**
  Find Hii String Block

  @param[in] HiiDatabase        EFI_MEMORY_TYPE Type of Memory Pool to be allocated
  @param[in] DriverFormSetGuid  Size of Memory to be allocated
  @param[in] Language           Buffer Pointer to allocate string

  @retval  EFI_HII_STRING_BLOCK*
**/
EFI_HII_STRING_BLOCK*
FindStringBlock (
  IN EFI_HII_DATABASE_PROTOCOL  *HiiDatabase,
  IN EFI_GUID                   *DriverFormSetGuid,
  IN CHAR8                      *Language
  )
{
  LIST_ENTRY                              *Link;
  HII_DATABASE_PRIVATE_DATA_XML           *Private;
  HII_DATABASE_RECORD_XML                 *DatabaseRecord;
  HII_DATABASE_PACKAGE_LIST_INSTANCE_XML  *PackageListNode;
  HII_STRING_PACKAGE_INSTANCE_XML         *StringPackage;
  EFI_HANDLE                              *HiiHandleBuffer;
  EFI_HANDLE                              HiiHandle;
  EFI_HII_STRING_BLOCK                    *StringBlock=NULL;

  HiiHandleBuffer = HiiGetHiiHandles (DriverFormSetGuid);
  if (HiiHandleBuffer == NULL) {
    return NULL;
  }
  HiiHandle = HiiHandleBuffer[0];

  Private = HII_DATABASE_DATABASE_PRIVATE_DATA_FROM_THIS_XML (HiiDatabase);
  PackageListNode = NULL;

  for (Link = Private->DatabaseList.ForwardLink; Link != &Private->DatabaseList; Link = Link->ForwardLink) {
    DatabaseRecord = CR (Link, HII_DATABASE_RECORD_XML, DatabaseEntry, HII_DATABASE_RECORD_XML_SIGNATURE);
    if (DatabaseRecord->Handle == HiiHandle) {
      PackageListNode = DatabaseRecord->PackageList;
      break;
    }
  }

  if (PackageListNode != NULL) {
    for (Link =  PackageListNode->StringPkgHdr.ForwardLink; Link != &PackageListNode->StringPkgHdr; Link =  Link->ForwardLink) {
      StringPackage = CR (Link, HII_STRING_PACKAGE_INSTANCE_XML, StringEntry, HII_STRING_PACKAGE_XML_SIGNATURE);
      if (AsciiStrCmp(StringPackage->StringPkgHdr->Language, Language) == 0) {
        StringBlock  = (EFI_HII_STRING_BLOCK*)StringPackage->StringBlock;
        break;
      }
    }
  }
  return StringBlock;
}

/**
  Save Default VarStore values to XmlCli Common Structure

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  VOID
**/
VOID
XmlSaveDefVarstore (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  )
{
  UINT32      SetupDataAttributes=0;
  UINTN       Index;
  UINTN       Size;
  CHAR16      UniCodeName[MAX_VARSTORE_NAME];
  EFI_STATUS  Status=EFI_SUCCESS;
  VOID        *DataPtr=NULL;
  VOID        *DefDataPtr=NULL;

  DataPtr = (VOID*)(UINTN)XmlCliCommon->CurrGetSetVarBuffer;
  for (Index=0; Index<XmlCliCommon->NumberOfVarstores; Index++) {
    // DEBUG ((DEBUG_INFO, "XML_CLI: GetVariable on the NVAR %a Size = 0x%X \n", XmlCliCommon->VarstoreTable[Index].Name, Size));
    AsciiStrToUnicodeStrS ((CHAR8 *)(XmlCliCommon->VarstoreTable[Index].Name), UniCodeName, sizeof (UniCodeName));
    Size = 0;   // set Size to 0, to Get actual Size of current Variable
    Status = gRT->GetVariable (UniCodeName, &XmlCliCommon->VarstoreTable[Index].Guid, &SetupDataAttributes, (UINTN*)&Size, NULL);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Status = %r  Size = 0x%X NvarGuid = %g \n", Status, Size, XmlCliCommon->VarstoreTable[Index].Guid));
    }
    XmlCliCommon->VarstoreTable[Index].Size = (UINT16)Size;
    if (Size == 0) {  // If Size is 0 is still zero, skip this entry
      DEBUG ((DEBUG_INFO, "XML_CLI: Size is zero for NVAR %a, skip this entry \n", XmlCliCommon->VarstoreTable[Index].Name));
      continue;
    }
    Status = gRT->GetVariable (UniCodeName, &XmlCliCommon->VarstoreTable[Index].Guid, &SetupDataAttributes, (UINTN*)&Size, DataPtr);
    if (!EFI_ERROR(Status)) {
      XmlCliCommon->VarstoreTable[Index].Data = DataPtr;  // save Data pointer for current Variable
      DefDataPtr = (VOID*)((UINT8*)DataPtr + Size + 0x100);  // 256 bytes of safety, in case the size increases
      if (XmlCliCommon->VarstoreTable[Index].DefName[0] != 0) {
        AsciiStrToUnicodeStrS ((CHAR8 *)(XmlCliCommon->VarstoreTable[Index].DefName), UniCodeName, sizeof (UniCodeName));
        Status = gRT->GetVariable (UniCodeName, &XmlCliCommon->VarstoreTable[Index].DefGuid, &SetupDataAttributes, (UINTN*)&Size, DefDataPtr);
        if (EFI_ERROR(Status)) {  // Write current Data As default
          if (XmlCliCommon->XmlSkipSavingDefVar == FALSE) {  // this is to let platform settle the dynamics
            SetupDataAttributes = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS);
            if (Size != XmlCliCommon->VarstoreTable[Index].Size) {
              Size = XmlCliCommon->VarstoreTable[Index].Size;
            }
            Status = gRT->SetVariable (UniCodeName, &XmlCliCommon->VarstoreTable[Index].DefGuid, SetupDataAttributes, Size, DataPtr);
            if (EFI_ERROR(Status)) {
              ASSERT_EFI_ERROR(Status);
            }
            DEBUG ((DEBUG_INFO, "XML_CLI: SetVariable on the L\"%a\" VAR\n", XmlCliCommon->VarstoreTable[Index].DefName));
          }
          CopyMem ((VOID*)DefDataPtr, (VOID*)DataPtr, Size);  // since we just wrote Def Data, copy the Current Data as Default Data
        }
      } else {
        CopyMem ((VOID*)DefDataPtr, (VOID*)DataPtr, Size);  // since this Nvar doesn't support DefVar, copy the Current Data as Default Data
      }
      XmlCliCommon->VarstoreTable[Index].DefData = DefDataPtr;  // save Data pointer for current Variable
      DataPtr = (VOID*)((UINT8*)DefDataPtr + Size + 0x100);  // next DataPtr, 256 bytes of safety, in case the size increases
    } else {
      XmlCliCommon->VarstoreTable[Index].Size = 0;
      ZeroMem((VOID*)&XmlCliCommon->VarstoreTable[Index].Guid, sizeof(EFI_GUID));
      ZeroMem((VOID*)&XmlCliCommon->VarstoreTable[Index].DefGuid, sizeof(EFI_GUID));
      ZeroMem((VOID*)XmlCliCommon->VarstoreTable[Index].Name, sizeof(XmlCliCommon->VarstoreTable[Index].Name));
      ZeroMem((VOID*)XmlCliCommon->VarstoreTable[Index].DefName, sizeof(XmlCliCommon->VarstoreTable[Index].DefName));
      DEBUG ((EFI_D_ERROR, "XML_CLI: GetVariable on the L\"%a\" VAR failed, ignore this Var from processing\n", XmlCliCommon->VarstoreTable[Index].Name));
    }
  }
  // DEBUG ((DEBUG_INFO, "XML_CLI: GetSetVarBuffer Advanced by 0x%X\n", (DataPtr - XmlCliCommon->CurrGetSetVarBuffer)));
  XmlCliCommon->CurrGetSetVarBuffer = (UINT32)(UINTN)DataPtr;  // initialize the pointer for someone else to use
}

/**
  Store Varstore info into varstoreTable

  @param[in] KnobParamData  Knob Parameter Data Pointer
  @param[in] Name           Size of Memory to be allocated
  @param[in] Guid           Buffer Pointer to allocate string
  @param[in] Size           Buffer Pointer to allocate string
  @param[in] VarStoreId     Buffer Pointer to allocate string

  @retval  VOID
**/
VOID TestAndSaveVarstore (
  IN KNOB_PARAM_DATA  *KnobParamData,
  IN UINT8            *Name,
  IN EFI_GUID         *Guid,
  IN UINT16           Size,
  IN EFI_VARSTORE_ID  VarStoreId
  )
{
  UINTN  Index;

  for (Index = 0; Index < KnobParamData->NumberOfVarstores; Index++) {
    if ((AsciiStrCmp((CHAR8 *) KnobParamData->VarstoreTable[Index].Name, (CHAR8 *) Name) == 0) && CompareGuid(&KnobParamData->VarstoreTable[Index].Guid, Guid)) {
      // DEBUG ((DEBUG_INFO, "XML_CLI: Hii Id = %d  Table Varstore Id = %d\n", VarStoreId, Index));
      // DEBUG ((DEBUG_INFO, "XML_CLI: NVAR name=%a Guid=%g \n", Name, Guid));
      // DEBUG ((DEBUG_INFO, "XML_CLI: Hii Varstore size = 0x%X  Size from Table = 0x%X \n", Size, KnobParamData->VarstoreTable[Index].Size));
      if (KnobParamData->VarstoreTable[Index].Size >= Size) {
        KnobParamData->VarstoreTable[Index].FoundInPackage = TRUE;
        KnobParamData->VarstoreTable[Index].VarStoreId = VarStoreId;
      }
      break;
    }
  }
}

/**
  Test if VarStoreId found in VarstoreTable

  @param[in]  KnobParamData  Knob Parameter Data Pointer
  @param[in]  VarStoreId     varstore id to lookup for
  @param[out] VarstoreIndex  Index in varstoreTable that has VarStoreId. the returned value is valid only of this function return is TRUE

  @retval  BOOLEAN  whether the VarStoreId is found
**/
BOOLEAN
TestVarstoreId (
  IN KNOB_PARAM_DATA  *KnobParamData,
  IN EFI_VARSTORE_ID  VarStoreId,
  OUT UINT8            *VarstoreIndex
  )
{
  UINT8 Index;
  if (VarstoreIndex == NULL) {
     return FALSE;
  }
  for (Index=0;Index<KnobParamData->NumberOfVarstores;Index++) {
    if (KnobParamData->VarstoreTable[Index].FoundInPackage == TRUE) {
      if (KnobParamData->VarstoreTable[Index].VarStoreId == VarStoreId) {
        *VarstoreIndex = Index;
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
  This function is supporting Hii opcode EFI_IFR_ONE_OF_OP
  since we don't support EFI_IFR_REFRESH_OP
  we need skip to EFI_IFR_END_OP when this opcode is found.
  If EFI_IFR_REFRESH_OP is not found than
  we can continue normally to EFI_IFR_ONE_OF_OPTION_OP

  @param[in] IfrOpCodeHeader  pointer to Hii opcode header

  @retval  EFI_IFR_OP_HEADER*  if EFI_IFR_REFRESH_OP is found in
                               the opcode chain than pointer to EFI_IFR_END_OP.
                               else where pointer to original opcode header
**/
EFI_IFR_OP_HEADER *
TestRefreshOpcode (
  IN EFI_IFR_OP_HEADER  *IfrOpCodeHeader
  )
{
  EFI_IFR_OP_HEADER  *SaveIfrOpCodeHeader=IfrOpCodeHeader;
  BOOLEAN            hasRefreshOpcode=FALSE;

  do {
    IfrOpCodeHeader = (EFI_IFR_OP_HEADER *)((UINTN)IfrOpCodeHeader + IfrOpCodeHeader->Length);
    if (IfrOpCodeHeader->OpCode == EFI_IFR_REFRESH_OP) {
      hasRefreshOpcode = TRUE;
    }
  } while (IfrOpCodeHeader->OpCode != EFI_IFR_END_OP);

  if (hasRefreshOpcode == TRUE) {
    return IfrOpCodeHeader;
  } else {
    return SaveIfrOpCodeHeader;
  }
}

/**
  This function incremets the IFR Opcode till it finds the
  end of the Opcode Header

  @param[in,out] IfrOpCodeHeader  pointer to Hii opcode header

  @retval  EFI_IFR_OP_HEADER  if EFI_IFR_REFRESH_OP is found in
                              the opcode chain than pointer to EFI_IFR_END_OP.
                              else where pointer to original opcode header
**/
EFI_IFR_OP_HEADER
*getEndOpcode (
  IN OUT EFI_IFR_OP_HEADER  *IfrOpCodeHeader
  )
{
  while (IfrOpCodeHeader->OpCode != EFI_IFR_END_OP) {
    IfrOpCodeHeader = (EFI_IFR_OP_HEADER *)((UINTN)IfrOpCodeHeader + IfrOpCodeHeader->Length);
  }
  return IfrOpCodeHeader;
}

/**
  Add Attribute of setup type for as per Hii Opcode (setup knob type)

  @param[in] IfrOpCodeHeader  pointer to Hii opcode header

  @retval  VOID
**/
VOID
AddHiiOpcodeElement (
  IN EFI_IFR_OP_HEADER  *IfrOpCodeHeader
  )
{
  CHAR8  *setupType=NULL;
  switch (IfrOpCodeHeader->OpCode) {
  case EFI_IFR_ONE_OF_OP:
    setupType = "oneof";
    break;
  case EFI_IFR_CHECKBOX_OP:
    setupType = "checkbox";
    break;
  case EFI_IFR_NUMERIC_OP:
    setupType = "numeric";
    break;
  case EFI_IFR_STRING_OP:
    setupType = "string";
    break;
  }
  ASSERT(setupType != NULL);
  XmlAddAttributes ("setupType", "%a", setupType);
}

/**
  Get length of Opcode Entry (setup knob)

  @param[in] IfrOpCodeHeader  pointer to Hii opcode header

  @retval  UINT64  size of the setup knob
**/
UINT64
GetOpcodeEntryLength (
  IN EFI_IFR_OP_HEADER  *IfrOpCodeHeader
  )
{
  if ((IfrOpCodeHeader->OpCode == EFI_IFR_NUMERIC_OP) || (IfrOpCodeHeader->OpCode == EFI_IFR_ONE_OF_OP)) {
    return(LShiftU64(1, (((EFI_IFR_ONE_OF*)IfrOpCodeHeader)->Flags & EFI_IFR_NUMERIC_SIZE)));
  }
  if (IfrOpCodeHeader->OpCode == EFI_IFR_STRING_OP) {
    return(UINT64)(((EFI_IFR_STRING*)IfrOpCodeHeader)->MaxSize * sizeof(CHAR16));
  }
  if (IfrOpCodeHeader->OpCode == EFI_IFR_CHECKBOX_OP) {
    return (1);
  }
  ASSERT(0);
  return (0);
}

/**
  Handle Special characters in the string

  @param[in] String  Character pointer for input string

  @retval  CHAR8*  escape special characters
**/
CHAR8*
HandleSpecialXmlChars (
  IN CHAR8  *String
  )
{
  UINT8  *StringPtr;
  if (String == NULL) {
    return (CHAR8*)NullString;
  }

  StringPtr = (UINT8*)String;

  while (*StringPtr != 0) {
    if ((*StringPtr == 0xAE) || (*StringPtr == 0x0A) || (*StringPtr == 0x0D)){
      *StringPtr = 0x20;
    } else if (*StringPtr == 0x3D) {
      *StringPtr = 0x2D;
    } else if (*StringPtr == 0x26) {
      *StringPtr = 0x6E;
    } else if (*StringPtr == 0xB5) {
      *StringPtr = 0x75;
    } else if ((*StringPtr == 0x3C) || (*StringPtr == 0x3E) || (*StringPtr == 0x22)
            || (*StringPtr == 0x0B) || (*StringPtr == 0x0C) || (*StringPtr == 0x27)
            || ((*StringPtr >= 0x01) && (*StringPtr <= 0x08))
            || ((*StringPtr >= 0x0E) && (*StringPtr <= 0x1F))
            || ((*StringPtr >= 0x7F) && (*StringPtr <= 0x84))
            || ((*StringPtr >= 0x86) && (*StringPtr <= 0xFF))) {
      *StringPtr = 0x3F;
    }
    if ((*StringPtr == 0x2D) && ((*(StringPtr+1) == 0x2D) || (*(StringPtr+1) == 0x3D))) { // avoid Dashes.
      *(StringPtr+1) = 0x20;
    }
    StringPtr++;
  }
  return (String);
}

/**
  Add the Attribute DEPEX operation from Hii

  @param[in,out] IfrOpCodeHeader     Pointer to Hii opcode header
  @param[in]     StringHash          Pointer* to String Hash
  @param[in]     DupIndexLst         List of Duplicate Instances
  @param[in]     VarstoreIndex       Nvar Index at which the DEPEX to be associated at
  @param[in]     DepexPtr            Character Array to DEPEX value for knob entry
  @param[in,out] KnobParamData       Knob Parameter Data
  @param[in]     KnobXmlEntryOffset  Offset at which Knob Entry exists within the Nvar Data

  @retval  EFI_IFR_OP_HEADER*
**/
EFI_IFR_OP_HEADER *
AddHiiOptionAttributes (
  IN OUT EFI_IFR_OP_HEADER  *IfrOpCodeHeader,
  IN CHAR8                  **StringHash,
  IN UINT8                  *DupIndexLst,
  IN UINT8                  VarstoreIndex,
  IN CHAR8                  *DepexPtr,
  IN OUT KNOB_PARAM_DATA    *KnobParamData,
  IN UINT32                 KnobXmlEntryOffset,
  IN BOOLEAN                BitKnobFound
  )
{
  UINT8              DupCount;
  UINT8              LoopCount;
  UINT8              Flags;
  UINT8              NoOfOptions=0;
  UINT8              OneOfScopeLvl=0;
  UINT64             Value=0;
  EFI_IFR_OP_HEADER  *LocalIfrOpCodeHeader=NULL;
  UINT64             TempKnobsSize;

  LoopCount = 1;
  if (DupIndexLst[0] > 0) {
    LoopCount = DupIndexLst[0]+1;
  }
  LocalIfrOpCodeHeader = IfrOpCodeHeader;

  for (DupCount=0; DupCount < LoopCount; DupCount++) {
    if (DupCount == 0) {
      if (LoopCount == 1) {
        XmlCreateElement ("options", 0);
      } else {
        XmlCreateElement ("options", 1);
        if (DepexPtr != NULL) {
          XmlAddAttributes ("depex", "%a", DepexPtr);
        }
        XmlCloseElement (0, 1);
      }
    } else {
      LocalIfrOpCodeHeader = KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[DupCount] ].PrevIfrOpCodeHdr;
      if (LocalIfrOpCodeHeader == NULL) break;
      XmlCreateElement ("options", 1);
      if (KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[DupCount] ].PrevKnobExp == NULL) {
        XmlAddAttributes ("depex", "%a", "TRUE");
      } else {
        XmlAddAttributes ("depex", "%a", KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[DupCount] ].PrevKnobExp);
      }
      XmlCloseElement (0, 1);
    }

    OneOfScopeLvl = OneOfScopeLvl + LocalIfrOpCodeHeader->Scope;
    while (OneOfScopeLvl > 0) {
      LocalIfrOpCodeHeader = (EFI_IFR_OP_HEADER *)((UINTN)LocalIfrOpCodeHeader + LocalIfrOpCodeHeader->Length);
      OneOfScopeLvl = OneOfScopeLvl + LocalIfrOpCodeHeader->Scope;
      if (LocalIfrOpCodeHeader->OpCode == EFI_IFR_END_OP) {
        OneOfScopeLvl = OneOfScopeLvl - 1;
      }
      if (LocalIfrOpCodeHeader->OpCode == EFI_IFR_ONE_OF_OPTION_OP) {
        XmlCreateChild ("option", 0);
        XmlAddAttributes ("text", "%a", HandleSpecialXmlChars(StringHash[ ((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Option ]));
        if (BitKnobFound) {
          Flags = EFI_IFR_TYPE_NUM_SIZE_32;  // for BitWise, the Width fields remain UINT32.
        } else {
          Flags = (((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Flags) & 0x0F;
        }
        switch (Flags) {
          case EFI_IFR_TYPE_BOOLEAN:
            Value = ((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Value.b;
            break;
          case EFI_IFR_TYPE_NUM_SIZE_8:
            Value = ((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Value.u8;
            break;
          case EFI_IFR_TYPE_NUM_SIZE_16:
            Value = ((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Value.u16;
            break;
          case EFI_IFR_TYPE_NUM_SIZE_32:
            Value = ((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Value.u32;
            break;
          case EFI_IFR_TYPE_NUM_SIZE_64:
            Value = ((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Value.u64;
            break;
          default:
            DEBUG((DEBUG_INFO, "XML_CLI: (((EFI_IFR_ONE_OF_OPTION *)LocalIfrOpCodeHeader)->Flags) = 0x%02x", Flags));
            ASSERT(0);
            break;
        }
        TempKnobsSize = KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize;
        XmlAddAttributes ("value", "0x%0.*lX", TempKnobsSize * 2, Value);
        XmlCloseChild ();

        NoOfOptions++;
        CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)&Value, (UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize);
        KnobParamData->KnobValMapCurrOff += (UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize;
      }
    }
    XmlCloseElement ("options", 0);
    if (DupCount == 0){
      IfrOpCodeHeader = LocalIfrOpCodeHeader;
    }
  }

  CopyMem((VOID*)&LoopCount, (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+(UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset), 1);
  if ((LoopCount != 0) && (LoopCount < KNOB_TYPE_CUTOFF)) {
    NoOfOptions += LoopCount;
  }
  CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+(UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset), (VOID*)&NoOfOptions, 1);

  return(IfrOpCodeHeader);
}

/**
  Add the Knob Element to DRAM interface memory in Xml format
  This method is responsible to verify the knob parameter to
  corresponding Hii Op code and generate valid xml entry with
  tag and attribute for every available knob element.

  @param[in,out] IfrOpCodeHeader  Pointer to Hii opcode header
  @param[in,out] KnobParamData    Knob Parameter Data
  @param[in]     EngStringHash    Character pointer to English Hash String
  @param[in]     VarstoreIndex    Index at Nvar stored in Nvar Table [matched varstoreindex with BiosKnobData.bin]
  @param[in]     FrontPageFormId  Form Id of front Page through
                                   which the hierarchy of knob element maintained

  @retval  EFI_IFR_OP_HEADER*
**/
EFI_IFR_OP_HEADER *
AddKnobElement (
  IN OUT EFI_IFR_OP_HEADER  *IfrOpCodeHeader,
  IN OUT KNOB_PARAM_DATA    *KnobParamData,
  IN CHAR8                  **EngStringHash,
  IN UINT8                  VarstoreIndex,
  IN UINT16                 *FrontPageFormId
  )
{
  BOOLEAN  DupKnobInst=FALSE;
  BOOLEAN  BitKnobFound=FALSE;
  UINT8    Data8;
  UINT8    BitOffset;
  UINT8    BitSize;
  UINT16   count;
  UINT16   dupindex=0;
  UINT16   dupfound=0;
  UINT16   CurrDupIndex=0;
  UINT16   DupIndexLst[MAX_DUP_INSTANCE];  // first variable of this array will tell number of duplicate instances, followed with the instance index list
  UINT32   EntryStartAddr;
  UINT32   KnobXmlEntryOffset=0;
  UINT32   Data32=0;
  UINT64   VarLength=0;
  UINT64   CurrentVal;
  UINT64   DefaultVal;
  UINT64   Data64;
  UINTN    Min=0;
  UINTN    Max=0;
  UINTN    Step=0;
  UINTN    VarOffset;
  CHAR8    TrueDepex[10]="TRUE";
  CHAR8    TmpDepex[UNITS_512];
  CHAR8    KnobValStr[20];
  CHAR8    *DepexPtr=NULL;
  CHAR8    *SetupPgPtr=NULL;
  CHAR8    *TempChar8Ptr=NULL;

  if (IfrOpCodeHeader == 0){
     return IfrOpCodeHeader;
  }

  IfrOpCodeHeader = TestRefreshOpcode(IfrOpCodeHeader);
  if (IfrOpCodeHeader->OpCode != EFI_IFR_END_OP) {
    VarOffset = ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.VarStoreInfo.VarOffset;
    if(KnobParamData->BitWiseQuo) {
      // Process Bit wise setup knob
      BitKnobFound = FALSE;
      VarOffset = VarOffset/8;
      if (1 == KnobParamData->VarstoreTable[VarstoreIndex].KnobInfo[VarOffset].BitWise) {
        for (count=KnobParamData->VarstoreTable[VarstoreIndex].BinKnobOffset[VarOffset]; count < (KnobParamData->VarstoreTable[VarstoreIndex].Size + 1 + KnobParamData->VarstoreTable[VarstoreIndex].BitKnobCount); count++) {
          if (KnobParamData->VarstoreTable[VarstoreIndex].BinKnobOffset[count] == VarOffset) {
            BitOffset = KnobParamData->VarstoreTable[VarstoreIndex].BinKnobSize[count] & 0x7;
            BitSize = (KnobParamData->VarstoreTable[VarstoreIndex].BinKnobSize[count] >> 3) & 0x3F;
            if ((1 == KnobParamData->VarstoreTable[VarstoreIndex].KnobInfo[count].BitWise)
              && (BitOffset == (UINT8)(((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.VarStoreInfo.VarOffset % 8))
              && (BitSize == (UINT8)(((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Flags & 0x3F)) ) {
                BitKnobFound = TRUE;
                break;
            }
          }
        }
        if (BitKnobFound) {
          VarLength = BitGetByteSize(BitOffset, BitSize);
          KnobXmlEntryOffset = KnobParamData->VarstoreTable[VarstoreIndex].KnobXmlEntryBase + (UINT32)VarOffset;
          if (((KNOB_ENTRY_TBL_BITWISE*)KnobParamData->KnobXmlEntry)[KnobXmlEntryOffset].KnobSize < VarLength) {
            ((KNOB_ENTRY_TBL_BITWISE*)KnobParamData->KnobXmlEntry)[KnobXmlEntryOffset].KnobSize = VarLength;
          }
          ((KNOB_ENTRY_TBL_BITWISE*)KnobParamData->KnobXmlEntry)[KnobXmlEntryOffset].IsBitWise = 1;
          CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)(UINTN)&KnobParamData->VarstoreTable[VarstoreIndex].BinKnobSize[count], 1);
          KnobParamData->KnobValMapCurrOff += 1;
          CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)(UINTN)&VarOffset, 2);
          KnobParamData->KnobValMapCurrOff += 2;
          VarOffset = count;
          KnobParamData->KnobXmlEntry[KnobParamData->VarstoreTable[VarstoreIndex].KnobXmlEntryBase + (UINT32)VarOffset].KnobValMapOffset = (UINT32)KnobParamData->KnobValMapCurrOff;
        } else {
          return IfrOpCodeHeader;
        }
      }
    }
    Data8 = KnobParamData->VarstoreTable[VarstoreIndex].BinKnobType[VarOffset];
    if ((EngStringHash != NULL) && (KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ] != NULL) && (Data8 == IfrOpCodeHeader->OpCode)) {
      KnobXmlEntryOffset = KnobParamData->VarstoreTable[VarstoreIndex].KnobXmlEntryBase + (UINT32)VarOffset;
      dupfound = 0;
      ZeroMem (DupIndexLst, sizeof(DupIndexLst));
      for (dupindex=0; dupindex < MAX_DUP_KNOBS_SUPPORTED; dupindex++) {
        if (KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ dupindex ].DupKnobName == NULL) {

          ///
          /// Out of Duplicate knobs initialized list then break
          ///
          break;
        }
        if (AsciiStrCmp((CHAR8 *) KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ], (CHAR8 *) KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ dupindex ].DupKnobName) == 0) {
          dupfound +=1;
          DupIndexLst[dupfound] = dupindex;
        }
      }
      DupIndexLst[0] = dupfound;
      if ((dupfound == 0) || (KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed == 0)) {
        if (KnobParamData->VarstoreTable[ VarstoreIndex ].ExpressionHash[ VarOffset ] == NULL) {
          DepexPtr = (CHAR8*)TrueDepex;
        } else {
          DepexPtr = (CHAR8*)KnobParamData->VarstoreTable[ VarstoreIndex ].ExpressionHash[ VarOffset ];
        }
      } else {
        if (KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed] ].DupKnobExp == NULL) {
          DepexPtr = (CHAR8*)TrueDepex;
        } else {
          DepexPtr = (CHAR8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed] ].DupKnobExp;
        }
      }

      if (KnobParamData->MergeDuplicateKnobs) {
        if (dupfound != KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed) {
          CurrDupIndex = DupIndexLst[ KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed + 1 ];
          KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ CurrDupIndex ].PrevIfrOpCodeHdr = IfrOpCodeHeader;
          if (KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed == 0) {
            KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ CurrDupIndex ].PrevKnobExp = (UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].ExpressionHash[ VarOffset ];
          } else {
            KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ CurrDupIndex ].PrevKnobExp = (UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed ] ].DupKnobExp;
          }
          if (IfrOpCodeHeader->OpCode == EFI_IFR_ONE_OF_OP) {
            while (IfrOpCodeHeader->OpCode != EFI_IFR_END_OP) {
              IfrOpCodeHeader = (EFI_IFR_OP_HEADER *)((UINTN)IfrOpCodeHeader + IfrOpCodeHeader->Length);
            }
          }
          KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed += 1;
          return (IfrOpCodeHeader);
        } else {
          if ((IfrOpCodeHeader->OpCode != EFI_IFR_ONE_OF_OP) && (dupfound > 0)) {
            ZeroMem (TmpDepex, sizeof(TmpDepex));
            AsciiStrCpyS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)"(");
            if (KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[1] ].PrevKnobExp == NULL) {
              AsciiStrCatS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)TrueDepex);
            } else {
              AsciiStrCatS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[1] ].PrevKnobExp);
            }
            for (dupindex=0; dupindex < dupfound; dupindex++) {
              AsciiStrCatS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)") OR (");
              if (KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[1+dupindex] ].DupKnobExp == NULL) {
                AsciiStrCatS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)TrueDepex);
              } else {
                AsciiStrCatS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[1+dupindex] ].DupKnobExp);
              }
              AsciiStrCatS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DupKnobDetails[ DupIndexLst[1+dupindex] ].DupKnobExp);
            }
            AsciiStrCatS((CHAR8*)TmpDepex, sizeof (TmpDepex), (CHAR8*)")");
            DepexPtr = (CHAR8*)TmpDepex;
          }
        }
      } else { // clear the duplicate data struct so that we dont merge the Dup knobs together
        dupfound = 0;
        ZeroMem (DupIndexLst, sizeof(DupIndexLst));
      }

      DupKnobInst = FALSE;
      if ((KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed > 0) && (dupfound == 0)) {
        DupKnobInst = TRUE;
      }
      EntryStartAddr = ReturnXmlPtrAddr();
      KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobXmlEntryOfst = (UINT32)(EntryStartAddr - (KnobParamData->GbtXmlAddress+4));
      Data32 = (UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset;
      KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset = (UINT32)KnobParamData->KnobValMapCurrOff;
      if (IfrOpCodeHeader->OpCode == EFI_IFR_ONE_OF_OP) {
        if (DupKnobInst) {
          if ((Data32 != 0) && ((UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize != 0)){
            CopyMem((VOID*)&Data8, (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32), 1);
            if (Data8 < KNOB_TYPE_CUTOFF) {
              CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+(UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset), (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32), ((UINTN)(Data8 *  (UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize)+1));
              KnobParamData->KnobValMapCurrOff += (UINT32)(Data8 * (UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize); // increment the KnobValMapCurrOff accordingly
            }
          }
        }
        XmlCreateElement ("knob ", 1);
      } else {
        XmlCreateChild ("knob", 0);
        Data8 = IfrOpCodeHeader->OpCode + KNOB_TYPE_CUTOFF;   // save the first byte of info as KnobType+0xD0 (if its not a One of)
        CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+(UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset), (VOID*)&Data8, 1);
      }
      KnobParamData->KnobValMapCurrOff++; // increment the KnobValMapCurrOff accordingly

      XmlAddAttributes ("type", "%a", "scalar");
      AddHiiOpcodeElement(IfrOpCodeHeader);
      if (DupKnobInst) {
        XmlAddAttributes("name", "%a_inst_%d", KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ], (KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed + 1));
      } else {
        XmlAddAttributes("name", "%a", KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ]);
      }
      XmlAddAttributes("varstoreIndex", "%02d", VarstoreIndex);
      if (AsciiStrCmp((CHAR8*)EngStringHash[ ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt ] , NullString) != 0) {
        XmlAddAttributes("prompt", "%a", HandleSpecialXmlChars(EngStringHash[ ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt ]));
      } else { // use the name as prompt (this maybe due to the fact that BIOS Functional area owner missed adding eng prompt string for this knob, shouldn't affect the operation if this is mising)
        XmlAddAttributes("prompt", "%a", KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ]);
      }
      XmlAddAttributes ("description", "%a", HandleSpecialXmlChars(EngStringHash[ ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Help ]));
      if (BitKnobFound) {
        XmlAddAttributes("size", "%02d", BitSize);
        XmlAddAttributes("offset", "0x%05X", 0xC0000 + (KnobParamData->VarstoreTable[VarstoreIndex].BinKnobOffset[VarOffset] * 8) + BitOffset);
        KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize = (UINT8)VarLength;
        KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].IsBitWise = 1;
      } else {
        VarLength = GetOpcodeEntryLength(IfrOpCodeHeader);
        KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize = (UINT8)VarLength;
        XmlAddAttributes("size", "%02d", VarLength);
        XmlAddAttributes("offset", "0x%04X", VarOffset);
      }
      if ((DepexPtr != NULL) && ((dupfound == 0) || (IfrOpCodeHeader->OpCode != EFI_IFR_ONE_OF_OP))) {
        XmlAddAttributes("depex", "%a", DepexPtr);
      } else {
        XmlAddAttributes("depex", "%a", "TRUE");
      }
      if (KnobParamData->ProcessSetupPgPtr) {
        UINT16  CurrentFormId, PrevFormId, counter;
        BOOLEAN ValidParent;

        CurrentFormId = KnobParamData->CurrentFormId;
        SetupPgPtr = (CHAR8*)((UINTN)KnobParamData->CurentPrompt + ((sizeof(CHAR8*)*MAX_FORMID) - (UNITS_512)));
        TempChar8Ptr = (CHAR8*)((UINTN)KnobParamData->CurentPrompt + ((sizeof(CHAR8*)*MAX_FORMID) - (UNITS_512)*2));
        ZeroMem (SetupPgPtr, UNITS_512);
        if (AsciiStrCmp((CHAR8*)EngStringHash[ ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt ] , NullString) != 0) {
          CopyMem ((VOID*)SetupPgPtr, (VOID*)EngStringHash[((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt], AsciiStrLen(EngStringHash[((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt]));
        } else { // use the name as prompt (this maybe due to the fact that BIOS Functional area owner missed adding eng prompt string for this knob, shouldnt affect the operation if this is mising)
          CopyMem ((VOID*)SetupPgPtr, (VOID*)"??\0", 3);
        }
        //DEBUG ((DEBUG_INFO, "\t\t\t%a\n", EngStringHash[((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt]));
        ValidParent = FALSE;
        while (1) {
          //DEBUG ((DEBUG_INFO, "SetupPgPtr = %a\n", SetupPgPtr));
          ZeroMem (TempChar8Ptr, UNITS_512);
          CopyMem ((VOID*)TempChar8Ptr, (VOID*)KnobParamData->CurentPrompt[CurrentFormId], AsciiStrLen(KnobParamData->CurentPrompt[CurrentFormId]));
          AsciiStrCatS(TempChar8Ptr, UNITS_512, "/");
          AsciiStrCatS(TempChar8Ptr, UNITS_512, SetupPgPtr);
          AsciiStrCpyS(SetupPgPtr, UNITS_512, TempChar8Ptr);
          PrevFormId = CurrentFormId;
          CurrentFormId = KnobParamData->ParentFormId[PrevFormId];
          if (CurrentFormId == 0) {
            for (counter = 0; counter < MAX_FRONT_PAGE_FORM; counter++) {
              if (PrevFormId == FrontPageFormId[counter]) {
                ValidParent = TRUE;
                break;
              }
              if (FrontPageFormId[counter] == 0) {
                break;
              }
            }
            if (ValidParent) {
              break;
            }
            ZeroMem (TempChar8Ptr, 6);
            *(UINT32*)(UINTN)(TempChar8Ptr) = ASCII_ORPHANED_SETUP_PAGE;  // equivalent of "???/", this indicates BIOS option associated with Orphaned Setup Page
            AsciiStrCatS(TempChar8Ptr, UNITS_512, SetupPgPtr);
            AsciiStrCpyS(SetupPgPtr, UNITS_512, TempChar8Ptr);
            break;
          }
        }
        XmlAddAttributes ("SetupPgPtr", "%a", HandleSpecialXmlChars(SetupPgPtr));
      }

      CurrentVal = 0;
      DefaultVal = 0;
      if (IfrOpCodeHeader->OpCode == EFI_IFR_STRING_OP) {
        UINT8 TmpVal[UNITS_256];
        CHAR8 TmpStrVal[UNITS_512];
        ZeroMem (TmpVal, sizeof(TmpVal));
        CopyMem ((VOID*)(UINT8*)&TmpVal, (VOID*)((UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DefData + VarOffset), VarLength);
        ZeroMem (TmpStrVal, sizeof(TmpStrVal));
        ConvHexArray2asciiVal(TmpStrVal, TmpVal, VarLength);
        XmlAddAttributes ("default", "0x%a", (CHAR8*)TmpStrVal);
        ZeroMem (TmpVal, sizeof(TmpVal));
        CopyMem ((VOID*)(UINT8*)&TmpVal, (VOID*)((UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].Data + VarOffset), VarLength);
        ZeroMem (TmpStrVal, sizeof(TmpStrVal));
        ConvHexArray2asciiVal(TmpStrVal, TmpVal, VarLength);
        XmlAddAttributes ("CurrentVal", "0x%a", (CHAR8*)TmpStrVal);
      } else {
        if (BitKnobFound) {
          CopyMem ((VOID*)&CurrentVal, (VOID*)((UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].Data + KnobParamData->VarstoreTable[VarstoreIndex].BinKnobOffset[VarOffset]), VarLength);
          CopyMem ((VOID*)&DefaultVal, (VOID*)((UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DefData + KnobParamData->VarstoreTable[VarstoreIndex].BinKnobOffset[VarOffset]), VarLength);
          CurrentVal = BitExtractValue64(CurrentVal, BitOffset, BitSize);
          DefaultVal = BitExtractValue64(DefaultVal, BitOffset, BitSize);
        } else {
          CopyMem ((VOID*)&CurrentVal, (VOID*)((UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].Data + VarOffset), VarLength);
          CopyMem ((VOID*)&DefaultVal, (VOID*)((UINT8*)KnobParamData->VarstoreTable[ VarstoreIndex ].DefData + VarOffset), VarLength);
        }
        ZeroMem ((UINT8*)KnobValStr, sizeof(KnobValStr));
        ConvHexArray2asciiVal(KnobValStr, (UINT8*)&DefaultVal, VarLength);
        XmlAddAttributes ("default", "0x%a", KnobValStr);
        ZeroMem ((UINT8*)KnobValStr, sizeof(KnobValStr));
        ConvHexArray2asciiVal(KnobValStr, (UINT8*)&CurrentVal, VarLength);
        XmlAddAttributes ("CurrentVal", "0x%a", KnobValStr);
      }
      if (IfrOpCodeHeader->OpCode == EFI_IFR_CHECKBOX_OP) {
        XmlCloseChild ();
      }

      if (IfrOpCodeHeader->OpCode == EFI_IFR_NUMERIC_OP) {
        if (BitKnobFound) {
          Min  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u32.MinValue;
          Max  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u32.MaxValue;
          Step = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u32.Step;
        } else {
          switch (VarLength) {
          case 1:
            Min  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u8.MinValue;
            Max  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u8.MaxValue;
            Step = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u8.Step;
          break;
          case 2:
            Min  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u16.MinValue;
            Max  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u16.MaxValue;
            Step = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u16.Step;
          break;
          case 4:
            Min  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u32.MinValue;
            Max  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u32.MaxValue;
            Step = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u32.Step;
          break;
          case 8:
            Min  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u64.MinValue;
            Max  = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u64.MaxValue;
            Step = ((EFI_IFR_NUMERIC *)IfrOpCodeHeader)->data.u64.Step;
          break;
          default:
            ASSERT(0);
          }
        }
        XmlAddAttributes ("min", "0x%lX", Min);
        XmlAddAttributes ("max", "0x%lX", Max);
        XmlAddAttributes ("step", "0x%lX", Step);
        XmlCloseChild ();
        if (DupKnobInst) {
          if ((Data32 != 0) && ((UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize != 0)) {
            CopyMem((VOID*)&Data8, (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32), 1);
            if ((Data8 >= KNOB_TYPE_CUTOFF) && (IfrOpCodeHeader->OpCode == (Data8-KNOB_TYPE_CUTOFF))) {
              Data64 = 0;
              CopyMem((VOID*)(UINTN)(&Data64), (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32+1),(UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize);
              if (Data64 < Min) {
                Min = Data64;
              }
              Data64 = 0;
              CopyMem((VOID*)(UINTN)(&Data64), (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32+1+(UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize),(UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize);
              if (Data64 > Max) {
                Max = Data64;
              }
            }
          }
        }
        CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)&Min, VarLength);
        KnobParamData->KnobValMapCurrOff += (UINT8)VarLength;
        CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)&Max, VarLength);
        KnobParamData->KnobValMapCurrOff += (UINT8)VarLength;
      }
      if (IfrOpCodeHeader->OpCode == EFI_IFR_STRING_OP) {
        Min = ((EFI_IFR_STRING*)IfrOpCodeHeader)->MinSize;
        Max = ((EFI_IFR_STRING*)IfrOpCodeHeader)->MaxSize;
        XmlAddAttributes ("minsize", "0x%lX", Min);
        XmlAddAttributes ("maxsize", "0x%lX", Max);
        XmlCloseChild ();
        if (DupKnobInst) {
          if ((Data32 != 0) && ((UINT8)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobSize != 0)) {
            CopyMem((VOID*)&Data8, (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32), 1);
            if ((Data8 >= KNOB_TYPE_CUTOFF) && (IfrOpCodeHeader->OpCode == (Data8-KNOB_TYPE_CUTOFF))) {
              Data64 = 0;
              CopyMem((VOID*)(UINTN)(&Data64), (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32+1),1);
              if (Data64 < Min) {
                Min = Data64;
              }
              Data64 = 0;
              CopyMem((VOID*)(UINTN)(&Data64), (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+Data32+2),1);
              if (Data64 > Max) {
                Max = Data64;
              }
            }
          }
        }
        CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)&Min, sizeof(UINT8));
        KnobParamData->KnobValMapCurrOff += (UINT8)sizeof(UINT8);
        CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)&Max, sizeof(UINT8));
        KnobParamData->KnobValMapCurrOff += (UINT8)sizeof(UINT8);
      }
      if (IfrOpCodeHeader->OpCode == EFI_IFR_ONE_OF_OP) {
        XmlCloseElement (0, 1);
        IfrOpCodeHeader = AddHiiOptionAttributes(IfrOpCodeHeader, EngStringHash, (UINT8*)&DupIndexLst, VarstoreIndex, DepexPtr, KnobParamData, KnobXmlEntryOffset, BitKnobFound);
        XmlCloseElement ("knob", 0);
      }
      KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].XmlEntrySize = (UINT16)(ReturnXmlPtrAddr()-EntryStartAddr);
      KnobParamData->VarstoreTable[ VarstoreIndex ].KnobInfo[ VarOffset ].KnobProcessed += 1;
    } else {
// <AS>      DEBUG ((DEBUG_INFO, "XML_CLI: Failed to add knob Prompt = %a Name = %a \n",
// <AS>                      EngStringHash[ ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt ],
// <AS>                      KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ]));

// <AS>      DEBUG ((DEBUG_INFO, "XML_CLI: prompt= %a", KnobParamData->VarstoreTable[ VarstoreIndex ].EngStringHash[ ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Prompt ]));
// <AS>      DEBUG ((DEBUG_INFO, "  Name= %a", KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ]));
// <AS>      DEBUG ((DEBUG_INFO, "  description= %a", EngStringHash[ ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.Header.Help ]));
// <AS>      DEBUG ((DEBUG_INFO, "  KnobParamData->VarstoreTable[ %d ].KnobName[ %x ] = 0x%x\n", VarstoreIndex, VarOffset, KnobParamData->VarstoreTable[ VarstoreIndex ].KnobName[ VarOffset ]));
    }
  }
  return(IfrOpCodeHeader);
}

/**
  Creates Hash for Hii String

  @param[in]     StringBlock    Pointer to Hii opcode header
  @param[in,out] AsciiStrBlock  Pointer* to String Hash

  @retval  CHAR8*
**/
CHAR8**
CreateHiiStringHash (
  IN EFI_HII_STRING_BLOCK  *StringBlock,
  IN OUT CHAR8             *AsciiStrBlock
  )
{
  BOOLEAN               EndStringBlock;
  BOOLEAN               MultipleStrings;
  UINT16                StringCount;
  UINT16                StringIndex;
  UINTN                 LastBlockSize;
  UINTN                 BufferSize;
  UINTN                 NumberOfHiiPackageStrings;
  EFI_STATUS            Status;
  EFI_STRING            StringText;
  EFI_HII_STRING_BLOCK  *HiiStringBlock;
  CHAR8                 **StringHash;

  HiiStringBlock            = NULL;
  StringHash                = NULL;
  NumberOfHiiPackageStrings = 1;
  LastBlockSize             = 0;
  EndStringBlock            = FALSE;
  MultipleStrings           = FALSE;
  BufferSize                = 0;

  HiiStringBlock  = StringBlock;
  EndStringBlock  = FALSE;
  MultipleStrings = FALSE;

  Status = gBS->AllocatePool(EfiBootServicesData, sizeof(CHAR8*)*MAX_HII_PACKAGE_STRINGS, (VOID**)&StringHash);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    SetMem64(StringHash, sizeof(CHAR8*)*MAX_HII_PACKAGE_STRINGS, (UINTN)NullString);
  }

  while (EndStringBlock == FALSE) {
    StringText = (EFI_STRING)(HiiStringBlock + 1);
    switch (HiiStringBlock->BlockType) {
      case EFI_HII_SIBT_END:
        EndStringBlock = TRUE;
        break;
      case EFI_HII_SIBT_STRING_SCSU:
        BufferSize = AsciiStrLen ((CHAR8*) StringText);
        AsciiStrCpyS (AsciiStrBlock, (BufferSize+1), (CHAR8*)StringText);
        StringHash[ NumberOfHiiPackageStrings++ ] = AsciiStrBlock;
        AsciiStrBlock = (CHAR8*)(UINTN)((UINTN)AsciiStrBlock + BufferSize + 1);
        ASSERT(NumberOfHiiPackageStrings < MAX_HII_PACKAGE_STRINGS);
        LastBlockSize = sizeof(EFI_HII_SIBT_STRING_SCSU_BLOCK) - sizeof(CHAR8) + BufferSize;
        break;
      case EFI_HII_SIBT_STRING_UCS2:
        BufferSize = StrLenUnaligned(StringText) + 1;
        UnicodeToAsciiUnaligned(StringText, AsciiStrBlock, BufferSize);
        StringHash[ NumberOfHiiPackageStrings++ ] = AsciiStrBlock;
        ASSERT(NumberOfHiiPackageStrings < MAX_HII_PACKAGE_STRINGS);
        AsciiStrBlock = (CHAR8*)(UINTN)((UINTN)AsciiStrBlock + BufferSize + 1);
        LastBlockSize = sizeof(EFI_HII_SIBT_STRING_UCS2_BLOCK) - sizeof(CHAR16) + BufferSize*2;
        break;
      case EFI_HII_SIBT_STRING_SCSU_FONT:
        LastBlockSize = sizeof(EFI_HII_SIBT_STRING_SCSU_FONT_BLOCK) - sizeof(CHAR8) + AsciiStrLen ((CHAR8*)StringText);
        break;
      case EFI_HII_SIBT_SKIP1:
        NumberOfHiiPackageStrings += (UINTN)(*(UINT8*)(StringText));   // its important to verify the skip block so that prompt is modified accordingly
        LastBlockSize = sizeof (EFI_HII_SIBT_SKIP1_BLOCK);
        break;
      case EFI_HII_SIBT_SKIP2:
        NumberOfHiiPackageStrings += (UINTN)(*(UINT16*)(StringText));   // its important to verify the skip block so that prompt is modified accordingly
        LastBlockSize = sizeof (EFI_HII_SIBT_SKIP2_BLOCK);
        break;
      case EFI_HII_SIBT_DUPLICATE:
        LastBlockSize = sizeof (EFI_HII_SIBT_DUPLICATE_BLOCK);
        break;
      case EFI_HII_SIBT_EXT1:
        LastBlockSize = sizeof (EFI_HII_SIBT_EXT1_BLOCK);
        break;
      case EFI_HII_SIBT_EXT2:
        LastBlockSize = sizeof (EFI_HII_SIBT_EXT2_BLOCK);
        break;
      case EFI_HII_SIBT_EXT4:
        LastBlockSize = sizeof (EFI_HII_SIBT_EXT4_BLOCK);
        break;
      case EFI_HII_SIBT_FONT:
        LastBlockSize = ((EFI_HII_SIBT_FONT_BLOCK *)HiiStringBlock)->Header.Length;
        break;
      case EFI_HII_SIBT_STRINGS_SCSU:
        StringCount = ((EFI_HII_SIBT_STRINGS_SCSU_BLOCK *)HiiStringBlock)->StringCount;
        StringText = (EFI_STRING)((UINTN)HiiStringBlock + sizeof(EFI_HII_SIBT_STRINGS_SCSU_BLOCK) - sizeof(CHAR8));
        for (StringIndex = 0; StringIndex < StringCount; StringIndex++) {
          StringText = StringText + AsciiStrLen ((CHAR8*)StringText);
        }
        HiiStringBlock = (EFI_HII_STRING_BLOCK *)(StringText);
        MultipleStrings = TRUE;
        break;
      case EFI_HII_SIBT_STRINGS_SCSU_FONT:
        StringCount = ((EFI_HII_SIBT_STRINGS_SCSU_FONT_BLOCK *)HiiStringBlock)->StringCount;
        StringText = (EFI_STRING)((UINTN)HiiStringBlock + sizeof(EFI_HII_SIBT_STRINGS_SCSU_FONT_BLOCK) - sizeof(CHAR8));
        for (StringIndex = 0; StringIndex < StringCount; StringIndex++) {
          StringText = StringText + AsciiStrLen ((CHAR8*)StringText);
        }
        HiiStringBlock = (EFI_HII_STRING_BLOCK *)(StringText);
        MultipleStrings = TRUE;
        break;
      case EFI_HII_SIBT_STRING_UCS2_FONT:
        LastBlockSize = sizeof(EFI_HII_SIBT_STRING_UCS2_FONT_BLOCK) - sizeof(CHAR16) + (StrLenUnaligned(StringText) + 1) * 2;
        break;
      case EFI_HII_SIBT_STRINGS_UCS2:
        StringCount = ((EFI_HII_SIBT_STRINGS_UCS2_BLOCK *)HiiStringBlock)->StringCount;
        StringText = (EFI_STRING)((UINTN)HiiStringBlock + sizeof(EFI_HII_SIBT_STRINGS_UCS2_BLOCK) - sizeof(CHAR16));
        for (StringIndex = 0; StringIndex < StringCount; StringIndex++) {
          StringText = StringText + (StrLenUnaligned (StringText) + 1) * 2;
        }
        HiiStringBlock = (EFI_HII_STRING_BLOCK *)(StringText);
        MultipleStrings = TRUE;
        break;
      case EFI_HII_SIBT_STRINGS_UCS2_FONT:
        StringCount = ((EFI_HII_SIBT_STRINGS_UCS2_FONT_BLOCK *)HiiStringBlock)->StringCount;
        StringText = (EFI_STRING)((UINTN)HiiStringBlock + sizeof(EFI_HII_SIBT_STRINGS_UCS2_FONT_BLOCK) - sizeof(CHAR16));
        for (StringIndex = 0; StringIndex < StringCount; StringIndex++) {
          StringText = StringText + (StrLenUnaligned (StringText) + 1) * 2;
        }
        HiiStringBlock = (EFI_HII_STRING_BLOCK *)(StringText);
        MultipleStrings = TRUE;
        break;
      default:
        // DEBUG ((DEBUG_INFO, "XML_CLI: CreateHiiStringHash::HiiStringBlock->BlockType = 0x%02x\n",HiiStringBlock->BlockType));
        ASSERT(0);
        break;
    }
    if (MultipleStrings == FALSE) {
      HiiStringBlock = (EFI_HII_STRING_BLOCK *)((UINTN)HiiStringBlock + LastBlockSize);
    }
  }

  return StringHash;
}

/**
  Publish setup knob elements to the XML
  Locating Hii Database and parsing it for Ifr and String Package
  With the help of Hii Database and BiosKnobsData bin file
  publishes knobs data to XmlCli DRAM Interface memory in XML format

  @param[in,out] KnobParamData  Knob Parameters

  @retval  VOID
**/
VOID
SetupKnobsElement (
  IN OUT KNOB_PARAM_DATA  *KnobParamData
  )
{
  BOOLEAN                        CaptureFormSet;
  UINT8                          KnobTypeSz;
  UINT8                          BitField;
  UINT8                          IdNumber;
  UINT8                          VarstoreIndex=0;
  UINT16                         KnobOffset;
  UINT16                         DupCount;
  UINT16                         ScopeLevel=0;
  UINT16                         FrontPageFormId[MAX_FRONT_PAGE_FORM];
  UINT32                         KnobXmlEntryOffset;
  UINT32                         count;
  UINTN                          BufferSize;
  UINTN                          VarIndex;
  UINTN                          NvarSize;
  UINTN                          IndexSize;
  EFI_GUID                       MyGuid;
  EFI_STATUS                     Status;
  EFI_PHYSICAL_ADDRESS           BufferAddress;
  CHAR8                          **EngStringHash=NULL;
  CHAR8                          *AsciiStrBlock=NULL;
  UINT8                          *BufferPtr;
  UINT8                          *BiosKnobsBin;
  UINT8                          *KnobsBuffPtrStart;
  EFI_HII_DATABASE_PROTOCOL      *HiiDatabase;
  EFI_HII_PACKAGE_EXPORT_HEADER  *TableHeader;
  EFI_HII_PACKAGE_LIST_HEADER    *HiiPackageListHeader;
  EFI_HII_PACKAGE_HEADER         *HiiPackageHeader;
  EFI_IFR_OP_HEADER              *IfrOpCodeHeader;
  EFI_IFR_VARSTORE               *IfrVarStore;
  EFI_IFR_VARSTORE_EFI           *IfrVarStoreEfi;
  EFI_HII_STRING_BLOCK           *StringBlock=NULL;
  KNOB_BIN_HDR                   *KnobBinHdr;
  VOID                           *Buffer;

  //Status = GetSectionFromAnyFv (&gBiosKnobsDataBinGuid, EFI_SECTION_RAW, 0, (VOID **) &BiosKnobsBin, &BufferSize)
  //if ((Status == EFI_SUCCESS) && (BiosKnobsBin != NULL)) { // BIOS Knobs Data Bin is present in FV
  BiosKnobsBin = (UINT8*)(UINTN)KnobParamData->BiosKnobsDataAddr;
  BufferSize = KnobParamData->BiosKnobsDataSize;
  if ((BufferSize != 0) && (BiosKnobsBin != NULL)) { // BIOS Knobs Data Bin is present in FV
    DEBUG ((DEBUG_INFO, "XML_CLI: BiosKnobsData BinAddr = 0x%X   BufferSize = 0x%X\n", BiosKnobsBin, BufferSize));
    KnobsBuffPtrStart = BiosKnobsBin;
    for (VarIndex=0; (KnobsBuffPtrStart + sizeof(KNOB_BIN_HDR) < (BiosKnobsBin+BufferSize));) {
      KnobBinHdr = (KNOB_BIN_HDR*)KnobsBuffPtrStart;
      // DEBUG ((DEBUG_INFO, "KnobBinHdr Sig = 0x%X VarId = %d KnobCnt = %d DupOff = 0x%X PktSz = 0x%X\n", KnobBinHdr->Hdr0.Signature, KnobBinHdr->Hdr0.VarId, KnobBinHdr->Hdr0.KnobCount, KnobBinHdr->Hdr1.DupKnobBufOff, KnobBinHdr->Hdr1.NvarPktSize));
      if (KnobBinHdr->Hdr0.Signature == NVRO_SIGNATURE) {    // compare Signature with "$NVRO"
        break;  // we are done parsing $NVAR
      }
      MyGuid = KnobBinHdr->NvarGuid;
      if ((KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE) && (AsciiStrCmp(KnobBinHdr->NvarName, KnobParamData->VarstoreTable[VarIndex].Name) == 0) && CompareGuid(&KnobParamData->VarstoreTable[VarIndex].Guid, &MyGuid)) {  // compare Signature with "$NVAR" and if it matched with VarStore Table.
        NvarSize = KnobBinHdr->NvarSize;
        if (NvarSize == 0) {  // If Size is 0 is still zero, skip this entry
          DEBUG ((DEBUG_INFO, "XML_CLI: SetupKnobsElement - Size is zero for %a , skip this entry \n", KnobParamData->VarstoreTable[VarIndex].Name));
          KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;  // Skip this entry
          VarIndex++;
          continue;
        }
        for (KnobXmlEntryOffset=0, count=0; count < VarIndex; count++) {
          KnobXmlEntryOffset += ((UINT16)KnobParamData->VarstoreTable[count].Size + 1 + KnobParamData->VarstoreTable[count].BitKnobCount);
        }
        KnobParamData->VarstoreTable[VarIndex].KnobXmlEntryBase = KnobXmlEntryOffset;
        IndexSize = NvarSize + 1;
        if (KnobBinHdr->Hdr1.HdrRevision >= KNOB_HEADER_REVISION_4) {  // BitWise support was added from revision 4 onwards
          IndexSize = IndexSize + KnobBinHdr->BitKnobCount;
        }

        MyAllocateZeroPool(EfiBootServicesData, IndexSize*sizeof(UINT8*),  (VOID**)&KnobParamData->VarstoreTable[VarIndex].KnobName);
        MyAllocateZeroPool(EfiBootServicesData, IndexSize*sizeof(UINT8*),  (VOID**)&KnobParamData->VarstoreTable[VarIndex].ExpressionHash);
        MyAllocateZeroPool(EfiBootServicesData, IndexSize*sizeof(UINT16),  (VOID**)&KnobParamData->VarstoreTable[VarIndex].BinKnobOffset);
        MyAllocateZeroPool(EfiBootServicesData, IndexSize,                 (VOID**)&KnobParamData->VarstoreTable[VarIndex].BinKnobType);
        MyAllocateZeroPool(EfiBootServicesData, IndexSize,                 (VOID**)&KnobParamData->VarstoreTable[VarIndex].BinKnobSize);
        MyAllocateZeroPool(EfiBootServicesData, IndexSize,                 (VOID**)&KnobParamData->VarstoreTable[VarIndex].KnobInfo);
        MyAllocateZeroPool(EfiBootServicesData, (MAX_DUP_KNOBS_SUPPORTED*sizeof(DUP_KNOB_DETAILS)), (VOID**)&KnobParamData->VarstoreTable[VarIndex].DupKnobDetails);
        KnobOffset = 0;
        IndexSize = NvarSize;
        for (BufferPtr=(KnobsBuffPtrStart+sizeof(KNOB_BIN_HDR)); ((BufferPtr < (KnobsBuffPtrStart+KnobBinHdr->Hdr1.DupKnobBufOff)) && (BufferPtr < (BiosKnobsBin+BufferSize)));) {
          CopyMem (&KnobOffset, BufferPtr, 2);
          BufferPtr = BufferPtr + 2;    // Additionally ignore 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0])
          CopyMem (&KnobTypeSz, BufferPtr, 1);
          BufferPtr = BufferPtr + 1;    // 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0])
          if ((KnobTypeSz & 0x80)) {  // EFI_IFR_STRING_OP
            KnobParamData->VarstoreTable[VarIndex].BinKnobType[KnobOffset] = EFI_IFR_STRING_OP;
            KnobParamData->VarstoreTable[VarIndex].BinKnobSize[KnobOffset] = (KnobTypeSz & 0x7F) * 2;
          } else {
            if ((KnobTypeSz & 0xF) >= 0xC) { // this indicates that the given Knob is Bitwise and is of Size mentioned in subsequent fields
              KnobParamData->VarstoreTable[VarIndex].KnobInfo[KnobOffset].BitWise = 1;
              IndexSize = IndexSize + 1;
              if(KnobParamData->VarstoreTable[VarIndex].BinKnobOffset[KnobOffset] == 0){  // this is to indicate that for given offset,this is where the Bitwise knobs index starts from
                KnobParamData->VarstoreTable[VarIndex].BinKnobOffset[KnobOffset] = (UINT16)IndexSize;
              }
              CopyMem (&BitField, BufferPtr, 1);
              BufferPtr = BufferPtr + 1;    // 1 byte of data Bitsize[7:3] BitOffset[2:0]
              KnobParamData->VarstoreTable[VarIndex].BinKnobOffset[IndexSize] = KnobOffset;

              KnobXmlEntryOffset = KnobParamData->VarstoreTable[VarIndex].KnobXmlEntryBase + KnobOffset;
              if( ((KNOB_ENTRY_TBL_BITWISE*)KnobParamData->KnobXmlEntry)[KnobXmlEntryOffset].StartIndex == 0) {
                ((KNOB_ENTRY_TBL_BITWISE*)KnobParamData->KnobXmlEntry)[KnobXmlEntryOffset].StartIndex = IndexSize;
              }
              ((KNOB_ENTRY_TBL_BITWISE*)KnobParamData->KnobXmlEntry)[KnobXmlEntryOffset].EndIndex = IndexSize;
              ((KNOB_ENTRY_TBL_BITWISE*)KnobParamData->KnobXmlEntry)[KnobXmlEntryOffset].BitWiseKnobCount += 1;

              KnobOffset = (UINT16)IndexSize;
              KnobParamData->VarstoreTable[VarIndex].KnobInfo[KnobOffset].BitWise = 1;
              KnobParamData->VarstoreTable[VarIndex].BinKnobSize[KnobOffset] = BitField;
            } else {
              KnobParamData->VarstoreTable[VarIndex].BinKnobSize[KnobOffset] = (KnobTypeSz & 0xF);
            }
            if (KnobBinHdr->Hdr1.HdrRevision >= 3) {
              if ((KnobTypeSz >> 4) < 0x4) {  // This indicates that current Knob entry is part of Depex, Adjust the Type accordingly and re-insert the info
                KnobTypeSz = (KnobTypeSz + 0x40);
                KnobParamData->VarstoreTable[VarIndex].KnobInfo[KnobOffset].PartOfDepex = 1;
              }
            }
            KnobParamData->VarstoreTable[VarIndex].BinKnobType[KnobOffset] = (KnobTypeSz >> 4);
          }
          if (*BufferPtr) {
            KnobParamData->VarstoreTable[VarIndex].KnobName[KnobOffset] = BufferPtr;
          }
          while (*BufferPtr) {
            BufferPtr++;
          }
          BufferPtr++;

          ///
          /// Knob with suppression has one byte equal to NULL, and without suppression, two bytes
          ///
          if (*BufferPtr) { // Suppression rule
            KnobParamData->VarstoreTable[VarIndex].ExpressionHash[KnobOffset] = BufferPtr;
          }
          while (*BufferPtr) {
            BufferPtr++;
          }
          BufferPtr++;
          // DEBUG ((DEBUG_INFO, "VarId = %d  KnobOff = 0x%04X KnobName = %a \n", VarIndex, KnobOffset, KnobParamData->VarstoreTable[VarIndex].KnobName[KnobOffset]);
        }
        for (DupCount = 0; ((BufferPtr < (KnobsBuffPtrStart+KnobBinHdr->Hdr1.NvarPktSize)) && (BufferPtr < (BiosKnobsBin+BufferSize))); DupCount++) {
          KnobParamData->VarstoreTable[VarIndex].DupKnobDetails[DupCount].DupKnobName = BufferPtr;
          while (*BufferPtr) {
            BufferPtr++;
          }
          BufferPtr++;
          if (*BufferPtr) {
            KnobParamData->VarstoreTable[VarIndex].DupKnobDetails[DupCount].DupKnobExp = BufferPtr;
          }
          while (*BufferPtr) {
            BufferPtr++;
          }
          BufferPtr++;
        }
        KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
        if (BufferPtr != KnobsBuffPtrStart) {
          DEBUG ((DEBUG_INFO, "XML_CLI: Parsing error for Nvar ID = %d  ExpectedBuffPtr = 0x%X   ActualBuffPtr = 0x%X\n", VarIndex, KnobsBuffPtrStart, BufferPtr));
        }
        VarIndex++;
      } else {
        KnobsBuffPtrStart = KnobsBuffPtrStart + sizeof(KNOB_BIN_HDR);
      }
    }
  } else { // The Build time Knobs binary data not found
    DEBUG ((DEBUG_INFO, "XML_CLI: BiosKnobsData Bin file not Found\n"));
    return;
  }
  ///
  /// Locate HII protocol
  ///
  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Call first time with zero buffer length. Should fail with EFI_BUFFER_TOO_SMALL.
  ///
  BufferSize = 0;
  Buffer     = NULL;
  Status     = HiiDatabase->ExportPackageLists (HiiDatabase, 0, &BufferSize, Buffer);
  ASSERT(Status == EFI_BUFFER_TOO_SMALL);

  ///
  /// Allocate buffer in Boot Service region to hold the HII database.
  ///
  BufferAddress = 0xFFFFFFFF;
  Status = gBS->AllocatePages (AllocateMaxAddress, EfiBootServicesData, EFI_SIZE_TO_PAGES(BufferSize + sizeof(EFI_HII_PACKAGE_EXPORT_HEADER)), &BufferAddress);
  ASSERT_EFI_ERROR (Status);

  Buffer = (VOID *)(UINTN)(BufferAddress + sizeof(EFI_HII_PACKAGE_EXPORT_HEADER));

  ///
  /// Export HII database into the buffer.
  ///
  Status = HiiDatabase->ExportPackageLists (HiiDatabase, 0, &BufferSize, Buffer);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "XML_CLI: HiiDbAddr = 0x%X  HiiDbSize = 0x%X \n", BufferAddress, BufferSize));
  //CopyMem ((VOID*)(UINTN)(KnobParamData->KnobValMapAddr-(BufferSize+0x1000)), (VOID*)(UINTN)BufferAddress, BufferSize);
  //DEBUG ((DEBUG_INFO, "XML_CLI: HiiDbAddr_XmlCliBuffer = 0x%X \n", (KnobParamData->KnobValMapAddr-(BufferSize+0x1000))));

  TableHeader = (EFI_HII_PACKAGE_EXPORT_HEADER *)(UINTN)BufferAddress;
  CopyMem (&TableHeader->HiiProtocolGuid, &gEfiHiiDatabaseProtocolGuid, sizeof(gEfiHiiDatabaseProtocolGuid));
  TableHeader->HiiDatabaseSize = (UINT32)BufferSize;

  HiiPackageListHeader = (EFI_HII_PACKAGE_LIST_HEADER *)(TableHeader + 1);
  ///
  /// Iterating over HII_PACKAGE_LIST
  ///
  MyAllocateZeroPool(EfiBootServicesData, UNITS_256*MAX_HII_PACKAGE_STRINGS, (VOID**)&AsciiStrBlock);
  KnobParamData->ParentFormId = NULL;
  KnobParamData->CurentPrompt = NULL;
  if (KnobParamData->ProcessSetupPgPtr) {
    UINT16 *ParentFormId=NULL;
    CHAR8  **CurentPrompt=NULL;
    MyAllocateZeroPool(EfiBootServicesData, sizeof(UINT16)*MAX_FORMID, (VOID**)&ParentFormId);
    MyAllocateZeroPool(EfiBootServicesData, sizeof(CHAR8*)*MAX_FORMID, (VOID**)&CurentPrompt);
    //DEBUG ((DEBUG_INFO, "XML_CLI: ParentFormId = 0x%X  CurentPrompt = 0x%X\n", ParentFormId, CurentPrompt));
    KnobParamData->ParentFormId = ParentFormId;
    KnobParamData->CurentPrompt = CurentPrompt;
  }

  CaptureFormSet = FALSE;
  IdNumber = 0;
  ZeroMem (FrontPageFormId, sizeof(FrontPageFormId));
  while ((UINTN)HiiPackageListHeader < (UINTN)(TableHeader + 1) + TableHeader->HiiDatabaseSize) {
    for (VarIndex=0;VarIndex<KnobParamData->NumberOfVarstores;VarIndex++) {
      KnobParamData->VarstoreTable[VarIndex].FoundInPackage = FALSE;
    }
    if (EngStringHash != NULL) {
      gBS->FreePool(EngStringHash);
      EngStringHash = NULL;
    }
    StringBlock = FindStringBlock(HiiDatabase, &HiiPackageListHeader->PackageListGuid, "en-US");
    if (StringBlock != NULL) {
      EngStringHash = CreateHiiStringHash(StringBlock, AsciiStrBlock);
    }
    HiiPackageHeader = (EFI_HII_PACKAGE_HEADER *)(HiiPackageListHeader + 1);
    while ((UINTN)HiiPackageHeader < (UINTN)HiiPackageListHeader + HiiPackageListHeader->PackageLength - sizeof(*HiiPackageListHeader)) {
      if (KnobParamData->ProcessSetupPgPtr) {
        ZeroMem (KnobParamData->ParentFormId, sizeof(UINT16)*MAX_HII_PACKAGE_STRINGS);
        ZeroMem (KnobParamData->CurentPrompt, sizeof(CHAR8*)*MAX_HII_PACKAGE_STRINGS);
      }
      if (HiiPackageHeader->Type == EFI_HII_PACKAGE_FORMS) {
        IfrOpCodeHeader = (EFI_IFR_OP_HEADER *)(HiiPackageHeader + 1);
        KnobParamData->BitWiseQuo = FALSE;
        while ((UINTN)IfrOpCodeHeader < (UINTN)HiiPackageHeader + HiiPackageHeader->Length) {
          switch (IfrOpCodeHeader->OpCode) {
          case EFI_IFR_SUPPRESS_IF_OP:
          case EFI_IFR_GRAY_OUT_IF_OP:
            ScopeLevel = 0;
            ScopeLevel = ScopeLevel + IfrOpCodeHeader->Scope;
            IfrOpCodeHeader = (EFI_IFR_OP_HEADER *)((UINTN)IfrOpCodeHeader + IfrOpCodeHeader->Length);
            if ((IfrOpCodeHeader->OpCode == EFI_IFR_TRUE_OP) && (IfrOpCodeHeader->Scope == 0)){
              while (ScopeLevel > 0) { // go till end of Suppress if TRUE, we need to skip all that stuff
                IfrOpCodeHeader = (EFI_IFR_OP_HEADER *)((UINTN)IfrOpCodeHeader + IfrOpCodeHeader->Length);
                ScopeLevel = ScopeLevel + IfrOpCodeHeader->Scope;
                if (IfrOpCodeHeader->OpCode == EFI_IFR_END_OP) {
                  ScopeLevel = ScopeLevel - 1;
                }
              }
            }
            break;
          case EFI_IFR_FORM_SET_OP:
            if (EngStringHash != NULL) {
              if ((AsciiStrCmp((CHAR8*)EngStringHash[(((EFI_IFR_FORM_SET *)IfrOpCodeHeader)->FormSetTitle)] , NullString) != 0) &&
                   (AsciiStrCmp((CHAR8*)EngStringHash[(((EFI_IFR_FORM_SET *)IfrOpCodeHeader)->FormSetTitle)] , " ") != 0)){
                XmlAddComment("Form Set: %a", HandleSpecialXmlChars(EngStringHash[((EFI_IFR_FORM_SET*)IfrOpCodeHeader)->FormSetTitle]));
                CaptureFormSet = TRUE;
                //DEBUG ((DEBUG_INFO, "FormSet: %a\n", EngStringHash[((EFI_IFR_FORM_SET *)IfrOpCodeHeader)->FormSetTitle]));
              }
            }
            break;
          case EFI_IFR_FORM_OP:
            if (EngStringHash != NULL) {
              if ((AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_FORM *)IfrOpCodeHeader)->FormTitle] , NullString) != 0) &&
                   (AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_FORM *)IfrOpCodeHeader)->FormTitle] , " ") != 0)) {
                if (KnobParamData->ProcessSetupPgPtr) {
                  KnobParamData->CurrentFormId = ((EFI_IFR_FORM *)IfrOpCodeHeader)->FormId;
                  if (KnobParamData->ParentFormId[KnobParamData->CurrentFormId] == 0) {
                    KnobParamData->CurentPrompt[KnobParamData->CurrentFormId] = (CHAR8*)EngStringHash[(((EFI_IFR_FORM *)IfrOpCodeHeader)->FormTitle)];
                  }
                  //DEBUG ((DEBUG_INFO, "%a  FormId=0x%X  \n", EngStringHash[((EFI_IFR_FORM *)IfrOpCodeHeader)->FormTitle], KnobParamData->CurrentFormId));
                }
                XmlAddComment("Form: %a", HandleSpecialXmlChars(EngStringHash[((EFI_IFR_FORM*)IfrOpCodeHeader)->FormTitle]));
                if (CaptureFormSet) {
                  if (IdNumber < MAX_FRONT_PAGE_FORM) {
                    FrontPageFormId[IdNumber] = ((EFI_IFR_FORM *)IfrOpCodeHeader)->FormId;
                  }
                  IdNumber++;
                  CaptureFormSet = FALSE;
                }
              }
            }
            break;
          case EFI_IFR_SUBTITLE_OP:
            if (EngStringHash != NULL) {
              if ((AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_SUBTITLE *)IfrOpCodeHeader)->Statement.Prompt] , NullString) != 0) &&
                   (AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_SUBTITLE *)IfrOpCodeHeader)->Statement.Prompt] , " ") != 0)) {
                XmlAddComment("%a", HandleSpecialXmlChars(EngStringHash[((EFI_IFR_SUBTITLE *)IfrOpCodeHeader)->Statement.Prompt]));
              }
            }
            break;
          case EFI_IFR_TEXT_OP:
            if (EngStringHash != NULL) {
              if ((AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_TEXT *)IfrOpCodeHeader)->Statement.Prompt] , NullString) != 0) &&
                   (AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_TEXT *)IfrOpCodeHeader)->Statement.Prompt] , " ") != 0)) {
                XmlAddComment("%a: %a", HandleSpecialXmlChars(EngStringHash[ ((EFI_IFR_TEXT *)IfrOpCodeHeader)->Statement.Prompt]), HandleSpecialXmlChars(EngStringHash[((EFI_IFR_TEXT *)IfrOpCodeHeader)->TextTwo]));
              }
            }
            break;
          case EFI_IFR_REF_OP:
            if ((EngStringHash != NULL) && (KnobParamData->ProcessSetupPgPtr)) {
              if ((AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_REF*)IfrOpCodeHeader)->Question.Header.Prompt] , NullString) != 0) &&
                   (AsciiStrCmp((CHAR8*)EngStringHash[((EFI_IFR_REF*)IfrOpCodeHeader)->Question.Header.Prompt] , " ") != 0)) {
                KnobParamData->ParentFormId[((EFI_IFR_REF*)IfrOpCodeHeader)->FormId] = KnobParamData->CurrentFormId;
                KnobParamData->CurentPrompt[((EFI_IFR_REF*)IfrOpCodeHeader)->FormId] = (CHAR8*)EngStringHash[((EFI_IFR_REF*)IfrOpCodeHeader)->Question.Header.Prompt];
                //DEBUG ((DEBUG_INFO, "\t\t%a  FormId=0x%X  ParentFormId=0x%X\n", KnobParamData->CurentPrompt[((EFI_IFR_REF*)IfrOpCodeHeader)->FormId],((EFI_IFR_REF*)IfrOpCodeHeader)->FormId, KnobParamData->CurrentFormId));
              }
            }
            break;
          case EFI_IFR_END_OP:
            if (KnobParamData->BitWiseQuo) {
              KnobParamData->BitWiseQuo = FALSE;
            }
            break;
          case EFI_IFR_GUID_OP:
            if (CompareGuid ((EFI_GUID *)((UINT8 *)IfrOpCodeHeader + sizeof (EFI_IFR_OP_HEADER)), &gEdkiiIfrBitVarstoreGuid)) {
              KnobParamData->BitWiseQuo = TRUE;
            }
            break;
          case EFI_IFR_VARSTORE_OP:
            IfrVarStore = (EFI_IFR_VARSTORE *)IfrOpCodeHeader;
            MyGuid = IfrVarStore->Guid;
            TestAndSaveVarstore(KnobParamData, IfrVarStore->Name, &MyGuid, IfrVarStore->Size, IfrVarStore->VarStoreId);
            break;
          case EFI_IFR_VARSTORE_EFI_OP:
            IfrVarStoreEfi = (EFI_IFR_VARSTORE_EFI *)IfrOpCodeHeader;
            MyGuid = IfrVarStoreEfi->Guid;
            TestAndSaveVarstore(KnobParamData, IfrVarStoreEfi->Name, &MyGuid, IfrVarStoreEfi->Size, IfrVarStoreEfi->VarStoreId);
            break;
          case EFI_IFR_ONE_OF_OP:
          case EFI_IFR_CHECKBOX_OP:
          case EFI_IFR_NUMERIC_OP:
          case EFI_IFR_STRING_OP:
            if (TestVarstoreId(KnobParamData, ((EFI_IFR_ONE_OF *)IfrOpCodeHeader)->Question.VarStoreId, &VarstoreIndex)) {
              if (EngStringHash != NULL) {
                  IfrOpCodeHeader = AddKnobElement(IfrOpCodeHeader, KnobParamData, EngStringHash, VarstoreIndex, FrontPageFormId);
              }
            }
            break;
          default:
            break;
          }
          if ((UINTN)IfrOpCodeHeader != ((UINTN)HiiPackageHeader + HiiPackageHeader->Length)) {
            IfrOpCodeHeader = (EFI_IFR_OP_HEADER *)((UINTN)IfrOpCodeHeader + IfrOpCodeHeader->Length);
          }
        }
      }

      ///
      /// Searching for next Form
      ///
      HiiPackageHeader = (EFI_HII_PACKAGE_HEADER *)((UINTN)HiiPackageHeader + HiiPackageHeader->Length);
    }
    HiiPackageListHeader = (EFI_HII_PACKAGE_LIST_HEADER *)((UINTN)HiiPackageListHeader + HiiPackageListHeader->PackageLength);
  }
  Status = gBS->FreePages (BufferAddress, EFI_SIZE_TO_PAGES(BufferSize + sizeof(EFI_HII_PACKAGE_EXPORT_HEADER)));
  ASSERT_EFI_ERROR (Status);
  BufferAddress = 0xFFFFFFFF;

  if (AsciiStrBlock != NULL) {
    gBS->FreePool(AsciiStrBlock);
    AsciiStrBlock = NULL;
  }
  if (EngStringHash != NULL) {
    gBS->FreePool(EngStringHash);
    EngStringHash = NULL;
  }

  if (KnobParamData->ProcessSetupPgPtr) {
    if (KnobParamData->ParentFormId != NULL) {
      gBS->FreePool(KnobParamData->ParentFormId);
      KnobParamData->ParentFormId = NULL;
    }
    if (KnobParamData->CurentPrompt != NULL) {
      gBS->FreePool(KnobParamData->CurentPrompt);
      KnobParamData->CurentPrompt = NULL;
    }
  }
}

/**
  Expose the knobs BootOrder

  @param[in,out] KnobParamData  Knob Parameter Data

  @retval  VOID
**/
VOID
GenerateMiscKnobs (
  IN OUT KNOB_PARAM_DATA  *KnobParamData
  )
{
  UINT16      CurVal;
  UINT16      Count;
  UINT16      option;
  UINT16      Index;
  UINT16      VarOffset;
  CHAR16      OptionName[sizeof("Boot####")];
  UINT32      SetupDataAttributes=0;
  UINT32      KnobXmlEntryOffset=0;
  UINT32      EntryStartAddr;
  UINT32      OptionEntryAddr=0;
  UINT32      XmlOptionsSize=0;
  UINTN       Size;
  UINTN       VarSize;
  EFI_STATUS  Status;
  VOID        *VarDataPtr=NULL;
  KnobXmlEntryOffset = 0;
  for (Index=0; Index < KnobParamData->NumberOfVarstores; Index++) {
    //DEBUG ((DEBUG_INFO, "VarId = %d VarName = %a\n", Index, KnobParamData->VarstoreTable[Index].Name));
    //DEBUG ((DEBUG_INFO, "KnobXmlEntryOff = 0x%X KnobValMapAddr = 0x%X KnobValMapCurrOff = 0x%X\n", KnobXmlEntryOffset, KnobParamData->KnobValMapAddr, KnobParamData->KnobValMapCurrOff));
    if ((Index != 0) && (KnobParamData->VarstoreTable[Index].KnobXmlEntryBase == 0)) {
      KnobParamData->VarstoreTable[Index].KnobXmlEntryBase = KnobParamData->VarstoreTable[Index-1].KnobXmlEntryBase + KnobParamData->VarstoreTable[Index-1].Size + KnobParamData->VarstoreTable[Index-1].BitKnobCount + 1;
    }
    if (KnobParamData->VarstoreTable[Index].Size == 0) {
      continue;
    }
    if ((AsciiStrCmp("BootOrder", (CHAR8 *)(KnobParamData->VarstoreTable[Index].Name)) == 0) && CompareGuid(&KnobParamData->VarstoreTable[Index].Guid, &gEfiGlobalVariableGuid)) {
      Size = KnobParamData->VarstoreTable[Index].Size;
      VarDataPtr = (VOID*)(UINTN)KnobParamData->CurrGetSetVarBuffer;
      for (Count=0; Count<(Size/2); Count++) {
        VarOffset = (Count*2);
        KnobXmlEntryOffset = KnobParamData->VarstoreTable[Index].KnobXmlEntryBase;
        EntryStartAddr = ReturnXmlPtrAddr();
        KnobParamData->KnobXmlEntry[KnobXmlEntryOffset+VarOffset].KnobXmlEntryOfst = (UINT32)(EntryStartAddr - (KnobParamData->GbtXmlAddress+4));
        CopyMem((VOID*)&CurVal, (VOID*)(UINTN)((UINT8*)KnobParamData->VarstoreTable[Index].Data+VarOffset), 2);
        KnobParamData->KnobXmlEntry[KnobXmlEntryOffset+VarOffset].KnobValMapOffset = (UINT32)KnobParamData->KnobValMapCurrOff;
        KnobParamData->KnobXmlEntry[KnobXmlEntryOffset+VarOffset].KnobSize = 2;
        KnobParamData->KnobValMapCurrOff++; // increment the KnobValMapCurrOff accordingly
        XmlCreateElement ("knob ", 1);
        XmlAddAttributes ("type", "%a", "scalar");
        XmlAddAttributes ("setupType", "%a", "oneof");
        XmlAddAttributes ("name", "BootOrder_%d", Count);
        XmlAddAttributes ("varstoreIndex", "%02d", Index);
        XmlAddAttributes ("prompt", "Boot Order %d", Count);
        XmlAddAttributes ("description", "Boot Order Index %d", Count);
        XmlAddAttributes ("size", "%02d", 2);
        XmlAddAttributes ("offset", "0x%04X", VarOffset);
        XmlAddAttributes ("depex", "%a", "TRUE");
        XmlAddAttributes ("default", "0x%04X", CurVal);
        XmlAddAttributes ("CurrentVal", "0x%04X", CurVal);
        XmlCloseElement (0, 1);
        XmlCreateElement ("options", 0);
        if (Count == 0) {
          OptionEntryAddr = ReturnXmlPtrAddr();
          for (option=0; option<(Size/2); option++) {
            CopyMem((VOID*)&CurVal, (VOID*)(UINTN)((UINT8*)KnobParamData->VarstoreTable[Index].Data+(option*2)), 2);
            XmlCreateChild ("option", 0);
            UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04X", CurVal);
            VarSize = 0;
            Status = gRT->GetVariable (OptionName, &KnobParamData->VarstoreTable[Index].Guid, &SetupDataAttributes, (UINTN*)&VarSize, NULL);
            Status = gRT->GetVariable (OptionName, &KnobParamData->VarstoreTable[Index].Guid, &SetupDataAttributes, (UINTN*)&VarSize, VarDataPtr);
            ASSERT_EFI_ERROR (Status);
            XmlAddAttributes ("text", "%S", (CHAR16*)((UINT8*)VarDataPtr+6));
            XmlAddAttributes ("value", "0x%04X", CurVal);
            XmlCloseChild ();
            CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+KnobParamData->KnobValMapCurrOff), (VOID*)&CurVal,2);
            KnobParamData->KnobValMapCurrOff += 2;
          }
          CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+(UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset+VarOffset].KnobValMapOffset),(VOID*)&option, 1);
          XmlOptionsSize = ReturnXmlPtrAddr() - OptionEntryAddr;
        } else {
          XmlStrCpySz((CHAR8*)(UINTN)OptionEntryAddr, (UINTN)XmlOptionsSize);
          CopyMem((VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+(UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset+VarOffset].KnobValMapOffset),
                  (VOID*)(UINTN)((UINTN)KnobParamData->KnobValMapAddr+(UINT32)KnobParamData->KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset), (Size+1));
          KnobParamData->KnobValMapCurrOff += (UINT32)(Size+1);
        }
        XmlCloseElement ("options", 0);
        XmlCloseElement ("knob", 0);
        KnobParamData->KnobXmlEntry[KnobXmlEntryOffset+VarOffset].XmlEntrySize = (UINT16)(ReturnXmlPtrAddr()-EntryStartAddr);
      }
    }
  }
}

/**
  Generates Read Only Bios Knobs in XML from knob parameters

  @param[in] KnobParamData  Data for Knob Parameters

  @retval  VOID
**/
VOID
GenerateReadonlyKnobs (
  IN KNOB_PARAM_DATA  *KnobParamData
  )
{
  BOOLEAN       PublishThis=TRUE;
  BOOLEAN       BitWise;
  UINT8         BitField;
  UINT8         KnobTypeSz;
  UINT8         Data8;
  UINT8         VarIndex;
  CHAR16        UniCodeName[MAX_VARSTORE_NAME];
  UINT16        KnobOffset;
  UINT16        KnobIndex;
  UINT32        SetupDataAttributes=0;
  UINTN         BufferSize;
  UINTN         VarSize;
  UINTN         CurVal;
  EFI_STATUS    Status;
  EFI_GUID      VarGuid;
  UINT8         *BufferPtr;
  UINT8         *BiosKnobsBin;
  UINT8         *KnobsBuffPtrStart;
  KNOB_BIN_HDR  *KnobBinHdr;
  VOID          *DataPtr=NULL;
  CHAR8         KnobValStr[20];

  // Process Read only Knobs
  BiosKnobsBin = (UINT8*)(UINTN)KnobParamData->BiosKnobsDataAddr;
  BufferSize = KnobParamData->BiosKnobsDataSize;
  KnobsBuffPtrStart = BiosKnobsBin;
  KnobBinHdr = (KNOB_BIN_HDR*)KnobsBuffPtrStart;
  if ((BufferSize != 0) && (BiosKnobsBin != NULL) && (KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE)) {
    while (KnobsBuffPtrStart < (BiosKnobsBin+BufferSize)) {
      if (KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE) {    // compare Signature with "$NVAR"
        VarGuid = KnobBinHdr->NvarGuid;
        AsciiStrToUnicodeStrS (KnobBinHdr->NvarName, UniCodeName, sizeof (UniCodeName));
        for (VarIndex=0; VarIndex < KnobParamData->NumberOfVarstores; VarIndex++) {
          if ((AsciiStrCmp(KnobBinHdr->NvarName, KnobParamData->VarstoreTable[VarIndex].Name) == 0) && CompareGuid(&KnobParamData->VarstoreTable[VarIndex].Guid, &VarGuid)) {
            VarSize = KnobParamData->VarstoreTable[VarIndex].Size;
            if (VarSize == 0) {
              break;
            }
            KnobOffset = 0;
            for (BufferPtr=(KnobsBuffPtrStart+sizeof(KNOB_BIN_HDR)); ((BufferPtr < (KnobsBuffPtrStart+KnobBinHdr->Hdr1.DupKnobBufOff)) && (BufferPtr < (BiosKnobsBin+BufferSize)));) {
              CopyMem (&KnobOffset, BufferPtr, 2);
              BufferPtr = BufferPtr + 2;  // Additionally ignore 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0]), 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0])
              KnobIndex = KnobOffset;
              CopyMem (&KnobTypeSz, BufferPtr, 1);
              BufferPtr = BufferPtr + 1;
              BitWise = FALSE;
              if ((KnobTypeSz & 0x80) == 0) {  // applicable only for non String Type
                if ((KnobTypeSz & 0xF) >= 0xC) {  // this indicates that the given Knob is Bitwise and is of Size mentioned in subsequent fields
                  BitWise = TRUE;
                  CopyMem (&BitField, BufferPtr, 1);
                  BufferPtr = BufferPtr + 1;  // 1 byte of data Bitsize[7:3] BitOffset[2:0]
                  KnobIndex = BitFetchIndex((UINT32)(UINTN)&KnobParamData->KnobXmlEntry[KnobParamData->VarstoreTable[VarIndex].KnobXmlEntryBase], KnobParamData->KnobValMapAddr, KnobOffset, BitField);
                }
                if ((KnobParamData->VarstoreTable[VarIndex].KnobInfo[KnobIndex].PartOfDepex == 1) && (KnobParamData->VarstoreTable[VarIndex].KnobInfo[ KnobIndex ].KnobProcessed == 0)) {
                  // If this entry is part of Depex and If not published already in XML
                  Data8 = (UINT8)KnobParamData->VarstoreTable[VarIndex].BinKnobSize[KnobIndex];
                  if (Data8 != 0) {
                    XmlCreateChild("knob ", 0);
                    XmlAddAttributes("setupType", "%a", "ReadOnly");
                    XmlAddAttributes("name", "%a", BufferPtr);
                    XmlAddAttributes("varstoreIndex", "%02d", KnobBinHdr->Hdr0.VarId);
                    CurVal = 0;
                    CopyMem((VOID*)&CurVal, (VOID*)(UINTN)((UINT8*)KnobParamData->VarstoreTable[VarIndex].Data+KnobOffset), Data8);
                    if(BitWise) {
                      XmlAddAttributes("size", "%02d", ((BitField >> 3 ) & 0x3F));
                      XmlAddAttributes("offset", "0x%04X", (0xC0000 + (KnobOffset*8) + (BitField & 0x7)));
                      CurVal = BitExtractValue64(CurVal, (BitField & 0x7), ((BitField >> 3) & 0x3F));
                    } else {
                      XmlAddAttributes("size", "%02d", Data8);
                      XmlAddAttributes("offset", "0x%04X", KnobOffset);
                    }
                    ZeroMem ((UINT8*)KnobValStr, sizeof(KnobValStr));
                    ConvHexArray2asciiVal(KnobValStr, (UINT8*)&CurVal, Data8);
                    XmlAddAttributes("default", "0x%a", KnobValStr);
                    XmlAddAttributes("CurrentVal", "0x%a", KnobValStr);
                    XmlCloseChild();
                  }
                }
              }
              while (*BufferPtr) {
                BufferPtr++;
              }
              BufferPtr++;
              while (*BufferPtr) {
                BufferPtr++;
              }
              BufferPtr++;
            }
            break;
          }
        }
        KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
        KnobBinHdr = (KNOB_BIN_HDR*)KnobsBuffPtrStart;
      } else if (KnobBinHdr->Hdr0.Signature == NVRO_SIGNATURE) {  // compare Signature with "$NVRO"
        KnobOffset = 0;
        AsciiStrToUnicodeStrS(KnobBinHdr->NvarName, UniCodeName, sizeof (UniCodeName));
        VarSize = 0;
        VarGuid = KnobBinHdr->NvarGuid;
        Status = gRT->GetVariable (UniCodeName, &VarGuid, &SetupDataAttributes, (UINTN*)&VarSize, NULL);
        if (VarSize == 0) {
          if (KnobBinHdr->NvarSize == 0) {  // If Size is 0 is still zero, skip this entry
            DEBUG ((DEBUG_INFO, "XML_CLI: GenerateReadonlyKnobs - Size is zero for %a , skip this entry \n", KnobBinHdr->NvarName));
            KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
            KnobBinHdr = (KNOB_BIN_HDR*)KnobsBuffPtrStart;
            continue;
          } else {
            VarSize = KnobBinHdr->NvarSize;
            DataPtr = (VOID*)(UINTN)KnobParamData->CurrGetSetVarBuffer;
          }
        } else {
          DataPtr = (VOID*)(UINTN)KnobParamData->CurrGetSetVarBuffer;
          Status = gRT->GetVariable (UniCodeName, &VarGuid, &SetupDataAttributes, (UINTN*)&VarSize, DataPtr);
          if (EFI_ERROR(Status)) {
            KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
            KnobBinHdr = (KNOB_BIN_HDR*)KnobsBuffPtrStart;
            continue;
          }
        }
        for (BufferPtr=(KnobsBuffPtrStart+sizeof(KNOB_BIN_HDR)); ((BufferPtr < (KnobsBuffPtrStart+KnobBinHdr->Hdr1.DupKnobBufOff)) && (BufferPtr < (BiosKnobsBin+BufferSize)));) {
          CopyMem(&KnobOffset, BufferPtr, 2);
          BufferPtr = BufferPtr + 2;    // Additionally ignore 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0])
          CopyMem(&Data8, BufferPtr, 1);
          BufferPtr = BufferPtr + 1;    // 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0])
          VarIndex = (UINT8)KnobBinHdr->Hdr0.VarId;
          PublishThis = TRUE;
          BitWise = FALSE;
          KnobIndex = KnobOffset;
          if ((Data8 & 0x80) == 0) {  // applicable only for non String Type
            if ((Data8 & 0xF) >= 0xC) {  // this indicates that the given Knob is Bitwise and is of Size mentioned in subsequent fields
              BitWise = TRUE;
              CopyMem (&BitField, BufferPtr, 1);
              BufferPtr = BufferPtr + 1;  // 1 byte of data Bitsize[7:3] BitOffset[2:0]
              KnobIndex = BitFetchIndex((UINT32)(UINTN)&KnobParamData->KnobXmlEntry[KnobParamData->VarstoreTable[VarIndex].KnobXmlEntryBase], KnobParamData->KnobValMapAddr, KnobOffset, BitField);
              Data8 = BitGetByteSize((BitField & 0x7), ((BitField >> 3) & 0x3F));
            } else {
              Data8 = Data8 & 0xF;
            }
            if (VarIndex < KnobParamData->NumberOfVarstores) {  // this means it was part of Regular $NVAR.
              if ((KnobParamData->VarstoreTable[VarIndex].KnobInfo[ KnobIndex ].KnobProcessed > 0) || (Data8 == 0)) {
                PublishThis = FALSE;
              }
            }
            if (PublishThis) {
              XmlCreateChild("knob ", 0);
              XmlAddAttributes("setupType", "%a", "ReadOnly");
              XmlAddAttributes("name", "%a", BufferPtr);
              XmlAddAttributes("varstoreIndex", "%02d", VarIndex);
              CurVal = 0;
              CopyMem((VOID*)&CurVal, (VOID*)(UINTN)((UINT8*)DataPtr+KnobOffset), Data8);
              if(BitWise) {
                XmlAddAttributes("size", "%02d", ((BitField >> 3 ) & 0x3F));
                XmlAddAttributes("offset", "0x%04X", (0xC0000 + (KnobOffset*8) + (BitField & 0x7)));
                CurVal = BitExtractValue64(CurVal, (BitField & 0x7), ((BitField >> 3) & 0x3F));
              } else {
                XmlAddAttributes("size", "%02d", Data8);
                XmlAddAttributes("offset", "0x%04X", KnobOffset);
              }
              ZeroMem ((UINT8*)KnobValStr, sizeof(KnobValStr));
              if (Data8 < (sizeof(KnobValStr)/2)) {
                ConvHexArray2asciiVal(KnobValStr, (UINT8*)&CurVal, Data8);
              }
              XmlAddAttributes("default", "0x%a", KnobValStr);
              XmlAddAttributes("CurrentVal", "0x%a", KnobValStr);
              XmlCloseChild ();
            }
          }
          while (*BufferPtr) {
            BufferPtr++;
          }
          BufferPtr++;
          while (*BufferPtr) {
            BufferPtr++;
          }
          BufferPtr++;
        }
        KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
        KnobBinHdr = (KNOB_BIN_HDR*)KnobsBuffPtrStart;
      } else {
        KnobsBuffPtrStart = KnobsBuffPtrStart + sizeof(KNOB_BIN_HDR);
        KnobBinHdr = (KNOB_BIN_HDR*)KnobsBuffPtrStart;
      }
    }
  }
}

/**
  Generate BIOS knobs parameter from XmlCli Common Structure

  @param[in,out] XmlCliCommon  XmlCliCommon Structure

  @retval  VOID
**/
VOID
XmlGenerateBiosKnobs (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  )
{
  UINT32           KnobEntryListSize=0;
  UINT32           VarIndex;
  KNOB_PARAM_DATA  KnobParamData;

  XmlSaveDefVarstore(XmlCliCommon);
  for (VarIndex=0;VarIndex<XmlCliCommon->NumberOfVarstores;VarIndex++) {
    KnobEntryListSize += ((UINT16)XmlCliCommon->VarstoreTable[VarIndex].Size + 1 + XmlCliCommon->VarstoreTable[VarIndex].BitKnobCount);
  }
  DEBUG ((DEBUG_INFO, "XML_CLI: Total supported Setup Knobs Size = 0x%x\n", KnobEntryListSize));
  KnobEntryListSize = KnobEntryListSize * sizeof(KNOB_ENTRY_TBL);  // UINT32 + UINT16
  ASSERT(KnobEntryListSize < FixedPcdGet8(PcdKnobsEntriesSize));  // Ran out of allocated knobs debug info data size

  XmlCliCommon->SetupKnobsAddress   = (UINT32)ReturnXmlPtrAddr();
  KnobParamData.KnobXmlEntry        = (KNOB_ENTRY_TBL*)(UINTN)XmlCliCommon->KnobXmlEntryAddr;
  KnobParamData.GbtXmlAddress       = XmlCliCommon->GbtXmlAddress;
  KnobParamData.BiosKnobsDataAddr   = XmlCliCommon->BiosKnobsDataAddr;
  KnobParamData.BiosKnobsDataSize   = XmlCliCommon->BiosKnobsDataSize;
  KnobParamData.KnobValMapAddr      = XmlCliCommon->KnobValMapAddr;
  KnobParamData.KnobValMapCurrOff   = 0x4;  // start from offset 4
  KnobParamData.MergeDuplicateKnobs = XmlCliCommon->MergeDuplicateKnobs;
  KnobParamData.NumberOfVarstores   = XmlCliCommon->NumberOfVarstores;
  KnobParamData.NvroCount           = XmlCliCommon->NvroCount;
  KnobParamData.VarstoreTable       = XmlCliCommon->VarstoreTable;
  KnobParamData.ProcessSetupPgPtr   = XmlCliCommon->PublishSetupPgPtr;
  KnobParamData.CurrGetSetVarBuffer = XmlCliCommon->CurrGetSetVarBuffer;
  KnobParamData.XmlCliComm          = (VOID*)XmlCliCommon;

  XmlCreateElement ("biosknobs", 0);
  SetupKnobsElement(&KnobParamData);
  GenerateMiscKnobs(&KnobParamData);
  GenerateReadonlyKnobs(&KnobParamData);
  XmlCloseElement ("biosknobs", 0);

  for (VarIndex=0; VarIndex < KnobParamData.NumberOfVarstores; VarIndex++) {
    if (KnobParamData.VarstoreTable[VarIndex].KnobName != NULL) {
      gBS->FreePool(KnobParamData.VarstoreTable[VarIndex].KnobName);
      KnobParamData.VarstoreTable[VarIndex].KnobName = NULL;
    }
    if (KnobParamData.VarstoreTable[VarIndex].ExpressionHash != NULL) {
      gBS->FreePool(KnobParamData.VarstoreTable[VarIndex].ExpressionHash);
      KnobParamData.VarstoreTable[VarIndex].ExpressionHash = NULL;
    }
    if (KnobParamData.VarstoreTable[VarIndex].BinKnobOffset != NULL) {
      gBS->FreePool(KnobParamData.VarstoreTable[VarIndex].BinKnobOffset);
      KnobParamData.VarstoreTable[VarIndex].BinKnobOffset = NULL;
    }
    if (KnobParamData.VarstoreTable[VarIndex].BinKnobType != NULL) {
      gBS->FreePool(KnobParamData.VarstoreTable[VarIndex].BinKnobType);
      KnobParamData.VarstoreTable[VarIndex].BinKnobType = NULL;
    }
    if (KnobParamData.VarstoreTable[VarIndex].BinKnobSize != NULL) {
      gBS->FreePool(KnobParamData.VarstoreTable[VarIndex].BinKnobSize);
      KnobParamData.VarstoreTable[VarIndex].BinKnobSize = NULL;
    }
    if (KnobParamData.VarstoreTable[VarIndex].KnobInfo != NULL) {
      gBS->FreePool(KnobParamData.VarstoreTable[VarIndex].KnobInfo);
      KnobParamData.VarstoreTable[VarIndex].KnobInfo = NULL;
    }
    if (KnobParamData.VarstoreTable[VarIndex].DupKnobDetails != NULL) {
      gBS->FreePool(KnobParamData.VarstoreTable[VarIndex].DupKnobDetails);
      KnobParamData.VarstoreTable[VarIndex].DupKnobDetails = NULL;
    }
  }

  XmlCliCommon->SetupKnobsSize = (UINT32)(ReturnXmlPtrAddr() - XmlCliCommon->SetupKnobsAddress);
  SharedMemConstructSharedMB((VOID*)XmlCliCommon, DRAMSharedMailBoxTable, ITP_XDP_BAR_NO, ITP_XDP_VEND_DEV_ID, XmlCliCommon->DramSharedMBAddr);
}
