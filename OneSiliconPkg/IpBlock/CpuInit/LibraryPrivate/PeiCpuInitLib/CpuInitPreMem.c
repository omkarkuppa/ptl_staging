/** @file
  PEIM to initialize the cache and program for unlock processor

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuPowerOnConfigLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiCpuInitFruLib.h>
#include <Library/PeiServicesLib.h>
#include <IntelRcStatusCode.h>
#include <Library/CpuInitPreMem.h>
#include <Library/LocalApicLib.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/SecPlatformInformation.h>
#include <Register/GenerationMsr.h>
#include <Register/ArchitecturalMsr.h>
#include <Register/Cpuid.h>
#include <Library/HobLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/BootGuardLib.h>
#include <Library/CpuInfoFruLib.h>

#include <MemInfoHob.h>

#pragma pack(1)

///
/// Store BIST data for BSP.
///
typedef struct {
  UINT32 ApicId;    ///< APIC ID
  UINT32 Health;    ///< BIST result
} BIST_HOB_DATA;

#pragma pack()

/**
  Dump Related Register for debug purpose
**/
VOID
DumpRelatedRegister (
  VOID
  )
{
  UINT32         Eax;
  UINT32         Ebx;
  UINT32         Ecx;
  UINT32         Edx;
  UINT32         CpuidMaxInput;
  UINT32         Index;
  UINT32         Index1;

  AsmCpuid (CPUID_SIGNATURE, &CpuidMaxInput, NULL, NULL, NULL);
  DEBUG ((DEBUG_INFO, "Dump Cpu Register in BSP:\n"));
  for(Index = 0; Index <= CpuidMaxInput; Index++){
    AsmCpuidEx (Index, 0, &Eax, &Ebx, &Ecx, &Edx);
    DEBUG ((DEBUG_INFO, "CPUID(0x%x, 0): EAX=0x%08x, EBX=0x%08x, ECX=0x%08x, EDX=0x%08x\n", Index, Eax, Ebx, Ecx, Edx));
    if ((Index==7)|| (Index == 0xB)|| (Index == 0xD)|| (Index == 0xF)|| (Index == 0x10)
         || (Index == 0x12)|| (Index == 0x14)|| (Index == 0x15)
         || (Index == 0x17)|| (Index == 0x18)|| (Index == 0x1A)
         || (Index == 0x1B)|| (Index == 0x1C)|| (Index == 0x1D) || (Index == 0x1F)){
      for (Index1 = 1; Index1 < 8; Index1++) {
        AsmCpuidEx (Index, Index1, &Eax, &Ebx, &Ecx, &Edx);
        DEBUG ((DEBUG_INFO, "CPUID(0x%x, 0x%x): EAX=0x%08x, EBX=0x%08x, ECX=0x%08x, EDX=0x%08x\n", Index, Index1, Eax, Ebx, Ecx, Edx));
      }
    }
  }

  CpuidMaxInput = 0;
  AsmCpuid (0x80000000, &CpuidMaxInput, NULL, NULL, NULL);
  for(Index = 0x80000000; Index <= CpuidMaxInput; Index++){
    AsmCpuidEx (Index, 0, &Eax, &Ebx, &Ecx, &Edx);
    DEBUG ((DEBUG_INFO, "CPUID(0x%x, 0): EAX=0x%08x, EBX=0x%08x, ECX=0x%08x, EDX=0x%08x\n", Index, Eax, Ebx, Ecx, Edx));
  }

  UINT32 MsrList[]={
  MSR_PLATFORM_INFO, //0xCE
  MSR_FLEX_RATIO, //0x194
  MSR_PERF_STATUS, //0x198
  MSR_PERF_CTL, //0x199
  MSR_TURBO_RATIO_LIMIT, //0x1ad
  MSR_VR_CURRENT_CONFIG, //0x601
  MSR_PACKAGE_POWER_SKU_UNIT, //0x606
  MSR_PACKAGE_RAPL_LIMIT, //0x610
  MSR_PACKAGE_POWER_SKU, //0x614
  MSR_PL3_CONTROL, //0x615
  MSR_CONFIG_TDP_NOMINAL, //0x648
  MSR_CONFIG_TDP_LEVEL1, //0x649
  MSR_CONFIG_TDP_LEVEL2, //0x64A
  MSR_CONFIG_TDP_CONTROL, //0x64B
  MSR_TURBO_ACTIVATION_RATIO, //0x64C
  MSR_PLATFORM_POWER_LIMIT, // 0x65C
  };

  for(Index = 0; Index < ARRAY_SIZE(MsrList); Index++){
    DEBUG ((DEBUG_INFO, "ReadMsr(0x%x) =0x%016lx\n", MsrList[Index], AsmReadMsr64 (MsrList[Index])));
  }
}

