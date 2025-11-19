/** @file
  Unit test case of the GetAsfProfile function

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

@par Specification
**/

// Unit test specific libraries
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

// WPS specific libraries
#include "WpsGetProfile.h"
#include <WifiProfileSync.h>
#include <WpsSetWcmProfile.h>
#include <WifiProfileSyncUtils.h>
#include <WifiProfileSyncConfig.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/EapConfiguration.h>

extern WIFI_PROFILE  *mSyncProfile;

/**
  Init the function to Get the ASF Wifi Profile.
  Retrieve WiFi profile information including client certificate and key using ASF

  @param[in,out]  Profile           WiFi profile protocol structure acquired from ASF

  @retval EFI_SUCCESS               Profiles returned
  @retval EFI_OUT_OF_RESOURCES      Not enough memory
  @retval EFI_UNSUPPORTED           Profile Sync is not supported via PCD or other parameter
  @retval Others                    Error Occurred
**/
extern
EFI_STATUS
GetAsfWifiProfile (
  IN OUT WIFI_PROFILE   *Profile
  );

/**
  Generates the WCM Profile data directly from Wifi data to test against

  @param[in, out]  TestWcmProfile   WiFi Connection Manager profile structure
  @param[in]       TestProfile      Generated test ASF Wifi profile data

  @return EFI_SUCCESS               Profiles returned
  @return EFI_UNSUPPORTED           Profile protocol sharing not supported or enabled
  @return EFI_NOT_FOUND             No profiles returned
  @return Others                    Error Occurred
**/
EFI_STATUS
GenWcmTestProfile (
  IN OUT WIFI_MGR_NETWORK_PROFILE   *TestWcmProfile,
  IN     WIFI_PROFILE               *TestProfile
)
{
  EFI_STATUS      Status;
  UINT32          Oui_Ieee_80211i;
  UINTN           Length;
  UINTN           Index;

  // Check the input for validity
  if ((TestWcmProfile == NULL) || (TestProfile  == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "[%a] Generating Wifi connection manager structure configured test control data...\n", __FUNCTION__));

  // Send Sync'd ASF profile data to WCM profile data
  TestWcmProfile->Signature                               = WIFI_MGR_PROFILE_SIGNATURE;
  TestWcmProfile->NicIndex                                = 1;
  TestWcmProfile->ProfileIndex                            = 0;

  CopyMem (&TestWcmProfile->SSId, &TestProfile->ProfileData->SSId, TestProfile->ProfileData->SSIdLength);

  // Psk Key
  if (TestProfile->ProfileData->PskKeyType == PSK_NETWORK) {
    Status = BufferToHexArray ((CHAR16 *) &TestWcmProfile->Password, sizeof (TestWcmProfile->Password), (UINT8 *) &TestProfile->ProfileData->PskKey, TestProfile->ProfileData->PskKeyLen);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "[%a] Failed to convert password with status of %r\n", __FUNCTION__, Status));
       return Status;
    }
  } else if (TestProfile->ProfileData->PskKeyType == PSK_PASS_PHRASE) {
    Length = 0;
    Status = AsciiStrnToUnicodeStrS (
               (CONST CHAR8 *) TestProfile->ProfileData->PskKey,
               (UINTN) TestProfile->ProfileData->PskKeyLen,
               TestWcmProfile->Password,
               PASSWORD_STORAGE_SIZE,
               &Length
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to convert PSK_PASS_PHRASE password with status of %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  // CA Certificate
  TestWcmProfile->CACertSize                              = TestProfile->CaCert->CertSize;
  if (TestProfile->CaCert->CertSize > 0) {
    TestWcmProfile->CACertData                            = &TestProfile->CaCert->Cert;
  } else {
    TestWcmProfile->CACertData                            = NULL;
  }
  // Client certificate and key
  if (TestProfile->ProfileData->UserCredentialsClientCertificateAvailable) {
    TestWcmProfile->ClientCertData                        = &TestProfile->Cert->Certificate;
    TestWcmProfile->ClientCertSize                        = TestProfile->Cert->CertSize;
    TestWcmProfile->PrivateKeyData                        = &TestProfile->Key->Key;
    TestWcmProfile->PrivateKeyDataSize                    = (UINTN) TestProfile->Key->KeySize;
  } else {
    TestWcmProfile->ClientCertData                        = NULL;
    TestWcmProfile->ClientCertSize                        = 0;
    TestWcmProfile->PrivateKeyData                        = NULL;
    TestWcmProfile->PrivateKeyDataSize                    = 0;
  }
  // 802.1x EAP
  TestWcmProfile->IsAvailable                             = TRUE;
  TestWcmProfile->EapAuthMethod                           = MapEapAuthMethod (TestProfile->ProfileData->AuthenticationProtocol_eapMethod);
  if (StrLen ((CHAR16 *) &mSyncProfile->ProfileData->UserCredentialsRoamingIdentity) > 0 ) {
    CopyMem (&TestWcmProfile->EapIdentity, &mSyncProfile->ProfileData->UserCredentialsRoamingIdentity, sizeof (mSyncProfile->ProfileData->UserCredentialsRoamingIdentity)-1);
  } else {
    CopyMem (&TestWcmProfile->EapIdentity, &mSyncProfile->ProfileData->UserCredentialsUsername, sizeof (mSyncProfile->ProfileData->UserCredentialsUsername)-1);
  }
  CopyMem (&TestWcmProfile->EapPassword, &mSyncProfile->ProfileData->UserCredentialsPassword, sizeof (mSyncProfile->ProfileData->UserCredentialsPassword));
  TestWcmProfile->EapSecondAuthMethod                     = MapAuthInnerMethodToEapSecondAuthMethod (TestProfile->ProfileData->AuthenticationProtocolInnerMethod);

  // Network authentication and encryption method
  TestWcmProfile->Network.BSSType                         = IeeeInfrastructureBSS;
  TestWcmProfile->Network.SSId.SSIdLen                    = TestProfile->ProfileData->SSIdLength;

  for (int i=0; i < SSID_SIZE; i++) {
    TestWcmProfile->Network.SSId.SSId[i] = TestProfile->ProfileData->SSId[i];
  }

  if (TestProfile->ProfileData->AuthenticationMethod != AuthenticationMethodWpa &&
      TestProfile->ProfileData->AuthenticationMethod != AuthenticationMethodRsn)
  {
    TestWcmProfile->Network.AKMSuite                        = (EFI_80211_AKM_SUITE_SELECTOR *) AllocateZeroPool (sizeof (EFI_80211_AKM_SUITE_SELECTOR));
    if (TestWcmProfile->Network.AKMSuite == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    TestWcmProfile->Network.CipherSuite                     = (EFI_80211_CIPHER_SUITE_SELECTOR *) AllocateZeroPool (sizeof (EFI_80211_CIPHER_SUITE_SELECTOR));
    if (TestWcmProfile->Network.CipherSuite == NULL) {
      FreePool (TestWcmProfile->Network.AKMSuite);
      TestWcmProfile->Network.AKMSuite = NULL;
      return EFI_OUT_OF_RESOURCES;
    }
    TestWcmProfile->Network.AKMSuite->AKMSuiteCount         = 1;
    TestWcmProfile->Network.CipherSuite->CipherSuiteCount   = 1;
    Oui_Ieee_80211i                                     = OUI_IEEE_80211I;
    memcpy (&TestWcmProfile->Network.AKMSuite->AKMSuiteList[0].Oui, &Oui_Ieee_80211i, sizeof (Oui_Ieee_80211i));
    memcpy (&TestWcmProfile->Network.CipherSuite->CipherSuiteList[0].Oui, &Oui_Ieee_80211i, sizeof (Oui_Ieee_80211i));

    TestWcmProfile->Network.AKMSuite->AKMSuiteList[0].SuiteType = MapAuthenticationToAKM (TestProfile->ProfileData->AuthenticationMethod);
    TestWcmProfile->Network.CipherSuite->CipherSuiteList[0].SuiteType = MapEncryptionToCipher (TestProfile->ProfileData->EncryptionMethod);
  } else {
    Length = sizeof (EFI_80211_AKM_SUITE_SELECTOR) + sizeof (EFI_80211_SUITE_SELECTOR) * (ENTERPRISE_AKM_SUITE_COUNT_MOCK - 1);
    TestWcmProfile->Network.AKMSuite = (EFI_80211_AKM_SUITE_SELECTOR *) AllocateZeroPool (Length);
    if (TestWcmProfile->Network.AKMSuite == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Length = sizeof (EFI_80211_CIPHER_SUITE_SELECTOR) + sizeof (EFI_80211_SUITE_SELECTOR) * (ENTERPRISE_CIPHER_SUITE_COUNT_MOCK - 1);
    TestWcmProfile->Network.CipherSuite = (EFI_80211_CIPHER_SUITE_SELECTOR *) AllocateZeroPool (Length);
    if (TestWcmProfile->Network.CipherSuite == NULL) {
      FreePool (TestWcmProfile->Network.AKMSuite);
      TestWcmProfile->Network.AKMSuite = NULL;
      return EFI_OUT_OF_RESOURCES;
    }
    TestWcmProfile->Network.AKMSuite->AKMSuiteCount = ENTERPRISE_AKM_SUITE_COUNT_MOCK;
    TestWcmProfile->Network.CipherSuite->CipherSuiteCount = ENTERPRISE_CIPHER_SUITE_COUNT_MOCK;
    for (Index = 0; Index < ENTERPRISE_AKM_SUITE_COUNT_MOCK; Index ++) {
      *((UINT32 *) (TestWcmProfile->Network.AKMSuite->AKMSuiteList + Index)) = (OUI_IEEE_80211I | (mEnterpriseAkmSuite[Index] << 24));
    }
    for (Index = 0; Index < ENTERPRISE_CIPHER_SUITE_COUNT_MOCK; Index ++) {
      *((UINT32 *) (TestWcmProfile->Network.CipherSuite->CipherSuiteList + Index)) = (OUI_IEEE_80211I | (mEnterpriseCipherSuite[Index] << 24));
    }
  }
  return EFI_SUCCESS;
}

/**
  ASF profile generated print.

  @param[in]  Profile    Generated RNG WiFi profile structure
**/
VOID
PrintAsfWifiProfile (
  IN WIFI_PROFILE   *Profile
)
{
  printf ("| WiFi Profile Status:                                       0x%X \n", Profile->ProfileData->Status);
  printf ("| WiFi Profile MACAddress:                                   0x%X",    Profile->ProfileData->MACAddress[0]);
  for (int i=1; i < sizeof (Profile->ProfileData->MACAddress); i++) {
    printf ("%X",  Profile->ProfileData->MACAddress[i]);
  }
  printf ("\n");
  printf ("| WiFi Profile ProfileName:                                  %s\n",    (char *) &Profile->ProfileData->ProfileName);
  printf ("| WiFi Profile SSIdLength:                                   0x%X \n", Profile->ProfileData->SSIdLength);
  printf ("| WiFi Profile SSId:                                         %s\n",    (char *) &Profile->ProfileData->SSId);
  printf ("| WiFi Profile EncryptionMethod:                             0x%X \n", Profile->ProfileData->EncryptionMethod);
  printf ("| WiFi Profile AuthenticationMethod:                         0x%X \n", Profile->ProfileData->AuthenticationMethod);
  printf ("| WiFi Profile PskKeyType:                                   0x%X \n", Profile->ProfileData->PskKeyType);
  printf ("| WiFi Profile PskKeyLen:                                    0x%X \n", Profile->ProfileData->PskKeyLen);
  printf ("| WiFi Profile PskKey:                                       0x");
  for (int i = 0; i < sizeof (Profile->ProfileData->PskKey); i++) {
    printf ("%X",  Profile->ProfileData->PskKey[i]);
  }
  printf ("\n");
  printf ("| WiFi Profile AuthenticationProtocol_eapMethod:             0x%X \n", Profile->ProfileData->AuthenticationProtocol_eapMethod);
  printf ("| WiFi Profile AuthenticationProtocolInnerMethod:            0x%X \n", Profile->ProfileData->AuthenticationProtocolInnerMethod);
  printf ("| WiFi Profile UserCredentialsRoamingIdentity:               %s\n",    (char *) &Profile->ProfileData->UserCredentialsRoamingIdentity);
  printf ("| WiFi Profile UserCredentialsDomain:                        %s\n",    (char *) &Profile->ProfileData->UserCredentialsDomain);
  printf ("| WiFi Profile UserCredentialsUsername:                      %s\n",    (char *) &Profile->ProfileData->UserCredentialsUsername);;
  printf ("| WiFi Profile UserCredentialsPassword:                      %s\n",    (char *) &Profile->ProfileData->UserCredentialsPassword);
  printf ("| WiFi Profile UserCredentialsClientCertificateAvailable:    0x%X \n", Profile->ProfileData->UserCredentialsClientCertificateAvailable);
  printf ("| WiFi Profile RootCACertificateIndex:                       0x%X \n", Profile->ProfileData->RootCACertificateIndex);
  printf ("| WiFi Profile ServerCertificateName:                        %s\n",    (char *) &Profile->ProfileData->ServerCertificateName);
  printf ("| WiFi Profile ServerCertificateNameComparison:              0x%X \n", Profile->ProfileData->ServerCertificateNameComparison);
}

/**
  Test WCM profile generated to print.

  @param[in]  Profile    Control WiFi Connection Manager profile structure
**/
VOID
PrintWcmProfile (
  IN WIFI_MGR_NETWORK_PROFILE   *Profile
)
{
  printf ("| WCM Profile:                                               0x%X \n", Profile->Signature);
  printf ("| WCM NicIndex:                                              0x%X \n", Profile->NicIndex);
  printf ("| WCM ProfileIndex:                                          0x%X \n", Profile->ProfileIndex);
  printf ("| WCM SSId:                                                  %s\n",    (char *) &Profile->SSId);
  printf ("| WCM SSId Length:                                           %d\n",    (int) strlen((char *) &Profile->SSId));
  printf ("| WCM Password:                                              ");
  for (int i = 0; i < PASSWORD_STORAGE_SIZE; i++) {
    printf ("%C", Profile->Password[i]);
  }
  printf ("\n");
  printf ("| WCM SecurityType:                                          0x%X \n", (UINT8) Profile->SecurityType);
  printf ("| WCM EapAuthMethod:                                         0x%X \n", Profile->EapAuthMethod);
  printf ("| WCM CACertSize:                                            0x%X \n", (UINT8) Profile->CACertSize);
  printf ("| WCM ClientCertSize:                                        0x%X \n", (UINT8) Profile->ClientCertSize);
  printf ("| WCM PrivateKeyDataSize:                                    0x%X \n", (UINT8) Profile->PrivateKeyDataSize);
  printf ("| WCM EapIdentity:                                           %s\n",    (char *) &Profile->EapIdentity);
  printf ("| WCM EapPassword:                                           ");
  for (int i= (int) (EAP_IDENTITY_SIZE - strlen ((char *) &Profile->EapPassword)); i < EAP_IDENTITY_SIZE; i++) {
    printf ("%X", (UINT16) Profile->EapPassword[i]);
  }
  printf ("\n");
  printf ("| WCM EapSecondAuthMethod:                                   0x%X \n", Profile->EapSecondAuthMethod);
  printf ("| WCM CipherSuiteSupported:                                  %s\n"   , Profile->CipherSuiteSupported ? "TRUE" : "FALSE");
  printf ("| WCM IsAvailable:                                           0x%X \n", Profile->IsAvailable);
  printf ("| WCM Network.AKMSuite->AKMSuiteCount:                       0x%X \n", Profile->Network.AKMSuite->AKMSuiteCount);
  printf ("| WCM Network.AKMSuite->AKMSuiteList->SuiteType:             0x%X \n", Profile->Network.AKMSuite->AKMSuiteList->SuiteType);
  printf ("| WCM Network.BSSType:                                       0x%X \n", (UINT8) Profile->Network.BSSType);
  printf ("| WCM Network.CipherSuite->CipherSuiteCount:                 0x%X \n", Profile->Network.CipherSuite->CipherSuiteCount);
  printf ("| WCM Network.CipherSuite->CipherSuiteList->SuiteType:       0x%X \n", Profile->Network.CipherSuite->CipherSuiteList->SuiteType);
}

/**
  Compare the control profile to that of the generated control file.

  @param[in]  WcmProfile                    WiFi Connection Manager profile structure from WPS function under test
  @param[in]  TestWcmProfile                Control WiFi Connection Manager profile structure

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
CompareProfiles (
  IN WIFI_MGR_NETWORK_PROFILE   *WcmProfile,
  IN WIFI_MGR_NETWORK_PROFILE   *TestWcmProfile
)
{
  int res;

  DEBUG ((DEBUG_INFO, "[%a] Compare function profile and control profile....\n", __FUNCTION__));

  // UT_ASSERT_MEM_EQUAL (MacAddress, TestMacAddress, sizeof (EFI_80211_MAC_ADDRESS));
  // DEBUG ((DEBUG_INFO, "-- MAC Address Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->Signature, TestWcmProfile->Signature);
  DEBUG ((DEBUG_INFO, "-- Signature Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->NicIndex, TestWcmProfile->NicIndex);
  DEBUG ((DEBUG_INFO, "-- NIC Index Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->ProfileIndex, TestWcmProfile->ProfileIndex);
  DEBUG ((DEBUG_INFO, "-- Profile Index Value Verified\n"));
  res = strncmp ((char *) &WcmProfile->SSId, (char *) &TestWcmProfile->SSId, SSID_STORAGE_SIZE);
  DEBUG ((DEBUG_INFO, "-- SSID Value %s\n", res == 0 ? "Verified" : "Failed"));
  if (res != 0) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  UT_ASSERT_MEM_EQUAL (WcmProfile->Password, TestWcmProfile->Password, PASSWORD_STORAGE_SIZE);
  DEBUG ((DEBUG_INFO, "-- Profile Password Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->SecurityType, TestWcmProfile->SecurityType);
  DEBUG ((DEBUG_INFO, "-- NIC Index Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->EapAuthMethod, TestWcmProfile->EapAuthMethod);
  DEBUG ((DEBUG_INFO, "-- EAP Authentication Method Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->CACertSize, TestWcmProfile->CACertSize);
  DEBUG ((DEBUG_INFO, "-- Root CA Certificate Size Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->ClientCertSize, TestWcmProfile->ClientCertSize);
  DEBUG ((DEBUG_INFO, "-- Client Certificate Size Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->PrivateKeyDataSize, TestWcmProfile->PrivateKeyDataSize);
  DEBUG ((DEBUG_INFO, "-- Private Key Data Size Value Verified\n"));
  res = strncmp ((char *) &WcmProfile->EapIdentity, (char *) &TestWcmProfile->EapIdentity, strlen ((char *) &WcmProfile->EapIdentity));
  DEBUG ((DEBUG_INFO, "-- EAP Identity Value  %s \n", res == 0 ? "Verified" : "Failed"));
  if (res != 0) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  res = strncmp ((char *) &WcmProfile->EapPassword, (char *) &TestWcmProfile->EapPassword, strlen ((char *) &WcmProfile->EapPassword));
  DEBUG ((DEBUG_INFO, "-- EAP Password Value  %s \n", res == 0 ? "Verified" : "Failed"));
  if (res != 0) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  UT_ASSERT_EQUAL (WcmProfile->EapSecondAuthMethod, TestWcmProfile->EapSecondAuthMethod);
  DEBUG ((DEBUG_INFO, "-- EAP Second Authentication Method Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->IsAvailable, TestWcmProfile->IsAvailable);
  DEBUG ((DEBUG_INFO, "-- Is Certificate Available Bit Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->Network.AKMSuite->AKMSuiteCount, TestWcmProfile->Network.AKMSuite->AKMSuiteCount);
  DEBUG ((DEBUG_INFO, "-- Network AKM Suite Count Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->Network.AKMSuite->AKMSuiteList->SuiteType, TestWcmProfile->Network.AKMSuite->AKMSuiteList->SuiteType);
  DEBUG ((DEBUG_INFO, "-- Network AKM Suite List Suite Type Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->Network.BSSType, TestWcmProfile->Network.BSSType);
  DEBUG ((DEBUG_INFO, "-- Network BSS Type Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->Network.CipherSuite->CipherSuiteCount, TestWcmProfile->Network.CipherSuite->CipherSuiteCount);
  DEBUG ((DEBUG_INFO, "-- Network Cipher Suite Count Value Verified\n"));
  UT_ASSERT_EQUAL (WcmProfile->Network.CipherSuite->CipherSuiteList->SuiteType, TestWcmProfile->Network.CipherSuite->CipherSuiteList->SuiteType);
  DEBUG ((DEBUG_INFO, "-- Network Cipher Suite List Suite Type Value Verified\n"));
  return UNIT_TEST_PASSED;
}

/**
  Verify Wifi Profile Sync getting and storing a profile from a mock ASF.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyGetAsfWifiProfile (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_STATUS            TestStatus;
  WIFI_MGR_NETWORK_PROFILE    *TestWcmProfile;
  WIFI_MGR_NETWORK_PROFILE    WcmProfile;
  EFI_80211_MAC_ADDRESS       MacAddress;

  TestWcmProfile    = NULL;
  TestStatus        = UNIT_TEST_PASSED;

  DEBUG ((DEBUG_INFO, "[%a] Allocating memory for test control data.\n", __FUNCTION__));

  //
  // Initialize memory space to receive test profile from WPS
  //
  memset (&MacAddress, 0, sizeof (MacAddress));
  mSyncProfile              = (WIFI_PROFILE *) AllocateZeroPool (sizeof (WIFI_PROFILE));
  UT_ASSERT_NOT_NULL (mSyncProfile);
  mSyncProfile->ProfileData = (WIFI_PROFILE_DATA *) AllocateZeroPool (sizeof (WIFI_PROFILE_DATA));
  mSyncProfile->Cert        = (WIFI_8021X_CLIENT_CERT *) AllocateZeroPool (sizeof (WIFI_8021X_CLIENT_CERT));
  mSyncProfile->Key         = (WIFI_8021X_CLIENT_KEY *) AllocateZeroPool (sizeof (WIFI_8021X_CLIENT_KEY));
  mSyncProfile->CaCert      = (ROOT_CA_CERTIFICATE *) AllocateZeroPool (sizeof (ROOT_CA_CERTIFICATE));

  //
  // Call the first function to get the mock AMT profile
  //
  Status = GetAsfWifiProfile (mSyncProfile);
  printf ("--------------------------------------------------------------------------\n");
  printf ("-------------------- GetAsfWifiProfile mSyncProfile ----------------------\n");
  printf ("--------------------------------------------------------------------------\n");
  PrintAsfWifiProfile (mSyncProfile);
  printf ("--------------------------------------------------------------------------\n");
  UT_ASSERT_STATUS_EQUAL (Status, EFI_SUCCESS);

  //
  // Generate the full control WCM profile
  //
  DEBUG ((DEBUG_INFO, "[%a] Running GenWcmTestProfile....\n", __FUNCTION__));
  UT_ASSERT_NOT_NULL (mSyncProfile);

  TestWcmProfile = (WIFI_MGR_NETWORK_PROFILE *) AllocateZeroPool (sizeof (WIFI_MGR_NETWORK_PROFILE));
  UT_ASSERT_NOT_NULL (TestWcmProfile);

  Status = GenWcmTestProfile (TestWcmProfile, mSyncProfile);
  printf ("--------------------------------------------------------------------------\n");
  printf ("------------------- GenWcmTestProfile TestWcmProfile ---------------------\n");
  printf ("--------------------------------------------------------------------------\n");
  PrintWcmProfile (TestWcmProfile);
  printf ("--------------------------------------------------------------------------\n");
  UT_ASSERT_STATUS_EQUAL (Status, EFI_SUCCESS);

  //
  // Initialize memory space to receive test profile from WPS
  //
  DEBUG ((DEBUG_INFO, "[%a] Running prep for get profile....\n", __FUNCTION__));
  Status = GetProfile (&WcmProfile, MacAddress);
  WcmProfile.AKMSuiteSupported = TestWcmProfile->AKMSuiteSupported;
  WcmProfile.CipherSuiteSupported = TestWcmProfile->CipherSuiteSupported;
  printf ("--------------------------------------------------------------------------\n");
  printf ("------------------------- GetProfile WcmProfile --------------------------\n");
  printf ("--------------------------------------------------------------------------\n");
  PrintWcmProfile (&WcmProfile);
  printf ("--------------------------------------------------------------------------\n");
  UT_ASSERT_STATUS_EQUAL (Status, EFI_SUCCESS);

  //
  // Compare received profile to control profile
  //
  TestStatus = CompareProfiles (&WcmProfile, TestWcmProfile);
  UT_ASSERT_STATUS_EQUAL (Status, UNIT_TEST_PASSED);

  // Free memory space
  if (TestWcmProfile != NULL) {
    FreePool (TestWcmProfile);
  }
  if (mSyncProfile != NULL) {
    FreePool (mSyncProfile);
  }

  // Exit with no issues
  return UNIT_TEST_PASSED;
}
