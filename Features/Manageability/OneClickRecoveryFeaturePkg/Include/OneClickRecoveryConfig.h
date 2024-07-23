/** @file
  Intel One Click Recovery Configuration and Definitions.

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

#ifndef _OCR_CONFIG_H_
#define _OCR_CONFIG_H_

#include <Uefi.h>
#include <AsfMsgs.h>

//
// Guid for OCR Boot Settings
//
#define OCR_BOOT_SETTINGS_GUID \
  { \
    0x6f31fbcb, 0xc924, 0x496f, {0x83, 0xc6, 0x8f, 0xa4, 0xf5, 0xdc, 0x79, 0x9d} \
  }

//
// OCR Boot Setting Name
//
#define OCR_BOOT_SETTINGS_NAME      L"OcrBootSettings"

// OCR Boot Options Exists
//
#define BOOT_OPTION_EXISTS_NONE           0
#define BOOT_OPTION_EXISTS                1

//
// OCR Restore Secure Boot Settings
//
#define RESTORE_SECURE_BOOT_NONE          0
#define RESTORE_SECURE_BOOT_ENABLED       1

//
// OCR HTTPS Response and Request Timeout Values
//
#define HTTP_BOOT_TIMEOUT_MIN        500       // 0.5sec for minimum allowed latency with http boot source.
#define HTTP_BOOT_TIMEOUT_MAX        120000    // 120sec for maximum allowed latency with http boot source.
#define HTTP_BOOT_TIMEOUT_DEFAULT    5000      // 5sec for default value of allowed latency.

//
// OCR HTTPS Authentication Settings
//
#define HTTP_BOOT_USERNAME_MAX_LEN            16
#define HTTP_BOOT_PASSWORD_MAX_LEN            16
#define HTTP_BOOT_AUTHENTICATION_INFO_MAX_LEN 80

//
// OCR Boot Setting Structure
//
typedef struct {
  UINT8 BootOptionExists;           ///< Boot Option Exists
  UINT8 RestoreSecureBootSetting;   ///< Restore Secure Boot Setting
} OCR_BOOT_SETTINGS;

//
// OCR Boot Option Parameter Type Structure
//
typedef struct {
  UINT16        VendorId;    ///< Vendor ID
  UINT16        ParamTypeId; ///< Parameter Type ID
} OCR_PARAMETER_TYPE;

//
// OCR Boot Option Parameter TLV Header Stucture
//
typedef struct {
  OCR_PARAMETER_TYPE  ParamType;  ///< OCR Parameter Type
  UINT32              Length;     ///< Length of the value array
} OCR_PARAMETER_TLV_HEADER;

//
// OCR Boot Option Parameter TLV Stucture
//
typedef struct {
  OCR_PARAMETER_TLV_HEADER  Header;   ///< OCR Parameter Type
  UINT8                     Value[0]; ///< OCR Paramter data
} OCR_PARAMETER_TLV;

#define MAX_UEFI_OPTIONAL_DATA  50

//
// OCR Boot Option Parameter Types
//
typedef enum {
  NoType,
  EfiNetworkDevicePathParamType,
  EfiFileDeviceParamType,
  EfiDevicePathLenParamType,
  BootImageHashSha256ParamType,
  BootImageHashSha384ParamType,
  BootImageHashSha512ParamType,
  EfiBootOptionalDataParamType = 10,
  HttpsCertSyncRootCaParamType = 20,
  HttpsCertServerNameParamType,
  HttpsServerNameVerifyMethodParamType,
  HttpsServerCertHashSha256ParamType,
  HttpsServerCertHashSha384ParamType,
  HttpsServerCertHashSha512ParamType,
  HttpsRequestTimeOutParamType = 30,
  HttpsDigestUserNameParamType = 40,
  HttpsDigestPasswordParamType
} OCR_PARAM_TYPE;


//
// OCR Boot Option Structure
//
typedef struct {
  CHAR16                DevicePath[MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH]; ///< Device Path
  CHAR16                Description[MAX_UEFI_BOOT_OPTION_DESC_LENGTH];    ///< Description
  UINT8                 *OptionalData;                                    ///< Optional Data
  UINT32                OptionalDataSize;                                 ///< Optional Data Size
} OCR_BOOT_OPTION;

//
// OCR Proxy Info Structure
//
typedef struct {
  CHAR16     DevicePath[MAX_NUMBER_OF_PROXIES][MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH];
  UINT32     Priority[MAX_NUMBER_OF_PROXIES];
  UINT32     NumberOfProxies;
  CHAR16     DnsSuffix[MAX_DNS_SUFFIX_LENGTH];
} OCR_PROXY_INFO;

#endif // _OCR_CONFIG_H_
