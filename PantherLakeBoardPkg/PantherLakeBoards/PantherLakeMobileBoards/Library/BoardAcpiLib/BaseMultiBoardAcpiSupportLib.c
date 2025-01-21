/** @file
  Multi-Board ACPI Support Library

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

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardAcpiTableLib.h>
#include <Library/MultiBoardAcpiSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>

EFI_STATUS
EFIAPI
PtlBoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  );

BOARD_ACPI_TABLE_FUNC  mPtlBoardAcpiTableFunc = {
  PtlBoardUpdateAcpiTable
};

/**
  The constructor function to register mPtlBoardAcpiTableFunc function.

  @retval     EFI_SUCCESS  This constructor always return EFI_SUCCESS.
                           It will ASSERT on errors.
**/
EFI_STATUS
EFIAPI
PtlBaseMultiBoardAcpiSupportLibConstructor (
  VOID
  )
{

  RegisterBoardAcpiTableFunc (&mPtlBoardAcpiTableFunc);

  return EFI_SUCCESS;
}

