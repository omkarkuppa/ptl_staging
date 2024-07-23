/** @file
  XML CLI SMM driver implementation.

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

@par Specification
**/

#include <XmlCliApi.h>
#include <XmlCliComLib.h>
#include <SharedMailbox.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmVariable.h>
#include <Library/SmmMemLib.h>
#include <Cli.h>

#pragma pack(push, 1)
typedef struct {
  XML_CLI_COMMON  *XmlCliCommon;
  VOID            *SmmVariable;
  UINT8           XmlCliSupport;
  UINT32          XmlCliTmpBuffPtr;
  UINT8           Digest[DIGEST_SIZE];
} PROTOCOL_PTR;

#pragma pack(pop)

// Global variables
EFI_SMM_BASE2_PROTOCOL  *gSMM;
EFI_SMM_SYSTEM_TABLE2   *mSmst=NULL;

PROTOCOL_PTR            ProtocolPtr;


/**
  Handles Cli Callback function entry.

  @param[in]     DispatchHandle   EFI dispatch handle
  @param[in]     DispatchContext  Software SMM Register Context
  @param[in,out] CommBuffer       Communication buffer pointer
  @param[in,out] CommBufferSize   Size of the communication buffer

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
SmmCliEntryCallback (
  IN EFI_HANDLE                         DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext, OPTIONAL
  IN OUT VOID                           *CommBuffer,      OPTIONAL
  IN OUT UINTN                          *CommBufferSize   OPTIONAL
  );

/**
  Call back function handles XmlCli variable change

  @param[in]     DispatchHandle           EFI dispatch handle
  @param[in]     DispatchContext          Software SMM Register Context
  @param[in,out] CommBuffer               Communication buffer pointer
  @param[in,out] CommBufferSize           Size of the communication buffer

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
XmlCliCommonSmmEnableCallback (
  IN EFI_HANDLE                         DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext, OPTIONAL
  IN OUT VOID                           *CommBuffer,      OPTIONAL
  IN OUT UINTN                          *CommBufferSize   OPTIONAL
  );

/**
  Initial Entry point for XmlCli Smm Driver.
  This function is called from SMM during SMM registration.

  @param[in] ImageHandle  Image Handle
  @param[in] SystemTable  Efi system table

**/
EFI_STATUS
EFIAPI
XmlCliCommonSmmInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_HANDLE                     Handle;
  EFI_SMM_SW_REGISTER_CONTEXT    SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;
  VOID                           *TmpProtocolPtr;
  VOID                           *SmmCliApiProto=NULL;

  ZeroMem((VOID*)&ProtocolPtr, sizeof(PROTOCOL_PTR));  //Clear data
  ProtocolPtr.XmlCliSupport = 0;  // Default XmlCliSupport is 1(Enabled) for DCG BIOS, & 0(Disabled) for CCG BIOS
  DEBUG ((DEBUG_INFO,"XML_CLI: SMM: InitSmmHandler routine start!\n"));
  Status = gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, (VOID **)&gSMM);
  ASSERT_EFI_ERROR (Status);

  TmpProtocolPtr = NULL;
  Status = gBS->LocateProtocol (&gXmlCliVarGuid, NULL, (VOID **) &TmpProtocolPtr);
  if (!EFI_ERROR(Status)) {
    ProtocolPtr.XmlCliTmpBuffPtr = *(UINT32*)(UINTN)(TmpProtocolPtr);
    CopyMem((VOID*)ProtocolPtr.Digest, (VOID*)((UINTN)TmpProtocolPtr + 4), DIGEST_SIZE);
    ZeroMem((VOID*)((UINTN)TmpProtocolPtr + 4), DIGEST_SIZE);
  }

  if (XmlCliExecuteInSmm())
  {
    Status = gSMM->GetSmstLocation (gSMM, &mSmst);
    ASSERT_EFI_ERROR (Status);

    Status = mSmst->SmmLocateProtocol(&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID **)&SwDispatch);
    ASSERT_EFI_ERROR (Status);

    TmpProtocolPtr = NULL;
    Status = mSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**)&TmpProtocolPtr);
    ASSERT_EFI_ERROR (Status);
    ProtocolPtr.SmmVariable = TmpProtocolPtr;

    TmpProtocolPtr = NULL;
    Status = gBS->LocateProtocol (&gXmlCliCommonGuid, NULL, (VOID **) &TmpProtocolPtr);
    if (EFI_ERROR (Status)) {
      //
      // Register SMI handler only for enabling Xml Cli via a secure mode.
      //
      Handle = NULL;
      Status  = mSmst->SmiHandlerRegister ((EFI_SMM_HANDLER_ENTRY_POINT2)XmlCliCommonSmmEnableCallback, NULL, &Handle);
      ASSERT_EFI_ERROR (Status);

      Handle = NULL;
      SwContext.SwSmiInputValue = SW_XML_CLI_ENTRY;
      Status = SwDispatch->Register (SwDispatch, (EFI_SMM_HANDLER_ENTRY_POINT2)XmlCliCommonSmmEnableCallback, &SwContext, &Handle);
      ASSERT_EFI_ERROR (Status);
      DEBUG ((DEBUG_INFO,"XML_CLI: SMM: Registered XmlCliCommonSmmEnableCallback(): S/W SMI Value = 0x%X\n", SwContext.SwSmiInputValue));
      ProtocolPtr.XmlCliSupport = 0;
      return EFI_SUCCESS;
    }

    ProtocolPtr.XmlCliSupport = 1;
    ProtocolPtr.XmlCliCommon = (XML_CLI_COMMON*)TmpProtocolPtr;
    ProtocolPtr.XmlCliCommon->Smst = (VOID*)mSmst;
    ProtocolPtr.XmlCliCommon->SmmVariable = ProtocolPtr.SmmVariable;

    // Register SMI handler for CLI.
    Handle = NULL;
    Status  = mSmst->SmiHandlerRegister ((EFI_SMM_HANDLER_ENTRY_POINT2)SmmCliEntryCallback, NULL, &Handle);
    ASSERT_EFI_ERROR (Status);

    SwContext.SwSmiInputValue = SW_XML_CLI_ENTRY;
    Handle = NULL;
    Status = SwDispatch->Register (SwDispatch,(EFI_SMM_HANDLER_ENTRY_POINT2)SmmCliEntryCallback, &SwContext, &Handle);
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO,"XML_CLI: SMM: Registered SmmCliEntryCallback(): S/W SMI Value = 0x%X\n", SwContext.SwSmiInputValue));

    // Allocate EFI Reserved memory for the SmmCliApi protocol instance.
    Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, (sizeof(REG_CLI_COMMAND_ENTRY)*MAX_CLI_APIs_SUPPORTED), (VOID**)&SmmCliApiProto);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "XML_CLI: Error Allocating SmmCliApi Prtocol! \n"));
    } else {
      ZeroMem((VOID*)SmmCliApiProto, (sizeof(REG_CLI_COMMAND_ENTRY)*MAX_CLI_APIs_SUPPORTED));  //Clear data
      Handle = NULL;
      Status = mSmst->SmmInstallProtocolInterface(&Handle, &gSmmCliApiGuid, EFI_NATIVE_INTERFACE, SmmCliApiProto);
      if (EFI_ERROR(Status))
      {
        DEBUG ((DEBUG_INFO, "XML_CLI: Error Publishing SmmCliApiProto! \n"));
        mSmst->SmmFreePool(SmmCliApiProto);
        return EFI_SUCCESS;
      }
      //DEBUG ((DEBUG_INFO, "XML_CLI: SmmCliApi Protocol published at 0x%X.\n", SmmCliApiProto));
      ProtocolPtr.XmlCliCommon->SmmCliApiProto = SmmCliApiProto;
      ProtocolPtr.XmlCliCommon->XmlCliApi.CliSmmRegisterApi = CliSmmRegisterApi;
    }
  }
  return EFI_SUCCESS;
}

