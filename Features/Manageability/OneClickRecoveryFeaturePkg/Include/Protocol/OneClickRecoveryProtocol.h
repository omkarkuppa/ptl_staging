/** @file
  APIs for Intel One Click Recovery Main Protocol.

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
#ifndef _ONE_CLICK_RECOVERY_PROTOCOL_H_
#define _ONE_CLICK_RECOVERY_PROTOCOL_H_

#include <AsfMsgs.h>

//
// Intel One Click Recovery Capabilites
//
typedef union {
  struct{
    UINT8 OcrBootHttps      : 1;    ///< [0]    BIOS supports Intel One Click Recovery Https  with AMT specified device path
    UINT8 OcrBootPba        : 1;    ///< [1]    BIOS supports Intel One Click Recovery PBA Boot with AMT specified device path
    UINT8 OcrBootWinRe      : 1;    ///< [2]    BIOS supports Intel One Click Recovery WinRe Boot
    UINT8 OcrAmtDisSecBoot  : 1;    ///< [3]    BIOS supports Intel One Click Recovery disable Secure Boot for AMT provided UEFI Load option
    UINT8 OcrWifiProfile    : 1;    ///< [4]    Deprecated - Updated to use PcdWifiProfileSyncEnable in Smbios Type 130
    UINT8 Reserved2         : 3;    ///< [7:5]  Reserved
  } Bits;
  UINT8   Data;
} ONE_CLICK_RECOVERY_CAP;

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                               gOneClickRecoveryProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _ONE_CLICK_RECOVERY_PROTOCOL   ONE_CLICK_RECOVERY_PROTOCOL;

//
// One Click Recovery variable space to store and access ME setup variable values
//
#define OCR_CONFIG_VARIABLE_NAME              L"OcrConfig"

//
// ME Setup data for OCR operation
//
typedef struct {
  UINT8 OcrConfigBootHttps;
  UINT8 OcrConfigBootPba;
  UINT8 OcrConfigBootWinRe;
  UINT8 OcrConfigAmtDisSecBoot;
} OCR_CONFIG;

//
// OEM Boot Option Structure
//
typedef struct {
  // {Description,  DevicePath}
  // Example:
  // {L"OEM PBA", L"\\OemDirectory\\OemPba.efi"}

  CHAR16      *Description; ///< Description
  CHAR16      *DevicePath;  ///< Device Path
} BOOT_OPTIONS;

/**
  The Intel One Click Recovery Setup main function.

  The function does the necessary work to Setup the One Click Recovery feature.

  @retval     EFI_SUCCESS       One Click Recovery tasks have been ran
  @retval     EFI_UNSUPPORTED   One Click Recovery is not supported
**/
typedef
EFI_STATUS
(EFIAPI *ONE_CLICK_RECOVERY_ENTRY) (
  VOID
  );

/**
  Gives Intel One Click Recovery BIOS Capabilities supported

  @retval ONE_CLICK_RECOVERY_CAPS       One Click Recovery BIOS Capabilites
**/
typedef
ONE_CLICK_RECOVERY_CAP
(EFIAPI *ONE_CLICK_RECOVERY_CAPABILITIES) (
  VOID
  );

/**
  Saves OCR Uefi Boot Option from AMT.

  Needs to Be called before Clear Boot Options.

  @retval     EFI_SUCCESS       Saved Boot Option
**/
typedef
EFI_STATUS
(EFIAPI *ONE_CLICK_RECOVERY_SAVE_UEFI_BOOT_OPTION) (
  IN ASF_BOOT_OPTIONS   AsfBootOptions
  );

/**
  Sets OEM provided Boot Options to Boot Option List

  @param[in] OemBootOptionList          OEM provided boot options

  @retval  EFI_SUCCESS                  Added OCR Boot Option
  @retval  EFI_BUFFER_TOO_SMALL         Not enough room for boot options
  @retval  EFI_INVALID_PARAMETER        parameters are null pointers
**/
typedef
EFI_STATUS
(EFIAPI *SET_OCR_BOOT_OPTION_LIST) (
  IN BOOT_OPTIONS   *OemBootOptionList
  );

struct _ONE_CLICK_RECOVERY_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT32                                    Revision;
  ONE_CLICK_RECOVERY_ENTRY                  OcrEntry;
  ONE_CLICK_RECOVERY_CAPABILITIES           OcrCap;
  ONE_CLICK_RECOVERY_SAVE_UEFI_BOOT_OPTION  OcrSaveUefiBootOption;
  SET_OCR_BOOT_OPTION_LIST                  SetOcrBootOptionList;
};

/**
  Intel One Click Recovery Setup Protocol revision number

  Revision 1:   Initial version
  Revision 2:   Deprecating OcrWifiProfile bit in ONE_CLICK_RECOVERY_CAP
**/
#define  ONE_CLICK_RECOVERY_PROTOCOL_REVISION   2

#endif
