/** @file
  WiFi Profile Sync functional definitions for getting wifi profile from ASF

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

#ifndef _WPS_GET_ASF_PROFILE_H_
#define _WPS_GET_ASF_PROFILE_H_

#include <PiDxe.h>
#include "WifiProfileSyncConfig.h"
#include <WifiProfileSyncAsfMsgs.h>

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
  );

/**
  Gets Client certificate and key from ASF and saves to profile sync data structure.

  @param[in, out]  WcmProfile       WiFi Connection Manager profile

  @return EFI_SUCCESS               Profile certificate added successfully
  @return EFI_INVALID_PARAMETER     Null pointer sent
  @return Others                    Error Occurred
**/
EFI_STATUS
GetClientCertAndKey (
  IN OUT WIFI_PROFILE   *Profile
  );

/**
  Gets Root CA certificate from ASF and saves to profile sync data structure.

  @param[in, out]  WcmProfile       WiFi Connection Manager profile

  @return EFI_SUCCESS               Profile certificate added successfully
  @return EFI_INVALID_PARAMETER     Null pointer sent
  @return Others                    Error Occurred
**/
EFI_STATUS
GetRootCaClientCert (
  IN OUT WIFI_PROFILE   *Profile
  );

#endif // _WPS_GET_ASF_PROFILE_H_
