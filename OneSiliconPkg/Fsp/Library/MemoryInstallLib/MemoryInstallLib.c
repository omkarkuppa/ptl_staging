/** @file
  Memory Install Library.

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
#include <Library/PeiServicesLib.h>
#include <Ppi/FspmArchConfigPpi.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/FspCommonLib.h>
#include <Library/FspSwitchStackLib.h>

/**
  Get the S3 Data Pointer.

  This function returns the S3 Data pointer from FspmArchConfigPpi.

  @param[in, out] S3DataPtr    Pointer to the S3 Data Pointer

  @retval         EFI_SUCCESS  S3 Data Pointer is retrieved and stored
  @retval         EFI_STATUS   In all other cases

**/
EFI_STATUS
EFIAPI
GetS3DataPtr (
 IN OUT VOID **S3DataPtr
 )
{
  FSPM_ARCH_CONFIG_PPI *FspmArchConfigPpi;
  EFI_STATUS Status;

  //
  // Locate FSP-M Arch Config PPI for Memory Configuration Save Data.
  //
  FspmArchConfigPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gFspmArchConfigPpiGuid,
             0,
             NULL,
             (VOID **)&FspmArchConfigPpi
             );
  if ((!EFI_ERROR (Status)) && (FspmArchConfigPpi != NULL))
  {
    *S3DataPtr = FspmArchConfigPpi->NvsBufferPtr;
    return EFI_SUCCESS;
  }
  return Status;
}

/**
  This function updates the return status of the FSP API with requested reset type and returns to Boot Loader.

  @param[in] Status     Returns the API return status

**/
VOID
EFIAPI
FspApiReturnCheck (
  IN EFI_STATUS  Status
  )
{
  volatile BOOLEAN  LoopUntilReset;

  LoopUntilReset = TRUE;
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
    ///
    /// Below code is not an infinite loop.The control will go back to API calling function in BootLoader each time BootLoader
    /// calls the FSP API as FSP can't proceed after a Memory Initialization Failure.
    ///
    do {
      SetFspApiReturnStatus (Status);
      Pei2LoaderSwitchStack ();
      DEBUG ((DEBUG_ERROR, "!!!ERROR: FspMemoryInitApi() - Initialization Failure - 0x%x\n", Status));
    } while (LoopUntilReset);
  }
}

/**
  This function will allocate memory for FSP PEI execution.
  @param[in]     PeiMemoryLength         The largest available PEI memory to allocate.
  @param[in,out] RequiredMemSize         Pointer to required Pei Memory Size
  @param[in,out] BootLoaderTolumSize     Pointer to size of memory to be reserved by FSP below "top
                                         of low usable memory" for bootloader usage

**/
VOID
EFIAPI
RetrieveMemorySizeInfo (
  IN UINT64 PeiMemoryLength,
  IN OUT UINTN *RequiredMemSize,
  IN OUT UINT32 *BootLoaderTolumSize
  )
{
  FSPM_ARCH_CONFIG_PPI *FspmArchConfigPpi;
  EFI_STATUS Status;

  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
    *RequiredMemSize = PcdGet32 (PcdFspReservedMemoryLength);
    FspmArchConfigPpi = NULL;
    *BootLoaderTolumSize = 0;
    Status = PeiServicesLocatePpi (
               &gFspmArchConfigPpiGuid,
               0,
               NULL,
               (VOID **) &FspmArchConfigPpi
               );
    if (!EFI_ERROR (Status)) {
      *BootLoaderTolumSize = FspmArchConfigPpi->BootLoaderTolumSize;
    }
    if ((*RequiredMemSize + *BootLoaderTolumSize) > PeiMemoryLength) {
      DEBUG ((DEBUG_ERROR, "Error: BootLoaderTolumSize Greater than available memory.\n"));
      DEBUG ((DEBUG_ERROR, "Force BootLoaderTolumSize to zero.\n"));
      *BootLoaderTolumSize = 0;
    }
  } else {
    *RequiredMemSize = 0;
    *BootLoaderTolumSize = 0;
  }
}

/**
  This function checks whether the Coalesce Capsule feature is needed.
  @retval TRUE            Coalesce Capsule feature is needed.
  @retval FALSE           Coalesce Capsule feature is not needed.
**/
BOOLEAN
EFIAPI
IsCoalesceCapsuleNeeded (
  )
{
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
    //
    // In FspApi mode, CapsulePpi->Coalesce and CreateState is executed outside of FSP.
    //
    return FALSE;
  }
  return TRUE;
}

