/**@file
    Unit test cases of InitializePrivateData.

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

@par Specification
**/

#include "InsertRecordToBertMock.h"
#include "CrashLogDxe.h"

extern CRASHLOG_DXE_PRIVATE_DATA mCrashlogDxe;

/**
  Unit test for InsertRecordToBert.

  @param[in]  Context  An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInsertRecordToBert (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT32                                       BootErrorRegionSize;
  UINT32                                       TempData1Size;
  UINT32                                       TempData2Size;
  CRASHLOG_HOB_RECORD_ENTRY                    *CrashRecord;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;

  UINT8 TempData1[0x10] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', '1', 0x1, };
  UINT8 TempData2[0x20] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', '2', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, };

  TempData1Size = 0x10;
  TempData2Size = 0x20;

  ZeroMem (&mCrashlogDxe, sizeof(CRASHLOG_DXE_PRIVATE_DATA));
  mCrashlogDxe.HobDataPtr = AllocateZeroPool (sizeof (CRASHLOG_HOB_DATA *) * 2);
  mCrashlogDxe.HobCount = 2;

  mCrashlogDxe.HobDataPtr[0] = AllocateZeroPool (sizeof (CRASHLOG_HOB_DATA) + sizeof (CRASHLOG_HOB_RECORD_ENTRY));
  mCrashlogDxe.HobDataPtr[0]->RecordCount = 1;
  mCrashlogDxe.HobDataPtr[0]->CrashlogAgent = CrashlogAgentCpu;
  CrashRecord = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)mCrashlogDxe.HobDataPtr[0] + sizeof (CRASHLOG_HOB_DATA));
  CrashRecord->Address = (UINT64)TempData1;
  CrashRecord->Size = TempData1Size;

  mCrashlogDxe.HobDataPtr[1] = AllocateZeroPool (sizeof (CRASHLOG_HOB_DATA) + sizeof (CRASHLOG_HOB_RECORD_ENTRY));
  mCrashlogDxe.HobDataPtr[1]->RecordCount = 1;
  mCrashlogDxe.HobDataPtr[1]->CrashlogAgent = CrashlogAgentSoc;
  CrashRecord = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)mCrashlogDxe.HobDataPtr[1] + sizeof (CRASHLOG_HOB_DATA));
  CrashRecord->Address = (UINT64)TempData2;
  CrashRecord->Size = TempData2Size;

  mCrashlogDxe.PayloadCount = 2;
  mCrashlogDxe.PayloadSize  = TempData1Size + TempData2Size;

  BootErrorRegionSize = sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE) + \
                        sizeof (FIRMWARE_GENERIC_ERROR) * mCrashlogDxe.PayloadCount + \
                        mCrashlogDxe.PayloadSize;

  mCrashlogDxe.Bert = AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));

  mCrashlogDxe.Bert->BootErrorRegion       = (UINT64)AllocateZeroPool ((UINTN) BootErrorRegionSize);
  mCrashlogDxe.Bert->BootErrorRegionLength = BootErrorRegionSize;

  InsertRecordToBert ();

  ErrStsBlk = (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)(mCrashlogDxe.Bert->BootErrorRegion);

  UT_ASSERT_EQUAL (ErrStsBlk->DataLength, mCrashlogDxe.PayloadSize + \
                   2 * sizeof (EFI_ACPI_6_5_GENERIC_ERROR_DATA_ENTRY_STRUCTURE)  + \
                   2 * sizeof (EFI_FIRMWARE_ERROR_DATA));
  UT_ASSERT_EQUAL (ErrStsBlk->BlockStatus.ErrorDataEntryCount, 2);

  FreePool (mCrashlogDxe.HobDataPtr[0]);
  FreePool (mCrashlogDxe.HobDataPtr[1]);
  FreePool ((VOID *)(mCrashlogDxe.Bert->BootErrorRegion));
  FreePool (mCrashlogDxe.Bert);

  return UNIT_TEST_PASSED;
}
