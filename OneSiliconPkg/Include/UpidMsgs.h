/** @file
  Definitions for Unique Platform ID messages support

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _UPID_MSGS_H_
#define _UPID_MSGS_H_

#include <HeciAddresses.h>

#pragma pack(1)

///
/// UPID (Unique Platform Id) Message Header
///
typedef union {
  UINT32 Data;
  struct {
    UINT32 Feature : 8;
    UINT32 Command : 8;
    UINT32 Length  : 16;
  } Fields;
} UPID_MESSAGE_HEADER;

#define UPID_MESSAGE_BYTE_COUNT_MAP(a) (sizeof(a)-sizeof(UPID_MESSAGE_HEADER))

///
/// Typedef for UPID Feature
///
typedef enum {
  UpidFeaturePlatId                  = 0x00
} UPID_FEATURE;

///
/// Typedef for common feature commands
///
typedef enum {
  UpidCommonGetFeatureSupport        = 0x00,
  UpidCommonGetFeatureState          = 0x01,
  UpidCommonSetFeatureState          = 0x02,
  UpidCommonGetOsControlFeatureState = 0x03,
  UpidCommonSetOsControlFeatureState = 0x04
} UPID_COMMON_FEATURE;

///
/// Typedef for Platform Id commands
///
typedef enum {
  PlatIdGetPlatformId                = 0x05,
  PlatIdGetRefurbishCounter          = 0x06,
  PlatIdUpdateOemPlatformId          = 0x07
} PLAT_ID_FEATURE;

///
/// Typedef for UPID Status
///
typedef enum {
  UpidStatusSuccess,
  UpidStatusFeatureNotSupported,
  UpidStatusInvalidInputParameter,
  UpidStatusInternalError,
  UpidStatusNotAllowedAfterEop,
  UpidStatusNotAllowedAfterManufLock,
  UpidStatusMaxCountersExceeded,
  UpidStatusInvalidState,
  UpidStatusReserved2,
  UpidStatusNotAllowedAfterCbd,
} UPID_STATUS;

///
/// Common Features Definitions
///

///
/// Get Feature Support
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_FEATURE_SUPPORT_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               PlatIdFeatureSupport;
} GET_FEATURE_SUPPORT_RESPONSE;

typedef union {
  GET_FEATURE_SUPPORT_REQUEST  Request;
  GET_FEATURE_SUPPORT_RESPONSE Response;
} GET_FEATURE_SUPPORT_BUFFER;

///
/// Get/Set Feature State
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               PlatIdFeatureState;
} GET_FEATURE_STATE_RESPONSE;

typedef union {
  GET_FEATURE_STATE_REQUEST  Request;
  GET_FEATURE_STATE_RESPONSE Response;
} GET_FEATURE_STATE_BUFFER;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT8               PlatIdFeatureState;
} SET_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
} SET_FEATURE_STATE_RESPONSE;

typedef union {
  SET_FEATURE_STATE_REQUEST  Request;
  SET_FEATURE_STATE_RESPONSE Response;
} SET_FEATURE_STATE_BUFFER;

///
/// Get/Set Os Control Feature State
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_OS_CONTROL_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               PlatIdFeatureOsControlState;
} GET_OS_CONTROL_FEATURE_STATE_RESPONSE;

typedef union {
  GET_OS_CONTROL_FEATURE_STATE_REQUEST  Request;
  GET_OS_CONTROL_FEATURE_STATE_RESPONSE Response;
} GET_OS_CONTROL_FEATURE_STATE_BUFFER;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT8               PlatIdFeatureOsControlState;
} SET_OS_CONTROL_FEATURE_STATE_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
} SET_OS_CONTROL_FEATURE_STATE_RESPONSE;

typedef union {
  SET_OS_CONTROL_FEATURE_STATE_REQUEST  Request;
  SET_OS_CONTROL_FEATURE_STATE_RESPONSE Response;
} SET_OS_CONTROL_FEATURE_STATE_BUFFER;

///
/// Platform Id Feature Defintions
///

///
/// Get Platform Id
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_PLATFORM_ID_REQUEST;

#define MAX_PLAT_ID_SIZE 32

typedef enum {
  OemPlatIdTypeNotSet,
  OemPlatIdTypeBinary,
  OemPlatIdTypePrintableString
} OEM_PLAT_ID_TYPE;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT32              OemPlatIdType;
  UINT8               OemPlatId[MAX_PLAT_ID_SIZE];
  UINT8               CsmePlatId[MAX_PLAT_ID_SIZE];
} GET_PLATFORM_ID_RESPONSE;

typedef union {
  GET_PLATFORM_ID_REQUEST  Request;
  GET_PLATFORM_ID_RESPONSE Response;
} GET_PLATFORM_ID_BUFFER;

//
/// Get Refurbish Counter
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
} GET_REFURBISH_COUNTER_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
  UINT8               RefurbishCounter;
} GET_REFURBISH_COUNTER_RESPONSE;

typedef union {
  GET_REFURBISH_COUNTER_REQUEST  Request;
  GET_REFURBISH_COUNTER_RESPONSE Response;
} GET_REFURBISH_COUNTER_BUFFER;

///
/// Update OEM Platform Id
///
typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              OemPlatIdType;
  UINT8               OemPlatId[MAX_PLAT_ID_SIZE];
  UINT32              ManufacturingType;
} UPDATE_OEM_PLATFORM_ID_REQUEST;

typedef struct {
  UPID_MESSAGE_HEADER UpidHeader;
  UINT32              Status;
} UPDATE_OEM_PLATFORM_ID_RESPONSE;

typedef union {
  UPDATE_OEM_PLATFORM_ID_REQUEST  Request;
  UPDATE_OEM_PLATFORM_ID_RESPONSE Response;
} UPDATE_OEM_PLATFORM_ID_BUFFER;

#pragma pack()

#endif // _UPID_MSGS_H_

