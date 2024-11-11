/** @file
  Dxe Telemetry Publish Reset Reason to PHAT ACPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _RESET_REASON_H_
#define _RESET_REASON_H_

#include <PiDxe.h>

#define RESET_REASON_OFFSET   0x0074
#define RESET_REASON_REVISION 1

///
/// This field indicates the source of the reset.
/// Only one bit should be set, or left zero if unknown.
///
typedef union {
  struct{
    UINT8 Unknown     : 1;  ///< Unknown reset source
    UINT8 Hardware    : 1;  ///< Hardware initiated reset
    UINT8 Firmware    : 1;  ///< Firmware initiated reset
    UINT8 Hypervisor  : 1;  ///< Hypervisor initiated reset
    UINT8 OS          : 1;  ///< Operation System initiated reset
    UINT8 Reserved    : 3;  ///< Reserved
  } Bits;
} RESET_SOURCE;

///
/// Reset reason table definition
///
typedef union {
  struct {
    UINT8   Unknown           : 1;  ///< [0]    An unknown reset reason
    UINT8   ColdBoot          : 1;  ///< [1]    The System booted or resumed from G3, S4 or S5
    UINT8   ColdReset         : 1;  ///< [2]    The system was successfully rebooted and performed a full cold reset.
    UINT8   WarmReset         : 1;  ///< [3]    The system performed a user or software initiated reset
    UINT8   Update            : 1;  ///< [4]    A system or software update was applied that required a reset.
    UINT32  Reserved          : 26; ///< [5:31] Reserved
    UINT8   UnexpectedReset   : 1;  ///< [32]   Any undisclosed reasons that triggered a reset
    UINT8   Fault             : 1;  ///< [33]   All errors that can cause a warm reset, cold reset, or global reset but not due to a WDT timeout.
    UINT8   Timeout           : 1;  ///< [34]   All timeout reasons
    UINT8   Thermal           : 1;  ///< [35]   All thermal global reset causes
    UINT8   PowerLoss         : 1;  ///< [36]   System unexpected power loss: PWR_FLR bits
    UINT8   PowerButton       : 1;  ///< [37]   Power button override global reset cause
  } Bits;
} RESET_REASON;

///
/// Vendor Data Entry
///
typedef struct {
  UINTN   VendorId;   ///< A vender defined UUID that describes this entry type
  UINT8   Length;     ///< Length of the vendor data entry
  UINTN   Data;       ///< Vendor specific data
} VENDOR_DATA;

///
/// Reset Reason Health Record Structure
///
typedef struct {
  RESET_SOURCE        ResetSource;
  RESET_SOURCE        SupportedSources;
  RESET_REASON        ResetReason;
  UINT16              VendorCount;
  UINTN               VendorSpecificResetReason;
} RESET_REASON_HEALTH_STRUCTURE;

/**
  Entry point to telemetry platform reset reason PHAT.

  @param[in] ImageHandle    The firmware allocated handle for the UEFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS   Able to get and set reset reason to PHAT
  @retval Other         Something occurred causing a failure
**/
EFI_STATUS
EFIAPI
ResetReasonEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif // _RESET_REASON_H_
