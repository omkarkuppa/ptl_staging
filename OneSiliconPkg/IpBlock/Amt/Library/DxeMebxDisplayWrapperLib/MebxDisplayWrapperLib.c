/** @file
  Wrapper functions for MEBx display protocol functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/MebxDisplayProtocol.h>

/**
  This function configures screen.

  @param[in]  ScreenType   Screen type, the value can indicate the BIOS screen or MEBx screen.

  @retval EFI_SUCCESS      Screen was initialized successfully
  @retval Others           Error has occured during screen initialization
**/
EFI_STATUS
MebxConfigureScreen (
  IN BOOLEAN        ScreenType
  )
{
  MEBX_DISPLAY_PROTOCOL *MebxDisplayProtocol;
  EFI_STATUS            Status;

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (!EFI_ERROR (Status)) {
    Status = MebxDisplayProtocol->ConfigureScreen (MebxDisplayProtocol, ScreenType);
  }

  return Status;
}

/**
  This function prints MEBx information string to screen.

  @param[in] MsgId         Message id

  @retval EFI_SUCCESS      Screen was initialized successfully
  @retval Others           Error has occured during screen initialization
**/
EFI_STATUS
MebxDisplayText (
  IN MEBX_MSG_ID  MsgId
  )
{
  MEBX_DISPLAY_PROTOCOL *MebxDisplayProtocol;
  EFI_STATUS            Status;

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (!EFI_ERROR (Status)) {
    Status = MebxDisplayProtocol->DisplayText (MebxDisplayProtocol, MsgId);
  }

  return Status;
}

/**
  This function prints MEBx error string to screen and to console.

  @param[in] MsgId         Message Id which will be displayed

  @retval EFI_SUCCESS      Text was displayed successfully
  @retval EFI_NOT_FOUND    The string specified by Token isn't present
**/
EFI_STATUS
MebxDisplayError (
  IN MEBX_MSG_ID  MsgId
  )
{
  MEBX_DISPLAY_PROTOCOL *MebxDisplayProtocol;
  EFI_STATUS            Status;

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (!EFI_ERROR (Status)) {
    Status = MebxDisplayProtocol->DisplayError (MebxDisplayProtocol, MsgId, 0);
  }

  return Status;
}

/**
  This function prints MEBx error string with information about corrupted data entry.

  @param[in] ModuleId       Module id
  @param[in] VariableId     Variable id

  @retval EFI_SUCCESS       Text was displayed successfully
  @retval Others            Error has occurred during displaying text
**/
EFI_STATUS
MebxDisplayErrorId (
  IN UINT16 ModuleId,
  IN UINT16 VariableId
  )
{
  MEBX_DISPLAY_PROTOCOL *MebxDisplayProtocol;
  EFI_STATUS            Status;

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (!EFI_ERROR (Status)) {
    Status = MebxDisplayProtocol->DisplayError (
                                    MebxDisplayProtocol,
                                    CorruptedDataEntryStart | ((ModuleId << 8) & 0xFF00) | (VariableId & 0xFF),
                                    0
                                    );
  }

  return Status;
}

/**
  This function gets user input.

  @param[out]  UserInput        User input

  @retval EFI_SUCCESS           Successfully read user input or no input
  @retval EFI_INVALID_PARAMETER Invalid parameter
**/
EFI_STATUS
MebxReadUserInput (
  OUT MEBX_USER_INPUT   *UserInput
  )
{
  MEBX_DISPLAY_PROTOCOL *MebxDisplayProtocol;
  EFI_STATUS            Status;

  if (UserInput == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (!EFI_ERROR (Status)) {
    Status = MebxDisplayProtocol->GetUserInput (MebxDisplayProtocol, (MEBX_USER_INPUT*) UserInput);
  }

  return Status;
}

/**
  This function clears a screen's content.

  @retval EFI_SUCCESS      The function succeeded
  @retval Others           Error has occurred
**/
EFI_STATUS
MebxClearScreen (
  VOID
  )
{
  MEBX_DISPLAY_PROTOCOL *MebxDisplayProtocol;
  EFI_STATUS            Status;

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (!EFI_ERROR (Status)) {
    Status = MebxDisplayProtocol->ClearScreen (MebxDisplayProtocol);
  }

  return Status;
}