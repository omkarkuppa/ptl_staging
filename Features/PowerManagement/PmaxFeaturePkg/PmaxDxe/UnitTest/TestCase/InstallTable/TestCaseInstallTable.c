/**@file

    TestCase of InstallTable function.

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

#include "TestCaseInstallTable.h"

extern  EFI_ACPI_DESCRIPTION_HEADER* gVerifyTable;

/**

  Unit Test case for FindPmaxTable function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
TestCaseInstallTable (
  IN UNIT_TEST_CONTEXT  Context
  )
{

  UINT8* Buffer;
  EFI_STATUS  Status;

  Status = InstallTable (NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  if (gVerifyTable->OemTableId == SIGNATURE_64 ('P', 'm', 'a', 'x', '_', 'D', 'e', 'v')) {
    DEBUG ((DEBUG_ERROR, "Pmax ACPI Table installed.\n"));
    Buffer = (UINT8*) &gVerifyTable->Signature;
    DEBUG ((DEBUG_ERROR, "Table Signature : %c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3]
      ));
    DEBUG ((DEBUG_ERROR, "Table Length : 0x%X\n", gVerifyTable->Length));
    DEBUG ((DEBUG_ERROR, "Table Revision : 0x%X\n", gVerifyTable->Revision));
    DEBUG ((DEBUG_ERROR, "Table Checksum : 0x%X\n", gVerifyTable->Checksum));
    Buffer = (UINT8*) &gVerifyTable->OemId;
    DEBUG ((DEBUG_ERROR, "Table OemId : %c%c%c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3],
      Buffer[4],
      Buffer[5]
      ));
    Buffer = (UINT8*) &gVerifyTable->OemTableId;
    DEBUG ((DEBUG_ERROR, "Table OemTableId : %c%c%c%c%c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3],
      Buffer[4],
      Buffer[5],
      Buffer[6],
      Buffer[7]
      ));
    DEBUG ((DEBUG_ERROR, "Table OemRevision : 0x%X\n", gVerifyTable->OemRevision));
    DEBUG ((DEBUG_ERROR, "Table CreatorId : 0x%X\n", gVerifyTable->CreatorId));
    DEBUG ((DEBUG_ERROR, "Table CreatorRevision : 0x%X\n", gVerifyTable->CreatorRevision));
  } else {
    DEBUG ((DEBUG_ERROR, "Pmax ACPI Table isn't installed.\n"));
    UT_ASSERT_FALSE (TRUE);
  }

  DEBUG ((DEBUG_ERROR, "[InstallTable] Testing result is pass.\n"));
  return UNIT_TEST_PASSED;
}
