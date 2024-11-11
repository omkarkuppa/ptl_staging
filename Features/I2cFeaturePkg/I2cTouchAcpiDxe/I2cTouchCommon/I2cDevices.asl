/** @file
  ACPI DSDT table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
// Since asl compiler update to 2018 version
// Empty template will casuse warrning
// using this Macro to replace empty ResourceTemplate with End Tag
//
#ifndef EMPTY_RESOURCE_TEMPLATE
#define EMPTY_RESOURCE_TEMPLATE Buffer(){0x79, 0x00}
#endif

#define GPIO_PPI_PULL_DEFAULT 0x0
#define GPIO_INT_MOD_LEVEL 0x0 // Interrupt is triggered in response to the signal being in either a high or low state.
#define GPIO_INT_POL_ACTIVE_LOW      0x1 // This interrupt is sampled when the signal is low, or false.
#define GPIO_INT_EXCLUSIVE           0x0 // This IO connection is used exclusively by one device, This interrupt is not capable of waking the system.
#define GPIO_INT_EXCLUSIVE_AND_WAKE  0x2

//
//  Define each SerialIO devices these item value must sync SetupVariable.h
//
#define SERIAL_IO_I2C_TOUCHPAD      0x1
#define SERIAL_IO_I2C_TOUCHPANEL    0x2
#define SERIAL_IO_I2C_UCMC          0x4
#define SERIAL_IO_I2C0              0x0
#define SERIAL_IO_I2C1              0x1
#define SERIAL_IO_I2C2              0x2
#define SERIAL_IO_I2C3              0x3
#define SERIAL_IO_I2C4              0x4
#define SERIAL_IO_I2C5              0x5
#define SERIAL_IO_UART0             0x9
#define SERIAL_IO_UART1             0xA
#define SERIAL_IO_UART2             0xB

Name(TPTD,Package () {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF})
Name(TDPH,Package () {0xFFFF,0xFFFF})
Name(TPDI,Package () {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF})


External (GNUM, MethodObj)
External (G_IN, MethodObj)
External (INUM, MethodObj)
External (\_SB.SHPO, MethodObj)
External (\_SB.SRXO, MethodObj)
External (\_SB.GRXE, MethodObj)
External (\_SB.PGPI.SRXO,MethodObj)
External (\_SB.PGPI.GNUM,MethodObj)
External (\_SB.PGPI.INUM,MethodObj)
External (\_SB.PGPI.SHPO,MethodObj)
External (\_SB.PGPI.SGRA,MethodObj)
External (\_SB.PGPI.SGII,MethodObj)
External (\_SB.PGPI.GRXE,MethodObj)
// GPIO methods
External (\_SB.SGRA, MethodObj)
External (\_SB.SGII, MethodObj)

External (PCHA)
External (SDS0)
External (SDS1)
External (SDS2)
External (SDS3)
External (SDS4)
External (SDS5)
External (TPDT)

External (\_SB.PC00,DeviceObj)
External (\_SB.PC02,DeviceObj)

External(\_SB.PC00.I2C0, DeviceObj) //I2C0 Controller
External(\_SB.PC00.I2C0.TPD0, DeviceObj) // Touch pad
External(\_SB.PC00.I2C0, DeviceObj) //I2C1 Controller
External(\_SB.PC00.I2C0.TPL1, DeviceObj) // Touch panel

External(\_SB.PC00.I2C1, DeviceObj) //I2C0 Controller
External(\_SB.PC00.I2C1.TPD0, DeviceObj) // Touch pad
External(\_SB.PC00.I2C1, DeviceObj) //I2C1 Controller
External(\_SB.PC00.I2C1.TPL1, DeviceObj) // Touch panel

External(\_SB.PC00.I2C2, DeviceObj) //I2C0 Controller
External(\_SB.PC00.I2C2.TPD0, DeviceObj) // Touch pad
External(\_SB.PC00.I2C2, DeviceObj) //I2C1 Controller
External(\_SB.PC00.I2C2.TPL1, DeviceObj) // Touch panel

External(\_SB.PC00.I2C3, DeviceObj) //I2C0 Controller
External(\_SB.PC00.I2C3.TPD0, DeviceObj) // Touch pad
External(\_SB.PC00.I2C3, DeviceObj) //I2C1 Controller
External(\_SB.PC00.I2C3.TPL1, DeviceObj) // Touch panel

External(\_SB.PC00.I2C4, DeviceObj) //I2C0 Controller
External(\_SB.PC00.I2C4.TPD0, DeviceObj) // Touch pad
External(\_SB.PC00.I2C4, DeviceObj) //I2C1 Controller
External(\_SB.PC00.I2C4.TPL1, DeviceObj) // Touch panel

External(\_SB.PC00.I2C5, DeviceObj) //I2C0 Controller
External(\_SB.PC00.I2C5.TPD0, DeviceObj) // Touch pad
External(\_SB.PC00.I2C5, DeviceObj) //I2C1 Controller
External(\_SB.PC00.I2C5.TPL1, DeviceObj) // Touch panel



External(\_SB.PC02.I2C0, DeviceObj) //I2C0 Controller
External(\_SB.PC02.I2C0.TPD0, DeviceObj) // Touch pad
External(\_SB.PC02.I2C0, DeviceObj) //I2C1 Controller
External(\_SB.PC02.I2C0.TPL1, DeviceObj) // Touch panel

External(\_SB.PC02.I2C1, DeviceObj) //I2C0 Controller
External(\_SB.PC02.I2C1.TPD0, DeviceObj) // Touch pad
External(\_SB.PC02.I2C1, DeviceObj) //I2C1 Controller
External(\_SB.PC02.I2C1.TPL1, DeviceObj) // Touch panel

External(\_SB.PC02.I2C2, DeviceObj) //I2C0 Controller
External(\_SB.PC02.I2C2.TPD0, DeviceObj) // Touch pad
External(\_SB.PC02.I2C2, DeviceObj) //I2C1 Controller
External(\_SB.PC02.I2C2.TPL1, DeviceObj) // Touch panel

External(\_SB.PC02.I2C3, DeviceObj) //I2C0 Controller
External(\_SB.PC02.I2C3.TPD0, DeviceObj) // Touch pad
External(\_SB.PC02.I2C3, DeviceObj) //I2C1 Controller
External(\_SB.PC02.I2C3.TPL1, DeviceObj) // Touch panel

External(\_SB.PC02.I2C4, DeviceObj) //I2C0 Controller
External(\_SB.PC02.I2C4.TPD0, DeviceObj) // Touch pad
External(\_SB.PC02.I2C4, DeviceObj) //I2C1 Controller
External(\_SB.PC02.I2C4.TPL1, DeviceObj) // Touch panel

External(\_SB.PC02.I2C5, DeviceObj) //I2C0 Controller
External(\_SB.PC02.I2C5.TPD0, DeviceObj) // Touch pad
External(\_SB.PC02.I2C5, DeviceObj) //I2C1 Controller
External(\_SB.PC02.I2C5.TPL1, DeviceObj) // Touch panel


Scope (\_SB.PC00) {

  Name (HIDG, ToUUID ("3CDFF6F7-4267-4555-AD05-B30A3D8938DE")) //GUID for HID-over-I2C
  Name (TP7G, ToUUID ("EF87EB82-F951-46DA-84EC-14871AC6F84B")) //GUID for Touch device support in Windows7

  Method (HIDD, 5, Serialized) {
    // Common _DSM method for HID I2C devices
    // Arg0..3 - original arguments to _DSM method, as passed by OS
    // Arg4 - HID descriptor address to be returned from Function1
    If (LEqual (Arg0, HIDG)) {
      If (LEqual (Arg2, Zero)) {  // Function 0 : Query Function
        If (LEqual (Arg1, One)) { // Revision 1
          Return (Buffer (One) { 0x03 })
        }
      }
      If (LEqual (Arg2, One)) { // Function 1 : HID Function
        Return (Arg4)           // HID Descriptor Address (IHV Specific)
      }
    }
    Return (Buffer (1) {0})
  }

  Method (TP7D, 6, Serialized) {
    // Common _DSM method for touch devices support in Win7
    // Arg0..3 - original arguments to _DSM method, as passed by OS
    // Arg4..5 - buffers containing resources (such as I2c bus, interrupt) to be returned from Function1
    If (LEqual (Arg0, TP7G)) {
      If (LEqual (Arg2, Zero)) {  // Function 0 : Query Function
        If (LEqual (Arg1, One)) { // Revision 1
          Return (Buffer (One) { 0x03 })
        }
      }
      If (LEqual (Arg2, One)) {   // Function 1 : Report resources
        Return (ConcatenateResTemplate (Arg4, Arg5))
      }
    }
    Return (Buffer (1) {0})
  }

  //
  // Create I2C Bus Resource Descriptor for _CRS Usage
  // Arg0 - I2C Controller Number (Resource Source)
  // Arg1 - I2C Bus Address of the Connection (Target Address)
  // Arg2 - I2C Bus Speed of the Connection (Connection Speed)
  // Returns Buffer with 'I2cSerialBusV2' Resource Descriptor
  //
  Method (I2CM, 3, Serialized) {
    Switch (ToInteger (Arg0)) {
      Case (SERIAL_IO_I2C0) { Name (IIC0 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C0", 0x00, ResourceConsumer, DEV0, , ) })
        CreateWordField (IIC0, DEV0._ADR, DAD0)
        CreateDWordField (IIC0, DEV0._SPE, DSP0)
        Store (Arg1, DAD0)
        Store (Arg2, DSP0)
        Return (IIC0) }
      Case (SERIAL_IO_I2C1) { Name (IIC1 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C1", 0x00, ResourceConsumer, DEV1, , ) })
        CreateWordField (IIC1, DEV1._ADR, DAD1)
        CreateDWordField (IIC1, DEV1._SPE, DSP1)
        Store (Arg1, DAD1)
        Store (Arg2, DSP1)
        Return (IIC1) }
      Case (SERIAL_IO_I2C2) { Name (IIC2 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C2", 0x00, ResourceConsumer, DEV2, , ) })
        CreateWordField (IIC2, DEV2._ADR, DAD2)
        CreateDWordField (IIC2, DEV2._SPE, DSP2)
        Store (Arg1, DAD2)
        Store (Arg2, DSP2)
        Return (IIC2) }
      Case (SERIAL_IO_I2C3) { Name (IIC3 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C3", 0x00, ResourceConsumer, DEV3, , ) })
        CreateWordField (IIC3, DEV3._ADR, DAD3)
        CreateDWordField (IIC3, DEV3._SPE, DSP3)
        Store (Arg1, DAD3)
        Store (Arg2, DSP3)
        Return (IIC3) }
      Case (SERIAL_IO_I2C4) { Name (IIC4 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C4", 0x00, ResourceConsumer, DEV4, , ) })
        CreateWordField (IIC4, DEV4._ADR, DAD4)
        CreateDWordField (IIC4, DEV4._SPE, DSP4)
        Store (Arg1, DAD4)
        Store (Arg2, DSP4)
        Return (IIC4) }
      Case (SERIAL_IO_I2C5) { Name (IIC5 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C5", 0x00, ResourceConsumer, DEV5, , ) })
        CreateWordField (IIC5, DEV5._ADR, DAD5)
        CreateDWordField (IIC5, DEV5._SPE, DSP5)
        Store (Arg1, DAD5)
        Store (Arg2, DSP5)
        Return (IIC5) }
      Default {Return (0)}
    }
  } // Method I2CM

  //
  // Create I2C Bus Resource of UCMC descriptor for _CRS usage
  // Arg0 - I2C controller number (Resource Source)
  // Returns buffer with 'I2cSerialBusV2' resource descriptor
  //
  Method (UCMM, 1, Serialized) {
    Switch (ToInteger (Arg0)) {
      Case (SERIAL_IO_I2C0) {
        Name (UCM0 , ResourceTemplate () {
        I2CSerialBusV2 (0x38, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C0", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x3F, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C0", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x20, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C0", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x27, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C0", 0, ResourceConsumer, , , )
        })
        Return (UCM0) }
      Case (SERIAL_IO_I2C1) {
        Name (UCM1 , ResourceTemplate () {
        I2CSerialBusV2 (0x38, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C1", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x3F, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C1", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x20, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C1", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x27, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C1", 0, ResourceConsumer, , , )
        })
        Return (UCM1) }
      Case (SERIAL_IO_I2C2) {
        Name (UCM2 , ResourceTemplate () {
        I2CSerialBusV2 (0x38, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C2", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x3F, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C2", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x20, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C2", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x27, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C2", 0, ResourceConsumer, , , )
        })
        Return (UCM2) }
      Case (SERIAL_IO_I2C3) {
        Name (UCM3 , ResourceTemplate () {
        I2CSerialBusV2 (0x38, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C3", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x3F, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C3", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x20, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C3", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x27, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C3", 0, ResourceConsumer, , , )
        })
        Return (UCM3) }
      Case (SERIAL_IO_I2C4) {
        Name (UCM4 , ResourceTemplate () {
        I2CSerialBusV2 (0x38, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C4", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x3F, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C4", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x20, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C4", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x27, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C4", 0, ResourceConsumer, , , )
        })
        Return (UCM4) }
      Case (SERIAL_IO_I2C5) {
        Name (UCM5 , ResourceTemplate () {
        I2CSerialBusV2 (0x38, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C5", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x3F, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C5", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x20, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C5", 0, ResourceConsumer, , , )
        I2CSerialBusV2 (0x27, ControllerInitiated, 400000, AddressingMode7Bit, "\\_SB.PC00.I2C5", 0, ResourceConsumer, , , )
        })
        Return (UCM5) }
      Default {Return (0)}
    }
  } // Method MCMM
} // Scope SB.PC00


//-----------------------------
//  Serial IO I2C0 Controller
//-----------------------------
Scope (\_SB.PC00.I2C0) {
  Name (I2CN, 0)
  Name (I2CX, 0)
  Name (I2CI, 0)
  Method (_INI) {
    Store (SDS0, I2CN)
    Store (SERIAL_IO_I2C0, I2CX)
  }
  Include ("I2cTouchPad.asl")
  Include ("I2cTouchPanel.asl")
  Include ("SerialIoI2cLtr.asl")
}

//-----------------------------
//  Serial IO I2C1 Controller
//-----------------------------
Scope (\_SB.PC00.I2C1) {
  Name (I2CN, 0)
  Name (I2CX, 0)
  Name (I2CI, 1)
  Method (_INI) {
    Store (SDS1, I2CN)
    Store (SERIAL_IO_I2C1, I2CX)
  }
  Include ("I2cTouchPad.asl")
  Include ("I2cTouchPanel.asl")
  Include ("SerialIoI2cLtr.asl")
}

//-----------------------------
//  Serial IO I2C2 Controller
//-----------------------------
Scope (\_SB.PC00.I2C2) {
  Name (I2CN, 0)
  Name (I2CX, 0)
  Name (I2CI, 2)
  Method (_INI) {
    Store (SDS2, I2CN)
    Store (SERIAL_IO_I2C2, I2CX)
  }
  Include ("I2cTouchPad.asl")
  Include ("I2cTouchPanel.asl")
  Include ("SerialIoI2cLtr.asl")
}

//-----------------------------
//  Serial IO I2C3 Controller
//-----------------------------
Scope (\_SB.PC00.I2C3) {
  Name (I2CN, 0)
  Name (I2CX, 0)
  Name (I2CI, 3)
  Method (_INI) {
    Store (SDS3, I2CN)
    Store (SERIAL_IO_I2C3, I2CX)
  }
  Include ("I2cTouchPad.asl")
  Include ("I2cTouchPanel.asl")
  Include ("SerialIoI2cLtr.asl")
}

//-----------------------------
//  Serial IO I2C4 Controller
//-----------------------------
Scope (\_SB.PC00.I2C4) {
  Name (I2CN, 0)
  Name (I2CX, 0)
  Name (I2CI, 4)
  Method (_INI) {
    Store (SDS4, I2CN)
    Store (SERIAL_IO_I2C4, I2CX)
  }
  Include ("I2cTouchPad.asl")
  Include ("I2cTouchPanel.asl")
  Include ("SerialIoI2cLtr.asl")
}

//-----------------------------
//  Serial IO I2C5 Controller
//-----------------------------
Scope (\_SB.PC00.I2C5) {
  Name (I2CN, 0)
  Name (I2CX, 0)
  Name (I2CI, 5)
  Method (_INI) {
    Store (SDS5, I2CN)
    Store (SERIAL_IO_I2C5, I2CX)
  }
  Include ("I2cTouchPad.asl")
  Include ("I2cTouchPanel2.asl")
  Include ("SerialIoI2cLtr.asl")
}


