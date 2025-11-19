/** @file
  Unit test case of the PsrGetInfo function.

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

#include "TestCasePsrGetInfo.h"
#include "PrivateMock.h"

/**
  Verify PSR Configuration Content.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyPsrConfigurationContent (
  VOID
  )
{
  UINTN   Index;
  UINT32  TempEvent;

  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.PsrLogState                 , (gRandomPsrData + PSR_LOG_STATE_OFFSET)                                     , sizeof (UINT8));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.PsrVersion.Major            , (gRandomPsrData + PSR_VERISON_OFFSET)                                       , sizeof (UINT16));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.PsrVersion.Minor            , (gRandomPsrData + PSR_VERISON_OFFSET + 2)                                   , sizeof (UINT16));
  UT_ASSERT_MEM_EQUAL (mPsrConfiguration.Psrid                        , (gRandomPsrData + PSRID_OFFSET)                                             , sizeof (UINT8) * PSR_UUID_LENGTH);
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name1                  , (gRandomPsrData + UPID_OFFSET)                                              , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name2                  , (gRandomPsrData + UPID_OFFSET +   8)                                        , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name3                  , (gRandomPsrData + UPID_OFFSET +  16)                                        , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name4                  , (gRandomPsrData + UPID_OFFSET +  24)                                        , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name5                  , (gRandomPsrData + UPID_OFFSET +  32)                                        , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name6                  , (gRandomPsrData + UPID_OFFSET +  40)                                        , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name7                  , (gRandomPsrData + UPID_OFFSET +  48)                                        , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Upid.Name8                  , (gRandomPsrData + UPID_OFFSET +  56)                                        , sizeof (UINT64));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Capabilities                , (gRandomPsrData + PSR_CAPABILITY_MASK_OFFSET)                               , sizeof (UINT8) * PSR_MAX_SUPPORTED_CAPABILITIES);
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.GenesisDate                 , (gRandomPsrData + GENESIS_DATE_OFFSET)                                      , MAX_DATE_LENGTH);
  UT_ASSERT_MEM_EQUAL (mPsrConfiguration.OemInfo                      , (gRandomPsrData + GENESIS_OEM_INFO_OFFSET)                                  , MAX_STRING_LENGTH);
  UT_ASSERT_MEM_EQUAL (mPsrConfiguration.OemMakeInfo                  , (gRandomPsrData + GENESIS_OEM_MAKE_OFFSET)                                  , MAX_STRING_LENGTH);
  UT_ASSERT_MEM_EQUAL (mPsrConfiguration.OemModelInfo                 , (gRandomPsrData + GENESIS_OEM_MODEL_OFFSET)                                 , MAX_STRING_LENGTH);
  UT_ASSERT_MEM_EQUAL (mPsrConfiguration.ManufCOE                     , (gRandomPsrData + GENESIS_MANUFAC_COE_OFFSET)                               , MAX_STRING_LENGTH);
  UT_ASSERT_MEM_EQUAL (mPsrConfiguration.OemDataStore                 , (gRandomPsrData + GENESIS_OEM_DATA_STORE_OFFSET)                            , MAX_STRING_LENGTH * 16);
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.S0RuntimeCtr                , (gRandomPsrData + LEDGER_CTR_S0_OFFSET)                                     , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.S0toS5Ctr                   , (gRandomPsrData + LEDGER_CTR_S5_OFFSET)                                     , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.S0toS4Ctr                   , (gRandomPsrData + LEDGER_CTR_S4_OFFSET)                                     , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.S0toS3Ctr                   , (gRandomPsrData + LEDGER_CTR_S3_OFFSET)                                     , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.WarmResetCtr                , (gRandomPsrData + LEDGER_CTR_WARM_RESET)                                    , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.IshConnectionCtr            , (gRandomPsrData + LEDGER_CTR_ISH_CONNECTION)                                , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.CsmeResetCtr                , (gRandomPsrData + LEDGER_CTR_CSME_RESET)                                    , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.PrtcFailureCtr              , (gRandomPsrData + LEDGER_CTR_PRTC_FAILURE)                                  , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.CsmeInvalidStateCtr         , (gRandomPsrData + LEDGER_CTR_CSME_INVALID_STATE)                            , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.CsmeDamCtr                  , (gRandomPsrData + LEDGER_CTR_CSME_DAM_STATE)                                , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.CsmeUnlockedCtr             , (gRandomPsrData + LEDGER_CTR_CSME_UNLOCK_STATE)                             , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.SvnIncreaseCtr              , (gRandomPsrData + LEDGER_CTR_SVN_INCREASE)                                  , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.ExcessiveShockCtr           , (gRandomPsrData + LEDGER_CTR_EXCESSIVE_SHOCK)                               , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.ExcessiveOperationalTempCtr , (gRandomPsrData + LEDGER_CTR_EXCESSIVE_OPER_TEMP)                           , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.FwRecoveryCtr               , (gRandomPsrData + LEDGER_CTR_FW_RECOVERY)                                   , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.FwUpdateCtr                 , (gRandomPsrData + LEDGER_CTR_FW_UPDATE)                                     , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.SysHangCtr                  , (gRandomPsrData + LEDGER_CTR_SYS_HANG)                                      , sizeof (UINT32));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.PwrDropCtr                  , (gRandomPsrData + LEDGER_CTR_PWR_DROP)                                      , sizeof (UINT32));

  if (*(UINT32*)(gRandomPsrData + EVENT_DATA_TOTAL_COUNT_START_OFFSET) > PSR_MAX_EVENT_COUNT) {
    UT_ASSERT_EQUAL (mPsrConfiguration.TotalEventCount                , PSR_MAX_EVENT_COUNT);
  } else {
    UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.TotalEventCount           , (gRandomPsrData + EVENT_DATA_TOTAL_COUNT_START_OFFSET)                      , sizeof (UINT32));
  }

  for (Index = 0; Index < mPsrConfiguration.TotalEventCount; Index++) {
    UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Events[Index].EventId     , (gRandomPsrData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index))     , sizeof (UINT8));
    if (mPsrConfiguration.Events[Index].EventId == PsrErase) {
      TempEvent = *(UINT32*)(gRandomPsrData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index) + 1);
      UT_ASSERT_EQUAL (mPsrConfiguration.Events[Index].EventStatus    , (UINT8) (TempEvent & 0x01));
      UT_ASSERT_EQUAL (mPsrConfiguration.Events[Index].EventAction    , (UINT8) ((TempEvent & 0x7C) >> 2));
      UT_ASSERT_EQUAL (mPsrConfiguration.Events[Index].EventSource    , (UINT8) ((TempEvent & 0x180) >> 7));
    }
    UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Events[Index].EventTime   , (gRandomPsrData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index) + 4) , MAX_DATE_LENGTH);
    UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.Events[Index].EventData   , (gRandomPsrData + EVENT_LOG_START_OFFSET + (SINGLE_EVENT_SIZE * Index) + 8) , PSR_MAX_EVENT_DATA_LENGTH);
  }

  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.FwVersion.Major             , (gRandomPsrData + CSME_FW_VERSION_OFFSET)                                   , sizeof (UINT16));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.FwVersion.Minor             , (gRandomPsrData + CSME_FW_VERSION_OFFSET + 2)                               , sizeof (UINT16));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.FwVersion.Hotfix            , (gRandomPsrData + CSME_FW_VERSION_OFFSET + 4)                               , sizeof (UINT16));
  UT_ASSERT_MEM_EQUAL (&mPsrConfiguration.FwVersion.Build             , (gRandomPsrData + CSME_FW_VERSION_OFFSET + 6)                               , sizeof (UINT16));

  return UNIT_TEST_PASSED;
}

/**
  Unit Test case for PsrGetInfo function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPsrGetInfo (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Verify returned result.
  // Expected Result: EFI_SUCCESS.
  //
  UT_ASSERT_STATUS_EQUAL (*(EFI_STATUS*)gReturnResult, EFI_SUCCESS);

  //
  // Verify PSR Configuration Content.
  // Expected Result: UNIT_TEST_PASSED.
  //
  UT_ASSERT_EQUAL (VerifyPsrConfigurationContent (), UNIT_TEST_PASSED);

  return UNIT_TEST_PASSED;
}
