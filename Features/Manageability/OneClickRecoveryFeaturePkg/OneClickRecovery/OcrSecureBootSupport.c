/** @file
  Intel One Click Recovery Secure Boot Support functional implementation.

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

@par Specification Reference:
**/

#include "OcrSecureBootSupport.h"
#include <OneClickRecovery.h>
#include "OcrBootOptionSupport.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/AsfProtocol.h>
#include <Guid/AuthenticatedVariableFormat.h>

//
// One click Recovery Boot Settings GUID
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID  mOcrBootSettingsGuid = OCR_BOOT_SETTINGS_GUID;

/**
  This routine disables Secure Boot if requested for One Click Recovery.

  @param[in] AsfBootOptions ASF provided boot command options

  @retval EFI_SUCCESS       Successfully disabled secure boot
  @retval EFI_ABORTED       Unable to get Secure Boot EFI Variable
  @retval Other             Another error occurred
**/
EFI_STATUS
OcrDisableSecureBootState (
  IN ASF_BOOT_OPTIONS       AsfBootOptions
  )
{
  EFI_STATUS                Status;
  UINTN                     VarSize;
  UINT32                    VarAttributes;
  OCR_CONFIG                OcrConfig;
  UINT8                     SecureBootState;
  OCR_BOOT_SETTINGS         OcrBootSettings;
  BOOLEAN                   EnforceSecureBoot;
  BOOLEAN                   OcrHttpsBoot;
  ONE_CLICK_RECOVERY_CAP    OcrCap;

  OcrCap = OcrGetCapabilities ();
  OcrBootSettings   = GetOcrBootSettings ();
  EnforceSecureBoot = (AsfBootOptions.SpecialCommandParamLowByte & ENFORCE_SECURE_BOOT) == ENFORCE_SECURE_BOOT;
  OcrHttpsBoot      = AsfBootOptions.SpecialCommand == ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD;

  VarSize = sizeof (OCR_CONFIG);
  Status = gRT->GetVariable (
                OCR_CONFIG_VARIABLE_NAME,
                &gOcrConfigVariableGuid,
                NULL,
                &VarSize,
                &OcrConfig
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  VarSize = sizeof (UINT8);
  SecureBootState = SECURE_BOOT_DISABLE;
  Status = gRT->GetVariable (
                  EFI_SECURE_BOOT_ENABLE_NAME,
                  &gEfiSecureBootEnableDisableGuid,
                  &VarAttributes,
                  &VarSize,
                  &SecureBootState
                  );
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  if (OcrCap.Bits.OcrAmtDisSecBoot &&
      OcrConfig.OcrConfigAmtDisSecBoot == 1 &&
      SecureBootState == SECURE_BOOT_ENABLE &&
      OcrHttpsBoot &&
      !EnforceSecureBoot) {
    // Secure Boot needs to be disabled if were doing One Click Recovery HTTPS Boot and Enforce Secure Boot is not set
    SecureBootState = SECURE_BOOT_DISABLE;

    Status = gRT->SetVariable (
                    EFI_SECURE_BOOT_ENABLE_NAME,
                    &gEfiSecureBootEnableDisableGuid,
                    VarAttributes,
                    sizeof (UINT8),
                    &SecureBootState
                    );
    ASSERT_EFI_ERROR (Status);

    // Set variable to restore previous secure boot state
    OcrBootSettings.RestoreSecureBootSetting = RESTORE_SECURE_BOOT_ENABLED;
    Status = gRT->SetVariable (
                  OCR_BOOT_SETTINGS_NAME,
                  &mOcrBootSettingsGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (OCR_BOOT_SETTINGS),
                  &OcrBootSettings
                  );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "Secure Boot disabled for OCR HTTPS Boot - Cold Reset!\n"));
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  return EFI_SUCCESS;
}

/**
  This routine restores Secure Boot for One Click Recovery.

  @retval EFI_SUCCESS     Successfully restored secure boot
  @retval EFI_ABORTED     Unable to get Secure Boot EFI Variable
**/
EFI_STATUS
OcrRestoreSecureBootState (
  VOID
  )
{
  EFI_STATUS           Status;
  UINTN                VarSize;
  UINT32               VarAttributes;
  UINT8                SecureBootState;
  OCR_BOOT_SETTINGS    OcrBootSettings;
  BOOLEAN              EnforceSecureBoot;
  BOOLEAN              OcrHttpsBoot;

  VarSize           = sizeof (UINT8);
  OcrBootSettings   = GetOcrBootSettings ();
  EnforceSecureBoot = (AsfGetSpecialCmdParamLowByte () & ENFORCE_SECURE_BOOT) == ENFORCE_SECURE_BOOT;
  OcrHttpsBoot      = (AsfGetSpecialCommand () == ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD);

  Status = gRT->GetVariable (
                  EFI_SECURE_BOOT_ENABLE_NAME,
                  &gEfiSecureBootEnableDisableGuid,
                  &VarAttributes,
                  &VarSize,
                  &SecureBootState
                  );
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  if (OcrBootSettings.RestoreSecureBootSetting == RESTORE_SECURE_BOOT_ENABLED &&
      SecureBootState == SECURE_BOOT_DISABLE &&
      !(OcrHttpsBoot && !EnforceSecureBoot)) {

    // Secure boot need to be re-enable from previous One Click Recovery HTTPS Boot
    SecureBootState = SECURE_BOOT_ENABLE;
    OcrBootSettings.RestoreSecureBootSetting = RESTORE_SECURE_BOOT_NONE;
    Status = gRT->SetVariable (
                    EFI_SECURE_BOOT_ENABLE_NAME,
                    &gEfiSecureBootEnableDisableGuid,
                    VarAttributes,
                    VarSize,
                    &SecureBootState
                    );
    ASSERT_EFI_ERROR (Status);

    Status = gRT->SetVariable (
                  OCR_BOOT_SETTINGS_NAME,
                  &mOcrBootSettingsGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (OCR_BOOT_SETTINGS),
                  &OcrBootSettings
                  );
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_INFO, "Secure Boot settings restored after OCR HTTPS Boot - Cold Reset!\n"));
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  return EFI_SUCCESS;
}
