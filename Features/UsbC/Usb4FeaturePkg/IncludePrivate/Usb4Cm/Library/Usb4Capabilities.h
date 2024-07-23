/** @file
  Header file for capabilities defined in USB4 configuration space.

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

#ifndef _USB4_CAPABILITIES_H_
#define _USB4_CAPABILITIES_H_

#include <Uefi.h>
#include <Library/Usb4ConfigBase.h>

#pragma pack(push, 1)

//
// USB4 Capability ID
//
#define USB4_RT_CAP_ID_TMU                3
#define USB4_RT_CAP_ID_VENDOR             5

#define USB4_ADP_CAP_ID_TMU               3
#define USB4_ADP_CAP_ID_USB4_PORT         6
#define USB4_ADP_CAP_ID_LANE              1
#define USB4_ADP_CAP_ID_PROTOCOL          4
#define USB4_ADP_CAP_ID_PCIE              4
#define USB4_ADP_CAP_ID_USB3              4
#define USB4_ADP_CAP_ID_DP                4
#define USB4_ADP_CAP_ID_VENDOR            5

//
// USB4 Capability DWORD offset
//
#define USB4_CAP_DW_0          0
#define USB4_CAP_DW_1          1
#define USB4_CAP_DW_2          2
#define USB4_CAP_DW_3          3
#define USB4_CAP_DW_4          4
#define USB4_CAP_DW_5          5
#define USB4_CAP_DW_6          6
#define USB4_CAP_DW_7          7
#define USB4_CAP_DW_18        18
#define USB4_CAP_DW_19        19

//
// Router TMU Configuration Cap CS3 - TSPacketInterval in units of 1 us
//
#define TSPKT_INTERVAL_DISABLED       0
#define TSPKT_INTERVAL_HIFI           16
#define TSPKT_INTERVAL_LOWRES         1000

//
// Link Speed (4 bits field) in LANE Adapter Configuration Capability
// Bit 0     - Reserved
// Bit 1     - Gen4
// Bit 2     - Gen3
// Bit 3     - Gen2
//
#define LANE_LINK_SPEED_GEN4           0x02
#define LANE_LINK_SPEED_GEN3           0x04
#define LANE_LINK_SPEED_GEN2           0x08

//
// Link Width (6 bits field) in LANE Adapter Configuration Capability
// bit 0     - x1
// bit 1     - x2
// bit [5:2] - Reserved
//
#define LANE_LINK_WIDTH_x1             0x01
#define LANE_LINK_WIDTH_x2             0x02

//
// Target Link Speed (4 bits field) in LANE Adapter Configuration Capability
// 1000b     - Gen2
// 1100b     - Gen3
//
#define LANE_LINK_TARGET_SPEED_GEN2          0x08
#define LANE_LINK_TARGET_SPEED_GEN3          0x0C

//
// Target Link Width (6 bits field) in LANE Adapter Configuration Capability
// 000001b   - Establish two Single-Lane Links
// 000011b   - Establish a Dual-Lane Link
//
#define LANE_LINK_TARGET_WIDTH_x1             0x01
#define LANE_LINK_TARGET_WIDTH_x2             0x03

//
// Lane Adapter Capability CS 1 - LANE Adapter State
//
typedef enum {
  LANE_STATE_DISABLED          = 0x00,
  LANE_STATE_TRAINING_BONDING  = 0x01,
  LANE_STATE_CL0               = 0x02,
  LANE_STATE_TX_CL0            = 0x03,
  LANE_STATE_RX_CL0            = 0x04,
  LANE_STATE_CL1               = 0x05,
  LANE_STATE_CL2               = 0x06,
  LANE_STATE_CLD               = 0x07,
  LANE_STATE_MAX               = 0x07,
  LANE_STATE_INVALID           = 0xFF
} LANE_ADAPTER_STATE, *PLANE_ADAPTER_STATE;

//
// USB4 Port Capability CS 1 - AT command target
//
#define PG_AT_TARGET_ROUTER         0x00
#define PG_AT_TARGET_LINK_PARTNER   0x01
#define PG_AT_TARGET_RETIMER        0x02

//
// USB4 Port Capability CS 18 - Cable USB4 Version
//
#define CABLE_USB4_VERSION_MAJOR (x)  ((UINT32)x>>4)
#define CABLE_USB4_VERSION_MINOR (x)  (x & 0x0F)

//
// USB3 Adapter Capabliity CS4 - Port Link State (PLS) definition
//
typedef enum {
  USB3_PLS_U0        = 0x00,
  USB3_PLS_U2        = 0x02,
  USB3_PLS_U3        = 0x03,
  USB3_PLS_DISABLED  = 0x04,
  USB3_PLS_RX_DETECT = 0x05,
  USB3_PLS_INACTIVE  = 0x06,
  USB3_PLS_POLLING   = 0x07,
  USB3_PLS_RECOVERY  = 0x08,
  USB3_PLS_HOT_RESET = 0x09,
  USB3_PLS_RESUME    = 0x0F,
  USB3_PLS_MAX       = 0x0F,
  USB3_PLS_INVALID   = 0xFF
} USB3_PLS, *PUSB3_PLS;

//
// DP Capability DWORD offset
//
#define DP_LOCAL_CAP          4
#define DP_REMOTE_CAP         5
#define DP_STATUS             6
#define DP_STATUS_CTRL        6
#define DP_COMMON_CAP         7

//
// PCIe Adapter Capability CS 0 - PCIe LTSSM states
//
typedef enum {
  PCIE_LTSSM_STATE_DETECT        = 0x00,
  PCIE_LTSSM_STATE_POLLING       = 0x01,
  PCIE_LTSSM_STATE_CONFIG        = 0x02,
  PCIE_LTSSM_STATE_CONFIG_IDLE   = 0x03,
  PCIE_LTSSM_STATE_RECOVERY      = 0x04,
  PCIE_LTSSM_STATE_RECOVERY_IDLE = 0x05,
  PCIE_LTSSM_STATE_L0            = 0x06,
  PCIE_LTSSM_STATE_L1            = 0x07,
  PCIE_LTSSM_STATE_L2            = 0x08,
  PCIE_LTSSM_STATE_DISABLED      = 0x09,
  PCIE_LTSSM_STATE_HOT_RESET     = 0x0A,
  PCIE_LTSSM_STATE_MAX           = 0x0A,
  PCIE_LTSSM_STATE_INVALID       = 0xFF
} PCIE_LTSSM_STATE, *PPCIE_LTSSM_STATE;

//
// General USB4 capability header
//
typedef struct _USB4_CAP_HEADER {
    UINT32 NextCap    : BITFIELD_RANGE (0, 7);
    UINT32 CapId      : BITFIELD_RANGE (8, 15);
    UINT32 Reserved   : BITFIELD_RANGE (16, 31);
} USB4_CAP_HEADER, *PUSB4_CAP_HEADER;

//
// Router TMU Configuration Capability
//
typedef union _USB4_RT_CAP_TMU_CS_0 {
  struct {
    UINT32 NextCap          : BITFIELD_RANGE (0, 7);
    UINT32 CapId            : BITFIELD_RANGE (8, 15);
    UINT32 FreqWindow       : BITFIELD_RANGE (16, 26);
    UINT32 TimeDisruption   : BITFIELD_BIT (27);
    UINT32 Reserved         : BITFIELD_RANGE (28, 29);
    UINT32 UniDirCap        : BITFIELD_BIT (30);
    UINT32 InterDomainEn    : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_RT_CAP_TMU_CS_0, *PUSB4_RT_CAP_TMU_CS_0;

typedef union _USB4_RT_CAP_TMU_CS_1 {
  struct {
    UINT32 LocalTimeLow;
  } Fields;
  UINT32 Value;
} USB4_RT_CAP_TMU_CS_1, *PUSB4_RT_CAP_TMU_CS_1;

typedef union _USB4_RT_CAP_TMU_CS_2 {
  struct {
    UINT32 LocalTimeMiddle;
  } Fields;
  UINT32 Value;
} USB4_RT_CAP_TMU_CS_2, *PUSB4_RT_CAP_TMU_CS_2;

typedef union _USB4_RT_CAP_TMU_CS_3 {
  struct {
    UINT32 LocalTimeHigh  : BITFIELD_RANGE (0, 15);
    UINT32 TsPktInterval  : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_RT_CAP_TMU_CS_3, *PUSB4_RT_CAP_TMU_CS_3;

//
// Vendor Specific Capability / Vendor Specific Extended Capability
//
typedef union _USB4_CAP_VSC_CS_0 {
  struct {
    UINT32 NextCap      : BITFIELD_RANGE (0, 7);
    UINT32 CapId        : BITFIELD_RANGE (8, 15);
    UINT32 VscId        : BITFIELD_RANGE (16, 23);
    UINT32 VscLength    : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_CAP_VSC_CS_0, *PUSB4_CAP_VSC_CS_0;

typedef union _USB4_CAP_VSEC_CS_0 {
  struct {
    UINT32 Reserved     : BITFIELD_RANGE (0, 7);
    UINT32 CapId        : BITFIELD_RANGE (8, 15);
    UINT32 VsecId       : BITFIELD_RANGE (16, 23);
    UINT32 VsecHeader   : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_CAP_VSEC_CS_0, *PUSB4_CAP_VSEC_CS_0;

typedef union _USB4_CAP_VSEC_CS_1 {
  struct {
    UINT32 NextCap      : BITFIELD_RANGE (0, 15);
    UINT32 VsecLength   : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_CAP_VSEC_CS_1, *PUSB4_CAP_VSEC_CS_1;

typedef USB4_CAP_VSC_CS_0     USB4_RT_CAP_VSC_CS_0, *PUSB4_RT_CAP_VSC_CS_0;
typedef USB4_CAP_VSEC_CS_0    USB4_RT_CAP_VSEC_CS_0, *PUSB4_RT_CAP_VSEC_CS_0;
typedef USB4_CAP_VSEC_CS_1    USB4_RT_CAP_VSEC_CS_1, *PUSB4_RT_CAP_VSEC_CS_1;

typedef struct _USB4_RT_CAP_VSC {
  USB4_RT_CAP_VSC_CS_0    CapCs0;
} USB4_RT_CAP_VSC, *PUSB4_RT_CAP_VSC;

typedef struct _USB4_RT_CAP_VENDOR_EX {
  USB4_RT_CAP_VSEC_CS_0    CapCs0;
  USB4_RT_CAP_VSEC_CS_1    CapCs1;
} USB4_RT_CAP_VSEC, *PUSB4_RT_CAP_VSEC;

//
// TMU Adapter Configuration Capability
//
typedef union _USB4_ADP_CAP_TMU_CS_0 {
  struct {
    UINT32 NextCap    : BITFIELD_RANGE (0, 7);
    UINT32 CapId      : BITFIELD_RANGE (8, 15);
    UINT32 Reserved   : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_0, *PUSB4_ADP_CAP_TMU_CS_0;

typedef union _USB4_ADP_CAP_TMU_CS_1 {
  struct {
    UINT32 TxTimeToWire;
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_1, *PUSB4_ADP_CAP_TMU_CS_1;

typedef union _USB4_ADP_CAP_TMU_CS_2 {
  struct {
    UINT32 RxTimeToWire;
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_2, *PUSB4_ADP_CAP_TMU_CS_2;

typedef union _USB4_ADP_CAP_TMU_CS_3 {
  struct {
    UINT32 Reserved             : BITFIELD_RANGE (0, 28);
    UINT32 EnableUdm            : BITFIELD_BIT (29);
    UINT32 Idm                  : BITFIELD_BIT (30);
    UINT32 Ids                  : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_3, *PUSB4_ADP_CAP_TMU_CS_3;

typedef union _USB4_ADP_CAP_TMU_CS_4 {
  struct {
    UINT32 RxTsnosCnt    : BITFIELD_RANGE (0, 15);
    UINT32 TxTsnosCnt    : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_4, *PUSB4_ADP_CAP_TMU_CS_4;

typedef union _USB4_ADP_CAP_TMU_CS_5 {
  struct {
    UINT32 RxPktCnt    : BITFIELD_RANGE (0, 15);
    UINT32 TxPktCnt    : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_5, *PUSB4_ADP_CAP_TMU_CS_5;

typedef union _USB4_ADP_CAP_TMU_CS_6 {
  struct {
    UINT32 Reserved               : BITFIELD_RANGE (0, 15);
    UINT32 VenderDefined          : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_6, *PUSB4_ADP_CAP_TMU_CS_6;

typedef union _USB4_ADP_CAP_TMU_CS_7 {
  struct {
    UINT32 LostTsnosCnt      : BITFIELD_RANGE (0, 9);
    UINT32 LostPktCnt        : BITFIELD_RANGE (10, 19);
    UINT32 BadPktCnt         : BITFIELD_RANGE (20, 29);
    UINT32 Reserved          : BITFIELD_RANGE (30, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_7, *PUSB4_ADP_CAP_TMU_CS_7;

typedef struct _USB4_ADP_CAP_TMU {
  USB4_ADP_CAP_TMU_CS_0     CapCs0;
  USB4_ADP_CAP_TMU_CS_1     CapCs1;
  USB4_ADP_CAP_TMU_CS_2     CapCs2;
  USB4_ADP_CAP_TMU_CS_3     CapCs3;
  USB4_ADP_CAP_TMU_CS_4     CapCs4;
  USB4_ADP_CAP_TMU_CS_5     CapCs5;
  USB4_ADP_CAP_TMU_CS_6     CapCs6;
  USB4_ADP_CAP_TMU_CS_7     CapCs7;
} USB4_ADP_CAP_TMU, *PUSB4_ADP_CAP_TMU;

typedef union _USB4_ADP_CAP_TNU_CS_8 {
  struct {
    UINT32 ReplenishTimeout   : BITFIELD_RANGE (0, 14);
    UINT32 EudmEnable         : BITFIELD_BIT (15);
    UINT32 ReplenishThreshold : BITFIELD_RANGE (16, 25);
    UINT32 Reserved           : BITFIELD_RANGE (26, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_8, *PUSB4_ADP_CAP_TMU_CS_8;

#define TMU_EUDM_DIRSWITCHN               100
#define TMU_EUDM_ADP_TIME_SYNC_INTERVAL   16

typedef union _USB4_ADP_CAP_TMU_CS_9 {
  struct {
    UINT32 ReplenishN      : BITFIELD_RANGE (0, 7);
    UINT32 DirSwitchN      : BITFIELD_RANGE (8, 15);
    UINT32 AdpSyncInterval : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_TMU_CS_9, *PUSB4_ADP_CAP_TMU_CS_9;

typedef struct _USB4V2_ADP_CAP_TMU {
  USB4_ADP_CAP_TMU_CS_0     CapCs0;
  USB4_ADP_CAP_TMU_CS_1     CapCs1;
  USB4_ADP_CAP_TMU_CS_2     CapCs2;
  USB4_ADP_CAP_TMU_CS_3     CapCs3;
  USB4_ADP_CAP_TMU_CS_4     CapCs4;
  USB4_ADP_CAP_TMU_CS_5     CapCs5;
  USB4_ADP_CAP_TMU_CS_6     CapCs6;
  USB4_ADP_CAP_TMU_CS_7     CapCs7;
  USB4_ADP_CAP_TMU_CS_8     CapCs8;
  USB4_ADP_CAP_TMU_CS_9     CapCs9;
} USB4V2_ADP_CAP_TMU, *PUSB4V2_ADP_CAP_TMU;

//
// LANE Adapter Configuration Capability
//

typedef union _USB4_ADP_CAP_LANE_CS_0 {
  struct {
    UINT32 NextCap              : BITFIELD_RANGE (0, 7);
    UINT32 CapId                : BITFIELD_RANGE (8, 15);
    UINT32 SupportedLinkSpeeds  : BITFIELD_RANGE (16, 19);
    UINT32 SupportedLinkWidths  : BITFIELD_RANGE (20, 25);
    UINT32 Cl0sSupport          : BITFIELD_BIT (26);
    UINT32 Cl1Support           : BITFIELD_BIT (27);
    UINT32 Cl2Support           : BITFIELD_BIT (28);
    UINT32 Reserved             : BITFIELD_RANGE (29,31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_LANE_CS_0, *PUSB4_ADP_CAP_LANE_CS_0;

typedef union _USB4_ADP_CAP_LANE_CS_1 {
  struct {
    UINT32 LinkSpeedTarget   : BITFIELD_RANGE (0, 3);
    UINT32 LinkWidthTarget   : BITFIELD_RANGE (4, 9);
    UINT32 Cl0sEnable        : BITFIELD_BIT (10);
    UINT32 Cl1Enable         : BITFIELD_BIT (11);
    UINT32 Cl2Enable         : BITFIELD_BIT (12);
    UINT32 Reserved1         : BITFIELD_BIT (13);
    UINT32 LaneDisable       : BITFIELD_BIT (14);
    UINT32 LaneBonding       : BITFIELD_BIT (15);
    UINT32 CurLinkSpeed      : BITFIELD_RANGE (16, 19);
    UINT32 CurLinkWidth      : BITFIELD_RANGE (20, 25);
    UINT32 AdapterState      : BITFIELD_RANGE (26, 29);
    UINT32 PmSecondary       : BITFIELD_BIT (30);
    UINT32 Reserved2         : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_LANE_CS_1, *PUSB4_ADP_CAP_LANE_CS_1;

typedef union _USB4_ADP_CAP_LANE_CS_2 {
  struct {
    UINT32 LogicalLayerErr   : BITFIELD_RANGE (0, 6);
    UINT32 Reserved1         : BITFIELD_RANGE (7, 15);
    UINT32 LogicalLayerErrEn : BITFIELD_RANGE (16, 22);
    UINT32 Reserved2         : BITFIELD_RANGE (23, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_LANE_CS_2, *PUSB4_ADP_CAP_LANE_CS_2;

typedef struct _USB4_ADP_CAP_LANE {
  USB4_ADP_CAP_LANE_CS_0    CapCs0;
  USB4_ADP_CAP_LANE_CS_1    CapCs1;
  USB4_ADP_CAP_LANE_CS_2    CapCs2;
} USB4_ADP_CAP_LANE, *PUSB4_ADP_CAP_LANE;

//
// USB4 Port Capability
//
typedef union _USB4_ADP_CAP_USB4_PORT_CS_0 {
  struct {
    UINT32 NextCap    : BITFIELD_RANGE (0, 7);
    UINT32 CapId      : BITFIELD_RANGE (8, 15);
    UINT32 Reserved   : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB4_PORT_CS_0, *PUSB4_ADP_CAP_USB4_PORT_CS_0;

typedef union _USB4_ADP_CAP_USB4_PORT_CS_1 {
  struct {
    UINT32 AtRegAddr     : BITFIELD_RANGE (0, 7);
    UINT32 AtRegLen      : BITFIELD_RANGE (8, 15);
    UINT32 AtRegTarget   : BITFIELD_RANGE (16, 18);
    UINT32 Reserved1     : BITFIELD_BIT (19);
    UINT32 RetimerIndex  : BITFIELD_RANGE (20,23);
    UINT32 Wnr           : BITFIELD_BIT (24);
    UINT32 NoResponse    : BITFIELD_BIT (25);
    UINT32 ResultCode    : BITFIELD_BIT (26);
    UINT32 Reserved2     : BITFIELD_RANGE (27, 30);
    UINT32 Pending       : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB4_PORT_CS_1, *PUSB4_ADP_CAP_USB4_PORT_CS_1;

typedef union _USB4_ADP_CAP_USB4_PORT_CS_18 {
  struct {
    UINT32 CableUsb4Ver       : BITFIELD_RANGE (0, 7);
    UINT32 BondingEnabled     : BITFIELD_BIT (8);
    UINT32 Tbt3CompatibleMode : BITFIELD_BIT (9);
    UINT32 LinkClxSupport     : BITFIELD_BIT (10);
    UINT32 Gen2RsFecEnabled   : BITFIELD_BIT (11);
    UINT32 Gen3RsFecEnabled   : BITFIELD_BIT (12);
    UINT32 RouterDetected     : BITFIELD_BIT (13);
    UINT32 Reserved1          : BITFIELD_RANGE (14, 15);
    UINT32 WakeOnConnect      : BITFIELD_BIT (16);
    UINT32 WakeOnDisconnect   : BITFIELD_BIT (17);
    UINT32 WakeOnUsb4Wake     : BITFIELD_BIT (18);
    UINT32 WakeOnInterDomain  : BITFIELD_BIT (19);
    UINT32 Reserved2          : BITFIELD_RANGE (20, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB4_PORT_CS_18, *PUSB4_ADP_CAP_USB4_PORT_CS_18;

typedef union _USB4_ADP_CAP_USB4_PORT_CS_19 {
  struct {
    UINT32 DsPortReset          : BITFIELD_BIT (0);
    UINT32 Gen2RsFecRequest     : BITFIELD_BIT (1);
    UINT32 Gen3RsFecRequest     : BITFIELD_BIT (2);
    UINT32 PgIsConfigured       : BITFIELD_BIT (3);
    UINT32 PgIsInterDomain      : BITFIELD_BIT (4);
    UINT32 Reserved1            : BITFIELD_RANGE (5, 15);
    UINT32 WakeOnConnectEn      : BITFIELD_BIT (16);
    UINT32 WakeOnDisconnectEn   : BITFIELD_BIT (17);
    UINT32 WakeOnUsb4WakeEn     : BITFIELD_BIT (18);
    UINT32 WakeOnInterDomainEn  : BITFIELD_BIT (19);
    UINT32 Reserved2            : BITFIELD_RANGE (20, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB4_PORT_CS_19, *PUSB4_ADP_CAP_USB4_PORT_CS_19;

typedef struct _USB4_ADP_CAP_USB4_PORT {
  USB4_ADP_CAP_USB4_PORT_CS_0     CapCs0;
  USB4_ADP_CAP_USB4_PORT_CS_1     CapCs1;
  UINT32                          CapData[16];
  USB4_ADP_CAP_USB4_PORT_CS_18    CapCs18;
  USB4_ADP_CAP_USB4_PORT_CS_19    CapCs19;
} USB4_ADP_CAP_USB4_PORT, *PUSB4_ADP_CAP_USB4_PORT;

//
// USB3 Adpapter Capabilities
//
typedef union _USB4_ADP_CAP_USB3_CS_0 {
  struct {
    UINT32 NextCap      : BITFIELD_RANGE (0, 7);
    UINT32 CapId        : BITFIELD_RANGE (8, 15);
    UINT32 Reserved     : BITFIELD_RANGE (16, 29);
    UINT32 Valid        : BITFIELD_BIT (30);
    UINT32 PathEnable   : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB3_CS_0, *PUSB4_ADP_CAP_USB3_CS_0;

typedef union _USB4_ADP_CAP_ADP_USB3_CS_1 {
  struct {
    UINT32 UsBwConsumed    : BITFIELD_RANGE (0, 11);
    UINT32 DsBwConsumed    : BITFIELD_RANGE (12, 23);
    UINT32 Reserved        : BITFIELD_RANGE (24, 30);
    UINT32 HcAck           : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB3_CS_1, *PUSB4_ADP_CAP_USB3_CS_1;

typedef union _USB4_ADP_CAP_ADP_USB3_CS_2 {
  struct {
    UINT32 UsBwAllocated   : BITFIELD_RANGE (0, 11);
    UINT32 DsBwAllocated   : BITFIELD_RANGE (12, 23);
    UINT32 Reserved        : BITFIELD_RANGE (24, 30);
    UINT32 CmRequest       : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB3_CS_2, *PUSB4_ADP_CAP_USB3_CS_2;

typedef union _USB4_ADP_CAP_ADP_USB3_CS_3 {
  struct {
    UINT32 Scale           : BITFIELD_RANGE (0, 5);
    UINT32 Reserved        : BITFIELD_RANGE (6, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB3_CS_3, *PUSB4_ADP_CAP_USB3_CS_3;

typedef union _USB4_ADP_CAP_USB3_CS_4 {
  struct {
    UINT32 ActualLinkRate         : BITFIELD_RANGE (0, 6);
    UINT32 UsbLinkValid           : BITFIELD_BIT (7);
    UINT32 PortLinkState          : BITFIELD_RANGE (8, 11);
    UINT32 MaxSupportedLinkRate   : BITFIELD_RANGE (12, 18);
    UINT32 Reserved               : BITFIELD_RANGE (19, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_USB3_CS_4, *PUSB4_ADP_CAP_USB3_CS_4;

typedef struct _USB4_ADP_CAP_ADP_USB {
  USB4_ADP_CAP_USB3_CS_0   CapCs0;
  USB4_ADP_CAP_USB3_CS_1   CapCs1;
  USB4_ADP_CAP_USB3_CS_2   CapCs2;
  // comment out due to TGL ITBT doesn't support it
/*
  USB4_ADP_CAP_USB3_CS_3   CapCs3;
  USB4_ADP_CAP_USB3_CS_4   CapCs4;
*/
} USB4_ADP_CAP_USB3, *PUSB4_ADP_CAP_USB3;

