## @file
#  Board description file initializes configuration (PCD) settings for the project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##
[Defines]
  PIN_GPIO_ACTIVE_HIGH            = 1
  PIN_GPIO_ACTIVE_LOW             = 0
  BOARD_REAL_BATTERY_SUPPORTED    = 1
  BOARD_VIRTUAL_BATTERY_SUPPORTED = 2
  BOARD_NO_BATTERY_SUPPORT        = 0

#####################################################################
# PTL simics Board ID: 0x31 (BoardIdPtlSimics)
#                  SKU ID: 0x31 (SkuIdPtlSimics)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlSimics]

  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdAddressTable| * |{CODE(
  { 16,
    { 0xA0 ,   // Port 0
      0x00 ,   // Port 1
      0xA0 ,   // Port 2
      0x00 ,   // Port 3
      0xA0 ,   // Port 4
      0x00 ,   // Port 5
      0xA0 ,   // Port 6
      0x00 ,   // Port 7
      0xA4 ,   // Port 8
      0x00 ,   // Port 9
      0xA4 ,   // Port 10
      0x00 ,   // Port 11
      0xA4 ,   // Port 12
      0x00 ,   // Port 13
      0xA4 ,   // Port 14
      0x00     // Port 15
    }
  })}
###
### !!! GPIOs designated to Native Functions shall not be configured by Platform Code.
### Native Pins shall be configured by Silicon Code (based on BIOS policies setting) or soft straps(set by CSME in FITc).
###
###
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable|*|{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem|*|{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap| * |{CODE(
    {0x0}
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap| * |{CODE(
    {0x0}
  )}