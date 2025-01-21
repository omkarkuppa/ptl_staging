/** @file
  ISH Messages

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
#ifndef _ISH_MSGS_H_
#define _ISH_MSGS_H_

#include <HeciAddresses.h>

#pragma pack(1)

#define ISH_SET_FILE_CMD  0x1

#define PDT_UNLOCK_DATA_DATA_TYPE     0x1
#define PDT_UNLOCK_DATA_PAYLOAD_SIZE  0x0
#define PDT_UPDATE_DATA_DATA_TYPE     0x2
#define PDT_SENSOR_CONTROL_DATA_TYPE  0x6

typedef struct {
  UINT16  DataType;    // 1 - PDT UNLOCK, 2 - PDT update, 6 - sensor control
  UINT16  PayloadSize; // Size of Payload file 2 bytes aligned
} PDT_DATA;

typedef struct {
  UINT16                 Command;
  UINT16                 MessageLength;
  UINT8                  FileName[12];
  UINT16                 NumberOfElements;
  PDT_DATA               Data;
} PDT_MESSAGE_REQUEST;

typedef struct {
  UINT16                 Command;
  UINT16                 MessageLength;
  UINT32                 Status;
} PDT_MESSAGE_RESPONSE;

typedef union {
  PDT_MESSAGE_REQUEST   Request;
  PDT_MESSAGE_RESPONSE  Response;
} PDT_MESSAGE_BUFFER;

typedef enum {
  BiosToIshStatusSuccess     = 0x0,
  BiosToIshStatusFailure     = 0xFFFFFFFF
} BIOSTOISH_RESPONSE_STATUS;

#pragma pack()

#endif // _ISH_MSGS_H_
