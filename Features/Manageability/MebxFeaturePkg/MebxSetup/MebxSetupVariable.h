/** @file
  Variable definition for MEBx Setup Configuration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _MEBX_SETUP_VARIABLE_H
#define _MEBX_SETUP_VARIABLE_H

#define ME_FW_VERSION_MAJOR_17       17

#define MAX_HASH_ENTRIES             33
#define HASHDATA_CHARACTERS          121

#define MAX_PASSWORD_SIZE            32
#define MAX_PASSWORD_SIZE_TERMINATED 33

#define MAX_IPV4_CHARACTERS          16
#define MAX_HOST_DOMAIN_NAME_LENGTH  256


/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
#define MEBX_CONFIGURATION_REVISION  1

typedef struct {
  UINT16       FwVersionMajor;
  // Settings
  UINT8        ShowError;
  UINT32       NetAccess;
  UINT8        MeOnHostSlpStates;
  UINT16       IdleTimeout;
  UINT32       PwdPolicy;
  UINT8        AmtState;
  UINT8        SolEnable;
  UINT8        StorageRedirEnable;
  UINT8        KvmEnable;
  UINT32       UserOptIn;
  UINT32       RemoteItOptInConfig;
  CHAR16       HostDomainName[MAX_HOST_DOMAIN_NAME_LENGTH];
  UINT8        FqdnType;
  UINT8        DhcpMode;
  CHAR16       Ipv4Addr[MAX_IPV4_CHARACTERS];
  CHAR16       Ipv4MaskAddr[MAX_IPV4_CHARACTERS];
  CHAR16       Ipv4GatewayAddr[MAX_IPV4_CHARACTERS];
  CHAR16       Ipv4PrefDnsAddr[MAX_IPV4_CHARACTERS];
  CHAR16       Ipv4AltDnsAddr[MAX_IPV4_CHARACTERS];
  CHAR16       ProvServerAddr[255];
  UINT16       ProvServerPort;
  UINT32       TlsPpkRemoteConfig;
  CHAR16       PkiDnsSuffix[223];
  UINT8        PrivacyLevel;
  UINT8        LanLessPlatform;
  UINT8        NumberOfPowerPkgs;
  UINT8        NoDefaultHashes; // IF one then we show "No Default Hashes" message
  UINT8        HashValid[MAX_HASH_ENTRIES];
  CHAR16       Pwd[MAX_PASSWORD_SIZE_TERMINATED];

  // Exposures
  UINT32       GlobalExposure;

  UINT32       AmtStateExposure;
  UINT32       AmtSubmenuExposure;
  UINT32       KvmExposure;
  UINT32       ProvisioningState;
  UINT32       OptInConfigExposure;
  UINT32       RemoteItOptInConfigExposure;
  UINT32       AddrMaskInvalid;

  // Read-only Provisioning record fields
  UINT8        ProvTlsMode;
  UINT32       SecureDns;
  UINT32       HostInitiated;
  UINT32       SelectedHashType;
  UINT16       SelectedHashData[79];       ///< 64 bytes of data + 15 hyphens
  UINT16       CaCertificateSerials[59];   ///< 48 bytes of data + 11 hyphens
  UINT32       AdditionalCaSerialNums;
  UINT32       IsOemDefault;
  UINT32       IsTimeValid;
  CHAR16       ProvRecordServerIp[30];
  EFI_HII_DATE Date;
  EFI_HII_TIME Time;
  CHAR16       ProvRecordServerFqdn[192];

  UINT32       CertQuantity;
  UINT32       CertHandle[MAX_HASH_ENTRIES];
  UINT32       CertActive[MAX_HASH_ENTRIES];
  UINT32       CertDefault[MAX_HASH_ENTRIES];
  UINT8        CertHashType[MAX_HASH_ENTRIES];
} MEBX_CONFIGURATION;

#endif // _MEBX_SETUP_VARIABLE_H
