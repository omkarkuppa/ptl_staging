/** @file
  This library class provides capability to get/set serial port baud rate.

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

#ifndef __SERIAL_PORT_PARAMETER_LIB__
#define __SERIAL_PORT_PARAMETER_LIB__

#define STATUS_CODE_USE_RAM        BIT0
#define STATUS_CODE_USE_ISA_SERIAL BIT1
#define STATUS_CODE_USE_USB        BIT2
#define STATUS_CODE_USE_USB3       BIT3
#define STATUS_CODE_USE_SERIALIO   BIT4
#define STATUS_CODE_USE_TRACEHUB   BIT5
#define STATUS_CODE_CMOS_INVALID   BIT6
#define STATUS_CODE_CMOS_VALID     BIT7


/**
  Returns the serial port baud rate.

  @return  Baud rate of serial port.

**/
UINT32
EFIAPI
GetSerialPortBaudRate (
  VOID
  );

/**
  Sets the serial port baud rate value.

  @param[in] BaudRate     Baud rate value to be set.

  @retval  TRUE           The baud rate of serial port was sucessfully set.
  @retval  FALSE          The baud rate of serial port could not be set.

**/
BOOLEAN
EFIAPI
SetSerialPortBaudRate (
  UINT32        BaudRate
  );

/**
  Returns enabled debug interfaces.

  @retval  Enabled debug interfaces bitmask.

**/
UINT8
GetDebugInterface (
  VOID
  );

/**
  Sets debug interfaces.

  @param[in] DebugInterface  Debug interfaces to be set.

**/
VOID
SetDebugInterface (
  UINT8 DebugInterface
  );


#endif
