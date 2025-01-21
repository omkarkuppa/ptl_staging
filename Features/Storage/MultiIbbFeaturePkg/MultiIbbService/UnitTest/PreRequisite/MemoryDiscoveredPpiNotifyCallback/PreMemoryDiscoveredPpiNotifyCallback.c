/** @file
  PreRequisite functions of the MemoryDiscoveredPpiNotifyCallback function.

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

#include "PreMemoryDiscoveredPpiNotifyCallback.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

/**
  Set up services for MemoryDiscoveredPpiNotifyCallback function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupMemoryDiscoveredPpiNotifyCallbackEnvironment (
  VOID
  )
{
#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 0
  EFI_FV_INFO    FvImageInfo;
#endif
  UINT8   TestSha384Digest[] = {
            0x0D, 0xAC, 0x9F, 0xD7, 0x62, 0xF4, 0xF4, 0x44,
            0xAB, 0xAE, 0x9C, 0xF4, 0x1A, 0x84, 0x79, 0xA3,
            0x27, 0xDD, 0xAF, 0x8C, 0xC4, 0x9F, 0x3C, 0x16,
            0x00, 0xC2, 0x9D, 0xB9, 0xAD, 0x34, 0x42, 0x04,
            0x33, 0xA1, 0xA8, 0x83, 0xC5, 0x40, 0x55, 0x05,
            0xD2, 0x3E, 0x47, 0xA2, 0x3E, 0xC0, 0xD7, 0x23
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
  gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].CurrentlyCached = FALSE;
  gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].FvInstallNeeded = TRUE;
  expect_value (PeiServicesLocatePpi, Guid, &gPeiFirmwareVolumeVerifyHashFvPpiGuid);
  expect_value (PeiServicesLocatePpi, Instance, 0);
  expect_value (PeiServicesLocatePpi, PpiDescriptor, NULL);
  expect_any (PeiServicesLocatePpi, Ppi);
  will_return (PeiServicesLocatePpi, gIbbHashInfoPpi);
  will_return (PeiServicesLocatePpi, EFI_SUCCESS);

  // FvIntegrity check start
  // Mock function for PeiServicesLocatePpi
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
  // FvIntegrity check end

  gMockVolumeHandle = AllocateZeroPool (sizeof (EFI_PEI_FILE_HANDLE));
  if (gMockVolumeHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 0
  // Mock function for PeiServicesFfsFindNextVolume
  expect_value (PeiServicesFfsFindNextVolume, Instance, 0);
  expect_any (PeiServicesFfsFindNextVolume, VolumeHandle);
  will_return (PeiServicesFfsFindNextVolume, &gMockVolumeHandle);
  will_return (PeiServicesFfsFindNextVolume, EFI_NOT_FOUND);

  // Mock function for PeiServicesFfsGetVolumeInfo
  expect_any (PeiServicesFfsGetVolumeInfo, VolumeHandle);
  will_return (PeiServicesFfsGetVolumeInfo, &FvImageInfo);
  will_return (PeiServicesFfsGetVolumeInfo, EFI_NOT_FOUND);
#endif

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute MemoryDiscoveredPpiNotifyCallback function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreMemoryDiscoveredPpiNotifyCallback (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupMemoryDiscoveredPpiNotifyCallbackEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = MemoryDiscoveredPpiNotifyCallback (NULL, NULL, NULL);

  return UNIT_TEST_PASSED;
}
