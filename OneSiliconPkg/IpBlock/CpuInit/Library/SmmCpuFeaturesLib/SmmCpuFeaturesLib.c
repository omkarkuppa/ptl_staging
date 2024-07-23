/** @file
  The CPU specific programming for PiSmmCpuDxeSmm module, such as
  SMRR.

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

////////
// Below section is common definition
////////

//
// Assumes UP, or MP with identical feature set
//
CPU_SMM_FEATURE_CONTEXT  mFeatureContext;
BOOLEAN                  mSmrrLockSupported;
UINT32                   mSmrr2Base;
UINT32                   mSmrr2Size;
UINT32                   mSmmBaseNotCoveredBySmrr;
UINT32                   mSmmSizeNotCoveredBySmrr = 0;
UINT8                    mSmrr2CacheType;
BOOLEAN                  mSmmUseDelayIndication;
BOOLEAN                  mSmmUseBlockIndication;
BOOLEAN                  mSmmEnableIndication;
BOOLEAN                  mSmmSpsPpam11Enable = FALSE;
BOOLEAN                  mSmmSpsStateSaveEnable = FALSE;
UINT32                   mSmmTestRsvMemorySize;
BOOLEAN                  mSmmProcTraceEnable;
EFI_PROCESSOR_INFORMATION *mSmmProcessorInfo;
UINT32                   *mSmBase;
SMM_CPU_SYNC_FEATURE     *mSmmSyncFeature;
//
// Protected Mode Entrypoint
//
BOOLEAN                  mSmmProtectedModeEnable;
SMM_PROT_MODE_CONTEXT    *mSmmProtModeContext;

//
// Set default value to assume MSR_SMM_FEATURE_CONTROL is not supported
//
BOOLEAN                  mSmmFeatureControlSupported;

BOOLEAN                  mSmmSupovrStateLock = FALSE;

EFI_SMRAM_DESCRIPTOR     *mSmmFeatureSmramRanges;
UINTN                    mSmmFeatureSmramRangeCount;

//
// Indicate SmBase for each Processors has been relocated or not. If TRUE,
// means no need to do the relocation in SmmCpuFeaturesInitializeProcessor().
//
BOOLEAN                  mSmmCpuFeaturesSmmRelocated;

//
// Indicate Smm Initialize Complete or Not
//
BOOLEAN                  mSmmInitializeComplete;

extern IA32_DESCRIPTOR  gSmmFeatureSmiHandlerIdtr;

//
// Variables from Protected Mode SMI Entry Template
//
extern volatile UINT32                     gSmmFeatureSmiStack;
extern UINT32                              gSmmFeatureSmbase;
extern UINT32                              gSmmStackSize;
extern UINT32                              gSmmFeatureSmiCr3;
extern BOOLEAN                             gSmmFeatureXdSupported;
extern UINT32                              gProtModeSmbase;
extern volatile UINT8                      gcSmmFeatureSmiHandlerTemplate[];
extern CONST UINT16                        gcSmmFeatureSmiHandlerSize;
extern UINT32                              gProtModeIdtr;
extern UINT32                              gPMStackDesc[2];
extern BOOLEAN                             gSmmFeatureRing3Supported;
extern BOOLEAN                             gSmmFeatureCetSupported;
extern IA32_PROT_DESCRIPTOR                gGdtDesc;

/**
  Disable CET.
**/
VOID
EFIAPI
DisableCet (
  VOID
  );

/**
  Enable CET.
**/
VOID
EFIAPI
EnableCet (
  VOID
  );

/**
  Allocate pages for code.

  @param  Pages Number of pages to be allocated.

  @return Allocated memory.
**/
VOID *
SmmFeatureAllocateCodePages (
  IN UINTN           Pages
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Memory;

  if (Pages == 0) {
    return NULL;
  }

  Status = gSmst->SmmAllocatePages (AllocateAnyPages, EfiRuntimeServicesCode, Pages, &Memory);
  if (EFI_ERROR (Status)) {
    return NULL;
  }
  return (VOID *) (UINTN) Memory;
}

/**
  Find out SMRAM range information from SMM ACCESS2 Protocol.

**/
VOID
FindSmramRange (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             Size;
  EFI_SMM_ACCESS2_PROTOCOL          *SmmAccess;

  //
  // Get SMM Access Protocol
  //
  Status = gBS->LocateProtocol (&gEfiSmmAccess2ProtocolGuid, NULL, (VOID **)&SmmAccess);
  ASSERT_EFI_ERROR (Status);

  //
  // Get SMRAM information
  //
  Size = 0;
  Status = SmmAccess->GetCapabilities (SmmAccess, &Size, NULL);
  ASSERT (Status == EFI_BUFFER_TOO_SMALL);

  mSmmFeatureSmramRanges = (EFI_SMRAM_DESCRIPTOR *)AllocatePool (Size);
  ASSERT (mSmmFeatureSmramRanges != NULL);

  Status = SmmAccess->GetCapabilities (SmmAccess, &Size, mSmmFeatureSmramRanges);
  ASSERT_EFI_ERROR (Status);

  mSmmFeatureSmramRangeCount = Size / sizeof (EFI_SMRAM_DESCRIPTOR);
}

