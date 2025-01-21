/** @file
  Defined network structure used by the WiFi Connection Manager.

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

#ifndef _WIFI_MGR_CONFIG_H_
#define _WIFI_MGR_CONFIG_H_

#include <PiDxe.h>
#include <Uefi.h>
#include <Protocol/WiFi2.h>

#define OUI_IEEE_80211I  0xAC0F00

#define SSID_STORAGE_SIZE           33
#define EAP_IDENTITY_SIZE           64
#define PASSWORD_STORAGE_SIZE       65
#define WIFI_FILENAME_STR_MAX_SIZE  224

// Timeout to get profile data from AMT
#define ME_DELAY_COUNT    150u      // delay cycles to get profile from ME with fail status
#define ME_WLAN_DELAY     100000u   // 100msec delay allowing ME WLAN connection completion time

// Profile security type
#define PSK_NONE          0
#define PSK_PASS_PHRASE   3
#define PSK_NETWORK       4

//
//  Wifi Support Variable used to load Network drivers in BDS phase
//
#define WIFI_PROFILE_SYNC_BOOT_VARIABLE_NAME  L"WifiProfileSyncBoot"

typedef struct {
  BOOLEAN WifiProfileSyncBootSupport;
} WIFI_PROFILE_SYNC_CONFIG;

typedef enum {
  Ieee80211PairwiseCipherSuiteUseGroupCipherSuite = 0,
  Ieee80211PairwiseCipherSuiteWEP40               = 1,
  Ieee80211PairwiseCipherSuiteTKIP                = 2,
  Ieee80211PairwiseCipherSuiteCCMP                = 4,
  Ieee80211PairwiseCipherSuiteWEP104              = 5,
  Ieee80211PairwiseCipherSuiteBIP                 = 6,
  Ieee80211PairwiseCipherSuiteGCMP                = 8,
  Ieee80211PairwiseCipherSuiteGCMP256             = 9,
} IEEE_80211_PAIRWISE_CIPHER_SUITE;

typedef enum {
  Ieee80211AkmSuite8021XOrPMKSA       = 1,
  Ieee80211AkmSuitePSK                = 2,
  Ieee80211AkmSuite8021XOrPMKSASHA256 = 5,
  Ieee80211AkmSuitePSKSHA256          = 6,
  Ieee80211AkmSuiteSAE                = 8,
  Ieee80211AkmSuite8021XSuiteB        = 11,
  Ieee80211AkmSuite8021XSuiteB192     = 12,
  Ieee80211AkmSuiteOWE                = 18,
} IEEE_80211_AKM_SUITE;

typedef enum {
  EapAuthMethodTtls     = 0,
  EapAuthMethodPeap     = 1,
  EapAuthMethodTls      = 2,
  EapAuthMethodMax      = 3,
  EapAuthMethodUnknown  = 255
} EAP_AUTHENTICATION_METHOD;

typedef enum {
  EapSecondAuthMethodMschapV2  = 0,
  EapSecondAuthMethodMax       = 1,
  EapSecondAuthMethodUnknown   = 255
} EAP_SECOND_AUTHENTICATION_METHOD;

typedef struct {
  UINT32               Signature;

  //
  // Link to the current profile list in NIC device data (WIFI_MGR_DEVICE_DATA)
  //
  LIST_ENTRY           Link;

  UINT32               NicIndex;
  UINT32               ProfileIndex;  // The unique identifier for network profile, starts from 1
  CHAR16               SSId[SSID_STORAGE_SIZE];
  CHAR16               Password[PASSWORD_STORAGE_SIZE];

  UINT8                SecurityType;  // Wifi connection manager driver will assign this based on cipher and akm
  UINT8                EapAuthMethod;

  CHAR16               CACertName[WIFI_FILENAME_STR_MAX_SIZE];
  VOID                 *CACertData;
  UINTN                CACertSize;
  CHAR16               ClientCertName[WIFI_FILENAME_STR_MAX_SIZE];
  VOID                 *ClientCertData;
  UINTN                ClientCertSize;
  CHAR16               PrivateKeyName[WIFI_FILENAME_STR_MAX_SIZE];
  VOID                 *PrivateKeyData;
  UINTN                PrivateKeyDataSize;
  CHAR16               PrivateKeyPassword[PASSWORD_STORAGE_SIZE];  // Password to protect private key file
  CHAR16               EapIdentity[EAP_IDENTITY_SIZE];
  CHAR16               EapPassword[PASSWORD_STORAGE_SIZE];
  UINT8                EapSecondAuthMethod;

  BOOLEAN              AKMSuiteSupported;
  BOOLEAN              CipherSuiteSupported;
  BOOLEAN              IsAvailable;
  EFI_80211_NETWORK    Network;
  UINT8                NetworkQuality;
  EFI_STRING_ID        TitleToken;
} WIFI_MGR_NETWORK_PROFILE;

#define WIFI_MGR_PROFILE_SIGNATURE  SIGNATURE_32 ('W','M','N','P')

#endif
