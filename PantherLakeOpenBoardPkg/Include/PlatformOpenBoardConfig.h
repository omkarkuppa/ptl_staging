/**@file
  Header file for Platform Boards Configurations.

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
#ifndef _PLATFORM_OPEN_BOARD_CONFIG_H_
#define _PLATFORM_OPEN_BOARD_CONFIG_H_

#include <ConfigBlock.h>
#include <PchPolicyCommon.h>

// Do not change the encoding. It must correspond with PCH_PCIE_CLOCK_USAGE from PCH RC.
#define NOT_USED     0xFF

#pragma pack(1)
typedef struct {
  UINT8 Section;
  UINT8 Pin;
} EXPANDER_GPIO_CONFIG;

typedef struct {
  UINT8 Type;
  UINT8 Reserved[3];  // alignment for COMMON_GPIO_CONFIG
  union {
    UINT32 Pin;
    EXPANDER_GPIO_CONFIG Expander;
  } u;
} BOARD_GPIO_CONFIG;

typedef struct {
  UINT8 ClkReqNumber : 4;
  UINT8 ClkReqSupported : 1;
  UINT8 DeviceResetPadActiveHigh : 1;
  UINT32 DeviceResetPad;
} ROOT_PORT_CLK_INFO;

typedef struct {
  UINT32 ClockUsage;
  UINT32 ClkReqSupported;
} PCIE_CLOCK_CONFIG;

typedef union {
  UINT64 Blob;
  BOARD_GPIO_CONFIG  BoardGpioConfig;
  ROOT_PORT_CLK_INFO Info;
  PCIE_CLOCK_CONFIG  PcieClock;
} PCD64_BLOB;

// Having Max CLK config to support S \ lp \ H segment
//@todo - Update this value to max number of segments
#define PCH_MAX_PCIE_CLOCKS_SBC 18

//seamless board configurations
typedef struct {
  UINT32 ClockUsage[PCH_MAX_PCIE_CLOCKS_SBC];
} PCIE_CLOCKS_USAGE;

#define SPD_DATA_SIZE 1024

//SBC MRC DQS
typedef struct {
  UINT8 DqsMapCpu2Dram[8][2];
} MRC_DQS;

//SBC MRC DQ
typedef struct {
  UINT8 DqMapCpu2Dram[8][2][8];
} MRC_DQ;

//SBC SPD DATA
typedef struct {
  BOOLEAN OverrideSpd;
  UINT8   SpdData[SPD_DATA_SIZE];
} SPD_DATA;

//SBC VPD helper structure that's used to updating Mrc DQ Byte Map
typedef struct {
  UINT8 MrcDqByteMap[2][6][2];
} VPD_MRC_DQ_BYTE_MAP;

//SBC VPD helper structure that's used to updating Rcom Target
typedef struct {
  UINT8   Size;
  UINT16  RcompTarget[5];
} VPD_RCOM_TARGET;

#define DDI_CONFIG_TABLE_SIZE 16

//SBC VPD helper structure that's used to updating Display DDi configuration
typedef struct {
  UINT8   Size;
  UINT8   DdiConfigTable[DDI_CONFIG_TABLE_SIZE];
} VPD_DISPLAY_DDI_CONFIG;

//SBC VPD helper structure that's used to updating Mrc SPD Address Table
typedef struct {
  UINT8   Size;
  UINT8   SpdAddressTable[16];
} VPD_MRC_SPD_ADDRESS_TABLE;

//SBC VPD helper structure that's used to updating Mrc CKD Address Table
typedef struct {
  UINT8   Size;
  UINT8   CkdAddressTable[16];
} VPD_MRC_CKD_ADDRESS_TABLE;

// SBC VPD helper structure that's used to updating Mrc Channel to CKD QCK Mapping
typedef struct {
  UINT8   Size;
  UINT8   ChannelToCkdQckMapping[8];
} VPD_MRC_CH_2_CKD_QCK;

// SBC VPD helper structure that's used to updating Mrc DDRIO Clock to CKD DIMM
typedef struct {
  UINT8   Size;
  UINT8   PhyClockToCkdDimm[8];
} VPD_MRC_DDRIO_2_CKD_CLOCK;


#define USB2_OC_MAX_PINS 16
#define USB3_OC_MAX_PINS 10

// Do not change the encoding. It must correspond with PCH_PCIE_CLOCK_USAGE from PCH RC.
#define FREE_RUNNING 0x80
#define LAN_CLOCK    0x70
#define PCIE_PEG     0x40
#define PCIE_PCH     0x00

#pragma pack()

#endif // _PLATFORM_OPEN_BOARD_CONFIG_H_
