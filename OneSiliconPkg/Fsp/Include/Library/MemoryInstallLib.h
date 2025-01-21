
/** @file
  Memory Install Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
  );

/**
  This function updates the return status of the FSP API with requested reset type and returns to Boot Loader.

  @param[in] FspResetType     Reset type that needs to returned as API return status

**/
VOID
EFIAPI
FspApiReturnCheck (
  IN EFI_STATUS  FspResetType
  );


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
  );


/**
  This function checks whether the Coalesce Capsule feature is needed.
  @retval TRUE            Coalesce Capsule feature is needed.
  @retval FALSE           Coalesce Capsule feature is not needed.
**/
BOOLEAN
EFIAPI
IsCoalesceCapsuleNeeded (
  );


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
);


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
);
