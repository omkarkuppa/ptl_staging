/** @file
  Lpss I2C Private Lib implementation MeteorLake specific.
  All function in this library is available for PEI, DXE, and SMM,
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
#include <IndustryStandard/Pci.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdInfoLib.h>
#include "LpssI2cDeviceIds.h"

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPtlPcdPLpssI2cDevId[] = {
  V_PTL_PCD_P_LPSS_I2C_CFG_I2C0_DEVICE_ID,
  V_PTL_PCD_P_LPSS_I2C_CFG_I2C1_DEVICE_ID,
  V_PTL_PCD_P_LPSS_I2C_CFG_I2C2_DEVICE_ID,
  V_PTL_PCD_P_LPSS_I2C_CFG_I2C3_DEVICE_ID,
  V_PTL_PCD_P_LPSS_I2C_CFG_I2C4_DEVICE_ID,
  V_PTL_PCD_P_LPSS_I2C_CFG_I2C5_DEVICE_ID
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPtlPcdHLpssI2cDevId[] = {
  V_PTL_PCD_H_LPSS_I2C_CFG_I2C0_DEVICE_ID,
  V_PTL_PCD_H_LPSS_I2C_CFG_I2C1_DEVICE_ID,
  V_PTL_PCD_H_LPSS_I2C_CFG_I2C2_DEVICE_ID,
  V_PTL_PCD_H_LPSS_I2C_CFG_I2C3_DEVICE_ID,
  V_PTL_PCD_H_LPSS_I2C_CFG_I2C4_DEVICE_ID,
  V_PTL_PCD_H_LPSS_I2C_CFG_I2C5_DEVICE_ID
};


/**
  Gets I2C Device Id

  @param[in] I2cNumber               Lpss device I2C number

  @retval                            Device Id
**/
UINT16
GetLpssI2cDeviceId (
  IN UINT8       I2cNumber
  )
{
  if (PtlIsPcdP()) {
    if (I2cNumber < ARRAY_SIZE (mPtlPcdPLpssI2cDevId)) {
      return mPtlPcdPLpssI2cDevId[I2cNumber];
    }
  } else {
    if (I2cNumber < ARRAY_SIZE (mPtlPcdHLpssI2cDevId)) {
      return mPtlPcdHLpssI2cDevId[I2cNumber];
    }
  }
  return 0xFFFF;
}

/**
  Checks if higher PCIe functions are enabled.
  Used for Function 0 Lpss Device disabling

  @param[in] Seg    PCIe Segment
  @param[in] Bus    PCIe Bus
  @param[in] Dev    PCIe Devie

  @retval TRUE            At least one higher function device is enabled
          FALSE           Higher functions are disabled
**/
BOOLEAN
LpssI2cHigherFunctionsEnabled (
  IN  UINT8   Seg,
  IN  UINT8   Bus,
  IN  UINT8   Dev
  )
{
  UINT8   FuncNum;
  UINT16  DevId;
  //
  // Check all other func devs(1 to 7) status except func 0.
  //
  for (FuncNum = 1; FuncNum <= PCI_MAX_FUNC; FuncNum++) {
    DevId = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (Seg, Bus, Dev, FuncNum, 0));
    if (DevId != 0xFFFF) {
      return TRUE;
    }
  }
  return FALSE;
}
