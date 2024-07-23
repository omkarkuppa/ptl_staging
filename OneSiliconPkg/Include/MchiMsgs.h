/** @file
  MCHI (Manufacturing and Configuration Host Interface) HECI messages definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _MCHI_MSGS_H_
#define _MCHI_MSGS_H_

#include <HeciAddresses.h>

#pragma pack(1)

///
/// MCHI message header. MCHI messages are supported by Bringup and MCA processes.
///
typedef union {
  UINT32  Data;
  struct {
    UINT32  GroupId    : 8;
    UINT32  Command    : 7;
    UINT32  IsResponse : 1;
    UINT32  Reserved   : 8;
    UINT32  Result     : 8;
  } Fields;
} MCHI_MESSAGE_HEADER;

///
/// Defines for GroupID
///
#define MCHI_MCA_GROUP_ID            0x0A

///
/// Typedef for MCHI MCA Commands
///
typedef enum {
  McaCommitFilesCmd           = 0x4,
  McaGetRpmcStatusCmd         = 0x8,
  McaReadFileExCmd            = 0xA,
  McaSetFileExCmd             = 0xB,
  McaArbSvnCommitCmd          = 0x1B,
  McaArbSvnGetInfoCmd         = 0x1C,
  McaGetManufacturingInfoCmd  = 0x20
} MCA_COMMAND;

///
/// Typedef for Result field of MCHI Header
///
typedef enum {
  McaStatusSuccess                               = 0x0,
  McaStatusGlobalResetRequired                   = 0x1,
  McaStatusAccessDenied                          = 0x2,
  McaStatusNoFile                                = 0x3,
  McaStatusBadSeek                               = 0x4,
  McaStatusReadFail                              = 0x5,
  McaStatusCantMalloc                            = 0x6,
  McaStatusNoFileAttribute                       = 0x7,
  McaStatusFileNoManuf                           = 0x8,
  McaStatusManufNotAllowed                       = 0x9,
  McaStatusAfterLock                             = 0xA,
  McaStatusInvalidInput                          = 0xB,
  McaStatusResetFailed                           = 0xC,
  McaStatusSizeExceedLimit                       = 0xD,
  McaStatusInvalidAttrHash                       = 0xE,
  McaStatusFpfFileNotWritten                     = 0xF,
  McaStatusFpfInternal                           = 0x10,
  McaStatusFpfCanary                             = 0x11,
  McaStatusFpfFatal                              = 0x12,
  McaStatusFpfAlreadyCommitted                   = 0x13,
  McaStatusFpfEomNotSet                          = 0x14,
  McaStatusFpfCommitNotAllowed                   = 0x15,
  McaStatusNoPermission                          = 0x16,
  McaStatusNoQuota                               = 0x17,
  McaStatusGeneralError                          = 0x18,
  McaStatusCommitNotAllowed                      = 0x19,
  McaStatusWriteFail                             = 0x1A,
  McaStatusNvmBlocked                            = 0x1B,
  McaStatusResourceInUse                         = 0x1C,
  McaStatusNoResource                            = 0x1D,
  McaStatusUnlocked                              = 0x1E,
  McaStatusMaxSvnReached                         = 0x1F,
  McaStatusArbSvnDisabled                        = 0x20,
  McaStatusAttkbAfterEop                         = 0x21,
  McaStatusAttkbSbDisabled                       = 0x22,
  McaStatusAttkbNotProvisioned                   = 0x23,
  McaStatusAttkbMissingSeed                      = 0x24,
  McaStatusAttkbWrongSize                        = 0x25,
  McaStatusAttkbPlatformOpenToDebug              = 0x26,
  McaStatusStorageInvalidOffset                  = 0x30,
  McaStatusStorageInvalidSize                    = 0x31,
  McaStatusStorageAccessDenied                   = 0x32,
  McaStatusStorageDeviceError                    = 0x33,
  McaStatusStorageProxyNotFound                  = 0x34,
  McaStatusStoragePartitionNotPresent            = 0x35,
  McaStatusStorageNotSupported                   = 0x36,
  McaStatusNotSupported                          = 0x37,
  McaStatusBadSignatureInfo                      = 0x38,
  McaStatusSignatureVerificationFailed           = 0x39,
  McaStatusPubKeyMismatch                        = 0x3A,
  McaStatusResourceInUseByBios                   = 0x3B,
  McaStatusResourceInUseByOem                    = 0x3C,
  McaStatusVerifyMcaEomConfigWrongSize           = 0x45,
  McaStatusVerifyWrongMcaEom                     = 0x46,
  McaStatusVerifyWrongMcaSampleFile              = 0x47,
  McaStatusInvalidBtgConfiguration               = 0x48,
  McaStatusWrongAlgSet                           = 0x49,
  McaStatusVerifyPolicyCfgRulesMngStatusMismatch = 0x59,
  McaStatusVerifyPolicyCfgRulesWrongSize         = 0x5A,
  McaStatusVerifyPolicyCfgRulesWrongManufState   = 0x5B,
  McaStatusVerifyPolicyCfgRulesWrongData         = 0x5C,
  McaStatusVerifyWrongPolicySysintidWrongSize    = 0x5D,
  McaStatusInvalidUpidPsrConfiguration           = 0x5E,
  McaStatusVerifyWrongPolicySysIntIdWrongData    = 0x5F,
  McaStatusVerifyWrongPolicySysIntdAlreadySet    = 0x60,
  McaStatusVerifyWrongPolicyPwdBlob              = 0x61,
  McaStatusVerifyWrongGpioNvarSize               = 0x78,
  McaStatusVerifyWrongGpioFeature                = 0x79,
  McaStatusVerifyWrongGpioFeatureInUse           = 0x7A,
  McaStatusVerifyWrongGpioOwnMode                = 0x7B,
  McaStatusVerifyWrongGpioPadMode                = 0x7C,
  McaStatusVerifyWrongGpioPadAddrParam           = 0x7D,
  McaStatusVerifyWrongGpioPadAddrInUse           = 0x7E,
  McaStatusVerifySmipWrongSize                   = 0x80,
  McaStatusVerifyPreProvInvalidState             = 0x9C,
  McaStatusVerifyWrongPrivacyLevel               = 0x9D,
  McaStatusVerifyEhbcStatePrivLevelMismatch      = 0x9E,
  McaStatusVerifyWrongEhbcState                  = 0x9F,
  McaStatusVerifyWrongConfigServerFqdn           = 0xA0,
  McaStatusVerifyWrongPkiSuffix                  = 0xA1,
  McaStatusVerifyWrongRsb                        = 0xA2,
  McaStatusVerifyWrongAmtWdResetState            = 0xA3,
  McaStatusVerifyWrongIdleTimeout                = 0xA4,
  McaStatusVerifyWrongProvState                  = 0xA5,
  McaStatusVerifyWrongAllHashesState             = 0xA6,
  McaStatusVerifyWrongUcPolicy                   = 0xA7,
  McaStatusVerifyWrongRedirState                 = 0xA8,
  McaStatusVerifyWrongServerAddress              = 0xA9,
  McaStatusVerifyWrongServerPort                 = 0xAA,
  McaStatusVerifyWrongHostFqdn                   = 0xAB,
  McaStatusVerifyWrongHostFqdnHostname           = 0xAC,
  McaStatusVerifyWrongHostFqdnDomain             = 0xAD,
  McaStatusVerifyWrongCertHashValue              = 0xAE,
  McaStatusVerifyWrongCertHashHashAlgorithm      = 0xAF,
  McaStatusVerifyWrongCertHashNameLength         = 0xB0,
  McaStatusVerifyWrongCertHashHashValue          = 0xB1,
  McaStatusVerifyWrongCertHashHashLength         = 0xB2,
  McaStatusVerifyWrongSealPolicy                 = 0xB3,
  McaStatusVerifyWrongResealTimeout              = 0xB4,
  McaStatusVerifyWrongTbtOemConfigLength         = 0xC0,
  McaStatusVerifyWrongIommgOemConfigLength       = 0xC1,
  McaStatusVerifyWrongPavpConfigLength           = 0xD0,
  McaStatusVerifyWrongPavpLspconConfigValue      = 0xD1,
  McaStatusVerifyWrongPavpLspconConfigCount      = 0xD2,
  McaStatusVerifyWrongPavpEdpConfigValue         = 0xD3,
  McaStatusVerifyWrongPavpEdpConfigCount         = 0xD4,
  McaStatusVerifyLspconEdpPortConflict           = 0xD5,
  McaStatusVerifyPavpInternalError               = 0xD6,
  McaStatusManufVerifyNcsiSmbusExclusiveAddress  = 0xD7,
  McaStatusManufVerifyNcsiI2cExclusiveAddress    = 0xD8,
  McaStatusManufVerifyValFile1WrongData          = 0xF0,
  McaStatusManufVerifyValFile1WrongSize          = 0xF1,
  McaStatusManufVerifyValFile2WrongSize          = 0xF2,
  McaStatusManufVerifyValFile3WrongSize          = 0xF3,
  McaStatusManufVerifyWrongPseConfig1Length      = 0xF5,
  McaStatusManufVerifyWrongPseConfig2Length      = 0xF6,
  McaStatusManufVerifyWrongPseConfig3Length      = 0xF7,
  McaStatusManufVerifyWrongPseConfig4Length      = 0xF8,
  McaStatusManufVerifyWrongPseConfig5Length      = 0xF9
} MCA_STATUS;

///
/// MCHI_MCA_GROUP_ID Definitions
///

///
/// Commit Files
///
typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
} COMMIT_FILES;

///
/// Read File / Set File common definitions
///
#define MCA_DAM_FILE_SIZE             1
#define MCA_PLAT_ID_SUPPORT_FILE_SIZE 1
#define MCA_MAX_FILE_SIZE             4096

///
/// Typedef for MCHI MCA Files IDs
///
typedef enum {
  McaAmtSecPkiFileId        = 0x1001A000,
  McaAmtProvAdtRecFileId    = 0x1001A600,
  McaPlatIdSupportFileId    = 0x1001B900,
  McaBrandEntitlementFileId = 0x10025100,
  McaDamFileId              = 0x10050000,
  McaAmtProvStateFileId     = 0x10080200,
  McaAmtBiosRedirFileId     = 0x10080300,
  McaNphyFileId             = 0x100A0000,
  McaSphyFileId             = 0x1001F200
} MCA_FILE_ID;

typedef struct {
  UINT32  ICPS              : 1;  ///< ICPS
  UINT32  vPROEnterprise    : 1;  ///< VPRO Enterprise based platform
  UINT32  vPROEssentials    : 1;  ///< VPRO Essential based platform
  UINT32  Reserved1         : 1;  ///< Reserved
  UINT32  IntelBased        : 1;  ///< Intel Based platform
  UINT32  Reserved2         : 26; ///< Reserved
  UINT32  ICPS_ICM          : 1;  ///< ICPS ICM
} BRAND_ENTITLEMENT;

///
/// Read File Ex
///
typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
  UINT32              FileId;
  UINT32              Offset;
  UINT32              DataSize;
  UINT8               Flags;
} READ_FILE_EX_REQUEST;

typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
  UINT32              DataSize;
  UINT8               Data[];    ///< Data Buffer
} READ_FILE_EX_RESPONSE;

typedef union {
  READ_FILE_EX_REQUEST  Request;
  READ_FILE_EX_RESPONSE Response;
} READ_FILE_EX_BUFFER;

///
/// Set File Ex
///
typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
  UINT32              FileId;
  UINT32              Offset;
  UINT32              DataSize;
  UINT8               Flags;
  UINT8               Data[];     ///< Data Buffer
} SET_FILE_EX_REQUEST;

typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
} SET_FILE_EX_RESPONSE;

typedef union {
  SET_FILE_EX_REQUEST   Request;
  SET_FILE_EX_RESPONSE  Response;
} SET_FILE_EX_BUFFER;

///
/// Request Device Ownership
///
typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
} REQUEST_OWNERSHIP;

typedef union {
  REQUEST_OWNERSHIP   Request;
  REQUEST_OWNERSHIP   Response;
} REQUEST_OWNERSHIP_BUFFER;

///
/// HW Anti-Rollback
///

typedef enum {
  MFT_KEY_USAGE_INDEX_CSE_RBE_MANIFEST   = 3,  ///< RBE manifest
  MFT_KEY_USAGE_INDEX_UNDEFINED_MANIFEST = 255 ///< Undefined and maximum usage
} MFT_KEY_USAGE;

#define ARB_SVN_COMMIT_ALL                      0xFF ///< Value 0xFF refers to committing all pending ARBSVN values.
///
/// HW Anti-Rollback (ARB) Security Version Number Commit
///
typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
  UINT8               UsageId;
  UINT8               Reserved[3]; ///< Reserved for future use, to allow specifying explicit usage to be committed.
} ARB_SVN_COMMIT_REQUEST;

typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
} ARB_SVN_COMMIT_RESPONSE;

typedef union {
  ARB_SVN_COMMIT_REQUEST  Request;
  ARB_SVN_COMMIT_RESPONSE Response;
} ARB_SVN_COMMIT_BUFFER;

///
/// HW Anti-Rollback (ARB) Security Version Number Get Info
///
typedef struct {
  MCHI_MESSAGE_HEADER  MchiHeader;
} ARB_SVN_GET_INFO_REQUEST;

typedef struct {
  UINT8           UsageId;              ///< Key usage number according to FAS manifest.
  UINT8           BootPartitionId;      ///< For ME FW the SVN is the same for all partitions.
  UINT8           ExecutingSvn;         ///< The SVN value of the FW currently running.
  UINT8           MinAllowedSvn;        ///< The minimal SVN allowed on platform for given usage.
} ARB_SVN_INFO_ENTRY;

typedef struct {
  MCHI_MESSAGE_HEADER  MchiHeader;
  UINT32               NumEntries;       ///< Number of ARB SVN Entry
  ARB_SVN_INFO_ENTRY   ArbSvnEntry[0];
} ARB_SVN_GET_INFO_RESPONSE;

typedef union {
  ARB_SVN_GET_INFO_REQUEST  Request;
  ARB_SVN_GET_INFO_RESPONSE Response;
} ARB_SVN_GET_INFO_BUFFER;

///
/// Get Manufacturing Info
///
typedef union {
  UINT32  Data;
  struct {
    UINT32  EomTriggered    : 1;
    UINT32  SocConfigLocked : 1;
    UINT32  FlashProtected  : 1;
    UINT32  ManufLocked     : 1;
    UINT32  Emulation       : 1;
    UINT32  FlexibleEom     : 1;
    UINT32  Reserved        : 26;
  } Fields;
} MANUFACTURING_INFO;

typedef struct {
  MCHI_MESSAGE_HEADER        MchiHeader;
} GET_MANUFACTURING_INFO_REQUEST;

typedef struct {
  MCHI_MESSAGE_HEADER        MchiHeader;
  MANUFACTURING_INFO         ManufacturingInfo;
} GET_MANUFACTURING_INFO_RESPONSE;

typedef union {
  GET_MANUFACTURING_INFO_REQUEST  Request;
  GET_MANUFACTURING_INFO_RESPONSE Response;
} GET_MANUFACTURING_INFO_BUFFER;

///
/// Get RPMC Status
///
typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
} GET_RPMC_STATUS_REQUEST;

typedef struct {
  UINT32  DeviceConfiguration : 1;  ///< Device configuration in fuses. 0: SPI; 1: EC;
  UINT32  DeviceDetected      : 2;  ///< RPMC device detected and candidate for RPMC 00: SPI 0; 01: SPI 1; 10: EC 11: None
  UINT32  BindStatus          : 1;  ///< 0: Pre-bind; 1: Post-bind
  UINT32  BindCounter         : 4;  ///< FPF Bind Counter
  UINT32  RebindAvailable     : 4;  ///< Number of rebind available
  UINT32  CseArMode           : 1;  ///< CSE anti replay state: 0: RPMC; 1: PRTC
  UINT32  CseStatus           : 4;  ///< RPMC Status values for CSE
  UINT32  BiosArMode          : 1;  ///< BIOS anti replay state: 0: RPMC; 1: PRTC
  UINT32  BiosStatus          : 4;  ///< BIOS Status values for CSE
  UINT32  Reserved            : 10; ///< Reserved
  UINT32  Reserved2;                ///< Reserved for future use
} RP_STATUS_3;

typedef struct {
  MCHI_MESSAGE_HEADER MchiHeader;
  RP_STATUS_3         Status;
} GET_RPMC_STATUS_RESPONSE;

typedef union {
  GET_RPMC_STATUS_REQUEST   Request;
  GET_RPMC_STATUS_RESPONSE  Response;
} GET_RPMC_STATUS_BUFFER;

#pragma pack()

#endif // _MCHI_MSGS_H_