/**
  The constructor function

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The constructor always returns EFI_SUCCESS.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to complete the function.

**/
EFI_STATUS
EFIAPI
SmmCpuFeaturesLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT32  RegEax;
  UINT32  RegEcx;
  UINT32  RegEdx;
  UINTN   FamilyId;
  UINTN   ModelId;
  DXE_CPU_POLICY_PROTOCOL      *CpuPolicyData;
  EFI_STATUS                   Status;

  //
  // Initialize address fixup
  //
  SmmCpuFeaturesLibSmiEntryFixupAddress ();

  //
  // Retrieve CPU Family and Model
  //
  AsmCpuid(CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  DEBUG ((DEBUG_INFO, "CPUID - 0x%08x\n", RegEax));
  FamilyId = (RegEax >> 8) & 0xf;
  ModelId  = (RegEax >> 4) & 0xf;
  if (FamilyId == 0x06 || FamilyId == 0x0f) {
    ModelId = ModelId | ((RegEax >> 12) & 0xf0);
  }
  DEBUG ((DEBUG_INFO, "FamilyId - 0x%08x\n", FamilyId));
  DEBUG ((DEBUG_INFO, "ModelId - 0x%08x\n", ModelId));

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
  mSmmFeatureSaveStateRegisterLma = EFI_SMM_SAVE_STATE_REGISTER_LMA_32BIT;
  if ((RegEdx & BIT29) != 0) {
    mSmmFeatureSaveStateRegisterLma = EFI_SMM_SAVE_STATE_REGISTER_LMA_64BIT;
  }
  if (FamilyId == 0x06) {
    if (ModelId == 0x17 || ModelId == 0x0f || ModelId == 0x1c) {
      mSmmFeatureSaveStateRegisterLma = EFI_SMM_SAVE_STATE_REGISTER_LMA_64BIT;
    }
  }

  gSmmFeatureXdSupported = FALSE;
  if ((RegEdx & CPUID1_EDX_XD_SUPPORT) != 0) {
    gSmmFeatureXdSupported = TRUE;
  }

  gSmmFeatureCetSupported = FALSE;
  if (PcdGet32 (PcdControlFlowEnforcementPropertyMask)) {
    AsmCpuidEx(CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO, NULL, NULL, &RegEcx, NULL);
    if ((RegEcx & CPUID_CET_SS) != 0) {
      gSmmFeatureCetSupported = TRUE;
    }
  }

  mSmBase = (UINT32 *)AllocatePool(sizeof(UINT32) * PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
  ASSERT(mSmBase != NULL);

  mSmmSyncFeature = (SMM_CPU_SYNC_FEATURE *)AllocatePool (sizeof (SMM_CPU_SYNC_FEATURE) * PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
  ASSERT (mSmmSyncFeature != NULL);

  //
  // Related PCD values are retrieved into global variables.
  //
  mSmmProtectedModeEnable = PcdGetBool (PcdCpuSmmProtectedModeEnable);

  //
  // Update module variable according to policy setting.
  //
  Status = gBS->LocateProtocol (&gDxeCpuPolicyProtocolGuid, NULL, (VOID **) &CpuPolicyData);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    mSmmUseDelayIndication  = CpuPolicyData->SmmUseDelayIndication;
    mSmmUseBlockIndication  = CpuPolicyData->SmmUseBlockIndication;
    mSmmEnableIndication    = CpuPolicyData->SmmUseSmmEnableIndication;

#if FixedPcdGetBool (PcdPpamEnable) == 1
    if (IsDgrEnable () == TRUE) {
      mSmmSpsPpam11Enable = TRUE;
    }
    if (IsStateSaveEnable () == TRUE) {
      mSmmSpsStateSaveEnable = TRUE;
    }
#endif
    if (!mSmmSpsStateSaveEnable) {
      //
      // Only set SMM ProcTrace when SMM StateSave Protection disable for the conflict.
      //
      mSmmProcTraceEnable = CpuPolicyData->SmmProcTraceEnable;
    }
  }

  //
  // Dump some SMM feature values
  //
  DEBUG ((DEBUG_INFO, "SmmUseSmmEnableIndication       = %d\n", mSmmEnableIndication));
  DEBUG ((DEBUG_INFO, "SmmUseBlockIndication           = %d\n", mSmmUseBlockIndication));
  DEBUG ((DEBUG_INFO, "SmmUseDelayIndication           = %d\n", mSmmUseDelayIndication));
  DEBUG ((DEBUG_INFO, "PcdCpuSmmProtectedModeEnable    = %d\n", mSmmProtectedModeEnable));
  DEBUG ((DEBUG_INFO, "mSmmProcTraceEnable             = %d\n", mSmmProcTraceEnable));

  if (mSmmProtectedModeEnable) {
    mSmmProtModeContext = SmmFeatureAllocateCodePages (EFI_SIZE_TO_PAGES(sizeof(SMM_PROT_MODE_CONTEXT) * PcdGet32(PcdCpuMaxLogicalProcessorNumber)));
    ASSERT(mSmmProtModeContext != NULL);
  }

  mSmrr2Base      = PcdGet32(PcdCpuSmmSmrr2Base);
  mSmrr2Size      = PcdGet32(PcdCpuSmmSmrr2Size);
  mSmrr2CacheType = PcdGet8 (PcdCpuSmmSmrr2CacheType);
  DEBUG ((DEBUG_INFO, "PcdCpuSmmSmrr2Base      = %x\n", mSmrr2Base));
  DEBUG ((DEBUG_INFO, "PcdCpuSmmSmrr2Size      = %x\n", mSmrr2Size));
  DEBUG ((DEBUG_INFO, "PcdCpuSmmSmrr2CacheType = %d\n", mSmrr2CacheType));
  ASSERT((mSmrr2CacheType == MTRR_CACHE_WRITE_PROTECTED) || (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK));
  if (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK) {
    //
    // PcdCpuSmmSmrr2Base and PcdCpuSmmSmrr2Size should be zero if cache type is WB
    //
    ASSERT (mSmrr2Base == 0);
    ASSERT (mSmrr2Size == 0);
  }

  mSmmTestRsvMemorySize = PcdGet32 (PcdSmmTestRsvMemorySize);

  FindSmramRange ();

  mSmmInitializeComplete = FALSE;

  //
  // If gSmmBaseHobGuid found, means SmBase info has been relocated and recorded
  // in the SmBase array.
  //
  mSmmCpuFeaturesSmmRelocated = (BOOLEAN)(GetFirstGuidHob (&gSmmBaseHobGuid) != NULL);

  return EFI_SUCCESS;
}


/**
  Verifiy if SMRR base/size meet SMRR programming requirement.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.

  @retval TRUE       SMRR base/size is valid.
  @retval TRUE       SMRR base/size is not valid.
**/
BOOLEAN
VerifySmrrBaseSize (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize
  )
{
  //
  // SMRR size cannot be less than 4-KBytes
  // SMRR size must be of length 2^n
  // SMRR base alignment cannot be less than SMRR length
  //
  if ((SmrrSize < SIZE_4KB) ||
      (SmrrSize != GetPowerOfTwo32 (SmrrSize)) ||
      ((SmrrBase & ~(SmrrSize - 1)) != SmrrBase)) {
    return FALSE;
  }

  return TRUE;
}

/**
  Return if SMRR is supported

  @retval TRUE  SMRR is supported.
  @retval FALSE SMRR is not supported.

**/
BOOLEAN
IsSmrrSupported (
  VOID
  )
{
  MSR_MTRRCAP_REGISTER  MtrrCap;

  //
  // Use ArchMsr.h MSR_MTRRCAP + MSR_MTRRCAP_REGISTER consistently in this file
  // although the Smrr bit is also present in ArchitectureMsr.h MSR_IA32_MTRRCAP +
  // MSR_IA32_MTRRCAP_REGISTER.
  //
  MtrrCap.Uint64 = AsmReadMsr64 (MSR_MTRRCAP);
  return (BOOLEAN) (MtrrCap.Bits.Smrr != 0);
}

/**
  Return if SMRR2 is supported

  @retval TRUE  SMRR2 is supported.
  @retval FALSE SMRR2 is not supported.

**/
BOOLEAN
IsSmrr2Supported (
  VOID
  )
{
  MSR_MTRRCAP_REGISTER  MtrrCap;

  //
  // Use ArchMsr.h MSR_MTRRCAP + MSR_MTRRCAP_REGISTER as the Smrr2 bit is
  // not present in ArchitectureMsr.h MSR_IA32_MTRRCAP + MSR_IA32_MTRRCAP_REGISTER.
  //
  MtrrCap.Uint64 = AsmReadMsr64 (MSR_MTRRCAP);
  return (BOOLEAN) (MtrrCap.Bits.Smrr2 != 0);
}

/**
  Return if SMRR lock is supported

  @retval TRUE  SMRR lock is supported.
  @retval FALSE SMRR lock is not supported.

**/
BOOLEAN
IsSmrrLockSupported (
  VOID
  )
{
  MSR_MTRRCAP_REGISTER  MtrrCap;

  //
  // Use ArchMsr.h MSR_MTRRCAP + MSR_MTRRCAP_REGISTER as the SmrrLock bit is
  // not present in ArchitectureMsr.h MSR_IA32_MTRRCAP + MSR_IA32_MTRRCAP_REGISTER.
  //
  MtrrCap.Uint64 = AsmReadMsr64 (MSR_MTRRCAP);
  return (BOOLEAN) (MtrrCap.Bits.SmrrLock != 0);
}

/**
  Initialize SMRR in SMM relocate.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
  @param  IsMonarch          TRUE if the current CPU is Monarch.
                             FALSE if the current CPU is not Monarch.
**/
VOID
InitSmrr (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize,
  IN BOOLEAN               IsMonarch
  )
{
  UINT32                   Smrr2Base;
  UINT32                   Smrr2Size;
  UINT32                   NewSmrrSize;
  UINT32                   Alignment;
  MSR_SMRR_BASE_REGISTER   MsrSmrrBase;
  MSR_SMRR_MASK_REGISTER   MsrSmrrMask;

  if (!VerifySmrrBaseSize (SmrrBase, SmrrSize)) {
    //
    // If SMRR base/size does not meet requirment, check if we could enable SMRR2
    //
    Smrr2Base = 0;
    Smrr2Size = 0;
    if (IsSmrr2Supported ()) {
      //
      // mSmrr2CacheType should be WB
      //
      ASSERT (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK);
      //
      // Calculate the alignment of the SMM base address.
      //
      Alignment = (UINT32)LShiftU64 (1, LowBitSet32 (SmrrBase));
      if (Alignment >= SmrrSize) {
        Smrr2Size   = SmrrSize - GetPowerOfTwo32 (SmrrSize);
        NewSmrrSize = SmrrSize - Smrr2Size;
      } else {
        Smrr2Size   = SmrrSize - Alignment;
        NewSmrrSize = Alignment;
      }
      Smrr2Base = SmrrBase + NewSmrrSize;
      if (IsMonarch) {
        DEBUG ((DEBUG_INFO, "SmrrBase  = %x, NewSmrrSize = %x\n", SmrrBase, NewSmrrSize));
        DEBUG ((DEBUG_INFO, "Smrr2Base = %x, Smrr2Size   = %x\n", Smrr2Base, Smrr2Size));
      }
      mSmmSizeNotCoveredBySmrr = Smrr2Size;
      mSmmBaseNotCoveredBySmrr = Smrr2Base;
    }
    if (!VerifySmrrBaseSize (Smrr2Base, Smrr2Size)) {
      //
      // Verify SMRR2 base/size
      //
      if (IsMonarch) {
        //
        // Print message and halt if CPU is Monarch
        //
        DEBUG ((DEBUG_ERROR, "SMM Base/Size does not meet alignment/size requirement!\n"));
      }
      CpuDeadLoop ();
    }
  }
  //
  // Use GenerationMsr.h MSR_SMRR_BASE + MSR_SMRR_BASE_REGISTER as the Memtype
  // bit width is not same with the Type bit width in ArchitectureMsr.h
  // MSR_IA32_SMRR_PHYSBASE + MSR_IA32_SMRR_PHYSBASE_REGISTER.
  //
  MsrSmrrBase.Uint64 = AsmReadMsr64 (MSR_SMRR_BASE);
  MsrSmrrBase.Bits.Memtype = MTRR_CACHE_WRITE_BACK;
  MsrSmrrBase.Bits.Base = (UINT32) (SmrrBase >> 12);
  AsmWriteMsr64 (MSR_SMRR_BASE, MsrSmrrBase.Uint64);
  //
  // Use GenerationMsr.h MSR_SMRR_MASK + MSR_SMRR_MASK_REGISTER consistently in this file
  // (the L bit is not present in ArchitectureMsr.h
  // MSR_IA32_SMRR_PHYSMASK + MSR_IA32_SMRR_PHYSMASK_REGISTER).
  //
  // Valid bit will be set in ConfigSmrr() at first SMI.
  //
  MsrSmrrMask.Uint64 = AsmReadMsr64 (MSR_SMRR_MASK);
  MsrSmrrMask.Bits.Mask = (UINT32) (~(SmrrSize - 1) >> 12);
  AsmWriteMsr64 (MSR_SMRR_MASK, MsrSmrrMask.Uint64);
}

/**
  Caculate SMRR2 base/size.

  @param  SmrrBase           The base address of SMRR.
  @param  SmrrSize           The size of SMRR.
  @param  IsMonarch          TRUE if the current CPU is Monarch.
                             FALSE if the current CPU is not Monarch.
**/
VOID
CalculateSmrr2 (
  IN UINT32                SmrrBase,
  IN UINT32                SmrrSize,
  IN BOOLEAN               IsMonarch
  )
{
  UINTN                 Index;
  BOOLEAN               Found;
  UINT32                Smrr2Base;
  UINT32                Smrr2Size;
  UINT32                CpuStart;
  UINT32                PhysicalSize;

  Smrr2Base = 0;
  Smrr2Size = 0;
  do {
    Found = FALSE;
    for (Index = 0; Index < mSmmFeatureSmramRangeCount; Index++) {
      CpuStart     = (UINT32) mSmmFeatureSmramRanges[Index].CpuStart;
      PhysicalSize = (UINT32) mSmmFeatureSmramRanges[Index].PhysicalSize;
      if ((CpuStart < SmrrBase) || (CpuStart >= SmrrBase + SmrrSize)) {
        //
        // If one SMM range is out of current SMRR range
        //
        if (Smrr2Size == 0) {
          //
          // Assign the first SMRR2 range
          //
          Smrr2Base = CpuStart;
          Smrr2Size = PhysicalSize;
          Found = TRUE;
        } else if ((Smrr2Base + Smrr2Size) == CpuStart) {
          //
          // Append new SMM range at end of current SMRR2 range
          //
          Smrr2Size += PhysicalSize;
          Found = TRUE;
        } else if (Smrr2Base == (CpuStart + PhysicalSize)) {
          //
          // Insert new SMM range in front of current SMRR2 range
          //
          Smrr2Base -= PhysicalSize;
          Smrr2Size += PhysicalSize;
          Found = TRUE;
        }
      }
    }
  } while (Found);

  if (IsMonarch) {
    DEBUG ((DEBUG_INFO, "SMRR2 Base: 0x%x, SMRR2 Size: 0x%x\n", Smrr2Base, Smrr2Size));
  }

  if (Smrr2Size != 0 && mSmmSizeNotCoveredBySmrr != 0) {
    //
    // We cannot support 2 SMRR2 range
    //
    if (IsMonarch) {
      DEBUG ((DEBUG_ERROR, "Can not support 2 SMRR2 ranges!\n"));
    }
    CpuDeadLoop ();
  }

  if (Smrr2Size == 0 && mSmmSizeNotCoveredBySmrr == 0) {
    //
    // If there is no SMRR2 requirement for WB memory, the SMRR2 range
    // mSmrr2Base and mSmrr2Size should be from PCDs in SmmCpuFeaturesLibConstructor()
    //
    ASSERT (mSmrr2CacheType == MTRR_CACHE_WRITE_PROTECTED);
  } else {
    ASSERT (mSmrr2CacheType == MTRR_CACHE_WRITE_BACK);
    if (mSmmSizeNotCoveredBySmrr != 0) {
      mSmrr2Base = mSmmBaseNotCoveredBySmrr;
      mSmrr2Size = mSmmSizeNotCoveredBySmrr;
    } else {
      //
      // Smrr2Size != 0
      //
      mSmrr2Base = Smrr2Base;
      mSmrr2Size = Smrr2Size;
    }
  }
  if (!VerifySmrrBaseSize (mSmrr2Base, mSmrr2Size)) {
    if (IsMonarch) {
      DEBUG ((DEBUG_ERROR, "SMRR2 range does not meet SMRR2 requirement!\n"));
    }
    CpuDeadLoop ();
  }
}

/**
  Configure SMRR register at each SMM entry.
**/
VOID
ConfigSmrr (
  VOID
  )
{
  MSR_SMRR_MASK_REGISTER MsrSmrrMask;

  //
  // Use GenerationMsr.h MSR_SMRR_MASK + MSR_SMRR_MASK_REGISTER as the L bit is not present
  // in ArchitectureMsr.h MSR_IA32_SMRR_PHYSMASK + MSR_IA32_SMRR_PHYSMASK_REGISTER.
  //
  MsrSmrrMask.Uint64 = AsmReadMsr64 (MSR_SMRR_MASK);
  if (mSmrrLockSupported) {
    if (MsrSmrrMask.Bits.L == 0) {
      MsrSmrrMask.Bits.Vld = 1;
      MsrSmrrMask.Bits.L = 1;
      AsmWriteMsr64 (MSR_SMRR_MASK, MsrSmrrMask.Uint64);
    }
  } else {
    if (MsrSmrrMask.Bits.Vld == 0) {
      MsrSmrrMask.Bits.Vld = 1;
      AsmWriteMsr64 (MSR_SMRR_MASK, MsrSmrrMask.Uint64);
    }
  }
}

/**
  Initialize SMRR2 in SMM relocate.

  @param  Smrr2Base          The base address of SMRR2.
  @param  Smrr2Size          The size of SMRR2.
**/
VOID
InitSmrr2 (
  IN UINT32                Smrr2Base,
  IN UINT32                Smrr2Size
  )
{
  MSR_SMRR2_MASK_REGISTER   MsrSmrr2Mask;
  MSR_SMRR2_BASE_REGISTER   MsrSmrr2Base;

  ASSERT (Smrr2Size != 0);

  MsrSmrr2Mask.Uint64 = AsmReadMsr64 (MSR_SMRR2_MASK);

  if (mSmrrLockSupported == TRUE) {
    if (MsrSmrr2Mask.Bits.L == 0) {
      //
      // Program SMRR2 Base and Mask
      //
      MsrSmrr2Base.Uint64 = AsmReadMsr64 (MSR_SMRR2_BASE);
      MsrSmrr2Base.Bits.Memtype = mSmrr2CacheType;
      MsrSmrr2Base.Bits.Base = (UINT32) (Smrr2Base >> 12);
      AsmWriteMsr64 (MSR_SMRR2_BASE, MsrSmrr2Base.Uint64);
      MsrSmrr2Mask.Bits.Vld = 1;
      MsrSmrr2Mask.Bits.L = 1;
      MsrSmrr2Mask.Bits.Mask = (UINT32) (~(Smrr2Size - 1) >> 12);
      AsmWriteMsr64 (MSR_SMRR2_MASK, MsrSmrr2Mask.Uint64);
    }
  } else {
    if (MsrSmrr2Mask.Bits.Vld == 0) {
      //
      // Program SMRR2 Base and Mask
      //
      MsrSmrr2Base.Uint64 = AsmReadMsr64 (MSR_SMRR2_BASE);
      MsrSmrr2Base.Bits.Memtype = mSmrr2CacheType;
      MsrSmrr2Base.Bits.Base = (UINT32) (Smrr2Base >> 12);
      AsmWriteMsr64 (MSR_SMRR2_BASE, MsrSmrr2Base.Uint64);
      MsrSmrr2Mask.Bits.Vld = 1;
      MsrSmrr2Mask.Bits.Mask = (UINT32) (~(Smrr2Size - 1) >> 12);
      AsmWriteMsr64 (MSR_SMRR2_MASK, MsrSmrr2Mask.Uint64);
    }
  }
}

////////
// Below section is external function
////////
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
  )
{
  UINT64                        RetVal;
  MSR_SMM_DELAYED_REGISTER      SmmDelayed;
  MSR_SMM_BLOCKED_REGISTER  SmmBlocked;

  RetVal = 0;

  switch (RegName) {
  //
  // Client uses MSR
  //
  case  SmmRegFeatureControl:
    RetVal = AsmReadMsr64 (MSR_SMM_FEATURE_CONTROL);
    break;
  case  SmmRegSmmEnable:
    RetVal = 0;   // Targeted SMI--disabling specific CPU cores to respond enter into SMI-- is not supported in Client BIOS.
    break;
  case  SmmRegSmmDelayed:
    SmmDelayed.Uint64 = AsmReadMsr64 (MSR_SMM_DELAYED);
    RetVal = SmmDelayed.Bits.DelayedCount;
    break;
  case  SmmRegSmmBlocked:
    SmmBlocked.Uint64 = AsmReadMsr64 (MSR_SMM_BLOCKED);
    RetVal = SmmBlocked.Bits.BlockedCount;
    break;
  default:
    ASSERT (FALSE);
  }
  return  RetVal;
}

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
  )
{
  switch (RegName) {
  //
  // Client uses MSR
  //
  case  SmmRegFeatureControl:
    AsmWriteMsr64 (MSR_SMM_FEATURE_CONTROL, RegValue);
    break;
  case  SmmRegSmmEnable:
    break; // Targeted SMI--disabling specific CPU cores to respond enter into SMI-- is not supported in Client BIOS.
  default:
    ASSERT (FALSE);
  }
}

