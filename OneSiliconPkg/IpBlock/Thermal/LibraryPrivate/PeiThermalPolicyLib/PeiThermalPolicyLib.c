/** @file
  This file provides services for Thermal PEI policy function

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

@par Specification Reference:
**/
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <ThermalConfig.h>

/**
  Print thermal config block settings.

  @param[in] SiPolicyPpi    Instance of SI_POLICY_PPI
**/
VOID
ThermalPrintConfig (
  IN  SI_POLICY_PPI         *SiPolicyPpi
  )
{
  EFI_STATUS                Status;
  THERMAL_CONFIG            *ThermalConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gThermalConfigGuid, (VOID *) &ThermalConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ PCH Thermal Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " TTLevels T0Level %x centigrade degree\n", ThermalConfig->TTLevels.T0Level));
  DEBUG ((DEBUG_INFO, " TTLevels T1Level %x centigrade degree\n", ThermalConfig->TTLevels.T1Level));
  DEBUG ((DEBUG_INFO, " TTLevels T2Level %x centigrade degree\n", ThermalConfig->TTLevels.T2Level));
  DEBUG ((DEBUG_INFO, " TTLevels TTEnable %x\n", ThermalConfig->TTLevels.TTEnable));
  DEBUG ((DEBUG_INFO, " TTLevels TTState13Enable %x\n", ThermalConfig->TTLevels.TTState13Enable));
  DEBUG ((DEBUG_INFO, " TTLevels TTLock %x\n", ThermalConfig->TTLevels.TTLock));
  DEBUG ((DEBUG_INFO, " TTLevels SuggestedSetting %x\n", ThermalConfig->TTLevels.SuggestedSetting));

  DEBUG ((DEBUG_INFO, " DmiHaAWC DmiTsawEn %x\n", ThermalConfig->DmiHaAWC.DmiTsawEn));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS0TW %x\n", ThermalConfig->DmiHaAWC.TS0TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS1TW %x\n", ThermalConfig->DmiHaAWC.TS1TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS2TW %x\n", ThermalConfig->DmiHaAWC.TS2TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC TS3TW %x\n", ThermalConfig->DmiHaAWC.TS3TW));
  DEBUG ((DEBUG_INFO, " DmiHaAWC SuggestedSetting %x\n", ThermalConfig->DmiHaAWC.SuggestedSetting));

  DEBUG ((DEBUG_INFO, " PchHotEnable = %x\n", ThermalConfig->PchHotEnable));
  DEBUG ((DEBUG_INFO, " PchHotLevel = %x\n", ThermalConfig->PchHotLevel));
}

/**
  Print IOE thermal config block settings.

  @param[in] SiPolicyPpi    Instance of SI_POLICY_PPI
**/
VOID
ThermalPrintIoeConfig (
  IN  SI_POLICY_PPI         *SiPolicyPpi
  )
{
  EFI_STATUS                Status;
  THERMAL_CONFIG            *ThermalIoeConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gThermalIoeConfigGuid, (VOID *) &ThermalIoeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ IOE Thermal Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " TTLevels T0Level %x centigrade degree\n", ThermalIoeConfig->TTLevels.T0Level));
  DEBUG ((DEBUG_INFO, " TTLevels T1Level %x centigrade degree\n", ThermalIoeConfig->TTLevels.T1Level));
  DEBUG ((DEBUG_INFO, " TTLevels T2Level %x centigrade degree\n", ThermalIoeConfig->TTLevels.T2Level));
  DEBUG ((DEBUG_INFO, " TTLevels TTEnable %x\n", ThermalIoeConfig->TTLevels.TTEnable));
  DEBUG ((DEBUG_INFO, " TTLevels TTLock %x\n", ThermalIoeConfig->TTLevels.TTLock));
  DEBUG ((DEBUG_INFO, " TTLevels SuggestedSetting %x\n", ThermalIoeConfig->TTLevels.SuggestedSetting));
}


/**
  Load thermal config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
ThermalLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  THERMAL_CONFIG  *ThermalConfig;
  ThermalConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "ThermalConfig->Header.GuidHob.Name = %g\n", &ThermalConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "ThermalConfig->Header.GuidHob.Header.HobLength = 0x%x\n", ThermalConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Thermal configuration.
  ********************************/
  ThermalConfig->PchHotLevel                 = 0x73;
  ThermalConfig->TTLevels.SuggestedSetting   = TRUE;
}

/**
  Load IOE thermal config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
ThermalLoadIoeConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  THERMAL_CONFIG  *ThermalIoeConfig;
  ThermalIoeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "ThermalIoeConfig->Header.GuidHob.Name = %g\n", &ThermalIoeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "ThermalIoeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", ThermalIoeConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Thermal configuration.
  ********************************/
  ThermalIoeConfig->TTLevels.SuggestedSetting   = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mThermalBlocks[] = {
  {&gThermalConfigGuid, sizeof (THERMAL_CONFIG), THERMAL_CONFIG_REVISION, ThermalLoadConfigDefault}
  };

STATIC COMPONENT_BLOCK_ENTRY  mThermalIoeBlock[] = {
  {&gThermalIoeConfigGuid, sizeof (THERMAL_CONFIG), THERMAL_CONFIG_REVISION, ThermalLoadIoeConfigDefault}
  };

/**
  Get Thermal config block table size.

  @retval      Size of config block
**/
UINT16
ThermalGetConfigBlockTotalSize (
  VOID
  )
{
  return mThermalBlocks[0].Size;
}

/**
  Get Thermal IOE config block table size.

  @retval      Size of config block
**/
UINT16
ThermalGetIoeConfigBlockTotalSize (
  VOID
  )
{
  return mThermalIoeBlock[0].Size;
}

/**
  Add Thermal ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
ThermalAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mThermalBlocks[0], sizeof (mThermalBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
  return Status;
}

/**
  Add Thermal IOE ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
ThermalAddIoeConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  EFI_STATUS  Status;
  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mThermalIoeBlock[0], sizeof (mThermalIoeBlock) / sizeof (COMPONENT_BLOCK_ENTRY));
  return Status;
}