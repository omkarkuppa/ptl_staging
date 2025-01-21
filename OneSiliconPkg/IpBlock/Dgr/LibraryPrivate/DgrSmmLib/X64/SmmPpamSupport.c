/** @file
  SMM PPAM support functions

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

#include <PiSmm.h>
#include "SmmPpam.h"
#include <Library/BaseLib.h>

///
/// Page Table Entry
///
#define IA32_PG_P                   BIT0
#define IA32_PG_RW                  BIT1
#define IA32_PG_PS                  BIT7

/**

  Create 4G page table for STM.
  2M PAE page table in X64 version.

  @param PageTableBase        The page table base in MSEG

**/
VOID
PpamGen4GPageTable (
  IN UINTN              PageTableBase
  )
{
  UINTN                             Index;
  UINTN                             SubIndex;
  UINT64                            *Pde;
  UINT64                            *Pte;
  UINT64                            *Pml4;

  Pml4 = (UINT64*)(UINTN)PageTableBase;
  PageTableBase += SIZE_4KB;
  *Pml4 = PageTableBase | IA32_PG_RW | IA32_PG_P;

  Pde = (UINT64*)(UINTN)PageTableBase;
  PageTableBase += SIZE_4KB;
  Pte = (UINT64 *)(UINTN)PageTableBase;

  for (Index = 0; Index < 4; Index++) {
    *Pde = PageTableBase | IA32_PG_RW | IA32_PG_P;
    Pde++;
    PageTableBase += SIZE_4KB;

    for (SubIndex = 0; SubIndex < SIZE_4KB / sizeof (*Pte); SubIndex++) {
      *Pte = LShiftU64 ((LShiftU64 (Index, 9) + SubIndex),21) | IA32_PG_PS | IA32_PG_RW | IA32_PG_P;
      Pte++;
    }
  }
}
