/** @file
  Initialize TDX in PEI

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

#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <Base.h>
#include <Register/Cpuid.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PeiTdxLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/TmeInfoLib.h>
#include <Library/TdxInfoLib.h>
#include <Library/TxtPeiLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TdxDataHob.h>
#include <Library/MemoryAllocationLib.h>
#include <Register/GenerationMsr.h>
#include <Library/PeiCpuInitFruLib.h>
#include <Library/PeiImrInitLib.h>
#include <Library/MemorySubSystemInfoLib.h>
#include <Library/TdxFruLib.h>
#include <Uefi/UefiMultiPhase.h>
#include <Library/TdxDataHob.h>
#include <IntelRcStatusCode.h>

#define INITIALIZED_MEMORY_ATTRIBUTES (EFI_RESOURCE_ATTRIBUTE_PRESENT | \
                                       EFI_RESOURCE_ATTRIBUTE_INITIALIZED)

EFI_PEI_NOTIFY_DESCRIPTOR  mPpiPeiTdxMemoryNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  PeiTdxMemoryNotifyCallback
};

/**
  This function will be called when MRC is done and for generating CMR entries

  @param  PeiServices      Indirect reference to the PEI Services Table.
  @param  NotifyDesc       Address of the notification descriptor data structure.
  @param  Ppi              Address of the PPI that was installed.

  @retval EFI_SUCCESS          If the function completed successfully.
  @retval EFI_OUT_OF_RESOURCES If the resource allocation fail
**/
EFI_STATUS
EFIAPI
PeiTdxMemoryNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
 )
{
  EFI_STATUS                       Status;

  DEBUG ((DEBUG_INFO, "[TDX] Notify from Memory Init\n"));
  DEBUG ((DEBUG_INFO, "[TDX] Calling FillCmrArray Function\n"));

  // Generate CMR entries
  Status = FillCmrArray ();
  return Status;
}

/**
  Count the number of memory Descriptors which are available memory regions in system memory

  @retval NumbDescriptor           Number of memory Descriptors
**/
UINT8
GetNumbDescriptors(
  VOID
  )
{
  EFI_PEI_HOB_POINTERS            Hob;
  UINT8                           NumbDescriptor;

  NumbDescriptor = 0;
  Hob.Raw = GetHobList ();
  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
    if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) &&
        ((Hob.ResourceDescriptor->ResourceAttribute & INITIALIZED_MEMORY_ATTRIBUTES) == INITIALIZED_MEMORY_ATTRIBUTES) &&
        !((Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_MORE_RELIABLE) == EFI_RESOURCE_ATTRIBUTE_MORE_RELIABLE) &&
        !((Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_PERSISTENT) == EFI_RESOURCE_ATTRIBUTE_PERSISTENT))
        {
          // Ignore empty regions
          if (Hob.ResourceDescriptor->ResourceLength == 0) {
            Hob.Raw = GET_NEXT_HOB (Hob);
            continue;
          }

          // Check if CmrBase is above 16MB
          if ((Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength) < BASE_16MB) {
            Hob.Raw = GET_NEXT_HOB (Hob);
            continue;
          }
        }
    NumbDescriptor++;
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  return NumbDescriptor;
}

/**
  Perform sorting on memory descriptor array to get contiguous memory region if any

  @param[in,out] *ResourceDescriptorList     A pointer to an array of temporary MCHECK_CMR structures
  @param[in]     DescriptorCount             Number of memory descriptor array entries populated

  @retval VOID   - No value to return
**/
VOID
PerformSorting (
  IN OUT MCHECK_CMR      *ResourceDescriptorList,
  IN UINT8               DescriptorCount
  )
{
  UINT8 Index = 0;
  UINT8 IndexJ = 0;
  MCHECK_CMR temp;

  for ( Index = 0; Index < DescriptorCount - 1; Index++) {
    for (IndexJ = 0; IndexJ < DescriptorCount - Index - 1; IndexJ++) {
      if (ResourceDescriptorList[IndexJ].CmrBase > ResourceDescriptorList[IndexJ + 1].CmrBase) {
          // Swap elements
          temp = ResourceDescriptorList[IndexJ];
          ResourceDescriptorList[IndexJ] = ResourceDescriptorList[IndexJ + 1];
          ResourceDescriptorList[IndexJ + 1] = temp;
      }
    }
  }
}

