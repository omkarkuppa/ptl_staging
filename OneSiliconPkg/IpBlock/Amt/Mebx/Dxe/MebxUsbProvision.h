/** @file
  This files contains API definition to get and remove USB provision record.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2005 Intel Corporation.

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

#ifndef _MEBX_USB_PROVISION_H_
#define _MEBX_USB_PROVISION_H_

#include <Protocol/SimpleFileSystem.h>
#include <AmthiMsgs.h>

#define PROVISION_FILE_NAME L"\\setup.bin"

#define FILE_TYPE_1_UUID \
  { \
    0x71FB16B5, 0xCB87, 0x4AF9, { 0xB4, 0x41, 0xCA, 0x7B, 0x38, 0x35, 0x78, 0xF9 } \
  }

#define FILE_TYPE_2_UUID \
  { \
    0x5881B296, 0x6BCF, 0x4C72, { 0x8B, 0x91, 0xA1, 0x5E, 0x51, 0x2E, 0x99, 0xC4 } \
  }

#define FILE_TYPE_3_UUID \
  { \
    0xC6F6F7A7, 0xC489, 0x47F6, { 0x93, 0xED, 0xE2, 0xE5, 0x02, 0x0D, 0xA5, 0x1D } \
  }

#define FILE_TYPE_4_UUID \
  { \
    0x5234A9AA, 0x29E1, 0x44A9, { 0x8D, 0x4D, 0x08, 0x1C, 0x07, 0xB9, 0x63, 0x53 } \
  }

#define USB_DATACHUNK_SIZE              512
#define MODULE_LIST_AREA_SIZE           118 * 2
//
// File flags are only valid from version 2 and up
//
#define FILE_FLAGS_DONT_CONSUME_RECORDS 0x01

#pragma pack(1)

#define MAJOR_VERSION_1  0x01
#define MAJOR_VERSION_2  0x02
#define MAJOR_VERSION_3  0x03
#define MAJOR_VERSION_4  0x04

#define MINOR_VERSION_0  0x00
#define MINOR_VERSION_1  0x01

typedef struct {
  EFI_GUID  Guid;
  UINT8     MajorVersion;
  UINT8     MinorVersion;
} PROVISION_FILE_VERSION;

typedef struct {
  EFI_GUID FileTypeUuid;
  UINT16   RecordChunkCount;
  UINT16   RecordHeaderByteCount;
  UINT32   RecordNumber;
  UINT8    MajorVersion;
  UINT8    MinorVersion;
  UINT16   FileFlags;
  UINT32   DataRecordCount;
  UINT32   DataRecordsConsumed;
  UINT16   DataRecordChunkCount;
  UINT16   Reserved;
  UINT16   ModuleList[MODULE_LIST_AREA_SIZE]; ///< List of modules ids, padded to 512 bytes.
} PROVISION_FILE_HEADER;

#define DATA_RECORD_FLAG_VALID  1

#define UNSCRAMBLE              0
#define SCRAMBLE                1
#define SCRAMBLE_VALUE          17

typedef union {
  UINT32 Data;
  struct {
    UINT32 Valid     : 1;  //0 - Record is valid
    UINT32 Scrambled : 1;  //0 - Record is scrambled
    UINT32 Rsvd      : 30; //1-31 - Rsvd
  } Bits;
} RECORDFLAGS;

typedef struct {
  UINT32      RecordTypeIdentifier;
  RECORDFLAGS RecordFlags;
  UINT32      Reserved[2];
  UINT16      RecordChunkCount;
  UINT16      RecordHeaderByteCount;
  UINT32      RecordNumber;
} PROVISION_DATA_RECORD_HEADER;

typedef struct {
  UINT16 ModuleId;
  UINT16 VariableId;
  UINT16 VariableLength;
  UINT16 Reserved;
  UINT8  VariableValue[];
} DATA_RECORD_ENTRY;

typedef struct {
  PROVISION_DATA_RECORD_HEADER  Header;
  UINT8                         Data[]; ///< Data Record Entries
} PROVISION_DATA_RECORD;

typedef struct {
  BOOLEAN                HandleValid;
  PROVISION_DATA_RECORD  *ProvisionRecordBuffer;
  UINT32                 ProvisionRecordSize;
  EFI_FILE_HANDLE        ProvisionFileHandle;
} PROVISION_RECORD_HANDLE;

#define MAX_ASCII_PSWD_LENGTH                      32
#define WAIT_FOR_FW_FEATURE_DEFAULT_TIMEOUT        15       ///< timeout in seconds
#define PT_CONSOLE_DELAY                           2000000  ///< 2 second delay

//
// New ANSI String for FQDN
//
#define MAX_HOSTNAME_LENGTH                        64
#define MAX_DOMAINNAME_LENGTH                      256
#define MAX_STRING_LENGTH                          256

//
// Host Based Provisioning
//
#define OPT_IN_NONE                                0x00000000
#define OPT_IN_KVM                                 0x00000001
#define USB_TOOL_OPT_IN_ALL                        0x0FF
#define OPT_IN_ALL                                 0xFFFFFFFF
#define OPT_IN_ALL_SELECTION                       0x02

#define AMT_ENABLE_DELAY                           3000000
#define AMTHI_COMPLETE_CONFIG_TIMEOUT              15  // 15s timeout for getting successful ACK when doing ManualSetup

#define ME_FW_CAPS_MANAGEABILITY_SUPP_DISABLED     0
#define ME_FW_CAPS_MANAGEABILITY_SUPP_AMT_ENABLED  1

///
/// Data Record Entry definitions
///

//Module identifiers
typedef enum {
  ModuleIdInvalid    = 0,
  ModuleIdMeKernel   = 1,
  ModuleIdIntelAmtCm = 2
} MODULE_ID;

//ME Kernel Variable identifiers
typedef enum {
  MeKernelVariableIdInvalid        = 0,
  MeKernelVariableIdCurrentMebxPwd = 1,
  MeKernelVariableIdNewMebxPwd     = 2,
  MeKernelVariableIdMngFeatureSel  = 3,
  MeKernelVariableIdLocalFwUpdate  = 4, ///< deprecated
  MeKernelVariableIdReserved       = 5,
  MeKernelVariableIdPowerPkgs      = 6
} ME_KERNEL_VARIABLE_ID;

//Intel AMT Variable identifiers
typedef enum {
  IntelAmtVariableIdInvalid                   = 0,
  IntelAmtVariableIdPid                       = 1,
  IntelAmtVariableIdPps                       = 2,
  IntelAmtVariableIdPkiDnsSuffix              = 3,
  IntelAmtVariableIdConfigServerFqdn          = 4,
  IntelAmtVariableIdZeroTouchSetupEnable      = 5,
  IntelAmtVariableIdPreinstalledCertEnable    = 6,
  IntelAmtVariableIdUserDefinedCertsConfig    = 7,
  IntelAmtVariableIdUserDefinedCertAdd        = 8,
  IntelAmtVariableIdSolStorageRedirConfig     = 10,
  IntelAmtVariableIdHostname                  = 11,
  IntelAmtVariableIdDomainName                = 12,
  IntelAmtVariableIdDhcp                      = 13,
  IntelAmtVariableIdIdleTimeout               = 15,
  IntelAmtVariableIdProvisioningServerAddress = 17,
  IntelAmtVariableIdProvisioningServerPort    = 18,
  IntelAmtVariableIdStaticIpv4Parameters      = 19,
  IntelAmtVariableIdPasswordPolicyFlag        = 21,
  IntelAmtVariableIdIpv6Settings              = 22,
  IntelAmtVariableIdSharedFqdn                = 23,
  IntelAmtVariableIdDdnsUpdate                = 24,
  IntelAmtVariableIdKvmState                  = 25,
  IntelAmtVariableIdKvmOptinUser              = 26,
  IntelAmtVariableIdKvmOptinRemoteIt          = 27,
  IntelAmtVariableIdMeProvisioningHalt        = 28,
  IntelAmtVariableIdManualSetupAndConfig      = 29
} INTEL_AMT_VARIABLE_ID;

#define DISABLE_ALL               0
#define ENABLE_ALL                1
#define DELETE_ALL                2

#define IPV4_BINARY_ADDRESS_SIZE  4
#define IPV6_BINARY_ADDRESS_SIZE  16
#define INTERFACE_ID_SIZE         17

typedef struct {
  UINT8 InterfaceIndex;
  UINT8 IPv6Enabled;
  UINT8 InterfaceIdType;
  UINT8 InterfaceId[INTERFACE_ID_SIZE];
  UINT8 IPv6Address[IPV6_BINARY_ADDRESS_SIZE];
  UINT8 IPv6DefaultRouter[IPV6_BINARY_ADDRESS_SIZE];
  UINT8 DnsPrimaryIPv6Address[IPV6_BINARY_ADDRESS_SIZE];
  UINT8 DnsSecondaryIPv6Address[IPV6_BINARY_ADDRESS_SIZE];
} IPv6_DATARECORD;

#define MAX_SHA_SIZE              48

typedef struct {
  UINT8           AlgorithmType;
  UINT8           CertHash[MAX_SHA_SIZE];
  IMB_ANSI_STRING CertName;
} USER_DEFINED_CERT_ADD;

#pragma pack()

/**
  This function searches file system for valid provision file
  and extract first valid provision record.

  @param[out] ProvisionRecordHandle  Pointer to provision record.

  @retval TRUE   Provision record found
  @retval FALSE  Provision record not found
**/
BOOLEAN
GetUsbProvisionRecord (
  OUT PROVISION_RECORD_HANDLE  *ProvisionRecordHandle
  );

/**
  Mark provision record as consumed if provision file
  allows consuming and MEBx consumed the record.
  This function should be called after MEBx invocation.

  This function is also responsible for cleanup of all of the
  resources allocated during initial data record discovery.
  After call to this function ProvisionRecord is no longer valid and
  shouldn't be used.

  @param[in] ProvisionRecordHandle  Provision record to consume
  @param[in] ReturnedDataRecord     Provision data record as returned from MEBx.
**/
VOID
RemoveUsbProvisionRecord (
  IN PROVISION_RECORD_HANDLE  *ProvisionRecordHandle,
  IN VOID                     *ReturnedDataRecord
  );

/**
  This function validates the USB Provisioning data record and parses through
  each data entry and passes the data to ME/AMT FW.

  @param[in] MebxUsbRecordPtr     Pointer to the USB Provisioning record

  @retval EFI_SUCCESS             USBProvisioning was successful
  @retval Others                  USB Provisioning has failed
**/
EFI_STATUS
MebxOneTouchProvisioning (
  IN VOID *MebxUsbRecordPtr
  );

#endif // _MEBX_USB_PROVISION_H_
