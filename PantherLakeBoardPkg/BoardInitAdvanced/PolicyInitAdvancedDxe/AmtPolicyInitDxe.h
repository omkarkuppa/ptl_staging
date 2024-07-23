/** @file
  Header file for the AmtPolicyInitDxe Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _AMT_POLICY_INIT_DXE_H_
#define _AMT_POLICY_INIT_DXE_H_

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <FirwmareConfigurations.h>
#include <Protocol/MePolicy.h>
#include <Protocol/AmtPolicy.h>
#include <Library/DxeAmtPolicyLib.h>
#include <Library/DxeAmtPolicyUpdateLib.h>

extern UINT8 mFirmwareConfiguration;

/**
  Initialize Intel AMT DXE Policy

  @param[in] ImageHandle          Image handle of this driver.

  @retval EFI_SUCCESS             Initialization complete.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
AmtPolicyInitDxe (
  IN  EFI_HANDLE                  ImageHandle
  );

#endif