/**
  Build CMR table from temporary cmr array and merge the contiguous memory region if any

  @param[in] *ResourceDescriptorList       A pointer to an array of temporary MCHECK_CMR structures
  @param[in] DescriptorCount               Number of Cmr array entries populated
  @param[in,out] *Cmr                      A pointer to an array of MCHECK_CMR structures
  @param[in,out] *CmrIndex                 Number of actual Cmr array entries populated after merging the contiguous memory region

  @retval VOID    - No value to return
**/

VOID
BuildCmrTable (
  IN MCHECK_CMR       *ResourceDescriptorList,
  IN UINT8            DescriptorCount,
  IN OUT MCHECK_CMR   *Cmr,
  IN OUT UINT32       *CmrIndex
  )
{
  UINT8 Index    = 0;

  for (Index = 0; Index < DescriptorCount; Index++) {

    if (Index > 0 && (ResourceDescriptorList[Index-1].CmrBase + ResourceDescriptorList[Index-1].CmrSize == ResourceDescriptorList[Index].CmrBase)) {

      // Merge memory descriptor entries if current region is contiguous with previous
      Cmr[(*CmrIndex)-1].CmrSize += ResourceDescriptorList[Index].CmrSize ;
      DEBUG ((DEBUG_INFO, "Resized Cmr[%02x] with : Size = 0x%016lx\n", Index-1, Cmr[(*CmrIndex)-1].CmrSize));

      // Stay at current CmrIndex, do not increment the number of entries in the Cmr array
    } else {

      // Check if the number of Cmrs does not exceed the limit (32)
      if((*CmrIndex) < MCHECK_MAX_CMR_ENTRIES) {
      // B. Create new entry at start and when current region is not contiguous with previous
      Cmr[*CmrIndex].CmrBase = ResourceDescriptorList[Index].CmrBase;
      Cmr[*CmrIndex].CmrSize = ResourceDescriptorList[Index].CmrSize;

      DEBUG ((DEBUG_INFO, "Created Cmr[%02x]: Base = 0x%016lx; Size = 0x%016lx\n", *CmrIndex, Cmr[*CmrIndex].CmrBase, Cmr[*CmrIndex].CmrSize));
      }

      // Iterate over to next Cmr entry
      (*CmrIndex)++;
    }
  }
}

