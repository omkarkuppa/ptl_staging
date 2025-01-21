/** @file
  This file contains context information for IP wrapper

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

@par Specification
**/
#ifndef __IP_WRAPPER_CNTXT_INFO_H__
#define __IP_WRAPPER_CNTXT_INFO_H__

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiMultiPhase.h>
#include <IpWrapper.h>
#include <RegisterAccess.h>

typedef struct {
  //
  // For UEFI IP wrapper, refer to DebugLib.h for debug print level.
  //
  UINT32  PrintLevel;
} IP_WR_PRINT_INFO;

typedef enum {
  IpWrRegSubTypeNone = 0,
  IpWrRegAccessP2Sb,
  IpWrRegSubTypeRat
} IP_WR_REG_SUB_TYPE;

typedef UINT8 IP_WR_P2SB_PID;

/**
  P2SB structure
  Stores information required to access to registers
  like base address, S:B:D:F etc.
  and definitions specific to P2SB.
**/
typedef struct {
  /**
    P2SB controller PCI config space address
    in PCI Segment Library representation.
  **/
  UINT64                      PciCfgBaseAddr;
  /**
    P2SB controller MMIO base address
  **/
  UINT64                      Mmio;
  /**
    Is 20 bits mode supported
  **/
  BOOLEAN                     P2sb20bPcrSupported;
} IP_WR_P2SB_CONTROLLER;

typedef enum {
  IpWrP2SbMmioAccess = 0,
  IpWrP2SbMsgAccess
} IP_WR_P2SB_SIDEBAND_ACCESS_METHOD;

typedef enum {
  IpWrP2SbMemory = 0,
  IpWrP2SbPciConfig,
  IpWrP2SbPrivateConfig
} IP_WR_P2SB_REGISTER_SPACE;

/**
  REGISTER_ACCESS for P2SB device to support access to sideband registers.
  Be sure to keep first member of this structure as REGISTER_ACCESS to allow
  for correct casting between caller who sees this structure as REGISTER_ACCESS
  and caller who will cast it to P2SB_SIDEBAND_REGISTER_ACCESS.
**/
typedef struct {
  REGISTER_ACCESS                    Access;
  IP_WR_P2SB_SIDEBAND_ACCESS_METHOD  AccessMethod;
  IP_WR_P2SB_PID                     P2SbPid;
  UINT16                             Fid;
  UINT16                             Bar;
  IP_WR_P2SB_REGISTER_SPACE          RegisterSpace;
  BOOLEAN                            PostedWrites;
  IP_WR_P2SB_CONTROLLER              *P2SbCtrl;
  UINT32                             Offset;
} IP_WR_P2SB_SIDEBAND_REGISTER_ACCESS;

typedef struct {
  IP_WR_REG_ACCESS   AccessType;
  IP_WR_REG_SUB_TYPE AccessSubType;
  IP_WR_REG_ACCESS   OverrideAccessType;
  union {
    struct {
      UINT8 Fun;
      UINT8 Dev;
      UINT8 Bus;
      UINT8 Seg;
    } Pci;
    struct {
      UINT8  Socket;
      UINT16 Instance;
      UINT64 Id;
    } Rat;
    UINT8 SbId;
    UINT64 MmioBase;
    IP_WR_P2SB_SIDEBAND_REGISTER_ACCESS P2Sb;
  } RegType;
} IP_WR_REG_INFO;

typedef struct {
  EFI_MEMORY_TYPE MemoryType;
} IP_WR_MEM_INFO;

typedef enum {
  IpWrVscIpNone = 0
} IP_WR_VSC_IP;

typedef struct {
  IP_WR_VSC_IP IpType;
} IP_WR_VSC_INFO;

//
// Reg Cntxt static template
//
extern CONST IP_WR_PRINT_INFO mPrintInfoError;
extern CONST IP_WR_PRINT_INFO mPrintInfoWarning;
extern CONST IP_WR_PRINT_INFO mPrintInfoLevel1;

extern CONST IP_WR_MEM_INFO mMemInfoBsData;
extern CONST IP_WR_MEM_INFO mMemInfoRtData;
extern CONST IP_WR_MEM_INFO mMemInfoRsvdData;

#endif // __IP_WRAPPER_CNTXT_INFO_H__
