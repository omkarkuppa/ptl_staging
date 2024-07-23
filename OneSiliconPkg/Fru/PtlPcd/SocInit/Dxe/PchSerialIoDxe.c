/** @file
  Initializes Serial IO Controllers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include <Library/S3BootScriptLib.h>
#include <Library/LpssI3cPrivateLib.h>
#include <Library/LpssI2cPrivateLib.h>
#include <Library/LpssControllerInfoLib.h>
#include <Library/PchInfoLib.h>
#include <Library/LpssI2cSocLib.h>
#include <IndustryStandard/Pci.h>
#include <Register/SerialIoRegs.h>
#include <PchInit.h>
#include <I3cCtrl.h>
#include <Library/HobLib.h>
#include <LpssI3cConfigHob.h>

/**
  Stores Pme Control Status and Command register values in S3 Boot Script

  @param[in] S3PciCfgBase      S3 Boot Script Pci Config Base
  @param[in] Command           Pci Command register data to save
  @param[in] Pme               Pci Pme Control register data to save

**/
VOID
STATIC
SerialIoS3Save (
  IN UINTN   S3PciCfgBase,
  IN UINTN   Command,
  IN UINTN   Pme
  )
{
  if (S3PciCfgBase != 0) {
    S3BootScriptSavePciCfgWrite (S3BootScriptWidthUint32, S3PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, 1, &Pme);
    S3BootScriptSavePciCfgWrite (S3BootScriptWidthUint32, S3PciCfgBase + PCI_COMMAND_OFFSET, 1, &Command);
  }
}

/**
  Puts all SerialIo controllers (except UARTs in debug mode) in D3.
  Clears MemoryEnable for all PCI-mode controllers on S3 resume
**/
VOID
ConfigureSerialIoAtS3Resume (
  VOID
  )
{
  LPSS_I3C_CONTROLLER         *I3cController;
  LPSS_I2C_CONTROLLER         *I2cController;
  LPSS_I2C_CONTROLLER_CONFIG  *I2cConfig;
  PCI_CONFIG_SPACE_ACCESS     *CfgAccess;
  UINT8                       Index;
  UINT64                      S3PciCfgBase;
  UINT32                      Command;
  UINT32                      Pme;
  BOOLEAN                     TurnOff;
  VOID                        *HobPtr;
  LPSS_I2C_CONFIG_HOB         *LpssI2cConfigHob;
  LPSS_I3C_CONFIG_HOB         *LpssI3cConfigHob;

  HobPtr = GetFirstGuidHob (&gPchLpssI2cConfigHobGuid);
  if (HobPtr == NULL) {
    ASSERT (FALSE);
    return;
  }
  LpssI2cConfigHob = (LPSS_I2C_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);

  S3PciCfgBase = 0;
  I2cController = NULL;
  for (Index = 0; Index < GetMaxI2cInterfacesNum (); Index++) {
    PtlPcdLpssI2cGetController (Index , &I2cController);
    CfgAccess = (PCI_CONFIG_SPACE_ACCESS *)I2cController->PcieCfgAccess;
    I2cConfig = &LpssI2cConfigHob->I2cDeviceConfig[Index];
    S3PciCfgBase = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS ( CfgAccess->Bus,
                                                    CfgAccess->Dev,
                                                    CfgAccess->Fun,
                                                    0
                                                    );
    TurnOff = FALSE;
    if (I2cConfig->Mode == LpssI2cPci) {
      TurnOff = TRUE;
    }
    if ((I2cConfig->Mode == LpssI2cDisabled) && (CfgAccess->Fun == 0)) {
      if (LpssI2cHigherFunctionsEnabled (CfgAccess->Seg, CfgAccess->Bus, CfgAccess->Dev)) {
        TurnOff = TRUE;
      }
    }
    LpssI2cS3Handler (I2cController, TurnOff, &Command, &Pme);
    SerialIoS3Save (S3PciCfgBase, Command, Pme);
  }

  if (IsPtlPcd ()) {
    S3PciCfgBase = 0;
    HobPtr = GetFirstGuidHob (&gPchLpssI3cConfigHobGuid);
    if (HobPtr == NULL) {
      ASSERT (FALSE);
      return;
    }
    LpssI3cConfigHob = (LPSS_I3C_CONFIG_HOB *) GET_GUID_HOB_DATA (HobPtr);
    for (Index = 0; Index < GetMaxI3cInterfacesNum (); Index++) {
      if (LpssI3cConfigHob->I3cDeviceConfig[Index].Mode < I3cPhantom) {
        PtlPcdLpssI3cGetController (Index, &I3cController);
        LpssI3cS3Handler (I3cController, TurnOff, &Command, &Pme);
        SerialIoS3Save (S3PciCfgBase, Command, Pme);
      }
    }
  }
}
