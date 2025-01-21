/** @file
  Intel One Click Recovery Boot Option Support functional implementation.

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

#include "OcrBootOptionSupport.h"
#include <OneClickRecovery.h>
#include "OcrHttpSupport.h"
#include "OcrProxySupport.h"
#include "OcrHttpsCertificate.h"
#include "OcrHttpBootCallbackSupport.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeAsfHeciLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/AsfProtocol.h>
#include <Guid/AuthenticatedVariableFormat.h>

//
// One click Recovery Boot Settings GUID
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID  mOcrBootSettingsOptionsGuid = OCR_BOOT_SETTINGS_GUID;

/**
  Get OCR Boot Setting EFI Variable

  @retval OCR_BOOT_SETTINGS  OCR boot Setting
**/
OCR_BOOT_SETTINGS
GetOcrBootSettings (
  VOID
  )
{
  EFI_STATUS           Status;
  UINTN                VarSize;
  OCR_BOOT_SETTINGS    OcrBootSettings;

  VarSize = sizeof (OCR_BOOT_SETTINGS);

  Status = gRT->GetVariable(
                  OCR_BOOT_SETTINGS_NAME,
                  &mOcrBootSettingsOptionsGuid,
                  NULL,
                  &VarSize,
                  &OcrBootSettings
                  );

  if (EFI_ERROR (Status)) {
    OcrBootSettings.BootOptionExists = BOOT_OPTION_EXISTS_NONE;
    OcrBootSettings.RestoreSecureBootSetting = RESTORE_SECURE_BOOT_NONE;
  }

  return OcrBootSettings;
}

/**
  Checks for secure boot enabled state

  @retval TRUE      Secure boot is enabled
  @retval FALSE     Secure boot is disabled
**/
BOOLEAN
IsSecureBootEnabled (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       VarSize;
  UINT8       SecureBootState;
  UINT32      VarAttributes;


  VarSize = sizeof (UINT8);
  SecureBootState = SECURE_BOOT_DISABLE;
  Status = gRT->GetVariable (
                  EFI_SECURE_BOOT_ENABLE_NAME,
                  &gEfiSecureBootEnableDisableGuid,
                  &VarAttributes,
                  &VarSize,
                  &SecureBootState
                  );
  if (!EFI_ERROR (Status) || SecureBootState == SECURE_BOOT_ENABLE) {
    return TRUE;
  }
  return FALSE;
}

/**
  Creates OCR Description Title

  @param[in]  Description            Title to Append to boot option
  @param[out] OcrDescription         OCR Description
  @param[in]  MaxOcrDescriptionSize  Max OCR Description Size

  @retval  EFI_SUCCESS               Created OCR Description
  @retval  EFI_INVALID_PARAMETER     Pointers are null
  @retval  EFI_BUFFER_TOO_SMALL      Description will be larger than max size
**/
EFI_STATUS
CreateOcrBootOptionDescription (
  IN  CHAR16       *Description,
  OUT CHAR16       *OcrDescription,
  IN  UINT32       MaxOcrDescriptionSize
  )
{
  EFI_STATUS Status;
  UINTN      BootOptionSize;

  if (Description == NULL || OcrDescription == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  BootOptionSize = StrSize (Description) + StrSize (OCR_STRING) - 1;

  if (BootOptionSize > MaxOcrDescriptionSize) {
    return EFI_BUFFER_TOO_SMALL;
  }

  Status = StrCpyS (OcrDescription, BootOptionSize, OCR_STRING);
  if (!EFI_ERROR (Status)) {
    Status = StrCatS (OcrDescription, BootOptionSize, Description);
  }

  return Status;
}

/**
  Frees OCR Boot Option

  @param[in]  OcrBootOption    Boot option variable
**/
VOID
FreeOcrBootOption (
  IN OCR_BOOT_OPTION   *OcrBootOption
  )
{
  if (OcrBootOption != NULL) {
    if (OcrBootOption->OptionalData != NULL) {
      FreePool (OcrBootOption->OptionalData);
    }

    FreePool (OcrBootOption);
    OcrBootOption = NULL;
  }
}

/**
  Cancels OCR Boot Option by freeing memory, report bios status failure
  and perform warm reset if secure boot is disabled

  @param[in]  OcrBootOption    Boot option variable
**/
VOID
CancelOcrBootOption (
  IN OCR_BOOT_OPTION   *OcrBootOption
  )
{
  OCR_BOOT_SETTINGS OcrBootSettings;
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, "[%a] - Enter\n", __FUNCTION__));

  FreeOcrBootOption (OcrBootOption);
  Status = ReportBiosStatus (AsfRbsGeneralFailure);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OCR failed to report BIOS Status\n"));
  }

  OcrBootSettings = GetOcrBootSettings ();
  // Reset System if secure boot was disabled for OCR HTTPS Boot but current boot is not OCR.
  if (OcrBootSettings.RestoreSecureBootSetting == RESTORE_SECURE_BOOT_ENABLED) {
    DEBUG ((DEBUG_INFO, "Secure Boot disabled for OCR HTTPS Boot but not booting to OCR HTTPS - Warm Reset!\n"));
    gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
  }
}

