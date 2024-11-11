/** @file
  Mock Free Space Skip Fv Copy Library

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Ppi/SpiDma.h>
#include <PrivateMock/PrivateMock.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/FirmwareVolumeShadowPpi.h>

SPI_DMA_READ_PPI                     SpiDmaPpi;
EDKII_PEI_FIRMWARE_VOLUME_SHADOW_PPI FvShadowPpi;

EFI_STATUS
MockSpiDmaRead (
  IN VOID                     *FlashAddress,
  IN VOID                     *Destination,
  IN UINTN                    ReadSize
  )
{
  DEBUG ((DEBUG_INFO,  "Mocking %a is called\r\n",__FUNCTION__));
  CopyMem(Destination, FlashAddress, ReadSize);
  return EFI_SUCCESS;
}

EFI_STATUS
MockFvShadow (
  IN EFI_PHYSICAL_ADDRESS  FirmwareVolumeBase,
  IN VOID                  *Destination,
  IN UINTN                 DestinationLength
  )
{
  DEBUG ((DEBUG_INFO,  "Mocking %a is called\r\n",__FUNCTION__));
  CopyMem(Destination, (VOID *)FirmwareVolumeBase, DestinationLength);
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS SpiDmaPpiStatus;
  EFI_STATUS FvShadowPpiStatus;
  DEBUG ((DEBUG_INFO,  "Mocking %a is called\r\n",__FUNCTION__));

  SpiDmaPpiStatus = mock_type(EFI_STATUS);
  FvShadowPpiStatus = mock_type(EFI_STATUS);
  if (CompareGuid( Guid, &gSpiDmaReadPpiGuid) && SpiDmaPpiStatus==EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,  "Mocking %a SPI DMA Ppi Found\r\n",__FUNCTION__));
    SpiDmaPpi.SpiDmaRead = MockSpiDmaRead;
    *Ppi = &SpiDmaPpi;
    return SpiDmaPpiStatus;
  }
  else if (CompareGuid( Guid, &gEdkiiPeiFirmwareVolumeShadowPpiGuid) && FvShadowPpiStatus==EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,  "Mocking %a FvShadow Ppi Found\r\n",__FUNCTION__));
    FvShadowPpi.FirmwareVolumeShadow = MockFvShadow;
    *Ppi = &FvShadowPpi;
    return FvShadowPpiStatus;
  }
  DEBUG ((DEBUG_INFO,  "Mocking %a Ppi not Found\r\n",__FUNCTION__));
  return EFI_NOT_FOUND;
}

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
) {
  EFI_STATUS SpiDmaPpiStatus;
  EFI_STATUS FvShadowPpiStatus;
  DEBUG ((DEBUG_INFO,  "Mocking %a is called\r\n",__FUNCTION__));

  SpiDmaPpiStatus = mock_type(EFI_STATUS);
  FvShadowPpiStatus = mock_type(EFI_STATUS);
  if (CompareGuid( PpiList->Guid, &gSpiDmaReadPpiGuid) && SpiDmaPpiStatus==EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO,  "Mocking %a SPI DMA Ppi Installed\r\n",__FUNCTION__));
    SpiDmaPpi.SpiDmaRead = ((SPI_DMA_READ_PPI *)PpiList->Ppi)->SpiDmaRead;
    SpiDmaInstalled = TRUE;
    return SpiDmaPpiStatus;
  } else if (CompareGuid( PpiList->Guid, &gEdkiiPeiFirmwareVolumeShadowPpiGuid) && FvShadowPpiStatus==EFI_SUCCESS){
    DEBUG ((DEBUG_INFO,  "Mocking %a FvShadow Ppi Installed\r\n",__FUNCTION__));
    FvShadowPpi.FirmwareVolumeShadow = ((EDKII_PEI_FIRMWARE_VOLUME_SHADOW_PPI *)PpiList->Ppi)->FirmwareVolumeShadow;
    FvShadowInstalled = TRUE;
    return FvShadowPpiStatus;
  }
  DEBUG ((DEBUG_INFO,  "Mocking %a Ppi not Found\r\n",__FUNCTION__));
  return EFI_NOT_FOUND;
}

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
  )
{
  FreeSpaceSkipReplacedinFvShadow = TRUE;
  FvShadowPpi.FirmwareVolumeShadow = ((EDKII_PEI_FIRMWARE_VOLUME_SHADOW_PPI *)NewPpi->Ppi)->FirmwareVolumeShadow;
  DEBUG ((DEBUG_INFO,  "Mocking %a is called. FvShadow replacing SPI DMA with FreeSpaceSkip\r\n",__FUNCTION__));
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
Test_PeiServices (
  VOID
) {
  EFI_STATUS           SpiDmaStatus;
  SPI_DMA_READ_PPI     *SpiDmaPpi_tmp;

  SpiDmaInstalled = FALSE;
  FvShadowInstalled = FALSE;
  FreeSpaceSkipReplacedinFvShadow = FALSE;
  DEBUG ((DEBUG_INFO, "Test_PeiServices Start\r\n"));
  SpiDmaStatus = PeiServicesLocatePpi (
                 &gSpiDmaReadPpiGuid,
                 0,
                 NULL,
                 (VOID **) &SpiDmaPpi_tmp
                 );
  if (EFI_ERROR (SpiDmaStatus)) {
    DEBUG ((DEBUG_INFO,  "SPI DMA Ppi not Found\r\n"));
  }
  return SpiDmaStatus;
}