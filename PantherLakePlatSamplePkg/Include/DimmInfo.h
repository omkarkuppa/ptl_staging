/** @file
  This header file provides Dimm info definitions used by other
  modules for dimm info presentation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#ifndef __DIMM_INFO_H__
#define __DIMM_INFO_H__

#include <Protocol/MemInfo.h>
#define MAX_LENGTH 10

typedef struct {
  UINT16  MfgId;
  CHAR8   String[16];
} MEMORY_MODULE_MANUFACTURER_LIST;

typedef enum {
  LPDDR5,
  DDR5,
  NUM_DDR_TYPES
} DDR_TYPE;

typedef struct {
  UINT8 DdrTypeNum;
  CHAR8 Meaning[MAX_LENGTH];
} SMBIOS_MEMORY_DEVICE_TYPE_LIST;

extern CHAR8 *MemoryTypeStr[];
extern MEMORY_MODULE_MANUFACTURER_LIST MemoryModuleManufacturerList[];
extern UINT8 MemoryModuleManufacturerListSize;
extern SMBIOS_MEMORY_DEVICE_TYPE_LIST MemoryTypeList[];
extern UINT8 MemoryTypeListSize;

#endif // __DIMM_INFO_H__
