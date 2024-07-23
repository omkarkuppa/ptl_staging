/** @file
  ACPI DSDT table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#include <Include/AcpiDebug.h>

DefinitionBlock (
  "DUMMYDSDT.aml",
  "DSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "INTEL",   // OEM ID (6 byte string)
  "CNL     ",// OEM table ID  (8 byte string)
  0x0 // OEM version of DSDT table (4 byte Integer)
)
// BEGIN OF ASL SCOPE
{
External (ECR1)
External (PICM)
External (HIDW, MethodObj)
External (HIWC, MethodObj)
External (\_SB.PC00.HDAS.PPMS, MethodObj)
External (\_SB.VMON, MethodObj)
External (\_SB.VMOF, MethodObj)

  Scope (\)
  {
    Name (PCIG, ToUUID ("E5C937D0-3553-4d7a-9117-EA4D19C3434D"))
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
    // Procedure: GPRW
    //
    // Description: Generic Wake up Control Method ("Big brother")
    //              to detect the Max Sleep State available in ASL Name scope
    //              and Return the Package compatible with _PRW format.
    // Input: Arg0 =  bit offset within GPE register space device event will be triggered to.
    //        Arg1 =  Max Sleep state, device can resume the System from.
    //                If Arg1 = 0, Update Arg1 with Max _Sx state enabled in the System.
    // Output:  _PRW package
    //
    Name (PRWP, Package () {Zero, Zero})   // _PRW Package
    Name (SS1,0)
    Name (SS2,0)
    Name (SS3,1)
    Name (SS4,1)

    Method (GPRW, 2)
    {
      Store (Arg0, Index (PRWP, 0))             // copy GPE#
      //
      // SS1-SS4 - enabled in BIOS Setup Sleep states
      //
      Store (ShiftLeft (SS1,1),Local0)          // S1 ?
      Or (Local0,ShiftLeft (SS2,2),Local0)      // S2 ?
      Or (Local0,ShiftLeft (SS3,3),Local0)      // S3 ?
      Or (Local0,ShiftLeft (SS4,4),Local0)      // S4 ?
      //
      // Local0 has a bit mask of enabled Sx(1 based)
      // bit mask of enabled in BIOS Setup Sleep states(1 based)
      //
      If (And (ShiftLeft (1, Arg1), Local0))
      {
        //
        // Requested wake up value (Arg1) is present in Sx list of available Sleep states
        //
        Store (Arg1, Index (PRWP, 1))           // copy Sx#
      }
      Else
      {
        //
        // Not available -> match Wake up value to the higher Sx state
        //
        ShiftRight (Local0, 1, Local0)
        // If (LOr (LEqual (OSFL, 1), LEqual (OSFL, 2))) {  // ??? Win9x
        // FindSetLeftBit (Local0, Index (PRWP,1))  // Arg1 == Max Sx
        // } Else {           // ??? Win2k / XP
        FindSetLeftBit (Local0, Index (PRWP,1))  // Arg1 == Min Sx
        // }
      }

      Return (PRWP)
    }
  } // end \ scope

  Scope (\_SB) {
    Name (PD04, 0x0000FFFF)
    Name (PD05, 0x0000FFFF)
    Name (PD06, 0x0000FFFF)
    Name (PD07, 0x0000FFFF)
    Name (AR04, 0x0000FFFF)
    Name (AR05, 0x0000FFFF)
    Name (AR06, 0x0000FFFF)
    Name (AR07, 0x0000FFFF)


    Device (PC00) { // PCI Bridge "Host Bridge"
      Name (_HID, EISAID("PNP0A08"))
      //
      // Get PCIe BAR
      //
      Method (GPCB,0,Serialized)
      {
        Return (0xE0000000)
      }
    }
  } // end _SB scope
  #include <Base.h>
  Include ("Pch.asl")
}// End of ASL File
