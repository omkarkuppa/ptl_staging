/** @file
  Header file for the TBT3 specific config space access

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

#ifndef _TBT3_CONFIG_SPACE_H_
#define _TBT3_CONFIG_SPACE_H_

#include <Base.h>
#include <Library/Usb4ConfigBase.h>

//
// TBT3 config space definitions that are not defined in USB4 spec.
//
#pragma pack(push, 1)

//
// VSC/VSEC ID used in TBT3
//
#define VSC_ID_0        0
#define VSC_ID_1        1
#define VSEC_ID_6       6

#define TBT3_CS_1       1
#define TBT3_CS_13     13

//
// TBT3 adapter configuration space
//

typedef union _TBT3_ADP_CS_0 {
  struct {
    UINT32 VenderId  : BITFIELD_RANGE (0, 15);
    UINT32 ProductId : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} TBT3_ADP_CS_0, *PTBT3_ADP_CS_0;

typedef union _TBT3_ADP_CS_1 {
  struct {
    UINT32 NextCapability : BITFIELD_RANGE (0, 7);
    UINT32 MaxCouterSet   : BITFIELD_RANGE (8, 18);
    UINT32 CcsFlag        : BITFIELD_BIT (19);
    UINT32 Tbt3Rsvd       : BITFIELD_RANGE (20, 23);
    UINT32 Revision       : BITFIELD_RANGE (24, 31);   // TBT3 only
  } Fields;
  UINT32 Value;
} TBT3_ADP_CS_1, *PTBT3_ADP_CS_1;

typedef union _TBT3_ADP_CS_4 {
  struct {
    UINT32 NfcBuffers : BITFIELD_RANGE (0, 9);
    UINT32 Tbt3Rsvd1  : BITFIELD_RANGE (10, 19);
    UINT32 MaxCredits : BITFIELD_RANGE (20, 29);    // TBT3 only, same with TotalBuffers defined in USB4
    UINT32 Plugged    : BITFIELD_BIT (30);
    UINT32 LocK       : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} TBT3_ADP_CS_4, *PTBT3_ADP_CS_4;

//
// TBT3 DP OUT TMU Capability CS_13
//
typedef union _TBT3_TMU_DP_CS_13
{
  struct {
    UINT32 Tbt3Rsvd1 : BITFIELD_RANGE (0, 15);
    UINT32 TmuStable : BITFIELD_RANGE (16, 23);
    UINT32 Tbt3Rsvd2 : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} TBT3_TMU_DP_CS_13, *PTBT3_TMU_DP_CS_13;

//
// TBT3 Router Vendor Specific Capability 1 CS_1
// Plug event disable (bit 2 ~ 6)
// Writing 1 to stop a Router from sending Hot Plug Event Packets for a specific type of Adapter
//
typedef union _TBT3_RT_VSC_1_CS_1 {
  struct {
    UINT32 Tbt3Rsvd1      : BITFIELD_RANGE (0, 1);
    UINT32 DisableUsbHp   : BITFIELD_BIT (2);
    UINT32 DisableLaneHp  : BITFIELD_BIT (3);
    UINT32 DisableDpoutHp : BITFIELD_BIT (4);
    UINT32 DisableDpIn0Hp : BITFIELD_BIT (5);
    UINT32 DisableDpIn1Hp : BITFIELD_BIT (6);
    UINT32 Tbt3Rsvd2      : BITFIELD_RANGE (7, 31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSC_1_CS_1, *PTBT3_RT_VSC_1_CS_1;

//
// TBT3 Router Vendor Specific Extended Capability 6
//
typedef union _TBT3_RT_VSEC_6_CS_0 {
  struct {
    UINT32 Tbt3Rsvd     : BITFIELD_RANGE (0, 7);
    UINT32 CapId        : BITFIELD_RANGE (8, 15);
    UINT32 VsecId       : BITFIELD_RANGE (16, 23);
    UINT32 VsecHeader   : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_CS_0, *PTBT3_RT_VSEC_6_CS_0;

typedef union _TBT3_RT_VSEC_6_CS_1 {
  struct {
    UINT32 NextCap      : BITFIELD_RANGE (0, 15);
    UINT32 VsecLength   : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_CS_1, *PTBT3_RT_VSEC_6_CS_1;

#define TBT3_RT_VSEC_6_CAP_STRUCT_OFFSET    2
typedef union _TBT3_RT_VSEC_6_CAP_STRUCT {
  struct {
    UINT32 Usb4Ports         : BITFIELD_RANGE (0, 3);
    UINT32 Tbt3Rsvd1         : BITFIELD_RANGE (4, 7);
    UINT32 CommonRegionLen   : BITFIELD_RANGE (8, 15);
    UINT32 Usb4PortRegionLen : BITFIELD_RANGE (16, 27);
    UINT32 Tbt3Rsvd2         : BITFIELD_RANGE (28, 31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_CAP_STRUCT, *PTBT3_RT_VSEC_6_CAP_STRUCT;

typedef struct _TBT3_VSEC6_COMMON_REGION {
  TBT3_RT_VSEC_6_CS_0          Cs0;
  TBT3_RT_VSEC_6_CS_1          Cs1;
  TBT3_RT_VSEC_6_CAP_STRUCT    Cs2;
} TBT3_VSEC6_COMMON_REGION, *PTBT3_VSEC6_COMMON_REGION;

#define TBT3_RT_VSEC_6_PORT_MODE_OFFSET    38
typedef union _TBT3_RT_VSEC_6_PORT_MODE {
  struct {
    UINT32 Tbt3Rsvd1 : BITFIELD_RANGE (0, 5);
    // Downstream port reset
    UINT32 Dpr       : BITFIELD_BIT (6);
    UINT32 Tbt3Rsvd2 : BITFIELD_RANGE (7, 31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_PORT_MODE, *PTBT3_RT_VSEC_6_PORT_MODE;

#define TBT3_RT_VSEC_6_CS_42_OFFSET    42
typedef union _TBT3_RT_VSEC_6_CS_42 {
  struct {
    UINT32 Tbt3Rsvd : BITFIELD_RANGE (0, 30);
    UINT32 UsbPlug  : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_CS_42, *PTBT3_RT_VSEC_6_CS_42;

#define TBT3_RT_VSEC_6_PORT_ATTR_OFFSET    141
typedef union _TBT3_RT_VSEC_6_PORT_ATTR {
  struct {
    UINT32 Tbt3Rsvd1      : BITFIELD_RANGE (0, 11);
    UINT32 BondingEnabled : BITFIELD_BIT (12);
    UINT32 Tbt3Rsvd2      : BITFIELD_RANGE (13, 23);
    UINT32 Rtd3Support    : BITFIELD_BIT (24);
    UINT32 Tbt3Rsvd3      : BITFIELD_RANGE (25, 31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_PORT_ATTR, *PTBT3_RT_VSEC_6_PORT_ATTR;

#define TBT3_RT_VSEC_6_LC_SX_CTRL_OFFSET    150
typedef union _TBT3_RT_VSEC_6_LC_SX_CTRL {
  struct {
    UINT32 EnableWakeEvts : BITFIELD_RANGE (0, 10);
    UINT32 Tbt3Rsvd1      : BITFIELD_RANGE (11, 15);
    UINT32 Ln0Configured  : BITFIELD_BIT (16);
    UINT32 Ln0InterDomain : BITFIELD_BIT (17);
    UINT32 Tbt3Rsvd2      : BITFIELD_RANGE (18, 19);
    UINT32 Ln1Configured  : BITFIELD_BIT (20);
    UINT32 Ln1InterDomain : BITFIELD_BIT (21);
    UINT32 Tbt3Rsvd3      : BITFIELD_RANGE (22, 26);
    UINT32 AlpineRidgeCm  : BITFIELD_BIT (27);
    UINT32 IntelCm        : BITFIELD_BIT (28);
    UINT32 StartLinkInit  : BITFIELD_BIT (29);
    UINT32 PortIsUpStream : BITFIELD_BIT (30);
    UINT32 EnterSleep     : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_LC_SX_CTRL, *PTBT3_RT_VSEC_6_LC_SX_CTRL;

#define TBT3_RT_VSEC_6_LINK_REQ_OFFSET    173
typedef union _TBT3_RT_VSEC_6_LINK_REQ {
  struct {
    UINT32 TbtRsvd    : BITFIELD_RANGE (0, 30);
    UINT32 ConnectXhc : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} TBT3_RT_VSEC_6_LINK_REQ, *PTBT3_RT_VSEC_6_LINK_REQ;

//
// TBT3 DP Adpapter Vendor Specific Capability 0 for DP IP register access
//
#define TBT3_DP_IP_CMD_OFFSET     1
#define TBT3_DP_IP_ADDR_OFFSET    2
#define TBT3_DP_WDATA_OFFSET      3
#define TBT3_DP_RDATA_OFFSET      4

#define TBT3_DP_IP_CMD_READ       0
#define TBT3_DP_IP_CMD_WRITE      1

//
// DP IP poll timeout 50 ms according to CM TBT3 Device Support 5.2.1.2
//
#define TBT3_DP_IP_TIMEOUT_MS     50     // ms
#define TBT3_DP_IP_POLL_US        500    // us
#define TBT3_DP_IP_POLL_MAX       (TBT3_DP_IP_TIMEOUT_MS * 1000 / TBT3_DP_IP_POLL_US)

#define TR_DP_IP_CMHS_INIT_ADDR    0xD282

//
// DP VSC0 CS_1
//
typedef union _TBT3_DP_VSC_0_IP_CMD {
  struct {
    UINT32 Valid    : BITFIELD_BIT (0);
    UINT32 Cmd      : BITFIELD_BIT (1);
    UINT32 Tbt3Rsvd : BITFIELD_RANGE (2, 31);
  } Fields;
  UINT32 Value;
} TBT3_DP_VSC_0_IP_CMD, *PTBT3_DP_VSC_0_IP_CMD;

//
// DP VSC0 CS_2
//
typedef union _TBT3_DP_VSC0_IP_ADDR {
  struct {
    UINT32 Addr     : BITFIELD_RANGE (0, 15);
    UINT32 Tbt3Rsvd : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} TBT3_DP_VSC_0_IP_ADDR, *PTBT3_DP_VSC_0_IP_ADDR;

typedef struct _TBT3_DP_VSC_0 {
  TBT3_DP_VSC_0_IP_CMD     IpCmd;
  TBT3_DP_VSC_0_IP_ADDR    IpAddr;
  UINT32                   IpWdata;
  UINT32                   IpRdata;
} TBT3_DP_VSC_0, *PTBT3_DP_VSC_0;

//
// TBT3 Host Interface Adapter Vendor Specific Capability 1 for NVM access
//
#define TBT3_HI_DATA_1_OFFSET       1
#define TBT3_HI_MAIL_IN_OFFSET     17
#define TBT3_HI_MAIL_OUT_OFFSET    18

//
// Host Interface VSC1 Mail In command CS_17
//

#define TBT3_NVM_CMD_WRITE         0
#define TBT3_NVM_CMD_WRITE_AUTH    1
#define TBT3_NVM_CMD_READ          2

//
// NVM command poll timeout according to CM TBT3 Device Support 8.3
//
#define TBT3_NVM_POLL_US                  500    // us

#define TBT3_NVM_READ_TIMEOUT_MS          100    // ms
#define TBT3_NVM_READ_POLL_MAX            (TBT3_NVM_READ_TIMEOUT_MS * 1000 / TBT3_NVM_POLL_US)

#define TBT3_NVM_WRITE_TIMEOUT_MS         500    // ms
#define TBT3_NVM_WRITE_POLL_MAX           (TBT3_NVM_WRITE_TIMEOUT_MS * 1000 / TBT3_NVM_POLL_US)

#define TBT3_NVM_WRITE_AUTH_TIMEOUT_MS    2000   // ms
#define TBT3_NVM_WRITE_AUTH_POLL_MAX      (TBT3_NVM_WRITE_AUTH_TIMEOUT_MS * 1000 / TBT3_NVM_POLL_US)

//
// TBT3 NVM command structure
//
typedef union _TBT3_NVM_CMD {
  struct {
    UINT32 OpReq     : BITFIELD_BIT (0);
    UINT32 Tbt3Rsvd  : BITFIELD_BIT (1);
    UINT32 NvmOffset : BITFIELD_RANGE (2, 23);
    UINT32 DwCount   : BITFIELD_RANGE (24, 27);
    UINT32 Cmd       : BITFIELD_RANGE (28, 31);
  } Fields;
  UINT32 Value;
} TBT3_NVM_CMD, *PTBT3_NVM_CMD;

//
// TBT3 NVM command response structure
//
typedef union _TBT3_NVM_CMD_RESP {
  struct {
    UINT32 Status    : BITFIELD_RANGE (0, 3);
    UINT32 Cmd       : BITFIELD_RANGE (4, 7);
    UINT32 Tbt3Rsvd1 : BITFIELD_RANGE (8, 28);
    UINT32 Response  : BITFIELD_BIT (29);
    UINT32 Tbt3Rsvd2 : BITFIELD_RANGE (30, 31);
  } Fields;
  UINT32 Value;
} TBT3_NVM_CMD_RESP, *PTBT3_NVM_CMD_RESP;

#pragma pack(pop)

#endif
