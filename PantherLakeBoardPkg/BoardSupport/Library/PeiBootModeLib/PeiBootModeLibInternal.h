/** @file
Private header file for the Boot Mode Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _BOOT_MODE_LIB_INTERNAL_H_
#define _BOOT_MODE_LIB_INTERNAL_H_

#define BOOT_SPECIAL_MASK                            0x80

/**
  Given the current boot mode, and a proposed new boot mode, determine
  which has priority. If the new boot mode has higher priority, then
  make it the current boot mode.

  @param[in][out] CurrentBootMode Pointer to current planned boot mode
  @param[in][out] NewBootMode     Proposed boot mode

  @retval         EFI_NOT_FOUND   If either boot mode is not recognized
  @retval         EFI_SUCCESS     If both boot mode values were
                                  recognized and were processed.
**/
EFI_STATUS
EFIAPI
PrioritizeBootMode (
  IN OUT EFI_BOOT_MODE    *CurrentBootMode,
  IN EFI_BOOT_MODE        NewBootMode
  );

/**
  Check Fast Boot 30-second WDT (TCO Timer) timeout has occurred

  @retval  TRUE   Timeout has occurred
  @retval  FALSE  Timeout didnt occur
**/
BOOLEAN
IsTimeOutOccurred (
  VOID
  );

/**
  Check if the value is of ODD parity.

  @param[in]  Value  The 8 bits value

  @retval     TRUE   The value is of ODD parity
  @retval     FALSE  The value is not of ODD parity
**/
BOOLEAN
CheckOddParity (
  IN  UINT8 Value
  );

/**
  Update boot mode

  @param[in] BootMode             Current System BootMode.

  @retval EFI_SUCCESS             The boot mode was returned successfully.
**/

EFI_STATUS
EFIAPI
PreMemUpdateBootMode (
  IN OUT EFI_BOOT_MODE          *BootMode
  );

/**
  Check Boot Flag Status to determine if previous boot was successful

  @retval  TRUE   Previous Boot was success
  @retval  FALSE  Previous Boot wasn't success
**/
BOOLEAN
EFIAPI
IsPreviousFastBootCompleted (
  VOID
  );

/**
  Query PCH to determine if Intrusion Detection set

  @retval  TRUE   No Change
  @retavl  FALSE  Change
**/

BOOLEAN
EFIAPI
IsBootWithNoChange (
  VOID
  );

/**
  Check if this system boot is due to 4sec power button override has occurred

  @retval  TRUE   Power Button Override occurred in last system boot
  @retval  FALSE  Power Button Override didnt occur
**/

BOOLEAN
EFIAPI
IsPowerButtonOverride (
  VOID
  );

/**
  Get simple boot flag

  @retval  LegacyValue
  @retval  EfiValue
**/

/**
  Detect recovery mode

  @retval  EFI_SUCCESS       System in Recovery Mode
  @retval  EFI_UNSUPPORTED   System doesn't support Recovery Mode
  @retval  EFI_NOT_FOUND     System is not in Recovery Mode
**/
EFI_STATUS
EFIAPI
IsRecoveryMode (
  VOID
  );


/**
  Detect coinless SPI NOR clear.

  @retval  TRUE   Execute coinless SPI NOR clear.
  @retval  FALSE  Doesn't execute coinless SPI NOR clear.
**/
BOOLEAN
EFIAPI
IsCoinlessSpiNorClear (
  VOID
  );

#endif
