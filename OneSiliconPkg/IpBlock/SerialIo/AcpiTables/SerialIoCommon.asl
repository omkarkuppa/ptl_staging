/**@file

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

#include <Register/SerialIoRegs.h>

#define SERIAL_IO_BAR_RESET 1
#define SERIAL_IO_D3        1

//
// Include PchSerialIo ACPI macros
//


//
//  Serial IO D3
//  Common code for D3 entry for all SerialIo devices including PCI mode
//
//  @param[in]  Arg0      PCI CFG address
//  @param[in]  Arg1      Boolean - Place Device in D3 (SERIAL_IO_D3)
//  @param[in]  Arg2      Boolean - Reset BAR0 (SERIAL_IO_BAR_RESET) or leave BAR0 unchanged (0)
//                                  depends on Arg1 equal to SERIAL_IO_D3 - device placed in D3
//
Method (SOD3, 3, Serialized) {
  OperationRegion (ICB1, SystemMemory, Add (\_SB.PC00.GPCB (), Arg0), Add (R_SERIAL_IO_CFG_PMECTRLSTATUS, 4))
  If (Arg1) { // SERIAL_IO_D3
    Field (ICB1, ByteAcc, NoLock, Preserve) {
      Offset (R_SERIAL_IO_CFG_PMECTRLSTATUS),
      PMEC, 8 //Note: SerialIO Does Not Support PME
    }
    Store (0x3, PMEC) // Set BIT[1:0] = 11b - Power State D3
    Or (PMEC, 0, PMEC) // perform a read to avoid ordering and noncoherency problems
  }
  If (LAnd (Arg1, Arg2)) { //SERIAL_IO_BAR_RESET only if device is placed in D3
    Field (ICB1, AnyAcc, NoLock, Preserve) {
      Offset (R_SERIAL_IO_CFG_BAR),
      BAR0, 64
    }
    Store (0, BAR0)
  }
}

