/** @file
  Sub Region Update Library. Writes to Sub Region in Flash.

  Caution: This module requires additional review when modified.
  This module will have external input - capsule image.
  This external input must be validated carefully to avoid security issue like
  buffer overflow, integer overflow.

  SubRegionWrite() and VerifySubRegion() will receive untrusted input and do basic validation.

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
#include <PiDxe.h>
#include <Library/SubRegionUpdateLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Protocol/FirmwareManagement.h>
#include <Library/BiosUpdateHelpersLib.h>
#include <Library/ExtractGuidedSectionLib.h>
#include <Library/FvImageExtractionLib.h>

/**
  Compute Subregion CRC. If Image is compressed, then this subroutine will decompress & Compute CRC

  Caution: This function may receive untrusted input.
  @param[in]     Image            The SubRegion image buffer.
  @param[in]     ImageSize        The size of SubRegion image buffer in bytes.
  @param[in]     FileName         The SubRegion GUID
  @param[out]    ImageCRC         A pointer to the Subregion Image CRC

  @retval EFI_SUCCESS             The SubRegion Version Found
  @retval EFI_LOAD_ERROR          The SubRegion image is corrupt.
  @retval  RETURN_UNSUPPORTED     The GUID from the section specified by InputSection does not match any of
                                  the GUIDs registered with ExtractGuidedSectionRegisterHandlers().
  @retval  Others                 The return status from the handler associated with the GUID retrieved from
                                  the section specified by InputSection.
**/

