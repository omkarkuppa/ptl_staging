/** @file
  Battery definitions

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

#ifndef _BATTERY_DATA_H_
#define _BATTERY_DATA_H_

#include <EcCommands.h>

#define BATTERY_STATE                       (BIT0|BIT1|BIT2)
#define PHYSICAL_BATTERY_TECHNOLOGY         0x00000001
#define PHYSICAL_MODEL_NUMBER               "BASE-BAT"
#define PHYSICAL_SERIAL_NUMBER              "123456789"
#define PHYSICAL_BATTERY_TYPE               "LiP"
#define PHYSICAL_OEM_INFORMATION            "Simplo"
#define VIRTUAL_DESIGNED_CAPACITY           0x00002710
#define VIRTUAL_LAST_FULL_CHARGE_CAPACITY   0x00002710
#define VIRTUAL_BATTERY_TECHNOLOGY          0x00000001
#define VIRTUAL_DESIGN_VOLTAGE              0xFFFFFFFF
#define VIRTUAL_DESIGN_CAPACITY_OF_WARNING  0x000003E8
#define VIRTUAL_DESIGN_CAPACITY_OF_LOW      0x00000190
#define VIRTUAL_MODEL_NUMBER                "CRB Battery 0"
#define VIRTUAL_SERIAL_NUMBER               "Battery 0"
#define VIRTUAL_BATTERY_TYPE                "Fake"
#define VIRTUAL_OEM_INFORMATION             "-Virtual Battery 0-"
#define VIRTUAL_BATTERY_PRESENT_RATE        0x7FFFFFFF
#define VIRTUAL_BATTERY_REMAINING_CAPACITY  0xFFFFFFFF
#define VIRTUAL_BATTERY_PRESENT_VOLTAGE     0xFFFFFFFF

#endif // _BATTERY_DXE_LIB_H_
