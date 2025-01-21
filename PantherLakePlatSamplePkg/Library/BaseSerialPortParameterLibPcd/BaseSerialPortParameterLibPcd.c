/** @file
  Serial Port Parameter library instance based on PCD.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
/**
  Returns the serial port baud rate.

  @return  Baud rate of serial port.

**/
UINT32
EFIAPI
GetSerialPortBaudRate (
  VOID
  )
{
  return PcdGet32 (PcdSerialBaudRate);
}

/**
  Sets the serial port baud rate value.

  @param[in]    Baud rate value to be set.

  @retval  TRUE           The baud rate of serial port was sucessfully set.
  @retval  FALSE          The baud rate of serial port could not be set.

**/
BOOLEAN
EFIAPI
SetSerialPortBaudRate (
  UINT32        BaudRate
  )
{
  ASSERT (FALSE);
  return FALSE;
}

/**
  Returns enabled debug interfaces.

  @retval  Enabled debug interfaces bitmask.

**/
UINT8
GetDebugInterface (
  )
{
  return PcdGet8(PcdStatusCodeFlags);

}

VOID
SetDebugInterface (
  UINT8 DebugInterface
  )
{
  return;
}
