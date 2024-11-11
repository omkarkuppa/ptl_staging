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
#include <cmocka.h>

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS)
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <sys/types.h>
#include <sys/stat.h>
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
#include <PrivateMock/PrivateMock.h>
#include <IncludePrivate/FreeSpaceSkipFvCopyLib.h>

#define MAX_PATH 4096

char FvNames[11][50] = {
  "FvAdvanced",
  "FvUefiBoot",
  "FvCryptoPei",
  "FvMicrocode",
  "FvOsBoot",
  "FvPostMemory",
  "FvPreMemory",
  "FvSecurity",
  "FvSetupData",
  "FvTestMenu",
  "FvValidation"
};

UNIT_TEST_STATUS
GetFvPaths( VOID)
{
  int errorno;
  FILE                 *platformstxt;
  char                 platforms[MAX_PATH];
  char                 active_platform[MAX_PATH];
  char                 command[MAX_PATH];

  system("dir /b ..\\..\\..\\..\\*BoardPkg /a:d /O:-D | findstr /v /i \"OpenBoard\"> platforms.txt");

  printf("Supported Platforms:\n");
  SetMem(active_platform, MAX_PATH, 0);
  errorno = fopen_s(&platformstxt, "platforms.txt", "r");
  while ( fgets(platforms, MAX_PATH, platformstxt) != NULL){
    if (active_platform[0] == 0) {
      strcpy_s(active_platform, MAX_PATH, platforms);
      printf("Active Platforms %s\n", platforms);
      break;
    }
  }
  fclose(platformstxt);
  //UT_ASSERT_NOT_EQUAL(active_platform[0], 0);
  printf("Supported Platforms:\n");
  if(active_platform[0]==0) {
    printf("No Platforms found--> Skipping Test. Make sure a platform was built before running this test\n");
    return UNIT_TEST_SKIPPED;
  }
  platforms[strlen(platforms)-1] = '\0';
  // Executing system("dir /s /b ..\\..\\..\\..\\LunarLakeBoardPkg\\Fv*.Fv | findstr /v /i \"Uncompact.fv\">>output1.txt");
  strcpy_s(command, MAX_PATH, "dir /s /b ..\\..\\..\\..\\");
  strcat_s(command, MAX_PATH, platforms);
  strcat_s(command, MAX_PATH, "\\Fv*.Fv | findstr /v /i \"Uncompact.fv\"> FvPaths.txt");
  printf("Executing Command: %s\n", command);
  system(command);
  return UNIT_TEST_PASSED;
}

VOID
DumpFVtoFile(
  char *FvName,
  VOID *FvBuffer,
  UINTN FvLength
  )
{
  FILE                 *FvFile;
  char                 FvPath[MAX_PATH];
  errno_t              error_code;

  strcpy_s(FvPath, MAX_PATH, FvName);
  strcat_s(FvPath, MAX_PATH, ".error");
  error_code = fopen_s(&FvFile, FvPath, "wb");
  fwrite(FvBuffer, 1, FvLength, FvFile);
  fflush(FvFile);
  fclose(FvFile);
}

UNIT_TEST_STATUS
EFIAPI
UnitTestFVCopyCheck(
  char *FvPath
  )
{
  VOID                         *FvBufferOriginal;
  VOID                         *FvBufferCustomCopy;
  UINTN                        FvLength;
  UINTN                        BytesRead;
  FILE                         *FvFile;
  errno_t                      error_code;
  UINTN                        Status;
  EFI_FIRMWARE_VOLUME_HEADER   FwVolHeader;

  printf("Checking FV at %s\n", FvPath);
  error_code = fopen_s(&FvFile, FvPath, "rb");
  //UT_ASSERT_EQUAL (error_code, 0);

  FvLength = fread(&FwVolHeader, 1, sizeof(EFI_FIRMWARE_VOLUME_HEADER), FvFile);
  UT_ASSERT_EQUAL(FvLength, sizeof(EFI_FIRMWARE_VOLUME_HEADER));
  FvLength = FwVolHeader.FvLength;

  fflush(FvFile);
  fseek(FvFile, 0, SEEK_SET);
  //
  // Copy FV to permanent memory to avoid potential TOC/TOU.
  //
  FvBufferOriginal = AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)FvLength));
  SetMem(FvBufferOriginal, FvLength, 0);
  BytesRead = fread(FvBufferOriginal, FvLength, 1, FvFile);
  UT_ASSERT_EQUAL(BytesRead, 1);

  FvBufferCustomCopy = AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)FvLength));
  FreeSpaceSkipFvCopy (FvBufferCustomCopy, FvBufferOriginal, (UINTN)FvLength);

  Status = CompareMem (FvBufferCustomCopy, FvBufferOriginal, FvLength);
  FreePages (FvBufferOriginal, EFI_SIZE_TO_PAGES (FvLength));

  fclose(FvFile);
  if (Status != 0){
    DumpFVtoFile(FvPath, FvBufferCustomCopy, FvLength);
    FreePages (FvBufferCustomCopy, EFI_SIZE_TO_PAGES (FvLength));
    printf("Test Case FAIL. Buffers did not match. Copy Failed\n");
    UT_ASSERT_EQUAL(Status, 0);
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  FreePages (FvBufferCustomCopy, EFI_SIZE_TO_PAGES (FvLength));
  printf("Test Case PASSED\n");
  return UNIT_TEST_PASSED;
}

/**
  Unit test for FirmwareVolumeCopy.
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
UnitTestFirmwareVolumeCopyWithoutSpiDma (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UNIT_TEST_STATUS Status;
  char             FvPath[MAX_PATH];
  int              errorno;
  FILE             *FvPathsTxt;

  Status = GetFvPaths();
  if (GetFvPaths != UNIT_TEST_PASSED) {
    return Status;
  }
  will_return_always(__wrap_PeiServicesLocatePpi, EFI_NOT_FOUND);
  Test_PeiServices();
  errorno = fopen_s(&FvPathsTxt, "FvPaths.txt", "r");
  while ( fgets(FvPath, MAX_PATH, FvPathsTxt) != NULL){
    FvPath[strlen(FvPath)-1] = '\0';
    printf("%s\n", FvPath);
    Status = Status | UnitTestFVCopyCheck(FvPath);
  }
  fclose(FvPathsTxt);
  return Status;
}

