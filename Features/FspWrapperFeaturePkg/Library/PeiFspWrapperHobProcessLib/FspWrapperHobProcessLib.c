/** @file
  Provide FSP wrapper hob process related function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <PiPei.h>

#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/FspWrapperPlatformLib.h>
#include <Library/FspCommonLib.h>
#include <Library/FspErrorInfoLib.h>
#include <Guid/GuidHobFspEas.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/GraphicsInfoHob.h>
#include <Guid/PcdDataBaseHobGuid.h>
#include <Guid/ZeroGuid.h>
#include <Ppi/Capsule.h>

#include <FspEas.h>
#include <MemInfoHob.h>
#include <SmbiosCacheInfoHob.h>
#include <SmbiosProcessorInfoHob.h>
#include <SiConfigHob.h>
#include <Ppi/SiPolicy.h>
#include <CpuRegs.h>
#include <Register/GenerationMsr.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Guid/MigratedFvInfo.h>

#if FixedPcdGetBool(PcdFusaSupport) == 1
#include <FusaInfoHob.h>
#include <Library/FusaInfoLib.h>
#endif
//
// Additional pages are used by DXE memory manager.
// It should be consistent between RetrieveRequiredMemorySize() and GetPeiMemSize()
//
#define PEI_ADDITIONAL_MEMORY_SIZE    (16 * EFI_PAGE_SIZE)

/**
  Get the mem size in memory type infromation table.

  @param[in] PeiServices  PEI Services table.

  @return the mem size in memory type infromation table.
**/
UINT64
GetMemorySizeInMemoryTypeInformation (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_HOB_POINTERS        Hob;
  EFI_MEMORY_TYPE_INFORMATION *MemoryData;
  UINT8                       Index;
  UINTN                       TempPageNum;

  MemoryData = NULL;
  Status     = (*PeiServices)->GetHobList ((CONST EFI_PEI_SERVICES**)PeiServices, (VOID **) &Hob.Raw);
  ASSERT_EFI_ERROR (Status);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
      CompareGuid (&Hob.Guid->Name, &gEfiMemoryTypeInformationGuid)) {
      MemoryData = (EFI_MEMORY_TYPE_INFORMATION *) (Hob.Raw + sizeof (EFI_HOB_GENERIC_HEADER) + sizeof (EFI_GUID));
      break;
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  if (MemoryData == NULL) {
    return 0;
  }

  TempPageNum = 0;
  for (Index = 0; MemoryData[Index].Type != EfiMaxMemoryType; Index++) {
    //
    // Accumulate default memory size requirements
    //
    TempPageNum += MemoryData[Index].NumberOfPages;
  }

  return (UINT64) TempPageNum * EFI_PAGE_SIZE;
}

/**
  Get the mem size need to be reserved in PEI phase.

  @param[in] PeiServices  PEI Services table.

  @return the mem size need to be reserved in PEI phase.
**/
UINT64
RetrieveRequiredMemorySize (
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  UINT64                      Size;

  Size = GetMemorySizeInMemoryTypeInformation (PeiServices);
  return Size + PEI_ADDITIONAL_MEMORY_SIZE;
}

/**
  Get the mem size need to be consumed and reserved in PEI phase.

  @param[in] PeiServices  PEI Services table.
  @param[in] BootMode     Current boot mode.

  @return the mem size need to be consumed and reserved in PEI phase.
**/
UINT64
GetPeiMemSize (
  IN EFI_PEI_SERVICES **PeiServices,
  IN UINT32           BootMode
  )
{
  UINT64                      Size;
  UINT64                      MinSize;

  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    return PcdGet32 (PcdPeiRecoveryMinMemSize);
  }

  Size = GetMemorySizeInMemoryTypeInformation (PeiServices);

  if (BootMode == BOOT_ON_FLASH_UPDATE) {
    MinSize = PcdGet32 (PcdPeiMinMemSize) + FixedPcdGet32 (PcdFspWrapperCapsuleImageMaxSize);
  } else {
    MinSize = PcdGet32 (PcdPeiMinMemSize);
  }

  return MinSize + Size + PEI_ADDITIONAL_MEMORY_SIZE;
}

