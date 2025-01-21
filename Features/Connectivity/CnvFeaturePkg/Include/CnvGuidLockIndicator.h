/**@file
  Definition for Guid Lock Status Indicator Protocol API.

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

@par Specification
**/

#ifndef __CNV_GUID_LOCK_INDICATOR_H__
#define __CNV_GUID_LOCK_INDICATOR_H__

//
// Statements that include other header files
//
#include <Base.h>
#include <Uefi.h>

//
//  The Revision of GUID Lock Status
//
#define CNV_UEFI_GUID_LOCK_REVISION  0

//
// Definition for Get GUID Lock Status Indicator - Disclaimer GuidLockStatusIndicator shall be invoked only one time during DXE and before END of DXE by UEFI driver.
//
typedef
  UINT8
(EFIAPI *EFI_GET_CNV_UEFI_GUID_LOCK_STATUS_INDICATOR)(
                                                      VOID
                                                      );

//
// Definition for CNV UEFI GUID Lock Status Indicator Protocol
//
typedef struct {
  UINT64                                         Revision;
  EFI_GET_CNV_UEFI_GUID_LOCK_STATUS_INDICATOR    GetGuidLockStatusIndicator;
} EFI_CNV_UEFI_GUID_LOCK_STATUS_INDICATOR_PROTOCOL;

//
// The function get the GUID lock status
//
UINT8
EFIAPI
GetGuidLockStatusIndicator (
  VOID
  );

#endif //__CNV_GUID_LOCK_INDICATOR_H__
