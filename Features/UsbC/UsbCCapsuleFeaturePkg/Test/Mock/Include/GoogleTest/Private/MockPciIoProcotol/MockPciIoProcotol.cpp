/** @file
  Google Test mocks for PciIoProcotol

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

#include "MockPciIoProcotol.h"

MOCK_INTERFACE_DEFINITION (MockPciIoProcotol);

MOCK_FUNCTION_DEFINITION (MockPciIoProcotol, MockPciIoConfigRead, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPciIoProcotol, MockPciIoConfigWrite, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPciIoProcotol, MockPciIoGetLocation, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPciIoProcotol, MockPciIoAttributes, 4, EFIAPI);

//
// Mock Pci Io Protocol Interface
//
EFI_PCI_IO_PROTOCOL LocalPciIoInterface = {
  NULL,                                 // PciIoPollMem
  NULL,                                 // PciIoPollIo
  {
    NULL,                               // PciIoMemRead
    NULL,                               // PciIoMemWrite
  },
  {
    NULL,                               // PciIoIoRead
    NULL,                               // PciIoIoWrite
  },
  {
    MockPciIoConfigRead,
    MockPciIoConfigWrite
  },
  NULL,                                 // PciIoCopyMem
  NULL,                                 // PciIoMap
  NULL,                                 // PciIoUnmap
  NULL,                                 // PciIoAllocateBuffer
  NULL,                                 // PciIoFreeBuffer
  NULL,                                 // PciIoFlush
  MockPciIoGetLocation,                 // PciIoGetLocation
  MockPciIoAttributes,                  // PciIoAttributes
  NULL,                                 // PciIoGetBarAttributes
  NULL,                                 // PciIoSetBarAttributes
  0,
  NULL
};

EFI_PCI_IO_PROTOCOL *gPciIoInterface = &LocalPciIoInterface;
