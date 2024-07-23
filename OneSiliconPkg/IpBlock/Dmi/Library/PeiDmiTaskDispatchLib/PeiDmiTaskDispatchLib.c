/**@file
  PEI DMI Task Dispatch library.

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

#include <Library/PeiDmiTaskDispatchLib.h>

/**
  DmiTaskDispatch: Dispatch the DMI tasks according to IP_DMI3_FEAT_ID_DISPATCH_ENTRY

  @param[in] IP_DMI3_FEAT_ID_DISPATCH_ENTRY       DmiCallTable
  @param[in] VOID                                 PeiDmiConfig
**/
VOID
EFIAPI
DmiTaskDispatch (
  IN IP_DMI3_FEAT_ID_DISPATCH_ENTRY     *DmiCallTable,
  IN VOID                               *PeiDmiConfig
)
{
  EFI_STATUS                             Status;
  UINT32                                 Index;
  const IP_DMI3_FEAT_ID_DISPATCH_ENTRY   *Task;

  //
  // Perform the DMI Task
  //

  if (DmiCallTable == NULL) {
    DEBUG((DEBUG_ERROR, "[DMI] DmiTaskDispatch: Invalid Call Table. Return."));
    return;
  }

  Index = 0;
  Task = &DmiCallTable[0];
  while (AsciiStrCmp (Task->String, "END_OF_TASK") != 0) {
    Task = &DmiCallTable[Index];

    //
    // Decide if we need to execute the selected DMI task.
    //
    if ((NULL == Task->DmiTask) || (Task->DmiBootModeFlag == DMI_NULL)) {
      Index++;
      continue;
    }
    DEBUG ((DEBUG_INFO, "[DMI] DmiTask [%a] Start\n", Task->String));
    Status = Task->DmiTask (PeiDmiConfig);
    DEBUG ((DEBUG_INFO, "[DMI] DmiTask [%a] End, Status = %r\n", Task->String, Status));

    if (EFI_ERROR(Status)){
      DEBUG((DEBUG_ERROR, "[DMI] DmiTask [%a] is failed. Aborting DmiTask Table Call.\n", Task->String));
      break;          // It will go out of While Loop
    }
    Index++;
  }
}