/**
  Set up flags in CR4 for XMM instruction enabling
**/
VOID
XmmInit (
  VOID
  )
{
  CPUID_VERSION_INFO_EDX CpuidVersionInfoEdx;
  UINTN                  Cr0;
  UINTN                  Cr4;

  ///
  /// Read the CPUID information
  ///
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, &CpuidVersionInfoEdx.Uint32);

  ///
  /// Check whether SSE2 is supported
  ///
  if (CpuidVersionInfoEdx.Bits.SSE2 == 1) {
    ///
    /// Enable XMM
    ///
    Cr0 = AsmReadCr0 ();
    Cr0 |= BIT1;
    AsmWriteCr0 (Cr0);

    Cr4 = AsmReadCr4 ();
    Cr4 |= (UINTN) (BIT9 | BIT10);
    AsmWriteCr4 (Cr4);
  }
}

/**
  Check some CPU policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicy    The Si PreMem Policy protocol instance
**/
VOID
EFIAPI
CpuValidatePolicy (
  IN  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  )
{
  ASSERT (SiPreMemPolicy->TableHeader.Header.Revision == SI_PREMEM_POLICY_REVISION);
}

/**
  Check on the processor if Debug Interface is supported

  @retval TRUE     DebugInterface is supported.
  @retval FALSE    DebugInterface is not supported.
**/
BOOLEAN
IsDebugInterfaceSupported (
  VOID
  )
{
  BOOLEAN                Support;
  CPUID_VERSION_INFO_ECX CpuidVersionInfoEcx;

  Support = FALSE;

  ///
  /// Debug interface is supported if CPUID (EAX=1): ECX[11] = 1,
  ///
  AsmCpuid (
    CPUID_VERSION_INFO,
    NULL,
    NULL,
    &CpuidVersionInfoEcx.Uint32,
    NULL
    );

  if (CpuidVersionInfoEcx.Bits.SDBG == 1) {
    Support = TRUE;
  }

  return Support;
}

/**
  Enable/Disable Debug Interfaces in the processor.

  @param[in] Support  - To enable or disable Debug Interface feature.

  @retval VOID - No value to return
**/
VOID
EnableDisableDebugInterface (
  IN UINTN Support
  )
{
  MSR_IA32_DEBUG_INTERFACE_REGISTER  DebugInterfaceReg;

  ///
  /// Check if the processor supports debug interface
  ///
  DebugInterfaceReg.Uint64 = AsmReadMsr64 (MSR_IA32_DEBUG_INTERFACE);
  if (!(DebugInterfaceReg.Bits.Lock)) {
    if (Support) {
      ///
      /// Enable Debug Interface (MSR 0xC80.Bit0 = 1)
      ///
      DebugInterfaceReg.Bits.Enable = 1;
    } else {
      ///
      /// Disable Debug Interface (MSR 0xC80.Bit0 = 0)
      ///
      DebugInterfaceReg.Bits.Enable = 0;
    }
    AsmWriteMsr64 (MSR_IA32_DEBUG_INTERFACE, DebugInterfaceReg.Uint64);
  }
}

