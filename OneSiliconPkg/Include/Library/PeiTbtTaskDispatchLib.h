/**@file
  PEI TBT Task Dispatch library Header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef __PEI_TBT_TASK_DISPATCH_LIB_H__
#define __PEI_TBT_TASK_DISPATCH_LIB_H__

#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>

typedef
EFI_STATUS
(EFIAPI *TBT_TASK) (
  VOID  *PeiTbtConfig
);

typedef enum {
  TBT_NULL,                             ///< All policy flags turned off.
  TBT_NORMAL   = (1 << 0),              ///< Execute TBT function on cold reset.
  TBT_S3       = (1 << 1),              ///< Execute TBT function on S3 exit.
  TBT_S4       = (1 << 2),              ///< Execute TBT function on S4 exit.
  TBT_ALL      = MAX_UINTN              ///< Execute TBT function always.
} TBT_BOOT_MODE;

typedef struct {
  TBT_TASK      TbtTask;                ///< Ptr to function to execute, with parameter list.
  TBT_BOOT_MODE TbtBootModeFlag;        ///< Call table base on TbtBootModeFlag
  CHAR8         *String;                ///< Output string describing this task.
} TBT_CALL_TABLE_ENTRY;

/**
  Covert the current EFI_BOOT_MODE to TBT_BOOT_MODE

  @retval  TBT_BOOT_MODE                Mode to be used to execute corresponding TBT tasks.
**/
TBT_BOOT_MODE
TbtGetBootMode (
  VOID
);

/**
  TbtTaskDispatch: Dispatch the TBT tasks according to TBT_CALL_TABLE_ENTRY

  @param[in] TBT_CALL_TABLE_ENTRY       TbtCallTable
  @param[in] VOID                       PeiTbtConfig
**/
VOID
TbtTaskDispatch (
  IN TBT_CALL_TABLE_ENTRY               *TbtCallTable,
  IN VOID                               *PeiTbtConfig
);
#endif
