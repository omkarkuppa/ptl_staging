/** @file
  Intel ACPI Sample Code for discrete connectivity modules

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

Scope (\_SB)
{
  // PDAT is a 32 bit value
  // PDAT_31:16 is a chain B tableset value
  // PDAT_15:0  is a chain A tableset value
  Name (PDAT, Zero) // Shared storage
  Name (DSSI, 0)    // Dynamic SAR Set init

  Device (WFDE)
  {
    Name (_HID, EISAID ("PNP0C14"))
    Name (_UID, "DSarDev")

    Name (_WDG, Buffer ()
    {
      0xEF, 0x9D, 0xC4, 0x2B, 0x15, 0x7B, 0x05, 0x4F, 0x8B, 0xB7, 0xEE, 0x37, 0xB9, 0x54, 0x7C, 0x0B, // GUID 2BC49DEF-7B15-4F05-8BB7-EE37B9547C0B
      0x44, 0x45,                                                                                     // ObjectId (DE)
      0x01,                                                                                           // InstanceCount (1)
      0x02,                                                                                           // Flags (WMIACPI_REGFLAG_METHOD)
      0x3E, 0xA3, 0xFE, 0xA6, 0xBF, 0xDA, 0xF5, 0x46, 0xBF, 0xC8, 0x46, 0x0D, 0x96, 0x1B, 0xEC, 0x9F, // GUID A6FEA33E-DABF-46F5-BFC8-460D961BEC9F
      0xD0, 0x00,                                                                                     // NotificationValue (0xD0)
      0x01,                                                                                           // InstanceCount (1)
      0x08,                                                                                           // Flags (WMIACPI_REGFLAG_EVENT)
      0x21, 0x12, 0x90, 0x05, 0x66, 0xd5, 0xd1, 0x11, 0xb2, 0xf0, 0x00, 0xa0, 0xc9, 0x06, 0x29, 0x10, // GUID
      0x43, 0x43,                                                                                     // ObjectId (CC) - Reference to MOF buffer
      0x01,                                                                                           // InstanceCount (1)
      0x00                                                                                            // Flags (none)
    })

    Method (WMDE, 3, NotSerialized)
    {
      If (LEqual (Arg1, One)) { // Get ASL Code version
        Return (One)
      }
      If (LEqual (Arg1, 0x02)) { // Is feature enabled
        // Set the default (boot up) values {one time init}
        If (LEqual (DSSI, 0)) {
          Store (0x10001, PDAT) // Setting Default
          Store (1, DSSI)
        }
        Return (One) // Return feature enable status
      }
      If (LEqual (Arg1, 0x03)) { // Data
        Return (PDAT)
      }
      Return (Zero)
    }

    Method (_WED, 1, NotSerialized) // WMI notification
    {
      If (LEqual (Arg0, 0xD0)) {
        Return (PDAT)
      }
      Return (Zero)
    }

    Name (WQCC, Buffer () // Compiled WMI MOF
    {
      Include ("MofSampleDev.h")
    })
  } // End of Device (WFDE)

  // -----------------------------------------
  // Test Device - Begin (Used for test purpose alone)
  // -----------------------------------------
  // This device will fire a test event to WFDE device
  Device (WFTE)
  {
    Name (_HID, EISAID ("PNP0C14"))
    Name (_UID, "TestDev")

    Name (_WDG, Buffer ()
    {
      0x7F, 0xAB, 0x13, 0x1F, 0x20, 0x62, 0x10, 0x42, 0x8F, 0x8E, 0x8B, 0xB5, 0xE7, 0x1E, 0xE9, 0x69, // GUID 1F13AB7F-6220-4210-8F8E-8BB5E71EE969
      0x44, 0x45,                                                                                     // ObjectId (DE)
      0x01,                                                                                           // InstanceCount (1)
      0x02,                                                                                           // Flags (WMIACPI_REGFLAG_METHOD)
      0x21, 0x12, 0x90, 0x05, 0x66, 0xd5, 0xd1, 0x11, 0xb2, 0xf0, 0x00, 0xa0, 0xc9, 0x06, 0x29, 0x10, // GUID
      0x43, 0x43,                                                                                     // ObjectId (CC) - Reference to MOF buffer
      0x01,                                                                                           // InstanceCount (1)
      0x00                                                                                            // Flags (none)
    })

    Method (WMDE, 3, NotSerialized)
    {
      If (LEqual (Arg1, 0x01)) { // Debug, fire event, real code should somehow update PDAT
        Store (Arg2, PDAT)
        Notify (WFDE, 0xD0)
        Return (PDAT)
      }
      Return (Zero)
    }

    Name (WQCC, Buffer () // Compiled WMI MOF
    {
      Include ("MofTestDev.h")
    })
  }

  // -----------------------------------------
  // Test Device - End (Used for test purpose alone)
  // -----------------------------------------
  // ----------------------------------------------------------------------------
  // Test method to update Current SAR Set - Begin (Used for test purpose alone)
  // This Current SAR Set Update needs to done in EC Query Methods if EC Implements
  // SCI event to Set the to Set the Current SAR SET Values by reading Proximity Sensor
  // ----------------------------------------------------------------------------
  //
  // Name:USSV (Update SAR SET Values)
  // Description: Updates the Current SAR SET values for Antenna A and Antenna B
  // Input: Arg0 -> Antenna B Current Set | Antenna A Current Set
  // Return: Nothing
  //
  Method (USSV, 1, Serialized)
  {
    Store (Arg0, PDAT)
  }

  // ----------------------------------------------------------------------------
  // Test method to get Current SAR Set - Begin (Used for test purpose alone)
  // ----------------------------------------------------------------------------
  //
  // Name:GSSV (Get SAR SET Values)
  // Description: Get the Current SAR SET values for Antenna A and Antenna B
  // Input: Nothing
  // Return: PDAT -> Antenna B Current Set | Antenna A Current Set
  //
  Method (GSSV, 0, Serialized)
  {
    Return (PDAT)
  }

  // ----------------------------------------------------------------------------
  // Test method to update Current SAR Set - End (Used for test purpose alone)
  // ----------------------------------------------------------------------------
} // End of Scope (\_SB)