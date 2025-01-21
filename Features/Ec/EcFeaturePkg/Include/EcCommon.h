/** @file
  EC library functions and definitions.

  This library provides EC common library use.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _EC_COMMON_H_
#define _EC_COMMON_H_

#include <Base.h>
#include <Uefi.h>

//
// Bitmasks for USBC IOM Ready Notify message request byte
//
#define EC_TCSS_BITMASK_REPLAY_MESSAGES     0x01

//
// EC Wait time after Sending SET PD MODE command in Factor of 50 MiliSec
//
#define EC_SET_PD_MODE_WAIT_TIME_IN_FACTOR_OF_50_MS  10

//
// Currently EC is supporting PD controller by TCP port and following BIT MAP for each
// controller.
//    BIT0 - 1 - PD1 I2C cmd successful (0 - fail )
//    BIT1 - 1 - PD1 in FORCE TBT Mode  (0 - not in force tbt mode)
//    BIT2 - 1 - PD2 I2C cmd successful (0 - fail )
//    BIT3 - 1 - PD2 in FORCE TBT Mode  (0 - not in force tbt mode)
//    BIT4 - 1 - PD3 I2C cmd successful (0 - fail)
//    BIT5 - 1 - PD3 in FORCE TBT Mode  (0 - not in force tbt mode)
//    BIT6 - 1 - PD4 I2C cmd successful (0 - fail )
//    BIT7 - 1 - PD4 in FORCE TBT Mode  (0 - not in force tbt mode)
// So currently EC is using 2 BIT for EACH Controller and 8 BIT in total.
//
#define GET_PD_MODE_STATUS_BIT_WIDTH_TOTAL            8
#define GET_PD_MODE_STATUS_BIT_WIDTH_PER_CONTROLLER   2
#define I2C_COMMAND_SUCCESSFUL                        BIT0  // PD I2C cmd successful  ( 0 - fail )
#define RETIMER_FW_UPDATE_MODE                        BIT1  // FORCE TBT Mode (0 - not in force tbt mode)

//
// Define all PD Mode supported by EC
//
typedef enum {
  RetimerFirmWareUpdateDisableMode = 0,
  RetimerFirmWareUpdateEnableMode  = 1,
  InvalidRetimerFirmWareUpdateMode
} EC_C_PD_RETIMER_FW_UPDATE_MODE;

#endif // _EC_COMMON_H_
