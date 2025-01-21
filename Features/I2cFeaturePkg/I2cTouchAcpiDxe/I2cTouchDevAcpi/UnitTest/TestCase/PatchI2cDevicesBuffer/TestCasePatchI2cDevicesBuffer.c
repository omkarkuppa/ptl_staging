/**@file
  Unit test case of the I2c Touch Device Acpi Driver

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

@par Specification Reference:
**/

#include "TestCasePatchI2cDevicesBuffer.h"

#define UNIT_TEST_FUNCTION_NAME     "UnitTestPatchI2cDevicesBuffer"
#define UNIT_TEST_FUNCTION_VERSION  "0.1"

/**
  Check the result of test case with PatchI2cDevicesBuffer.

  @param[in] Context                   The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED             The result is correct.
  @retval UNIT_TEST_ERROR_TEST_FAILED  The result is wrong.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPatchI2cDevicesBuffer (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT8   *I2cStartPointer;
  UINT8   *I2cEndPointer;
  UINT32  *Signature;
  UINT8   *Operation;
  UINT8   *DataPtr;

  DEBUG ((DEBUG_INFO, "[Test Case] %a v%a\n", UNIT_TEST_FUNCTION_NAME, UNIT_TEST_FUNCTION_VERSION));

  I2cStartPointer = (UINT8 *) gI2cDevicesAcpiTable;
  I2cEndPointer = I2cStartPointer + gI2cDevicesAcpiTable->Length;

  for (; I2cStartPointer <= I2cEndPointer; I2cStartPointer++) {

    Signature = (UINT32 *) (I2cStartPointer + 1);

    switch (*Signature) {
      case (SIGNATURE_32 ('T','P','T','D')):
        if (*I2cStartPointer == AML_NAME_OP) {
          Operation = I2cStartPointer + 8;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 9;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPanelNvsVar.TouchPanelType);
          }
          Operation = I2cStartPointer + 10;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 11;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPanelNvsVar.TouchPanelIrqMode);
          }
          Operation = I2cStartPointer + 12;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 13;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPanelNvsVar.TouchPanelBusAddress);
          }
          Operation = I2cStartPointer + 14;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 15;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPanelNvsVar.TouchPanelSpeed);
          }
          Operation = I2cStartPointer + 16;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 17;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPadNvsVar.TouchPadType);
          }
          Operation = I2cStartPointer + 18;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 19;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPadNvsVar.TouchPadIrqMode);
          }
          Operation = I2cStartPointer + 20;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 21;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPadNvsVar.TouchPadBusAddress);
          }
          Operation = I2cStartPointer + 22;
          if (*Operation == AML_BYTE_PREFIX) {
            DataPtr = I2cStartPointer + 23;
            UT_ASSERT_EQUAL (*DataPtr, gMockI2cTPTD->TouchPadNvsVar.TouchPadSpeed);
          }
        }
        break;

      case (SIGNATURE_32 ('T','D','P','H')):
        if (*I2cStartPointer == AML_NAME_OP) {
          Operation = I2cStartPointer + 8;
          if (*Operation == AML_WORD_PREFIX) {
            DataPtr = I2cStartPointer + 9;
            UT_ASSERT_EQUAL (*(UINT16 *) DataPtr, gMockI2cTPTD->TouchPanelNvsVar.TouchPanelHidAddress);
          }
          Operation = I2cStartPointer + 11;
          if (*Operation == AML_WORD_PREFIX) {
            DataPtr = I2cStartPointer + 12;
            UT_ASSERT_EQUAL (*(UINT16 *) DataPtr, gMockI2cTPTD->TouchPadNvsVar.TouchPadHidAddress);
          }
        }
        break;

      case (SIGNATURE_32 ('T','P','D','I')):
        if (*I2cStartPointer == AML_NAME_OP) {
          Operation = I2cStartPointer + 8;
          if (*Operation == AML_DWORD_PREFIX) {
            DataPtr = I2cStartPointer + 9;
            UT_ASSERT_EQUAL (*(UINT32 *) DataPtr, gMockI2cTPTD->TouchPadNvsVar.TouchpadIrqGpio);
          }
          Operation = I2cStartPointer + 13 ;
          if (*Operation == AML_DWORD_PREFIX) {
            DataPtr = I2cStartPointer + 14;
            UT_ASSERT_EQUAL (*(UINT32 *) DataPtr, gMockI2cTPTD->TouchPanelNvsVar.TouchpanelIrqGpio);
          }
          Operation = I2cStartPointer + 18;
          if (*Operation == AML_DWORD_PREFIX) {
            DataPtr = I2cStartPointer + 19;
            UT_ASSERT_EQUAL (*(UINT32 *) DataPtr, gMockI2cTPTD->TouchPanelNvsVar.Touchpanel0IrqGpio);
          }
        }
        break;

      default:
        break;
    }
  }

  DEBUG ((DEBUG_INFO, "Testing result is pass.\n"));

  return UNIT_TEST_PASSED;
}
