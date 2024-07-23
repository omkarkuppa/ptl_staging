/**@file
 Touch Panel Device of ACPI

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
// TPL Connected Device
//
#define ATMEL3432_PANEL               1
#define ATMEL2952_PANEL               2
#define ELAN9048_PANEL                3
#define NTRIG_SAMSUNG_PANEL           4
#define NTRIG_SHARP_PANEL             5
#define WACOM_PANEL                   6
#define SERIAL_IO_TPL_CUSTOM_DEVICE   7 // Custom TouchPanel device

//------------------------
// Touch Panels on I2C for common use
// Note: instead of adding more touch panels, parametrize this one with appropriate _HID value and GPIO numbers
//------------------------
  Device (TPL1) {
    Name (HID2,0)
    Name (TPGI, 0)
    Name (SBFB, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit, "NULL", , , I2CB) })
    Name (SBFG, ResourceTemplate () {
      GpioInt (Level, ActiveLow, Exclusive, PullUp, 0x0000, "\\_SB.GPI0", 0x00, ResourceConsumer,INTG ) { 0 }
    })
    Name (SBFI, ResourceTemplate () {
      Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive,,,INTI) {0}
    })
    CreateWordField (SBFB,I2CB._ADR,BADR)
    CreateDWordField (SBFB,I2CB._SPE,SPED)
    CreateWordField (SBFG,INTG._PIN,INT1)
    CreateDWordField (SBFI,INTI._INT,INT2)
    Method (_INI) {
      Store(DeRefOf(Index(TPDI,2)), TPGI)
      Store (GNUM (TPGI),INT1)
      Store (INUM (TPGI),INT2)
      If (LEqual (DeRefOf(Index(TPTD,1)),0)) {
        SHPO (TPGI,1) // configure gpio pad in gpio driver mode
      }
      If (LEqual (DeRefOf(Index(TPTD,0)), ATMEL3432_PANEL)) {
        Store ("ATML3432",_HID)
        Store (0,HID2)
        Store (0x4C,BADR)
        Store (400000,SPED)
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,0)), ATMEL2952_PANEL)) {
        Store ("ATML2952",_HID)
        Store (0,HID2)
        Store (0x4A,BADR)
        Store (400000,SPED)
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,0)), ELAN9048_PANEL)) {
        Store ("ELAN9048",_HID)
        Store (1,HID2)
        Store (0x10,BADR)
        Store (400000,SPED)
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,0)), NTRIG_SAMSUNG_PANEL)) {
        Store ("NTRG0001",_HID)
        Store (1,HID2)
        Store (0x07,BADR)
        Store (400000,SPED)
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,0)), NTRIG_SHARP_PANEL)) {
        Store ("NTRG0002",_HID)
        Store (1,HID2)
        Store (0x64,BADR)
        Store (400000,SPED)
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,0)), WACOM_PANEL)) {
        Store ("WCOM508E",_HID)
        Store (1,HID2)
        Store (0x0A,BADR)
        If (LEqual (DeRefOf(Index(TPTD,3)),0)) { Store ( 400000,SPED) } // Fast mode
        If (LEqual (DeRefOf(Index(TPTD,3)),1)) { Store ( 400000,SPED) } // Fast mode
        If (LEqual (DeRefOf(Index(TPTD,3)),2)) { Store (1000000,SPED) } // Fast mode
        Return
      }
      If (LEqual (DeRefOf(Index(TPTD,0)), SERIAL_IO_TPL_CUSTOM_DEVICE)) { // Custom TouchPanel
        Store ("CUST0000",_HID)
        Store (DeRefOf(Index(TDPH,0)),HID2)
        Store (DeRefOf(Index(TPTD,2)),BADR)
        If (LEqual (DeRefOf(Index(TPTD,3)),0)) { Store ( 100000,SPED) }
        If (LEqual (DeRefOf(Index(TPTD,3)),1)) { Store ( 400000,SPED) }
        If (LEqual (DeRefOf(Index(TPTD,3)),2)) { Store (1000000,SPED) }
        Return
      }
    }

    Name (_HID, "XXXX0000")
    Name (_CID, "PNP0C50")
    Method (_UID, 0) {
      Return (I2CI)
    }

    Name (_S0W, 4)
    Method (_DSM, 0x4, Serialized) {
      If (LEqual (Arg0,HIDG)) { Return (HIDD (Arg0,Arg1,Arg2,Arg3,HID2)) }
      If (LEqual (Arg0,TP7G)) { Return (TP7D (Arg0,Arg1,Arg2,Arg3,SBFB,SBFG)) }
      Return (Buffer (1) {0})
    }

    Method (_STA, 0, NotSerialized) {
      If (LAnd (LNotEqual (DeRefOf(Index(TPTD,0)),0),And ( I2CN, SERIAL_IO_I2C_TOUCHPANEL))) { Return (0x0F) }
      Return (0x00)
    }

    Method (_CRS, 0, NotSerialized) {
      If (LEqual (DeRefOf(Index(TPTD,1)),0)) { Return (ConcatenateResTemplate (I2CM (I2CX,BADR,SPED), SBFG)) }
      Return (ConcatenateResTemplate (I2CM (I2CX,BADR,SPED), SBFI))
    }
  } // Device (TPL1)

