/** @file
  Definitions for querying PSR message

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

#ifndef _PSR_MSG_H_
#define _PSR_MSG_H_

#include <HeciAddresses.h>

#define PSR_GENESIS_SIZE                               1284
#define PSR_LEDGER_SIZE                                128
#define PSR_EVENT_SIZE                                 12
#define PSR_MAX_EVENT_COUNT                            100
#define PSR_MAX_CERT_CHAIN_SIZE                        3000
#define PSR_ODCA_CHAIN_LEN                             4
#define PSR_MAX_SIGNATURE_LENGTH                       512
#define NONCE_MAXSIZE                                  20
#define PSR_SHA512_LEN_BYTES                           64
#define PSR_PLATFORM_ID_LENGTH                         64
#define PSR_MAX_SUPPORTED_CAPABILITIES                 32

//
// PSR Commands
//
#define PSR_HECI_CAPABILITIES_UPDATE                   0x09
#define PSR_HECI_PLATFORM_SERVICE_RECORD_GET           0x0A
#define PSR_HECI_EVENT_REPORT                          0x13
#define PSR_HECI_CHASSIS_INTRUSION_REPORT              0x14
#define PSR_HECI_EXCESSIVE_TEMPERATURE_REPORT          0x15
#define PSR_HECI_DEVICE_ERASE_REPORT                   0x30

#pragma pack(1)

typedef struct {
  UINT16           Major;
  UINT16           Minor;
} PSR_VERSION;

typedef struct {
  UINT16            Major;
  UINT16            Minor;
  UINT16            Hotfix;
  UINT16            Build;
} CSME_FW_VERSION;

#ifndef VFRCOMPILE

typedef enum {
  Ecdsa384WithSha384  = 0,
} LOG_SIGNING_MECHANISM;

typedef enum {
  PsrStatusSuccess,
  PsrStatusFeatureNotSupported,
  PsrStatusUpidDisabled,
  PsrStatusActionNotAllowed,
  PsrStatusInvalidInputParameter,
  PsrStatusInternalError,
  PsrStatusNotAllowedAfterEop,
} PSR_STATUS;

typedef enum {
  PsrLogStateNotStarted = 0, /* Default value */
  PsrLogStateStarted,
  PsrLogStateStopped,
} PSR_LOG_STATE;

typedef struct {
  UINT8             Command;
  UINT8             Reserved;
  UINT16            ByteCount;
} PSR_HECI_HEADER;

typedef struct {
  PSR_HECI_HEADER    Header;
  UINT8              UserNonce[NONCE_MAXSIZE];
} PSR_REQUEST;

typedef struct {
  UINT8              Psrid[16];
  UINT8              Upid[PSR_PLATFORM_ID_LENGTH];
  UINT8              GenesisBuffer[PSR_GENESIS_SIZE];
  UINT8              Capabilities[PSR_MAX_SUPPORTED_CAPABILITIES];
  UINT8              Ledger[PSR_LEDGER_SIZE];
  UINT32             TotalEventCount;
  UINT8              EventsBuffer[(PSR_EVENT_SIZE * PSR_MAX_EVENT_COUNT)];
} PLATFORM_SERVICE_RECORD;

typedef struct {
  PSR_LOG_STATE                    PsrLogState;
  PSR_VERSION                      PsrVersion;
  PLATFORM_SERVICE_RECORD          Psr;
  UINT8                            PsrHash[PSR_SHA512_LEN_BYTES];
  UINT8                            UserNonce[NONCE_MAXSIZE];
  UINT8                            CsmeNonce[NONCE_MAXSIZE];
  CSME_FW_VERSION                  FwVersion;
  LOG_SIGNING_MECHANISM            SignatureMechanism;
  UINT8                            Signature[PSR_MAX_SIGNATURE_LENGTH];
  UINT16                           LengthsOfCertificate[PSR_ODCA_CHAIN_LEN];
  UINT8                            Certificates[PSR_MAX_CERT_CHAIN_SIZE];
} PSR_DATA;

typedef struct {
  PSR_HECI_HEADER                  Header;
  PSR_STATUS                       Status;
  PSR_DATA                         PsrData;
} PSR_RESPONSE;

typedef union {
  PSR_REQUEST  Request;
  PSR_RESPONSE Response;
} GET_PSR_BUFFER;

