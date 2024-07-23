/** @file
  Header file for the PolicyInitAdvancedDxe Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _POLICY_INIT_ADVANCED_DXE_H_
#define _POLICY_INIT_ADVANCED_DXE_H_


#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <SetupVariable.h>

#include "AmtPolicyInitDxe.h"
#include "MePolicyInitDxe.h"
#include "SaPolicyInitDxe.h"
#include "PchPolicyInitDxe.h"
#include "SiliconPolicyInitDxe.h"
#include "GopPolicyInitDxe.h"
#include "CpuPolicyInitDxe.h"
#include "TbtPolicyInitDxe.h"

//#include <Library/DxeTbtPolicyLib.h>
/**
  Initialize DXE Platform Policy

  @param[in] ImageHandle - Image handle of this driver.
  @param[in] SystemTable - Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @exception EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/

EFI_STATUS
EFIAPI
PolicyInitAdvancedDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

#endif
