/** @file
  Xml Cli Common Dxe driver implementation.

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
#include <Protocol/LoadedImage.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/RngLib.h>
#include <Library/BaseCryptLib.h>
#include <Cli.h>


#pragma pack(push, 1)
typedef struct {
  UINT32  XmlCliTmpBuffPtr;
  UINT8   Digest[DIGEST_SIZE];
} DXE_PROTOCOL_PTR;

typedef
VOID
(*GEN_GBT_XML) (
  VOID  *XmlCliProto
  );

typedef struct _XML_CLI_PROTOCOL {
  // this (GenerateGbtXml) has to be the first entry always.
  GEN_GBT_XML     GenerateGbtXml;
  // there should not be any entry before the one above
  XML_CLI_COMMON  *XmlCliCommon;
  BOOLEAN         XmlCliStructInitialized;
  UINT32          KnobsSignature;
} XML_CLI_PROTOCOL;

#pragma pack(pop)



/**
  Cli Entry Point Exposed as a XmlCli API structure

  @param[in] XmlCliCom  XmlCli Common Structure

  @retval  EFI_SUCCESS
  @retval  EFI_INVALID_PARAMETER  Null pointer passed as parameter
  @retval  EFI_NO_RESPONSE        Signature in Structure is still not in READY state
  @retval  EXIT_OPCODE            Specified Operation code in Request buffer match to EXIT_OPCODE
**/
EFI_STATUS
CliEntry (
  IN VOID  *XmlCliCom
  );

/**
  Generate GBT Xml file at GbtXmlAddress

  @param[in] XmlCliProto  Pointer to XmlCli Protocol

  @retval  VOID
**/
VOID
GenerateGbtXml (
  IN VOID  *XmlCliProto
  );

/**
  This is the entrypoint of the XmlCliCommonHii driver.
  This initialize the XmlCli configuration form.

  @retval  EFI_SUCCESS           The XmlCli configuration form is initialized.
  @retval  EFI_OUT_OF_RESOURCES  Failed to allocate memory.
  @retval  Others                Other errors as indicated.
**/
EFI_STATUS
EFIAPI
InitXmlCliSetupEntry ();

DXE_PROTOCOL_PTR  DxeProtocolPtr;

