/** @file
  Wifi Profile Sync and Wifi Connection Manager profile alignment
  helper functional implementation.

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

#include "WpsSetWcmProfile.h"
#include "WifiProfileSyncUtils.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/WiFi2.h>

//
// Suites and Suite Counters
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mEnterpriseAkmSuite[] = {
  Ieee80211AkmSuite8021XSuiteB192,
  Ieee80211AkmSuite8021XSuiteB,
  Ieee80211AkmSuite8021XOrPMKSASHA256,
  Ieee80211AkmSuite8021XOrPMKSA,
};
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mEnterpriseCipherSuite[] = {
  Ieee80211PairwiseCipherSuiteGCMP256,
  Ieee80211PairwiseCipherSuiteGCMP,
  Ieee80211PairwiseCipherSuiteCCMP,
};

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
  )
{
  if ((MacAddress == NULL) || (ProfileData == NULL) || (WcmProfile == NULL)) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  WcmProfile->Signature     = WIFI_MGR_PROFILE_SIGNATURE;
  WcmProfile->NicIndex      = 1;
  WcmProfile->ProfileIndex  = 0;
  CopyMem (&WcmProfile->SSId, &ProfileData->SSId, ProfileData->SSIdLength);
  CopyMem (&MacAddress->Addr, &ProfileData->MACAddress, sizeof (ProfileData->MACAddress));
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS    Status;
  UINTN         Length;

  if ((WcmProfile == NULL) || (ProfileData == NULL)) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (ProfileData->PskKeyType == PSK_NETWORK) {
    Status = BufferToHexArray (
               (CHAR16 *) &WcmProfile->Password,
               PASSWORD_STORAGE_SIZE,
               (UINT8 *) &ProfileData->PskKey,
               ProfileData->PskKeyLen
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "[%a] Failed to convert password with status of %r\n", __FUNCTION__, Status));
      return Status;
    }
  } else if (ProfileData->PskKeyType == PSK_PASS_PHRASE) {
    Length = 0;
    Status = AsciiStrnToUnicodeStrS (
               (CONST CHAR8 *) ProfileData->PskKey,
               (UINTN) ProfileData->PskKeyLen,
               WcmProfile->Password,
               PASSWORD_STORAGE_SIZE,
               &Length
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to convert PSK_PASS_PHRASE password with status of %r\n", __FUNCTION__, Status));
      return Status;
    }
  }
  return EFI_SUCCESS;
}

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
  )
{
  if ((WcmProfile == NULL) || (WpsProfile == NULL)) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  WcmProfile->CACertSize    = WpsProfile->CaCert->CertSize;
  if (WpsProfile->CaCert->CertSize > 0) {
    WcmProfile->CACertData  = (VOID *) &WpsProfile->CaCert->Cert;
  } else {
    WcmProfile->CACertData  = (VOID *) NULL;
  }
  return EFI_SUCCESS;
}

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
  )
{
  if ((WcmProfile == NULL) || (WpsProfile == NULL)) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (WpsProfile->ProfileData->UserCredentialsClientCertificateAvailable) {
    WcmProfile->ClientCertData      = (VOID *) &WpsProfile->Cert->Certificate;
    WcmProfile->ClientCertSize      = WpsProfile->Cert->CertSize;
    WcmProfile->PrivateKeyData      = (VOID *) &WpsProfile->Key->Key;
    WcmProfile->PrivateKeyDataSize  = WpsProfile->Key->KeySize;
  } else {
    WcmProfile->ClientCertData      = NULL;
    WcmProfile->ClientCertSize      = 0;
    WcmProfile->PrivateKeyData      = NULL;
    WcmProfile->PrivateKeyDataSize  = 0;
  }
  return EFI_SUCCESS;
}

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
  )
{
  if ((WcmProfile == NULL) || (ProfileData == NULL)) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  WcmProfile->IsAvailable           = TRUE;
  WcmProfile->EapAuthMethod         = MapEapAuthMethod (ProfileData->AuthenticationProtocol_eapMethod);

  if (StrLen ((CHAR16 *) &ProfileData->UserCredentialsRoamingIdentity) > 0 ) {
    CopyMem (&WcmProfile->EapIdentity, &ProfileData->UserCredentialsRoamingIdentity, sizeof (ProfileData->UserCredentialsRoamingIdentity)-1);
  } else {
    CopyMem (&WcmProfile->EapIdentity, &ProfileData->UserCredentialsUsername, sizeof (ProfileData->UserCredentialsUsername)-1);
  }

  CopyMem (&WcmProfile->EapPassword, &ProfileData->UserCredentialsPassword, sizeof (ProfileData->UserCredentialsPassword));
  WcmProfile->EapSecondAuthMethod   = MapAuthInnerMethodToEapSecondAuthMethod (ProfileData->AuthenticationProtocolInnerMethod);

  return EFI_SUCCESS;
}

/**
  Allocate the memory space for AKM and Cipher data based on authentication method.

  @param[in]      Network         Pointer to WCM Network profile
  @param[in, out] AuthMethod      AMT provided authentication method

  @return EFI_SUCCESS               Profiles returned
  @return EFI_OUT_OF_RESOURCES      System hit memory limit
  @return Others                    Error Occurred
**/
EFI_STATUS
AllocateAkmAndCipher (
  IN     UINT32               AuthMethod,
  IN OUT EFI_80211_NETWORK    *Network
  )
{
  UINTN     AkmLength;
  UINTN     CipherLength;

  AkmLength     = sizeof (EFI_80211_AKM_SUITE_SELECTOR);
  CipherLength  = sizeof (EFI_80211_CIPHER_SUITE_SELECTOR);

  if ((AuthMethod == AuthenticationMethodWpa) || (AuthMethod == AuthenticationMethodRsn)) {
    AkmLength     += sizeof (EFI_80211_SUITE_SELECTOR) * (ENTERPRISE_AKM_SUITE_COUNT - 1);
    CipherLength  += sizeof (EFI_80211_SUITE_SELECTOR) * (ENTERPRISE_CIPHER_SUITE_COUNT - 1);
  }

  Network->AKMSuite = (EFI_80211_AKM_SUITE_SELECTOR *) AllocateZeroPool (AkmLength);
  if (Network->AKMSuite == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Network->CipherSuite  = (EFI_80211_CIPHER_SUITE_SELECTOR *) AllocateZeroPool (CipherLength);
  if (Network->CipherSuite == NULL) {
    FreePool (Network->AKMSuite);
    Network->AKMSuite = NULL;
    return EFI_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}

/**
  Sets the akm and cipher List data and type based on authentication method.

  @param[in]      Network       Pointer to WCM Network profile
  @param[in, out] ProfileData   Pointer to AMT provided profile data
**/
VOID
SetAkmAndCipherList (
  IN     WIFI_PROFILE_DATA    *ProfileData,
  IN OUT EFI_80211_NETWORK    *Network
  )
{
  UINTN     Index;
  UINT32    Oui_Ieee_80211i;

  if ((ProfileData->AuthenticationMethod != AuthenticationMethodWpa) && (ProfileData->AuthenticationMethod != AuthenticationMethodRsn)) {
    Oui_Ieee_80211i                                     = OUI_IEEE_80211I;
    CopyMem (&Network->AKMSuite->AKMSuiteList[0].Oui, &Oui_Ieee_80211i, sizeof (Oui_Ieee_80211i));
    CopyMem (&Network->CipherSuite->CipherSuiteList[0].Oui, &Oui_Ieee_80211i, sizeof (Oui_Ieee_80211i));

    Network->AKMSuite->AKMSuiteList[0].SuiteType = MapAuthenticationToAKM (ProfileData->AuthenticationMethod);
    Network->CipherSuite->CipherSuiteList[0].SuiteType = MapEncryptionToCipher (ProfileData->EncryptionMethod);
  } else {
    //
    // CSME stores all the 1x profiles with the same authentication method and ignores specific
    // AKM. For 1x profile, WifiProfileSync shall configure ALL supported enterprise AKM/Cipher.
    // The driver will select one and configure supplicant accordingly.
    //
    for (Index = 0; Index < ENTERPRISE_AKM_SUITE_COUNT; Index ++) {
      *((UINT32 *) (Network->AKMSuite->AKMSuiteList + Index)) = (OUI_IEEE_80211I | (mEnterpriseAkmSuite[Index] << 24));
    }
    for (Index = 0; Index < ENTERPRISE_CIPHER_SUITE_COUNT; Index ++) {
      *((UINT32 *) (Network->CipherSuite->CipherSuiteList + Index)) = (OUI_IEEE_80211I | (mEnterpriseCipherSuite[Index] << 24));
    }
  }
}

/**
  Sets the akm and cipher count based on the authentication method.

  @param[in]      Network        Pointer to WCM Network profile
  @param[in, out] AuthMethod     AMT provided authentication method
**/
VOID
SetSuiteCount (
  IN     UINT32               AuthMethod,
  IN OUT EFI_80211_NETWORK    *Network
  )
{
  Network->AKMSuite->AKMSuiteCount         = 1;
  Network->CipherSuite->CipherSuiteCount   = 1;
  if ((AuthMethod == AuthenticationMethodWpa) || (AuthMethod == AuthenticationMethodRsn)) {
    Network->AKMSuite->AKMSuiteCount       = ENTERPRISE_AKM_SUITE_COUNT;
    Network->CipherSuite->CipherSuiteCount = ENTERPRISE_CIPHER_SUITE_COUNT;
  }
}

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
  )
{
  if ((WcmProfile == NULL) || (WpsProfile == NULL)) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  // Set BSS type
  WcmProfile->Network.BSSType = IeeeInfrastructureBSS;

  // Assign SSID
  WcmProfile->Network.SSId.SSIdLen  = WpsProfile->ProfileData->SSIdLength;
  CopyMem (WcmProfile->Network.SSId.SSId, &WpsProfile->ProfileData->SSId, EFI_MAX_SSID_LEN);
  // Allocate AKM and Cipher
  AllocateAkmAndCipher (WpsProfile->ProfileData->AuthenticationMethod, &WcmProfile->Network);

  // Set AKM and Cipher Lists
  SetAkmAndCipherList (WpsProfile->ProfileData, &WcmProfile->Network);

  // Set Suite Count
  SetSuiteCount (WpsProfile->ProfileData->AuthenticationMethod, &WcmProfile->Network);

  return EFI_SUCCESS;
}