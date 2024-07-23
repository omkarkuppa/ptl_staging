/** @file
  Code implementing the CLI wrapper & other CLI API's.

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

#include <XmlCliComLib.h>
#include <Cli.h>


const CLI_COMMAND_ENTRY gCliCommandTable[] = {
  { READ_BIOS_KNOBS_OPCODE,     CliReadBiosKnobs,      FALSE},
  { APPEND_BIOS_KNOBS_OPCODE,   CliAppendBiosKnobs,    FALSE},
  { UPDATE_BIOS_KNOBS_OPCODE,   CliUpdateBiosKnobs,    FALSE},
  { RESTORE_MODIFY_KNOBS_OPCODE, CliRestoreModifyKnobs, FALSE},
  { LOAD_DEFAULT_KNOBS_OPCODE,  CliLoadDefaultKnobs,   FALSE},
  { GET_SET_VARIABLE_OPCODE,    CliGetSetVariable,    TRUE },  // This function is supported for XmlCli Lite as well.
};


UINT8        InstallXmlCli=1;
CLI_BUFFER*  gCliPhyRequestBuffer;
CLI_BUFFER*  gCliPhyResponseBuffer;
CLI_BUFFER*  gCliSharedRequestBuffer;
CLI_BUFFER*  gCliSharedResponseBuffer;

/**
  Call Cli Handler for Valid Operation Code.
  Also registers dynamically handled Cli Api for SMM/DXE protocol

  @param[in] XmlCliCommon  XmlCli Common Structure
  @param[in] OpCode        Operation code

  @retval  EFI_UNSUPPORTED  Invalid Cli Command
**/
EFI_STATUS
CallHandler (
  IN XML_CLI_COMMON  *XmlCliCommon,
  IN UINT16          OpCode
  );

/**
  Get command id from Physical Request Buffer

  @param[in] XmlCliCommon  XmlCli Common Structure

  @retval  UINT16  Request command id
**/
UINT16
GetRequest (
  XML_CLI_COMMON  *XmlCliCommon
  );

/**
  Entry point method to validate request/response buffer
  and perform operation for given valid operation code as per
  validated request buffer.

  @param[in] XmlCliCom  XmlCli Common Structure
                        Defined as VOID so that anyone can use this as
                        API without consuming while XML_CLI_COMMON structure

  @retval  EFI_SUCCESS
  @retval  EFI_NOT_READY          XmlCli Interface is either disabled or not Ready to service the request
  @retval  EFI_INVALID_PARAMETER  Null pointer passed as parameter
  @retval  EFI_NO_RESPONSE        Signature in Structure is still not in READY state
  @retval  EXIT_OPCODE            Specified Operation code in Request buffer match to EXIT_OPCODE
**/
EFI_STATUS
CliEntryPoint (
  IN VOID  *XmlCliCom
  )
{
  UINT16          opcode;
  UINT32          SetupDataAttributes;
  UINTN           SetupDataSize;
  EFI_STATUS      Status;
  XMLCLI_SETUP    XmlCliSetup;
  XML_CLI_COMMON  *XmlCliCommon;

  if (XmlCliCom == NULL){
    return EFI_INVALID_PARAMETER;
  }

  XmlCliCommon = (XML_CLI_COMMON*) XmlCliCom;

  if ((InstallXmlCli != 0) && (XmlCliCommon != NULL)) {
    SetupDataAttributes=0;
    SetupDataSize = sizeof (XMLCLI_SETUP);
    Status = XmlCliGetNvramData(XmlCliCommon, XMLCLI_SETUP_NAME, &gXmlCliSetupGuid, &SetupDataAttributes, &SetupDataSize, &XmlCliSetup);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "XML_CLI: GetVariable on Setup failed !!\n"));
      return EFI_NOT_READY;
    }
    InstallXmlCli = XmlCliSetup.XmlCliSupport;

    gCliPhyRequestBuffer  = (VOID*)(UINTN)XmlCliCommon->CliRequestBufferAddress;
    gCliPhyResponseBuffer = (VOID*)(UINTN)XmlCliCommon->CliResponseBufferAddress;

    gCliSharedRequestBuffer  = gCliPhyRequestBuffer;
    gCliSharedResponseBuffer = gCliPhyResponseBuffer;

    if (WaitForCommand(XmlCliCommon) != EFI_SUCCESS) {
      return EFI_NO_RESPONSE;
    }

    opcode = GetRequest(XmlCliCommon);
    if (opcode == EXIT_OPCODE) {
      gCliPhyResponseBuffer->CommandId                        = EXIT_OPCODE;
      gCliPhyResponseBuffer->ParametersSize                   = 0;
      gCliPhyResponseBuffer->Flags.Fields.CommandSideEffects  = CLI_CMD_NO_SIDE_EFFECT;
      if (XmlCliCommon->CliGen2Enable) {
        gCliPhyResponseBuffer->Signature = CLI_GEN2_SGN_RESPONSE_READY;
      } else {
        gCliPhyResponseBuffer->Signature = CLI_SGN_RESPONSE_READY;
      }
      return EXIT_OPCODE;
    }
    Status = CallHandler(XmlCliCommon, opcode);
    if (Status != EFI_SUCCESS) {  // In some cases we Fail Silently, hence return Success.
      // DEBUG ((DEBUG_INFO, "XML_CLI: Current CLI command failed !!\n"));
      return EFI_SUCCESS;
    }
  } else {
    if (XmlCliCommon->CliGen2Enable) {
      CopyMem((VOID*)&SetupDataAttributes, (VOID*)(UINTN)((UINTN)XmlCliCommon->DramSharedMBAddr+LEG_MAILBOX_OFFSET+OFF_GEN2_XML_CLI_ENABLED), 4);
      SetupDataAttributes = SetupDataAttributes & 0xFFFFFFFD;   // Clear BIT 1, this indicates that XmlCli Interface is now Disabled
      CopyMem((VOID*)(UINTN)((UINTN)XmlCliCommon->DramSharedMBAddr+LEG_MAILBOX_OFFSET+OFF_GEN2_XML_CLI_ENABLED), (VOID*)&SetupDataAttributes, 4);
    }
  }
  return EFI_SUCCESS;
}

/**
  Call Cli Handler for Valid Operation Code.
  Also registers dynamically handled Cli Api for SMM/DXE protocol

  @param[in] XmlCliCommon  XmlCli Common Structure
  @param[in] OpCode        Operation code

  @retval  EFI_UNSUPPORTED  Invalid Cli Command
**/
EFI_STATUS
CallHandler (
  IN XML_CLI_COMMON  *XmlCliCommon,
  IN UINT16          OpCode
  )
{
  UINT32                 iter;
  UINT32                 count;
  UINT32                 tableSize;
  REG_CLI_COMMAND_ENTRY  *CliApiProtocol=NULL;

  tableSize = sizeof(gCliCommandTable)/sizeof(CLI_COMMAND_ENTRY);

  for (iter = 0; iter < tableSize; iter++) {
    if ((XmlCliCommon->EnableXmlCliLite == 1) && (gCliCommandTable[iter].XmlCliLiteSupport == FALSE)) {
      continue;
    }
    if (gCliCommandTable[iter].CommandId == OpCode){
      ZeroMem (gCliPhyResponseBuffer, sizeof(CLI_BUFFER));    // clear/initialize response buffer
      return gCliCommandTable[iter].CmdHandler(XmlCliCommon);
    }
  }

  // Handle CLI API's that were dynamically registered

  if (XmlCliExecuteInSmm()) {
    CliApiProtocol = (REG_CLI_COMMAND_ENTRY*)(XmlCliCommon->SmmCliApiProto);
  } else {
    CliApiProtocol = (REG_CLI_COMMAND_ENTRY*)(XmlCliCommon->DxeCliApiProto);
  }
  if (CliApiProtocol == NULL) {
    return EFI_UNSUPPORTED;
  }
  for (count=0; count<MAX_CLI_APIs_SUPPORTED; count++) {
    if ((CliApiProtocol->CommandId == 0) || (CliApiProtocol->CmdHandler == NULL)) {
      return EFI_UNSUPPORTED;
    } else {
      if (CliApiProtocol->CommandId == OpCode) {
        ZeroMem (gCliPhyResponseBuffer, sizeof(CLI_BUFFER));    // clear/initialize response buffer
        return CliApiProtocol->CmdHandler(CliApiProtocol->Buffer);
      }
    }
    CliApiProtocol++;
  }
  // no such a command
  return EFI_UNSUPPORTED;
}

