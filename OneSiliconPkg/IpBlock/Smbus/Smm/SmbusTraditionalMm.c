/** @file
  SMM Smbus Driver Entry

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include "SmbusSmm.h"

/**
  <b>SMBus SMM Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SMBus module is a SMM driver that provides a standard way to execute an SMBus command.

  - @pre
    - EFI_SMM_BASE2_PROTOCOL
      - Documented in the System Management Mode Core Interface Specification

  - @result
    The SMBus driver produces EFI_SMBUS_HC_PROTOCOL documented in the SMBus Host Controller Protocol Specification.

  - <b>Integration Checklist</b>\n
    - Verify prerequisites are met
    - Add Smbus\Smm\SmbusSmm.inf to the platform DSC file.

  @param[in] ImageHandle          ImageHandle of this module
  @param[in] SystemTable          EFI System Table

  @retval EFI_SUCCESS             Driver initializes successfully
  @retval Other values            Some error occurred
**/
EFI_STATUS
EFIAPI
SmbusInitSmm (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  return InitializeSmbusSmm ();
}