/**
  Set UEFI Boot Options State

  @param[in] OcrConfig  OCR Setup Configuration
  @param[in] OcrCap     System OCR Capabilities

  @retval EFI_SUCCESS   Set AMT UEFI boot options states
**/
EFI_STATUS
SetUefiBootOptionsState (
  IN OCR_CONFIG               OcrConfig,
  IN ONE_CLICK_RECOVERY_CAP   OcrCap
  )
{
  EFI_STATUS                   Status;
  AMT_BOOT_CONTROL             UserAmtBootControl;
  AMT_BOOT_CONTROL             CurrentAmtBootControl;

  Status = AsfGetUefiBootOptionsState (&CurrentAmtBootControl);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  UserAmtBootControl.Uint32 = CurrentAmtBootControl.Uint32;

  UserAmtBootControl.Bits.AmtTrigBootToHttps        = (OcrCap.Bits.OcrBootHttps) ? OcrConfig.OcrConfigBootHttps : 0;
  UserAmtBootControl.Bits.AmtTrigBootToPba          = (OcrCap.Bits.OcrBootPba) ? OcrConfig.OcrConfigBootPba : 0;
  UserAmtBootControl.Bits.AmtTrigBootToWinRe        = (OcrCap.Bits.OcrBootWinRe) ? OcrConfig.OcrConfigBootWinRe : 0;
  UserAmtBootControl.Bits.AmtAllowedToDisSecureBoot = (OcrCap.Bits.OcrAmtDisSecBoot) ? OcrConfig.OcrConfigAmtDisSecBoot : 0;

  if (CurrentAmtBootControl.Uint32 != UserAmtBootControl.Uint32) {
    Status = AsfSetUefiBootOptionsState (UserAmtBootControl);
  }

  DEBUG ((DEBUG_INFO, "OcrBootHttps: %d\n", UserAmtBootControl.Bits.AmtTrigBootToHttps));
  DEBUG ((DEBUG_INFO, "OcrBootPba: %d\n", UserAmtBootControl.Bits.AmtTrigBootToPba));
  DEBUG ((DEBUG_INFO, "OcrBootWinRe: %d\n", UserAmtBootControl.Bits.AmtTrigBootToWinRe));
  DEBUG ((DEBUG_INFO, "OcrAmtDisSecBoot: %d\n", UserAmtBootControl.Bits.AmtAllowedToDisSecureBoot));

  return Status;
}

/**
  Adds OCR Boot Option to EFI Boot Manager

  @param[in] BootOption        Boot Option

  @retval EFI_SUCCESS          Added OCR Boot Option
  @retval EFI_INVALID          OCR Boot Option is NULL
  @retval EFI_NOT_FOUND        OCR Boot Device Path not found
  @retval EFI_UNSUPPORTED      OCR Boot Option is not supported
 **/
