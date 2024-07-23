/** @file
  Silicon Program MTRR PPI is used for initializing MTRR values
  after memory initialization complete.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include "SiInitPreMem.h"
#include <PiPei.h>
#include <CpuRegs.h>
#include <Register/Msr.h>
#include <Register/Cpuid.h>
#include <Register/TraceHubRegs.h>
#include <ConfigBlock.h>
#include <TraceHubConfig.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/MtrrLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <IndustryStandard/Pci.h>
#include <Guid/SmramMemoryReserve.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <SpiInfoHob.h>
#include <Library/FspCommonLib.h>
#include <Pi/PiStatusCode.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PerformanceLib.h>

/**
  Get system memory resource descriptor by owner.

  @param[in] OwnerGuid   resource owner guid
**/
EFI_HOB_RESOURCE_DESCRIPTOR *
EFIAPI
GetResourceDescriptorByOwner (
  IN EFI_GUID   *OwnerGuid
  )
{
  EFI_PEI_HOB_POINTERS    Hob;

  //
  // Get the HOB list for processing
  //
  Hob.Raw = GetHobList ();

  //
  // Collect memory ranges
  //
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED) && \
          (CompareGuid (&Hob.ResourceDescriptor->Owner, OwnerGuid))) {
        return  Hob.ResourceDescriptor;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  return NULL;
}

/**
  Get system memory from HOB.

  @param[in,out] LowMemoryLength   less than 4G memory length
  @param[in,out] HighMemoryLength  greater than 4G memory length
**/
VOID
GetSystemMemorySize (
  IN OUT UINT64              *LowMemoryLength,
  IN OUT UINT64              *HighMemoryLength
  )
{
  EFI_STATUS                  Status;
  EFI_BOOT_MODE               BootMode;
  EFI_RESOURCE_ATTRIBUTE_TYPE ResourceAttribute;
  EFI_PEI_HOB_POINTERS        Hob;
  CONST EFI_PEI_SERVICES      **PeiServices;

  ResourceAttribute = (
                       EFI_RESOURCE_ATTRIBUTE_PRESENT |
                       EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                       EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                       );

  PeiServices = GetPeiServicesTablePointer ();
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode != BOOT_ON_S3_RESUME) {
    ResourceAttribute |= EFI_RESOURCE_ATTRIBUTE_TESTED;
  }

  *HighMemoryLength = 0;
  *LowMemoryLength  = SIZE_1MB;
  //
  // Get the HOB list for processing
  //
  Hob.Raw = GetHobList ();

  //
  // Collect memory ranges
  //
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) ||
          ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED) &&
           (Hob.ResourceDescriptor->ResourceAttribute == ResourceAttribute))) {
        //
        // Need memory above 1MB to be collected here
        //
        if (Hob.ResourceDescriptor->PhysicalStart >= BASE_1MB &&
            Hob.ResourceDescriptor->PhysicalStart < (EFI_PHYSICAL_ADDRESS) BASE_4GB) {
          *LowMemoryLength += (UINT64) (Hob.ResourceDescriptor->ResourceLength);
        } else if (Hob.ResourceDescriptor->PhysicalStart >= (EFI_PHYSICAL_ADDRESS) BASE_4GB) {
          *HighMemoryLength += (UINT64) (Hob.ResourceDescriptor->ResourceLength);
        }
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
}

UINT8
CalculatingMtrrReservedNumber (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT8                           ReservedNum;
  CONST EFI_PEI_SERVICES          **PeiServices;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;
  TRACE_HUB_PREMEM_CONFIG         *TraceHubPreMemConfig;

  ReservedNum = 0;
  TraceHubPreMemConfig = NULL;

  PeiServices = GetPeiServicesTablePointer ();
  //
  // Reserve variable MTRRs for TraceHub and Processor Trace (RTIT) if required
  //
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gSiPreMemPolicyPpiGuid,
                             0,
                             NULL,
                             (VOID **) &SiPreMemPolicyPpi
                             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode != TraceHubModeDisabled) {
    ReservedNum += 2;  // one for MSC (UC) and another is for RTIT (USWC)
  }

  return ReservedNum;
}