/**
  Fills given Cmr array with CMR entries and sets NumCmrArrayEntries to a corresponding number of
  populated entries

  @param[in,out] CmrList                     A pointer to an MCHECK_PARAM_INFO_SEAM_DATA struture

  @retval EFI_SUCCESS           Success
  @retval EFI_OUT_OF_RESOURCES  If the resource allocation fail
**/
EFI_STATUS
FillCmrArray(
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT64                          DescriptorSize;
  PHYSICAL_ADDRESS                DescriptorBase;
  UINT8                           NumbDescriptors;
  EFI_PEI_HOB_POINTERS            Hob;
  MCHECK_CMR                      *ResourceDescriptorList;
  UINT8                           DescriptorIndex;
  UINT32                          CmrIndex;
  TDX_DATA_HOB                    *TdxDataHob;

  TdxDataHob = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
  if (TdxDataHob == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status                 = EFI_SUCCESS;
  DescriptorSize         = 0;
  DescriptorBase         = 0;
  DescriptorIndex        = 0;
  CmrIndex               = 0;

  DEBUG ((DEBUG_INFO, "[TDX] %a BEGIN\n", __FUNCTION__));

  // Count of memory Descriptors
  NumbDescriptors = GetNumbDescriptors();
  DEBUG ((DEBUG_INFO, "[TDX] NumbDescriptors %d \n", NumbDescriptors));

  ResourceDescriptorList = (MCHECK_CMR *) AllocateZeroPool (sizeof(MCHECK_CMR) * NumbDescriptors);
  if (ResourceDescriptorList == NULL) {
    DEBUG ((DEBUG_ERROR, "[TDX] Error allocating ResourceDescriptorList\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Hob.Raw = GetHobList ();
  while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw)) != NULL) {
    if ((Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) &&
        ((Hob.ResourceDescriptor->ResourceAttribute & INITIALIZED_MEMORY_ATTRIBUTES) == INITIALIZED_MEMORY_ATTRIBUTES) &&
        !((Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_MORE_RELIABLE) == EFI_RESOURCE_ATTRIBUTE_MORE_RELIABLE) &&
        !((Hob.ResourceDescriptor->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_PERSISTENT) == EFI_RESOURCE_ATTRIBUTE_PERSISTENT)) {

      DEBUG ((DEBUG_INFO, "Inside EFI_RESOURCE_SYSTEM_MEMORY condition ........................\n"));
      DescriptorBase = Hob.ResourceDescriptor->PhysicalStart;
      DescriptorSize = Hob.ResourceDescriptor->ResourceLength;

      // Ignore empty regions
      if (DescriptorSize == 0) {
        DEBUG ((DEBUG_INFO, "MemoryDescriptor.Length is equal to 0, skipping...\n"));
        Hob.Raw = GET_NEXT_HOB (Hob);
        continue;
      }

      // Check if CmrBase is above 16MB
      if ((DescriptorBase + DescriptorSize) < BASE_16MB) {
        DEBUG ((DEBUG_INFO, "MemoryDescriptor.Base is not above 16 MB, skipping...\n"));
        Hob.Raw = GET_NEXT_HOB (Hob);
        continue;
      } else if ((DescriptorBase < BASE_16MB) && ((DescriptorBase + DescriptorSize) > BASE_16MB)) {
        DescriptorSize = (DescriptorSize - (SIZE_16MB - DescriptorBase));
        DescriptorBase = BASE_16MB;
      }

      ResourceDescriptorList[DescriptorIndex].CmrBase = DescriptorBase;
      ResourceDescriptorList[DescriptorIndex].CmrSize = DescriptorSize;

      // Store the base and size with index in structure
      DEBUG ((DEBUG_INFO, "Created ResourceDescriptorList[%02x] : Base = 0x%016lx; Size = 0x%016lx\n",
                  DescriptorIndex, ResourceDescriptorList[DescriptorIndex].CmrBase, ResourceDescriptorList[DescriptorIndex].CmrSize));
      DescriptorIndex++;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  // Sort the CMR table Based on the Base address
  PerformSorting (ResourceDescriptorList, DescriptorIndex);

  // Display the sorted table.
  for (UINT32 Index = 0; Index < DescriptorIndex; Index++) {
      DEBUG ((DEBUG_INFO, "BaseAddress:  0x%016lx, Size: %016lx\n", ResourceDescriptorList[Index].CmrBase, ResourceDescriptorList[Index].CmrSize));
  }

  // Build the CMR Table using sorted CMR table.
  // As part of this merge the contiguous memory region if any
  BuildCmrTable(ResourceDescriptorList, DescriptorIndex, TdxDataHob->CmrList.Cmr, &CmrIndex);

  // Set an output number of processed Cmr entries
  TdxDataHob->CmrList.NumCmrArrayEntries = CmrIndex;

  // Free allcoated memory
  if (ResourceDescriptorList) {
    FreePool (ResourceDescriptorList);
  }
  return Status;
}

/**
   Create TDX DATA HOB

   @param[out] TdxDataHobPtr  - Pointer to the TDX Data HOB structure

   @retval  EFI_SUCCESS  TdxDataHobPtr has been initialized successfully.
   @retval  Others       HOB creation failed.
**/
EFI_STATUS
TdxHobInit (
  OUT TDX_DATA_HOB **TdxDataHobPtr
  )
{
  TDX_DATA_HOB *HobPtr;
  EFI_STATUS    Status;

  Status = EFI_SUCCESS;
  *TdxDataHobPtr = NULL;

  DEBUG ((DEBUG_INFO, "Trust Domain Extension (TDX) HOB Initialization\n"));

  //
  // Get TdxDataHob, if it is already present, reload the hob and exit
  //
  HobPtr = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
  if (HobPtr != NULL) {
    DEBUG ((DEBUG_INFO, "Trust Domain Extension (TDX) HOB is already initialized! exiting..\n"));
    *TdxDataHobPtr = HobPtr;
    return Status;
  }

  //
  // Create hob for storing TDX data
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TDX_DATA_HOB),
             (VOID **) &HobPtr
             );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  ZeroMem (&(HobPtr->Revision), sizeof (TDX_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE));
  HobPtr->EfiHobGuidType.Name = gTdxDataHobGuid;
  HobPtr->Revision = TDX_DATA_HOB_REVISION;
  DEBUG ((DEBUG_INFO, "TdxDataHob->EfiHobGuidType.Name: %g\n", &HobPtr->EfiHobGuidType.Name));

  *TdxDataHobPtr = HobPtr;
  return Status;
}

/**
  Reports if Tdx pre-requisite supported or not

  @param[in] MktmePolicy   - MkTme policy enable
  @param[in] VtdPolicy     - Vtd policy enable
  @param[in] VmxPolicy     - Vmx policy enable

  @retval TRUE             If TDX pre-requisite is supported
  @retval FALSE            If TDX pre-requisite is not supported
**/
BOOLEAN
CheckTdxDependancy (
  UINT32 MktmePolicy,
  UINT32 VtdPolicy,
  UINT32 VmxPolicy
 )
{
  CPUID_VERSION_INFO_ECX              CpuIdVersionInfoEcx;

  DEBUG ((DEBUG_INFO, "[TDX] %a \n", __FUNCTION__));

  // Detect TDX IP is supported
  if (IsTdxSupported () == FALSE )
  {
    DEBUG ((DEBUG_INFO, "TDX is not supported in the platform.\n"));
    DEBUG ((DEBUG_INFO, "Skip the TDX flow \n"));
    return FALSE;
  }

  // VTx - for launch of VMx.
  GetSupportedCpuFeatures ((UINT32 *) &CpuIdVersionInfoEcx);
  if (CpuIdVersionInfoEcx.Bits.VMX != 1) {
    DEBUG ((DEBUG_INFO, "[TDX] VTX is not supported.\n"));
    return FALSE;
  }

  // VMX enabled from Setup menu
  if (VmxPolicy == FALSE) {
    DEBUG ((DEBUG_INFO, "[TDX] Vmx is disabled.\n"));
    return FALSE;
  }

  // VTd - for I/O device virtualization
  if ((IsVtdSupported () == FALSE) || (VtdPolicy == FALSE)) {
    DEBUG ((DEBUG_INFO, "[TDX] VTD is not supported.\n"));
    return FALSE;
  }

  // SMX GetSec Instruction for launching the Intel TDx SEAMLDR.
  if (IsTxtProcessor () == FALSE) {
    DEBUG ((DEBUG_INFO, "[TDX] SMX is not supported.\n"));
    return FALSE;
  }

  // Detect MK-TME IP is supported in the platform
  if (IsMkTmeSupported () == FALSE) {
    DEBUG ((DEBUG_INFO, "[TDX] MK-TME is not supported.\n"));
    return FALSE;
  }

  // Check if MKTME is enabled
  if (MktmePolicy == FALSE) {
    DEBUG ((DEBUG_INFO, "[TDX] Tme is not enabled.\n"));
    return FALSE;
  }

  // Check Cache Level
  if (IsSufficientCacheAvailable () != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "[TDX] Cache level is not supported for TDX enablement\n"));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "[TDX] %a END\n", __FUNCTION__));
  return TRUE;
}


