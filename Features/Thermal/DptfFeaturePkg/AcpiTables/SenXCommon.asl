/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning Technology.
  Common file for all Sensor Participants

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
    Name (PTYP, 0x03) // Participant type: Sensor
    Method (_HID)
    {
      Return (\_SB.PLDT.GHID(_UID))  // Intel(R) Dynamic Tuning Technology platform power device
    }

    // _TMP (Temperature)
    //
    // This control method returns the thermal zone's current operating temperature.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing the current temperature of the thermal zone (in tenths of degrees Kelvin)
    //
    Method (_TMP,0,Serialized)
    {
      If (CondRefOf (\_SB.DPTF.SNRD))
      {
        Store (\_SB.IETM.C10K (\_SB.DPTF.SNRD (SNID)), Local0)
        ADBG (Concatenate ("SEN ID: ", ToDecimalString (Add (SNID,1))))
        ADBG (Concatenate("SEN EC Value in 10th: ", ToHexString (Local0)))
        Return (Local0)
      }
      ADBG ("_TMP: SNRD not available")
      Return(0)
    }

    Name (LSTM,0)  // Last temperature reported

    // _DTI (Device Temperature Indication)
    //
    // Conveys the temperature of a device's internal temperature sensor to the platform when a temperature trip point
    // is crossed or when a meaningful temperature change occurs.
    //
    // Arguments: (1)
    //   Arg0 - An Integer containing the current value of the temperature sensor (in tenths Kelvin)
    // Return Value:
    //   None
    //
    Method (_DTI, 1)
    {
      Store (Arg0,LSTM)
      Notify (^, 0x91) // notify the participant of a trip point change event
      ADBG (Concatenate("DTI SEN ID: ", ToDecimalString(Add(SNID,1))))
    }

    // _NTT (Notification Temperature Threshold)
    //
    // Returns the temperature change threshold for devices containing native temperature sensors to cause
    // evaluation of the _DTI object
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing the temperature threshold in tenths of degrees Kelvin.
    //
    Method (_NTT, 0)
    {
      Return (2782)  // 5 degree Celcius, this could be a platform policy with setup item
    }


    // _TSP (Thermal Sampling Period)
    //
    // Sets the polling interval in 10ths of seconds. A value of 0 tells the driver to use interrupts.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing the polling rate in tenths of seconds.
    //   A value of 0 will specify using interrupts through the ACPI notifications.
    //
    //   The granularity of the sampling period is 0.1 seconds. For example, if the sampling period is 30.0
    //   seconds, then _TSP needs to report 300; if the sampling period is 0.5 seconds, then it will report 5.
    //
    Method (_TSP,0,Serialized)
    {
      Return (SNSP)
    }

    // _ACx (Active Cooling)
    //
    // This ACPI method is optional.
    // OEM/ODM can directly set the value on Intel(R) Dynamic Tuning Technology UI for the participant and then export the value to BIOS GDDV for storage.
    // The value programmed here is the default value used on Intel RVP for the validation purpose.
    // OEM/ODM can set a different default value for a specific platform if necessary.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    An Integer containing the active cooling temperature threshold in tenths of degrees Kelvin
    //
    Method (_AC0,0,Serialized)
    {
      Store (\_SB.IETM.CTOK (SNAC),Local1) // Active Cooling Policy
      If (LGreaterEqual (LSTM,Local1))
      {
        Return (Subtract (Local1, GTSH)) // subtract 2 degrees which is the Hysteresis
      }
      Else
      {
        Return (Local1)
      }
    }

    // _ACx (Active Cooling)
    //
    // This ACPI method is optional.
    // OEM/ODM can directly set the value on Intel(R) Dynamic Tuning Technology UI for the participant and then export the value to BIOS GDDV for storage.
    // The value programmed here is the default value used on Intel RVP for the validation purpose.
    // OEM/ODM can set a different default value for a specific platform if necessary.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    An Integer containing the active cooling temperature threshold in tenths of degrees Kelvin
    //
    Method (_AC1,0,Serialized)
    {
      Return (\_SB.IETM.CTOK (SNA1))
    }

    // _ACx (Active Cooling)
    //
    // This ACPI method is optional.
    // OEM/ODM can directly set the value on Intel(R) Dynamic Tuning Technology UI for the participant and then export the value to BIOS GDDV for storage.
    // The value programmed here is the default value used on Intel RVP for the validation purpose.
    // OEM/ODM can set a different default value for a specific platform if necessary.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    An Integer containing the active cooling temperature threshold in tenths of degrees Kelvin
    //
    Method (_AC2,0,Serialized)
    {
      Return (\_SB.IETM.CTOK (SNA2))
    }

    // _PSV (Passive)
    //
    // This ACPI method is optional.
    // OEM/ODM can directly set the value on Intel(R) Dynamic Tuning Technology UI for the participant and then export the value to BIOS GDDV for storage.
    // The value programmed here is the default value used on Intel RVP for the validation purpose.
    // OEM/ODM can set a different default value for a specific platform if necessary.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    An Integer containing the passive cooling temperature threshold in tenths of degrees Kelvin
    //
    Method (_PSV,0,Serialized)
    {
      Return (\_SB.IETM.CTOK (SNPV)) // Passive Cooling Policy
    }

    // _CRT (Critical Temperature)
    //
    // This ACPI method is optional.
    // OEM/ODM can directly set the value on Intel(R) Dynamic Tuning Technology UI for the participant and then export the value to BIOS GDDV for storage.
    // The value programmed here is the default value used on Intel RVP for the validation purpose.
    // OEM/ODM can set a different default value for a specific platform if necessary.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    An Integer containing the critical temperature threshold in tenths of degrees Kelvin
    //
    Method (_CRT,0,Serialized)
    {
      Return (\_SB.IETM.CTOK (SNCC))
    }

    // _CR3 (Critical Temperature for S3/CS)
    //
    // This ACPI method is optional.
    // OEM/ODM can directly set the value on Intel(R) Dynamic Tuning Technology UI for the participant and then export the value to BIOS GDDV for storage.
    // The value programmed here is the default value used on Intel RVP for the validation purpose.
    // OEM/ODM can set a different default value for a specific platform if necessary.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    An Integer containing the critical temperature threshold in tenths of degrees Kelvin
    //
    Method (_CR3,0,Serialized)
    {
      Return (\_SB.IETM.CTOK (SNC3))
    }

    // _HOT (Hot Temperature)
    //
    // This ACPI method is optional.
    // OEM/ODM can directly set the value on Intel(R) Dynamic Tuning Technology UI for the participant and then export the value to BIOS GDDV for storage.
    // The value programmed here is the default value used on Intel RVP for the validation purpose.
    // OEM/ODM can set a different default value for a specific platform if necessary.
    //
    //  Arguments: (0)
    //    None
    //  Return Value:
    //    The return value is an integer that represents the critical sleep threshold tenths of degrees Kelvin.
    //
    Method (_HOT,0,Serialized)
    {
      Return (\_SB.IETM.CTOK (SNHP))
    }