/**
  Disable SMRR register if SMRR is supported and SmmCpuFeaturesNeedConfigureMtrrs()
  returns TRUE.
**/
VOID
EFIAPI
SmmCpuFeaturesDisableSmrr (
  VOID
  )
{
}

/**
  Enable SMRR register if SMRR is supported and SmmCpuFeaturesNeedConfigureMtrrs()
  returns TRUE.
**/
VOID
EFIAPI
SmmCpuFeaturesReenableSmrr (
  VOID
  )
{
}

/**
  Determines if MTRR registers must be configured to set SMRAM cache-ability
  when executing in System Management Mode.

  @retval TRUE   MTRR registers must be configured to set SMRAM cache-ability.
  @retval FALSE  MTRR registers do not need to be configured to set SMRAM
                 cache-ability.
**/
BOOLEAN
EFIAPI
SmmCpuFeaturesNeedConfigureMtrrs (
  VOID
  )
{
  return FALSE;
}

/**
  This function sets DR6 & DR7 according to SMM save state, before running SMM C code.
  They are useful when you want to enable hardware breakpoints in SMM without entry SMM mode.

  NOTE: It might not be appreciated in runtime since it might
        conflict with OS debugging facilities. Turn them off in RELEASE.

  @param    CpuIndex              CPU Index

**/
VOID
EFIAPI
SmmFeatureCpuSmmDebugEntry (
  IN UINTN  CpuIndex
  )
{
  SMRAM_SAVE_STATE_MAP *CpuSaveState;

  if (!mSmmInitializeComplete) {
    //
    // Before SmmInitializeComplete, mSmBase is not initialized to the relocated address.
    // So, here, skip directly.
    //
    return;
  }

  if (FeaturePcdGet (PcdCpuSmmDebug)) {
    CpuSaveState = (SMRAM_SAVE_STATE_MAP *)((UINTN)mSmBase[CpuIndex] + SMRAM_SAVE_STATE_MAP_OFFSET);
    if (mSmmFeatureSaveStateRegisterLma == EFI_SMM_SAVE_STATE_REGISTER_LMA_32BIT) {
      AsmWriteDr6 (CpuSaveState->x86._DR6);
      AsmWriteDr7 (CpuSaveState->x86._DR7);
    } else {
      AsmWriteDr6 ((UINTN)CpuSaveState->x64._DR6);
      AsmWriteDr7 ((UINTN)CpuSaveState->x64._DR7);
    }
  }
}

