/** @file
  This file provides services for I2c policy function

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

#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <LpssI2cConfig.h>

/**
  Print LPSS_I2C_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
LpssI2cPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  LPSS_I2C_CONFIG   *I2cConfig;
  UINT8             Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLpssI2cConfigGuid, (VOID *) &I2cConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ LPSS I2C Config ------------------\n"));

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    DEBUG ((DEBUG_INFO, "I2C[%d].Mode              = %d\n",   Index, I2cConfig->I2cDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, "I2C[%d].Sda.PinMux        = 0x%x\n", Index, I2cConfig->I2cDeviceConfig[Index].PinMux.Sda));
    DEBUG ((DEBUG_INFO, "I2C[%d].Scl.PinMux        = 0x%x\n", Index, I2cConfig->I2cDeviceConfig[Index].PinMux.Scl));
    DEBUG((DEBUG_INFO, " I2C[%d].PadTermination    = %d\n",   Index, I2cConfig->I2cDeviceConfig[Index].PadTermination));
  }
  DEBUG ((DEBUG_INFO, "-----------------------------------------------------\n"));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LpssI2cLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT8                  Index;
  LPSS_I2C_CONFIG*       I2cConfig;

  I2cConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "I2cConfig->Header.GuidHob.Name = %g\n", &I2cConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "I2cConfig->Header.GuidHob.Header.HobLength = 0x%x\n", I2cConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    I2cConfig->I2cDeviceConfig[Index].Mode                 = LpssI2cDisabled;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mI2cBlocks = {
  &gLpssI2cConfigGuid,
  sizeof (LPSS_I2C_CONFIG),
  LPSS_I2C_CONFIG_REVISION,
  LpssI2cLoadConfigDefault
};

/**
  Get I2c config block table size.

  @retval      Size of config block
**/
UINT16
LpssI2cGetConfigBlockTotalSize (
  VOID
  )
{
  return mI2cBlocks.Size;
}

/**
  Add I2c ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval    EFI_SUCCESS                   The policy default is initialized.
  @retval    EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
LpssI2cAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mI2cBlocks, 1);
}
