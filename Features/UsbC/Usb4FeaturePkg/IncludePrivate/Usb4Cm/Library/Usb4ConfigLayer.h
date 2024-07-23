/** @file
  Header file for USB4 configuration layer.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _USB4_CONFIG_LAYER_H_
#define _USB4_CONFIG_LAYER_H_

#include <Base.h>
#include <Library/Usb4ConfigBase.h>

#pragma pack(push, 1)


#define USB4_CONTROL_DATA_DW_MAX            60
#define USB4_MAX_TOPOLOGY_DEPTH             5
extern GLOBAL_REMOVE_IF_UNREFERENCED UINT8  mUsb4TopologyDepth;

//
// Event codes of Notification packet
//
typedef enum _USB4_NOTIFICATION_EVENT {
  //
  // Error Events
  //
  USB4_ERR_CONN        = 0,
  USB4_ERR_LINK        = 1,
  USB4_ERR_ADDR        = 2,
  USB4_ERR_RESERVED3   = 3,
  USB4_ERR_ADP         = 4,
  USB4_ERR_RESERVED5   = 5,
  USB4_ERR_RESERVED6   = 6,
  USB4_ERR_ENUM        = 8,
  USB4_ERR_NUA         = 9,
  USB4_ERR_RESERVED10  = 10,
  USB4_ERR_LEN         = 11,
  USB4_ERR_HEC         = 12,
  USB4_ERR_FC          = 13,
  USB4_ERR_PLUG        = 14,
  USB4_ERR_LOCK        = 15,
  //
  // Notifications
  //
  USB4_HOTPLUG_ACK     = 7,
  USB4_ROP_CMPLT       = 33,  // New in USB4 v2
  USB4_POP_CMPLT       = 34,  // New in USB4 v2
  USB4_PCIE_WAKE       = 35,  // New in USB4 v2
  USB4_DP_CON_CHANGE   = 36,  // New in USB4 v2
  USB4_LINK_RECOVERY   = 38,  // New in USB4 v2
  USB4_ASYM_LINK       = 39,  // New in USB4 v2
  //
  // Protocol Adapter Events
  //
  USB4_DP_BW           = 32,
  USB4_DPTX_DISCOVERY  = 37,  // New in USB4 v2
} USB4_NOTIFICATION_EVENT, *PUSB4_NOTIFICATION_EVENT;

//
// PDF values for SW<->FW communication in raw mode
//
typedef enum _USB4_CTRL_PDF {
  PDF_RESERVED_0           = 0,
  PDF_READ_REQUEST         = 1,
  PDF_WRITE_REQUEST        = 2,
  PDF_NOTIFICATION_PACKET  = 3,
  PDF_NOTIFICATION_ACK     = 4,
  PDF_HOT_PLUG             = 5,
  PDF_INTERDOMAIN_REQUEST  = 6,
  PDF_INTERDOMAIN_RESPONSE = 7,
  PDF_DONT_CARE = 255
} USB4_CTRL_PDF, *PUSB4_CTRL_PDF;

#define ROUTE_STRING_HIGH_MASK      0x003F3F3F
#define ROUTE_STRING_LOW_MASK       0x3F3F3F3F

//
// USB4 route string.
// Route string format in control request.
//
typedef union {
  struct {
      // Route String High
      UINT32 Level4Adapter    : BITFIELD_RANGE (0, 5);
      UINT32 Reserved4        : BITFIELD_RANGE (6, 7);
      UINT32 Reserved5        : BITFIELD_RANGE (8, 15);
      UINT32 Reserved6        : BITFIELD_RANGE (16, 23);
      UINT32 Reserved7        : BITFIELD_RANGE (24, 30);
      UINT32 Cm               : BITFIELD_BIT (31);
      // Route String Low
      UINT32 Level0Adapter    : BITFIELD_RANGE (0, 5);
      UINT32 Reserved0        : BITFIELD_RANGE (6, 7);
      UINT32 Level1Adapter    : BITFIELD_RANGE (8, 13);
      UINT32 Reserved1        : BITFIELD_RANGE (14, 15);
      UINT32 Level2Adapter    : BITFIELD_RANGE (16, 21);
      UINT32 Reserved2        : BITFIELD_RANGE (22, 23);
      UINT32 Level3Adapter    : BITFIELD_RANGE (24, 29);
      UINT32 Reserved3        : BITFIELD_RANGE (30, 31);
  } Bits;
  struct {
    UINT32 High;
    UINT32 Low;
  } Str;
  UINT64 Value;
} ROUTE_STRING, *PROUTE_STRING;

#define TOPOLOGY_ID_HIGH_MASK      0x003F3F3F
#define TOPOLOGY_ID_LOW_MASK       0x3F3F3F3F

#define TOPOLOGY_ID_ADAPTER_SIZE   8

//
// USB4 Topology ID.
// Topology ID format in USB4 device configuration space
// USB4 support domain topology up to depth 5
// Topology ID format is 00 00 00 P4 P3 P2 P1 P0
// Pn is the Adapter Number of the Adapter in the Downstream Facing Port at level n
//
typedef union {
  struct {
      // Topology ID Low
      UINT32 Level0Adapter    : BITFIELD_RANGE (0, 5);
      UINT32 Reserved0        : BITFIELD_RANGE (6, 7);
      UINT32 Level1Adapter    : BITFIELD_RANGE (8, 13);
      UINT32 Reserved1        : BITFIELD_RANGE (14, 15);
      UINT32 Level2Adapter    : BITFIELD_RANGE (16, 21);
      UINT32 Reserved2        : BITFIELD_RANGE (22, 23);
      UINT32 Level3Adapter    : BITFIELD_RANGE (24, 29);
      UINT32 Reserved3        : BITFIELD_RANGE (30, 31);
      // Topology ID High
      UINT32 Level4Adapter    : BITFIELD_RANGE (0, 5);
      UINT32 Reserved4        : BITFIELD_RANGE (6, 7);
      UINT32 Reserved5        : BITFIELD_RANGE (8, 15);
      UINT32 Reserved6        : BITFIELD_RANGE (16, 23);
      UINT32 Reserved7        : BITFIELD_RANGE (24, 30);
      UINT32 Valid            : BITFIELD_BIT (31);
  } Bits;
  struct {
    UINT32 Low;
    UINT32 High;
  } Id;
  UINT8   Adapter[TOPOLOGY_ID_ADAPTER_SIZE];
  UINT64  Value;
} TOPOLOGY_ID, *PTOPOLOGY_ID;

//
// Convert Topology ID format to Route String format
//
#define TOPOLOGY_ID_TO_ROUTE_STR(Id)  ((((UINT64)Id>>32)&0x00FFFFFF)|(((UINT64)Id&0xFFFFFFFF)<<32))

//
// This struct defines a USB4 control packet header
//
typedef struct _USB4_CONTROL_PACKET_HEADER {
  // DWORD 0 - 1
  ROUTE_STRING   RouteString;
  // DWORD 2
  UINT32         Offset                   : BITFIELD_RANGE (0, 12);
  UINT32         Count                    : BITFIELD_RANGE (13, 18);
  UINT32         AdpNum                   : BITFIELD_RANGE (19, 24);
  UINT32         ConfigSpace              : BITFIELD_RANGE (25, 26);
  UINT32         SequenceNum              : BITFIELD_RANGE (27, 28);
  UINT32         Reserved                 : BITFIELD_RANGE (29, 31);
} USB4_CONTROL_PACKET_HEADER, *PUSB4_CONTROL_PACKET_HEADER;

//
// USB4 Control Read Request Packet
//
typedef struct _USB4_CONTROL_READ_REQUEST {
  USB4_CONTROL_PACKET_HEADER   Header;
  UINT32                       Crc;
} USB4_CONTROL_READ_REQUEST, *PUSB4_CONTROL_READ_REQUEST;

//
// USB4 Control Read Response Packet
// CRC location depends on actual response data length, but not a fixed location
// CRC field is not defined in structure to prevent it is referenced directly by mistake
//
typedef struct _USB4_CONTROL_READ_RESPONSE {
  USB4_CONTROL_PACKET_HEADER   Header;
  // Allocate one DWORD for CRC after Read response data
  UINT32                       Data[USB4_CONTROL_DATA_DW_MAX + 1];
} USB4_CONTROL_READ_RESPONSE, *PUSB4_CONTROL_READ_RESPONSE;

//
// USB4 Control Write Request Packet
// CRC location depends on actual write data length, but not a fixed location
// CRC field is not defined in structure to prevent it is referenced directly by mistake
//
typedef struct _USB4_CONTROL_WRITE_REQUEST {
  USB4_CONTROL_PACKET_HEADER    Header;
  // Allocate one DWORD for CRC after data field
  UINT32                        Data[USB4_CONTROL_DATA_DW_MAX + 1];
} USB4_CONTROL_WRITE_REQUEST, *PUSB4_CONTROL_WRITE_REQUEST;

//
// USB4 Control Write Response Packet
//
typedef struct _USB4_CONTROL_WRITE_RESPONSE {
  USB4_CONTROL_PACKET_HEADER    Header;
  UINT32                        Crc;
} USB4_CONTROL_WRITE_RESPONSE, *PUSB4_CONTROL_WRITE_RESPONSE;

//
// USB4 Notification Packet
//
typedef struct _USB4_NOTIFICATION_PACKET {
    ROUTE_STRING    RouteString;
    UINT32          EventCode           : BITFIELD_RANGE (0, 7);
    UINT32          AdpNum              : BITFIELD_RANGE (8, 13);
    UINT32          Reserved            : BITFIELD_RANGE (14, 29);
    UINT32          Plug                : BITFIELD_RANGE (30, 31);
    UINT32          Crc;
} USB4_NOTIFICATION_PACKET, *PUSB4_NOTIFICATION_PACKET;

//
// PG differentiates Hot Plug and Hot Unplug Events in a Hot Plug Acknowledgment Packet.
//
typedef enum _USB4_NOTIFICATION_PG {
  PG_NOT_HOT_PLUG = 0,
  PG_RESERVED = 1,
  PG_HOT_PLUG_EVENT = 2,
  PG_HOT_UNPLUG_EVENT = 3
} USB4_NOTIFICATION_PG, *PUSB4_NOTIFICATION_PG;


//
// USB4 Notification Packet Acknowledgment
//
typedef struct _USB4_NOTIFICATION_ACK_PACKET {
  ROUTE_STRING    RouteString;
  UINT32          Crc;
} USB4_NOTIFICATION_ACK_PACKET, *PUSB4_NOTIFICATION_ACK_PACKET;

//
// USB4 Hot Plug Packet
//
typedef struct _USB4_HOT_PLUG_EVENT_PACKET {
  // DWORD 0 - 1
  ROUTE_STRING    RouteString;
  // DWORD 2
  UINT32          AdpNum                : BITFIELD_RANGE (0, 5);
  UINT32          Reserved              : BITFIELD_RANGE (6, 30);
  // 0: plug, 1: unplug
  UINT32          Unplug                : BITFIELD_BIT (31);
  UINT32          Crc;
} USB4_HOT_PLUG_EVENT_PACKET, *PUSB4_HOT_PLUG_EVENT_PACKET;

//
// This enum defines plug event type
//
typedef enum _USB4_HOT_PLUG_EVENT_TYPE {
  USB4_HOT_PLUG_EVENT_PLUG,
  USB4_HOT_PLUG_EVENT_UNPLUG
} USB4_HOT_PLUG_EVENT_TYPE, *PUSB4_HOT_PLUG_EVENT_TYPE;

//
// USB4 inter domain packet.
//
typedef struct _INTER_DOMAIN_PACKET_HEADER {
    ROUTE_STRING    RouteString;
    UINT32          DataSize                : BITFIELD_RANGE (0, 5);
    UINT32          Reserved0               : BITFIELD_RANGE (6, 26);
    UINT32          SequenceNum             : BITFIELD_RANGE (27, 28);
    UINT32          Reserved1               : BITFIELD_RANGE (29, 31);
} INTER_DOMAIN_PACKET_HEADER, *PINTER_DOMAIN_PACKET_HEADER;

//
// This struct defines an inter domain packet.
// The Crc location depends on the actual data size.
// Don't use Crc field for CRC calculation.
// Crc field is defined in the structure only for space consideration.
// It makes sure we declare a DWORD in inter domain request/response structure for CRC.
//
typedef struct _INTER_DOMAIN_PACKET {
    INTER_DOMAIN_PACKET_HEADER  Header;
    UINT32                      Data[USB4_CONTROL_DATA_DW_MAX+1];
} INTER_DOMAIN_PACKET, *PINTER_DOMAIN_PACKET;

typedef INTER_DOMAIN_PACKET    INTER_DOMAIN_PACKET_REQUEST, *PINTER_DOMAIN_PACKET_REQUEST;
typedef INTER_DOMAIN_PACKET    INTER_DOMAIN_PACKET_RESPONSE, *PINTER_DOMAIN_PACKET_RESPONSE;

#pragma pack(pop)

#endif