/**
  This function restores DR6 & DR7 to SMM save state.

  NOTE: It might not be appreciated in runtime since it might
        conflict with OS debugging facilities. Turn them off in RELEASE.

  @param    CpuIndex              CPU Index

**/
VOID
EFIAPI
SmmFeatureCpuSmmDebugExit (
  IN UINTN  CpuIndex
  )
{
  SMRAM_SAVE_STATE_MAP *CpuSaveState;

  if (!mSmmInitializeComplete) {
    //
    // Before SmmInitializeComplete, mSmBase is not initialized to the relocated address.
    // So, here, skip directly.
    //
    return;
  }

  if (FeaturePcdGet (PcdCpuSmmDebug)) {
    CpuSaveState = (SMRAM_SAVE_STATE_MAP *)((UINTN)mSmBase[CpuIndex] + SMRAM_SAVE_STATE_MAP_OFFSET);
    if (mSmmFeatureSaveStateRegisterLma == EFI_SMM_SAVE_STATE_REGISTER_LMA_32BIT) {
      CpuSaveState->x86._DR7 = (UINT32)AsmReadDr7 ();
      CpuSaveState->x86._DR6 = (UINT32)AsmReadDr6 ();
    } else {
      CpuSaveState->x64._DR7 = AsmReadDr7 ();
      CpuSaveState->x64._DR6 = AsmReadDr6 ();
    }
  }
}

