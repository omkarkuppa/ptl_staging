/** @file
  Common THC Confure Private Lib implementation
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

@par Specification Reference:
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Library/DebugLib.h>
#include <Library/PchLimits.h>
#include <Library/PchPciBdfLib.h>
#include <ThcConfig.h>
#include <Register/ThcRegs.h>

/**
  Places ThcInstance device in D3

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
ThcInstanceSetD3 (
  IN UINT64                    PciCfgBase
  )
{
  if (!PciCfgBase) {
    DEBUG ((DEBUG_WARN, "%a: Invalid value detected!\n", __FUNCTION__));
    return;
  }
  PciSegmentOr32 (PciCfgBase + R_THC_CFG_PMD_PMCSRBSE_PMCSR, V_THC_CFG_PMD_PMCSRBSE_PMCSR_PWRST_D3);
}

/**
  Configures THC Controller before control is passd to the OS

  @param[in] ThcIndex          Thc Controller Number
  @param[in] ThcDeviceConfig   ThcInstance device Config

**/
VOID
ThcBootHandler (
  IN UINT8                ThcIndex,
  IN THC_PORT             *ThcDeviceConfig
  )
{
  UINT64   PciCfgBase;

  if (ThcDeviceConfig->Mode == ThcModeNone) {
    PciCfgBase = ThcPciCfgBase (ThcIndex);
    DEBUG ((DEBUG_INFO, "%a: For THC Instance %d programming D3 ThcMode: %d\n", __FUNCTION__, ThcIndex, ThcDeviceConfig->Mode));
    ThcInstanceSetD3 (PciCfgBase);
  }
}
