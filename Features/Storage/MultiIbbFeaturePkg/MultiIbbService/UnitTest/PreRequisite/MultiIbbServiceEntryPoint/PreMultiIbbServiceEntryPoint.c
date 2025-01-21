/** @file
  PreRequisite functions of the MultiIbbServiceEntryPoint function.

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

#include "PreMultiIbbServiceEntryPoint.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

/**
  Set up services for MultiIbbServiceEntryPoint function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupMultiIbbServiceEntryPointEnvironment (
  VOID
  )
{
  // Mock function for PeiServicesNotifyPpi
  expect_any (PeiServicesNotifyPpi, NotifyList);
  will_return (PeiServicesNotifyPpi, EFI_SUCCESS);

  //
  // Global variable initialization
  //
  gReturnResult = AllocateZeroPool (sizeof (EFI_STATUS));
  if (gReturnResult == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *(EFI_STATUS*)gReturnResult = EFI_UNSUPPORTED;
#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1

  expect_any (LibPcdGetBool, TokenNumber);
  will_return (LibPcdGetBool, TRUE);

  // Mock function for PeiServicesNotifyPpi
  expect_any (PeiServicesNotifyPpi, NotifyList);
  will_return (PeiServicesNotifyPpi, EFI_SUCCESS);

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);
#endif

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute MultiIbbServiceEntryPoint function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreMultiIbbServiceEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupMultiIbbServiceEntryPointEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = MultiIbbServiceEntryPoint (NULL, NULL);

  return UNIT_TEST_PASSED;
}
