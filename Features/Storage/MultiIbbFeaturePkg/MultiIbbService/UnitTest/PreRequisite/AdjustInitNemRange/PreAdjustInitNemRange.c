/** @file
  PreRequisite functions of the AdjustInitNemRange function.

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

#include "PreAdjustInitNemRange.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/AccessNemConfigLib.h>

/**
  Set up services for AdjustInitNemRange function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupAdjustInitNemRangeEnvironment (
  VOID
  )
{
  //
  // Global variable initialization
  //
  gReturnResult = AllocateZeroPool (sizeof (EFI_STATUS));
  if (gReturnResult == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *(EFI_STATUS*)gReturnResult = EFI_UNSUPPORTED;

  // Mock function for MtrrGetAllMtrrs
  gRandomMtrrs = AllocateZeroPool (sizeof (MTRR_SETTINGS));
  if (gRandomMtrrs == NULL || !RandomData ((UINT8*)gRandomMtrrs, sizeof (MTRR_SETTINGS), FALSE)) {
    return EFI_OUT_OF_RESOURCES;
  }
  will_return_always (MtrrGetAllMtrrs, gRandomMtrrs);

  // Mock function for PeiServicesLocatePpi
  gIbbHashInfoPpi = AllocateZeroPool (sizeof (PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI));
  if (gIbbHashInfoPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  expect_value (PeiServicesLocatePpi, Guid, &gPeiFirmwareVolumeVerifyHashFvPpiGuid);
  expect_value (PeiServicesLocatePpi, Instance, 0);
  expect_value (PeiServicesLocatePpi, PpiDescriptor, NULL);
  expect_any (PeiServicesLocatePpi, Ppi);
  will_return (PeiServicesLocatePpi, gIbbHashInfoPpi);
  will_return (PeiServicesLocatePpi, EFI_SUCCESS);

  // Mock function for GetVariableMtrrCount
  will_return_always (GetVariableMtrrCount, 8);

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute AdjustInitNemRange function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreAdjustInitNemRange (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupAdjustInitNemRangeEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = AdjustInitNemRange ();

  return UNIT_TEST_PASSED;
}
