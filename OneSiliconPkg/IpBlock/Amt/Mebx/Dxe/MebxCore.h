/** @file
  Include for AMT Bios Extentions Loader

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#ifndef _MEBX_CORE_H_
#define _MEBX_CORE_H_

#define UNCONFIGURE_TIMEOUT                       60

//
// MEBX Software Class DXE Subclass Progress Code definitions.
//
#define EFI_SW_DXE_MEBX_OPROM_DONE                (EFI_OEM_SPECIFIC | 0x00000000)
#define EFI_SW_DXE_MEBX_RESET_ACTION              (EFI_OEM_SPECIFIC | 0x0000000a)

#define INVOKE_MEBX_BIT                           BIT3

//Reboot reason definitions
#define MEBX_RESET_NO_RESET                       0
#define MEBX_RESET_UNCONFIG_FINISHED              BIT0
#define MEBX_RESET_UNCONFIG_WITHOUT_PASSWORD      BIT1
#define MEBX_RESET_CPU_REPLACEMENT_USER_FEEDBACK  BIT2
#define MEBX_RESET_CPU_REPLACEMENT_RESET_REQUIRED BIT3
#define MEBX_RESET_SOL_SYNC                       BIT4
#define MEBX_RESET_WLAN_PWR_CONFIG_CHANGE         BIT5

/**
  Signal an event for Amt Invocation Code ready to boot.
**/
VOID
EFIAPI
MebxInvocationCode (
  VOID
  );

/**
  Signal a event for Amt ready to boot.
**/
VOID
EFIAPI
MebxOnReadyToBoot (
  VOID
  );

#endif
