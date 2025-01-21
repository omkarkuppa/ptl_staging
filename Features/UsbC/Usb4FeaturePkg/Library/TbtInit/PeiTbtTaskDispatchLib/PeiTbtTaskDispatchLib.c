/** @file
  PEI TBT Task Dispatch library.

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

#include <Library/PeiTbtTaskDispatchLib.h>
#include <Library/DebugLib.h>

/**
  Covert the current EFI_BOOT_MODE to TBT_BOOT_MODE

  @retval  TBT_BOOT_MODE                Mode to be used to execute corresponding TBT tasks.
**/
TBT_BOOT_MODE
TbtGetBootMode (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_BOOT_MODE                         BootMode;

  Status = PeiServicesGetBootMode (&BootMode);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "TbtGetBootMode: Fail. Return TBT_NULL."));
    return TBT_NULL;
  }

  if (BootMode == BOOT_ON_S3_RESUME) {
    return TBT_S3;
  }

  if (BootMode == BOOT_ON_S4_RESUME) {
    return TBT_S4;
  }

  return TBT_NORMAL;
}

/**
  TbtTaskDispatch: Dispatch the TBT tasks according to TBT_CALL_TABLE_ENTRY

  @param[in] TBT_CALL_TABLE_ENTRY       TbtCallTable
  @param[in] VOID                       PeiTbtConfig
**/
VOID
TbtTaskDispatch (
  IN TBT_CALL_TABLE_ENTRY               *TbtCallTable,
  IN VOID                               *PeiTbtConfig
)
{
  EFI_STATUS                            Status;
  UINT32                                Index;
  TBT_BOOT_MODE                         TbtBootMode;
  const TBT_CALL_TABLE_ENTRY            *Task;

  //
  // Perform the TBT Task
  //

  if (TbtCallTable == NULL)
  {
    DEBUG((DEBUG_ERROR, "[TBT] TbtTaskDispatch: Invalid Call Table. Return."));
    return;
  }

  TbtBootMode = TbtGetBootMode ();

  Index = 0;
  Task = &TbtCallTable[0];
  while (AsciiStrCmp (Task->String, "END_OF_TASK") != 0) {
    Task = &TbtCallTable[Index];

    //
    // Decide if we need to execute the selected TBT task.
    //
    if ((NULL == Task->TbtTask) || (Task->TbtBootModeFlag == TBT_NULL)) {
      Index++;
      continue;
    }
    if (((TbtBootMode == TBT_S3) && (Task->TbtBootModeFlag & TBT_S3))
      || ((TbtBootMode == TBT_S4) && (Task->TbtBootModeFlag & TBT_S4))
      || ((TbtBootMode == TBT_NORMAL) && (Task->TbtBootModeFlag & TBT_NORMAL))
      )
    {
      DEBUG ((DEBUG_INFO, "[TBT] TbtTask [%a] Start\n", Task->String));
      Status = Task->TbtTask (PeiTbtConfig);
      DEBUG ((DEBUG_INFO, "[TBT] TbtTask [%a] End, Status = %r\n", Task->String, Status));

      if (EFI_ERROR(Status)){
        DEBUG((DEBUG_ERROR, "[TBT] TbtTask [%a] is failed. Aborting TbtTask Table Call.\n", Task->String));
        break;          // It will go out of While Loop
      }
    }

    Index++;
  }
}