/** @file
  This file processes hotkey dispatch and callback event.

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

#include <Library/IoLib.h>
#include <Uefi.h>
#include <EcCommands.h>

/**
  eSPI EC SMI callback function.

  @param[in] DispatchHandle     The handle that was specified when the handler was registered.

**/
VOID
EFIAPI
eSpiEcSmiCallback (
  IN  EFI_HANDLE    DispatchHandle
  )
{
  //
  // This is a dummy handling routine.
  //
  IoWrite8 (EC_C_PORT, EC_C_SMI_QUERY);
  IoRead8 (EC_D_PORT);
}
