/** @file
  SMM CPU misc functions for x64 arch specific.

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

#include "SmmCpuFeatures.h"

//
// Code select value
//
// Need to figure out how to get these values from
// GDT produces by SMM module
//
#define PROTECT_MODE_CODE_SEGMENT          0x08
#define SMMSEG_PROTECT_MODE_CODE_SEGMENT   0x10
#define LONG_MODE_CODE_SEGMENT             0x38

#define EXCEPTION_VECTOR_NUMBER     0x20

//
// IA32_IDT_GATE_DESCRIPTOR is aliased here for X64 build benefit
//
typedef union {
  struct {
    UINT32  OffsetLow:16;   ///< Offset bits 15..0.
    UINT32  Selector:16;    ///< Selector.
    UINT32  Reserved_0:8;   ///< Reserved.
    UINT32  GateType:8;     ///< Gate Type.  See #defines above.
    UINT32  OffsetHigh:16;  ///< Offset bits 31..16.
  } Bits;
  UINT64  Uint64;
} SMM_PM_IA32_IDT_GATE_DESCRIPTOR;

UINTN                         mMceHandlerLongMode;
IA32_DESCRIPTOR               mSmiPMIdtr;
UINT32                        gProtModeIdtr = 0;
extern UINTN                  SmiPMExceptionEntryPoints;
extern UINT32                 gSmiExceptionCr3;
extern BOOLEAN                             gSmmFeatureCetSupported;
extern UINT32                              gSmmFeatureCetPl0Ssp;
extern UINT32                              gSmmFeatureCetInterruptSsp;
extern UINT32                              gSmmFeatureCetInterruptSspTable;

UINTN  mSmmFeatureInterruptSspTables = 0;

/**
  Protected Mode IDT handler of machine check.
**/
VOID
EFIAPI
PMIdtHandlerMachineCheck (
  VOID
  );

/**
  Protected Mode IDT handler.
**/
VOID
EFIAPI
PMIdtHandler (
  VOID
  );

/**
  Initialize SMM Protected Mode IDT table.

  @param[in]  Cr3      CR3 value.
**/
VOID
InitProtectedModeIdt (
  IN UINT32  Cr3
  )
{
  UINTN                             Index;
  UINTN                             PmIdtSize;
  SMM_PM_IA32_IDT_GATE_DESCRIPTOR   *PmIdtEntry;
  UINTN                             InterruptHandler;
  IA32_IDT_GATE_DESCRIPTOR          *IdtEntry;

  if (gProtModeIdtr != 0) {
    return;
  }

  //
  // Patch CR3 for SMM Protected Mode
  //
  gSmiExceptionCr3 = (UINT32)(UINTN)Cr3;

  //
  // Allocate IDT table size
  //
  PmIdtSize = sizeof (SMM_PM_IA32_IDT_GATE_DESCRIPTOR) * (EXCEPT_IA32_MACHINE_CHECK + 1) * 2;
  mSmiPMIdtr.Base  = (UINTN) SmmFeatureAllocateCodePages (EFI_SIZE_TO_PAGES(PmIdtSize));
  ASSERT (mSmiPMIdtr.Base != 0);
  if (mSmiPMIdtr.Base == 0) {
    return;
  }

  ZeroMem ((VOID *)mSmiPMIdtr.Base, PmIdtSize);
  mSmiPMIdtr.Limit = (UINT16) PmIdtSize - 1;
  PmIdtEntry =  (SMM_PM_IA32_IDT_GATE_DESCRIPTOR *)(mSmiPMIdtr.Base);

  gProtModeIdtr = (UINT32)(UINTN)&mSmiPMIdtr;
  //
  // Set up IA32 IDT handler
  //
  for (Index = 0; Index < EXCEPTION_VECTOR_NUMBER; Index++) {
    if (Index == EXCEPT_IA32_MACHINE_CHECK) {
      InterruptHandler = (UINTN)PMIdtHandlerMachineCheck;
    } else {
      InterruptHandler = (UINTN)PMIdtHandler;
    }
    PmIdtEntry[Index].Bits.Selector   = PROTECT_MODE_CODE_SEGMENT;
    PmIdtEntry[Index].Bits.GateType   = IA32_IDT_GATE_TYPE_INTERRUPT_32;
    PmIdtEntry[Index].Bits.OffsetLow  = (UINT16) (0x0000FFFF & InterruptHandler);
    PmIdtEntry[Index].Bits.OffsetHigh = (UINT16) (0x0000FFFF & RShiftU64 (InterruptHandler, 16));
  }
  //
  // Set X64 MCA IDT handler at location 0x24&0x25 in IA32 IDT Table. In case, MCA issues
  // before X64 IDT table is loaded in long mode, this MCA IDT handler will be invoked.
  //
  IdtEntry  = (IA32_IDT_GATE_DESCRIPTOR *) mSmiPMIdtr.Base;
  IdtEntry += EXCEPT_IA32_MACHINE_CHECK;
  mMceHandlerLongMode = IdtEntry->Bits.OffsetLow + (((UINTN) IdtEntry->Bits.OffsetHigh)  << 16) +
                                    (((UINTN) IdtEntry->Bits.OffsetUpper) << 32);
  IdtEntry  = (IA32_IDT_GATE_DESCRIPTOR *) mSmiPMIdtr.Base;
  IdtEntry += EXCEPT_IA32_MACHINE_CHECK;
  IdtEntry->Bits.Selector       = LONG_MODE_CODE_SEGMENT;
  IdtEntry->Bits.OffsetLow      = (UINT16)mMceHandlerLongMode;
  IdtEntry->Bits.Reserved_0     = 0;
  IdtEntry->Bits.GateType       = IA32_IDT_GATE_TYPE_INTERRUPT_32;
  IdtEntry->Bits.OffsetHigh     = (UINT16) RShiftU64 (mMceHandlerLongMode, 16);
  IdtEntry->Bits.OffsetUpper    = (UINT32) RShiftU64 (mMceHandlerLongMode, 32);
  IdtEntry->Bits.Reserved_1     = 0;
}

