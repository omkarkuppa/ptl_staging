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

#ifndef MOCK_PCIO_IO_PROTOCOL_LIB_H_
#define MOCK_PCIO_IO_PROTOCOL_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Protocol/PciIo.h>

extern EFI_PCI_IO_PROTOCOL localPp;
}

struct MockPciIopProtocolLib {
  MOCK_INTERFACE_DECLARATION (MockPciIopProtocolLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    PciIopProtocol_GetLocation,
    (IN EFI_PCI_IO_PROTOCOL *This,
     OUT UINTN              *SegmentNumber,
     OUT UINTN              *BusNumber,
     OUT UINTN              *DeviceNumber,
     OUT UINTN              *FunctionNumber)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    Config_Attributes,
    (IN  EFI_PCI_IO_PROTOCOL                      *This,
     IN  EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
     IN  UINT64                                   Attributes,
     OUT UINT64                                   *Result)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    Config_Read,
    (IN     EFI_PCI_IO_PROTOCOL        *This,
     IN     EFI_PCI_IO_PROTOCOL_WIDTH  Width,
     IN     UINT32                     Offset,
     IN     UINTN                      Count,
     IN OUT VOID                       *Buffer)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    Config_Write,
    (IN     EFI_PCI_IO_PROTOCOL        *This,
     IN     EFI_PCI_IO_PROTOCOL_WIDTH  Width,
     IN     UINT32                     Offset,
     IN     UINTN                      Count,
     IN OUT VOID                       *Buffer)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    Mem_Read,
    (IN     EFI_PCI_IO_PROTOCOL        *This,
     IN     EFI_PCI_IO_PROTOCOL_WIDTH  Width,
     IN     UINT8                      BarIndex,
     IN     UINT64                     Offset,
     IN     UINTN                      Count,
     IN OUT VOID                       *Buffer)
    );
};

#endif
