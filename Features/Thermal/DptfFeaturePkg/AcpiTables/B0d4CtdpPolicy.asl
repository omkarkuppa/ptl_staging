/** @file
  Intel ACPI Reference Code for Intel Configurable TDP

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
   Name (CTSP, Package ()
    {
    //
    // ConfigTDP Policy
    //
      ToUUID ("E145970A-E4C1-4d73-900E-C9C5A69DD067")
    })
}

Scope (\_SB.PC00.TCPU)
{
  //
  // Returns the max TDP configurable for the platform.
  //
  Name (MAXT,0)

  // TDPC (Thermal Design Power Capabilities)
  //
  // The TDPC object evaluates to an integer that identifies the maximum usable TDP point
  // from the table returned by the TDPL object at any given time.
  //
  // Arguments: (0)
  //   None
  // Return Value:
  //   This value is the zero based index into the TDPL object's entries.
  //
  Method (TDPC)    // For CRB, please return 0.
  {
    Return (MAXT)  // Return valid TDP point.
  }//end Method (TDPC)

}//end Scope (\_SB.PC00.TCPU)