/**
  Lock Debug Interface in the processor.

  @param[in] Support  - To lock Debug Interface feature.

  @retval VOID - No value to return
**/
VOID
LockDebugInterface (
  )
{
  MSR_IA32_DEBUG_INTERFACE_REGISTER  DebugInterfaceReg;

  ///
  /// Check if the processor supports debug interface
  ///
  DebugInterfaceReg.Uint64 = AsmReadMsr64 (MSR_IA32_DEBUG_INTERFACE);
  if (!(DebugInterfaceReg.Bits.Lock)) {
    DebugInterfaceReg.Bits.Lock = 1;
    AsmWriteMsr64 (MSR_IA32_DEBUG_INTERFACE, DebugInterfaceReg.Uint64);
  }
}

/**
  This function performs basic initialization for CPU in PEI phase after Policy produced.
  Enable/Disable CPU Debug as per Setup and locks it.

  @param[in] SiPreMemPolicyPpi The Si Pre-Mem Policy PPI instance
**/
EFI_STATUS
EFIAPI
CpuOnPolicyInstalled (
  IN  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  )
{
  EFI_STATUS Status;
  CPU_INIT_PREMEM_CONFIG         *CpuInitPreMemConfig;
  CPU_SECURITY_PREMEM_CONFIG     *CpuSecurityPreMemConfig;

  DEBUG_CODE_BEGIN();
  DumpRelatedRegister();
  DEBUG_CODE_END ();

  DEBUG ((DEBUG_INFO, "CpuOnPolicyInstalled Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_INIT_START_PREMEM); //PostCode (0xC10)

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "XmmInit Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_XMM_INIT_START); //PostCode (0xC11)
  ///
  /// Init XMM support
  ///
  XmmInit ();

  IsValiduCodeEntry ();

  if (CpuSecurityPreMemConfig->SkipStopPbet != TRUE) {
    ///
    /// Disable PBET
    ///
    StopPbeTimer ();
  }

  if (!IsSimicsEnvironment () && !IsHfpgaEnvironment ()) {
    ///
    /// Init CPU Straps
    ///
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_CPUSTRAPS_INIT_PREMEM); //PostCode (0xC12)
    SetCpuStrapAndEarlyPowerOnConfig (SiPreMemPolicyPpi);
  }

  //
  // Enable/Disable CPU Debug MSR as per setup and Lock Before Enabling TME (TME is being activated in Memory Init - MrcDone function)
  // If TME is enabled and not in Debug mode, then CPU Debug must be disabled and Locked.
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_DEBUG_MSRSET); //PostCode (0xC16)
  DEBUG ((DEBUG_INFO, "Invoking LockEnableDisableCpuDebug \n"));
  if (IsDebugInterfaceSupported ()) {
    if (CpuInitPreMemConfig->DebugInterfaceEnable < 2) {
      EnableDisableDebugInterface (CpuInitPreMemConfig->DebugInterfaceEnable);
    }

    if (CpuInitPreMemConfig->DebugInterfaceLockEnable == 1) {
      LockDebugInterface ();
    }
  }

  //
  // Program Crashlog setting at PreMem phase
  //
  PeiCpuCrashLogMailboxEnable ((BOOLEAN)(CpuInitPreMemConfig->CrashLogEnable));

  DEBUG ((DEBUG_INFO, "CPU Pre-Mem Exit \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_INIT_DONE_PREMEM); //PostCode (0xC1F)

  return EFI_SUCCESS;
}

/**
  Detect if KeyLocker is supported.

  @retval TRUE -  Processor Support KeyLocker
  @retval FALSE - Processor doesnt support KeyLocker
**/
BOOLEAN
EFIAPI
IsKeyLockerSupported (
  VOID
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_ECX  CpuidStructuredExtendedFeatureEcx;
  //
  // Read the Extended Feature Flag for Keylocker support
  //
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, NULL, NULL, &CpuidStructuredExtendedFeatureEcx.Uint32, NULL);
  if (CpuidStructuredExtendedFeatureEcx.Uint32 & BIT23) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Retrieves the data structure associated with the GUIDed HOB of type gEfiSmmSmramMemoryGuid

  @retval NULL   A HOB of type gEfiSmmSmramMemoryGuid could not be found.
  @retval !NULL  A pointer to the GUID data from a HIB of type gEfiSmmSmramMemoryGuid

**/
EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *
GetSmramHobData (
  VOID
  )
{
  VOID  *GuidHob;

  //
  // Search SmramMemoryReserve HOB that describes SMRAM region
  //
  GuidHob = GetFirstGuidHob (&gEfiSmmSmramMemoryGuid);
  if (GuidHob == NULL) {
    return NULL;
  }
  return (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *)GET_GUID_HOB_DATA (GuidHob);
}

/**
  This routine will split SmramReserve hob to reserve 1 page for SMRAM content in S3 phase
  for R9 SMM core.

  @retval EFI_SUCCESS           The gEfiSmmSmramMemoryGuid is split successfully.
  @retval EFI_NOT_FOUND         The gEfiSmmSmramMemoryGuid is not found.

**/
EFI_STATUS
EFIAPI
SplitSmramReserveHob (
  VOID
  )
{
  EFI_HOB_GUID_TYPE                *GuidHob;
  EFI_PEI_HOB_POINTERS             Hob;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK   *DescriptorBlock;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK   *NewDescriptorBlock;
  UINTN                            BufferSize;
  UINTN                            SmramRanges;
  UINTN                            Index;
  UINTN                            SubIndex;

  //
  // Retrieve the GUID HOB data that contains the set of SMRAM descriptors
  //
  GuidHob = GetFirstGuidHob (&gEfiSmmSmramMemoryGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }

  DescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *)GET_GUID_HOB_DATA (GuidHob);

  //
  // Allocate one extra EFI_SMRAM_DESCRIPTOR to describe a page of SMRAM memory that contains a pointer
  // to the SMM Services Table that is required on the S3 resume path
  //
  SmramRanges = DescriptorBlock->NumberOfSmmReservedRegions;
  BufferSize = sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK) + (SmramRanges * sizeof (EFI_SMRAM_DESCRIPTOR));

  Hob.Raw = BuildGuidHob (
              &gEfiSmmSmramMemoryGuid,
              BufferSize
              );
  ASSERT (Hob.Raw);
  NewDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *)Hob.Raw;

  //
  // Copy old EFI_SMRAM_HOB_DESCRIPTOR_BLOCK to new allocated region
  //
  CopyMem ((VOID *)Hob.Raw, DescriptorBlock, BufferSize - sizeof(EFI_SMRAM_DESCRIPTOR));

  //
  // Increase the number of SMRAM descriptors by 1 to make room for the ALLOCATED descriptor of size EFI_PAGE_SIZE
  //
  NewDescriptorBlock->NumberOfSmmReservedRegions = (UINT32)(SmramRanges + 1);

  ASSERT (SmramRanges >= 1);
  //
  // Copy last entry to the end - we assume TSEG is last entry, which is same assumption as R8 CPU/SMM driver
  //
  CopyMem (&NewDescriptorBlock->Descriptor[SmramRanges], &NewDescriptorBlock->Descriptor[SmramRanges - 1], sizeof(EFI_SMRAM_DESCRIPTOR));

  //
  // Update the last but 1 entry in the array with a size of EFI_PAGE_SIZE and put into the ALLOCATED state
  //
  NewDescriptorBlock->Descriptor[SmramRanges - 1].PhysicalSize    = EFI_PAGE_SIZE;
  NewDescriptorBlock->Descriptor[SmramRanges - 1].RegionState    |= EFI_ALLOCATED;

  //
  // Reduce the size of the last SMRAM descriptor by EFI_PAGE_SIZE
  //
  NewDescriptorBlock->Descriptor[SmramRanges].PhysicalStart += EFI_PAGE_SIZE;
  NewDescriptorBlock->Descriptor[SmramRanges].CpuStart      += EFI_PAGE_SIZE;
  NewDescriptorBlock->Descriptor[SmramRanges].PhysicalSize  -= EFI_PAGE_SIZE;

  //
  // Now, we have created SmramReserve Hob for SmmAccess drive. But the issue is that, R8 SmmAccess will assume there is 2 SmramReserve region only.
  // Reporting 3 SmramReserve region will cause buffer overflow. Moreover, we would like to filter AB-SEG or H-SEG to avoid SMM cache-poisoning issue.
  // So we uses scan SmmReserve Hob to remove AB-SEG or H-SEG.
  //
  for (Index = 0; Index <= SmramRanges; Index++) {
    if (NewDescriptorBlock->Descriptor[Index].PhysicalSize == 0) {
      //
      // Skip zero entry
      //
      continue;
    }
    if (NewDescriptorBlock->Descriptor[Index].PhysicalStart < BASE_1MB) {
      //
      // Find AB-SEG or H-SEG
      // remove this region
      //
      for (SubIndex = Index; SubIndex < NewDescriptorBlock->NumberOfSmmReservedRegions - 1; SubIndex++) {
        CopyMem (&NewDescriptorBlock->Descriptor[SubIndex], &NewDescriptorBlock->Descriptor[SubIndex + 1], sizeof (EFI_SMRAM_DESCRIPTOR));
      }
      //
      // Zero last one
      //
      ZeroMem (&NewDescriptorBlock->Descriptor[SubIndex], sizeof(EFI_SMRAM_DESCRIPTOR));
      //
      // Decrease Number
      //
      NewDescriptorBlock->NumberOfSmmReservedRegions --;
      //
      // Decrease Index to let it test mew entry
      //
      Index --;
    }
  }

  //
  // Last step, we can scrub old one
  //
  ZeroMem (&GuidHob->Name, sizeof(GuidHob->Name));

  return EFI_SUCCESS;
}

