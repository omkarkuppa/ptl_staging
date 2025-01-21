/** @file
  This file provids two functionalites related to Capsule process in PEI:
  1. Coalesce capsule before installing PeiMemory
  2. CreateState after permanent memory is ready.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Ppi/Capsule.h>
#include <Ppi/MasterBootMode.h>

#include <Library/MemoryInstallLib.h>
#include <Library/CapsulePpiProcessLib.h>

#define CAPSULE_BUFFER_INFO_SIGNATURE  SIGNATURE_32 ('C', 'P', 'B', 'F')

typedef struct {
  UINTN                        Signature;
  EFI_PEI_NOTIFY_DESCRIPTOR    MemoryDiscoveredNotifyList;
  VOID                         *CapsuleBase;
  UINTN                        CapsuleSize;
} PEI_CAPSULE_BUFFER_INFO;

#define PEI_CAPSULE_PPI_PROCESS_FROM_THIS_NOTIFY(a)  CR (a, PEI_CAPSULE_BUFFER_INFO, MemoryDiscoveredNotifyList, CAPSULE_BUFFER_INFO_SIGNATURE)

/**
  This is a callback function of gEfiPeiMemoryDiscoveredPpiGuid PPI installation.
  It invokes Capsule PPI CreateState function with preserved Capsule buffer.

  @param[in]  PeiServices       Indirect reference to the PEI Services Table.
  @param[in]  NotifyDescriptor  Address of the notification descriptor data structure.
  @param[in]  Ppi               Address of the PPI that was installed.

  @retval  EFI_SUCCESS  The function is executed successfully.
  @retval  Others       Some unexpected error occurs during the execution of this function.

**/
EFI_STATUS
EFIAPI
CapsuleCreateState (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS               Status;
  PEI_CAPSULE_BUFFER_INFO  *Private;
  PEI_CAPSULE_PPI          *Capsule;

  DEBUG ((DEBUG_INFO, "CapsuleCreateState callback - Entry\n"));

  Private = PEI_CAPSULE_PPI_PROCESS_FROM_THIS_NOTIFY (NotifyDescriptor);

  Capsule = NULL;
  Status  = PeiServicesLocatePpi (
              &gPeiCapsulePpiGuid,
              0,
              NULL,
              (VOID **)&Capsule
              );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate Capsule PPI - Exit\n"));
    return EFI_SUCCESS;
  }

  Status = Capsule->CreateState (
                      (EFI_PEI_SERVICES **)PeiServices,
                      Private->CapsuleBase,
                      Private->CapsuleSize
                      );

  DEBUG ((DEBUG_INFO, "CapsuleCreateState(%r) - Exit\n", Status));

  return EFI_SUCCESS;
}

