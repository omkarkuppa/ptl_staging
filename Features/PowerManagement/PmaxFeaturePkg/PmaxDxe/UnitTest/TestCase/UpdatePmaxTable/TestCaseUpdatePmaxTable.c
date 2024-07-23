/**@file

    Testcase of the UpdatePmax function.

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

#include "TestCaseUpdatePmaxTable.h"
#include <PmaxDevDef.h>
#include <Library/BaseLib.h>

extern  EFI_ACPI_DESCRIPTION_HEADER  *gPmaxAcpiTable;
extern  VPD_PMAX_DEV                 *gMockPmaxCfg;
/**

  Unit Test case for UpdatePmaxTable function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
TestCaseUpdatePmaxTable(
  IN UNIT_TEST_CONTEXT  Context
  )
{

  UINT8*         CurrPtr;
  UINT8*         TableEndPtr;
  UINT32*        NameStrPtr;
  UINT8*         DataPtr;
  UINT16*        PtrBuffer16;
  EFI_STATUS     Status;

  CurrPtr     = NULL;
  TableEndPtr = NULL;
  NameStrPtr  = NULL;
  DataPtr     = NULL;
  PtrBuffer16 = NULL;

  Status = UpdatePmaxTable (NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gPmaxFeaturePkgTokenSpaceGuid_VpdPcdPmaxDevices);
  will_return (LibPcdGetExPtr, NULL);

  Status = UpdatePmaxTable (gPmaxAcpiTable) ;
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);

  gMockPmaxCfg[0].DevD0 = 0xFFFF;
  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gPmaxFeaturePkgTokenSpaceGuid_VpdPcdPmaxDevices);
  will_return (LibPcdGetExPtr, gMockPmaxCfg);

  Status = UpdatePmaxTable (gPmaxAcpiTable) ;
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  ZeroMem (gMockPmaxCfg,(sizeof (VPD_PMAX_DEV) * 4));

  CopyMem (gMockPmaxCfg[0].DevStr, "\\_SB.PC00.HDAS", sizeof("\\_SB.PC00.HDAS"));
  gMockPmaxCfg[0].DevD0 = 0xBB8;
  gMockPmaxCfg[0].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[1].DevStr, "\\_SB.LNK0", sizeof("\\_SB.LNK0"));
  gMockPmaxCfg[1].DevD0 = 0x32A;
  gMockPmaxCfg[1].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[2].DevStr, "\\_SB.LNK1", sizeof("\\_SB.LNK1"));
  gMockPmaxCfg[2].DevD0 = 0x33E;
  gMockPmaxCfg[2].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[3].DevStr, "\\_SB.FLM0", sizeof("\\_SB.FLM0"));
  gMockPmaxCfg[3].DevD0 = 0x2328;
  gMockPmaxCfg[3].DevDx = 0x00;

  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gPmaxFeaturePkgTokenSpaceGuid_VpdPcdPmaxDevices);
  will_return (LibPcdGetExPtr, gMockPmaxCfg);

  Status = UpdatePmaxTable (gPmaxAcpiTable) ;
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  // Got Pmax ACPI table content and length.
  //
  CurrPtr = (UINT8*) gPmaxAcpiTable;
  TableEndPtr = CurrPtr + gPmaxAcpiTable->Length;


  while (CurrPtr < TableEndPtr) {
    CurrPtr++;
    //
    // Find name aml op code
    //
    if ((*CurrPtr) == AML_NAME_OP) {
      //
      // Got signature.
      //
      NameStrPtr = (UINT32 *) (CurrPtr + 1);
      //
      // According to the asl opcode format to get data.
      //
      DataPtr = CurrPtr + 6;

      switch (*NameStrPtr) {
        case (SIGNATURE_32 ('R','T','K','S')):
          //
          // If 'RTKS' device string not expected, then assert and print it.
          //
          if ((AsciiStrCmp (DataPtr, gMockPmaxCfg[0].DevStr)) != 0) {
            DEBUG ((DEBUG_ERROR, "RTKS devie string is not expected!!\n"));
            DEBUG ((DEBUG_ERROR, "[RTKS] Table string = %a\n", DataPtr));
            DEBUG ((DEBUG_ERROR, "[RTKS] Default string = %a\n", gMockPmaxCfg[0].DevStr));
            UT_ASSERT_NOT_EQUAL ((AsciiStrCmp (DataPtr, gMockPmaxCfg[0].DevStr)), 0);
          }
          break;

        case (SIGNATURE_32 ('R','T','K','0')):
          //
          // If 'RTK0' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[0].DevD0) {
            DEBUG ((DEBUG_ERROR, "RTK0 value is not expected!!, [RTK0] DevD0 = 0x%x\n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        case (SIGNATURE_32 ('R','T','K','X')):
          //
          // If 'RTKX' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[0].DevDx) {
            DEBUG ((DEBUG_ERROR,"RTKX value is not expected!!, [RTKX] DevDx = 0x%x\n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        case (SIGNATURE_32 ('W','F','C','S')):
          //
          // If 'WFCS' device string not expected, then assert and print it.
          //
          if ((AsciiStrCmp (DataPtr, gMockPmaxCfg[1].DevStr)) != 0) {
            DEBUG ((DEBUG_ERROR, "WFCS devie string is not expected!!\n"));
            DEBUG ((DEBUG_ERROR, "[WFCS] Table device string   = %a\n", DataPtr));
            DEBUG ((DEBUG_ERROR, "[WFCS] Default device string = %a\n", gMockPmaxCfg[1].DevStr));
            UT_ASSERT_NOT_EQUAL ((AsciiStrCmp (DataPtr, gMockPmaxCfg[1].DevStr)), 0);
          }
          break;

        case (SIGNATURE_32 ('W','F','C','0')):
          //
          // If 'WFC0' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[1].DevD0) {
            DEBUG ((DEBUG_ERROR,"WFC0 value is not expected!!, [WFC0] DevD0 = 0x%x \n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        case (SIGNATURE_32 ('W','F','C','X')):
          //
          // If 'WFCX' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[1].DevDx) {
            DEBUG ((DEBUG_ERROR,"WFCX value is not expected!!, [WFCX] DevDx = 0x%x \n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        case (SIGNATURE_32 ('U','F','C','S')):
          //
          // If 'UFCS' device string not expected, then assert and print it.
          //
          if ((AsciiStrCmp (DataPtr, gMockPmaxCfg[2].DevStr)) != 0) {
            DEBUG ((DEBUG_ERROR, "UFCS devie string is not expected!!\n"));
            DEBUG ((DEBUG_ERROR, "[UFCS] Table device string = %a\n", DataPtr));
            DEBUG ((DEBUG_ERROR, "[UFCS] Default device string = %a\n", gMockPmaxCfg[2].DevStr));
            UT_ASSERT_NOT_EQUAL ((AsciiStrCmp (DataPtr, gMockPmaxCfg[2].DevStr)), 0);
          }
          break;

        case (SIGNATURE_32 ('U','F','C','0')):
          //
          // If 'UFC0' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[2].DevD0) {
            DEBUG ((DEBUG_ERROR,"UFC0 value is not expected!!, [UFC0] DevD0 = 0x%x \n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        case (SIGNATURE_32 ('U','F','C','X')):
          //
          // If 'UFCX' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[2].DevDx) {
            DEBUG ((DEBUG_ERROR, "UFCX value is not expected!!, [UFCX] DevDx = 0x%x \n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        case (SIGNATURE_32 ('F','L','D','S')):
          //
          // If 'FLDS' device string not expected, then assert and print it.
          //
          if ((AsciiStrCmp (DataPtr, gMockPmaxCfg[3].DevStr)) != 0) {
            DEBUG ((DEBUG_ERROR, "FLDS devie string is not expected!!\n"));
            DEBUG ((DEBUG_ERROR,"[FLDS] Table device string   = %a \n", DataPtr));
            DEBUG ((DEBUG_ERROR,"[FLDS] Default device string = %a \n", gMockPmaxCfg[3].DevStr));
            UT_ASSERT_NOT_EQUAL ((AsciiStrCmp (DataPtr, gMockPmaxCfg[3].DevStr)), 0);
          }
          break;

        case (SIGNATURE_32 ('F','L','D','0')):
          //
          // If 'FLD0' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[3].DevD0) {
            DEBUG ((DEBUG_ERROR, "FLD0 value is not expected!!, [FLD0] DevD0 = 0x%x \n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        case (SIGNATURE_32 ('F','L','D','X')):
          //
          // If 'FLDX' value not expected, then assert and print it.
          //
          PtrBuffer16 = (UINT16*)(DataPtr);
          if (*PtrBuffer16 != gMockPmaxCfg[3].DevDx) {
            DEBUG ((DEBUG_ERROR, "FLDX value is not expected!!, [FLDX] DevDx = 0x%x \n", *PtrBuffer16));
            UT_ASSERT_FALSE (TRUE);
          }
          break;

        default:
          break;
      }
    }
  }

  DEBUG ((DEBUG_ERROR, "[UpdatePmaxTable] Testing result is pass.\n"));
  return UNIT_TEST_PASSED;
}

/**

  Unit Test case for GetPmaxConfig function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
TestCaseCheckPmaxConfig(
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS              Status;

// Testcase conditions for invalid device config
  CopyMem (gMockPmaxCfg[0].DevStr, "\\_SB.PC00.HDAS", sizeof("\\_SB.PC00.HDAS"));
  gMockPmaxCfg[0].DevD0 = 0xFFFF;
  gMockPmaxCfg[0].DevDx = 0x00;

  Status = CheckPmaxConfig (gMockPmaxCfg);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  ZeroMem (gMockPmaxCfg,(sizeof (VPD_PMAX_DEV) * 4));

  CopyMem (gMockPmaxCfg[0].DevStr, "\\_SB.PC00.HDAS", sizeof("\\_SB.PC00.HDAS"));
  gMockPmaxCfg[0].DevD0 = 0xBB8;
  gMockPmaxCfg[0].DevDx = 0xFFFF;

  Status = CheckPmaxConfig (gMockPmaxCfg);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  ZeroMem (gMockPmaxCfg,(sizeof (VPD_PMAX_DEV) * 4));

// Testcase condition for only one valid device config out of four device
  CopyMem (gMockPmaxCfg[0].DevStr, "\\_SB.PC00.HDAS", sizeof("\\_SB.PC00.HDAS"));
  gMockPmaxCfg[0].DevD0 = 0xBB8;
  gMockPmaxCfg[0].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[1].DevStr, "", sizeof(""));
  gMockPmaxCfg[1].DevD0 = 0x32A;
  gMockPmaxCfg[1].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[2].DevStr, "", sizeof(""));
  gMockPmaxCfg[2].DevD0 = 0x33E;
  gMockPmaxCfg[2].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[3].DevStr, "", sizeof(""));
  gMockPmaxCfg[3].DevD0 = 0x00;
  gMockPmaxCfg[3].DevDx = 0x00;

  Status = CheckPmaxConfig (gMockPmaxCfg);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  ZeroMem (gMockPmaxCfg,(sizeof (VPD_PMAX_DEV) * 4));

//Testcase condition for valid four device config
  CopyMem (gMockPmaxCfg[0].DevStr, "\\_SB.PC00.HDAS", sizeof("\\_SB.PC00.HDAS"));
  gMockPmaxCfg[0].DevD0 = 0xBB8;
  gMockPmaxCfg[0].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[1].DevStr, "\\_SB.LNK0", sizeof("\\_SB.LNK0"));
  gMockPmaxCfg[1].DevD0 = 0x32A;
  gMockPmaxCfg[1].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[2].DevStr, "\\_SB.LNK1", sizeof("\\_SB.LNK1"));
  gMockPmaxCfg[2].DevD0 = 0x33E;
  gMockPmaxCfg[2].DevDx = 0x00;

  CopyMem (gMockPmaxCfg[3].DevStr, "\\_SB.FLM0", sizeof("\\_SB.FLM0"));
  gMockPmaxCfg[3].DevD0 = 0x2328;
  gMockPmaxCfg[3].DevDx = 0x00;

  Status = CheckPmaxConfig (gMockPmaxCfg);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  return UNIT_TEST_PASSED;
}
