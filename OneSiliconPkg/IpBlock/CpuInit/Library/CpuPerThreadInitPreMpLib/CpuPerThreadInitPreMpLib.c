/** @file
  CPU Per Thread Init Pre MP Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <Base.h>
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/LocalApicLib.h>
#include <Library/TimerLib.h>
#include <Register/Cpuid.h>
//#include <Register/ArchMsr.h>
//#include <Register/AdlMsr.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include <Library/CpuPerThreadInitPreMpLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/CpuCommonLib.h>

#define MSR_FUSA_CONFIG                     0x000002D1
#define MSR_TRIGGER_STARTUP_SCAN_BIST       0x000002D2
#define MSR_STARTUP_SCAN_BIST_STATUS        0x000002D3
#define MSR_TRIGGER_STARTUP_MEM_BIST        0x000002D4
#define MSR_STARTUP_MEM_BIST_STATUS         0x000002D5
#define MSR_CONFIG_PERIODIC_SCAN_BIST       0x000002DA
#define MSR_TRIGGER_PERIODIC_SCAN_BIST      0x000002DB
#define MSR_PERIODIC_SCAN_BIST_STATUS       0x000002CA
#define MSR_FUSA_CAPABILITIES_A             0x000002D9

//#define MSR_ENABLE_LSM          0x000002D8

extern UINT8   gApInitPreMpTemplate[];
extern UINT16  gApInitPreMpTemplateSize;
extern UINT16  gLockstepEnableOffset;
extern UINT16  gApicIdMaskOffset;
extern UINT16  gX2ApicEnableOffset;
extern UINT16  gApCountOffset;

/**
  Detects if HWLS feature supported on current processor.

  @retval TRUE     HWLS feature is supported.
  @retval FALSE    HWLS feature is not supported.

**/
BOOLEAN
IsFusaCapSupported (
  VOID
  )
{
  MSR_CORE_CAPABILITIES_REGISTER                Msr;
  UINT64 FusaCapMsr;

  Msr.Uint64 = AsmReadMsr64 (MSR_CORE_CAPABILITIES);
  if (Msr.Bits.FusaSupported) {
    FusaCapMsr = AsmReadMsr64 (MSR_FUSA_CAPABILITIES_A);

    DEBUG ((DEBUG_INFO, "FuSA Capability %lx \n", FusaCapMsr ));
    // check if MSR is not 0
    if (FusaCapMsr) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Detect Fusa diagnostic mode
  The existence of gSiFusaInfoGuid HOB implies the mode is on

  @return TRUE/FALSE of Fusa Diagnostic Mode

**/
STATIC
BOOLEAN
IsInFusaDiagnosticMode (
  VOID
  )
{
  EFI_HOB_GUID_TYPE   *GuidHob;
  BOOLEAN             FusaDiagnosticMode;

  GuidHob = GetFirstGuidHob (&gSiFusaInfoGuid);
  if (GuidHob != NULL) {
    FusaDiagnosticMode = TRUE;
  }
  else
  {
    FusaDiagnosticMode = FALSE;
  }
  DEBUG ((DEBUG_ERROR, "CpuPerThreadInitPreMpLib.c IsInFusaDiagnosticMode %s\n", (FusaDiagnosticMode? "TRUE":"FALSE") ));
  return FusaDiagnosticMode;
}

/**
  Detect Fusa Lockstep mode executed
  The existence of gSiFusaLockstepGuid HOB implies the lockstep config is executed

  @return TRUE/FALSE of Fusa Lockstep Executed

**/
STATIC
UINT32
IsFusaLockstepConfigExecuted (
  VOID
  )
{
  EFI_HOB_GUID_TYPE   *GuidHob;
  UINT32              FusaLockstepMode = 0x0;
  VOID                *Hob;
  EFI_GUID            SiFusaLockstepGuid = {0xf72c8651, 0x87f8, 0x4e81, {0x8b, 0x51, 0xcf, 0x51, 0x95, 0xff, 0x3c, 0x95}};

  GuidHob = GetFirstGuidHob (&SiFusaLockstepGuid);
  if (GuidHob != NULL) {
    FusaLockstepMode = 0x1;
  }
  else
  {
    FusaLockstepMode = 0x0;
    Hob = BuildGuidDataHob (
      &gSiFusaLockstepGuid,
      &FusaLockstepMode,
      sizeof(UINT32)
    );
    ASSERT (Hob != NULL);
  }

  return FusaLockstepMode;
}

/**
  This function will do per thread init before MpInit, like setting Lockstep mode, enabling X2Apic mode and etc.
  This function must be called after gEfiPeiMemoryDiscoveredPpiGuid is installed and before MpInit.
**/
VOID
EFIAPI
CpuPerThreadInitPreMp (
  UINT16 ModuleLockstep
  )
{

  CPUID_NATIVE_MODEL_ID_AND_CORE_TYPE_EAX Eax;
  UINTN               BufferBelow1M;
  VOID                *BackupBuffer;

  BOOLEAN             LockstepCapable;
  UINT32              *LockstepEnable;
  UINT32              *ApicIdMask;
  UINT32              CurrentApicId;

  UINT32              *X2ApicEnable;
  UINT32              *ApCount;
  BOOLEAN             RunInitSipiSipi;

  UINT16              ModuleLockstepConfig = 0;
  BOOLEAN             InterruptState;


  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  LockstepCapable = IsFusaCapSupported();

  if (LockstepCapable == FALSE)
  {
    return;
  }

  if (IsInFusaDiagnosticMode () == TRUE)
  {
    DEBUG ((DEBUG_INFO, "Lockstep Not Supported in Fusa Diagnostic Mode.\n"));
    return;
  }

  if (IsFusaLockstepConfigExecuted() == TRUE)
  {
    DEBUG ((DEBUG_INFO, "Lockstep Config Executed.\n"));
    return;
  }

  ProgramXApic (TRUE);

  MicroSecondDelay (PcdGet32 (PcdCpuApInitTimeOutInMicroSeconds));
  //
  // Prepare buffer to hold asm code
  //
  BufferBelow1M = BASE_4KB;
  BackupBuffer  = AllocatePages (1);
  ASSERT (BackupBuffer != NULL);
  CopyMem (
    BackupBuffer,
    (VOID *)BufferBelow1M,
    EFI_PAGES_TO_SIZE (1)
    );
  ASSERT (gApInitPreMpTemplateSize < EFI_PAGES_TO_SIZE (1));
  CopyMem (
    (VOID *)BufferBelow1M,
    gApInitPreMpTemplate,
    gApInitPreMpTemplateSize
    );
  ApCount = (UINT32 *)(UINTN)(BufferBelow1M + (UINT32)gApCountOffset);
  RunInitSipiSipi = FALSE;

  LockstepEnable = (UINT32 *)(UINTN)(BufferBelow1M + (UINT32)gLockstepEnableOffset);
  ApicIdMask     = (UINT32 *)(UINTN)(BufferBelow1M + (UINT32)gApicIdMaskOffset);

  DEBUG ((DEBUG_INFO, "CpuHwlsMode = %x\n", ModuleLockstep));
  //DEBUG ((DEBUG_INFO, "LockstepCapable = %d\n", LockstepCapable));

  *ApicIdMask = 0x0;
  *ApCount = 0;

  if (LockstepCapable && (ModuleLockstep != 0)) {
    *LockstepEnable = 2;
    RunInitSipiSipi = TRUE;
    ModuleLockstepConfig = ModuleLockstep & 0x0f;
    switch (ModuleLockstepConfig) {
        case 0:
        //
        // Disable lock-step for all 4 cores in first module.
        //
        *ApicIdMask = 0b0000;
        break;
      case 1:
        //
        // Enable lock-step for all 4 cores in first module.
        //
        *ApicIdMask = 0b1111;
        *ApCount += 4;
        break;
      case 2:
        //
        // Enable lock-step for first 2 cores in first module.
        //
        *ApicIdMask = 0b0011;
        *ApCount += 2;
        break;
      case 3:
        //
        // Enable lock-step for second 2 cores in first module.
        //
        *ApicIdMask = 0b1100;
        *ApCount += 2;
        break;
      default:
        break;
    }

    ModuleLockstepConfig = (ModuleLockstep >> 4) & 0x0f;
    switch (ModuleLockstepConfig) {
      case 0:
        //
        // Disable lock-step for all 4 cores in first module.
        //
        break;
      case 1:
        //
        // Enable lock-step for all 4 cores in second module.
        //
        *ApicIdMask = 0b11110000 | *ApicIdMask;
        *ApCount += 4;
        break;
      case 2:
        //
        // Enable lock-step for first 2 cores in second module.
        //
        *ApicIdMask =  0b00110000 | *ApicIdMask;
        *ApCount += 2;
        break;
      case 3:
        //
        // Enable lock-step for second 2 cores in second module.
        //
        *ApicIdMask =  0b11000000 | *ApicIdMask;
        *ApCount += 2;
        break;
      default:
        break;
    }

    ModuleLockstepConfig = (ModuleLockstep >> 8) & 0x0f;
    switch (ModuleLockstepConfig) {
      case 0:
        //
        // Disable lock-step for all 4 cores in first module.
        //
        break;
      case 1:
        //
        // Enable lock-step for all 4 cores in second module.
        //
        *ApicIdMask = 0b111100000000 | *ApicIdMask;
        *ApCount += 4;
        break;
      case 2:
        //
        // Enable lock-step for first 2 cores in second module.
        //
        *ApicIdMask =  0b001100000000 | *ApicIdMask;
        *ApCount += 2;
        break;
      case 3:
        //
        // Enable lock-step for second 2 cores in second module.
        //
        *ApicIdMask =  0b110000000000 | *ApicIdMask;
        *ApCount += 2;
        break;
      default:
        break;
    }

    DEBUG ((DEBUG_INFO, "ApicIdMask = %x\n", *ApicIdMask));
    DEBUG ((DEBUG_INFO, "ApCount    = %x\n", *ApCount));
    X2ApicEnable = (UINT32 *)(UINTN)(BufferBelow1M + (UINT32)gX2ApicEnableOffset);
    *X2ApicEnable = 0;
    RunInitSipiSipi = TRUE;
  }

  AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax.Uint32, NULL, NULL, NULL);
  CurrentApicId = GetApicId();
  CurrentApicId = CurrentApicId & 0x0f;
  CurrentApicId = (CurrentApicId >> 1);
  DEBUG ((DEBUG_INFO, "Shifted CurrentApicId %x\n", CurrentApicId));

  if (RunInitSipiSipi) {
    DEBUG ((DEBUG_INFO, "Send INIT_SIPI_SIPI All Excluding Self\n"));

    InterruptState = SaveAndDisableInterrupts ();
    //SendInitSipiSipiAllExcludingSelf ((UINT32)BufferBelow1M);
    SendStartupIpiAllExcludingSelf ((UINT32) BufferBelow1M);
    MicroSecondDelay (100);

    if ((CurrentApicId & (*ApicIdMask)) != (0x0) && ((UINT8)Eax.Bits.CoreType == CPUID_CORE_TYPE_INTEL_ATOM)) {
      AsmWriteMsr64 (MSR_ENABLE_LSM, 1);
    } else {
      AsmWriteMsr64 (MSR_ENABLE_LSM, 0);
    }

    //
    // Delay for all AP finish.
    //
    MicroSecondDelay (PcdGet32 (PcdCpuApInitTimeOutInMicroSeconds));
    //
    // Sending all APs an INIT to place the AP in Wait-For-SIPI state.
    // This avoids AP comes out of HLT state due to any event other than
    // an INIT-SIPI-SIPI that the AP will enter an unexpected state
    //
    SendInitIpiAllExcludingSelf ();
    //
    // Delay for all AP going into Wait-For-SIPI state.
    //
    MicroSecondDelay (PcdGet32 (PcdCpuApInitTimeOutInMicroSeconds));
    SetInterruptState (InterruptState);
  }

  //
  // Restore memory and free buffer
  //
  CopyMem (
    (VOID *)BufferBelow1M,
    BackupBuffer,
    EFI_PAGES_TO_SIZE (1)
    );
  FreePages (BackupBuffer, 1);
}