/**
  Wait for Cli Command to execute successfully

  @param[in] XmlCliCommon  XmlCli Common Structure

  @retval EFI_SUCCESS      XmlCli Response is successful and Signature is Ready
  @retval EFI_NO_RESPONSE  XmlCli Response is not ready!
**/
EFI_STATUS
WaitForCommand (
  IN XML_CLI_COMMON  *XmlCliCommon
  )
{
  if (XmlCliCommon->CliGen2Enable) {
    if (gCliSharedRequestBuffer->Signature == CLI_GEN2_SGN_REQUEST_READY) {
      return EFI_SUCCESS;
    }
  } else {
    if (gCliSharedRequestBuffer->Signature == CLI_SGN_REQUEST_READY) {
      return EFI_SUCCESS;
    }
  }
  //never should be here
  return EFI_NO_RESPONSE;
}

/**
  Get command id from Physical Request Buffer

  @param[in] XmlCliCommon  XmlCli Common Structure

  @retval  UINT16  Request command id
**/
UINT16
GetRequest (
  IN XML_CLI_COMMON  *XmlCliCommon
  )
{
  if (XmlCliCommon->CliGen2Enable) {
    gCliSharedRequestBuffer->Signature = CLI_GEN2_SGN_RESPONSE_GET;
  } else {
    gCliSharedRequestBuffer->Signature = CLI_SGN_RESPONSE_GET;
  }
  return gCliPhyRequestBuffer->CommandId;
}

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliUpdateBiosKnobs (
  IN OUT XML_CLI_COMMON *XmlCliCommon
  )
{
  XmlCliCommon->CliRT.CommandSubType     = CLI_KNOB_APPEND;    // Set as append
  XmlCliCommon->CliRT.UpdateKnobsEnabled = TRUE;
  CliProcessBiosKnobs(XmlCliCommon);   // reuse this guy
  XmlCliCommon->CliRT.UpdateKnobsEnabled = FALSE;

  ((CLI_UPDATE_BIOS_KNOBS_RSP_PARAM*)gCliPhyResponseBuffer->Parameters)->RawAccess = 0;
  if ((gCliPhyResponseBuffer->Flags.Fields.CommandSideEffects == CLI_CMD_RESTART_REQUIRED) || (XmlCliCommon->CliRT.ResetRequired == 1))
  {
    ((CLI_UPDATE_BIOS_KNOBS_RSP_PARAM*)gCliPhyResponseBuffer->Parameters)->Status.SetupModified = 1;
    XmlCliCommon->CliRT.ResetRequired = 1;  // set the Global Variable, this will be cleared automatically after reboot
  }

  gCliPhyResponseBuffer->CommandId      = UPDATE_BIOS_KNOBS_OPCODE;
  gCliPhyResponseBuffer->ParametersSize = sizeof(CLI_UPDATE_BIOS_KNOBS_RSP_PARAM);
  if (XmlCliCommon->CliGen2Enable) {
    gCliPhyResponseBuffer->Signature = CLI_GEN2_SGN_RESPONSE_READY;
  } else {
    gCliPhyResponseBuffer->Signature = CLI_SGN_RESPONSE_READY;
  }

  return EFI_SUCCESS;
}

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliAppendBiosKnobs(
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  )
{
  XmlCliCommon->CliRT.CommandSubType = CLI_KNOB_APPEND;    // Set as append
  CliProcessBiosKnobs(XmlCliCommon);   // reuse this guy

  gCliPhyResponseBuffer->CommandId   = APPEND_BIOS_KNOBS_OPCODE;
  if (XmlCliCommon->CliGen2Enable) {
    gCliPhyResponseBuffer->Signature = CLI_GEN2_SGN_RESPONSE_READY;
  } else {
    gCliPhyResponseBuffer->Signature = CLI_SGN_RESPONSE_READY;
  }

  return EFI_SUCCESS;
}

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliRestoreModifyKnobs(
  IN OUT XML_CLI_COMMON *XmlCliCommon
  )
{
  XmlCliCommon->CliRT.CommandSubType = CLI_KNOB_RESTORE_MODIFY;
  CliProcessBiosKnobs(XmlCliCommon);   // reuse this guy

  gCliPhyResponseBuffer->CommandId   = RESTORE_MODIFY_KNOBS_OPCODE;
  if (XmlCliCommon->CliGen2Enable) {
    gCliPhyResponseBuffer->Signature = CLI_GEN2_SGN_RESPONSE_READY;
  } else {
    gCliPhyResponseBuffer->Signature = CLI_SGN_RESPONSE_READY;
  }
  return EFI_SUCCESS;
}

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliReadBiosKnobs(
  IN OUT XML_CLI_COMMON *XmlCliCommon
  )
{
  XmlCliCommon->CliRT.CommandSubType = CLI_KNOB_READ_ONLY;
  CliProcessBiosKnobs(XmlCliCommon);   // reuse this guy

  gCliPhyResponseBuffer->CommandId   = READ_BIOS_KNOBS_OPCODE;
  if (XmlCliCommon->CliGen2Enable) {
    gCliPhyResponseBuffer->Signature = CLI_GEN2_SGN_RESPONSE_READY;
  } else {
    gCliPhyResponseBuffer->Signature = CLI_SGN_RESPONSE_READY;
  }
  return EFI_SUCCESS;
}

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliLoadDefaultKnobs (
  IN OUT XML_CLI_COMMON *XmlCliCommon
  )
{
  XmlCliCommon->CliRT.CommandSubType = CLI_KNOB_LOAD_DEFAULTS;    // Set as append
  CliProcessBiosKnobs(XmlCliCommon);   // reuse this guy

  gCliPhyResponseBuffer->CommandId   = LOAD_DEFAULT_KNOBS_OPCODE;
  if (XmlCliCommon->CliGen2Enable) {
    gCliPhyResponseBuffer->Signature = CLI_GEN2_SGN_RESPONSE_READY;
  } else {
    gCliPhyResponseBuffer->Signature = CLI_SGN_RESPONSE_READY;
  }
  return EFI_SUCCESS;
}

/**
  Get Knob Variable data from NVRAM

  @param[in] XmlCliCommon         XmlCli Common Structure
  @param[in] UniCodeName          Name of the variable in unicode character array
  @param[in] VarGuid              Guid of the NVRAM variable
  @param[in] SetupDataAttributes  Attribute of the NVRAM Variable
  @param[in] VarSize              Size of the NVRAM Variable
  @param[in] NvarData             NVRAM Data value (setup knob values)

  @retval  EFI_STATUS
**/
EFI_STATUS
XmlCliGetNvramData (
  IN XML_CLI_COMMON  *XmlCliCommon,
  IN CHAR16          *UniCodeName,
  IN EFI_GUID        *VarGuid,
  IN UINT32          *SetupDataAttributes,
  IN UINTN           *VarSize,
  IN VOID            *NvarData
  )
{
  EFI_STATUS  Status;


  if (XmlCliExecuteInSmm()) {
    Status  = ((EFI_SMM_VARIABLE_PROTOCOL*)XmlCliCommon->SmmVariable)->SmmGetVariable(UniCodeName, VarGuid, SetupDataAttributes, VarSize, NvarData);
  } else {
    Status  = gRT->GetVariable(UniCodeName, VarGuid, SetupDataAttributes, VarSize, NvarData);
  }
  return Status;
}

