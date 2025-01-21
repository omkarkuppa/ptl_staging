/** @file
  BIOS Debug Message Tracing Catalog

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

#ifndef __TRACE_HUB_STATUS__WORKER_COMMON_H__
#define __TRACE_HUB_STATUS__WORKER_COMMON_H__
#include <Core/Dxe/DxeMain.h>

/**
  Get the image's private data from its handle.

  @param  ImageHandle             The image handle

  @return Return the image private data associated with ImageHandle.
**/
LOADED_IMAGE_PRIVATE_DATA *
GetLoadedImageInfo (
  IN EFI_HANDLE  ImageHandle
  );

#endif // __TRACE_HUB_DISCARD_DATA_H__
