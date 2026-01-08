/** @file
  Intel One Click Recovery Setup Implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <OneClickRecovery.h>
#include "OcrHttpSupport.h"
#include "OcrPetEvent.h"
#include "OcrProxySupport.h"
#include "OcrPbaBootSupport.h"
#include "OcrSecureBootSupport.h"
#include "OcrBootOptionSupport.h"
#include "OcrHttpBootCallbackSupport.h"
#include <MeBiosPayloadHob.h>

#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/HttpLib.h>
#include <Library/DxeMeLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/PchInfoLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeAsfHeciLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/AsfProtocol.h>
#include <Protocol/ReportStatusCodeHandler.h>

#include <Guid/GlobalVariable.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/AuthenticatedVariableFormat.h>

//
// One Click Recovery Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED ONE_CLICK_RECOVERY_PROTOCOL mOneClickRecoveryProtocol = {
  ONE_CLICK_RECOVERY_PROTOCOL_REVISION,
  OcrMain,
  OcrGetCapabilities,
  OcrSaveUefiBootOption,
  SetOcrBootOptionList,
};

//
// One Click Recovery Boot Option
//
OCR_BOOT_OPTION                   *mOcrBootOption = NULL;

//
// AMT UEFI Boot Option Parameter
//
UEFI_BOOT_OPTION_PARAMETER        mAmtUefiBootOption;

//
// OCR Capabilities
//
ONE_CLICK_RECOVERY_CAP            mOcrCap;

//
// OEM OCR boot Option list
//
BOOT_OPTIONS                      *mBootOptionsList = NULL;

STATIC EFI_RSC_HANDLER_PROTOCOL   *mRscHandlerProtocol = NULL;

/**
  Gets Intel One click Recovery BIOS Capabilities

  @retval ONE_CLICK_RECOVERY_CAPS       One Click Recovery BIOS Capabilites
**/
ONE_CLICK_RECOVERY_CAP
EFIAPI
OcrGetCapabilities (
  VOID
  )
{
  mOcrCap.Data = 0;
  mOcrCap.Bits.OcrBootHttps     = (PcdGet8 (PcdOneClickRecoveryCapabilities) & 0x1);
  mOcrCap.Bits.OcrBootPba       = (PcdGet8 (PcdOneClickRecoveryCapabilities) & 0x2) >> 1;
  mOcrCap.Bits.OcrBootWinRe     = (PcdGet8 (PcdOneClickRecoveryCapabilities) & 0x4) >> 2;
  mOcrCap.Bits.OcrAmtDisSecBoot = (PcdGet8 (PcdOneClickRecoveryCapabilities) & 0x8) >> 3;

  DEBUG ((DEBUG_INFO, "OCR Capabilities:\n\tHTTPS: %x\n\tPBA: %x\n\tWinRe: %x\n\tDisable Sec Boot: %x\n",
    mOcrCap.Bits.OcrBootHttps,
    mOcrCap.Bits.OcrBootPba,
    mOcrCap.Bits.OcrBootWinRe,
    mOcrCap.Bits.OcrAmtDisSecBoot));

  return mOcrCap;
}