/**
  Processor specific hook point each time a CPU enters System Management Mode.

  @param[in] CpuIndex  The index of the CPU that has entered SMM.  The value
                       must be between 0 and the NumberOfCpus field in the
                       System Management System Table (SMST).
**/
VOID
EFIAPI
SmmCpuFeaturesRendezvousEntry (
  IN UINTN  CpuIndex
  )
{
  MSR_IA32_RTIT_CTL_REGISTER               RtitCtl;
  UINT32 ThreadIndex;

  GetProcessorLocationByApicId (GetInitialApicId (), NULL, NULL, &ThreadIndex);
  if (ThreadIndex == 0) {
    //
    // Configure SMRR register at each SMM entry.
    //
    if (mFeatureContext.SmrrEnabled) {
      ConfigSmrr ();
    }
    if (mFeatureContext.Smrr2Enabled) {
      //
      // Note that SMRR2 registers can't be written in the first SMI because
      // MSR Save State feature is not enabled.
      //
      InitSmrr2 (mSmrr2Base, mSmrr2Size);
    }
  }

  //
  // Configure ProcTrace feature at SMM entry.
  //
  if (mSmmProcTraceEnable) {
    RtitCtl.Uint64 = AsmReadMsr64 (MSR_IA32_RTIT_CTL);
    if (RtitCtl.Bits.TraceEn == 0) {
      RtitCtl.Bits.TraceEn = 1;
      AsmWriteMsr64 (MSR_IA32_RTIT_CTL, RtitCtl.Uint64);
    }
  }
}

/**
  Processor specific hook point each time a CPU exits System Management Mode.

  @param[in] CpuIndex  The index of the CPU that is exiting SMM.  The value must
                       be between 0 and the NumberOfCpus field in the System
                       Management System Table (SMST).
**/
VOID
EFIAPI
SmmCpuFeaturesRendezvousExit (
  IN UINTN  CpuIndex
  )
{
  MSR_IA32_RTIT_CTL_REGISTER       RtitCtl;
  //
  // Configure ProcTrace feature at SMM Exit.
  //
  if (mSmmProcTraceEnable) {
    RtitCtl.Uint64 = AsmReadMsr64(MSR_IA32_RTIT_CTL);
    if (RtitCtl.Bits.TraceEn != 0) {
      RtitCtl.Bits.TraceEn = 0;
      AsmWriteMsr64 (MSR_IA32_RTIT_CTL, RtitCtl.Uint64);
    }
  }
}

/**
  Hook point in normal execution mode that allows the one CPU that was elected
  as monarch during System Management Mode initialization to perform additional
  initialization actions immediately after all of the CPUs have processed their
  first SMI and called SmmCpuFeaturesInitializeProcessor() relocating SMBASE
  into a buffer in SMRAM and called SmmCpuFeaturesHookReturnFromSmm().
**/
VOID
EFIAPI
SmmCpuFeaturesSmmRelocationComplete (
  VOID
  )
{
  mSmmInitializeComplete = TRUE;

  //
  // Dump some SMM feature setting after SMM rellocation
  //
  DEBUG ((DEBUG_INFO, "CpuSmmFeatureControlSupported= %d\n", mSmmFeatureControlSupported));
  DEBUG ((DEBUG_INFO, "CpuSmmProtectedModeEnable    = %d\n", mSmmProtectedModeEnable));
  DEBUG ((DEBUG_INFO, "CpuSmmUseDelayIndication     = %d\n", mSmmUseDelayIndication));
  DEBUG ((DEBUG_INFO, "CpuSmmUseBlockIndication     = %d\n", mSmmUseBlockIndication));
  DEBUG ((DEBUG_INFO, "CpuSmmUseSmmEnableIndication = %d\n", mSmmEnableIndication));
  DEBUG ((DEBUG_INFO, "CpuSmmSupovrStateLock        = %d\n", mSmmSupovrStateLock));
  DEBUG ((DEBUG_INFO, "mSmmSpsPpam11Enable          = %d\n", mSmmSpsPpam11Enable));

  if (!IsSmrrSupported ()) {
    return;
  }
  mFeatureContext.SmrrEnabled = TRUE;
  mFeatureContext.Smrr2Enabled = FALSE;

  if (IsSmrrLockSupported ()) {
    mSmrrLockSupported = TRUE;
  } else {
    mSmrrLockSupported = FALSE;
  }
  DEBUG ((DEBUG_INFO, "SmrrLockSupported = %x\n", mSmrrLockSupported));
}