SHARED_MAILBOX_TYPE DramMailBoxTable[] = {
  { LEG_MAILBOX_SIG,     LEG_MAILBOX_OFFSET,    LEG_MAILBOX_SIZE,   SHARED_MEMORY_FLAG_MEMORY_TYPE },
  { CLI_REQ_SIG,         CLI_REQ_BUFFER_OFFSET, CLI_BUFFER_SIZE,    SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { CLI_RES_SIG,         CLI_RES_BUFFER_OFFSET, CLI_BUFFER_SIZE,    SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { SHARED_MB_LAST_SIG,  SHARED_MB_LAST_SIG,    SHARED_MB_LAST_SIG, SHARED_MB_LAST_SIG}
};

SHARED_MAILBOX_TYPE DramMailBoxTableLite[] = {
  { LEG_MAILBOX_SIG,     LEG_MAILBOX_OFFSET,         LEG_MAILBOX_SIZE,     SHARED_MEMORY_FLAG_MEMORY_TYPE },
  { CLI_REQ_SIG,         CLI_REQ_BUFFER_OFFSET,      CLI_LITE_BUFFER_SIZE, SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { CLI_RES_SIG,         CLI_LITE_RES_BUFFER_OFFSET, CLI_LITE_BUFFER_SIZE, SHARED_MEMORY_FLAG_MEMORY_TYPE},
  { SHARED_MB_LAST_SIG,  SHARED_MB_LAST_SIG,         SHARED_MB_LAST_SIG,   SHARED_MB_LAST_SIG}
};


/**
  Find Address and size of FFS Region for given GUID

  @param[in]  ImageHandle    EFI_HANDLE image
  @param[in]  MyFfsGuid      Guid of FFS to lookup for
  @param[out] MyFfsDataAddr  Address of FFS
  @param[out] MyFfsDataSize  Size of FFS

  @retval  EFI_SUCCESS   FFS Found
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
FindMyFfs (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_GUID    *MyFfsGuid,
  OUT UINT32     *MyFfsDataAddr,
  OUT UINT32     *MyFfsDataSize
  )
{
  UINT32                              FfsSize;
  UINT32                              FFSDataOffset;
  EFI_PHYSICAL_ADDRESS                FvbBaseAddress;
  EFI_PHYSICAL_ADDRESS                FvMainPtr;
  EFI_PHYSICAL_ADDRESS                FvEndAddr;
  EFI_STATUS                          Status=EFI_NOT_FOUND;
  EFI_LOADED_IMAGE_PROTOCOL           *LoadedImage;
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb;

  Status = gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&LoadedImage);
  ASSERT_EFI_ERROR (Status);
  Status = gBS->HandleProtocol (LoadedImage->DeviceHandle, &gEfiFirmwareVolumeBlockProtocolGuid, (VOID **)&Fvb);
  Fvb->GetPhysicalAddress (Fvb, &FvbBaseAddress);

  if (((EFI_FIRMWARE_VOLUME_HEADER*)FvbBaseAddress)->FvLength == 0xFFFFFFFFFFFFFFFF) {
    return Status;
  }

  if (((EFI_FIRMWARE_VOLUME_HEADER*)FvbBaseAddress)->ExtHeaderOffset != 0) {
    FvMainPtr = FvbBaseAddress + ((EFI_FIRMWARE_VOLUME_HEADER*)FvbBaseAddress)->ExtHeaderOffset;
    FvMainPtr = FvMainPtr + ((EFI_FIRMWARE_VOLUME_EXT_HEADER*)FvMainPtr)->ExtHeaderSize;
  } else {
    FvMainPtr = FvbBaseAddress + ((EFI_FIRMWARE_VOLUME_HEADER*)FvbBaseAddress)->HeaderLength;
  }

  FvMainPtr = ((FvMainPtr + 7) & 0xFFFFFFF8);  // ffs always starts ar 8 byte aligned address.
  FvEndAddr = FvbBaseAddress + ((EFI_FIRMWARE_VOLUME_HEADER*)FvbBaseAddress)->FvLength;
  while(FvMainPtr < FvEndAddr) {
    if (IS_FFS_FILE2 (FvMainPtr)) {
      FfsSize = FFS_FILE2_SIZE (FvMainPtr) ;
      FFSDataOffset = sizeof(EFI_FFS_FILE_HEADER2) + sizeof(EFI_COMMON_SECTION_HEADER2);
    } else {
      FfsSize = FFS_FILE_SIZE (FvMainPtr) ;
      FFSDataOffset = sizeof(EFI_FFS_FILE_HEADER) + sizeof(EFI_COMMON_SECTION_HEADER);
    }
    if ((FfsSize == 0) || ((FfsSize & 0xFFFFFF) == 0xFFFFFF)) {
      break;
    }
    if (CompareGuid (&((EFI_FFS_FILE_HEADER*)FvMainPtr)->Name, MyFfsGuid)){
      *MyFfsDataAddr = (UINT32)(FvMainPtr + FFSDataOffset);
      *MyFfsDataSize = (UINT32)(FfsSize - FFSDataOffset);
      DEBUG ((DEBUG_INFO, "XML_CLI: Found Given FFS at Address = 0x%X  DataSize = 0x%X\n", *MyFfsDataAddr, *MyFfsDataSize));
      return EFI_SUCCESS;
    }
    FvMainPtr = ((FvMainPtr + FfsSize + 7) & 0xFFFFFFF8);  // ffs always starts ar 8 byte aligned address.
  }
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "XML_CLI[%a:%a]: FFS not found!!! Status=%r\n", gEfiCallerBaseName, __FUNCTION__, Status));
  return Status;
}

/**
  For XmlCli Lite Features allow to store Default values of All of the Nvar

  @param[in] DataPtr            Pointer to current Nvar data buffer.
  @param[in] BiosKnobsDataAddr  Address of Bios knobs Data bin.
  @param[in] BiosKnobsDataSize  Size of the Bios knobs Data bin.

**/
VOID
SaveDefNvars (
  IN VOID    *DataPtr,
  IN UINT32  BiosKnobsDataAddr,
  IN UINT32  BiosKnobsDataSize
  )
{
  EFI_STATUS    Status=EFI_SUCCESS;
  CHAR8         DefName[MAX_VARSTORE_NAME];
  CHAR16        UniCodeName[MAX_VARSTORE_NAME];
  UINT32        SetupDataAttributes = 0;
  UINTN         Size;
  UINTN         CachedSize;
  EFI_GUID      MyGuid;
  UINT8         *KnobsBuffPtrStart;
  KNOB_BIN_HDR  *KnobBinHdr;
  VOID          *DefDataPtr=NULL;

  if ((BiosKnobsDataAddr != 0) && (BiosKnobsDataSize != 0)) {
    for (KnobsBuffPtrStart = (UINT8*)(UINTN)BiosKnobsDataAddr; (KnobsBuffPtrStart < (UINT8*)(UINTN)((UINTN)BiosKnobsDataAddr+BiosKnobsDataSize));) {
      KnobBinHdr = (KNOB_BIN_HDR*) KnobsBuffPtrStart;
      if (KnobBinHdr->Hdr0.Signature == NVRO_SIGNATURE) {    // compare Signature with "$NVRO"
        KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;  // Skip NVRO, i.e. Read only Variables
        continue;
      }
      if (KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE) {    // compare Signature with "$NVAR"
        KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
        AsciiStrToUnicodeStrS ((CHAR8 *)(KnobBinHdr->NvarName), UniCodeName, sizeof (UniCodeName));
        MyGuid = KnobBinHdr->NvarGuid;
        if (KnobBinHdr->NvarSize == 0) {  // If Size is zero, skip this entry
          continue;
        }
        Size = 0;  // set Size to 0, to Get actual Size of current Variable
        SetupDataAttributes = 0;
        Status = gRT->GetVariable (UniCodeName, &MyGuid, &SetupDataAttributes, (UINTN*)&Size, NULL);
        if (EFI_ERROR(Status)) {
          DEBUG ((DEBUG_ERROR, "Status = %r \n", Status));
        }
        if (Size == 0) {  // If Size is 0 is still zero, skip this entry
          continue;
        } else {
          CachedSize = Size;
        }
        Status = gRT->GetVariable (UniCodeName, &MyGuid, &SetupDataAttributes, (UINTN*)&Size, DataPtr);
        if (!EFI_ERROR(Status)) {
          AsciiStrCpyS((CHAR8*)DefName, sizeof(KnobBinHdr->NvarName), (CHAR8*)"Def");
          AsciiStrCatS((CHAR8*)DefName, sizeof(KnobBinHdr->NvarName), (CHAR8*)KnobBinHdr->NvarName);
          AsciiStrToUnicodeStrS ((CHAR8 *)DefName, UniCodeName, sizeof (UniCodeName));
          MyGuid = KnobBinHdr->NvarGuid;
          // modify first 4 bytes of DefGuid so that they dont conflict with GlobalVariable Guid or any other restricted Guid.
          MyGuid.Data1 = 0xDEFA901D;
          DefDataPtr = (VOID*)((UINT8*)DataPtr + Size + 0x100);
          Status = gRT->GetVariable (UniCodeName, &MyGuid, &SetupDataAttributes, (UINTN*)&Size, DefDataPtr);
          if (EFI_ERROR(Status)) {  // Write current Nvar Data As default
            SetupDataAttributes = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS);
            if (Size != CachedSize) {
              Size = CachedSize;
            }
            Status = gRT->SetVariable (UniCodeName, &MyGuid, SetupDataAttributes, Size, DataPtr);
            if (EFI_ERROR(Status)) {
              ASSERT_EFI_ERROR(Status);
            }
            DEBUG ((DEBUG_INFO, "XML_CLI: SetVariable on the L\"%a\" VAR\n", DefName));
          }
        }
      } else {
        KnobsBuffPtrStart = KnobsBuffPtrStart + sizeof(KNOB_BIN_HDR);
      }
    }
  }
}

/**
  Generate XmlCli Hash at given Address pointer

  @param[in,out] AddressPtr  Pointer at which XmlCli reads input for digest

**/
VOID
GenXmlCliHash (
  IN OUT VOID  *AddressPtr
  )
{
  UINT8     Digest[DIGEST_SIZE];
  EFI_GUID  XmlCliValidatorGuid = {0x5b8483ee, 0x0af6, 0x4ca0, {0x7c, 0x80, 0x31, 0x0d, 0xef, 0xd9, 0x4a, 0x32}};
  VOID      *HmacContext;

  ZeroMem (Digest, DIGEST_SIZE);
  HmacContext = HmacSha256New();

  HmacSha256SetKey(HmacContext, (UINT8*)((UINTN)AddressPtr + KEY_OFFSET), KEY_SIZE);
  HmacSha256Update(HmacContext, (VOID*)&XmlCliValidatorGuid, GUID_SIZE);
  HmacSha256Final(HmacContext, Digest);

  CopyMem((VOID *)((UINTN) AddressPtr + DIGEST_OFFSET), &Digest, DIGEST_SIZE);
  HmacSha256Free(HmacContext);
}

/**
  Compress and save the data

  @param[in] CompStartAddr  Address at which compress data to be stored
  @param[in] DataBaseAddr   Base Address for Data to be compressed
  @param[in] DataSize       Size of the data to be compressed
  @param[in] DataType       Type of data to be compressed

  @retval  UINT32  Address where compressed data is stored
**/
UINT32
XmlCompressSave (
  IN UINT32  CompStartAddr,
  IN UINT32  DataBaseAddr,
  IN UINT32  DataSize,
  IN UINT8   DataType
  )
{
  UINT32      CompSize32;
  UINT32      XmlPatchingDataAddr;
  EFI_STATUS  Status;

  CompSize32 = 0x80000;
  Status = TianoCompress((UINT8*)(UINTN)DataBaseAddr, DataSize, (UINT8*)(UINTN)((UINTN)CompStartAddr+8), &CompSize32);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "XML_CLI: Tiano XML Compression Failed, continue with regular boot \n"));
    CompSize32 = 0;
  } else {
    DEBUG ((DEBUG_INFO, "XML_CLI: Tiano XML Compression done, Base = 0x%X  size = 0x%X  Ratio = %d \n", CompStartAddr, CompSize32, (DataSize/CompSize32)));
    if (DataType == KNOBS_DATA_BIN) {
      CopyMem((VOID*)(UINTN)CompStartAddr, (VOID*)"$TNKB", 5);
    } else {
      CopyMem((VOID*)(UINTN)CompStartAddr, (VOID*)"$TNOC", 5);
    }
    CopyMem((VOID*)((UINT8*)(UINTN)CompStartAddr+5), (VOID*)&CompSize32, 3);
    CompStartAddr = ((CompStartAddr+8+CompSize32+EFI_PAGE_MASK) & PAGE_ALIGNMENT_32_BIT);
  }
  XmlPatchingDataAddr = CompStartAddr;
  CopyMem((VOID*)(UINTN)XmlPatchingDataAddr, (VOID*)"$XKDT", 5);
  CompSize32 = 0;
  CopyMem((VOID*)((UINT8*)(UINTN)XmlPatchingDataAddr+5), (VOID*)&CompSize32, 3);
  return (UINT32)XmlPatchingDataAddr;
}

