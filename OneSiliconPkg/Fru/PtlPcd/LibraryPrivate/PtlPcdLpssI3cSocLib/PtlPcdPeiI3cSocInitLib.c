/** @file
  LPSS Private I3c Lib implementation PantherLake specific.
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
#include <Library/PeiLpssI3cInitLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiLpssI3cInitLib.h>
#include <Library/LpssControllerInfoLib.h>
#include <Library/PcdPeiLpssI3cSocInitLib.h>
#include <LpssI3cConfig.h>
#include <LpssI3cHandle.h>

/**
  Build Lpss I3C Handle with all Callbacks and Soc Private Config

  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in, out] I3cHandle        The Lpss I3C Handle instance
  @param[in]      I3cDeviceConfig  Lpss I3C Config Block
  @param[in]      I3cIndex         I3C Controller index

  @retval     EFI_SUCCESS           Completed successfully
              EFI_UNSUPPORTED       SOC is not supported
**/
EFI_STATUS
PtlPcdLpssI3cBuildHandle (
  IN     SI_POLICY_PPI            *SiPolicy,
  IN OUT LPSS_I3C_HANDLE          *I3cHandle,
  IN     LPSS_I3C_DEVICE_CONFIG   *I3cDeviceConfig,
  IN     UINT8                    I3cIndex
  )
{
  EFI_STATUS                     Status;
  PCI_CONFIG_SPACE_ACCESS   *CfgAccess;

  if (I3cIndex >= GetMaxI3cInterfacesNum ()) {
    return EFI_UNSUPPORTED;
  }

  I3cHandle->ControllerId = I3cIndex;

  Status = PtlPcdLpssI3cGetController (I3cIndex, &I3cHandle->Controller);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // I3C Config Block
  //
  I3cHandle->I3cConfig = I3cDeviceConfig;

  CfgAccess = (PCI_CONFIG_SPACE_ACCESS *)I3cHandle->Controller->PcieCfgAccess;
  ItssGetDevIntConfig (
                      SiPolicy,
                      CfgAccess->Dev,
                      CfgAccess->Fun,
                      &I3cHandle->PrivateConfig.InterruptPin,
                      &I3cHandle->PrivateConfig.Irq);

  return EFI_SUCCESS;
}

/**
  Initialize the I3C controller

  @param[in]      SiPolicy         The SI Policy PPI instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       The SOC is not supported
              EFI_NOT_FOUND         I3C Config Block not found
**/
EFI_STATUS
PtlPcdLpssI3cSocInit (
  IN     SI_POLICY_PPI          *SiPolicy
  )
{
  EFI_STATUS                Status;
  BOOLEAN                   ErrorFlag;
  UINT8                     I3cIndex;
  LPSS_I3C_CONFIG           *LpssI3cConfig;
  LPSS_I3C_HANDLE           LpssI3cHandle;

  DEBUG ((DEBUG_INFO, "%a: Start.\n", __FUNCTION__));

  Status = GetConfigBlock ((VOID *) SiPolicy, &gLpssI3cConfigGuid, (VOID *) &LpssI3cConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get I3c config block!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  ErrorFlag = FALSE;
  for (I3cIndex = 0; I3cIndex < GetMaxI3cInterfacesNum (); I3cIndex++) {
    Status = PtlPcdLpssI3cBuildHandle (SiPolicy, &LpssI3cHandle, &(LpssI3cConfig->I3cDeviceConfig[I3cIndex]), I3cIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot build I3C handle structure for Ctrl index %d (Status %r)!\n", __FUNCTION__, I3cIndex, Status));
      DEBUG ((DEBUG_ERROR, "Skip I3C Initialization for controller index %d!\n", I3cIndex));
      ErrorFlag = TRUE;
      PtlPcdLpssI3cControllerFree (LpssI3cHandle.Controller);
      continue;
    }
    Status = LpssI3cInit (&LpssI3cHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Failed to initialize I3C controller #%d!\n", __FUNCTION__, I3cIndex));
      ErrorFlag = TRUE;
    }
    PtlPcdLpssI3cControllerFree (LpssI3cHandle.Controller);
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
