/** @file
  BUP Messages - HECI messages exchanged in pre-DID phase with CSME FW

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _BUP_MSGS_H
#define _BUP_MSGS_H

#include <MkhiMsgs.h>
#include <MeBiosPayloadData.h>

#pragma pack(1)

///
/// BUP group IDs
///
#define BUP_COMMON_GROUP_ID  0xF0
#define BUP_ICC_GROUP_ID     0xF1
#define BUP_MPHY_GROUP_ID    0xF2
#define BUP_PM_GROUP_ID      0xF3
#define BUP_BIOS_AR_GROUP_ID 0xF4

///
/// Defines for Command
///
///
/// Defines for BUP_COMMON_GROUP_ID Command
///
#define DRAM_INIT_DONE_CMD       0x01
#define MBP_REQUEST_CMD          0x02
#define ME_ENABLE_CMD            0x03
#define HMRFPO_DISABLE_CMD       0x04
#define TERMINATE_HECI_CMD       0x05
#define GET_IMR_SIZE_CMD         0x0C
#define MANUF_RESET_AND_HALT_CMD 0x0E
#define BUP_GET_ME_TYPE          0x11
#define GET_ER_LOG_CMD           0x1B
#define DATA_CLEAR_CMD           0x20
#define GET_IP_FIRMWARE_CMD      0x21
#define GET_DEBUG_TOKEN_DATA_CMD 0x22
#define GET_BIOS_SEED_CMD        0x23
#define LOAD_BINARY_CMD          0x26

///
/// Defines for BUP_MPHY_GROUP_ID Command
///
#define ICC_CMD  0x01
///
/// Defines for BUP_MPHY_GROUP_ID Command
///
#define MPHY_READ_FROM_MPHY_CMD 0x2
///
/// Defines for BUP_PM_GROUP_ID Command
///
#define NPCR_NOTIFICATION_CMD  0x1

///
/// Defines for BUP_BIOS_AR_GROUP_ID Command
///
#define BUP_MKHI_READ_MC_CMD       0x01
#define BUP_MKHI_INCREMENT_MC_CMD  0x02

///
/// Common group definitions
///

///
/// DRAM Init Done definitions
///

///
/// Memory Init Status codes
///
#define BIOS_MSG_DID_SUCCESS              0
#define BIOS_MSG_DID_NO_MEMORY            0x1
#define BIOS_MSG_DID_INIT_ERROR           0x2

typedef struct {
  UINT32  BiosImrsBaLow;
  UINT32  BiosImrsBaHigh;
} DRAM_INIT_DONE_IMRS_REQ_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER          MkhiHeader;
  DRAM_INIT_DONE_IMRS_REQ_DATA ImrData;
  UINT8                        Reserved1[6];
  UINT8                        MemStatus;
  UINT8                        Rs3BusNumber;
} DRAM_INIT_DONE_CMD_REQUEST;

///
/// BIOS Reset Action Codes
///
#define DID_ACK_NON_PCR       0x1
#define DID_ACK_PCR           0x2
#define DID_ACK_GRST          0x3

typedef struct {
  UINT32  AdrLow;
  UINT32  AdrHigh;
} PCI_2_PRIV_BASE;

typedef struct {
  MKHI_MESSAGE_HEADER          MkhiHeader;
  UINT32                       Reserved1[4];
  PCI_2_PRIV_BASE              Pci2PrivBase;
  UINT8                        Reserved2;
  UINT8                        BiosAction;
  UINT8                        Reserved3[6];
} DRAM_INIT_DONE_CMD_RESPONSE;

typedef union {
  DRAM_INIT_DONE_CMD_REQUEST    Request;
  DRAM_INIT_DONE_CMD_RESPONSE   Response;
} DRAM_INIT_DONE_CMD_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              SkipMbp;
} GET_MBP_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  MBP_HEADER          MbpHeader;
  UINT32              MbpItems[MAX_MBP_ITEMS_SIZE];
} GET_MBP_RESPONSE;

typedef union {
  GET_MBP_REQUEST   Request;
  GET_MBP_RESPONSE  Response;
} GET_MBP_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} ME_ENABLE_CMD_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} ME_ENABLE_CMD_RESPONSE;

typedef union {
  ME_ENABLE_CMD_REQUEST   Request;
  ME_ENABLE_CMD_RESPONSE  Response;
} ME_ENABLE_CMD_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} HMRFPO_DISABLE_CMD_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} HMRFPO_DISABLE_CMD_RESPONSE;

typedef union {
  HMRFPO_DISABLE_CMD_REQUEST  Request;
  HMRFPO_DISABLE_CMD_RESPONSE Response;
} HMRFPO_DISABLE_CMD_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              Reserved;
} TERMINATE_HECI_CMD_REQUEST;

///
/// Get IMR size message
///
typedef struct {
  UINT32        BiosImrDisableMask0;
  UINT32        BiosImrDisableMask1;
  UINT32        Pci2PrivSize;
} GET_IMR_CMD_REQ_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  GET_IMR_CMD_REQ_DATA  Data;
} GET_IMR_CMD_REQUEST;

typedef struct {
  UINT32        ImrsSize;
  UINT32        Alignment;
  UINT32        Flags;
} GET_IMR_CMD_RESP_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER    MkhiHeader;
  GET_IMR_CMD_RESP_DATA  Data;
} GET_IMR_CMD_RESPONSE;

typedef union {
  GET_IMR_CMD_REQUEST   Request;
  GET_IMR_CMD_RESPONSE  Response;
} GET_IMR_CMD_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} MANUF_RESET_AND_HALT_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} MANUF_RESET_AND_HALT_RESPONSE;

typedef union {
  MANUF_RESET_AND_HALT_REQUEST    Request;
  MANUF_RESET_AND_HALT_RESPONSE   Response;
} MANUF_RESET_AND_HALT_BUFFER;

typedef struct {
  UINT32 FwType    : 3;
  UINT32 FwSubType : 8;
  UINT32 Reserved  : 21;
} FW_TYPE_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER    MkhiHeader;
  FW_TYPE_DATA           FwTypeData;
} GEN_GET_FW_TYPE_RESPONSE;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} GEN_GET_FW_TYPE_REQUEST;

typedef union {
  GEN_GET_FW_TYPE_REQUEST   Request;
  GEN_GET_FW_TYPE_RESPONSE  Response;
} GEN_GET_FW_TYPE_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  UINT8                 FirstLogEntryIndex;
  UINT16                Flags;
  UINT8                 Reserved;
} GET_ER_LOG_CMD_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  UINT8                 MeasuredBootEnabled;
  UINT8                 FirstLogEntryIndex;
  UINT8                 LastLogEntryIndex;
  UINT8                 RemainingEntries;
  UINT16                DataLength;
  UINT8                 LogData[]; // Data buffer
} GET_ER_LOG_CMD_RESPONSE;

typedef union {
  GET_ER_LOG_CMD_REQUEST  Request;
  GET_ER_LOG_CMD_RESPONSE Response;
} GET_ER_LOG_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              State;
} SET_EDEBUG_MODE_STATE_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} SET_EDEBUG_MODE_STATE_RESPONSE;

typedef union {
  SET_EDEBUG_MODE_STATE_REQUEST   Request;
  SET_EDEBUG_MODE_STATE_RESPONSE  Response;
} SET_EDEBUG_MODE_STATE_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT32              Reserved;
} DATA_CLEAR_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} DATA_CLEAR_RESPONSE;

typedef union {
  DATA_CLEAR_REQUEST    Request;
  DATA_CLEAR_RESPONSE   Response;
} DATA_CLEAR_BUFFER;

#define MAX_DIGEST_SIZE                   64
#define IP_LOADING_CMD_VERSION            1

typedef struct {
  MKHI_MESSAGE_HEADER                     MkhiHeader;
  UINT32                                  Version;
  UINT32                                  Operation;
  UINT32                                  DramBaseLow;
  UINT32                                  DramBaseHigh;
  UINT32                                  MemorySize;
  UINT32                                  Reserved;
} IP_LOADING_CMD_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER                     MkhiHeader;
  UINT32                                  PayloadSize;
  UINT32                                  Reserved[2];
  UINT32                                  Status;
  UINT8                                   HashType;
  UINT8                                   Hash[];
} IP_LOADING_CMD_RESPONSE;

typedef union {
  IP_LOADING_CMD_REQUEST    Request;
  IP_LOADING_CMD_RESPONSE   Response;
} IP_LOADING_CMD_BUFFER;

/**
  Enable Debug Transports specifying Power Controls.
    0000: Use existing setup settings</b>
    0001 Enable DCI, USB3DbC, TraceHub is enabled and trace is active.
    0010 Enable DCI, TraceHub is enabled
    0011 Enable DCI, TraceHub is power-off
    0100: Disable Debug.
    Everything else is reserved.
**/
typedef enum {
  SetupMenuDefault = 0x0,
  TraceActive      = 0x1,
  TraceReady       = 0x2,
  TracePowerOff    = 0x3,
  ForceDisabled    = 0x4
} STREAMING_TRACE_SINK;