/**
  Post FSP-M HOB process for Memory Resource Descriptor.

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

  @return If platform process the FSP hob list successfully.
**/
EFI_STATUS
EFIAPI
PostFspmHobProcess (
  IN VOID                 *FspHobList
  )
{
  EFI_PEI_HOB_POINTERS                        Hob;
  UINT64                                      PeiMemSize;
  EFI_PHYSICAL_ADDRESS                        PeiMemBase;
  UINT64                                      S3PeiMemSize;
  EFI_PHYSICAL_ADDRESS                        S3PeiMemBase;
  EFI_STATUS                                  Status;
  EFI_BOOT_MODE                               BootMode;
  EFI_PEI_CAPSULE_PPI                         *Capsule;
  VOID                                        *CapsuleBuffer;
  UINTN                                       CapsuleBufferLength;
  UINT64                                      RequiredMemSize;
  UINT64                                      ResourceLength;
  EFI_PEI_SERVICES                            **PeiServices;
  UINT32                                      ProcessorTraceTotalMemSize;
  MSR_CORE_THREAD_COUNT_REGISTER              MsrCoreThreadCount;
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX Ebx;
  EFI_HOB_GUID_TYPE                           *GuidHob;
  EDKII_MIGRATED_FV_INFO                      *MigratedFvInfo;

  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();

  PeiServicesGetBootMode (&BootMode);

  PeiMemBase      = 0;
  PeiMemSize      = 0;
  RequiredMemSize = 0;
  ResourceLength  = 0;
  ProcessorTraceTotalMemSize = 0;

  //
  // Dump FSP error information HOB.
  //
  Status = DumpFspErrorInfo (FspHobList);

  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, NULL, &Ebx.Uint32, NULL, NULL);

  if (Ebx.Bits.IntelProcessorTrace == 1) {
    ///
    /// Get the total number of enabled processor threads.
    /// Read MSR 0x35 to get the max enabled number of cores/threads.
    ///
    MsrCoreThreadCount.Uint64 = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);

    ProcessorTraceTotalMemSize = PcdGet32 (PcdCpuProcTraceMemSize);
    if (ProcessorTraceTotalMemSize > RtitTopaMemorySize128M) {
        ProcessorTraceTotalMemSize = 0;
    } else {
      ProcessorTraceTotalMemSize = BIT0 << (ProcessorTraceTotalMemSize + 12);
      //
      // ProcessorTraceTotalMemSize is mutiplied by 2 for adding aligned memory (overhead) for natural alignment.
      // please refer to InternalAllocateAlignedPages (), the overhead will be free when creating memory map, it won't waste
      //
      ProcessorTraceTotalMemSize = 2 * ProcessorTraceTotalMemSize * (UINT32) MsrCoreThreadCount.Bits.Threadcount;
    }
  }

  //
  // Parse the hob list from fsp
  // Report all the resource hob except MMIO and IO resource Hob's
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    PeiMemSize = GetPeiMemSize (PeiServices, BootMode) + ProcessorTraceTotalMemSize;
    RequiredMemSize = RetrieveRequiredMemorySize (PeiServices);
    Hob.Raw = (UINT8 *)(UINTN)FspHobList;
    DEBUG((DEBUG_INFO, "FspHobList - 0x%x\n", FspHobList));
    //
    // Find the largest available system Memory and use it for PeiMemory
    //
    while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)
        && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
        && (Hob.ResourceDescriptor->PhysicalStart >= PeiMemBase)
        && (Hob.ResourceDescriptor->ResourceLength >= PeiMemSize)) {
        PeiMemBase = Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength - PeiMemSize;
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
    }
  }

  Hob.Raw = (UINT8 *)(UINTN)FspHobList;

  //
  // Skip the MMIO and IO reource map from the FSP Hob list
  //
  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
    if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_IO) || (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_MAPPED_IO)) {
      Hob.Raw = GET_NEXT_HOB (Hob);
      continue;
    }
    ResourceLength = Hob.ResourceDescriptor->ResourceLength;
    DEBUG((DEBUG_INFO, "Resource start %lx resource length %lx resource type %d\n",Hob.ResourceDescriptor->PhysicalStart,Hob.ResourceDescriptor->ResourceLength,Hob.ResourceDescriptor->ResourceType));
    if (BootMode != BOOT_ON_S3_RESUME) {
      //
      // If the system memory found in FSP Hob is determined for PeiMemory. Split the Resource descriptor Hob
      //
      if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)
        && (Hob.ResourceDescriptor->PhysicalStart <= PeiMemBase)
        && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength >= PeiMemBase + PeiMemSize)
        && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)) {
        if ((CompareGuid (&Hob.ResourceDescriptor->Owner, &gZeroGuid))) {
          BuildResourceDescriptorHob (
            Hob.ResourceDescriptor->ResourceType,
            Hob.ResourceDescriptor->ResourceAttribute,
            PeiMemBase,
            PeiMemSize
            );
        } else {
          BuildResourceDescriptorWithOwnerHob (
            Hob.ResourceDescriptor->ResourceType,
            Hob.ResourceDescriptor->ResourceAttribute,
            PeiMemBase,
            PeiMemSize,
            &Hob.ResourceDescriptor->Owner
            );
        }
        ResourceLength = (Hob.ResourceDescriptor->ResourceLength) - (PeiMemSize);
      }
    }

    //
    // Report the resource hob
    //
    if ((CompareGuid (&Hob.ResourceDescriptor->Owner, &gZeroGuid))) {
      BuildResourceDescriptorHob (
        Hob.ResourceDescriptor->ResourceType,
        Hob.ResourceDescriptor->ResourceAttribute,
        Hob.ResourceDescriptor->PhysicalStart,
        ResourceLength
        );
    } else {
      BuildResourceDescriptorWithOwnerHob (
        Hob.ResourceDescriptor->ResourceType,
        Hob.ResourceDescriptor->ResourceAttribute,
        Hob.ResourceDescriptor->PhysicalStart,
        ResourceLength,
        &Hob.ResourceDescriptor->Owner
        );
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  //
  // @todo: It is a W/A for SetMemorySpaceAttribute issue in PchSpi and PchReset drivers in FVMAIN of SSPT.
  //        We need to modify it instead of hard code here. Due to InstallEfiMemory is using hard code to
  //        describe memory resource, we have to hard code in here. Once InstallEfiMemory is merged, we should
  //        be able to remove this.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE,
    0xFE000000,
    0x10000
    );
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE,
    0xFE010000,
    0x1000
    );

  if (BootMode == BOOT_ON_S3_RESUME) {
    S3PeiMemBase = 0;
    S3PeiMemSize = 0;
    Status = GetS3MemoryInfo (&S3PeiMemSize, &S3PeiMemBase);
    ASSERT_EFI_ERROR (Status);
    DEBUG((DEBUG_INFO, "S3 memory %016XLh - %016LXh bytes\n", S3PeiMemBase, S3PeiMemSize));

    //
    // Make sure Stack and PeiMemory are not overlap
    //
    Status = PeiServicesInstallPeiMemory (
               S3PeiMemBase,
               S3PeiMemSize
               );
    ASSERT_EFI_ERROR (Status);
  } else {

    //
    // Capsule mode
    //
    Capsule = NULL;
    CapsuleBuffer = NULL;
    CapsuleBufferLength = 0;
    if (BootMode == BOOT_ON_FLASH_UPDATE) {
      Status = PeiServicesLocatePpi (
                 &gEfiPeiCapsulePpiGuid,
                 0,
                 NULL,
                 (VOID **) &Capsule
                 );
      ASSERT_EFI_ERROR (Status);

      if (Status == EFI_SUCCESS) {
        Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
        ASSERT_EFI_ERROR (Status);
        while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
          if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY)
            && (Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength <= BASE_4GB)
            && (Hob.ResourceDescriptor->PhysicalStart >= BASE_1MB)
            && (Hob.ResourceDescriptor->PhysicalStart != PeiMemBase)
            && (Hob.ResourceDescriptor->ResourceLength >= CapsuleBufferLength)) {
            CapsuleBufferLength = (UINTN)Hob.ResourceDescriptor->ResourceLength;
            CapsuleBuffer = (VOID*)(UINTN)Hob.ResourceDescriptor->PhysicalStart;
          }
          Hob.Raw = GET_NEXT_HOB (Hob);
        }

        //
        // Call the Capsule PPI Coalesce function to coalesce the capsule data.
        //
        Status = Capsule->Coalesce (PeiServices, &CapsuleBuffer, &CapsuleBufferLength);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Failed to coalesce capsule data!!\n"));
          ASSERT_EFI_ERROR (Status);
        }
      }
    }

    Hob.Raw = (UINT8 *)(UINTN)FspHobList;
    GuidHob = GetNextGuidHob (&gMeDidSentHobGuid, Hob.Raw);
    if (GuidHob != NULL) {
      BuildGuidDataHob (&gMeDidSentHobGuid, GET_GUID_HOB_DATA (Hob), GET_GUID_HOB_DATA_SIZE (Hob));
    }

    DEBUG ((DEBUG_INFO, "FSP wrapper PeiMemBase      : 0x%08x\n", PeiMemBase));
    DEBUG ((DEBUG_INFO, "FSP wrapper PeiMemSize      : 0x%08x\n", PeiMemSize));
    DEBUG ((DEBUG_INFO, "FSP wrapper RequiredMemSize : 0x%08x\n", RequiredMemSize));

    //
    // Install efi memory
    //
    Status = PeiServicesInstallPeiMemory (
               PeiMemBase,
               PeiMemSize - RequiredMemSize
               );
    ASSERT_EFI_ERROR (Status);

    if (Capsule != NULL) {
      Status = Capsule->CreateState ((EFI_PEI_SERVICES **)PeiServices, CapsuleBuffer, CapsuleBufferLength);
    }
  }

  //
  // Update PcdFspmUpdDataAddress64 accordingly after FSP migrates FspmUpdData
  //
  DEBUG ((DEBUG_INFO, "FspWrapper: Update FSP-M UPD address from %x ", PcdGet64 (PcdFspmUpdDataAddress64)));
  PcdSet64S (PcdFspmUpdDataAddress64, (UINTN) GetFspMemoryInitUpdDataPointer ());
  DEBUG ((DEBUG_INFO, "to %x\n", PcdGet64 (PcdFspmUpdDataAddress64)));

  //
  // Build HOB to fetch FSP-M migration information
  //
  Hob.Raw = (UINT8 *)(UINTN)FspHobList;
  Hob.Raw = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, Hob.Raw);
  while (Hob.Raw != NULL) {
    MigratedFvInfo = GET_GUID_HOB_DATA (Hob);
    if (PcdGet32 (PcdFspmBaseAddress) == MigratedFvInfo->FvOrgBase) {
      BuildGuidDataHob (&gEdkiiMigratedFvInfoGuid, GET_GUID_HOB_DATA (Hob), GET_GUID_HOB_DATA_SIZE (Hob));
      break;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
    Hob.Raw = GetNextGuidHob (&gEdkiiMigratedFvInfoGuid, Hob.Raw);
  }

  return EFI_SUCCESS;
}