/**
  This function initializes CPU Shadow Stack.

  @param[in]  CpuIndex    The processor index.
  @param[in]  ShadowStack A pointer to Shadow Stack.
**/
VOID
SmmCpuFeaturesInitShadowStack (
  IN UINTN   CpuIndex,
  IN VOID    *ShadowStack
  )
{
  UINTN       SmmShadowStackSize;
  UINT64      *InterruptSspTable;
  UINT32      InterruptSsp;

  if (PcdGet32 (PcdControlFlowEnforcementPropertyMask) && gSmmFeatureCetSupported) {
    SmmShadowStackSize = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (PcdGet32 (PcdCpuSmmShadowStackSize)));
    //
    // Add 1 page as known good shadow stack
    //
    SmmShadowStackSize += EFI_PAGES_TO_SIZE (1);

    if (FeaturePcdGet (PcdCpuSmmStackGuard)) {
      DEBUG ((DEBUG_INFO, "SMM Stack Guard Enabled\n"));
      //
      // Add one guard page between Known Good Shadow Stack and SMM Shadow Stack.
      //
      SmmShadowStackSize += EFI_PAGES_TO_SIZE (1);
    }

    gSmmFeatureCetPl0Ssp = (UINT32)((UINTN)ShadowStack + SmmShadowStackSize - sizeof(UINT64));
    DEBUG ((DEBUG_INFO, "gSmmFeatureCetPl0Ssp - 0x%x\n", gSmmFeatureCetPl0Ssp));
    DEBUG ((DEBUG_INFO, "ShadowStack - 0x%x\n", ShadowStack));
    DEBUG ((DEBUG_INFO, "  SmmShadowStackSize - 0x%x\n", SmmShadowStackSize));

    if (mSmmFeatureInterruptSspTables == 0) {
      mSmmFeatureInterruptSspTables = (UINTN)AllocateZeroPool(sizeof(UINT64) * 8 * PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
      ASSERT (mSmmFeatureInterruptSspTables != 0);
      DEBUG ((DEBUG_INFO, "mSmmFeatureInterruptSspTables - 0x%x\n", mSmmFeatureInterruptSspTables));
    }

    //
    // The highest address on the stack (0xFE0) is a save-previous-ssp token pointing to a location that is 40 bytes away - 0xFB8.
    // The supervisor shadow stack token is just above it at address 0xFD8. This is where the interrupt SSP table points.
    // So when an interrupt of exception occurs, we can use SAVESSP/RESTORESSP/CLEARSSBUSY for the supervisor shadow stack,
    // due to the reason the RETF in SMM exception handler cannot clear the BUSY flag with same CPL.
    // (only IRET or RETF with different CPL can clear BUSY flag)
    // Please refer to UefiCpuPkg/Library/CpuExceptionHandlerLib/X64 for the full stack frame at runtime.
    // According to SDM (ver. 075 June 2021), shadow stack should be 32 bytes aligned.
    //
    InterruptSsp = (UINT32)(((UINTN)ShadowStack + EFI_PAGES_TO_SIZE(1) - (sizeof(UINT64) * 4)) & ~0x1f);
    *(UINT64 *)(UINTN)InterruptSsp = (InterruptSsp - sizeof(UINT64) * 4) | 0x2;
    gSmmFeatureCetInterruptSsp = InterruptSsp - sizeof(UINT64);

    gSmmFeatureCetInterruptSspTable = (UINT32)(UINTN)(mSmmFeatureInterruptSspTables + sizeof(UINT64) * 8 * CpuIndex);

    InterruptSspTable = (UINT64 *)(UINTN)(gSmmFeatureCetInterruptSspTable);
    InterruptSspTable[1] = gSmmFeatureCetInterruptSsp;
    DEBUG ((DEBUG_INFO, "gSmmFeatureCetInterruptSsp - 0x%x\n", gSmmFeatureCetInterruptSsp));
    DEBUG ((DEBUG_INFO, "gSmmFeatureCetInterruptSspTable - 0x%x\n", gSmmFeatureCetInterruptSspTable));
  }
}