/**
  This function is called from DXE Dispatcher.
  This is Entry point for DXE phase of XmlCli and responsible
  for initializing buffers, installing protocols

  @param[in] ImageHandle  EFI Image Handle for driver entry point
  @param[in] SystemTable  EFI System Table for driver entry point

  @retval  EFI_PROTOCOL_ERROR  Failed to
                                Publishishing XmlCliCommon Protocol
                                or Allocating XmlCliCommon Protocol
                                or Publishishing GBT XML Protocol
                                or Allocating GBT XML Protocol
**/
EFI_STATUS
EFIAPI
XmlCliCommonDxeInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  BOOLEAN           GetVarFailed=FALSE;
  UINT8             XmlCliVar;
  UINT8             VarIndex;
  UINT8             NvroCount;
  UINT8             *KnobsBuffPtrStart;
  UINT8             XmlCliSupport=FixedPcdGet8(PcdXmlCliSupport);  // Default XmlCliSupport is 1(Enabled) for Server BIOS, & 0(Disabled) for Client BIOS or any external bios
  UINT8             PublishSetupPgPtr=FixedPcdGet8(PcdPublishSetupPgPtr);
  UINT8             EnableXmlCliLite=FixedPcdGet8(PcdEnableXmlCliLite);
  UINT32            Attributes;
  UINT32            XmlCliTmpAddr;
  UINT32            InterfaceMemSize;
  UINT32            XmlCliCommSize;
  UINT32            Data32;
  UINT32            CliBuffSize;
  UINT32            BiosKnobsDataAddr=0;
  UINT32            BiosKnobsDataSize=0;
  UINTN             VariableSize;
  UINTN             GBT_XML_Memory=0;
  EFI_HANDLE        Handle=NULL;
  EFI_STATUS        Status;
  XMLCLI_SETUP      XmlCliSetup;
  XML_CLI_COMMON    *XmlCliCommon;
  XML_CLI_PROTOCOL  *XmlCliProtocol;
  KNOB_BIN_HDR      *KnobBinHdr;
  VOID              *DxeCliApiProto;
  VOID              *XmlCliTempBuff;

  Status = InitXmlCliSetupEntry();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "XML_CLI[%a]: Setup Initilization Failed \n", __FUNCTION__));
  }

  Attributes = 0;
  VariableSize = sizeof(XMLCLI_SETUP);
  Status = gRT->GetVariable(XMLCLI_SETUP_NAME, &gXmlCliSetupGuid, &Attributes, &VariableSize, &XmlCliSetup);
  if (!EFI_ERROR (Status)) {
    // XmlCli Setup Variable is available and found
    GetVarFailed = FALSE;
    XmlCliSupport = XmlCliSetup.XmlCliSupport;
    PublishSetupPgPtr = XmlCliSetup.PublishSetupPgPtr;
    EnableXmlCliLite =  XmlCliSetup.EnableXmlCliLite;
  } else {
    // XmlCli Setup Variable not found
    GetVarFailed = TRUE;
    DEBUG((DEBUG_INFO, "XMLCLI[%a]: Failed to get xmlcli setup variable: Status: %r. \n", __FUNCTION__, Status));
    DEBUG((DEBUG_INFO, "XMLCLI[%a]: Initializing PCD Default values", __FUNCTION__));
    XmlCliSupport = FixedPcdGet8(PcdXmlCliSupport);
    PublishSetupPgPtr = FixedPcdGet8(PcdPublishSetupPgPtr);
    EnableXmlCliLite = FixedPcdGet8(PcdEnableXmlCliLite);
    DEBUG((DEBUG_INFO, "XMLCLI[%a]: New Values XmlCliSupport:%X EnableXmlCliLite:%X", __FUNCTION__, XmlCliSupport, EnableXmlCliLite));
  }

  // Find FFS where Bios knobs data bin is stored
  Status = FindMyFfs(ImageHandle, &gBiosKnobsDataBinGuid, &BiosKnobsDataAddr, &BiosKnobsDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "XML_CLI[%a]: FindMyFfs -> FFS not found..\n", __FUNCTION__));
    BiosKnobsDataAddr = 0;
    BiosKnobsDataSize = 0;
  }

  //
  // Installing XmlCli Protocol
  //
  Status = gBS->AllocatePool (EfiReservedMemoryType, CLI_TEMP_BUFFER_SIZE, (VOID**)&XmlCliTempBuff);
  ZeroMem((VOID*)XmlCliTempBuff, CLI_TEMP_BUFFER_SIZE);  //Clear data
  XmlCliTmpAddr = ((UINT32)(UINTN)XmlCliTempBuff + ADDRESS_ALIGNMENT) & 0xFFFFFFC0;  // 0x40 byte aligned address
  CopyMem((VOID*)(UINTN)XmlCliTmpAddr, &gXmlCliInterfaceBufferGuid, GUID_SIZE);
  if (GetRandomNumber128((UINT64*)(UINTN)((UINTN)XmlCliTmpAddr + GUID_SIZE)) != TRUE) {
    return EFI_UNSUPPORTED;
  }
  if (GetRandomNumber128((UINT64*)(UINTN)((UINTN)XmlCliTmpAddr + GUID_SIZE + 0x10)) != TRUE) {
    return EFI_UNSUPPORTED;
  }
  // Generate Hash value from the random numbers
  GenXmlCliHash((VOID*)((UINTN)XmlCliTmpAddr));
  XmlCliTempBuff = NULL;
  gBS->AllocatePool(EfiReservedMemoryType, CLI_TEMP_BUFFER_RESERVE, (VOID**)&XmlCliTempBuff);
  CopyMem((VOID*)(UINTN)XmlCliTempBuff, &XmlCliTmpAddr, 4);
  CopyMem((VOID*)((UINTN)XmlCliTempBuff+4), (VOID*)(UINTN)((UINTN)XmlCliTmpAddr+DIGEST_OFFSET), DIGEST_SIZE);  // Copy Result hash
  ZeroMem((VOID*)&DxeProtocolPtr, sizeof(DXE_PROTOCOL_PTR));  //Clear data
  DxeProtocolPtr.XmlCliTmpBuffPtr = *(UINT32*)(UINTN)(XmlCliTempBuff);
  CopyMem((VOID*)DxeProtocolPtr.Digest, (VOID*)(UINTN)((UINTN)XmlCliTmpAddr+DIGEST_OFFSET), DIGEST_SIZE);  // Copy Result hash
  ZeroMem((VOID*)(UINTN)((UINTN)XmlCliTmpAddr+DIGEST_OFFSET), DIGEST_SIZE);  // Clear data
  Handle = NULL;
  gBS->InstallProtocolInterface(&Handle, &gXmlCliVarGuid, EFI_NATIVE_INTERFACE, (VOID*)(UINTN)XmlCliTempBuff);

  //
  // We exit here if XmlCli is disabled (gXmlCliProtocolGuid is not installed so XmlCliSmm DEPEX fails)
  //
  if (0 == XmlCliSupport) {
    SHARED_MEMORY_TABLE  sharedMemTable;
    VOID                 *TempData32;
    SharedMemInitTable(&sharedMemTable, (UINT16)(ITP_XDP_VEND_DEV_ID >> 16) , ITP_XDP_BAR_NO);
    sharedMemTable.Header.CliSpecVersion.MinorVersion   = 0;
    sharedMemTable.Header.CliSpecVersion.MajorVersion   = 8;
    sharedMemTable.Header.CliSpecVersion.ReleaseVersion = 0;

    Status = SharedMemAddEntry(&sharedMemTable, XML_CLI_DISABLED_SIG, (UINT32)XmlCliTmpAddr, CLI_TEMP_BUFFER_SIZE, SHARED_MEMORY_FLAG_MEMORY_TYPE);
    ASSERT_EFI_ERROR(Status);

    Status = gBS->AllocatePool (EfiReservedMemoryType, 0x10100, (VOID**)&TempData32);
    ZeroMem((VOID*)TempData32, 0x10100);  //Clear data
    Data32 = ((UINT32)(UINTN)TempData32 + 0xFFFF) & ALIGNMENT_64_KB_32_BIT;
    CopyMem ((VOID*)(UINTN)Data32, &sharedMemTable, sizeof(SHARED_MEMORY_TABLE));
    gBS->InstallConfigurationTable (&gDramSharedMailBoxGuid, (VOID*)(UINTN)Data32);
    WriteDramMbAddr2Cmos(Data32);
    //
    // Check whether XmlCli utility has set the XmlCli variable
    //
    VariableSize = sizeof(XmlCliVar);
    Status = gRT->GetVariable(XMLCLI_SETUP_NAME, &gXmlCliVarGuid, NULL, &VariableSize, &XmlCliVar);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "XmlCliVar = %d\n", XmlCliVar));
      if ((XmlCliVar == 1) && (GetVarFailed == FALSE)) {
          XmlCliSetup.XmlCliSupport = 1;
          VariableSize = sizeof(XMLCLI_SETUP);
          Status = gRT->SetVariable (XMLCLI_SETUP_NAME, &gXmlCliSetupGuid, Attributes, VariableSize, &XmlCliSetup);
          ASSERT_EFI_ERROR(Status);
        //
        // Delete the XmlCli Variable if the Variable exist, we dont need it anymore and then Reset the system.
        //
        VariableSize = XmlCliVar = 0;
        Attributes = (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE);
        Status = gRT->SetVariable(XMLCLI_SETUP_NAME, &gXmlCliVarGuid, Attributes, 0, NULL);
        ASSERT_EFI_ERROR(Status);
        DEBUG ((DEBUG_INFO, "Rebooting the System for the XmlCli support enabling to take effect\n"));
        gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);  // Perform System Reboot, doesn't return anything
        //
        // Do not do anything after ResetSystem
        //
        CpuDeadLoop();
      }
    }
    return EFI_SUCCESS;
  }

  if (EnableXmlCliLite) {
    InterfaceMemSize = FixedPcdGet32(PcdLiteXmlReserved);
    XmlCliCommSize = sizeof (XML_CLI_BASE);
    CliBuffSize = CLI_LITE_BUFFER_SIZE;
  } else {
    InterfaceMemSize = FixedPcdGet32(PcdGbtXmlReserved);
    XmlCliCommSize = sizeof (XML_CLI_COMMON);
    CliBuffSize = CLI_BUFFER_SIZE;
  }

  // Allocate BootServices memory for the XmlCliCommon protocol instance.
  Status = gBS->AllocatePool (EfiReservedMemoryType, XmlCliCommSize, (VOID**)&XmlCliCommon);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "XML_CLI: Error Allocating XmlCliCommon Protocol, returning! \n"));
    return EFI_PROTOCOL_ERROR;
  }
  ZeroMem((VOID*)XmlCliCommon, XmlCliCommSize);  //Clear data

  // Install the XmlCliCommon onto a new handle.
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (&Handle, &gXmlCliCommonGuid, EFI_NATIVE_INTERFACE, XmlCliCommon);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "XML_CLI: Error Publishing XmlCliCommon Protocol, returning! \n"));
    gBS->FreePool(XmlCliCommon);
    return EFI_PROTOCOL_ERROR;
  }
  DEBUG ((DEBUG_INFO, "XML_CLI: XmlCliCommon Protocol published at Addr = 0x%X Size = 0x%X.\n", XmlCliCommon, XmlCliCommSize));
  XmlCliCommon->PcieAddr = (UINT32)(UINTN)(VOID*)XmlCliCommon;  // set some Value that is much above DramSharedMB for now, other Platform modules should populate correct Pcie Address value.
  XmlCliCommon->XmlCliInterfaceMemSize = InterfaceMemSize;
  XmlCliCommon->EnableXmlCliLite = EnableXmlCliLite;
  XmlCliCommon->CliBuffSize = CliBuffSize;
  XmlCliCommon->CliGen2Enable = TRUE;
  XmlCliCommon->BiosKnobsDataAddr = BiosKnobsDataAddr;
  XmlCliCommon->BiosKnobsDataSize = BiosKnobsDataSize;

  Status = gBS->AllocatePool (EfiReservedMemoryType, (sizeof(REG_CLI_COMMAND_ENTRY)*MAX_CLI_APIs_SUPPORTED), (VOID**)&DxeCliApiProto);
  if (!EFI_ERROR (Status)) {
    ZeroMem((VOID*)DxeCliApiProto, (sizeof(REG_CLI_COMMAND_ENTRY)*MAX_CLI_APIs_SUPPORTED));  //Clear data
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (&Handle, &gDxeCliApiGuid, EFI_NATIVE_INTERFACE, DxeCliApiProto);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "XML_CLI: Error Publishing DxeCliApiProto! \n"));
      gBS->FreePool(DxeCliApiProto);
    }
    //DEBUG ((DEBUG_INFO, "XML_CLI: DxeCliApi Protocol published at 0x%X.\n", DxeCliApiProto));
    XmlCliCommon->DxeCliApiProto = DxeCliApiProto;
  } else {
    DEBUG ((DEBUG_ERROR, "XML_CLI: Error Allocating DxeCliApi Protocol! \n"));
  }

  DEBUG ((DEBUG_INFO, "XML_CLI:   Allocating GBT_XML_Memory (size = %x)\n", InterfaceMemSize));
  Status = gBS->AllocatePool (EfiReservedMemoryType, (UINTN)InterfaceMemSize, (VOID*)&GBT_XML_Memory);
  ASSERT_EFI_ERROR (Status);

  ZeroMem((VOID*)(UINTN)GBT_XML_Memory, InterfaceMemSize);  //Clear data
  // Align the GBT Address to a 4 KB boundary Allocate pool assigns address from TOLM and below growing down for EfiRuntimeServicesCode
  //DEBUG ((DEBUG_INFO, "XML_CLI: GBT_XML_Memory address before alignment (0x%lX)\n", GBT_XML_Memory));
  GBT_XML_Memory = (UINT32)(((UINTN)GBT_XML_Memory + EFI_PAGE_MASK) & ~(UINTN)EFI_PAGE_MASK);
  XmlCliCommon->GbtXmlAddress  = (UINT32)GBT_XML_Memory;
  DEBUG ((DEBUG_INFO, "XML_CLI: GBT_XML_Memory address after alignment (0x%lX)\n", GBT_XML_Memory));

  UpdateBiosVersionBiosRepository(XmlCliCommon);
  #ifdef SKIP_AUTHENTICATE_XML_CLI_API
    XmlCliCommon->XmlType = XML_OS_INT;
  #else
    XmlCliCommon->XmlType = XML_OS_EXT;
  #endif

  XmlCliCommon->DramSharedMBAddr               = ((XmlCliCommon->GbtXmlAddress + InterfaceMemSize - (LEG_MAILBOX_OFFSET + LEG_MAILBOX_SIZE + CliBuffSize + CliBuffSize + EFI_PAGE_SIZE)) & ALIGNMENT_64_KB_32_BIT);
  XmlCliCommon->CliRequestBufferAddress        = XmlCliCommon->DramSharedMBAddr + CLI_REQ_BUFFER_OFFSET;
  XmlCliCommon->CliResponseBufferAddress       = XmlCliCommon->CliRequestBufferAddress + CliBuffSize;
  XmlCliCommon->XmlCliApi.DxeCliEntry          = CliEntry;
  XmlCliCommon->XmlCliApi.CliDxeRegisterApi    = CliDxeRegisterApi;
  XmlCliCommon->XmlCliApi.CliGetSetMyVariable  = CliGetSetMyVariable;
  XmlCliCommon->XmlCliApi.TianoCompress        = TianoCompress;
  XmlCliCommon->XmlCliApi.TianoDecompress      = TianoDecompress;
  DEBUG ((DEBUG_INFO, "XML_CLI: Shared Mailbox Memory address (0x%lX)\n", XmlCliCommon->DramSharedMBAddr));

  XmlCliTmpAddr = XmlCliTmpAddr & 0xFFFF0000;
  WRITE_MAILBOX(OFF_XML_CLI_TEMP_ADDR, 4, &XmlCliTmpAddr, XmlCliCommon->DramSharedMBAddr, XmlCliCommon->PcieAddr);
  gBS->InstallConfigurationTable (&gDramSharedMailBoxGuid, (VOID*)(UINTN)XmlCliCommon->DramSharedMBAddr);
  WriteDramMbAddr2Cmos(XmlCliCommon->DramSharedMBAddr);

  if ((XmlCliSetup.XmlCliDramCmosAddr == 0xFFFFFFFF) || (XmlCliSetup.XmlCliDramCmosAddr < 0x10000) || ((XmlCliSetup.XmlCliDramCmosAddr & 0xFFFF) != 0)) {
    XmlCliCommon->XmlSkipSavingDefVar = TRUE;  // let Platform Settle with all dynamics.
  } else {
    XmlCliCommon->XmlSkipSavingDefVar = FALSE;
  }
  DEBUG ((DEBUG_INFO, "XML_CLI: XmlCliSetup.XmlCliDramCmosAddr = 0x%X\n", XmlCliSetup.XmlCliDramCmosAddr));
  //
  // Restoring DramSharedMBAddr in nvram, to be used during S3 resume flow.
  //
  if ((XmlCliSetup.XmlCliDramCmosAddr != XmlCliCommon->DramSharedMBAddr) && (GetVarFailed == FALSE)) {
    XmlCliSetup.XmlCliDramCmosAddr =  XmlCliCommon->DramSharedMBAddr;
    Status = gRT->SetVariable (XMLCLI_SETUP_NAME, &gXmlCliSetupGuid, Attributes, VariableSize, &XmlCliSetup);
    ASSERT_EFI_ERROR (Status);
  }

  if (EnableXmlCliLite) {
    Data32 = (UINT32)(GBT_XML_Memory + 4);
    MyPrintf((UINTN)&Data32, (CHAR8*)"<SYSTEM>\r\n\t<PLATFORM NAME=\"UNKNOWN\" CPU=\"UNKNOWN\" CHIPSET=\"UNKNOWN\"/>\r\n\t<BIOS VERSION=\"%a\" TSTAMP=\"%a\"/>\r\n\t<GBT Version=\"3.0302\" TSTAMP=\"Feb 25 2015\" Type=\"OsInternal\" XmlCliVer=\"%a\" XmlCliType=\"Lite\"/>\r\n</SYSTEM>\r\n", XmlCliCommon->BiosVersion, XmlCliCommon->BuildTimeStamp, XML_CLI_VERSION);
    *(UINT32*)(UINTN)GBT_XML_Memory = (UINT32)AsciiStrLen((CHAR8*)(UINTN)(GBT_XML_Memory+4));
    XmlCompressSave((UINT32)(GBT_XML_Memory+0x1000), BiosKnobsDataAddr, BiosKnobsDataSize, KNOBS_DATA_BIN);  // Compress Bios Knobs Data bin FFS.
    SharedMemConstructSharedMB((VOID*)XmlCliCommon, DramMailBoxTableLite, ITP_XDP_BAR_NO, ITP_XDP_VEND_DEV_ID, XmlCliCommon->DramSharedMBAddr);
    XmlCliCommon->CurrGetSetVarBuffer = XmlCliCommon->DramSharedMBAddr - 0x8000;
    if (XmlCliCommon->XmlSkipSavingDefVar == FALSE) {
      SaveDefNvars((VOID*)(UINTN)XmlCliCommon->CurrGetSetVarBuffer, BiosKnobsDataAddr, BiosKnobsDataSize);
    }
  } else {
    XmlCliCommon->SkipXmlComprs  = 0;

    if ((BiosKnobsDataAddr == 0) || (BiosKnobsDataSize == 0)) {
      DEBUG ((DEBUG_INFO, "XML_CLI: gBiosKnobsDataBin FFS not found..\n"));
    } else {
      VarIndex = 0;
      NvroCount = 0;
      for (KnobsBuffPtrStart = (UINT8*)(UINTN)BiosKnobsDataAddr; (KnobsBuffPtrStart < (UINT8*)(UINTN)((UINTN)BiosKnobsDataAddr+BiosKnobsDataSize));) {
        KnobBinHdr = (KNOB_BIN_HDR*) KnobsBuffPtrStart;
        if (KnobBinHdr->Hdr0.Signature == NVRO_SIGNATURE) {    // compare Signature with "$NVRO"
          NvroCount++;
          KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;  // Skip NVRO, i.e. Read only Variables
          continue;
        }
        if (KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE) {    // compare Signature with "$NVAR"
          KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
          XmlCliCommon->VarstoreTable[VarIndex].Size = (UINT16)KnobBinHdr->NvarSize;
          XmlCliCommon->VarstoreTable[VarIndex].BitKnobCount = (UINT16)KnobBinHdr->BitKnobCount;
          AsciiStrCpyS((CHAR8*)XmlCliCommon->VarstoreTable[VarIndex].Name, sizeof(KnobBinHdr->NvarName), (CHAR8*)KnobBinHdr->NvarName);
          XmlCliCommon->VarstoreTable[VarIndex].Guid = KnobBinHdr->NvarGuid;
          if (KnobBinHdr->NvarSize == 0) {  // If Size is zero, skip this entry
            VarIndex++;
            continue;
          }
          AsciiStrCpyS((CHAR8*)XmlCliCommon->VarstoreTable[VarIndex].DefName, sizeof(KnobBinHdr->NvarName), (CHAR8*)"Def");
          AsciiStrCatS((CHAR8*)XmlCliCommon->VarstoreTable[VarIndex].DefName, sizeof(KnobBinHdr->NvarName), (CHAR8*)KnobBinHdr->NvarName);
          XmlCliCommon->VarstoreTable[VarIndex].DefGuid = KnobBinHdr->NvarGuid;
          // modify first 4 bytes of DefGuid so that they dont conflict with GlobalVariable Guid or any other restricted Guid.
          XmlCliCommon->VarstoreTable[VarIndex].DefGuid.Data1 = 0xDEFA901D;
          VarIndex++;
        } else {
          KnobsBuffPtrStart = KnobsBuffPtrStart + sizeof(KNOB_BIN_HDR);
        }
      }
      XmlCliCommon->NumberOfVarstores = (UINT8)VarIndex;
      XmlCliCommon->NvroCount = (UINT8)NvroCount;
    }

    XmlCliCommon->PublishSetupPgPtr       = PublishSetupPgPtr;
    XmlCliCommon->XmlGenBiosKnobsSection  = XmlGenBiosKnobsSection;
    XmlCliCommon->DxeXmlCompress          = DxeXmlCompress;
    XmlCliCommon->KnobXmlEntryAddr        = XmlCliCommon->DramSharedMBAddr - FixedPcdGet32(PcdKnobsEntriesSize);
    XmlCliCommon->KnobValMapAddr          = XmlCliCommon->KnobXmlEntryAddr - FixedPcdGet32(PcdKnobValueMapSize);
    XmlCliCommon->XmlCliDebugLogBuff      = XmlCliCommon->KnobValMapAddr - XMLCLI_DEBUG_LOG_BUFF_SIZE;
    XmlCliCommon->CurrGetSetVarBuffer     = XmlCliCommon->XmlCliDebugLogBuff - GET_SET_TEMP_DATA_BUFF_SIZE;

    SharedMemConstructSharedMB((VOID*)XmlCliCommon, DramMailBoxTable, ITP_XDP_BAR_NO, ITP_XDP_VEND_DEV_ID, XmlCliCommon->DramSharedMBAddr);

    //   ****************  ------Install Default Common XmlCli Protocol Start ------  ***************
    // Allocate BootServices memory for the XmlCliProtocol protocol instance.
    Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (XML_CLI_PROTOCOL), (VOID**)&XmlCliProtocol);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "XML_CLI: Error Allocating GBT XML Protocol, returning! \n"));
      return EFI_PROTOCOL_ERROR;
    }
    ZeroMem((VOID*)XmlCliProtocol, sizeof(XML_CLI_PROTOCOL));  //Clear data

    // Install the XmlCliProtocol onto a new handle.
    Handle = NULL;
    Status = gBS->InstallProtocolInterface (&Handle, &gXmlCliProtocolGuid, EFI_NATIVE_INTERFACE, XmlCliProtocol);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "XML_CLI: Error Publishing GBT XML Protocol, returning! \n"));
      gBS->FreePool(XmlCliProtocol);
      return EFI_PROTOCOL_ERROR;
    }
    XmlCliCommon->XmlCliProtocolHandle  = Handle;
    XmlCliProtocol->XmlCliCommon        = XmlCliCommon;
    XmlCliProtocol->GenerateGbtXml      = GenerateGbtXml;
  }

