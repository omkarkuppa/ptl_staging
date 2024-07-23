/** @file
  MEBx Display Protocol definitions

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
#ifndef _MEBX_DISPLAY_PROTOCOL_H_
#define _MEBX_DISPLAY_PROTOCOL_H_


///
/// MEBx Display Protocol
///

typedef struct _MEBX_DISPLAY_PROTOCOL MEBX_DISPLAY_PROTOCOL;

typedef enum {
  ScreenModeMebx,
  ScreenModeBios
} SCREEN_MODE;

typedef enum {
  MebxUserInputNoInput,
  MebxUserInputEsc,
  MebxUserInputEnter,
  MebxUserInputOtherKey
} MEBX_USER_INPUT;

///
/// MEBx Message ID
///
typedef enum {
  RecordInvalidError,
  LoginFailedError,
  ProvDataMissingError,
  FailedEnabledFeaturesError,
  FailedUpdateManageabilityModeError,
  AppFailedLoadError,
  FwUpdateError,
  FwUpdateDeprecatedError,
  FailedGetProvStatusError,
  PkiDnsSuffixInvalidError,
  PkiDnsSuffixError,
  ConfigServerFqdnInvalidError,
  RemoteConfigEnDisInvalidError,
  RemoteConfigEnDisError,
  DefaultHashEnInvalidError,
  DefaultHashEnError,
  DefaultHashDisError,
  CustomHashConfigInvalidError,
  CustomHashConfigError,
  DeleteHashError,
  CustomHashStateError,
  InvalidCustomHashError,
  SolStorageRedirConfigInvalidError,
  SolStorageRedirDataError,
  SolStorageRedirAuthProtError,
  HostNameLargeError,
  DomainNameLargeError,
  DhcpInvalidError,
  IdleTimeoutInvalidError,
  IdleTimeoutError,
  ProvServerInvalidError,
  ProvServerPortInvalidError,
  Ipv4ParamsInvalidError,
  PwdPolicyInvalidError,
  Ipv6DataInvalidError,
  Ipv6SettingError,
  SharedFqdnInvalidError,
  DdnsInvalidError,
  KvmStateInvalidError,
  KvmStateError,
  OptInDataInvalidError,
  MeProvHaltDataInvalidError,
  MeProvHaltError,
  ManualSetupConfDataInvalidError,
  ProvServAddressError,
  HashHandlesError,
  HashEntriesError,
  AddCustomHashError,
  MeProvActivateError,
  Ipv4ParametersError,
  FqdnSettingError,
  OptinDataError,
  CompleteConfigFailedError,
  NewPasswordError,
  PowerPackagesError,
  ConinError,
  AmthiGetAmthiInterfaceVersionApiError,
  AmthiGetKvmStateApiError,
  AmthiGetPolicyAmtRedirectionStateApiError,
  AmthiGetOptinStateApiError,
  AmthiGetConfigSvrDataApiError,
  AmthiGetZtcConfigApiError,
  AmthiGetPkiFqdnSuffixApiError,
  AmthiGetIpv4ParamsApiError,
  AmthiGetFqdnApiError,
  AmthiGetIdleTimeoutApiError,
  AmthiGetProvisionStateApiError,
  AmthiGetAuditRecordApiError,
  AmthiGetHashDataApiError,
  AmthiGetConnectionStatusApiError,
  AmthiGetIpv6LanIntfSettingsApiError,
  AmthiGetPrivacyLevelApiError,
  AmthiGetPowerPolicyApiError,
  AmthiSetMebxEnterStateApiError,
  AmthiSetMebxExitStateApiError,
  KvmActiveSessionMsg,
  AmthiSetIpv4ParamsApiError,
  AmthiSetPkiFqdnSuffixApiError,
  AmthiSetSolStorageRedirectionStateApiError,
  AmthiSetKvmStateApiError,
  AmthiSetPwdPolicyApiError,
  AmthiSetFqdnApiError,
  AmthiSetOptinStateApiError,
  AmthiSetIdleTimeoutApiError,
  AmthiSetConfigServerApiError,
  AmthiPowerPolicyApiError,
  AmthiSetHashStateApiError,
  AmthiSetZtcApiError,
  AmthiStopConfigUnprovisionApiError,
  MeGetUserCapsApiError,
  MeGetFwCapsApiError,
  MeGetFwEnabledFeatureApiError,
  MeMeWaitFwFeatureAvailableApiError,
  MeSetAmtStateApiError,
  CoreUnconfigWoPassGetUnconfigStatusError,
  CoreUsbProvError,
  AmthiCloseUserInitiatedConnApiError,
  PressAnyKeyMsg,
  FoundUsbKeyMsg,
  ContinueAutoProvMsg,
  LoadingAmtMsg,
  DoneMsg,
  AmtManageabilityUsbDataMissingMsg,
  AmtManageabilityAlreadyProvMsg,
  StringNotAppliedTooManyHashesMsg,
  ConfigAppliedMsg,
  ContinueBootMsg,
  CoreCautionMsg,
  CoreCpuReplacementMsg,
  CoreFeaturesDisabledMsg,
  CoreFeaturesEnabledMsg,
  CoreConfirm1Msg,
  CoreConfirm2Msg,
  AmtManageabilityCilaMsg,
  CoreUnconfigWoPassMsg,
  AmtManualConfigUnsupportedMsg,

  // Values from 0x80000 to 0x8FFFF are reserved for USB Provisioning record display
  CorruptedDataEntryStart = 0x80000,
  CorruptedDataEntryEnd   = 0x8FFFF
} MEBX_MSG_ID;

/**
  This function prints MEBx information string to screen.

  @param[in,out] This      Pointer to MEBx Display Protocol
  @param[in]     MsgId     Message id

  @retval EFI_SUCCESS      Text was displayed successfully
  @retval Others           Error has occurred during displaying text
**/
typedef
EFI_STATUS
(EFIAPI *MEBX_DISPLAY_TEXT) (
  IN OUT MEBX_DISPLAY_PROTOCOL *This,
  IN     MEBX_MSG_ID           MsgId
  );