#define BOOTLOADER_SEED_LEN_EX    64
#define BIOS_ADDITIONAL_DATA_LEN  48

typedef enum {
  DifferentSeedProperty,
  SameSeedProperty
} SEED_PROPERTY;

/**
  Various Seed Types.
    000: BIOS and CSME SVN based
    001: fixed/constant
    010: ephemeral key
    011: ephemeral key for trusted channel
    Everything else is reserved.
**/
typedef enum {
  BiosCsmeSvnBasedSeedType          = 0x0,
  FixedSeedType                     = 0x1,
  EphmeralKeySeedType               = 0x2,
  EphmeralKeyTrustedChannelSeedType = 0x3
} SEED_TYPE;

/**
  Various Seed Lengths.
    00: 512 bits
    01: 256 bits
    Everything else is reserved.
**/
typedef enum {
  SeedLength512Bits = 0x0,
  SeedLength256Bits = 0x1
} SEED_LENGTH;

typedef struct {
  UINT32 SeedDebugProperty : 1;
  UINT32 SeedHwFpfProperty : 1;
  UINT32 SeedBtgProperty   : 1;
  UINT32 SeedType          : 3;
  UINT32 SeedLength        : 2;
  UINT32 Reserved          : 24;
} SEED_PROPERTIES_BIT_MAP;