// ****************  ------Install Default Common XmlCli Protocol End ------  ***************
  return EFI_SUCCESS;
}

/**
  We have this function so that we can have following XML block generating functions
  available only in one driver and the other driver will just reuse them.
  Not required, but will save both FV_MAIN & RunTime/BootTime memory resources.

  @param[in]  XmlCliCom            XmlCli Common Structure
  @param[out] NewXmlWriterContext  Get New XmlCi Writer Context at the specified pointer

**/
UINTN
XmlGenBiosKnobsSection (
  IN VOID   *XmlCliCom,
  OUT VOID  *NewXmlWriterContext
 )
{
  UINTN           CurrXmlWriterContext;
  XML_CLI_COMMON  *XmlCliCommon=(XML_CLI_COMMON*)XmlCliCom;

  OvrdXmlWriterContext(NewXmlWriterContext);
  DEBUG ((DEBUG_INFO, "\t XmlGenerateBiosKnobs\n"));
  XmlGenerateBiosKnobs(XmlCliCommon); // <biosknobs>
  CurrXmlWriterContext = (UINTN)RetXmlWriterContext();
  return  CurrXmlWriterContext;
}

/**
  Compress the XML specified at location
  XmlPatchBaseAddr from parameter XmlCli Common structure.


  @param[in] XmlCliCom  XmlCli Common Structure
  @param[in] XmlSize    Uncompressed Xml Size

  @retval  UINT32  End Address of the Compressed XML
**/
UINT32
DxeXmlCompress (
  IN VOID    *XmlCliCom,
  IN UINT32  XmlSize
  )
{
  UINT32          XmlDataEndAddress=0;
  XML_CLI_COMMON  *XmlCliCommon = (XML_CLI_COMMON*)XmlCliCom;

  XmlDataEndAddress = ((XmlCliCommon->GbtXmlAddress + 4 + XmlSize + EFI_PAGE_MASK) & PAGE_ALIGNMENT_32_BIT);
  if (XmlCliCommon->SkipXmlComprs == 0) {  // Skip compression if its disabled.
    DEBUG ((DEBUG_INFO, "XML_CLI: Start Xml Compression\n"));
    XmlCliCommon->XmlPatchBaseAddr = XmlCompressSave(XmlDataEndAddress, (XmlCliCommon->GbtXmlAddress+4), XmlSize, XML_DATA_ONLY);  // Compress XML Data
    CopyMem ((VOID*)(UINTN)((UINTN)XmlCliCommon->DramSharedMBAddr + LEG_MAILBOX_OFFSET + OFF_GEN2_GBT_COMPR_ADDR), &XmlDataEndAddress, 4);  // Compress XML Data
    XmlCliCommon->XmlPatchBaseAddr = XmlCompressSave(XmlCliCommon->XmlPatchBaseAddr, XmlCliCommon->BiosKnobsDataAddr, XmlCliCommon->BiosKnobsDataSize, KNOBS_DATA_BIN);  // Compress Bios Knobs Data bin FFS.
    XmlDataEndAddress = (UINT32)((XmlCliCommon->XmlPatchBaseAddr + FixedPcdGet32(PcdKnobPatchDataBufferSize) + 0xFFF)& 0xFFFFF000);    // reserved XML patch DataBuffer of size 0x20000 for XML Knobs delta information
  }
  return XmlDataEndAddress;
}