/**
  Registers Register Cli Api Protocol to Smm

  @param[in] CommandId      Command Id
  @param[in] ClicmdHandler  Method to Invoke Valid Cli Command to call
  @param[in] Buffer         Input Buffer

  @retval  VOID
**/
VOID
CliSmmRegisterApi (
  IN UINT16                CommandId,
  IN CLI_CMD_HANDLER_FUNC  ClicmdHandler,
  IN VOID                  *Buffer
  )
{
  UINT8                  count;
  EFI_STATUS             Status;
  REG_CLI_COMMAND_ENTRY  *SmmCliApiProto=NULL;
  REG_CLI_COMMAND_ENTRY  *TempCmdEntryPtr=NULL;

  Status = ((EFI_SMM_SYSTEM_TABLE2*)ProtocolPtr.XmlCliCommon->Smst)->SmmLocateProtocol(&gSmmCliApiGuid, NULL, (VOID **)&SmmCliApiProto);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "XML_CLI: Error locating SmmCliApiProto! \n"));
    return;
  }
  if (SmmCliApiProto == NULL) {
    return;
  }
  TempCmdEntryPtr = SmmCliApiProto;
  for (count = 0; count < MAX_CLI_APIs_SUPPORTED; count++) {
    if ((TempCmdEntryPtr->CommandId == 0) && (TempCmdEntryPtr->CmdHandler == NULL)) {
      TempCmdEntryPtr->CommandId  = CommandId;
      TempCmdEntryPtr->CmdHandler = ClicmdHandler;
      TempCmdEntryPtr->Buffer     = Buffer;
      return;
    }
    TempCmdEntryPtr++;
  }
}

