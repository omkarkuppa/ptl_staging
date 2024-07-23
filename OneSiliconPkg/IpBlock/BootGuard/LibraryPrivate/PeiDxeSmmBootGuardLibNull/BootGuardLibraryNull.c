/** @file
  BootGuardLibrary implementation.

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
#include <Uefi.h>
#include <Library/BootGuardLib.h>

/**
  Determine if Boot Guard is supported

  @retval TRUE  - Processor is Boot Guard capable.
  @retval FALSE - Processor is not Boot Guard capable.

**/
BOOLEAN
EFIAPI
IsBootGuardSupported (
  VOID
  )
{
  return FALSE;
}

/**
   Handle Tpm2Startup (State) failures reported by ACM.

   The function checks for the TpmStartupFailureOnS3 flag if TRUE it will
   request the system to perform a cold reset.

   @param[in] *BootGuardInfo - Pointer to BootGuardInfo.
**/
VOID
EFIAPI
HandleTpmStartupFailureOnS3 (
  IN BOOT_GUARD_INFO *BootGuardInfo
  )
{
  return;
}

/**
  Report platform specific Boot Guard information.

  @param[out] *BootGuardInfo - Pointer to BootGuardInfo.
**/
VOID
EFIAPI
GetBootGuardInfo (
  OUT BOOT_GUARD_INFO *BootGuardInfo
  )
{
  return;
}

/**
  Stop PBE timer if system is in Boot Guard boot

  @retval EFI_SUCCESS        - Stop PBE timer
  @retval EFI_UNSUPPORTED    - Not in Boot Guard boot mode.
**/
EFI_STATUS
EFIAPI
StopPbeTimer (
  VOID
  )
{
  return EFI_UNSUPPORTED;
}