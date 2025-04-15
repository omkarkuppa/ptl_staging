/** @file
  Intel ACPI Reference Code for LCH Driver

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

External (\_SB.PC00.HCID, MethodObj)

Scope(\_SB.PC00) {

  Device(CVVS) {
    ADBG ("CVVS Device Entry")
    Name (_UID, "CVVS")
    Name(_DDN, "Intel CVS Device")

    //
    //This object is used to supply OSPM with the devices PNP ID or ACPI ID.
    //
    Method (_HID) {
      Return ("INTC10E1")
    }

    Name (IICF, 0) // 1: Shared I2C, 0: Dedicated I2C
    Method (IICS, 0, NotSerialized) {
      Return (IICF)
    }

    Method (SID,1, Serialized) {
      Return(\_SB.PC00.HCID(1))
    }

    Name(PKG0, Package() {"\\_SB.PC00.XHCI.RHUB.HS04.VGP1", "\\_SB.PC00.XHCI.RHUB.HS04.VIC2"})

    Method (_DEP, 0, NotSerialized) { // _DEP: Operation Region Dependencies
      If(LEqual (LCHS, 1)) {
        Return (PKG0)
      }
      Return (Package() {})
    }

    Method (_CRS, 0, Serialized) { // _CRS: Current Resource Settings
      //
      // Host Interrupt
      //
      Store (G_IN (LDGP, GPIO_INT_MOD_EDGE, GPIO_INT_POL_ACTIVE_LOW, GPIO_INT_SHARED, GPIO_PPI_PULL_DEFAULT, 0), Local1)

      //
      // CV Chip Reset Pin
      //
      Store (G_IO (LRGP, GPIO_IO_SHR_EXCLUSIVE , GPIO_PPI_PULL_DEFAULT, 0, 0, GPIO_IO_IOR_OUTPUT_ONLY), Local2)
      ConcatenateResTemplate (Local1, Local2, Local0)

      Name (VGB1, ResourceTemplate () {
        //
        // Virtual GPIOs from USB chip - To support GPIO interrupt.
        //
        GpioIo (Exclusive, PullUp, 0, 0, IoRestrictionOutputOnly, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) { 0x00}

        GpioIo (Exclusive, PullUp, 0, 0, IoRestrictionInputOnly, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) { 0x07}
      })

      //
      // Virtual I2C Resource to support CVS Camera
      //
      Name (IIC1, ResourceTemplate () {
              I2cSerialBusV2 (0x0000, ControllerInitiated, 0x400000,
                  AddressingMode7Bit, "\\_SB.PC00.XHCI.RHUB.HS04.VIC2",
                  0x00, ResourceConsumer, DEV1, Exclusive,
                  )
      })
      CreateWordField (IIC1, DEV1._ADR, DAD1)  // _ADR: Address
      DAD1 = 0x76
      Return(ConcatenateResTemplate(ConcatenateResTemplate(Local0, VGB1), IIC1))
    }
  } // End of CVSS(Computer Vision Sensing Support) Device
} // End of Scope (\_SB.PC00)

If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS04)) {
  Scope (\_SB.PC00.XHCI.RHUB.HS04) {
    //
    // Virtual GPIO Device to access the GPIO pins from ucontroller interfaced with CVS.
    //
    Device (VGP1) {
      ADBG ("AEP VGP1 Virutal GPIO Device Entry")
      Name (_UID, "VGP1")
      Name (_DDN, "Intel UsbGpio Device")

      Method (_HID) {
        Return ("INTC10E2")
      }

      //
      // When GPIO test device is enabled in setup.
      //
      If (LEqual (LCHT, 0x01)) { // Virutal GPIO Test Device - Added for validation purpose.
        Device (GPOO) {
          Name (_UID, "GPOT")
          Name (_HID, "GPTS0000")
          Name (_CID, "GPTS0000")

          Method (_CRS, 0x0, Serialized) {
            Name (SBUF,ResourceTemplate () {
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {0}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {1}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {2}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {3}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {4}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {5}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {6}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {7}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {8}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {9}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {10}
              GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionNone, "\\_SB.PC00.XHCI.RHUB.HS04.VGP1",,,,) {11}
            })
            Return (SBUF)
          } // End of _CRS()
        } // End of GPOT
      } // Endof GPOT device check

    }// End of VGP1

    Device (VIC2) {
      ADBG ("AEP VIC2: Virtual I2C Device Entry")
      Name (_UID, "VIC2")
      Name (_DDN, "Intel UsbI2C Device")

      Method (_HID) {
        Return ("INTC10E3")
      }
      //
      // When I2C Test Device is enabled in Setup.
      //
      If (LEqual (LCHT, 0x02)) { // Virutal I2C Test Device - Added for internal validation purpose.
        Device (I2CT) {
          ADBG ("IICT: Virtual I2C Test device Entry")
          Name (_UID, "IICT")
          Name (_HID, "IIC0001")
          Name (_CID, "IIC0001")

          Method (_CRS, 0x0, NotSerialized) {
            Name (VICT, ResourceTemplate () {
              I2cSerialBusV2 (0x0000, ControllerInitiated, 0x400000,
                AddressingMode7Bit,"\\_SB.PC00.XHCI.RHUB.HS04.VIC2",
                0x00, ResourceConsumer, DEV2, Exclusive,
                )
            })
            CreateWordField (VICT, DEV2._ADR, DAD2)
            Store (0x76, DAD2)
            Return (VICT) // Arg1 value 0 is used for Virtual Device VIC2
          }// End of _CRS()
        }// End of IICT Device
      }// Enof IICT check
    }// End of VIC2


    Device (VIC3) {
      ADBG (" AEP VIC3: Virtual I2C Device Entry")
      Name (_UID, "VIC3")
      Name (_DDN, "Intel UsbI2C Device")

      Method (_HID) {
        Return ("INTC10E3")
      }
      //
      // When I2C Test Device is enabled in Setup.
      //
      If (LEqual (LCHT, 0x02)) { // Virutal I2C Test Device - Added for internal validation purpose.
        Device (I2T2) {
          ADBG ("IIT2: Virtual I2C Test device Entry")
          Name (_UID, "IIT2")
          Name (_HID, "IIC0001")
          Name (_CID, "IIC0001")

          Method (_CRS, 0x0, NotSerialized) {
            Name (VCCT, ResourceTemplate () {
              I2cSerialBusV2 (0x0000, ControllerInitiated, 0x400000,
                AddressingMode7Bit, "\\_SB.PC00.XHCI.RHUB.HS04.VIC3",
                0x00, ResourceConsumer, DEV3, Exclusive,
                )
            })
            CreateWordField (VCCT, DEV3._ADR, DAD3)  // _ADR: Address
            Store (0x10, DAD3)
            Return (VCCT)
          }// End of _CRS()
        }// End of IICT Device
      }// Enof IICT check
    }// End of VIC3
  } // End of Scope (\_SB.PC00.XHCI.RHUB.HS04)
}