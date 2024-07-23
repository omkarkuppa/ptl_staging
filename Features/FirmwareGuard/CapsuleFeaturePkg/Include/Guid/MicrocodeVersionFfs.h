/** @file
  Format of the raw data in the FFS file gIntelMicrocodeVersionFfsFileGuid

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

@par Specification
**/

#ifndef __INTEL_MICROCODE_VERSION_FFS_FILE_GUID_H__
#define __INTEL_MICROCODE_VERSION_FFS_FILE_GUID_H__

#define INTEL_MICROCODE_VERSION_FFS_FILE_GUID { 0xE0562501, 0xB41B, 0x4566, { 0xAC, 0x0F, 0x7E, 0xA8, 0xEE, 0x81, 0x7F, 0x20 } }

///
/// Format of the raw data in the FFS file gIntelMicrocodeVersionFfsFileGuid.
/// This include the 32-bit Version, 32-bit LowestSupportVersion, and Null
/// terminated Unicode Version String associated with the array of microcode
/// patches that are stored in the gIntelMicrocodeFirmwareVolumeGuid Firmware
/// Volume.
///
typedef struct {
  UINT32  Version;
  UINT32  LowestSupportedVersion;
  CHAR16  VersionString[0];
} INTEL_MICROCODE_VERSION_FFS_DATA;

extern EFI_GUID gIntelMicrocodeVersionFfsFileGuid;

#endif
