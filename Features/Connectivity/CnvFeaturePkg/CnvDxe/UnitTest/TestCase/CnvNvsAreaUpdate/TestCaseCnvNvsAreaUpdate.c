/**@file
  Unit test case of the Cnv Dxe driver.

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

#include "TestCaseCnvNvsAreaUpdate.h"
#include <Protocol/CnvNvsArea.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UnitTestHostBaseLib.h>
#include <Library/DebugLib.h>
#include <CnvBoardConfigPcd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

extern CNV_BOARD_CONFIG       *mCnvDxeBoardConfigPtr;
extern CNV_NVS_AREA_PROTOCOL  mCnvNvsAreaProtocol;

/**
  CNV NVS Area Update
**/
extern
VOID
CnvNvsAreaUpdate (
  VOID
  );

/**
  Unit-Test Function for CnvNvsAreaInit.

  CNV NVS Area Initialize when AllocatePages.

  @param[in]  Context                            An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case is met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCnvNvsAreaUpdate (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT32  ResultDiscreteWlanRfKillGpio;

  mCnvDxeBoardConfigPtr    = AllocateZeroPool (sizeof (CNV_BOARD_CONFIG));
  mCnvNvsAreaProtocol.Area = AllocateZeroPool (sizeof (CNV_PKG_NVS_AREA));

  mCnvDxeBoardConfigPtr->WlanRfKillGpio = 0xFF;

  CnvNvsAreaUpdate ();

  ResultDiscreteWlanRfKillGpio = mCnvNvsAreaProtocol.Area->DiscreteWlanRfKillGpio;

  UT_ASSERT_EQUAL (ResultDiscreteWlanRfKillGpio, 0xFF);
  if (ResultDiscreteWlanRfKillGpio != 0xFF) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  FreePool (mCnvDxeBoardConfigPtr);
  FreePool (mCnvNvsAreaProtocol.Area);

  return UNIT_TEST_PASSED;
}
