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

///
/// Definitions for PHAT and Reset Reason table
///
#define RESET_REASON_PHAT_RECORD_TYPE   0x1
#define RESET_REASON_OFFSET             0x0074

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
