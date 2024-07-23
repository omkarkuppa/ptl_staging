/**@file
    Unit test case Prerequisite of InstallApeiBertTables.

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

@par Specification
**/

#include "InstallApeiBertTablesMock.h"

EFI_ACPI_TABLE_PROTOCOL *gAcpiTbl = NULL;

/**

  The Mock of InstallAcpiTable() function.

  @param This                 A pointer to a EFI_ACPI_TABLE_PROTOCOL.

  @param AcpiTableBuffer      A pointer to a buffer containing the
                              ACPI table to be installed.

  @param AcpiTableBufferSize  Specifies the size, in bytes, of
                              the AcpiTableBuffer buffer.


  @param TableKey             Returns a key to refer to the ACPI table.

  @retval EFI_SUCCESS           The table was successfully inserted

  @retval EFI_INVALID_PARAMETER Either AcpiTableBuffer is NULL,
                                TableKey is NULL, or
                                AcpiTableBufferSize and the size
                                field embedded in the ACPI table
                                pointed to by AcpiTableBuffer
                                are not in sync.

  @retval EFI_OUT_OF_RESOURCES  Insufficient resources exist to
                                complete the request.
  @retval EFI_ACCESS_DENIED     The table signature matches a table already
                                present in the system and platform policy
                                does not allow duplicate tables of this type.

**/
EFI_STATUS
InstallAcpiTable_Mock(
  IN   EFI_ACPI_TABLE_PROTOCOL       *This,
  IN   VOID                          *AcpiTableBuffer,
  IN   UINTN                         AcpiTableBufferSize,
  OUT  UINTN                         *TableKey
  )
{
  return EFI_SUCCESS;
}

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreInstallApeiBertTables (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  gAcpiTbl = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  gAcpiTbl->InstallAcpiTable = InstallAcpiTable_Mock;
  return UNIT_TEST_PASSED;
}
