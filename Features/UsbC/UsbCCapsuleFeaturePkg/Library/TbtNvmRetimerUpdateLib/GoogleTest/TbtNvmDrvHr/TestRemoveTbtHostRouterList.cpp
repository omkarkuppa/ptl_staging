/** @file
  Google test for RemoveTbtHostRouterList function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

extern "C" {
VOID
RemoveTbtHostRouterList (
  IN TBT_HOST_ROUTER  *TbtHostRouter
  );
}
// **********************************************************
// RemoveTbtHostRouterList Unit Test                        *
// **********************************************************
class RemoveTbtHostRouterListTest : public Test {
  protected:
    TBT_HOST_ROUTER *TbtHostRouter;

  void SetUp() override {
    TbtHostRouter = (TBT_HOST_ROUTER *) AllocateZeroPool (sizeof (TBT_HOST_ROUTER));
  }
};

//
// Case 1 - Correct Flow
//
TEST_F (RemoveTbtHostRouterListTest, Case1) {
  cout << "[---------- Case 1 ----------]"<< endl;

  RemoveTbtHostRouterList (NULL);
}

//
// Case 2 - Correct Flow
//
TEST_F (RemoveTbtHostRouterListTest, Case2) {
  cout << "[---------- Case 2 ----------]"<< endl;

  RemoveTbtHostRouterList (TbtHostRouter);
}
