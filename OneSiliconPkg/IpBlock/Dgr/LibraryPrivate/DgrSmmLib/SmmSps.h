/** @file
  The DGR specific Header file for SMM SPS.

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

#ifndef _SMM_SPS_H_
#define _SMM_SPS_H_

#include "PpamApi.h"
#include "SpsApi.h"
#include <Txt.h>
#include <TxtInfoHob.h>

#define TSS_SIZE              104
#define FULL_TSS_SIZE         (104 + 256/8 + 0x10000/8 + 1)
#define TSS_IOMAP_OFFSET      102
#define TSS_X64_RSP0_OFFSET   4
#define TSS_X64_IST1_OFFSET   36
#define IA32_DPL(x)           ((x) << 5)

#ifndef IA32_PG_PS
#define IA32_PG_PS            BIT7
#endif

#define IA32_PG_U             BIT2
#define IA32_PG_PAT_4K        IA32_PG_PS
#define IA32_PG_PMNT          BIT62
#define IA32_PG_NX            BIT63

#ifndef IA32_PG_P
#define IA32_PG_P             BIT0
#endif

#ifndef IA32_PG_RW
#define IA32_PG_RW            BIT1
#endif

#ifndef IA32_PG_A
#define IA32_PG_A             BIT5
#endif

#ifndef IA32_PG_D
#define IA32_PG_D             BIT6
#endif

#define PAGE_ATTRIBUTE_BITS       (IA32_PG_D | IA32_PG_A | IA32_PG_U | IA32_PG_RW | IA32_PG_P)
#define PAGING_PAE_INDEX_MASK     0x1FF
#define PAGING_4K_ADDRESS_MASK_64 0x000FFFFFFFFFF000ull
#define PAGING_2M_ADDRESS_MASK_64 0x000FFFFFFFE00000ull
#define PAGING_1G_ADDRESS_MASK_64 0x000FFFFFC0000000ull

#define PAGING_4K_MASK            0xFFF
#define PAGING_2M_MASK            0x1FFFFF
#define PAGING_1G_MASK            0x3FFFFFFF
#define PAGE_PROGATE_BITS         (IA32_PG_NX | PAGE_ATTRIBUTE_BITS)

#ifndef EFI_PAGE_SIZE
#define EFI_PAGE_SIZE  0x00001000      // 4KB page size
#endif

#define MAX_PAGE_ENTRIES          512

typedef enum {
  PageNone,
  Page4K,
  Page2M,
  Page1G,
} PAGE_ATTRIBUTE;

typedef struct {
  PAGE_ATTRIBUTE   Attribute;
  UINT64           Length;
  UINT64           AddressMask;
} PAGE_ATTRIBUTE_TABLE;

typedef enum {
  PageAccessRightSupervisor,
  PageAccessRightUser,
  PageAccessRightIgnore,
} PAGE_ACCESS_RIGHT;

extern IA32_DESCRIPTOR                     mSmmDgrSmiGdtr;

VOID
SpsConstructor (
  VOID
  );

VOID
SpsPatchIdt (
  IN VOID *IdtBase,
  IN UINTN IdtSize
  );

/**
  Return the size, in bytes, of a custom SMI Handler in bytes.  If 0 is
  returned, then a custom SMI handler is not provided by this library,
  and the default SMI handler must be used.

  @retval 0    Use the default SMI handler.
  @retval > 0  Use the SMI handler installed by SmmCpuFeaturesInstallSmiHandler()
               The caller is required to allocate enough SMRAM for each CPU to
               support the size of the custom SMI handler.
**/
UINTN
EFIAPI
SmmCpuFeaturesGetSmiHandlerSizeSps (
  VOID
  );

/**
  Install a custom SMI handler for the CPU specified by CpuIndex.  This function
  is only called if SmmCpuFeaturesGetSmiHandlerSize() returns a size is greater
  than zero and is called by the CPU that was elected as monarch during System
  Management Mode initialization.

  @param[in] CpuIndex   The index of the CPU to install the custom SMI handler.
                        The value must be between 0 and the NumberOfCpus field
                        in the System Management System Table (SMST).
  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
  @param[in] SmiStack   The stack to use when an SMI is processed by the
                        the CPU specified by CpuIndex.
  @param[in] StackSize  The size, in bytes, if the stack used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtBase    The base address of the GDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtSize    The size, in bytes, of the GDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtBase    The base address of the IDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtSize    The size, in bytes, of the IDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] Cr3        The base address of the page tables to use when an SMI
                        is processed by the CPU specified by CpuIndex.
**/
VOID
EFIAPI
SmmCpuFeaturesInstallSmiHandlerSps (
  IN UINTN   CpuIndex,
  IN UINT32  SmBase,
  IN VOID    *SmiStack,
  IN UINTN   StackSize,
  IN UINTN   GdtBase,
  IN UINTN   GdtSize,
  IN UINTN   IdtBase,
  IN UINTN   IdtSize,
  IN UINT32  Cr3
  );

/**
  This function sets
   - Page access rights within SMRAM based on ring separation policy requirements.
   - MMIO access rights.
   - System Memory access rights which are being accessed from SMM.

  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
**/
EFI_STATUS
SetSmmDgrPolicy (
  IN UINT32   *SmBase
  );

/**
  Setup the page access rights within SMRAM based on ring separation policy requirements

  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
**/
VOID
SetPageTableAccessRight (
  IN UINT32   *SmBase
  );

/**
  Restrict write access for specified VTD MMIO resource within SMM.
**/
VOID
RestrictSmmVtdMmioAccess (
  VOID
  );

/**
  Restrict write access for all TXT MMIO (TXT Private & Public Space).
  Also restrict Write access to its Page Table Entry and keep in supervisor access right.

  TXT Private Space 0xFED20000 - 0xFED2FFFF (64K = 16 * 4K = 16 Pages).
  TXT Public Space 0xFED30000 - 0xFED3FFFF (16 Pages).

**/
VOID
RestrictSmmTxtMmioAccess (
  VOID
  );

/**
  Restrict System Memory Access for Write inside SMM.
**/
EFI_STATUS
RestrictSmmSystemMemoryAccess (
  VOID
  );

VOID
EFIAPI
AsmOemExceptionHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  );

#endif
