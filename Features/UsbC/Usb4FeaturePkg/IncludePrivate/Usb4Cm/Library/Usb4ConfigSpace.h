/** @file
  Header file for USB4 configuration space definition.

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

#ifndef _USB4_CONFIG_SPACE_H_
#define _USB4_CONFIG_SPACE_H_

#include <Base.h>
#include <Library/Usb4ConfigBase.h>

#pragma pack(push, 1)

//
// USB4 Configuration Space Types
//
typedef enum _USB4_CONFIG_SPACE {
  USB4_CS_PATH = 0,
  USB4_CS_ADAPTER,
  USB4_CS_ROUTER,
  USB4_CS_COUNTER
} USB4_CONFIG_SPACE, *PUSB4_CONFIG_SPACE;

//
// Adapter Types : Protocol[23:16] Version[15:8] SubType[7:0]
//
// USB4_ADP_TYPE_UNSUPPORTED is still a valid adapter type
// Unused adapter may report USB4_ADP_TYPE_UNSUPPORTED adapter type
//
typedef enum _USB4_ADAPTER_TYPE {
  USB4_ADP_TYPE_UNSUPPORTED    = 0x000000,
  USB4_ADP_TYPE_LANE           = 0x000001,
  USB4_ADP_TYPE_HOST_INTERFACE = 0x000002,
  USB4_ADP_TYPE_DP_IN          = 0x0E0101,
  USB4_ADP_TYPE_DP_OUT         = 0x0E0102,
  USB4_ADP_TYPE_PCIE_DS        = 0x100101,
  USB4_ADP_TYPE_PCIE_US        = 0x100102,
  USB4_ADP_TYPE_USB3_DS        = 0x200101,
  USB4_ADP_TYPE_USB3_US        = 0x200102,
  USB4_ADP_TYPE_INVALID        = 0xFFFFFF
} USB4_ADAPTER_TYPE, *PUSB4_ADAPTER_TYPE;

//
// USB4 Configuration Space DWORD offset
//
#define USB4_CS_0          0
#define USB4_CS_1          1
#define USB4_CS_2          2
#define USB4_CS_3          3
#define USB4_CS_4          4
#define USB4_CS_5          5
#define USB4_CS_6          6
#define USB4_CS_7          7
#define USB4_CS_18         18
#define USB4_CS_25         25
#define USB4_CS_26         26

//
// Device Configuration Space
//

typedef union _USB4_RT_CS_0 {
  struct {
    UINT32 VendorId                     : BITFIELD_RANGE (0, 15);
    UINT32 DeviceId                     : BITFIELD_RANGE (16, 31);
  } Fields;
  UINT32 Value;
} USB4_RT_CS_0, *PUSB4_RT_CS_0;

typedef union _USB4_RT_CS_1 {
  struct {
    UINT32 NextCapabilityPointer        : BITFIELD_RANGE (0, 7);
    UINT32 UpstreamAdapter              : BITFIELD_RANGE (8, 13);
    UINT32 MaxAdapter                   : BITFIELD_RANGE (14, 19);
    UINT32 Depth                        : BITFIELD_RANGE (20, 22);
    UINT32 Reserved                     : BITFIELD_BIT (23);
    UINT32 RevisionNumber               : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_RT_CS_1, *PUSB4_RT_CS_1;

typedef union _USB4_RT_CS_2 {
  struct {
    UINT32 ToplogyIdLow;
  } Fields;
  UINT32 Value;
} USB4_RT_CS_2, *PUSB4_RT_CS_2;

typedef union _USB4_RT_CS_3 {
  struct {
    UINT32 ToplogyIdHigh                : BITFIELD_RANGE (0, 23);
    UINT32 Reserved                     : BITFIELD_RANGE (24, 30);
    UINT32 ToplogyIdValid               : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_RT_CS_3, *PUSB4_RT_CS_3;

//
// USB4 Version definition in Router configuration space
// Major Version bit [7:5]
// Minor Version bit [4:0]
//
#define RT_USB4_VERSION_1    0x20
#define RT_USB4_VERSION_2    0x40

typedef union _USB4_RT_CS_4 {
  struct {
    UINT32 NotificationTimeout       : BITFIELD_RANGE (0, 7);
    UINT32 CmUsb4VerMinor            : BITFIELD_RANGE (8, 11);
    UINT32 CmUsb4VerMajor            : BITFIELD_RANGE (12, 15);
    UINT32 Reserved                  : BITFIELD_RANGE (16, 23);
    UINT32 Usb4Version               : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_RT_CS_4, *PUSB4_RT_CS_4;

//
// Router_CS_5 enable bits
//
#define RT_CS5_SLP           0x00000001    // bit 0  Enter Sleep
#define RT_CS5_WOP           0x00000002    // bit 1  Enable Wake on PCIe
#define RT_CS5_WOU           0x00000004    // bit 2  Enable Wake on USB
#define RT_CS5_C3S           0x00800000    // bit 23 CM TBT3 Support
#define RT_CS5_PTO           0x01000000    // bit 24 PCIe Tunneling On
#define RT_CS5_UTO           0x02000000    // bit 25 USB3 Tunneling On
#define RT_CS5_HCO           0x04000000    // bit 26 Internal Host Controller On
#define RT_CS5_CV            0x80000000    // bit 31 Configuration Valid

#define RT_CS5_BIT_MASK      0x87800007    // Valid bits in RT CS5

typedef union _USB4_RT_CS_5 {
  struct {
    UINT32 EnterSleep         : BITFIELD_BIT (0);
    UINT32 WakeOnPcieEnable   : BITFIELD_BIT (1);
    UINT32 WakeOnUsbEnable    : BITFIELD_BIT (2);
    UINT32 Reserved1          : BITFIELD_RANGE (3, 22);
    UINT32 CmTbt3Support      : BITFIELD_BIT (23);
    UINT32 PcieTunOn          : BITFIELD_BIT (24);
    UINT32 UsbTunOn           : BITFIELD_BIT (25);
    UINT32 UsbHcOn            : BITFIELD_BIT (26);
    UINT32 Reserved2          : BITFIELD_RANGE (27, 30);
    UINT32 ConfigValid        : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_RT_CS_5, *PUSB4_RT_CS_5;

//
// Router_CS_6 status bits
//
#define RT_CS6_SLPR          0x00000001    // bit 0  Sleep Ready
#define RT_CS6_WOP           0x00000004    // bit 2  Wake on PCIe
#define RT_CS6_WOU           0x00000008    // bit 3  Wake on USB
#define RT_CS6_RR            0x01000000    // bit 24 Router Ready
#define RT_CS6_CR            0x02000000    // bit 25 Configuration Ready

typedef union _USB4_RT_CS_6 {
  struct {
    UINT32 SleepReady                 : BITFIELD_BIT (0);
    UINT32 Tbt3NotSupported           : BITFIELD_BIT (1);
    UINT32 WakeOnPcieStatus           : BITFIELD_BIT (2);
    UINT32 WakeOnUsbStatus            : BITFIELD_BIT (3);
    UINT32 Reserved1                  : BITFIELD_RANGE (4, 17);
    UINT32 UsbHcImplemented           : BITFIELD_BIT (18);
    UINT32 Reserved2                  : BITFIELD_RANGE (19, 23);
    UINT32 RouterReady                : BITFIELD_BIT (24);
    UINT32 ConfigReady                : BITFIELD_BIT (25);
    UINT32 Reserved3                  : BITFIELD_RANGE (26, 31);
  } Fields;
  UINT32 Value;
} USB4_RT_CS_6, *PUSB4_RT_CS_6;

typedef union _USB4_RT_CS_7 {
  struct {
    UINT32 UuidHigh;
  } Fields;
  UINT32 Value;
} USB4_RT_CS_7, *PUSB4_RT_CS_7;

typedef union _USB4_RT_CS_8 {
  struct {
    UINT32 UuidLow;
  } Fields;
  UINT32 Value;
} USB4_RT_CS_8, *PUSB4_RT_CS_8;

typedef struct _USB4_RT_CS_BASIC_INFO {
  USB4_RT_CS_0 RouterCs0;
  USB4_RT_CS_1 RouterCs1;
  USB4_RT_CS_2 RouterCs2;
  USB4_RT_CS_3 RouterCs3;
  USB4_RT_CS_4 RouterCs4;
} USB4_RT_CS_BASIC_INFO, *PUSB4_RT_CS_BASIC_INFO;

typedef struct _USB4_RT_CS_INFO {
  USB4_RT_CS_0 RouterCs0;
  USB4_RT_CS_1 RouterCs1;
  USB4_RT_CS_2 RouterCs2;
  USB4_RT_CS_3 RouterCs3;
  USB4_RT_CS_4 RouterCs4;
  USB4_RT_CS_5 RouterCs5;
  USB4_RT_CS_6 RouterCs6;
  USB4_RT_CS_7 RouterCs7;
  USB4_RT_CS_8 RouterCs8;
} USB4_RT_CS_INFO, *PUSB4_RT_CS_INFO;

//
// Router Operations
//
#define RT_OP_QUERY_DP_RESOURCES          0x10
#define RT_OP_ALLOC_DP_RESOURCES          0x11
#define RT_OP_DEALLOC_DP_RESOURCES        0x12
#define RT_OP_NVM_WRITE                   0x20
#define RT_OP_NVM_AUTHENTICATE_WRITE      0x21
#define RT_OP_NVM_READ                    0x22
#define RT_OP_NVM_SET_OFFSET              0x23
#define RT_OP_DROM_READ                   0x24
#define RT_OP_GET_NVM_SECTOR_SIZE         0x25
#define RT_OP_GET_PCIE_DS_ENTRY_MAPPING   0x30
#define RT_OP_GET_CAPABILITIES            0x31
#define RT_OP_SET_CAPABILITIES            0x32
#define RT_OP_BUFFER_ALLOC_REQUEST        0x33
#define RT_OP_BLOCK_SB_PORT_OP            0x34
#define RT_OP_UNBLOCK_SB_PORT_OP          0x35
#define RT_OP_GET_CONTAINER_ID            0x36

//
// Router Operation Status
//
#define RT_OP_STATUS_SUCCESS               0
#define RT_OP_STATUS_FAILURE               1
#define RT_OP_STATUS_ERROR(OpStatus)       (OpStatus != RT_OP_STATUS_SUCCESS)

typedef union _USB4_RT_CS_26 {
  struct {
    UINT32 OpCode           : BITFIELD_RANGE (0, 15);
    UINT32 Reserved         : BITFIELD_RANGE (16, 23);
    UINT32 Status           : BITFIELD_RANGE (24, 29);
    UINT32 OpNotSupported   : BITFIELD_BIT (30);
    UINT32 OpValid          : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_RT_CS_26;

typedef struct _USB4_RT_OP {
  UINT32             MetaData;
  USB4_RT_CS_26      Op;
} USB4_RT_OP, *PUSB4_RT_OP;

//
// Adapter Configuration Space Definitions
//
typedef union _USB4_ADP_CS_0 {
  struct {
    UINT32 Reserved;
  } Fields;
  UINT32 Value;
} USB4_ADP_CS_0, *PUSB4_ADP_CS_0;

typedef union _USB4_ADP_CS_1 {
  struct {
    UINT32 NextCapability     : BITFIELD_RANGE (0, 7);
    UINT32 MaxCouterSet       : BITFIELD_RANGE (8, 18);
    UINT32 CcsFlag            : BITFIELD_BIT (19);
    UINT32 Reserved           : BITFIELD_RANGE (20, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CS_1, *PUSB4_ADP_CS_1;

typedef union _USB4_ADP_CS_2 {
  struct {
    UINT32 AdapterType     : BITFIELD_RANGE (0, 23);
    UINT32 Reserved        : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CS_2, *PUSB4_ADP_CS_2;

typedef union _USB4_ADP_CS_3 {
  struct {
    UINT32 Reserved1              : BITFIELD_RANGE (0, 19);
    UINT32 AdapterNumber          : BITFIELD_RANGE (20, 25);
    UINT32 Reserved2              : BITFIELD_RANGE (26, 28);
    UINT32 HecError               : BITFIELD_BIT (29);
    UINT32 FlowCtrlErr            : BITFIELD_BIT (30);
    UINT32 SharedBufferCapable    : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CS_3, *PUSB4_ADP_CS_3;

typedef union _USB4_ADP_CS_4 {
  struct
  {
    UINT32 NfcBuffers             : BITFIELD_RANGE (0, 9);
    UINT32 Reserved1              : BITFIELD_RANGE (10, 19);
    UINT32 TotalBuffers           : BITFIELD_RANGE (20, 29);
    UINT32 Plugged                : BITFIELD_BIT (30);
    UINT32 LocK                   : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CS_4, *PUSB4_ADP_CS_4;

typedef union _USB4_ADP_CS_5 {
  struct {
    UINT32 MaxInputHopId         : BITFIELD_RANGE (0, 10);
    UINT32 MaxOutputHopId        : BITFIELD_RANGE (11, 21);
    UINT32 LinkCreditsAllocated  : BITFIELD_RANGE (22, 28);
    UINT32 HecErrorEn            : BITFIELD_BIT (29);
    UINT32 FlowCtrlErrEn         : BITFIELD_BIT (30);
    UINT32 DisableHotplugEvents  : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_ADP_CS_5, *PUSB4_ADP_CS_5;

typedef struct _USB4_ADP_CS_INFO {
  USB4_ADP_CS_0     AdpCs0;
  USB4_ADP_CS_1     AdpCs1;
  USB4_ADP_CS_2     AdpCs2;
  USB4_ADP_CS_3     AdpCs3;
  USB4_ADP_CS_4     AdpCs4;
  USB4_ADP_CS_5     AdpCs5;
} USB4_ADP_CS_INFO, *PUSB4_ADP_CS_INFO;

//
// Path Configuration Space Definitions
//
typedef union _USB4_PATH_0_CS_0 {
  struct {
    UINT32 Reserved1         : BITFIELD_RANGE (0, 16);
    UINT32 CreditsAllocated  : BITFIELD_RANGE (17, 23);
    UINT32 Reserved2         : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_PATH_0_CS_0, *PUSB4_PATH_0_CS_0;

typedef union _USB4_PATH_0_CS_1 {
  struct {
    UINT32 Reserved1       : BITFIELD_RANGE (0, 11);
    UINT32 CounterId       : BITFIELD_RANGE (12, 22);
    UINT32 CounterEnable   : BITFIELD_BIT (23);
    UINT32 Reserved2       : BITFIELD_RANGE (24, 31);
  } Fields;
  UINT32 Value;
} USB4_PATH_0_CS_1, *PUSB4_PATH_0_CS_1;

typedef struct _USB4_PATH_0_CS_ENTRY {
  USB4_PATH_0_CS_0     PathCs0;
  USB4_PATH_0_CS_1     PathCs1;
} USB4_PATH_0_CS_ENTRY, *PUSB4_PATH_0_CS_ENTRY;

typedef union _USB4_PATH_CS_0 {
  struct {
    UINT32 OutputHopId      : BITFIELD_RANGE (0, 10);
    UINT32 OutputAdapter    : BITFIELD_RANGE (11, 16);
    UINT32 CreditsAllocated : BITFIELD_RANGE (17, 23);
    UINT32 Reserved         : BITFIELD_RANGE (24, 30);
    UINT32 Valid            : BITFIELD_BIT (31);
  } Fields;
  UINT32 Value;
} USB4_PATH_CS_0, *PUSB4_PATH_CS_0;

typedef union _USB4_PATH_CS_1 {
  struct {
    UINT32 Weight           : BITFIELD_RANGE (0, 7);
    UINT32 Priority         : BITFIELD_RANGE (8, 10);
    UINT32 Reserved1        : BITFIELD_BIT (11);
    UINT32 CounterId        : BITFIELD_RANGE (12, 22);
    UINT32 CounterEn        : BITFIELD_BIT (23);
    UINT32 Ifc              : BITFIELD_BIT (24);
    UINT32 Efc              : BITFIELD_BIT (25);
    UINT32 Ise              : BITFIELD_BIT (26);
    UINT32 Ese              : BITFIELD_BIT (27);
    UINT32 PendingPkts      : BITFIELD_BIT (28);
    UINT32 BlockLow         : BITFIELD_BIT (29);
    UINT32 Reserved2        : BITFIELD_RANGE (30, 31);
  } Fields;
  UINT32 Value;
} USB4_PATH_CS_1, *PUSB4_PATH_CS_1;

typedef struct _USB4_PATH_CS_ENTRY {
  USB4_PATH_CS_0  PathCs0;
  USB4_PATH_CS_1  PathCs1;
} USB4_PATH_CS_ENTRY, *PUSB4_PATH_CS_ENTRY;

#define PATH_ENTRY_DW_SIZE     (sizeof (USB4_PATH_CS_ENTRY) / sizeof (UINT32))

#pragma pack(pop)
#endif
