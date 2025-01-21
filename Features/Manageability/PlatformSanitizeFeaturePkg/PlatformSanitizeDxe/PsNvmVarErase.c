/**@file
  Platform Sanitization Dxe driver to perform NVM erase.

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/Spi.h>
#include <Guid/VariableFormat.h>

/**
  Clear All BIOS NVM Variable Region using SPI Protocols

  @retval EFI_SUCCESS           Cleared BIOS NVM Variables successfully
  @retval EFI_UNSUPPORTED       Flash update funtion failed
**/
EFI_STATUS
ClearAllBiosNvmVariableRegion (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINT32                      HeaderSize;
  UINT8                       *FvHeaderStore;
  PCH_SPI_PROTOCOL            *SpiProtocol;

  FvHeaderStore = NULL;

  Status = gBS->LocateProtocol (
                  &gPchSpiProtocolGuid,
                  NULL,
                  (VOID **) &SpiProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_ERROR, "PS: Failed to locate gPchSpiProtocolGuid.\n"));
    return EFI_UNSUPPORTED;
  }

  HeaderSize = sizeof (EFI_FIRMWARE_VOLUME_HEADER) + sizeof (EFI_FV_BLOCK_MAP_ENTRY) + sizeof (VARIABLE_STORE_HEADER);
  FvHeaderStore = AllocateZeroPool (HeaderSize);
  if (FvHeaderStore == NULL) {
    DEBUG ((DEBUG_ERROR, "PS: Out of memory\n"));
    return EFI_UNSUPPORTED;
  }

  Status = SpiProtocol->FlashRead (
                          SpiProtocol,
                          FlashRegionBios,
                          FixedPcdGet32 (PcdFlashNvStorageVariableOffset),
                          HeaderSize,
                          FvHeaderStore
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS: FlashRead Error Status - %r\n", Status));
    FreePool (FvHeaderStore);
    return EFI_UNSUPPORTED;
  }

  Status = SpiProtocol->FlashErase (
                          SpiProtocol,
                          FlashRegionBios,
                          FixedPcdGet32 (PcdFlashNvStorageVariableOffset),
                          FixedPcdGet32 (PcdFlashNvStorageVariableSize)
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS: FlashErase Error Status - %r\n", Status));
    FreePool (FvHeaderStore);
    return EFI_UNSUPPORTED;
  }

  Status = SpiProtocol->FlashWrite (
                          SpiProtocol,
                          FlashRegionBios,
                          FixedPcdGet32 (PcdFlashNvStorageVariableOffset),
                          HeaderSize,
                          FvHeaderStore
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS: Flash Write Error Status - %r\n", Status));
    FreePool (FvHeaderStore);
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "PS: Successfully erased NVM Variable region.\n"));
  FreePool (FvHeaderStore);
  return EFI_SUCCESS;
}
