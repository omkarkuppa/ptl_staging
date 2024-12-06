/** @file
  TbtRetimer common definition.

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

@par Specification Reference:

**/

#ifndef __TBT_RETIMER_NVM_H__
#define __TBT_RETIMER_NVM_H__

#include "UsbCPdBridgeRetimer.h"

//
// Data structures used in Retimer/Discrete TBT image payload
//
#define RETIMER_PAYLOAD_HEADER_SIGNATURE      SIGNATURE_32 ('R', 'T', 'M', 'R')
#define DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE SIGNATURE_32 ('D', 'T', 'B', 'T')

#define MAX_RETIMER_FLASH_PART     4
#define MAX_RETIMER_INSTANCES      20

#define OFFLINE_MODE_ENTRY         0
#define OFFLINE_MODE_EXIT          1

#pragma pack(1)
typedef struct {
  UINT16  Bus;
  UINT16  Device;
  UINT16  Function;
  UINT8   Port;           // DMA retimer
  UINT8   CascadedRetimerIndex;
  UINT8   Reserved[8];
} RETIMER_DEV_ADDRESS;

typedef struct {
  UINT8                    FirmwareType;      // 0:Integrated Tbt retimer, 1:Discrete TBT retimer, Others: Reserved
  UINT8                    PcieRpType;        // see PCIE_RP_TYPE
  UINT8                    PcieRootPort;
  RETIMER_DEV_ADDRESS      RetimerDevAddress;
} RETIMER_CONFIG;

typedef struct {
  RETIMER_CONFIG    RetimerConfig[MAX_RETIMER_FLASH_PART];
} RETIMER_CONFIG_ARRAY;

typedef struct {
  RETIMER_DEV_ADDRESS      RetimerDevAddress;
  UINT32                   ImageOffset;
  UINT32                   ImageSize;
  UINT8                    FirmwareType;      // 0:Integrated Tbt retimer, 1:Discrete TBT retimer, Others: Reserved
  UINT8                    PcieRpType;        // see PCIE_RP_TYPE
  UINT8                    PcieRootPort;
  UINT8                    Reserve[5];
} RETIMER_ITEM;

typedef struct {
  UINT32                   ImageOffset;
  UINT32                   ImageSize;
  UINT8                    FirmwareType;      // 2:Discrete TBT, Others: Reserved
  UINT8                    PcieRpType;        // see PCIE_RP_TYPE
  UINT8                    PcieRootPort;
  UINT8                    Reserve[5];
} DISCRETE_TBT_ITEM;
#pragma pack()

typedef struct {
  UINT16 Bus;
  UINT16 Device;
  UINT16 Function;
} PCIE_BDF;

typedef struct {
  UINT8 PcieRpType;
  UINT8 PcieRootPort;
} PCIE_RP_CONFIG;

#endif  // __TBT_RETIMER_NVM_H__
