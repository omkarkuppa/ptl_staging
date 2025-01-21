/** @file
  SMRAM Save State Map header file.

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

#ifndef MM_SAVESTATE_H_
#define MM_SAVESTATE_H_

#include <Uefi/UefiBaseType.h>
#include <Protocol/MmCpu.h>
#include <Protocol/SmmCpu.h>
#include <Register/Intel/SmramSaveStateMap.h>
#include <Register/Intel/Cpuid.h>
#include <Library/DebugLib.h>
#include <Library/MmSaveStateLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#if FixedPcdGetBool (PcdPpamEnable) == 1
#include <Library/DgrSmmLib.h>
#endif
#include <Library/MmServicesTableLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/LocalApicLib.h>
#include <Guid/MsrSaveStateInfoHob.h>
#include <Register/GenerationMsr.h>


#define INTEL_MM_SAVE_STATE_REGISTER_SMMREVID_INDEX   1
#define INTEL_MM_SAVE_STATE_REGISTER_IOMISC_INDEX     2
#define INTEL_MM_SAVE_STATE_REGISTER_IOMEMADDR_INDEX  3
#define INTEL_MM_SAVE_STATE_REGISTER_MAX_INDEX        4

//
// GDTLIMIT, IDTLIMIT, LDTLIMIT and LDTINFO are not defined in struct SMRAM_SAVE_STATE_MAP,
// so macro MM_CPU_OFFSET could not be used.
//
#define SMRAM_SAVE_STATE_OFFSET_GDTLIMIT  0x290
#define SMRAM_SAVE_STATE_OFFSET_IDTLIMIT  0x298
#define SMRAM_SAVE_STATE_OFFSET_LDTLIMIT  0x2A0
#define SMRAM_SAVE_STATE_OFFSET_LDTINFO   0x2A4

//
// Macro used to simplify the lookup table entries of type CPU_MM_SAVE_STATE_LOOKUP_ENTRY
//
#define MM_CPU_OFFSET(Field)  OFFSET_OF (SMRAM_SAVE_STATE_MAP, Field)

//
// Macro used to simplify the lookup table entries of type CPU_MM_SAVE_STATE_REGISTER_RANGE
//
#define MM_REGISTER_RANGE(Start, End)  { Start, End, End - Start + 1 }

extern  UINTN  mMmNumberOfCpus;
extern  UINT32 *mMmApicId;

extern  BOOLEAN  mMmSaveStateInMsr;

//
// Structure used to describe a range of registers
//
typedef struct {
  EFI_MM_SAVE_STATE_REGISTER    Start;
  EFI_MM_SAVE_STATE_REGISTER    End;
  UINTN                         Length;
} CPU_MM_SAVE_STATE_REGISTER_RANGE;

//
// Structure used to build a lookup table to retrieve the widths and offsets
// associated with each supported EFI_MM_SAVE_STATE_REGISTER value
//
typedef struct {
  UINT8      Width32;
  UINT8      Width64;
  UINT16     Offset32;
  UINT16     Offset64Lo;
  UINT16     Offset64Hi;
  UINT32     MsrIndex;
  UINT8      OffsetInMsr;
  BOOLEAN    Writeable;
} CPU_MM_SAVE_STATE_LOOKUP_ENTRY;

typedef struct {
  UINT32    MsrIndex;
  UINT64    MsrValue;
} MM_MSR_INFO;

//
// Structure used to build a lookup table for the IOMisc width information
//
typedef struct {
  UINT8                         Width;
  EFI_MM_SAVE_STATE_IO_WIDTH    IoWidth;
} CPU_MM_SAVE_STATE_IO_WIDTH;

/**
  The common constructor function.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
MmSaveStateLibConstructorCommon (
  VOID
  );

#endif
