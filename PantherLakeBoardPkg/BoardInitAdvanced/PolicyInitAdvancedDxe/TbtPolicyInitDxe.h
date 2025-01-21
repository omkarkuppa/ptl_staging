/** @file
  Header file for the TbtPolicyInitDxe Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _TBT_POLICY_INIT_DXE_H_
#define _TBT_POLICY_INIT_DXE_H_

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <FirwmareConfigurations.h>
#include <Protocol/ITbtPolicy.h>
#include <Library/DxeITbtPolicyLib.h>
#include <Library/DxeTbtPolicyUpdateLib.h>

extern UINT8 mFirmwareConfiguration;

/**
  <b>TBT DXE Policy Driver Entry Point</b> \n
  - <b>Introduction</b> \n
    TBT DXE drivers behavior can be controlled by platform policy without modifying reference code directly.
    Platform policy Protocol is initialized with default settings in this funciton.
    This policy Protocol has to be initialized prior to System Agent initialization DXE drivers execution.

  - @pre
    - Runtime variable service should be ready if policy initialization required.

  - @result
    ITBT_POLICY_PROTOCOL will be installed successfully and ready for TBT reference code use.

  - <b>Porting Recommendations</b> \n
    Policy should be initialized basing on platform design or user selection (like BIOS Setup Menu)

  @param[in] ImageHandle - Image handle of this driver.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED    The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
TbtPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  );

#endif
