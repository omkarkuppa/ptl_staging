/** @file
  This file provides Fusa Memory Mbist reporting implementation.
  The real test happen within MRC

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/PchInfoLib.h>

#include "PeiFusaPrivateLibInternal.h"
#include "PeiFusaResultReportingLib.h"

STATIC UINTN             mMchBase;

#define MC0_IBECC_MBIST_POST_CTRL_WDB_REG 0x0D490U
#define MC0_IBECC_MBIST_POST_CTRL_RDB_REG 0x0D494U
#define MC1_IBECC_MBIST_POST_CTRL_WDB_REG 0x1D490U
#define MC1_IBECC_MBIST_POST_CTRL_RDB_REG 0x1D494U
#define MC0_CH0_CR_WDB_MBIST_0_REG        0xE4E8U
#define MC0_CH0_CR_WDB_MBIST_1_REG        0xE4ECU
#define MC0_CH1_CR_WDB_MBIST_0_REG        0xECE8U
#define MC0_CH1_CR_WDB_MBIST_1_REG        0xECECU
#define MC1_CH0_CR_WDB_MBIST_0_REG        0x1E4E8U
#define MC1_CH0_CR_WDB_MBIST_1_REG        0x1E4ECU
#define MC1_CH1_CR_WDB_MBIST_0_REG        0x1ECE8U
#define MC1_CH1_CR_WDB_MBIST_1_REG        0x1ECECU
#define MC0_CH0_CR_RDB_MBIST_REG          0x0E4F8U
#define MC0_CH1_CR_RDB_MBIST_REG          0x0ECF8U
#define MC1_CH0_CR_RDB_MBIST_REG          0x1E4F8U
#define MC1_CH1_CR_RDB_MBIST_REG          0x1ECF8U

typedef union {
  struct {
    UINT32  Pass      : 1;
    UINT32  Completed : 1;
    UINT32  Reserved  : 29;
    UINT32  RunBusy   : 1;
  } Field;
  UINT32 Data32;
} MBIST_REG_STRUCT;

UINT8
MbistResultExtract (
  IN UINT32 RegisterOffset
  )
{
  MBIST_REG_STRUCT Reg;
  UINT8 CheckResult;
  Reg.Data32 = MmioRead32 (mMchBase + RegisterOffset);
  DEBUG ((DEBUG_INFO, "Mbist register offset 0x%x, value = 0x%x\n", RegisterOffset, Reg.Data32));

  //imply the register is not accessible
  if (0xFFFFFFFFU == Reg.Data32)
  {
    CheckResult = FUSA_TEST_DEVICE_NOTAVAILABLE;
  }
  else if (1 == Reg.Field.Completed)
  {
    if (1 == Reg.Field.Pass)
    {
      CheckResult = FUSA_TEST_PASS;
    }
    else
    {
      CheckResult = FUSA_TEST_FAIL;
    }
  }
  else
  {
    CheckResult = FUSA_TEST_NOTRUN;
  }

  return CheckResult;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc0MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;
  CheckResult = MbistResultExtract(MC0_CH0_CR_RDB_MBIST_REG);
  LibStatus   = UpdateResults(CheckResult, 0U, pFusaTestResult);
  CheckResult = MbistResultExtract(MC0_CH1_CR_RDB_MBIST_REG);
  LibStatus  |= UpdateResults(CheckResult, 1U, pFusaTestResult);

  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc1MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC1_CH0_CR_RDB_MBIST_REG);
  LibStatus   = UpdateResults(CheckResult, 0U, pFusaTestResult);
  CheckResult = MbistResultExtract(MC1_CH1_CR_RDB_MBIST_REG);
  LibStatus  |= UpdateResults(CheckResult, 1U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc0Ch0MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC0_CH0_CR_WDB_MBIST_0_REG);

  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc0Ch1MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC0_CH0_CR_WDB_MBIST_1_REG);
  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc0Ch2MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC0_CH1_CR_WDB_MBIST_0_REG);

  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc0Ch3MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC0_CH1_CR_WDB_MBIST_1_REG);

  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc1Ch0MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC1_CH0_CR_WDB_MBIST_0_REG);

  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc1Ch1MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC1_CH0_CR_WDB_MBIST_1_REG);

  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc1Ch2MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

  CheckResult = MbistResultExtract(MC1_CH1_CR_WDB_MBIST_0_REG);

  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Mc1Ch3MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;

    CheckResult = MbistResultExtract(MC1_CH1_CR_WDB_MBIST_1_REG);
  
  LibStatus = UpdateResults(CheckResult, 0U, pFusaTestResult);
  return LibStatus;
}

#if 0 //-Werror=unused-function
/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Ibecc0MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;
  CheckResult = MbistResultExtract(MC0_IBECC_MBIST_POST_CTRL_WDB_REG);
  LibStatus   = UpdateResults(CheckResult, 0U, pFusaTestResult);
  CheckResult = MbistResultExtract(MC0_IBECC_MBIST_POST_CTRL_RDB_REG);
  LibStatus  |= UpdateResults(CheckResult, 1U, pFusaTestResult);
  return LibStatus;
}

/**
  Perform the related MBIST result extraction

  @param[in,out] pFusaTestResult - result buffer where the test
       result to be updated to
  @return FUSA_LIB_STATUS
**/
STATIC
FUSA_LIB_STATUS
Ibecc1MbistResultExtract (
  IN OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS LibStatus;
  UINT8           CheckResult;
  CheckResult = MbistResultExtract(MC1_IBECC_MBIST_POST_CTRL_WDB_REG);
  LibStatus   = UpdateResults(CheckResult, 0U, pFusaTestResult);
  CheckResult = MbistResultExtract(MC1_IBECC_MBIST_POST_CTRL_RDB_REG);
  LibStatus  |= UpdateResults(CheckResult, 1U, pFusaTestResult);
  return LibStatus;
}
#endif

