/** @file
  The Header File for PMON (Performance Monitoring) library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _PMON_LIB_H_
#define _PMON_LIB_H_

#include <Ppi/SiPolicy.h>

#define PMON_MEMORY_SIZE      0x1000  // PMON reserved memory size
#define INVALID_ADDRESS_64    0xFFFFFFFFFFFFFFFF
extern EFI_GUID gPmonDataHobGuid;

//
// Define the BAR and PORT ID for PMON
// @todo: after OSXML update, get the definition from MC IP, remove this
//
#define MCHBAR_MC0              0xD800
#define MCHBAR_MC1              0x1D800
#define SEGMENT_ID_C_DIE        0xEE
#define PMON_UNIT_MCH_ADDR      0x1C8
#define DEV0_BAR_EN             0x0001

#define SAFBAR_MEMORY_ADDRESS(Segment,PortId,Offset) \
  ( ((Offset) & 0xFFF)                 | \
    (((PortId) & 0xFF) << 12)          | \
    (((Segment) & 0xFF) << 20)           \
  )

#pragma pack (push,1)
//
// PMON Data Hob
//
typedef struct {
  BOOLEAN      PmonEnable;
  UINT64       PmonAddress;
} PMON_HOB_DATA;

//
// Global discovery table structure
//
typedef struct {
  UINT64  Type:8;                 // Unit type - future feature, LNL - zero
  UINT64  BlockStride:8;          // Memory space reserved for each discovery block relevant to 8B
  UINT64  Maxblocks:10;           // The number of strides
  UINT64  Reserved1:35;
  UINT64  AccessType:3;           // 00 - MSR space, 01 - MMIO space
  UINT64  GlobalControlAddress;   // Address to the Global Control Register
  UINT64  GlbCtrStatAddress:8;    // 8b offset from the Global Control Addr to the first counter status register
  UINT64  NumBlockStatusBitsCtrl:16;      // Numbers of status bits are allocated to track overflows
  UINT64  Reserved2:40;
  UINT64  Reserved3;
} PMON_GLOBAL_DISCOVERY;

//
// Unit discovery table structure
//
typedef struct {
  UINT64  ControlRegs:8;           // Number of counter control registers
  UINT64  CtrCtrl0Addr:8;          // 8b offset from Unit Control Address to 1st counter control register
  UINT64  CtrWidth:8;              // Number of bits in the data register
  UINT64  Count0Addr:8;            // 8b offset from Unit Control Address to 1st counter
  UINT64  UnitStatusAddr:8;        // 8b offset from Unit Control Addr to 1st unit status register
  UINT64  Reserved1:21;
  UINT64  AccessType:3;            // 00 - MSR space, 01 - MMIO space, 10 - PCICFG space
  UINT64  UnitControlAddress;      // 64b address of the control register for this Unit
  UINT64  UnitType:16;             // unit type field is not used in LNL-therefore all zero
  UINT64  UnitId:16;
  UINT64  GlobalStatusPosition:16; // 16b field to tell SW which bit in the Global Status belongs to the PMON block
  UINT64  Reserved2:16;
  UINT64  PmonBlockId:16;
  UINT64  Reserved3:48;
} PMON_UNIT_DISCOVERY;

typedef enum {
  GLOBAL_DISCOVERY_TABLE,
  UNIT_DISCOVERY_TABLE
} PMON_DISCOVERY_TABLE_TYPE;

#pragma pack (pop)

/**
  Program the PMON allocated memory so that PMON Software can consume it.

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
PmonMemoryAddressProgram (
  IN SI_POLICY_PPI       *SiPolicyPpi
  );

/**
  Populate PMON discovery table to the memory.

  @retval EFI_SUCCESS    The function completes successfully
**/
EFI_STATUS
PmonPopulateDiscoveryTable (
  );

#endif  // _PMON_LIB_H_

