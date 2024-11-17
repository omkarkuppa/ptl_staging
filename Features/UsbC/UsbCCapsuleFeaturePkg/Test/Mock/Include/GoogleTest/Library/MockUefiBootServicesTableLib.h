/** @file
  Google Test mocks for UefiBootServicesTableLib

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

#ifndef MOCK_UEFI_BOOT_SERVICES_TABLE_LIB_H_
#define MOCK_UEFI_BOOT_SERVICES_TABLE_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
}

struct MockUefiBootServicesTableLib {
  MOCK_INTERFACE_DECLARATION (MockUefiBootServicesTableLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    gBS_LocateProtocol,
    (IN  EFI_GUID  *Protocol,
     IN  VOID      *Registration  OPTIONAL,
     OUT VOID      **Interface)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    gBS_LocateHandleBuffer,
    (IN  EFI_LOCATE_SEARCH_TYPE  SearchType,
     IN  EFI_GUID                *Protocol       OPTIONAL,
     IN  VOID                    *SearchKey      OPTIONAL,
     OUT UINTN                   *NoHandles,
     OUT EFI_HANDLE              **Buffer)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    gBS_DisconnectController,
    (IN EFI_HANDLE  ControllerHandle,
     IN EFI_HANDLE  DriverImageHandle  OPTIONAL,
     IN EFI_HANDLE  ChildHandle        OPTIONAL)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    gBS_FreePool,
    (IN VOID *Buffer)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    gBS_CoreStall,
    (IN UINTN  Microseconds)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    gBS_ConnectController,
    (IN  EFI_HANDLE                ControllerHandle,
     IN  EFI_HANDLE                *DriverImageHandle    OPTIONAL,
     IN  EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath  OPTIONAL,
     IN  BOOLEAN                   Recursive)
    );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    gBS_HandleProtocol,
    (IN  EFI_HANDLE  Handle,
     IN  EFI_GUID    *Protocol,
     OUT VOID        **Interface)
    );
};

#endif
