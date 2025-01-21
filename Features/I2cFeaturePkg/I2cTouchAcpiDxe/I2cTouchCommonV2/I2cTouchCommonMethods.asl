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
**/

//
//  Define each SerialIO devices these item value must sync SetupVariable.h
//
#define SERIAL_IO_I2C0              0x0
#define SERIAL_IO_I2C1              0x1
#define SERIAL_IO_I2C2              0x2
#define SERIAL_IO_I2C3              0x3
#define SERIAL_IO_I2C4              0x4
#define SERIAL_IO_I2C5              0x5
#define SERIAL_IO_I2C6              0x6
#define SERIAL_IO_I2C7              0x7

  Scope (\_SB) {
    Name (HIDG, ToUUID ("3CDFF6F7-4267-4555-AD05-B30A3D8938DE")) //GUID for HID-over-I2C
    Name (TP7G, ToUUID ("EF87EB82-F951-46DA-84EC-14871AC6F84B")) //GUID for Touch device support in Windows7

    Method (HIDP, 5, Serialized) {
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
    // Arg0 - I2C Bridge Number (PC00=0, PC02=2)
    // Arg1 - I2C Controller Number (Resource Source)
    // Arg2 - I2C Bus Address of the Connection (Target Address)
    // Arg3 - I2C Bus Speed of the Connection (Connection Speed)
    // Returns Buffer with 'I2cSerialBusV2' Resource Descriptor
    //
    Method (IBRD, 4, Serialized) {
    If (LEqual (Arg0, 0)) {
      Switch (ToInteger (Arg1)) {
        Case (SERIAL_IO_I2C0) { Name (RT00 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C0", 0x00, ResourceConsumer, DEV0, , ) })
          CreateWordField (RT00, DEV0._ADR, DA00)
          CreateDWordField (RT00, DEV0._SPE, DS00)
          Store (Arg2, DA00)
          Store (Arg3, DS00)
          Return (RT00) }
        Case (SERIAL_IO_I2C1) { Name (RT01 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C1", 0x00, ResourceConsumer, DEV1, , ) })
          CreateWordField (RT01, DEV1._ADR, DA01)
          CreateDWordField (RT01, DEV1._SPE, DS01)
          Store (Arg2, DA01)
          Store (Arg3, DS01)
          Return (RT01) }
        Case (SERIAL_IO_I2C2) { Name (RT02 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C2", 0x00, ResourceConsumer, DEV2, , ) })
          CreateWordField (RT02, DEV2._ADR, DA02)
          CreateDWordField (RT02, DEV2._SPE, DS02)
          Store (Arg2, DA02)
          Store (Arg3, DS02)
          Return (RT02) }
        Case (SERIAL_IO_I2C3) { Name (RT03 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C3", 0x00, ResourceConsumer, DEV3, , ) })
          CreateWordField (RT03, DEV3._ADR, DA03)
          CreateDWordField (RT03, DEV3._SPE, DS03)
          Store (Arg2, DA03)
          Store (Arg3, DS03)
          Return (RT03) }
        Case (SERIAL_IO_I2C4) { Name (RT04 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C4", 0x00, ResourceConsumer, DEV4, , ) })
          CreateWordField (RT04, DEV4._ADR, DA04)
          CreateDWordField (RT04, DEV4._SPE, DS04)
          Store (Arg2, DA04)
          Store (Arg3, DS04)
          Return (RT04) }
        Case (SERIAL_IO_I2C5) { Name (RT05 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C5", 0x00, ResourceConsumer, DEV5, , ) })
          CreateWordField (RT05, DEV5._ADR, DA05)
          CreateDWordField (RT05, DEV5._SPE, DS05)
          Store (Arg2, DA05)
          Store (Arg3, DS05)
          Return (RT05) }
        Case(SERIAL_IO_I2C6) { Name (RT06 , ResourceTemplate () { I2cSerialBus (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C6", 0x00, ResourceConsumer, DEV6,) })
          CreateWordField (RT06, DEV6._ADR, DA06)
          CreateDWordField(RT06,DEV6._SPE,DS06)
          Store (Arg2, DA06)
          Store (Arg3, DS06)
          Return (RT06) }
        Case(SERIAL_IO_I2C7) { Name (RT07 , ResourceTemplate () { I2cSerialBus (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC00.I2C7", 0x00, ResourceConsumer, DEV7,) })
          CreateWordField (RT07, DEV7._ADR, DA07)
          CreateDWordField(RT07,DEV7._SPE,DS07)
          Store (Arg2, DA07)
          Store (Arg3, DS07)
          Return (RT07) }
        Default {Return (0)}
      }
    } ElseIf (LEqual (Arg0, 2)){
      Switch (ToInteger (Arg1)) {
          Case (SERIAL_IO_I2C0) { Name (RT20 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C0", 0x00, ResourceConsumer, D2V0, , ) })
            CreateWordField (RT20, D2V0._ADR, DA20)
            CreateDWordField (RT20, D2V0._SPE, DS20)
            Store (Arg2, DA20)
            Store (Arg3, DS20)
            Return (RT20) }
          Case (SERIAL_IO_I2C1) { Name (RT21 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C1", 0x00, ResourceConsumer, D2V1, , ) })
            CreateWordField (RT21, D2V1._ADR, DA21)
            CreateDWordField (RT21, D2V1._SPE, DS21)
            Store (Arg2, DA21)
            Store (Arg3, DS21)
            Return (RT21) }
          Case (SERIAL_IO_I2C2) { Name (RT22 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C2", 0x00, ResourceConsumer, D2V2, , ) })
            CreateWordField (RT22, D2V2._ADR, DA22)
            CreateDWordField (RT22, D2V2._SPE, DS22)
            Store (Arg2, DA22)
            Store (Arg3, DS22)
            Return (RT22) }
          Case (SERIAL_IO_I2C3) { Name (RT23 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C3", 0x00, ResourceConsumer, D2V3, , ) })
            CreateWordField (RT23, D2V3._ADR, DA23)
            CreateDWordField (RT23, D2V3._SPE, DS23)
            Store (Arg2, DA23)
            Store (Arg3, DS23)
            Return (RT23) }
          Case (SERIAL_IO_I2C4) { Name (RT24 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C4", 0x00, ResourceConsumer, D2V4, , ) })
            CreateWordField (RT24, D2V4._ADR, DA24)
            CreateDWordField (RT24, D2V4._SPE, DS24)
            Store (Arg2, DA24)
            Store (Arg3, DS24)
            Return (RT24) }
          Case (SERIAL_IO_I2C5) { Name (RT25 , ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C5", 0x00, ResourceConsumer, D2V5, , ) })
            CreateWordField (RT25, D2V5._ADR, DA25)
            CreateDWordField (RT25, D2V5._SPE, DS25)
            Store (Arg2, DA25)
            Store (Arg3, DS25)
            Return (RT25) }
          Case(SERIAL_IO_I2C6) { Name (RT26 , ResourceTemplate () { I2cSerialBus (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C6", 0x00, ResourceConsumer, D2V6,) })
            CreateWordField (RT26, D2V6._ADR, DA26)
            CreateDWordField(RT26,D2V6._SPE,DS26)
            Store (Arg2, DA26)
            Store (Arg3, DS26)
            Return (RT26) }
          Case(SERIAL_IO_I2C7) { Name (RT27 , ResourceTemplate () { I2cSerialBus (0, ControllerInitiated, 400000, AddressingMode7Bit,"\\_SB.PC02.I2C7", 0x00, ResourceConsumer, D2V7,) })
            CreateWordField (RT27, D2V7._ADR, DA27)
            CreateDWordField(RT27,D2V7._SPE,DS27)
            Store (Arg2, DA27)
            Store (Arg3, DS27)
            Return (RT27) }
          Default {Return (0)}
        }
    }
    Return (0)
   } // Method IBRD

  } // Scope \_SB
