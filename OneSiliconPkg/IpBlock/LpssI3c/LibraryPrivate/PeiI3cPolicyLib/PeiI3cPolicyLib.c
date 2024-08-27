/** @file
  This file provides services for I3c policy function

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
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchLimits.h>
#include <LpssI3cConfig.h>

/**
  Print LPSS_I3C_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
LpssI3cPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS        Status;
  LPSS_I3C_CONFIG   *I3cConfig;
  UINT8             Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLpssI3cConfigGuid, (VOID *) &I3cConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ I3C Config ------------------\n"));

  for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
    DEBUG ((DEBUG_INFO, "I3C[%d].Mode              = %d\n",   Index, I3cConfig->I3cDeviceConfig[Index].Mode));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sda.PinMux        = 0x%x\n", Index, I3cConfig->I3cDeviceConfig[Index].Sda.PinMux));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sda.Termination   = 0x%x\n", Index, I3cConfig->I3cDeviceConfig[Index].Sda.PadTermination));
    DEBUG ((DEBUG_INFO, "I3C[%d].Scl.PinMux        = 0x%x\n", Index, I3cConfig->I3cDeviceConfig[Index].Scl.PinMux));
    DEBUG ((DEBUG_INFO, "I3C[%d].Scl.Termination   = 0x%x\n", Index, I3cConfig->I3cDeviceConfig[Index].Scl.PadTermination));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sclfb.PinMux      = 0x%x\n", Index, I3cConfig->I3cDeviceConfig[Index].SclFb.PinMux));
    DEBUG ((DEBUG_INFO, "I3C[%d].Sclfb.Termination = 0x%x\n", Index, I3cConfig->I3cDeviceConfig[Index].SclFb.PadTermination));
  }
  DEBUG ((DEBUG_INFO, "------------------------------------------------\n"));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LpssI3cLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINT8                  Index;
  LPSS_I3C_CONFIG*       I3cConfig;

  I3cConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "I3cConfig->Header.GuidHob.Name = %g\n", &I3cConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "I3cConfig->Header.GuidHob.Header.HobLength = 0x%x\n", I3cConfig->Header.GuidHob.Header.HobLength));

  for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
    I3cConfig->I3cDeviceConfig[Index].Mode                 = I3cDisabled;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mI3cBlocks = {
  &gLpssI3cConfigGuid,
  sizeof (LPSS_I3C_CONFIG),
  LPSS_I3C_CONFIG_REVISION,
  LpssI3cLoadConfigDefault
};

/**
  Get I3c config block table size.

  @retval      Size of config block
**/
UINT16
LpssI3cGetConfigBlockTotalSize (
  VOID
  )
{
  return mI3cBlocks.Size;
}

/**
  Add I3c ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval    EFI_SUCCESS                   The policy default is initialized.
  @retval    EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
LpssI3cAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mI3cBlocks, 1);
}
