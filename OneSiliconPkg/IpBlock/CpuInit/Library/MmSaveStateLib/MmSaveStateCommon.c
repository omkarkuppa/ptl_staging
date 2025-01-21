/** @file
  Provides services to access SMRAM Save State Map

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

#include "MmSaveState.h"

//
// Lookup table used to retrieve the widths and offsets associated with each
// supported EFI_MM_SAVE_STATE_REGISTER value
//
CONST CPU_MM_SAVE_STATE_LOOKUP_ENTRY  mMmCpuWidthOffset[] = {
  { 0, 0, 0,                             0,                                  0,                                  0,                    0, FALSE }, //  Reserved

  //
  // Internally defined CPU Save State Registers. Not defined in PI SMM CPU Protocol.
  //
  { 4, 4, MM_CPU_OFFSET (x86.SMMRevId),  MM_CPU_OFFSET (x64.SMMRevId),       0,                                  MSR_SMRAM_SMM_REVID,  0, FALSE }, // SMM_SAVE_STATE_REGISTER_SMMREVID_INDEX        = 1
  { 4, 4, MM_CPU_OFFSET (x86.IOMisc),    MM_CPU_OFFSET (x64.IOMisc),         0,                                  MSR_SMRAM_IOMISCINFO, 0, FALSE }, // SMM_SAVE_STATE_REGISTER_IOMISC_INDEX          = 2
  { 4, 8, MM_CPU_OFFSET (x86.IOMemAddr), MM_CPU_OFFSET (x64.IOMemAddr),      MM_CPU_OFFSET (x64.IOMemAddr) + 4,  0,                    0, FALSE }, // SMM_SAVE_STATE_REGISTER_IOMEMADDR_INDEX       = 3

  //
  // CPU Save State registers defined in PI SMM CPU Protocol.
  //
  { 0, 8, 0,                             MM_CPU_OFFSET (x64.GdtBaseLoDword), MM_CPU_OFFSET (x64.GdtBaseHiDword), MSR_SMRAM_GDTR_BASE,  0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_GDTBASE  = 4
  { 0, 8, 0,                             MM_CPU_OFFSET (x64.IdtBaseLoDword), MM_CPU_OFFSET (x64.IdtBaseHiDword), MSR_SMRAM_IDTR_BASE,  0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_IDTBASE  = 5
  { 0, 8, 0,                             MM_CPU_OFFSET (x64.LdtBaseLoDword), MM_CPU_OFFSET (x64.LdtBaseHiDword), MSR_SMRAM_LDTR_BASE,  0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_LDTBASE  = 6
  { 0, 4, 0,                             SMRAM_SAVE_STATE_OFFSET_GDTLIMIT,   0,                                  0,                    0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_GDTLIMIT = 7
  { 0, 4, 0,                             SMRAM_SAVE_STATE_OFFSET_IDTLIMIT,   0,                                  0,                    0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_IDTLIMIT = 8
  { 0, 4, 0,                             SMRAM_SAVE_STATE_OFFSET_LDTLIMIT,   0,                                  0,                    0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_LDTLIMIT = 9
  { 0, 4, 0,                             SMRAM_SAVE_STATE_OFFSET_LDTINFO,    0,                                  0,                    0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_LDTINFO  = 10

  { 4, 4, MM_CPU_OFFSET (x86._ES),       MM_CPU_OFFSET (x64._ES),            0,                                  MSR_SMRAM_CS_ES,      0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_ES       = 20
  { 4, 4, MM_CPU_OFFSET (x86._CS),       MM_CPU_OFFSET (x64._CS),            0,                                  MSR_SMRAM_CS_ES,      4, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_CS       = 21
  { 4, 4, MM_CPU_OFFSET (x86._SS),       MM_CPU_OFFSET (x64._SS),            0,                                  MSR_SMRAM_DS_SS,      0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_SS       = 22
  { 4, 4, MM_CPU_OFFSET (x86._DS),       MM_CPU_OFFSET (x64._DS),            0,                                  MSR_SMRAM_DS_SS,      4, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_DS       = 23
  { 4, 4, MM_CPU_OFFSET (x86._FS),       MM_CPU_OFFSET (x64._FS),            0,                                  MSR_SMRAM_GS_FS,      0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_FS       = 24
  { 4, 4, MM_CPU_OFFSET (x86._GS),       MM_CPU_OFFSET (x64._GS),            0,                                  MSR_SMRAM_GS_FS,      4, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_GS       = 25
  { 0, 4, 0,                             MM_CPU_OFFSET (x64._LDTR),          0,                                  MSR_SMRAM_TR_LDTR,    0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_LDTR_SEL = 26
  { 4, 4, MM_CPU_OFFSET (x86._TR),       MM_CPU_OFFSET (x64._TR),            0,                                  MSR_SMRAM_TR_LDTR,    4, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_TR_SEL   = 27
  { 4, 8, MM_CPU_OFFSET (x86._DR7),      MM_CPU_OFFSET (x64._DR7),           MM_CPU_OFFSET (x64._DR7)     + 4,   MSR_SMRAM_DR7,        0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_DR7      = 28
  { 4, 8, MM_CPU_OFFSET (x86._DR6),      MM_CPU_OFFSET (x64._DR6),           MM_CPU_OFFSET (x64._DR6)     + 4,   MSR_SMRAM_DR6,        0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_DR6      = 29
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R8),            MM_CPU_OFFSET (x64._R8)      + 4,   MSR_SMRAM_R8,         0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R8       = 30
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R9),            MM_CPU_OFFSET (x64._R9)      + 4,   MSR_SMRAM_R9,         0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R9       = 31
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R10),           MM_CPU_OFFSET (x64._R10)     + 4,   MSR_SMRAM_R10,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R10      = 32
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R11),           MM_CPU_OFFSET (x64._R11)     + 4,   MSR_SMRAM_R11,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R11      = 33
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R12),           MM_CPU_OFFSET (x64._R12)     + 4,   MSR_SMRAM_R12,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R12      = 34
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R13),           MM_CPU_OFFSET (x64._R13)     + 4,   MSR_SMRAM_R13,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R13      = 35
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R14),           MM_CPU_OFFSET (x64._R14)     + 4,   MSR_SMRAM_R14,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R14      = 36
  { 0, 8, 0,                             MM_CPU_OFFSET (x64._R15),           MM_CPU_OFFSET (x64._R15)     + 4,   MSR_SMRAM_R15,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_R15      = 37
  { 4, 8, MM_CPU_OFFSET (x86._EAX),      MM_CPU_OFFSET (x64._RAX),           MM_CPU_OFFSET (x64._RAX)     + 4,   MSR_SMRAM_RAX,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RAX      = 38
  { 4, 8, MM_CPU_OFFSET (x86._EBX),      MM_CPU_OFFSET (x64._RBX),           MM_CPU_OFFSET (x64._RBX)     + 4,   MSR_SMRAM_RBX,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RBX      = 39
  { 4, 8, MM_CPU_OFFSET (x86._ECX),      MM_CPU_OFFSET (x64._RCX),           MM_CPU_OFFSET (x64._RCX)     + 4,   MSR_SMRAM_RCX,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RCX      = 40
  { 4, 8, MM_CPU_OFFSET (x86._EDX),      MM_CPU_OFFSET (x64._RDX),           MM_CPU_OFFSET (x64._RDX)     + 4,   MSR_SMRAM_RDX,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RDX      = 41
  { 4, 8, MM_CPU_OFFSET (x86._ESP),      MM_CPU_OFFSET (x64._RSP),           MM_CPU_OFFSET (x64._RSP)     + 4,   MSR_SMRAM_RSP,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RSP      = 42
  { 4, 8, MM_CPU_OFFSET (x86._EBP),      MM_CPU_OFFSET (x64._RBP),           MM_CPU_OFFSET (x64._RBP)     + 4,   MSR_SMRAM_RBP,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RBP      = 43
  { 4, 8, MM_CPU_OFFSET (x86._ESI),      MM_CPU_OFFSET (x64._RSI),           MM_CPU_OFFSET (x64._RSI)     + 4,   MSR_SMRAM_RSI,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RSI      = 44
  { 4, 8, MM_CPU_OFFSET (x86._EDI),      MM_CPU_OFFSET (x64._RDI),           MM_CPU_OFFSET (x64._RDI)     + 4,   MSR_SMRAM_RDI,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RDI      = 45
  { 4, 8, MM_CPU_OFFSET (x86._EIP),      MM_CPU_OFFSET (x64._RIP),           MM_CPU_OFFSET (x64._RIP)     + 4,   MSR_SMRAM_RIP,        0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RIP      = 46

  { 4, 8, MM_CPU_OFFSET (x86._EFLAGS),   MM_CPU_OFFSET (x64._RFLAGS),        MM_CPU_OFFSET (x64._RFLAGS)  + 4,   MSR_SMRAM_EFLAGS,     0, TRUE  }, //  EFI_SMM_SAVE_STATE_REGISTER_RFLAGS   = 51
  { 4, 8, MM_CPU_OFFSET (x86._CR0),      MM_CPU_OFFSET (x64._CR0),           MM_CPU_OFFSET (x64._CR0)     + 4,   MSR_SMRAM_CR0,        0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_CR0      = 52
  { 4, 8, MM_CPU_OFFSET (x86._CR3),      MM_CPU_OFFSET (x64._CR3),           MM_CPU_OFFSET (x64._CR3)     + 4,   MSR_SMRAM_CR3,        0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_CR3      = 53
  { 0, 4, 0,                             MM_CPU_OFFSET (x64._CR4),           0,                                  MSR_SMRAM_CR4,        0, FALSE }, //  EFI_SMM_SAVE_STATE_REGISTER_CR4      = 54
};

//
// Lookup table for the IOMisc width information
//
STATIC CONST CPU_MM_SAVE_STATE_IO_WIDTH  mMmCpuIoWidth[] = {
  { 0, EFI_MM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 0
  { 1, EFI_MM_SAVE_STATE_IO_WIDTH_UINT8  },  // SMM_IO_LENGTH_BYTE  = 1
  { 2, EFI_MM_SAVE_STATE_IO_WIDTH_UINT16 },  // SMM_IO_LENGTH_WORD  = 2
  { 0, EFI_MM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 3
  { 4, EFI_MM_SAVE_STATE_IO_WIDTH_UINT32 },  // SMM_IO_LENGTH_DWORD = 4
  { 0, EFI_MM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 5
  { 0, EFI_MM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 6
  { 0, EFI_MM_SAVE_STATE_IO_WIDTH_UINT8  }   // Undefined           = 7
};

///
/// Lookup table for the IOMisc type information
///
STATIC CONST EFI_MM_SAVE_STATE_IO_TYPE  mMmCpuIoType[] = {
  EFI_MM_SAVE_STATE_IO_TYPE_OUTPUT,     // SMM_IO_TYPE_OUT_DX        = 0
  EFI_MM_SAVE_STATE_IO_TYPE_INPUT,      // SMM_IO_TYPE_IN_DX         = 1
  EFI_MM_SAVE_STATE_IO_TYPE_STRING,     // SMM_IO_TYPE_OUTS          = 2
  EFI_MM_SAVE_STATE_IO_TYPE_STRING,     // SMM_IO_TYPE_INS           = 3
  (EFI_MM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 4
  (EFI_MM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 5
  EFI_MM_SAVE_STATE_IO_TYPE_REP_PREFIX, // SMM_IO_TYPE_REP_OUTS      = 6
  EFI_MM_SAVE_STATE_IO_TYPE_REP_PREFIX, // SMM_IO_TYPE_REP_INS       = 7
  EFI_MM_SAVE_STATE_IO_TYPE_OUTPUT,     // SMM_IO_TYPE_OUT_IMMEDIATE = 8
  EFI_MM_SAVE_STATE_IO_TYPE_INPUT,      // SMM_IO_TYPE_OUT_IMMEDIATE = 9
  (EFI_MM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 10
  (EFI_MM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 11
  (EFI_MM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 12
  (EFI_MM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 13
  (EFI_MM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 14
  (EFI_MM_SAVE_STATE_IO_TYPE)0          // Undefined                 = 15
};

//
// Table used by MmSaveStateGetRegisterIndex() to convert an EFI_MM_SAVE_STATE_REGISTER
// value to an index into a table of type CPU_MM_SAVE_STATE_LOOKUP_ENTRY
//
CONST CPU_MM_SAVE_STATE_REGISTER_RANGE  mCpuRegisterRanges[] = {
  MM_REGISTER_RANGE (EFI_MM_SAVE_STATE_REGISTER_GDTBASE, EFI_MM_SAVE_STATE_REGISTER_LDTINFO),
  MM_REGISTER_RANGE (EFI_MM_SAVE_STATE_REGISTER_ES,      EFI_MM_SAVE_STATE_REGISTER_RIP),
  MM_REGISTER_RANGE (EFI_MM_SAVE_STATE_REGISTER_RFLAGS,  EFI_MM_SAVE_STATE_REGISTER_CR4),
  { (EFI_MM_SAVE_STATE_REGISTER)0,                       (EFI_MM_SAVE_STATE_REGISTER)0,      0}
};

BOOLEAN  mMmSpsStateSaveEnable = FALSE;

//
// Global spin lock used to serialize AP MSR access
//
SPIN_LOCK  *mSaveStateMsrSpinLock = NULL;

BOOLEAN  mMmSaveStateInMsr = FALSE;

///
/// The mode of the CPU at the time an SMI occurs
///
UINT8  mMmFeatureSaveStateRegisterLma;

UINTN  mMmNumberOfCpus = 1;
UINT32 *mMmApicId      = NULL;

UINT32 mSmmRevId = 0;

/**
  Returns LMA value of the Processor.

  @retval     UINT8     returns LMA bit value.
**/
UINT8
MmSaveStateGetRegisterLma (
  VOID
  )
{
  UINT32  RegEax;
  UINT32  RegEdx;
  UINTN   FamilyId;
  UINTN   ModelId;
  UINT8   SmmSaveStateRegisterLma;

  //
  // Retrieve CPU Family
  //
  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  FamilyId = (RegEax >> 8) & 0xf;
  ModelId  = (RegEax >> 4) & 0xf;
  if ((FamilyId == 0x06) || (FamilyId == 0x0f)) {
    ModelId = ModelId | ((RegEax >> 12) & 0xf0);
  }

  RegEdx = 0;
  AsmCpuid (CPUID_EXTENDED_FUNCTION, &RegEax, NULL, NULL, NULL);
  if (RegEax >= CPUID_EXTENDED_CPU_SIG) {
    AsmCpuid (CPUID_EXTENDED_CPU_SIG, NULL, NULL, NULL, &RegEdx);
  }

  //
  // Determine the mode of the CPU at the time an SMI occurs
  //   Intel(R) 64 and IA-32 Architectures Software Developer's Manual
  //   Volume 3C, Section 34.4.1.1
  //
  SmmSaveStateRegisterLma = EFI_MM_SAVE_STATE_REGISTER_LMA_32BIT;
  if ((RegEdx & BIT29) != 0) {
    SmmSaveStateRegisterLma = EFI_MM_SAVE_STATE_REGISTER_LMA_64BIT;
  }

  if (FamilyId == 0x06) {
    if ((ModelId == 0x17) || (ModelId == 0x0f) || (ModelId == 0x1c)) {
      SmmSaveStateRegisterLma = EFI_MM_SAVE_STATE_REGISTER_LMA_64BIT;
    }
  }

  return SmmSaveStateRegisterLma;
}

