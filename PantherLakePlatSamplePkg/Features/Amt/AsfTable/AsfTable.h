/**@file
  Header file of ASF ACPI table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#ifndef _ASF_TABLE_H_
#define _ASF_TABLE_H_

#include <IndustryStandard/AlertStandardFormatTable.h>
#include <Protocol/AcpiTable.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/EventGroup.h>
#include <Protocol/AsfProtocol.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <SetupVariable.h>

#define ASF_INFO_MIN_WATCH_DOG_RESET_VALUE  0xFF
#define ASF_INFO_MIN_POLLING_INTERVAL       0xFF
#define ASF_INFO_SYSTEM_ID                  0x01

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER TableHeader;
  UINT8                       TableData[];
} ASF_TABLE;

/**
  Building Alert Standard Format (ASF) Table.

  @param[in] Event         - A pointer to the Event that triggered the callback.
  @param[in] Context       - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
BuildAsfTable (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

#endif
