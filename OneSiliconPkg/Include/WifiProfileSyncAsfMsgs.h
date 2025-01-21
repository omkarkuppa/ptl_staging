/** @file
  Supporting functions for profile sync driver ASF communication.

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
#ifndef _WIFI_PROFILE_SYNC_ASF_MSGS_H_
#define _WIFI_PROFILE_SYNC_ASF_MSGS_H_

#include "AsfMsgs.h"

// Root CA index status if no root CA avaliable
#define ROOT_CA_NOT_AVAILABLE     0xFFFFFFFF

//
// AMT WiFi profile data defines
//
#define MAC_ADDR_SIZE                 6
#define PROFILE_NAME_SIZE             33
#define SSID_SIZE                     32
#define PSK_KEY_SIZE                  64
#define USER_CRED_IDENTITY_SIZE       81
#define USER_CRED_DOMAIN_SIZE         129
#define USER_CRED_USERNAME_SIZE       129
#define USER_CRED_PASSWORD_SIZE       33
#define SERVER_CERT_NAME_SIZE         81
#define MAX_CLIENT_KEY_SIZE           6500
#define MAX_CLIENT_CERTIFICATE_SIZE   4100

#pragma pack(push, 1)
///
/// ASF Message header with profile name char array as data
///
typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;                      ///< ASF Message Header
  CHAR8                           Data[PROFILE_NAME_SIZE];        ///< ASF Data Request
} ASF_PROFILE_MESSAGE_HEADER;

///
/// Get Wifi profile name response from ASF
///
typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;                      ///< ASF Message Header
  UINT32                          Status;                         ///< ASF Response Message Status
  CHAR8                           ProfileName[PROFILE_NAME_SIZE]; ///< ASF Return WiFi Profile Name.
} GET_WIFI_PROFILE_NAME_RESPONSE;

///
/// ASF buffer for Wifi profile name
///
typedef union {
  ASF_MESSAGE_HEADER              Request;                        ///< ASF Request
  GET_WIFI_PROFILE_NAME_RESPONSE  Response;                       ///< ASF Response
} GET_WIFI_PROFILE_NAME_BUFFER;

///
/// Get Wifi profile data response from ASF
///
typedef struct {
  UINT32  Status;
  UINT8   MACAddress[MAC_ADDR_SIZE];
  CHAR8   ProfileName[PROFILE_NAME_SIZE];
  UINT8   SSIdLength;
  CHAR8   SSId[SSID_SIZE];
  UINT32  EncryptionMethod;
  UINT32  AuthenticationMethod;
  UINT8   PskKeyType;
  UINT8   PskKeyLen;
  UINT8   PskKey[PSK_KEY_SIZE];
//
//  The following fields are related to 802.1x Authentication:
//  (see EDKII_8021XProfile)
//
  UINT32  AuthenticationProtocol_eapMethod;
  UINT32  AuthenticationProtocolInnerMethod;
  CHAR8   UserCredentialsRoamingIdentity[USER_CRED_IDENTITY_SIZE];
  CHAR8   UserCredentialsDomain[USER_CRED_DOMAIN_SIZE];
  CHAR8   UserCredentialsUsername[USER_CRED_USERNAME_SIZE];
  CHAR8   UserCredentialsPassword[USER_CRED_PASSWORD_SIZE];
  UINT8   UserCredentialsClientCertificateAvailable;
  UINT32  RootCACertificateIndex;
  CHAR8   ServerCertificateName[SERVER_CERT_NAME_SIZE];
  UINT16  ServerCertificateNameComparison;
} WIFI_PROFILE_DATA;

typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;                      ///< ASF Message Header
  WIFI_PROFILE_DATA               Profile;                        ///< ASF Return WiFi Profile Data.
} GET_WIFI_PROFILE_DATA_RESPONSE;

///
/// ASF buffer for Wifi profile data
///
typedef union {
  ASF_PROFILE_MESSAGE_HEADER      Request;                        ///< ASF Request
  GET_WIFI_PROFILE_DATA_RESPONSE  Response;                       ///< ASF Response
} GET_WIFI_PROFILE_DATA_BUFFER;

///
/// Get Wifi profile specific 8021X client certificate from ASF
///
typedef struct {
  UINT32  Status;
  UINT32  CertSize;
  UINT8   Certificate[MAX_CLIENT_CERTIFICATE_SIZE];
} WIFI_8021X_CLIENT_CERT;

typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;                      ///< ASF Message Header
  WIFI_8021X_CLIENT_CERT          Cert;                           ///< ASF Return WiFi Client Certificate.
} GET_WIFI_CLIENT_CERT_RESPONSE;

///
/// ASF buffer for Wifi profile client certificate
///
typedef union {
  ASF_PROFILE_MESSAGE_HEADER      Request;                        ///< ASF Request
  GET_WIFI_CLIENT_CERT_RESPONSE   Response;                       ///< ASF Resposne
} GET_WIFI_CLIENT_CERT_BUFFER;

///
/// Get Wifi profile specific 8021X client key from ASF
///
typedef struct {
  UINT32  Status;
  UINT32  KeySize;
  UINT8   Key[MAX_CLIENT_KEY_SIZE];
} WIFI_8021X_CLIENT_KEY;

typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;                      ///< ASF Message Header
  WIFI_8021X_CLIENT_KEY           Key;                            ///< ASF Return WiFi Client Key.
} GET_WIFI_CLIENT_KEY_RESPONSE;

///
/// ASF buffer for Wifi profile client key
///
typedef union {
  ASF_PROFILE_MESSAGE_HEADER      Request;                        ///< ASF Request
  GET_WIFI_CLIENT_KEY_RESPONSE    Response;                       ///< ASF Response
} GET_WIFI_CLIENT_KEY_BUFFER;

//
//  WiFi profile data structure recieved from ASF
//
typedef struct {
  WIFI_PROFILE_DATA       *ProfileData;
  WIFI_8021X_CLIENT_CERT  *Cert;
  WIFI_8021X_CLIENT_KEY   *Key;
  ROOT_CA_CERTIFICATE     *CaCert;
} WIFI_PROFILE;

#pragma pack(pop)

#endif
