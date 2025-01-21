/**@file
  Integrated TSN PTL SOC controller ACPI methods

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

Scope (\_SB.PC00) {
  //
  // GBE TSN (PCI Mode)
  //
  Device (TSN1) {
    Name (_ADR, 0x001D0000)
    OperationRegion (TSRT,PCI_Config,0x00,0x100)
    Field (TSRT,AnyAcc,NoLock,Preserve)
    {
      DVID, 16,
      Offset (0x10),
      TADL,  32,
      TADH,  32,
    }
  } // Device (TSN1)

  Device (TSN2) {
    Name (_ADR, 0x001D0001)
    OperationRegion (TSRT,PCI_Config,0x00,0x100)
    Field (TSRT,AnyAcc,NoLock,Preserve)
    {
      DVID, 16,
      Offset (0x10),
      TADL,  32,
      TADH,  32,
    }
  } // Device (TSN2)

  Device (TSN3) {
    Name (_ADR, 0x001D0002)
    OperationRegion (TSRT,PCI_Config,0x00,0x100)
    Field (TSRT,AnyAcc,NoLock,Preserve)
    {
      DVID, 16,
      Offset (0x10),
      TADL,  32,
      TADH,  32,
    }
  } // Device (TSN3)

  Device (TSN4) {
    Name (_ADR, 0x001D0003)
    OperationRegion (TSRT,PCI_Config,0x00,0x100)
    Field (TSRT,AnyAcc,NoLock,Preserve)
    {
      DVID, 16,
      Offset (0x10),
      TADL,  32,
      TADH,  32,
    }
  } // Device (TSN4)
}