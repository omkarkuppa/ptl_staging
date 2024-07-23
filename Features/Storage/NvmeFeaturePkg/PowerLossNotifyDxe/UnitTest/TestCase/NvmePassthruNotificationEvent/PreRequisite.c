/**@file
    Unit test case Prerequisite of NvmePassthruNotificationEvent.

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

#include "NvmePassthruNotificationEventMock.h"
#include "TestCase/ConfigureDevicePln/ConfigureDevicePlnMock.h"

EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *gPassThru = NULL;

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreNotificationEvent_MockFail (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BOOT_SERVICES  *mBS;

  //
  // Mock boot service
  //
  mBS = BootServiceConstructorMock ();
  mBS->LocateHandle   = MockBootServiceLocateHandle;
  mBS->HandleProtocol = MockBootServiceHandleProtocol;

  will_return (MockBootServiceLocateHandle, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, EFI_NOT_FOUND);
  will_return (MockBootServiceOpenProtocol, NULL);
}

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreNotificationEvent_MockSuccess (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BOOT_SERVICES  *mBS;

  //
  // Mock NvmePassThruProtocol
  //
  gPassThru = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  if (gPassThru == NULL) {
    DEBUG ((DEBUG_ERROR, "PreNotificationEvent_MockSuccess(): Allocate memory for gPassThru failed!!\n"));
    return;
  }
  gPassThru->PassThru         = MockNvmePassThruProtocol_PassThru;
  gPassThru->GetNextNamespace = MockNvmePassThruProtocol_GetNextNamespace;

  //
  // Mock POWER_LOSS_NOTIFY_SETTING_PROTOCOL
  //
  mPlnProtocol = AllocateZeroPool (sizeof (POWER_LOSS_NOTIFY_SETTING_PROTOCOL));
  if (mPlnProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "PreNotificationEvent_MockSuccess(): Allocate memory for mPlnProtocol failed!!\n"));
    return;
  }
  mPlnProtocol->GetPlnSetting    = MockPlnSettingProtocol_GetPlnSetting;
  mPlnProtocol->SetPlnController = MockPlnSettingProtocol_SetPlnController;
  gPlnSetting = PLN_ENABLE;

  //
  // Mock boot service
  //
  mBS = BootServiceConstructorMock ();
  mBS->LocateHandle   = MockBootServiceLocateHandle;
  mBS->HandleProtocol = MockBootServiceHandleProtocol;

  will_return (MockBootServiceLocateHandle, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, gPassThru);
}

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNotificationEvent_Fail_1 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PreRequisiteSetMock (
    NULL,                           // Mock context
    PreNotificationEvent_MockFail,  // Mock function
    CleanUpNotificationEvent,       // CleanUp function
    NULL                            // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNotificationEvent_Fail_2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PreRequisiteSetMock (
    NULL,                              // Mock context
    PreNotificationEvent_MockSuccess,  // Mock function
    CleanUpNotificationEvent,          // CleanUp function
    NULL                               // Expect Output
    );

  //
  // Mock ConfigureDevicePln fail for namespace 0xffffffff and namespace 1
  //
  PreConfigureDevicePln_DisToEnFail (Context);
  PreConfigureDevicePln_DisToEnFail (Context);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNotificationEvent_Success (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PreRequisiteSetMock (
    NULL,                              // Mock context
    PreNotificationEvent_MockSuccess,  // Mock function
    CleanUpNotificationEvent,          // CleanUp function
    NULL                               // Expect Output
    );

  //
  // Mock ConfigureDevicePln success
  //
  PreConfigureDevicePln_DisToEnSuccess (Context);

  return UNIT_TEST_PASSED;
}