/**
  Set Knob Variable data from NVRAM

  @param[in] XmlCliCommon         XmlCli Common Structure
  @param[in] UniCodeName          Name of the variable in unicode character array
  @param[in] VarGuid              Guid of the NVRAM variable
  @param[in] SetupDataAttributes  Attribute of the NVRAM Variable
  @param[in] VarSize              Size of the NVRAM Variable
  @param[in] NvarData             NVRAM Data value (setup knob values)

  @retval  EFI_STATUS  Response Status of SetVariable/SmmSetVariable
**/
EFI_STATUS
XmlCliSetNvramData (
  IN XML_CLI_COMMON    *XmlCliCommon,
  IN CHAR16            *UniCodeName,
  IN EFI_GUID          *VarGuid,
  IN UINT32            SetupDataAttributes,
  IN UINTN             VarSize,
  IN VOID              *NvarData
  )
{
  EFI_STATUS  Status;


  if (XmlCliExecuteInSmm()) {
    Status  = ((EFI_SMM_VARIABLE_PROTOCOL*)XmlCliCommon->SmmVariable)->SmmSetVariable(UniCodeName, VarGuid, SetupDataAttributes, VarSize, NvarData);
  } else {
    Status  = gRT->SetVariable(UniCodeName, VarGuid, SetupDataAttributes, VarSize, NvarData);
  }
  return Status;
}

/**
  This function validates the requested knob value whether the
  knob type is valid knob type or not (valid EFI IFR code)

  @param[in] XmlCliCommon        XmlCli Common Structure
  @param[in] KnobXmlEntryOffset  Offset of the setup knob
  @param[in] ReqKnobValue        Value of Requested Knob

  @retval  TRUE   Requested Knob is within allowed range
                  (Valid EFI IFR code/Valid Knob type)
  @retval  FALSE  Requested Knob is not within allowed range
**/
BOOLEAN
IsKnobValValid(
  IN XML_CLI_COMMON  *XmlCliCommon,
  IN UINT32          KnobXmlEntryOffset,
  IN UINT8           *ReqKnobValue
  )
{
  BOOLEAN         ReturnStatus=FALSE;
  UINT8           KnobType=0;
  UINT8           NoOfOptions=0;
  UINT8           KnobSize=0;
  UINT8           LoopCount=0;
  UINT32          KnobValMapAddr;
  UINT32          KnobValMapOffset=0;
  UINTN           MinVal=0;
  UINTN           MaxVal=0;
  UINTN           ReqKnobValInt=0;
  KNOB_ENTRY_TBL  *KnobXmlEntry;

  KnobXmlEntry     = (KNOB_ENTRY_TBL*)(UINTN)XmlCliCommon->KnobXmlEntryAddr;
  KnobValMapAddr   = XmlCliCommon->KnobValMapAddr;
  KnobValMapOffset = (UINT32)KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset;
  KnobSize         = (UINT8)KnobXmlEntry[KnobXmlEntryOffset].KnobSize;
  CopyMem((VOID*)&KnobType, (VOID*)(UINTN)((UINTN)KnobValMapAddr+KnobValMapOffset), 1);
  if (KnobType < KNOB_TYPE_CUTOFF) {
    //
    // if EFI_IFR_ONE_OF_OP
    //
    NoOfOptions = KnobType;
    KnobType = EFI_IFR_ONE_OF_OP;
    for (LoopCount=0; LoopCount < NoOfOptions; LoopCount++) {
      if (CompareMem((UINT8*)ReqKnobValue, (UINT8*)(UINTN)((UINTN)KnobValMapAddr+KnobValMapOffset+1+(LoopCount*KnobSize)), KnobSize) == 0) {
        ReturnStatus = TRUE;  // this means the requested value is found in one of the supported option values
        break;
      }
    }
  } else {
    KnobType = (KnobType - KNOB_TYPE_CUTOFF);   // we added value 0xD0 when we encoded it while generating XML, undoing the same now
    switch(KnobType) {
      case EFI_IFR_CHECKBOX_OP:
        CopyMem((VOID*)&ReqKnobValInt, (VOID*)ReqKnobValue, KnobSize);
        // checkbox can take value 0 or 1 only.
        if ((ReqKnobValInt == 0) || (ReqKnobValInt == 1)) {
          ReturnStatus = TRUE;  // this means the requested value is either 0 or 1
        } else {
          ReturnStatus = FALSE; // this means the requested value is the one not supported or viable for CheckBox option
        }
      break;
      case EFI_IFR_NUMERIC_OP:
        CopyMem((VOID*)&ReqKnobValInt, (VOID*)ReqKnobValue, KnobSize);
        CopyMem((VOID*)&MinVal, (VOID*)(UINTN)((UINTN)KnobValMapAddr+KnobValMapOffset+1), KnobSize);
        CopyMem((VOID*)&MaxVal, (VOID*)(UINTN)((UINTN)KnobValMapAddr+KnobValMapOffset+1+KnobSize), KnobSize);
        if ((ReqKnobValInt >= MinVal) && (ReqKnobValInt <= MaxVal)) {
          ReturnStatus = TRUE;  // this means the requested value is within the allowed range
        } else {
          ReturnStatus = FALSE; // this means the requested value is outside the allowed range
        }
      break;
      case EFI_IFR_STRING_OP:
        CopyMem((VOID*)&MinVal, (VOID*)(UINTN)((UINTN)KnobValMapAddr+KnobValMapOffset+1), sizeof(UINT8));
        CopyMem((VOID*)&MaxVal, (VOID*)(UINTN)((UINTN)KnobValMapAddr+KnobValMapOffset+2), sizeof(UINT8));
        while (*ReqKnobValue) {
          ReqKnobValue = ReqKnobValue+2;
          ReqKnobValInt++;
        }
        if ((ReqKnobValInt >= MinVal) && (ReqKnobValInt <= MaxVal)) {
          ReturnStatus = TRUE;  // this means the requested value is within the allowed range
        } else {
          ReturnStatus = FALSE; // this means the requested value is outside the allowed range
        }
      break;
    }
  }
  //DEBUG ((DEBUG_INFO, "XML_CLI: KnobType = 0x%X: KnobSize = %d IsKnobValValid = %a  ReqVal = 0x%X\n", KnobType, KnobSize, ReturnStatus? "TRUE":"FALSE", ReqKnobValue));
  return ReturnStatus;
}

