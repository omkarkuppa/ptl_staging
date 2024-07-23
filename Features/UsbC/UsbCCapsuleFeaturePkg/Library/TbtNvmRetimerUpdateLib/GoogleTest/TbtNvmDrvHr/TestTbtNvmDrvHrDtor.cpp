/** @file
  Google test for TbtNvmDrvHrDtor function.

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

//**********************************************************
// TbtNvmDrvHrDtor Unit Test                               *
//**********************************************************
class TbtNvmDrvHrDtorTest : public CommonMock {
  protected:
    TBT_HOST_ROUTER       *HrPtr;
    TBT_HR_IMPL           *HrImplPtr;
    EFI_PCI_IO_PROTOCOL   *PciIoProto;

    void SetUp() override {
      PciIoProto               = (EFI_PCI_IO_PROTOCOL *) AllocateZeroPool (sizeof (EFI_PCI_IO_PROTOCOL));
      HrImplPtr                = (TBT_HR_IMPL *) AllocateZeroPool (sizeof (TBT_HR_IMPL));
      HrImplPtr->pPciIoProto   = PciIoProto;
      HrImplPtr->pDma          = gDmaPtrMock;
      HrImplPtr->ForcePwrFunc  = TbtNvmDrvYflForcePwrFunc;
      HrPtr                    = (TBT_HOST_ROUTER *) AllocateZeroPool (sizeof (TBT_HOST_ROUTER));
      HrPtr->Impl              = HrImplPtr;
    }
};

//
// Case 1 - Correct Flow
//
TEST_F (TbtNvmDrvHrDtorTest, CorrectFlow) {
  cout << "[---------- Case 1 ----------]"<< endl;

  //
  //  Mock call TbtNvmDrvYflForcePwrFunc
  //
  EXPECT_CALL (TbtNvmDrvYflRouterMock,
    TbtNvmDrvYflForcePwrFunc (
      HrPtr->Impl->pPciIoProto,
      FALSE,      
      HrPtr->Impl->pDma->TBTControllerWasPowered
    ))
    .WillOnce (Return (TBT_STATUS_SUCCESS));

  //
  //  Mock call TbtNvmDrvDmaDtor
  //
  EXPECT_CALL (TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDtor (
      HrPtr->Impl->pDma
    ));

  TbtNvmDrvHrDtor (HrPtr);
}
