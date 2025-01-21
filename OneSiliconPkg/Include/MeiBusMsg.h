/** @file
  Definitions for MEI Bus message support

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
#ifndef _MEI_BUS_MSG_H_
#define _MEI_BUS_MSG_H_

#include <HeciAddresses.h>

#define HBM_MINOR_VERSION_0             0
#define HBM_MINOR_VERSION_2             2
#define HBM_MAJOR_VERSION_1             1
#define HBM_MAJOR_VERSION_2             2

#define HOST_VERSION_REQUEST            0x01
#define HOST_STOP_REQUEST               0x02
#define HOST_ENUMERATE_REQUEST          0x04
#define HOST_CLIENT_PROPERTIES_REQUEST  0x05
#define CLIENT_CONNECT_REQUEST          0x06
#define CLIENT_DISCONNECT_REQUEST       0x07
#define FLOW_CONTROL                    0x08
#define HECI_BUS_DISABLE_OPCODE         0x0C
#define CAPABILITIES_REQUEST            0x13
#define CLIENT_DMA_MAP_REQUEST          0x14
#define CLIENT_DMA_UNMAP_REQUEST        0x15

#pragma pack(1)

typedef union {
  UINT8 Data;
  struct {
    UINT8 Command    : 7;
    UINT8 IsResponse : 1;
  } Fields;
} HBM_COMMAND;

typedef struct {
  UINT8 MinorVersion;
  UINT8 MajorVersion;
} HBM_VERSION;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Reserved;
  HBM_VERSION HostVersion;
} HBM_HOST_VERSION_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       HostVersionSupported;
  HBM_VERSION MeMaxVersion;
} HBM_HOST_VERSION_RESPONSE;

typedef union {
  HBM_HOST_VERSION_REQUEST  Request;
  HBM_HOST_VERSION_RESPONSE Response;
} HBM_HOST_VERSION_BUFFER;

#define STOP_REASON_REQUEST_FROM_DRIVER  0x00

typedef struct {
  HBM_COMMAND Command;
  UINT8       Reason;
  UINT8       Reserved[2];
} HBM_HOST_STOP_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Reserved[3];
} HBM_HOST_STOP_RESPONSE;

typedef union {
  HBM_HOST_STOP_REQUEST   Request;
  HBM_HOST_STOP_RESPONSE  Response;
} HBM_HOST_STOP_BUFFER;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Reserved[3];
} HBM_HOST_ENUMERATION_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Reserved[3];
  UINT8       ValidAddresses[32];
} HBM_HOST_ENUMERATION_RESPONSE;

typedef union {
  HBM_HOST_ENUMERATION_REQUEST  Request;
  HBM_HOST_ENUMERATION_RESPONSE Response;
} HBM_HOST_ENUMERATION_BUFFER;

#define NUMBER_OF_VALID_ADDRESS_BITS 32 * 8

typedef struct {
  HBM_COMMAND Command;
  UINT8       Address;
  UINT8       Reserved[2];
} HBM_HOST_CLIENT_PROPERTIES_REQUEST;

typedef struct {
  EFI_GUID ProtocolName;
  UINT8    ProtocolVersion;
  UINT8    MaxNumberOfConnections;
  UINT8    FixedAddress;
  UINT8    SingleReceiveBuffer;
  UINT32   MaxMessageLength;
  UINT32   Reserved;
} HECI_CLIENT_PROPERTIES;

typedef struct {
  HBM_COMMAND            Command;
  UINT8                  Address;
  UINT8                  Status;
  UINT8                  Reserved;
  HECI_CLIENT_PROPERTIES ClientProperties;
} HBM_HOST_CLIENT_PROPERTIES_RESPONSE;

typedef union {
  HBM_HOST_CLIENT_PROPERTIES_REQUEST  Request;
  HBM_HOST_CLIENT_PROPERTIES_RESPONSE Response;
} HBM_HOST_CLIENT_PROPERTIES_BUFFER;

typedef struct {
  HBM_COMMAND Command;
  UINT8       MeAddress;
  UINT8       HostAddress;
  UINT8       Reserved;
} HBM_CLIENT_CONNECT_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       MeAddress;
  UINT8       HostAddress;
  UINT8       Status;
} HBM_CLIENT_CONNECT_RESPONSE;

typedef union {
  HBM_CLIENT_CONNECT_REQUEST  Request;
  HBM_CLIENT_CONNECT_RESPONSE Response;
} HBM_CLIENT_CONNECT_BUFFER;

typedef struct {
  HBM_COMMAND Command;
  UINT8       MeAddress;
  UINT8       HostAddress;
  UINT8       Reserved;
} HBM_CLIENT_DISCONNECT_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       MeAddress;
  UINT8       HostAddress;
  UINT8       Status;
} HBM_CLIENT_DISCONNECT_RESPONSE;

typedef union {
  HBM_CLIENT_DISCONNECT_REQUEST  Request;
  HBM_CLIENT_DISCONNECT_RESPONSE Response;
} HBM_CLIENT_DISCONNECT_BUFFER;

typedef struct {
  HBM_COMMAND Command;
  UINT8       MeAddress;
  UINT8       HostAddress;
  UINT8       Reserved[5];
} HBM_FLOW_CONTROL;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Reserved[3];
} HECI_BUS_DISABLE_CMD_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Status;
  UINT8       Reserved[2];
} HECI_BUS_DISABLE_CMD_RESPONSE;

typedef union {
  UINT8 Data;
  struct {
    UINT8 Vm        : 1;   ///< [0] VM tag
    UINT8 GscSgl    : 1;   ///< [1] GSC SGL
    UINT8 ClientDma : 1;   ///< [2] Client DMA Mapping and DMA Map/Unmap HBM messages
    UINT8 Reserved  : 5;   ///< [7:3] Reserved
  } Capabilities;
} HBM_CAPS_DATA;

typedef struct {
  HBM_COMMAND   Command;
  HBM_CAPS_DATA Capabilities;  ///< Host driver HBM capabilities
  UINT8         Reserved[2];
} HBM_CAPABILITIES_REQUEST;

typedef struct {
  HBM_COMMAND   Command;
  HBM_CAPS_DATA Capabilities;  ///< FW driver HBM capabilities
  UINT8         Reserved[2];
} HBM_CAPABILITIES_RESPONSE;

typedef union {
  HBM_CAPABILITIES_REQUEST  Request;
  HBM_CAPABILITIES_RESPONSE Response;
} HBM_CAPABILITIES_BUFFER;

typedef enum {
  ClientDmaBufferIdError = 0,
  ClientDmaBufferIdWlan
} HBM_CLIENT_DMA_BUFFER_ID;

typedef struct {
  HBM_COMMAND Command;
  UINT8       ClientBufferId;
  UINT8       Reserved[2];
  UINT32      AddressLow;
  UINT32      AddressHigh;
  UINT32      Size;
} HBM_CLIENT_DMA_MAP_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Status;
  UINT8       Reserved[2];
} HBM_CLIENT_DMA_MAP_RESPONSE;

typedef union {
  HBM_CLIENT_DMA_MAP_REQUEST  Request;
  HBM_CLIENT_DMA_MAP_RESPONSE Response;
} HBM_CLIENT_DMA_MAP_BUFFER;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Status;
  UINT8       ClientBufferId;
  UINT8       Reserved;
} HBM_CLIENT_DMA_UNMAP_REQUEST;

typedef struct {
  HBM_COMMAND Command;
  UINT8       Status;
  UINT8       Reserved[2];
} HBM_CLIENT_DMA_UNMAP_RESPONSE;

typedef union {
  HBM_CLIENT_DMA_UNMAP_REQUEST  Request;
  HBM_CLIENT_DMA_UNMAP_RESPONSE Response;
} HBM_CLIENT_DMA_UNMAP_BUFFER;

#pragma pack()

#endif // _MEI_BUS_MSG_H_

