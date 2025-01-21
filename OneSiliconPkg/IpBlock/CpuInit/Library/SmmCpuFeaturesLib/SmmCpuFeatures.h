/** @file
  The CPU specific programming for PiSmmCpuDxeSmm module.

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

#ifndef __SMM_FEATURES_H__
#define __SMM_FEATURES_H__

#include <PiSmm.h>

#include <Guid/SmmBaseHob.h>
#include <Protocol/SmmAccess2.h>
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
#include <Library/CpuPlatformLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuExceptionHandlerLib.h>
#include <Library/PeCoffGetEntryPointLib.h>
#include <Library/MsrFruLib.h>
#include <Library/DgrSmmLib.h>

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

//
// SMM CPU synchronization features available on a processor
//
typedef struct {
  BOOLEAN          TargetedSmiSupported;
  BOOLEAN          DelayIndicationSupported;
  BOOLEAN          BlockIndicationSupported;
} SMM_CPU_SYNC_FEATURE;

///
///
///
extern UINT8      mSmmFeatureSaveStateRegisterLma;
extern BOOLEAN    mSmmProcTraceEnable;
extern UINT64     gSmmSupovrStateLockData;

/**
  Read MSR or CSR based on the CPU type Register to read.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.

  @return 64-bit value read from register.
**/
UINT64
SmmReadReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName
  );

/**
  Write MSR or CSR based on the CPU type Register to write.

  NOTE: Since platform may uses I/O ports 0xCF8 and 0xCFC to access
        CSR, we need to use SPIN_LOCK to avoid collision on MP System.

  @param[in]  CpuIndex  The processor index.
  @param[in]  RegName   Register name.
  @param[in]  RegValue  64-bit Register value.
**/
VOID
SmmWriteReg64 (
  IN  UINTN           CpuIndex,
  IN  SMM_REG_NAME    RegName,
  IN  UINT64          RegValue
  );

/**
  Initialize SMM Protected Mode IDT table.

  @param[in]  Cr3      CR3 value.
**/
VOID
InitProtectedModeIdt (
  IN UINT32  Cr3
  );

/**
  Allocate pages for code.

  @param[in]  Pages Number of pages to be allocated.

  @return Allocated memory.
**/
VOID *
SmmFeatureAllocateCodePages (
  IN UINTN           Pages
  );

/**
  This function sets SmmSupovrStateLock MSR.

  NOTE: This function does not take effect before
  SmmCpuFeaturesCompleteSmmReadyToLock().
**/
VOID
SmmSupovrStateLock (
  VOID
  );

/**
  This function initializes CPU Shadow Stack.

  @param[in]  CpuIndex    The processor index.
  @param[in]  ShadowStack A pointer to Shadow Stack.
**/
VOID
SmmCpuFeaturesInitShadowStack (
  IN UINTN   CpuIndex,
  IN VOID    *ShadowStack
  );

/**
  This function fixes up the address of the global variable or function
  referred in SmiEntry assembly files to be the absoute address.
**/
VOID
EFIAPI
SmmCpuFeaturesLibSmiEntryFixupAddress (
  VOID
  );

#endif
