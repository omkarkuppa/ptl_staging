/** @file
  PreRequisite functions of the FvIntegrityCheck function.

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

#include "PreFvIntegrityCheck.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

/**
  Set up services for FvIntegrityCheck function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupFvIntegrityCheckEnvironment (
  VOID
  )
{
  UINT8   TestSha384Digest[] = {
            0xF7, 0x25, 0x90, 0x6A, 0xC5, 0xB4, 0xB0, 0x62,
            0x8E, 0x62, 0xD4, 0x2E, 0x9D, 0xF8, 0xB3, 0x58,
            0x64, 0xAE, 0x04, 0xE2, 0x6D, 0x62, 0xB3, 0xE3,
            0xC0, 0xF1, 0xF5, 0x26, 0xF8, 0x4E, 0xE7, 0xB4,
            0xCB, 0x1E, 0xE3, 0x38, 0x5D, 0xBC, 0x08, 0x38,
            0x58, 0x2D, 0x6A, 0x61, 0x5A, 0x9F, 0x00, 0xB5
          };

  //
  // Global variable initialization
  //
  gIbbTestIndex = 0;
  gReturnResult = AllocateZeroPool (sizeof (EFI_STATUS));
  if (gReturnResult == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *(EFI_STATUS*)gReturnResult = EFI_UNSUPPORTED;

  // Mock function for PeiServicesLocatePpi
  if (!GenerateRandomIbbHashInfoPpi (&gIbbHashInfoPpi)) {
    return EFI_LOAD_ERROR;
  }
  expect_value (PeiServicesLocatePpi, Guid, &gPeiFirmwareVolumeVerifyHashFvPpiGuid);
  expect_value (PeiServicesLocatePpi, Instance, 0);
  expect_value (PeiServicesLocatePpi, PpiDescriptor, NULL);
  expect_any (PeiServicesLocatePpi, Ppi);
  will_return (PeiServicesLocatePpi, gIbbHashInfoPpi);
  will_return (PeiServicesLocatePpi, EFI_SUCCESS);

  // Mock function for PeiServicesFfsFindFileByName
  gMockPeiFileHandle = AllocateZeroPool (sizeof (EFI_PEI_FILE_HANDLE));
  if (gMockPeiFileHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  expect_value (PeiServicesFfsFindFileByName, FileName, gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].HashGuid);
  expect_value (PeiServicesFfsFindFileByName, VolumeHandle, FixedPcdGet32 (PcdFlashFvPreMemoryBase));
  will_return (PeiServicesFfsFindFileByName, *gMockPeiFileHandle);
  will_return (PeiServicesFfsFindFileByName, EFI_SUCCESS);

  // Mock function for PeiServicesFfsFindSectionData
  gSha384Digest = AllocateCopyPool (sizeof (TestSha384Digest), TestSha384Digest);
  if (gSha384Digest == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  expect_value (PeiServicesFfsFindSectionData, SectionType, EFI_SECTION_RAW);
  expect_value (PeiServicesFfsFindSectionData, FileHandle, *gMockPeiFileHandle);
  will_return (PeiServicesFfsFindSectionData, gSha384Digest);
  will_return (PeiServicesFfsFindSectionData, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute FvIntegrityCheck function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreFvIntegrityCheck (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupFvIntegrityCheckEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = FvIntegrityCheck (
                                  gIbbTestIndex,
                                  (VOID*)gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].Base,
                                  gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].Length
                                  );

  return UNIT_TEST_PASSED;
}