/**
  Process FSP HOB list

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

**/
VOID
ProcessFspHobList (
  IN VOID                 *FspHobList
  )
{
  UINT8                 PhysicalAddressBits;
  UINT32                RegEax;
  EFI_PEI_HOB_POINTERS  FspHob;

  FspHob.Raw = FspHobList;

  AsmCpuid (0x80000000, &RegEax, NULL, NULL, NULL);
  if (RegEax >= 0x80000008) {
    AsmCpuid (0x80000008, &RegEax, NULL, NULL, NULL);
    PhysicalAddressBits = (UINT8) RegEax;
  } else {
    PhysicalAddressBits = 36;
  }

  ///
  /// Create a CPU hand-off information
  ///
  BuildCpuHob (PhysicalAddressBits, 16);

  //
  // Add all the HOBs from FSP binary to FSP wrapper
  //
  while (!END_OF_HOB_LIST (FspHob)) {
    if (FspHob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      //
      // Skip FSP binary creates PcdDataBaseHobGuid
      //
     if (!CompareGuid(&FspHob.Guid->Name, &gPcdDataBaseHobGuid)) {
        BuildGuidDataHob (
          &FspHob.Guid->Name,
          GET_GUID_HOB_DATA(FspHob),
          GET_GUID_HOB_DATA_SIZE(FspHob)
        );
      }
    }
    FspHob.Raw = GET_NEXT_HOB (FspHob);
  }
}

