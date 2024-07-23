/** @file
  WiFi Profile Sync functions for getting wifi profile from ASF

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

#include "WpsGetAsfProfile.h"
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtHeciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Gets the wifi profile name from ASF within the set timeout time.

  @param[in, out]  ProfileData    Stored profile details

  @return EFI_SUCCESS             Profiles returned
  @return EFI_NOT_FOUND           Timeout before profile name returned
  @return Others                  Error Occurred
**/
EFI_STATUS
GetProfileName (
  IN OUT WIFI_PROFILE_DATA    *ProfileData
  )
{
  EFI_STATUS  Status;
  UINT32      TimeoutCount;

  TimeoutCount = 0;

  ProfileData = (WIFI_PROFILE_DATA *) AllocateZeroPool (sizeof (WIFI_PROFILE_DATA));
  if (ProfileData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  while (TimeoutCount < ME_DELAY_COUNT) {
    Status = AsfWifiGetProfileName (ProfileData);
    if (!EFI_ERROR (Status) && (ProfileData->Status == AmtStatusSuccess)) {
      TimeoutCount = 0;
      break;
    }

    MicroSecondDelay (ME_WLAN_DELAY);
    TimeoutCount++;
    if ((TimeoutCount >= ME_DELAY_COUNT) || (ProfileData->Status != AmtStatusNotFound)) {
      DEBUG ((DEBUG_ERROR, "[%a] Failed to get profile name within time limit with status 0x%X\n", __FUNCTION__, ProfileData->Status));
      FreePool (ProfileData);
      return EFI_NOT_FOUND;
    }
  }
  return EFI_SUCCESS;
}

/**
  Gets Client certificate and key from ASF and saves to profile sync data structure.

  @param[in, out]  Profile          WiFi Connection Manager profile

  @return EFI_SUCCESS               Profile certificate added successfully
  @return EFI_INVALID_PARAMETER     Null pointer sent
  @return Others                    Error Occurred
**/
EFI_STATUS
GetClientCertAndKey (
  IN OUT WIFI_PROFILE   *Profile
  )
{
  EFI_STATUS    Status;

  if (Profile == NULL) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Profile->ProfileData->UserCredentialsClientCertificateAvailable != 0x01) {
    return EFI_SUCCESS;
  }

  // Get Certificate
  Profile->Cert = (WIFI_8021X_CLIENT_CERT *) AllocateZeroPool (sizeof (WIFI_8021X_CLIENT_CERT));
  if (Profile->Cert == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AsfWifiGetClientCertificate (Profile);
  if (EFI_ERROR (Status) || (Profile->Cert->Status != AmtStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to get client certificate with status 0x%x\n", __FUNCTION__, Profile->Cert->Status));
    FreePool (Profile->Cert);
    return Status;
  }

  // Get key
  Profile->Key = (WIFI_8021X_CLIENT_KEY *) AllocateZeroPool (sizeof (WIFI_8021X_CLIENT_KEY));
  if (Profile->Key == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AsfWifiGetClientKey (Profile);
  if (EFI_ERROR (Status) || (Profile->Key->Status != AmtStatusSuccess)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to get client key with status 0x%x\n", __FUNCTION__, Profile->Key->Status));
    FreePool (Profile->Cert);
    FreePool (Profile->Key);
  }
  return Status;
}

/**
  Gets Root CA certificate from ASF and saves to profile sync data structure.

  @param[in, out]  Profile          WiFi Connection Manager profile

  @return EFI_SUCCESS               Profile certificate added successfully
  @return EFI_INVALID_PARAMETER     Null pointer sent
  @return Others                    Error Occurred
**/
EFI_STATUS
GetRootCaClientCert (
  IN OUT WIFI_PROFILE   *Profile
  )
{
  EFI_STATUS    Status;

  if (Profile == NULL) {
    DEBUG ((DEBUG_WARN, "[%a] Invalid profile provided.\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Profile->ProfileData->RootCACertificateIndex == ROOT_CA_NOT_AVAILABLE) {
    return EFI_SUCCESS;
  }

  Profile->CaCert = (ROOT_CA_CERTIFICATE *) AllocateZeroPool (sizeof (ROOT_CA_CERTIFICATE));
  if (Profile->CaCert == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AsfGetRootCaCertificate (0, Profile->CaCert);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to get root CA certificate with status 0x%x\n", __FUNCTION__, Status));
    FreePool (Profile->CaCert);
  }
  return Status;
}