/**
  This function will calculate the PEI memory base address and length and will also report the size of the hob that FSP reserves.
  @param[in]  PeiMemoryLength               The largest available PEI memory to allocate.
  @param[in]  PeiMemoryBaseAddress          The base address of largest available PEI Memory available.
  @param[in]  BootLoaderTolumSize           Size of memory to be reserved by FSP below "top
                                            of low usable memory" for bootloader usage.
  @param[in]  RequiredMemSize               Required PEI Memory Size.
  @param[out] InstalledPeiMemoryBaseAddress Pointer to the FSP PEI memory base address.
  @param[out] InstalledPeiMemoryLength      Pointer to the FSP PEI memory size.
  @param[out] FspReportedHobMemoryLength    Pointer to the size of the hob reserved for FSP.
**/
VOID
EFIAPI
GetInstalledPeiMemoryInfo (
  IN  UINT64               PeiMemoryLength,
  IN  EFI_PHYSICAL_ADDRESS PeiMemoryBaseAddress,
  IN  UINT32               BootLoaderTolumSize,
  IN  UINTN                RequiredMemSize,
  OUT EFI_PHYSICAL_ADDRESS *InstalledPeiMemoryBaseAddress,
  OUT UINT64               *InstalledPeiMemoryLength,
  OUT UINT64               *FspReportedHobMemoryLength
)
{
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
    *InstalledPeiMemoryBaseAddress = PeiMemoryBaseAddress + PeiMemoryLength - BootLoaderTolumSize - RequiredMemSize;
    *InstalledPeiMemoryLength = RequiredMemSize;
    *FspReportedHobMemoryLength = BootLoaderTolumSize + RequiredMemSize;
  }
}

/**
  This function will report FSP reserved memory hobs.
  @param[in] BootLoaderTolumBase        The base address of the memory to be reserved by FSP below "top
                                        of low usable memory" for bootloader usage
  @param[in] BootLoaderTolumSize        The size the of memory to be reserved by FSP below "top
                                        of low usable memory" for bootloader usage
  @param[in] FspPeiMemoryBase           FSP PEI memory base address.
  @param[in] FspPeiMemoryLength         FSP PEI memory size.
  @param[in] ResourceAttributeTested    flag to indicate whether memory is tested or not.
**/
EFI_STATUS
EFIAPI
InstallFspHobs (
  IN  EFI_PHYSICAL_ADDRESS BootLoaderTolumBase,
  IN  UINT32               BootLoaderTolumSize,
  IN  EFI_PHYSICAL_ADDRESS FspPeiMemoryBase,
  IN  UINT64               FspPeiMemoryLength,
  IN  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
)
{
  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) { // FSP_MODE_CHECK
    DEBUG ((DEBUG_INFO, "FSP_RESERVED_MEMORY_RESOURCE_HOB\n"));
    //
    // Build HOB that describes at BootLoader TOLUM Resource reserved at top of main memory below 4GB.
    //
    if (BootLoaderTolumSize > 0) {
      DEBUG ((DEBUG_INFO, "FSP_BOOT_LOADER_TOLUM_HOB\n"));
      DEBUG ((DEBUG_INFO, "FspBootLoaderTolum Base : %x\n", BootLoaderTolumBase));
      DEBUG ((DEBUG_INFO, "FspBootLoaderTolum Size : %x\n", BootLoaderTolumSize));
      BuildResourceDescriptorWithOwnerHob (
        EFI_RESOURCE_MEMORY_RESERVED,             // MemoryType,
        (
          EFI_RESOURCE_ATTRIBUTE_PRESENT |
          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
          ResourceAttributeTested |
          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
          ),
        BootLoaderTolumBase,
        BootLoaderTolumSize,
        &gFspBootLoaderTolumHobGuid
        );
    }
    //
    // Build HOB that describes FSP reserved memory below BootLoader TOLUM.
    //
    DEBUG ((DEBUG_INFO, "FspReservedMemoryResource Base : %x\n", FspPeiMemoryBase));
    DEBUG ((DEBUG_INFO, "FspReservedMemoryResource Size : %x\n", FspPeiMemoryLength));
    BuildResourceDescriptorWithOwnerHob (
      EFI_RESOURCE_MEMORY_RESERVED,             // MemoryType,
      (
        EFI_RESOURCE_ATTRIBUTE_PRESENT |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
        ResourceAttributeTested |
        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
        EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
        EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
        EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
        ),
      FspPeiMemoryBase,
      FspPeiMemoryLength,
      &gFspReservedMemoryResourceHobGuid
      );
      return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}
