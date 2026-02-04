/** @file
  The GUID definition for GraphicsDataHob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _IGPU_DATA_HOB_H_
#define _IGPU_DATA_HOB_H_

#include <Base.h>

extern EFI_GUID  gIGpuDataHobGuid;
#pragma pack (push,1)

/**
  These members describe the configuration of IGPU_DATA_HOB.
**/
typedef struct {
  EFI_HOB_GUID_TYPE    EfiHobGuidType;          ///< Offset 0 - 23: GUID Hob type structure for gIGpuDataHobGuid
  UINT8                PrimaryDisplayDetection; ///< Primary Display - default is IGD. [IGD = 0; PEG = 1; PCIE = 2]
  UINT64               FlatCcsBaseAddr;         ///< Flat CCS Base Address
  UINT32               FlatCcsMemSize;          ///< Flat CCS Size in MB
  UINT32               GraphicsConfigPtr;       ///< Graphics Configuration Ptr.
  UINT32               VbtSize;                 ///< Intel Graphics VBT (Video BIOS Table) Size.
  UINT8                LidStatus;               ///< LFP Display Lid Status (LID_STATUS enum).
  UINT8                VgaDisplayConfig;        ///< VGA Display configuration
  UINT8                PreviousProgressBar;     ///< Previous Progress Bar Position
  UINT8                Rsvd[1];                 ///< Reserved for future use.
  UINT64               Gsm2BaseAddress;         ///< Graphics stolen memory 2 base address
  UINT64               Gsm2Size;                ///< Graphics stolen memory 2 size
} IGPU_DATA_HOB;
#pragma pack (pop)
#endif