EFI_STATUS
AddOcrBootOption (
  IN OCR_BOOT_OPTION                 *BootOption
  )
{
  EFI_STATUS                       Status;
  EFI_BOOT_MANAGER_LOAD_OPTION     SetupOption;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;

  if (BootOption == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DevicePath = ConvertTextToDevicePath (BootOption->DevicePath);

  if (DevicePath == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = EfiBootManagerInitializeLoadOption (
             &SetupOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             LOAD_OPTION_ACTIVE,
             BootOption->Description,
             DevicePath,
             BootOption->OptionalData,
             BootOption->OptionalDataSize
             );

  if (!EFI_ERROR (Status)) {
    // Place as default boot option to load
    Status = EfiBootManagerAddLoadOptionVariable (&SetupOption, 0);
    DEBUG ((DEBUG_INFO, "OCR EfiBootManagerAddLoadOptionVariable: %x\n", Status));
  }

  return Status;
}

/**
  Delete One Click Recovery Boot Options if they exist
**/
VOID
DeleteOcrBootOption (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions;
  UINTN                        BootOptionCount;
  UINTN                        Index;
  EFI_STATUS                   Status;
  OCR_BOOT_SETTINGS            OcrBootSettings;

  OcrBootSettings = GetOcrBootSettings ();

  if (OcrBootSettings.BootOptionExists != BOOT_OPTION_EXISTS_NONE) {
    BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

    if (BootOptions != NULL) {
      // Remove OCR boot options from load options
      for (Index = 0; Index < BootOptionCount; Index++) {
        if ((StrnCmp(BootOptions[Index].Description, OCR_STRING, StrLen(OCR_STRING))) == 0) {
          EfiBootManagerDeleteLoadOptionVariable (BootOptions[Index].OptionNumber, LoadOptionTypeBoot);
        }
      }

      DEBUG ((DEBUG_INFO, "Remove OCR Boot options\n"));
      EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
    }

    // remove all previous certificates
    Status = DeleteAllCertificates ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "[%a] Failed with status %r", __FUNCTION__, Status));
    }

    //
    // Update OcrBootSettings variable with Boot Option Exists to None
    //
    OcrBootSettings.BootOptionExists = BOOT_OPTION_EXISTS_NONE;

    Status = gRT->SetVariable (
                    OCR_BOOT_SETTINGS_NAME,
                    &mOcrBootSettingsOptionsGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (OCR_BOOT_SETTINGS),
                    &OcrBootSettings
                    );
    ASSERT_EFI_ERROR (Status);
  }
}