/**
  Read information from the CPU save state.

  @param  Register  Specifies the CPU register to read form the save state.
  @param  RegOffset Offset for the next register index.

  @retval 0   Register is not valid
  @retval >0  Index into mMmCpuWidthOffset[] associated with Register

**/
UINTN
MmSaveStateGetRegisterIndex (
  IN EFI_MM_SAVE_STATE_REGISTER  Register,
  IN UINTN                       RegOffset
  )
{
  UINTN  Index;
  UINTN  Offset;

  for (Index = 0, Offset = RegOffset; mCpuRegisterRanges[Index].Length != 0; Index++) {
    if ((Register >= mCpuRegisterRanges[Index].Start) && (Register <= mCpuRegisterRanges[Index].End)) {
      return Register - mCpuRegisterRanges[Index].Start + Offset;
    }

    Offset += mCpuRegisterRanges[Index].Length;
  }

  return 0;
}

/**
  Read a CPU Save State MSR on an AP to the buffer.

  @param[in,out] Buffer  Pointer to private data buffer.
**/
VOID
EFIAPI
MmReadSaveStateMsrOnAp (
  IN OUT VOID  *Buffer
  )
{
  MM_MSR_INFO  *MsrInfo;

  MsrInfo           = (MM_MSR_INFO *)Buffer;
  MsrInfo->MsrValue = AsmReadMsr64 (MsrInfo->MsrIndex);

  //
  // Release the MSR spin lock.
  //
  ReleaseSpinLock (mSaveStateMsrSpinLock);
}

