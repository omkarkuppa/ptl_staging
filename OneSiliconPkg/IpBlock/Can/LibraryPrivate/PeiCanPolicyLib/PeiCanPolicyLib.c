/** @file
  This file provides services for CAN policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <CanConfig.h>

/**
  Print CAN_CONFIG and serial out.

  @param[in] SiPolicyPpi   Pointer to a SI_POLICY_PPI
**/
VOID
CanPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS   Status;
  CAN_CONFIG   *CanConfig;
  UINT8        Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCanConfigGuid, (VOID *) &CanConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ CAN Config ------------------\n"));
  for (Index = 0; Index < PCH_MAX_CAN_PORT; Index++) {
    DEBUG ((DEBUG_INFO, " Enable[%d] = %x\n", Index, CanConfig->Enable[Index]));
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer   Pointer to config block
**/
VOID
CanLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  CAN_CONFIG   *CanConfig;
  UINT8        Index;

  CanConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CanConfig->Header.GuidHob.Name = %g\n", &CanConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CanConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CanConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < PCH_MAX_CAN_PORT; Index++) {
    CanConfig->Enable[Index] = TRUE;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mCanBlocks = {
  &gCanConfigGuid,
  sizeof (CAN_CONFIG),
  CAN_CONFIG_REVISION,
  CanLoadConfigDefault
};

/**
  Get CAN config block table size.

  @retval      Size of config block
**/
UINT16
CanGetConfigBlockTotalSize (
  VOID
  )
{
  return mCanBlocks.Size;
}

/**
  Add CAN ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
CanAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mCanBlocks, 1);
}
