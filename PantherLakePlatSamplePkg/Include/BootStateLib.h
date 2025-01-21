/** @file
# Component Information file for BootState Variable.
#
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

# @par Specification Reference:
**/

#ifndef _BOOT_STATE_H_
#define _BOOT_STATE_H_

#define BOOT_STATE_VARIABLE_NAME  L"BootState"

#endif

/**
 This function call Checks BootState variable is NULL or not

 @retval         FALSE                  It's the first boot after reflashing.
 @retval         TRUE                   It's not the first boot after reflashing.
**/

BOOLEAN
EFIAPI
IsBootStatePresent (
  VOID
);

/**
 This function call use to set bootstate variable.
 In DXE  phase BootState variable got set using setvariable().Form onward it will treat as second boot.

 @param[in]      BootState              Set value for bootstate
**/

VOID
EFIAPI
SetBootState (
  VOID
);

/**
 Delete BootState variable to force next boot is FullConfiguration boot
**/

VOID
EFIAPI
UnsetBootState (
  VOID
);