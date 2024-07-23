/**@file
 Audio Secondary Devices ACPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

//
// I2S Devices definition
//
If (I2SB == 0) {
  // I2C0 Controller Scope
  Scope (\_SB.PC00.I2C0) {
    Include ("HdaI2sCodec.asl")
  } // END SCOPE I2C0
}
ElseIf (I2SB == 1) {
  // I2C1 Controller Scope
  Scope (\_SB.PC00.I2C1) {
    Include ("HdaI2sCodec.asl")
  } // END SCOPE I2C1
}
ElseIf (I2SB == 2) {
  // I2C2 Controller Scope
  Scope (\_SB.PC00.I2C2) {
    Include ("HdaI2sCodec.asl")
  } // END SCOPE I2C2
}
ElseIf (I2SB == 3) {
  // I2C3 Controller Scope
  Scope (\_SB.PC00.I2C3) {
    Include ("HdaI2sCodec.asl")
  } // END SCOPE I2C3
}
ElseIf (I2SB == 4) {
  // I2C4 Controller Scope
  Scope (\_SB.PC00.I2C4) {
    Include ("HdaI2sCodec.asl")
  } // END SCOPE I2C4
}
ElseIf (I2SB == 5) {
  // I2C5 Controller Scope
  Scope (\_SB.PC00.I2C5) {
    Include ("HdaI2sCodec.asl")
  } // END SCOPE I2C5
}

//
// Realtek codec definition
//
Scope (\_SB.PC00.HDAS)
{
  //
  // Realtek codec
  //
  Device (RTKC) {
    Name (_ADR, 0x01)
    Method (_STA) {
      If (PXDC == 1) {
        Return (0x0)
      } else {
        Return (0xF)
      }
    }
  } // END Device (RTKC)
} // END Scope (HDAS)
