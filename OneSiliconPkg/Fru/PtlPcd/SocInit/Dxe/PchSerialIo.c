/** @file
  Initializes Serial IO Controllers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#include "PchInit.h"
#include <Library/BaseLib.h>
#include <Library/LpssI2cPrivateLib.h>
#include <Library/LpssControllerInfoLib.h>
#include <Library/LpssI2cSocLib.h>
#include <Library/PchInfoLib.h>
#include <LpssI2cController.h>

/**
  Calls Boot Os Hand off routine for each Serial IO Controller
**/
VOID
ConfigureSerialIoAtBoot (
  VOID
  )
{
  UINT8                       Index;
  LPSS_I2C_CONTROLLER         *I2cController;
  LPSS_I2C_CONTROLLER_CONFIG  *I2cConfig;
  PCI_CONFIG_SPACE_ACCESS     *CfgAccess;
  BOOLEAN                     TurnOff;
  VOID                        *HobPtr;
  LPSS_I2C_CONFIG_HOB         *LpssI2cConfigHob;

  HobPtr = GetFirstGuidHob (&gPchLpssI2cConfigHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return;
  }
  LpssI2cConfigHob = (LPSS_I2C_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);

  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    I2cController = NULL;
    PtlPcdLpssI2cGetController (Index, &I2cController);
    CfgAccess = (PCI_CONFIG_SPACE_ACCESS *)I2cController->PcieCfgAccess;
    I2cConfig = &LpssI2cConfigHob->I2cDeviceConfig[Index];
    TurnOff = FALSE;
    if (I2cConfig->Mode == LpssI2cPci) {
      TurnOff = TRUE;
    }
    if ((I2cConfig->Mode == LpssI2cDisabled) && (CfgAccess->Fun == 0x0)) {
      if (LpssI2cHigherFunctionsEnabled (CfgAccess->Seg, CfgAccess->Bus, CfgAccess->Dev)) {
        TurnOff = TRUE;
      }
  }

    if (TurnOff) {
      LpssI2cBootHandler (I2cController);
    }
  }
}