/**
  This function validates the requested knob value and if the
  requested value is within the allowed range this function
  will return TRUE, else will return FALSE

  @param[in] XmlCliCommon       XmlCli Common Structure
  @param[in] ProcBiosKnobsRsp   Processed Bios Knobs Response Buffer
  @param[in] KnobIndex          Offset of the Knob within Nvar
  @param[in] VarId              Id of Nvar
  @param[in] KnobXmlNvarOffset  Nvar Knob Offset within Xml
  @param[in] PreValueData       Existing Data before replaced with valid requested value
  @param[in] ReqKnobValue       Requested Value of setup knob

**/
VOID
ReportReplaceKnobData (
  IN XML_CLI_COMMON  *XmlCliCommon,
  IN VOID            *ProcBiosKnobsRsp,
  IN UINT32          KnobIndex,
  IN UINT16          VarId,
  IN UINTN           KnobXmlNvarOffset,
  IN UINT8           *PreValueData,
  IN UINT8           *ReqKnobValue
  )
{
  BOOLEAN                           BitWise;
  UINT8                             iter;
  UINT8                             KnobSize;
  UINT8                             CorrectIndex;
  UINT8                             CorrectOffset;
  UINT16                            count;
  UINT16                            found;
  UINT16                            KnobEntrySize;
  UINT32                            KnobOffset;
  UINT32                            GbtXmlBaseAddr;
  UINT32                            KnobEntryOfst;
  UINT32                            XmlPatchBaseAddr;
  UINT32                            XmdBuffSize;
  UINT64                            Value;
  UINT64                            Data64;
  UINTN                             Address;
  CHAR8                             StrAsciiBuf[200];
  KNOB_ENTRY_TBL                    *KnobXmlEntry;
  BIT_KNOB_INDEX_LOOKUP             *BitKnobLookup;
  CLI_PROCESS_BIOS_KNOBS_RSP_PARAM  *ProcessBiosKnobsRsp;

  ProcessBiosKnobsRsp = (CLI_PROCESS_BIOS_KNOBS_RSP_PARAM*)ProcBiosKnobsRsp;
  GbtXmlBaseAddr      = XmlCliCommon->GbtXmlAddress + 4;
  XmlPatchBaseAddr    = XmlCliCommon->XmlPatchBaseAddr;
  KnobXmlEntry        = (KNOB_ENTRY_TBL*)(UINTN)XmlCliCommon->KnobXmlEntryAddr;
  KnobSize            = (UINT8)KnobXmlEntry[KnobXmlNvarOffset+KnobIndex].KnobSize;
  KnobEntryOfst       = (UINT32)(KnobXmlEntry[KnobXmlNvarOffset+KnobIndex].KnobXmlEntryOfst);
  KnobEntrySize       = (UINT16)KnobXmlEntry[KnobXmlNvarOffset+KnobIndex].XmlEntrySize;

  //DEBUG ((DEBUG_INFO, "Before Calling ReplaceCurVal: Offset = 0x%X VarId = %d EntryOfst = 0x%X\n", KnobIndex, VarId, KnobEntryOfst));
  //DEBUG ((DEBUG_INFO, "EntrySize =0x%X KnobSize = %d  Val = 0x%X XmlPatchBaseAddr = 0x%X\n", KnobEntrySize, KnobSize, ReqKnobValue, XmlCliCommon->XmlPatchBaseAddr));
  //ReplaceCurrentVal(GbtXmlBaseAddr, KnobIndex, VarId, KnobEntryOfst, KnobEntrySize, KnobSize, ReqKnobValue, XmlCliCommon->XmlPatchBaseAddr);
  Address          = (UINTN)((UINTN)GbtXmlBaseAddr+KnobEntryOfst);
  XmdBuffSize      = 0;
  Data64           = 0;
  CorrectIndex     = 0;
  CorrectOffset    = 0;
  found            = 0;
  BitWise          = FALSE;
  if ((Address != 0) && (KnobEntrySize != 0)) {
    for (count = 0; count < KnobEntrySize; count++) {
      Value = *(UINT64*)Address;
      /// compare with <eIndex=">
      if (Value == 0x223D7865646e4965) {
        Address += 8;
        ZeroMem ((UINT8*)StrAsciiBuf, sizeof(StrAsciiBuf));
        StrAsciiBuf[0] = *(CHAR8*)Address;
        StrAsciiBuf[1] = *(CHAR8*)(Address+1);
        CorrectIndex = ((UINT8)AsciiStrDecimalToUintn(StrAsciiBuf) == VarId) ? 1 : 0;
        Address += 2;
      }
      /// compare with <offset=">
      if (CorrectIndex && (Value == 0x223D74657366666f)) {
        Address += 8;
        ZeroMem ((UINT8*)StrAsciiBuf, sizeof(StrAsciiBuf));
        for (iter = 0 ; iter < 8 ; iter++) {
          if (*(UINT8*)(Address + iter) == 0x22) {
            break;
          }
          StrAsciiBuf[iter] = *(CHAR8*)(Address + iter);
        }
        KnobOffset = KnobIndex;
        Value = AsciiStrHexToUintn(StrAsciiBuf);
        if (Value >= 0xC0000) {
          Value = Value & 0x3FFFF;
          if (KnobXmlEntry[KnobXmlNvarOffset+KnobIndex].IsBitWise) {
            BitWise = TRUE;
            BitKnobLookup = (BIT_KNOB_INDEX_LOOKUP*)(UINTN)((UINTN)XmlCliCommon->KnobValMapAddr + KnobXmlEntry[KnobXmlNvarOffset+KnobIndex].KnobValMapOffset - sizeof(BIT_KNOB_INDEX_LOOKUP));
            KnobOffset = (BitKnobLookup->ByteOffset*8) + (BitKnobLookup->BitField & 0x7);
          }
        }
        CorrectOffset = (Value == KnobOffset) ? 1 : 0;
        Address += iter;
      }
      // compare with <CurrentV>
      if (CorrectOffset && (Value == 0x56746E6572727543)) {
        Address += 8;
        Value = (*(UINT64*)Address) & 0xFFFFFFFFFFFF;
        // // compare with <al="0x>
        if (Value == 0x7830223D6C61) {
          Address += 6;
          found = 1;
          break;
        }
      }
      Address ++;
    }
    if (found) {
      if (XmlPatchBaseAddr != 0) {
        Data64 = (*(UINT64*)(UINTN)XmlPatchBaseAddr) & 0xFFFFFFFFFF;
        // compare with $XKDT
        if (Data64 == 0x54444B5824) {
          CopyMem((UINT8*)&XmdBuffSize, (UINT8*)(UINTN)((UINTN)XmlPatchBaseAddr+5), 3);
          CopyMem((UINT8*)(((KNOB_PATCH_TBL*)(UINTN)((UINTN)XmlPatchBaseAddr+8+XmdBuffSize))->KnobXmlEntryOfst), (UINT8*)&KnobEntryOfst, 3);
          ((KNOB_PATCH_TBL*)(UINTN)((UINTN)XmlPatchBaseAddr+8+XmdBuffSize))->CurrentVal.RelOffset = (UINT16)(Address - GbtXmlBaseAddr - KnobEntryOfst);
          ((KNOB_PATCH_TBL*)(UINTN)((UINTN)XmlPatchBaseAddr+8+XmdBuffSize))->KnobSize = KnobSize;
          CopyMem((UINT8*)(UINTN)((UINTN)XmlPatchBaseAddr+8+XmdBuffSize+sizeof(KNOB_PATCH_TBL)), (UINT8*)ReqKnobValue, KnobSize);
          XmdBuffSize = XmdBuffSize + sizeof(KNOB_PATCH_TBL) + KnobSize;
          CopyMem((UINT8*)(UINTN)((UINTN)XmlPatchBaseAddr+5), (UINT8*)&XmdBuffSize, 3);
        }
      }
      ZeroMem ((UINT8*)StrAsciiBuf, sizeof(StrAsciiBuf));
      ConvHexArray2asciiVal(StrAsciiBuf, ReqKnobValue, KnobSize);
      CopyMem((UINT8*)Address, (UINT8*)StrAsciiBuf, (KnobSize*2));
    }
  }

  if (ProcessBiosKnobsRsp != NULL) {
    ProcessBiosKnobsRsp->KnobXmlEntryPtr  = GbtXmlBaseAddr+KnobEntryOfst;
    ProcessBiosKnobsRsp->KnobXmlEntrySize = KnobEntrySize;
    ProcessBiosKnobsRsp->VarStoreIndex    = (UINT8)VarId;
    if (BitWise) {
      ProcessBiosKnobsRsp->KnobOffset     = (UINT16)(BitKnobLookup->ByteOffset | 0x8000);
      ProcessBiosKnobsRsp->KnobSize       = BitKnobLookup->BitField;
    } else {
      ProcessBiosKnobsRsp->KnobOffset     = (UINT16)KnobIndex;
      ProcessBiosKnobsRsp->KnobSize       = KnobSize;
    }
    ///
    /// Copy Default Knob Value
    ///
    CopyMem(((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1),(UINT8*)PreValueData, KnobSize);

    ///
    /// Copy Current Knob Value
    ///
    CopyMem(((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1+KnobSize),(UINT8*)ReqKnobValue, KnobSize);
  }
}

/**
  XmlCli Function to Process the BIOS Knobs.
  This method is responsible to process the request buffer and determine
  requested operation to perform which could be reading, modifying the knob(s)
  or restoring it to default values.

  @param[in,out] XmlCliCommon  XmlCli Common Structure

**/
EFI_STATUS
CliProcessBiosKnobs (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  )
{
  BOOLEAN                            BitWise;
  UINT8                              Index;
  UINT8                              CommandSubType;
  UINT8                              KnobSize=0;
  UINT8                              CurrVarID;
  UINT8                              CurrData[MAX_KNOB_SIZE_SUPPORTED];
  UINT8                              ReqKnobValue[MAX_KNOB_SIZE_SUPPORTED];
  UINT8                              CurVarList[MAX_KNOB_SIZE_SUPPORTED];
  UINT16                             BitWiseIndex;
  UINT16                             count;
  UINT16                             Offset;
  CHAR16                             UniCodeName[64];
  UINT32                             KnobXmlEntryOffset;
  UINT32                             SetupDataAttributes;
  UINTN                              KnobEntry;
  UINTN                              Modified=0;
  UINTN                              SetupModified=0;
  UINTN                              loopCount=0;
  UINTN                              KnobXmlNvarOffset;
  UINTN                              CurrKnobOffset;
  UINTN                              VarSize;
  UINT64                             Data64;
  UINT64                             TempData64;
  EFI_GUID                           VarGuid;
  EFI_STATUS                         Status=EFI_SUCCESS;
  KNOB_ENTRY_TBL                     *KnobXmlEntry=(KNOB_ENTRY_TBL*)(UINTN)XmlCliCommon->KnobXmlEntryAddr;
  BIT_KNOB_INDEX_LOOKUP              *BitKnobLookup;
  CLI_PROCESS_BIOS_KNOBS_RQST_PARAM  *ProcessKnobsCmd;
  CLI_PROCESS_BIOS_KNOBS_RSP_PARAM   *ProcessBiosKnobsRsp;
  VOID                               *SetupData;
  VOID                               *DefSetupData;
  VOID                               *CurrentData;

  BitWiseIndex           = 0;
  KnobXmlEntryOffset     = 0;

  ProcessBiosKnobsRsp = (CLI_PROCESS_BIOS_KNOBS_RSP_PARAM*)(gCliPhyResponseBuffer->Parameters);
  CommandSubType = XmlCliCommon->CliRT.CommandSubType;
  ZeroMem (CurVarList, sizeof (CurVarList));

  ///
  /// Check that we have knobs to set in the command parameters by reading the 'ParametersSize'
  ///
  if ((gCliPhyRequestBuffer->ParametersSize == 0) && (CommandSubType != CLI_KNOB_LOAD_DEFAULTS) && (CommandSubType != CLI_KNOB_RESTORE_MODIFY)) {
    gCliPhyResponseBuffer->Flags.Fields.WrongParameter = 1;
    return EFI_SUCCESS;
  }

  if (XmlCliCommon->CliRT.UpdateKnobsEnabled == TRUE) {
    loopCount = gCliPhyRequestBuffer->ParametersSize / sizeof(CLI_UPDATE_BIOS_KNOBS_RQST_PARAM);
  } else {
    loopCount = gCliPhyRequestBuffer->ParametersSize;
  }

  // Check & parse input parameters for optimized operation
  ProcessKnobsCmd = (CLI_PROCESS_BIOS_KNOBS_RQST_PARAM*)gCliPhyRequestBuffer->Parameters;
  for (count=0; count < XmlCliCommon->NumberOfVarstores; count++) {
    if ((CommandSubType == CLI_KNOB_LOAD_DEFAULTS) || (CommandSubType == CLI_KNOB_RESTORE_MODIFY)) {
     CurVarList[count] = 0xE9;
    } else {
     CurVarList[count] = 0xFF;
    }
  }
  for (KnobEntry=0; KnobEntry < loopCount; KnobEntry++) {
    if (XmlCliCommon->CliRT.UpdateKnobsEnabled == TRUE) {
      KnobSize = sizeof(UINT64);
    } else {
      KnobSize = ProcessKnobsCmd->KnobSize;
      if (ProcessKnobsCmd->KnobOffset & 0x8000) {
        KnobSize = BitGetByteSize((KnobSize & 0x7), (KnobSize >> 3));
      }
    }
    if (CurVarList[ProcessKnobsCmd->VarStoreIndex] == 0xFF) {
      CurVarList[ProcessKnobsCmd->VarStoreIndex] = 0xE9;
    }
    ProcessKnobsCmd = (CLI_PROCESS_BIOS_KNOBS_RQST_PARAM*)((UINT8*)&ProcessKnobsCmd->KnobSize+1+KnobSize);
  }

  for (Index=0; Index < XmlCliCommon->NumberOfVarstores; Index++) {
    if (((CommandSubType == CLI_KNOB_APPEND) || (CommandSubType == CLI_KNOB_READ_ONLY)) && (CurVarList[Index] != 0xE9)) {
      continue;
    }
    ProcessKnobsCmd = (CLI_PROCESS_BIOS_KNOBS_RQST_PARAM*)gCliPhyRequestBuffer->Parameters;

    KnobXmlNvarOffset = XmlCliCommon->VarstoreTable[Index].KnobXmlEntryBase;

    VarSize = XmlCliCommon->VarstoreTable[Index].Size;
    SetupDataAttributes = 0;
    DefSetupData = (VOID*)(UINTN)XmlCliCommon->VarstoreTable[Index].DefData;
    SetupData = (VOID*)(UINTN)XmlCliCommon->CurrGetSetVarBuffer;
    CurrentData =  (VOID*)((UINT8*)SetupData + VarSize);   // Order of the combined Allocated buffer will be  || SetupData | CurrentData ||
    AsciiStrToUnicodeStrS ((CHAR8 *) XmlCliCommon->VarstoreTable[Index].Name, UniCodeName, sizeof (UniCodeName));
    VarGuid = XmlCliCommon->VarstoreTable[Index].Guid;
    Status = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, &VarSize, SetupData);
    if (EFI_ERROR (Status)) {
      Status = EFI_SUCCESS;
      continue;  // ignore these knobs and move to next NVAR
    }
    CopyMem ((UINT8*)CurrentData, (UINT8*)SetupData, VarSize); // Keep a copy of Current Setup Data for later use.
    if (XmlCliCommon->VarstoreTable[Index].DefName[0] == 0) {
      CopyMem ((UINT8*)DefSetupData, (UINT8*)SetupData, VarSize); //Default Var is not defined, use same as current copy.
    } else if ((CommandSubType == CLI_KNOB_RESTORE_MODIFY) || (CommandSubType == CLI_KNOB_LOAD_DEFAULTS)) {
      CopyMem ((UINT8*)SetupData, (UINT8*)DefSetupData, VarSize); // since we are restoring the options
    }

    if (CommandSubType == CLI_KNOB_LOAD_DEFAULTS) {
      loopCount = 0;  // dont iterate over the CLI Knobs request buffer loop
    }
    Modified = 0; // initialize it for current NVAR.

    for (KnobEntry=0; KnobEntry < loopCount; KnobEntry++) {
      CurrKnobOffset = ProcessKnobsCmd->KnobOffset;
      CurrVarID = ProcessKnobsCmd->VarStoreIndex;
      BitWise = FALSE;
      if (XmlCliCommon->CliRT.UpdateKnobsEnabled == TRUE) {
        KnobSize = sizeof(UINT64);
      } else {
        KnobSize = ProcessKnobsCmd->KnobSize;
        KnobXmlEntryOffset = (UINT32)(KnobXmlNvarOffset + CurrKnobOffset);
        if (CurrKnobOffset & 0x8000) {  // if BIT15 is set, this means current knob is BitWise
          BitWise = TRUE;
          CurrKnobOffset = CurrKnobOffset & 0x7FFF;
          BitWiseIndex = BitFetchIndex((UINT32)(UINTN)&KnobXmlEntry[KnobXmlNvarOffset], XmlCliCommon->KnobValMapAddr, (UINT16)CurrKnobOffset, KnobSize);
          KnobSize = BitGetByteSize((KnobSize & 0x7), (KnobSize >> 3));
          if (BitWiseIndex == 0xFFFF) {
            ProcessKnobsCmd = (CLI_PROCESS_BIOS_KNOBS_RQST_PARAM*)((UINT8*)&ProcessKnobsCmd->KnobSize+1+KnobSize);
            continue;  // invalid bitwise knob entry
          }
          KnobXmlEntryOffset = (UINT32)(KnobXmlNvarOffset + BitWiseIndex);
        }
      }

      //DEBUG ((DEBUG_INFO, "XML_CLI: ReqBuffPtr = 0x%X  ResBuffPtr = 0x%X  CurProcessingVarID = %d  \n", ProcessKnobsCmd, ProcessBiosKnobsRsp, Index));
      //DEBUG ((DEBUG_INFO, "XML_CLI: ReqVarID = %d  ReqKnobSize = 0x%X  ReqKnobOffset = 0x%X\n", CurrVarID, KnobSize, CurrKnobOffset));
      //DEBUG ((DEBUG_INFO, "KnobXmlEntryAddr = 0x%X KnobValMapAddr = 0x%X KnobValMapCurrOff = 0x%X\n", &KnobXmlEntry[KnobXmlEntryOffset], XmlCliCommon->KnobValMapAddr, (UINT32)KnobXmlEntry[KnobXmlEntryOffset].KnobValMapOffset));
      if ((CurrVarID != Index) || (CurrVarID >= XmlCliCommon->NumberOfVarstores)
          || ((KnobXmlEntry != NULL) && (KnobSize != (UINT8)KnobXmlEntry[KnobXmlEntryOffset].KnobSize))
          || (CurrKnobOffset > XmlCliCommon->VarstoreTable[ Index ].Size) ) {
        ProcessKnobsCmd = (CLI_PROCESS_BIOS_KNOBS_RQST_PARAM*)((UINT8*)&ProcessKnobsCmd->KnobSize+1+KnobSize);
        continue;  // process the rest, greacefully treat this error by ignoring this entry.
      }

      if ((XmlCliCommon->CliRT.UpdateKnobsEnabled != TRUE) && (KnobXmlEntry)) {
        ProcessBiosKnobsRsp->KnobXmlEntryPtr  = XmlCliCommon->GbtXmlAddress+4+(UINT32)KnobXmlEntry[KnobXmlEntryOffset].KnobXmlEntryOfst;
        ProcessBiosKnobsRsp->KnobXmlEntrySize = (UINT16)KnobXmlEntry[KnobXmlEntryOffset].XmlEntrySize;
        ProcessBiosKnobsRsp->KnobOffset       = ProcessKnobsCmd->KnobOffset;
        ProcessBiosKnobsRsp->VarStoreIndex    = CurrVarID;
        ProcessBiosKnobsRsp->KnobSize         = ProcessKnobsCmd->KnobSize;
      }

      ZeroMem((UINT8*)ReqKnobValue, sizeof(ReqKnobValue));
      CopyMem((UINT8*)ReqKnobValue, (UINT8*)&ProcessKnobsCmd->KnobSize+1, KnobSize);
      if ((CommandSubType == CLI_KNOB_APPEND) || (CommandSubType == CLI_KNOB_RESTORE_MODIFY)) {
        if (BitWise) {
          if (BitDataMatch64((VOID*)((UINT8*)SetupData+CurrKnobOffset),(VOID*)ReqKnobValue, (ProcessKnobsCmd->KnobSize & 0x7), (ProcessKnobsCmd->KnobSize >> 3), 0) == FALSE) {
            /// Not equal, need to update this knob
            if (IsKnobValValid(XmlCliCommon, KnobXmlEntryOffset, (UINT8*)ReqKnobValue)) {
              // Knob Value Validation
              BitDataCopy64((VOID*)((UINT8*)SetupData+CurrKnobOffset),(VOID*)ReqKnobValue, (ProcessKnobsCmd->KnobSize & 0x7), (ProcessKnobsCmd->KnobSize >> 3));
              if (CommandSubType == CLI_KNOB_APPEND) {
                ZeroMem((UINT8*)CurrData, sizeof(CurrData));
                CopyMem((UINT8*)CurrData, (UINT8*)CurrentData+CurrKnobOffset, KnobSize);
                ReportReplaceKnobData(XmlCliCommon, NULL, (UINT32)BitWiseIndex, Index, KnobXmlNvarOffset, CurrData, ReqKnobValue);
                Modified++;
              }
            }
          }
        } else {
          if (CompareMem((UINT8*)SetupData+CurrKnobOffset, (UINT8*)ReqKnobValue, KnobSize) != 0) {
            //not equal, need to apply this knob
            if (IsKnobValValid(XmlCliCommon, KnobXmlEntryOffset, (UINT8*)ReqKnobValue)) {
              // Knob Value Validation
              CopyMem((UINT8*)SetupData+CurrKnobOffset,(UINT8*)ReqKnobValue, KnobSize);
              if (CommandSubType == CLI_KNOB_APPEND) {
                ZeroMem ((UINT8*)CurrData, sizeof(CurrData));
                CopyMem((UINT8*)CurrData, (UINT8*)CurrentData+CurrKnobOffset, KnobSize);
                ReportReplaceKnobData(XmlCliCommon, NULL, (UINT32)CurrKnobOffset, Index, KnobXmlNvarOffset, CurrData, ReqKnobValue);
                Modified++;
              }
            }
          }
        }
      }

      if (!XmlCliCommon->CliRT.UpdateKnobsEnabled) {
        if (BitWise) {
          Data64 = 0;
          CopyMem((UINT8*)&Data64,(UINT8*)DefSetupData+CurrKnobOffset, KnobSize);  // copy Default Knob Value
          Data64 = BitExtractValue64(Data64, (ProcessKnobsCmd->KnobSize & 0x7), (ProcessKnobsCmd->KnobSize >> 3));
          CopyMem(((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1),(UINT8*)&Data64, KnobSize);  // copy Default Knob Value
          Data64 = 0;
          CopyMem((UINT8*)&Data64,(UINT8*)SetupData+CurrKnobOffset, KnobSize);  // copy Current Knob Value
          Data64 = BitExtractValue64(Data64, (ProcessKnobsCmd->KnobSize & 0x7), (ProcessKnobsCmd->KnobSize >> 3));
          CopyMem(((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1+KnobSize),(UINT8*)&Data64, KnobSize);  // copy Default Knob Value
        } else {
          CopyMem(((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1),(UINT8*)DefSetupData+CurrKnobOffset, KnobSize);  // copy Default Knob Value
          CopyMem(((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1+KnobSize),(UINT8*)SetupData+CurrKnobOffset, KnobSize);  // copy Current Knob Value
        }
        ProcessBiosKnobsRsp = (CLI_PROCESS_BIOS_KNOBS_RSP_PARAM*)((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1+(2*KnobSize));
      }
      ProcessKnobsCmd = (CLI_PROCESS_BIOS_KNOBS_RQST_PARAM*)((UINT8*)&ProcessKnobsCmd->KnobSize+1+KnobSize);
    }
    ///
    /// Now we have constructed buffer (SetupData) based on User expectation.
    ///

    if ((KnobXmlEntry != NULL) && ((CommandSubType == CLI_KNOB_RESTORE_MODIFY) || (CommandSubType == CLI_KNOB_LOAD_DEFAULTS))) {
      for (Offset=0; Offset < XmlCliCommon->VarstoreTable[Index].Size;) {
        if ((KnobXmlEntry[KnobXmlNvarOffset+Offset].KnobXmlEntryOfst == 0) || (KnobXmlEntry[KnobXmlNvarOffset+Offset].KnobSize == 0)) {
          Offset++;
          continue;
        }
        //DEBUG ((DEBUG_INFO, "XML_CLI: VarIndex: %d KnobXmlNvarOffset = 0x%X  KnobOffset = 0x%X\n", Index, KnobXmlNvarOffset, Offset));
        // compare the final buffer with existing values.
        if (CompareMem((UINT8*)SetupData+Offset,(UINT8*)CurrentData+Offset, (UINT8)KnobXmlEntry[KnobXmlNvarOffset+Offset].KnobSize) != 0) {
          ///
          /// Not equal, need to update this knobs CurrentVal in XML
          ///
          if (1 == KnobXmlEntry[KnobXmlNvarOffset+Offset].IsBitWise) {  // Bitwise Knob
            for (count = (UINT16)((KNOB_ENTRY_TBL_BITWISE*)KnobXmlEntry)[KnobXmlNvarOffset+Offset].StartIndex; count <= (UINT16)((KNOB_ENTRY_TBL_BITWISE*)KnobXmlEntry)[KnobXmlNvarOffset+Offset].EndIndex; count++) {
              if (0 == KnobXmlEntry[KnobXmlNvarOffset+count].IsBitWise) {
                continue;
              }
              BitKnobLookup = (BIT_KNOB_INDEX_LOOKUP*)(UINTN)((UINTN)XmlCliCommon->KnobValMapAddr + ((KNOB_ENTRY_TBL*)KnobXmlEntry)[KnobXmlNvarOffset+count].KnobValMapOffset - sizeof(BIT_KNOB_INDEX_LOOKUP));
              if (BitKnobLookup->ByteOffset == Offset){
                Data64 = 0;
                TempData64 = 0;
                CopyMem((UINT8*)&TempData64, (UINT8*)SetupData+Offset, (UINT8)KnobXmlEntry[KnobXmlNvarOffset+count].KnobSize);
                CopyMem((UINT8*)&Data64, (UINT8*)CurrentData+Offset, (UINT8)KnobXmlEntry[KnobXmlNvarOffset+count].KnobSize);
                if (!BitDataMatch64((VOID*)&TempData64,(VOID*)&Data64, (BitKnobLookup->BitField & 0x7), ((BitKnobLookup->BitField >> 3) & 0x3F), 1)) {
                  Data64 = BitExtractValue64(Data64, (BitKnobLookup->BitField & 0x7), ((BitKnobLookup->BitField >> 3) & 0x3F));
                  TempData64 = BitExtractValue64(TempData64, (BitKnobLookup->BitField & 0x7), ((BitKnobLookup->BitField >> 3) & 0x3F));
                  if (CommandSubType == CLI_KNOB_RESTORE_MODIFY) {
                    ReportReplaceKnobData(XmlCliCommon, NULL, (UINT32)count, Index, KnobXmlNvarOffset, (UINT8*)&Data64, (UINT8*)&TempData64);
                  } else {
                    ReportReplaceKnobData(XmlCliCommon, (VOID*)ProcessBiosKnobsRsp, (UINT32)count, Index, KnobXmlNvarOffset, (UINT8*)&Data64, (UINT8*)&TempData64);
                    ProcessBiosKnobsRsp = (CLI_PROCESS_BIOS_KNOBS_RSP_PARAM*)((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1+(2*KnobXmlEntry[KnobXmlNvarOffset+count].KnobSize));
                  }
                  Modified++;
                }
              }
            }
          } else {  // Regular Knob
            ZeroMem((UINT8*)ReqKnobValue, sizeof(ReqKnobValue));
            ZeroMem((UINT8*)CurrData, sizeof(CurrData));
            CopyMem((UINT8*)ReqKnobValue, (UINT8*)SetupData+Offset, (UINT8)KnobXmlEntry[KnobXmlNvarOffset+Offset].KnobSize);
            CopyMem((UINT8*)CurrData, (UINT8*)CurrentData+Offset, (UINT8)KnobXmlEntry[KnobXmlNvarOffset+Offset].KnobSize);
            if (CommandSubType == CLI_KNOB_RESTORE_MODIFY) {
              ReportReplaceKnobData(XmlCliCommon, NULL, (UINT32)Offset, Index, KnobXmlNvarOffset, CurrData, ReqKnobValue);
            } else {
              ReportReplaceKnobData(XmlCliCommon, (VOID*)ProcessBiosKnobsRsp, (UINT32)Offset, Index, KnobXmlNvarOffset, CurrData, ReqKnobValue);
              ProcessBiosKnobsRsp = (CLI_PROCESS_BIOS_KNOBS_RSP_PARAM*)((UINT8*)&ProcessBiosKnobsRsp->KnobSize+1+(2*KnobXmlEntry[KnobXmlNvarOffset+Offset].KnobSize));
            }
            Modified++;
          }
        }
        Offset += (UINT8)KnobXmlEntry[KnobXmlNvarOffset+Offset].KnobSize;
      }
    }
    if (Modified) {
      SetupModified += Modified;
      Status = XmlCliSetNvramData(XmlCliCommon, UniCodeName, &VarGuid, SetupDataAttributes, VarSize, SetupData);
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "XML_CLI: Cli Process Bios Knobs: SetVariable Failed Status = 0x%X!! \n", Status));
        gCliPhyResponseBuffer->Flags.Fields.CannotExecute = 1;
        gCliPhyResponseBuffer->Status = (UINT32)Status;
        return EFI_SUCCESS;
      }
      if (XmlCliCommon->XmlSkipSavingDefVar == TRUE) {  // Was Save Defaults Skipped?
        AsciiStrToUnicodeStrS ((CHAR8 *) XmlCliCommon->VarstoreTable[Index].DefName, UniCodeName, sizeof (UniCodeName));
        VarGuid = XmlCliCommon->VarstoreTable[Index].DefGuid;
        Status = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, (UINTN*)&VarSize, DefSetupData);
        if (EFI_ERROR(Status)) {  // Write current Data As default
          SetupDataAttributes = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS);
          Status = XmlCliSetNvramData(XmlCliCommon, UniCodeName, &VarGuid, SetupDataAttributes, VarSize, CurrentData);
          //DEBUG ((DEBUG_INFO, "XML_CLI: SetVariable on the L\"%a\" VAR\n", XmlCliCommon->VarstoreTable[Index].DefName));
        }
      }
      //DEBUG ((DEBUG_INFO, "XML_CLI: Cli Process Bios Knobs: Modified knobs count = %d modified for VarStore Index = %d\n", Modified, Index));
    }
  }

  gCliPhyResponseBuffer->Status = (UINT32)Status;
  gCliPhyResponseBuffer->Flags.Fields.CommandSideEffects = CLI_CMD_NO_SIDE_EFFECT;
  if (((gCliPhyResponseBuffer->Flags.Fields.WrongParameter == 0) &&
        (gCliPhyResponseBuffer->Flags.Fields.CannotExecute == 0) &&
        (SetupModified != 0))  || (XmlCliCommon->CliRT.ResetRequired == 1)) {
    gCliPhyResponseBuffer->Flags.Fields.CommandSideEffects = CLI_CMD_RESTART_REQUIRED;

    ///
    /// Set the Global Variable, this will be cleared automatically after reboot
    ///
    XmlCliCommon->CliRT.ResetRequired = 1;
  }
  // ((DEBUG_INFO, "XML_CLI: Cli Process Bios Knobs Ended: total Modified knobs count = %d \n", SetupModified));
  if (XmlCliCommon->CliRT.UpdateKnobsEnabled == TRUE) {
    return EFI_SUCCESS;
  }
  gCliPhyResponseBuffer->ParametersSize = (UINT32)((UINT8*)ProcessBiosKnobsRsp - (UINT8*)gCliPhyResponseBuffer->Parameters);

  return EFI_SUCCESS;
}

/**
  Perform Cli command to get-set variable based on the
  request and request parameters

  @param[in] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Successfully performed operation to get or set variable
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliGetSetVariable (
  IN XML_CLI_COMMON  *XmlCliCommon
  )
{
  BOOLEAN                     BitWise;
  UINT8                       BitOffset;
  UINT8                       BitSize;
  UINT8                       Operation;
  UINT8                       CurSize;
  UINT16                      LoopCount;
  UINT16                      VarCount;
  UINT16                      CurOffset;
  CHAR16                      UniCodeName[128];
  UINT32                      SetupDataAttributes;
  UINT32                      NvarNameLen;
  UINT32                      SetupModified;
  UINT32                      OverallStatus;
  UINTN                       VarSize;
  UINTN                       Currptr;
  UINTN                       Endptr;
  UINTN                       DataBufSize;
  EFI_GUID                    VarGuid;
  EFI_STATUS                  Status;
  VOID                        *SetupData;
  CLI_GET_SET_VARIABLE_PARAM  *params;
  CLI_GET_SET_VARIABLE_PARAM  *rsp;

  params = (CLI_GET_SET_VARIABLE_PARAM*)(gCliPhyRequestBuffer->Parameters);
  rsp    = (CLI_GET_SET_VARIABLE_PARAM*)(gCliPhyResponseBuffer->Parameters);
  LoopCount = (UINT8)gCliPhyRequestBuffer->ParametersSize;
  if (LoopCount == 0) {
    if (params->Operation == GET_ALL_VARS) {
      VarSize = sizeof(CHAR16);
      ZeroMem (UniCodeName, sizeof(UniCodeName));
      ZeroMem (&VarGuid, sizeof(EFI_GUID));
      while (TRUE) {
        if (XmlCliExecuteInSmm()) {
          Status = ((EFI_SMM_VARIABLE_PROTOCOL*)XmlCliCommon->SmmVariable)->SmmGetNextVariableName(&VarSize, UniCodeName, &VarGuid);
        } else {
          Status = gRT->GetNextVariableName(&VarSize, UniCodeName, &VarGuid);
        }
        if (Status == EFI_NOT_FOUND) {
          // Only break look when Variable list ends, for every other error Status, data is ignored
          break;
        }
        if (!EFI_ERROR (Status)) {
          params->NvarGuid = VarGuid;
          params->NvarAttribute = 0;
          params->NvarSize = 0;
          params->NvarRetStatus = 0;
          params->Operation = GET_VAR;
          UnicodeStrToAsciiStrS(UniCodeName, (CHAR8*)params->NvarName, (VarSize/2));
          params = (CLI_GET_SET_VARIABLE_PARAM*)(UINTN)((UINTN)params->NvarName + (VarSize/2));  // return VarName size is in sizeof(CHAR16)*bytes, includes null char, hence divide by 2
          LoopCount++;
        }
      }
      gCliPhyRequestBuffer->ParametersSize = LoopCount;
      params = (CLI_GET_SET_VARIABLE_PARAM*)(gCliPhyRequestBuffer->Parameters);
    }
  }
  SetupModified = 0;
  Status = EFI_INVALID_PARAMETER;
  OverallStatus = 0;
  for (VarCount=0; VarCount<LoopCount; VarCount++) {
    Operation = params->Operation;
    VarGuid = params->NvarGuid;
    VarSize = (UINTN)(params->NvarSize & 0xFFFFFFFF);
    SetupDataAttributes = params->NvarAttribute;
    ZeroMem (UniCodeName, sizeof(UniCodeName));
    AsciiStrToUnicodeStrS ((CHAR8*)params->NvarName, UniCodeName, sizeof (UniCodeName));
    AsciiStrCpyS(rsp->NvarName, 128, params->NvarName);
    NvarNameLen = (UINT32)AsciiStrSize(rsp->NvarName);
    SetupData = (VOID*)((UINTN)rsp->NvarName + NvarNameLen);

    //DEBUG ((DEBUG_INFO, "CliGetSetVariable: Name = %S Guid = %g Size, 0x%X  DataPtr = 0x%X\n", UniCodeName, VarGuid, VarSize, SetupData));
    if (Operation == GET_VAR) {
      if (VarSize == 0) { // GetVariable
        Status = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, &VarSize, NULL);
        if (EFI_ERROR(Status)) {
          DEBUG ((DEBUG_ERROR, "XmlCliGetNvramData Status = %r \n", Status));
        }
      }
      Status = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, &VarSize, SetupData);
      if (EFI_ERROR(Status)) {
          DEBUG ((DEBUG_ERROR, "XmlCliGetNvramData SetupData Status = %r \n", Status));
      }
    } else if (Operation == SET_VAR) { // SetVariable
      SetupData = (VOID*)((UINTN)params->NvarName + NvarNameLen);
      Status = XmlCliSetNvramData(XmlCliCommon, UniCodeName, &VarGuid, SetupDataAttributes, VarSize, SetupData);
      SetupModified++;
      if ((!EFI_ERROR(Status)) && (VarSize != 0)) {
        SetupData = (VOID*)((UINTN)rsp->NvarName + NvarNameLen);
        Status  = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, &VarSize, SetupData);
      }
    } else if (Operation == GET_MOD_SET_VAR) {
      DataBufSize = VarSize;
      VarSize = 0;
      Status  = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, &VarSize, NULL);
      if (VarSize != 0) {
        Status  = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, &VarSize, SetupData);
        if (!EFI_ERROR(Status)) {
          Currptr = (UINTN)params->NvarName + NvarNameLen;
          Endptr = Currptr + DataBufSize;
          for (; Currptr < Endptr;) {
            BitWise = FALSE;
            CurOffset = ((NVAR_DATA_BUFF*)Currptr)->Offset;
            CurSize = ((NVAR_DATA_BUFF*)Currptr)->Size;
            if (CurOffset & 0x8000) {  // if BIT15 is set, this means current knob is BitWise
              BitWise   = TRUE;
              CurOffset = CurOffset & 0x7FFF;
              BitOffset = (CurSize & 0x7);
              BitSize   = (CurSize >> 3);
              CurSize = BitGetByteSize(BitOffset, BitSize);
            }
            if ((CurOffset + CurSize) <= VarSize) {
              if (BitWise) {
                if (!BitDataMatch64((VOID*)((UINT8*)SetupData+CurOffset),(VOID*)((UINT8*)(Currptr+sizeof(NVAR_DATA_BUFF))), BitOffset, BitSize, 0)) {
                  /// Not equal, need to update this knob
                  BitDataCopy64((VOID*)((UINT8*)SetupData+CurOffset),(VOID*)((UINT8*)(Currptr+sizeof(NVAR_DATA_BUFF))), BitOffset, BitSize);
                  SetupModified++;
                }
              } else {
                if (CompareMem((UINT8*)SetupData+CurOffset, (UINT8*)(Currptr+sizeof(NVAR_DATA_BUFF)), CurSize) != 0) {
                  /// Not equal, need to update this knob
                  CopyMem((UINT8*)SetupData+CurOffset,(UINT8*)(Currptr+sizeof(NVAR_DATA_BUFF)), CurSize);
                  SetupModified++;
                }
              }
            }
            Currptr = Currptr + sizeof(NVAR_DATA_BUFF) + CurSize;
          }
          if (SetupModified > 0) {
            Status = XmlCliSetNvramData(XmlCliCommon, UniCodeName, &VarGuid, SetupDataAttributes, VarSize, SetupData);
            if ((!EFI_ERROR(Status)) && (VarSize != 0)) {
              Status  = XmlCliGetNvramData(XmlCliCommon, UniCodeName, &VarGuid, &SetupDataAttributes, &VarSize, SetupData);
            }
          }
        }
      }
    } else {
      Status = EFI_UNSUPPORTED;
    } //error case handling
    //DEBUG ((DEBUG_INFO, "CliGetSetVariable: Name = %S Guid = %g Size, 0x%X  DataPtr = 0x%X\n", UniCodeName, VarGuid, VarSize, SetupData));
    rsp->NvarSize  = (UINT32)VarSize;
    rsp->NvarAttribute = SetupDataAttributes;
    rsp->NvarGuid  = VarGuid;
    rsp->Operation = Operation;
    rsp->NvarRetStatus  = (UINT32)Status;
    if (Status != EFI_SUCCESS) {
      OverallStatus++;
    }
    if (Operation == GET_VAR) {
      params = (CLI_GET_SET_VARIABLE_PARAM*)(UINTN)((UINTN)params->NvarName + NvarNameLen);
    } else if (Operation == SET_VAR) {
      params = (CLI_GET_SET_VARIABLE_PARAM*)(UINTN)((UINTN)params->NvarName + NvarNameLen + VarSize);
    } else if (Operation == GET_MOD_SET_VAR) {
      params = (CLI_GET_SET_VARIABLE_PARAM*)(UINTN)((UINTN)params->NvarName + NvarNameLen + params->NvarSize);
    }
    rsp = (CLI_GET_SET_VARIABLE_PARAM*)(UINTN)((UINTN)rsp->NvarName + NvarNameLen + VarSize);
  }
  if (LoopCount == 1) {
    OverallStatus = (UINT32)Status;
  } else if (LoopCount != OverallStatus) {
    OverallStatus = 0;
  }
  if (SetupModified == 0) {
    gCliPhyResponseBuffer->Flags.Fields.CommandSideEffects = CLI_CMD_NO_SIDE_EFFECT;
  } else {
    gCliPhyResponseBuffer->Flags.Fields.CommandSideEffects = CLI_CMD_RESTART_REQUIRED;
  }
  gCliPhyResponseBuffer->CommandId      = GET_SET_VARIABLE_OPCODE;
  gCliPhyResponseBuffer->ParametersSize = (UINT32)((UINTN)rsp - (UINTN)gCliPhyResponseBuffer->Parameters);
  gCliPhyResponseBuffer->Status         = (UINT32) OverallStatus;;
  if (XmlCliCommon->CliGen2Enable) {
    gCliPhyResponseBuffer->Signature = CLI_GEN2_SGN_RESPONSE_READY;
  } else {
    gCliPhyResponseBuffer->Signature = CLI_SGN_RESPONSE_READY;
  }

  return EFI_SUCCESS;
}
