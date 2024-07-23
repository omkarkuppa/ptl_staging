/** @file
  Dimm Info Rountines

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

#include <Uefi.h>
#include <DimmInfo.h>

SMBIOS_MEMORY_DEVICE_TYPE_LIST MemoryTypeList[] = {
  {0x18,"DDR3"},
  {0x1A,"DDR4"},
  {0x1D,"LPDDR3"},
  {0x1E,"LPDDR4"},
  {0x22,"DDR5"},
  {0x23,"LPDDR5"},
  {0x02,"UnKnown"}
};

UINT8 MemoryTypeListSize = sizeof (MemoryTypeList) / sizeof (MemoryTypeList[0]);


//
// The order must match DDR_TYPE_xxx defines in MemInfo.h
//
CHAR8 *MemoryTypeStr[] = {
  "LPDDR5",
  "DDR5",
  "UNKNOWN"
};

MEMORY_MODULE_MANUFACTURER_LIST MemoryModuleManufacturerList[] = {
  {0x2c00, "Micron"},
  {0xad00, "SK Hynix"},
  {0xce00, "Samsung"},
  {0x4f01, "Transcend"},
  {0x9801, "Kingston"},
  {0xfe02, "Elpida"}
};

UINT8 MemoryModuleManufacturerListSize = sizeof (MemoryModuleManufacturerList) / sizeof (MemoryModuleManufacturerList[0]);
