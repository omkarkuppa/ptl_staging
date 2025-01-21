/** @file
  Library that contains common parts of WdtPei and WdtDxe. Not a standalone module.

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
#ifndef _PCH_WDT_COMMON_LIB_H_
#define _PCH_WDT_COMMON_LIB_H_

#define OC_WDT_TIMEOUT_VALUE     8     ///< Overclocking Watch Dog Timer Timeout Value

extern UINT8    mAllowExpectedReset;

/**
  HOB structure for WDT.
**/
typedef struct {
  EFI_HOB_GUID_TYPE Header;
  UINT16            TimeoutValue;
  UINT8             Active;
} WDT_HOB;

/**
  Reads LPC bridge to get Watchdog Timer address


  @retval UINT32                  Watchdog's address
**/
UINT32
WdtGetAddress (
  VOID
  );

/**
  Reloads WDT with new timeout value and starts it. Also sets Unexpected Reset bit, which
  causes the next reset to be treated as watchdog expiration - unless AllowKnownReset()
  function was called too.

  @param[in] TimeoutValue         Time in seconds before WDT times out. Supported range = 1 - 1024.

  @retval EFI_SUCCESS             if everything's OK
  @retval EFI_INVALID_PARAMETER   if TimeoutValue parameter is wrong
**/
EFI_STATUS
EFIAPI
WdtReloadAndStart (
  IN  UINT32  TimeoutValue
  );

/**
  Disables WDT timer.


**/
VOID
EFIAPI
WdtDisable (
  VOID
  );

/**
  Returns WDT failure status.


  @retval V_PCH_OC_WDT_CTL_STATUS_FAILURE   If there was WDT expiration or unexpected reset
  @retval V_PCH_OC_WDT_CTL_STATUS_OK        Otherwise
**/
UINT8
EFIAPI
WdtCheckStatus (
  VOID
  );

/**
  Normally, each reboot performed while watchdog runs is considered a failure.
  This function allows platform to perform expected reboots with WDT running,
  without being interpreted as failures.
  In DXE phase, it is enough to call this function any time before reset.
  In PEI phase, between calling this function and performing reset, ReloadAndStart()
  must not be called.


**/
VOID
EFIAPI
WdtAllowKnownReset (
  VOID
  );

/**
  Sets AllowKnownReset status to 0. This function should be called during WDT initialization.

**/
VOID
EFIAPI
WdtClearAllowKnownReset (
  VOID
  );

/**
  Returns information if WDT coverage for the duration of BIOS execution
  was requested by an OS application


  @retval TRUE                    if WDT was requested
  @retval FALSE                   if WDT was not requested
**/
BOOLEAN
EFIAPI
IsWdtRequired (
  VOID
  );

/**
  Returns WDT enabled/disabled status.


  @retval TRUE                    if WDT is enabled
  @retval FALSE                   if WDT is disabled
**/
BOOLEAN
EFIAPI
IsWdtEnabled (
  VOID
  );

/**
  Returns WDT locked status.


  @retval TRUE                    if WDT is locked
  @retval FALSE                   if WDT is unlocked
**/
BOOLEAN
EFIAPI
IsWdtLocked (
  VOID
  );

#endif
