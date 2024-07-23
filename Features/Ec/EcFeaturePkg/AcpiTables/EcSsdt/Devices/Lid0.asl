/** @file
  ACPI EC SSDT table for LID switch.

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

#define FlavorMobile                        0x1
#define FlavorMobileWorkstation             0x6

//
// Define a Lid Switch.
//
Device (LID0)
{
  External (\PFLV, IntObj)
  External (\GPRW, MethodObj)
  External (\GGPE, MethodObj)

  Name (_HID, EISAID ("PNP0C0D"))
  Method (_STA)
  {
    If (LOr(LEqual (\PFLV, FlavorMobile),LEqual(\PFLV,FlavorMobileWorkstation))){
      Return (0x0F)
    }
    Return (0x00)
  }
  Method (_LID, 0)
  {
    // 0 = Closed, 1 = Open.
    If ((ECRD (RefOf (LSTE))) == LIDS) {
      Return (LIDS)
    } Else {
      ADBG ("Returning simulated Value")
      Store(LIDS, Local0)
      Store(ECRD (RefOf (LSTE)), LIDS)
      Return (Local0)
    }
  }

  //
  // Enable SCI wake for LID SWITCH
  //
  If (LNotEqual (LSWP, 0)) {
    Method (_PRW, 0) {
      Return (GPRW (GGPE (LSWP), 3))
    }
  }
}
