/** @file
  RTC public library header for PTL PCD

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _PTLPCD_RTC_ACCESS_LIB_H_
#define _PTLPCD_RTC_ACCESS_LIB_H_

/**
  Check RTC BIOS interface is locked

  @retval TRUE  RTC BIOS interface is locked
          FALSE RTC BIOS interface is not locked
**/
BOOLEAN
PtlPcdRtcIsBiosInterfaceLock (
  VOID
  );

/**
  Get TopSwap Status

  @retval TRUE  TopSwap is set
  @retval FALSE TopSwap is clear

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
BOOLEAN
PtlPcdTopSwapStatus (
  VOID
  );

/**
  Set TopSwap Status

  @param[in]  TopSwapEnable     Enable TopSwap or Disable it.

  @retval     EFI_SUCCESS       TopSwap set successfully
  @retval     EFI_DEVICE_ERROR  SBI command error

  @note  It is caller's responsiblity to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)
**/
EFI_STATUS
PtlPcdTopSwapSet (
  IN  BOOLEAN         TopSwapEnable
  );

#endif // _PTLPCD_RTC_ACCESS_LIB_H_

