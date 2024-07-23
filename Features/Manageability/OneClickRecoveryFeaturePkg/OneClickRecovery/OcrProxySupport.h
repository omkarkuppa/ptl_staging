/** @file
  Intel One Click Recovery Proxy Support functional definition

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

#ifndef _OCR_PROXY_SUPPORT_H_
#define _OCR_PROXY_SUPPORT_H_

#include <PiDxe.h>
#include <OneClickRecoveryConfig.h>
#include <Protocol/DevicePath.h>

/**
  Checks if the Path for image proxy is found for OCR boot option

  @param[in]  CurrentDevicePath    System path for device source

  @retval TRUE    Path found to match, OCR option supported
  @retval FALSE   Path not found to match
**/
BOOLEAN
IsOcrProxyOption (
  IN UINT16     *CurrentDevicePath
  );

/**
  Adds OCR HTTP Boot Option with Proxy to EFI Boot Manager

  @param[in]  BootOption        Boot Option

  @retval EFI_SUCCESS           Added OCR HTTP Boot Option with Proxy or Proxy not requested
  @retval EFI_INVALID           OCR Boot Option is NULL
  @retval EFI_NOT_FOUND         OCR Boot Device Path not found
 **/
EFI_STATUS
AddProxyBootOption (
  IN OCR_BOOT_OPTION      *BootOption
  );


/**
  Sets the priority of the OCR HTTP Boot Option with Proxy

  @param[in]  UriPath       Endpoint URI Path

  @retval EFI_SUCCESS       Added OCR HTTP Boot Option with Proxy or Proxy not requested
  @retval EFI_INVALID       OCR Paths invalid
 **/
EFI_STATUS
SetProxyPriority (
  IN CHAR16     *UriPath
  );


/**
  Adds OCR HTTP Boot Option with Proxy to EFI Boot Manager

  @param[in]  DevicePath        Proxy device path
  @param[in]  UriPath           URI path for proxy
  @param[in]  UriLength         Length of URI path

  @retval EFI_SUCCESS           Added OCR HTTP Boot Option with Proxy or Proxy not requested
  @retval EFI_INVALID           OCR Paths invalid
  @retval EFI_OUT_OF_RESOURCES  No system resources
 **/
EFI_STATUS
OcrProxyInfoInit (
  IN UINT16     *DevicePath,
  IN CHAR16     *UriPath,
  IN UINTN      UriLength
  );

#endif // _OCR_PROXY_SUPPORT_H_
