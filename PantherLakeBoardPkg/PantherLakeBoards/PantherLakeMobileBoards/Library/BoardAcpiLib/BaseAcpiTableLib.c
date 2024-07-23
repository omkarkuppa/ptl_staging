/** @file
  Board ACPI Table Library

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

#include <Base.h>
#include <Uefi.h>
#include <Library/BoardAcpiTableLib.h>

/**
  Update ACPI Table

  @param[in, out]  Table    Pointer to table, need to be update in Acpi table.
  @param[in, out]  Version  ACPI table version

  @retval     EFI_SUCCESS   The function always return successfully.
**/
EFI_STATUS
EFIAPI
PtlBoardUpdateAcpiTable (
  IN OUT EFI_ACPI_COMMON_HEADER       *Table,
  IN OUT EFI_ACPI_TABLE_VERSION       *Version
  )
{
  return EFI_SUCCESS;
}

