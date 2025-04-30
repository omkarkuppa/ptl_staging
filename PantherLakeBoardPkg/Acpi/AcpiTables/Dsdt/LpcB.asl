/** @file
  ACPI DSDT table

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


// LPC Bridge - Device 31, Function 0
Scope (\_SB.PC00.LPCB) {
  Include ("LpcDev.asl")
  Include ("PlatformEcMethods.asl")
  Include ("EcUart.asl")

  // Define the KBC_COMMAND_REG-64, KBC_DATA_REG-60 Registers as an ACPI Operating
  // Region.  These registers will be used to skip kbd mouse
  // resource settings if not present.
  OperationRegion (PKBS, SystemIO, 0x60, 0x05)
  Field (PKBS, ByteAcc, Lock, Preserve)
  {
    PKBD, 8,
    Offset(4),
    PKBC, 8
  }
  Device (PS2K)    // PS2 Keyboard
  {
    Name (_HID,"MSFT0001")
    Name (_CID,EISAID ("PNP0303"))

    Method (_STA)
    {
      If (SCMK) //Scan matrix Keyboard Support Enable
      {
        Return (0x000F)
      }
      Return (0x0000)
    }

    Name (_CRS,ResourceTemplate ()
    {
      IO (Decode16,0x60,0x60,0x01,0x01)
      IO (Decode16,0x64,0x64,0x01,0x01)
      IRQ (Edge,ActiveHigh,Exclusive) {0x01}
    })
  }

  Device (PS2M)    // PS/2 Mouse
  {
    Name (_HID,"MSFT0003")
    Name (_CID,EISAID ("PNP0F03"))

    Method (_STA)
    {
      If (P2ME) //Ps2 Mouse Enable
      {
        Return (0x000F)
      }
      Return (0x0000)
    }

    Name (_CRS,ResourceTemplate ()
    {
      IO (Decode16, 0x60, 0x60, 0x01, 0x01)
      IO (Decode16, 0x64, 0x64, 0x01, 0x01)
      IRQ (Edge, ActiveHigh, Exclusive) {0x0C}
    })
  }
}
