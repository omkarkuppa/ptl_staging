 /**@file
  This file defines the device objects for the IPU device

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
// Device IPUA is the IPU AVStream virtual device and it appears under GFX0
//
Scope (\_SB.PC00.GFX0)
{
  Device (IPUA) // IPU AVStream virtual device name
  {
    /*
      The identifier for this device (Same as in
      _DOD above). This is required so GFX driver can
      associate a matching device ID for the AVStream
      driver and provide it to PnP (this device ID
      should appear in the INF file of the AVStream
      driver).
    */
    Name (_ADR, 0x00003480)
    /*
      The following is a technique that may be used (per OEM needs) to prevent
      the load of the camera device in one of the following cases:
      - Camera device is fused out
      - If the platform setup requires that in a secured boot the camera device
      should not be enabled
    */
    Method (_STA, 0, NotSerialized) {
      If (LEqual (IPTP,1)) { // IGFX need report IPU AVStream virtual device as GFX0 child
        Return (0xF)
      } Else { // IGFX should NOT report IPU AVStream virtual device as GFX0 child
        Return (0x0)
      }
    }
  }
}

Scope (\_SB.PC00.IPU0)
{
//----------------------------------------------------------------------------------------
//  Intel Proprietary Passing LTR information from BIOS to IPU Driver. DSM Method
//
//  Method (_DSM, 0x4, Serialized, 0, {IntObj, BuffObj}, {BuffObj, IntObj, IntObj, PkgObj})
//  Arguments:
//  Arg0: GUID: "9A9E6AB4-E3FC-475D-AD1C-C4789E4CFE90"
//  Arg1: Integer Revision Level (Current revision is 0)
//  Arg2: Integer Function Index
//
//-----------------------------------------------------------------------------------------
Method (_DSM, 4, NotSerialized) { // _DSM: Device-Specific Method
    If (LEqual (Arg0, ToUUID ("9A9E6AB4-E3FC-475D-AD1C-C4789E4CFE90")))
    {
      // Function 0 : Query Function
      If (LEqual (Arg2, 0))
      {
        // Revision 0
        If (LEqual (Arg1, 0)) // The current revision is 0
        {
          Return (Buffer () { 0x0 }) // There are 2 function defined other than Query.
        } Else {
          Return (0) // Revision mismatch
        }
      }

      Return (0) // Function number mismatch but normal return.
    } Else {
      Return (0)
    }
  }
}