/** @file
  Dxe library function to reset the system from FSP wrapper.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#include <PiDxe.h>
#include <PchResetPlatformSpecific.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <FspEas.h>

/**
  Perform platform related reset in FSP wrapper.

  @param[in] ResetType  The type of reset the platform has to perform.

  @return Will reset the system based on Reset status provided.
**/
VOID
EFIAPI
CallFspWrapperResetSystem (
  EFI_STATUS    ResetType
  )
{
  PCH_RESET_DATA             ResetData;
  EFI_RESET_TYPE             EfiResetType;
  UINTN                      DataSize;

  DataSize = 0;
  switch (ResetType) {
    case FSP_STATUS_RESET_REQUIRED_COLD:
      EfiResetType = EfiResetCold;
      break;
    case FSP_STATUS_RESET_REQUIRED_WARM:
      EfiResetType = EfiResetWarm;
      break;
    case FSP_STATUS_RESET_REQUIRED_3:
      EfiResetType = EfiResetPlatformSpecific;
      CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
      DataSize = sizeof (PCH_RESET_DATA);
      StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
      break;
    default:
      DEBUG ((DEBUG_ERROR, "UnSupported reset type requested. Initiating cold reset\n"));
      EfiResetType = EfiResetCold;
  }
  gRT->ResetSystem (EfiResetType, EFI_SUCCESS, DataSize, &ResetData);
}