EFI_STATUS
SubRegionImageComputeCRC (
    IN  CONST VOID        *Image,
    IN  UINTN             ImageSize,
    IN  CONST GUID        *FileName,
    OUT UINT32            *ImageCRC
  )
{
  VOID            *ImageBuffer;
  UINTN           ImageBufferSize;
  UINT8           *ScratchBuffer;
  UINT8           *OutputBuffer;
  UINT8           *ExtractedImageBuffer;
  UINT32          ScratchBufferSize;
  UINT32          OutputBufferSize;
  UINT16          SectionAttribute;
  UINT32          AuthenticationStatus;
  EFI_STATUS      Status;
  EFI_GUID        *SectionDefinitionGuid;
  UINT32          SectionSize;
  //
  // Init local variable
  //
  OutputBuffer         = NULL;
  ScratchBuffer        = NULL;
  ExtractedImageBuffer = NULL;
  ImageBuffer          = NULL;
  ImageBufferSize      = 0;
  AuthenticationStatus = 0;
  ScratchBufferSize    = 0;
  OutputBufferSize     = 0;
  AuthenticationStatus = 0;
  Status               = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "SubRegionImageComputeCRC: Image %x\n",(UINT32 *)Image));

  /*Extract Image using GUID*/
  if (!ExtractImageByName (
                        (VOID *)Image,
                        ImageSize,
                        (EFI_GUID *)FileName,
                        &ImageBuffer,
                        &ImageBufferSize)) {
   DEBUG ((DEBUG_ERROR, "Fail to find Config Section\n"));
   ASSERT (FALSE);
   return EFI_LOAD_ERROR;
  }


  DEBUG ((DEBUG_INFO, "Extracted Image Details %x size:%x\n",(UINT32 *)ImageBuffer,ImageBufferSize));

  if (IS_SECTION2 (ImageBuffer)) {
     SectionDefinitionGuid = &(((EFI_GUID_DEFINED_SECTION2 *) ImageBuffer)->SectionDefinitionGuid);
     SectionSize = SECTION2_SIZE(ImageBuffer);
  } else {
     SectionDefinitionGuid = &(((EFI_GUID_DEFINED_SECTION *) ImageBuffer)->SectionDefinitionGuid);
     SectionSize = SECTION_SIZE(ImageBuffer);
  }

  DEBUG ((DEBUG_INFO, "SectionDefinitionGuid->Data1 %x\n",SectionDefinitionGuid->Data1));

  //
  // Check if LZMA Compression is present in the Binary. Decompression Required if Compressed Binary.
  //
  if (CompareGuid (&gLzmaCustomDecompressGuid, SectionDefinitionGuid)){

    Status = ExtractGuidedSectionGetInfo (
                     (VOID *)ImageBuffer,
                     &OutputBufferSize,
                     &ScratchBufferSize,
                     &SectionAttribute
                     );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "GetInfo from guided section Failed - %r\n", Status));
      return Status;
    }

    if (ScratchBufferSize != 0) {
      //
      // Allocate scratch buffer
      //
      ScratchBuffer = AllocatePages (EFI_SIZE_TO_PAGES (ScratchBufferSize));
      if (ScratchBuffer == NULL) {
        DEBUG ((DEBUG_INFO, "Scratch buffer Allocation Failed"));
        return EFI_OUT_OF_RESOURCES;
      }
    }

    if (((SectionAttribute & EFI_GUIDED_SECTION_PROCESSING_REQUIRED) != 0) && OutputBufferSize > 0) {
      //
      // Allocate output buffer
      //
      OutputBuffer = AllocatePages (EFI_SIZE_TO_PAGES (OutputBufferSize));
      if (OutputBuffer == NULL) {
        DEBUG ((DEBUG_INFO, "Output buffer Allocation Failed"));
        if (ScratchBuffer != NULL) {
          FreePages (ScratchBuffer, EFI_SIZE_TO_PAGES (ScratchBufferSize));
        }
        return EFI_OUT_OF_RESOURCES;
      }
      DEBUG ((DEBUG_INFO, "Customized Guided section Memory Size required is 0x%x and address is 0x%p\n", OutputBufferSize,OutputBuffer));
    }
    //
    // Decode Start
    //
    Status = ExtractGuidedSectionDecode (
                   (VOID *)ImageBuffer,
                   (VOID **) &OutputBuffer,
                   (VOID *)ScratchBuffer,
                   &AuthenticationStatus
                   );

    if (EFI_ERROR (Status)) {
      //
      // Decode failed
      //
      if (OutputBuffer != NULL) {
        FreePages (OutputBuffer, EFI_SIZE_TO_PAGES (OutputBufferSize));
      }
      if (ScratchBuffer != NULL) {
        FreePages (ScratchBuffer, EFI_SIZE_TO_PAGES (ScratchBufferSize));
      }
      DEBUG ((DEBUG_ERROR, "Extract guided section Failed - %r\n", Status));
      return Status;
    }
    //
    // Remove EFI COMMON SECTION HEADER
    //
    if (IS_SECTION2 (ImageBuffer)) {
      SectionSize = SECTION2_SIZE (ImageBuffer);
    } else {
     SectionSize = SECTION_SIZE (ImageBuffer);
    }
    ExtractedImageBuffer = OutputBuffer+sizeof (EFI_COMMON_SECTION_HEADER);
  }else {
    //
    // Remove EFI COMMON SECTION HEADER for uncompressed Image
    //
    ExtractedImageBuffer = (UINT8 *)ImageBuffer + sizeof (EFI_COMMON_SECTION_HEADER);
  }

  // Calculate CRC of SubRegion Capsule
  gBS->CalculateCrc32 (ExtractedImageBuffer, SectionSize, ImageCRC);

  DEBUG((DEBUG_INFO, "CRC %x\n",*ImageCRC));

  // Free Memory of the OutputBuffer if allocated
  if (OutputBuffer != NULL) {
    FreePages(OutputBuffer, EFI_SIZE_TO_PAGES (OutputBufferSize));
  }

  if (ScratchBuffer != NULL) {
    FreePages (ScratchBuffer, EFI_SIZE_TO_PAGES (ScratchBufferSize));
  }

  return Status;
}

