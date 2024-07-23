/** @file
  Trusted Channel Message Definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _TRUSTED_CHANNEL_H_
#define _TRUSTED_CHANNEL_H_

#include "DataServicesMsg.h"
#include <Base.h>
#include <Uefi.h>

#define  TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE  32

#pragma pack(push, 1)

typedef struct {
  UINT8                Signature[TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE];
  UINT32               MonotonicCounter;
  BIOS_STORAGE_COMMAND Command;
} TRUSTED_CHANNEL_BIOS_HEADER;

typedef struct {
  TRUSTED_CHANNEL_BIOS_HEADER  TrustedHeader;
  UINT8                        Reserved1;
  UINT16                       Reserved2;
} TRUSTED_CHANNEL_BIOS_REQUEST_HEADER;

typedef struct {
  TRUSTED_CHANNEL_BIOS_HEADER  TrustedHeader;
  UINT8                        Status;
  UINT16                       Reserved2;
} TRUSTED_CHANNEL_BIOS_RESPONSE_HEADER;


///
/// Trusted Channel Request Monotonic Counter
///
typedef struct {
  TRUSTED_CHANNEL_BIOS_REQUEST_HEADER  Header;
  UINT32                               CounterId;
} BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_REQUEST;

///
/// Trusted Channel Request Monotonic Counter response
///
typedef struct {
  TRUSTED_CHANNEL_BIOS_RESPONSE_HEADER Header;
  UINT32                               Counter;
} BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_RESPONSE;

typedef union {
  BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_REQUEST     Request;
  BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_RESPONSE    Response;
} BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_BUFFER;

///
/// Trusted Channel Request Increment Counter
///
typedef struct {
  TRUSTED_CHANNEL_BIOS_REQUEST_HEADER  Header;
  UINT32                               CounterId;
} BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_REQUEST;

///
/// Trusted Channel Request Increment Counter response
///
typedef struct {
  TRUSTED_CHANNEL_BIOS_RESPONSE_HEADER Header;
  UINT32                               Counter;
} BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_RESPONSE;

typedef union {
  BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_REQUEST     Request;
  BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_RESPONSE    Response;
} BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_BUFFER;

#pragma pack(pop)

#endif
