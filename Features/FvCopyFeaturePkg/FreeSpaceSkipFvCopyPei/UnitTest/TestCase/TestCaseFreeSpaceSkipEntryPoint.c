/**@file
    Unit test case of the FreeSpaceSkipFvCopyLib PEIM module.

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

**/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <malloc.h>
#include <process.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <cmocka.h>
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PerformanceLib.h>
#include <PiPei.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Guid/FirmwareFileSystem2.h>
#include <TestCaseFirmwareVolumeCopy.h>
#include <Ppi/SpiDma.h>
#include <PrivateMock/PrivateMock.h>
#include <IncludePrivate/FreeSpaceSkipFvCopyLib.h>
#include <Library/PeimEntryPoint.h>

#define MAX_PATH 4096

/**
  Unit test for Free Space Skip Entry Point
  @param[in]  Context    [Optional] An optional parameter that enables:
                         1) test-case reuse with varied parameters and
                         2) test-case re-entry for Target tests that need a
                         reboot.  This parameter is a VOID* and it is the
                         responsibility of the test author to ensure that the
                         contents are well understood by all test cases that may
                         consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestFreeSpaceSkipEntryPointFvShadowPpiandSpiDmaPpiFound (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UNIT_TEST_STATUS Status;
  Status = UNIT_TEST_PASSED;

  // If SPI DMA & FV Shadow is installed
  will_return_always(__wrap_PeiServicesLocatePpi, EFI_SUCCESS);  // SPI DMA Ppi Read return Success
  Test_PeiServices();

//  will_return_always(__wrap_PeiServicesInstallPpi, EFI_SUCCESS);  // SPI DMA Ppi && FVShadow return Success on Install
  FreeSpaceSkipEntryPoint(NULL, NULL);

  UT_ASSERT_EQUAL(FreeSpaceSkipReplacedinFvShadow, TRUE);
  return Status;
}


UNIT_TEST_STATUS
EFIAPI
UnitTestFreeSpaceSkipEntryPointFvShadowPpiNotFoundandSpiDmaPpiFound (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UNIT_TEST_STATUS Status;
  Status = UNIT_TEST_PASSED;

  // If SPI DMA & FV Shadow is installed
  will_return(__wrap_PeiServicesLocatePpi, EFI_SUCCESS);  // SPI DMA Ppi Read return Success
  will_return(__wrap_PeiServicesLocatePpi, EFI_NOT_FOUND);  // FvShadow Ppi return NOT_FOUND
  Test_PeiServices();

  will_return(__wrap_PeiServicesLocatePpi, EFI_SUCCESS);  // SPI DMA Ppi Read return Success
  will_return(__wrap_PeiServicesLocatePpi, EFI_NOT_FOUND);  // FvShadow Ppi return NOT_FOUND

  will_return(__wrap_PeiServicesLocatePpi, EFI_SUCCESS);  // SPI DMA Ppi Read return Success
  will_return(__wrap_PeiServicesLocatePpi, EFI_NOT_FOUND);  // FvShadow Ppi return NOT_FOUND

  will_return_always(__wrap_PeiServicesInstallPpi, EFI_SUCCESS);  // SPI DMA Ppi && FVShadow return Success on Install
  FreeSpaceSkipEntryPoint(NULL, NULL);

  UT_ASSERT_EQUAL(FreeSpaceSkipReplacedinFvShadow, FALSE);
  UT_ASSERT_EQUAL(FvShadowInstalled, TRUE);

  return Status;
}
