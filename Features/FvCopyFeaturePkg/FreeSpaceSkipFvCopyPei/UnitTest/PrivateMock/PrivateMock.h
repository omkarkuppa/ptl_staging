/** @file
  Mock SPI DMA Ppi and __wrap_PeiServicesLocatePpi

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
#ifdef ENABLE_UNIT_TESTING
#ifndef _SPI_DMA_MOCK_H_
#define _SPI_DMA_MOCK_H_

#include <Uefi.h>
#include <PiPei.h>

BOOLEAN                              SpiDmaInstalled;
BOOLEAN                              FvShadowInstalled;
BOOLEAN                              FreeSpaceSkipReplacedinFvShadow;

EFI_STATUS
MockSpiDmaRead (
  IN VOID                     *FlashAddress,
  IN VOID                     *Destination,
  IN UINTN                    ReadSize
  );

/**
 * This is a mocked Object!
  This service enables PEIMs to discover a given instance of an interface.

  @param  Guid                  A pointer to the GUID whose corresponding interface needs to be
                                found.
  @param  Instance              The N-th instance of the interface that is required.
  @param  PpiDescriptor         A pointer to instance of the EFI_PEI_PPI_DESCRIPTOR.
  @param  Ppi                   A pointer to the instance of the interface.

  @retval EFI_SUCCESS           The interface was successfully returned.
  @retval EFI_NOT_FOUND         The PPI descriptor is not found in the database.

**/
EFI_STATUS
EFIAPI
__wrap_PeiServicesLocatePpi (
  IN CONST EFI_GUID              *Guid,
  IN UINTN                       Instance,
  IN OUT EFI_PEI_PPI_DESCRIPTOR  **PpiDescriptor  OPTIONAL,
  IN OUT VOID                    **Ppi
  );

/**
 * This is a mocked Object!
  This service enables PEIMs to replace an entry in the PPI database with an alternate entry.

  @param  OldPpi                Pointer to the old PEI PPI Descriptors.
  @param  NewPpi                Pointer to the new PEI PPI Descriptors.

  @retval EFI_SUCCESS           The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER The OldPpi or NewPpi is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.
  @retval EFI_NOT_FOUND         The PPI for which the reinstallation was requested has not been
                                installed.

**/
EFI_STATUS
EFIAPI
__wrap_PeiServicesReInstallPpi (
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *OldPpi,
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *NewPpi
  );

/**
 * This is a mocked Object
  This service enables a given PEIM to register an interface into the PEI Foundation.

  @param  PpiList               A pointer to the list of interfaces that the caller shall install.

  @retval EFI_SUCCESS           The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.

**/
EFI_STATUS
EFIAPI
__wrap_PeiServicesInstallPpi (
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *PpiList
);


EFI_STATUS
EFIAPI
Test_PeiServices (
  VOID
);


#pragma comment(linker, "/ALTERNATENAME:__real_PeiServicesLocatePpi=PeiServicesLocatePpi")
#define PeiServicesLocatePpi __real_PeiServicesLocatePpi
#undef PeiServicesLocatePpi
#define PeiServicesLocatePpi __wrap_PeiServicesLocatePpi

#pragma comment(linker, "/ALTERNATENAME:__real_PeiServicesInstallPpi=PeiServicesInstallPpi")
#define PeiServicesInstallPpi __real_PeiServicesInstallPpi
#undef PeiServicesInstallPpi
#define PeiServicesInstallPpi __wrap_PeiServicesInstallPpi

#pragma comment(linker, "/ALTERNATENAME:__real_PeiServicesReInstallPpi=PeiServicesReInstallPpi")
#define PeiServicesReInstallPpi __real_PeiServicesReInstallPpi
#undef PeiServicesReInstallPpi
#define PeiServicesReInstallPpi __wrap_PeiServicesReInstallPpi

#endif //_SPI_DMA_MOCK_H_
#endif // ENABLE_UNIT_TESTING
