/** @file
  Lpss Private I2c Lib implementation PantherLake specific.
  All functions in this library are available in PEI,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci30.h>
#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiLpssI2cInitLib.h>
#include <Library/PeiItssLib.h>
#include <Library/LpssControllerInfoLib.h>
#include <Library/LpssI2cSocLib.h>
#include <LpssI2cConfig.h>
#include <LpssI2cHandle.h>

/**
  Build LPSS I2C Handle structures

  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in, out] I2cHandle        The Lpss I2C Handle instance
  @param[in]      I2cDeviceConfig  Lpss I2C Config Block
  @param[in]      I2cIndex         I2C Controller index

  @retval     EFI_SUCCESS          Completed successfully
              EFI_UNSUPPORTED      SOC is not supported
**/
EFI_STATUS
PtlPcdLpssI2cBuildHandle (
  IN SI_POLICY_PPI                   *SiPolicy,
  IN OUT LPSS_I2C_HANDLE             *I2cHandle,
  IN     LPSS_I2C_CONTROLLER_CONFIG  *I2cDeviceConfig,
  IN     UINT8                       I2cIndex
  )
{
  EFI_STATUS                Status;
  PCI_CONFIG_SPACE_ACCESS   *CfgAccess;

  if (I2cIndex >= GetMaxI2cInterfacesNum ()) {
    return EFI_UNSUPPORTED;
  }

  I2cHandle->ControllerId = I2cIndex;

  Status = PtlPcdLpssI2cGetController (I2cIndex, &I2cHandle->Controller);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // I2C Config Block
  //
  I2cHandle->I2cConfig = I2cDeviceConfig;

  //
  // Private configuration options
  //
  CfgAccess = (PCI_CONFIG_SPACE_ACCESS *)I2cHandle->Controller->PcieCfgAccess;
  ItssGetDevIntConfig (
                      SiPolicy,
                      CfgAccess->Dev,
                      CfgAccess->Fun,
                      &I2cHandle->PrivateConfig.InterruptPin,
                      &I2cHandle->PrivateConfig.Irq);


  return EFI_SUCCESS;
}

/**
  Initialize the all I2C controllers integrated in the SOC's LPSS

  @param[in]      SiPolicy         The SI Policy PPI instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       The SOC is not supported
              EFI_NOT_FOUND         I2C Config Block not found
**/
EFI_STATUS
PtlPcdLpssI2cSocInit (
  IN     SI_POLICY_PPI          *SiPolicy
  )
{
  EFI_STATUS                Status;
  BOOLEAN                   ErrorFlag;
  UINT8                     I2cIndex;
  LPSS_I2C_CONFIG           *LpssI2cConfig;
  LPSS_I2C_HANDLE           LpssI2cHandle;

  DEBUG ((DEBUG_INFO, "%a: Start.\n", __FUNCTION__));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI2cConfigGuid, (VOID *) &LpssI2cConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get I2c config block!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  ErrorFlag = FALSE;
  for (I2cIndex = 0; I2cIndex < GetMaxI2cInterfacesNum (); I2cIndex++) {
    Status = PtlPcdLpssI2cBuildHandle (SiPolicy, &LpssI2cHandle, &(LpssI2cConfig->I2cDeviceConfig[I2cIndex]), I2cIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot build I2c handle structure for Ctrl index %d (Status %r)!\n", __FUNCTION__, I2cIndex, Status));
      DEBUG ((DEBUG_ERROR, "Skip I2C Initialization for controller index %d!\n", I2cIndex));
      ErrorFlag = TRUE;
      continue;
    }
    Status = LpssI2cInit (&LpssI2cHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Failed to initialize I2C controller #%d!\n", __FUNCTION__, I2cIndex));
      ErrorFlag = TRUE;
    }
    PtlPcdLpssI2cControllerFree (LpssI2cHandle.Controller);
  }

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));

  if (ErrorFlag == TRUE) {
    if (EFI_ERROR (Status)) {
      return Status;
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}
