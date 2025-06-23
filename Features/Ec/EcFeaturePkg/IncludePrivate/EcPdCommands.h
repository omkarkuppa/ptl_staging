/** @file
  EC Private PD commands and definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _EC_PD_COMMANDS_H_
#define _EC_PD_COMMANDS_H_

#define EC_PRIVATE_DATA_PORT      0x6A0
#define EC_PRIVATE_CMD_PORT       0x6A4

///
/// Status Port 0x6A0
///
#define EC_PRIVATE_CMD_STATUS_SUCCESS      0x80 ///< Command success
#define EC_PRIVATE_CMD_STATUS_FAILED       0x07 ///< Command failed
#define EC_PRIVATE_CMD_STATUS_IN_PROGRESS  0x1A ///< Command execution in progress
#define EC_PRIVATE_CMD_STATUS_NO_ACK       0x10 ///< Command no ack

///
/// EC PD commands that are issued to the EC through the command port (0x6A4).
/// New commands and command parameters should only be written by the host when I2C_COMMUNICATION_LOCK=0.
///
#define EC_PRIVATE_CMD_EXECUTE 0x0C ///< Execute vendor register
#define EC_PRIVATE_CMD_LOCK    0x10 ///< Lock I2C target

///
/// EC PD I2C target lock status
///
#define EC_PD_UNLOCK_I2C  0x00 ///< Unlock the I2C target
#define EC_PD_LOCK_I2C    0x01 ///< Lock the I2C target

#endif