/**
  Perform Trust Domain Execution(Tdx) initialization.

  @param[in] TdxPolicy           - TDx policy enable
  @param[in] MrcData             - MRC Parameter Structure
  @param[in] TdxActmModuleAddr   - Tdx Actm Module address
  @param[in] TdxActmModuleSize   - Tdx Actm Module Size

  @retval VOID              - No value to return
**/
VOID
TdxInit (
  IN VOID *TdxPolicy,
  IN MrcParameters *CONST MrcData,
  IN UINTN  TdxActmModuleAddr,
  IN UINT32 TdxActmModuleSize,
  IN UINT8  TdxSeamldrSeSvn
  )
{

  TDX_DATA_HOB          *TdxDataHobPtr;
  EFI_STATUS            Status = EFI_SUCCESS;
  MrcOutput             *Outputs;
  BOOLEAN               TdxPreconditionsMet = FALSE;
  TDX_POLICY            *LocalTdxPolicy = NULL;
  UINT64                ActmStatus;
  EFI_PHYSICAL_ADDRESS  TempRamPtr;
  UINT64                *ActmRelocPointer;
  MrcMemoryMap          *MemoryMapData;

  Outputs = &MrcData->Outputs;
  MemoryMapData = &MrcData->Outputs.MemoryMapData;

  DEBUG ((DEBUG_INFO, "Trust Domain Extension (TDX) Initialization\n"));

  if (TdxPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "TdxPolicy is null, exiting\n"));
    return;
  } else {
    LocalTdxPolicy = (TDX_POLICY *) TdxPolicy;
  }
  // Check for Tdx Dependancy
  TdxPreconditionsMet = CheckTdxDependancy (LocalTdxPolicy->MktmeEnable,
                                            LocalTdxPolicy->VtdEnable,
                                            LocalTdxPolicy->VmxEnable
                                            );
  if (TdxPreconditionsMet == FALSE)
  {
    DEBUG ((DEBUG_INFO, "TDX pre-requisite technologies not supported.\n"));
    DEBUG ((DEBUG_INFO, "Skip the TDX flow \n"));
    return;
  }

  Status = TdxHobInit (
             &TdxDataHobPtr
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] Error creating TDX HOB, Status = %r\n", Status));
    return;
  }
  //
  // Update HOB values
  //
  TdxDataHobPtr->TdxEnabled = LocalTdxPolicy->TdxEnable;
  DEBUG ((DEBUG_INFO, " TDX_DATA_HOB: TdxEnabled: 0x%X\n", TdxDataHobPtr->TdxEnabled));

  // Check for Tdx enable Policy
  if (LocalTdxPolicy->TdxEnable == FALSE)
  {
    DEBUG ((DEBUG_INFO, "TDX Enable Policy is disabled.\n"));
    DEBUG ((DEBUG_INFO, "Skip the TDX flow \n"));
    return;
  }

  // Update SeamrrBaseAddress and SeamrrSize in HOB
  TdxDataHobPtr->SeamRrBaseAddress = MemoryMapData->SeamrrBase;
  TdxDataHobPtr->SeamrrSize = CalculateSeamrrSize (Outputs);

  //
  // Register for MemoryDiscover notification.
  //
  DEBUG ((DEBUG_INFO, "[TDX] Notify the CMR event from Tdx init\n"));
  Status = PeiServicesNotifyPpi(&mPpiPeiTdxMemoryNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] PeiServicesNotifyPpi Failed (%r)\n", Status));
    return;
  }

  //
  // Building Dimm manifest
  //
  if (Outputs->DdrType == MRC_DDR_TYPE_DDR5) {
    DEBUG ((DEBUG_INFO, "[TDX] Memory Type is DDR5 %a %d\n", __FUNCTION__, __LINE__));
    DEBUG ((DEBUG_INFO, "[TDX] Building Dimm Manifest.....\n"));
    Status = PublishActmDimmManifest(MrcData);
    if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TDX] Error building the Dimm Manifest Status = %r\n", Status));
    return;
    }
  } else {
    DEBUG ((DEBUG_INFO, "[TDX] Memory Type is LP5 %a %d\n", __FUNCTION__, __LINE__));
    DEBUG ((DEBUG_INFO, "[TDX] Not Building Dimm Manifest.....\n"));
    return;
  }

  //program Dimm manifest to TEE_INPUT MSR
  AsmWriteMsr64 (MSR_TEE_INPUT_PARAM, (UINT64)&TdxDataHobPtr->ActmDimmManifest);

  // Launching the ACTM
  DEBUG ((DEBUG_INFO, "[TDX] Allocate Temp ram for ACTM module\n"));

  Status = PeiServicesAllocatePages (
                   EfiLoaderCode,
                   EFI_SIZE_TO_PAGES (TdxActmModuleSize),
                   &TempRamPtr
                   );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[TDX] Page Allocation Failed, ACTM skipped\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Page Allocation Success, Address = 0x%X\n", TempRamPtr));
    ActmRelocPointer = (UINT64 *)((UINTN)TempRamPtr);

    CopyMem (
        ActmRelocPointer,
        (UINT64 *)((UINTN)TdxActmModuleAddr),
        TdxActmModuleSize
        );

    // Launching the ACTM, need to subtract BIOS image header size to obtain true ACTM module size
    DEBUG((DEBUG_INFO, "[TDX] Launching ACTM\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_TDX_ACM_ENTRY); //PostCode (0xD903)
    ActmStatus = AsmLaunchActm (TempRamPtr, TdxActmModuleSize - BIOS_IMAGE_HEADER_SIZE);
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_TDX_ACM_EXIT); //PostCode (0xD904)
    DEBUG((DEBUG_INFO, "[TDX] ACTM Return Status = 0x%x\n", ActmStatus));
  }

  Status = PeiServicesFreePages (TempRamPtr, EFI_SIZE_TO_PAGES (TdxActmModuleSize));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[TDX] PeiServicesFreePages failed (%r)\n", Status));
  }

  return;
}