/**
  Is Intel One Click Recovery supported

  @retval TRUE   Intel One Click Recovery is supported
  @retval FALSE  Intel One Click Recovery is not supported
**/
BOOLEAN
IsOcrSupported (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB *MbpHob;

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    return FALSE;
  }

  // Check Brand Identity for OCR Enablement
  if (MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.OCR != 1 ) {
    DEBUG ((DEBUG_WARN, "[%a] Brand Identity reported OCR not enabled!\n", __FUNCTION__));
    return FALSE;
  }

  // Get the system OCR capabilites
  OcrGetCapabilities ();

  // Check that BIOS supports one of boot options: HTTPS, PBA, Win Re
  if (mOcrCap.Bits.OcrBootHttps || mOcrCap.Bits.OcrBootPba || mOcrCap.Bits.OcrBootWinRe) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  During Ready To Boot Event, Removes One Click Recovery from load options
  and sends status to AMT

  @param[in] Event     Event whose notification function is being invoked
  @param[in] Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OcrReadyToBootService (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS         Status;
  BOOLEAN            IsOcrBootOption;
  OCR_BOOT_SETTINGS  OcrBootSettings;
  UINT32             AsfBiosStatus;

  IsOcrBootOption = IsCurrentBootOcrBootOption (mOcrBootOption);

  if (IsOcrBootOption) {
    DEBUG ((DEBUG_INFO, "Booting OCR Boot Option\n"));
    AsfBiosStatus = AsfRbsSuccess;

    if (mOcrBootOption != NULL) {
      InstallNetworkCallback (mOcrBootOption->DevicePath);
    }

    if (AsfIsFwProgressEnabled ()) {
      //
      // Get Report Status Code Handler Protocol.
      //
      Status = gBS->LocateProtocol (&gEfiRscHandlerProtocolGuid, NULL, (VOID **) &mRscHandlerProtocol);
      ASSERT_EFI_ERROR (Status);

      //
      // Register report status code listener for OS Loader load and start.
      //
      DEBUG ((DEBUG_INFO, "Register report status code listener\n"));
      Status = mRscHandlerProtocol->Register (OcrStatusCodeListener, TPL_HIGH_LEVEL);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "OCR failed to register report status code listener\n"));
        return;
      }
    }
  } else {
    AsfBiosStatus = AsfRbsGeneralFailure;
    if (AsfIsFwProgressEnabled ()) {
      UninstallNetworkCallback ();
    }

    //
    // Unregister report status code listener.
    //
    if (mRscHandlerProtocol != NULL) {
      DEBUG ((DEBUG_INFO, "Unregister report status code listener\n"));
      mRscHandlerProtocol->Unregister (OcrStatusCodeListener);
      mRscHandlerProtocol = NULL;
    }
  }
  // report Bios Status to AMT based on if we're going to boot OCR boot option requested
  Status = ReportBiosStatus (AsfBiosStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OCR failed to report BIOS Status\n"));
  }

  // report PET if we're booting OCR Boot Option and PET are enabled
  if (AsfIsFwProgressEnabled () && IsOcrBootOption) {
    Status = SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS, ASF_OCR_EVENT_DATA3_ATTEMPT_TO_BOOT, 0);
  }

  if (!IsOcrBootOption) {
    OcrBootSettings   = GetOcrBootSettings ();
    // Reset System if secure boot was disable for OCR HTTPS Boot but current boot is not OCR.
    if (OcrBootSettings.RestoreSecureBootSetting == RESTORE_SECURE_BOOT_ENABLED) {
      DEBUG ((DEBUG_INFO, "Secure Boot disabled for OCR HTTPS Boot but not booting to OCR HTTPS - Warm Reset!\n"));
      gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
  }

  return;
}

/**
  During Exit Boot Event, sends AMT PET on whether OCR Boot Option Booted

  @param Event     Event whose notification function is being invoked
  @param Context   Pointer to the notification function's context
**/
VOID
EFIAPI
OcrExitBootService (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  // report PET if we're booting OCR Boot Option and PET are enabled
  if (AsfIsFwProgressEnabled () && IsCurrentBootOcrBootOption (mOcrBootOption)) {
    DEBUG ((DEBUG_INFO, "OCR Boot Option Booted\n"));
    UninstallNetworkCallback ();

    //
    // Unregister report status code listener.
    //
    if (mRscHandlerProtocol != NULL) {
      DEBUG ((DEBUG_INFO, "Unregister report status code listener\n"));
      mRscHandlerProtocol->Unregister (OcrStatusCodeListener);
      mRscHandlerProtocol = NULL;
    }
    SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS, ASF_OCR_EVENT_DATA3_EXIT_BOOT_SERVICES, 0);
  }

  DeleteOcrBootOption ();

  gBS->CloseEvent (Event);

  return;
}

