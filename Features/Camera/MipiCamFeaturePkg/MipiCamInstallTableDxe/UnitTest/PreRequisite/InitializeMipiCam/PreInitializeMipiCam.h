/**@file
  Unit test case of the Mipi Camera Install Table Dxe driver.

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

#ifndef _PRE_INITIALIZE_MIPI_CAM_H_
#define _PRE_INITIALIZE_MIPI_CAM_H_

#include <PrivateMock.h>

/**
  This procedure does all the Mipi Camera initialization and loads the ACPI tables.

  @param[in] ImageHandle  The firmware allocated handle to the Driver Image
  @param[in] SystemTable  Pointer to the EFI System Table

  @retval EFI_SUCCESS     The driver installed/initialized correctly.

**/
EFI_STATUS
EFIAPI
InitializeMipiCam (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Set up services for InitializeMipiCam function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupInitializeMipiCamEnvironment (
  VOID
  );

/**
  Set up the environment and execute InitializeMipiCam function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreInitializeMipiCam (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // _PRE_INITIALIZE_MIPI_CAM_H_