/**
  Extract Memory SubSystem MBIST result which resided within
  registers, where the MBIST has been performed within the MRC
  stage and yet for some reason not able to populate the result
  HOB at the point of time.

  @note The test assumes MchBar has been set up with 32 bit
        addressing and being enabled.

  @param[out] pFusaTestResult - pointer to test result
                         buffer for test from
                         FusaTestNumMc0Mbist to
                         FusaTestNumIbecc1Mbist
**/
VOID
FspDxCheckMemoryMbist (
  OUT FUSA_TEST_RESULT *pFusaTestResult
  )
{
  FUSA_LIB_STATUS   LibStatus;

  DEBUG ((DEBUG_INFO, "FspDxCheckMemoryMbist\n"));
  LibStatus = MchBarAddressGet (&mMchBase);
  if (LibStatus == FusaNoError) {
    LibStatus = FusaTestAndReporting (
                  Mc0MbistResultExtract,
                  FusaTestNumMc0Mbist,
                  2U,
                  &(pFusaTestResult[FusaTestNumMc0Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc1MbistResultExtract,
                  FusaTestNumMc1Mbist,
                  2U,
                  &(pFusaTestResult[FusaTestNumMc1Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc0Ch0MbistResultExtract,
                  FusaTestNumMc0Ch0Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch0Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc0Ch1MbistResultExtract,
                  FusaTestNumMc0Ch1Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch1Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc0Ch2MbistResultExtract,
                  FusaTestNumMc0Ch2Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch2Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc0Ch3MbistResultExtract,
                  FusaTestNumMc0Ch3Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch3Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc1Ch0MbistResultExtract,
                  FusaTestNumMc1Ch0Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch0Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc1Ch1MbistResultExtract,
                  FusaTestNumMc1Ch1Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch1Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc1Ch2MbistResultExtract,
                  FusaTestNumMc1Ch2Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch2Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  Mc1Ch3MbistResultExtract,
                  FusaTestNumMc1Ch3Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch3Mbist - FusaTestNumMc0Mbist])
                  );
    ASSERT (LibStatus == FusaNoError);
    for (UINT32 i = 0; i <=  FusaTestNumMc1Ch3Mbist - FusaTestNumMc0Mbist; i++)
    {
      DumpResults (&(pFusaTestResult[i]));
    }
  } else {
    LibStatus = FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc0Mbist,
                  2U,
                  &(pFusaTestResult[FusaTestNumMc0Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc1Mbist,
                  2U,
                  &(pFusaTestResult[FusaTestNumMc1Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc0Ch0Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch0Mbist - FusaTestNumMc0Mbist])
                  );

    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc0Ch1Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch1Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc0Ch2Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch2Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc0Ch3Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc0Ch3Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc1Ch0Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch0Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc1Ch1Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch1Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc1Ch2Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch2Mbist - FusaTestNumMc0Mbist])
                  );
    LibStatus |= FusaTestAndReporting (
                  NULL,
                  FusaTestNumMc1Ch3Mbist,
                  1U,
                  &(pFusaTestResult[FusaTestNumMc1Ch3Mbist - FusaTestNumMc0Mbist])
                  );
    ASSERT (LibStatus == FusaNoError);
    ASSERT (FALSE);
  }
}
