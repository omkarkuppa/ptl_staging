/** @file
  User Consent bitmap conversion and display definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#ifndef _MEBX_USER_CONSENT_H_
#define _MEBX_USER_CONSENT_H_


/**
  This function checks if User Consent flow should be executed

  @retval TRUE    User Consent flow should be executed
  @retval FALSE   User Consent flow shouldn't be executed, this includes also a situation where query failed due to HECI error
**/
BOOLEAN
IsUserConsentRequired (
  VOID
  );

/**
  Function reads bitmap from AMT and displays it as USER CONSENT. It continuosly scans keyboard for user input
  and reads HECI to see if it should update the picture on display.

  @retval EFI_SUCCESS         An user aborted user consent flow
  @retval EFI_PROTOCOL_ERROR  Cannot read user input
  @retval EFI_DEVICE_ERROR    User consent flow ended with an error in HECI, KBD or graphics
**/
EFI_STATUS
PerformUserConsent (
  VOID
  );

#endif // _MEBX_USER_CONSENT_H_
