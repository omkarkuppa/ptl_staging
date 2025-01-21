/** @file
  Extract images from FV

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

#ifndef __FV_IMAGE_EXTRACTION_LIB_H__
#define __FV_IMAGE_EXTRACTION_LIB_H__

/**
  Extract the image raw data within input FvImage by FileName GUID.

  @param[in]   FvImage                 Pointer to a buffer containing FVs
  @param[in]   FvImageSize             The size of FvImage buffer in bytes.
  @param[in]   FileName                The FileName GUID of FFS to be found
  @param[out]  RawDataImage            The image pointer to requested raw data.
  @param[out]  RawDataImageSize        The size of RawDataImage.

  @retval TRUE  The image is extracted.
  @retval FALSE The image is not extracted.
**/
BOOLEAN
ExtractImageByName (
  IN VOID                  *FvImage,
  IN UINTN                 FvImageSize,
  IN EFI_GUID              *FileName,
  OUT VOID                 **RawDataImage,
  OUT UINTN                *RawDataImageSize
  );

#endif
