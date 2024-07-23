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

#ifndef _DISPLAY_WRAPPER_H_
#define _DISPLAY_WRAPPER_H_

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
  );

/**
  This function prints MEBx information string to screen.

  @param[in] MsgId         Message id

  @retval EFI_SUCCESS      Screen was initialized successfully
  @retval Others           Error has occured during screen initialization
**/
EFI_STATUS
MebxDisplayText (
  IN MEBX_MSG_ID Token
  );

/**
  This function prints MEBx error string to screen and to console.

  @param[in] MsgId         Message Id which will be displayed

  @retval EFI_SUCCESS      Text was displayed successfully
  @retval EFI_NOT_FOUND    The string specified by Token isn't present
**/
EFI_STATUS
MebxDisplayError (
  IN MEBX_MSG_ID  MsgId
  );

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
  );

/**
  This function gets user input.

  @param[out]  UserInput        User input

  @retval EFI_SUCCESS           Successfully read user input
  @retval EFI_NOT_READY         There was no input from an user.
  @retval EFI_INVALID_PARAMETER Invalid parameter
**/
EFI_STATUS
MebxReadUserInput (
  OUT MEBX_USER_INPUT   *UserInput
  );

/**
  This function clears a screen's content.

  @retval EFI_SUCCESS      The function succeeded
  @retval Others           Error has occurred
**/
EFI_STATUS
MebxClearScreen (
  VOID
  );

#endif // _DISPLAY_WRAPPER_H_
