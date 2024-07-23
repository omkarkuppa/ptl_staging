/** @file
  Smbus Common Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#ifndef _SMBUS_COMMON_LIB_H_
#define _SMBUS_COMMON_LIB_H_

//
// Definitions
//
#define SMBUS_NUM_RESERVED          38      ///< Number of device addresses that are reserved by the SMBus spec.
#define SMBUS_ADDRESS_ARP           0xC2 >> 1
#define SMBUS_DATA_PREPARE_TO_ARP   0x01
#define SMBUS_DATA_RESET_DEVICE     0x02
#define SMBUS_DATA_GET_UDID_GENERAL 0x03
#define SMBUS_DATA_ASSIGN_ADDRESS   0x04
#define SMBUS_GET_UDID_LENGTH       17      ///< 16 byte UDID + 1 byte address
//
// Private data and functions
//

#define PCH_SMBUS_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('p', 's', 'm', 'b')

/**
  This function provides a standard way to read PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.

  @retval UINT8                   Returns data read from IO.
**/
UINT8
SmbusIoRead (
  IN      UINT8           Offset
  );

/**
  This function provides a standard way to write PCH Smbus IO registers.

  @param[in] Offset               Register offset from Smbus base IO address.
  @param[in] Data                 Data to write to register.

**/
VOID
SmbusIoWrite (
  IN      UINT8           Offset,
  IN      UINT8           Data
  );

/**
  This function provides a standard way to execute Smbus protocols
  as defined in the SMBus Specification. The data can either be of
  the Length byte, word, or a block of data. The resulting transaction will be
  either the SMBus Device accepts this transaction or this function
  returns with an error

  @param[in] DeviceAddress        Smbus device the command is directed at
  @param[in] Command              Smbus Device dependent
  @param[in] Operation            Which SMBus protocol will be used
  @param[in] PecCheck             Defines if Packet Error Code Checking is to be used
  @param[in, out] Length          How many bytes to read. Must be 0 <= Length <= 32 depending on Operation
                                  It will contain the actual number of bytes read/written.
  @param[in, out] Buffer          Contain the data read/written.

  @retval EFI_SUCCESS             The operation completed successfully.
  @exception EFI_UNSUPPORTED      The operation is unsupported.

  @retval EFI_INVALID_PARAMETER   Length or Buffer is NULL for any operation besides
                                  quick read or quick write.
  @retval EFI_TIMEOUT             The transaction did not complete within an internally
                                  specified timeout period, or the controller is not
                                  available for use.
  @retval EFI_DEVICE_ERROR        There was an Smbus error (NACK) during the operation.
                                  This could indicate the SMBus device is not present
                                  or is in a hung condition.
**/
EFI_STATUS
SmbusExec (
  IN      EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND  Command,
  IN      EFI_SMBUS_OPERATION       Operation,
  IN      BOOLEAN                   PecCheck,
  IN OUT  UINTN                     *Length,
  IN OUT  VOID                      *Buffer
  );

/**
  This function initializes the Smbus Registers.

**/
VOID
InitializeSmbusRegisters (
  VOID
  );

#endif //_SMBUS_COMMON_LIB_H_
