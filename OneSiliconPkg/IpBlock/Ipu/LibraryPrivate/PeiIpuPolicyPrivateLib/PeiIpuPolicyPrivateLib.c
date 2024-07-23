/** @file
  This file provides services for IPU PEI policy default initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <IpuPreMemConfig.h>

#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PeiIpuPolicyPrivateLib.h>

static COMPONENT_BLOCK_ENTRY  mIpuBlocksPreMem = {
  &gIpuPreMemConfigGuid,
  sizeof (IPU_PREMEM_CONFIG),
  IPU_PREMEM_CONFIG_REVISION,
  &IpuLoadPreMemDefault
};

/**
  Get IPU config block table size.

  @retval      Size of IPU config block table
**/
UINT16
EFIAPI
IpuGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return mIpuBlocksPreMem.Size;
}

/**
  AddConfigBlocksPreMem add IPU config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add IPU config block

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
IpuAddConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mIpuBlocksPreMem, 1);
  return Status;
}
