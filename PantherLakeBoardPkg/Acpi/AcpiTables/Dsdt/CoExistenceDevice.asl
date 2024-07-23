/** @file
  Intel ACPI Sample Code for CoExistence Device ACPI implementation

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

// -------------------------------------------------------
//        CoExistence Device ACPI implementation - Begin
// -------------------------------------------------------
Device (COEX)
{
  //
  // Intel CoExistence Device
  //
  Name (_HID, EISAID ("INT3519"))
  Name (_STR, Unicode ("CoExistence Manager"))
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
    // The CoExistence Manager can support only Intel WWAN and Intel Connectivity combination
    If (LEqual (COEM,1)) {
      Return (0x0F)
    } Else {
      Return (0x00)
    }
  }
} // End of Device (COEX)
// -------------------------------------------------------
//        CoExistence Device ACPI implementation - End
// -------------------------------------------------------