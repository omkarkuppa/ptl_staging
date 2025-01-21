/** @file
  Intel One Click Recovery Proxy Support functional implementation

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

#include "OcrProxySupport.h"
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/MemoryAllocationLib.h>

//
// HTTP Boot Proxy Server Information
//
STATIC OCR_PROXY_INFO             *mOcrProxyInfo = NULL;

/**
  Checks if the Path for image proxy is found for OCR boot option

  @param[in]  CurrentDevicePath    System path for device source

  @retval TRUE    Path found to match, OCR option supported
  @retval FALSE   Path not found to match
**/
BOOLEAN
IsOcrProxyOption (
  IN UINT16     *CurrentDevicePath
  )
{
  UINTN   Index;
  if (mOcrProxyInfo == NULL) {
    return FALSE;
  }

  for (Index = 0; Index < mOcrProxyInfo->NumberOfProxies; Index++) {
    if (StrnCmp (mOcrProxyInfo->DevicePath[Index], CurrentDevicePath, MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH) == 0) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Adds OCR HTTP Boot Option with Proxy to EFI Boot Manager

  @param[in]  BootOption        Boot Option

  @retval EFI_SUCCESS           Added OCR HTTP Boot Option with Proxy or Proxy not requested
  @retval EFI_INVALID           OCR Boot Option is NULL
  @retval EFI_NOT_FOUND         OCR Boot Device Path not found
 **/
EFI_STATUS
AddProxyBootOption (
  IN  OCR_BOOT_OPTION   *BootOption
  )
{
  UINT32                           Index;
  EFI_STATUS                       Status;
  EFI_BOOT_MANAGER_LOAD_OPTION     SetupOption;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;

  if ((BootOption == NULL) || (mOcrProxyInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < mOcrProxyInfo->NumberOfProxies; Index++) {
    DevicePath = ConvertTextToDevicePath (mOcrProxyInfo->DevicePath[Index]);
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
      // Add boot option priority. Device path for direct connection to URI without proxy is highest priority.
      Status = EfiBootManagerAddLoadOptionVariable (&SetupOption, mOcrProxyInfo->Priority[Index] + 1);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "OCR Proxy Add LoadOption Variable Status: %r, Index = %d\n", Status, Index));
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Sets the priority of the OCR HTTP Boot Option with Proxy

  @param[in]  UriPath       Endpoint URI Path

  @retval EFI_SUCCESS       Added OCR HTTP Boot Option with Proxy or Proxy not requested
  @retval EFI_INVALID       OCR Paths invalid
 **/
EFI_STATUS
SetProxyPriority (
  IN CHAR16                *UriPath
  )
{
  UINT32 Index;
  UINT8  DnsMatch;
  UINT8  Proxy;

  if ((UriPath == NULL) || (mOcrProxyInfo == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  DnsMatch = 0;
  Proxy = (UINT8) mOcrProxyInfo->NumberOfProxies - 1;

  for (Index = 0; Index < mOcrProxyInfo->NumberOfProxies; Index++) {
    if (StrStr (UriPath, mOcrProxyInfo->DnsSuffix)) {
      mOcrProxyInfo->Priority[Index] = DnsMatch++;
    } else {
      mOcrProxyInfo->Priority[Index] = Proxy--;
    }
  }
  return EFI_SUCCESS;
}

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
  IN UINT16           *DevicePath,
  IN CHAR16           *UriPath,
  IN UINTN            UriLength
  )
{
  EFI_STATUS          Status;
  ASF_HTTP_PROXY      *AsfHttpProxy;
  UINT32              Index;
  UINT32              NumberOfProxies;
  CHAR8               ProxyDevicePath[MAX_FQDN_LENGTH];
  CHAR16              ProxyUriPath[MAX_FQDN_LENGTH];
  CHAR8               ProxyDnsSuffix[MAX_DNS_SUFFIX_LENGTH];

  AsfHttpProxy = NULL;

  if ((DevicePath == NULL) || (UriPath == NULL) || (UriLength == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  AsfHttpProxy = (ASF_HTTP_PROXY *)  AllocateZeroPool (sizeof (ASF_HTTP_PROXY) * MAX_NUMBER_OF_PROXIES);
  if (AsfHttpProxy == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = GetHttpProxy (AsfHttpProxy, &NumberOfProxies);

  if (EFI_ERROR (Status) || (NumberOfProxies == 0)) {
    //
    // Returned Status indicates if HttpDevice was found. As the first boot
    // option is already set, return SUCCESS even if proxy HECI call has failed.
    //
    return EFI_SUCCESS;
  }

  mOcrProxyInfo = (OCR_PROXY_INFO *) AllocateZeroPool (sizeof (OCR_PROXY_INFO));
  if (mOcrProxyInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mOcrProxyInfo->NumberOfProxies = NumberOfProxies;
  for (Index = 0; Index < NumberOfProxies; Index++) {
    AsciiSPrint (
      ProxyDevicePath,
      sizeof (CHAR8) * MAX_FQDN_LENGTH,
      "%a:%d",
      AsfHttpProxy[Index].AccessInfo,
      AsfHttpProxy[Index].Port
      );

    AsciiStrnToUnicodeStrS (
      (CHAR8*)ProxyDevicePath,
      AsciiStrLen(ProxyDevicePath),
      ProxyUriPath,
      MAX_FQDN_LENGTH,
      &UriLength
      );

    UnicodeSPrint (
      mOcrProxyInfo->DevicePath[Index],
      MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH*sizeof(CHAR16),
      L"%s/Uri(http://%s)/Uri(%s)", // Proxy URL is always expected to support HTTP protocol
      DevicePath,
      ProxyUriPath,
      &UriPath
      );

    DEBUG ((DEBUG_INFO, "Printing OCR Proxy Devicepath = %s\n", mOcrProxyInfo->DevicePath[Index]));
  }

  CopyMem (ProxyDnsSuffix, AsfHttpProxy->NetworkDnsSuffix, sizeof (AsfHttpProxy->NetworkDnsSuffix));
  AsciiStrnToUnicodeStrS (
    (CHAR8*)ProxyDnsSuffix,
    AsciiStrLen(ProxyDnsSuffix),
    mOcrProxyInfo->DnsSuffix,
    MAX_DNS_SUFFIX_LENGTH,
    &UriLength
    );

  SetProxyPriority (UriPath);
  return EFI_SUCCESS;
}
