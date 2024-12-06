/** @file
  Google test for CreateHrDevInstance function.

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

#include <GTestTbtRetimerNvmUpdateLib.h>

extern "C" {
#include <TbtNvmRetimer.h>
typedef VOID  *RETIMER_DEV_INSTANCE;
typedef VOID  *HR_DEV_INSTANCE;
}

// **********************************************************
// CreateHrDevInstance Unit Test                            *
// **********************************************************
class CreateHrDevInstanceTest : public Test {
  protected:
    EFI_STATUS           Status;
    UINT32               RetimerIndex;
    UINT32               HrIndex;
    RETIMER_THRU_HR      *RetimerHr;
    RETIMER_THRU_HR      *TargetHr;
    BOOLEAN              NeedUpdated;
    UINT32               *RetimerDeviceInstancesCount;
    UINT32               *HrDeviceInstancesCount;
    HR_DEV_INSTANCE      *HrDeviceInstances;
    RETIMER_DEV_INSTANCE *RetimerDeviceInstances;

  void SetUp() override {
    RetimerDeviceInstancesCount = (UINT32 *) AllocateZeroPool (sizeof (RetimerDeviceInstancesCount));
    HrDeviceInstancesCount      = (UINT32 *) AllocateZeroPool (sizeof (HrDeviceInstancesCount));
    HrDeviceInstances           = (HR_DEV_INSTANCE *) AllocateZeroPool (sizeof (HR_DEV_INSTANCE));
  }
};

//
// Case 1 - RetimerDeviceInstancesCount == 0 , return error.
// Expected Result - will return EFI_UNSUPPORTED.
//
TEST_F (CreateHrDevInstanceTest, Case1) {
  cout << "[---------- Case 1 ----------]"<< endl;
  *RetimerDeviceInstancesCount = 0;

  Status = CreateHrDevInstance (
             RetimerDeviceInstances,
             RetimerDeviceInstancesCount,
             HrDeviceInstances,
             HrDeviceInstancesCount
             );
  EXPECT_EQ (Status, EFI_UNSUPPORTED);
}

//
// Case 2 - HrDeviceInstancesCount != 0 , return error.
// Expected Result - will return EFI_INVALID_PARAMETER.
//
TEST_F (CreateHrDevInstanceTest, Case2) {
  cout << "[---------- Case 2 ----------]"<< endl;
  *RetimerDeviceInstancesCount = 1;
  *HrDeviceInstancesCount      = 1;

  Status = CreateHrDevInstance (
             RetimerDeviceInstances,
             RetimerDeviceInstancesCount,
             HrDeviceInstances,
             HrDeviceInstancesCount
             );
  EXPECT_EQ (Status, EFI_INVALID_PARAMETER);
}
