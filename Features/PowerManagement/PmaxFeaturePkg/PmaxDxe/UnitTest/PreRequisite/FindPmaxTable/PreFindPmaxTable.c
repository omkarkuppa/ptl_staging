/**@file

    PreRequisite of the FindPmaxTable function.

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

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Protocol/AcpiTable.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Protocol/FirmwareVolume2.h>

#include "PreFindPmaxTable.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Acpi.h>


EFI_ACPI_DESCRIPTION_HEADER   *gPmaxTable = NULL;
EFI_FIRMWARE_VOLUME2_PROTOCOL *gFwVol = NULL;
/**
  Mock FirmwareVolume2Protocol ReadSection: Locates the requested section within a file and returns it in a buffer.

  ReadSection() is used to retrieve a specific section from a file
  within a firmware volume. The section returned is determined
  using a depth-first, left-to-right search algorithm through all
  sections found in the specified file. The output buffer is specified by a double indirection
  of the Buffer parameter. The input value of Buffer is used to
  determine if the output buffer is caller allocated or is
  dynamically allocated by ReadSection(). If the input value of
  Buffer!=NULL, it indicates that the output buffer is caller
  allocated. In this case, the input value of *BufferSize
  indicates the size of the caller-allocated output buffer. If
  the output buffer is not large enough to contain the entire
  requested output, it is filled up to the point that the output
  buffer is exhausted and EFI_WARN_BUFFER_TOO_SMALL is returned,
  and then BufferSize is returned with the size that is required
  to successfully complete the read. All other
  output parameters are returned with valid values. If the input
  value of *Buffer==NULL, it indicates the output buffer is to
  be allocated by ReadSection(). In this case, ReadSection()
  will allocate an appropriately sized buffer from boot services
  pool memory, which will be returned in *Buffer. The size of
  the new buffer is returned in *BufferSize and all other output
  parameters are returned with valid values. ReadSection() is
  callable only from TPL_NOTIFY and below. Behavior of
  ReadSection() at any EFI_TPL above TPL_NOTIFY is
  undefined.

  @param  This                Indicates the EFI_FIRMWARE_VOLUME2_PROTOCOL instance.

  @param NameGuid             Pointer to an EFI_GUID, which indicates the
                              file name from which the requested section
                              will be read.

  @param SectionType          Indicates the section type to return.
                              SectionType in conjunction with
                              SectionInstance indicates which section to
                              return.

  @param SectionInstance      Indicates which instance of sections
                              with a type of SectionType to return.
                              SectionType in conjunction with
                              SectionInstance indicates which
                              section to return. SectionInstance is
                              zero based.

  @param Buffer               Pointer to a pointer to a buffer in which the
                              section contents are returned, not including
                              the section header.

  @param BufferSize           Pointer to a caller-allocated UINTN. It
                              indicates the size of the memory
                              represented by Buffer.

  @param AuthenticationStatus Pointer to a caller-allocated
                              UINT32 in which the authentication
                              status is returned.


  @retval EFI_SUCCESS   The call completed successfully.

  @retval EFI_WARN_BUFFER_TOO_SMALL   The caller-allocated
                                      buffer is too small to
                                      contain the requested
                                      output. The buffer is
                                      filled and the output is
                                      truncated.

  @retval EFI_OUT_OF_RESOURCES  An allocation failure occurred.

  @retval EFI_NOT_FOUND   The requested file was not found in
                          the firmware volume. EFI_NOT_FOUND The
                          requested section was not found in the
                          specified file.

  @retval EFI_DEVICE_ERROR  A hardware error occurred when
                            attempting to access the firmware
                            volume.

  @retval EFI_ACCESS_DENIED The firmware volume is configured to
                            disallow reads. EFI_PROTOCOL_ERROR
                            The requested section was not found,
                            but the file could not be fully
                            parsed because a required
                            GUIDED_SECTION_EXTRACTION_PROTOCOL
                            was not found. It is possible the
                            requested section exists within the
                            file and could be successfully
                            extracted once the required
                            GUIDED_SECTION_EXTRACTION_PROTOCOL
                            is published.
**/
EFI_STATUS
EFIAPI
MockFwVol2ReadSection (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN EFI_SECTION_TYPE                     SectionType,
  IN UINTN                                SectionInstance,
  IN OUT VOID                             **Buffer,
  IN OUT UINTN                            *BufferSize,
  OUT UINT32                              *AuthenticationStatus
  )
{
  check_expected_ptr (This);
  check_expected_ptr (NameGuid);
  check_expected_ptr (SectionType);
  check_expected_ptr (SectionInstance);
  check_expected_ptr (Buffer);
  check_expected_ptr (BufferSize);
  check_expected_ptr (AuthenticationStatus);

  *Buffer = mock_ptr_type (VOID*);
  *BufferSize = mock_ptr_type (UINTN);

  return mock_type (EFI_STATUS);
}

