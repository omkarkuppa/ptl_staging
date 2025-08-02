/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

External (\_SB.PC00.LPCB.H_EC.B1C1, FieldUnitObj)

Scope (\_SB.IETM)
{

  Device (CHRG)  // Charger participant
  {
    Name (_UID, "CHRG")
    Method (_HID)
    {
      Return(\_SB.PLDT.GHID (_UID))  // Intel(R) Dynamic Tuning Technology Charger device
    }

    Name (_STR, Unicode ("Charger"))
    Name (PTYP, 0x0B)

    // PFLG
    //   0 - Default, participant is on the main board
    //   1 - Participant device is on the docking station
    //   2 - Participant device is on the detachable base
    //   3 - Participant device is an external device (such as a USB device, Intel (R) Dynamic Tuning Technology does not support this type of device today)
    Name (PFLG, 0)

    // _STA (Status)
    //
    // This object returns the current status of a device.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing a device status bitmap:
    //    Bit 0 - Set if the device is present.
    //    Bit 1 - Set if the device is enabled and decoding its resources.
    //    Bit 2 - Set if the device should be shown in the UI.
    //    Bit 3 - Set if the device is functioning properly (cleared if device failed its diagnostics).
    //    Bit 4 - Set if the battery is present.
    //    Bits 5-31 - Reserved (must be cleared).
    //
    Method (_STA)
    {
      If (LEqual (\_SB.CHGE,1)) {
        Return (0x0F)
      } Else {
        Return (0x00)
      }
    }

    // BCSP (Battery Charge control states package)
    //
    // This object evaluates to a packaged list of information about Battery Charge control states.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   Package of packages
    //
    Name (BCSP, Package ()
    {
      Package ()     // Definition of the Charge Control Object
      {
        0,           // DWordConst: UChrgLevel Upper Bound of charge level that the Charge rate needs to be used
        0,           // DWordConst: ChrgMode Recommended charging mode (Bit 0 -> 0 = fast/no-restriction, 1 = normal) //7th bit is charging mode and remaining 7bits are reserved
        0,           // DWordConst: ChargeLevelCap recommended Charge to limit
      },
      Package ()     // Charge Control Object
      {
        0,           // DWordConst: UChrgLevel Upper Bound of charge level that the Charge rate needs to be used
        0,           // DWordConst: ChrgMode Recommended charging mode (Bit 0 -> 0 = fast/no-restriction, 1 = normal) //7th bit is charging mode and remaining 7bits are reserved
        0,           // DWordConst: ChargeLevelCap recommended Charge to limit
      },
      Package ()     // Charge Control Object
      {
        0,           // DWordConst: UChrgLevel Upper Bound of charge level that the Charge rate needs to be used
        0,           // DWordConst: ChrgMode Recommended charging mode (Bit 0 -> 0 = fast/no-restriction, 1 = normal) //7th bit is charging mode and remaining 7bits are reserved
        0,           // DWordConst: ChargeLevelCap recommended Charge to limit
      },
      Package ()     // Charge Control Object
      {
        0,           // DWordConst: UChrgLevel Upper Bound of charge level that the Charge rate needs to be used
        0,           // DWordConst: ChrgMode Recommended charging mode (Bit 0 -> 0 = fast/no-restriction, 1 = normal) //7th bit is charging mode and remaining 7bits are reserved
        0,           // DWordConst: ChargeLevelCap recommended Charge to limit
      }
    })

    Name (PSSS, 0) // Participant Performance Supported States (PPSS) packge size
    Name (PPPS, 0) // Participant Performance Present State

    // PPSS (Participant Performance Supported States)
    //
    // This object evaluates to a packaged list of information about available power performance states.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   Package of packages
    //
    Name (PPS1, Package ()
    {
      Package ()     // Zeroth Power/Performance State
      {
        100,         // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        0,           // DWordConst: Control
        3500,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        85,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        1,           // DWordConst: Control
        3000,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        71,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        2,           // DWordConst: Control
        2500,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        57,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        3,           // DWordConst: Control
        2000,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        42,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        4,           // DWordConst: Control
        1500,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        28,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        5,           // DWordConst: Control
        1000,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        14,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        6,           // DWordConst: Control
        500,         // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Last Power/Performance State
      {
        0,           // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        7,           // DWordConst: Control
        0,           // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      }
    })

    Name (PPS2, Package ()  // Fast Charging Enabled.
    {
      Package ()     // Zeroth Power/Performance State
      {
        100,         // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        0,           // DWordConst: Control
        4500,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        88,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        1,           // DWordConst: Control
        4000,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        77,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        2,           // DWordConst: Control
        3500,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        66,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        3,           // DWordConst: Control
        3000,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        55,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        4,           // DWordConst: Control
        2500,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        44,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        5,           // DWordConst: Control
        2000,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Power/Performance State
      {
        33,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        6,           // DWordConst: Control
        1500,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Last Power/Performance State
      {
        22,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        7,           // DWordConst: Control
        1000,        // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Last Power/Performance State
      {
        11,          // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        8,           // DWordConst: Control
        500,         // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      },
      Package ()     // Last Power/Performance State
      {
        0,           // DWordConst: Performance
        0,           // DWordConst: Power
        0,           // DWordConst: TransitionLatency
        0,           // Boolean DWordConst : Linear
        9,           // DWordConst: Control
        0,           // DWordConst: RawPerformance
        "MilliAmps", // String: RawUnit
        0            // DWordConst: Reserved1
      }
    })

    Name (PPS3, Package()  // Programmable Power supply (PPS)
    {
      Package ()        // Best Charging
      {
        0,              // DWordConst: Performance
        0,              // DWordConst: Power
        0,              // DWordConst: TransitionLatency
        0,              // Boolean DWordConst : Linear
        0,              // DWordConst: Control
        15000,          // DWordConst: RawPerformance
        "MilliVoltage", // String: RawUnit
        0               // DWordConst: Reserved1
      },
      Package ()        // Better Charging
      {
        0,              // DWordConst: Power
        0,              // DWordConst: Performance
        0,              // DWordConst: TransitionLatency
        0,              // Boolean DWordConst : Linear
        1,              // DWordConst: Control
        12000,          // DWordConst: RawPerformance
        "MilliVoltage", // String: RawUnit
        0               // DWordConst: Reserved1
      },
      Package ()        // Better Performance
      {
        0,              // DWordConst: Performance
        0,              // DWordConst: Power
        0,              // DWordConst: TransitionLatency
        0,              // Boolean DWordConst : Linear
        2,              // DWordConst: Control
        0,              // DWordConst: RawPerformance
        "",             // String: RawUnit
        0               // DWordConst: Reserved1
      },
      Package ()        // Best Performance
      {
        0,              // DWordConst: Performance
        0,              // DWordConst: Power
        0,              // DWordConst: TransitionLatency
        0,              // Boolean DWordConst : Linear
        3,              // DWordConst: Control
        0,              // DWordConst: RawPerformance
        "",             // String: RawUnit
        0               // DWordConst: Reserved1
      }
    })

    // PPSS (Participant Performance Supported States)
    //
    // Decide which Performance Supported states is enabled and return the packages of the specific selected package.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   Packages of the specific package.

    Method (PPSS, 0, Serialized, ,PkgObj)
    {
      ADBG ("DTT calls PPSS")
      If (LAnd (LEqual (ODV0,1), LEqual (ODV1,1))) {  // USB PPS is Enabled and attached
        ADBG ("PPSS: selecting PPS")
        Return (PPS3)
      } ElseIf (LAnd (CondRefOf (\_SB.DPTF.FCHG), LEqual (\_SB.DPTF.FCHG (), 1))) {  // Fast Charging is Enabled
        ADBG ("PPSS: selecting FCHG")
        Return (PPS2)
      } Else {
        ADBG ("PPSS: FCHG and PPS are not available")
        Return (PPS1)
      }
    }

    // PCAL
    //
    // Calculate the size of the relevant PPSS package object and updates the global object that stores it.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   None
    Method (PCAL, 0, Serialized)
    {
      ADBG ("PCAL!")
      If (LAnd (LEqual (ODV0, 1), LEqual (ODV1, 1))) {   // USB PPS is Enabled and attached
        ADBG ("Save PPS package size to PSSS!")
        Store (SizeOf (PPS3), PSSS)
      } ElseIf (LAnd (CondRefOf (\_SB.DPTF.FCHG), LEqual (\_SB.DPTF.FCHG (), 1))) {  // Fast Charging is Enabled
        Store (SizeOf (PPS2), PSSS)
      } Else {
        ADBG ("PCAL: FCHG not available")
        Store (SizeOf (PPS1), PSSS)
      }
    }

    // PPPC (Participant Performance Present Capabilities)
    //
    // PPPC evaluates to an integer that will indicate the maximum allowable P-State for OSPM
    // to use at any given time. The present maximum allowable P-State is stored in PPPS which
    // could be set by SPPC.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   P states
    //
    Method (PPPC, 0)
    {
      ADBG ("PPPC return the present p-state!")
      Return(PPPS) // return present P-state
    }

    // SPPC (Set Participant Performance Capability)
    //
    // SPPC is a control method object that takes one integer parameter that will indicate the maximum
    // allowable P-State for OSPM to use at any given time.
    //
    // Arguments: (1)
    //   Arg0 - P-state value, integer
    // Return Value:
    //   None
    //
    Method (SPPC,1,Serialized)
    {
      ADBG ("SPPC to transfer the selected power package!")
      ADBG (Concatenate ("Package Index: ", ToHexString (Arg0)))
      If (CondRefOf (\_SB.DPTF.SCHG))
      {
        PCAL ()                                                                    // Calculate the size of the relevant PPSS package.

        // BIOS translates P-state to set charging current in mA to EC
        If (LLessEqual (ToInteger (Arg0), Subtract (PSSS, 1))) {                   // bounds check requested P-state
          If (LAnd (LEqual (ODV0, 1), LEqual (ODV1, 1))) {                         // USB PPS is Enabled and attached
            Store (DeRefOf (Index (DeRefOf (Index (PPS3, Arg0)), 4)), Local1)      // Get the requested P-state
            ADBG (Concatenate ("PPS p-state value:", ToHexString (Local1)))
            Store (Local1, PPPS)
            If (CondRefOf (\_SB.DPTF.UPPM)) {
              ADBG (Concatenate ("PPS sets P-state: ", ToHexString (Local1)))
              \_SB.DPTF.UPPM (Local1)
            }
          } ElseIf (LAnd (CondRefOf (\_SB.DPTF.FCHG), LEqual (\_SB.DPTF.FCHG (), 1))) { // Check Fast Charging is Enabled
            Store (DeRefOf (Index (DeRefOf (Index (PPS2,Arg0)),5)), Local1)        // get Charge Rate amp value
            Store (DeRefOf (Index (DeRefOf (Index (PPS2,Arg0)),4)), PPPS)          // get requested P-state
            \_SB.DPTF.SCHG (Local1)                                 // Setting charge rate
          } Else {
            Store (DeRefOf (Index (DeRefOf (Index (PPS1,Arg0)),5)), Local1)        // get Charge Rate amp value
            Store (DeRefOf (Index (DeRefOf (Index (PPS1,Arg0)),4)), PPPS)          // get requested P-state
            \_SB.DPTF.SCHG (Local1)                                 // Setting charge rate
          }
        }
        Return
      }
      ADBG ("SPPC: SCHG not available")
    }

    // PPDL (Participant P-state Depth Limit)
    //
    // Indicates dynamically a lower limit on the number of control states currently supported.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing the P-state Depth Limit _PSS entry number:
    //   0 - P0 is the only P-state available for OSPM use
    //   1 - state 1 is the lowest power P-state available
    //   2 - state 2 is the lowest power P-state available
    //   n - state n is the lowest power P-state available
    //
    Method (PPDL, 0)
    {
      ADBG ("DTT calls PPDL")
      PCAL ()                     // Calculate the size of the relevant PPSS package.
      Return (Subtract (PSSS,1))
    }

    If (LEqual (CBCF,0x01)) {
      // BCCA (Participant Capabilities Discovery)
      //
      // The BCCA gets the capabilities of the participant.
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   An Integer containing participant capabilities
      //    Bit 3:0   - Smart Fuel Guage Revision
      //    Bit 4     - Fast Charging Support Status
      //    Bit 5     - Charging Rate Control Support Status
      //    Bit 7:6   - Reserved
      //    Bit 11:8  - CxBP parameter version
      //    Bit 31:12 - reserved
      //
      Method (BCCA)
      {
        Return (\_SB.DPTF.BCCE)
      }

      // BCCS (Battery Charge Control States)
      //
      // BIOS sets battery charge control state objects.
      //
      // Arguments: (1)
      //   Arg0 - BCCS_Version_Number
      //   Arg1 - Num_Valid_States
      //   Arg2 - BCCS Array
      // Return Value:
      //   An Integer containing Status
      //    0x00  - Success
      //    0x01  - Version num is incorrect
      //    0x02  - Num of valid states is incorrect
      //
      Method (BCCS,3,Serialized)
      {
        If (LNotEqual (Arg0,1)) {
          Return ( 0x01 )
        }
        If (LOr (LLess (Arg1,1),LGreater(Arg1,4))) {
          Return ( 0x02 )
        }

        //  Store the BCCS content
        for (Local0 = 0, Local0 < 3, Local0++) {
          Store (DeRefOf(Index (Arg2,Local0)),Local1)
          Store (Local1, Index (DeRefOf (Index (BCSP, 0)), Local0))
        }
        for (Local0 = 3, Local0 < 6, Local0++) {
          Store (DeRefOf(Index (Arg2,Local0)),Local1)
          Store (Local1, Index (DeRefOf (Index (BCSP, 1)), (Local0-3)))
        }
        for (Local0 = 6, Local0 < 9, Local0++) {
          Store (DeRefOf(Index (Arg2,Local0)),Local1)
          Store (Local1, Index (DeRefOf (Index (BCSP, 2)), (Local0-6)))
        }
        for (Local0 = 9, Local0 < 12, Local0++) {
          Store (DeRefOf(Index (Arg2,Local0)),Local1)
          Store (Local1, Index (DeRefOf (Index (BCSP, 3)), (Local0-9)))
        }

        //Get the remaining Battery capacity
        Store (\_SB.DPTF.B1CS(), Local0)

        for (Local1 = 0, Local1 < 4, Local1++)
        {
          If (Local0 < DeRefOf (Index (DeRefOf (Index (BCSP, Local1)), 0))) { //Comparing Battery 1 remaining percentage with UChrgLevel
            //Store (DeRefOf (Index (DeRefOf (Index (BCSP, Local1)), 2)), 134)
            Store (And(DeRefOf (Index (DeRefOf (Index (BCSP, Local1)), 1)), 0x01), Local2) //Get the value of ChrgMode
            If (LEqual (Local2, 0x01)) {
              Store (Or(DeRefOf (Index (DeRefOf (Index (BCSP, Local1)), 2)), 0x80), Local3) //Combine the value of ChrgMode and ChargeLevelCap
            }
            Else {
              Store (And(DeRefOf (Index (DeRefOf (Index (BCSP, Local1)), 2)), 0x7F), Local3) //Combine the value of ChrgMode and ChargeLevelCap
            }
            \_SB.DPTF.SCLC(Local3)
          }
        }
        Return ( 0x00 )
      }

      // C1BP (Context-based charging BIOS parameters-1)
      //
      // BIOS to relay battery subsystem specific parameters for Context-based Charging, 1 of 3
      // These parameters are provided by OEM's based on battery Specification
      // We are creating PCD's to generate battery related data based on platform
      // Arguments: (0)
      //   Arg0 - None
      // Return Value:
      //   An Integer containing participant capabilities
      //    Bit 15:0   - Constant parameter for charge level related battery degradation calculation.
      //    Bit 23:16  - Upper bound of charge level range in which charging rate benefit calculation applies
      //    Bit 31:24  - Lower bound of charge level range in which charging rate benefit calculation applies
      //
      Method (C1BP,0,Serialized)
      {
        Store (0x00000000, Local0)
        Or (Local0, PLOR, Local0)
        ShiftLeft(Local0,8,Local0)
        Or (Local0, PUPR, Local0)
        ShiftLeft(Local0,16,Local0)
        Or (Local0, PCGL, Local0)
        Return (Local0)
      }

      // C2BP (Context-based charging BIOS parameters-2)
      //
      // BIOS to relay battery subsystem specific parameters for Context-based Charging, 2 of 3
      // These parameters are provided by OEM's based on battery Specification
      // We are creating PCD's to generate battery related data based on platform
      // Arguments: (0)
      //   Arg0 - None
      // Return Value:
      //   An Integer containing participant capabilities
      //    Bit 7:0    - Reserved
      //    Bit 23:8   - Constant parameter for charging rate
      //    Bit 31:24  - Battery State of Health target % at battery end of life
      //
      Method (C2BP,0,Serialized)
      {
        Store (0x00000000, Local1)
        Store (0x00000000, Local0)
        Or (Local0, HEOL, Local0)
        ShiftLeft(Local0,16,Local0)
        Or (Local0, PRTE, Local0)
        ShiftLeft(Local0,8,Local0)
        Or (Local0, Local1, Local0)
        Return (Local0)
      }

      // C3BP (Context-based charging BIOS parameters-3)
      //
      // BIOS to relay battery subsystem specific parameters for Context-based Charging, 3 of 3
      // These parameters are provided by OEM's based on battery Specification
      // We are creating PCD's to generate battery related data based on platform
      // Arguments: (0)
      //   Arg0 - None
      // Return Value:
      //   An Integer containing participant capabilities
      //    Bit 15:0   - Number of seconds needed to charge battery from 0-80%(design capacity) with normal (not fast charging) rate
      //    Bit 31:16  - Max charging current supported in mA
      //
      Method (C3BP,0,Serialized)
      {
        Store (0x00000000, Local0)
        ShiftLeft(Local0,16,Local0)
        Or (Local0, TNML, Local0)
        Return (Local0)
      }
    }  // Endof CBCF

  } // End CHRG Device
}// end Scope (\_SB.IETM)

