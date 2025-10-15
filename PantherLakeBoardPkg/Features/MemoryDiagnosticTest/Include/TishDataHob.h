/** @file
  Memory Test Info Structure Hob(TISH)  definition.

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

#ifndef _TISH_DATA_HOB_
#define _TISH_DATA_HOB_

#pragma pack(1)
typedef struct {
  EFI_HOB_GUID_TYPE       EfiHobGuidType;
  EFI_PHYSICAL_ADDRESS    TsegRegionAddress;
  EFI_PHYSICAL_ADDRESS    TsegMemFailureAddress;
  UINT32                  TsegSize;
  UINT8                   SafeLoadingBiosEnableState;
  UINT8                   PprRecoveryStatusEnable;
  UINT8                   TsegReallocateEnableCount;
  UINT8                   RetryCount           : 3;
  UINT8                   Reserved1            : 1;
  UINT8                   TsegMemoryTestStatus : 1;
  UINT8                   Reserved             : 3;
  EFI_PHYSICAL_ADDRESS    BiosPeiMemoryBaseAddress;
  UINT64                  BiosPeiMemoryLength;
} TISH_CONFIG_HOB;

#pragma pack()
#endif
