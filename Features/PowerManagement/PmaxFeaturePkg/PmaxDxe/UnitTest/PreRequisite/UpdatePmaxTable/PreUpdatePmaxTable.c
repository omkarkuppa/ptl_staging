/**@file

    PreRequisite of the UpdatePmaxTable function.

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

#include "PreUpdatePmaxTable.h"
#include <PmaxDevDef.h>
#include <Library/MemoryAllocationLib.h>

EFI_ACPI_DESCRIPTION_HEADER  *gPmaxAcpiTable = NULL;
VPD_PMAX_DEV                 *gMockPmaxCfg = NULL;

/**
  Set up services for FindPmaxTable function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupUpdatePmaxTableEnvironment (
  )
{
  EFI_STATUS                     Status;
  UINTN                          PmaxBufferSize;

  //
  // Mock Pmax ACPI table.
  //
  will_return (MockPmaxAcpiTableFile, "Build/PowerManagement/PmaxFeaturePkg/HostTest/NOOPT_VS2019/X64/PowerManagement/PmaxFeaturePkg/PmaxDxe/PmaxDxe/OUTPUT/PmaxDev.aml");
  Status = MockPmaxAcpiTableFile ((UINT8**) &gPmaxAcpiTable, &PmaxBufferSize);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "MockPmaxAcpiTableFile Fail!!\n"));
    UT_ASSERT_FALSE(TRUE);
  }
  //
  // Reserved memory for Mock VpdPcdPmaxDevices.
  //
  gMockPmaxCfg = AllocateZeroPool (sizeof (VPD_PMAX_DEV) * 4);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute FindPmaxTable function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreUpdatePmaxTable (
  IN UNIT_TEST_CONTEXT        Context
  )
{
  EFI_STATUS  Status;

  Status = SetupUpdatePmaxTableEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  return UNIT_TEST_PASSED;
}

/**
  Set up the environment for GetPmaxConfig.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/

UNIT_TEST_STATUS
EFIAPI
PreCheckPmaxConfig (
  IN UNIT_TEST_CONTEXT        Context
  )
{
  gMockPmaxCfg = AllocateZeroPool (sizeof (VPD_PMAX_DEV) * 4);
  UT_ASSERT_NOT_NULL (gMockPmaxCfg);

  return UNIT_TEST_PASSED;
}
