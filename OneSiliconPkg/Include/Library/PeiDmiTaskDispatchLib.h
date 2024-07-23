/**@file
  PEI DMI Task Dispatch library Header file

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
#ifndef __PEI_DMI_TASK_DISPATCH_LIB_H__
#define __PEI_DMI_TASK_DISPATCH_LIB_H__

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
(EFIAPI *DMI_TASK) (
  VOID  *PeiDmiConfig
);

typedef enum {
  DMI_NULL,                             ///< All policy flags turned off.
  DMI_NORMAL   = (1 << 0),              ///< Execute DMI function on cold reset.
  DMI_S3       = (1 << 1),              ///< Execute DMI function on S3 exit.
  DMI_S4       = (1 << 2),              ///< Execute DMI function on S4 exit.
  DMI_ALL      = MAX_UINTN              ///< Execute DMI function always.
} DMI_BOOT_MODE;

typedef struct {
  DMI_TASK      DmiTask;                ///< Ptr to function to execute, with parameter list.
  DMI_BOOT_MODE DmiBootModeFlag;        ///< Call table base on DmiBootModeFlag
  CHAR8         *String;                ///< Output string describing this task.
} IP_DMI3_FEAT_ID_DISPATCH_ENTRY;


/**
  Covert the current EFI_BOOT_MODE to DMI_BOOT_MODE

  @retval  DMI_BOOT_MODE                Mode to be used to execute corresponding DMI tasks.
**/
DMI_BOOT_MODE
DmiGetBootMode (
  VOID
);

/**
  DmiTaskDispatch: Dispatch the DMI tasks according to IP_DMI3_FEAT_ID_DISPATCH_ENTRY

  @param[in] IP_DMI3_FEAT_ID_DISPATCH_ENTRY    DmiCallTable
  @param[in] VOID                              PeiDmiConfig
**/
VOID
EFIAPI
DmiTaskDispatch (
  IN IP_DMI3_FEAT_ID_DISPATCH_ENTRY     *DmiCallTable,
  IN VOID                               *PeiDmiConfig
);
#endif
