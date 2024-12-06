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

#include <GTestTbtNvmDrvHr.h>
#include <GoogleTest/Private/MockTbtNvmDrvDma/MockTbtNvmDrvDma.h>

extern "C" {
VOID
TbtNvmDrvHrDtor (
  TBT_HOST_ROUTER  *This
  );
}
// **********************************************************
// TbtNvmDrvHrDtor Unit Test                                *
// **********************************************************
class TbtNvmDrvHrDtorTest : public Test {
  protected:
    TBT_HOST_ROUTER     *HrPtr;
    TBT_HR_IMPL         *HrImplPtr;
    EFI_PCI_IO_PROTOCOL *PciIoProto;
    MockTbtNvmDrvDma    TbtNvmDrvDmaMock;
    TBT_DMA             *gDmaPtrMock = &LocalDmaPtr;

  void SetUp() override {
    PciIoProto             = (EFI_PCI_IO_PROTOCOL *) AllocateZeroPool (sizeof (EFI_PCI_IO_PROTOCOL));
    HrImplPtr              = (TBT_HR_IMPL *) AllocateZeroPool (sizeof (TBT_HR_IMPL));
    HrImplPtr->pPciIoProto = PciIoProto;
    HrImplPtr->pDma        = gDmaPtrMock;
    HrPtr                  = (TBT_HOST_ROUTER *) AllocateZeroPool (sizeof (TBT_HOST_ROUTER));
    HrPtr->Impl            = HrImplPtr;
  }
};

//
// Case 1 - This->RefCount > 0 .
// Mock - TbtNvmDrvDmaMock
//
TEST_F (TbtNvmDrvHrDtorTest, Case1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  HrPtr->RefCount = 0x01;

  EXPECT_CALL (
    TbtNvmDrvDmaMock,
    TbtNvmDrvDmaDtor (
      HrPtr->Impl->pDma
      )
    );

  TbtNvmDrvHrDtor (HrPtr);
}

//
// Case 2 - This->RefCount = 0 , Return.
//
TEST_F (TbtNvmDrvHrDtorTest, Case2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  TbtNvmDrvHrDtor (HrPtr);
}