/**
  This routine will create AcpiVariable hob to point the reserved smram in S3 phase
  for R9 SMM core.

  @retval EFI_SUCCESS           The variable hob with gEfiAcpiVariableGuid is created successfully.
  @retval EFI_NOT_FOUND         The gEfiSmmSmramMemoryGuid is not found.

**/
EFI_STATUS
CreateAcpiVariableHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS             Hob;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK   *DescriptorBlock;
  UINTN                            SmramRanges;

  //
  // Retrieve the GUID HOB data that contains the set of SMRAM descriptors
  //
  DescriptorBlock = GetSmramHobData ();
  if (DescriptorBlock == NULL) {
    return EFI_NOT_FOUND;
  }

  Hob.Raw = BuildGuidHob (
              &gEfiAcpiVariableGuid,
              sizeof (EFI_SMRAM_DESCRIPTOR)
              );
  ASSERT (Hob.Raw);

  //
  // It should be already patch, so just copy last but 1 region directly.
  //
  SmramRanges = DescriptorBlock->NumberOfSmmReservedRegions;
  ASSERT (SmramRanges >= 2);
  if (SmramRanges >= 2) {
    CopyMem ((VOID *)Hob.Raw, &DescriptorBlock->Descriptor[SmramRanges - 2], sizeof (EFI_SMRAM_DESCRIPTOR));
  }

  return EFI_SUCCESS;
}

