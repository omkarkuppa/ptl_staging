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

#include "CpuPowerMgmt.h"
#include <Include/AcpiDebug.h>

#ifndef PARENT_OF_LPCB
#define PARENT_OF_LPCB PC00
#endif

DefinitionBlock (
  "Dptf.aml",
  "SSDT",
  2,
  "DptfTb",
  "DptfTabl",
  0x1000
  )
{
  Store ("[Dptf DptfTabl SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  ADBG ("[Dptf DptfTabl SSDT][AcpiTableEntry]")
  External (\P8XH, MethodObj)
  External (\TSOD, IntObj)
  External (\PNHM, IntObj)
  External (\TCNT, IntObj)
  External (\PWRS, IntObj)
  External (\CRTT, IntObj)
  External (\ACTT, IntObj)
  External (\PSVT, IntObj)
  External (\ATPC, IntObj)
  External (\PTPC, IntObj)
  External (\PF00, IntObj) // PR00 _PDC Flags

  External (\_TZ.ETMD, IntObj)
  External (\_TZ.TZ00, ThermalZoneObj)

  External (\_SB.IETM, DeviceObj)
  External (\_SB.PC00, DeviceObj)
  External (\_SB.PC00.TCPU, DeviceObj)
  External (\_SB.PC00.MC.MHBR, FieldUnitObj)

  External (\_SB.PARENT_OF_LPCB.LPCB.H_EC, DeviceObj)

  //
  // Sensors APIs
  //
  External (\_SB.DPTF.STRP, MethodObj)
  External (\_SB.DPTF.SNRD, MethodObj)

  External (\_SB.DPTF.GFNS, MethodObj)
  External (\_SB.DPTF.FNSL, MethodObj)
  External (\_SB.DPTF.B1RC, MethodObj)
  External (\_SB.DPTF.B1FC, MethodObj)
  External (\_SB.DPTF.BMAX, MethodObj)

  External (\_SB.DPTF.GFHE, MethodObj)
  External (\_SB.DPTF.GFLE, MethodObj)
  External (\_SB.DPTF.SFHE, MethodObj)
  External (\_SB.DPTF.SFLE, MethodObj)
  External (\_SB.DPTF.GSHE, MethodObj)
  External (\_SB.DPTF.GFCE, MethodObj)
  External (\_SB.DPTF.GFME, MethodObj)
  External (\_SB.DPTF.SFME, MethodObj)

  //
  // Power Boss
  //
  External (\_SB.DPTF.PBOK, MethodObj)
  External (\_SB.DPTF.PWRT, MethodObj)
  External (\_SB.DPTF.PBSS, MethodObj)
  External (\_SB.DPTF.ARTG, MethodObj)
  External (\_SB.DPTF.CTYP, MethodObj)
  External (\_SB.DPTF.PROP, MethodObj)

  External (\_SB.DPTF.FCHG, MethodObj)
  External (\_SB.DPTF.SCHG, MethodObj)
  External (\_SB.DPTF.UPPM, MethodObj)

  External (\_SB.PR00._PSS, MethodObj)
  External (\_SB.PR00.TPSS, PkgObj)
  External (\_SB.PR00.LPSS, PkgObj)
  External (\_SB.PR00._PPC, MethodObj)
  External (\_SB.PR00._TSS, MethodObj)
  External (\_SB.CFGD, FieldUnitObj)
  External (\_SB.PR00.TSMF, PkgObj)
  External (\_SB.PR00.TSMC, PkgObj)
  External (\_SB.PR00._PTC, MethodObj)
  External (\_SB.PR00._TSD, MethodObj)
  External (\_SB.PR00._TPC, IntObj)

  External (\_SB.CLVL, FieldUnitObj)
  External (\_SB.CBMI, FieldUnitObj)
  External (\_SB.PL1X, FieldUnitObj)
  External (\_SB.PL2X, FieldUnitObj)
  External (\_SB.PLWX, FieldUnitObj)
  External (\_SB.APSV, FieldUnitObj)
  External (\_SB.AAC0, FieldUnitObj)
  External (\_SB.ACRT, FieldUnitObj)
  External (\_SB.PAGD, DeviceObj)
  External (\_SB.PAGD._PUR, PkgObj)
  External (\_SB.PAGD._STA, MethodObj)

  // Platform-Wide OS Capable externals
  External (\_SB.OSCP, IntObj)

  // Intel Proprietary Wake up Event support externals.
  External (\_SB.HIDD.HPEM, MethodObj)
  External (\_SB.SLPB, DeviceObj)
  External (HIDW, MethodObj)
  External (HIWC, MethodObj)

  // Platform Data External
  External (\_SB.PLDT.ART0, PkgObj)
  External (\_SB.PLDT.ART1, PkgObj)
  External (\_SB.PLDT.PSVT, PkgObj)
  External (\_SB.PLDT.PTRT, MethodObj)
  External (\_SB.PLDT.GDDV, MethodObj)
  External (\_SB.PLDT.GHID, MethodObj)
  External (\_SB.PLDT.FPST, MethodObj)

Scope (\_SB)
{
  Include ("DptfNvs.asl")
  //
  // DPTF Thermal Zone Device
  //
  //
  If (CondRefOf (\_SB.IETM)) {
    Scope (\_SB.IETM)
    {

      //-------------------------------------------
      //  Intel Proprietary Wake up Event solution
      //-------------------------------------------
      Method (_DSM, 0x4, Serialized, 0, {IntObj, BuffObj}, {BuffObj, IntObj, IntObj, PkgObj})
      {
        If (CondRefOf (HIWC)) {
          If (HIWC (Arg0)) {
            If (CondRefOf (HIDW)) {
              Return (HIDW (Arg0, Arg1, Arg2, Arg3))
            }
          }
        }
        Return (Buffer (One) { 0x00 }) // Guid mismatch
      }

      //
      // Save original trip points so _OSC method can enable/disable Legacy thermal policies by manipulating trip points.
      //
      Name (PTRP,0)  // Passive trip point
      Name (PSEM,0)  // Passive semaphore
      Name (ATRP,0)  // Active trip point
      Name (ASEM,0)  // Active semaphore
      Name (YTRP,0)  // Critical trip point
      Name (YSEM,0)  // Critical semaphore

      // _OSC (Operating System Capabilities)
      //
      // This object is evaluated by each DPTF policy implementation to communicate to the platform of the existence and/or control transfer.
      //
      // Arguments: (4)
      //   Arg0 - A Buffer containing a UUID
      //   Arg1 - An Integer containing a Revision ID of the buffer format
      //   Arg2 - An Integer containing a count of entries in Arg3
      //   Arg3 - A Buffer containing a list of DWORD capabilities
      // Return Value:
      //   A Buffer containing a list of capabilities
      //
      Method (_OSC, 4,Serialized,,BuffObj, {BuffObj,IntObj,IntObj,BuffObj})
      {

        // Point to Status DWORD in the Arg3 buffer (STATUS)
        CreateDWordField (Arg3, 0, STS1)

        // Point to Caps DWORDs of the Arg3 buffer (CAPABILITIES)
        CreateDWordField (Arg3, 4, CAP1)

        //
        // _OSC needs to validate the Revision.
        //
        // IF Unsupported Revision
        //  Return Unsupported Revision _OSC Failure
        //
        //    STS0[0] = Reserved
        //    STS0[1] = _OSC Failure
        //    STS0[2] = Unrecognized UUID // IDSP is no longer required for Intel (R) Dynamic Tuning dynamic participants.
                                          // Hence bios do not need to report Unrecognized UUID
        //    STS0[3] = Unsupported Revision
        //    STS0[4] = Capabilities masked
        //

        If (LNot (LEqual (Arg1, 1)))
        {
          //
          // Return Unsupported Revision _OSC Failure
          //
          And (STS1,0xFFFFFF00,STS1)
          Or (STS1,0xA,STS1)
          Return (Arg3)
        }

        If (LNot (LEqual (Arg2, 2)))
        {
          //
          // Return Argument 3 Buffer Count not sufficient
          //
          And (STS1,0xFFFFFF00,STS1)
          Or (STS1,0x2,STS1)
          Return (Arg3)
        }

        //
        // Save Auto Passive Trip Point
        //
        If (CondRefOf (\_SB.APSV)) {
          If (LEqual (PSEM,0)) {
            Store (1,PSEM)
            Store (\_SB.APSV,PTRP)  // use semaphore so variable is only initialized once
          }
        }
        //
        // Save Auto Active Trip Point
        //
        If (CondRefOf (\_SB.AAC0)) {
          If (LEqual (ASEM,0)) {
            Store (1,ASEM)
            Store (\_SB.AAC0,ATRP)  // use semaphore so variable is only initialized once
          }
        }
        //
        // Save Auto Critical Trip Point
        //
        If (CondRefOf (\_SB.ACRT)) {
          If (LEqual (YSEM,0)) {
            Store (1,YSEM)
            Store (\_SB.ACRT,YTRP)  // use semaphore so variable is only initialized once
          }
        }

        // CAP1 contains 4 bits. 1st bit is to indicate that Intel (R) Dynamic Tuning is enabled and wants to enabled some policy.
        // Bios can ignore that 1st bit (indicated by "x" in following table) and
        // only look at bits which are dedicated to each policy as shown in following table.
        // When the bit0 is zero the all CAP bits will be zero.
        // 001x: Enable Active Policy
        // 010x: Enable Passive Policy
        // 100x: Enable Critical Policy
        // 011x: Enable Active/Passive Policy
        // 101x: Enable Active/Critical Policy
        // 110x: Enable Passive/Critical Policy
        // 111x: Enable Active/Passive/Critical Policy
        // xxx0: Disabled all of the Intel (R) Dynamic Tuning Policies.

        //
        // Verify the Intel (R) Dynamic Tuning UUID.
        //
        If (LEqual (Arg0, ToUUID ("B23BA85D-C8B7-3542-88DE-8DE2FFCFD698"))) {  // Intel (R) Dynamic Tuning GUID
          If (Not (And (STS1, 0x01))) // Test Query Flag
          { // Not a query operation, so process the request
            If (And (CAP1, 0x1)) {     // Validate Intel (R) Dynamic Tuning input, When the bit0 is zero then no Intel (R) Dynamic Tuning policy is enabled.
              If (And (CAP1, 0x2)) {  // Enable Active Policy. Nullify the legacy thermal zone.
                Store (110,\_SB.AAC0)
                Store (0, \_TZ.ETMD)  // Legacy Active TM Management relies on this variable.
              } Else {  // re-enable legacy thermal zone with active trip point
                Store (ATRP,\_SB.AAC0)
                Store (1, \_TZ.ETMD)
              }
              If (And (CAP1,0x4)) {   // Enable Passive Policy. Nullify the legacy thermal zone.
                Store (110,\_SB.APSV)
              }Else {  // re-enable legacy thermal zone with passive trip point
                Store (PTRP,\_SB.APSV)
              }
              If (And (CAP1, 0x8)) {  // Enable Critical Policy. Nullify the legacy thermal zone.
                Store (210,\_SB.ACRT)
              }Else {  // re-enable legacy thermal zone with critical trip point
                Store (YTRP,\_SB.ACRT)
              }
              // Send notification to legacy thermal zone for legacy policy to be enabled/disabled
              If (CondRefOf (\_TZ.TZ00)) {
                Notify (\_TZ.TZ00, 0x81)
              }
            } Else {
              Store (YTRP,\_SB.ACRT) // re-enable legacy thermal zone with critical trip point
              Store (PTRP,\_SB.APSV) // re-enable legacy thermal zone with passive trip point
              Store (ATRP,\_SB.AAC0) // re-enable legacy thermal zone with active trip point
              Store (1, \_TZ.ETMD)
            }
            // Send notification to legacy thermal zone for legacy policy to be enabled/disabled
            If (CondRefOf (\_TZ.TZ00)){
              Notify (\_TZ.TZ00, 0x81)
            }
          }
          Return (Arg3)
        }

        Return (Arg3)
      }

      // DCFG (DPTF Configuration)
      //
      // Returns a DWORD data representing the desired behavior of DPTF besides supported DSP and participants.
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   An Integer containing the DPTF Configuration bitmap:
      //    Bit 0 = Generic UI Access Control (0 - enable as default, 1 - disable access)
      //    Bit 1 = Restricted UI Access Control ( 0 - enable as default, 1 - disable access )
      //    Bit 2 = Shell Access Control ( 0 - enable as default, 1 - disable access)
      //    Bit 3 = Environment Monitoring Report Control ( 0 - report is allowed as default, 1 - No environmental monitoring report to Microsoft )
      //    Bit 4 = Thermal Mitigation Report Control ( 0 - No mitigation report to Microsoft as default, 1 - report is allowed)
      //    Bit 5 = Thermal Policy Report Control ( 0 - No policy report to Microsoft as default, 1 - report is allowed)
      //    Bits[31:6] - Reserved (must be cleared).
      //
      Method (DCFG)
      {
        Return (\_SB.DCFE)
      }

      // ODVP (Oem Design Variables Package)
      //
      // Variables for OEM's to customize DPTF behavior based on platform changes.
      //
      Name (ODVX,Package () {0,0,0,0,0,0})

      // ODVP (Oem Design Variables Package)
      //
      // Variables for OEM's to customize DPTF behavior based on platform changes.
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   Package of integers
      //
      Method (ODVP,0,Serialized,,PkgObj)
      {
        Store (\_SB.ODV0,Index (ODVX,0))
        Store (\_SB.ODV1,Index (ODVX,1))
        Store (\_SB.ODV2,Index (ODVX,2))
        Store (\_SB.ODV3,Index (ODVX,3))
        Store (\_SB.ODV4,Index (ODVX,4))
        Store (\_SB.ODV5,Index (ODVX,5))
        Return (ODVX)
      }

      // PTTL (Participant Temperature Tolerance Level)
      // This object evaluates to an integer representing the temperature range within which any change
      // in participant temperature is considered acceptable and no policy actions will be taken by the
      // policy. The temperature tolerance level is expressed in the units of 10s of Kelvin.
      //
      Name (PTTL, 20) // ToleranceLevel


      // _TRT (Thermal Relationship Table)
      //
      // Arguments: (0)
      //   None
      // Return Value:
      //   A variable-length Package containing a list of Thermal Relationship Packages as described below.
      //
      // Return Value Information
      //   Package {
      //   ThermalRelationship[0] // Package
      //    ...
      //   ThermalRelationship[n] // Package
      //   }
      //
      Method (_TRT,,,,PkgObj)
      {
        Return (\_SB.PLDT.PTRT ())
      } // End _TRT

      // Used by passive policy 2.0
      //
      //  Name (PSVT, Package()
      //  {
      //    0x80000000,     // Revision: 1 = true proportional limit, 2 = depth limit
      //    Package() {
      //      "Source",     // Source device: String
      //      "Target",     // Target device: String
      //      0x80000000,   // Priority: for use by passive policy 2.0
      //      0x80000000,   // Sampling Period: in 1/10 of seconds unit
      //      0x80000000,   // Passive Temp: Threshold(1/10 of K) at which Passive Policy 2.0 will take action
      //      0x80000000,   // Domain: type under the source that is controlled
      //      0x80000000,   // Control Knob: type of control knob that need to be exercised
      //      0x80000000,   // Control Value: Depth Limit or Target State which policy will jump to on a thermal condition(based on revision).
      //      0x80000000,   // Step size for this specific control knob
      //      0x80000000,   // Limit Coefficient: 1/10 of units, used with StepSize to control limiting action
      //      0x80000000,   // UnLimit Coefficient: 1/10 of units, used with StepSize to control unlimiting action
      //      0x80000000    // Reserved1
      //    }
      //  })
      Method (PSVT,,,,PkgObj)
      {
        Return (\_SB.PLDT.PSVT)
      }

      // _ART (Active Cooling Relationship Table)
      //
      // Arguments:
      //   None
      // Return Value:
      //   A variable-length Package containing a Revision ID and a list of Active Relationship Packages as described below:
      //
      // Return Value Information
      //   Package {
      //   Revision, // Integer - Current revision is: 0
      //   ActiveRelationship[0] // Package
      //   ...
      //   ActiveRelationship[n] // Package
      //   }
      //
      Method (_ART,,,,PkgObj)
      {
        If (\_SB.IETM.SEN3.CTYP)
        {
          Return (\_SB.PLDT.ART1)
        } Else {
          Return (\_SB.PLDT.ART0)
        }
      } // End _ART

      // GDDV (Get Dptf Data Vault)
      //
      // The data vault can contain APCT, APAT, and PSVT tables.
      //
      //  Arguments: (0)
      //    None
      //  Return Value:
      //    A package containing the data vault
      //
      Method (GDDV,0,Serialized,0,PkgObj)
      {
        Return (\_SB.PLDT.GDDV())
      }

      // IMOK
      //
      // IMOK to test if DPTF is OK and alive.
      //
      // Arguments: (1)
      //   Arg0 - integer
      //  Return Value: (0)
      //    An Integer containing the status of IMOK
      //
      Method (IMOK,1,,,IntObj)
      {
        //It is mainly a stub placeholder
        //OEMs can implement it in a way they choose
        Return (Arg0)
      }

      // DTNS
      //
      // DTNS to notify the OSPM about change in a sensor participant
      // Each bit of the passed in argument represents the sensor participant being notified
      //
      // Arguments: (1)
      //   Arg0 - integer
      // Return Value:
      //   None
      Method (DTNS,1,,,IntObj)
      {
        Store (Arg0, Local0)
#if FixedPcdGetBool(PcdDttSen5Participant) == 1
        If (And (Local0, 0x10))
        { // BIT4: Sensor 5 Threshold Crossed
          ADBG ("Notify Sensor 5")
          Notify (\_SB.IETM.SEN5, 0x90)
        }
#endif
#if FixedPcdGetBool(PcdDttSen4Participant) == 1
        If (And (Local0, 0x8))
        { // BIT3: Sensor 4 Threshold Crossed
          ADBG ("Notify Sensor 4")
          Notify (\_SB.IETM.SEN4, 0x90)
        }
#endif
#if FixedPcdGetBool(PcdDttSen3Participant) == 1
        If (And (Local0, 0x4))
        { // BIT2: Sensor 3 Threshold Crossed
          ADBG ("Notify Sensor 3")
          Notify (\_SB.IETM.SEN3, 0x90)
        }
#endif
#if FixedPcdGetBool(PcdDttSen2Participant) == 1
        If (And (Local0, 0x2))
        { // BIT1: Sensor 2 Threshold Crossed
          ADBG ("Notify Sensor 2")
          Notify (\_SB.IETM.SEN2, 0x90)
        }
#endif
#if FixedPcdGetBool(PcdDttSen1Participant) == 1
        If (And (Local0, 0x1))
        { // BIT0: Sensor 1 Threshold Crossed
          ADBG ("Notify Sensor 1")
          Notify (\_SB.IETM.SEN1, 0x90)
        }
#endif
      }

    } // End IETM Device
  }

  Include ("DptfDevice.asl")
} // End \_SB Scope

//
// DPTF Helper functions.
//
  Include("HelperFunctions.asl")


// Add/Remove DTT participants from a particular platform
// by editing the corresponding Participants.asl
  Include("Participants.asl")


  Include("Dppm.asl")

//
// DPTF Trigger Event function.
//
  Include("DptfTriggerEvent.asl")

ADBG ("[Dptf DptfTabl SSDT][AcpiTableExit]")
Store ("[Dptf DptfTabl SSDT][AcpiTableExit]", Debug)
Store (Timer, Debug)
} // End SSDT
