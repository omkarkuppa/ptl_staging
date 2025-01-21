/** @file
  ACPI PTID SSDT table

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

//
// Defined as an SSDT to be able to dynamically load based on BIOS
// setup options
//

#include <Include/AcpiDebug.h>

DefinitionBlock (
  "PTID.aml",
  "SSDT",
  0x02,
  "TrmRef",
  "PtidDevc",
  0x1000
  )

{
  Store ("[PTID PtidDevc SSDT][AcpiTableEntry]", Debug)
  Store (Timer, Debug)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  ADBG ("[PTID PtidDevc SSDT][AcpiTableEntry]")

  Scope (\_SB)
  {
    // External References to the actual data locations that stores
    // various temperature and power values (either from EC or by
    // other means)
    //

    // externs for TSDD
    External (\_TZ.TZ00._TMP)              // Thermal Zone 00 Temperature

    External (\PAMT)
    External (\TSDB)

    External (\_SB.PC00.LPCB.TSDD, MethodObj)
    External (\_SB.PC00.LPCB.PSDD, MethodObj)
    External (\_SB.PC00.LPCB.OSDD, MethodObj)
    External (\_SB.PC00.LPCB.RPMD, MethodObj)
    External (\_SB.PC00.LPCB.WPMD, MethodObj)
    External (\_SB.PC00.LPCB.ISPC, MethodObj)
    External (\_SB.PC00.LPCB.ENPC, MethodObj)
    External (\_SB.PC00.LPCB.RPCS, MethodObj)
    External (\_SB.PC00.LPCB.RPEC, MethodObj)

    Device (PTID)
    {
      Name (_HID, EISAID ("INT340E"))
      Name (_CID, EISAID ("PNP0C02"))
      Name (IVER, 0x00030000)      // Version 3
      Name (_STA, 0x0F)

      Method (TSDL)
      {
        If (LEqual (TSDB,1))
        {
          Return (TSD2)
        }
        Return (TSD1) // default all other boards
      }

      // RVP, all other boards
      Name (TSD1, Package ()
      { //Device Class  //Name of Temperature Value
        0x00000000, "CPU Core 0 DTS", //not from ec
        0x00000000, "CPU Core 1 DTS", //not from ec
        0x00000000, "CPU Core 2 DTS", //not from ec
        0x00000000, "CPU Core 3 DTS", //not from ec
        0x00000000, "CPU Core Package DTS", //not from ec
        0x00000003, "DIMM0_hotspot_U4C1",
        0x00000003, "DIMM1_hotspot_Q4D1",
        0x00000003, "IMVP_conn_Q7C1",
        0x00000003, "board_hotspot1_U3G3",
        0x00000003, "board_hotspot2_Q3G1",
        0x00000000, "CPU Package Temperature",
        0x00000005, "PCH DTS Temperature from PCH",
        0x00000000, "CPU PECI reading",
        0x00000005, "SA DTS Temperature from PCH",
        0x00000002, "TZ00 _TMP",
        0x00000003, "SDRAM_hotspot_RT5B1",
        0x00000003, "NGFF_slot_RT6G1",
        0x00000003, "IMVP_conn_RT7D1",
        0x00000003, "board_hotspot3_RT8F1"
      })

      // SDS
      Name (TSD2, Package ()
      { //Device Class  //Name of Temperature Value
        0x00000000, "CPU Core 0 DTS", //not from ec
        0x00000000, "CPU Core 1 DTS", //not from ec
        0x00000000, "CPU Core 2 DTS", //not from ec
        0x00000000, "CPU Core 3 DTS", //not from ec
        0x00000000, "CPU Core Package DTS", //not from ec
        0x00000003, "A-Skin Hotspot U21",
        0x00000003, "A-Skin Hotspot U21 IR",
        0x00000003, "B-Skin Hotspot U1",
        0x00000003, "B-Skin Hotspot U1 IR",
        0x00000000, "CPU Package Temperature",
        0x00000005, "PCH DTS Temperature from PCH",
        0x00000000, "CPU PECI reading",
        0x00000005, "SA DTS Temperature from PCH",
        0x00000002, "TZ00 _TMP",
        0x00000003, "SoC VR Thermsitor RT11",
        0x00000003, "Memory Thermistor RT9",
        0x00000003, "GNSS Board Ambient Thermistor R4",
        0x00000003, "System VR Thermistor RT10",
        0x00000003, "A-Skin MipiCam Thermistor RT8"
      })

      Name (PSDL, Package ()
      { // Device Class   //Name of Power Value
        0x0000000B,       "Platform Power (mW)",
        0x0000000B,       "Brick Power cW (100ths)",
        0x0000000B,       "Battery Discharge Power cW (100ths)",
        0x0000000B,       "Platform Average Power (mW)",
        0x0000000B,       "Brick Average Power cW (0.01)",
        0x0000000B,       "Battery Discharge Average Power cW (0.01)",
        0x0000000C,       "Battery 1 Design Capacity (mWh)",
        0x0000000C,       "Battery 1 Remaining Capacity (mWh)",
        0x0000000C,       "Battery 1 Full Charge Capacity (mWh)",
        0x0000000C,       "Battery 1 Full Resolution Voltage (mV)",
        0x0000000C,       "Battery 1 Full Resolution Discharge Current (mA)",
        0x0000000C,       "Battery 1 Full Resolution Charge Current (mA)",
        0x0000000C,       "Battery 2 Remaining Capacity (mWh)",
        0x0000000C,       "Battery 2 Full Charge Capacity (mWh)",
        0x0000000C,       "Battery 2 Full Resolution Voltage (mV)",
        0x0000000C,       "Battery 2 Full Resolution Discharge Current (mA)",
        0x0000000C,       "Battery 2 Full Resolution Charge Current (mA)",
        0x0000000C,       "Battery Pack 1 maximum power (mW)",
        0x0000000C,       "Battery Pack 2 maximum power (mW)"
      })

      Method (OSDL)
      {
        If (LEqual (TSDB,1))
        {
          Return (OSD2)
        }
          Return (OSD1) // default all other boards
      }

      // SKL RVP, all other boards
      Name (OSD1, Package ()
      { // Device Class   // Descriptive Name   //Unit
        0x00000004, "CPU Fan Duty Cycle", "RAW",
        0x00000004, "CPU Fan #1 Speed", "RPM",
        0x00000003, "Skin Temp 0", "RAW",
        0x00000003, "Thermistor 1 ", "RAW",
        0x00000003, "Thermistor 2 ", "RAW",
        0x00000003, "Thermistor 3 ", "RAW",
        0x00000003, "Thermistor 4 ", "RAW"
      })

      // SKL SDS
      Name (OSD2, Package ()
      { // Device Class   // Descriptive Name   //Unit
        0x00000004, "CPU Fan Duty Cycle", "RAW",
        0x00000004, "CPU Fan #1 Speed", "RPM",
        0x00000003, "Skin Temp 0", "RAW",
        0x00000003, "Thermistor 1 ", "RAW",
        0x00000003, "Thermistor 2 ", "RAW",
        0x00000003, "Thermistor 3 ", "RAW",
        0x00000003, "Thermistor 4 ", "RAW",
        0x00000003, "Thermistor 5 ", "RAW"
      })

      //
      // Temperature 1, size == 19. Only used in TSDD.
      //
      Name (TMP1, Package () {
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000
      })
      //
      // Temperature 2, size == 20. Only used in TSDD.
      //
      Name (TMP2, Package () {
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000
      })
      Method (TSDD, 0, Serialized)
      {
        If (LEqual (TSDB, 1)) {
          Return (\_SB.PC00.LPCB.TSDD (1, TMP1))
        }
        Else {
          Return (\_SB.PC00.LPCB.TSDD (0, TMP2))
        }
      } // end of Method (TSDD)

      Name (PWRV, Package () {
        0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000
      })
      Method (PSDD, 0, Serialized)
      {
        Return (\_SB.PC00.LPCB.PSDD (PWRV))
      }

      Name (OSV1, Package () {
        0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000
      })
      Name (OSV2, Package () {
        0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000
      })
      Method (OSDD, 0, Serialized)
      {
        If (LEqual (TSDB, 1)) {
          Return (\_SB.PC00.LPCB.OSDD (1, OSV1))
        }
        Else {
          Return (\_SB.PC00.LPCB.OSDD (0, OSV2))
        }
      }

      Method (SDSP)
      {
        Return (10)      // Sampling period .
      }

      // PECI ACPI Access method
      Name (PADA, Package ()
      {
        0x01,              // Access Mode -ACPI
        0xFFFFFFFF,        // Command Status port
        0xFFFFFFFF,        // Data port
        26,                // MailBox length
        0xFFFFFFFF,        // WritePECIMailBoxDataCommand
        0xFFFFFFFF,        // ReadPECIMailBoxDataCommand
        0xFFFFFFFF,        // IssuePECIcommand
        0xFFFFFFFF,        // EndPECICommand
        0xFFFFFFFF,        // ReadStatusCommand
        0xFFFFFFFF         // ReadErrorCountCommand
      })

      // PECI Direct I/O Access method
      Name (PADD, Package ()
      {
        0x02,        // Access Mode
        0x6A4,       // Command Status port
        0x6A0,       // Data port
        26,          // MailBox length
        0x62,        // WritePECIMailBoxDataCommand
        0x63,        // ReadPECIMailBoxDataCommand
        0x60,        // IssuePECIcommand
        0x61,        // EndPECICommand
        0x65,        // ReadStatusCommand
        0x64         // ReadErrorCountCommand
      })

      Method (PADT)
      {
        If (PAMT) {
          Return (PADA)
        }
        Return (PADD)
      }

      Method (RPMD, 0, Serialized)
      {
        Return (\_SB.PC00.LPCB.RPMD ())
      }

      Method (WPMD,1)
      {
        If (LNotEqual (Sizeof (Arg0), 26)) {
          Return (0xFFFFFFFF)
        }
        \_SB.PC00.LPCB.WPMD (Arg0)
        Return (0)
      }

      Method (ISPC)
      {
        \_SB.PC00.LPCB.ISPC ()
        Return (0)
      }

      Method (ENPC)
      {
        \_SB.PC00.LPCB.ENPC ()
        Return (0)
      }

      Method (RPCS)
      {
        Return (\_SB.PC00.LPCB.RPCS ())
      }

      Method (RPEC)
      {
        Return (\_SB.PC00.LPCB.RPEC ())
      }

    } // end Device (PTID)
  } // end Scope (\_SB)
  ADBG ("[PTID PtidDevc SSDT][AcpiTableExit]")
  Store ("[PTID PtidDevc SSDT][AcpiTableExit]", Debug)
  Store (Timer, Debug)
} // end DefinitionBlock