/**
  This function tries to find the largest memory range excluding that given to PEI (PeiMemory).
  The memory range will used for putting coalesced capsule image buffer.

  @param[in]   MemoryMap             A pointer to the array of main memory ranges.
  @param[in]   NumRanges             Number of main memory ranges.
  @param[in]   PeiMemoryBaseAddress  The base address of PEI memory to be installed.
  @param[in]   PeiMemoryLength       The length of of PEI memory to be installed.
  @param[out]  CapsuleMemoryBase     A pointer to base address of the memory buffer for coalescing Capsule image.
  @param[out]  CapsuleMemSize        A pointer to the length of available memory buffer.

  @retval  EFI_SUCCESS            Found memory buffer for Capsule coalescing
  @retval  EFI_INVALID_PARAMETER  Input parameter is invalid
  @retval  Others                 Some unexpected error occurs during the execution of this function.

**/
EFI_STATUS
EFIAPI
FindProperBufferForCapsule (
  IN  PEI_DDR_MAIN_MEMORY_MAP_RANGE  *MemoryMap,
  IN  UINT8                          NumRanges,
  IN  EFI_PHYSICAL_ADDRESS           PeiMemoryBaseAddress,
  IN  UINT64                         PeiMemoryLength,
  OUT VOID                           **CapsuleMemoryBase,
  OUT UINTN                          *CapsuleMemSize
  )
{
  UINT8                 Index;
  UINT8                 *CapsuleBuffer;
  UINT64                CapsuleBufferLength;
  EFI_PHYSICAL_ADDRESS  PhysicalAddress;
  UINT64                RangeLength;
  UINT64                Section1Length;
  UINT64                Section2Length;

  DEBUG ((DEBUG_INFO, "FindProperBufferForCapsule - Entry\n"));

  if ((CapsuleMemoryBase == NULL) || (CapsuleMemSize == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "PEI memory base: 0x%X\n", PeiMemoryBaseAddress));
  DEBUG ((DEBUG_INFO, "PEI memory length: 0x%X\n", PeiMemoryLength));

  CapsuleBuffer       = NULL;
  CapsuleBufferLength = 0;

  //
  // Find the largest memory range excluding that given to PEI.
  // Assumption: PeiMemory range must not fall across two ranges.
  //
  for (Index = 0; Index < NumRanges; Index++) {
    PhysicalAddress = MemoryMap[Index].PhysicalAddress;
    RangeLength     = MemoryMap[Index].RangeLength;

    DEBUG ((DEBUG_INFO, "PhysicalAddress: 0x%X\n", PhysicalAddress));
    DEBUG ((DEBUG_INFO, "RangeLength    : 0x%X\n", RangeLength));

    if ((PhysicalAddress + RangeLength > MAX_ADDRESS)) {
      continue;
    }

    if ((PeiMemoryBaseAddress >= PhysicalAddress) &&
        ((PeiMemoryBaseAddress + PeiMemoryLength) <= (PhysicalAddress + RangeLength))) {
      //
      // This is the range contains PeiMemory
      //
      // |---Section 1---|---PeiMemory---|---Section 2---|
      // |               |               |               |<-PhysicalAddress + RangeLength
      // |               |               |<-PeiMemoryBaseAddress + PeiMemoryLength
      // |               |<-PeiMemoryBaseAddress
      // |<-PhysicalAddress
      //
      Section1Length = PeiMemoryBaseAddress - PhysicalAddress;
      Section2Length = (PhysicalAddress + RangeLength) - (PeiMemoryBaseAddress + PeiMemoryLength);

      //
      // Check Section 1
      //
      if (Section1Length > CapsuleBufferLength) {
        CapsuleBuffer       = (VOID *)((UINTN)PhysicalAddress);
        CapsuleBufferLength = Section1Length;
      }

      //
      // Check Section 2
      //
      if (Section2Length > CapsuleBufferLength) {
        CapsuleBuffer       = (VOID *)((UINTN)(PeiMemoryBaseAddress + PeiMemoryLength));
        CapsuleBufferLength = Section2Length;
      }
    } else if (RangeLength > CapsuleBufferLength) {
      //
      // This is the range which does not contain PeiMemory
      //
      CapsuleBuffer       = (VOID *)((UINTN)PhysicalAddress);
      CapsuleBufferLength = RangeLength;
    }
  }

  DEBUG ((DEBUG_INFO, "Capsule memory base: 0x%X\n", CapsuleBuffer));
  DEBUG ((DEBUG_INFO, "Capsule memory length: 0x%X\n", CapsuleBufferLength));

  *CapsuleMemoryBase = CapsuleBuffer;
  *CapsuleMemSize    = CapsuleBufferLength;

  return EFI_SUCCESS;
}

