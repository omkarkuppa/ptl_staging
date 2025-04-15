/** @file
  Intel specific vendor data for ACPI PHAT Reset Reason record

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef __PHAT_RESET_REASON_VENDOR_DATA_H__
#define __PHAT_RESET_REASON_VENDOR_DATA_H__

#include <IndustryStandard/Acpi65.h>

// {808601BF-44B2-4D67-9B27-CCA2642285AD}
#define INTEL_PHAT_RESET_REASON_VENDOR_DATA_GUID \
  { \
    0x808601BF, 0x44B2, 0x4D67, { 0x9B, 0x27, 0xCC, 0xA2, 0x64, 0x22, 0x85, 0xAD } \
  }

//
// Minor.Major
// Byte 0 (Minor) indicates that changes to the vendor-specific data are backwards
// compatible with the Vendor Data ID.
// Byte 1 (Major) indicates that changes to the data are not backwards compatible.
//
//
// Revision: 1.0 (Minor.Major)
//
#define INTEL_PHAT_RESET_REASON_HEALTH_RECORD_VENDOR_DATA_REVISION_0_1 0x0001

#pragma pack(1)

typedef struct {
  UINT32    GblrstCause0;
  UINT32    GblrstCause1;
  UINT32    HbrCause0;
  UINT32    GenPmconA;
} RESET_CAUSE_PM_REGISTERS;

typedef struct {
  EFI_ACPI_6_5_PHAT_RESET_REASON_HEALTH_RECORD_VENDOR_DATA_ENTRY    Header;
  RESET_CAUSE_PM_REGISTERS                                          Payload;
} INTEL_PHAT_RESET_REASON_VENDOR_DATA;

#pragma pack()

extern EFI_GUID gIntelPhatResetReasonVendorDataGuid;

#endif
