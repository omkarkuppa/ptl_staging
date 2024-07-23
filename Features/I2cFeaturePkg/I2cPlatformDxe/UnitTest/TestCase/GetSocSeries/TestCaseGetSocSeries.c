/** @file
  Implementations to get the Device ID based on SOC Segment.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include "TestCaseGetSocSeries.h"

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mSocPtlPcdPLpssI2cDevId[] = {
  PTL_PCD_P_LPSS_I2C_CFG_I2C0_DEVICE_ID,
  PTL_PCD_P_LPSS_I2C_CFG_I2C1_DEVICE_ID,
  PTL_PCD_P_LPSS_I2C_CFG_I2C2_DEVICE_ID,
  PTL_PCD_P_LPSS_I2C_CFG_I2C3_DEVICE_ID,
  PTL_PCD_P_LPSS_I2C_CFG_I2C4_DEVICE_ID,
  PTL_PCD_P_LPSS_I2C_CFG_I2C5_DEVICE_ID
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mSocPtlPcdHLpssI2cDevId[] = {
  PTL_PCD_H_LPSS_I2C_CFG_I2C0_DEVICE_ID,
  PTL_PCD_H_LPSS_I2C_CFG_I2C1_DEVICE_ID,
  PTL_PCD_H_LPSS_I2C_CFG_I2C2_DEVICE_ID,
  PTL_PCD_H_LPSS_I2C_CFG_I2C3_DEVICE_ID,
  PTL_PCD_H_LPSS_I2C_CFG_I2C4_DEVICE_ID,
  PTL_PCD_H_LPSS_I2C_CFG_I2C5_DEVICE_ID
};


SOC_SERIES
GetSocSeries () 
{
  UINT64  LpcBaseAddress;
  UINT16  EspiDid;
  LpcBaseAddress = PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_LPC,
           PCI_FUNCTION_NUMBER_PCH_LPC,
           0
           );
  EspiDid = PciSegmentRead16 (LpcBaseAddress + PCI_DEVICE_ID_OFFSET);

  switch (EspiDid & LPC_CFG_DID) {
    case ESPI_CFG_DID_PTL_PCD_P:
      return SOC_PTL_PCD_P;
    case ESPI_CFG_DID_PTL_PCD_H:
      return SOC_PTL_PCD_H;
    default:
      return SOC_UNKNOWN_SERIES;
  }
}

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
  SOC_SERIES     SocSeries;
  SocSeries = GetSocSeries ();

  if (SocSeries == SOC_PTL_PCD_P) {
    if (I2cNumber < ARRAY_SIZE (mSocPtlPcdPLpssI2cDevId)) {
      return mSocPtlPcdPLpssI2cDevId[I2cNumber];
    }
  } else if (SocSeries == SOC_PTL_PCD_H) {
    if (I2cNumber < ARRAY_SIZE (mSocPtlPcdHLpssI2cDevId)) {
      return mSocPtlPcdHLpssI2cDevId[I2cNumber];
    }
  }
  return 0xFFFF;
}