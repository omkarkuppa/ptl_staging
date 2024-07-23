/** @file
  Intel One Click Recovery HTTP Support Functional Implementation

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

#include <OneClickRecovery.h>
#include "OcrHttpsCertificate.h"
#include <IndustryStandard/Pci30.h>
#include <Library/PcdLib.h>
#include <Library/NetLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/WiFi2.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/WiFiProfileSyncProtocol.h>
#include <Guid/TlsAuthentication.h>
#include <Guid/ImageAuthentication.h>

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
  )
{
  CHAR8*    TempString;
  UINT32    HttpsLength;
  UINT32    Index;
  BOOLEAN   IsHttpsString;
  CHAR8     *HttpsString;

  HttpsString = "https://";

  HttpsLength = (UINT32)AsciiStrLen (HttpsString);

  TempString = AllocateZeroPool (HttpsLength);

  if ((UrlLength < HttpsLength) || (Url == NULL) || (TempString == NULL)) {
    return FALSE;
  }

  // Copy beginning of URL that contains https string and lower case it
  for (Index = 0; Index < HttpsLength; Index++) {
    if (Url[Index] > 64 && Url[Index] < 91) {
      TempString[Index] = Url[Index] + 32;
    } else {
      TempString[Index] = Url[Index];
    }
  }

  IsHttpsString = (AsciiStrnCmp (HttpsString, TempString, HttpsLength) == 0);

  FreePool (TempString);

  return IsHttpsString;
}

/**
  Function to connect WiFi device driver, check if WiFi connection successful and return
  the device path to the WiFi device

  @param[out] WifiDevicePath     WiFi Device Path, NULL if one could not be located
**/
VOID
ConnectWifiController (
  OUT EFI_DEVICE_PATH_PROTOCOL      **WifiDevicePath
  )
{
  EFI_STATUS                        Status;
  UINTN                             Index;
  UINTN                             HandleCount;
  EFI_HANDLE                        *HandleBuffer;
  PCI_TYPE00                        PciConfig;
  EFI_PCI_IO_PROTOCOL               *PciIo;
  EDKII_WIFI_PROFILE_SYNC_PROTOCOL  *WiFiProfileSyncProtocol;

  WiFiProfileSyncProtocol = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[%a] Failed to get PCI handle with status %r", __FUNCTION__, Status));
    return;
  }
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID *) &PciIo
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "[%a] Failed to get PCI device handle with status %r", __FUNCTION__, Status));
      FreePool (HandleBuffer);
      return;
    }
    PciIo->Pci.Read (
                PciIo,
                EfiPciIoWidthUint8,
                0,
                sizeof (PciConfig),
                &PciConfig
                );
    if (PciConfig.Hdr.ClassCode[1] == PCI_CLASS_NETWORK_OTHER && PciConfig.Hdr.ClassCode[2] == PCI_CLASS_NETWORK) {
      gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
    }
  }

  FreePool (HandleBuffer);
  Status = gBS->LocateProtocol (
                  &gEdkiiWiFiProfileSyncProtocolGuid,
                  NULL,
                  (VOID **) &WiFiProfileSyncProtocol
                  );
  if (!EFI_ERROR (Status)) {
    if (WiFiProfileSyncProtocol->GetConnectState () == EFI_SUCCESS) {
      Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiWiFi2ProtocolGuid,
                      NULL,
                      &HandleCount,
                      &HandleBuffer
                      );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "[%a] Failed to locate Wifi2 Protocol with status %r", __FUNCTION__, Status));
        return;
      }
      for (Index = 0; Index < HandleCount; Index++) {
        *WifiDevicePath = DevicePathFromHandle (HandleBuffer[Index]);
        if (*WifiDevicePath != NULL) {
          DEBUG ((DEBUG_INFO, "[%a] Found Wifi device path!", __FUNCTION__));
          break;
        }
      }
    }
  }
  FreePool (HandleBuffer);
  return;
}

/**
  This function will get the device path for a wifi device if there is one connected
  using the WiFi profile sync protocol.

  @param[out] WifiDevicePath    Valid IPv4 WiFi device path

  @retval EFI_SUCCESS           Successful search for Http Device Path
  @retval EFI_NOT_FOUND         Could not find any simple network devices
  @retval EFI_UNSUPPORTED       Device path not supported
 **/
EFI_STATUS
GetWifiDevicePath (
  OUT EFI_DEVICE_PATH_PROTOCOL      **WifiDevicePath
  )
{
  ConnectWifiController (WifiDevicePath);

  if (*WifiDevicePath != NULL) {
    while (!IsDevicePathEnd (*WifiDevicePath)) {
      if (DevicePathType (*WifiDevicePath) == MESSAGING_DEVICE_PATH) {
        if (DevicePathSubType (*WifiDevicePath) == MSG_IPv4_DP) {
          DEBUG ((DEBUG_INFO, "[%a] Found IP4 Path\n", __FUNCTION__));
          return EFI_SUCCESS;
        } else if (DevicePathSubType (*WifiDevicePath) == MSG_IPv6_DP) {
          DEBUG ((DEBUG_INFO, "[%a] IP6 found but not supported, continue searching...\n", __FUNCTION__));
          // IPv6 not supported so break and return unsupported with NULL as device path
          *WifiDevicePath = NULL;
        }
      }
      *WifiDevicePath = NextDevicePathNode (*WifiDevicePath);
    }
  }
  return EFI_NOT_FOUND;
}

