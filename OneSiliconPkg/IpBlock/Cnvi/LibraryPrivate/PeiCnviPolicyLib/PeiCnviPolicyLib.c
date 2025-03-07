/** @file
  This file provides services for CNVi PEI policy function

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <CnviConfig.h>

/**
  Print CNVI_CONFIG settings.

  @param[in] SiPolicyPpi    Instance of SI_POLICY_PPI
**/
VOID
CnviPrintConfig (
  IN  SI_POLICY_PPI         *SiPolicyPpi
  )
{
  EFI_STATUS  Status;
  CNVI_CONFIG *CnviConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCnviConfigGuid, (VOID *) &CnviConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ CNVi Config ------------------\n"));
  DEBUG ((DEBUG_INFO, "CNVi Mode                  = %x\n", CnviConfig->Mode));
  DEBUG ((DEBUG_INFO, "Wi-Fi Core                 = %x\n", CnviConfig->WifiCore));
  DEBUG ((DEBUG_INFO, "BT Core                    = %x\n", CnviConfig->BtCore));
  DEBUG ((DEBUG_INFO, "BT Audio Offload           = %x\n", CnviConfig->BtAudioOffload));
  DEBUG ((DEBUG_INFO, "BT Audio Offload Interface = %x\n", CnviConfig->BtAudioOffloadInterface));
  DEBUG ((DEBUG_INFO, "BT Interface               = %x\n", CnviConfig->BtInterface));
  DEBUG ((DEBUG_INFO, "Wwan Coex                  = %x\n", CnviConfig->WwanCoex));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer  Pointer to config block
**/
VOID
CnviLoadConfigDefault (
  IN VOID *ConfigBlockPointer
  )
{
  CNVI_CONFIG *CnviConfig;

  CnviConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "CnviConfig->Header.GuidHob.Name = %g\n", &CnviConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CnviConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CnviConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Cnvi configuration.
  ********************************/
  CnviConfig->Mode                    = CnviModeAuto;           // Automatic detection
  CnviConfig->WifiCore                = TRUE;                   // Wi-Fi Core enabled
  CnviConfig->BtCore                  = TRUE;                   // BT Core enabled
  CnviConfig->BtAudioOffload          = TRUE;                   // BT Audio Offload enabled
  CnviConfig->BtAudioOffloadInterface = CnviBtAudioOffloadI2s;  // BT Audio Offload Interface I2S (
  CnviConfig->BtInterface             = CnviBtIfUsb;            // USB interface
  CnviConfig->WwanCoex                = 0x0;                    // WWAN COEX
}

STATIC COMPONENT_BLOCK_ENTRY mCnviBlock = {
  &gCnviConfigGuid,  sizeof (CNVI_CONFIG),  CNVI_CONFIG_REVISION,  CnviLoadConfigDefault
  };

/**
  Get CNVi config block table size.

  @retval Size of config block
**/
UINT16
CnviGetConfigBlockTotalSize (
  VOID
  )
{
  return mCnviBlock.Size;
}

/**
  Add CNVi ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
CnviAddConfigBlock (
  IN VOID *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mCnviBlock, 1);
}
