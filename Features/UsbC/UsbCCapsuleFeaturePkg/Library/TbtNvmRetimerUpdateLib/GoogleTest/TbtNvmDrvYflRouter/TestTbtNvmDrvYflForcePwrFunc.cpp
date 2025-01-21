/** @file
  Google test for TbtNvmDrvYflForcePwrFunc function.

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

#include <GTestTbtNvmDrvYflRouter.h>
#include <GoogleTest/Library/MockUefiBootServicesTableLib.h>
#include <GoogleTest/Private/MockPciIopProtocolLib/MockPciIopProtocolLib.h>

// **********************************************************
// TbtNvmDrvYflForcePwrFunc Unit Test                       *
// **********************************************************
class TbtNvmDrvYflForcePwrFuncTest : public Test {
  protected:
    TBT_STATUS                   TbtStatus;
    BOOLEAN                      Enable;
    BOOLEAN                      TBTControllerWasPowered;
    EFI_PCI_IO_PROTOCOL          PciIopProtocol;
    UINT32                       RegValue;
    UINT32                       RegValue2;
    UINT32                       RegValue3;
    MockPciIopProtocolLib        PciIoProcotolMock;
    MockUefiBootServicesTableLib UefiBootServicesTableLibMock;

  void SetUp() override {
    std::memcpy (&PciIopProtocol, &localPp, sizeof (EFI_PCI_IO_PROTOCOL));
  }
};

//
// Case 1 - When PciIoProto parameter is NULL
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, PciIoProtoIsNullError) {
  cout << "[---------- Case 1 ----------]"<< endl;
  Enable    = FALSE;
  TbtStatus = TbtNvmDrvYflForcePwrFunc (NULL, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 2 - TBTControllerWasPowered parameter is NULL
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, TBTControllerWasPoweredIsNullError) {
  cout << "[---------- Case 2 ----------]"<< endl;
  Enable    = TRUE;
  TbtStatus = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, NULL);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 3 - Check if TBT Controller was already powered in SW CM mode.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, TBTControllerWasPoweredInTrueSuccess) {
  cout << "[---------- Case 3 ----------]"<< endl;
  Enable                  = TRUE;
  TBTControllerWasPowered = TRUE;
  TbtStatus               = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 4 - Couldn't read from PCIe register.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, PciReadRegError) {
  cout << "[---------- Case 4 ----------]"<< endl;
  Enable                  = TRUE;
  TBTControllerWasPowered = FALSE;
  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read
    )
    .WillOnce (Return (EFI_LOAD_ERROR));
  TbtStatus = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 5 - Check if TBT Controller was already powered.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
// Expected Result - TBTControllerWasPowered will return TRUE
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, FORCE_PWR_REG_BIT_MaskSuccess) {
  cout << "[---------- Case 5 ----------]"<< endl;
  Enable                  = TRUE;
  TBTControllerWasPowered = FALSE;
  RegValue                = 0x02;
  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&RegValue, sizeof (RegValue)),
         Return (EFI_SUCCESS)
         )
       );
  TbtStatus = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
  EXPECT_EQ (TBTControllerWasPowered, TRUE);
}

//
// Case 6 - Couldn't write to PCIe register for enabled force power.
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, PciWriteRegError) {
  cout << "[---------- Case 6 ----------]"<< endl;
  Enable                  = TRUE;
  TBTControllerWasPowered = FALSE;
  RegValue                = 0x00;
  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&RegValue, sizeof (RegValue)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Write
    )
    .WillOnce (Return (EFI_UNSUPPORTED));
  TbtStatus = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}

//
// Case 7 - In case the operation is disable, no need to verify FW is ready.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, TBTControllerWasPoweredDisabledSuccess) {
  cout << "[---------- Case 7 ----------]"<< endl;
  Enable                  = FALSE;
  TBTControllerWasPowered = FALSE;
  RegValue                = 0x00;
  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&RegValue, sizeof (RegValue)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Write (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));
  TbtStatus = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 8 - FW of TBT controller is ready.
// Expected Result - TbtStatus will return TBT_STATUS_SUCCESS
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, FW_Of_TBT_Controller_is_ready) {
  cout << "[---------- Case 8 ----------]"<< endl;
  Enable                  = TRUE;
  TBTControllerWasPowered = FALSE;

  RegValue = 0x00;
  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&RegValue, sizeof (RegValue)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Write (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  RegValue2 = 0x0;
  RegValue3 = 0x80000000;
  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FW_READY_REG_OFFSET,
      1,
      _
      )
    )
    .Times (3)
    .WillOnce (Return (EFI_LOAD_ERROR))
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&RegValue2, sizeof (RegValue2)),
         Return (EFI_SUCCESS)
         )
       )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&RegValue3, sizeof (RegValue3)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    UefiBootServicesTableLibMock,
    gBS_CoreStall (
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  TbtStatus = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_SUCCESS);
}

//
// Case 9 - Waiting too much on power on. Exiting...
// Expected Result - TbtStatus will return TBT_STATUS_NON_RECOVERABLE_ERROR
//
TEST_F (TbtNvmDrvYflForcePwrFuncTest, Waiting_too_much_on_power_on) {
  cout << "[---------- Case 9 ----------]"<< endl;
  Enable                  = TRUE;
  TBTControllerWasPowered = FALSE;

  RegValue = 0x00;
  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (
       DoAll (
         SetArgBuffer<4>(&RegValue, sizeof (RegValue)),
         Return (EFI_SUCCESS)
         )
       );

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Write (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FORCE_PWR_REG_OFFSET,
      1,
      _
      )
    )
    .WillOnce (Return (EFI_SUCCESS));

  EXPECT_CALL (
    PciIoProcotolMock,
    Config_Read (
      &localPp,
      Eq (EfiPciIoWidthUint32),
      FW_READY_REG_OFFSET,
      1,
      _
      )
    )
    .WillRepeatedly (Return (EFI_LOAD_ERROR));

  TbtStatus = TbtNvmDrvYflForcePwrFunc (&localPp, Enable, &TBTControllerWasPowered);
  EXPECT_EQ (TbtStatus, TBT_STATUS_NON_RECOVERABLE_ERROR);
}
