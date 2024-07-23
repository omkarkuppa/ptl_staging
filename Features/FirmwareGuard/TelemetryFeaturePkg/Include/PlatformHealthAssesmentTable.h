/** @file
  Intel Platform Telememtry Data Table (PHAT) related definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __PLATFORM_HEALTH_ASSESSMENT_TABLE_H__
#define __PLATFORM_HEALTH_ASSESSMENT_TABLE_H__

#include <IndustryStandard/Acpi.h>

#pragma pack(1)

//
// SAS Gen2, chapter 2.5.4  Motherboard firmware health record
//
typedef struct {
  UINT8                                            Revision;                 // Motherboard Revision, 1 - Revision of this device-specific record
  UINT64                                           MeanBootTime;             // Average time in nanoseconds elapsed just prior to loading the OS boot loader.
  UINT64                                           CurrentBootTime;          // Time in nanoseconds elapsed just prior to loading the OS boot loader.
  UINT16                                           NumberOfBootsSinceUpdate; // Number of completed (reached READY_TO_BOOT) boots since the last successful firmware update.
} EFI_ACPI_MOTHERBOARD_HEALTH_DATA;

#pragma pack()

#endif
