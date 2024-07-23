/** @file
  Implementation of TraceHub common utilities.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <Base.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Guid/EventGroup.h>
#include <Core/Dxe/DxeMain.h>

/**
  Get the image's private data from its handle.

  @param  ImageHandle             The image handle

  @return Return the image private data associated with ImageHandle.
**/
LOADED_IMAGE_PRIVATE_DATA *
GetLoadedImageInfo (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS                 Status;
  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage;
  LOADED_IMAGE_PRIVATE_DATA  *Image;

  Status = gBS->HandleProtocol (
             ImageHandle,
             &gEfiLoadedImageProtocolGuid,
             (VOID **)&LoadedImage
             );
  if (!EFI_ERROR (Status)) {
    Image = LOADED_IMAGE_PRIVATE_DATA_FROM_THIS (LoadedImage);
  } else {
    DEBUG ((DEBUG_LOAD, "GetLoadedImageInfo: Not an ImageHandle %p\n", ImageHandle));
    Image = NULL;
  }

  return Image;
}