/**
  Write SubRegion to spi.

  Caution: This function may receive untrusted input.

  @param[in]   Image               The SubRegion image buffer.
  @param[in]   ImageSize           The size of SubRegion image buffer in bytes.
  @param[in]   FlashSubRegionInfo  The info for the sub region being updated.
  @param[out]  AbortReason         A pointer to a pointer to a null-terminated string providing more
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
  )
{
  EFI_STATUS                                     Status = EFI_SUCCESS;
  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress = NULL;
  FLASH_ACCESS_REQUEST         Request;


  *AbortReason    = NULL;

  if (ImageSize > FlashSubRegionInfo->FvSize) {
    DEBUG ((DEBUG_INFO, "Image is too large for Sub Region.\n"));
    return EFI_INVALID_PARAMETER;
  }

/*
  Status = SpiFlashUpdate (
               FlashRegionBios,
               (UINT32) FlashSubRegionInfo->Offset,
               (UINT8 *) Image,
               (UINT32) FlashSubRegionInfo->FvSize,
               Progress,
               0,
               100
               );
*/

  ZeroMem (&Request, sizeof (Request));
  Request.FlashRegionType = FlashRegionBios;
  Request.FlashAddress = FlashSubRegionInfo->FvOffset;
  Request.Data = (UINT8 *) Image;
  Request.DataSize = (UINTN) FlashSubRegionInfo->FvSize;

  Status = FlashUpdate (&Request, Progress, 0, 100);

  return Status;
}

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
  )
{
  //
  // Check for invalid parameters
  //
  if (Image == NULL || ImageSize == NULL || FlashSubRegionInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Make sure the buffer is big enough to hold the device image
  //
  if (*ImageSize < FlashSubRegionInfo->FvSize) {
    *ImageSize = FlashSubRegionInfo->FvSize;
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Copy the device image to the buffer
  //
  *ImageSize = FlashSubRegionInfo->FvSize;
  CopyMem (
    Image,
    (VOID *)(UINTN) ((FlashSubRegionInfo->FvOffset + PcdGet32 (PcdBiosAreaBaseAddress)) & (UINT64)~B_FMP_HIGH_32BIT_MASK),
    *ImageSize
    );

  return EFI_SUCCESS;
}

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
  )
{
  UINT32         FlashImgCRC;
  UINT32         CapsuleImgCRC;
  EFI_STATUS     Status;
  VOID           *FvBuf;
  UINTN          FvBufSize;

  FlashImgCRC   = 0;
  CapsuleImgCRC = 0;
  FvBuf         = NULL;

  if (ImageUpdateable == NULL) {
    DEBUG((DEBUG_ERROR, "CheckImage - ImageUpdateable Pointer Parameter is NULL.\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (Image == NULL) {
    DEBUG((DEBUG_ERROR, "CheckImage - Image Pointer Parameter is NULL.\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Set to invalid and then if CRC of Flash Image & CRC of Capsule is the same, then we will update this flag.
  //
  *ImageUpdateable = IMAGE_UPDATABLE_INVALID;

  //
  // Extract Capsule Image CRC
  //
  Status = SubRegionImageComputeCRC((VOID *)Image, ImageSize, FlashSubRegionInfo->FfsGuid, &CapsuleImgCRC);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SubRegionExtractImageVersion Failed - %r\n", Status));
    return Status;
  }

  DEBUG((DEBUG_INFO, "CapsuleImgCRC %x\n", CapsuleImgCRC));

  //
  // Get Flash SubRegion Data
  //
  FvBufSize = FlashSubRegionInfo->FvSize;
  FvBuf = AllocatePool (FvBufSize);
  if(FvBuf == NULL) {
    DEBUG((DEBUG_ERROR, "Failed to Allocate Buffer"));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = SubRegionGetImage (FlashSubRegionInfo, FvBuf, &FvBufSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FmpDeviceGetImage Failed - %r\n", Status));
    return Status;
  }
  //
  // Extract Flash Image CRC
  //
  Status = SubRegionImageComputeCRC ((VOID *) FvBuf, FvBufSize, FlashSubRegionInfo->FfsGuid, &FlashImgCRC);
  DEBUG ((DEBUG_INFO, "FlashImgCRC %x\n", FlashImgCRC));

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SubRegionExtractImageVersion Failed - %r\n", Status));
    return Status;
  }

  //
  // Free Allocated Buffer
  //
  if (FvBuf != NULL) {
    FreePool(FvBuf);
  }

  //
  // Set to valid only if Capsule Image CRC is not equal to Flash Image CRC
  //
  if (CapsuleImgCRC != FlashImgCRC) {
    *ImageUpdateable = IMAGE_UPDATABLE_VALID;
  }

  return EFI_SUCCESS;
}