//
// DP Adapter Configuration Capability
//

typedef union _USB4_ADP_CAP_DP_CS_0 {
  struct {
    UINT32 NextCap      : BITFIELD_RANGE (0, 7);
    UINT32 CapId        : BITFIELD_RANGE (8, 15);
    UINT32 VideoHopId   : BITFIELD_RANGE (16, 26);
    UINT32 Reserved     : BITFIELD_RANGE (27, 29);
    UINT32 AuxEnable    : BITFIELD_BIT (30);
    UINT32 VideoEnable  : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_CS_0, *PUSB4_ADP_CAP_DP_CS_0;

typedef USB4_ADP_CAP_DP_CS_0     USB4_ADP_CAP_DP_IN_CS_0, *PUSB4_ADP_CAP_DP_IN_CS_0;
typedef USB4_ADP_CAP_DP_CS_0     USB4_ADP_CAP_DP_OUT_CS_0, *PUSB4_ADP_CAP_DP_OUT_CS_0;

typedef union _USB4_ADP_CAP_DP_CS_1 {
  struct {
    UINT32 AuxTxHopId   : BITFIELD_RANGE (0, 10);
    UINT32 AuxRxHopId   : BITFIELD_RANGE (11, 21);
    UINT32 Reserved     : BITFIELD_RANGE (22, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_CS_1, *PUSB4_ADP_CAP_DP_CS_1;

typedef USB4_ADP_CAP_DP_CS_1     USB4_ADP_CAP_DP_IN_CS_1, *PUSB4_ADP_CAP_DP_IN_CS_1;
typedef USB4_ADP_CAP_DP_CS_1     USB4_ADP_CAP_DP_OUT_CS_1, *PUSB4_ADP_CAP_DP_OUT_CS_1;

typedef union _USB4_ADP_CAP_DP_IN_CS_2 {
  struct {
    UINT32 Reserved1    : BITFIELD_RANGE (0, 2);
    UINT32 SwLinkInit   : BITFIELD_BIT (3);
    UINT32 Reserved2    : BITFIELD_RANGE (4, 5);
    UINT32 Hpd          : BITFIELD_BIT (6);
    UINT32 Reserved3    : BITFIELD_RANGE (7, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_IN_CS_2, *PUSB4_ADP_CAP_DP_IN_CS_2;

typedef union _USB4_ADP_CAP_DP_OUT_CS_2 {
  struct {
    UINT32 Reserved1    : BITFIELD_RANGE (0, 2);
    UINT32 SwLinkInit   : BITFIELD_BIT (3);
    UINT32 Reserved2    : BITFIELD_RANGE (4, 5);
    UINT32 Hpd          : BITFIELD_BIT (6);
    UINT32 Reserved3    : BITFIELD_BIT (7);
    UINT32 MaxAccCycles : BITFIELD_RANGE (8, 23);
    UINT32 Reserved4    : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_OUT_CS_2, *PUSB4_ADP_CAP_DP_OUT_CS_2;

typedef union _USB4_ADP_CAP_DP_IN_CS_3 {
  struct {
    UINT32 Reserved1   : BITFIELD_RANGE (0, 8);
    UINT32 HpdOutClear : BITFIELD_BIT (9);
    UINT32 HpdOutSet   : BITFIELD_BIT (10);
    UINT32 Reserved2   : BITFIELD_RANGE (11, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_IN_CS_3, *PUSB4_ADP_CAP_DP_IN_CS_3;

typedef union _USB4_ADP_CAP_DP_OUT_CS_3 {
  struct {
    UINT32 Reserved    : BITFIELD_RANGE (0, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_OUT_CS_3, *PUSB4_ADP_CAP_DP_OUT_CS_3;

//
// DP Local/Remote capabilities for CS4 and CS5
//
typedef union _USB4_ADP_CAP_DP_CS_4_5 {
  struct {
    UINT32 ProtocolAdpRev     : BITFIELD_RANGE (0, 3);
    UINT32 MaxDpcdRev         : BITFIELD_RANGE (4, 7);
    UINT32 MaxLinkRate        : BITFIELD_RANGE (8, 11);
    UINT32 MaxLaneCnt         : BITFIELD_RANGE (12, 14);
    UINT32 MstCapSupport      : BITFIELD_BIT (15);
    UINT32 Reserved1          : BITFIELD_RANGE (16, 21);
    UINT32 Tps3CapSupport     : BITFIELD_BIT (22);
    UINT32 Reserved2          : BITFIELD_BIT (23);
    UINT32 Tps4CapSupport     : BITFIELD_BIT (24);
    UINT32 FecNotSupport      : BITFIELD_BIT (25);
    UINT32 SecSplitCapSupport : BITFIELD_BIT (26);
    UINT32 LttprNotSupport    : BITFIELD_BIT (27);
    UINT32 Reserved3          : BITFIELD_BIT (28);
    UINT32 DscNotSupport      : BITFIELD_BIT (29);
    UINT32 Reserved4          : BITFIELD_RANGE (30, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_CS_4_5, *PUSB4_ADP_CAP_DP_CS_4_5;

//
// Static local capabilities of the DP Adapter
//
typedef USB4_ADP_CAP_DP_CS_4_5     USB4_ADP_CAP_DP_IN_CS_4, *PUSB4_ADP_CAP_DP_IN_CS_4;
typedef USB4_ADP_CAP_DP_CS_4_5     USB4_ADP_CAP_DP_OUT_CS_4, *PUSB4_ADP_CAP_DP_OUT_CS_4;

//
// Static local capabilities of the paired DP Adapter
//
typedef USB4_ADP_CAP_DP_CS_4_5     USB4_ADP_CAP_DP_IN_CS_5, *PUSB4_ADP_CAP_DP_IN_CS_5;
typedef USB4_ADP_CAP_DP_CS_4_5     USB4_ADP_CAP_DP_OUT_CS_5, *PUSB4_ADP_CAP_DP_OUT_CS_5;

//
// Only valid after DP link is established
//
typedef union _USB4_ADP_CAP_DP_IN_CS_6 {
  struct {
    UINT32 LaneCnt   : BITFIELD_RANGE (0, 2);
    UINT32 Reserved1 : BITFIELD_RANGE (3, 7);
    UINT32 LinkRate  : BITFIELD_RANGE (8, 11);
    UINT32 Reserved2 : BITFIELD_RANGE (12, 16);
    UINT32 Reserved3 : BITFIELD_RANGE (17, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_IN_CS_6, *PUSB4_ADP_CAP_DP_IN_CS_6;

typedef union _USB4_ADP_CAP_DP_OUT_CS_6 {
  struct {
    UINT32 LaneCnt      : BITFIELD_RANGE (0, 2);
    UINT32 Reserved1    : BITFIELD_RANGE (3, 7);
    UINT32 LinkRate     : BITFIELD_RANGE (8, 11);
    UINT32 Reserved2    : BITFIELD_RANGE (12, 16);
    UINT32 Reserved3    : BITFIELD_RANGE (17, 24);
    UINT32 CmHandshake  : BITFIELD_BIT (25);
    UINT32 DpInUsb4Flag : BITFIELD_BIT (26);
    UINT32 Reserved4    : BITFIELD_RANGE (27, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_OUT_CS_6, *PUSB4_ADP_CAP_DP_OUT_CS_6;

//
// Lowest common capabilities between DP adapter and its paired DP adapter
//
typedef union _USB4_ADP_CAP_DP_IN_CS_7 {
  struct {
    UINT32 ProtocolAdpRev     : BITFIELD_RANGE (0, 3);
    UINT32 MaxDpcdRev         : BITFIELD_RANGE (4, 7);
    UINT32 MaxLinkRate        : BITFIELD_RANGE (8, 11);
    UINT32 MaxLaneCnt         : BITFIELD_RANGE (12, 14);
    UINT32 MstCapSupport      : BITFIELD_BIT (15);
    UINT32 Reserved1          : BITFIELD_RANGE (16, 21);
    UINT32 Tps3CapSupport     : BITFIELD_BIT (22);
    UINT32 Reserved2          : BITFIELD_BIT (23);
    UINT32 Tps4CapSupport     : BITFIELD_BIT (24);
    UINT32 FecNotSupport      : BITFIELD_BIT (25);
    UINT32 SecSplitCapSupport : BITFIELD_BIT (26);
    UINT32 LttprNotSupport    : BITFIELD_BIT (27);
    UINT32 Reserved3          : BITFIELD_BIT (28);
    UINT32 DscNotSupport      : BITFIELD_BIT (29);
    UINT32 Reserved4          : BITFIELD_BIT (30);
    UINT32 DprxCapReadDone    : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_IN_CS_7, *PUSB4_ADP_CAP_DP_IN_CS_7;

typedef union _USB4_ADP_CAP_DP_OUT_CS_7 {
  struct {
    UINT32 ProtocolAdpRev     : BITFIELD_RANGE (0, 3);
    UINT32 MaxDpcdRev         : BITFIELD_RANGE (4, 7);
    UINT32 MaxLinkRate        : BITFIELD_RANGE (8, 11);
    UINT32 MaxLaneCnt         : BITFIELD_RANGE (12, 14);
    UINT32 MstCapSupport      : BITFIELD_BIT (15);
    UINT32 Reserved1          : BITFIELD_RANGE (16, 21);
    UINT32 Tps3CapSupport     : BITFIELD_BIT (22);
    UINT32 Reserved2          : BITFIELD_BIT (23);
    UINT32 Tps4CapSupport     : BITFIELD_BIT (24);
    UINT32 FecNotSupport      : BITFIELD_BIT (25);
    UINT32 SecSplitCapSupport : BITFIELD_BIT (26);
    UINT32 LttprNotSupport    : BITFIELD_BIT (27);
    UINT32 Reserved3          : BITFIELD_BIT (28);
    UINT32 DscNotSupport      : BITFIELD_BIT (29);
    UINT32 Reserved4          : BITFIELD_RANGE (30, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_DP_OUT_CS_7, *PUSB4_ADP_CAP_DP_OUT_CS_7;

typedef struct _USB4_ADP_CAP_DP_IN {
  USB4_ADP_CAP_DP_IN_CS_0       CapCs0;
  USB4_ADP_CAP_DP_IN_CS_1       CapCs1;
  USB4_ADP_CAP_DP_IN_CS_2       CapCs2;
  USB4_ADP_CAP_DP_IN_CS_3       CapCs3;
  USB4_ADP_CAP_DP_IN_CS_4       CapCs4;
  USB4_ADP_CAP_DP_IN_CS_5       CapCs5;
  USB4_ADP_CAP_DP_IN_CS_6       CapCs6;
  USB4_ADP_CAP_DP_IN_CS_7       CapCs7;
} USB4_ADP_CAP_DP_IN, *PUSB4_ADP_CAP_DP_IN;

typedef struct _USB4_ADP_CAP_DP_OUT {
  USB4_ADP_CAP_DP_OUT_CS_0       CapCs0;
  USB4_ADP_CAP_DP_OUT_CS_1       CapCs1;
  USB4_ADP_CAP_DP_OUT_CS_2       CapCs2;
  USB4_ADP_CAP_DP_OUT_CS_3       CapCs3;
  USB4_ADP_CAP_DP_OUT_CS_4       CapCs4;
  USB4_ADP_CAP_DP_OUT_CS_5       CapCs5;
  USB4_ADP_CAP_DP_OUT_CS_6       CapCs6;
  USB4_ADP_CAP_DP_OUT_CS_7       CapCs7;
} USB4_ADP_CAP_DP_OUT, *PUSB4_ADP_CAP_DP_OUT;

typedef union _USB4_ADP_CAP_DP {
  USB4_ADP_CAP_DP_IN     DpIn;
  USB4_ADP_CAP_DP_OUT    DpOut;
} USB4_ADP_CAP_DP, *PUSB4_ADP_CAP_DP;

typedef union _USB4_ADP_CAP_PCIE_CS_0 {
  struct {
    UINT32 NextCap      : BITFIELD_RANGE (0, 7);
    UINT32 CapId        : BITFIELD_RANGE (8, 15);
    UINT32 Link         : BITFIELD_BIT (16);
    UINT32 TxIdle       : BITFIELD_BIT (17);
    UINT32 RxIdle       : BITFIELD_BIT (18);
    UINT32 Reset        : BITFIELD_BIT (19);
    UINT32 Reserved1    : BITFIELD_RANGE (20, 24);
    UINT32 LtssmState   : BITFIELD_RANGE (25, 28);
    UINT32 Reserved2    : BITFIELD_BIT (29);
    UINT32 Reserved3    : BITFIELD_BIT (30);
    UINT32 PathEnable   : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_PCIE_CS_0, *PUSB4_ADP_CAP_PCIE_CS_0;

typedef union _USB4_ADP_CAP_PCIE_CS_1 {
  struct {
    UINT32 ExtendedEncapsulation       : BITFIELD_BIT (0);
    UINT32 Reserved                    : BITFIELD_RANGE (1, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CAP_PCIE_CS_1, *PUSB4_ADP_CAP_PCIE_CS_1;

typedef struct _USB4_ADP_CAP_PCIE {
  USB4_ADP_CAP_PCIE_CS_0         CapCs0;
} USB4_ADP_CAP_PCIE, *PUSB4_ADP_CAP_PCIE;

typedef struct _USB4_ADP_CAP_PCIE_V2 {
  USB4_ADP_CAP_PCIE_CS_0         CapCs0;
  USB4_ADP_CAP_PCIE_CS_1         CapCs1;
} USB4_ADP_CAP_PCIE_V2, *PUSB4_ADP_CAP_PCIE_V2;

typedef USB4_CAP_VSC_CS_0     USB4_ADP_CAP_VSC_CS_0, *PUSB4_ADP_CAP_VSC_CS_0;
typedef USB4_CAP_VSEC_CS_0    USB4_ADP_CAP_VSEC_CS_0, *PUSB4_ADP_CAP_VSEC_CS_0;
typedef USB4_CAP_VSEC_CS_1    USB4_ADP_CAP_VSEC_CS_1, *PUSB4_ADP_CAP_VSEC_CS_1;

typedef struct _USB4_ADP_CAP_VSC {
  USB4_ADP_CAP_VSC_CS_0    CapCs0;
} USB4_ADP_CAP_VSC, *PUSB4_ADP_CAP_VSC;

typedef struct _USB4_ADP_CAP_VENDOR_EX {
  USB4_ADP_CAP_VSEC_CS_0    CapCs0;
  USB4_ADP_CAP_VSEC_CS_1    CapCs1;
} USB4_ADP_CAP_VSEC, *PUSB4_ADP_CAP_VSEC;

typedef union _USB4_ADP_CAP {
  USB4_CAP_HEADER           AdpCapHeader;
  USB4_ADP_CAP_TMU          AdpTmu;
  USB4_ADP_CAP_LANE         Lane;
  USB4_ADP_CAP_USB4_PORT    Usb4Port;
  USB4_ADP_CAP_USB3         AdpUsb3;
  USB4_ADP_CAP_DP_IN        AdpDpIn;
  USB4_ADP_CAP_DP_OUT       AdpDpOut;
  USB4_ADP_CAP_PCIE         AdpPcie;
  USB4_ADP_CAP_VSC          Vsc;
  USB4_ADP_CAP_VSEC         Vsec;
} USB4_ADP_CAP, *PUSB4_ADP_CAP;

#pragma pack(pop)

#endif
