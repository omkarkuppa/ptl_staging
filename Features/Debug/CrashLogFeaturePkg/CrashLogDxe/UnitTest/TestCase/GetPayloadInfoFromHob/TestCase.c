/**@file
    Unit test cases of GetPayloadInfoFromHob.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "GetPayloadInfoFromHobMock.h"
#include "CrashLogDxe.h"

extern CRASHLOG_DXE_PRIVATE_DATA  mCrashlogDxe;
extern VOID                       *gHobList;

UINT8 RecordData1[0x100] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', '1', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, };
UINT8 RecordData2[0x200] = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', '2', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, };
UINT8 RecordData3[0x80]  = {'T', 'e', 'm', 'p', 'D', 'a', 't', 'a', '3', 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, };

/**
  The Mock of gCrashLogRecordHobGuid.

**/
VOID *
MockCrashLogRecordHob (
  )
{
  UINT16                     length = 0;
  VOID                       *HobList;
  EFI_HOB_GUID_TYPE          *Hob;
  CRASHLOG_HOB_DATA          *CrashlogHobPtr;
  CRASHLOG_HOB_RECORD_ENTRY  *Record;
  EFI_HOB_GENERIC_HEADER     *HobEnd;

  length = sizeof (EFI_HOB_GUID_TYPE) * 2 + sizeof (CRASHLOG_HOB_DATA) * 2 + sizeof (CRASHLOG_HOB_RECORD_ENTRY) * 3 + sizeof (EFI_HOB_GENERIC_HEADER);
  HobList = AllocateZeroPool (length);

  //
  // First CrashLog HOB (SOC)
  //
  // ------------
  // HOB header
  // ------------
  // GUID
  // ------------
  // CRASHLOG_HOB_DATA
  //   -- CRASHLOG_HOB_RECORD_ENTRY
  //   -- CRASHLOG_HOB_RECORD_ENTRY
  Hob = (EFI_HOB_GUID_TYPE *)HobList;
  Hob->Header.HobType = EFI_HOB_TYPE_GUID_EXTENSION;
  Hob->Header.HobLength = sizeof (EFI_HOB_GUID_TYPE) + sizeof (CRASHLOG_HOB_DATA) + sizeof (CRASHLOG_HOB_RECORD_ENTRY) * 2;
  CopyGuid (&Hob->Name, &gCrashLogRecordHobGuid);
  CrashlogHobPtr = (CRASHLOG_HOB_DATA *)((UINTN)Hob + sizeof (EFI_HOB_GUID_TYPE));
  CrashlogHobPtr->RecordCount = 2;
  CrashlogHobPtr->CrashlogAgent = CrashlogAgentSoc;
  Record = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)Hob + sizeof (EFI_HOB_GUID_TYPE) + sizeof (CRASHLOG_HOB_DATA));
  Record->Address = (UINT64) RecordData1;
  Record->Size = 0x100;
  Record = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)Record + sizeof (CRASHLOG_HOB_RECORD_ENTRY));
  Record->Address = (UINT64) RecordData2;
  Record->Size = 0x200;

  //
  // Second CrashLog HOB (CPU)
  //
  // ------------
  // HOB header
  // ------------
  // GUID
  // ------------
  // CRASHLOG_HOB_DATA
  //   -- CRASHLOG_HOB_RECORD_ENTRY
  Hob = (EFI_HOB_GUID_TYPE *)((UINTN)Hob + Hob->Header.HobLength);
  Hob->Header.HobType = EFI_HOB_TYPE_GUID_EXTENSION;
  Hob->Header.HobLength = sizeof (EFI_HOB_GUID_TYPE) + sizeof (CRASHLOG_HOB_DATA) + sizeof (CRASHLOG_HOB_RECORD_ENTRY);
  CopyGuid (&Hob->Name, &gCrashLogRecordHobGuid);
  CrashlogHobPtr = (CRASHLOG_HOB_DATA *)((UINTN)Hob + sizeof (EFI_HOB_GUID_TYPE));
  CrashlogHobPtr->RecordCount = 1;
  CrashlogHobPtr->CrashlogAgent = CrashlogAgentCpu;
  Record = (CRASHLOG_HOB_RECORD_ENTRY *)((UINTN)Hob + sizeof (EFI_HOB_GUID_TYPE) + sizeof (CRASHLOG_HOB_DATA));
  Record->Address = (UINT64) RecordData3;
  Record->Size = 0x80;

  //
  // Last HOB
  //
  HobEnd = (EFI_HOB_GENERIC_HEADER *)((UINTN)Hob + Hob->Header.HobLength);
  HobEnd->HobType   = EFI_HOB_TYPE_END_OF_HOB_LIST;
  HobEnd->HobLength = (UINT16)sizeof (EFI_HOB_GENERIC_HEADER);
  HobEnd->Reserved  = 0;

  return HobList;
}

/**
  Unit test for GetPayloadInfoFromHob.

  @param[in]  Context  An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetPayloadInfoFromHob (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  gHobList = MockCrashLogRecordHob ();

  GetPayloadInfoFromHob ();

  UT_ASSERT_NOT_NULL (mCrashlogDxe.HobDataPtr);
  UT_ASSERT_EQUAL (mCrashlogDxe.PayloadSize, 0x380);
  UT_ASSERT_EQUAL (mCrashlogDxe.PayloadCount, 3);
  UT_ASSERT_EQUAL (mCrashlogDxe.HobCount, 2);

  FreePool (gHobList);
  return UNIT_TEST_PASSED;
}
