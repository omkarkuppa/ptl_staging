/** @file
  Google Test mocks for TbtNvmRetimerUpdateLib

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

#ifndef MOCK_TBT_NVM_RETIMER_UPDATE_LIB_H
#define MOCK_TBT_NVM_RETIMER_UPDATE_LIB_H

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>

extern "C" {
#include <Uefi.h>
#include <Library/TbtNvmRetimerUpdateLib.h>
}

struct MockTbtNvmRetimerUpdateLib {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmRetimerUpdateLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    ReadRetimerNvmVersion, 
    (IN   RETIMER_DEV_INSTANCE   RetimerDevice,
     OUT  UINT32                *Version)
     );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    DestroyRetimerDevInstance,
    (IN  RETIMER_DEV_INSTANCE  RetimerDevice)
     );
  MOCK_FUNCTION_DECLARATION (
    VOID,
    UpdateRetimerNvmInformation,
    (IN   UINT8                    FirmwareType,
     IN   UINT8                    PcieRpType,
     IN   UINT8                    PcieRootPort,
     IN   RETIMER_DEV_ADDRESS      *DevAddress,
     IN   UINT32                   RetimerVersion)
     );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    UpdateRetimerNvmFirmware,
    (IN  RETIMER_DEV_INSTANCE                              RetimerDevice,
     IN  UINT8                                             *RetimerImage,
     IN  UINTN                                             ImageSize,
     IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS     Progress,          OPTIONAL
     IN  UINTN                                             StartPercentage,
     IN  UINTN                                             EndPercentage)
     );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    CreateRetimerDevInstance,
    (IN   UINT8                    FirmwareType,
     IN   UINT8                    PcieRpType,
     IN   UINT8                    PcieRootPort,
     IN   RETIMER_DEV_ADDRESS      *DevAddress,
     OUT  RETIMER_DEV_INSTANCE     *RetimerDevice)
     );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    CreateTBTDevInstance,
    (IN   UINT8                         FirmwareType,
     IN   UINT8                         PcieRpType,
     IN   UINT8                         PcieRootPort,
     OUT  DISCRETE_TBT_DEV_INSTANCE     *DiscreteTbtDevice)
     );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    UpdateDiscreteTbtNvmFirmware,
    (IN  DISCRETE_TBT_DEV_INSTANCE                         DiscreteTbtDevice,
     IN  UINT8                                             *DiscreteTbtImage,
     IN  UINTN                                             ImageSize,
     IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS     Progress,          OPTIONAL
     IN  UINTN                                             StartPercentage,
     IN  UINTN                                             EndPercentage)
     );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    DestroyTbtDevInstance,
    (IN  DISCRETE_TBT_DEV_INSTANCE    DiscreteTbtDevice)
     );
};

#endif
