/**@file
    Unit test cases of Platform Sanitize Simulation.

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

#include "PlatformSanitizeSimulationMock.h"

#define REPORT_DATA_MAX_LENGTH       3000
#define REPORT_TEMP_DATA_MAX_LENGTH  800

EFI_RUNTIME_SERVICES *gRT;

EFI_STATUS
GetTimeMock(
  OUT  EFI_TIME                    *Time,
  OUT  EFI_TIME_CAPABILITIES       *Capabilities OPTIONAL
  )
{
  return EFI_SUCCESS;
}

/**
  Add the header of the report

  @param[in,out] Data  Target data buffer
  @param[in] TempData  Temp buffer
  @param[in] Length    Size in byte of temp buffer

  @return EFI_SUCCESS Add report header successfully.
  @return Others      Fail.

**/
EFI_STATUS
AddReportHeader (
  IN OUT CHAR16  *Data,
  IN CHAR16      *TempData,
  IN UINTN       Length
  );

/**
  Unit test for AddReportHeader.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
AddReportHeaderUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS Status;
  CHAR16     ReportData[REPORT_DATA_MAX_LENGTH] = {0};
  CHAR16     TempData[REPORT_TEMP_DATA_MAX_LENGTH] = {0};

  Status = AddReportHeader (ReportData, TempData, sizeof (CHAR16) * REPORT_TEMP_DATA_MAX_LENGTH);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_NOT_EQUAL (ReportData[0], 0);

  return UNIT_TEST_PASSED;
}

/**
  Add erase report of device

  @param[in,out] Data  Target data buffer
  @param[in] TempData  Temp buffer
  @param[in] Length    Size in byte of temp buffer

  @return EFI_SUCCESS  Add device information to report successfully.
  @return Others       Fail.

**/
EFI_STATUS
AppendSimulationReport (
  IN OUT CHAR16  *Data,
  IN CHAR16      *TempData,
  IN UINTN       Length
  );

/**
  Unit test for AppendSimulationReport.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
AppendSimulationReportUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS Status;
  CHAR16     ReportData[REPORT_DATA_MAX_LENGTH] = {0};
  CHAR16     TempData[REPORT_TEMP_DATA_MAX_LENGTH] = {0};

  gRT = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  gRT->GetTime = GetTimeMock;
  Status = AppendSimulationReport (ReportData, TempData, sizeof (CHAR16) * REPORT_TEMP_DATA_MAX_LENGTH);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_NOT_EQUAL (ReportData[0], 0);
  FreePool (gRT);

  return UNIT_TEST_PASSED;
}

/**
  Add the footer of the report

  @param[in,out] Data  Target data buffer
  @param[in] TempData  Temp buffer
  @param[in] Length    Size in byte of temp buffer

  @return EFI_SUCCESS Add report header successfully.
  @return Others      Fail.

**/
EFI_STATUS
AppendReportFooter (
  IN OUT CHAR16  *Data,
  IN CHAR16      *TempData,
  IN UINTN       Length
  );

/**
  Unit test for AppendReportFooter.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
AppendReportFooterUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS Status;
  CHAR16     ReportData[REPORT_DATA_MAX_LENGTH] = {0};
  CHAR16     TempData[REPORT_TEMP_DATA_MAX_LENGTH] = {0};

  Status = AppendReportFooter (ReportData, TempData, sizeof (CHAR16) * REPORT_TEMP_DATA_MAX_LENGTH);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_NOT_EQUAL (ReportData[0], 0);

  return UNIT_TEST_PASSED;
}
/**
  Create report data with JSON format

  @param[in,out] ReportDoc  Pointer to report document

  @return EFI_SUCCESS Create file and write data status.
  @return Others      Fail.

**/
EFI_STATUS
CreateReport (
  ERASE_DOCUMENTATION * ReportDoc
  );

/**
  Unit test for CreateReport.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
CreateReportUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS Status;
  ERASE_DOCUMENTATION * ReportDoc;

  gRT = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  gRT->GetTime = GetTimeMock;

  ReportDoc = AllocateZeroPool (sizeof (ERASE_DOCUMENTATION));
  if (ReportDoc != NULL) {
    Status = CreateReport (ReportDoc);
    UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
    UT_ASSERT_NOT_EQUAL (ReportDoc->DataSize, 0);
    UT_ASSERT_NOT_EQUAL (ReportDoc->Data, NULL);
  }
  FreePool (gRT);

  return UNIT_TEST_PASSED;
}
