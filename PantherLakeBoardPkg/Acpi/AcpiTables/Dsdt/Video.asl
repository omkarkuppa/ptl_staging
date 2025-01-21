/** @file
  ACPI DSDT table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

External (DIDX)
External (DIDY)
External (\_SB.PC00.GFX0.DD2F, DeviceObj)

// Brightness Notification:
//    Generate a brightness related notification
//      to the LFP if its populated.
//
//  Arguments:
//    Arg0: Notification value.
//
//  Return Value:
//    None
Method (BRTN,1,Serialized)
{
    If (LEqual (And (DIDX,0x0F0F),0x400))
    {
        Notify (\_SB.PC00.GFX0.DD1F,Arg0)
    }
    If (LEqual (And (DIDY,0x0F0F),0x401))
    {
        Notify (\_SB.PC00.GFX0.DD2F,Arg0)
    }
}
