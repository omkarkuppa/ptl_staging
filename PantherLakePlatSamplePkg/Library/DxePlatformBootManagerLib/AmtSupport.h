/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#ifndef _AMT_SUPPORT_H_
#define _AMT_SUPPORT_H_

#include <Protocol/AsfProtocol.h>

#define RESTORE_SECURE_BOOT_NONE          0
#define RESTORE_SECURE_BOOT_ENABLED       1

#define DISABLE_SECURE_BOOT_STATE         0
#define RESTORE_SECURE_BOOT_STATE         1

#define SECURITY_EVENT_STRING             "Secure boot disable"

/**
  Configure platform's Secure Boot State for AMT support and register the callback,
  which stops AMT Watchdog on Setup Entry.

  @retval EFI_SUCCESS           Initialization complete.
  @retval Others                Error occured during the initialization.
**/
EFI_STATUS
AmtInitializePlatformForAmtSupport (
  VOID
  );

/**
  Performs boot actions requested by AMT through Boot Options.
**/
VOID
AmtBdsBoot (
  VOID
  );

/**
  Function updating ConIn Variable.
**/
VOID
AmtUpdateConsoleVariable (
  VOID
  );

/**
  AMT action when console is ready.
  Dynamically switch the output mode to 80x25 when SOL is enabled.
**/
VOID
AmtConsoleReady (
  VOID
  );

/**
  Check if Remote Assistance Prompt String should be displayed.

  @retval TRUE              Remote Assistance prompt should be displayed.
  @retval FALSE             Remote Assistance prompt should not be displayed.
**/
BOOLEAN
AmtIsRemoteAssistancePromptSupported (
  VOID
  );

/**
  Register Amt Hotkey.
**/
VOID
AmtRegisterHotKey (
  VOID
  );

/**
  Check whether do full configuration enumeration for media asset table

  @param[in, out]   BootMode        A pointer to BootMode to be updated.

**/
VOID
AmtUpdateBootMode (
  IN OUT EFI_BOOT_MODE *BootMode
  );

/**
  Restart ASF BIOS Watchdog after third party option ROM is complete.
**/
VOID
AmtStartWatchdog (
  VOID
  );

/**
  Checks if device is in a One Click Recovery boot flow. Will get the boot capabilities
  from ME setup data and pass too OCR_CONFIG use in OneClickRecovery feature. Then will
  call the OCR feature for operation, if in a recovery boot.
**/
VOID
AmtPerformOneClickRecoveryBoot (
  VOID
  );

#endif //_AMT_SUPPORT_H_