/**
  Mock FirmwareVolume2Protocol ReadFile: Read data or size of either a File Entry or an Extended File Entry.

  @param[in]      BlockIo         BlockIo interface.
  @param[in]      DiskIo          DiskIo interface.
  @param[in]      Volume          Volume information pointer.
  @param[in]      ParentIcb       Long Allocation Descriptor pointer.
  @param[in]      FileEntryData   FE/EFE structure pointer.
  @param[in, out] ReadFileInfo    Read file information pointer.

  @retval EFI_SUCCESS             Data or size of a FE/EFE was read.
  @retval EFI_OUT_OF_RESOURCES    Data or size of a FE/EFE was not read due to
                                  lack of resources.
  @retval EFI_INVALID_PARAMETER   The read file flag given in ReadFileInfo is
                                  invalid.
  @retval EFI_UNSUPPORTED         The FE recording flag given in FileEntryData
                                  is not supported.
  @retval other                   Data or size of a FE/EFE was not read.
**/
EFI_STATUS
EFIAPI
MockFwVol2ReadFile (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN OUT VOID                             **Buffer,
  IN OUT UINTN                            *BufferSize,
  OUT EFI_FV_FILETYPE                     *FoundType,
  OUT EFI_FV_FILE_ATTRIBUTES              *FileAttributes,
  OUT UINT32                              *AuthenticationStatus
  )
{
  check_expected_ptr (This);
  check_expected_ptr (NameGuid);
  check_expected_ptr (Buffer);
  check_expected_ptr (BufferSize);
  check_expected_ptr (FoundType);
  check_expected_ptr (FileAttributes);
  check_expected_ptr (AuthenticationStatus);

  return mock_type (EFI_STATUS);
}

/**
  Mock BootService HandleProtocol: Queries a handle to determine if it supports a specified protocol.

  @param  UserHandle             The handle being queried.
  @param  Protocol               The published unique identifier of the protocol.
  @param  Interface              Supplies the address where a pointer to the
                                 corresponding Protocol Interface is returned.

  @return The requested protocol interface for the handle
**/
EFI_STATUS
EFIAPI
MockBootServiceHandleProtocol (
  IN EFI_HANDLE  UserHandle,
  IN EFI_GUID    *Protocol,
  OUT VOID       **Interface
  )
{
  check_expected (UserHandle);
  check_expected_ptr (Protocol);
  check_expected_ptr (Interface);

  *Interface = mock_ptr_type (VOID*);

  return mock_type (EFI_STATUS);
}

/**

  Mock BootService LocateHandleBuffer: Function returns an array of handles that support the requested protocol
  in a buffer allocated from pool. This is a version of UnitTestLocateHandle()
  that allocates a buffer for the caller.

  @param  SearchType             Specifies which handle(s) are to be returned.
  @param  Protocol               Provides the protocol to search by.    This
                                 parameter is only valid for SearchType
                                 ByProtocol.
  @param  SearchKey              Supplies the search key depending on the
                                 SearchType.
  @param  NumberHandles          The number of handles returned in Buffer.
  @param  Buffer                 A pointer to the buffer to return the requested
                                 array of  handles that support Protocol.

  @retval EFI_SUCCESS            The result array of handles was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the
                                 matching results.
  @retval EFI_INVALID_PARAMETER  One or more parameters are not valid.

**/
EFI_STATUS
EFIAPI
MockBootServiceLocateHandleBuffer (
  IN EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN EFI_GUID                *Protocol   OPTIONAL,
  IN VOID                    *SearchKey  OPTIONAL,
  IN OUT UINTN               *NumberHandles,
  OUT EFI_HANDLE             **Buffer
  )
{
  check_expected (SearchType);
  check_expected_ptr (Protocol);
  check_expected_ptr (SearchKey);
  check_expected_ptr (NumberHandles);
  check_expected_ptr (Buffer);

  *NumberHandles = mock_type (UINTN);
  *Buffer = mock_ptr_type (VOID*);

  return mock_type (EFI_STATUS);
}