/**
  Read a CPU Save State MSR on the target processor.

  If the target processor is the BSP, directly read the MSR. Otherwise, call blocking SmmStartupThisAp()
  to read the MSR on the target processor.

  @retval EFI_SUCCESS           The register was read from Save State MSR.
  @retval EFI_UNSUPPORTED       The executing CPU is AP and it requires to read Save State MSR on other CPU.
**/
EFI_STATUS
MmReadSaveStateMsr (
  IN     UINTN        CpuIndex,
  IN OUT MM_MSR_INFO  *MsrInfo
  )
{
  EFI_STATUS  Status;
  UINT64      CurrentApicId;

  ASSERT (mMmApicId != NULL);
  CurrentApicId = GetApicId ();

  ASSERT (CpuIndex < mMmNumberOfCpus);
  if (mMmApicId[CpuIndex] == CurrentApicId) {
    //
    // The executing CPU can directorly read the local MSR
    //
    MsrInfo->MsrValue = AsmReadMsr64 (MsrInfo->MsrIndex);
  } else {
    ASSERT (gMmst->CurrentlyExecutingCpu < mMmNumberOfCpus);

    //
    // AP read MSR on other CPU is not permittted.
    //
    ASSERT (mMmApicId[gMmst->CurrentlyExecutingCpu] == CurrentApicId);
    if (mMmApicId[gMmst->CurrentlyExecutingCpu] != CurrentApicId) {
      return EFI_UNSUPPORTED;
    }

    //
    // Acquire MSR spin lock.  The AP will release the
    // spin lock when it is done executing MmReadSaveStateMsrOnAp().
    //
    AcquireSpinLock (mSaveStateMsrSpinLock);

    //
    // Call SmmStartupThisAp() to read MSR on AP.
    //
    Status = gMmst->MmStartupThisAp (MmReadSaveStateMsrOnAp, CpuIndex, MsrInfo);
    ASSERT_EFI_ERROR (Status);

    //
    // Wait for the AP to release the MSR spin lock.
    //
    while (!AcquireSpinLockOrFail (mSaveStateMsrSpinLock)) {
      CpuPause ();
    }

    //
    // Release the MSR spin lock.
    //
    ReleaseSpinLock (mSaveStateMsrSpinLock);
  }

  return EFI_SUCCESS;
}

