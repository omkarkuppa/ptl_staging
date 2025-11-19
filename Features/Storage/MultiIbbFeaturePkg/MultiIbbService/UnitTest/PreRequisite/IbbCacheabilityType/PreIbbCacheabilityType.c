/** @file
  PreRequisite functions of the IbbCacheabilityType function.

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

#include "PreIbbCacheabilityType.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/AccessNemConfigLib.h>

/**
  Set up services for IbbCacheabilityType function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupIbbCacheabilityTypeEnvironment (
  VOID
  )
{
  UINT8   TestSha384Digest[] = {
            0x11, 0x3A, 0x55, 0x0D, 0x36, 0x39, 0xCB, 0x50,
            0x3A, 0x99, 0x74, 0xB7, 0x86, 0xF6, 0x37, 0x55,
            0x50, 0x04, 0x21, 0xE6, 0xF9, 0x2E, 0x7C, 0x47,
            0x1E, 0x96, 0xF2, 0x04, 0x85, 0x5A, 0xC2, 0x4E,
            0x1B, 0x31, 0x7E, 0x8B, 0x8F, 0xBF, 0xF1, 0x04,
            0xF6, 0x75, 0x65, 0x77, 0x0B, 0xC5, 0x4A, 0x6A
          };

  //
  // Global variable initialization
  //
  gIbbTestIndex = 2;
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

  // Mock function for MtrrGetAllMtrrs
  gRandomMtrrs = AllocateZeroPool (sizeof (MTRR_SETTINGS));
  if (gRandomMtrrs == NULL || !RandomData ((UINT8*)gRandomMtrrs, sizeof (MTRR_SETTINGS), FALSE)) {
    return EFI_OUT_OF_RESOURCES;
  }
  will_return_always (MtrrGetAllMtrrs, gRandomMtrrs);

  // Mock function for MtrrSetMemoryAttributeInMtrrSettings
  expect_any (MtrrSetMemoryAttributeInMtrrSettings, MtrrSetting);
  expect_any (MtrrSetMemoryAttributeInMtrrSettings, BaseAddress);
  expect_any (MtrrSetMemoryAttributeInMtrrSettings, Length);
  expect_value (MtrrSetMemoryAttributeInMtrrSettings, Attribute, CacheUncacheable);
  will_return (MtrrSetMemoryAttributeInMtrrSettings, gRandomMtrrs);
  will_return (MtrrSetMemoryAttributeInMtrrSettings, RETURN_SUCCESS);

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);

  // Mock function for MtrrGetMemoryAttribute
  expect_value (MtrrGetMemoryAttribute, Address, gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].Base);
  will_return (MtrrGetMemoryAttribute, CacheWriteBack);

  // Mock function for MtrrSetMemoryAttributeInMtrrSettings
  expect_any (MtrrSetMemoryAttributeInMtrrSettings, MtrrSetting);
  expect_any (MtrrSetMemoryAttributeInMtrrSettings, BaseAddress);
  expect_any (MtrrSetMemoryAttributeInMtrrSettings, Length);
  expect_value (MtrrSetMemoryAttributeInMtrrSettings, Attribute, CacheUncacheable);
  will_return (MtrrSetMemoryAttributeInMtrrSettings, gRandomMtrrs);
  will_return (MtrrSetMemoryAttributeInMtrrSettings, RETURN_SUCCESS);

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);

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

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute IbbCacheabilityType function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreIbbCacheabilityType (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupIbbCacheabilityTypeEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = IbbCacheabilityType (gIbbTestIndex, CacheUncacheable);

  return UNIT_TEST_PASSED;
}
