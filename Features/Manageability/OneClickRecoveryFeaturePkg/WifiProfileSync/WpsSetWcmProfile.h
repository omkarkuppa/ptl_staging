/** @file
  Wifi Profile Sync and Wifi Connection Manager profile alignment
  helper functional definitions.

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

#ifndef _WPS_ALIGNMENT_SUPPORT_H_
#define _WPS_ALIGNMENT_SUPPORT_H_

#include <PiDxe.h>
#include "WifiProfileSync.h"
#include "WifiProfileSyncConfig.h"
#include <Protocol/WiFi2.h>

//
// Suites and Suite Counters
//
// GLOBAL_REMOVE_IF_UNREFERENCED STATIC UINT32 mEnterpriseAkmSuite[] = {
//   Ieee80211AkmSuite8021XSuiteB192,
//   Ieee80211AkmSuite8021XSuiteB,
//   Ieee80211AkmSuite8021XOrPMKSASHA256,
//   Ieee80211AkmSuite8021XOrPMKSA,
// };
#define ENTERPRISE_AKM_SUITE_COUNT (sizeof (mEnterpriseAkmSuite)  / sizeof (UINT32))
// GLOBAL_REMOVE_IF_UNREFERENCED STATIC UINT32 mEnterpriseCipherSuite[] = {
//   Ieee80211PairwiseCipherSuiteGCMP256,
//   Ieee80211PairwiseCipherSuiteGCMP,
//   Ieee80211PairwiseCipherSuiteCCMP,
// };
#define ENTERPRISE_CIPHER_SUITE_COUNT (sizeof (mEnterpriseCipherSuite)  / sizeof (UINT32))

/**
  Converts the stored AMT wifi profile SSID and MAC address to Wifi
  Connection Manager profile structure and stores WCM profile provided.

  @param[in]      MacAddress        WCM MAC address location
  @param[in]      ProfileData       Stored profile details
  @param[in, out] WcmProfile        WiFi Connection Manager profile structure

  @return EFI_SUCCESS               Profiles returned
  @return EFI_INVALID_PARAMETER     Pointers to profiles are null
  @return Others                    Error Occurred
**/
EFI_STATUS
AlignSsidAndMacAdr (
  IN     EFI_80211_MAC_ADDRESS      *MacAddress,
  IN     WIFI_PROFILE_DATA          *ProfileData,
  IN OUT WIFI_MGR_NETWORK_PROFILE   *WcmProfile
  );

/**
  Converts the stored AMT wifi profile PSK key to Wifi Connection Manager
  profile structure and stores WCM profile provided.

  @param[in]      ProfileData      Stored profile details
  @param[in, out] WcmProfile       WiFi Connection Manager profile structure

  @return EFI_SUCCESS               Profiles returned
  @return EFI_INVALID_PARAMETER     Pointers to profiles are null
  @return Others                    Error Occurred
**/
EFI_STATUS
AlignPskKey (
  IN     WIFI_PROFILE_DATA          *ProfileData,
  IN OUT WIFI_MGR_NETWORK_PROFILE   *WcmProfile
  );

/**
  Converts the stored AMT wifi profile CA Certificate to Wifi Connection Manager
  profile structure and stores WCM profile provided.

  @param[in]      WpsProfile        Stored profile details
  @param[in, out] WcmProfile        WiFi Connection Manager profile structure

  @return EFI_SUCCESS               Profiles returned
  @return EFI_INVALID_PARAMETER     Pointers to profiles are null
  @return Others                    Error Occurred
**/
EFI_STATUS
AlignCaCertificate (
  IN     WIFI_PROFILE               *WpsProfile,
  IN OUT WIFI_MGR_NETWORK_PROFILE   *WcmProfile
  );

/**
  Converts the stored AMT wifi profile Client Certificate and Key to Wifi Connection
  Manager profile structure and stores WCM profile provided.

  @param[in]      WpsProfile        Stored profile details
  @param[in, out] WcmProfile        WiFi Connection Manager profile structure

  @return EFI_SUCCESS               Profiles returned
  @return EFI_INVALID_PARAMETER     Pointers to profiles are null
  @return Others                    Error Occurred
**/
EFI_STATUS
AlignClientCertAndKey (
  IN     WIFI_PROFILE               *WpsProfile,
  IN OUT WIFI_MGR_NETWORK_PROFILE   *WcmProfile
  );

/**
  Converts the stored AMT wifi profile 802.1X EAP to Wifi Connection
  Manager profile structure and stores WCM profile provided.

  @param[in]      ProfileData       Stored profile details
  @param[in, out] WcmProfile        WiFi Connection Manager profile structure

  @return EFI_SUCCESS               Profiles returned
  @return EFI_INVALID_PARAMETER     Pointers to profiles are null
  @return Others                    Error Occurred
**/
EFI_STATUS
Align8021xEAP (
  IN     WIFI_PROFILE_DATA          *ProfileData,
  IN OUT WIFI_MGR_NETWORK_PROFILE   *WcmProfile
  );

/**
  Converts the stored AMT wifi profile 802.1X EAP to Wifi Connection
  Manager profile structure and stores WCM profile provided.

  @param[in]      WpsProfile      Stored profile details
  @param[in, out] WcmProfile      WiFi Connection Manager profile structure

  @return EFI_SUCCESS               Profiles returned
  @return EFI_INVALID_PARAMETER     Pointers to profiles are null
  @return Others                    Error Occurred
**/
EFI_STATUS
AlignEncryptionAndAuthentication (
  IN     WIFI_PROFILE               *WpsProfile,
  IN OUT WIFI_MGR_NETWORK_PROFILE   *WcmProfile
  );

#endif // _WPS_ALIGNMENT_SUPPORT_H_