/**
  Dump FSP SMBIOS memory info HOB

**/
VOID
DumpFspSmbiosMemoryInfoHob (
  VOID
  )
{
  MEMORY_INFO_DATA_HOB   *FspSmbiosMemoryInfo;
  UINT8                  ChannelIndex;
  UINT8                  DimmIndex;
  UINT8                  Contollerindex;
  CHAR8                  *Bar;
  EFI_HOB_GUID_TYPE      *GuidHob;

  GuidHob = NULL;
  FspSmbiosMemoryInfo = NULL;

  GuidHob = GetFirstGuidHob (&gSiMemoryInfoDataGuid);
  if (GuidHob != NULL) {
    FspSmbiosMemoryInfo = (MEMORY_INFO_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
  }
  if (FspSmbiosMemoryInfo != NULL) {
    DEBUG((DEBUG_INFO, "\nFspSmbiosMemoryInfo\n"));
    DEBUG((DEBUG_INFO, "  |-> Revision : %d\n", FspSmbiosMemoryInfo->Revision));
    DEBUG((DEBUG_INFO, "  |-> MemoryType : %d\n", FspSmbiosMemoryInfo->MemoryType));
    DEBUG((DEBUG_INFO, "  |-> MaximumMemoryClockSpeed : %d\n", FspSmbiosMemoryInfo->MaximumMemoryClockSpeed));
    DEBUG((DEBUG_INFO, "  |-> ConfiguredMemoryClockSpeed : %d\n", FspSmbiosMemoryInfo->ConfiguredMemoryClockSpeed));
    DEBUG((DEBUG_INFO, "  |-> ErrorCorrectionType : %d\n", FspSmbiosMemoryInfo->ErrorCorrectionType));
    DEBUG((DEBUG_INFO, "  |-> ChannelCount : %d\n", FspSmbiosMemoryInfo->Controller[0].ChannelCount));
    DEBUG((DEBUG_INFO, "  |-> ChannelInfo\n"));
    for (Contollerindex = 0; Contollerindex < MAX_NODE; Contollerindex++) {
      DEBUG((DEBUG_INFO, "        |-> Memory Controller Id : %d\n", Contollerindex));
      for (ChannelIndex = 0; ChannelIndex < MAX_CH; ChannelIndex++) {
        {
          CHANNEL_INFO  *ChannelInfo;

          ChannelInfo = &FspSmbiosMemoryInfo->Controller[Contollerindex].ChannelInfo[ChannelIndex];
          DEBUG((DEBUG_INFO, "        |-> ChannelId : %d\n", ChannelInfo->ChannelId));
          DEBUG((DEBUG_INFO, "        |-> DimmCount : %d\n", ChannelInfo->DimmCount));
          DEBUG((DEBUG_INFO, "        |-> DimmInfo\n"));
          for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
            {
              DIMM_INFO  *DimmInfo;

              DimmInfo = &FspSmbiosMemoryInfo->Controller[Contollerindex].ChannelInfo[ChannelIndex].DimmInfo[DimmIndex];
              Bar = (ChannelIndex < (MAX_CH - 1)) ? "|" : " ";
              DEBUG((DEBUG_INFO, "        %a     |-> DimmId : %u\n", Bar, DimmInfo->DimmId));
              DEBUG((DEBUG_INFO, "        %a     |-> SizeInMb : %u\n", Bar, DimmInfo->DimmCapacity));
              DEBUG((DEBUG_INFO, "        %a     |-> DataWidth : %u\n", Bar, DimmInfo->DataWidth));
            }
          }
        }
      }
    }
    DEBUG((DEBUG_INFO, "\n"));
  }
}


/**
  Dump FSP SMBIOS Processor Info HOB

**/
VOID
DumpFspSmbiosProcessorInfoHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS      Hob;
  EFI_STATUS                Status;
  SMBIOS_PROCESSOR_INFO     *FspSmbiosProcessorInfo = NULL;

  Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  if (!EFI_ERROR (Status)) {
     if (Hob.Raw != NULL) {
       if ((Hob.Raw = GetNextGuidHob (&gSmbiosProcessorInfoHobGuid, Hob.Raw)) != NULL) {
         FspSmbiosProcessorInfo = GET_GUID_HOB_DATA (Hob.Guid);
       } else {
         DEBUG((DEBUG_ERROR, "Get SmbiosProcessorInfoHob failed!\n"));
       }
     }

     if (FspSmbiosProcessorInfo != NULL) {
       DEBUG((DEBUG_INFO, "\n FspSmbiosProcessorInfo\n"));
       DEBUG((DEBUG_INFO, "  |-> Total Number Of Sockets   : 0x%x\n", FspSmbiosProcessorInfo->TotalNumberOfSockets));
       DEBUG((DEBUG_INFO, "  |-> Current Socket Number   : 0x%x\n", FspSmbiosProcessorInfo->CurrentSocketNumber));
       DEBUG((DEBUG_INFO, "  |-> ProcessorType : 0x%x\n", FspSmbiosProcessorInfo->ProcessorType));
       DEBUG((DEBUG_INFO, "  |-> ProcessorFamily : 0x%x\n", FspSmbiosProcessorInfo->ProcessorFamily));
       DEBUG((DEBUG_INFO, "  |-> ProcessorManufacturerStrIndex : 0x%x\n", FspSmbiosProcessorInfo->ProcessorManufacturerStrIndex));
       DEBUG((DEBUG_INFO, "  |-> ProcessorId     : 0x%x\n", FspSmbiosProcessorInfo->ProcessorId));
       DEBUG((DEBUG_INFO, "  |-> ProcessorVersionStrIndex : 0x%x\n", FspSmbiosProcessorInfo->ProcessorVersionStrIndex));
       DEBUG((DEBUG_INFO, "  |-> Voltage : 0x%x\n", FspSmbiosProcessorInfo->Voltage));
       DEBUG((DEBUG_INFO, "  |-> ExternalClockInMHz : 0x%x\n", FspSmbiosProcessorInfo->ExternalClockInMHz));
       DEBUG((DEBUG_INFO, "  |-> MaxSpeedInMHz : 0x%x\n", FspSmbiosProcessorInfo->MaxSpeedInMHz));
       DEBUG((DEBUG_INFO, "  |-> CurrentSpeedInMHz : 0x%x\n", FspSmbiosProcessorInfo->CurrentSpeedInMHz));
       DEBUG((DEBUG_INFO, "  |-> Status : 0x%x\n", FspSmbiosProcessorInfo->Status));
       DEBUG((DEBUG_INFO, "  |-> ProcessorUpgrade : 0x%x\n", FspSmbiosProcessorInfo->ProcessorUpgrade));
       DEBUG((DEBUG_INFO, "  |-> CoreCount : 0x%x\n", FspSmbiosProcessorInfo->CoreCount));
       DEBUG((DEBUG_INFO, "  |-> EnabledCoreCount : 0x%x\n", FspSmbiosProcessorInfo->EnabledCoreCount));
       DEBUG((DEBUG_INFO, "  |-> ThreadCount : 0x%x\n", FspSmbiosProcessorInfo->ThreadCount));
       DEBUG((DEBUG_INFO, "  |-> ProcessorCharacteristics : 0x%x\n", FspSmbiosProcessorInfo->ProcessorCharacteristics));
       DEBUG((DEBUG_INFO, "\n"));
      }
    }
}

