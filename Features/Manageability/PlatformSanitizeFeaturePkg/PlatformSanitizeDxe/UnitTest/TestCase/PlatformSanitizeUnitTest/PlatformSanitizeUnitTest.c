/**@file
    Unit test cases of AddDeviceToTail.

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

#include "PlatformSanitizeMock.h"

GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_PARAMETERS                     *mPsBootParameters;
GLOBAL_REMOVE_IF_UNREFERENCED PS_BOOT_STORAGE_PARAMETERS             mPsBootStorageParameter;

EFI_BOOT_SERVICES    *gBS;
EFI_RUNTIME_SERVICES *gRT;

extern VOID
PsSimulateSanitizeActions (
  VOID
  );

/**
  Unit test for PlatformSanitize.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
PlatformSanitizeUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gRT = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  mPsBootParameters = AllocateZeroPool (sizeof (PS_BOOT_PARAMETERS));
  UT_ASSERT_NOT_NULL (mPsBootParameters);

  //Intialize Boot Parameters
  mPsBootParameters->TriggerSource = BootRpe;
  mPsBootParameters->PsRequestedList |= (PS_ERASE_STORAGE_MEDIA | \
                                        PS_TPM_CLEAR | \
                                        PS_OEM_CUSTOM_ACTION | \
                                        PS_CLEAR_ALL_BIOS_NVM_VARIABLE_REGION | \
                                        PS_BIOS_RELOAD_GOLDEN_CONFIG | \
                                        PS_CSME_UNCONFIGURE \
                                        );
  mPsBootParameters->PsPendingList = mPsBootParameters->PsRequestedList;
  mPsBootParameters->IsSanitizeSummaryDisplayed= FALSE;

  PsSimulateSanitizeActions ();

  FreePool (gBS);
  FreePool (gRT);
  FreePool (mPsBootParameters);

  return UNIT_TEST_PASSED;
}
