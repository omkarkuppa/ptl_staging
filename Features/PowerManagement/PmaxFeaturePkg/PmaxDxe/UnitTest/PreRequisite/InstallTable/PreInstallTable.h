/**@file

    PreRequisite of the InstallTable function.

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

#include <PiPei.h>
#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <PrivateMock.h>

/**

  Set up services for FindPmaxTable function.
  @retval     EFI_SUCCESS                 Environment set up success.

**/
EFI_STATUS
EFIAPI
SetupFindPmaxTableEnvironment (
  );

/**

  Set up the environment and execute FindPmaxTable function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreInstallTable(
  IN UNIT_TEST_CONTEXT        Context
  );

/**

  Find PMAX SSDT Table.

  @param[out] PmaxTable     Point to PMAX table.

  @retval EFI_SUCCESS       Find PMAX table successfully.
  @retval Other             The function encountered an error and could not complete process.

**/
EFI_STATUS
InstallTable(
  OUT EFI_ACPI_DESCRIPTION_HEADER *PmaxTable
  );

/**
  Mock AcpiTableProtocol InstallAcpiTable: The InstallAcpiTable() function allows a caller to install an
  ACPI table. When successful, the table will be linked by the
  RSDT/XSDT. AcpiTableBuffer specifies the table to be installed.
  InstallAcpiTable() will make a copy of the table and insert the
  copy into the RSDT/XSDT. InstallAcpiTable() must insert the new
  table at the end of the RSDT/XSDT. To prevent namespace
  collision, ACPI tables may be created using UEFI ACPI table
  format. If this protocol is used to install a table with a
  signature already present in the system, the new table will not
  replace the existing table. It is a platform implementation
  decision to add a new table with a signature matching an
  existing table or disallow duplicate table signatures and
  return EFI_ACCESS_DENIED. On successful output, TableKey is
  initialized with a unique key. Its value may be used in a
  subsequent call to UninstallAcpiTable to remove an ACPI table.
  If an EFI application is running at the time of this call, the
  relevant EFI_CONFIGURATION_TABLE pointer to the RSDT is no
  longer considered valid.

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
EFIAPI
MockInstallAcpiTable (
  IN EFI_ACPI_TABLE_PROTOCOL  *This,
  IN VOID                     *AcpiTableBuffer,
  IN UINTN                    AcpiTableBufferSize,
  OUT UINTN                   *TableKey
  );