/**
  Reserve Tdx Memory
  @param[in,out] TopUseableMemAddr       Top usable memory address
  @param[in,out] TopMemSize              Top of memory size
  @param[in]     ResourceAttributeTested Memory tested attribute
  @param[out]    BaseAddress             Seamrr Base Address
**/
VOID
PeiTdxMemoryAllocation(
  IN OUT EFI_PHYSICAL_ADDRESS      *TopUseableMemAddr,
  IN OUT UINT64                    *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributeTested,
  OUT EFI_PHYSICAL_ADDRESS         *BaseAddress
  )
{
  EFI_STATUS                  Status;
  UINT32                      SeamrrSize;
  EFI_PHYSICAL_ADDRESS        SeamRrBaseAddress;
  EFI_RESOURCE_TYPE           ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE ResourceAttribute;
  UINT64                      MemoryHoleLength = 0;

  // Program Seamrr Base and Mask MSR's
  if (IsTdxEnabled ()) {
    Status = GetSeamrrSize (&SeamrrSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[TDX] Fail to retrieve Seamrr Size GetSeamrrSize() - %r \n", Status));
      ASSERT_EFI_ERROR (Status);
    }

    if (SeamrrSize != 0) {
      ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
      ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                        ResourceAttributeTested |
                        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;

      //
      // SeamRr has 32MB base Alignment requirement
      //
      SeamRrBaseAddress = NaturalAlignment (*TopUseableMemAddr, SIZE_32MB);
      if (SeamRrBaseAddress != *TopUseableMemAddr) {
        MemoryHoleLength = SeamRrBaseAddress - *TopUseableMemAddr;
        BuildResourceDescriptorHob (
          ResourceType,         // MemoryType,
          ResourceAttribute,    // MemoryAttribute
          *TopUseableMemAddr,   // MemoryBegin
          MemoryHoleLength      // MemoryLength
          );
        BuildMemoryAllocationHob (
          *TopUseableMemAddr,
          MemoryHoleLength,
          EfiReservedMemoryType
          );
        *Touud -= MemoryHoleLength;
      }

      BuildResourceDescriptorHob (
        ResourceType,           // MemoryType,
        ResourceAttribute,      // MemoryAttribute
        SeamRrBaseAddress,      // MemoryBegin
        (SeamrrSize << 20)      // MemoryLength
      );

      BuildMemoryAllocationHob (
        SeamRrBaseAddress,
        (SeamrrSize << 20),
        EfiReservedMemoryType
      );

      *BaseAddress = SeamRrBaseAddress;

      *TopUseableMemAddr = SeamRrBaseAddress + (SeamrrSize << 20);
      *Touud -= ((UINT64)SeamrrSize << 20);
      DEBUG ((DEBUG_INFO, "SEAMRR BASE = 0x%016lx SIZE = 0x%x\n", SeamRrBaseAddress, SeamrrSize));

      Status = PeiCpuSetSeamrrRegion (SeamRrBaseAddress, (SeamrrSize << 20));
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "[TDX] Fail to program Seamrr Base and Mask MSR's %a %d \n", __FUNCTION__, __LINE__));
        ASSERT_EFI_ERROR (Status);
      }
      // Set the IMR for Seamrr (IMR26)
      Status = SetImr (IMR_SEAMRR, SeamRrBaseAddress, (SeamrrSize << 20));
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "[TDX] Fail to program Seamrr IMR %a %d \n", __FUNCTION__, __LINE__));
        ASSERT_EFI_ERROR (Status);
      }
    }
  } else {
    //
    // Tdx is disabled, lock the SEAMRR region.
    //
    Status = LockSeamrrMsr ();
    if (EFI_ERROR(Status) && Status != EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_INFO, "Fail to program Seamrr Mask MSR's %a \n", __FUNCTION__));
      ASSERT_EFI_ERROR (Status);
    }
  }
}

