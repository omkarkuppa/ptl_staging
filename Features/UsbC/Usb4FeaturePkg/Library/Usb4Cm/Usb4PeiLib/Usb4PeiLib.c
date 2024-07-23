/** @file
  USB4 PEI Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4PlatformHob.h>

/**
  Create USB4 Info Hob for passing USB4 configuration to Usb4CmDxe

  @param[in]  Usb4PlatformInfo - USB4 configuration information from platform

  @retval EFI_SUCCESS           - USB4 Info Hob is created successfully
  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_UNSUPPORTED       - Fail to create Hob
**/
EFI_STATUS
CreateUsb4InfoHob (
  IN USB4_PLATFORM_INFO     *Usb4PlatformInfo
  )
{
  EFI_STATUS           Status;
  USB4_PLATFORM_HOB    *Usb4PlatformHob;

  if (Usb4PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "CreateUsb4InfoHob: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Create USB4 Info Hob for Usb4CmDxe
  //
  Usb4PlatformHob = BuildGuidHob (&gUsb4PlatformHobGuid, sizeof (USB4_PLATFORM_HOB));
  if (Usb4PlatformHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to create USB4 Info Hob\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Save USB4 configuration information to USB4 Info Hob
  //
  CopyMem (&(Usb4PlatformHob->Usb4PlatformInfo), Usb4PlatformInfo, sizeof (USB4_PLATFORM_INFO));
  Status = EFI_SUCCESS;

Exit:
  return Status;
}