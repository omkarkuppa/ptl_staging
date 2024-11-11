/** @file
  TraceHub Utility Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <TraceHubDataHob.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiTraceHubInitLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/TraceHubRegs.h>
#include <Defines/PchReservedResources.h>
#include <Library/PeiImrInitLib.h>
#include <Library/MemorySubSystemInfoLib.h>

/**
  Calculate total trace buffer size and adjust it to be power of two to utilize least dynamic MTRR entry.

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance

  @retval UINT64                             Total size of trace buffers
**/
STATIC
UINT64
TraceHubCalculateTotalBufferSize (
  IN SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                   Status;
  UINT64                       TotalTraceBufferSize;
  TRACE_HUB_PREMEM_CONFIG      *TraceHubPreMemConfig;

  TotalTraceBufferSize = 0;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTraceHubPreMemConfigGuid, (VOID *) &TraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  if (TraceHubPreMemConfig->TraceHub[SocTraceHub].EnableMode != TraceHubModeDisabled) {
    TotalTraceBufferSize += (TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg0Size + TraceHubPreMemConfig->TraceHub[SocTraceHub].MemReg1Size);
  }
  if (TraceHubPreMemConfig->TraceHub[PchTraceHub].EnableMode != TraceHubModeDisabled) {
    TotalTraceBufferSize += (TraceHubPreMemConfig->TraceHub[PchTraceHub].MemReg0Size + TraceHubPreMemConfig->TraceHub[PchTraceHub].MemReg1Size);
  }
  //
  // Make TraceHub total size to be power of 2
  //
  if (TotalTraceBufferSize > GetPowerOfTwo64 (TotalTraceBufferSize)) {
    DEBUG ((DEBUG_INFO, "Total TraceHub memory = 0x%lx is not power of 2\n", TotalTraceBufferSize));
    TotalTraceBufferSize = 2 * GetPowerOfTwo64 (TotalTraceBufferSize);
  }
  if (TotalTraceBufferSize > 0) {
    DEBUG ((DEBUG_INFO, "Required total TraceHub memory = 0x%lx\n", TotalTraceBufferSize));
  } else {
    DEBUG ((DEBUG_INFO, "TraceHub memory is not required.\n"));
  }
  return TotalTraceBufferSize;
}

/**
  Create and initialize TraceHub Data HOB

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
  @param[in] FwBase            TraceHub Firmware BAR
**/
VOID
TraceHubDataHobInit (
  IN SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi,
  IN UINT32                      FwBase
  )
{
  EFI_STATUS                     Status;
  TRACEHUB_DATA_HOB              *TraceHubDataHob;
  UINT8                          TraceVerbosity;

  ///
  /// Create HOB for TraceHub Data HOB
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TRACEHUB_DATA_HOB),
             (VOID **) &TraceHubDataHob
             );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Initialize HOB data
  ///
  CopyGuid (&TraceHubDataHob->EfiHobGuidType.Name, &gTraceHubDataHobGuid);
  DEBUG ((DEBUG_INFO, "TraceHubDataHob->EfiHobGuidType.Name: %g\n", &TraceHubDataHob->EfiHobGuidType.Name));
  ZeroMem (&(TraceHubDataHob->TraceHubMemBase), sizeof (TRACEHUB_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE));
  TraceHubDataHob->TraceHubMemSize = TraceHubCalculateTotalBufferSize (SiPreMemPolicyPpi);
  ///
  /// HOB init with zero memory, before TraceHub initialization
  /// enable all verbosity and FW base to let traces pass thru.
  ///
  TraceVerbosity = B_TRACE_HUB_MEM_SCRPD2_BIOS_TRACE_EN | (V_TRACE_HUB_MEM_SCRPD2_TRACE_ALL << N_TRACE_HUB_MEM_SCRPD2_TRACE_VERBO);
  UpdateTraceHubDataHob (TraceVerbosity, FwBase);

  DEBUG ((DEBUG_INFO, "TraceHubDataHob @ %X\n", TraceHubDataHob));
  DEBUG ((DEBUG_INFO, "TraceHubDataHobSize - HobHeader: %X\n", sizeof (TRACEHUB_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "TraceHubDataHobSize: %X\n", sizeof (TRACEHUB_DATA_HOB)));
}

/**
  Store trace verbosity value into HOB

  @param[in] TraceVerbosity BIOS trace verbosity
  @param[in] FwBase         TraceHub Firmware BAR
**/
VOID
UpdateTraceHubDataHob (
  IN UINT8           TraceVerbosity,
  IN UINT32          FwBase
  )
{
  TRACEHUB_DATA_HOB                     *TraceHubDataHob;

  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);
  ASSERT (TraceHubDataHob != NULL);
  TraceHubDataHob->TraceVerbosity = TraceVerbosity;
  TraceHubDataHob->TraceHubFwBase = FwBase;
  DEBUG ((DEBUG_INFO, "TraceVerbosity: %02x\n", TraceHubDataHob->TraceVerbosity));
  DEBUG ((DEBUG_INFO, "TraceHubFwBase: %08x\n", TraceHubDataHob->TraceHubFwBase));
}

/**
  Reserve Trace Hub Memory

  @param[in] TopUseableMemAddr  Top usable memory address
  @param[in] TopMemSize         Top of memory size
**/
VOID
PeiTraceHubMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud
)
{
  EFI_STATUS                            Status;
  TRACEHUB_DATA_HOB                     *TraceHubDataHob;
  EFI_RESOURCE_TYPE                     ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE           ResourceAttribute;

  TraceHubDataHob = NULL;

  //
  // Locate the Hob structures
  //
  TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);
  if ((TraceHubDataHob != NULL) && (TraceHubDataHob->TraceHubMemSize != 0)) {
    //
    // TraceHub memory is required
    //
    TraceHubDataHob->TraceHubMemBase = NaturalAlignment (*TopUseableMemAddr, TraceHubDataHob->TraceHubMemSize);
    ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
    ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                        EFI_RESOURCE_ATTRIBUTE_TESTED |
                        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE;
    BuildResourceDescriptorWithOwnerHob (
      ResourceType,
      ResourceAttribute,
      TraceHubDataHob->TraceHubMemBase,
      TraceHubDataHob->TraceHubMemSize,
      &gFspTraceHubMemoryResourceHobGuid
      );
    BuildMemoryAllocationHob (
      TraceHubDataHob->TraceHubMemBase,
      TraceHubDataHob->TraceHubMemSize,
      EfiReservedMemoryType
      );
    DEBUG ((DEBUG_INFO, "TraceHub memory base = 0x%lx Size = 0x%lx\n", TraceHubDataHob->TraceHubMemBase, TraceHubDataHob->TraceHubMemSize));
    Status = SetImr (IMR_NPK, TraceHubDataHob->TraceHubMemBase, TraceHubDataHob->TraceHubMemSize);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Fail to program TraceHub IMR\n"));
    }
    *TopUseableMemAddr = TraceHubDataHob->TraceHubMemBase + TraceHubDataHob->TraceHubMemSize;
    *Touud -= TraceHubDataHob->TraceHubMemSize;
  }
}
