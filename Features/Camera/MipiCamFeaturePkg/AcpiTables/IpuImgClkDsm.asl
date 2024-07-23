/** @file
  ACPI DSDT table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

//----------------------------------------------------------------------------------------
//  Intel Proprietary IMGCLK Enabling DSM Method
//
//  Method (_DSM, 0x4, Serialized, 0, {IntObj, BuffObj}, {BuffObj, IntObj, IntObj, PkgObj})
//  Arguments:
// Arg0: UUID Unique function identifier
// Arg1: Integer Revision Level
// Arg2: Integer Function Index
// Arg3: Package Parameters -
//       Index 0 - Clock source select (0: IMGCLKOUT_0, 1: IMGCLKOUT_1, 2: IMGCLKOUT_2, 3: IMGCLKOUT_3, 4: IMGCLKOUT_4)
//       Index 1 - Clock Enable / Disable (0: Disable, 1: Enable)
//       Index 2 - Select 24MHz / 19.2 MHz (0: 24MHz, 1: 19.2MHz)
//-----------------------------------------------------------------------------------------
// DSM UUID for IMGCLK Enabling. Do Not change.

External (\_SB.ICLK.CLKC, MethodObj)
External (\_SB.ICLK.CLKF, MethodObj)

If (LEqual (Arg0, ToUUID ("82C0D13A-78C5-4244-9BB1-EB8B539A8D11")))
{
  // Function 0 : Query Function
  If (LEqual (Arg2, Zero))
  {
    // Revision 0
    If (LEqual (Arg1, Zero)) // The current revision is 0
    {
      Return (Buffer (One) { 0x03 }) // There are 1 function defined other than Query.
    }
    Else
    {
      Return (0) // Revision mismatch
    }
  }
  // Function 1 :
  If (LEqual (Arg2, One))
  {
    If (CondRefOf (\_SB.ICLK.CLKC)) {
      \_SB.ICLK.CLKC (ToInteger (DeRefOf (Index (Arg3, 0))), ToInteger (DeRefOf (Index (Arg3, 1))))
    }
    If (CondRefOf (\_SB.ICLK.CLKF)) {
      \_SB.ICLK.CLKF (ToInteger (DeRefOf (Index (Arg3, 0))), ToInteger (DeRefOf (Index (Arg3, 2))))
    }
  }
  Else
  {
    Return (0) // Function number mismatch but normal return.
  }
}
