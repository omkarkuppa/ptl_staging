/** @file
  PreRequisite functions of the UpdatePsrCapabilitiesInfo function.

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

#include "PreUpdatePsrCapabilitiesInfo.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include "PrivateMock.h"

#define SUPPORTED_LANGUAGE        "ENG"

EFI_STRING_ID  PsrCapabilityTable[] = {
  STRING_TOKEN (STR_OPTION_PSR_CAP_CHASSIS_INTRUSION_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_EXCESSIVE_OPERATIONAL_TEMPERATURE_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_EXCESSIVE_SHOCK_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_REMOTE_PLATFORM_ERASE_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_LOCAL_PLATFORM_ERASE_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_FW_RECOVERY_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_FW_UPDATE_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_SYS_HANG_VALUE),
  STRING_TOKEN (STR_OPTION_PSR_CAP_POWER_DROP_VALUE),
};

/**
  Set up services for UpdatePsrCapabilitiesInfo function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupUpdatePsrCapabilitiesInfoEnvironment (
  VOID
  )
{
  // Variable initialization
  UINT16          PsrCapability;
  CHAR8           *AsciiStr;
  MOCK_HII_HANDLE *MockHiiHandle;

  // Mock HiiHandle
  MockHiiHandle = AllocateZeroPool (sizeof (MOCK_HII_HANDLE));
  MockHiiHandle->Signature = MOCK_HII_HANDLE_SIGNATURE;
  gMockHiiHandle = (EFI_HII_HANDLE *) MockHiiHandle;

  // Mock boot service
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->LocateProtocol = MockBootServiceLocateProtocol;

  // Mock EFI_HII_STRING_PROTOCOL
  gHiiStringProtocol = AllocateZeroPool (sizeof (EFI_HII_STRING_PROTOCOL));
  if (gHiiStringProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gHiiStringProtocol->SetString = MockHiiSetString;

  // Mock mPsrConfiguration.Capabilities
  mPsrConfiguration.Capabilities[0] = 1;
  mPsrConfiguration.Capabilities[2] = 1;

  for (PsrCapability = 0; PsrCapability < PSR_MAX_SUPPORTED_CAPABILITIES; PsrCapability++) {

    if (PsrCapability >= (sizeof (PsrCapabilityTable) / sizeof (EFI_STRING_ID))) {
      break;
    }

    AsciiStr = AllocateCopyPool (sizeof (SUPPORTED_LANGUAGE), SUPPORTED_LANGUAGE);
    expect_not_value (HiiGetSupportedLanguages, HiiHandle, NULL);
    will_return (HiiGetSupportedLanguages, AsciiStr);

    // Mock gBS->LocateProtocol when initial
    if (PsrCapability == 0) {
      expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiHiiStringProtocolGuid);
      expect_value (MockBootServiceLocateProtocol, Registration, NULL);
      expect_any (MockBootServiceLocateProtocol, Interface);
      will_return (MockBootServiceLocateProtocol, gHiiStringProtocol);
      will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);
    }

    // Mock HiiSetString for EFI_HII_STRING_PROTOCOL
    expect_not_value (MockHiiSetString, PackageList, NULL);
    expect_value (MockHiiSetString, ((MOCK_HII_HANDLE *) PackageList)->Signature, MOCK_HII_HANDLE_SIGNATURE);
    expect_memory (MockHiiSetString, Language, SUPPORTED_LANGUAGE, sizeof (SUPPORTED_LANGUAGE));
    expect_value (MockHiiSetString, StringId, PsrCapabilityTable[PsrCapability]);
    if (mPsrConfiguration.Capabilities[PsrCapability]) {
      expect_memory (MockHiiSetString, String, PSR_CAPABILITIES_SUPPORTED, sizeof (PSR_CAPABILITIES_SUPPORTED));
    } else {
      expect_memory (MockHiiSetString, String, PSR_CAPABILITIES_NOT_SUPPORTED, sizeof (PSR_CAPABILITIES_NOT_SUPPORTED));
    }
    expect_any (MockHiiSetString, StringFontInfo);
    will_return (MockHiiSetString, EFI_SUCCESS);
  }

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute UpdatePsrCapabilitiesInfo function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreUpdatePsrCapabilitiesInfo (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS     Status;

  Status = SetupUpdatePsrCapabilitiesInfoEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  UpdatePsrCapabilitiesInfo (gMockHiiHandle);

  return UNIT_TEST_PASSED;
}