typedef struct {
  UINT32 IsIntelUnlocked : 1;
  UINT32 IsOemUnlocked   : 1;
  UINT32 IsDamEnabled    : 1;
  UINT32 IsHwFpfEnabled  : 1;
  UINT32 IsBtgEnabled    : 1;
  UINT32 Reserved        : 27;
} SEED_INFO_BIT_MAP;

typedef struct {
  UINT8                         CsmeSvn;
  UINT8                         BiosSvn;
  SEED_INFO_BIT_MAP             SeedInfoBitMap;
} SEED_INFO;

typedef struct {
  UINT8                         CsmeSvn;
  UINT8                         BiosSvn;
  SEED_PROPERTIES_BIT_MAP       SeedPropertiesBitMap;
  UINT8                         BiosAdditionalData[BIOS_ADDITIONAL_DATA_LEN];
} SEED_PROPERTIES;

typedef struct {
  MKHI_MESSAGE_HEADER           MkhiHeader;
} GET_DEBUG_TOKEN_DATA_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER           MkhiHeader;
  UINT8                         TokenId;     ///< 1: Intel Token. 2: OEM Token
  UINT8                         Reserved[3];
  UINT32                        KnobData;
} GET_DEBUG_TOKEN_DATA_RESPONSE;

typedef union {
  GET_DEBUG_TOKEN_DATA_REQUEST    Request;
  GET_DEBUG_TOKEN_DATA_RESPONSE   Response;
} GET_DEBUG_TOKEN_DATA_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER           MkhiHeader;
  SEED_PROPERTIES               BiosSeedProperties;
  UINT8                         LockSeedGeneration;
} GET_BIOS_SEED_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER           MkhiHeader;
  UINT32                        EphemeralKeyCounter;
  SEED_INFO                     BiosSeedInfo;
  UINT8                         BiosSeed[BOOTLOADER_SEED_LEN_EX];
} GET_BIOS_SEED_RESPONSE;

