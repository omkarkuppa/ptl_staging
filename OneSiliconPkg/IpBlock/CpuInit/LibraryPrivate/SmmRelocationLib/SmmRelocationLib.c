/** @file
  SMM Relocation Lib for each processor.

  This Lib produces the SMM_BASE_HOB in HOB database which tells
  the PiSmmCpuDxeSmm driver (runs at a later phase) about the new
  SMBASE for each processor. PiSmmCpuDxeSmm driver installs the
  SMI handler at the SMM_BASE_HOB.SmBase[Index]+0x8000 for processor
  Index.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/SmramMemoryReserve.h>
#include <Guid/SmmBaseHob.h>
#include <Register/Intel/SmramSaveStateMap.h>
#include <Register/ArchitecturalMsr.h>
#include <Library/SmmRelocationLib.h>

///
/// Encapsulate all information AP procedure needs into a structure.
/// A pointer to the structure will be passed to AP procedure.
///
typedef struct {
  EDKII_PEI_MP_SERVICES2_PPI        *MpServices2;
  UINT64                            *SmBaseForAllCpu;
  UINTN                             BspNumber;
} SMM_BASE_SETTING_CONTEXT;


/**
  Program to the SMBASE Register for each thread.

  @param[in,out] Buffer     Pointer to SMM_BASE_SETTING_CONTEXT.

**/
VOID
EFIAPI
ProgramSmmRelocationRegisters (
  IN OUT VOID *Buffer
  )
{
  EFI_STATUS                     Status;
  SMM_BASE_SETTING_CONTEXT       *Context;
  UINTN                          CpuNumber;

  Context = (SMM_BASE_SETTING_CONTEXT*) Buffer;

  Status = Context->MpServices2->WhoAmI (Context->MpServices2, &CpuNumber);
  ASSERT_EFI_ERROR (Status);

  if (CpuNumber == Context->BspNumber) {
    DEBUG ((DEBUG_INFO, "ProgramSmmRelocationRegisters: (BSP) Writing SmBase to MSR....\n"));
  }

  AsmWriteMsr64 (MSR_IA32_SMBASE, Context->SmBaseForAllCpu[CpuNumber]);

  if (CpuNumber == Context->BspNumber) {
    DEBUG ((DEBUG_INFO, "ProgramSmmRelocationRegisters: (BSP) Write SmBase to MSR sucessfully!\n"));
  }

  return;
}

/**
  This routine will split SmramReserve hob to reserve SmmRelocationSize for Smm relocated memory.

  @param[in]       SmmRelocationSize   SmmRelocationSize for all processors.
  @param[in out]   SmmRelocationStart  Return the start address of Smm relocated memory in SMRAM.

  @retval EFI_SUCCESS           The gEfiSmmSmramMemoryGuid is split successfully.
  @retval EFI_DEVICE_ERROR      Failed to build new hob for gEfiSmmSmramMemoryGuid.
  @retval EFI_NOT_FOUND         The gEfiSmmSmramMemoryGuid is not found.

**/
EFI_STATUS
SplitSmramHobForSmmRelocation (
  IN     UINT64                  SmmRelocationSize,
  IN OUT EFI_PHYSICAL_ADDRESS*   SmmRelocationStart
  )
{
  EFI_HOB_GUID_TYPE*               GuidHob;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK   *DescriptorBlock;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK   *NewDescriptorBlock;
  UINTN                            BufferSize;
  UINTN                            SmramRanges;

  NewDescriptorBlock = NULL;

  //
  // Retrieve the GUID HOB data that contains the set of SMRAM descriptors
  //
  GuidHob = GetFirstGuidHob (&gEfiSmmSmramMemoryGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }

  DescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) GET_GUID_HOB_DATA (GuidHob);

  //
  // Allocate one extra EFI_SMRAM_DESCRIPTOR to describe SMRAM memory that contains a pointer
  // to the Smm relocated memory.
  //
  SmramRanges = DescriptorBlock->NumberOfSmmReservedRegions;
  BufferSize = sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK) + (SmramRanges * sizeof (EFI_SMRAM_DESCRIPTOR));

  NewDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) BuildGuidHob (
                                                            &gEfiSmmSmramMemoryGuid,
                                                            BufferSize
                                                            );
  ASSERT (NewDescriptorBlock != NULL);
  if (NewDescriptorBlock == NULL) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Copy old EFI_SMRAM_HOB_DESCRIPTOR_BLOCK to new allocated region
  //
  CopyMem ((VOID *)NewDescriptorBlock, DescriptorBlock, BufferSize - sizeof (EFI_SMRAM_DESCRIPTOR));

  //
  // Increase the number of SMRAM descriptors by 1 to make room for the ALLOCATED descriptor of size EFI_PAGE_SIZE
  //
  NewDescriptorBlock->NumberOfSmmReservedRegions = (UINT32)(SmramRanges + 1);

  ASSERT (SmramRanges >= 1);
  //
  // Copy last entry to the end - we assume TSEG is last entry.
  //
  CopyMem (&NewDescriptorBlock->Descriptor[SmramRanges], &NewDescriptorBlock->Descriptor[SmramRanges - 1], sizeof (EFI_SMRAM_DESCRIPTOR));

  //
  // Update the entry in the array with a size of SmmRelocationSize and put into the ALLOCATED state
  //
  NewDescriptorBlock->Descriptor[SmramRanges - 1].PhysicalSize = SmmRelocationSize;
  NewDescriptorBlock->Descriptor[SmramRanges - 1].RegionState |= EFI_ALLOCATED;

  //
  // Return the start address of Smm relocated memory in SMRAM.
  //
  if (SmmRelocationStart != NULL) {
    *SmmRelocationStart = NewDescriptorBlock->Descriptor[SmramRanges - 1].CpuStart;
  }

  //
  // Reduce the size of the last SMRAM descriptor by SmmRelocationSize
  //
  NewDescriptorBlock->Descriptor[SmramRanges].PhysicalStart += SmmRelocationSize;
  NewDescriptorBlock->Descriptor[SmramRanges].CpuStart += SmmRelocationSize;
  NewDescriptorBlock->Descriptor[SmramRanges].PhysicalSize -= SmmRelocationSize;

  //
  // Last step, we can scrub old one
  //
  ZeroMem (&GuidHob->Name, sizeof (GuidHob->Name));

  return EFI_SUCCESS;
}


