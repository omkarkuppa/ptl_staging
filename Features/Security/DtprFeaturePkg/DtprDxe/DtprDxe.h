/** @file
  DTPR DXE header file.

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

#ifndef _DTPR_DXE_H_
#define _DTPR_DXE_H_

#include <TprInfoHob.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AcpiTable.h>
#include <Library/HobLib.h>

#define DTPR_ACPI_TABLE_SIGNATURE SIGNATURE_32('D', 'T', 'P', 'R')
#define MAX_TPR_COUNT             2
#define MAX_INS_COUNT             2
#define MAX_SRL_REG_COUNT         10

#pragma pack(push, 1)

typedef struct
{
  UINT64                         TprBase;
} TPR_BASE_LIMIT_STRUCT;


typedef struct
{
  UINT32                         Flags;
  UINT32                         TprCnt;
  TPR_BASE_LIMIT_STRUCT          TprArr[MAX_TPR_COUNT];
} TPR_INS_STRUCT;

typedef struct
{
  EFI_ACPI_DESCRIPTION_HEADER    Header;
  UINT32                         Flags;
  UINT32                         InsCnt;
  TPR_INS_STRUCT                 TprIns[MAX_INS_COUNT];
  UINT32                         SrlCnt;
  UINT64                         SrlRegArr[MAX_SRL_REG_COUNT];
} DTPR_TABLE_STRUCT;

#pragma pack(pop)

#endif