/**
  Setup SMM Protected Mode context for processor.

  @param  ProcessorNumber    The processor number.
  @param  SmBase             The SMBASE value of the processor.
  @param  StackAddress       Stack address of the processor.
  @param  GdtBase            Gdt table base address of the processor.
  @param  GdtSize            Gdt table size of the processor.
  @param  CodeSegment        Code segment value.
  @param  ProtModeIdt        Pointer to protected-mode IDT descriptor.
**/
VOID
SetupSmmProtectedModeContext(
  IN UINTN                    ProcessorNumber,
  IN UINT32                   SmBase,
  IN UINT32                   StackAddress,
  IN UINTN                    GdtBase,
  IN UINTN                    GdtSize,
  IN UINT16                   CodeSegment,
  IN IA32_DESCRIPTOR          *ProtModeIdt
  )
{
  SMMSEG                      *ThreadSmmSeg;
  IA32_SEGMENT_DESCRIPTOR     *GdtDescriptor;
  UINTN                       GdtEntryIndex;

  if (!mSmmProtectedModeEnable) {
    return;
  }

  //
  // Fill SMMSEG structure fields
  //
  ThreadSmmSeg = &mSmmProtModeContext[ProcessorNumber].SmmProtectedModeSMMSEG;
  ThreadSmmSeg->GDTRLimit = (UINT32)(GdtSize - 1);
  ThreadSmmSeg->GDTRBaseOffset = (UINT32)GdtBase - SmBase;
  ThreadSmmSeg->CSSelector = CodeSegment;
  ThreadSmmSeg->ESPOffset = StackAddress - SmBase;
  ThreadSmmSeg->IDTRLimit = ProtModeIdt->Limit;
  ThreadSmmSeg->IDTRBaseOffset = (UINT32)ProtModeIdt->Base - SmBase;
  //
  // Patch 32bit CS/SS segment base to SMBASE, SS = CS + 8 decided by hardware
  //
  GdtEntryIndex = ThreadSmmSeg->CSSelector / sizeof(IA32_SEGMENT_DESCRIPTOR);
  GdtDescriptor = (IA32_SEGMENT_DESCRIPTOR *)GdtBase;
  GdtDescriptor[GdtEntryIndex].Bits.BaseLow = (UINT16)0x0000FFFF & SmBase;
  GdtDescriptor[GdtEntryIndex].Bits.BaseMid = (UINT8)0x000000FF & (SmBase >> 16);
  GdtDescriptor[GdtEntryIndex].Bits.BaseHigh = (UINT8)0x000000FF & (SmBase >> 24);
  GdtDescriptor[GdtEntryIndex + 1].Bits.BaseLow = (UINT16)0x0000FFFF & SmBase;
  GdtDescriptor[GdtEntryIndex + 1].Bits.BaseMid = (UINT8)0x000000FF & (SmBase >> 16);
  GdtDescriptor[GdtEntryIndex + 1].Bits.BaseHigh = (UINT8)0x000000FF & (SmBase >> 24);
}

/**
  Enable SMM Protected Mode.

  @param  ProcessorNumber    The processor number.

**/
VOID
EnableSmmProtectedMode (
  IN UINTN   ProcessorNumber
  )
{
  MSR_SMM_MCA_CAP_REGISTER          SmmMcaCapMsr;
  MSR_SMM_PROT_MODE_BASE_REGISTER   SmmProtModeBaseMsr;
  SMMSEG                            *ThreadSmmSeg;

  //
  // Check if hardware support SMM PROT MODE feature
  //
  SmmMcaCapMsr.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP);
  if (SmmMcaCapMsr.Bits.SmmProtMode == 0) {
    mSmmProtectedModeEnable = FALSE;
    FreePages (mSmmProtModeContext, EFI_SIZE_TO_PAGES(sizeof(SMM_PROT_MODE_CONTEXT) * PcdGet32(PcdCpuMaxLogicalProcessorNumber)));
    return ;
  }

  //
  // Enable the SMM PROT MODE feature in the SMMSEG structure
  //
  ThreadSmmSeg = &mSmmProtModeContext[ProcessorNumber].SmmProtectedModeSMMSEG;
  ThreadSmmSeg->SmmSegFeatureEnables = (SMMSEG_FEATURE_ENABLE | SMMSEG_FEATURE_CR4_MCE_CTL_ENABLE);

  //
  // SMMSEG should be 256-byte aligned 32-bit address
  //
  SmmProtModeBaseMsr.Uint64 = (UINT64) (UINTN) ThreadSmmSeg;
  SmmProtModeBaseMsr.Bits.Enable = 1;
  AsmWriteMsr64 (MSR_SMM_PROT_MODE_BASE, SmmProtModeBaseMsr.Uint64);
}