/**
  Dump FSP SMBIOS Cache Info Hob

**/
VOID
DumpFspSmbiosCacheInfoHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS      Hob;
  EFI_STATUS                Status;
  SMBIOS_CACHE_INFO         *FspSmbiosCacheInfo = NULL;

  Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  if (!EFI_ERROR (Status)) {
    if (Hob.Raw != NULL) {
      if ((Hob.Raw = GetNextGuidHob (&gSmbiosCacheInfoHobGuid, Hob.Raw)) != NULL) {
        FspSmbiosCacheInfo = GET_GUID_HOB_DATA (Hob.Guid);
      } else {
        DEBUG((DEBUG_ERROR, "Get SmbiosCacheInfoHob failed!\n"));
      }
    }

    if (FspSmbiosCacheInfo != NULL) {
      DEBUG((DEBUG_INFO, "\nFspSmbiosCacheInfo\n"));
      DEBUG((DEBUG_INFO, "  |-> ProcessorSocketNumber   : 0x%x\n", FspSmbiosCacheInfo->ProcessorSocketNumber));
      DEBUG((DEBUG_INFO, "  |-> NumberOfCacheLevels   : 0x%x\n", FspSmbiosCacheInfo->NumberOfCacheLevels));
      DEBUG((DEBUG_INFO, "  |-> SocketDesignationStrIndex : 0x%x\n", FspSmbiosCacheInfo->SocketDesignationStrIndex));
      DEBUG((DEBUG_INFO, "  |-> CacheConfiguration : 0x%x\n", FspSmbiosCacheInfo->CacheConfiguration));
      DEBUG((DEBUG_INFO, "  |-> MaxCacheSize  : 0x%x\n", FspSmbiosCacheInfo->MaxCacheSize));
      DEBUG((DEBUG_INFO, "  |-> InstalledSize : 0x%x\n", FspSmbiosCacheInfo->InstalledSize));
      DEBUG((DEBUG_INFO, "  |-> SupportedSramType : 0x%x\n", FspSmbiosCacheInfo->SupportedSramType));
      DEBUG((DEBUG_INFO, "  |-> CurrentSramType : 0x%x\n", FspSmbiosCacheInfo->CurrentSramType));
      DEBUG((DEBUG_INFO, "  |-> CacheSpeed : 0x%x\n", FspSmbiosCacheInfo->CacheSpeed));
      DEBUG((DEBUG_INFO, "  |-> ErrorCorrectionType : 0x%x\n", FspSmbiosCacheInfo->ErrorCorrectionType));
      DEBUG((DEBUG_INFO, "  |-> SystemCacheType : 0x%x\n", FspSmbiosCacheInfo->SystemCacheType));
      DEBUG((DEBUG_INFO, "  |-> Associativity : 0x%x\n", FspSmbiosCacheInfo->Associativity));
      DEBUG((DEBUG_INFO, "\n"));
    }
  }
}

/**
  Dump FSP HOB list

**/
VOID
DumpFspHobList (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS Hob;
  EFI_STATUS           Status;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  ASSERT_EFI_ERROR (Status);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION) {
      DEBUG ((DEBUG_INFO, "FSP Extended    GUID HOB: {%g}\n", &(Hob.Guid->Name)));
    }
    if ((Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) &&
       (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_MEMORY_RESERVED)) {
      DEBUG ((DEBUG_INFO, "FSP Reserved Resource HOB: %016lX ~ %016lX\n", \
              Hob.ResourceDescriptor->PhysicalStart, Hob.ResourceDescriptor->PhysicalStart \
              + Hob.ResourceDescriptor->ResourceLength));
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
}

