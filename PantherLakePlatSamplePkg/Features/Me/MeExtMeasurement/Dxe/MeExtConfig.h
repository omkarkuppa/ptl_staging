/** @file
  Header file for Extending CSME Config to PCR[1].

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
#ifndef _EXTEND_CSME_CONFIG_H_
#define _EXTEND_CSME_CONFIG_H_

#include <Library/BaseLib.h>

#define TLS_CERT_PKI_MAX_FQDN_LENGTH                  256
#define CERT_HASH_MAX_LENGTH                          64
#define TLS_CERT_PKI_MAX_SERIAL_NUMS                  3
#define TLS_CERT_PKI_MAX_SERIAL_NUMS_EXTENDED         10
#define TLS_CERT_PKI_MAX_SERIAL_NUM_LENGTH            16
#define TLS_CERT_PKI_MAX_SERIAL_NUM_LENGTH_EXTENDED   20
#define TLS_LEAF_CERT_STRING_MAX_LENGTH               256
#define TLS_LEAF_CERT_STRING_EXPIRATION_DATE          16

#define  AMT_CONFIG_RECORD_LENGTH                     328  // Intel AMT Configuration Record Length as per ICL CSME Measured Boot FAS document
#define  AMT_CONFIG_SIG_LENGTH                        20   // Signature - 'IntelCSMEAmtConfig'
#define  SECURE_PKI_DNS_SUFFIX_LENGTH                 256  // Trusted NVAR Size for PKI DNS Suffix
#define  PKI_PROV_LEAF_CERT_HASH_FINGERPRINT_LENGTH   64   // PKI Provisioning leaf Certificate hash Fingerprint Max Size
#define  TM_PROTOCOL_MAX_ADDR_SIZE                    14

typedef INT32    TIME_T;    // Time in sec since 1 Jan 1970 0000 GMT

typedef enum {
  ProvisioningStatePre   = 0,
  ProvisioningStateIn    = 1,
  ProvisioningStatePost  = 2
} PT_PROVISIONING_STATE;

typedef struct {
  // 128-bit IPv6-formatted address in network byte order.
  union {
    UINT8     Ip6U8[16];
    UINT16    Ip6U16[8];
    UINT32    Ip6U32[4];
  } Ip6Addr;
} IN6_ADDR;

typedef struct {
  UINT8             Sin6_Len;          // Length (in bytes) of the sockaddr_in6 structure.
  UINT8             Sin6_Family;       // Address family.
  UINT16            Sin6_Port;         // Transport layer port number.
  UINT32            Sin6_FlowInfo;     // This field contains the IPv6 packet header Flow Label as the low-order 20 bits and Traffic Class as the next more
                                       // significant 8 bits. The high-order 4 bits of this field are reserved. This field allows the application to set the flow label
                                       // and traffic class associated with the socket via a call to bind or connect. ([RFC2460].R7:10, [IPV6REQ].R2.16:10)
  IN6_ADDR          Sin6_Addr;         // 128-bit IPv6-formatted address in network byte order.
  UINT32            Sin6_Scope_Id;     // sin6_scope_id is only used for local scope (i.e. link-local, site-local) unicast IPv6 addresses.
                                       // For global scope unicast IPv6 addresses, as well as for IPv4 addresses, this field is not used and should be set to 0.
} SOCKADDR_IN6;

typedef struct {
  UINT32        S_Addr;     // 32bit netid/hostid address in network byte order
} IN_ADDR;

typedef struct {
  UINT8         Sin_Len;        // length of struct sockaddr_in
  UINT8         Sin_Family;     // AF_INET
  UINT8         Sin_Port;       // 16bit Port Number in network byte order
  IN_ADDR       Sin_Addr;       // 32bit netid/hostid in network byte order
  UINT8         Sin_Zero [8];   // unused
} SOCKADDR_IN;

typedef struct {
  UINT8     Sa_Len;         // Total Length
  UINT8     Sa_Family;      // Address Family AF_xxx
  INT8      Sa_Data [TM_PROTOCOL_MAX_ADDR_SIZE];   // Up to TM_PROTOCOL_MAX_ADDR_SIZE bytes of protocol specific address
} SOCKADDR;

typedef struct {
  union
  {
    SOCKADDR_IN6     IpV6;
    SOCKADDR_IN      IpV4;
    SOCKADDR         IpSockAddr;   // For PC-LINT
  } Addr;
} SOCKADDR_STORAGE;

typedef struct {
  UINT8       Length;
  UINT8       Value [TLS_CERT_PKI_MAX_SERIAL_NUM_LENGTH_EXTENDED];
} CERT_SERIAL_NUMBER_T;

typedef UINT8 CERT_HASH_ALGORITHM;

typedef struct {
  UINT8                   HostInitiated;
  UINT8                   TLSDataValid;
  UINT8                   FQDN [TLS_CERT_PKI_MAX_FQDN_LENGTH];
  UINT8                   HashValue [CERT_HASH_MAX_LENGTH];
  UINT8                   CA [TLS_CERT_PKI_MAX_SERIAL_NUMS * TLS_CERT_PKI_MAX_SERIAL_NUM_LENGTH];
  UINT8                   HashAlgorithm   : 2;
  UINT8                   IsDefault       : 1;
  UINT8                   IntermidiateCAs : 1;
  UINT8                   Reserved0       : 1;
  UINT8                   SecureDNS       : 1;
  UINT8                   IsLocalConfig   : 1;
  UINT8                   RecordUpToDate  : 1; //  Indicates if this struct holds current provisioning or last provisioning
  TIME_T                  Time;                //  The time when we verify the leaf certificate
  SOCKADDR_STORAGE        IPAddress;           //  The IP that we accept for incoming connection
  BOOLEAN                 TimeValidityPass;
  CERT_SERIAL_NUMBER_T    CASerialNumbers [TLS_CERT_PKI_MAX_SERIAL_NUMS_EXTENDED];
  UINT8                   LeafIssuerDN [TLS_LEAF_CERT_STRING_MAX_LENGTH];
  UINT8                   LeafSubjectDN [TLS_LEAF_CERT_STRING_MAX_LENGTH];
  UINT8                   LeafExpirationDate [TLS_LEAF_CERT_STRING_EXPIRATION_DATE];
  CERT_HASH_ALGORITHM     LeafHashAlgorithm;
  UINT8                   LeafCertFingerprint [CERT_HASH_MAX_LENGTH];
} ADMIN_SETUP_AUDIT_RECORD;

typedef union {
  struct
  {
    UINT32   AmtEnabled           : 1;
    UINT32   MebxPwdChanged       : 1;
    UINT32   AmtGloballyEnabled   : 1;
    UINT32   NetAclSetFromRemote  : 1;
    UINT32   AdminNetAclUpdated   : 1;
    UINT32   Reserved12           : 2;
    UINT32   UnprovisionInProcess : 1;
    UINT32   ProvisioningState    : 2; // ADMIN_PROVISIONING_STATE_PRE = 0, ADMIN_PROVISIONING_STATE_IN  = 1, ADMIN_PROVISIONING_STATE_POST = 2
    UINT32   Reserved3            : 1; // former SMB bit
    UINT32   Reserved4            : 1; // former FWU bit
    UINT32   TimeSet              : 1;
    UINT32   Reserved5            : 2;
    UINT32   UUIDUpdated          : 1;
    UINT32   Reserved6            : 1;
    UINT32   Reserved7            : 1;
    UINT32   KerberosEnabled      : 1;
    UINT32   RevertingToDefault   : 1;
    UINT32   Reserved8            : 1;
    UINT32   ZTCEnabled           : 1; // 1-ZTC provisioning is enabled, 0-ZTC Provisioning is disabled
    UINT32   PkiCertCreated       : 1;
    UINT32   CertHashesSet        : 1;
    UINT32   ClientControlMode    : 1; //1-CCM, 0-ACM
    UINT32   ReInitInProcess      : 1;
    UINT32   Reserved             : 6;
  } Bits;
  UINT32   All;
} PROV_DATA_STATE;

typedef struct {
  UINT32      IderHostIf  : 1;   // 1 - IDER is enabled from HostIf
  UINT32      IderPtni    : 1;   // 1 - IDER is enabled from PTNI
  UINT32      SolHostIf   : 1;   // 1 - SOL is enabled from HostIf
  UINT32      SolPtni     : 1;   // 1 - SOL is enabled from PTNI
  UINT32      KvmHostIf   : 1;   // 1 - KVM is enabled from HostIf
  UINT32      KvmPtni     : 1;   // 1 - KVM is enabled from PTNI
  UINT32      Reserved    : 26;
} BIOS_REDIRECTION_SETTINGS;

typedef struct {
  UINT16      AmtGloballyEnabled  : 1;  ///< Bit 0 - AMT Globally Enabled
  UINT16      MebxPwdSet          : 1;  ///< Bit 1 - MEBx Password Changed
  UINT16      AmtProvisioned      : 1;  ///< Bit 2 - Provisioning State
  UINT16      AmtProvisioningMode : 1;  ///< BIT 3 - AMT Provision Mode
  UINT16      ZtcEnabled          : 1;  ///< Bit 4 - ZTC Enabled
  UINT16      Reserved1           : 3;  ///< Bit 5-7 - Reserved
  UINT16      KvmEnabled          : 1;  ///< Bit 8 - KVM Enabled
  UINT16      SolEnabled          : 1;  ///< Bit 9 - SOL Enabled
  UINT16      UsbrEnabled         : 1;  ///< Bit 10 - USBr Enabled
  UINT16      Reserved2           : 5;  ///< Bit 11-15 - Reserved
} AMT_STATUS;

typedef struct {
  AMT_STATUS           AmtStatus;
  UINT16               Reserved;
  UINT8                SecPkiDnsSfx [SECURE_PKI_DNS_SUFFIX_LENGTH];
  UINT32               PkiProvLeafCertHashAlgm;
  UINT8                PkiProvLeafCertHashFingerPrint [PKI_PROV_LEAF_CERT_HASH_FINGERPRINT_LENGTH];
} AMT_CONFIG_DATA;

typedef struct {
  UINT8     Signature [AMT_CONFIG_SIG_LENGTH];
  UINT32    DataLength;
  UINT8     ConfigData [AMT_CONFIG_RECORD_LENGTH];
} INTEL_ME_CONFIG_DATA;

#endif // _EXTEND_CSME_CONFIG_H_
