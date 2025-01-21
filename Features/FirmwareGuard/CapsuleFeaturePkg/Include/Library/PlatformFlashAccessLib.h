/** @file
  Platform flash device access library.

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

#ifndef __PLATFORM_FLASH_ACCESS_LIB_H__
#define __PLATFORM_FLASH_ACCESS_LIB_H__

#include <Protocol/FirmwareManagement.h>
#include <Protocol/Spi.h>
//
//  General datastructure to pass a single Flash ACCESS request to different flash storage media.
//  Request can be Flash Read or Write
//  Use this structure to provide stream level I/O operation.
//  The caller such as FMP instance driver should not care about underlying hardware devices.
//
//  1) To descrbe each flash region in Flash Descriptor mode, config FlashRegionType. It is now only useful for NOR SPI flash.
//  2) To describe source/target flash address, configue FlashAddress. Address range is implementation specific.
//  3) To describe Input/Output data buffer, configure Data/DataSize
//  4) To support BiosGuard Update, set BgupImage/BgupImageSize
//

typedef struct {
  //
  // Flash Read/Update Address.
  // It is not required in BiosGuard Update mode. Script itself carries address carries
  //
  //FLASH_MEDIA_TYPE   MediaType;           //  Media type of the Flash. It is a reserved field for future extension
  FLASH_REGION_TYPE  FlashRegionType;       //  Flash Region type for flash cycle which is listed in the Descriptor. Now only used by SPI flash
  UINTN              FlashAddress;          //  Flash Address
                                            //     For SPI flash. The Flash Linear Address accessed by CPU
                                            //     For other storage, address is interpreted by storage
  //
  // For Flash Write/Update, describe input update image
  // For Flash Read, describe output data buffer
  //
  UINT8              *Data;
  UINTN              DataSize;

  //
  // Signed BgupImage Script for update. Only required with a write request when BiosGuard is enabled
  //
  UINT8              *BgupImage;
  UINTN              BgupImageSize;

} FLASH_ACCESS_REQUEST;

/*
  Get and return the pointer of SPI protocol instance.

  @param[in]    VOID

  @retval       PCH_SPI_PROTOCOL    Pointer point to the SPI procotol.
*/
PCH_SPI_PROTOCOL*
EFIAPI
GetPchSpiProtocol (
  VOID
  );

/**
  Generic API for Gen3 capsule update, Read data from SPI flash.

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
  );

/**
  Generic API for Gen3 capsule update, Write content with SPI flash.

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
  );
#endif
