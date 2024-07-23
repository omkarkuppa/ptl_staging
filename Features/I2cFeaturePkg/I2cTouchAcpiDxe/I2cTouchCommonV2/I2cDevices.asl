/** @file
  ACPI DSDT table

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

// GPIO methods
External (\_SB.SGRA, MethodObj)
External (\_SB.SGII, MethodObj)

External (\_SB.PC00,DeviceObj)
External (\_SB.PC02,DeviceObj)

External(\_SB.PC00.I2C0, DeviceObj)
External(\_SB.PC00.I2C1, DeviceObj)
External(\_SB.PC00.I2C2, DeviceObj)
External(\_SB.PC00.I2C3, DeviceObj)
External(\_SB.PC00.I2C4, DeviceObj)
External(\_SB.PC00.I2C5, DeviceObj)
External(\_SB.PC00.I2C6, DeviceObj)
External(\_SB.PC00.I2C7, DeviceObj)

External(\_SB.PC02.I2C0, DeviceObj)
External(\_SB.PC02.I2C1, DeviceObj)
External(\_SB.PC02.I2C2, DeviceObj)
External(\_SB.PC02.I2C3, DeviceObj)
External(\_SB.PC02.I2C4, DeviceObj)
External(\_SB.PC02.I2C5, DeviceObj)
External(\_SB.PC02.I2C6, DeviceObj)
External(\_SB.PC02.I2C7, DeviceObj)

//-----------------------------
//  Serial IO I2C0 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C0)) {
  Scope (\_SB.PC00.I2C0) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C1 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C1)) {
  Scope (\_SB.PC00.I2C1) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C2 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C2)) {
  Scope (\_SB.PC00.I2C2) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C3 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C3)) {
  Scope (\_SB.PC00.I2C3) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C4 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C4)) {
  Scope (\_SB.PC00.I2C4) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C5 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C5)) {
  Scope (\_SB.PC00.I2C5) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C6 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C6)) {
  Scope (\_SB.PC00.I2C6) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C7 Controller
//-----------------------------
If (CondRefOf(\_SB.PC00.I2C7)) {
  Scope (\_SB.PC00.I2C7) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C0 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C0)) {
  Scope (\_SB.PC02.I2C0) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C1 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C1)) {
  Scope (\_SB.PC02.I2C1) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C2 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C2)) {
  Scope (\_SB.PC02.I2C2) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C3 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C3)) {
  Scope (\_SB.PC02.I2C3) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C4 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C4)) {
  Scope (\_SB.PC02.I2C4) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C5 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C5)) {
  Scope (\_SB.PC02.I2C5) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C6 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C6)) {
  Scope (\_SB.PC02.I2C6) {
    Include ("SerialIoI2cLtr.asl")
  }
}

//-----------------------------
//  Serial IO I2C7 Controller
//-----------------------------
If (CondRefOf(\_SB.PC02.I2C7)) {
  Scope (\_SB.PC02.I2C7) {
    Include ("SerialIoI2cLtr.asl")
  }
}