/**
  Called during the very first SMI into System Management Mode to initialize
  CPU features, including SMBASE, for the currently executing CPU.  Since this
  is the first SMI, the SMRAM Save State Map is at the default address of
  SMM_DEFAULT_SMBASE + SMRAM_SAVE_STATE_MAP_OFFSET.  The currently executing
  CPU is specified by CpuIndex and CpuIndex can be used to access information
  about the currently executing CPU in the ProcessorInfo array and the
  HotPlugCpuData data structure.

  @param[in] CpuIndex        The index of the CPU to initialize.  The value
                             must be between 0 and the NumberOfCpus field in
                             the System Management System Table (SMST).
  @param[in] IsMonarch       TRUE if the CpuIndex is the index of the CPU that
                             was elected as monarch during System Management
                             Mode initialization.
                             FALSE if the CpuIndex is not the index of the CPU
                             that was elected as monarch during System
                             Management Mode initialization.
  @param[in] ProcessorInfo   Pointer to an array of EFI_PROCESSOR_INFORMATION
                             structures.  ProcessorInfo[CpuIndex] contains the
                             information for the currently executing CPU.
  @param[in] CpuHotPlugData  Pointer to the CPU_HOT_PLUG_DATA structure that
                             contains the ApidId and SmBase arrays.
**/
VOID
EFIAPI
SmmCpuFeaturesInitializeProcessor (
  IN UINTN                      CpuIndex,
  IN BOOLEAN                    IsMonarch,
  IN EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  IN CPU_HOT_PLUG_DATA          *CpuHotPlugData
  )
{
  SMRAM_SAVE_STATE_MAP              *CpuState;
  MSR_SMM_MCA_CAP_REGISTER          SmmMcaCapMsr;
  MSR_SMM_FEATURE_CONTROL_REGISTER  SmmFeatureControl;
  SMM_CPU_SYNC_FEATURE              *SyncFeature;
  UINTN                             Cr0;
  STATIC BOOLEAN                    SmmTestRsvMemoryInit = FALSE;
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX  Ebx;
  MSR_PLATFORM_INFO_REGISTER        PlatformInfoMsr;
  BOOLEAN                           CetEnabled;
  EFI_STATUS                        Status;

  mSmmProcessorInfo = ProcessorInfo;

  //
  // If requested, allocate SMM memory for test purposes only.
  //

  if (mSmmTestRsvMemorySize && !SmmTestRsvMemoryInit) {
    VOID *RsvRegion;

    DEBUG ((DEBUG_INFO, "\nPcdSmmTestRsvMemorySize = 0x%x\n", mSmmTestRsvMemorySize));

    RsvRegion = AllocatePool (mSmmTestRsvMemorySize);
    if (RsvRegion == NULL) {
      DEBUG ((DEBUG_ERROR, "Can't allocate reserved memory range.\n"));
    } else {
      DEBUG ((DEBUG_INFO, "Base of test reserved memory = 0x%x", (UINT32)(UINTN)RsvRegion));
    }

    SmmTestRsvMemoryInit = TRUE;
  }

  mSmBase[CpuIndex] = (UINT32)CpuHotPlugData->SmBase[CpuIndex];

  SyncFeature = &(mSmmSyncFeature[CpuIndex]);
  SyncFeature->TargetedSmiSupported = FALSE;
  SyncFeature->DelayIndicationSupported = FALSE;
  SyncFeature->BlockIndicationSupported = FALSE;

  //
  // Configure SMBASE.
  //

  //
  // Check SMM Code Access Check bit before access SMM Feature Control MSR
  //
  SmmMcaCapMsr.Uint64 = AsmReadMsr64 (MSR_SMM_MCA_CAP);
  if (SmmMcaCapMsr.Bits.SmmCodeAccessChk != 0) {
    mSmmFeatureControlSupported = TRUE;
    SmmFeatureControl.Uint64 = SmmReadReg64 (CpuIndex, SmmRegFeatureControl);
    ASSERT (SmmFeatureControl.Bits.SmmCpuSaveEn == 0);
  }

  //
  // No need to configure SMBASE if SmBase relocation has been done.
  //
  if (!mSmmCpuFeaturesSmmRelocated) {
    //
    // Fall back to legacy SMBASE setup.
    //
    CpuState = (SMRAM_SAVE_STATE_MAP*)(UINTN)(SMM_DEFAULT_SMBASE + SMRAM_SAVE_STATE_MAP_OFFSET);
    CpuState->x86.SMBASE = (UINT32)CpuHotPlugData->SmBase[CpuIndex];
  }

  if (IsSmrrSupported ()) {
    InitSmrr (CpuHotPlugData->SmrrBase, CpuHotPlugData->SmrrSize, IsMonarch);
  }
  if (IsSmrr2Supported ()) {
    CalculateSmrr2 (CpuHotPlugData->SmrrBase, CpuHotPlugData->SmrrSize, IsMonarch);
  }

  if (mSmmUseDelayIndication) {
    SyncFeature->DelayIndicationSupported = (BOOLEAN) (SmmMcaCapMsr.Bits.LongFlowIndication != 0);
  }
  if (mSmmUseBlockIndication) {
    SyncFeature->BlockIndicationSupported = TRUE;
  }
  if (mSmmEnableIndication) {
    SyncFeature->TargetedSmiSupported = (BOOLEAN) (SmmMcaCapMsr.Bits.TargetedSmi != 0);
  }

  //
  // Check ProcTrace Capability
  //
  AsmCpuidEx(CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, NULL, &Ebx.Uint32, NULL, NULL);
  if (Ebx.Bits.IntelProcessorTrace == 0) {
    mSmmProcTraceEnable = FALSE;
  }

  PlatformInfoMsr.Uint64 = AsmReadMsr64 (MSR_PLATFORM_INFO);
  if (PlatformInfoMsr.Bits.SmmSupovrStateLockEnable) {
    //
    // At this point in normal boot, supervisor state lock data being cleared.
    // At S3 resume time in SmmCpuFeaturesCompleteSmmReadyToLock() supervisor state lock data is set
    // to enable lock for paging state, SMBase, Monitor control.
    //

    CetEnabled = ((AsmReadCr4() & BIT23) != 0) ? TRUE : FALSE;
    if (CetEnabled) {
      //
      // CET must be disabled if WP is disabled.
      //
      DisableCet();
    }

    Cr0 = AsmReadCr0();
    AsmWriteCr0 (Cr0 & ~CR0_WP);

    if (mSmmSpsPpam11Enable) {
      Status = DgrPatch0SupovrAndRing3Support (mSmBase, CpuIndex);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Failed to execute DgrPatch0SupovrAndRing3Support. Status = %r\n", Status));
      }
    } else {
      *(UINT64 *)(UINTN)(mSmBase[CpuIndex] + (UINTN)&gSmmSupovrStateLockData - (UINTN)gcSmmFeatureSmiHandlerTemplate + SMM_HANDLER_OFFSET) = 0;
    }

    AsmWriteCr0 (Cr0);
    if (CetEnabled) {
      //
      // re-enable CET.
      //
      EnableCet();
    }

    mSmmSupovrStateLock = TRUE;
    if (!PcdGetBool(PcdCpuSmmRestrictedMemoryAccess)) {
      if (CpuIndex == 0) {
        DEBUG ((DEBUG_ERROR, "Unsupported Configuration - PcdCpuSmmRestrictedMemoryAccess is FALSE but SmmSupovrStateLock is TRUE\n"));
      }
#if FixedPcdGetBool(PcdSpecialPoolEnable) == 0
      ASSERT (FALSE);
#endif
    }
   }
  if (mSmmProtectedModeEnable) {
    EnableSmmProtectedMode (CpuIndex);
  }

  if (CpuIndex == 0) {
    DEBUG((DEBUG_INFO, "  Microcode ID (0x%08x)  = 0x%016lx\n", MSR_IA32_BIOS_SIGN_ID, AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID)));
    DEBUG((DEBUG_INFO, "  Platform ID (0x%08x)   = 0x%016lx\n", MSR_IA32_PLATFORM_ID, AsmReadMsr64 (MSR_IA32_PLATFORM_ID)));
    DEBUG((DEBUG_INFO, "  SMM_MCA_CAP (0x%08x)   = 0x%016lx\n", MSR_SMM_MCA_CAP, AsmReadMsr64 (MSR_SMM_MCA_CAP)));
    }

  if (mSmmSpsPpam11Enable) {
    DgrFinishInitializeProcessor ();
  }
}

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
SmmCpuFeaturesGetSmiHandlerSize (
  VOID
  )
{
  if (mSmmSpsPpam11Enable) {
    return DgrGetSmiHandlerSizeSps ();
  }

  return gcSmmFeatureSmiHandlerSize;
}

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
SmmCpuFeaturesInstallSmiHandler (
  IN UINTN   CpuIndex,
  IN UINT32  SmBase,
  IN VOID    *SmiStack,
  IN UINTN   StackSize,
  IN UINTN   GdtBase,
  IN UINTN   GdtSize,
  IN UINTN   IdtBase,
  IN UINTN   IdtSize,
  IN UINT32  Cr3
  )
{
  if (mSmmSpsPpam11Enable) {
    DgrInstallSmiHandlerSps (CpuIndex, SmBase, SmiStack, StackSize, GdtBase, GdtSize, IdtBase, IdtSize, Cr3);
    return;
  }

  if (mSmmProtectedModeEnable) {
    //
    // Initialize protected mode IDT
    //
    InitProtectedModeIdt (Cr3);
  }

  //
  // Initialize values in template before copy
  //
  gSmmFeatureSmiStack   = (UINT32)((UINTN)SmiStack + StackSize - sizeof (UINTN));


  if (PcdGet32 (PcdControlFlowEnforcementPropertyMask)) {
    SmmCpuFeaturesInitShadowStack (CpuIndex, (VOID *)((UINTN)SmiStack + StackSize));
    DEBUG ((DEBUG_INFO, "StackSize - 0x%x\n", StackSize));
  }

  gSmmFeatureSmiCr3     = Cr3;
  gSmmFeatureSmiHandlerIdtr.Base = IdtBase;
  gSmmFeatureSmiHandlerIdtr.Limit = (UINT16)(IdtSize - 1);
  DEBUG ((DEBUG_INFO, "gSmmFeatureSmiHandlerIdtr - 0x%x\n", &gSmmFeatureSmiHandlerIdtr));
  DEBUG ((DEBUG_INFO, "Base - 0x%x\n", gSmmFeatureSmiHandlerIdtr.Base));
  DEBUG ((DEBUG_INFO, "Limit - 0x%x\n", gSmmFeatureSmiHandlerIdtr.Limit));
  gGdtDesc.Base = (UINT32)GdtBase;
  gGdtDesc.Limit = (UINT16)(GdtSize - 1);

  //
  // Set the value at the top of the CPU stack to the CPU Index
  //
  *(UINTN*)(UINTN)gSmmFeatureSmiStack = CpuIndex;
  gPMStackDesc[0] = gSmmFeatureSmiStack;

  //
  // Copy template to CPU specific SMI handler location
  //
  if (mSmmProtectedModeEnable) {
    gSmmFeatureSmbase = SmBase;
    gProtModeSmbase = SmBase;
    CopyMem (
      (VOID*)((UINTN)SmBase + SMM_HANDLER_OFFSET),
      (VOID*)gcSmmFeatureSmiHandlerTemplate,
      gcSmmFeatureSmiHandlerSize
      );
    //
    // Prepare for the SMMSEG structure
    //
    SetupSmmProtectedModeContext (
      CpuIndex,
      (UINT32)SmBase,
      gSmmFeatureSmiStack,
      GdtBase,
      GdtSize,
      SMMSEG_PROTECT_MODE_CODE_SEGMENT,
      (IA32_DESCRIPTOR *)(UINTN)gProtModeIdtr
      );
  } else {
    gSmmFeatureSmbase = SmBase;
    CopyMem (
      (VOID*)((UINTN)SmBase + SMM_HANDLER_OFFSET),
      (VOID*)gcSmmFeatureSmiHandlerTemplate,
      gcSmmFeatureSmiHandlerSize
      );
  }
}

