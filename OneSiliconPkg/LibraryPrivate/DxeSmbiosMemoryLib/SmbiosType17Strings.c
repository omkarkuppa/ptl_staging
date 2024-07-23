/** @file
  This library will determine memory configuration information from the chipset
  and memory and create SMBIOS memory structures appropriately.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include "SmbiosMemory.h"

GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 *DeviceLocatorStringsDimm[][8] = {
 {"Controller0-ChannelA-DIMM0",
  "Controller0-ChannelA-DIMM1",
  "Controller0-ChannelB-DIMM0",
  "Controller0-ChannelB-DIMM1",
  "Controller0-ChannelC-DIMM0",
  "Controller0-ChannelC-DIMM1",
  "Controller0-ChannelD-DIMM0",
  "Controller0-ChannelD-DIMM1"},
 {"Controller1-ChannelA-DIMM0",
  "Controller1-ChannelA-DIMM1",
  "Controller1-ChannelB-DIMM0",
  "Controller1-ChannelB-DIMM1",
  "Controller1-ChannelC-DIMM0",
  "Controller1-ChannelC-DIMM1",
  "Controller1-ChannelD-DIMM0",
  "Controller1-ChannelD-DIMM1"}
};

GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 *DeviceLocatorStringsMemoryDown[][8] = {
 {"Controller0-ChannelA",
  "Controller0-ChannelA",
  "Controller0-ChannelB",
  "Controller0-ChannelB",
  "Controller0-ChannelC",
  "Controller0-ChannelC",
  "Controller0-ChannelD",
  "Controller0-ChannelD"},
 {"Controller1-ChannelA",
  "Controller1-ChannelA",
  "Controller1-ChannelB",
  "Controller1-ChannelB",
  "Controller1-ChannelC",
  "Controller1-ChannelC",
  "Controller1-ChannelD",
  "Controller1-ChannelD"}
};

GLOBAL_REMOVE_IF_UNREFERENCED CHAR8 *BankLocatorStrings[] = {
  "BANK 0",
  "BANK 1",
  "BANK 2",
  "BANK 3"
};
