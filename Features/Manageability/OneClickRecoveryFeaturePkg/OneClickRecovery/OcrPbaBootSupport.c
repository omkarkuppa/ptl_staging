/** @file
  Intel One Click Recovery PBA boot functional implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include "OcrPbaBootSupport.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DevicePathLib.h>

/*
  Add PBA Boot Option to Boot Option List

  @param[in]     BootOptionsList        Boot Options List
  @param[in,out] UefiBootOptions        Boot Options
  @param[in,out] NumOfUefiBootOptions   Number of UEFI boot options

  @retval EFI_SUCCESS                   Added OCR Boot Option
  @retval EFI_BUFFER_TOO_SMALL          Not enough room for boot options
  @retval EFI_INVALID_PARAMETER         parameters are null pointers
*/
EFI_STATUS
AddPbaBootOptions (
  IN     BOOT_OPTIONS       *BootOptionsList,
  IN OUT UEFI_BOOT_OPTION   *UefiBootOptions,
  IN OUT UINT16             *NumOfUefiBootOptions
  )
{
  UINTN       Index;

  if ((BootOptionsList == NULL) || (UefiBootOptions == NULL) || (NumOfUefiBootOptions == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Index = 0;
  while ((BootOptionsList[Index].Description != NULL) && (BootOptionsList[Index].DevicePath != NULL)) {
    if (*NumOfUefiBootOptions < MAX_UEFI_BOOT_OPTIONS) {
      UnicodeStrToAsciiStrS (
        BootOptionsList[Index].Description,
        (CHAR8 *) UefiBootOptions[*NumOfUefiBootOptions].Description,
        MAX_UEFI_BOOT_OPTION_DESC_LENGTH * sizeof (CHAR8)
        );
      UefiBootOptions[*NumOfUefiBootOptions].DescriptionLength = (UINT16)StrLen (BootOptionsList[Index].Description);

      UnicodeStrToAsciiStrS (
        BootOptionsList[Index].DevicePath,
        (CHAR8 *) UefiBootOptions[*NumOfUefiBootOptions].EfiDevicePath,
        MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH * sizeof (CHAR8)
        );
      UefiBootOptions[*NumOfUefiBootOptions].DevicePathLength = (UINT16)StrLen (BootOptionsList[Index].DevicePath);

      UefiBootOptions[*NumOfUefiBootOptions].UefiBootOptionType = Pba;
      (*NumOfUefiBootOptions)++;
    } else {
      DEBUG ((DEBUG_ERROR, "No free space to put Oem Pba entry in\n"));
      return EFI_BUFFER_TOO_SMALL;
    }

    Index++;
  }

  return EFI_SUCCESS;
}
