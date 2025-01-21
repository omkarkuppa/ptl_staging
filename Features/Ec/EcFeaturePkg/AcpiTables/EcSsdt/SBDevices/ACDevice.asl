/** @file
  ACPI EC SSDT table for AC device.

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

//
// Define an AC Device for ULT, ULX and Halo Platforms.
//
Device (ADP1)
{
  External (\PWRS, FieldUnitObj)

  Name (_HID,"ACPI0003")

  Method (_STA)
  {
    If (LEqual (ADAS, 0)){
      Return (0x00)
    }
    Return (0x0F)
  }

  // Return the value that determines if running
  // from AC or not.

  Method (_PSR, 0)
  {
    Return (\PWRS)
  }

  // Return that everything runs off of AC.

  Method (_PCL, 0)
  {
    Return (
      Package () { _SB }
    )
  }
}
