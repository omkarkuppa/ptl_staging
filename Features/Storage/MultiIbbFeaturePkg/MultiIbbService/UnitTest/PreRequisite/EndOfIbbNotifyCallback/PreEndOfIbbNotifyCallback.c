/** @file
  PreRequisite functions of the EndOfIbbNotifyCallback function.

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

#include "PreEndOfIbbNotifyCallback.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/AccessNemConfigLib.h>

/**
  Set up services for EndOfIbbNotifyCallback function.

  @retval EFI_SUCCESS  - Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupEndOfIbbNotifyCallbackEnvironment (
  VOID
  )
{
  UINT8   TestSha384Digest[] = {
            0xE0, 0xF2, 0x23, 0x2B, 0x55, 0xE3, 0xBC, 0x12,
            0x0D, 0xAC, 0x1C, 0x91, 0x18, 0xCB, 0xC7, 0x48,
            0x30, 0x06, 0x71, 0x68, 0x39, 0xC2, 0x36, 0x3C,
            0x63, 0x08, 0x13, 0x0F, 0x0C, 0xED, 0x1B, 0x16,
            0xBC, 0xE3, 0x8C, 0xA2, 0x91, 0x7B, 0x39, 0x30,
            0xAD, 0x95, 0xDC, 0x8C, 0xCF, 0x06, 0x6C, 0x7B
          };

  //
  // Global variable initialization
  //
  gIbbTestIndex = 1;
  gReturnResult = AllocateZeroPool (sizeof (EFI_STATUS));
  if (gReturnResult == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *(EFI_STATUS*)gReturnResult = EFI_UNSUPPORTED;

  // Mock function for PeiServicesLocatePpi
  if (!GenerateRandomIbbHashInfoPpi (&gIbbHashInfoPpi)) {
    return EFI_LOAD_ERROR;
  }
  gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].CacheType = CacheWriteBack;
  gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].CurrentlyCached = FALSE;
  gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].FvInstallNeeded = TRUE;
  expect_value (PeiServicesLocatePpi, Guid, &gPeiFirmwareVolumeVerifyHashFvPpiGuid);
  expect_value (PeiServicesLocatePpi, Instance, 0);
  expect_value (PeiServicesLocatePpi, PpiDescriptor, NULL);
  expect_any (PeiServicesLocatePpi, Ppi);
  will_return (PeiServicesLocatePpi, gIbbHashInfoPpi);
  will_return (PeiServicesLocatePpi, EFI_SUCCESS);

  // Mock function for PeiServicesLocatePpi
  expect_value (PeiServicesLocatePpi, Guid, &gPeiFirmwareVolumeVerifyHashFvPpiGuid);
  expect_value (PeiServicesLocatePpi, Instance, 0);
  expect_value (PeiServicesLocatePpi, PpiDescriptor, NULL);
  expect_any (PeiServicesLocatePpi, Ppi);
  will_return (PeiServicesLocatePpi, gIbbHashInfoPpi);
  will_return (PeiServicesLocatePpi, EFI_SUCCESS);

  // Mock function for MtrrGetMemoryAttribute
  expect_value (MtrrGetMemoryAttribute, Address, gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].Base);
  will_return (MtrrGetMemoryAttribute, CacheUncacheable);

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
  expect_value (MtrrSetMemoryAttributeInMtrrSettings, Attribute, CacheWriteBack);
  will_return (MtrrSetMemoryAttributeInMtrrSettings, gRandomMtrrs);
  will_return (MtrrSetMemoryAttributeInMtrrSettings, RETURN_SUCCESS);

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

  // Mock function for PeiServicesInstallFvInfo2Ppi
  expect_value (PeiServicesInstallFvInfo2Ppi, FvFormat, &((EFI_FIRMWARE_VOLUME_HEADER*)gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].Base)->FileSystemGuid);
  expect_value (PeiServicesInstallFvInfo2Ppi, FvInfo, gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].Base);
  expect_value (PeiServicesInstallFvInfo2Ppi, FvInfoSize, gIbbHashInfoPpi->MultiIbbInfo[gIbbTestIndex].Length);
  expect_value (PeiServicesInstallFvInfo2Ppi, ParentFvName, NULL);
  expect_value (PeiServicesInstallFvInfo2Ppi, ParentFileName, NULL);
  expect_value (PeiServicesInstallFvInfo2Ppi, AuthenticationStatus, 0);

#if FixedPcdGetBool (PcdCacheMissDetectEnable) == 0
  // Mock function for PeiServicesFfsFindNextVolume
  expect_value (PeiServicesFfsFindNextVolume, Instance, 0);
  expect_any (PeiServicesFfsFindNextVolume, VolumeHandle);
  will_return (PeiServicesFfsFindNextVolume, NULL);
  will_return (PeiServicesFfsFindNextVolume, EFI_NOT_FOUND);
#endif

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute EndOfIbbNotifyCallback function.

  @param[in]  Context  The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      - Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  - Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreEndOfIbbNotifyCallback (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupEndOfIbbNotifyCallbackEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  *(EFI_STATUS*)gReturnResult = EndOfIbbNotifyCallback (NULL, NULL, NULL);

  return UNIT_TEST_PASSED;
}