/**
  This function programs Cache Attributes.
**/
VOID
ProgramCacheAttributes (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINTN                       Index;
  UINT64                      LowMemoryLength;
  UINT64                      HighMemoryLength;
  UINT64                      TopHighMemory;
  UINT64                      MaxLowMemoryLength;
  UINT32                      VariableMtrrCount;
  UINT8                       MtrrReserved;
  MTRR_SETTINGS               MtrrSetting;
  EFI_PHYSICAL_ADDRESS        TsegBaseAddress;
  EFI_HOB_RESOURCE_DESCRIPTOR *Descriptor;
  VOID                        *SpiInfoHob;
  SPI_INFO_HOB                *SpiInfoHobData;
  BOOLEAN                     ExtendedBiosSupport = FALSE;

  TsegBaseAddress = 0;
  Descriptor = NULL;
  DEBUG ((DEBUG_INFO, "Program Cache Attributes\n"));

  SpiInfoHob = GetFirstGuidHob (&gSpiInfoHobGuid);

  if (SpiInfoHob != NULL) {
    SpiInfoHobData =  GET_GUID_HOB_DATA (SpiInfoHob);
    ExtendedBiosSupport = SpiInfoHobData->ExtendedBiosDecodeRangeEnable;
  } else {
    DEBUG ((DEBUG_INFO, "SPI Info Hob not found\n"));
  }
  ///
  /// Reset all MTRR setting.
  ///
  ZeroMem (&MtrrSetting, sizeof (MTRR_SETTINGS));

  ///
  /// Cache the Flash area as WP to boost performance
  /// Set WP cache size to fixed at 16MB for saving MTRR usage.
  ///
  PERF_INMODULE_BEGIN ("MtrrSetAttrFlashArea");
  Status = MtrrSetMemoryAttributeInMtrrSettings (
             &MtrrSetting,
             0xff000000,
             0x01000000,
             CacheWriteProtected
             );
  PERF_INMODULE_END ("MtrrSetAttrFlashArea");
  if (EFI_ERROR (Status)) {
    REPORT_STATUS_CODE_EX (
      (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
      (EFI_COMPUTING_UNIT_HOST_PROCESSOR|EFI_CU_HP_EC_CACHE),
      0,
      &gCpuFspErrorTypeCallerId,
      NULL,
      NULL,
      0
      );
  }
  ASSERT_EFI_ERROR (Status);

  if (ExtendedBiosSupport) {
    //
    // Cache the Extended BIOS area as WP to boost performance
    //
    DEBUG ((DEBUG_INFO, "Extended BIOS Support has been Enabled, Set Extended BIOS Region to WP to boot performace\n"));
    PERF_INMODULE_BEGIN ("MtrrSetAttrExtBios");
    Status = MtrrSetMemoryAttributeInMtrrSettings (
              &MtrrSetting,
              (PHYSICAL_ADDRESS) FixedPcdGet32 (PcdSiExtendedBiosRegionBaseAddress),
              (UINT64) FixedPcdGet32 (PcdSiExtendedBiosRegionSize),
              CacheWriteProtected
              );
    PERF_INMODULE_END ("MtrrSetAttrExtBios");
    if (EFI_ERROR (Status)) {
      REPORT_STATUS_CODE_EX (
        (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
        (EFI_COMPUTING_UNIT_HOST_PROCESSOR|EFI_CU_HP_EC_CACHE),
        0,
        &gCpuFspErrorTypeCallerId,
        NULL,
        NULL,
        0
        );
    }
    ASSERT_EFI_ERROR (Status);
  }

  MtrrSetAllMtrrs (&MtrrSetting);

  //
  // Get system memory from HOB
  //
  GetSystemMemorySize (&LowMemoryLength, &HighMemoryLength);
  DEBUG ((DEBUG_INFO, "Memory Length (Below 4GB) = %lx.\n", LowMemoryLength));
  DEBUG ((DEBUG_INFO, "Memory Length (Above 4GB) = %lx.\n", HighMemoryLength));

  MaxLowMemoryLength = (UINT32) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);

  //
  // Set 0 - Tolud as WB.
  // Set TSEG - Tolud later to avoid MTRR hole.
  //
  PERF_INMODULE_BEGIN ("MtrrSetAttrTolud");
  Status = MtrrSetMemoryAttributeInMtrrSettings (
              &MtrrSetting,
              0,
              MaxLowMemoryLength,
              CacheWriteBack
              );
  PERF_INMODULE_END ("MtrrSetAttrTolud");
  if (EFI_ERROR (Status)) {
    REPORT_STATUS_CODE_EX (
      (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
      (EFI_COMPUTING_UNIT_HOST_PROCESSOR|EFI_CU_HP_EC_CACHE),
      0,
      &gCpuFspErrorTypeCallerId,
      NULL,
      NULL,
      0
      );
  }
  ASSERT_EFI_ERROR (Status);
//
// If VGA is used then that range must be mapped as UC.
// otherwise encounters MC exception.
//
  PERF_INMODULE_BEGIN ("MtrrSetAttrVga");
  Status = MtrrSetMemoryAttributeInMtrrSettings (
              &MtrrSetting,
              0xA0000,
              0x20000,
              CacheUncacheable
              );
  PERF_INMODULE_END ("MtrrSetAttrVga");
  if (EFI_ERROR (Status)) {
    REPORT_STATUS_CODE_EX (
      (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
      (EFI_COMPUTING_UNIT_HOST_PROCESSOR|EFI_CU_HP_EC_CACHE),
      0,
      &gCpuFspErrorTypeCallerId,
      NULL,
      NULL,
      0
      );
  }
  ASSERT_EFI_ERROR (Status);

  Descriptor = GetResourceDescriptorByOwner (&gFspReservedMemoryResourceHobTsegGuid);
  if (Descriptor != NULL) {
    TsegBaseAddress = Descriptor->PhysicalStart;
  }

  //
  // Set TSEG - TOLUD to UC to reduce MTRR register usage
  //
  if (TsegBaseAddress != 0) {
    PERF_INMODULE_BEGIN ("MtrrSetAttrTseg");
    Status = MtrrSetMemoryAttributeInMtrrSettings (
                &MtrrSetting,
                TsegBaseAddress,
                MaxLowMemoryLength - TsegBaseAddress,
                CacheUncacheable
                );
    PERF_INMODULE_END ("MtrrSetAttrTseg");
    if (EFI_ERROR (Status)) {
      REPORT_STATUS_CODE_EX (
        (EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED),
        (EFI_COMPUTING_UNIT_HOST_PROCESSOR|EFI_CU_HP_EC_CACHE),
        0,
        &gCpuFspErrorTypeCallerId,
        NULL,
        NULL,
        0
        );
    }
    ASSERT_EFI_ERROR (Status);
  }

  if (HighMemoryLength) {
    //
    // Enlarge above 4GB memory size to be power of two aligned.
    // Note: above 4GB MMIO resource region should not overlap this memory address
    //
    TopHighMemory = GetPowerOfTwo64 (HighMemoryLength + SIZE_4GB);
    if (TopHighMemory != (HighMemoryLength + SIZE_4GB)) {
      TopHighMemory = LShiftU64 (TopHighMemory, 1);
    }

    do {
      PERF_INMODULE_BEGIN ("MtrrSetAttr4Gb");
      Status = MtrrSetMemoryAttributeInMtrrSettings (
                 &MtrrSetting,
                 SIZE_4GB,
                 TopHighMemory - SIZE_4GB,
                 CacheWriteBack
                 );
      PERF_INMODULE_END ("MtrrSetAttr4Gb");
      if (TopHighMemory > SIZE_4GB) {
        TopHighMemory = RShiftU64 (TopHighMemory, 1);
      }
    } while ((EFI_SUCCESS != Status) && (TopHighMemory > SIZE_4GB));
  }

  //
  // When key features needed we will free some MTRR for them later and sacrifice some of above 4GB coverage.
  // In later phase boot loader code can re-configure MTRR to exclude flash region and get back above 4GB coverage.
  //
  VariableMtrrCount = GetVariableMtrrCount ();
  MtrrReserved = CalculatingMtrrReservedNumber ();
  for (Index = VariableMtrrCount - 1; MtrrReserved > 0; Index--, MtrrReserved--) {
    MtrrSetting.Variables.Mtrr[Index].Base = 0;
    MtrrSetting.Variables.Mtrr[Index].Mask = 0;
  }

  ///
  /// Update MTRR setting from MTRR buffer
  ///
  MtrrSetAllMtrrs (&MtrrSetting);
}

/**
  This function prints MTRRs.
**/
VOID
PrintMtrrs (
  VOID
  )
{
  DEBUG_CODE_BEGIN();

  typedef struct {
    char    *Desc;
    UINT32  Msr;
  } MTRR_INFO;

  MSR_IA32_MTRRCAP_REGISTER Msr;
  UINT32                    Index;
  UINT32                    Mtrr;

  static MTRR_INFO MtrrInfo[] = {
    {"MSR_IA32_MTRR_FIX64K_00000",  MSR_IA32_MTRR_FIX64K_00000},
    {"MSR_IA32_MTRR_FIX16K_80000",  MSR_IA32_MTRR_FIX16K_80000},
    {"MSR_IA32_MTRR_FIX16K_A0000",  MSR_IA32_MTRR_FIX16K_A0000},
    {"MSR_IA32_MTRR_FIX4K_C0000 ",  MSR_IA32_MTRR_FIX4K_C0000},
    {"MSR_IA32_MTRR_FIX4K_C8000 ",  MSR_IA32_MTRR_FIX4K_C8000},
    {"MSR_IA32_MTRR_FIX4K_D0000 ",  MSR_IA32_MTRR_FIX4K_D0000},
    {"MSR_IA32_MTRR_FIX4K_D8000 ",  MSR_IA32_MTRR_FIX4K_D8000},
    {"MSR_IA32_MTRR_FIX4K_E0000 ",  MSR_IA32_MTRR_FIX4K_E0000},
    {"MSR_IA32_MTRR_FIX4K_E8000 ",  MSR_IA32_MTRR_FIX4K_E8000},
    {"MSR_IA32_MTRR_FIX4K_F0000 ",  MSR_IA32_MTRR_FIX4K_F0000},
    {"MSR_IA32_MTRR_FIX4K_F8000 ",  MSR_IA32_MTRR_FIX4K_F8000}
  };

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "MTRR programming:\n"));

  for (Index = 0; Index < sizeof(MtrrInfo) / sizeof(MTRR_INFO); ++Index) {
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "%a Msr %x = %016llx\n", MtrrInfo[Index].Desc, MtrrInfo[Index].Msr, AsmReadMsr64(MtrrInfo[Index].Msr)));
  }

  Msr.Uint64 = AsmReadMsr64(MSR_IA32_MTRRCAP);
  for (Index = 0; Index < Msr.Bits.VCNT; ++Index) {
    Mtrr = MSR_IA32_MTRR_PHYSBASE0 + (2 * Index);
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "MSR_IA32_MTRR_PHYSBASE%d Msr %x = %016llx\n",
        Index, Mtrr, AsmReadMsr64(Mtrr)
    ));

    DEBUG ((DEBUG_INFO | DEBUG_INIT, "MSR_IA32_MTRR_PHYSMASK%d Msr %x = %016llx\n",
        Index, Mtrr + 1, AsmReadMsr64(Mtrr + 1)
    ));
  }
  DEBUG_CODE_END();
}

/**
  Program MTRR values and print MTRRs.
**/
EFI_STATUS
EFIAPI
TempRamExit (
  IN  VOID    *TempRamExitParamPtr
  )
{
  PERF_INMODULE_BEGIN ("TempRamExit");
  //
  // Disable CAR
  //
  PERF_INMODULE_BEGIN ("DisabelCacheAsRam");
  DisableCacheAsRam (TRUE);
  PERF_INMODULE_END ("DisabelCacheAsRam");
  //
  // Program MTRR values.
  //
  PERF_INMODULE_BEGIN ("ProgramCacheAttr");
  ProgramCacheAttributes ();
  PERF_INMODULE_END ("ProgramCacheAttr");
  //
  // Print MTRR values.
  //
  PrintMtrrs ();
  PERF_INMODULE_END ("TempRamExit");
  return EFI_SUCCESS;
}

