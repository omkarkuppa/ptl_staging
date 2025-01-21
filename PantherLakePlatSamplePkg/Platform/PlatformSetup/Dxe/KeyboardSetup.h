/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "PlatformSetup.h"

#ifndef _PLATFORM_SETUP_KEYBOARD_SETUP_H_
#define _PLATFORM_SETUP_KEYBOARD_SETUP_H_

//
// PS/2 Keyboard Controller registers
//
#define KEYBOARD_8042_DATA_REGISTER     0x60
#define KEYBOARD_8042_STATUS_REGISTER   0x64
#define KEYBOARD_8042_COMMAND_REGISTER  0x64

#define KEYBOARD_STATUS_REGISTER_HAS_OUTPUT_DATA     BIT0        ///< 0 - Output register has no data; 1 - Output register has data
#define KEYBOARD_STATUS_REGISTER_HAS_INPUT_DATA      BIT1        ///< 0 - Input register has no data;  1 - Input register has data
#define KEYBOARD_STATUS_REGISTER_SYSTEM_FLAG         BIT2        ///< Set to 0 after power on reset
#define KEYBOARD_STATUS_REGISTER_INPUT_DATA_TYPE     BIT3        ///< 0 - Data in input register is data; 1 - Data in input register is command
#define KEYBOARD_STATUS_REGISTER_ENABLE_FLAG         BIT4        ///< 0 - Keyboard is disable; 1 - Keyboard is enable
#define KEYBOARD_STATUS_REGISTER_TRANSMIT_TIMEOUT    BIT5        ///< 0 - Transmit is complete without timeout; 1 - Transmit is timeout without complete
#define KEYBOARD_STATUS_REGISTER_RECEIVE_TIMEOUT     BIT6        ///< 0 - Receive is complete without timeout; 1 - Receive is timeout without complete
#define KEYBOARD_STATUS_REGISTER_PARITY              BIT7        ///< 0 - Odd parity; 1 - Even parity

#define KEYBOARD_8048_COMMAND_CLEAR_OUTPUT_DATA             0xF4
#define KEYBOARD_8048_COMMAND_SET_DEFAULT                   0xF6
#define KEYBOARD_8048_COMMAND_RESET                         0xFF
#define KEYBOARD_8048_COMMAND_SELECT_SCAN_CODE_SET          0xF0

#define KEYBOARD_MAX_TRY                256     // 256

#define KEYBOARD_TIMEOUT                65536   // 0.07s
#define KEYBOARD_WAITFORVALUE_TIMEOUT   1000000 // 1s

#define KEYBOARD_8048_RETURN_8042_ACK                       0xFA

#endif