/**
  Registers the Cli API
  to expose the Api this function has to be used
  usage:
    CliDxeRegisterApi(MEM_READ_OPCODE, CliMemRead, XmlCliProtocol)

  @param[in] CommandId      Command Id for the API
  @param[in] ClicmdHandler  Handler function
  @param[in] Buffer         Pointer to Protocol Structure

  @retval  VOID
**/
VOID
CliDxeRegisterApi (
  IN UINT16                CommandId,
  IN CLI_CMD_HANDLER_FUNC  ClicmdHandler,
  IN VOID                  *Buffer
  )
{
  UINT8                  count;
  EFI_STATUS             Status;
  REG_CLI_COMMAND_ENTRY  *DxeCliApiProto=NULL;

  Status = gBS->LocateProtocol(&gDxeCliApiGuid, NULL, (VOID **)&DxeCliApiProto);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "XML_CLI: Error locating DxeCliApiProto! \n"));
    return;
  }
  if (DxeCliApiProto == NULL) {
    return;
  }

  for (count = 0; count < MAX_CLI_APIs_SUPPORTED; count++) {
    if ((DxeCliApiProto->CommandId == 0) && (DxeCliApiProto->CmdHandler == NULL)) {
      DxeCliApiProto->CommandId  = CommandId;
      DxeCliApiProto->CmdHandler = ClicmdHandler;
      DxeCliApiProto->Buffer     = Buffer;
      return;
    }
    DxeCliApiProto++;
  }
}

