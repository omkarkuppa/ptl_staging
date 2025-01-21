/**@file

  Serial IO SPI Controller ACPI definitions

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

    If (LEqual (SPI_DEVICE_MODE, SERIAL_IO_SPI_PCI)) {
      Method (_DSM,4,Serialized) {if (PCIC (Arg0)) { Return (PCID (Arg0,Arg1,Arg2,Arg3)) }; Return (Buffer () {0})}
      Method (_PS3) { SOD3 (SPI_DEVICE_PCI_BASE, SERIAL_IO_D3, SERIAL_IO_BAR_RESET) }
      Method (_PS0) {  }
    }
    Method (_ADR) { Return (SPI_DEVICE_ADR) }
