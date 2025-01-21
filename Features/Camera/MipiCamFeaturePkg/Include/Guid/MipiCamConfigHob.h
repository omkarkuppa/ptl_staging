/** @file
  MIPI camera create config HOB file

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

@par Specification

**/
#ifndef MIPICAM_CONFIG_HOB_H_
#define MIPICAM_CONFIG_HOB_H_

#include <Base.h>

typedef struct {
  UINT8              PlatformId;         /// PlatformId
  UINT8              EmulationConfig;    /// Emulation Status
  UINT8              PlatformChipId;     /// Platform ChipsetId
  UINT8              Reserved[5];        /// Reserved for natural alignment
} MIPICAM_CONFIG_HOB;

typedef struct {
  UINT8              PchScopeNum;        /// Check PCH
  UINT8              Reserved[7];        /// Reserved for natural alignment
} MIPICAM_CONFIG_HOB_EXT;

extern EFI_GUID gMipiCamConfigHobGuid;
extern EFI_GUID gMipiCamConfigHobExtGuid;

#endif
