/** @file
  EC Private PD RAM offset definition.

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

#ifndef _EC_PD_RAM_OFFSET_H_
#define _EC_PD_RAM_OFFSET_H_

#define BIOS_EC_COMMAND             0x00 ///< BIOS EC Command
#define COMMAND_EXECUTION_STATUS    0x01 ///< Command execution status
#define PD_CONTROLLER_INDEX         0x02 ///< PD controller index (0-based).
#define VENDOR_REGISTER_OR_COMMAND  0x03 ///< PD Vendor register offset or command data
#define I2C_COMMUNICATION_LOCK      0x04 ///< when the offset is set to 1, EC shall freeze other
                                         ///< EC-PD communication
#define DATA_SIZE                   0x05 ///< Data length corresponding to command
#define RESERVED_0                  0x06 ///< Reserved_0
#define RESERVED_1                  0x07 ///< Reserved_1
#define DATA_00                     0x08 ///< Start offset of the Data field
#define RESERVED_2                  0x09 ///< Reserved_2
#define RESERVED_3                  0x0A ///< Reserved_3
#define RESERVED_4                  0x0B ///< Reserved_4
#define VENDOR_ID                   0x0C ///< Vendor ID (VID) from Byte 12 to 13
#define PRODUCT_ID                  0x0E ///< Product ID (PID) from Byte 14 to 15
#define VENDOR_CMD_DATA             0x10 ///< Vendor Command Data from Byte 16 to 79

#endif