/**
  Cli Entry Point Exposed as a XmlCli API structure

  @param[in] XmlCliCom  XmlCli Common Structure

  @retval  EFI_SUCCESS
  @retval  EFI_INVALID_PARAMETER  Null pointer passed as parameter
  @retval  EFI_NO_RESPONSE        Signature in Structure is still not in READY state
  @retval  EXIT_OPCODE            Specified Operation code in Request buffer match to EXIT_OPCODE
**/
EFI_STATUS
CliEntry (
  IN VOID  *XmlCliCom
  )
{
  UINT32          Signature=0;
  EFI_STATUS      Status=EFI_SUCCESS;
  XML_CLI_COMMON  *XmlCliCommon;

  XmlCliCommon = (XML_CLI_COMMON*)XmlCliCom;
  Signature = ((CLI_BUFFER*)(UINTN)XmlCliCommon->CliRequestBufferAddress)->Signature;
  if ((Signature == CLI_GEN2_SGN_REQUEST_READY) || (Signature == CLI_SGN_REQUEST_READY)) { // Enter Only if Req Buff Signature is valid, Optimized Call
    if (CompareMem((VOID*)((UINTN)DxeProtocolPtr.Digest), (VOID*)((UINTN)DxeProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET), DIGEST_SIZE) == 0) {
      ZeroMem((VOID*)((UINTN)DxeProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET), DIGEST_SIZE);
      Status = CliEntryPoint((VOID*)XmlCliCommon);
      *(UINT64*)(UINTN)((UINTN)DxeProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET) = XMLCLI_REQUEST_VALIDATED_SIG1;
      *(UINT64*)(UINTN)((UINTN)DxeProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET + sizeof(UINT64)) = XMLCLI_REQUEST_VALIDATED_SIG2;
    }
  }
  return Status;
}

/**
  Generate GBT Xml file at GbtXmlAddress

  @param[in] XmlCliProto  Pointer to XmlCli Protocol

  @retval  VOID
**/
VOID
GenerateGbtXml (
  IN VOID  *XmlCliProto
  )
{
  UINT32            XmlSize;
  UINT32            XmlDataEndAddress;
  EFI_STATUS        Status;
  EFI_HANDLE        Handle=NULL;
  XML_CLI_PROTOCOL  *XmlCliProtocol=(XML_CLI_PROTOCOL*)XmlCliProto;
  VOID              *CurrXmlWrtrContext;

  if (XmlCliProtocol == NULL) { // XmlCliProtocol not published.
    DEBUG ((DEBUG_INFO, "XML_CLI: XmlCliProtocol is NULL, returning! \n"));
    return;
  } else {
    if ((XmlCliProtocol->XmlCliCommon->GbtXmlAddress != 0) && (XmlCliProtocol->XmlCliStructInitialized ==  TRUE)) {
      if (XmlValid(XmlCliProtocol->XmlCliCommon->GbtXmlAddress)) {
        DEBUG ((DEBUG_INFO, "XML_CLI: GBT XML is already published, returning! \n"));
        return; // XML is already published, return
      }
    }
  }

  XmlCliProtocol->XmlCliCommon->MergeDuplicateKnobs = 0;    // Merge Duplicate knobs.

  OpenXmlFile(XmlCliProtocol->XmlCliCommon->GbtXmlAddress);

  DEBUG ((DEBUG_INFO, "XML_CLI: XmlGenerateSystemInfo\n"));
  XmlGenerateSystemInfo(XmlCliProtocol->XmlCliCommon);

  CurrXmlWrtrContext = RetXmlWriterContext();
  CurrXmlWrtrContext = (VOID*)(XmlCliProtocol->XmlCliCommon->XmlGenBiosKnobsSection (XmlCliProtocol->XmlCliCommon, CurrXmlWrtrContext));
  OvrdXmlWriterContext(CurrXmlWrtrContext);

  XmlSize = CloseXmlFile(XmlCliProtocol->XmlCliCommon->GbtXmlAddress);
  XmlDataEndAddress = XmlCliProtocol->XmlCliCommon->DxeXmlCompress((VOID*)XmlCliProtocol->XmlCliCommon, XmlSize);

  XmlCliProtocol->XmlCliCommon->FreeGbtMemAddr = XmlDataEndAddress;
  if (XmlCliProtocol->XmlCliCommon->KnobValMapAddr <= XmlCliProtocol->XmlCliCommon->FreeGbtMemAddr) {
    DEBUG ((DEBUG_ERROR, "XML_CLI: ERROR: XML Size exceeded than what we expected \n"));
    ASSERT(FALSE);
  } else {
    XmlCliProtocol->XmlCliCommon->FreeGbtMemSize = XmlCliProtocol->XmlCliCommon->KnobValMapAddr - XmlCliProtocol->XmlCliCommon->FreeGbtMemAddr;
    DEBUG ((DEBUG_INFO, "XML_CLI: XML Base = 0x%X  XML size = 0x%X  DramMbAddress = 0x%x  SafetyBuffer = %d KB\n", XmlCliProtocol->XmlCliCommon->GbtXmlAddress, XmlSize, XmlCliProtocol->XmlCliCommon->DramSharedMBAddr, (XmlCliProtocol->XmlCliCommon->FreeGbtMemSize/1024)));
  }
  XmlCliProtocol->XmlCliStructInitialized = TRUE;
  Status = gBS->InstallProtocolInterface(&Handle, &gXmlCliInterfaceReadyGuid, EFI_NATIVE_INTERFACE, NULL);
  DEBUG((DEBUG_INFO, "XML_CLI: Triggerring Dummy protocol to Mark Xmlcli Ready, Status: %r\n", Status));
}

