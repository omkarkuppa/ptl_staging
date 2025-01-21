/** @file
   Google Test mocks for PciIoProcotol.

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

#ifndef MOCK_PCI_IO_PROCOTOL_LIB_H_
#define MOCK_PCI_IO_PROCOTOL_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/PciIo.h>
}

struct MockPciIoProcotol {
  MOCK_INTERFACE_DECLARATION (MockPciIoProcotol);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    MockPciIoConfigRead,
    (IN     EFI_PCI_IO_PROTOCOL        *This,
     IN     EFI_PCI_IO_PROTOCOL_WIDTH  Width,
     IN     UINT32                     Offset,
     IN     UINTN                      Count,
     IN OUT VOID                       *Buffer)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    MockPciIoConfigWrite,
    (IN     EFI_PCI_IO_PROTOCOL        *This,
     IN     EFI_PCI_IO_PROTOCOL_WIDTH  Width,
     IN     UINT32                     Offset,
     IN     UINTN                      Count,
     IN OUT VOID                       *Buffer)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    MockPciIoGetLocation,
    (IN  EFI_PCI_IO_PROTOCOL  *This,
     OUT UINTN                *Segment,
     OUT UINTN                *Bus,
     OUT UINTN                *Device,
     OUT UINTN                *Function)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    MockPciIoAttributes,
    (IN  EFI_PCI_IO_PROTOCOL                      *This,
     IN  EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
     IN  UINT64                                   Attributes,
     OUT UINT64                                   *Result)
    );
};

#endif
