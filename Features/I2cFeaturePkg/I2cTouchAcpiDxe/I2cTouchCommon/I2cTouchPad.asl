/**@file
 Touch Pad Device of ACPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
// TPD Connected Device
//
#define SYNAPTICS_PRECISION_TOUCHPAD  1
#define SYNAPTICS_FORCEPAD            2
// 3,4: reserved
#define SERIAL_IO_TPD_CUSTOM_DEVICE   5 // Custom TouchPad device
#define ALPS_PRECISION_TOUCHPAD       6
#define HYNITRON_HFW68H_THAT_TOUCHPAD 7 // For THAT TouchPad Support

External(PPDI)

//------------------------
// Touch Pads on I2C for common use
// Note: instead of adding more touch pads, parametrize this one with appropriate _HID value and GPIO numbers
//------------------------
  Device (TPD0) {
    Name (HID2,0)
    Name (SBFB, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit, "NULL", , , I2CB) })
    Name (SBFI, ResourceTemplate () {
      Interrupt (ResourceConsumer, Level, ActiveLow, ExclusiveAndWake,,,INTI) {0}
    })
    CreateWordField (SBFB,I2CB._ADR,BADR)
    CreateDWordField (SBFB,I2CB._SPE,SPED)
    CreateDWordField (SBFI,INTI._INT,INT2)
    Method (_INI) {
      Store (INUM (DeRefOf(Index(TPDI,0))), INT2)
      If (LEqual (DeRefOf(Index(TPTD,5)), 0)) {
        SHPO (DeRefOf(Index(TPDI,0)), 1) // configure gpio pad in gpio driver mode
      }
      //
      //  IO APIC support
      //
      If (LEqual (DeRefOf(Index(TPTD,5)), 1)) {
        Xor(PPDI, 1, Local0)
        SGRA (DeRefOf(Index(TPDI,0)), Local0) // Enable GPIRoutIOxAPIC
        SGII (DeRefOf(Index(TPDI,0)), 0) // Disable RX inversion
        GRXE (DeRefOf(Index(TPDI,0)), 0) // Set interrupt delivery to level
      }
      If (LEqual (DeRefOf(Index(TPTD,4)), SYNAPTICS_PRECISION_TOUCHPAD)) {
        Store ("SYNA2393", _HID)
        Store (0x20, HID2)
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,4)), SYNAPTICS_FORCEPAD)) {
        Store ("06CB2846", _HID)
        Store (0x20, HID2)
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,4)), ALPS_PRECISION_TOUCHPAD)) {     // ClickPad
        Store (0x20, HID2)
        Store (0x2C, BADR)
        Return
      }
      If (LEqual(DeRefOf(Index(TPTD,4)), HYNITRON_HFW68H_THAT_TOUCHPAD)) { // THAT TouchPad Support
        Store (0x20, HID2)
        Store (0x2C, BADR)
        If (LEqual (DeRefOf(Index(TPTD,7)), 0)) { Store ( 400000, SPED) }
        If (LEqual (DeRefOf(Index(TPTD,7)), 1)) { Store (1000000, SPED) }
        Return
      }      
      If (LEqual (DeRefOf(Index(TPTD,4)), SERIAL_IO_TPD_CUSTOM_DEVICE)) { // Custom TouchPad
        Store (DeRefOf(Index(TDPH,1)), HID2)
        Store (DeRefOf(Index(TPTD,6)), BADR)
        If (LEqual (DeRefOf(Index(TPTD,7)), 0)) { Store ( 100000, SPED) }
        If (LEqual (DeRefOf(Index(TPTD,7)), 1)) { Store ( 400000, SPED) }
        If (LEqual (DeRefOf(Index(TPTD,7)), 2)) { Store (1000000, SPED) }
        Return
      }
    }

    Name (_HID, EISAID("PNP0C50"))
    Name (_S0W, 3)          // PTP will be in D3hot during CS, and wake capable
    Method (_DSM, 0x4, Serialized) {
      If (LEqual (Arg0,HIDG)) { Return (HIDD (Arg0,Arg1,Arg2,Arg3,HID2)) }
      If (LEqual (Arg0,TP7G)) { Return (TP7D (Arg0,Arg1,Arg2,Arg3,SBFB, G_IN (DeRefOf(Index(TPDI,0)), GPIO_INT_MOD_LEVEL, GPIO_INT_POL_ACTIVE_LOW, GPIO_INT_EXCLUSIVE_AND_WAKE, GPIO_PPI_PULL_DEFAULT, 0))) }
      Return (Buffer (1) {0})
    }

    Method (_STA, 0, NotSerialized) {
      If (LAnd (LNotEqual (DeRefOf(Index(TPTD,4)),0),And (I2CN, SERIAL_IO_I2C_TOUCHPAD))) { Return (0x0F) }
      Return (0x00)
    }

    Method (_CRS, 0, NotSerialized) {
      If (LEqual(DeRefOf(Index(TPTD,5)),0)) {
        Return (
          ConcatenateResTemplate(I2CM (I2CX,BADR,SPED), G_IN (DeRefOf(Index(TPDI,0)), GPIO_INT_MOD_LEVEL, GPIO_INT_POL_ACTIVE_LOW, GPIO_INT_EXCLUSIVE_AND_WAKE, GPIO_PPI_PULL_DEFAULT, 0))
        )
      }
      Return (ConcatenateResTemplate (I2CM (I2CX,BADR,SPED), SBFI))
    }
  }  // Device (TPD0)

