/**@file
    Unit test cases of InitBertTbl.

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

#include "InitBertTblMock.h"
#include "CrashLogDxe.h"

extern CRASHLOG_DXE_PRIVATE_DATA  mCrashlogDxe;

/**
  Unit test for InitBertTbl.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitBertTbl (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                                   Status;
  CRASHLOG_HOB_RECORD_ENTRY                    *CrashRecord;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;
  UINT8                                        *PayloadAddr;
  UINT32                                       ErrStsHeaderSize;

  //
  // Mock Input
  //
  UINT8 TempData1[0x100] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', '1', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, };
  UINT8 TempData2[0x200] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', '2', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, };
  ZeroMem (&mCrashlogDxe, sizeof(CRASHLOG_DXE_PRIVATE_DATA));
  mCrashlogDxe.HobDataPtr = AllocateZeroPool (sizeof (CRASHLOG_HOB_DATA *) * 2);
  mCrashlogDxe.HobCount = 2;

  mCrashlogDxe.HobDataPtr[0] = AllocateZeroPool (sizeof (CRASHLOG_HOB_DATA) + sizeof (CRASHLOG_HOB_RECORD_ENTRY));
  mCrashlogDxe.HobDataPtr[0]->RecordCount = 1;
  mCrashlogDxe.HobDataPtr[0]->CrashlogAgent = CrashlogAgentCpu;
  CrashRecord = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)mCrashlogDxe.HobDataPtr[0] + sizeof (CRASHLOG_HOB_DATA));
  CrashRecord->Address = (UINT64)TempData1;
  CrashRecord->Size = 0x100;

  mCrashlogDxe.HobDataPtr[1] = AllocateZeroPool (sizeof (CRASHLOG_HOB_DATA) + sizeof (CRASHLOG_HOB_RECORD_ENTRY));
  mCrashlogDxe.HobDataPtr[1]->RecordCount = 1;
  mCrashlogDxe.HobDataPtr[1]->CrashlogAgent = CrashlogAgentSoc;
  CrashRecord = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)mCrashlogDxe.HobDataPtr[1] + sizeof (CRASHLOG_HOB_DATA));
  CrashRecord->Address = (UINT64)TempData2;
  CrashRecord->Size = 0x200;

  mCrashlogDxe.PayloadCount = 2;
  mCrashlogDxe.PayloadSize  = 0x300;

  //
  // Running platform function
  //
  Status = InitBertTbl ();
  ErrStsBlk = (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)mCrashlogDxe.Bert->BootErrorRegion;
  ErrStsHeaderSize = sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE);

  //
  // Check result
  //
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (mCrashlogDxe.Bert->BootErrorRegionLength, ErrStsHeaderSize + 0x300 + 2 * sizeof (FIRMWARE_GENERIC_ERROR));
  PayloadAddr = (UINT8 *)(VOID *)ErrStsBlk + ErrStsHeaderSize + sizeof (FIRMWARE_GENERIC_ERROR);
  UT_ASSERT_MEM_EQUAL (PayloadAddr, TempData1, 0x100);
  PayloadAddr += 0x100 + sizeof (FIRMWARE_GENERIC_ERROR);
  UT_ASSERT_MEM_EQUAL (PayloadAddr, TempData2, 0x200);

  return UNIT_TEST_PASSED;
}