/**
  CPU SmmBase Relocation Init.

  This function is to relocate CPU SmmBase.

  @param[in] MpServices2        Pointer to this instance of the MpServices.

  @retval EFI_UNSUPPORTED       CPU SmmBase Relocation unsupported.
  @retval EFI_OUT_OF_RESOURCES  CPU SmmBase Relocation failed.
  @retval EFI_SUCCESS           CPU SmmBase Relocated successfully.

**/
EFI_STATUS
EFIAPI
SmmRelocationInit (
  IN EDKII_PEI_MP_SERVICES2_PPI  *MpServices2
  )
{
  EFI_STATUS                        Status;
  UINTN                             NumberOfProcessors;
  UINTN                             NumberOfEnabledProcessors;
  UINTN                             MaxNumberOfCpus;
  UINTN                             TileSize;
  UINT64                            SmmRelocationSize;
  EFI_PHYSICAL_ADDRESS              SmmRelocationStart;
  UINTN                             Index;
  SMM_BASE_HOB_DATA                 *SmmBaseHobData;
  UINT32                            CpuCount;
  UINT32                            NumberOfProcessorsInHob;
  UINT32                            MaxCapOfProcessorsInHob;
  UINT32                            HobCount;
  SMM_BASE_SETTING_CONTEXT          Context;

  SmmRelocationStart       = 0;
  SmmBaseHobData           = NULL;
  CpuCount                 = 0;
  NumberOfProcessorsInHob  = 0;
  MaxCapOfProcessorsInHob  = 0;
  HobCount                 = 0;
  Context.MpServices2      = NULL;
  Context.SmBaseForAllCpu  = NULL;

  DEBUG ((DEBUG_INFO, "SmmRelocationInit Start \n"));
  if (MpServices2 == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the number of processors
  // If support CPU hot plug, we need to allocate resources for possibly hot-added processors
  //
  Status = MpServices2->GetNumberOfProcessors (
                          MpServices2,
                          &NumberOfProcessors,
                          &NumberOfEnabledProcessors
                          );
  ASSERT_EFI_ERROR (Status);

  if (FeaturePcdGet (PcdCpuHotPlugSupport)) {
    MaxNumberOfCpus = PcdGet32 (PcdCpuMaxLogicalProcessorNumber);
  } else {
    MaxNumberOfCpus = NumberOfProcessors;
  }

  //
  // Calculate SmmRelocationSize for all of the tiles.
  //
  // The CPU save state and code for the SMI entry point are tiled within an SMRAM
  // allocated buffer. The minimum size of this buffer for a uniprocessor system
  // is 32 KB, because the entry point is SMBASE + 32KB, and CPU save state area
  // just below SMBASE + 64KB. If more than one CPU is present in the platform,
  // then the SMI entry point and the CPU save state areas can be tiles to minimize
  // the total amount SMRAM required for all the CPUs. The tile size can be computed
  // by adding the CPU save state size, any extra CPU specific context, and
  // the size of code that must be placed at the SMI entry point to transfer
  // control to a C function in the native SMM execution mode. This size is
  // rounded up to the nearest power of 2 to give the tile size for a each CPU.
  // The total amount of memory required is the maximum number of CPUs that
  // platform supports times the tile size.
  //
  TileSize          = SIZE_8KB;
  SmmRelocationSize = EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (SIZE_32KB + TileSize * (MaxNumberOfCpus - 1)));

  //
  // Split SmramReserve hob to reserve SmmRelocationSize for Smm relocated memory
  //
  Status = SplitSmramHobForSmmRelocation (
             SmmRelocationSize,
             &SmmRelocationStart
             );
  ASSERT_EFI_ERROR (Status);

  ASSERT (SmmRelocationStart != 0);
  DEBUG ((DEBUG_INFO, "SmmRelocationInit - SmmRelocationSize: 0x%08x\n", SmmRelocationSize));
  DEBUG ((DEBUG_INFO, "SmmRelocationInit - SmmRelocationStart: 0x%08x\n", SmmRelocationStart));

  //
  // Init Context
  //
  Context.MpServices2 = MpServices2;
  Context.SmBaseForAllCpu = (UINT64*) AllocatePages (EFI_SIZE_TO_PAGES (sizeof (UINT64) * MaxNumberOfCpus));
  if (Context.SmBaseForAllCpu == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Count the hob instance maximum capacity of CPU (MaxCapOfProcessorsInHob) since the max HobLength is 0xFFF8.
  //
  MaxCapOfProcessorsInHob = (0xFFF8 - sizeof (EFI_HOB_GUID_TYPE) - sizeof (SMM_BASE_HOB_DATA)) / sizeof (UINT64) + 1;
  DEBUG ((DEBUG_INFO, "SmmRelocationInit - MaxCapOfProcessorsInHob: %03x\n", MaxCapOfProcessorsInHob));

  //
  // Create Guided SMM Base HOB Instances.
  //
  while (CpuCount != MaxNumberOfCpus) {
    NumberOfProcessorsInHob = MIN ((UINT32) MaxNumberOfCpus - CpuCount, MaxCapOfProcessorsInHob);

    SmmBaseHobData = BuildGuidHob (
                       &gSmmBaseHobGuid,
                       sizeof (SMM_BASE_HOB_DATA) + sizeof (UINT64) * NumberOfProcessorsInHob
                       );
    ASSERT (SmmBaseHobData != NULL);
    if (SmmBaseHobData == NULL) {
      FreePages (Context.SmBaseForAllCpu, EFI_SIZE_TO_PAGES (sizeof (UINT64) * MaxNumberOfCpus));
      return EFI_OUT_OF_RESOURCES;
    }

    SmmBaseHobData->ProcessorIndex = CpuCount;
    SmmBaseHobData->NumberOfProcessors = NumberOfProcessorsInHob;

    DEBUG ((DEBUG_INFO, "SmmRelocationInit - SmmBaseHobData[%d]->ProcessorIndex: %03x\n", HobCount, SmmBaseHobData->ProcessorIndex));
    DEBUG ((DEBUG_INFO, "SmmRelocationInit - SmmBaseHobData[%d]->NumberOfProcessors: %03x\n", HobCount, SmmBaseHobData->NumberOfProcessors));
    for (Index = 0; Index < SmmBaseHobData->NumberOfProcessors; Index++) {
      //
      // Calculate the new SMBASE address
      //
      SmmBaseHobData->SmBase[Index] = (UINTN)(SmmRelocationStart)+ (Index + CpuCount) * TileSize - SMM_HANDLER_OFFSET;
      DEBUG ((DEBUG_INFO, "SmmRelocationInit - SmmBaseHobData[%d]->SmBase[%03x]: %08x\n", HobCount, Index, SmmBaseHobData->SmBase[Index]));

      //
      // Record each SmBase in Context.SmBaseForAllCpu
      //
      Context.SmBaseForAllCpu[Index + CpuCount] = SmmBaseHobData->SmBase[Index];
    }

    CpuCount += NumberOfProcessorsInHob;
    HobCount ++;
    SmmBaseHobData = NULL;
  }

  //
  // Check BspNumber
  //
  Status = MpServices2->WhoAmI (MpServices2, &Context.BspNumber);
  ASSERT_EFI_ERROR (Status);

  //
  // Program to the SMBASE Registers in parallel if Msr Based Smbase Relocation enabled.
  //
  Status = MpServices2->StartupAllCPUs (
                          MpServices2,
                          (EFI_AP_PROCEDURE) ProgramSmmRelocationRegisters,
                          0,
                          &Context
                          );
  ASSERT_EFI_ERROR (Status);

  FreePages (Context.SmBaseForAllCpu, EFI_SIZE_TO_PAGES (sizeof (UINT64)* MaxNumberOfCpus));

  DEBUG ((DEBUG_INFO, "SmmRelocationInit Done \n"));

  return EFI_SUCCESS;
}