/**
  Read a CPU Save State register on the target processor.

  This function abstracts the differences that whether the CPU Save State register is in the
  IA32 CPU Save State Map or X64 CPU Save State Map.

  This function supports reading a CPU Save State register in SMBase relocation handler.

  @param[in]  CpuIndex       Specifies the zero-based index of the CPU save state.
  @param[in]  RegisterIndex  Index into mMmCpuWidthOffset[] look up table.
  @param[in]  Width          The number of bytes to read from the CPU save state.
  @param[out] Buffer         Upon return, this holds the CPU register value read from the save state.

  @retval EFI_SUCCESS           The register was read from Save State.
  @retval EFI_NOT_FOUND         The register is not defined for the Save State of Processor.
  @retval EFI_UNSUPPORTED       No corresponding save state MSR for the RegisterIndex.
                                The executing CPU is AP and it requires to read Save State MSR on other CPU.
  @retval EFI_INVALID_PARAMTER  This or Buffer is NULL.

**/
EFI_STATUS
MmSaveStateReadRegisterByIndex (
  IN UINTN  CpuIndex,
  IN UINTN  RegisterIndex,
  IN UINTN  Width,
  OUT VOID  *Buffer
  )
{
  MM_MSR_INFO  MsrInfo;
  EFI_STATUS   Status;

  if (RegisterIndex == 0) {
    return EFI_NOT_FOUND;
  }

  if (mMmFeatureSaveStateRegisterLma == EFI_MM_SAVE_STATE_REGISTER_LMA_32BIT) {
    //
    // If 32-bit mode width is zero, then the specified register can not be accessed
    //
    if (mMmCpuWidthOffset[RegisterIndex].Width32 == 0) {
      return EFI_NOT_FOUND;
    }

    //
    // If Width is bigger than the 32-bit mode width, then the specified register can not be accessed
    //
    if (Width > mMmCpuWidthOffset[RegisterIndex].Width32) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Write return buffer
    //
    ASSERT (gMmst->CpuSaveState[CpuIndex] != NULL);
    CopyMem (Buffer, (UINT8 *)gMmst->CpuSaveState[CpuIndex] + mMmCpuWidthOffset[RegisterIndex].Offset32, Width);
  } else {
    //
    // If 64-bit mode width is zero, then the specified register can not be accessed
    //
    if (mMmCpuWidthOffset[RegisterIndex].Width64 == 0) {
      return EFI_NOT_FOUND;
    }

    //
    // If Width is bigger than the 64-bit mode width, then the specified register can not be accessed
    //
    if (Width > mMmCpuWidthOffset[RegisterIndex].Width64) {
      return EFI_INVALID_PARAMETER;
    }

    if (mMmSaveStateInMsr) {
      MsrInfo.MsrIndex = mMmCpuWidthOffset[RegisterIndex].MsrIndex;
      if (MsrInfo.MsrIndex == 0) {
        //
        // If no corresponding MSR
        //
        return EFI_UNSUPPORTED;
      }

      Status = MmReadSaveStateMsr (CpuIndex, &MsrInfo);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      CopyMem (Buffer, (UINT8 *)&MsrInfo.MsrValue + mMmCpuWidthOffset[RegisterIndex].OffsetInMsr, Width);
    } else {
      //
      // Write lower 32-bits of return buffer
      //
      CopyMem (Buffer, (UINT8 *)gMmst->CpuSaveState[CpuIndex] + mMmCpuWidthOffset[RegisterIndex].Offset64Lo, MIN (4, Width));
      if (Width > 4) {
        //
        // Write upper 32-bits of return buffer
        //
        CopyMem ((UINT8 *)Buffer + 4, (UINT8 *)gMmst->CpuSaveState[CpuIndex] + mMmCpuWidthOffset[RegisterIndex].Offset64Hi, Width - 4);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Read an SMM Save State register on the target processor.  If this function
  returns EFI_UNSUPPORTED, then the caller is responsible for reading the
  SMM Save Sate register.

  @param[in]  CpuIndex  The index of the CPU to read the SMM Save State.  The
                        value must be between 0 and the NumberOfCpus field in
                        the System Management System Table (SMST).
  @param[in]  Register  The SMM Save State register to read.
  @param[in]  Width     The number of bytes to read from the CPU save state.
  @param[out] Buffer    Upon return, this holds the CPU register value read
                        from the save state.

  @retval EFI_SUCCESS           The register was read from Save State.
  @retval EFI_INVALID_PARAMTER  Buffer is NULL.
  @retval EFI_UNSUPPORTED       This function does not support reading Register.
  @retval EFI_NOT_FOUND         If desired Register not found.
**/
EFI_STATUS
EFIAPI
MmSaveStateReadRegister (
  IN  UINTN                       CpuIndex,
  IN  EFI_MM_SAVE_STATE_REGISTER  Register,
  IN  UINTN                       Width,
  OUT VOID                        *Buffer
  )
{
  SMRAM_SAVE_STATE_IOMISC    IoMisc;
  EFI_MM_SAVE_STATE_IO_INFO  *IoInfo;

  //
  // Check for special EFI_MM_SAVE_STATE_REGISTER_LMA
  //
  if (Register == EFI_MM_SAVE_STATE_REGISTER_LMA) {
    //
    // Only byte access is supported for this register
    //
    if (Width != 1) {
      return EFI_INVALID_PARAMETER;
    }

    *(UINT8 *)Buffer = mMmFeatureSaveStateRegisterLma;

    return EFI_SUCCESS;
  }

  //
  // Check for special EFI_MM_SAVE_STATE_REGISTER_IO
  //
  if (Register == EFI_MM_SAVE_STATE_REGISTER_IO) {
    if (!mMmSpsStateSaveEnable) {
      if (mSmmRevId == 0) {
        //
        // Get SMM Revision ID
        //
        MmSaveStateReadRegisterByIndex (CpuIndex, INTEL_MM_SAVE_STATE_REGISTER_SMMREVID_INDEX, sizeof (mSmmRevId), &mSmmRevId);
      }

      //
      // See if the CPU supports the IOMisc register in the save state
      //
      if (mSmmRevId < SMRAM_SAVE_STATE_MIN_REV_ID_IOMISC) {
        return EFI_NOT_FOUND;
      }
    }

#if FixedPcdGetBool (PcdPpamEnable) == 1
  //
  // Get the IOMisc register value
  //
  if (mMmSpsStateSaveEnable) {
    // IoMisc.Uint32 = IOMisc value from user exception stack buffer;
    IoMisc.Uint32 = DgrStateSaveReadIoMisc (CpuIndex);
  } else {
     MmSaveStateReadRegisterByIndex (CpuIndex, INTEL_MM_SAVE_STATE_REGISTER_IOMISC_INDEX, sizeof (IoMisc.Uint32), &IoMisc.Uint32);
  }
#else
  MmSaveStateReadRegisterByIndex (CpuIndex, INTEL_MM_SAVE_STATE_REGISTER_IOMISC_INDEX, sizeof (IoMisc.Uint32), &IoMisc.Uint32);
#endif


    //
    // Check for the SMI_FLAG in IOMisc
    //
    if (IoMisc.Bits.SmiFlag == 0) {
      return EFI_NOT_FOUND;
    }

    //
    // Only support IN/OUT, but not INS/OUTS/REP INS/REP OUTS.
    //
    if ((mMmCpuIoType[IoMisc.Bits.Type] != EFI_MM_SAVE_STATE_IO_TYPE_INPUT) &&
        (mMmCpuIoType[IoMisc.Bits.Type] != EFI_MM_SAVE_STATE_IO_TYPE_OUTPUT))
    {
      return EFI_NOT_FOUND;
    }

    //
    // Compute index for the I/O Length and I/O Type lookup tables
    //
    if ((mMmCpuIoWidth[IoMisc.Bits.Length].Width == 0) || (mMmCpuIoType[IoMisc.Bits.Type] == 0)) {
      return EFI_NOT_FOUND;
    }

    //
    // Make sure the incoming buffer is large enough to hold IoInfo before accessing
    //
    if (Width < sizeof (EFI_MM_SAVE_STATE_IO_INFO)) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Zero the IoInfo structure that will be returned in Buffer
    //
    IoInfo = (EFI_MM_SAVE_STATE_IO_INFO *)Buffer;
    ZeroMem (IoInfo, sizeof (EFI_MM_SAVE_STATE_IO_INFO));

    //
    // Use lookup tables to help fill in all the fields of the IoInfo structure
    //
    IoInfo->IoPort  = (UINT16)IoMisc.Bits.Port;
    IoInfo->IoWidth = mMmCpuIoWidth[IoMisc.Bits.Length].IoWidth;
    IoInfo->IoType  = mMmCpuIoType[IoMisc.Bits.Type];

#if FixedPcdGetBool (PcdPpamEnable) == 1
  if (mMmSpsStateSaveEnable) {
    IoInfo->IoData = DgrStateSaveReadIoInfoData (CpuIndex, IoInfo->IoWidth);
  } else {
    MmSaveStateReadRegister (CpuIndex, EFI_MM_SAVE_STATE_REGISTER_RAX, mMmCpuIoWidth[IoMisc.Bits.Length].Width, &IoInfo->IoData);
  }
#else
  MmSaveStateReadRegister (CpuIndex, EFI_MM_SAVE_STATE_REGISTER_RAX, mMmCpuIoWidth[IoMisc.Bits.Length].Width, &IoInfo->IoData);
#endif


    return EFI_SUCCESS;
  }

  //
  // Read RAX vlaue from Exception Stack if input argument Register value is of RAX.
  //
#if FixedPcdGetBool (PcdPpamEnable) == 1
  if ((mMmSpsStateSaveEnable == TRUE) && (Register == EFI_SMM_SAVE_STATE_REGISTER_RAX)) {
    return DgrStateSaveReadRax (CpuIndex, Width, Buffer);
  }

  //
  // Sps state save enable, SMI handler can read only IO_MISC register, and parts of RAX.
  // There is an exception to access the cpu save state to read other registers
  //
  if (mMmSpsStateSaveEnable) {
    return EFI_NOT_FOUND;
  }
#endif

  //
  // Convert Register to a register lookup table index
  //
  return MmSaveStateReadRegisterByIndex (CpuIndex, MmSaveStateGetRegisterIndex (Register, INTEL_MM_SAVE_STATE_REGISTER_MAX_INDEX), Width, Buffer);
}

/**
  Write the value in the buffer into a CPU Save State MSR on an AP.

  @param[in,out] Buffer  Pointer to private data buffer.
**/
VOID
EFIAPI
MmWriteSaveStateMsrOnAp (
  IN OUT VOID  *Buffer
  )
{
  MM_MSR_INFO  *MsrInfo;

  MsrInfo = (MM_MSR_INFO *)Buffer;
  AsmWriteMsr64 (MsrInfo->MsrIndex, MsrInfo->MsrValue);

  //
  // Release the MSR spin lock.
  //
  ReleaseSpinLock (mSaveStateMsrSpinLock);
}

/**
  Write value to a CPU Save State MSR on the target processor.

  If the target processor is the BSP, directly write the MSR. Otherwise, call blocking SmmStartupThisAp()
  to write the MSR on the target processor.

  @param[in] CpuIndex  Specifies the zero-based index of the CPU save state.
  @param[in] MsrInfo   Pointer to a MM_MSR_INFO data structure holding the index and value of a MSR.

  @retval EFI_SUCCESS           The register was written to Save State MSR.
  @retval EFI_UNSUPPORTED       The executing CPU is AP and it requires to write Save State MSR on other CPU.
**/
EFI_STATUS
MmWriteSaveStateMsr (
  IN UINTN        CpuIndex,
  IN MM_MSR_INFO  *MsrInfo
  )
{
  EFI_STATUS  Status;
  UINT64      CurrentApicId;

  ASSERT (mMmApicId != NULL);
  CurrentApicId = GetApicId ();

  //
  // For MSR_EFLAGS, we can NOT write BIT1, which is reserved field.
  // Even though its reserve value is "1", writing "1" to BIT1 of MSR_EFLAGS will cause #GP.
  //
  if (MsrInfo->MsrIndex == MSR_SMRAM_EFLAGS) {
    MsrInfo->MsrValue &= ~((UINT64)BIT1);
  }

  ASSERT (CpuIndex < mMmNumberOfCpus);
  if (mMmApicId[CpuIndex] == CurrentApicId) {
    //
    // The executing CPU can directorly write the local MSR
    //
    AsmWriteMsr64 (MsrInfo->MsrIndex, MsrInfo->MsrValue);
  } else {
    ASSERT (gMmst->CurrentlyExecutingCpu < mMmNumberOfCpus);

    //
    // AP write MSR on other CPU is not permittted.
    //
    ASSERT (mMmApicId[gMmst->CurrentlyExecutingCpu] == CurrentApicId);
    if (mMmApicId[gMmst->CurrentlyExecutingCpu] != CurrentApicId) {
      return EFI_UNSUPPORTED;
    }

    //
    // Acquire MSR spin lock.  The AP will release the
    // spin lock when it is done executing MmReadSaveStateMsrOnAp().
    //
    AcquireSpinLock (mSaveStateMsrSpinLock);

    //
    // Call SmmStartupThisAp() to write MSR on AP.
    //
    Status = gMmst->MmStartupThisAp (MmWriteSaveStateMsrOnAp, CpuIndex, MsrInfo);
    ASSERT_EFI_ERROR (Status);

    //
    // Wait for the AP to release the MSR spin lock.
    //
    while (!AcquireSpinLockOrFail (mSaveStateMsrSpinLock)) {
      CpuPause ();
    }

    //
    // Release the MSR spin lock.
    //
    ReleaseSpinLock (mSaveStateMsrSpinLock);
  }

  return EFI_SUCCESS;
}

/**
  Writes an SMM Save State register on the target processor.  If this function
  returns EFI_UNSUPPORTED, then the caller is responsible for writing the
  SMM Save Sate register.

  @param[in] CpuIndex  The index of the CPU to write the SMM Save State.  The
                       value must be between 0 and the NumberOfCpus field in
                       the System Management System Table (SMST).
  @param[in] Register  The SMM Save State register to write.
  @param[in] Width     The number of bytes to write to the CPU save state.
  @param[in] Buffer    Upon entry, this holds the new CPU register value.

  @retval EFI_SUCCESS           The register was written to Save State.
  @retval EFI_INVALID_PARAMTER  Buffer is NULL.
  @retval EFI_UNSUPPORTED       This function does not support writing Register.
  @retval EFI_NOT_FOUND         If desired Register not found.
**/
EFI_STATUS
EFIAPI
MmSaveStateWriteRegister (
  IN UINTN                       CpuIndex,
  IN EFI_MM_SAVE_STATE_REGISTER  Register,
  IN UINTN                       Width,
  IN CONST VOID                  *Buffer
  )
{
  UINTN                 RegisterIndex;
  SMRAM_SAVE_STATE_MAP  *CpuSaveState;
  MM_MSR_INFO           MsrInfo;
  EFI_STATUS            Status;

  //
  // Writes to EFI_MM_SAVE_STATE_REGISTER_LMA are ignored
  //
  if (Register == EFI_MM_SAVE_STATE_REGISTER_LMA) {
    return EFI_SUCCESS;
  }

  //
  // Writes to EFI_MM_SAVE_STATE_REGISTER_IO are not supported
  //
  if (Register == EFI_MM_SAVE_STATE_REGISTER_IO) {
    return EFI_NOT_FOUND;
  }

  //
  // Convert Register to a register lookup table index
  //
  RegisterIndex = MmSaveStateGetRegisterIndex (Register, INTEL_MM_SAVE_STATE_REGISTER_MAX_INDEX);
  if (RegisterIndex == 0) {
    return EFI_NOT_FOUND;
  }

  CpuSaveState = gMmst->CpuSaveState[CpuIndex];

  //
  // Do not write non-writable SaveState, because it will cause exception.
  //
  if (!mMmCpuWidthOffset[RegisterIndex].Writeable) {
    return EFI_UNSUPPORTED;
  }

  //
  // Check CPU mode
  //
  if (mMmFeatureSaveStateRegisterLma == EFI_MM_SAVE_STATE_REGISTER_LMA_32BIT) {
    //
    // If 32-bit mode width is zero, then the specified register can not be accessed
    //
    if (mMmCpuWidthOffset[RegisterIndex].Width32 == 0) {
      return EFI_NOT_FOUND;
    }

    //
    // If Width is bigger than the 32-bit mode width, then the specified register can not be accessed
    //
    if (Width > mMmCpuWidthOffset[RegisterIndex].Width32) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // Write SMM State register
    //
    ASSERT (CpuSaveState != NULL);
    CopyMem ((UINT8 *)CpuSaveState + mMmCpuWidthOffset[RegisterIndex].Offset32, Buffer, Width);
  } else {
    //
    // If 64-bit mode width is zero, then the specified register can not be accessed
    //
    if (mMmCpuWidthOffset[RegisterIndex].Width64 == 0) {
      return EFI_NOT_FOUND;
    }

    //
    // If Width is bigger than the 64-bit mode width, then the specified register can not be accessed
    //
    if (Width > mMmCpuWidthOffset[RegisterIndex].Width64) {
      return EFI_INVALID_PARAMETER;
    }

    if (mMmSaveStateInMsr) {
      MsrInfo.MsrIndex = mMmCpuWidthOffset[RegisterIndex].MsrIndex;
      if (MsrInfo.MsrIndex == 0) {
        //
        // If no corresponding MSR
        //
        return EFI_UNSUPPORTED;
      }

      //
      // If the CPU Save State register is to be partially modified, read the current value to keep the un-modified part intact.
      //
      if ((mMmCpuWidthOffset[RegisterIndex].Width64 != sizeof (UINT64)) ||
          (mMmCpuWidthOffset[RegisterIndex].Width64 != Width))
      {
        Status = MmReadSaveStateMsr (CpuIndex, &MsrInfo);
        if (EFI_ERROR (Status)) {
          return Status;
        }
      }

      CopyMem ((UINT8 *)&MsrInfo.MsrValue + mMmCpuWidthOffset[RegisterIndex].OffsetInMsr, Buffer, Width);
      Status = MmWriteSaveStateMsr (CpuIndex, &MsrInfo);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    } else {
      //
      // Write at most 4 of the lower bytes of SMM State register
      //
#if FixedPcdGetBool (PcdPpamEnable) == 1
      if ((mMmSpsStateSaveEnable) && (Register == EFI_SMM_SAVE_STATE_REGISTER_RAX)) {
        DgrStateSaveWriteRaxLower (CpuIndex, Width, Buffer);
      } else {
        CopyMem ((UINT8 *)CpuSaveState + mMmCpuWidthOffset[RegisterIndex].Offset64Lo, Buffer, MIN (4, Width));
      }
#else
      CopyMem ((UINT8 *)CpuSaveState + mMmCpuWidthOffset[RegisterIndex].Offset64Lo, Buffer, MIN (4, Width));
#endif

      if (Width > 4) {
        //
        // Write at most 4 of the upper bytes of SMM State register
        //
#if FixedPcdGetBool (PcdPpamEnable) == 1
        if ((mMmSpsStateSaveEnable) && (Register == EFI_SMM_SAVE_STATE_REGISTER_RAX)) {
          // Copy higher 32bits of RAX to higher 32 bits of user exception stack buffer.
          DgrStateSaveWriteRaxHigher (CpuIndex, Width, Buffer);
        } else {
          CopyMem ((UINT8 *)CpuSaveState + mMmCpuWidthOffset[RegisterIndex].Offset64Hi, (UINT8 *)Buffer + 4, Width - 4);
        }
#else
        CopyMem ((UINT8 *)CpuSaveState + mMmCpuWidthOffset[RegisterIndex].Offset64Hi, (UINT8 *)Buffer + 4, Width - 4);
#endif
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  The common constructor function.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
MmSaveStateLibConstructorCommon (
  VOID
  )
{
  UINTN                    SpinLockAlign;
  VOID                     *SpinLockBuffer;
  VOID                     *GuidHob;
  MSR_SAVE_STATE_INFO_HOB  *MsrSaveStateInfo;

#if FixedPcdGetBool (PcdPpamEnable) == 1
  mMmSpsStateSaveEnable = IsStateSaveEnable ();
#endif

  mMmFeatureSaveStateRegisterLma = MmSaveStateGetRegisterLma ();

  GuidHob = GetFirstGuidHob (&gMsrSaveStateInfoHobGuid);
  if (GuidHob != NULL) {
    MsrSaveStateInfo  = (MSR_SAVE_STATE_INFO_HOB *)GET_GUID_HOB_DATA (GuidHob);
    mMmSaveStateInMsr = MsrSaveStateInfo->SaveStateInMsr;
    DEBUG ((DEBUG_INFO, "MSR_SAVE_STATE_INFO_HOB detected. SaveStateInMsr = %d\n", mMmSaveStateInMsr));
  } else {
    DEBUG ((DEBUG_INFO, "MSR_SAVE_STATE_INFO_HOB is not detected. SaveStateInMsr is disabled\n"));
    mMmSaveStateInMsr = FALSE;
  }

  //
  // Allocate memory for SPIN LOCK buffer
  //
  SpinLockAlign  = GetSpinLockProperties ();
  SpinLockBuffer = AllocatePool (sizeof (SPIN_LOCK) + SpinLockAlign - 1);

  if (SpinLockBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "AllocatePool Returned NULL pointer\n"));
    ASSERT (SpinLockBuffer != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  mSaveStateMsrSpinLock = (SPIN_LOCK *) ALIGN_POINTER (SpinLockBuffer, SpinLockAlign);

  //
  // Initialize spin lock for MSR access
  //
  InitializeSpinLock (mSaveStateMsrSpinLock);

  return EFI_SUCCESS;
}
