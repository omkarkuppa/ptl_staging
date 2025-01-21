/** @file
 Header file for Platform Config Tool Structures.

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

@par Specification Reference:
**/

#ifndef _PLATFORM_CONFIG_TOOL_STRUCTURES_H_
#define _PLATFORM_CONFIG_TOOL_STRUCTURES_H_

#include <Uefi.h>

#pragma pack(push, 1)

typedef enum {
  PctPreMemGpio         = 1,
  PctPostMemGpio        = 2,
  PctAcpi               = 3
} PCT_CONFIG_DATA_TYPE;

typedef struct {
  UINT16  BoardId;
  UINT16  Revision;
  UINT8   Rsvd[8];
} PCT_SECTION_HEADER;

typedef struct {
  EFI_GUID  Guid;
  UINT8     Revision;
  UINT16    Count;
  UINT8     PctRevision[3];
  UINT8     Rsvd[2];
} PCT_GPIO_BLOCK_HDR, PCT_VERB_TABLE_HDR, PCT_USB_OCC_MAP_HDR, PCT_PCI_CLOCK_MAP_HDR, PCT_ACPI_BLOCK_HDR;

#pragma pack(pop)

typedef struct {
  PCT_CONFIG_DATA_TYPE  PctDataType;
  EFI_GUID              *Guid;
} PCT_CONFIG_DATA_TYPE_GUID;

typedef struct {
  UINTN GpioPinData;
  CHAR8 *GpioPinDataString;
} GPIO_PIN_STRING;

#endif // _PLATFORM_CONFIG_TOOL_STRUCTURES_H_