/**
  This function tries to coalesce capsule image buffer before calling
  PeiServicesInstallPeiMemory() when BootMode is BOOT_ON_FLASH_UPDATE mode.

  @param[in]  MemoryMap             A pointer to the array of main memory ranges.
  @param[in]  NumRanges             Number of main memory ranges.
  @param[in]  PeiMemoryBaseAddress  The base address of PEI memory to be installed.
  @param[in]  PeiMemoryLength       The length of of PEI memory to be installed.

**/
VOID
EFIAPI
CoalesceCapsulesIfNeeded (
  IN PEI_DDR_MAIN_MEMORY_MAP_RANGE  *MemoryMap,
  IN UINT8                          NumRanges,
  IN EFI_PHYSICAL_ADDRESS           PeiMemoryBaseAddress,
  IN UINT64                         PeiMemoryLength
  )
{
  EFI_STATUS               Status;
  EFI_BOOT_MODE            BootMode;
  CONST EFI_PEI_SERVICES   **PeiServices;
  PEI_CAPSULE_PPI          *Capsule;
  PEI_CAPSULE_BUFFER_INFO  *Private;
  VOID                     *CapsuleBuffer;
  UINTN                    CapsuleBufferLength;

  DEBUG ((DEBUG_INFO, "CoalesceCapsulesIfNeeded - Entry\n"));

#ifdef FSP_FLAG
  if (!IsCoalesceCapsuleNeeded ()) {
    DEBUG ((DEBUG_INFO, "No need to coalesce in FSP API mode - Exit\n"));
    return;
  }
#endif

  PeiServices         = GetPeiServicesTablePointer ();
  Capsule             = NULL;
  Private             = NULL;
  CapsuleBuffer       = NULL;
  CapsuleBufferLength = 0;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode != BOOT_ON_FLASH_UPDATE) {
    DEBUG ((DEBUG_INFO, "No need to coalesce in non BOOT_ON_FLASH_UPDATE mode - Exit\n"));
    return;
  }

  Status = PeiServicesLocatePpi (
             &gPeiCapsulePpiGuid,
             0,
             NULL,
             (VOID **)&Capsule
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate Capsule PPI - Exit\n"));
    return;
  }

  Status = FindProperBufferForCapsule (
             MemoryMap,
             NumRanges,
             PeiMemoryBaseAddress,
             PeiMemoryLength,
             &CapsuleBuffer,
             &CapsuleBufferLength
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to find memory buffer for capsule coalescing - Exit\n"));
    return;
  }

  //
  // Call Capsule PPI Coalesce function to coalesce the capsule data.
  //
  Status = Capsule->Coalesce ((EFI_PEI_SERVICES **)PeiServices, &CapsuleBuffer, &CapsuleBufferLength);
  DEBUG ((DEBUG_INFO, "Capsule->Coalesce: %r\n", Status));

  if (Status == EFI_SUCCESS) {
    //
    // Register notify callback to create Capsule HOB once permanent memory is ready.
    //
    Private = (PEI_CAPSULE_BUFFER_INFO *)AllocateZeroPool (sizeof (PEI_CAPSULE_BUFFER_INFO));
    if (Private != NULL) {
      Private->Signature   = CAPSULE_BUFFER_INFO_SIGNATURE;
      Private->CapsuleBase = CapsuleBuffer;
      Private->CapsuleSize = CapsuleBufferLength;

      Private->MemoryDiscoveredNotifyList.Flags  = (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
      Private->MemoryDiscoveredNotifyList.Guid   = &gEfiPeiMemoryDiscoveredPpiGuid;
      Private->MemoryDiscoveredNotifyList.Notify = CapsuleCreateState;

      Status = PeiServicesNotifyPpi (&Private->MemoryDiscoveredNotifyList);
      DEBUG ((DEBUG_INFO, "Notify MemoryDiscovered PPI: %r\n", Status));
      DEBUG ((DEBUG_INFO, "CapsuleBase: 0x%X\n", Private->CapsuleBase));
      DEBUG ((DEBUG_INFO, "CapsuleSize: 0x%X\n", Private->CapsuleSize));
    }
  }

  DEBUG ((DEBUG_INFO, "CoalesceCapsulesIfNeeded - Exit\n"));

  return;
}
