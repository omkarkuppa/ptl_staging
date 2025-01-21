/** @file
  Header file for the Boot Media Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _BOOT_MEDIA_LIB_H_
#define _BOOT_MEDIA_LIB_H_

#include <Uefi.h>

typedef enum {
  BootMediaSpi,
  BootMediaMax
} BOOT_MEDIA_TYPE;

typedef struct {
  BOOT_MEDIA_TYPE BootMedia;
} BOOT_MEDIA_HOB_DATA;

/**
  Determines the current platform boot media device.

  @param[out] BootMedia       The current platform Boot Media type.

  @retval     EFI_SUCCESS     The operation completed successfully.
  @retval     EFI_NOT_FOUND   The boot media type could not be found.
**/
EFI_STATUS
EFIAPI
GetBootMediaType (
  OUT BOOT_MEDIA_TYPE   *BootMedia
  );

/**
  Determines if the boot media is presently known.

  Note: Very early in boot this data may not be available.

  @retval TRUE        Boot media is known
  @retval FALSE       Boot media is not known
**/
BOOLEAN
EFIAPI
BootMediaIsKnown (
  VOID
  );

/**
  Determines if the platform firmware is booting from SPI or not.

  @retval TRUE        Platform firmware is booting from SPI
  @retval FALSE       Platform firmware is booting from a non-SPI device (eMMC, UFS, etc.)
**/
BOOLEAN
EFIAPI
BootMediaIsSpi (
  VOID
  );

#endif
