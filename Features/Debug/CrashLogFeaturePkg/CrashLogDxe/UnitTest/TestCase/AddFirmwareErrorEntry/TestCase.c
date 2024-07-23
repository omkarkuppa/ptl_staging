/**@file
    Unit test cases of AddFirmwareErrorEntry.

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

#include "AddFirmwareErrorEntryMock.h"

/**
  Unit test for AddFirmwareErrorEntry.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddFirmwareErrorEntry (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;
  FIRMWARE_GENERIC_ERROR                       *FirmwareGenericErrorAddr;
  UINT8                                        *FirmwareCrashLogPayloadAddr;
  UINT32                                       EntrySize;
  EFI_GUID                                     RecordIdGuid;
  UINT32                                       DataLength;
  UINT32                                       ErrStsHeaderSize;

  EFI_GUID Guid = { 0x8f87f311, 0xc998, 0x4d9e, { 0xa0, 0xc4, 0x60, 0x65, 0x51, 0x8c, 0x4f, 0x6d } };

  //
  // Mock Input
  //
  FirmwareGenericErrorAddr    = NULL;
  FirmwareCrashLogPayloadAddr = NULL;
  EntrySize        = 0x100;
  RecordIdGuid     = Guid;
  DataLength       = 0x20 + EntrySize + sizeof (FIRMWARE_GENERIC_ERROR); // Orign size + Entry size + header size
  ErrStsHeaderSize = sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE);
  ErrStsBlk = AllocateZeroPool (ErrStsHeaderSize + DataLength);
  ErrStsBlk->DataLength = 0x20;
  ErrStsBlk->BlockStatus.ErrorDataEntryCount = 1;

  //
  // Running platform function
  //
  AddFirmwareErrorEntry (ErrStsBlk, &FirmwareGenericErrorAddr, &FirmwareCrashLogPayloadAddr, EntrySize, RecordIdGuid);

  //
  // Check result
  //
  UT_ASSERT_MEM_EQUAL (&FirmwareGenericErrorAddr->EfiFirmwareErrorData.RecordIdGuid, &Guid, sizeof(EFI_GUID));
  UT_ASSERT_EQUAL ((UINT8 *)(VOID *)ErrStsBlk + ErrStsHeaderSize + 0x20, FirmwareGenericErrorAddr);
  UT_ASSERT_EQUAL ((UINT8 *)(VOID *)ErrStsBlk + ErrStsHeaderSize + 0x20 + sizeof (FIRMWARE_GENERIC_ERROR) , FirmwareCrashLogPayloadAddr);
  UT_ASSERT_EQUAL (ErrStsBlk->DataLength, DataLength);
  UT_ASSERT_EQUAL (ErrStsBlk->BlockStatus.ErrorDataEntryCount, 2);
  UT_ASSERT_EQUAL (FirmwareGenericErrorAddr->GenericErrorDataEntry.ErrorDataLength, EntrySize + sizeof (EFI_FIRMWARE_ERROR_DATA));
  UT_ASSERT_EQUAL (FirmwareGenericErrorAddr->GenericErrorDataEntry.ErrorSeverity, 1);
  UT_ASSERT_EQUAL (FirmwareGenericErrorAddr->GenericErrorDataEntry.Revision, 0x0300);
  UT_ASSERT_EQUAL (FirmwareGenericErrorAddr->GenericErrorDataEntry.ValidationBits, 0);
  UT_ASSERT_EQUAL (FirmwareGenericErrorAddr->EfiFirmwareErrorData.ErrorType, 2);
  UT_ASSERT_EQUAL (FirmwareGenericErrorAddr->EfiFirmwareErrorData.Revision, 2);

  FreePool (ErrStsBlk);
  return UNIT_TEST_PASSED;
}
