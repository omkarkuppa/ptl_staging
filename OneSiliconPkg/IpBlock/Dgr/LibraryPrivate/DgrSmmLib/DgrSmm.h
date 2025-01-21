/** @file
  Include file for DGR SMM library

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
#ifndef _DGR_SMM_LIB_H_
#define _DGR_SMM_LIB_H_

#include <PiSmm.h>

#include <Protocol/SmmAccess2.h>
#include <Protocol/SmmCpuService.h>
#include <Protocol/CpuPolicyProtocol.h>
#include <Library/SmmCpuFeaturesLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/LocalApicLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MtrrLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Register/GenerationMsr.h>
#include <Library/HobLib.h>
#include <Library/DgrInfoLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuExceptionHandlerLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Library/MsrFruLib.h>
#include <Register/Intel/SmramSaveStateMap.h>
#include <Library/CpuPlatformLib.h>
#include <Protocol/SmmCpu.h>
#include <Library/DgrSmmLib.h>
#include <IntelRcStatusCode.h>
#include "SmmPpam.h"
#include "SmmSps.h"

#define SUPERVISOR_PAGE_NUM  2

#define PROTECT_MODE_CS        0x8
#define SMMSEG_PROTECT_MODE_CS 0x10
#define SMMSEG_PROTECT_MODE_DS 0x18
#define PROTECT_MODE_DS        0x20
#define REAL_MODE_CS           0x28
#define REAL_MODE_DS           0x30
#define LONG_MODE_CS           0x38
#define LONG_MODE_DS           0x40
#define R3_CS_32               0x48
#define R3_DS_32               0x50
#define R3_CS                  0x58
#define R3_DS                  0x60
#define TSS_SEGMENT            0x70

//
// CET definition
//
#define CPUID_CET_SS           BIT7
#define CPUID_CET_IBT          BIT20

#define CR4_CET_ENABLE         BIT23

#define CPUID1_EDX_XD_SUPPORT  0x100000

#define CR0_WP                 BIT16

//
// Define for Protected Mode SMM Entry Template
//
#define SMMSEG_PROTECT_MODE_CODE_SEGMENT   0x10

///
/// Structure to represent a buffer communicate between DGR and State Save Protocol.
///
typedef struct {
  UINT32     IoMisc;
  UINT32     RaxLow;
  UINT32     RaxHigh;
  UINT32     Flags;   // Bit 0 to set on Write Save State. Bit31 - Bit1 - Reserved.
} DGR_COMM_BUFFER;

///
/// Structure used to build a lookup table for the IOMisc width information
///
typedef struct {
  UINT8                        Width;
  EFI_SMM_SAVE_STATE_IO_WIDTH  IoWidth;
} CPU_SMM_DGR_SAVE_STATE_IO_WIDTH;

#pragma pack(1)
typedef struct {
  UINT16  Limit;
  UINT32  Base;
} IA32_PROT_DESCRIPTOR;
#pragma pack()

////////
// Below section is definition for CPU SMM Feature context
////////


//
// Structure to describe CPU_SMM_FEATURE_CONTEXT
//
typedef struct {
  BOOLEAN          SmrrEnabled;
  BOOLEAN          Smrr2Enabled;
} CPU_SMM_FEATURE_CONTEXT;

//
// SMMSEG_FEATURE_ENABLES bitmap
//
#define SMMSEG_FEATURE_ENABLE                  BIT0
#define SMMSEG_FEATURE_CR4_MCE_CTL_ENABLE      BIT1

//
// Structure to describe CPU SMM Protected Mode
//
typedef struct {
  UINT32  Reserved1;
  UINT32  SmmSegFeatureEnables;   // SMM_SEG_FEATURE_ENABLES
  UINT32  GDTRLimit;
  UINT32  GDTRBaseOffset;
  UINT32  CSSelector;
  UINT32  Reserved3;
  UINT32  ESPOffset;
  UINT32  Reserved4;
  UINT32  IDTRLimit;
  UINT32  IDTRBaseOffset;
} SMMSEG;

#define  SMM_PROT_MODE_GDT_ENTRY_COUNT    27

//
// SMM PROT MODE CONTEXT (total 0x100 bytes)
//
typedef struct {
  SMMSEG                    SmmProtectedModeSMMSEG;                       // 40 bytes
  IA32_SEGMENT_DESCRIPTOR   SmmProtectedModeGdt[SMM_PROT_MODE_GDT_ENTRY_COUNT];  // 27 * 8 = 216 bytes
} SMM_PROT_MODE_CONTEXT;

///
///
///

/**
  Allocate pages for code.

  @param  Pages Number of pages to be allocated.

  @return Allocated memory.
**/
VOID *
SmmFeatureAllocateCodePagesDgr (
  IN UINTN  Pages
  );

/**
  Initialize SMM Protected Mode IDT table.

  @param[in]  Cr3      CR3 value.
**/
VOID
InitProtectedModeIdtDgr (
  IN UINT32  Cr3
  );

#endif
