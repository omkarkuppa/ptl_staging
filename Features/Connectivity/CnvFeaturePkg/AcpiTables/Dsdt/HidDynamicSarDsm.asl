/** @file
  ACPI DSDT table

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

Method (ISAR, 4)
{
  ADBG ("ISAR")
  Name (BUF1, Buffer (2) {0, 0})
  // --------------------------------------------
  //  Intel Proprietary Dynamic SAR solution
  //
  //  Method (_DSM, 0x4, Serialized, 0, {IntObj, BuffObj}, {BuffObj, IntObj, IntObj, PkgObj})
  //
  // --------------------------------------------
  If (LEqual (Arg0, ToUUID ("1730E71D-E5DD-4A34-BE57-4D76B6A2FE37"))) {
    // Function 3 : SAR Sensor data upate
    If (LEqual (Arg2, 3)) {
      If (CondRefOf (\_SB.USSV)) {
        // BYTE[0] split into two parts:
        // bit[7:4] is used to indicate Protocol ID and
        // bit[3:0] is used to indicate number of SAR channels or Wi-Fi Antennas
        // Store (DeRefOf (Index (Arg3, 0)), Local0)

        // BYTE[1] : Power policy index of Antenna A
        Store (DeRefOf (Index (Arg3, 1)), Local1)
        Add (Local1, 0x1, Local1)

        // BYTE[2] : Power policy index of Antenna B
        Store (DeRefOf (Index (Arg3, 2)), Local2)
        Add (Local2, 0x1, Local2)

        ShiftLeft (Local2, 16, Local2)
        Or (Local2, Local1, Local2)

        \_SB.USSV (Local2)
        If (CondRefOf (\_SB.USSV)) {
          Notify (\_SB.WFDE, 0xD0)
        }
      }
      Return (0)
    } Else {
      Return (0)   // Function number mismatch but normal return.
    }
  } Else {
    Return (Buffer (One) { 0x00 })   // Guid mismatch
  }
}   // End Method (ISAR)