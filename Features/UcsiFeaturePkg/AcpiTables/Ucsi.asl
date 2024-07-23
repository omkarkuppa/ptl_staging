/** @file
  ACPI SSDT table for UCSI

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

#ifndef PARENT_OF_LPCB
#define PARENT_OF_LPCB PC00
#endif

#include <Ucsi.h>

DefinitionBlock (
  "Ucsi.aml",
  "SSDT",
  2,
  "Intel_",
  "UcsiTabl",
  0x1000
  )
{
  External (\ADBG, MethodObj)
  #define ADBG(x) \
    If (CondRefOf (ADBG)) { \
      ADBG(x) \
    }

  Store ("[UcsiTabl SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)

  ADBG ("[UcsiTabl SSDT][AcpiTableEntry]")

  External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI, MethodObj)
  External (\_SB.PARENT_OF_LPCB.LPCB.H_EC.WOPM, MethodObj)

  External (XDCE)
  External (USTC)
  External (UCMS)
  External (UDRS)

  Include ("UcsiNvs.asl")

  External (P8XH, MethodObj)
  External (\_SB.PC00.XHCI.RHUB, DeviceObj)
  External (\_SB.PC00.XHCI.RHUB.TPLD, MethodObj)
  External (\_SB.TPLD, MethodObj)
  External (\_SB.TUPC, MethodObj)

  Scope (\_SB)
  {
    Device (UBTC)  // USB type C device
    {
      Name (_HID, EISAID ("USBC000"))
      Name (_CID, EISAID ("PNP0CA0"))
      Name (_UID, 0)
      Name (_DDN, "USB Type C")

      //
      // Message Data Structure Size (MGBS)
      // Returns USB Type-C Message Data Structure Size
      //
      Method (MGBS, 0, Serialized) {
        If (LEqual (UCMS, 0x2)) {
          //
          // UCSI 2.0 MGI and MGO Data Structure size is 0 - 0xFF (0x100)
          //
          Store (0x100, Local0)
        } else {
          //
          // USCI 1.0 MGI and MGO Data Structure size is 0 - 0xF (0x10)
          //
          Store (0x10, Local0)
        }
        ADBG (Concatenate ("USBC.MGBS", ToHexString (Local0)))
        Return (Local0)
      }

      //
      // USB Type-C Message In (UCMI)
      // Returns USB Type-C Message In Data Structure Offset
      //
      Method (UCMI, 0, Serialized) {
        // Get UCSI Controller Data Structure (VERSION+RESERVED+CCI+CONTROL) Length (0x10)
        Store (0x10, Local0)
        // UCSI Input Data Structure offset = USB Type C Opregion base address + UCSI Controller Data Structure Length (0x10)
        Store (Add (UBCB, Local0), Local1)
        ADBG (Concatenate ("UBTC", ToHexString (UBCB)))
        ADBG (Concatenate ("UBTC.UCMI", ToHexString (Local1)))
        Return (Local1)
      }

      //
      // USB Type-C Message Out (UCMO)
      // Returns USB Type-C Message Out Data Structure Offset
      //
      Method (UCMO, 0, Serialized) {
        // Get USB Type-C Message In Data Structure Length
        Store (MGBS (), Local0)
        // USB Type-C Message In Data Structure Length + UCSI Controller Data Structure (VERSION+RESERVED+CCI+CONTROL) Length (0x10)
        Store (Add (Local0, 0x10), Local0)
        // UCSI Output Data Structure offset = USB Type C Opregion base address + USB Type-C Message In Data Structure Length + UCSI Controller Data Structure (0x10)
        Store (Add (UBCB, Local0), Local1)
        ADBG (Concatenate ("UBTC.UCMO", ToHexString (Local1)))
        Return (Local1)
      }

      Name (CRS, ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0x00000000, 0x1000, USBR)
      })

      //
      // UCSI Controller Data Structure - VER+RESERVED+CCI+CONTROL
      //
      OperationRegion (USBC, SystemMemory, UBCB, 0x10)
      Field (USBC, ByteAcc, Lock, Preserve)
      {
        VER1, 8,
        VER2, 8,
        RSV1, 8,
        RSV2, 8,
        Offset (4),
        CCI0, 8,  //  PPM->OPM CCI indicator
        CCI1, 8,
        CCI2, 8,
        CCI3, 8,

        CTL0, 8,  //  OPM->PPM Control message
        CTL1, 8,
        CTL2, 8,
        CTL3, 8,
        CTL4, 8,
        CTL5, 8,
        CTL6, 8,
        CTL7, 8,
      }

      //
      // USB Type-C Message In Data Structure
      //
      OperationRegion (USCI, SystemMemory, UCMI (), MGBS ())
      Field (USCI, ByteAcc, Lock, Preserve)
      {
        // USB Type C Mailbox Interface
        MGI0, 8,  //  PPM->OPM Message In
        MGI1, 8,
        MGI2, 8,
        MGI3, 8,
        MGI4, 8,
        MGI5, 8,
        MGI6, 8,
        MGI7, 8,
        MGI8, 8,
        MGI9, 8,
        MGIA, 8,
        MGIB, 8,
        MGIC, 8,
        MGID, 8,
        MGIE, 8,
        MGIF, 8,
      }

      //
      // USB Type-C Message Out Data Structure
      //
      OperationRegion (UCSO, SystemMemory, UCMO (), MGBS ())
      Field (UCSO, ByteAcc, Lock, Preserve)
      {
        MGO0, 8,  //  OPM->PPM Message Out
        MGO1, 8,
        MGO2, 8,
        MGO3, 8,
        MGO4, 8,
        MGO5, 8,
        MGO6, 8,
        MGO7, 8,
        MGO8, 8,
        MGO9, 8,
        MGOA, 8,
        MGOB, 8,
        MGOC, 8,
        MGOD, 8,
        MGOE, 8,
        MGOF, 8,
      }

      Method (_CRS,0, Serialized)
      {
        CreateDWordField (CRS, \_SB.UBTC.USBR._BAS, CBAS)
        Store (UBCB, CBAS)
        Return (CRS)
      }

      Method (_STA,0)
      {
        If (LAnd (LEqual (USTC, 1), LNotEqual (TCPM, 0))) // Check if USB Type C is supported
        {
          If (LOr (LEqual (UCMS, 1), LEqual (UCMS, 2)))   // Check if UCSI is supported
          {
            Return (0x000F)
          }
        }
        Return (0x0000)
      }

      //
      // Expose connector based on Total supported Connectors
      //

      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0001), 0), LAnd (LOr (LEqual (TP1U, UCSI_ENABLE), LEqual (TP1U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT1, 8),LLessEqual(CRT1,10))))) {
        Device (CR01) { // USB Type C Connector#1
          Name (_ADR, 0x0)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV1, CRP1))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC1, CRT1, CRB1))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0002), 0), LAnd (LOr (LEqual (TP2U, UCSI_ENABLE), LEqual (TP2U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT2, 8),LLessEqual(CRT2,10))))) {
        Device (CR02) { // USB Type C Connector#2
          Name (_ADR, 0x1)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV2, CRP2))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC2, CRT2, CRB2))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0004), 0), LAnd (LOr (LEqual (TP3U, UCSI_ENABLE), LEqual (TP3U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT3, 8),LLessEqual(CRT3,10))))) {
        Device (CR03) { // USB Type C Connector#3
          Name (_ADR, 0x2)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV3, CRP3))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC3, CRT3, CRB3))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0008), 0), LAnd (LOr (LEqual (TP4U, UCSI_ENABLE), LEqual (TP4U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT4, 8),LLessEqual(CRT4,10))))) {
        Device (CR04) { // USB Type C Connector#4
          Name (_ADR, 0x3)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV4, CRP4))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC4, CRT4, CRB4))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0010), 0), LAnd (LOr (LEqual (TP5U, UCSI_ENABLE), LEqual (TP5U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT5, 8),LLessEqual(CRT5,10))))) {
        Device (CR05) { // USB Type C Connector#5
          Name (_ADR, 0x4)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV5, CRP5))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC5, CRT5, CRB5))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0020), 0), LAnd (LOr (LEqual (TP6U, UCSI_ENABLE), LEqual (TP6U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT6, 8),LLessEqual(CRT6,10))))) {
        Device (CR06) { // USB Type C Connector#6
          Name (_ADR, 0x5)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV6, CRP6))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC6, CRT6, CRB6))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0040), 0), LAnd (LOr (LEqual (TP7U, UCSI_ENABLE), LEqual (TP7U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT7, 8),LLessEqual(CRT7,10))))) {
        Device (CR07) { // USB Type C Connector#7
          Name (_ADR, 0x6)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV7, CRP7))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC7, CRT7, CRB7))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0080), 0), LAnd (LOr (LEqual (TP8U, UCSI_ENABLE), LEqual (TP8U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT8, 8),LLessEqual(CRT8,10))))) {
        Device (CR08) { // USB Type C Connector#8
          Name (_ADR, 0x7)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV8, CRP8))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC8, CRT8, CRB8))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0100), 0), LAnd (LOr (LEqual (TP9U, UCSI_ENABLE), LEqual (TP9U, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRT9, 8),LLessEqual(CRT9,10))))) {
        Device (CR09) { // USB Type C Connector#9
          Name (_ADR, 0x8)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRV9, CRP9))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRC9, CRT9, CRB9))
          }
        }
      }
      // If UCxx Driver Support not force Disable
      If (LAnd (LNotEqual (And (TCPM, 0x0200), 0), LAnd (LOr (LEqual (TPAU, UCSI_ENABLE), LEqual (TPAU, UCSI2x_ENABLE)), LAnd(LGreaterEqual (CRTA, 8),LLessEqual(CRTA,10))))) {
        Device (CR0A) { // USB Type C Connector#10
          Name (_ADR, 0x9)
          Method (_PLD, 0, Serialized) {
            Return (\_SB.TPLD (CRVA, CRPA))
          }
          Method (_UPC, 0, Serialized) {
            Return (\_SB.TUPC (CRCA, CRTA, CRBA))
          }
        }
      }

      Method (_DSM, 4, Serialized, 0, UnknownObj, {BuffObj, IntObj, IntObj, PkgObj} )
      {
        Name (OPMP, Buffer (24) {})
        // Compare passed in UUID with supported UUID.
        If (LEqual (Arg0, ToUUID ("6f8398c2-7ca4-11e4-ad36-631042b5008f")))  // UUID for USB type C
        {
          Switch (ToInteger (Arg2))  // Arg2:  0 for query, 1 for write and 2 for read
          {
            Case (0)
            {
              Return (Buffer () {0x3F}) // 1-5 functions defined other than Query.
            }

            Case (1)  // OPM write to EC
            {
              Store (MGO0, Index (OPMP, 0))
              Store (MGO1, Index (OPMP, 1))
              Store (MGO2, Index (OPMP, 2))
              Store (MGO3, Index (OPMP, 3))
              Store (MGO4, Index (OPMP, 4))
              Store (MGO5, Index (OPMP, 5))
              Store (MGO6, Index (OPMP, 6))
              Store (MGO7, Index (OPMP, 7))
              Store (MGO8, Index (OPMP, 8))
              Store (MGO9, Index (OPMP, 9))
              Store (MGOA, Index (OPMP, 10))
              Store (MGOB, Index (OPMP, 11))
              Store (MGOC, Index (OPMP, 12))
              Store (MGOD, Index (OPMP, 13))
              Store (MGOE, Index (OPMP, 14))
              Store (MGOF, Index (OPMP, 15))

              Store (CTL0, Index (OPMP, 16))
              Store (CTL1, Index (OPMP, 17))
              Store (CTL2, Index (OPMP, 18))
              Store (CTL3, Index (OPMP, 19))
              Store (CTL4, Index (OPMP, 20))
              Store (CTL5, Index (OPMP, 21))
              Store (CTL6, Index (OPMP, 22))
              Store (CTL7, Index (OPMP, 23))

              \_SB.PARENT_OF_LPCB.LPCB.H_EC.WOPM (OPMP)
              ADBG ("_DSM OPM write to EC")
            }

            Case (2)  // OPM read from EC
            {
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 0)), MGI0 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 1)), MGI1 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 2)), MGI2 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 3)), MGI3 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 4)), MGI4 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 5)), MGI5 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 6)), MGI6 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 7)), MGI7 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 8)), MGI8 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 9)), MGI9 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 10)), MGIA )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 11)), MGIB )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 12)), MGIC )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 13)), MGID )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 14)), MGIE )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 15)), MGIF )

              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 16)), CCI0 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 17)), CCI1 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 18)), CCI2 )
              Store ( DeRefOf (Index (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPOI(), 19)), CCI3 )
            }

            Case (3) //xDCI FN EN/DIS Status
            {
              Return (XDCE) // 0: Disable 1: Enable
            }

            Case (4) //USBC DR_SWAP Platform support
            {
              Return (UDRS) // 0: Enable DR_SWAP 1: Platform disabled DR_SWAP
            }

            Case (5) //Expose platform UCSI capability
            {
              If (LEqual (UCMS, 0x2)) {
                Return (Buffer () {1}) // 1:  PpmReportedUcsiVersion 2.1
              } Else {
                Return (Buffer () {0}) // 0:  PpmReportedUcsiVersion 1.2
              }
            }
          }  // End switch
        }  // End UUID check for USB type C
        Return (Buffer () {0})
      }  // End _DSM Method
    }  // end of Device
  } // end \_SB scope

  ADBG ("[UcsiTabl SSDT][AcpiTableExit]")
  Store ("[UcsiTabl SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
} // end SSDT