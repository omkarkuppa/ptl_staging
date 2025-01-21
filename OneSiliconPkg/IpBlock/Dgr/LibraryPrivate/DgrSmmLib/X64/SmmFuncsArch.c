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

#include "DgrSmm.h"

//
// Code select value
//
// BUGBUG: Need to figure out how to get these values from
// GDT produces by SMM module
//
#define PROTECT_MODE_CODE_SEGMENT          0x08
#define SMMSEG_PROTECT_MODE_CODE_SEGMENT   0x10
#define LONG_MODE_CODE_SEGMENT             0x38

#define EXCEPTION_VECTOR_NUMBER            0x20

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
} SMM_PM_IA32_IDT_GATE_DESCRIPTOR_DGR;

UINTN                         mSmmDgrMceHandlerLongMode;
IA32_DESCRIPTOR               mSmmDgrSmiPMIdtr;
UINT32                        gSmmDgrProtModeIdtr = 0;
extern UINT32                 gSmmDgrSmiExceptionCr3;

/**
  Protected Mode IDT handler of machine check.
**/
VOID
EFIAPI
PMIdtHandlerMachineCheckDgr (
  VOID
  );

/**
  Protected Mode IDT handler.
**/
VOID
EFIAPI
PMIdtHandlerDgr (
  VOID
  );

/**
  Initialize SMM Protected Mode IDT table.

  @param[in]  Cr3      CR3 value.
**/
VOID
InitProtectedModeIdtDgr (
  IN UINT32  Cr3
  )
{
  UINTN                               Index;
  UINTN                               PmIdtSize;
  SMM_PM_IA32_IDT_GATE_DESCRIPTOR_DGR *PmIdtEntry;
  UINTN                               InterruptHandler;
  IA32_IDT_GATE_DESCRIPTOR            *IdtEntry;

  if (gSmmDgrProtModeIdtr != 0) {
    return;
  }

  //
  // Patch CR3 for SMM Protected Mode
  //
  gSmmDgrSmiExceptionCr3 = (UINT32)(UINTN)Cr3;

  //
  // Allocate IDT table size
  //
  PmIdtSize = sizeof (SMM_PM_IA32_IDT_GATE_DESCRIPTOR_DGR) * (EXCEPT_IA32_MACHINE_CHECK + 1) * 2;
  mSmmDgrSmiPMIdtr.Base  = (UINTN) SmmFeatureAllocateCodePagesDgr (EFI_SIZE_TO_PAGES(PmIdtSize));
  ASSERT (mSmmDgrSmiPMIdtr.Base != 0);
  if (mSmmDgrSmiPMIdtr.Base == 0) {
    return;
  }

  ZeroMem ((VOID *)mSmmDgrSmiPMIdtr.Base, PmIdtSize);
  mSmmDgrSmiPMIdtr.Limit = (UINT16) PmIdtSize - 1;
  PmIdtEntry =  (SMM_PM_IA32_IDT_GATE_DESCRIPTOR_DGR *)(mSmmDgrSmiPMIdtr.Base);

  gSmmDgrProtModeIdtr = (UINT32)(UINTN)&mSmmDgrSmiPMIdtr;
  //
  // Set up IA32 IDT handler
  //
  for (Index = 0; Index < EXCEPTION_VECTOR_NUMBER; Index++) {
    if (Index == EXCEPT_IA32_MACHINE_CHECK) {
      InterruptHandler = (UINTN)PMIdtHandlerMachineCheckDgr;
    } else {
      InterruptHandler = (UINTN)PMIdtHandlerDgr;
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
  IdtEntry  = (IA32_IDT_GATE_DESCRIPTOR *) mSmmDgrSmiPMIdtr.Base;
  IdtEntry += EXCEPT_IA32_MACHINE_CHECK;
  mSmmDgrMceHandlerLongMode = IdtEntry->Bits.OffsetLow + (((UINTN) IdtEntry->Bits.OffsetHigh)  << 16) +
                                    (((UINTN) IdtEntry->Bits.OffsetUpper) << 32);
  IdtEntry  = (IA32_IDT_GATE_DESCRIPTOR *) mSmmDgrSmiPMIdtr.Base;
  IdtEntry += EXCEPT_IA32_MACHINE_CHECK;
  IdtEntry->Bits.Selector       = LONG_MODE_CODE_SEGMENT;
  IdtEntry->Bits.OffsetLow      = (UINT16)mSmmDgrMceHandlerLongMode;
  IdtEntry->Bits.Reserved_0     = 0;
  IdtEntry->Bits.GateType       = IA32_IDT_GATE_TYPE_INTERRUPT_32;
  IdtEntry->Bits.OffsetHigh     = (UINT16)RShiftU64 (mSmmDgrMceHandlerLongMode, 16);
  IdtEntry->Bits.OffsetUpper    = (UINT32)RShiftU64 (mSmmDgrMceHandlerLongMode, 32);
  IdtEntry->Bits.Reserved_1     = 0;
}