/**
  Reserve Tdx Memory

  @param[in] SeamrrBase               Seamrr Base Address
  @param[in] SeamrrSize               Seamrr Size
  @param[in] ResourceAttributeTested  Memory tested attribute
**/
VOID
PeiTdxMemoryAllocationV2 (
  IN EFI_PHYSICAL_ADDRESS         SeamrrBase,
  IN UINT64                       SeamrrSize,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
  )
{
  EFI_STATUS                         Status;
  EFI_RESOURCE_TYPE                  ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE        ResourceAttribute;

  // Program Seamrr Base and Mask MSR's
  if (IsTdxEnabled ()) {
    if ((SeamrrSize != 0) && (SeamrrBase != 0)) {
      ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
      ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                          ResourceAttributeTested |
                          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;

      BuildResourceDescriptorHob (
        ResourceType,           // MemoryType,
        ResourceAttribute,      // MemoryAttribute
        SeamrrBase,             // MemoryBegin
        SeamrrSize              // MemoryLength
      );

      BuildMemoryAllocationHob (
        SeamrrBase,
        SeamrrSize,
        EfiReservedMemoryType
      );

      Status = PeiCpuSetSeamrrRegion (SeamrrBase, (UINT32) SeamrrSize);
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "Fail to program Seamrr Base and Mask MSR's\n"));
        ASSERT_EFI_ERROR (Status);
      }
    }
  } else {
    //
    // Tdx is disabled, lock the SEAMRR region.
    //
    Status = LockSeamrrMsr ();
    if (EFI_ERROR(Status) && Status != EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_INFO, "Fail to program Seamrr Mask MSR's\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }
}

/**
  Reserve Tdx Memory and program IMR and MSR for SeamRr Below 4GB

**/
VOID
PeiTdxProgramImrBelow4Gb (
  VOID
  )
{
  EFI_STATUS                         Status;
  UINT32                             SeamrrSize;
  EFI_RESOURCE_TYPE                  ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE        ResourceAttribute;
  TDX_DATA_HOB                       *TdxDataHobPtr;
  EFI_PHYSICAL_ADDRESS               SeamRrBaseAddress;

  Status = EFI_SUCCESS;

  // Program Seamrr Base and Mask MSR's
  DEBUG ((DEBUG_INFO, "%a \n",__FUNCTION__));
  TdxDataHobPtr = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
  if (TdxDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "TdxInfoLib: Error getting TDX Data HOB!\n"));
    Status = EFI_NOT_READY;
    return;
  }
  //
  // SeamRrBaseaddress is already aligned
  //
  SeamRrBaseAddress = (TdxDataHobPtr->SeamRrBaseAddress) << 20;

  if (IsTdxEnabled ()) {
    Status = GetSeamrrSize (&SeamrrSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[TDX] Fail to retrieve Seamrr Size GetSeamrrSize() - %r \n", Status));
      ASSERT_EFI_ERROR (Status);
    }

    if (SeamrrSize != 0) {

      ResourceType = EFI_RESOURCE_MEMORY_RESERVED;

      ResourceAttribute = \
        EFI_RESOURCE_ATTRIBUTE_PRESENT |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE;

      BuildResourceDescriptorHob (
        ResourceType,                           // MemoryType,
        ResourceAttribute,                      // MemoryAttribute
        SeamRrBaseAddress,                      // MemoryBegin
        (SeamrrSize << 20)                      // MemoryLength
        );

      BuildMemoryAllocationHob (
        SeamRrBaseAddress,                      // MemoryBegin
        (SeamrrSize << 20),                     // MemoryLength
        EfiReservedMemoryType
        );

      DEBUG ((DEBUG_INFO, "SEAMRR BASE = 0x%016lx SIZE = 0x%x\n", SeamRrBaseAddress, SeamrrSize));

      Status = PeiCpuSetSeamrrRegion (SeamRrBaseAddress, (SeamrrSize << 20));
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "[TDX] Fail to program Seamrr Base and Mask MSR's %a %d \n", __FUNCTION__, __LINE__));
        ASSERT_EFI_ERROR (Status);
      }
      // Set the IMR for Seamrr (IMR26)
      Status = SetImr (IMR_SEAMRR, SeamRrBaseAddress, (SeamrrSize << 20));
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "[TDX] Fail to program Seamrr IMR %a %d \n", __FUNCTION__, __LINE__));
        ASSERT_EFI_ERROR (Status);
      }
    }
  } else {
    //
    // Tdx is disabled, lock the SEAMRR region.
    //
    Status = LockSeamrrMsr ();
    if (EFI_ERROR(Status) && Status != EFI_UNSUPPORTED) {
      DEBUG ((DEBUG_INFO, "Fail to program Seamrr Mask MSR's %a \n", __FUNCTION__));
      ASSERT_EFI_ERROR (Status);
    }
  }
}