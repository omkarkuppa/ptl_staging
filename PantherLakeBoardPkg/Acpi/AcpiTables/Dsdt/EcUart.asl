/** @file

  ACPI device for EC UART

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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


Device (ECUA)  // EC UART
{
  Name (_HID, EISAID ("PNP0501"))

  Name (_UID, 1)

  //
  // Status Method for EC UART.
  //
  Method (_STA, 0, Serialized)
  {
    If (LNotEqual (EUAR,0))
    {
      Return (0x000F)
    }
    Return (0x0000)
  }
  //
  // Current Resource Setting Method for EC UART.
  //
  Method (_CRS, 0, Serialized)
  {
    //
    // Create the Buffer that stores the resources to be returned.
    //
    Name (BUF0, ResourceTemplate ()
    {
      IO (Decode16,0x03F8,0x03F8,0x01,0x08)
      IRQNoFlags () {4}
    })
    //
    // TO-DO: Need to implement the code updating BUF0 (I/O 0x3F8~0x3FF and IRQ 4 are the default setting)
    // according to the real resources assigned when the EC commands are available.
    //
    Return (BUF0)
  }
  //
  // D0 Method for EC UART.
  //
  Method (_PS0,0,Serialized)
  {
    //
    // TO-DO: Need to implement the code put the device on D0 state when the EC commands are available.
    //
  }
  //
  // D3 Method for EC UART.
  //
  Method (_PS3,0,Serialized)
  {
    //
    // TO-DO: Need to implement the code putting the device on D3 state when the EC commands are available.
    //
  }
}
