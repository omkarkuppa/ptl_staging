/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning Technology

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

Scope (\_SB.IETM)
{

  Device (TFN2)
  {
    Name (_UID, "TFN2")
    Method (_HID)
    {
      Return (\_SB.PLDT.GHID (_UID))  // Intel(R) Dynamic Tuning Technology platform power device
    }
    Name  (_STR, Unicode ("DDR Fan"))
    Name (PTYP, 0x04)

    Name (FON, 1) // Indicates if Fan is ON Currently
    // PFLG
    //   0 - Default, participant is on the main board
    //   1 - Participant device is on the docking station
    //   2 - Participant device is on the detachable base
    //   3 - Participant device is an external device (such as a USB device, Intel(R) Dynamic Tuning Technology does not support this type of device today)
    Name (PFLG, 0)
    Name (FSLV, 0)
    Name (FNID, 1)

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
      If (LEqual (FND2,1)){
        Return (0x0F)
      } Else {
        Return (0x00)
      }
    }

    // _FIF (Fan Information)
    //
    // The optional _FIF object provides OSPM with fan device capability information.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //  A Package containing the fan device parameters.
    //
    Method (_FIF)
    {
        Return (Package (){
        0,                            // Revision:Integer
        1,                            // FineGrainControl:Integer Boolean
        2,                            // StepSize:Integer DWORD
        0                             // LowSpeedNotificationSupport:Integer Boolean
        })
    }

    // _FPS (Fan Performance States)
    //
    // Evaluates to a variable-length package containing a list of packages that describe the fan device's performance states.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //  A variable-length Package containing a Revision ID and a list of Packages that describe the fan device's performance states.
    //
    Method (_FPS,,,,PkgObj)
    {
      Return (\_SB.PLDT.FPST(FNID))
    }

    // _FSL (Fan Set Level)
    //
    // The optional _FSL object is a control method that OSPM evaluates to set a fan device's speed (performance state) to a specific level.
    //
    // Arguments: (1)
    //  Arg0 - Level (Integer): conveys to the platform the fan speed level to be set.
    // Return Value:
    //  None
    //
    // Argument Information
    //  Arg0: Level. If the fan supports fine-grained control, Level is a percentage of maximum level (0-100)
    //  that the platform is to engage the fan. If the fan does not support fine-grained control,
    //  Level is a Control field value from a package in the _FPS object's package list.
    //  A Level value of zero causes the platform to turn off the fan.
    //
    Method (_FSL,1,Serialized)
    {
      If (CondRefOf (\_SB.DPTF.FNSL))
      {
        If (LNot (LEqual (Arg0, FSLV))) // Check if new level is different than previous level
        {
          \_SB.DPTF.FNSL (FNID, Arg0, FSLV)
          Store (Arg0, FSLV)
        }
        else {
        ADBG ("_FSL: New Fan Level is the same as previous level. Ignoring. No action taken")
        }
      }
      else {
      ADBG ("_FSL: FNSL not available")
      }
    }

    Name (TFST, Package()
    {
        0,          // Revision:Integer
        0xFFFFFFFF, // Control:Integer DWORD
        0xFFFFFFFF  // Speed:Integer DWORD
    })

    //_FST (Fan Status)
    //
    // The optional _FST object provides status information for the fan device.
    //
    // Arguments: (0)
    //  None
    // Return Value:
    //  A Package containing fan device status information
    //
    Method (_FST,0,Serialized,,PkgObj)
    {
      If (CondRefOf (\_SB.DPTF.GFNS))
      {
        Store (\_SB.DPTF.GFNS (FNID), Local0)
        If (LNotEqual (Local0, 0xFFFFFFFF))
        {
          Store (FSLV, Index (TFST, 1))
          Store (Local0, Index (TFST, 2))
        } Else {
          ADBG ("_FST: EC not available")
        }
      }
      Else {
        ADBG ("_FST: GFNS not available")
      }
      Return (TFST)
    }
  } // End TFN2 Device

}// end Scope (\_SB.IETM)