/**
  Driver Entry for AcpiVariableHobOnSmramReservHob PEIM

  @param   FileHandle       Handle of the file being invoked.
  @param   PeiServices      Describes the list of possible PEI Services.

  @retval EFI_SUCCESS      Success create variable hob with gEfiAcpiVariableGuid and
                           split gEfiSmmSmramMemoryGuid.
  @retval EFI_NOT_FOUND    Can not get gEfiSmmSmramMemoryGuid hob

**/
EFI_STATUS
EFIAPI
CreateS3AcpiVariableHob (
  VOID
  )
{
  EFI_STATUS              Status;

  //
  // Split SmramReserve hob, which is required for R9 SMM Core for S3.
  //
  Status = SplitSmramReserveHob ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Create AcpiVariable hob, which is required for R9 SMM Core for S3.
  //
  Status = CreateAcpiVariableHob ();

  return Status;
}

/**
  Obtain Memory Init Config

  @param[in, out] CpuMemoryInitConfig - A pointer to CPU_MEMORY_INIT_CONFIG
**/
VOID
EFIAPI
CpuGetMemoryInitConfig (
  IN OUT CPU_MEMORY_INIT_CONFIG *CpuMemoryInitConfig
  )
{
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;
  CPU_INIT_PREMEM_CONFIG       *CpuInitPreMemConfig;
  CPU_SECURITY_PREMEM_CONFIG   *CpuSecurityPreMemConfig;
  EFI_STATUS                   Status;

  if (CpuMemoryInitConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "CpuMemoryInitConfig is NULL!\n"));
    ASSERT(FALSE);
    return;
  }

  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (SiPreMemPolicyPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "SiPreMemPolicyPpi is NULL!\n"));
    ASSERT(FALSE);
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (CpuInitPreMemConfig != NULL) {
    ///
    /// TME policy enable
    ///
    if ((CpuInitPreMemConfig->TmeEnable == 1) && (IsTmeSupported () == TRUE)) {
      CpuMemoryInitConfig->TmeEnable = 1;
    }
    ///
    /// Prmrr size is in MB units
    ///
    if (CpuSecurityPreMemConfig != NULL) {
      CpuMemoryInitConfig->PrmrrSize = CpuGetAndVerifyPrmrrSize ((BOOLEAN) CpuSecurityPreMemConfig->TdxEnable, CpuInitPreMemConfig->PrmrrSize) >> 20;
    }
    DEBUG ((DEBUG_INFO, "PrmrrSize is %d MB\n", CpuMemoryInitConfig->PrmrrSize));

    CpuMemoryInitConfig->TsegSize = GetPowerOfTwo32 (CpuInitPreMemConfig->TsegSize) >> 20;
  }

  if (CpuSecurityPreMemConfig != NULL) {
    CpuMemoryInitConfig->TmeExcludeBase    = CpuSecurityPreMemConfig->TmeExcludeBase;
    CpuMemoryInitConfig->TmeExcludeSize    = CpuSecurityPreMemConfig->TmeExcludeSize;
    CpuMemoryInitConfig->TdxEnable         = (BOOLEAN) CpuSecurityPreMemConfig->TdxEnable;
    CpuMemoryInitConfig->GenerateNewTmeKey = (UINT8) CpuSecurityPreMemConfig->GenerateNewTmeKey;
  }

  return;
}

/**
  Get PRMRR consumed memory size

  @param[in, out] CpuMemoryInitConfig - A pointer to CPU_MEMORY_INIT_CONFIG
 **/
VOID
PeiPrmrrMemoryAllocation(
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud
)
{
  UINT64 PrmrrBase;
  UINT32 PrmrrSize;

  MEMORY_PLATFORM_DATA_HOB     *MemInfoHob;

  MemInfoHob = (MEMORY_PLATFORM_DATA_HOB*)GetFirstGuidHob (&gSiMemoryPlatformDataGuid);
  if (MemInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "MemoryPlatformData hob not found, returning from PrmrrMemoryAllocation \n"));
    return;
  }

  PrmrrBase = LShiftU64 (MemInfoHob->Data.PrmrrBase, 20);
  PrmrrSize = MemInfoHob->Data.PrmrrSize << 20;
  if (PrmrrBase == *TopUseableMemAddr) {
    *TopUseableMemAddr += PrmrrSize;
    *Touud -= PrmrrSize;
  }
}
