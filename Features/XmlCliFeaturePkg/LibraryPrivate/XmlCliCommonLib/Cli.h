/** @file
  Definitions for CLI interface/Wrapper

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

#ifndef _CLI_MODULE_H
#define _CLI_MODULE_H

#include <XmlCliComLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmVariable.h>


#define CLI_DELAY                       100
#define CLI_ASSERT                      EFI_DEADLOOP()

#define CLI_KNOB_APPEND                 0x0
#define CLI_KNOB_RESTORE_MODIFY         0x1
#define CLI_KNOB_READ_ONLY              0x2
#define CLI_KNOB_LOAD_DEFAULTS          0x3


#pragma pack(push, 1)

typedef EFI_STATUS (*cmdHandlerFunc)(XML_CLI_COMMON  *XmlCliCommon);

typedef struct {
  UINT16          CommandId;
  cmdHandlerFunc  CmdHandler;
  BOOLEAN         XmlCliLiteSupport;
} CLI_COMMAND_ENTRY;

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
  );

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliAppendBiosKnobs (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  );

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliRestoreModifyKnobs (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  );

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliReadBiosKnobs (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  );

/**
  This function update the NVRAM varstores according to
  varstoreIndex, KnobOffset, KnobSize and KnobValue

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  EFI_SUCCESS   Operation completed successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
CliLoadDefaultKnobs (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  );

/**
  XmlCli Function to Process the BIOS Knobs.
  This method is responsible to process the request buffer and determine
  requested operation to perform which could be reading, modifying the knob(s)
  or restoring it to default values.

  @param[in,out] XmlCliCommon  XmlCli Common Structure

**/
EFI_STATUS
CliProcessBiosKnobs (
  IN OUT XML_CLI_COMMON *XmlCliCommon
  );

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
  );

/**
  Wait for Cli Command to execute successfully

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval EFI_SUCCESS      XmlCli Response is successful
  @retval EFI_NO_RESPONSE  XmlCli Response is not ready!
**/
EFI_STATUS
WaitForCommand (
  IN XML_CLI_COMMON  *XmlCliCommon
  );

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
  );


#pragma pack(pop)
#endif //_CLI_MODULE_H
