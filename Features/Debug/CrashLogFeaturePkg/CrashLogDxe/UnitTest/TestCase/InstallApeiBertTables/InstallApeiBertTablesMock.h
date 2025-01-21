/**@file
    Unit test case mock header for InstallApeiBertTables.

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
#ifndef _INSTALL_APEI_BERT_TABLES_MOCK_H_
#define _INSTALL_APEI_BERT_TABLES_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/AcpiTable.h>
#include <IndustryStandard/Acpi.h>

extern EFI_ACPI_TABLE_PROTOCOL *gAcpiTbl;

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
  );

/**
  Unit-Test InstallApeiBertTables CleanUp Function. Clean the input for mock

  @param[in]  Context    Input context for mock.

**/
VOID
EFIAPI
CleanUpInstallApeiBertTables (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Install ACPI table for APEI BERT

  @param[in] AcpiTbl  Acpi table protocol instance.
  @param[in] Bert     APEI BERT Instance.

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InstallApeiBertTables (
  IN EFI_ACPI_TABLE_PROTOCOL                      *AcpiTbl,
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert
  );

/**
  Set BootErrRegionLen in Bert table.

  @param[in] Bert  APEI BERT Instance.

**/
VOID
SetBootErrRegionLen (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert
  );

/**
  Unit test for SetBootErrRegionLen.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSetBootErrRegionLen (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit test for InstallApeiBertTables.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallApeiBertTables (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_INSTALL_APEI_BERT_TABLES_MOCK_H_*/
