/** @file
  Debug Lib for PlatformFlashAccessLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "PlatformFlashAccessDebugLib.h"

extern EFI_RUNTIME_SERVICES  *gRT;

/*
  For fault tolerant test usage. Will clean it after moving to CR Common

*/
EFI_STATUS
EFIAPI
TestDeadLoop (
  VOID
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsFirstDeadLoop;
  UINTN       DataSize;

  DataSize = sizeof (BOOLEAN);
  Status = gRT->GetVariable (
                  L"IsFirstDeadLoop",
                  &gEfiCallerIdGuid,
                  NULL,
                  &DataSize,
                  &IsFirstDeadLoop
                  );
  if (Status != EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "Recovery from crash! Skip deadloop.\n"));
    return Status;
  }

  IsFirstDeadLoop = TRUE;
  Status = gRT->SetVariable (
                  L"IsFirstDeadLoop",
                  &gEfiCallerIdGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                  sizeof(BOOLEAN),
                  &IsFirstDeadLoop
                  );

  DEBUG ((DEBUG_INFO, "Recovery from crash! deadloop.\n"));
  //
  // Either stop and manual remove power to cause crash or auto reset system can be used
  //
  CpuDeadLoop ();
  //gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);

  return EFI_SUCCESS;
}
