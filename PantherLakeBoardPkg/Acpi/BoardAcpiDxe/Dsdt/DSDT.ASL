/** @file
  ACPI DSDT table

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

#define FlavorUnknown                       0x0
#define FlavorMobile                        0x1
#define FlavorDesktop                       0x2
#define FlavorWorkstation                   0x3
#define FlavorUpServer                      0x4
#define FlavorEmbedded                      0x5
#define FlavorPlatformMax                   0x6

#define TypeUnknown                         0x0
#define TypeTrad                            0x1
#define TypeUltUlx                          0x2

// Definitions for INTB (Interrupt descriptor buffer method)
#define INT_LEVEL_TRIG  0
#define INT_EDGE_TRIG   1
#define INT_ACTIVE_LOW  0
#define INT_ACTIVE_HIGH 1

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
  External (\_SB.PC00.GFX0.CDCK)
  External (\_SB.PC00.GFX0.DD1F, DeviceObj)
  External (\_SB.PC00.GFX0.GDCK, MethodObj)
  External (\_SB.PC00.GFX0.GLID, MethodObj)
  External (\_SB.PC00.HDAS.PPMS, MethodObj)
  External (\_SB.PR00._PSS, MethodObj)
  External (\_SB.PR00.LPSS, PkgObj)
  External (\_SB.PR00.TPSS, PkgObj)
  External (\_SB.PR00._PPC, MethodObj)
  External (\_SB.CPPC, IntObj)
  External (\_SB.VMOF, MethodObj)
  External (\_SB.VMON, MethodObj)
  External (\_TZ.TZ00, DeviceObj)
  External (\_TZ.TZ01, DeviceObj)
  External (\_TZ.ETMD, IntObj)
  External (\_TZ.FN00._OFF, MethodObj)
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
  Include ("VmdNvs.asl")
  Include ("TseNvs.asl")
  Include ("AMLUPD.asl")
  Include ("PlatformNvs.asl")
  Include ("PciTree.asl")


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
          If (LGreaterEqual (Arg1,3)) {                                               // revision at least 3
            If (LEqual (Arg2,0)) { Return (Buffer (2) {0x01,0x03}) }                   // function 0: list of supported functions
            If (LEqual (Arg2,8)) { Return (1) }                                       // function 8: Avoiding Power-On Reset Delay Duplication on Sx Resume
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
        Default {Return (0)}
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
      CreateByteField (INTR, INTI._HE, LEVI)   // Level or Edge
      Store (Arg1, LEVI)
      CreateByteField (INTR, INTI._LL, ACTI)   // Active High or Low
      Store (Arg2, ACTI)

      Return (INTR)
    }

  } // END Scope (\_SB.PC00)

  Include ("Pch.asl")  // Not in this package. Refer to the PCH Reference Code accordingly
  //
  // PCI Express definition
  //
  Include ("Fru/PtlPcd/AcpiTables/PciePtl.asl")
  Include ("LpcB.asl")
  Include ("Platform.asl")
  Include ("PCI_DRC.ASL")
  Include ("Video.asl")
  Include ("Gpe.asl")

  If (LEqual (VMDE,1)) {
    Name (VDSD, 1) // VMD _DSD Method - 1: Expose 0: Hide

    If (LEqual (\STD3, 0)) {
      Store (0, VDSD)
    }
    ADBG (Concatenate ("VMD _DSD Method option:", ToHexString (VDSD)))
    Include ("Fru/PtlPcd/AcpiTables/VmdPtl.asl")
  }

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
}// End of ASL File

