/** @file
  Sub Region update header file.

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

**/

#ifndef _SUB_REGION_UPDATE_LIB_H_
#define _SUB_REGION_UPDATE_LIB_H_

#include <Uefi/UefiBaseType.h>


#define B_FMP_HIGH_32BIT_MASK    0xFFFFFFFF00000000

///
/// Types of Sub Regions
///
typedef enum {
  TSN_MAC_ADDRESS_SUB_REGION_TYPE = 0,
  OOB_CONFIG_SUB_REGION_TYPE,
  TCC_CONFIG_SUB_REGION_TYPE,
  SIIP_FW_SUB_REGION_TYPE,
  PSE_TSN_CONFIG_SUB_REGION_TYPE
} SUB_REGION_TYPES;

///
/// Flash Sub Region Info
///
typedef struct {
  EFI_GUID  *FfsGuid;
  UINTN     FvOffset;
  UINT32    FvSize;
} FLASH_SUB_REGION_INFO;

/**
  Compute Subregion CRC. If Image is compressed, then this subroutine will decompress & Compute CRC

  Caution: This function may receive untrusted input.
  @param[in]  Image            The SubRegion image buffer.
  @param[in]  ImageSize        The size of SubRegion image buffer in bytes.
  @param[in]  FileName         The SubRegion GUID
  @param[out] ImageCRC         A pointer to the Subregion Image CRC

  @retval EFI_SUCCESS          The SubRegion Version Found
  @retval EFI_LOAD_ERROR       The SubRegion image is corrupt.
  @retval RETURN_UNSUPPORTED   The GUID from the section specified by InputSection does not match any of
                               the GUIDs registered with ExtractGuidedSectionRegisterHandlers().
  @retval Others               The return status from the handler associated with the GUID retrieved from
                               the section specified by InputSection.
**/
EFI_STATUS
SubRegionImageComputeCRC (
    IN  CONST VOID        *Image,
    IN  UINTN             ImageSize,
    IN  CONST GUID        *FileName,
    OUT UINT32            *ImageCRC
  );

/**
  Write SubRegion to spi.

  Caution: This function may receive untrusted input.

  @param[in]  Image                The SubRegion image buffer.
  @param[in]  ImageSize            The size of SubRegion image buffer in bytes.
  @param[in]  FlashSubRegionInfo   The info for the sub region being updated.
  @param[out] AbortReason          A pointer to a pointer to a null-terminated string providing more
                                   details for the aborted operation. The buffer is allocated by this function
                                   with AllocatePool(), and it is the caller's responsibility to free it with a
                                   call to FreePool().

  @retval EFI_SUCCESS               The SubRegion image is written.
  @retval EFI_VOLUME_CORRUPTED      The SubRegion image is corrupt.
  @retval EFI_INCOMPATIBLE_VERSION  The SubRegion image version is incorrect.
  @retval EFI_SECURITY_VIOLATION    The SubRegion image fails to load.
  @retval EFI_WRITE_PROTECTED       The flash device is read only.
**/
EFI_STATUS
SubRegionWrite (
  IN  VOID                              *Image,
  IN  UINTN                             ImageSize,
  IN  FLASH_SUB_REGION_INFO             *FlashSubRegionInfo,
  OUT CHAR16                            **AbortReason
  );

/**
  Retrieves a copy of the current subregion image of the device.

  @param[in]  FlashSubRegionInfo  The info for the sub region being updated.
  @param[out] Image               Points to the buffer where the current image is copied
                                  to.
  @param[out] ImageSize           On entry, points to the size of the buffer pointed to
                                  by Image, in bytes.  On return, points to the length of
                                  the image, in bytes.

  @retval EFI_SUCCESS             The image was successfully read from the device.
  @retval EFI_BUFFER_TOO_SMALL    The buffer specified by ImageSize is too small
                                  to hold the image. The current buffer size
                                  needed to hold the image is returned in
                                  ImageSize.
  @retval EFI_INVALID_PARAMETER   The Image was NULL.
**/
EFI_STATUS
SubRegionGetImage (
  IN  FLASH_SUB_REGION_INFO *FlashSubRegionInfo,
  IN OUT VOID               *Image,
  IN OUT UINTN              *ImageSize
  );

/**
  Checks if the subregion image is valid for the device.

  @param[in]  FlashSubRegionInfo The info for the sub region being updated.
  @param[in]  Image              Points to the new image.
  @param[in]  ImageSize          Size of the new image in bytes.
  @param[out] ImageUpdatable     Indicates if the new image is valid for update. It also provides,
                                 if available, additional information if the image is invalid.

  @retval EFI_SUCCESS            The image was successfully checked.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.

**/
EFI_STATUS
SubRegionCheckImage (
  IN  FLASH_SUB_REGION_INFO *FlashSubRegionInfo,
  IN  CONST VOID            *Image,
  IN  UINTN                 ImageSize,
  OUT UINT32                *ImageUpdateable
  );

#endif