/**
  Registers a callback on Exit Boot Service event that will remove One Click Recovery from load options

  @retval EFI_SUCCESS    Callback was successfully registered
  @retval Others         Callback was not registered
**/
EFI_STATUS
EFIAPI
RegisterOcrCallback (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBootServiceEvent;
  EFI_EVENT   ExitBootServiceEvent;

  //
  // Register Ready to Boot Event for One Click Recovery
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             OcrReadyToBootService,
             NULL,
             &ReadyToBootServiceEvent
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Register Exit Boot Services Event for One Click Recovery
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  OcrExitBootService,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServiceEvent
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

// List of WinRe Boot option supported Parameter types
STATIC UINT32 mWinReBootOptionParamSupport[] = {
  EfiBootOptionalDataParamType
};

// List of PBA Boot option supported Parameter types
STATIC UINT32 mPbaBootOptionParamSupport[] = {
  EfiFileDeviceParamType,
  BootImageHashSha256ParamType,
  BootImageHashSha384ParamType,
  BootImageHashSha512ParamType,
  EfiBootOptionalDataParamType
};

// List of Https Boot option supported Parameter types
STATIC UINT32 mHttpsBootOptionParamSupport[] = {
  EfiNetworkDevicePathParamType,
  HttpsCertSyncRootCaParamType,
  HttpsCertServerNameParamType,
  HttpsServerNameVerifyMethodParamType,
  HttpsServerCertHashSha256ParamType,
  HttpsServerCertHashSha384ParamType,
  HttpsServerCertHashSha512ParamType,
  HttpsRequestTimeOutParamType,
  HttpsDigestUserNameParamType,
  HttpsDigestPasswordParamType
};

/**
  Check if Parameter is supported by OCR Boot Option Type

  @param[in] BootType   OCR Boot Option Type
  @param[in] ParamType  OCR Parameter Type

  @retval TRUE          Parameter is supported
  @retval FALSE         Parameter isn't supported
**/
BOOLEAN
IsParamSupported (
  IN UINT32    BootType,
  IN UINT32    ParamType
  )
{
  UINT32    *SupportedBootParam;
  UINT32    SupportedBootParamCount;
  UINT32    Index;

  switch (BootType) {
    case WinRe:
      SupportedBootParam = mWinReBootOptionParamSupport;
      SupportedBootParamCount = sizeof (mWinReBootOptionParamSupport) / sizeof (UINT32);
      break;
    case Pba:
      SupportedBootParam = mPbaBootOptionParamSupport;
      SupportedBootParamCount = sizeof (mPbaBootOptionParamSupport) / sizeof (UINT32);
      break;
    case Https:
      SupportedBootParam = mHttpsBootOptionParamSupport;
      SupportedBootParamCount = sizeof (mHttpsBootOptionParamSupport) / sizeof (UINT32);
      break;
    default:
      return FALSE;
  }

  for (Index = 0;  Index < SupportedBootParamCount; Index++) {
    if (SupportedBootParam[Index] == ParamType) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Setups up Http Device Path

  @param[in]     OcrParamTlv     OCR Parameter TLV
  @param[in,out] BootOption      OCR Boot Option

  @retval EFI_SUCCESS            Successful consume OCR Param
  @retval EFI_INVALID_PARAMETER  Pointer is NULL or Length is too long
  @retval EFI_UNSUPPORTED        Url is not https
**/
EFI_STATUS
NetworkDevicePathParam (
  IN OCR_PARAMETER_TLV        *OcrParamTlv,
  IN OUT OCR_BOOT_OPTION      *BootOption
  )
{
  EFI_STATUS                      Status;
  CHAR16                          *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *HttpDevice;
  UINTN                           Length;
  CHAR16                          UriPath[MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH];
  UINTN                           UriLength;

  if ((OcrParamTlv == NULL) || (BootOption == NULL) || (OcrParamTlv->Header.Length > MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH)) {
    return EFI_INVALID_PARAMETER;
  }

  HttpDevice = AllocateZeroPool (sizeof (EFI_DEVICE_PATH_PROTOCOL));
  if (HttpDevice == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = SearchHttpDevicePath (&HttpDevice);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (!IsDevicePathValid (HttpDevice, 0)) {
    return EFI_NOT_FOUND;
  }

  DevicePath = ConvertDevicePathToText (HttpDevice, TRUE, TRUE);

  if (DevicePath == NULL) {
    return EFI_NOT_FOUND;
  }

  if (!IsHttpsUrl ((CHAR8*)OcrParamTlv->Value, OcrParamTlv->Header.Length)) {
    return EFI_UNSUPPORTED;
  }

  Length = StrnLenS (DevicePath, MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH)
            + StrnLenS (L"/Uri()", MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH)
            + OcrParamTlv->Header.Length;

  if (Length > MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH) {
    return EFI_INVALID_PARAMETER;
  }

  AsciiStrnToUnicodeStrS (
    (CHAR8*)OcrParamTlv->Value,
    OcrParamTlv->Header.Length,
    UriPath,
    MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH,
    &UriLength
    );

  UnicodeSPrint (
    BootOption->DevicePath,
    MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH * sizeof(CHAR16),
    L"%s/Uri(%s)",
    DevicePath,
    UriPath
    );

  Status = OcrProxyInfoInit (DevicePath, UriPath, UriLength);
  return Status;
}

/**
  Setups up File Device Path

  @param[in]      OcrParamTlv     OCR Parameter TLV
  @param[in,out]  BootOption      OCR Boot Option

  @retval EFI_SUCCESS                    Successful consume OCR Param
  @retval EFI_INVALID_PARAMETER          Pointer are NULL or Length is too long
**/
EFI_STATUS
FileDeviceParam (
  IN OCR_PARAMETER_TLV        *OcrParamTlv,
  IN OUT OCR_BOOT_OPTION      *BootOption
  )
{
  UINTN                           Length;

  if ((OcrParamTlv == NULL) || (BootOption == NULL) || (OcrParamTlv->Header.Length > MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH)) {
    return EFI_INVALID_PARAMETER;
  }

  AsciiStrnToUnicodeStrS (
    (CHAR8*)OcrParamTlv->Value,
    OcrParamTlv->Header.Length,
    BootOption->DevicePath,
    MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH,
    &Length
    );

  return EFI_SUCCESS;
}

/**
  Setups up Optional Data Parameter for Boot Option

  @param[in]      OcrParamTlv     OCR Parameter TLV
  @param[in,out]  BootOption      OCR Boot Option

  @retval EFI_SUCCESS             Successful consume OCR Param
  @retval EFI_INVALID_PARAMETER   Pointer are NULL or Length is too long
  @retval EFI_OUT_OF_RESOURCES    Out of resources
**/
EFI_STATUS
OptionalDataParam (
  IN OCR_PARAMETER_TLV        *OcrParamTlv,
  IN OUT OCR_BOOT_OPTION      *BootOption
  )
{
  UINT32           BufferSize;
  UINT8            *Buffer;

  if ((OcrParamTlv == NULL) || (BootOption == NULL) || (OcrParamTlv->Header.Length > MAX_UEFI_OPTIONAL_DATA) ||
       (BootOption->OptionalData != NULL && BootOption->OptionalDataSize == 0) ||
       (BootOption->OptionalData == NULL && BootOption->OptionalDataSize != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BootOption->OptionalData == NULL) {
    BufferSize = OcrParamTlv->Header.Length;
    Buffer = AllocatePool (BufferSize);
  } else {
    BufferSize = BootOption->OptionalDataSize + OcrParamTlv->Header.Length;
    Buffer = ReallocatePool (BootOption->OptionalDataSize, BufferSize, BootOption->OptionalData);
  }

  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BootOption->OptionalData = Buffer;
  CopyMem ((BootOption->OptionalData + BootOption->OptionalDataSize), OcrParamTlv->Value, OcrParamTlv->Header.Length);
  BootOption->OptionalDataSize = BufferSize;

  return EFI_SUCCESS;
}

/**
  Gets the HTTP Boot Authentication UserName

  @param[in]      OcrParamTlv     OCR Parameter TLV
  @param[out]     Username        Username string

  @retval EFI_SUCCESS                    Successful consume OCR Param
  @retval EFI_INVALID_PARAMETER          Pointer is NULL
**/
EFI_STATUS
GetUsernameParam (
  IN OCR_PARAMETER_TLV        *OcrParamTlv,
  OUT CHAR8                   **Username
  )
{
  if (OcrParamTlv == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (OcrParamTlv->Header.ParamType.ParamTypeId == HttpsDigestUserNameParamType) {
    *Username = AllocateZeroPool (HTTP_BOOT_USERNAME_MAX_LEN);
    CopyMem (*Username, OcrParamTlv->Value, OcrParamTlv->Header.Length);
  }
  return EFI_SUCCESS;
}

/**
  Gets the HTTP Boot Authentication Password

  @param[in]      OcrParamTlv     OCR Parameter TLV
  @param[out]     Password        Password string

  @retval EFI_SUCCESS                    Successful consume OCR Param
  @retval EFI_INVALID_PARAMETER          Pointer is NULL
**/
EFI_STATUS
GetPasswordParam (
  IN  OCR_PARAMETER_TLV       *OcrParamTlv,
  OUT CHAR8                   **Password
  )
{
  if (OcrParamTlv == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (OcrParamTlv->Header.ParamType.ParamTypeId == HttpsDigestPasswordParamType) {
    *Password = AllocateZeroPool (HTTP_BOOT_PASSWORD_MAX_LEN);
    CopyMem (*Password, OcrParamTlv->Value, OcrParamTlv->Header.Length);
  }
  return EFI_SUCCESS;
}

/*
  Set the OEM OCR boot option list for devices and device path.

  @param[in] OemOcrBootOptionList       OEM provided boot option device and path

  @retval EFI_SUCCESS                   OCR boot options received
  @retval EFI_OUT_OF_RESOURCES          Not enough room for boot options
  @retval EFI_INVALID_PARAMETER         parameters are null pointers
*/
EFI_STATUS
EFIAPI
SetOcrBootOptionList (
  IN  BOOT_OPTIONS    *OemOcrBootOptionList
  )
{
  if (OemOcrBootOptionList != NULL) {
    mBootOptionsList = OemOcrBootOptionList;
    return EFI_SUCCESS;
  }
  return EFI_INVALID_PARAMETER;
}

/**
  The Intel One Click Recovery Setup main function.

  The function does the necessary work to Setup the One Click Recovery feature.

  @retval     EFI_SUCCESS       One Click Recovery tasks have been ran
  @retval     EFI_UNSUPPORTED   One Click Recovery is not supported
**/
EFI_STATUS
EFIAPI
OcrMain (
  VOID
  )
{
  EFI_STATUS                    Status;
  UEFI_BOOT_OPTION              *UefiBootOptions;
  UINT16                        NumOfUefiBootOptions;
  OCR_CONFIG                    OcrConfig;
  UINTN                         VariableSize;

  DEBUG ((DEBUG_INFO, "OneClickRecovery setup\n"));

  VariableSize = sizeof (OCR_CONFIG);
  Status = gRT->GetVariable (
                OCR_CONFIG_VARIABLE_NAME,
                &gOcrConfigVariableGuid,
                NULL,
                &VariableSize,
                &OcrConfig
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Restore Secure Boot State from pre-existing OCR HTTPS
  OcrRestoreSecureBootState ();

  // Delete pre-existing OCR Boot Options
  DeleteOcrBootOption ();

  // Check for a pending OCR request and set it up
  mOcrBootOption = AllocateZeroPool (sizeof (OCR_BOOT_OPTION));
  if (mOcrBootOption == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = OcrBootOptionRequest (OcrConfig, &mAmtUefiBootOption, mOcrBootOption);

  // If OCR boot is setup, skip syncing available boot options and boot states
  if (EFI_ERROR (Status)) {
    Status = SetUefiBootOptionsState (OcrConfig, mOcrCap);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "SetUefiBootOptionsState Status = %r\n", Status));
    }


    // Allocate Boot Options list
    NumOfUefiBootOptions = 0;
    UefiBootOptions = AllocateZeroPool (MAX_UEFI_BOOT_OPTIONS * sizeof (UEFI_BOOT_OPTION));

    if (UefiBootOptions == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    // Add PBA Boot Options
    NumOfUefiBootOptions = 0;
    if (OcrConfig.OcrConfigBootPba && mOcrCap.Bits.OcrBootPba) {
      Status = AddPbaBootOptions (mBootOptionsList, UefiBootOptions, &NumOfUefiBootOptions);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "AddPbaBootOptions Status = %r\n", Status));
      }
    }

    // Add WinRe Boot Options
    if (OcrConfig.OcrConfigBootWinRe && mOcrCap.Bits.OcrBootWinRe) {
      Status = AddWinReBootOptions (UefiBootOptions, &NumOfUefiBootOptions);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "AddWinReBootOptions Status = %r\n", Status));
      }
    }

    Status = AsfUpdateUefiBootOptions (UefiBootOptions, NumOfUefiBootOptions);

    if ((UefiBootOptions != NULL) && (mBootOptionsList != NULL)) {
      FreePool(UefiBootOptions);
      mBootOptionsList = NULL;
    }
  }

  return Status;
}

/**
  Saves OCR Uefi Boot Option from AMT.

  Needs to Be called before Clear Boot Options.

  @param[in] AsfBootOptions     ASF provided boot option

  @retval     EFI_SUCCESS       Saved Boot Option
**/
EFI_STATUS
EFIAPI
OcrSaveUefiBootOption (
  IN ASF_BOOT_OPTIONS           AsfBootOptions
  )
{
  EFI_STATUS                      Status;

  // Disable Secure Boot if requested
  Status = OcrDisableSecureBootState (AsfBootOptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "OcrDisableSecureBootState Status = %r\n", Status));
  }

  // Get Asf UEFI Boot Option
  Status = AsfGetUefiBootParameters (&mAmtUefiBootOption);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AsfGetUefiBootParameters Status = %r\n", Status));
  }

  if (AsfBootOptions.BootOptionsMaskByte2 & FORCE_PROGRESS_EVENTS) {
    SendOcrPetEvent (ASF_EVENT_OFFSET_ONE_CLICK_RECOVERY_PROGRESS, ASF_OCR_EVENT_DATA3_BOOT_PARAMETER_RECEIVED, 0);
  }

  return Status;
}

/**
  Entry point of the Intel One Click Recovery Setup DXE Driver

  Installs the One Click Recovery Setup Protocol for other functions to locate and use

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval     EFI_SUCCESS       The protocol has installed successfully
  @retval     EFI_UNSUPPORTED   The protocol is not supported
  @retval     Others            The protocol did not install successfully
**/
EFI_STATUS
EFIAPI
OneClickRecoveryEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "[%a] - Enter\n", __FUNCTION__));

  if (!IsOcrSupported ()) {
    return EFI_UNSUPPORTED;
  }

  ZeroMem ((VOID *) &mAmtUefiBootOption, sizeof (UEFI_BOOT_OPTION_PARAMETER));

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gOneClickRecoveryProtocolGuid,
                  &mOneClickRecoveryProtocol,
                  NULL
                  );

  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "OneClickRecovery Protocol Install Status: %r\n", Status));

  return Status;
}
