/** @file
  This file contains the IPU SSDT Table ASL code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

DefinitionBlock (
  "TxtSsdt.aml",
  "SSDT",
  2,
  "INTEL ",
  "TxtSsdt",
  0x1000
  )
{
  External (CPID)

  Scope(\_SB) {
    //If (LEqual(TXTE, 1)) {
      Device (ACM) {
        Name (_HID, "INTC1025" )  // _HID: Hardware ID
        //
        // Update _CID (Compatibility ID)
        //   _CID will be updated based on CPU ID
        //
        Method (_CID) {
          //
          // Mask CPU Family
          //
          And (CPID, 0xFFFFFFF0, Local0)
          If (LOr (LOr(LEqual (Local0, 0x000C06D0), LEqual (Local0, 0x000C06D2)), LOr(LEqual (Local0, 0x000C06C0), LEqual (Local0, 0x000C06C2)))) {
            Return ("INT_PTL_SINIT")
          }
          Return ("INT_UNK_SINIT")
        } // End of Method (_CID)
      } // End of Device (ACM)
    //}
  } // End of Scope(\_SB) for TxtAcm
}  // End of defintion block
