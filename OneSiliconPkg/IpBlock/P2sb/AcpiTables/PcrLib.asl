/**@file
  Intel ACPI Reference Code for PCR

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
// PCR Register Access Methods
// Include this file into a device scope which implements private configuration register access.
// Required variables:
// SBRG - holds the value of the sideband MMIO base addres.
//
//  SBREG_BAR_20BITADDRESS is configured by SoC
//
//  SBREG_BAR_20BITADDRESS=1, the format has included 16b addressing.
//  +---------------------------------------------------------------------------------------------+
//  | Addr[63:28]    | Addr[27:24]           | Addr[23:16]     | Addr[15:2]           | Addr[1:0] |
//  +----------------+-----------------------+-----------------+----------------------------------+
//  | REG_BAR[63:28] | TargetRegister[19:16] | TargetPort[7:0] | TargetRegister[15:2]             |
//  +---------------------------------------------------------------------------------------------+
//
//  SBREG_BAR_20BITADDRESS=0
//  +---------------------------------------------------------------------------------------------+
//  | Addr[63:24]    | Addr[27:24]           | Addr[23:16]     | Addr[15:2]           | Addr[1:0] |
//  +----------------+-----------------------+-----------------+----------------------------------+
//  | REG_BAR[63:24] | REG_BAR[27:24]        | TargetPort[7:0] | TargetRegister[15:2]             |
//  +---------------------------------------------------------------------------------------------+
//

//
// PCR Dword Read
// arg0: PID
// arg1: Offset
//
Method (PCRR, 2, Serialized) {
  And (arg1, 0xffff, Local1)
  ShiftLeft (And (arg1, 0x0f0000), 8, Local2)
  Add (ShiftLeft (And (arg0, 0xff), 16), Local1, Local0)
  Add (Add (Local2, Local0), SBRG, Local0)
  OperationRegion (PCR0, SystemMemory, Local0, 0x4)
  Field (PCR0, DWordAcc, Lock, Preserve) {
    Offset (0x00),
    DAT0, 32
  } // End Field PCR0
  Return (DAT0)
} // End Method PCRR

//
// PCR Dword Write
// arg0: PID
// arg1: Offset
// arg2: write data
//
Method (PCRW, 3, Serialized) {
  And (arg1, 0xffff, Local1)
  ShiftLeft (And (arg1, 0x0f0000), 8, Local2)
  Add (ShiftLeft (And (arg0, 0xff), 16), Local1, Local0)
  Add (Add (Local2, Local0), SBRG, Local0)
  OperationRegion (PCR0, SystemMemory, Local0, 0x4)
  Field (PCR0, DWordAcc, Lock, Preserve) {
    Offset (0x00),
    DAT0, 32
  } // End Field PCR0
  Store (arg2, DAT0)
} // End Method PCRW

//
// PCR Dword Or
// arg0: PID
// arg1: Offset
// arg2: Or data
//
Method (PCRO, 3, Serialized) {
  Store (PCRR (And (arg0, 0xff), arg1), Local0) // Store PCR Read data in Local0
  Store (Or (Local0, arg2), Local1)             // Or data
  PCRW (And (arg0, 0xff), arg1, Local1)         // Write data back
}

//
// PCR Dword And
// arg0: PID
// arg1: Offset
// arg2: And data
//
Method (PCRA, 3, Serialized) {
  Store (PCRR (And (arg0, 0xff), arg1), Local0) // Store PCR Read data in Local0
  Store (And (Local0, arg2), Local1)            // And data
  PCRW (And (arg0, 0xff), arg1, Local1)         // Write data back
}

//
// PCR Dword AndThenOr
// arg0: PID
// arg1: Offset
// arg2: And data
// arg3: Or data
//
Method (PCAO, 4, Serialized) {
  Store (PCRR (And (arg0, 0xff), arg1), Local0) // Store PCR Read data in Local0
  Store (Or (And (Local0, arg2), arg3), Local1) // AndThenOr
  PCRW (And (arg0, 0xff), arg1, Local1)         // Write data back
}
