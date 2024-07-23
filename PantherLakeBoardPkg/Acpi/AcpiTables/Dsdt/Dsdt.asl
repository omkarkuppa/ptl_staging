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

#include "PlatformBoardId.h"
#include <Include/AcpiDebug.h>
#include <Include/AcpiGpio.h>
#include <Register/PmcRegs.h>

// Definitions for INTB (Interrupt descriptor buffer method)
#define INT_LEVEL_TRIG  0
#define INT_EDGE_TRIG   1
#define INT_ACTIVE_LOW  0
#define INT_ACTIVE_HIGH 1

#define LCH_INTERFACE_USB 0x01
#define LCH_LINK_NUMBER   0x00
#define LCH_FLASH_NUMBER  0x00
#define VGPIO_PIN3        0x03

DefinitionBlock (
  "DSDT.aml",
  "DSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "INTEL",   // OEM ID (6 byte string)
  "PTL     ",// OEM table ID (8 byte string)
  0x0 // OEM version of DSDT table (4 byte Integer)
)

// BEGIN OF ASL SCOPE
{
  Store ("[DSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  External (\_SB.PC00.GFX0.CLID)
  External (\_SB.PC00.GFX0.CDCK)
  External (\_SB.PC00.GFX0.DD1F, DeviceObj)
  External (\_SB.PC00.GFX0.DD2F, DeviceObj)
  External (\_SB.PC00.GFX0.GDCK, MethodObj)
  External (\_SB.PC00.RP01, DeviceObj)
  External (\_SB.PC00.RP02, DeviceObj)
  External (\_SB.PC00.RP03, DeviceObj)
  External (\_SB.PC00.RP04, DeviceObj)
  External (\_SB.PC00.RP05, DeviceObj)
  External (\_SB.PC00.RP06, DeviceObj)
  External (\_SB.PR00._PSS, MethodObj)
  External (\_SB.PR00.LPSS, PkgObj)
  External (\_SB.PR00.TPSS, PkgObj)
  External (\_SB.PR00._PPC, MethodObj)
  External (\_SB.CPPC, IntObj)
  External (\_TZ.TZ00, DeviceObj)
  External (\_TZ.ETMD, IntObj)
  External (\_TZ.FN00._OFF, MethodObj)
  External (\_SB.PC00.TXHC, DeviceObj) // CPU XHCI object of TCSS, it is a must existed object for CPU TCSS
  External (\STD3, FieldUnitObj) // 0: D3 disable. 1: D3Hot support for Storages. 2: D3Cold support for Storages.
  // Miscellaneous services enabled in Project

  Name (TCNB, 0xFFFF0000)
  Name (TCNL, 0xAA55)
  Name (IPNB, 0xFFFF0000)
  Name (IPNL, 0xAA55)
  Name (HGNB, 0xFFFF0000)
  Name (HGNL, 0xAA55)
  Name (IGNB, 0xFFFF0000)
  Name (IGNL, 0xAA55)
  Name (HBNB, 0xFFFF0000)
  Name (HBNL, 0xAA55)
  Name (VMNB, 0xFFFF0000)
  Name (VMNL, 0xAA55)

  Name (TSNB, 0xFFFF0000)
  Name (TSNL, 0xAA55)

  Include ("TcssNvs.asl")
  Include ("IpuNvs.asl")
  Include ("IGpuNvs.asl")
  Include ("HgNvs.asl")
  Include ("HostBridgeNvs.asl")
  Include ("TseNvs.asl")
  Include ("VmdNvs.asl")
  Include ("AmlUpd.asl")
  Include ("PlatformNvs.asl")
  Include ("PciTree.asl")

  #define WADT_WAKE          GPE_WADT_EN
  #define WADT_ACPI_DEV_PATH_ACWA \ACWA
  #define WADT_ACPI_DEV_PATH_DCWA \DCWA
  #define WADT_ACPI_DEV_PATH_ACET \ACET
  #define WADT_ACPI_DEV_PATH_DCET \DCET
  Scope(\_SB.PC00) {
    Include ("TimeAndAlarmDevice.asl")
  }
  #undef WADT_ACPI_DEV_PATH_ACWA
  #undef WADT_ACPI_DEV_PATH_DCWA
  #undef WADT_ACPI_DEV_PATH_ACET
  #undef WADT_ACPI_DEV_PATH_DCET
  #undef WADT_WAKE

  Include ("AcpiAssert.asl")
  Include ("PcieRpObjCommonExternalInclude.asl")
  Include ("Ipf.asl")

  // Naming Rule
  // First 2 chars:
  //   EX - TCSS USB3 Port#
  //   HX - DTBT1 USB3 Port#
  //   JX - DTBT2 USB3 Port#
  // Last 2 chars:
  //   UP - UPC
  //   PL - PLD
  Scope (\_SB) {
    Method (E1UP, 0, Serialized) {
      return (\_SB.TUPC (E1CN, E1TP, E1CP))
    }
    Method (E2UP, 0, Serialized) {
      return (\_SB.TUPC (E2CN, E2TP, E2CP))
    }
    Method (E3UP, 0, Serialized) {
      return (\_SB.TUPC (E3CN, E3TP, E3CP))
    }
    Method (E4UP, 0, Serialized) {
      return (\_SB.TUPC (E4CN, E4TP, E4CP))
    }
    Method (H1UP, 0, Serialized) {
      return (\_SB.TUPC (H1CN, H1TP, H1CP))
    }
    Method (H2UP, 0, Serialized) {
      return (\_SB.TUPC (H2CN, H2TP, H2CP))
    }
    Method (J1UP, 0, Serialized) {
      return (\_SB.TUPC (J1CN, J1TP, J1CP))
    }
    Method (J2UP, 0, Serialized) {
      return (\_SB.TUPC (J2CN, J2TP, J2CP))
    }
    Method (E1PL, 0, Serialized) {
      return (\_SB.TPLD (E1VS, E1GP))
    }
    Method (E2PL, 0, Serialized) {
      return (\_SB.TPLD (E2VS, E2GP))
    }
    Method (E3PL, 0, Serialized) {
      return (\_SB.TPLD (E3VS, E3GP))
    }
    Method (E4PL, 0, Serialized) {
      return (\_SB.TPLD (E4VS, E4GP))
    }
    Method (H1PL, 0, Serialized) {
      return (\_SB.TPLD (H1VS, H1GP))
    }
    Method (H2PL, 0, Serialized) {
      return (\_SB.TPLD (H2VS, H2GP))
    }
    Method (J1PL, 0, Serialized) {
      return (\_SB.TPLD (J1VS, J1GP))
    }
    Method (J2PL, 0, Serialized) {
      return (\_SB.TPLD (J2VS, J2GP))
    }
    Name (PUPP, Package (2) { // Platform UPC Package
      0,                      // UPC package is valid or not
      Package () {
        0xFF,                 // Port is connectable
        0xFF,                 // Connector type (N/A for non-visible ports)
        0x00000000,           // reserved must be zero
        0x00000000
      }
    })

    Name (PPLP, Package (2) {   // Platform PLD Package
      0,                        // PLD package is valid or not
      Buffer (0x14) {
        0x82, 0x00, 0x00, 0x00, // Revision 2, Ignore color width and height is not required as this is an embedded connector
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, // Not user visible
        0xFF, 0xFF, 0xFF, 0xFF  // Not ejectable
      }
    })

    Method (A5UP, 0, Serialized) {
      Store (0, Index (PUPP, 0)) // Set UPC package default to invalid
      If (LAnd (CondRefOf (VTCM), LEqual (VTCM, 1))) { // This entry will expose only when Expose "XHCI SDEV Entry" is enabled
        If (LOr (LEqual (SXP1, 5), LEqual (SXP2, 5))) { // This entry will expose only when Root Port Number is 5 // This enrty will expose only when Expose "XHCI SDEV Entry" is enabled
        }
      }
      Return (PUPP)
    }
    Method (A5PL, 0, serialized) {
      Store (0, Index (PPLP, 0)) // Set PLD package default to invalid
      If (LAnd (CondRefOf (VTCM), LEqual (VTCM, 1))) { // This entry will expose only when Expose "XHCI SDEV Entry" is enabled
        If (LOr (LEqual (SXP1, 5), LEqual (SXP2, 5))) { // This entry will expose only when Root Port Number is 5 // This enrty will expose only when Expose "XHCI SDEV Entry" is enabled
        }
      }
      Return (PPLP)
    }
    Method (A6UP, 0, Serialized) {
      Store (0, Index (PUPP, 0)) // Set PLD package default to invalid
      If (LAnd (CondRefOf (VTCM), LEqual (VTCM, 1))) { // This entry will expose only when Expose "XHCI SDEV Entry" is enabled
        If (LOr (LEqual (SXP1, 6), LEqual (SXP2, 6))) { // This entry will expose only when Root Port Number is 6 // This enrty will expose only when Expose "XHCI SDEV Entry" is enabled
        }
      }
      Return (PUPP)
    }
    Method (A6PL, 0, serialized) {
      Store (0, Index (PPLP, 0)) // Set PLD package default to invalid
      If (LAnd (CondRefOf (VTCM), LEqual (VTCM, 1))) { // This entry will expose only when Expose "XHCI SDEV Entry" is enabled
        If (LOr (LEqual (SXP1, 6), LEqual (SXP2, 6))) { // This entry will expose only when Root Port Number is 6 // This enrty will expose only when Expose "XHCI SDEV Entry" is enabled
        }
      }
      Return (PPLP)
    }
  }

  if (LEqual (ECR1,1)) {
    Scope (\_SB.PC00) {
      //
      // PCI-specific method's GUID
      //
      Name (PCIG, ToUUID ("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))
      //
      // PCI's _DSM - an attempt at modular _DSM implementation
      // When writing your own _DSM function that needs to include PCI-specific methods, do this:
      //
      // Method (_YOUR_DSM,4) {
      //   if (Lequal (Arg0,PCIG)) { return (PCID (Arg0,Arg1,Arg2,Arg3)) }
      //   ...continue your _DSM by checking different GUIDs...
      //   else { return (0) }
      // }
      //
      Method (PCID, 4, Serialized) {
        If (LEqual (Arg0, PCIG)) {         // PCIE capabilities UUID
          If (LGreaterEqual (Arg1,3)) {                                                // revision at least 3
            If (LEqual (Arg2,0)) { Return (Buffer (2) {0x01,0x03}) }                   // function 0: list of supported functions
            If (LEqual (Arg2,8)) { Return (1) }                                        // function 8: Avoiding Power-On Reset Delay Duplication on Sx Resume
            If (LEqual (Arg2,9)) { Return (Package (5) {50000,Ones,Ones,50000,Ones}) } // function 9: Specifying Device Readiness Durations
          }
        }
        return (Buffer (1) {0})
      }
    }//scope
  }//if

  Scope (\_SB.PC00) {
    //PciCheck, Arg0=UUID, returns true if support for 'PCI delays optimization ECR' is enabled and the UUID is correct
    Method (PCIC,1,Serialized) {
      If (LEqual (ECR1,1)) {
        If (LEqual (Arg0, PCIG)) {
          return (1)
        }
      }
      return (0)
    }

    //
    // Create I2C Bus Resource descriptor for _CRS usage
    // Arg0 - I2C bus address of the connection (Target Address)
    // Arg1 - I2C controller number (Resource Source)
    // Returns buffer with 'I2cSerialBusV2' resource descriptor
    //
    Method (IICB, 2, Serialized) {
      Switch (ToInteger (Arg1)) {
        Case (0) {Name (IIC0, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
          "\\_SB.PC00.I2C0", 0x00, ResourceConsumer, DEV0, , ) })
          CreateWordField (IIC0, DEV0._ADR, DAD0)
          Store (Arg0, DAD0)
          Return (IIC0) }
        Case (1) {Name (IIC1, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
          "\\_SB.PC00.I2C1", 0x00, ResourceConsumer, DEV1, , ) })
          CreateWordField (IIC1, DEV1._ADR, DAD1)
          Store (Arg0, DAD1)
          Return (IIC1) }
        Case (2) {Name (IIC2, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
          "\\_SB.PC00.I2C2", 0x00, ResourceConsumer, DEV2, , ) })
          CreateWordField (IIC2, DEV2._ADR, DAD2)
          Store (Arg0, DAD2)
          Return (IIC2) }
        Case (3) {Name (IIC3, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
          "\\_SB.PC00.I2C3", 0x00, ResourceConsumer, DEV3, , ) })
          CreateWordField (IIC3, DEV3._ADR, DAD3)
          Store (Arg0, DAD3)
          Return (IIC3) }
        Case (4) {Name (IIC4, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
          "\\_SB.PC00.I2C4", 0x00, ResourceConsumer, DEV4, , ) })
          CreateWordField (IIC4, DEV4._ADR, DAD4)
          Store (Arg0, DAD4)
          Return (IIC4) }
        Case (5) {Name (IIC5, ResourceTemplate () {I2cSerialBusV2 (0, ControllerInitiated, 400000, AddressingMode7Bit,
          "\\_SB.PC00.I2C5", 0x00, ResourceConsumer, DEV5, , ) })
          CreateWordField (IIC5, DEV5._ADR, DAD5)
          Store (Arg0, DAD5)
          Return (IIC5) }
        Default {Return (0) }
      }
    }

    Method (VPIN, 1, Serialized) { // Create VGPIO resource template buffer
      // Arg0 - Gpio Number
      If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS05.VGPO)) {
        Name (GPOR, ResourceTemplate () {
          GpioIo (Exclusive, PullDefault, 0, 0, IoRestrictionOutputOnly, "\\_SB.PC00.XHCI.RHUB.HS05.VGPO",,,GPOD) { 0xFFFF }
        })
        CreateWordField (GPOR, GPOD._PIN, PINV)
        Store(Arg0, PINV)
        Return (GPOR)
      }
      Return (0)
    }

    //
    // Create I2C Bus Resource descriptor for _CRS usage
    // Arg0 - I2C bus address of the connection (Peripheral Address)
    // Arg1 - I2C Device number
    // Returns buffer with 'I2cSerialBusV2' resource descriptor
    //
    Method (VIIC, 2, Serialized) {
      Switch (ToInteger (Arg1))
      {
        Case (Zero) // When virtual I2C device VIC0 is selected
        {
          If (CondRefOf (\_SB.PC00.XHCI.RHUB.VIC0)) {
            Name (VI00, ResourceTemplate () {
                I2cSerialBusV2 (0, ControllerInitiated, 400000,
                    AddressingMode7Bit, "\\_SB.PC00.XHCI.RHUB.VIC0",
                    0x00, ResourceConsumer, VII0,)
            })

            CreateWordField (VI00, VII0._ADR, DAR0)
            Store (Arg0, DAR0)
            Return (VI00)
          }
          Return (0)
        }
        Case (One) // When virtual I2C device VIC1 is selected
        {
          If (CondRefOf (\_SB.PC00.XHCI.RHUB.VIC1)) {
            Name (VI11, ResourceTemplate () {
                I2cSerialBusV2 (0, ControllerInitiated, 400000,
                    AddressingMode7Bit, "\\_SB.PC00.XHCI.RHUB.VIC1",
                    0x00, ResourceConsumer, VII1,)
            })

            CreateWordField (VI11, VII1._ADR, DAR1)
            Store (Arg0, DAR1)
            Return (VI11)
          }
          Return (0)
        }
        Default
        {
          Return (0)
        }
      }
    }

    //
    // Create Interrupt Resource descriptor for _CRS usage
    // Arg0 - GPIO Pad used as Interrupt source
    // Arg1 - Edge (1) or Level (0) interrupt triggering
    // Arg2 - Active Level: High (1) or Low (0)
    // Returns buffer with 'Interrupt' resource descriptor
    //
    Method (INTB, 3, Serialized) {
      Name (INTR, ResourceTemplate () { Interrupt (ResourceConsumer, Level, ActiveLow, ExclusiveAndWake,,,INTI) {0} })
      // Update APIC Interrupt descriptor
      CreateDWordField (INTR, INTI._INT, NUMI) // Interrupt Number
      Store (INUM (Arg0), NUMI)
      CreateBitField (INTR, INTI._HE, LEVI) // Level or Edge
      Store (Arg1, LEVI)
      CreateBitField (INTR, INTI._LL, ACTI) // Active High or Low
      Store (Arg2, ACTI)

      Return (INTR)
    }

    Method (LNKC, 1, Serialized) {
      If (Land (LEqual (Arg0, LCH_LINK_NUMBER), LEqual (LCHS, LCH_INTERFACE_USB))) {
        Return (1)
      }
      Return (0)
    }

    //
    // Arg0 - Link Index
    // Returns Package with Lch Dependencies for Link Device
    //
    Method (LNKD, 1, Serialized) {
      If (Land (LEqual (Arg0, LCH_LINK_NUMBER), LEqual (LCHS, LCH_INTERFACE_USB))) {
        If (CondRefOf (\_SB.PC00.CVSS)) {
          ADBG ("CVS : Return dependency for Lch specific Camera")
          Return (Package() {"\\_SB.PC00.CVSS"})
        }
        Return (0)
      }
      Return (0)
    }

    Method (FLMC, 1, Serialized) {
      If (Land (LEqual (Arg0, LCH_FLASH_NUMBER), LEqual (LCHS, LCH_INTERFACE_USB))) {
       Return (1)
      }
      Return (0)
    }

    // Arg0 - Link Index
    // Returns Package with Lch Dependencies for Flash Device
    //
    Method (FLMD, 1, Serialized) {
      If (Land (LEqual (Arg0, LCH_FLASH_NUMBER), LEqual (LCHS, LCH_INTERFACE_USB))) {
        If (CondRefOf (\_SB.PC00.CVSS)) {
          ADBG ("CVS : Return dependency for Lch specific Flash")
          Return (Package() {"\\_SB.PC00.CVSS"})
        }
        Return (0)
      }
      Return (0)
    }

    //
    // Arg0 - Link Index
    // Return Lch enabled/disabled status for DsmLch
    //
    Method (DSMM, 1, Serialized) {
      If (LEqual (Arg0, LCH_LINK_NUMBER)) {
        Return (LCHS) // Return Lch enabled/disabled status
      }
      Return (0xFFFFFFFF)
    }

    //
    // Arg0 - Link Index
    // Arg1 - I2C Device Address on Link1
    // Returns buffer with 'I2cSerialBusV2' resource descriptor
    //
    Method (VICC, 2, Serialized) {
      //If (Land (LEqual (Arg0, LCH_LINK_NUMBER), LEqual (LCHS, LCH_INTERFACE_USB))) {
      //  Return (VIIC (Arg1, 0)) // When virtual I2C device VIC0 is selected
      //}
      Return (0)
    }

    //
    // Arg0 - Link Index
    // Returns buffer with 'GpioIo' resource descriptor
    //
    Method (VPN3, 1, Serialized) {
      If (Land (LEqual (Arg0, LCH_LINK_NUMBER), LEqual (LCHS, LCH_INTERFACE_USB))) {
        Return (VPIN (VGPIO_PIN3))
      }
      Return (0)
    }
  } // END Scope (\_SB.PC00)

  Include ("Fru/PtlPcd/AcpiTables/Dsdt/PtlPcd.asl")
  Include ("Fru/PtlPcd/AcpiTables/PciePtl.asl")
  Include ("LpcB.asl")
  Include ("Platform.asl")
  Include ("Cpu.asl")
  Include ("PciDrc.asl")
  Include ("Video.asl")
  Include ("Gpe.asl")
  Include ("VirtualDock.asl")
  Include ("Pep.asl")
  Include ("SerialIoDevices.asl")
  Include ("HdaCodecsDevices.asl")
  Include ("HdaDspModules.asl")

  Include ("HidPlatformEventDev.asl")
  Include ("PinDriverLib.asl")

  If (LEqual (LCHS,1)) {
    Include ("Lch.asl")
  }

  If (LEqual(VMDE,1)) {
    Name(VDSD, 1) // VMD _DSD Method - 1: Expose 0: Hide

    If (LEqual (\STD3, 0)) {
      Store (0, VDSD)
    }
    ADBG (Concatenate ("VMD _DSD Method option:", ToHexString (VDSD)))
    Include ("Fru/PtlPcd/AcpiTables/VmdPtl.asl")
  }
  Include ("Pld.asl")
  Include ("UsbUpcPldLib.asl")

  //
  // WWAN
  //
  Include ("CoExistenceDevice.asl")
  Include ("WirelessConfigDevice.asl")
  Include ("PchRpPxsxWrapper.asl")

  //
  // ISH Runtime D3
  //
  Include("IshD3.asl")

  //
  // PCIe RootPort Power Resources for Wake
  //
  Scope (\_SB.PC00.RP01) {
    Method (PRWH, 1) {
      If (LNotEqual(RW01, 0)) {
        Return (GPRW (GGPE (RW01), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP02) {
    Method (PRWH, 1) {
      If (LNotEqual(RW02, 0)) {
        Return (GPRW (GGPE (RW02), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP03) {
    Method (PRWH, 1) {
      If (LNotEqual(RW03, 0)) {
        Return (GPRW (GGPE (RW03), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP04) {
    Method (PRWH, 1) {
      If (LNotEqual(RW04, 0)) {
        Return (GPRW (GGPE (RW04), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP05) {
    Method (PRWH, 1) {
     If (LNotEqual(RW05, 0)) {
        Return (GPRW (GGPE (RW05), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP06) {
    Method (PRWH, 1) {
      If (LNotEqual(RW06, 0)) {
        Return (GPRW (GGPE (RW06), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP07) {
    Method (PRWH, 1) {
     If (LNotEqual(RW07, 0)) {
        Return (GPRW (GGPE (RW07), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP08) {
    Method (PRWH, 1) {
      If (LNotEqual(RW08, 0)) {
        Return (GPRW (GGPE (RW08), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }

  Scope (\_SB.PC00.RP09) {
    Method (PRWH, 1) {
     If (LNotEqual(RW09, 0)) {
        Return (GPRW (GGPE (RW09), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }
  Scope (\_SB.PC00.RP10) {
    Method (PRWH, 1) {
      If (LNotEqual(RW10, 0)) {
        Return (GPRW (GGPE (RW10), 4))
      } Else {
        Return (GPRW (Arg0, 4))
      }
    }
  }

  If (LEqual (PCHS, PCHH)) {
    Scope (\_SB.PC00.RP11) {
      Method (PRWH, 1) {
       If (LNotEqual(RW11, 0)) {
          Return (GPRW (GGPE (RW11), 4))
        } Else {
          Return (GPRW (Arg0, 4))
        }
      }
    }
    Scope (\_SB.PC00.RP12) {
      Method (PRWH, 1) {
        If (LNotEqual(RW12, 0)) {
          Return (GPRW (GGPE (RW12), 4))
        } Else {
          Return (GPRW (Arg0, 4))
        }
      }
    }
  }

  //
  // Add WWAN Firmware Flash Device Support
  //
  If (LEqual (WFFD, 1)) {
    Include ("WwanFlash.asl")
  }

  //
  // Connectivity
  //
  Include ("CnvDsdt.asl")

  Name (\_S0, Package (4) {0x0,0x0,0,0}) // mandatory System state
  if (SS1) { Name (\_S1, Package (4) {0x1,0x0,0,0})}
  if (SS3) { Name (\_S3, Package (4) {0x5,0x0,0,0})}
  if (SS4) { Name (\_S4, Package (4) {0x6,0x0,0,0})}
  Name (\_S5, Package (4) {0x7,0x0,0,0}) // mandatory System state

  Method (PTS, 1) {        // METHOD CALLED FROM _PTS PRIOR TO ENTER ANY SLEEP STATE
    If (Arg0)              // entering any sleep state
      {
      }
    }
    Method (WAK, 1) {      // METHOD CALLED FROM _WAK RIGHT AFTER WAKE UP
  }

  //
  // PCH xDCI scope for platform specific functions
  //
  If (CondRefOf (\_SB.PC00.XDCI)) {
    Scope (\_SB.PC00.XDCI) {
      Method(XDAT, 0, Serialized) {
        If (CondRefOf (\_SB.PC00.LPCB.H_EC.XDAT)) {
          Return (\_SB.PC00.LPCB.H_EC.XDAT ())
        }
        Return (0)
      }
    }
  }

  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS05)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS05) {
      If (LAnd (CondRefOf (VTCM), LEqual (VTCM, 1))) { // This entry will expose only when Expose "XHCI SDEV Entry" is enabled
        If (LOr (LEqual (SXP1, 5), LEqual (SXP2, 5))) { // This entry will expose only when Root Port Number is 5
          Device (CIR) {
            Method (_ADR) {
              If (LEqual (SXP1, 5)) {
                Return (Add (SXP1, SXI1)) // Address = Port + Interface
              } Else {
                Return (Add (SXP2, SXI2)) // Address = Port + Interface
              }
            }
          }
        }
      }
    }
  }

  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS06)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS06) {
      If (LAnd (CondRefOf (VTCM), LEqual (VTCM, 1))) { // This entry will expose only when Expose "XHCI SDEV Entry" is enabled
        If (LOr (LEqual (SXP1, 6), LEqual (SXP2, 6))) { // This entry will expose only when Root Port Number is 6
          Device (CIR) {
            Method (_ADR) {
              If (LEqual (SXP1, 6)) {
                Return (Add (SXP1, SXI1)) // Address = Port + Interface
              } Else {
                Return (Add (SXP2, SXI2)) // Address = Port + Interface
              }
            }
          }
        }
      }
    }
  }

  ADBG ("[DSDT][AcpiTableExit]")
  Store ("[DSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
}// End of ASL File