/**
  Is the current Boot Option the One Click Recovery Boot Option

  @param[in,out]  OcrBootOption    Boot option variable

  @retval TRUE   Current Boot Option is One Click Recovery Boot Option
  @retval False  Current Boot Option is not One Click Recovery Boot Option
**/
BOOLEAN
IsCurrentBootOcrBootOption (
  IN OUT OCR_BOOT_OPTION    *OcrBootOption
  )
{
  EFI_STATUS                    Status;
  BOOLEAN                       IsOcrBootOption;
  UINTN                         VarSize;
  UINT16                        BootCurrent;
  CHAR16                        BootXXXX[16];
  EFI_BOOT_MANAGER_LOAD_OPTION  CurrentBootOption;
  CHAR16                        *CurrentDevicePath;

  if (OcrBootOption == NULL) {
    return FALSE;
  }

  IsOcrBootOption = FALSE;
  BootCurrent     = 0;
  VarSize = sizeof (UINT16);
  Status = gRT->GetVariable (
                  L"BootCurrent",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &VarSize,
                  &BootCurrent
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  UnicodeSPrint (BootXXXX, sizeof(BootXXXX), L"Boot%04X", BootCurrent);

  Status = EfiBootManagerVariableToLoadOption (BootXXXX, &CurrentBootOption);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  CurrentDevicePath = ConvertDevicePathToText (CurrentBootOption.FilePath, TRUE, TRUE);
  if (CurrentDevicePath != NULL) {
    DEBUG ((DEBUG_INFO, "Current Boot description: %s\nCurrent Boot DevicePath: %s\n",CurrentBootOption.Description, CurrentBootOption.FilePath));

    // Compare Current Boot option and OCR Boot option
    if ((StrnCmp (OcrBootOption->Description, CurrentBootOption.Description, MAX_UEFI_BOOT_OPTION_DESC_LENGTH) == 0) &&
        (CompareMem(OcrBootOption->OptionalData, CurrentBootOption.OptionalData, OcrBootOption->OptionalDataSize) == 0) &&
        (OcrBootOption->OptionalDataSize == CurrentBootOption.OptionalDataSize)) {

      if (StrnCmp (OcrBootOption->DevicePath, CurrentDevicePath, MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH) == 0) {
        IsOcrBootOption = TRUE;
      } else {
        IsOcrBootOption = IsOcrProxyOption (CurrentDevicePath);
      }
    }
  }
  EfiBootManagerFreeLoadOption (&CurrentBootOption);

  return IsOcrBootOption;
}

/**
  Sets the boot description, boot option and load option

  @param[in]      OcrConfig            OCR Setup Configuration
  @param[in]      AmtUefiBootOption    OCR AMT provided boot options
  @param[out]     Description          OCR boot option string
  @param[out]     OcrBootOption        OCR boot options
  @param[in, out] LoadOptionType       AMT boot control structure

  @retval EFI_SUCCESS             OCR Boot Option is setup.
  @retval EFI_NOT_FOUND           OCR Boot was not requested.
  @retval EFI_OUT_OF_RESOURCES    Ran out of resources
  @retval EFI_ABORTED             Boot Option requested is not valid
**/
EFI_STATUS
CheckSupportedOption (
  IN      OCR_CONFIG                    *OcrConfig,
  IN      UEFI_BOOT_OPTION_PARAMETER    *AmtUefiBootOption,
  OUT     CHAR16                        *Description,
  OUT     OCR_BOOT_OPTION               *OcrBootOption,
  IN OUT  AMT_BOOT_CONTROL              *LoadOptionType
  )
{
  UINT16                       SpecialCommand;
  ONE_CLICK_RECOVERY_CAP       OcrCap;

  SpecialCommand = AsfGetSpecialCommand ();

  if (SpecialCommand != ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD &&
      SpecialCommand != ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD) {
    DEBUG ((DEBUG_INFO, "OCR was not requested.\n"));
    return EFI_NOT_FOUND;
  }

  // Get OCR system capabilities
  OcrCap = OcrGetCapabilities ();

  if (SpecialCommand == ASF_INTEL_OEM_FORCE_HTTPS_BOOT_CMD &&
      AmtUefiBootOption->UefiBootOptionType == Https &&
      OcrConfig->OcrConfigBootHttps == 1 &&
      OcrCap.Bits.OcrBootHttps) {
    // OCR HTTPS Boot was requested
    StrCpyS(Description, MAX_UEFI_BOOT_OPTION_DESC_LENGTH, L" HTTPS");
    LoadOptionType->Bits.AmtTrigBootToHttps = 1;
  } else if (SpecialCommand == ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD &&
             AmtUefiBootOption->UefiBootOptionType == Pba &&
             OcrConfig->OcrConfigBootPba == 1 &&
             OcrCap.Bits.OcrBootPba) {
    // OCR PBA Boot was requested
    StrCpyS(Description, MAX_UEFI_BOOT_OPTION_DESC_LENGTH, L" PBA");
    LoadOptionType->Bits.AmtTrigBootToPba = 1;
  } else if (SpecialCommand == ASF_INTEL_OEM_FORCE_PBA_BOOT_CMD &&
             AmtUefiBootOption->UefiBootOptionType == WinRe &&
             OcrConfig->OcrConfigBootWinRe == 1 &&
             OcrCap.Bits.OcrBootWinRe) {
    // OCR Win Re Boot was requested
    StrCpyS(Description, MAX_UEFI_BOOT_OPTION_DESC_LENGTH, L" WinRe");
    LoadOptionType->Bits.AmtTrigBootToWinRe = 1;

    // add Optional Data to launch WinRe
    OcrBootOption->OptionalDataSize = (UINT32) StrSize (WINRE_OPTIONAL_DATA);
    OcrBootOption->OptionalData = AllocatePool (OcrBootOption->OptionalDataSize);
    if (OcrBootOption->OptionalData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (OcrBootOption->OptionalData, WINRE_OPTIONAL_DATA, OcrBootOption->OptionalDataSize);
  } else {
    DEBUG ((DEBUG_INFO, "Boot Option Request is not supported.\n"));
    return EFI_ABORTED;
  }
  return EFI_SUCCESS;
}


/**
  Check if Boot Option was requested and add it.

  @param[in,out] BootOption           OCR Boot Option
  @param[in]     AmtUefiBootOption    AMT Provided Boot Option

  @retval EFI_SUCCESS                 Successfully consumed OCR Parameters
  @retval EFI_INVALID_PARAMETER       Parameter pointer is null
**/
EFI_STATUS
SetupOcrBootOptionParams (
  IN UEFI_BOOT_OPTION_PARAMETER   *AmtUefiBootOption,
  IN OUT OCR_BOOT_OPTION          *BootOption
  )
{
  EFI_STATUS           Status;
  OCR_PARAMETER_TLV    *OcrParamTlv;
  UINT32               Index;
  CHAR8                *Username;
  CHAR8                *Password;

  Status   = EFI_SUCCESS;
  Username = NULL;
  Password = NULL;

  if (BootOption == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OcrParamTlv = (OCR_PARAMETER_TLV*)AmtUefiBootOption->ParametersArray;

  for (Index = 0; Index < AmtUefiBootOption->NumberOfParams; Index++) {
    DEBUG ((DEBUG_INFO, "Index:%d ParamTypeId: %d\n length:%d\n", Index, OcrParamTlv->Header.ParamType.ParamTypeId, OcrParamTlv->Header.Length));

    if (IsParamSupported (AmtUefiBootOption->UefiBootOptionType, OcrParamTlv->Header.ParamType.ParamTypeId)) {
      switch (OcrParamTlv->Header.ParamType.ParamTypeId) {
        case EfiNetworkDevicePathParamType:
          Status = NetworkDevicePathParam (OcrParamTlv, BootOption);
          if (EFI_ERROR (Status)) {
            goto Exit;
          }
          break;
        case EfiFileDeviceParamType:
          Status = FileDeviceParam (OcrParamTlv, BootOption);
          break;
        case EfiBootOptionalDataParamType:
          Status = OptionalDataParam (OcrParamTlv, BootOption);
          break;
        case HttpsCertSyncRootCaParamType:
          if (*OcrParamTlv->Value) {
            Status = GetRootCaCertificates ();
          }
          break;
        case HttpsRequestTimeOutParamType:
          Status = SetHttpTimeout (OcrParamTlv);
          break;
        case HttpsDigestUserNameParamType:
          Status = GetUsernameParam (OcrParamTlv, &Username);
          break;
        case HttpsDigestPasswordParamType:
          Status = GetPasswordParam (OcrParamTlv, &Password);
          break;
        case EfiDevicePathLenParamType:
        case BootImageHashSha256ParamType:
        case BootImageHashSha384ParamType:
        case BootImageHashSha512ParamType:
        case HttpsCertServerNameParamType:
        case HttpsServerNameVerifyMethodParamType:
        case HttpsServerCertHashSha256ParamType:
        case HttpsServerCertHashSha384ParamType:
        case HttpsServerCertHashSha512ParamType:
        default:
          break;
      }
    }

    // Increment to next OcrParamTlv
    OcrParamTlv = (OCR_PARAMETER_TLV*)(((UINT8 *)OcrParamTlv) + sizeof (OCR_PARAMETER_TLV_HEADER) + OcrParamTlv->Header.Length);
  }

  UpdateOcrHttpAuthInfo (Username, Password);

Exit:
  if (Username != NULL) {
    FreePool (Username);
    Username = NULL;
  }

  if (Password != NULL) {
    FreePool (Password);
    Password = NULL;
  }

  return Status;
}

/**
  Setup OCR Boot Option if requested from AMT

  @param[in]      OcrConfig            OCR Setup Configuration
  @param[in]      AmtUefiBootOption    OCR AMT provided boot options
  @param[in,out]  OcrBootOption        OCR boot options

  @retval EFI_SUCCESS             OCR Boot Option is setup.
  @retval EFI_NOT_FOUND           OCR Boot was not requested.
  @retval EFI_OUT_OF_RESOURCES    Ran out of resources
  @retval EFI_ABORTED             Boot Option requested is not valid
  @retval EFI_SECURITY_VIOLATION  Secure Boot needs to be enable for OCR WinRe and PBA
**/
EFI_STATUS
OcrBootOptionRequest (
  IN     OCR_CONFIG                   OcrConfig,
  IN     UEFI_BOOT_OPTION_PARAMETER   *AmtUefiBootOption,
  IN OUT OCR_BOOT_OPTION              *OcrBootOption
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *Description = NULL;
  OCR_BOOT_SETTINGS               OcrBootSettings;
  AMT_BOOT_CONTROL                LoadOptionType;

  Description = AllocateZeroPool(MAX_UEFI_BOOT_OPTION_DESC_LENGTH * sizeof(CHAR16));
  if (Description == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  LoadOptionType.Uint32 = 0;

  // report in progress Bios Status to AMT
  Status = ReportBiosStatus (AsfRbsInProgress);

  if (OcrBootOption == NULL) {
    CancelOcrBootOption (OcrBootOption);
    return EFI_INVALID_PARAMETER;
  }

  // Check that system supports requested OCR boot option
  Status = CheckSupportedOption (&OcrConfig, AmtUefiBootOption, Description, OcrBootOption, &LoadOptionType);
  if (EFI_ERROR (Status)) {
    CancelOcrBootOption (OcrBootOption);
    return Status;
  }

  // Check to make sure Secure Boot is enabled for PBA or WinRe
  if (AmtUefiBootOption->UefiBootOptionType == WinRe || AmtUefiBootOption->UefiBootOptionType  == Pba) {
    if (!IsSecureBootEnabled ()) {
      CancelOcrBootOption (OcrBootOption);
      return Status;
    }
  }

  // Set Boot Option Description
  Status = CreateOcrBootOptionDescription (Description, OcrBootOption->Description, MAX_UEFI_BOOT_OPTION_DESC_LENGTH);
  if (EFI_ERROR (Status)) {
    CancelOcrBootOption (OcrBootOption);
    return Status;
  }

  if (AmtUefiBootOption->EfiDevicePathLen != 0) {
    // Convert Device Path back to Unicode
    Status = AsciiStrToUnicodeStrS (
                (CHAR8 *) &AmtUefiBootOption->EfiDevicePath,
                OcrBootOption->DevicePath,
                MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH
                );
    if (EFI_ERROR (Status)) {
      CancelOcrBootOption (OcrBootOption);
      return Status;
    }
  }

  Status = SetupOcrBootOptionParams (AmtUefiBootOption, OcrBootOption);
  if (EFI_ERROR (Status)) {
    CancelOcrBootOption (OcrBootOption);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "OCR Boot request\n  devicepath: %s\n  devicepath len:0x%x\n  UEFItype: 0x%x", OcrBootOption->DevicePath, AmtUefiBootOption->EfiDevicePathLen, AmtUefiBootOption->UefiBootOptionType));
  DEBUG ((DEBUG_INFO, "  numofparam: 0x%x\n length:0x%x\n", AmtUefiBootOption->NumberOfParams, AmtUefiBootOption->LengthOfParamsArray));

  Status = AddOcrBootOption (OcrBootOption);
  if (EFI_ERROR (Status)) {
    CancelOcrBootOption (OcrBootOption);
    return Status;
  }

  Status = AddProxyBootOption (OcrBootOption);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddProxyBootOption Status = %r\n", Status));
  }

  if (AsfIsFwProgressEnabled ()) {
    Status = SendOcrPetEvent (
               ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS,
               ASF_OCR_EVENT_DATA3_CSME_BOOT_OPTION_ADDED,
               (UINT8) LoadOptionType.Uint32
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OCR failed to report PET event for adding OCR Boot option\n"));
    }
  }

  //
  // Set variable to delete OCR boot option if system is power off before
  //
  OcrBootSettings = GetOcrBootSettings ();
  OcrBootSettings.BootOptionExists = 1;
  Status = gRT->SetVariable (
                  OCR_BOOT_SETTINGS_NAME,
                  &mOcrBootSettingsOptionsGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (OCR_BOOT_SETTINGS),
                  &OcrBootSettings
                  );
  ASSERT_EFI_ERROR (Status);

  Status = RegisterOcrCallback ();
  if (EFI_ERROR (Status)) {
    CancelOcrBootOption (OcrBootOption);
    return Status;
  }

  return Status;
}
