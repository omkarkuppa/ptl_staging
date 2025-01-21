/**@file
  Touch Panel Device of ACPI
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

External (\_SB.TLDT)
External (\_SB.SRXO, MethodObj)
External (\_SB.INUM, MethodObj)
External (\_SB.HIDP, MethodObj)
External (\_SB.IBRD, MethodObj)
External (\_SB.TP7D, MethodObj)
External (G_IN, MethodObj)

//
// TPL Connected Device
//
#define ATMEL3432_PANEL               1
#define ATMEL2952_PANEL               2
#define ELAN9048_PANEL                3
#define NTRIG_SAMSUNG_PANEL           4
#define NTRIG_SHARP_PANEL             5
#define WACOM_PANEL                   6
#define ELAN8E18_PANEL                8
#define SERIAL_IO_TPL_CUSTOM_DEVICE   7 // Custom TouchPanel device

//------------------------
// Touch Panels on I2C for common use
// Note: instead of adding more touch panels, parametrize this one with appropriate _HID value and GPIO numbers
//------------------------


Name (TPTY, 0) // Touch panel Type
Name (TPBR, 0) // Touch panel Bridge
Name (TPCT, 0) // Touch panel I2C Controller
Name (TPIM, 0) // I2C SerialIo Devices Interrupt Mode
Name (TPBA, 0) // I2C Custom TouchPanel's BUS Address
Name (TPBS, 0) // Touch panel Bus Speed
Name (TPHA, 0) // I2C Custom TouchPanel's HID Address
Name (TPIN, 0) // Touch panel IRQ GPIO
Name (TPIP, 0) // TouchPaneL Interrupt Gpio pin polarity
Name (TRT3, 0) // Touch RTD3 support
Name (TPDL, 0) // TouchPanel RTD3 DeLay


Store(DeRefOf(Index (\_SB.TLDT, (TPLD_TYPE_INDEX + TPLD_INDEX * NO_OF_ELT))), TPTY)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_BRIDGE_INDEX + TPLD_INDEX * NO_OF_ELT))), TPBR)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_CONTROLLER_INDEX + TPLD_INDEX * NO_OF_ELT))), TPCT)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_IRQ_MODE_INDEX + TPLD_INDEX * NO_OF_ELT))), TPIM)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_BUS_ADR_INDEX + TPLD_INDEX * NO_OF_ELT))), TPBA)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_BUS_SPEED_INDEX + TPLD_INDEX * NO_OF_ELT))), TPBS)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_HID_ADR_INDEX + TPLD_INDEX * NO_OF_ELT))), TPHA)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_IRQ_GPIO_INDEX + TPLD_INDEX * NO_OF_ELT))), TPIN)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_IRQ_POLARITY_INDEX + TPLD_INDEX * NO_OF_ELT))), TPIP)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_D3_SUPPORT_ENABLE_INDEX + TPLD_INDEX * NO_OF_ELT))), TRT3)
Store(DeRefOf(Index (\_SB.TLDT, (TPLD_D3_DELAY_INDEX + TPLD_INDEX * NO_OF_ELT))), TPDL)

Name (HID2,0)
Name (SBFB, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit, "NULL", , , I2CB) })

Name (SBFI, ResourceTemplate () {
  Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive,,,INTI) {0}
})
  CreateWordField (SBFB,I2CB._ADR,BADR)
  CreateDWordField (SBFB,I2CB._SPE,SPED)
  CreateDWordField (SBFI,INTI._INT,INT2)
  Method (_INI) {

    Store (INUM (TPIN),INT2)
    If (LEqual (TPIM,0)) {
      SHPO (TPIN,1) // configure gpio pad in gpio driver mode
    }
    If (LEqual (TPTY, ATMEL3432_PANEL)) {
      Store ("ATML3432",_HID)
      Store (0,HID2)
      Store (0x4C,BADR)
      Store (400000,SPED)
      Return
    }
    If (LEqual (TPTY, ATMEL2952_PANEL)) {
      Store ("ATML2952",_HID)
      Store (0,HID2)
      Store (0x4A,BADR)
      Store (400000,SPED)
      Return
    }
    If (LEqual (TPTY, ELAN9048_PANEL)) {
      Store ("ELAN9048",_HID)
      Store (1,HID2)
      Store (0x10,BADR)
      Store (400000,SPED)
      Return
    }
    If (LEqual (TPTY, NTRIG_SAMSUNG_PANEL)) {
      Store ("NTRG0001",_HID)
      Store (1,HID2)
      Store (0x07,BADR)
      Store (400000,SPED)
      Return
    }
    If (LEqual (TPTY, NTRIG_SHARP_PANEL)) {
      Store ("NTRG0002",_HID)
      Store (1,HID2)
      Store (0x64,BADR)
      Store (400000,SPED)
      Return
    }
    If (LEqual (TPTY, WACOM_PANEL)) {
      Store ("WCOM508E",_HID)
      Store (1,HID2)
      Store (0x0A,BADR)
      If (LEqual (TPBS,0)) { Store ( 400000,SPED) } // Fast mode
      If (LEqual (TPBS,1)) { Store ( 400000,SPED) } // Fast mode
      If (LEqual (TPBS,2)) { Store (1000000,SPED) } // Fast mode
      Return
    }
    If (LEqual (TPTY, ELAN8E18_PANEL)) {
      Store ("ELAN8E18",_HID)
      Store (1,HID2)
      Store (0x16,BADR)
      If (LEqual (TPBS,0)) { Store ( 100000,SPED) }
      If (LEqual (TPBS,1)) { Store ( 400000,SPED) }
      If (LEqual (TPBS,2)) { Store (1000000,SPED) }
      Return
    }
    If (LEqual (TPTY, SERIAL_IO_TPL_CUSTOM_DEVICE)) { // Custom TouchPanel
      Store ("CUST0000",_HID)
      Store (TPHA,HID2)
      Store (TPBA,BADR)
      If (LEqual (TPBS,0)) { Store ( 100000,SPED) }
      If (LEqual (TPBS,1)) { Store ( 400000,SPED) }
      If (LEqual (TPBS,2)) { Store (1000000,SPED) }
      Return
    }
  }

  Name (_HID, "XXXX0000")
  Name (_CID, "PNP0C50")

  Method (_UID, 0) {
    Return (TPCT)
  }

  Name (_S0W, 4)

  Method (_DSM, 0x4, Serialized) {
    If (LEqual (Arg0,HIDG)) { Return (HIDP (Arg0,Arg1,Arg2,Arg3,HID2)) }
    If (LEqual (Arg0,TP7G)) { Return (TP7D (Arg0,Arg1,Arg2,Arg3,SBFB,G_IN (TPIN, GPIO_INT_MOD_LEVEL, GPIO_INT_POL_ACTIVE_LOW, GPIO_INT_EXCLUSIVE, GPIO_PPI_PULL_DEFAULT, 0))) }
    Return (Buffer (1) {0})
  }

  Method (_STA, 0, NotSerialized) {

    If (LAnd(LNotEqual (TPTY,0),LLess (TPTY,0xFF))) {
      ADBG (Concatenate ("_STA Touch Panel Present on I2C Controller: ", ToHexString (TPCT)))
      Return (0x0F)
    }
    Return (0x00)

  }

  Method (_CRS, 0, NotSerialized) {
    If (LEqual (TPIM,0)) { Return (ConcatenateResTemplate (IBRD (TPBR,TPCT,BADR,SPED), G_IN (TPIN, GPIO_INT_MOD_LEVEL, GPIO_INT_POL_ACTIVE_LOW, GPIO_INT_EXCLUSIVE, GPIO_PPI_PULL_DEFAULT, 0))) }
        Return (ConcatenateResTemplate (IBRD (TPBR,TPCT,BADR,SPED), SBFI))
  }
