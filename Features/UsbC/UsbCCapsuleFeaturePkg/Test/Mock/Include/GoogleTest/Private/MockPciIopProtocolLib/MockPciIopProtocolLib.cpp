/** @file
  Google Test mocks for PciIopProtocolLib

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

#include "MockPciIopProtocolLib.h"

MOCK_INTERFACE_DEFINITION (MockPciIopProtocolLib);

MOCK_FUNCTION_DEFINITION (MockPciIopProtocolLib, Config_Read, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPciIopProtocolLib, Config_Write, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPciIopProtocolLib, PciIopProtocol_GetLocation, 5, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPciIopProtocolLib, Config_Attributes, 4, EFIAPI);
MOCK_FUNCTION_DEFINITION (MockPciIopProtocolLib, Mem_Read, 6, EFIAPI);

EFI_PCI_IO_PROTOCOL  localPp = {
  NULL,                                                          // PollMem
  NULL,                                                          // PollIo
  {
    (EFI_PCI_IO_PROTOCOL_IO_MEM) Mem_Read,                       // Mem(Read)
    NULL                                                         // Mem(Write)
  },
  {
    NULL,                                                        // Io(Read)
    NULL                                                         // Io(Write)
  },
  {
    (EFI_PCI_IO_PROTOCOL_CONFIG)  Config_Read,                   // Pci(Read)
    (EFI_PCI_IO_PROTOCOL_CONFIG)  Config_Write                   // Pci(Write)
  },
  NULL,                                                          // CopyMem
  NULL,                                                          // Map
  NULL,                                                          // Unmap
  NULL,                                                          // AllocateBuffer
  NULL,                                                          // FreeBuffer
  NULL,                                                          // Flush
  (EFI_PCI_IO_PROTOCOL_GET_LOCATION) PciIopProtocol_GetLocation, // GetLocation
  (EFI_PCI_IO_PROTOCOL_ATTRIBUTES) Config_Attributes,            // Attributes
  NULL,                                                          // GetBarAttributes
  NULL,                                                          // SetBarAttributes

  ///
  /// The size, in bytes, of the ROM image.
  ///
  (UINT64) NULL,                                                 // RomSize

  ///
  /// A pointer to the in memory copy of the ROM image. The PCI Bus Driver is responsible
  /// for allocating memory for the ROM image, and copying the contents of the ROM to memory.
  /// The contents of this buffer are either from the PCI option ROM that can be accessed
  /// through the ROM BAR of the PCI controller, or it is from a platform-specific location.
  /// The Attributes() function can be used to determine from which of these two sources
  /// the RomImage buffer was initialized.
  ///
  NULL                                                            // *RomImage
};
