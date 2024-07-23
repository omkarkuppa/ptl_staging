/** @file
  Intel One Click Recovery HTTP Support functional definitions

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

#ifndef __OCR_HTTP_SUPPORT_H__
#define __OCR_HTTP_SUPPORT_H__

#include <PiDxe.h>
#include <OneClickRecoveryConfig.h>

/**
  Checks for "https://" string in the given url

  @param[in] Url        Pointer to ascii url
  @param[in] UrlLength  Length of url

  @retval TRUE          URL is HTTPS
  @retval FALSE         URL is not HTTPS
**/
BOOLEAN
IsHttpsUrl (
  IN CHAR8    *Url,
  IN UINT32   UrlLength
  );

/**
  Gets the timeout value for requesting and receiving the recovery OS image from
  remote source when in HTTP recovery boot flow from AMT flow and sets the value
  of PcdHttpIoTimeout to update in NetworkPkg HttpBoot.

  @param[in] OcrParamTlv         Boot Option for HTTP boot image request and
                                 response timeout value

  @retval EFI_SUCCESS            Successfully received timeout value and set PCD
  @retval other                  Failed to get and set timeout value
**/
EFI_STATUS
SetHttpTimeout (
  IN OCR_PARAMETER_TLV       *OcrParamTlv
  );

#endif // __OCR_HTTP_SUPPORT_H__