/**
  This function prints MEBx error string to screen and to console.

  @param[in,out] This      Pointer to MEBx Display Protocol.
  @param[in]     MsgId     The identifier of message.
  @param[in]     Delay     The amount of time for which the text will be displayed in microseconds.
                           If value is 0, the text is displayed in a fixed microseconds.

  @retval EFI_SUCCESS      Text was displayed successfully.
  @retval EFI_NOT_FOUND    The string specified by MsgId wasn't found.
**/
typedef
EFI_STATUS
(EFIAPI *MEBX_DISPLAY_ERROR) (
  IN OUT MEBX_DISPLAY_PROTOCOL *This,
  IN     MEBX_MSG_ID           MsgId,
  IN     UINT32                Delay
  );

/**
  This function draws image on the graphics screen.

  @param[in,out] This           Pointer to MEBx Display Protocol
  @param[in]     Bitmap         The data to transfer to the graphics screen
  @param[in]     ImageWidth     The width of image
  @param[in]     ImageHeight    The height of image

  @retval EFI_SUCCESS           Image was displayed successfully
  @retval EFI_INVALID_PARAMETER Invalid parameter
**/
typedef
EFI_STATUS
(EFIAPI *MEBX_DISPLAY_IMAGE) (
  IN OUT MEBX_DISPLAY_PROTOCOL *This,
  IN     UINT8                 *Bitmap,
  IN     UINT32                ImageWidth,
  IN     UINT32                ImageHeight
  );

/**
  This function sets specified graphics mode.

  @param[in,out] This      Pointer to MEBx Display Protocol
  @param[in]     Mode      Graphics mode to set

  @retval EFI_SUCCESS      New graphics mode set successfully
  @retval Others           Error has occurred
**/
typedef
EFI_STATUS
(EFIAPI *MEBX_CONFIGURE_SCREEN) (
  IN OUT MEBX_DISPLAY_PROTOCOL *This,
  IN     SCREEN_MODE           Mode
  );

/**
  This function reads user input.

  @param[in,out] This      Pointer to MEBx Display Protocol
  @param[out]    UserInput User input

  @retval EFI_SUCCESS      Successfully read user input
  @retval EFI_NOT_READY    There was no input from an user.
  @retval Others           Error has occurred
**/
typedef
EFI_STATUS
(EFIAPI *MEBX_GET_USER_INPUT) (
  IN OUT MEBX_DISPLAY_PROTOCOL *This,
     OUT MEBX_USER_INPUT       *UserInput
  );

/**
  This function gets information about current screen resolution.

  @param[in,out] This      Pointer to MEBx Display Protocol
  @param[out]    Width     Current screen width
  @param[out]    Height    Current screen height

  @retval EFI_SUCCESS      The function succeeded
  @retval Others           Error has occurred
**/
typedef
EFI_STATUS
(EFIAPI *MEBX_GET_SCREEN_RESOLUTION) (
  IN OUT MEBX_DISPLAY_PROTOCOL *This,
     OUT UINT16                *Width,
     OUT UINT16                *Height
  );

/**
  This function clears a screen's content.

  @param[in,out] This      Pointer to MEBx Display Protocol

  @retval EFI_SUCCESS      The function succeeded
  @retval Others           Error has occurred
**/
typedef
EFI_STATUS
(EFIAPI *MEBX_CLEAR_SCREEN) (
  IN OUT MEBX_DISPLAY_PROTOCOL *This
  );

/**
  MEBx Display Protocol
  The interface functions are used for I/O operations related to screen.
  It is platform vendor's responsibility to implement the function instance.
**/
typedef struct _MEBX_DISPLAY_PROTOCOL {
  MEBX_CONFIGURE_SCREEN      ConfigureScreen;
  MEBX_DISPLAY_TEXT          DisplayText;
  MEBX_DISPLAY_ERROR         DisplayError;
  MEBX_DISPLAY_IMAGE         DisplayImage;
  MEBX_GET_USER_INPUT        GetUserInput;
  MEBX_GET_SCREEN_RESOLUTION GetScreenResolution;
  MEBX_CLEAR_SCREEN          ClearScreen;
} MEBX_DISPLAY_PROTOCOL;

extern EFI_GUID gMebxDisplayProtocolGuid;

#endif // _MEBX_DISPLAY_PROTOCOL_H_