typedef union {
  GET_BIOS_SEED_REQUEST   Request;
  GET_BIOS_SEED_RESPONSE  Response;
} GET_BIOS_SEED_BUFFER;

typedef enum {
  UfsPhyBinaryId = 0x100
} BINARY_ID;

typedef struct {
  MKHI_MESSAGE_HEADER  MkhiHeader;
  UINT32               IdsCount;
  UINT32               BinaryIds[0];
} LOAD_BINARY_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER  MkhiHeader;
} LOAD_BINARY_RESPONSE;

typedef union {
  LOAD_BINARY_REQUEST  Request;
  LOAD_BINARY_RESPONSE Response;
} LOAD_BINARY_BUFFER;

///
/// ICC group defines
///
typedef struct {
  UINT32 FwNeedsWarmResetFlag:1;    ///< BIT0
  UINT32 Reserved:31;               ///< BIT1-31
} ICC_MESSAGE_RESP_DATA;

typedef struct {
  MKHI_MESSAGE_HEADER  MkhiHeader;
} ICC_CMD_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER   MkhiHeader;
  ICC_MESSAGE_RESP_DATA Data;
} ICC_CMD_RESPONSE;

typedef union {
  ICC_CMD_REQUEST   Request;
  ICC_CMD_RESPONSE  Response;
} ICC_CMD_BUFFER;

///
/// MPHY group defines
///
typedef struct{
  MKHI_MESSAGE_HEADER                MkhiHeader;
  UINT32                             Reserved1;
  UINT32                             Offset;
  UINT32                             Reserved2;
  UINT32                             NumOfBytes;
} BUP_MPHY_READ_FROM_MPHY_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER                MkhiHeader;
  UINT32                             Reserved1;
  UINT32                             FileSize;
  UINT32                             Reserved2;
  UINT32                             ReadSize;
  UINT32                             Reserved3;
  UINT8                              Data[];
} BUP_MPHY_READ_FROM_MPHY_RESPONSE;

typedef union {
  BUP_MPHY_READ_FROM_MPHY_REQUEST   Request;
  BUP_MPHY_READ_FROM_MPHY_RESPONSE  Response;
} BUP_MPHY_READ_FROM_MPHY_BUFFER;

///
/// PM group defines
///
typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
} HR_NOTIFICATION_CMD_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER MkhiHeader;
  UINT8               ResetRequested;
} HR_NOTIFICATION_CMD_RESPONSE;

typedef union {
  HR_NOTIFICATION_CMD_REQUEST   Request;
  HR_NOTIFICATION_CMD_RESPONSE  Response;
} HR_NOTIFICATION_BUFFER;

///
/// BIOS AR group defines
///
typedef struct {
  MKHI_MESSAGE_HEADER       MkhiHeader;
  UINT32                    CounterId;
} BIOS_AR_READ_MC_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER        MkhiHeader;
  UINT32                     Counter;
} BIOS_AR_READ_MC_RESPONSE;

typedef union {
  BIOS_AR_READ_MC_REQUEST   Request;
  BIOS_AR_READ_MC_RESPONSE  Response;
} BIOS_AR_READ_MC_BUFFER;

typedef struct {
  MKHI_MESSAGE_HEADER            MkhiHeader;
  UINT32                         CounterId;
} BIOS_AR_INCREMENT_MC_REQUEST;

typedef struct {
  MKHI_MESSAGE_HEADER             MkhiHeader;
  UINT32                          Counter;
} BIOS_AR_INCREMENT_MC_RESPONSE;

typedef union {
  BIOS_AR_INCREMENT_MC_REQUEST   Request;
  BIOS_AR_INCREMENT_MC_RESPONSE  Response;
} BIOS_AR_INCREMENT_MC_BUFFER;
#pragma pack()

#endif // _BUP_MSGS_H

