/** @file
  ACPI EC SSDT table for Ambient Light Sensor.

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

Device (ALSD)
{
  External (\ALSE, IntObj)
  External (\LHIH, IntObj)
  External (\LLOW, IntObj)

  Name (_HID, "ACPI0008")

  Method (_STA, 0)
  {
    If (LEqual (\ALSE, 2))
    {
      Return (0x000B)    // ALS Enabled.  Don't show it in UI.
    }

    Return (0x0000)      // ALS Disabled.  Hide it.
  }

  Method (_ALI)
  {
    Return (Or (ShiftLeft (\LHIH, 8), \LLOW))
  }

  Name (_ALR, Package ()
  {
    Package () {70,  0},
    Package () {73,  10},
    Package () {85,  80},
    Package () {100, 300},
    Package () {150, 1000}
  })

}
