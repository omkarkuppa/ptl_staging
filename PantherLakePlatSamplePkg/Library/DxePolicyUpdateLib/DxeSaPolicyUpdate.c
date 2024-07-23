/** @file
  This file is the library for SA DXE Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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

#include <DxeSaPolicyUpdate.h>

/**
  Retrieves a custom string for the SMBIOS Type 17 Table DeviceLocator field.

  @param[in]  Controller                    Desired Controller to get a DeviceLocator string for.
  @param[in]  Dimm                          Desired DIMM to get a DeviceLocator string for.
  @param[in]  MdSocket                      0 = Memory Down, 1 = Socketed.

  @retval                                   The DeviceLocator string
  @retval     NULL                          If the return value is NULL, the default value will be used.
**/
CHAR8*
EFIAPI
GetPlatformDeviceLocatorString (
  IN          UINT8               Controller,
  IN          UINT8               Dimm,
  IN          UINT8               MdSocket
  )
{
  return NULL;
}

/**
  Retrieves a custom string for the SMBIOS Type 17 Table BankLocator field.

  @param[in]  Controller                    Desired Controller to get a BankLocator string for.
  @param[in]  Dimm                          Desired DIMM to get a BankLocator string for.
  @param[in]  MdSocket                      0 = Memory Down, 1 = Socketed.

  @retval                                   The BankLocator string
  @retval     NULL                          If the return value is NULL, the default value will be used.
**/
CHAR8*
EFIAPI
GetPlatformBankLocatorString (
  IN          UINT8               Controller,
  IN          UINT8               Dimm,
  IN          UINT8               MdSocket
  )
{
  return NULL;
}

/**
  Get data for platform policy from setup options.

  @param[in] IGpuPolicy                  The pointer to get SA Policy protocol instance

  @retval EFI_SUCCESS                  Operation success.

**/
EFI_STATUS
EFIAPI
UpdateDxeSaPolicy (
  IN OUT  IGPU_POLICY_PROTOCOL    *IGpuPolicy
  )
{
  UINTN                     VariableSize;
  SA_SETUP                  SaSetup;
  SETUP_DATA                SetupVariables;
  EFI_STATUS                Status;
  IGPU_DXE_CONFIG           *IGpuDxeConfig;
  UINT8                     DmarFlags;

  IGpuDxeConfig = NULL;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
  Status = GetConfigBlock ((VOID *)IGpuPolicy, &gIGpuDxeConfigGuid, (VOID *)&IGpuDxeConfig);
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  ZeroMem (&SetupVariables, VariableSize);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupVariables
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  VariableSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SaSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (PcdGetBool (PcdSpdAddressOverride)) {
    PcdSet8S (PcdChannel0Dimm0SpdAddressTable, DIMM_SMB_SPD_P0C0D0_STP);
    PcdSet8S (PcdChannel0Dimm1SpdAddressTable, DIMM_SMB_SPD_P0C0D1_STP);
    PcdSet8S (PcdChannel1Dimm0SpdAddressTable, DIMM_SMB_SPD_P0C1D0_STP);
    PcdSet8S (PcdChannel1Dimm1SpdAddressTable, DIMM_SMB_SPD_P0C1D1_STP);
  }
  PcdSet64S (PcdGetPlatformDeviceLocatorString, (UINT64) &GetPlatformDeviceLocatorString);
  PcdSet64S (PcdGetPlatformBankLocatorString, (UINT64) &GetPlatformBankLocatorString);

  if (!EFI_ERROR(Status)) {
    //
    // Global NVS Graphics configuration
    //
    IGpuDxeConfig->AlsEnable               = SaSetup.AlsEnable;
  }

  DmarFlags = PcdGet8 (PcdDmarFlags);

  if (SaSetup.DmaControlGuarantee) {
    DmarFlags |= BIT2;
  } else {
    DmarFlags &= (UINT8) (~BIT2);
  }

  //
  // X2APIC_OPT_OUT flag automatically becomes the inverse of X2APIC configuration.
  // The flag is "don't care" if VT-d is disabled.
  //
  if (GetApicMode () == LOCAL_APIC_MODE_XAPIC) {
    DmarFlags |= BIT1;
  } else {
    DmarFlags &= (UINT8) (~BIT1);
  }

  PcdSet8S (PcdDmarFlags, DmarFlags);

  //
  // Initialize the Platform dependent Graphics configuration.
  // Set BIT0 & BIT1 if Platform is Connected Standby capable & it's capability field is valid respectively.
  // Please refer to IGD's ACPI Opregion spec for other bit definitions.
  //
  if (SetupVariables.LowPowerS0Idle == 0) {
    IGpuDxeConfig->PlatformConfig |= (BIT1 | BIT0);
  } else {
    IGpuDxeConfig->PlatformConfig &= (UINT32) (~BIT0);
  }

  return EFI_SUCCESS;
}

