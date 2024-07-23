/** @file
  Intel ACPI Reference Code for Intel(R) Platform Data Module.
  Used by independent ACPI modules to get platform related settings.

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

@par Specification Reference:
**/

#include <HID.h>
#include <PlatformBoardId.h>
#include <Include/AcpiDebug.h>

DefinitionBlock (
  "PlatformData.aml",
  "SSDT",
  0x02,
  "INTEL ",
  "PDatTabl",
  0x1000
  )
{

  ACPI_DEBUG_EXTERNAL_REFERENCE
  External(\PLID, IntObj)       // PlatformId

#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  // Externals for Dptf Tables handled by PlatformData
  External(\_SB.IETM.CHRG, DeviceObj)
  External(\_SB.IETM.SEN2, DeviceObj)
  External(\_SB.IETM.SEN3, DeviceObj)
  External(\_SB.IETM.SEN4, DeviceObj)
  External(\_SB.IETM.SEN5, DeviceObj)
  External(\_SB.IETM.TFN1, DeviceObj)
  External(\_SB.IETM.TPWR, DeviceObj)
  External(\_SB.PC00.TCPU, DeviceObj)
#endif

Scope(\_SB)
{
  //
  // DPTF Thermal Zone Device
  //
  //
  Device(PLDT)
  {
    //
    // Intel DPTF Thermal Framework Device
    //
    Name(_HID,EISAID("PNP0A05"))
    Name(_UID,6)
    Name(_STR, Unicode ("Platform Data")) // Platform Data

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
    Method(_STA)
    {
      Return(0x0F)
    }

#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
    // GHID (Get HID)
    //
    // This method returns the HID value of the device
    // Arguments: (1)
    //   Arg0: _UID Unique ID for the Device
    Method(GHID, 1, Serialized,0,,StrObj)
    {
      If (LEqual(Arg0, "SEN1")) {
        Return(SENX_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "SEN2")) {
        Return(SENX_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "SEN3")) {
        Return(SENX_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "SEN4")) {
        Return(SENX_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "SEN5")) {
        Return(SENX_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "TPCH")) {
        Return(PCHP_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "TFN1")) {
        Return(TFN1_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "TFN2")) {
        Return(TFN1_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "TFN3")) {
        Return(TFN1_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "TPWR")) {
        Return(TPWR_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "1")) {
        Return(BAT1_PARTICIPANT_HID)
      }
      If (LEqual(Arg0, "CHRG")) {
        Return(CHRG_PARTICIPANT_HID)
      }
      Return("XXXX9999")
    }

    // GDDV (Get Dptf Data Vault)
    //
    // The data vault can contain APCT, APAT, and PSVT tables.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    A package containing the data vault
    //
    Method(GDDV, 0, Serialized, 0, PkgObj)
    {
      Return(Package()
      {
        Buffer()
        {
          Include("BiosDataVault.asl") // empty data vault for documentation purposes
        }
      })
    }

    // FIDC (Fan ID Convert to Port Method)
    //
    // Convert Fan ID to Fan Port
    //
    //  Arguments: (1)
    //    Arg0 - Fan Index (0-2) for Fan 1-3
    //  Return Value:
    //    EC encoded Fan Port. Bitwise Fan 0->001(decimal 1), Fan 1 -> 010(decimal 2), Fan 2 -> 100 (decimal 4) and so on
    //
    Method(FIDC, 1, Serialized, 0, IntObj)
    {
      Switch(ToInteger(Arg0))
      {
        Case(0){
          Return(1)
        }
        Case(1){
          Return(2)
        }
        Case(2){
          Return(4)
        }
        Default{
          Return(0xF0)
        }
      }
    }

    // FPST (Fan Performance State Table Method)
    //
    // Get Fan Performance State Table by Fan Index
    //
    //  Arguments: (1)
    //    Arg0 - Fan Index (0-2) for Fans 1-3
    //  Return Value:
    //    Fan Performance State Table. OEMs expected to modify the Fan Performance State Table as per Fan Design per Fan ID.
    //    Add more cases when required
    //
    Method(FPST, 1, Serialized, 0, PkgObj)
    {
      Switch(ToInteger(Arg0))
      {
        Case(0){
          Return(FPS0)
        }
        Default{ //    Add more cases when multiple fans present
          Return(FPS0)
        }
      }
    }

Include("Fps.asl") // Include Fan Performance Speed Tables
Include("Art.asl")  // Include Active Relationship Tables
Include("Trt.asl")  // Include Thermal Relationship Tables
Include("Psvt.asl") // Include Fan Performance Speed Tables
#endif

  } // End PLDT Device
} // End Scope

} // End of DefinitionBlock
