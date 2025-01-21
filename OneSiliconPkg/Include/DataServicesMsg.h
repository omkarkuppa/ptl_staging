/** @file
  CSME Data Services Message Definitions.

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

@par Specification
**/

#ifndef _CSME_DATA_SERVICES_MSG_H_
#define _CSME_DATA_SERVICES_MSG_H_

#include <MkhiMsgs.h>

#define  HMAC_SHA256_SIGNATURE_SIZE  32

#pragma pack(push, 1)

///
/// Storage Command Types
///
#define  BIOS_READ_AR_MC_COUNTER_CMD   0x8
#define  BIOS_INCREMENT_AR_MC_CMD      0x9

///
/// Storage Command
///
typedef struct {
  UINT8                 Id         : 7;   ///< Command ID (XXX_CMD_ID)
  UINT8                 IsResponse : 1;   ///< Request = 0, Response = 1
} BIOS_STORAGE_COMMAND;

///
/// Storage Request Info
///
typedef struct {
  UINT8                Signature[HMAC_SHA256_SIGNATURE_SIZE];
  UINT32               RequestCounter;
  BIOS_STORAGE_COMMAND Command;
  UINT8                Reserved1;
  UINT16               Reserved2;
} BIOS_STORAGE_REQUEST_HEADER;

///
/// Storage Response Info
///
typedef struct {
  UINT8                Signature[HMAC_SHA256_SIGNATURE_SIZE];
  UINT32               Counter;
  BIOS_STORAGE_COMMAND Command;
  UINT8                Status;
  UINT16               Reserved2;
} BIOS_STORAGE_RESPONSE_HEADER;

///
/// Request Monotonic Counter
///
typedef struct {
  BIOS_STORAGE_REQUEST_HEADER Header;
  UINT32                      CounterId;
} BIOS_STORAGE_READ_AR_MC_REQUEST;

///
/// Request Monotonic Counter response
///
typedef struct {
  BIOS_STORAGE_RESPONSE_HEADER Header;
  UINT32                       Counter;
} BIOS_STORAGE_READ_AR_MC_RESPONSE;

///
/// Request Increment Monotonic Counter
///
typedef struct {
  BIOS_STORAGE_REQUEST_HEADER  Header;
  UINT32                       CounterId;
} BIOS_STORAGE_INCREMENT_AR_MC_Request;

///
/// Response Increment Monotonic Counter
///
typedef struct {
  BIOS_STORAGE_RESPONSE_HEADER Header;
  UINT32                       Counter;
} BIOS_STORAGE_INCREMENT_AR_MC_Response;

#pragma pack(pop)

#endif
