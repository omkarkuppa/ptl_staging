/** @file
  Intel One Click Recovery PBA boot functional definition

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

#ifndef __OCR_PBA_BOOT_SUPPORT_H__
#define __OCR_PBA_BOOT_SUPPORT_H__

#include <PiDxe.h>
#include <Protocol/OneClickRecoveryProtocol.h>

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
  );

#endif // __OCR_PBA_BOOT_SUPPORT_H__