/**
  Check to see if an SMM register is supported by a specified CPU.

  @param[in] CpuIndex  The index of the CPU to check for SMM register support.
                       The value must be between 0 and the NumberOfCpus field
                       in the System Management System Table (SMST).
  @param[in] RegName   Identifies the SMM register to check for support.

  @retval TRUE   The SMM register specified by RegName is supported by the CPU
                 specified by CpuIndex.
  @retval FALSE  The SMM register specified by RegName is not supported by the
                 CPU specified by CpuIndex.
**/
BOOLEAN
EFIAPI
SmmCpuFeaturesIsSmmRegisterSupported (
  IN UINTN         CpuIndex,
  IN SMM_REG_NAME  RegName
  )
{
  switch (RegName) {
  case SmmRegFeatureControl:
    return mSmmFeatureControlSupported;
  case SmmRegSmmEnable:
    return mSmmSyncFeature[CpuIndex].TargetedSmiSupported;
  case SmmRegSmmDelayed:
    return mSmmSyncFeature[CpuIndex].DelayIndicationSupported;
  case SmmRegSmmBlocked:
    return mSmmSyncFeature[CpuIndex].BlockIndicationSupported;
  }
  return FALSE;
}

/**
  Returns the current value of the SMM register for the specified CPU.
  If the SMM register is not supported, then 0 is returned.

  @param[in] CpuIndex  The index of the CPU to read the SMM register.  The
                       value must be between 0 and the NumberOfCpus field in
                       the System Management System Table (SMST).
  @param[in] RegName   Identifies the SMM register to read.

  @return  The value of the SMM register specified by RegName from the CPU
           specified by CpuIndex.
**/
UINT64
EFIAPI
SmmCpuFeaturesGetSmmRegister (
  IN UINTN         CpuIndex,
  IN SMM_REG_NAME  RegName
  )
{
  if (SmmCpuFeaturesIsSmmRegisterSupported (CpuIndex, RegName)) {
    return SmmReadReg64 (CpuIndex, RegName);
  }
  return 0;
}

/**
  Sets the value of an SMM register on a specified CPU.
  If the SMM register is not supported, then no action is performed.

  @param[in] CpuIndex  The index of the CPU to write the SMM register.  The
                       value must be between 0 and the NumberOfCpus field in
                       the System Management System Table (SMST).
  @param[in] RegName   Identifies the SMM register to write.
                       registers are read-only.
  @param[in] Value     The value to write to the SMM register.
**/
VOID
EFIAPI
SmmCpuFeaturesSetSmmRegister (
  IN UINTN         CpuIndex,
  IN SMM_REG_NAME  RegName,
  IN UINT64        Value
  )
{
  if (SmmCpuFeaturesIsSmmRegisterSupported (CpuIndex, RegName)) {
    //
    // Only SmmRegFeatureControl in SMM_REG_NAME is Read-write register,
    // while others are Read-only register.
    //
    ASSERT (RegName == SmmRegFeatureControl);
    if (RegName == SmmRegFeatureControl) {
      SmmWriteReg64 (CpuIndex, RegName, Value);
    }
  }
}

/**
  This function is hook point called after the gEfiSmmReadyToLockProtocolGuid
  notification is completely processed.
**/
VOID
EFIAPI
SmmCpuFeaturesCompleteSmmReadyToLock (
  VOID
  )
{
  UINTN                                 Index;
  BOOLEAN                               CetEnabled;
  MSR_SMM_SUPOVR_STATE_LOCK_REGISTER    SupovrStateLock;

  DEBUG ((DEBUG_INFO, "SmmCpuFeaturesCompleteSmmReadyToLock...\n"));

  //
  // Disable write protection, because we need mark page table to be write protected.
  // We need *write* page table memory, to mark itself to be *read only*.
  //
  CetEnabled = ((AsmReadCr4 () & BIT23) != 0) ? TRUE : FALSE;
  if (CetEnabled) {
    //
    // CET must be disabled if WP is disabled.
    //
    DisableCet ();
  }

  AsmWriteCr0 (AsmReadCr0 () & ~CR0_WP);

  if (mSmmSupovrStateLock) {
    DEBUG ((DEBUG_INFO, "Patch SmmSupovrStateLock\n"));

    for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
      SupovrStateLock.Uint64 = 0;
      SupovrStateLock.Bits.PagingStates = 1;
      SupovrStateLock.Bits.Smbase = 1;
      SupovrStateLock.Bits.Ia32SmmMonitorCtlMsr = SmmSupovrStateLockPatch ();

#if FixedPcdGetBool (PcdSpaEnable) == 1
      if (mSmmSpsPpam11Enable) {
        if (IsSpaEnable ()) {
          SupovrStateLock.Bits.PagingStates = 0;
        }
      }
#endif

      if (mSmmSpsPpam11Enable) {
        DgrPatch1SupovrAndRing3Support (mSmBase, Index, SupovrStateLock.Uint64);
      } else {
        *(UINT64 *) (UINTN)(mSmBase[Index] + (UINTN)&gSmmSupovrStateLockData - (UINTN)gcSmmFeatureSmiHandlerTemplate + SMM_HANDLER_OFFSET) = SupovrStateLock.Uint64;
      }
    }
  }


  if (mSmmSpsPpam11Enable) {
    DgrSetPolicy (mSmBase);
  }

  //
  // Enable write protection, after page table updated.
  //
  AsmWriteCr0 (AsmReadCr0 () | CR0_WP);

  if (CetEnabled) {
    //
    // re-enable CET.
    //
    EnableCet ();
  }

  DEBUG ((DEBUG_INFO, "SmmCpuFeaturesCompleteSmmReadyToLock Done\n"));
}

/**
  This API provides a method for a CPU to allocate a specific region for storing page tables.

  This API can be called more than once to allocate the page table memory.

  Allocates the number of 4KB pages of type EfiRuntimeServicesData and returns a pointer to the
  allocated buffer.  The buffer returned is aligned on a 4KB boundary.  If Pages is 0, then NULL
  is returned.  If there is not enough memory remaining to satisfy the request, then NULL is
  returned.

  This function can also return NULL if there is no preference on where the page tables are allocated in SMRAM.

  @param  Pages                 The number of 4 KB pages to allocate.

  @return A pointer to the allocated buffer for page tables.
  @retval NULL      Fail to allocate a specific region for storing page tables,
                    Or there is no preference on where the page tables are allocated in SMRAM.

**/
VOID *
EFIAPI
SmmCpuFeaturesAllocatePageTableMemory (
  IN UINTN           Pages
  )
{
  if (Pages == 0) {
    return NULL;
  }

  return NULL;
}
