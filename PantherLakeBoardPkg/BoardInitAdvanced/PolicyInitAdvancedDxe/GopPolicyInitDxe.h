/** @file
  Header file for the GopPolicyInitDxe Driver.

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

  @par Specification Reference:
**/
#ifndef _GOP_POLICY_INIT_DXE_H_
#define _GOP_POLICY_INIT_DXE_H_

#include <Protocol/FirmwareVolume2.h>
#include <SetupVariable.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/EcMiscLib.h>
#include <PlatformBoardId.h>
#include <Library/PcdLib.h>
#include <Library/IGpuInfoLib.h>

/**
Initialize GOP DXE Policy

@param[in] ImageHandle          Image handle of this driver.

@retval EFI_SUCCESS             Initialization complete.
@retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
@retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
@retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
GopPolicyInitDxe (
  IN EFI_HANDLE           ImageHandle
  );

#endif
