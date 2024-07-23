/** @file
  @This file implements the functions to get VMD info.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Register/VmdRegs.h>
#include <Library/VmdInfoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Defines/HostBridgeDefines.h>

#define BAR_VALUE_MASK   0xFFFFFFF0

/**
  GetVmdBusNumber: Get VMD Bus Number

  @retval PCI bus number for VMD
**/
UINT8
EFIAPI
GetVmdBusNumber (
  VOID
  )
{
  return (UINT8)VMD_BUS_NUM;
}

/**
  GetVmdDevNumber: Get VMD Dev Number

  @retval PCI dev number for VMD
**/
UINT8
EFIAPI
GetVmdDevNumber (
  VOID
  )
{
  return (UINT8)VMD_DEV_NUM;
}

/**
  GetVmdFunNumber: Get VMD Fun Number

  @retval PCI fun number for VMD
**/
UINT8
EFIAPI
GetVmdFuncNumber (
  VOID
  )
{
  return (UINT8)VMD_FUN_NUM;
}

/**
  IsVmdEnabled: Check if VMD is enabled by reading the VMD Device Id

  @retval TRUE  if  VMD is enabled
  @retval False if  VMD is not enabled
**/

BOOLEAN
EFIAPI
IsVmdEnabled ()
{
  UINT16 DevId;
  DevId = PciSegmentRead16 (
            PCI_SEGMENT_LIB_ADDRESS (
              SA_SEG_NUM,
              VMD_BUS_NUM,
              VMD_DEV_NUM,
              VMD_FUN_NUM,
              PCI_DEVICE_ID_OFFSET
            )
          );
  if (DevId == 0xFFFF) {
    return FALSE;
  }
  return TRUE;
}



/**
  IsVmdLocked: Get the status of VMD lock for critical config space registers of VMD

  @retval TRUE if  VMD lock bit is set
  @retval False if  VMD lock bit is not set
**/
BOOLEAN
EFIAPI
IsVmdLocked (
  VOID
  )
{
  if ((PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, VMD_BUS_NUM, VMD_DEV_NUM, VMD_FUN_NUM, VMCONFIG_IOC_VMD_REG)) & BIT0) != 0) {
    return TRUE;
  }
  return FALSE;
}

/**
  IsVmdSupported: Get the status of VMD support bit from capability register.

  @retval TRUE  if  VMD is supported
  @retval False if  VMD is not supported
**/
BOOLEAN
EFIAPI
IsVmdSupported (
  VOID
  )
{
#if (FixedPcdGetBool (PcdVmdEnable) == 1)
  BOOLEAN  VmdEnable;

  VmdEnable = UncoreCapIdRead (Vmd, 0);
  if (VmdEnable == FALSE) {
    DEBUG ((DEBUG_INFO, "------- Vmd is not supported -----\n"));
    return FALSE;
  }
  return TRUE;
#else
  return FALSE;
#endif
}


/**
  ProgramVmdTempBars: Program VMD temporary bars.

  @param[in]     VmdCfgBarBase    temporary address for CfgBar.
  @param[in]     VmdMemBar1Base   temporary address for CfgBar.
  @param[in]     VmdMemBar1Base   temporary address for CfgBar.

  @retval VOID
**/
VOID
EFIAPI
ProgramVmdTempBars(
    IN UINT32 VmdCfgBarBase,
    IN UINT32 VmdMemBar1Base,
    IN UINT32 VmdMemBar2Base
  )
{
  UINTN           DeviceBaseAddress;

  ///
  /// Program VMD  temp PEI Bars
  ///
  DEBUG((DEBUG_INFO, "Programming temp VMD Bars \n"));
  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, VMD_BUS_NUM, VMD_DEV_NUM, VMD_FUN_NUM,0);
  PciSegmentWrite32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG, VmdCfgBarBase);  //32MB
  PciSegmentWrite32 (DeviceBaseAddress + MEMBAR1_IOC_VMD_REG, VmdMemBar1Base);  //32MB
  PciSegmentWrite32 (DeviceBaseAddress + MEMBAR2_IOC_VMD_REG, VmdMemBar2Base);  //1MB
  ///
  /// Read back the programmed BARs
  ///
  DEBUG ((DEBUG_INFO, "VMD CFG BAR 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG)) & BAR_VALUE_MASK));
  DEBUG ((DEBUG_INFO, "VMD MemBar1 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + MEMBAR1_IOC_VMD_REG)) & BAR_VALUE_MASK));
  DEBUG ((DEBUG_INFO, "VMD MemBar2 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + MEMBAR2_IOC_VMD_REG)) & BAR_VALUE_MASK));
}

/**
  ClearVmdTempBars: Clear VMD temporary bars.

  @retval VOID
**/
VOID
EFIAPI
ClearVmdTempBars(
  VOID
  )
{
  UINTN           DeviceBaseAddress;

  ///
  /// Clear VMD  temp PEI Bars
  ///
  if (IsVmdEnabled() == TRUE) {
    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, VMD_BUS_NUM, VMD_DEV_NUM, VMD_FUN_NUM,0);
    DEBUG ((DEBUG_INFO, "VMD: Reading VMD temp PEI Bars to make sure its cleared\n"));
    // Read back the programmed BARs
    DEBUG ((DEBUG_INFO, "VMD CFG BAR 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG)) & BAR_VALUE_MASK));
    DEBUG ((DEBUG_INFO, "VMD MemBar1 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + MEMBAR1_IOC_VMD_REG)) & BAR_VALUE_MASK));
    DEBUG ((DEBUG_INFO, "VMD MemBar2 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + MEMBAR2_IOC_VMD_REG)) & BAR_VALUE_MASK));
    if ((PciSegmentRead32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG) & BAR_VALUE_MASK) != 0) {
      DEBUG ((DEBUG_INFO, "VMD: Clearing VMD  temp PEI Bars \n"));
      PciSegmentWrite32 (DeviceBaseAddress + PCI_COMMAND_OFFSET, 0x0);
      PciSegmentWrite32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG, 0x0);
      PciSegmentWrite32 (DeviceBaseAddress + MEMBAR1_IOC_VMD_REG, 0x0);
      PciSegmentWrite32 (DeviceBaseAddress + MEMBAR2_IOC_VMD_REG, 0x0);
    } else {
      DEBUG ((DEBUG_INFO, "VMD: Temp PEI Bars are cleared!!\n"));
    }
    ///
    /// Read back the BARs
    ///
    DEBUG ((DEBUG_INFO, "VMD CFG BAR 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG)) & BAR_VALUE_MASK));
    DEBUG ((DEBUG_INFO, "VMD MemBar1 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + MEMBAR1_IOC_VMD_REG)) & BAR_VALUE_MASK));
    DEBUG ((DEBUG_INFO, "VMD MemBar2 0X%x:\n", (PciSegmentRead32 (DeviceBaseAddress + MEMBAR2_IOC_VMD_REG)) & BAR_VALUE_MASK));
  }
}