/**
  Handles Cli Callback function entry.

  @param[in]     DispatchHandle   EFI dispatch handle
  @param[in]     DispatchContext  Software SMM Register Context
  @param[in,out] CommBuffer       Communication buffer pointer
  @param[in,out] CommBufferSize   Size of the communication buffer

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
SmmCliEntryCallback (
  IN EFI_HANDLE                         DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext, OPTIONAL
  IN OUT VOID                           *CommBuffer,      OPTIONAL
  IN OUT UINTN                          *CommBufferSize   OPTIONAL
  )
{
  UINT32   Signature=0;

  Signature = ((CLI_BUFFER*)(UINTN)ProtocolPtr.XmlCliCommon->CliRequestBufferAddress)->Signature;
  if ((Signature == CLI_GEN2_SGN_REQUEST_READY) || (Signature == CLI_SGN_REQUEST_READY)) { // Enter Only if Req Buff Signature is valid, Optimized Call
    if (CompareMem((VOID*)ProtocolPtr.Digest, (VOID*)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET), DIGEST_SIZE) == 0) {
      ZeroMem((VOID*)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET), DIGEST_SIZE);
      if (SmmIsBufferOutsideSmmValid(ProtocolPtr.XmlCliCommon->CliResponseBufferAddress, CLI_BUFFER_SIZE) &&
      SmmIsBufferOutsideSmmValid(ProtocolPtr.XmlCliCommon->CliRequestBufferAddress, CLI_BUFFER_SIZE)) {
        CliEntryPoint((VOID*)ProtocolPtr.XmlCliCommon);
      }
      *(UINT64*)(UINTN)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET) = XMLCLI_REQUEST_VALIDATED_SIG1;
      *(UINT64*)(UINTN)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET + sizeof(UINT64)) = XMLCLI_REQUEST_VALIDATED_SIG2;
    }
  }
  return EFI_SUCCESS;
}

/**
  Call back function handles XmlCli variable change

  @param[in]     DispatchHandle   EFI dispatch handle
  @param[in]     DispatchContext  Software SMM Register Context
  @param[in,out] CommBuffer       Communication buffer pointer
  @param[in,out] CommBufferSize   Size of the communication buffer

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
XmlCliCommonSmmEnableCallback (
  IN EFI_HANDLE                         DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext, OPTIONAL
  IN OUT VOID                           *CommBuffer,      OPTIONAL
  IN OUT UINTN                          *CommBufferSize   OPTIONAL
  )
{
  BOOLEAN     CreateVar;
  BOOLEAN     HashKeyValid=FALSE;
  UINT8       XmlCliVar;
  UINT32      Attributes;
  UINTN       VariableSize;
  EFI_STATUS  Status;

  if (CompareMem((VOID*)ProtocolPtr.Digest, (VOID*)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET), DIGEST_SIZE) == 0) {
    ZeroMem((VOID*)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET), DIGEST_SIZE);
    HashKeyValid = TRUE;
  } else {
    return EFI_SUCCESS;
  }
  if (ProtocolPtr.XmlCliSupport == 0){
    CreateVar = TRUE;
  } else {
    CreateVar = FALSE;
  }
  if (CreateVar){
    VariableSize = sizeof(XmlCliVar);
    Status = ((EFI_SMM_VARIABLE_PROTOCOL*)ProtocolPtr.SmmVariable)->SmmGetVariable(L"XmlCli", &gXmlCliVarGuid, &Attributes, &VariableSize, &XmlCliVar);
    if (EFI_ERROR (Status) && HashKeyValid) {
      //
      // User delivered Hashing matches, now create the XmlCli Variable to enable XmlCli support in BIOS.
      //
      XmlCliVar = 1;
      VariableSize = sizeof(XmlCliVar);
      Attributes = (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE);
      Status = ((EFI_SMM_VARIABLE_PROTOCOL*)ProtocolPtr.SmmVariable)->SmmSetVariable(L"XmlCli", &gXmlCliVarGuid, Attributes, VariableSize, &XmlCliVar);
      *(UINT64*)(UINTN)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET) = XMLCLI_REQUEST_VALIDATED_SIG1;
      *(UINT64*)(UINTN)((UINTN)ProtocolPtr.XmlCliTmpBuffPtr + DIGEST_OFFSET + sizeof(UINT64)) = XMLCLI_REQUEST_VALIDATED_SIG2;
    }
  }
  return EFI_SUCCESS;
}
