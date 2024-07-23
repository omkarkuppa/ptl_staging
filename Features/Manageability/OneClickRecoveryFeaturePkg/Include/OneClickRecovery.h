/** @file
  Intel One Click Recovery functional definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _OCR_H_
#define _OCR_H_

#include <Uefi.h>
#include <OneClickRecoveryConfig.h>

//
//  Protocol's
//
#include <Protocol/OneClickRecoveryProtocol.h>

/**
  Gets Intel One click Recovery BIOS Capabilities

  @retval ONE_CLICK_RECOVERY_CAPS       One Click Recovery BIOS Capabilites
**/
ONE_CLICK_RECOVERY_CAP
EFIAPI
OcrGetCapabilities (
  VOID
  );

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
  );

/**
  Saves OCR Uefi Boot Option from AMT.

  Needs to Be called before Clear Boot Options.

  @retval     EFI_SUCCESS       Saved Boot Option
**/
EFI_STATUS
EFIAPI
OcrSaveUefiBootOption (
  IN ASF_BOOT_OPTIONS           AsfBootOptions
  );

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
  );

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
  );

/**
  Registers a callback on Exit Boot Service event that will remove One Click Recovery from load options

  @retval EFI_SUCCESS    Callback was successfully registered
  @retval Others         Callback was not registered
**/
EFI_STATUS
EFIAPI
RegisterOcrCallback (
  VOID
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Function to find Http Device Path, it will favor ip4 over ip6

  @param[out] HttpDevicePath     Http Device Path, NULL if one could not be located

  @retval EFI_SUCCESS            Successful search for Http Device Path
  @retval EFI_NOT_FOUND          Couldn't to find any simple network devices
  @retval EFI_OUT_OF_RESOURCES   Out of resources
**/
EFI_STATUS
SearchHttpDevicePath (
  OUT EFI_DEVICE_PATH_PROTOCOL      **HttpDevicePath
  );

/**
  Finds All WinRe Boot Options and adds them as boot options

  @param[in, out] UefiBootOptions      Boot Options
  @param[in, out] NumUefiBootOptions   Number of UEFI boot options

  @retval EFI_SUCCESS                  Added winre Boot Option if exist
  @retval EFI_BUFFER_TOO_SMALL         Not enough room to add boot option
  @retval EFI_INVALID_PARAMETER        parameters are null pointers
**/
EFI_STATUS
AddWinReBootOptions (
  IN OUT UEFI_BOOT_OPTION     *UefiBootOptions,
  IN OUT UINT16               *NumUefiBootOptions
  );

#endif //_OCR_H_
