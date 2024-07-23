/**@file

    Testcase of the FindPmaxTable function.

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
#include "PrivateMock/PrivateMock.h"

/**
  Unit Test case for FindPmaxTable function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseFindPmaxTable (
  IN UNIT_TEST_CONTEXT  Context
  );


/**
  Unit Test case for GetHandlesWithFvProtocol function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseGetHandlesWithFvProtocol (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Get the handles containing FV protocol.

  @param[out] NumberOfHandles       The number of handles returned in Buffer.
  @param[out] HandleBuffer          A pointer to the buffer containing the requested.
                                    array of handles that support the protocol.

  @retval EFI_SUCCESS      The result array of handles was returned.
  @retval Other            The function encountered an error and could not complete process.
**/
EFI_STATUS
GetHandlesWithFvProtocol (
  OUT  UINTN       *NumberOfHandles,
  OUT  EFI_HANDLE  **HandleBuffer
  );

/**
  Unit Test case for GetPmaxFirmwareVolume function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseGetPmaxFirmwareVolume(
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Get Firmware volume contaning the Pmax ACPI file.

  @param[in] NumberOfHandles       The number of handles returned in Buffer.
  @param[in] HandleBuffer          A pointer to the buffer containing the requested.
                                  array of handles that support the protocol.

  @retval FwVol                    Firmware volume contaning the Pmax ACPI file.
**/
EFI_FIRMWARE_VOLUME2_PROTOCOL *
GetPmaxFirmwareVolume (
  IN  UINTN       NumberOfHandles,
  IN  EFI_HANDLE  *HandleBuffer
);

/**
  Unit Test case for ReadPmaxAcpiFile function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseReadPmaxAcpiFile(
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Reads the pmax acpi file and gets the pmax table.

  @param[in] FwVol        Firmware volume contaning the Pmax ACPI file.
  @param[out] PmaxTable   Point to PMAX table.

  @retval EFI_SUCCESS     The function completed successfully.
  @retval Other           The function encountered an error and could not complete process.

**/
EFI_STATUS
ReadPmaxAcpiFile (
IN EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol,
OUT EFI_ACPI_DESCRIPTION_HEADER **PmaxTable
);
