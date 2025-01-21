/** @file
  Google Test mocks for FirmwareManagementLib

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

#ifndef MOCK_FIRMWARE_MANAGEMENT_LIB_H
#define MOCK_FIRMWARE_MANAGEMENT_LIB_H

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>

extern "C" {
#include <Uefi.h>
#include <Protocol/FirmwareManagement.h>

extern EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  LocalProgress;
}

struct MockFirmwareManagementLib {
  MOCK_INTERFACE_DECLARATION (MockFirmwareManagementLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    Progress,
    (IN UINTN  Completion)
    );
};

#endif
