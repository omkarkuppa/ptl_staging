/** @file
  Mock DXE AMT HECI Library functions for One Click Recovery Unit Testing

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

#include <Uefi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <WifiProfileSync.h>
#include <GenerateRandomDataLib.h>
#include <Library/DebugLib.h>

/**
  Get the wireless profile name

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
EFIAPI
AsfWifiGetProfileName (
  IN OUT WIFI_PROFILE_DATA        *Profile
)
{
  DEBUG((DEBUG_INFO, "[%a] running...\n", __FUNCTION__));

  if (Profile == NULL) {
    DEBUG ((DEBUG_INFO, "[%a] Profile NULL! \n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  strncpy_s ((char *) &Profile->ProfileName, PROFILE_NAME_SIZE, "Unit_Test_Profilexxxx", PROFILE_NAME_SIZE);
  Profile->Status = AmtStatusSuccess;
  return EFI_SUCCESS;
}

/**
  Get the wireless profile data

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetProfileData (
  IN OUT WIFI_PROFILE_DATA        *Profile
)
{
  if (Profile == NULL) {
    DEBUG ((DEBUG_INFO, "[%a] Profile NULL! \n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  DEBUG((DEBUG_INFO, "[%a] running...\n", __FUNCTION__));
  // Seed the random number generator.
  srand((unsigned int) time(NULL));
  // Generate the Wifi Profile Test Data
  GenerateRandomData ((UINT8 *) &Profile->MACAddress,  MAC_ADDR_SIZE);
  strncpy_s ((char *) &Profile->SSId, SSID_SIZE, "Unit_Test_SSID#$#^^&&^^88^^_ :)", SSID_SIZE);
  Profile->SSIdLength = (UINT8) strlen (Profile->SSId);
  GenerateRandomData ((UINT8 *) &Profile->EncryptionMethod, 8);
  GenerateRandomData ((UINT8 *) &Profile->AuthenticationMethod, 8);
  Profile->PskKeyType = 4;
  GenerateRandomData ((UINT8 *) &Profile->PskKey, PSK_KEY_SIZE);
  Profile->PskKeyLen = (UINT8) strlen (Profile->PskKey);
  GenerateRandomData ((UINT8 *) &Profile->AuthenticationProtocol_eapMethod, 8);
  GenerateRandomData ((UINT8 *) &Profile->AuthenticationProtocolInnerMethod, 8);
  strncpy_s ((char *) &Profile->UserCredentialsRoamingIdentity, USER_CRED_IDENTITY_SIZE, "Unit_Test_Cred_Roaming_ID230948^$08$ :)", USER_CRED_IDENTITY_SIZE);
  strncpy_s ((char *) &Profile->UserCredentialsPassword, USER_CRED_DOMAIN_SIZE, "Unit_Test_User_Cred_Password230@#)(*0958342 :)", USER_CRED_DOMAIN_SIZE);
  strncpy_s ((char *) &Profile->UserCredentialsUsername, USER_CRED_USERNAME_SIZE, "Unit_Test_User_Certificate_Password!+=@#$^&*( :)", USER_CRED_USERNAME_SIZE);
  strncpy_s ((char *) &Profile->UserCredentialsPassword, USER_CRED_PASSWORD_SIZE, "Unit_Test_Server_Cert_Name :)", USER_CRED_PASSWORD_SIZE);
  // Set to 1 so that we test getting and storing Client Cert and Key
  Profile->UserCredentialsClientCertificateAvailable = 1;
  GenerateRandomData ((UINT8 *) &Profile->RootCACertificateIndex, 8);
  strncpy_s ((char *) &Profile->ServerCertificateName, SERVER_CERT_NAME_SIZE, "Unit_Test_Server_Certificate_Name", SERVER_CERT_NAME_SIZE);
  GenerateRandomData ((UINT8 *) &Profile->ServerCertificateNameComparison, 4);
  Profile->Status = AmtStatusSuccess;
  return EFI_SUCCESS;
}

/**
  Get the wireless profile specific 8021X client certificate

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetClientCertificate (
  IN OUT WIFI_PROFILE             *Profile
)
{
  DEBUG((DEBUG_INFO, "[%a] running...\n", __FUNCTION__));
  // Generate CA Cert Test Data
  GenerateRandomData ((UINT8 *) &Profile->Cert->Certificate, MAX_ROOT_CA_CERTIFICATE_SIZE / sizeof (Profile->Cert->Certificate[0]));
  Profile->Cert->CertSize = sizeof (Profile->Cert->Certificate);
  return EFI_SUCCESS;
}

/**
  Get the wireless profile specific 8021X client key

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetClientKey (
  IN OUT WIFI_PROFILE             *Profile
)
{
  DEBUG((DEBUG_INFO, "[%a] running...\n", __FUNCTION__));
  // Generate Key Test Data
  GenerateRandomData ((UINT8 *) &Profile->Key->Key, MAX_CLIENT_KEY_SIZE / sizeof (Profile->Key->Key[0]));
  Profile->Key->KeySize = sizeof (Profile->Key->Key);
  return EFI_SUCCESS;
}

/**
  Get Root Certificate Authority(CA) Certificate from ASF at specified Index by sending
  proper HECI Command

  @param[in]   Index             Index
  @param[out]  Cert              Root CA Certificate

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
  @retval EFI_DEVICE_ERROR       ASF Response is invalid
**/
EFI_STATUS
AsfGetRootCaCertificate (
  IN UINT32                   Index,
  OUT ROOT_CA_CERTIFICATE     *Cert
)
{
  DEBUG((DEBUG_INFO, "[%a] running...\n", __FUNCTION__));
  // Generate Cert Test Data
  GenerateRandomData ((UINT8 *) &Cert->Cert, MAX_CLIENT_CERTIFICATE_SIZE / sizeof (Cert->Cert[0]));
  Cert->CertSize = sizeof (Cert->Cert);
  GenerateRandomData ((UINT8 *) &Cert->LastCert, 8);
  return EFI_SUCCESS;
}