/**
  This function will get the device path for an ethernet device if there is one connected.

  @param[out] LanDevicePath     Ethernet connected device path

  @retval EFI_SUCCESS           Successful search for Http Device Path
  @retval EFI_NOT_FOUND         Could not find any simple network devices
  @retval EFI_UNSUPPORTED       Device path not supported
 **/
EFI_STATUS
GetEthernetDevicePath (
  OUT EFI_DEVICE_PATH_PROTOCOL      **LanDevicePath
  )
{
  EFI_STATUS                        Status;
  UINTN                             Index;
  UINTN                             HandleCount;
  EFI_HANDLE                        *HandleBuffer;
  EFI_SIMPLE_NETWORK_PROTOCOL       *HandleSimpleNetwork;
  EFI_DEVICE_PATH_PROTOCOL          *HandleDevicePath;
  EFI_DEVICE_PATH_PROTOCOL          *TempDevicePath;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleNetworkProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Can not Locate EfiSimpleNetworkProtocolGuid\n", __FUNCTION__));
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status =  gBS->HandleProtocol(
                     HandleBuffer[Index],
                     &gEfiSimpleNetworkProtocolGuid,
                     (VOID **) &HandleSimpleNetwork
                     );

    // check for Ethernet network device
    if (HandleSimpleNetwork->Mode->IfType == NET_IFTYPE_ETHERNET) {

      Status =  gBS->HandleProtocol(
                       HandleBuffer[Index],
                       &gEfiDevicePathProtocolGuid,
                       (VOID **) &HandleDevicePath
                       );

      if (EFI_ERROR (Status) || (HandleDevicePath == NULL)) {
        DEBUG ((DEBUG_ERROR, "[%a] No Device Path\n", __FUNCTION__));
        continue;
      }

      // Check if the device is ip4/ip6
      TempDevicePath = HandleDevicePath;
      while (!IsDevicePathEnd (TempDevicePath)) {
        if (DevicePathType (TempDevicePath) == MESSAGING_DEVICE_PATH) {
          if (DevicePathSubType (TempDevicePath) == MSG_IPv4_DP) {
            DEBUG ((DEBUG_INFO, "[%a] Found IP4 Path\n", __FUNCTION__));
            *LanDevicePath = HandleDevicePath;
            // Return success only for IPv4 as currently IPv6 is not supported
            FreePool (HandleBuffer);
            return EFI_SUCCESS;
          } else if (DevicePathSubType (TempDevicePath) == MSG_IPv6_DP) {
            DEBUG ((DEBUG_INFO, "[%a] IP6 found but not supported, continue searching...\n", __FUNCTION__));
            // IPv6 not supported so break and return unsupported with NULL as device path
            *LanDevicePath = NULL;
          }
        }
        TempDevicePath = NextDevicePathNode (TempDevicePath);
      }
    }
  }
  DEBUG ((DEBUG_ERROR, "[%a] No Device Path Found\n", __FUNCTION__));
  FreePool (HandleBuffer);
  return EFI_NOT_FOUND;
}

/**
  Function to find Http Device Path, favoring LAN over Wireless LAN.
  OCR does not currently support IPv6, only IPv4 will return device path.

  @param[out] HttpDevicePath     Http Device Path, NULL if one could not be located

  @retval EFI_SUCCESS            Successful search for Http Device Path
  @retval EFI_NOT_FOUND          Could not find any simple network devices
  @retval EFI_UNSUPPORTED        Device path not supported
  @retval EFI_OUT_OF_RESOURCES   Out of resources
**/
EFI_STATUS
SearchHttpDevicePath (
  OUT EFI_DEVICE_PATH_PROTOCOL      **HttpDevicePath
  )
{
  EFI_STATUS                        Status;

  Status = GetEthernetDevicePath (HttpDevicePath);
  if (EFI_ERROR (Status)) {
#if FixedPcdGetBool (PcdWifiProfileSyncEnable) == 1
    Status = GetWifiDevicePath (HttpDevicePath);
#endif
  }
  DEBUG ((DEBUG_INFO, "[%a] Device Path found with status %r\n", __FUNCTION__, Status));
  return Status;
}

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
  )
{
  UINT32       TimeoutValue;

  TimeoutValue = HTTP_BOOT_TIMEOUT_DEFAULT;

  //
  // Get AMT HTTP timeout value
  //
  if (OcrParamTlv->Header.ParamType.ParamTypeId == HttpsRequestTimeOutParamType) {
    ASSERT (OcrParamTlv->Header.Length == sizeof (UINT16));
    TimeoutValue = *((UINT16*) OcrParamTlv->Value);
    DEBUG ((DEBUG_INFO, "Get AMT HTTP timeout value %d\n", TimeoutValue));
  }
  if ((TimeoutValue < HTTP_BOOT_TIMEOUT_MIN) || (TimeoutValue > HTTP_BOOT_TIMEOUT_MAX)) {
    TimeoutValue = HTTP_BOOT_TIMEOUT_DEFAULT;
  }

  DEBUG ((DEBUG_INFO, "Set PcdHttpIoTimeout %d\n", TimeoutValue));
  return PcdSet32S (PcdHttpIoTimeout, TimeoutValue);
}
