/** @file
  Platform Board Sanitize Dxe driver supports Platform and OEM specific sanitization.

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
#ifndef _PS_BOARD_OEM_DXE_H_
#define _PS_BOARD_OEM_DXE_H_

#include <Protocol/PsBoardOemProtocol.h>

//
// Structure definition to list setup variables name and its GUID
//
typedef struct {
  CHAR16                        *Name;
  EFI_GUID                      *Guid;
} BIOS_SETUP_NVM_VARIABLE_LIST;

//
// Board specific, OEM specific function prototypes
//
/**
  Platform Sanitize to perform Reload NVM Variable with Golden Configuration settings.
  Intel RVP-BIOS, Setup options are reloaded to default configuration.

  Note: OEM can modify to reload their own set of BIOS Configuration.

  @param[in] *This              - A pointer to the PS_BOARD_OEM_PROTOCOL instance

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
EFI_STATUS
EFIAPI
PsNvmGoldenConfig (
  IN PS_BOARD_OEM_PROTOCOL *This
  );

/**
  This function invoked Post "PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION" action, to perform any action to take after formating the entire
  NVM variable region like adding Secure Boot, Keys, Certificates related NVM variables which might essential for UEFI Secure Boot.

  Note: Intel-RVP BIOS returns simply with EFI_SUCCESS.
        OEM can modify this function as per their need.

  @param[in] *This              - A pointer to the PS_BOARD_OEM_PROTOCOL instance

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
EFI_STATUS
EFIAPI
PsPostClearAllBiosNvmVariableRegionAction (
  IN PS_BOARD_OEM_PROTOCOL *This
  );

/**
  This function invoked when "PS_OEM_CUSTOM_ACTION" corresponding to Bitmask 16 is triggered to perform OEM customized sanitize action.

  Note: Intel RVP-BIOS returns simply with EFI_SUCCESS.
        OEM can modify this function as per their need.

  @param[in] *This                - A pointer to the PS_BOARD_OEM_PROTOCOL instance
  @param[in] *PsOemHookTlvData    - A pointer to OEM TLV Data

  @retval EFI_SUCCESS           - The operation succeeds.
  @retval Others                - Other status errors
**/
EFI_STATUS
EFIAPI
PsOemCustomActionBitMask16 (
  IN PS_BOARD_OEM_PROTOCOL   *This,
  IN PS_OEM_HOOK_TLV_DATA    *PsOemHookTlvData
  );

#endif    // _PS_BOARD_OEM_DXE_H_