/**
  Dump FSP memory resource

**/
VOID
DumpFspMemoryResource (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS Hob;
  EFI_STATUS           Status;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "\nFSP Memory Resource\n"));
  DEBUG ((DEBUG_INFO, "         Resource Range           Type    Attribute                   Owner\n"));
  DEBUG ((DEBUG_INFO, "================================= ==== ================ ====================================\n"));
  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
    if (!CompareGuid (&(Hob.ResourceDescriptor->Owner), &gZeroGuid)) {
      DEBUG ((DEBUG_INFO, "%016lx-%016lx %4x %016x %g\n",
              Hob.ResourceDescriptor->PhysicalStart,
              Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength,
              Hob.ResourceDescriptor->ResourceType,
              Hob.ResourceDescriptor->ResourceAttribute,
              &(Hob.ResourceDescriptor->Owner)
              ));
    } else {
      DEBUG ((DEBUG_INFO, "%016lx-%016lx %4x %016x \n",
              Hob.ResourceDescriptor->PhysicalStart,
              Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength,
              Hob.ResourceDescriptor->ResourceType,
              Hob.ResourceDescriptor->ResourceAttribute
              ));
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  Dump FSP memory resource

**/
VOID
DumpFspGraphicsInfoHob (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS             Hob;
  EFI_STATUS                       Status;
  EFI_PEI_GRAPHICS_INFO_HOB        *FspGraphicsInfo;
  EFI_PEI_GRAPHICS_DEVICE_INFO_HOB *FspGraphicsDeviceInfo;
  UINT8                            *HobStart;

  FspGraphicsInfo       = NULL;
  FspGraphicsDeviceInfo = NULL;
  HobStart              = NULL;

  Status = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  HobStart = Hob.Raw;

  if (!EFI_ERROR (Status)) {
    if (HobStart != NULL) {
      if ((Hob.Raw = GetNextGuidHob (&gEfiGraphicsInfoHobGuid, HobStart)) != NULL) {
        FspGraphicsInfo = GET_GUID_HOB_DATA (Hob.Guid);
      }
      if ((Hob.Raw = GetNextGuidHob (&gEfiGraphicsDeviceInfoHobGuid, HobStart)) != NULL) {
        FspGraphicsDeviceInfo = GET_GUID_HOB_DATA (Hob.Guid);
      }
    }
    if (FspGraphicsInfo != NULL) {
      DEBUG((DEBUG_INFO, "\nGraphicsInfo\n"));
      DEBUG((DEBUG_INFO, "  |-> FrameBufferBase : 0x%016lx\n", FspGraphicsInfo->FrameBufferBase));
      DEBUG((DEBUG_INFO, "  |-> FrameBufferSize : 0x%016lx\n", FspGraphicsInfo->FrameBufferSize));
      DEBUG((DEBUG_INFO, "  |-> GraphicsMode\n"));
      DEBUG((DEBUG_INFO, "    |-> Version              : 0x%08x\n", FspGraphicsInfo->GraphicsMode.Version));
      DEBUG((DEBUG_INFO, "    |-> HorizontalResolution : %d\n", FspGraphicsInfo->GraphicsMode.HorizontalResolution));
      DEBUG((DEBUG_INFO, "    |-> VerticalResolution   : %d\n", FspGraphicsInfo->GraphicsMode.VerticalResolution));
      DEBUG((DEBUG_INFO, "    |-> PixelFormat          : %d\n", FspGraphicsInfo->GraphicsMode.PixelFormat));
      DEBUG((DEBUG_INFO, "    |-> PixelInformation     : %d|%d|%d|%d\n",
        FspGraphicsInfo->GraphicsMode.PixelInformation.RedMask,
        FspGraphicsInfo->GraphicsMode.PixelInformation.GreenMask,
        FspGraphicsInfo->GraphicsMode.PixelInformation.BlueMask,
        FspGraphicsInfo->GraphicsMode.PixelInformation.ReservedMask
        ));
      DEBUG((DEBUG_INFO, "    |-> PixelsPerScanLine    : %d\n", FspGraphicsInfo->GraphicsMode.PixelsPerScanLine));
      DEBUG((DEBUG_INFO, "\n"));
    } else {
      DEBUG((DEBUG_ERROR, "\nNo GraphicsInfo\n\n"));
    }
    if (FspGraphicsDeviceInfo != NULL) {
      DEBUG((DEBUG_INFO, "\nGraphicsDeviceInfo\n"));
      DEBUG((DEBUG_INFO, "  |-> VendorId          : 0x%04x\n", FspGraphicsDeviceInfo->VendorId));
      DEBUG((DEBUG_INFO, "  |-> DeviceId          : 0x%04x\n", FspGraphicsDeviceInfo->DeviceId));
      DEBUG((DEBUG_INFO, "  |-> SubsystemVendorId : 0x%04x\n", FspGraphicsDeviceInfo->SubsystemVendorId));
      DEBUG((DEBUG_INFO, "  |-> SubsystemId       : 0x%04x\n", FspGraphicsDeviceInfo->SubsystemId));
      DEBUG((DEBUG_INFO, "  |-> RevisionId        : 0x%02x\n", FspGraphicsDeviceInfo->RevisionId));
      DEBUG((DEBUG_INFO, "  |-> BarIndex          : 0x%02x\n", FspGraphicsDeviceInfo->BarIndex));
      DEBUG((DEBUG_INFO, "\n"));
    } else {
      DEBUG((DEBUG_ERROR, "\nNo GraphicsDeviceInfo\n\n"));
    }
  }
}

#if FixedPcdGetBool(PcdFusaSupport) == 1
/**
  Map TestResult to its matching string for FuSa test

  @param[in] TestResult

  @return matching string in related to the TestResult value
**/
STATIC
VOID
TestResultString (
  IN UINT8 TestResult
  )
{
  switch (TestResult) {
    case FUSA_TEST_DEVICE_NOTAVAILABLE:
      DEBUG((DEBUG_INFO, "TEST_DEV_NA\n"));
      break;
    case FUSA_TEST_NOTRUN:
      DEBUG((DEBUG_INFO, "TEST_NOTRUN\n"));
      break;
    case FUSA_TEST_FAIL:
      DEBUG((DEBUG_INFO, "TEST_FAIL\n"));
      break;
    case FUSA_TEST_PASS:
      DEBUG((DEBUG_INFO, "TEST_PASS\n"));
      break;
    case FUSA_TEST_NOT_SUPPORTED:
      DEBUG((DEBUG_INFO, "TEST_NOT_SUPPORTED\n"));
      break;
    default:
      DEBUG((DEBUG_INFO, "ILLEGAL_STATE\n"));
      break;
  }
  return;
}

#define CRC32C_POLY_NORMAL        0x1EDC6F41
#define CRC32C_INITIAL_REMAINDER  0xFFFFFFFF

UINT32 mCrc32cTable[256];

/**
  This internal function reverses bits for 32bit data.

  @param  Value The data to be reversed.

  @return       Data reversed.

**/
UINT32
ReverseBits (
  UINT32  Value
  )
{
  UINTN  Index;
  UINT32 NewValue;

  NewValue = 0;
  for (Index = 0; Index < 32; Index++) {
    if ((Value & (1 << Index)) != 0) {
      NewValue = NewValue | (1 << (31 - Index));
    }
  }

  return NewValue;
}


/**
  Generate a CRC-32C table that can speed up CRC calculation by table look up.
**/
VOID
Crc32cInit(
  VOID
  )
{
  UINT32 TableEntry;
  UINT32 Index;
  UINT32 CrcVal;

  for (TableEntry = 0; TableEntry < 256; TableEntry++) {
    CrcVal = ReverseBits ((UINT32) TableEntry);
    for (Index = 0; Index < 8; Index++) {
      if ((CrcVal & 0x80000000) != 0) {
        CrcVal = (CrcVal << 1) ^ CRC32C_POLY_NORMAL;
      } else {
        CrcVal = CrcVal << 1;
      }
    }

    mCrc32cTable[TableEntry] = ReverseBits (CrcVal);
  }
}

/**
  Check if the TestResult->Crc32 is correct.

  @param[in] pTestResult  Pointer to the test result buffer.

  @retval    TRUE         If the TestResult->Crc32 value is correct.
  @retval    FALSE        Otherwise.
**/

BOOLEAN
Crc32Check (
  FUSA_TEST_RESULT *pTestResult
  )
{
  UINT32 Len = sizeof (FUSA_TEST_RESULT);
  UINT32 Crc;
  UINT32 Index;
  UINT8  *Ptr;
  UINT32 Crc32Result;

  if (pTestResult == NULL) {
    return FALSE;
  }

  Crc32cInit ();

  Crc = CRC32C_INITIAL_REMAINDER;
  for (Index = 0, Ptr = ((UINT8 *) pTestResult); Index < Len; Index++, Ptr++) {
    Crc = (Crc >> 8) ^ mCrc32cTable[(UINT8)Crc ^ *Ptr];
  }

  Crc32Result = Crc ^ 0xFFFFFFFF;

  return (Crc32Result == 0xFFFFFFFF); //True if the TestResult->Crc32 value is correct.
}

/**
  Dump FSP Fusa info HOB
**/
VOID
DumpFspFusaInfoHob (
  VOID
  )
{
  FUSA_INFO_HOB   *FspFusaInfo;
  EFI_HOB_GUID_TYPE      *GuidHob;

  GuidHob = NULL;
  FspFusaInfo = NULL;

  GuidHob = GetFirstGuidHob (&gSiFusaInfoGuid);
  if (GuidHob != NULL) {
    FspFusaInfo = (FUSA_INFO_HOB *) GET_GUID_HOB_DATA (GuidHob);
  }
  if (FspFusaInfo != NULL) {
    DEBUG((DEBUG_INFO, "\nFspFusaInfo\n"));
    DEBUG((DEBUG_INFO, "  |-> Version : %d\n", FspFusaInfo->Version));
    for (UINT32 TestIndex = 0; TestIndex < FusaTestNumTotal; TestIndex++) {
      DEBUG((DEBUG_INFO, "  |-> Test Id : %d\n", FspFusaInfo->FspDxCtcTestResult[TestIndex].TestNumber));
      if (Crc32Check(&(FspFusaInfo->FspDxCtcTestResult[TestIndex]))) {
        DEBUG((DEBUG_INFO, "    |-> Total checks : %d\n", FspFusaInfo->FspDxCtcTestResult[TestIndex].TotalChecks));
        DEBUG((DEBUG_INFO, "    |-> Test result : "));
        TestResultString (FspFusaInfo->FspDxCtcTestResult[TestIndex].TestResult);
        for (UINT32 CheckIndex = 0; CheckIndex < FspFusaInfo->FspDxCtcTestResult[TestIndex].TotalChecks; CheckIndex++){
          DEBUG((DEBUG_INFO, "      |-> Check Id %d result: ", CheckIndex ));
          TestResultString (FspFusaInfo->FspDxCtcTestResult[TestIndex].CheckResults[CheckIndex]);
        }
      } else {
        DEBUG((DEBUG_INFO, "    |-> CRC32 check on test result failed\n"));
      }
    }
    DEBUG((DEBUG_INFO, "\n"));
  }
}

#endif

/**
  Post FSP-S HOB process (not Memory Resource Descriptor).

  @param[in] FspHobList  Pointer to the HOB data structure produced by FSP.

  @return If platform process the FSP hob list successfully.
**/
EFI_STATUS
EFIAPI
PostFspsHobProcess (
  IN VOID                 *FspHobList
  )
{

  if (PcdGet8 (PcdFspModeSelection) == 1) {
    ASSERT (FspHobList != NULL);
    //
    // Dump FSP error information HOB.
    //
    DumpFspErrorInfo (FspHobList);
    //
    // Only in FSP API mode the wrapper has to build hobs basing on FSP output data.
    //
    ProcessFspHobList (FspHobList);
  }

#if FixedPcdGetBool(PcdFusaSupport) == 1
  //
  //IOTG FuSa SKU Key-off processing
  //
  if (IsInFusaDiagnosticMode ()) {
    //Sample code only performs result dumping; in production code, expect passing the result to the MCU
    DEBUG_CODE_BEGIN ();
    DumpFspFusaInfoHob ();
    DEBUG_CODE_END ();
    CpuDeadLoop (); //expecting the MCU to G3 reset the system
  }
#endif

  //
  // Get Silicon Config data HOB
  //

  DEBUG_CODE_BEGIN ();
    DumpFspSmbiosMemoryInfoHob ();
    DumpFspSmbiosProcessorInfoHob();
    DumpFspSmbiosCacheInfoHob();
    DumpFspGraphicsInfoHob ();
    DumpFspHobList ();
    DumpFspMemoryResource ();
  DEBUG_CODE_END ();

  return EFI_SUCCESS;
}

/**
  Wrapper for a thunk to transition from long mode to compatibility mode to execute 32-bit code and then transit back to
  long mode.

  @param[in] Function     The 32bit code entry to be executed.
  @param[in] Param1       The first parameter to pass to 32bit code.
  @param[in] Param2       The second parameter to pass to 32bit code.

  @return EFI_STATUS.
**/
EFI_STATUS
Execute32BitCode (
  IN UINT64      Function,
  IN UINT64      Param1,
  IN UINT64      Param2
  );

/**
  Wrapper for a thunk to transition from compatibility mode to long mode to execute 64-bit code and then transit back to
  compatibility mode.

  @param[in] Function     The 64bit code entry to be executed.
  @param[in] Param1       The first parameter to pass to 64bit code.
  @param[in] Param2       The second parameter to pass to 64bit code.

  @return EFI_STATUS.
**/
EFI_STATUS
Execute64BitCode (
  IN UINT64      Function,
  IN UINT64      Param1,
  IN UINT64      Param2
  );

/**
  Call FspMultiPhaseSiInit API.

  @param[in] FspMultiPhaseParams - Parameters for MultiPhaseSiInit API.

  @return EFI status returned by FspMultiPhaseSiInit API.
**/
EFI_STATUS
EFIAPI
CallFspMultiPhaseSiliconInit (
  IN VOID                       *FspMultiPhaseParams
  )
{
  FSP_INFO_HEADER               *FspHeader;
  FSP_MULTI_PHASE_SI_INIT       FspMultiPhaseSiliconInitApi;
  EFI_STATUS                    Status;
  BOOLEAN                       InterruptState;

  FspHeader = (FSP_INFO_HEADER *)FspFindFspHeader (PcdGet32 (PcdFspsBaseAddress));
  if (FspHeader == NULL) {
    return EFI_DEVICE_ERROR;
  }

  FspMultiPhaseSiliconInitApi = (FSP_MULTI_PHASE_SI_INIT)((UINTN)FspHeader->ImageBase + FspHeader->FspMultiPhaseSiInitEntryOffset);
  InterruptState = SaveAndDisableInterrupts ();
  if ((FspHeader->ImageAttribute & BIT2) == 0) { // BIT2: IMAGE_ATTRIBUTE_64BIT_MODE_SUPPORT
    Status = Execute32BitCode ((UINTN)FspMultiPhaseSiliconInitApi, (UINTN)FspMultiPhaseParams, (UINTN)NULL);
  } else {
    Status = Execute64BitCode ((UINTN)FspMultiPhaseSiliconInitApi, (UINTN)FspMultiPhaseParams, (UINTN)NULL);
  }
  SetInterruptState (InterruptState);

  return Status;
}

/**
  It's a reference code for how to call CallFspMultiPhaseSiliconInit.

  @retval EFI_STATUS        Always return EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
FspWrapperCallMultiPhaseSiInit (
  VOID
  )
{
  EFI_STATUS                                   Status;
  FSP_MULTI_PHASE_PARAMS                       *FspMultiPhaseParams;
  FSP_MULTI_PHASE_GET_NUMBER_OF_PHASES_PARAMS  *FspMultiPhaseGetNumber;
  UINT32                                       Index;

  FspMultiPhaseParams = (FSP_MULTI_PHASE_PARAMS *)AllocateZeroPool (sizeof (FSP_MULTI_PHASE_PARAMS));
  if (FspMultiPhaseParams == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  FspMultiPhaseGetNumber = (FSP_MULTI_PHASE_GET_NUMBER_OF_PHASES_PARAMS *)AllocateZeroPool (sizeof (FSP_MULTI_PHASE_GET_NUMBER_OF_PHASES_PARAMS));
  if (FspMultiPhaseGetNumber == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  FspMultiPhaseParams->MultiPhaseAction = 0;
  FspMultiPhaseParams->PhaseIndex = 0;
  FspMultiPhaseParams->MultiPhaseParamPtr = (VOID *)FspMultiPhaseGetNumber;
  Status = CallFspMultiPhaseSiliconInit (FspMultiPhaseParams);
  ASSERT_EFI_ERROR (Status);

  for (Index = 1; Index <= FspMultiPhaseGetNumber->NumberOfPhases; Index++) {
    FspMultiPhaseParams->MultiPhaseAction = 1;
    FspMultiPhaseParams->PhaseIndex = Index;
    FspMultiPhaseParams->MultiPhaseParamPtr = NULL;
    Status = CallFspMultiPhaseSiliconInit (FspMultiPhaseParams);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
