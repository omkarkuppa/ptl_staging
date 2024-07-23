/** @file
  Intel ACPI Reference Code for WWAN _DSM Implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

  // DSM UUID for WWAN. Do Not change.
  If (LEqual (Arg0, ToUUID ("bad01b75-22a8-4f48-8792-bdde9467747d")))
  {
    //
    // Function 0: Query of supported functions.
    //
    If (LEqual (Arg2, Zero))
    {
      Return(Buffer(One) { 0x09 })
    }

    //
    // Function 1: Reserved.
    //
    If (LEqual (Arg2, 1))
    {
      // Reserved
    }

    //
    // Function 2: Reserved.
    //
    If (LEqual (Arg2, 2))
    {
      // Reserved
    }

    //
    // Function 3: Conveys the BIOS ACPI Device Power Mode setup option selection to the WWAN driver
    //
    If (LEqual (Arg2, 3))
    {
      //
      // _DSM Power
      // WRTO - WWAN RTD3 Option
      //   3: D3/L2
      //   2: D3/L1.2
      //   1: D0/L1.2
      //   0: Disabled
      //
      If (LEqual (WRTO, 1)) {
        ADBG ("WWAN Power _DSM D0/L1.2")
      } ElseIf (LEqual (WRTO, 2)) {
        ADBG ("WWAN Power _DSM D3/L1.2")
      } ElseIf (LEqual (WRTO, 3)) {
        ADBG ("WWAN Power _DSM D3/L2")
      } Else {
        ADBG ("WWAN Power _DSM Disabled")
      }
      Return (WRTO)
    }
  }
