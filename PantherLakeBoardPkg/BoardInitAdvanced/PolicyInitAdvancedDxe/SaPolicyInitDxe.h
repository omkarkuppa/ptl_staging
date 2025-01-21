/** @file
  Header file for the SaPolicyInitDxe Driver.

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
#ifndef _SA_POLICY_INIT_DXE_H_
#define _SA_POLICY_INIT_DXE_H_

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <FirwmareConfigurations.h>
#include <Protocol/IGpuPolicy.h>
#include <Protocol/GopPolicy.h>
#include <Library/DxeSaPolicyUpdateLib.h>
#include <Library/IGpuInfoLib.h>

extern UINT8 mFirmwareConfiguration;

/**
  <b>SA DXE Policy Driver Entry Point</b> \n
  - <b>Introduction</b> \n
    IGPU DXE drivers behavior can be controlled by platform policy without modifying reference code directly.
    Platform policy Protocol is initialized with default settings in this funciton.
    This policy Protocol has to be initialized prior to IGPU initialization DXE drivers execution.

  - @pre
    - Runtime variable service should be ready if policy initialization required.

  - @result
    IGPU_POLICY_PROTOCOL will be installed successfully and ready for IGPU reference code use.

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
SaPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  );

/**
  Function to register call back event to get GOP VBT data.

  @retval EFI_SUCCESS           Call back registered successfully.
  @retval EFI_UNSUPPORTED:      Call back registration fails.

**/
EFI_STATUS
GetGopVbtData (
  VOID
  );

/**
  Callback function to Update SaPolicyGopVbtPointer and Size.

  @param[in] Event    - The triggered event.
  @param[in] Context  - Context for this event.

**/
VOID
UpdateSaPolicyGopVbtPointer (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  );

#endif