/**

  Set up services for FindPmaxTable function.
  @retval     EFI_SUCCESS                 Environment set up success.

**/

EFI_STATUS
EFIAPI
SetupFindPmaxTableEnvironment (
  )
{
  EFI_STATUS                     Status;
  UINTN                          PmaxBufferSize;
  VOID*                          HandleBuffer;

  //
  // Load Pmax AcpiTables from building file.
  //
  will_return (MockPmaxAcpiTableFile, "Build/PowerManagement/PmaxFeaturePkg/HostTest/NOOPT_VS2019/X64/PowerManagement/PmaxFeaturePkg/PmaxDxe/PmaxDxe/OUTPUT/PmaxDev.aml");
  Status = MockPmaxAcpiTableFile ((UINT8**) &gPmaxTable, &PmaxBufferSize);
  UT_ASSERT_NOT_EFI_ERROR (Status);


  //
  // Mock boot service
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    DEBUG ((DEBUG_ERROR, "Can't locate memory for mock gBS\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  gBS->HandleProtocol     = MockBootServiceHandleProtocol;

  //
  // Mock protocol gEfiFirmwareVolume2ProtocolGuid
  //
  gFwVol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  if (gFwVol == NULL) {
    DEBUG ((DEBUG_ERROR, "Can't locate memory for mock Firmware Volume 2 protocol.\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  gFwVol->ReadFile    = MockFwVol2ReadFile;
  gFwVol->ReadSection = MockFwVol2ReadSection;

  //
  // Mock function for gBS->LocateHandleBuffer
  //
  HandleBuffer = AllocateZeroPool (0x1000);
  if (HandleBuffer == NULL) {
    DEBUG((DEBUG_ERROR, "Can't locate memory for mock HandleBuffer.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  expect_value (MockBootServiceLocateHandleBuffer, SearchType, ByProtocol);
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_value (MockBootServiceLocateHandleBuffer, SearchKey, NULL);
  expect_any (MockBootServiceLocateHandleBuffer, NumberHandles);
  expect_any (MockBootServiceLocateHandleBuffer, Buffer);
  will_return (MockBootServiceLocateHandleBuffer, 0xFF);
  will_return (MockBootServiceLocateHandleBuffer, HandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);
  //
  // Mock function for gBS->HandleProtocol
  //
  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, gFwVol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);
  //
  // Mock function for FwVol->ReadFile
  //
  expect_value (MockFwVol2ReadFile, This, gFwVol);
  expect_value (MockFwVol2ReadFile, NameGuid, &gPmaxAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadFile, Buffer);
  expect_any (MockFwVol2ReadFile, BufferSize);
  expect_any (MockFwVol2ReadFile, FoundType);
  expect_any (MockFwVol2ReadFile, FileAttributes);
  expect_any (MockFwVol2ReadFile, AuthenticationStatus);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);
  //
  // Mock function for FwVol->ReadSection
  //
  expect_value (MockFwVol2ReadSection, This, gFwVol);
  expect_value (MockFwVol2ReadSection, NameGuid, &gPmaxAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadSection, SectionType);
  expect_any (MockFwVol2ReadSection, SectionInstance);
  expect_any (MockFwVol2ReadSection, Buffer);
  expect_any (MockFwVol2ReadSection, BufferSize);
  expect_any (MockFwVol2ReadSection, AuthenticationStatus);
  will_return (MockFwVol2ReadSection, gPmaxTable);
  will_return (MockFwVol2ReadSection, PmaxBufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

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
PreFindPmaxTable(
  IN UNIT_TEST_CONTEXT        Context
  )
{

  EFI_STATUS  Status;

  Status = SetupFindPmaxTableEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  Status = FindPmaxTable (&gPmaxTable);

  return UNIT_TEST_PASSED;
}

/**

  Set up the environment for GetHandlesWithFvProtocol.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.

**/

UNIT_TEST_STATUS
EFIAPI
PreGetHandlesWithFvProtocol(
  IN UNIT_TEST_CONTEXT        Context
  )
{
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  UT_ASSERT_NOT_NULL (gBS);

  return UNIT_TEST_PASSED;
}

/**

  Set up the environment for GetPmaxFirmwareVolum.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.

**/

UNIT_TEST_STATUS
EFIAPI
PreGetPmaxFirmwareVolume (
  IN UNIT_TEST_CONTEXT        Context
  )
{
  gFwVol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  UT_ASSERT_NOT_NULL (gFwVol);

  return UNIT_TEST_PASSED;
}
