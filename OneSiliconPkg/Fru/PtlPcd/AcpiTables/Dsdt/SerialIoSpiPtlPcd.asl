/** @file

  Serial IO SPI ACPI definitions

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

@par Specification Reference:
**/

#define SPI0_MODE         SM00
#define SPI1_MODE         SM01
#define SPI2_MODE         SM02
#define SPI3_MODE         SM03
#define SPI4_MODE         SM04
#define SPI5_MODE         SM05
#define SPI6_MODE         SM06

#define SPI0_PCIE_BASE    SC00
#define SPI1_PCIE_BASE    SC01
#define SPI2_PCIE_BASE    SC02
#define SPI3_PCIE_BASE    SC03
#define SPI4_PCIE_BASE    SC04
#define SPI5_PCIE_BASE    SC05
#define SPI6_PCIE_BASE    SC06


#define SPI_DEVICE_ADR       SERIAL_IO_SPI0_ADR
#define SPI_DEVICE_MODE      SPI0_MODE
#define SPI_DEVICE_PCI_BASE  SPI0_PCIE_BASE
Device (SPI0) {
Include ("SerialIoSpiController.asl")
}

#undef SPI_DEVICE_ADR
#undef SPI_DEVICE_MODE
#undef SPI_DEVICE_PCI_BASE
#define SPI_DEVICE_ADR       SERIAL_IO_SPI1_ADR
#define SPI_DEVICE_MODE      SPI1_MODE
#define SPI_DEVICE_PCI_BASE  SPI1_PCIE_BASE
Device (SPI1) {
Include ("SerialIoSpiController.asl")
}

#undef SPI_DEVICE_ADR
#undef SPI_DEVICE_MODE
#undef SPI_DEVICE_PCI_BASE
#define SPI_DEVICE_ADR       SERIAL_IO_SPI2_ADR
#define SPI_DEVICE_MODE      SPI2_MODE
#define SPI_DEVICE_PCI_BASE  SPI2_PCIE_BASE
Device (SPI2) {
Include ("SerialIoSpiController.asl")
}