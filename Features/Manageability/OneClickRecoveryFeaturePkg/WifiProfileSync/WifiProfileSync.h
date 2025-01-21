/** @file
  WiFi Profile Sync driver definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _WIFI_PROFILE_SYNC_H_
#define _WIFI_PROFILE_SYNC_H_

#include <PiDxe.h>
#include <Txt.h>
#include <WifiProfileSyncAsfMsgs.h>
#include "WifiProfileSyncConfig.h"

#define WIFI_PROFILE_SYNC_VERSION   0x01

/**
  Function to set the WiFi connection status received by the WiFiConnectionManager
  as EFI_80211_CONNECT_NETWORK_RESULT_CODE, this will get converted to EFI_STATUS
  type

  @param[in] EFI_80211_CONNECT_NETWORK_RESULT_CODE    WiFi connection attempt results
**/
VOID
EFIAPI
SetConnectStatus (
  IN EFI_80211_CONNECT_NETWORK_RESULT_CODE  ConnectionStatus
  );

/**
  Function to retrieve the WiFi connection status when in OCR WLAN flow

  @retval Status                    WiFi connection status stored
**/
EFI_STATUS
EFIAPI
GetConnectStatus (
  VOID
  );

/**
  Callback to reset EFI network data setup data

  @param[in] Event                  A pointer to the Event that triggered the callback
  @param[in] Context                A pointer to private data registered with the callback function

**/
VOID
EFIAPI
DisableNetwork (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

/**
  Callback set for exit boot services to clear the profile data from memory and uninstall protocol

  @param[in] Event                  A pointer to the Event that triggered the callback
  @param[in] Context                A pointer to private data registered with the callback function

**/
VOID
EFIAPI
WifiProfileSyncCleanCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

/**
  This API will be used by the WiFi connection manager to get the WiFi profile that ASF shared
  and was stored in WiFi profile protocol, aligning and passing the ASF data structure to WCM
  structure

  @param[in, out]  WcmProfile       WiFi Connection Manager profile structure
  @param[in, out]  MacAddress       MAC address sent to the NiC aside profile data

  @return EFI_SUCCESS               Profiles returned
  @return EFI_UNSUPPORTED           Profile protocol sharing not supported or enabled
  @return EFI_NOT_FOUND             No profiles returned
  @return Others                    Error Occurred
**/
EFI_STATUS
EFIAPI
GetProfile (
  IN OUT WIFI_MGR_NETWORK_PROFILE  *WcmProfile,
  IN OUT EFI_80211_MAC_ADDRESS     MacAddress
  );

/**
  The entry point for the Wifi Profile Sync driver.

  @param[in] ImageHandle            The firmware allocated handle for the EFI image
  @param[in] SystemTable            A pointer to the EFI System Table

  @retval EFI_SUCCESS               The entry point is executed successfully
  @retval other                     Some error occurs when executing this entry point
**/
EFI_STATUS
EFIAPI
WifiProfileSyncEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

#endif
