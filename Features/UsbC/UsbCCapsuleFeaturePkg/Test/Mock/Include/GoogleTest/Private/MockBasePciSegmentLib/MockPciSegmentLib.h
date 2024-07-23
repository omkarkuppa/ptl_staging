/** @file
  Google Test mocks for BasePciSegmentLib

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

#ifndef MOCK_PCI_SEGMENT_LIB_H_
#define MOCK_PCI_SEGMENT_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
  #include <Base.h>
  #include <Uefi/UefiBaseType.h>
}

struct MockPciSegmentLib {
  MOCK_INTERFACE_DECLARATION (MockPciSegmentLib);

  MOCK_FUNCTION_DECLARATION (
    UINT8,
    PciSegmentRead8,
    (IN UINT64  Address)
    );
};

#endif
