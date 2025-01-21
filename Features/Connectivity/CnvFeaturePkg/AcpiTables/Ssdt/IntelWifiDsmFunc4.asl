/** @file
  Intel ACPI Sample Code for Intel WiFi DSM FUNC4 Implementation

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

Name (FDEL, 0)
Name (PDEL, 0)
Name (VDEL, 0)

// Function 4: CNVd PLDR Timer Control
// Arg 3 DWORD
// struct PLDR_MODE
// {
//   UINT16 TimerType;
//   UINT16 TimerDurationInMs;
// };
//
// TimerType 1        -  Set Product Reset FLR Timeout (FDEL)
// TimerDurationInMs  -  10 ms Default
//         Min Value  -  0 (No Timer after FLR triggered by BIOS)
//         Max Value  -  100 ms
//
// TimerType 2        -  Set Product Reset PERST Timeout (PDEL)
// TimerDurationInMs  -  100 ms Default
//         Min Value  -  0 (No Timer after PERST triggered by BIOS)
//         Max Value  -  200 ms
//
// TimerType 3        -  Set Product Reset VSEQ Bits Timeout (VDEL)
// TimerDurationInMs  -  10 ms Default
//         Min Value  -  0 (No Timer after PERST triggered by BIOS)
//         Max Value  -  200 ms
//
If (LEqual (Arg2, 4)) {
  CreateWordField (Arg3, 0, PRTT)
  CreateWordField (Arg3, 2, PRTD)
  ADBG (Concatenate ("TimerType = ", ToHexString (PRTT)))
  ADBG (Concatenate ("TimerDuration (ms) = ", ToDecimalString (PRTD)))
  If (LEqual (PRTT, 1)) {
    Store (PRTD, FDEL)
  } ElseIf (LEqual (PRTT, 2)) {
    Store (PRTD, PDEL)
  } ElseIf (LEqual (PRTT, 3)) {
    Store (PRTD, VDEL)
  } Else {
    ADBG ("TimerDuration not set")
  }
}