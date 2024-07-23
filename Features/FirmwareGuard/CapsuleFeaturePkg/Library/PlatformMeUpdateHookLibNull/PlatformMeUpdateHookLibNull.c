/** @file
  Platform hook to get ME information on platform or perform platform specific actions.

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
#include <Library/PlatformMeUpdateHookLib.h>

/**
  Returns the GUID value used to fill in the ImageTypeId field of the
  EFI_FIRMWARE_IMAGE_DESCRIPTOR structure that is returned by the GetImageInfo()
  service of the Firmware Management Protocol.

  @param [in][out]  Guid  Double pointer to a GUID value that is updated to point
                          to a GUID value. The GUID value is not allocated and must
                          not be modified or freed by the caller.

  @retval EFI_SUCCESS      EFI_FIRMWARE_IMAGE_DESCRIPTOR ImageTypeId GUID is set
                           to the returned Guid value.
  @retval EFI_UNSUPPORTED  Not support this function. No GUID be returned.

**/
EFI_STATUS
EFIAPI
PlatformFmpGetMeTypeIdGuidPtr (
  IN OUT  EFI_GUID  **Guid
  )
{
  return EFI_UNSUPPORTED;
}
