/** @file
    Unit test case of the InitializeDptf function.
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
#include "TestCaseInitializeDptf.h"
#include "PrivateMock.h"

/**
  Verify DptfNvsAreaProtocol Content with DPTF setup data.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyDptfNvsAreaProtocolContent (
  VOID
  )
{
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableDptf, mDptfSetupData.EnableDptf);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableDCFG, mDptfSetupData.EnableDCFG);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->PpccStepSize, mDptfSetupData.PpccStepSize);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableFan1Device, mDptfSetupData.EnableFan1Device);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableFan2Device, mDptfSetupData.EnableFan2Device);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableFan3Device, mDptfSetupData.EnableFan3Device);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableChargerParticipant, mDptfSetupData.EnableChargerParticipant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnablePowerParticipant, mDptfSetupData.EnablePowerParticipant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->PowerParticipantPollingRate, mDptfSetupData.PowerParticipantPollingRate);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableBatteryParticipant, mDptfSetupData.EnableBatteryParticipant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnablePchFivrParticipant, mDptfSetupData.EnablePchFivrParticipant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableSen1Participant, mDptfSetupData.EnableSen1Participant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableSen2Participant, mDptfSetupData.EnableSen2Participant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableSen3Participant, mDptfSetupData.EnableSen3Participant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableSen4Participant, mDptfSetupData.EnableSen4Participant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->EnableSen5Participant, mDptfSetupData.EnableSen5Participant);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->OemDesignVariable0, mDptfSetupData.OemDesignVariable0);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->OemDesignVariable1, mDptfSetupData.OemDesignVariable1);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->OemDesignVariable2, mDptfSetupData.OemDesignVariable2);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->OemDesignVariable3, mDptfSetupData.OemDesignVariable3);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->OemDesignVariable4, mDptfSetupData.OemDesignVariable4);
  UT_ASSERT_EQUAL (mDptfNvsAreaProtocol.Area->OemDesignVariable5, mDptfSetupData.OemDesignVariable5);

  return UNIT_TEST_PASSED;
}

/**
  Verify DPTF SSDT table Content.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyDptfSsdtTableContent (
  VOID
  )
{
  UINT8                          *SsdtPointer;
  UINT32                         *Signature;
  UINT8                          *Operation;
  UINT32                         *Address;
  UINT16                         *Size;

  for (SsdtPointer = (UINT8 *)gSsdtTargetTable;
       SsdtPointer < ((UINT8 *)gSsdtTargetTable + ((EFI_ACPI_COMMON_HEADER *) gSsdtTargetTable)->Length - sizeof (UINT32));
       SsdtPointer++)
  {
    Signature = (UINT32 *) SsdtPointer;
    if (*Signature == SIGNATURE_32 ('D', 'N', 'V', 'S')) {
      Operation = SsdtPointer - 1;
      if (*Operation == AML_EXT_REGION_OP) {
        Address = (UINT32 *) (SsdtPointer + 6);
        UT_ASSERT_EQUAL (*Address, (UINT32)(UINTN) mDptfNvsAreaProtocol.Area);
        Size = (UINT16 *) (SsdtPointer + 11);
        UT_ASSERT_EQUAL (*Size, sizeof (DPTF_NVS_AREA));
      }
      break;
    }
  }
  return UNIT_TEST_PASSED;
}

/**
  Unit Test case for InitializeDptf function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitializeDptf (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Verify returned result.
  // Expected Result: EFI_SUCCESS.
  //
  UT_ASSERT_STATUS_EQUAL (gReturnResult, EFI_SUCCESS);

  //
  // Verify DptfNvsAreaProtocol Content with DPTF setup data.
  // Expected Result: UNIT_TEST_PASSED.
  //
  UT_ASSERT_EQUAL (VerifyDptfNvsAreaProtocolContent (), UNIT_TEST_PASSED);

  //
  // Verify DPTF SSDT table Content.
  // Expected Result: UNIT_TEST_PASSED.
  //
  UT_ASSERT_EQUAL (VerifyDptfSsdtTableContent (), UNIT_TEST_PASSED);

  return UNIT_TEST_PASSED;
}
