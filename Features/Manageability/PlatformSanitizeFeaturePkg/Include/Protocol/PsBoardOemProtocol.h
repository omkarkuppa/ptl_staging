/**@file
  Platform Sanitize Protocol services for Platform dependent, OEM specific
  sanitizations.

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
#ifndef _PS_BOARD_OEM_PROTOCOL_H_
#define _PS_BOARD_OEM_PROTOCOL_H_

#include <PsParameter.h>

//
// Declaration for the PS_BOARD_OEM_PROTOCOL.
//
typedef struct _PS_BOARD_OEM_PROTOCOL  PS_BOARD_OEM_PROTOCOL;

/**
  Board Platform and OEM Specific Protocol revision number
**/
#define PS_BOARD_OEM_PROTOCOL_REVISION  1

/**
  Platform Sanitize to perform Reload NVM Variable with Golden Configuration settings.
  Intel RVP-BIOS, Setup options are reloaded to default configuration.
  OEM can modify to reload their own set of BIOS Configuration.

  @param[in] *This              - A pointer to the PS_BOARD_OEM_PROTOCOL instance

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
typedef
EFI_STATUS
(EFIAPI *PS_NVM_GOLDEN_CONFIG) (
  IN  PS_BOARD_OEM_PROTOCOL    *This
  );

/**
  Invoke OEM Customized Erase action corresponding to Bitmask 16 as per Platform Erase
  Bit Representation.
  Intel RVP-BIOS, Debug prints "OEM Custom Action" and returns.
  OEM can have this function to implement their own customized erase action.

  @param[in] *This              - A pointer to the PS_BOARD_OEM_PROTOCOL instance
  @param[in] *Context           - A pointer to the OEM Context

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
typedef
EFI_STATUS
(EFIAPI *PS_OEM_CUSTOM_ACTION_API) (
  IN  PS_BOARD_OEM_PROTOCOL    *This,
  IN  PS_OEM_HOOK_TLV_DATA     *PsOemHookTlvData
  );

/**
  Invoke Post Erase All NVM Variable function, in case if OEM choose to add any of NVM Variables (Secure Boot related)
  to keep the system as per End Of Manufacturing state.

  @param[in] *This              - A pointer to the PS_BOARD_OEM_PROTOCOL instance

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
typedef
EFI_STATUS
(EFIAPI *PS_POST_ERASE_ALL_NVM_VARS) (
  IN  PS_BOARD_OEM_PROTOCOL    *This
  );

//
// This Protocol contains a set of functional interfaces to interact with Platform specific
// Erase actions and OEM specific Erase actions.
//
typedef struct _PS_BOARD_OEM_PROTOCOL {
  UINT32                          Revision;
  PS_NVM_GOLDEN_CONFIG            PsNvmGoldenConfig;
  PS_OEM_CUSTOM_ACTION_API        PsOemCustomAction;
  PS_POST_ERASE_ALL_NVM_VARS      PsPostEraseAllNvmVars;
} PS_BOARD_OEM_PROTOCOL;

extern EFI_GUID gPsBoardOemProtocolGuid;

#endif  // _PS_BOARD_OEM_PROTOCOL_H_
