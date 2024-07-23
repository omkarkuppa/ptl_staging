/** @file
  This file is PeiITbtPolicyLib library.

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

@par Specification Reference:
**/
#include <PeiITbtPolicyLibrary.h>

VOID
LoadTbtPeiDefault (
  IN   VOID          *ConfigBlockPointer
  )
{
  PEI_ITBT_CONFIG            *ITbtPeiConfig;
  UINT8                      Index;

  ITbtPeiConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "ITbtPeiConfig->Header.GuidHob.Name = %g\n", &ITbtPeiConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "ITbtPeiConfig->Header.GuidHob.Header.HobLength = 0x%x\n", ITbtPeiConfig->Header.GuidHob.Header.HobLength));

  //
  // Load iTbt default. ForcePowerOnTimeout = 500 ms, ConnectTopologyTimeout = 5 seconds
  //
  ITbtPeiConfig->ITbtGenericConfig.ITbtForcePowerOnTimeoutInMs = 500;
  ITbtPeiConfig->ITbtGenericConfig.ITbtConnectTopologyTimeoutInMs = 5000;
  ITbtPeiConfig->ITbtGenericConfig.Usb4CmMode = USB4_CM_MODE_SW_CM;
  ITbtPeiConfig->ITbtGenericConfig.ITbtPcieTunnelingForUsb4 = 0x1;

  for (Index = 0; Index < MAX_HOST_ITBT_DMA_NUMBER; Index ++) {
    ITbtPeiConfig->ITbtDmaLtr[Index] = 0x97FF;
  }
}

static COMPONENT_BLOCK_ENTRY  mTbtIpBlocks [] = {
  {&gPeiITbtConfigGuid,       sizeof (PEI_ITBT_CONFIG),   PEI_ITBT_CONFIG_REVISION,      LoadTbtPeiDefault}
};

/**
  Get TBT config block table total size.

  @retval     Size of TBT config block table
**/
UINT16
EFIAPI
ITbtGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mTbtIpBlocks[0], sizeof (mTbtIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  ITbtAddConfigBlocks add all TBT config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add TBT config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
ITbtAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "TBT AddConfigBlocks. TotalBlockCount = 0x%x\n",  sizeof (mTbtIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY)));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mTbtIpBlocks[0], sizeof (mTbtIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
