/** @file
  AMTHI Messages

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#ifndef _AMTHI_MSGS_H_
#define _AMTHI_MSGS_H_

#include <HeciAddresses.h>

#pragma pack(1)

///
/// AMT Status Enum
///
typedef enum {
  AmtStatusSuccess                       = 0,
  AmtStatusInternalError                 = 1,
  AmtStatusNotReady                      = 2,
  AmtStatusInvalidPtMode                 = 3,
  AmtStatusInvalidMessageLength          = 4,
  AmtStatusTableFingerprintNotAvailable  = 5,
  AmtStatusIntegrityCheckFailed          = 6,
  AmtStatusUnsupportedIsvsVersion        = 7,
  AmtStatusInvalidRegistrationData       = 9,
  AmtStatusApplicationDoesNotExist       = 10,
  AmtStatusNotEnoughStorage              = 11,
  AmtStatusInvalidName                   = 12,
  AmtStatusBlockDoesNotExist             = 13,
  AmtStatusInvalidByteOffset             = 14,
  AmtStatusInvalidByteCount              = 15,
  AmtStatusNotPermitted                  = 16,
  AmtStatusNotOwner                      = 17,
  AmtStatusBlockLockedByOther            = 18,
  AmtStatusBlockNotLocked                = 19,
  AmtStatusInvalidGroupPermissions       = 20,
  AmtStatusGroupDoesNotExist             = 21,
  AmtStatusInvalidMemberCount            = 22,
  AmtStatusMaxLimitReached               = 23,
  AmtStatusInvalidAuthType               = 24,
  AmtStatusAuthenticationFailed          = 25,
  AmtStatusInvalidDhcpMode               = 26,
  AmtStatusInvalidIpAddress              = 27,
  AmtStatusInvalidDomainName             = 28,
  AmtStatusUnsupportedVersion            = 29,
  AmtStatusRequestUnexpected             = 30,
  AmtStatusInvalidTableType              = 31,
  AmtStatusInvalidProvisioningMode       = 32,
  AmtStatusUnsupportedObject             = 33,
  AmtStatusInvalidTime                   = 34,
  AmtStatusInvalidIndex                  = 35,
  AmtStatusInvalidParameter              = 36,
  AmtStatusInvalidNetmask                = 37,
  AmtStatusFlashWriteLimitExceeded       = 38,
  AmtStatusProposeAnotherVersion         = 41,
  AmtStatusInvalidPidFormat              = 42,
  AmtStatusInvalidPpsFormat              = 43,
  AmtStatusBistCommandBlocked            = 44,
  AmtStatusConnectionFailed              = 45,
  AmtStatusConnectionTooMany             = 46,
  AmtStatusRngGenerationInProgress       = 47,
  AmtStatusRngNotReady                   = 48,
  AmtStatusCertificateNotReady           = 49,
  AmtStatusAuditRecordNotPrersent        = 50,
  AmtStatusRedirectionInProgress         = 51,
  AmtStatusIpSyncEnabled                 = 52,
  AmtStatusRemoteAccessNotGranted        = 512,
  AmtStatusRemoteAccessHostVpnIsDisabled = 513,
  AmtStatusSuccessWithoutDdns            = 514,
  AmtStatusSuccessWithDdns               = 515,
  AmtStatusDisabledByPolicy              = 1024,
  AmtStatusNetworkIfErrorBase            = 2048,
  AmtStatusInvalidHandle                 = 2053,
  AmtStatusInvalidPassword               = 2054,
  AmtStatusDataMissing                   = 2057,
  AmtStatusDuplicate                     = 2058,
  AmtStatusUnsupported                   = 2066,
  AmtStatusNotFound                      = 2068,
  AmtStatusNoAssociation                 = 2072,
  AmtStatusAuditFail                     = 2075,
  AmtStatusBlockingComponent             = 2076,
  AmtStatusDestinationUnreachable        = 2077,
  AmtStatusUpdateInProgress              = 2078,
  AmtStatusInvalidId                     = 2079,
  AmtStatusDeleted                       = 2080,
  AmtStatusNoData                        = 2081,
  AmtStatusIpv6InterfaceDisabled         = 2500,
  AmtStatusInterfaceDoesNotExist         = 2501,
  AmtStatusIpv6IllegalInterfaceId        = 2502,
  AmtStatusIpv6IllegalStaticAddress      = 2503,
  AmtStatusIpv6IllegalRouterAddress      = 2504,
  AmtStatusIpv6IllegalDnsAddress         = 2505,
  AmtStatusSdkDefinedErrorBase           = 4096
} AMT_STATUS;

///
/// AMTHI Commands Enum
///
typedef enum {
  AmthiCfgGetInterfaceVersion                  = 0x04000000,
  AmthiCfgGetTcpipParameters                   = 0x04000006,
  AmthiCfgSetTcpipParameters                   = 0x04000007,

  AmthiCfgUnprovision                          = 0x04000010,
  AmthiCfgGetProvisioningState                 = 0x04000011,
  AmthiCfgSetSolStorageRedirStatus             = 0x04000014,

  AmthiCfgGetEndOfPostState                    = 0x04000020,
  AmthiCfgPartialUnprovision                   = 0x04000024,
  AmthiCfgGetProvisioningTlsMode               = 0x0400002B,
  AmthiCfgEnumerateHashHandles                 = 0x0400002C,
  AmthiCfgGetCertificateHashEntry              = 0x0400002D,
  AmthiCfgSetPkiFqdnSuffix                     = 0x0400002F,

  AmthiCfgGetZtcEnabledStatus                  = 0x04000030,
  AmthiCfgGetIdleTimeout                       = 0x04000031,
  AmthiCfgSetIdleTimeout                       = 0x04000032,
  AmthiCfgGetPkiFqdnSuffix                     = 0x04000036,
  AmthiCfgSetCertificateHashState              = 0x04000037,
  AmthiCfgAddCustomizedCertificateHashEntry    = 0x04000038,
  AmthiCfgDeleteCustomizedCertificateHashEntry = 0x04000039,
  AmthiCfgSetZeroTouchEnabled                  = 0x0400003A,

  AmthiCfgOpenUserInitiatedConnection          = 0x04000044,
  AmthiCfgCloseUserInitiatedConnection         = 0x04000045,
  AmthiCfgGetRemoteAccessConnectionStatus      = 0x04000046,
  AmthiCfgGetLanInterfaceSettings              = 0x04000048,
  AmthiCfgSetMebxPwdChangePolicy               = 0x0400004B,
  AmthiCfgGetMebxPwdChangePolicy               = 0x0400004C,
  AmthiCfgGetConfigServerData                  = 0x0400004E,
  AmthiCfgSetConfigServerData                  = 0x0400004F,

  AmthiCfgGetMeSetupAuditRecord                = 0x04000050,
  AmthiCfgGetIpv6LanInterfaceStatus            = 0x04000052,
  AmthiCfgSetIpv6LanInterfaceSettings          = 0x04000054,
  AmthiCfgGetFqdn                              = 0x04000056,
  AmthiCfgSetFqdn                              = 0x04000057,
  AmthiCfgGetRedirectionState                  = 0x04000058,
  AmthiCfgCompleteConfiguration                = 0x0400005A,
  AmthiCfgStopConfiguration                    = 0x0400005E,

  AmthiCfgStartConfigurationEx                 = 0x04000062,
  AmthiCfgGetUnprovisioningState               = 0x04000068,
  AmthiCfgSetMebxState                         = 0x0400006A,
  AmthiCfgGetControlMode                       = 0x0400006B,
  AmthiCfgSetOptInState                        = 0x0400006C,
  AmthiCfgGetOptInState                        = 0x0400006D,
  AmthiCfgSetKvmEnabled                        = 0x0400006E,
  AmthiCfgGetKvmEnabled                        = 0x0400006F,

  AmthiCfgGetPrivacyLevel                      = 0x04000075,
  AmthiCfgGetOptInChangeAvailable              = 0x04000076,
  AmthiCfgUiAction                             = 0x04000077,
  AmthiCfgGetGfxUiInfo                         = 0x04000078,
  AmthiCfgGetGfxFragment                       = 0x04000079,
  AmthiCfgEnumeratePowerPackages               = 0x04000080,
  AmthiCfgGetCurrentPowerPackage               = 0x04000081,
  AmthiCfgGetPowerPackageInfo                  = 0x04000082,
  AmthiCfgSetPowerPackage                      = 0x04000083
} AMTHI_COMMAND;

#define AMTHI_VERSION_MINOR                          0x01
#define AMTHI_VERSION_MAJOR                          0x01

typedef struct {
  UINT8 MajorVersion;
  UINT8 MinorVersion;
} AMTHI_VERSION;

#define AMTHI_MESSAGE_BYTE_COUNT_MAP(a) (a-sizeof(AMTHI_HEADER))

typedef struct {
  AMTHI_VERSION Version;
  UINT16        Reserved;
  UINT32        Command;
  UINT32        Length;
} AMTHI_HEADER;

#define AMTHI_COMMAND_IS_RESPONSE   BIT23

typedef struct {
  AMTHI_VERSION Version;
  UINT16        Reserved;
  UINT32        Command;
  UINT32        Length;
  UINT32        Status;
} AMTHI_RESP_HEADER;

///
/// Get Interface Version
///
typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_GET_INTERFACE_VERSION_RESPONSE;

///
/// Get / Set IPV4 Settings
///
#define MAXSEG      4

typedef enum {
  DhcpModeNone = 0,
  DhcpModeDisabled,
  DhcpModeEnabled
} DHCP_MODE;

typedef union {
  UINT32 Address;
  UINT8  SegAddr[MAXSEG];
} IP_ADDR;

#define MAX_ASCII_STRING         192
typedef struct {
  UINT16 Length;
  UINT8  Buffer[MAX_ASCII_STRING];
} IMB_ANSI_STRING;

#define MAX_STRING_LENGTH_FQDN 256
typedef struct {
  UINT16 Length;
  UINT8  Buffer[MAX_STRING_LENGTH_FQDN];
} FQDN_ANSI_STRING;

#define MAX_STRING_LENGTH_FQDN_SUFFIX 224
typedef struct {
  UINT16 Length;
  UINT8  Buffer[MAX_STRING_LENGTH_FQDN_SUFFIX];
} FQDN_SUFFIX_ANSI_STRING;

typedef struct {
  UINT32          DhcpMode;
  UINT32          LocalAddr;
  UINT32          SubnetMask;
  UINT32          GatewayAddr;
  UINT32          PriDnsAddr;
  UINT32          SecDnsAddr;
  IMB_ANSI_STRING DomainName;
} IMB_TCPIP_PARAMS;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_TCPIP_PARAMETERS_REQUEST;

typedef struct {
  AMTHI_HEADER     Header;
  UINT32           Status;
  IMB_TCPIP_PARAMS TcpipParams;
} CFG_GET_TCPIP_PARAMETERS_RESPONSE;

typedef union {
  CFG_GET_TCPIP_PARAMETERS_REQUEST  Request;
  CFG_GET_TCPIP_PARAMETERS_RESPONSE Response;
} CFG_GET_TCPIP_PARAMETERS_BUFFER;

typedef struct {
  AMTHI_HEADER     Header;
  IMB_TCPIP_PARAMS TcpipParams;
} CFG_SET_TCPIP_PARAMETERS;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_TCPIP_PARAMETERS_RESPONSE;

///
/// Unprovision
///
typedef struct {
  AMTHI_HEADER Header;
  UINT32       Mode;
} CFG_UNPROVISION_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_UNPROVISION_RESPONSE;

///
/// Get Provision Status
///
typedef enum {
  ProvisioningStatePre          = 0,
  ProvisioningStateIn           = 1,
  ProvisioningStatePost         = 2
} PT_PROVISIONING_STATE;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       ProvisioningState;
} CFG_GET_PROVISIONING_STATE_RESPONSE;

///
/// Set Sol/Storage Redirection
///
typedef struct {
  AMTHI_HEADER Header;
  UINT32       SolEnable;
  UINT32       StorageRedirEnable;
} CFG_SET_SOL_STORAGE_REDIR_STATE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_SOL_STORAGE_REDIR_STATE_RESPONSE;

///
/// Get Eop State
///
typedef enum {
  EopStatusBoot = 0,
  EopStatusMebx = 1,
  EopStatusPost = 2
} CFG_EOP_STATE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_EOP_STATE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT8        EopState;
} CFG_GET_EOP_STATE_RESPONSE;

typedef union {
  CFG_GET_EOP_STATE_REQUEST  Request;
  CFG_GET_EOP_STATE_RESPONSE Response;
} CFG_GET_EOP_STATE_BUFFER;

///
/// Partial Unprovision
///
typedef enum {
  ProvisioningModeNone = 0,
  ProvisioningModeEnterprise,
  ProvisioningModeReserved,
  ProvisioningModeRemoteConnectivityService
} AMTHI_PROVISION_MODE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_PARTIAL_UNPROVISION_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_PARTIAL_UNPROVISION_RESPONSE;

///
/// Certificate Hash Operations:
/// Enumerate Hash Entries/Get Cert Hash Entry/Set Cert Hash State/
/// Add Customized Cert Hash/Delete Customized Cert Hash
///
#define MAX_HASH_ENTRIES 33

#define CERT_NAME_MAX_ASCII_STRING 32 + 1 ///< Certificate name + null termination

typedef struct {
  UINT16 Length;
  UINT8  Buffer[CERT_NAME_MAX_ASCII_STRING];
} CERT_NAME;

typedef struct {
  UINT32    CertDefault;
  UINT32    CertActive;
  UINT8     CertHash[64];
  UINT8     HashAlgorithm;
  CERT_NAME CertName;
} CERT_HASH_ENTRY;

typedef struct {
  AMTHI_HEADER Header;
} CFG_ENUMERATE_HASH_HANDLES_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       EntriesCount;
  UINT32       Handles[MAX_HASH_ENTRIES];
} CFG_ENUMERATE_HASH_HANDLES_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Handle;
} CFG_GET_CERTHASH_ENTRY_REQUEST;

typedef struct {
  AMTHI_HEADER    Header;
  UINT32          Status;
  CERT_HASH_ENTRY CertHashEntry;
} CFG_GET_CERTHASH_ENTRY_RESPONSE;

typedef union {
  CFG_GET_CERTHASH_ENTRY_REQUEST  Request;
  CFG_GET_CERTHASH_ENTRY_RESPONSE Response;
} CFG_GET_CERTHASH_ENTRY_BUFFER;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Handle;
  UINT32       IsActive;
} CFG_SET_CERTHASH_STATE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_CERTHASH_STATE_RESPONSE;

typedef enum {
  CertHashAlgorithmMd5    = 0,
  CertHashAlgorithmSha1   = 1,
  CertHashAlgorithmSha256 = 2,
  CertHashAlgorithmSha384 = 3,
  CertHashAlgorithmSha224 = 4,
  CertHashAlgorithmSha512 = 5,
} CERT_HASH_ALGORITHM;

#define MAX_CUSTOMIZED_HASHES 3
typedef struct {
  AMTHI_HEADER    Header;
  UINT32          IsActive;
  UINT8           CertificateHash[64];
  UINT8           HashAlgorithm;
  IMB_ANSI_STRING Name;
} CFG_ADD_CERTHASH_ENTRY_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       Handle;
} CFG_ADD_CERTHASH_ENTRY_RESPONSE;

typedef union {
  CFG_ADD_CERTHASH_ENTRY_REQUEST  Request;
  CFG_ADD_CERTHASH_ENTRY_RESPONSE Response;
} CFG_ADD_CERTHASH_ENTRY_BUFFER;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Handle;
} CFG_DELETE_CERTHASH_ENTRY_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_DELETE_CERTHASH_ENTRY_RESPONSE;

///
/// Get/Set PKI FQDN Suffix
///
#define MAX_ADDRESS_LENGTH 256

typedef struct {
  AMTHI_HEADER            Header;
  FQDN_SUFFIX_ANSI_STRING Suffix;
} CFG_SET_PKI_FQDN_SUFFIX_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_PKI_FQDN_SUFFIX_RESPONSE;

typedef struct {
  AMTHI_HEADER            Header;
  UINT32                  Status;
  FQDN_SUFFIX_ANSI_STRING Suffix;
} CFG_GET_PKI_FQDN_SUFFIX_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_PKI_FQDN_SUFFIX_REQUEST;

///
/// Get/Set ZTC Enabled Status
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_ZTC_ENABLED_STATUS_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       Enable;
} CFG_GET_ZTC_ENABLED_STATUS_RESPONSE;

typedef union {
  CFG_GET_ZTC_ENABLED_STATUS_REQUEST  Request;
  CFG_GET_ZTC_ENABLED_STATUS_RESPONSE Response;
} CFG_GET_ZTC_ENABLED_STATUS_BUFFER;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Mode;
} CFG_SET_ZTC_MODE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_ZTC_MODE_RESPONSE;

///
/// Get/Set Idle Timeout Request
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_IDLE_TIMEOUT_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       IdleTimeout;
} CFG_GET_IDLE_TIMEOUT_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       IdleTimeout;
} CFG_SET_IDLE_TIMEOUT_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_IDLE_TIMEOUT_RESPONSE;

///
/// Open/Close/Query User Initiatied Connection for CIRA
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_OPEN_USER_INITIATED_CONNECTION_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_OPEN_USER_INITIATED_CONNECTION_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_CLOSE_USER_INITIATED_CONNECTION_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_CLOSE_USER_INITIATED_CONNECTION_RESPONSE;

typedef enum {
  AmtNetworkConnectionDirect = 0,
  AmtNetworkConnectionVpn,
  AmtNetworkConnectionOutsideEnterprise,
  AmtNetworkConnectionOutsideUnknown
} AMT_NETWORK_CONNECTION_STATUS;

typedef enum {
  RemoteAccessConnectionStatusNotConnected = 0,
  RemoteAccessConnectionStatusConnecting,
  RemoteAccessConnectionStatusConnected
} REMOTE_ACCESS_CONNECTION_STATUS;

typedef struct {
  UINT32           AmtNetworkConnectionStatus;
  UINT32           RemoteAccessConnectionStatus;
  UINT32           RemoteAccessConnectionTrigger;
  FQDN_ANSI_STRING MpsHostname;
} REMOTE_ACCESS_CONNECTION_STRUCT;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_REMOTE_ACCESS_CONNECTION_STATUS_REQUEST;

typedef struct {
  AMTHI_HEADER     Header;
  UINT32           Status;
  UINT32           AmtNetworkConnectionStatus;
  UINT32           RemoteAccessConnectionStatus;
  UINT32           RemoteAccessConnectionTrigger;
  FQDN_ANSI_STRING MpsHostname;
} CFG_GET_REMOTE_ACCESS_CONNECTION_STATUS_RESPONSE;

///
/// Get LAN Interface Settings
///
typedef enum {
  LanInterfaceWired    = 0,
  LanInterfaceWireless = 1
} LAN_INTERFACE;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       InterfaceIndex;
} CFG_GET_LAN_INTERFACE_SETTINGS_REQUEST;

typedef struct {
  UINT32 Enabled;
  UINT32 Ipv4Address;
  UINT32 DhcpEnabled;
  UINT8  DhcpIpMode;
  UINT8  LinkStatus;
  UINT8  MacAddress[6];
} LAN_INTERFACE_SETTINGS;

typedef struct {
  AMTHI_HEADER           Header;
  UINT32                 Status;
  LAN_INTERFACE_SETTINGS InterfaceSettings;
} CFG_GET_LAN_INTERFACE_SETTINGS_RESPONSE;

typedef union {
  CFG_GET_LAN_INTERFACE_SETTINGS_REQUEST  Request;
  CFG_GET_LAN_INTERFACE_SETTINGS_RESPONSE Response;
} CFG_GET_LAN_INTERFACE_SETTINGS_BUFFER;

///
/// Get/Set MEBx Password Change Policy
///
typedef enum {
  PwdChangePolicyOnlyDefault   = 0,
  PwdChangePolicyBlockInPost   = 1,
  PwdChangePolicyAlwaysAllowed = 2
} CFG_MEBX_PWD_CHANGE_POLICY;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_MEBX_PWD_CHANGE_POLICY_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       Policy;
} CFG_GET_MEBX_PWD_CHANGE_POLICY_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Policy;
} CFG_SET_MEBX_PWD_CHANGE_POLICY_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_MEBX_PWD_CHANGE_POLICY_RESPONSE;

///
/// Get/Set Config Server Data
///
#define CFG_IPV6_ADDR_LEN_MAX                       46

typedef struct {
  UINT8 Address[CFG_IPV6_ADDR_LEN_MAX];
} CFG_IP_ADDR;

typedef struct {
  UINT16           ServerPort;
  CFG_IP_ADDR      ServerAddr;
  FQDN_ANSI_STRING Fqdn;
} CONFIG_SERVER_DATA;

typedef struct {
  AMTHI_HEADER       Header;
  UINT32             Status;
  CONFIG_SERVER_DATA ConfigServerData;
} CFG_GET_CONFIG_SERVER_DATA_RESPONSE;

typedef struct {
  AMTHI_HEADER       Header;
  CONFIG_SERVER_DATA ConfigServerData;
} CFG_SET_CONFIG_SERVER_DATA_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_CONFIG_SERVER_DATA_RESPONSE;

typedef union {
  CFG_SET_CONFIG_SERVER_DATA_REQUEST  Request;
  CFG_SET_CONFIG_SERVER_DATA_RESPONSE Response;
} CFG_SET_CONFIG_SERVER_DATA_BUFFER;

///
/// Get Me Setup Audit Record
///
typedef enum {
  ProvisioningTlsModePki = 0
} PROVISIONING_TLS_MODE;

typedef struct {
  UINT16 Year;
  UINT16 Month;
  UINT16 DayOfWeek;
  UINT16 Day;
  UINT16 Hour;
  UINT16 Minute;
  UINT16 Second;
} TIME_DATE;

typedef struct {
  UINT8           ProvisioningTlsMode;
  UINT32          SecureDns;
  UINT32          HostInitiated;
  UINT32          SelectedHashType;
  UINT8           SelectedHashData[64];
  UINT8           CaCertificateSerials[48];
  UINT32          AdditionalCaSerialNums;
  UINT32          IsOemDefault;
  UINT32          IsTimeValid;
  CFG_IP_ADDR     ProvServerIp;
  TIME_DATE       TlsStartTime;
  IMB_ANSI_STRING ProvServerFqdn;
} ME_ADMIN_AUDIT_RECORD;

typedef struct {
  AMTHI_HEADER          Header;
  UINT32                Status;
  ME_ADMIN_AUDIT_RECORD AuditRecord;
} CFG_GET_AUDIT_RECORD_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_AUDIT_RECORD_REQUEST;

///
/// Get/Set IPV6 LAN Interface Settings
///
#define MAX_INTERFACE_ID 17

typedef struct {
  CHAR8 InterfaceId[MAX_INTERFACE_ID];
} CFG_IPV6_INTERFACE_ID;

typedef enum {
  Ipv6AddrTypeLinkLocal  = 0,
  Ipv6AddrTypeRouterConf = 1,
  Ipv6AddrTypeDhcp       = 2,
  Ipv6AddrTypeManual     = 3,
  Ipv6AddrTypeUnknown    = 4
} CFG_IPV6_ADDR_TYPE;

typedef enum {
  Ipv6AddrStateTentative  = 0,
  Ipv6AddrStatePreferred  = 1,
  Ipv6AddrStateDeprecated = 2,
  Ipv6AddrStateValid      = 3,
  Ipv6AddrStateInvalid    = 4,
  Ipv6AddrStateCollision  = 5,
  Ipv6AddrStateUnknown    = 6
} CFG_IPV6_ADDR_STATE;

typedef struct {
  CFG_IP_ADDR         Address;
  CFG_IPV6_ADDR_TYPE  Type;
  CFG_IPV6_ADDR_STATE State;
} CFG_IPV6_ADDR_INFO;

typedef struct {
  UINT32                Ipv6Enabled;
  UINT32                InterfaceIdType;
  CFG_IPV6_INTERFACE_ID InterfaceId;
  CFG_IP_ADDR           Ipv6Address;
  CFG_IP_ADDR           Ipv6DefaultRouter;
  CFG_IP_ADDR           DnsPrimaryIpv6Address;
  CFG_IP_ADDR           DnsSecondaryIpv6Address;
} CFG_IPV6_LAN_INTERFACE_SETTINGS;

#define MAX_IPV6_ADDRESS_NUM                        10

typedef struct {
  CFG_IP_ADDR        Ipv6DefaultRouter;
  CFG_IP_ADDR        PrimaryDns;
  CFG_IP_ADDR        SecondaryDns;
  UINT32             NumOfAddr;
  CFG_IPV6_ADDR_INFO Ipv6Addresses[MAX_IPV6_ADDRESS_NUM];
} IPV6_LAN_INTERACE_STRUC;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       InterfaceIndex;
} CFG_GET_IPV6_LAN_INTERFACE_STATUS_REQUEST;

typedef struct {
  AMTHI_HEADER            Header;
  UINT32                  Status;
  IPV6_LAN_INTERACE_STRUC Ipv6LanIntfStrc;
} CFG_GET_IPV6_LAN_INTERFACE_STATUS_RESPONSE;

typedef union {
  CFG_GET_IPV6_LAN_INTERFACE_STATUS_REQUEST  Request;
  CFG_GET_IPV6_LAN_INTERFACE_STATUS_RESPONSE Response;
} CFG_GET_IPV6_LAN_INTERFACE_STATUS_BUFFER;

typedef struct {
  AMTHI_HEADER                    Header;
  UINT32                          InterfaceIndex;
  CFG_IPV6_LAN_INTERFACE_SETTINGS Ipv6Settings;
} CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_RESPONSE;

typedef union {
  CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_REQUEST  Request;
  CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_RESPONSE Response;
} CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_BUFFER;

///
/// Get/Set Fqdn
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_FQDN_REQUEST;

typedef struct {
  UINT32           SharedFqdn;
  UINT32           Reserved[3];
  UINT32           HostNameLength;
  FQDN_ANSI_STRING Fqdn;
} FQDN_DATA;

typedef struct {
  AMTHI_HEADER     Header;
  UINT32           Status;
  FQDN_DATA        FqdnData;
} CFG_GET_FQDN_RESPONSE;

typedef union {
  CFG_GET_FQDN_REQUEST  Request;
  CFG_GET_FQDN_RESPONSE Response;
} CFG_GET_FQDN_BUFFER;

typedef struct {
  AMTHI_HEADER     Header;
  FQDN_DATA        FqdnData;
} CFG_SET_FQDN_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_FQDN_RESPONSE;

///
/// Get Redirection Features State
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_REDIRECTION_STATE_REQUEST;

typedef struct {
  UINT32 SolState          : 1;
  UINT32 StorageRedirState : 1;
  UINT32 KvmState          : 1;
  UINT32 Reserved0         : 29;
} REDIRECTION_FEATURES_STATE;

typedef struct {
  AMTHI_HEADER               Header;
  UINT32                     Status;
  REDIRECTION_FEATURES_STATE RedirectionFeatureStates;
} CFG_GET_REDIRECTION_STATE_RESPONSE;

///
/// Complete Configuration
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_COMPLETE_CONFIGURATION_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_COMPLETE_CONFIGURATION_RESPONSE;

///
/// Stop/Start Configuration
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_STOP_CONFIGURATION_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_STOP_CONFIGURATION_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       IPv6Enable;
} CFG_START_CONFIGURATION_EX_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_START_CONFIGURATION_EX_RESPONSE;

typedef union {
  CFG_START_CONFIGURATION_EX_REQUEST  Request;
  CFG_START_CONFIGURATION_EX_RESPONSE Response;
} CFG_START_CONFIGURATION_EX_BUFFER;

///
/// Get Unprovisioning State
///
typedef enum {
  UnprovisioningStateNone = 0,
  UnprovisioningStateIn   = 1
} CFG_UNPROVISIONING_STATE;

typedef struct {
  AMTHI_HEADER             Header;
  UINT32                   Status;
  CFG_UNPROVISIONING_STATE UnprovisioningState;
} CFG_GET_UNPROVISIONING_STATE_RESPONSE;

///
/// Set MEBx state
///
typedef enum {
  AmtMebxStateEnter = 0,
  AmtMebxStateExit  = 1
} AMT_MEBX_STATE;

typedef struct {
  AMTHI_HEADER   Header;
  AMT_MEBX_STATE MebxState;
} CFG_SET_MEBX_STATE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_MEBX_STATE_RESPONSE;

typedef union {
  CFG_SET_MEBX_STATE_REQUEST  Request;
  CFG_SET_MEBX_STATE_RESPONSE Response;
} CFG_SET_MEBX_STATE_BUFFER;

///
/// Get Control Mode
///
typedef enum {
  AmtControlModeNone = 0,
  AmtControlModeUser,
  AmtControlModeAdmin
} AMT_CONTROL_MODE;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       ControlMode;  // returned upon success only
} CFG_GET_CONTROL_MODE_RESPONSE;

///
/// Get/Set OptIn State
///

// optin policy bitmap
typedef enum {
  OptinPolicyBitmapNone    = 0x1,
  OptinPolicyBitmapKvmOnly = 0x2,
  OptinPolicyBitmapAll     = 0x4
} OPTIN_POLICY_BITMAP;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       OptInState;
  UINT32       RemoteOptInState;
} CFG_SET_OPTIN_STATE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_OPTIN_STATE_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_OPTIN_STATE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       OptInState;
  UINT32       RemoteOptInState;
} CFG_GET_OPTIN_STATE_RESPONSE;

///
/// Get/Set KVM State
///
typedef struct {
  AMTHI_HEADER Header;
  UINT32       Enabled;
} CFG_SET_KVM_ENABLED_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_KVM_ENABLED_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_KVM_ENABLED_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       Enabled;
} CFG_GET_KVM_ENABLED_RESPONSE;

///
/// Get AMT Privacy Level
///
typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_PRIVACY_LEVEL_REQUEST;

typedef enum {
  PrivacyLevelDefault = 1,
  PrivacyLevelEnhanced,
  PrivacyLevelExtreme
} AMT_PRIVACY_LEVEL;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       PrivacyLevel;
} CFG_GET_PRIVACY_LEVEL_RESPONSE;

///
/// Get OptIn Change Availability
///
typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       OptInPolicy;
  UINT32       OptInRemoteEnabled;
} CFG_GET_OPTIN_CHANGE_AVAILABLE_RESPONSE;

typedef enum {
  NullImageId                     = 0,
  ImageIdExitUi                   = 1,
  ImageIdUserConsentCodeDialog    = 256,
  ImageIdUserConsentGrantedDialog = 257
} AMT_IMAGE_ID;

///
/// Get UI Action
///
typedef enum {
  UserInputNoInput,
  UserInputEsc,
  UserInputEnter,
  UserInputOtherKey
} AMT_MEBX_USER_INPUT;

typedef struct {
  AMTHI_HEADER        Header;
  AMT_IMAGE_ID        CurrentImageId;
  AMT_MEBX_USER_INPUT UserInput;
} CFG_UI_ACTION_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       NextScreen;
} CFG_UI_ACTION_RESPONSE;

typedef union {
  CFG_UI_ACTION_REQUEST  Request;
  CFG_UI_ACTION_RESPONSE Response;
} CFG_UI_ACTION_BUFFER;

///
/// Get GFX UI Info/Fragment
///
typedef struct {
  UINT16  Width;
  UINT16  Height;
} AMT_SCREEN_RESOLUTION;

typedef struct {
  AMTHI_HEADER          Header;
  AMT_SCREEN_RESOLUTION ScreenResolution;
  AMT_IMAGE_ID          ImageId;
} CFG_GET_GFX_UI_INFO_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       ImageDataLength;
  UINT16       UpperLeftXPos;
  UINT16       UpperLeftYPos;
} CFG_GET_GFX_UI_INFO_RESPONSE;

typedef union {
  CFG_GET_GFX_UI_INFO_REQUEST  Request;
  CFG_GET_GFX_UI_INFO_RESPONSE Response;
} CFG_GET_GFX_UI_INFO_BUFFER;

typedef struct {
  AMTHI_HEADER Header;
  AMT_IMAGE_ID ImageId;
  UINT32       ByteOffset;
  UINT32       ByteSize;
} CFG_GET_GFX_FRAGMENT_REQUEST;

#define AMT_UI_MAX_DATA_SIZE                        0x400

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       ByteSize;                   ///<Returned upon success only
  UINT8        Data[AMT_UI_MAX_DATA_SIZE]; ///<Returned upon success only
} CFG_GET_GFX_FRAGMENT_RESPONSE;

typedef union {
  CFG_GET_GFX_FRAGMENT_REQUEST  Request;
  CFG_GET_GFX_FRAGMENT_RESPONSE Response;
} CFG_GET_GFX_FRAGMENT_BUFFER;

///
/// Enumerate Power Packages
/// Get/Set Active Power Package
//  Get Power Package Info
///
#define MAX_PWR_PKG_COUNT     3
#define MAX_PWR_PKG_DESC_SIZE 64

typedef struct {
  AMTHI_HEADER Header;
} CFG_ENUMERATE_POWER_PACKAGES_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT32       NumOfGuids;               ///< Returned upon success only
  EFI_GUID     Guids[MAX_PWR_PKG_COUNT]; ///< Returned upon success only
} CFG_ENUMERATE_POWER_PACKAGES_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
} CFG_GET_CURRENT_POWER_PACKAGE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  EFI_GUID     Guid; ///< Returned upon success only
} CFG_GET_CURRENT_POWER_PACKAGE_RESPONSE;

typedef struct {
  AMTHI_HEADER Header;
  EFI_GUID     Guid;
} CFG_GET_POWER_PACKAGE_INFO_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
  UINT16       Length;
  CHAR8        Description[MAX_PWR_PKG_DESC_SIZE];
} CFG_GET_POWER_PACKAGE_INFO_RESPONSE;

typedef union {
  CFG_GET_POWER_PACKAGE_INFO_REQUEST  Request;
  CFG_GET_POWER_PACKAGE_INFO_RESPONSE Response;
} CFG_GET_POWER_PACKAGE_INFO_BUFFER;

typedef struct {
  AMTHI_HEADER Header;
  EFI_GUID     Guid;
} CFG_SET_POWER_PACKAGE_REQUEST;

typedef struct {
  AMTHI_HEADER Header;
  UINT32       Status;
} CFG_SET_POWER_PACKAGE_RESPONSE;

#pragma pack()

#endif // _AMTHI_MSGS_H