typedef struct {
  PSR_HECI_HEADER Header; ///< Header.command == 0x13 "Report Event"
  UINT32          Id;     ///< Different event ID for each event
  UINT32          Data;   ///< Data payload
} PSR_HECI_EVENT_REPORT_REQUEST;

typedef struct {
  PSR_HECI_HEADER Header;
  PSR_STATUS      Status;
} PSR_HECI_EVENT_REPORT_RESPONSE;

typedef union {
  PSR_HECI_EVENT_REPORT_REQUEST  Request;
  PSR_HECI_EVENT_REPORT_RESPONSE Response;
} PSR_HECI_EVENT_REPORT_BUFFER;

//
// Chassis Intrusion Detection
//
typedef struct {
  PSR_HECI_HEADER         Header;
} PSR_CHASSIS_INTRUSION_DET_EVENT_REQUEST;

typedef struct {
  PSR_HECI_HEADER         Header;
  PSR_STATUS              Status;
} PSR_CHASSIS_INTRUSION_DET_EVENT_RESPONSE;

typedef union {
  PSR_CHASSIS_INTRUSION_DET_EVENT_REQUEST  Request;
  PSR_CHASSIS_INTRUSION_DET_EVENT_RESPONSE Response;
} PSR_CHASSIS_INTRUSION_DET_EVENT_BUFFER;

//
// Critical Temperature Shutdown
//
typedef struct {
  PSR_HECI_HEADER         Header;
} PSR_CRITICAL_TEMPERATURE_REPORT_REQUEST;

typedef struct {
  PSR_HECI_HEADER         Header;
  PSR_STATUS              Status;
} PSR_CRITICAL_TEMPERATURE_REPORT_RESPONSE;

typedef union {
  PSR_CRITICAL_TEMPERATURE_REPORT_REQUEST  Request;
  PSR_CRITICAL_TEMPERATURE_REPORT_RESPONSE Response;
} PSR_CRITICAL_TEMP_SHUTDOWN_BUFFER;

//
// PSR Capability list
//
typedef struct {
  PSR_HECI_HEADER         Header;
  UINT32                  Capability;
} PSR_SUPPORTED_CAPABILITIES_UPDATE_REQUEST;

typedef struct {
  PSR_HECI_HEADER         Header;
  PSR_STATUS              Status;
} PSR_SUPPORTED_CAPABILITIES_UPDATE_RESPONSE;

typedef union {
  PSR_SUPPORTED_CAPABILITIES_UPDATE_REQUEST  Request;
  PSR_SUPPORTED_CAPABILITIES_UPDATE_RESPONSE Response;
} PSR_SUPPORTED_CAPABILITIES_BUFFER;

//
// PSR Device Erase Report
//
typedef enum {
  PsrEraseSrcRemote = 0,
  PsrEraseSrcLocal,
} PSR_ERASE_SOURCE;

///
/// Device Erase Report
///
typedef struct {
  PSR_ERASE_SOURCE   EraseSource;            ///< Erase source
  UINT32             EraseActionsAttempted;  ///< Bit mask represents erase action attempted : 1 - attempted, 0 - not attempted
  UINT32             EraseActionsStatus;     ///< Bit mask represents erase action status : 1 - Passed, 0 - Failed
  UINT8              EraseFailureReason[32]; ///< Array of Erase Actions Reason (Array index is same as EraseActionsAttempted bitmask)
} PSR_DEVICE_ERASE_REPORT;

typedef struct {
  PSR_HECI_HEADER          Header;
  PSR_DEVICE_ERASE_REPORT  EraseReport;
} PSR_HECI_DEVICE_ERASE_REPORT_REQUEST;

typedef struct {
  PSR_HECI_HEADER       Header;
  PSR_STATUS            Status;
} PSR_HECI_DEVICE_ERASE_REPORT_RESPONSE;

//
// BIOS Platform Erase Status Structure
//
typedef union {
  PSR_HECI_DEVICE_ERASE_REPORT_REQUEST   Request;
  PSR_HECI_DEVICE_ERASE_REPORT_RESPONSE  Response;
} PSR_HECI_DEVICE_ERASE_REPORT_BUFFER;

#endif //VFRCOMPILE

#pragma pack()

#endif // _PSR_MSG_H_
