/** @file
  Firmware File System protocol. Layers on top of Firmware
  Block protocol to produce a file abstraction of FV based files.

#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.

@par Specification Reference:
**/

#ifndef __CNV_COMPATIBILITY_DRIVER_H
#define __CNV_COMPATIBILITY_DRIVER_H

#include <Uefi.h>
#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Guid/FileSystemInfo.h>
#include <Guid/FileInfo.h>
#include <Pi/PiFirmwareVolume.h>
#include <Protocol/PciIo.h>
#include <Library/PchPciBdfLib.h>
#include <IndustryStandard/Pci22.h>
#include <CnvVfrSetupMenuHii.h>

#define INTEL_VENDOR_ID  0x8086

/**
  Given the supplied FW_VOL_BLOCK_PROTOCOL, allocate a buffer for output and
  copy the real length volume header into it.

  @param  Fvb                   The FW_VOL_BLOCK_PROTOCOL instance from which to
                                read the volume header
  @param  FwVolHeader           Pointer to pointer to allocated buffer in which
                                the volume header is returned.

  @retval EFI_OUT_OF_RESOURCES  No enough buffer could be allocated.
  @retval EFI_SUCCESS           Successfully read volume header to the allocated
                                buffer.
  @retval EFI_INVALID_PARAMETER The FV Header signature is not as expected or
                                the file system could not be understood.

**/
EFI_STATUS
CnvGetFwVolHeader (
  IN     EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb,
  OUT    EFI_FIRMWARE_VOLUME_HEADER          **FwVolHeader
  );

/**
  Read data from Firmware Block by FVB protocol Read.
  The data may cross the multi block ranges.

  @param  Fvb                   The FW_VOL_BLOCK_PROTOCOL instance from which to read data.
  @param  StartLba              Pointer to StartLba.
                                On input, the start logical block index from which to read.
                                On output,the end logical block index after reading.
  @param  Offset                Pointer to Offset
                                On input, offset into the block at which to begin reading.
                                On output, offset into the end block after reading.
  @param  DataSize              Size of data to be read.
  @param  Data                  Pointer to Buffer that the data will be read into.

  @retval EFI_SUCCESS           Successfully read data from firmware block.
  @retval others

**/
EFI_STATUS
CnvReadFvbData (
  IN     EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb,
  IN OUT EFI_LBA                             *StartLba,
  IN OUT UINTN                               *Offset,
  IN     UINTN                               DataSize,
  OUT    UINT8                               *Data
  );

/**
  Verify checksum of the firmware volume header.

  @param  FvHeader       Points to the firmware volume header to be checked

  @retval TRUE           Checksum verification passed
  @retval FALSE          Checksum verification failed

**/
BOOLEAN
CnvVerifyFvHeaderChecksum (
  IN EFI_FIRMWARE_VOLUME_HEADER  *FvHeader
  );

/**
  This function to check weather Cnv is present

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Cnv is not found

**/
EFI_STATUS
IsSupportedCnvPresent (
  VOID
  );

/**
  This function to check weather Cnv Driver present

  @param KeyGuid                   Guid to check is CnvDriverPresent

  @retval EFI_SUCCESS              Cnv Driver is Present
  @retval EFI_NOT_FOUND            Cnv Driver is not present

**/
EFI_STATUS
IsCnvDriverPresent (
  IN EFI_GUID  *KeyGuid
  );

/**
  Find and Load Cnv driver from fs0: to RAM and then call LoadCnvDriver()

  @retval EFI_SUCCESS           Successfully Loaded Fv from External Harddisk

  @retval EFI_NOT_FOUND         Unable to Load from Fv

**/
EFI_STATUS
EFIAPI
LoadCnvDriverFromFv (
  VOID
  );

/**
  This Function loads Cnv Driver from EFI System partition

  @param[in] FvBaseAddress   Pointer stores starting Address.
  @param[in] FvSize          Size of Firmware Volume

 **/
VOID
EFIAPI
LoadCnvDriver (
  IN UINT8  *FvBaseAddress,
  IN UINTN  FvSize
  );

#endif
