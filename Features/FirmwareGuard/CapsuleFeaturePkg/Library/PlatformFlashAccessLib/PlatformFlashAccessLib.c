/** @file
  Platform Flash Access library to update different system firmware components

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DxeMeLib.h>
#include <Library/TimerLib.h>
#include <Library/BiosUpdateHelpersLib.h>

#include <Protocol/Spi.h>
#include <Library/HobLib.h>

#include <BiosGuard.h>
#include <Library/CpuPlatformLib.h>

#include "PlatformFlashAccessLibInternal.h"

PCH_SPI_PROTOCOL                               *mSpiProtocol = NULL;

/**
  Locate PCH SpiProtocol interface module pointer.

  @retval EFI_SUCCESS    PCH_SPI_PROTOCOL was successfully located.
  @retval EFI_NOT_FOUND  PCH_SPI_PROTOCOL was not found.
**/
EFI_STATUS
LocateSpiProtocolInterface (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  if (mSpiProtocol == NULL) {
    Status = gBS->LocateProtocol (
                    &gPchSpiProtocolGuid,
                    NULL,
                    (VOID **)&mSpiProtocol
                    );
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/*
  Get and return the pointer of SPI protocol.

  @param[in]    VOID

  @retval       PCH_SPI_PROTOCOL    Pointer point to the SPI procotol.
*/
PCH_SPI_PROTOCOL*
EFIAPI
GetPchSpiProtocol (
  VOID
  )
{
  LocateSpiProtocolInterface ();
  return mSpiProtocol;
}

/**
  Generic interface for read data from SPI flash.

  @param[in]  FLASH_ACCESS_REQUEST     Request strucutre for read.

  @retval     EFI_SUCCESS.             Operation is successful.
  @retval     EFI_OUT_OF_RESOURCES     Failed to allocate needed memory buffer.
  @retval     EFI_VOLUME_CORRUPTED     The block is not updated as expected.
  @retval     Others                   If there is any device errors.

**/
EFI_STATUS
EFIAPI
FlashRead (
  IN FLASH_ACCESS_REQUEST  *ReadReq
  )
{
  EFI_STATUS  Status;

  Status = LocateSpiProtocolInterface ();
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }
  //
  // Compare Buffer block with the destination.
  //    Caution! If the library is used in Resilience udpate, FlashRead shouldn't use MMIO read but has to directly use sSPI address
  //
  Status = mSpiProtocol->FlashRead (
                           mSpiProtocol,
                           ReadReq->FlashRegionType,
                           (UINT32)ReadReq->FlashAddress,
                           (UINT32)ReadReq->DataSize,
                           ReadReq->Data
                           );

  return Status;
}

/**
  Interface for flash update, call internal function to update flash.

  @param[in]  WriteReq                 Request information for update flash.

  @retval     EFI_SUCCESS.             Operation is successful.
  @retval     EFI_OUT_OF_RESOURCES     Failed to allocate needed memory buffer.
  @retval     EFI_VOLUME_CORRUPTED     The block is not updated as expected.
  @retval     Others                   If there is any device errors.

**/
EFI_STATUS
EFIAPI
FlashUpdate (
  IN FLASH_ACCESS_REQUEST                           *WriteReq,
  IN EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,        OPTIONAL
  IN UINTN                                          StartPercentage,
  IN UINTN                                          EndPercentage
  )
{
  EFI_STATUS  Status;
  BOOLEAN     BiosGuardEnabled;

  BiosGuardEnabled = IsBiosGuardEnabled ();

  //
  // This is an example shows how to implement a generic Flash Read/Write Interface
  //
  if (   BiosGuardEnabled \
      && (WriteReq->BgupImage     != NULL) \
      && (WriteReq->BgupImageSize != 0)
      ) {
    Status = InternalBiosGuardUpdate (WriteReq, Progress, StartPercentage, EndPercentage);
  } else {
    Status = InternalSpiFlashUpdate (WriteReq, Progress, StartPercentage, EndPercentage);
  }

  return Status;
}
