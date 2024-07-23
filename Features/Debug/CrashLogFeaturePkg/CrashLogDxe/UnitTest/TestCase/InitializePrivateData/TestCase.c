/**@file
    Unit test cases of InitializePrivateData.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include "InitializePrivateDataMock.h"
#include "CrashLogDxe.h"

extern EFI_BOOT_SERVICES          *gBS;
extern CRASHLOG_DXE_PRIVATE_DATA  mCrashlogDxe;
/**
  The Mock of LocateProtocol() function.

  Returns the first protocol instance that matches the given protocol.

  @param[in]  Protocol      Provides the protocol to search for.
  @param[in]  Registration  Optional registration key returned from
                            RegisterProtocolNotify().
  @param[out] Interface     On return, a pointer to the first interface that matches Protocol and
                            Registration.

  @retval EFI_SUCCESS            A protocol instance matching Protocol was found and returned in
                                 Interface.
  @retval EFI_NOT_FOUND          No protocol instances were found that match Protocol and
                                 Registration.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.
                                 Protocol is NULL.

**/
EFI_STATUS
EFIAPI
Mock_LocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  )
{
  EFI_STATUS  Status;

  Status     = (EFI_STATUS)mock ();
  *Interface = (EFI_ACPI_TABLE_PROTOCOL *)mock ();

  return Status;
}

/**
  Unit test for InitializePrivateData.

  @param[in]  Context  An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestLocateAcpiProtocol (
  IN UNIT_TEST_CONTEXT  Context
  )
{
    EFI_ACPI_TABLE_PROTOCOL  *AcpiTblProtocol;
    EFI_ACPI_TABLE_PROTOCOL  *ExpectedAcpiTblProtocol;

    gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
    gBS->LocateProtocol = Mock_LocateProtocol;

    ExpectedAcpiTblProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));

    will_return (Mock_LocateProtocol, EFI_NOT_FOUND);
    will_return (Mock_LocateProtocol, ExpectedAcpiTblProtocol);

    AcpiTblProtocol = LocateAcpiProtocol ();

    UT_ASSERT_EQUAL (AcpiTblProtocol, NULL);

    will_return (Mock_LocateProtocol, EFI_SUCCESS);
    will_return (Mock_LocateProtocol, ExpectedAcpiTblProtocol);

    AcpiTblProtocol = LocateAcpiProtocol ();

    UT_ASSERT_EQUAL (AcpiTblProtocol, ExpectedAcpiTblProtocol);

    FreePool (gBS);
    FreePool (ExpectedAcpiTblProtocol);

    return UNIT_TEST_PASSED;
}

/**
  Unit test for InitializePrivateData.

  @param[in]  Context  An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitializePrivateData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ACPI_TABLE_PROTOCOL  *ExpectedAcpiTblProtocol;
  EFI_STATUS               Status;

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->LocateProtocol = Mock_LocateProtocol;

  ExpectedAcpiTblProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));

  will_return (Mock_LocateProtocol, EFI_NOT_FOUND);
  will_return (Mock_LocateProtocol, ExpectedAcpiTblProtocol);

  Status = InitializePrivateData ();

  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  UT_ASSERT_EQUAL (mCrashlogDxe.AcpiTblProtocol, NULL);

  will_return (Mock_LocateProtocol, EFI_SUCCESS);
  will_return (Mock_LocateProtocol, ExpectedAcpiTblProtocol);

  Status = InitializePrivateData ();

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_NOT_NULL (mCrashlogDxe.HobDataPtr);
  UT_ASSERT_EQUAL (mCrashlogDxe.AcpiTblProtocol, ExpectedAcpiTblProtocol);

  FreePool (gBS);
  FreePool (ExpectedAcpiTblProtocol);

  return UNIT_TEST_PASSED;
}
