/**  @file
  PayloadResiliencySupportLib to support Payloads management resiliency feature

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

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/PayloadResiliencySupportLib.h>

/**
  Check whether the Payloads management backup Feature is enabled.

  @return TRUE       The Payloads management backup Feature is enabled.
  @return FALSE      The Payloads management backup Feature is not enabled.

**/
BOOLEAN
EFIAPI
IsPayloadBackupEnabled (
 )
{
  DEBUG((DEBUG_INFO, "The Payloads management backup Feature is enabled.\n"));
  return TRUE;
}

/**
  Get the Microcode Address in recovery boot

  @return       The Microcode Address value.

**/
UINT32
EFIAPI
GetMicrocodeBaseAddressInRecovery (
 )
{
  UINT32 MicrocodeFvBaseAddress;

  if (!SpiIsTopSwapEnabled ()) {
    MicrocodeFvBaseAddress = FixedPcdGet32 (PcdFlashFvMicrocodeBase);
  } else {
    MicrocodeFvBaseAddress = FixedPcdGet32 (PcdFlashFvMicrocodeRBase);
  }

  DEBUG ((DEBUG_INFO, "GetMicrocodeBaseAddressInRecovery - 0x%x\n", MicrocodeFvBaseAddress));

  return MicrocodeFvBaseAddress;
}