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
#define DATA_00                     0x04 ///< Data start from Byte 4 to Byte 35
#define DATA_SIZE                   0x24 ///< Data length corresponding to command
#define RESERVED_0                  0x25 ///< Reserved_0
#define RESERVED_1                  0x26 ///< Reserved_1
#define RESERVED_2                  0x27 ///< Reserved_2
#define I2C_COMMUNICATION_LOCK      0x28 ///< when the offset is set to 1, EC shall freeze other
                                         ///< EC-PD communication
#define RESERVED_3                  0x29 ///< Reserved_3
#define RESERVED_4                  0x2A ///< Reserved_4
#define RESERVED_5                  0x2B ///< Reserved_5

#endif