/**
  Checks whether the Knob data is valid by matching the header
  Signature to NVAR_SIGNATURE (0x5241564E24)

  @param[in,out] XmlCliCommon  XmlCli Common Structure
                                Update BiosKnobsDataAddr if Data32 found when
                                Reading Mailbox

  @retval  TRUE   Knob header Signature match to NVAR_SIGNATURE `$NVAR`
  @retval  FALSE  Failure
**/
BOOLEAN
IsKnobsDataBinValid (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  )
{
  UINT32        Data32;
  KNOB_BIN_HDR  *KnobBinHdr;
  READ_MAILBOX(OFF_BIOS_KNOBS_DATA_BIN_ADDR, 4, &Data32, XmlCliCommon->DramSharedMBAddr, XmlCliCommon->PcieAddr);
  if (Data32) {
    KnobBinHdr = (KNOB_BIN_HDR*)(UINTN)Data32;
    if (KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE) {
      XmlCliCommon->BiosKnobsDataAddr = Data32;
      READ_MAILBOX(OFF_BIOS_KNOBS_DATA_BIN_SIZE, 4, &Data32, XmlCliCommon->DramSharedMBAddr, XmlCliCommon->PcieAddr);
      XmlCliCommon->BiosKnobsDataSize = Data32;
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Get Offset and size of the knob(setup variable)  from Bios Knobs Data bin.

  @param[in]     BiosKnobsDataAddr  Address of Bios knobs Data bin.
  @param[in]     BiosKnobsDataSize  Size of Bios knobs Data bin.
  @param[in]     VarName            Name of Nvar.
  @param[in]     VarGuid            Nvar Guid.
  @param[in,out] MyKnobList          List of knob for which Offset and size to be find.
                                      Size and offset will be part of the structure as output.
  @param[in]     MyKnobCount        Total number of knob for which Offset and size to be find.
  @param[in,out] NvarSize           Size of the setup variable

  @retval  UINT16
**/
UINT16
CliGetSetMyVariable (
  IN VOID      *XmlCliCom,
  IN UINT8     Operation,
  IN OUT VOID  *MyKnobList,
  IN UINT16     MyKnobCount
  )
{
  BOOLEAN         SkipThisNvar;
  BOOLEAN         ValidDataFound=FALSE;
  BOOLEAN         BitWise;
  UINT8           BitField;
  UINT8           KnobTypeSz;
  UINT8           CurKnobSize;
  CHAR8           DefName[64];
  CHAR16          UniCodeName[64];
  CHAR16          DefUniCodeName[64];
  UINT16          DataOffset;
  UINT16          Modified=0;
  UINT16          SetupModified=0;
  UINT16          Count=0;
  UINT16          CurCount=0;
  UINT32          Attributes=0;
  UINT32          DefAttributes=0;
  UINTN           VariableSize;
  UINTN           DefVariableSize;
  EFI_STATUS      Status;
  EFI_GUID        MyGuid;
  EFI_GUID        DefGuid;
  UINT8           *BufferPtr;
  UINT8           *KnobsBuffPtrStart;
  KNOB_LIST       *MyKnobListPtr;
  KNOB_BIN_HDR    *KnobBinHdr;
  XML_CLI_COMMON  *XmlCliCommon;
  VOID            *Setup=NULL;
  VOID            *DefSetup=NULL;

  MyKnobListPtr = (KNOB_LIST *)MyKnobList;
  for (Count=0; Count < MyKnobCount; Count++, MyKnobListPtr++) {
    MyKnobListPtr->KnobSize = 0;  // this also indicates that the knob was not processed.
    MyKnobListPtr->KnobOffset = 0;
  }
  XmlCliCommon = (XML_CLI_COMMON*)XmlCliCom;
  if ((XmlCliCommon == 0) || (MyKnobCount == 0)) {
    return 0;
  }

  if ((XmlCliCommon->BiosKnobsDataAddr == 0) || (XmlCliCommon->BiosKnobsDataSize == 0)) {
    ValidDataFound = IsKnobsDataBinValid(XmlCliCommon);
  } else {
    KnobBinHdr = (KNOB_BIN_HDR*)(UINTN)XmlCliCommon->BiosKnobsDataAddr;
    if (KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE) {
      ValidDataFound = TRUE;
    } else {
      ValidDataFound = IsKnobsDataBinValid(XmlCliCommon);
    }
  }

  if (ValidDataFound != TRUE) {
    DEBUG ((DEBUG_ERROR, "XML_CLI: gBiosKnobsDataBin FFS not found..\n"));
    return 0;
  }
  // DEBUG ((DEBUG_INFO, "BiosKnobsData Addr = 0x%X  Size = 0x%X MyKnobCount= %d\n", XmlCliCommon->BiosKnobsDataAddr, XmlCliCommon->BiosKnobsDataSize, MyKnobCount));

  if (Operation == GET_VAR) {
    (VOID) gBS->AllocatePool(EfiBootServicesData, 0x10000, (VOID**)&Setup);  // Allocate common 64K buffer for all Get Variables, will be faster.
  } else {
    (VOID) gBS->AllocatePool(EfiBootServicesData, 0x20000, (VOID**)&Setup);  // Allocate common 64K * 2 buffer for all Get Variables, will be faster.
    DefSetup = (VOID*) ((UINTN) Setup + 0x10000);
  }
  SetupModified = 0;
  for (KnobsBuffPtrStart = (UINT8*)(UINTN) XmlCliCommon->BiosKnobsDataAddr; (KnobsBuffPtrStart < (UINT8*)(UINTN)((UINTN)XmlCliCommon->BiosKnobsDataAddr+XmlCliCommon->BiosKnobsDataSize));) {
    KnobBinHdr = (KNOB_BIN_HDR*) KnobsBuffPtrStart;
    if (CurCount >= MyKnobCount) {
      break;
    }
    if ((KnobBinHdr->Hdr0.Signature == NVAR_SIGNATURE) || (KnobBinHdr->Hdr0.Signature == NVRO_SIGNATURE)) {  // compare Signature with "$NVAR" or "$NVRO"
      MyGuid = KnobBinHdr->NvarGuid;
      VariableSize = 0;
      SkipThisNvar = FALSE;
      Modified = 0;
      for (BufferPtr=(KnobsBuffPtrStart+sizeof(KNOB_BIN_HDR)); ((BufferPtr < (KnobsBuffPtrStart+KnobBinHdr->Hdr1.DupKnobBufOff)) && ((UINT32)(UINTN) BufferPtr < (XmlCliCommon->BiosKnobsDataAddr+XmlCliCommon->BiosKnobsDataSize)));) {
        if (SkipThisNvar) {
          break;  // This Nvar has some issues, skip it
        }
        if (CurCount >= MyKnobCount) {
          break;
        }
        CopyMem(&DataOffset, BufferPtr, 2);
        BufferPtr = BufferPtr + 2;  // Aditionally ignore 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0])
        CopyMem(&KnobTypeSz, BufferPtr, 1);
        BufferPtr = BufferPtr + 1;  // 1 byte of data (reserved for Knob Type[7:4] & KnobSize[3:0])
        BitWise = FALSE;
        CurKnobSize = 7;
        if ((KnobTypeSz & 0x80) == 0) {  // dont process EFI_IFR_STRING_OP
          if ((KnobTypeSz & 0xF) >= 0xC) { // this indicates that the given Knob is Bitwise and is of Size mentioned in subsequent fields
            BitWise = TRUE;
            CopyMem (&BitField, BufferPtr, 1);
            BufferPtr = BufferPtr + 1;  // 1 byte of data Bitsize[7:3] BitOffset[2:0]
            CurKnobSize = BitGetByteSize((BitField & 0x7), ((BitField >> 3) & 0x3F));
          } else {
            CurKnobSize = (KnobTypeSz & 0xF);
          }
        }
        MyKnobListPtr = (KNOB_LIST *)MyKnobList;
        for (Count = 0; Count < MyKnobCount; Count++, MyKnobListPtr++) {
          if (MyKnobListPtr->KnobSize) {
            continue;  // This knob was processed already
          }
          if (AsciiStrCmp((CHAR8*)(VOID*)BufferPtr, MyKnobListPtr->KnobName) == 0) {
            if (VariableSize == 0) {
              MyKnobListPtr->VarId = (UINT8)KnobBinHdr->Hdr0.VarId;
              ZeroMem((VOID*)UniCodeName, sizeof(UniCodeName));  //Clear data
              AsciiStrToUnicodeStrS (KnobBinHdr->NvarName, UniCodeName, (sizeof (UniCodeName)/sizeof(CHAR16)));
              (VOID) gRT->GetVariable(UniCodeName, &MyGuid, &Attributes, &VariableSize, NULL);
              if (VariableSize != 0) {
                Status = gRT->GetVariable(UniCodeName, &MyGuid, &Attributes, &VariableSize, Setup);
                if (EFI_ERROR (Status)) {
                  DEBUG ((DEBUG_INFO, "XML_CLI[GetSetMyVar]: Status=%r\n", Status));
                  SkipThisNvar = TRUE;
                  break;
                } else {
                  if (Operation == GET_DEF_CUR_VAR || Operation == GET_MOD_SET_VAR) {
                    AsciiStrCpyS((CHAR8*)DefName, sizeof(KnobBinHdr->NvarName), (CHAR8*)"Def");
                    AsciiStrCatS((CHAR8*)DefName, sizeof(KnobBinHdr->NvarName), (CHAR8*)KnobBinHdr->NvarName);
                    ZeroMem((VOID*)DefUniCodeName, sizeof(DefUniCodeName));  //Clear data
                    AsciiStrToUnicodeStrS ((CHAR8 *)DefName, DefUniCodeName, sizeof (DefUniCodeName));
                    DefGuid = MyGuid;
                    DefGuid.Data1 = 0xDEFA901D;
                    DefVariableSize = VariableSize;
                    Status = gRT->GetVariable(DefUniCodeName, &DefGuid, &DefAttributes, &DefVariableSize, DefSetup);
                    if (EFI_ERROR (Status)) {
                      CopyMem (DefSetup, Setup, VariableSize);
                    }
                  }
                }
              }
            }
            CurCount++;
            if (CurKnobSize == 7) {  // EFI_IFR_STRING_OP
              MyKnobListPtr->KnobSize = 0x7;  // Limit String Type knob's size to a non valid 7 bytes, this means we dont support String type knobs as of now.
              continue;
            } else {
              MyKnobListPtr->KnobSize = CurKnobSize;
            }
            MyKnobListPtr->KnobOffset = DataOffset;
            if ((VariableSize != 0) && (MyKnobListPtr->KnobSize <= sizeof(UINTN))) {
              if ((Operation == GET_VAR) || (Operation == GET_DEF_CUR_VAR)) {
                MyKnobListPtr->KnobValue = 0;
                CopyMem(&MyKnobListPtr->KnobValue, (UINT8*)Setup+DataOffset, MyKnobListPtr->KnobSize);
                if (BitWise) {
                  MyKnobListPtr->KnobValue = BitExtractValue64(MyKnobListPtr->KnobValue, (BitField & 0x7), ((BitField >> 3) & 0x3F));
                }
                if (Operation == GET_DEF_CUR_VAR) {
                  MyKnobListPtr->DefKnobValue = 0;
                  CopyMem(&MyKnobListPtr->DefKnobValue, (UINT8*)DefSetup+DataOffset, MyKnobListPtr->KnobSize);
                  if (BitWise) {
                    MyKnobListPtr->DefKnobValue = BitExtractValue64(MyKnobListPtr->DefKnobValue, (BitField & 0x7), ((BitField >> 3) & 0x3F));
                  }
                }
              } else if (Operation == GET_MOD_SET_VAR) {
                MyKnobListPtr->DefKnobValue = 0;
                CopyMem(&MyKnobListPtr->DefKnobValue, (UINT8*)DefSetup+DataOffset, MyKnobListPtr->KnobSize);
                if (MyKnobListPtr->KnobValue <= (0xFFFFFFFFFFFFFFFF >> (64-(8*MyKnobListPtr->KnobSize)))) {  // Allow only if input Value is within the Knob size range
                  if (BitWise) {
                    if(BitDataMatch64((VOID*)((UINT8*)Setup+DataOffset),(VOID*)&MyKnobListPtr->KnobValue, (BitField & 0x7), (BitField >> 3), 0) == FALSE) {
                      BitDataCopy64((VOID*)((UINT8*)Setup+DataOffset),(VOID*)&MyKnobListPtr->KnobValue, (BitField & 0x7), (BitField >> 3));
                      Modified++;
                      SetupModified++;
                    }
                  } else {
                    if (CompareMem((UINT8*)Setup+DataOffset, &MyKnobListPtr->KnobValue, MyKnobListPtr->KnobSize) != 0) {
                      CopyMem((UINT8*)Setup+DataOffset, &MyKnobListPtr->KnobValue, MyKnobListPtr->KnobSize);
                      Modified++;
                      SetupModified++;
                    }
                  }
                } else {
                  MyKnobListPtr->KnobValue = 0;
                  CopyMem(&MyKnobListPtr->KnobValue, (UINT8*)Setup+DataOffset, MyKnobListPtr->KnobSize);
                  if (BitWise) {
                    MyKnobListPtr->KnobValue = BitExtractValue64(MyKnobListPtr->KnobValue, (BitField & 0x7), ((BitField >> 3) & 0x3F));
                  }
                }
              }
            } else {
              MyKnobListPtr->KnobSize = 0;  // this means this specific Knob was not found or consider it as InValid
            }
            // DEBUG ((DEBUG_INFO, "%a[0x%04X][%d] = 0x%X\n", BufferPtr, DataOffset, MyKnobListPtr->KnobSize, MyKnobListPtr->KnobValue));
            if (CurCount >= MyKnobCount) {
              break;
            }
          }
        }
        // increment the knob name string
        while (*BufferPtr) {
          BufferPtr++;
        }
        BufferPtr++;  // Skip Null byte encounter
        // Skip the DEPEX
        while (*BufferPtr) {
          BufferPtr++;
        }
        BufferPtr++;  // Skip Null byte encounter
      }
      if (Modified) {
        Status = gRT->SetVariable(UniCodeName, &MyGuid, Attributes, VariableSize, Setup);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "XML_CLI[GetSetMyVar]: Set Variable Status = %r..for %s \n", Status, UniCodeName));
        }
      }
      KnobsBuffPtrStart = KnobsBuffPtrStart + KnobBinHdr->Hdr1.NvarPktSize;
    } else {
      KnobsBuffPtrStart = KnobsBuffPtrStart + sizeof(KNOB_BIN_HDR);
    }
  }
  // DEBUG ((DEBUG_INFO, "XML_CLI[GetSetMyVar]: Total Modified Knobs = %d \n", SetupModified));
  gBS->FreePool(Setup);
  return SetupModified;
}
