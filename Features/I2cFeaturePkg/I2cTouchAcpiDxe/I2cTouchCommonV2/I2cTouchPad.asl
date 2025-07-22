/**@file
Touch Pad Device of ACPI
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
// TPD Connected Device
//
#define SYNAPTICS_PRECISION_TOUCHPAD  1
#define SYNAPTICS_FORCEPAD            2
// 3,4: reserved
#define SERIAL_IO_TPD_CUSTOM_DEVICE   5 // Custom TouchPad device
#define ALPS_PRECISION_TOUCHPAD       6
#define HYNITRON_HFW68H_THAT_TOUCHPAD 7 // For THAT TouchPad Support
#define SENSEL_FORCEPAD               8 // For Sensel ForcePad Support

External (TPDP)
External (TPIN)
External (PCHA)
External (TPIM)
External (TPHA)
External (TPBA)
External (TPBS)

External (\_SB.SRXO, MethodObj)
External (\_SB.PGPI.SRXO, MethodObj)
External (\_SB.PGPI.INUM, MethodObj)
External (\_SB.PGPI.SHPO, MethodObj)
External (\_SB.PGPI.SGRA, MethodObj)
External (\_SB.PGPI.SGII, MethodObj)
External (\_SB.GRXE, MethodObj)
External (\_SB.PGPI.GRXE, MethodObj)

External (\_SB.INUM, MethodObj)
External (\_SB.HIDP, MethodObj)
External (\_SB.IBRD, MethodObj)
External (\_SB.TP7D, MethodObj)
External (G_IN, MethodObj)

//------------------------
// Touch Pads on I2C for common use
// Note: instead of adding more touch pads, parametrize this one with appropriate _HID value and GPIO numbers
//------------------------

Name (TPTY, 0) // Touch panel Type
Name (TPBR, 0) // Touch panel Bridge
Name (TPCT, 0) // Touch panel I2C Controller
Name (TPIM, 0) // I2C SerialIo Devices Interrupt Mode
Name (TPBA, 0) // I2C Custom TouchPanel's BUS Address
Name (TPBS, 0) // Touch panel Bus Speed
Name (TPHA, 0) // I2C Custom TouchPanel's HID Address
Name (TPIN, 0) // Touch panel IRQ GPIO
Name (TPIP, 0)  // TouchPaneL Interrupt Gpio pin polarity
Name (TRT3, 0)  //Touch RTD3 support
Name (TPDL, 0)  // TouchPad RTD3 DeLay


Store(DeRefOf(Index (\_SB.TDDT, (TPLD_TYPE_INDEX + TPLD_INDEX * NO_OF_ELT))), TPTY)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_BRIDGE_INDEX + TPLD_INDEX * NO_OF_ELT))), TPBR)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_CONTROLLER_INDEX + TPLD_INDEX * NO_OF_ELT))), TPCT)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_IRQ_MODE_INDEX + TPLD_INDEX * NO_OF_ELT))), TPIM)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_BUS_ADR_INDEX + TPLD_INDEX * NO_OF_ELT))), TPBA)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_BUS_SPEED_INDEX + TPLD_INDEX * NO_OF_ELT))), TPBS)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_HID_ADR_INDEX + TPLD_INDEX * NO_OF_ELT))), TPHA)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_IRQ_GPIO_INDEX + TPLD_INDEX * NO_OF_ELT))), TPIN)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_IRQ_POLARITY_INDEX + TPLD_INDEX * NO_OF_ELT))), TPIP)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_D3_SUPPORT_ENABLE_INDEX + TPLD_INDEX * NO_OF_ELT))), TRT3)
Store(DeRefOf(Index (\_SB.TDDT, (TPLD_D3_DELAY_INDEX + TPLD_INDEX * NO_OF_ELT))), TPDL)

Name (HID2,0)
Name (SBFB, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit, "NULL", , , I2CB) })
Name (SBFI, ResourceTemplate () {
  Interrupt (ResourceConsumer, Level, ActiveLow, ExclusiveAndWake,,,INTI) {0}
})
CreateWordField (SBFB,I2CB._ADR,BADR)
CreateDWordField (SBFB,I2CB._SPE,SPED)
CreateDWordField (SBFI,INTI._INT,INT2)
Method (_INI) {
  If (LEqual (PCHA, 0)) {
    Store (INUM (TPIN), INT2)
    If (LEqual (TPIM, 0)) {
      SHPO (TPIN, 1) // configure gpio pad in gpio driver mode
    }
    //
    //  IO APIC support
    //
    If (LEqual (TPIM, 1)) {
      Xor (TPDP, 1, Local0)
      SGRA (TPIN, Local0) // Enable GPIRoutIOxAPIC
      SGII (TPIN, 0) // Disable RX inversion
      GRXE (TPIN, 0) // Set interrupt delivery to level
    }
  } else {
    Store (\_SB.PGPI.INUM (TPIN), INT2)
    If (LEqual (TPIM, 0)) {
      \_SB.PGPI.SHPO (TPIN, 1) // configure gpio pad in gpio driver mode
    }
    //
    //  IO APIC support
    //
    If (LEqual (TPIM, 1)) {
      Xor(TPDP, 1, Local0)
      \_SB.PGPI.SGRA (TPIN, Local0) // Enable GPIRoutIOxAPIC
      \_SB.PGPI.SGII (TPIN, 0) // Disable RX inversion
      \_SB.PGPI.GRXE (TPIN, 0) // Set interrupt delivery to level
    }
  }
  If (LEqual (TPTY, SYNAPTICS_PRECISION_TOUCHPAD)) {
    Store ("SYNA2393", _HID)
    Store (0x20, HID2)
    Return
  }
  If (LEqual (TPTY, SYNAPTICS_FORCEPAD)) {
    Store ("06CB2846", _HID)
    Store (0x20, HID2)
    Return
  }
  If (LEqual (TPTY, ALPS_PRECISION_TOUCHPAD)) {     // ClickPad
    Store (0x20, HID2)
    Store (0x2C, BADR)
    Return
  }
  If (LEqual(TPTY, HYNITRON_HFW68H_THAT_TOUCHPAD)) {            // THAT TouchPad Support
    Store (0x20, HID2)
    Store (0x2C, BADR)
    If (LEqual (TPBS, 0)) { Store ( 400000, SPED) }
    If (LEqual (TPBS, 1)) { Store ( 1000000, SPED) }
    Return
  }
  If (LEqual(TPTY, SENSEL_FORCEPAD)) { // Sensel ForcePad Support
    Store ("SNSL0033", _HID)    // Hardware ID
    Store (0x0001, HID2)        // HID descriptor address
    Store (0x2C,   BADR)        // HID slave Address
    Return
  }
  If (LEqual (TPTY, SERIAL_IO_TPD_CUSTOM_DEVICE)) { // Custom TouchPad
    Store (TPHA, HID2)
    Store (TPBA, BADR)
    If (LEqual (TPBS, 0)) { Store ( 100000, SPED) }
    If (LEqual (TPBS, 1)) { Store ( 400000, SPED) }
    If (LEqual (TPBS, 2)) { Store ( 1000000, SPED) }
    Return
  }
}

Name (_HID, EISAID ("PNP0C50"))
Name (_S0W, 3)          // PTP will be in D3hot during CS, and wake capable
Method (_DSM, 0x4, Serialized) {
  If (LEqual (Arg0,HIDG)) { Return (HIDP (Arg0,Arg1,Arg2,Arg3,HID2)) }
  If (LEqual (Arg0,TP7G)) { Return (TP7D (Arg0,Arg1,Arg2,Arg3,SBFB,G_IN (TPIN, GPIO_INT_MOD_LEVEL, GPIO_INT_POL_ACTIVE_LOW, GPIO_INT_EXCLUSIVE_AND_WAKE, GPIO_PPI_PULL_DEFAULT, 0))) }
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
  If (LEqual (TPIM,0)) { Return (ConcatenateResTemplate (IBRD (TPBR,TPCT,BADR,SPED), G_IN (TPIN, GPIO_INT_MOD_LEVEL, GPIO_INT_POL_ACTIVE_LOW, GPIO_INT_EXCLUSIVE_AND_WAKE, GPIO_PPI_PULL_DEFAULT, 0))) }
  Return (ConcatenateResTemplate (IBRD (TPBR,TPCT,BADR,SPED), SBFI))
}
