/** @file
  VTD Info library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuPlatformLib.h>
#include <VtdInfo.h>
#include <Register/HacRegs.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/IpuLib.h>
#include <Library/NpuInfoLib.h>
#include <Library/IaxInfoLib.h>

///
/// Miscellaneous 64bit Masks
/// Description of VTDBAR bit (38:12)
///
#define B_SA_MCHBAR_VTDBAR_MASK_64B    (0x0000007FFFFFF000ULL)

/**
  Get VT-d BAR size.

  @return     VT-d BAR size
**/
UINT32
GetVtdBarSize (
  IN VTD_ENGINE VtdEngineNumber
  )
{
  return SIZE_64KB;
}

/**
  Get VT-d base address.

  @attention  This function returns VT-d base address even if a specific VT-d engine is not supported/enabled.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @return     VT-d engine base sddress
**/
UINT32
GetVtdBaseAddress (
  IN VTD_ENGINE VtdEngineNumber
  )
{
  if (VtdEngineNumber >= VTD_ENGINE_NUMBER) {
    return 0x0;
  }

  /**
    VT-d MMIO space looks as it's presented below:
               /   +========================+  \
              |    | VT-d(7) register space |   |
              |    +------------------------+    > currently unused, for future VT-d engines
              |    |          ...           |   |
              |    +------------------------+  /
      512KB  <     | VT-d(2) register space |
              |    +------------------------+ <====== IOC VT-d base address
              |    | VT-d(1) register space |
              |    +------------------------+ <====== non-GFX VT-d base address
              |    | VT-d(0) register space |
               \   +========================+ <====== GFX VT-d base address
  **/
  return (PcdGet32 (PcdVtdMmioBase) + (VtdEngineNumber * GetVtdBarSize (VtdEngineNumber)));
}

/**
  Check if a specific VT-d engine is supported.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @retval     TRUE              Specific VT-d engine is supported.
  @retval     FALSE             Specific VT-d engine isn't supported.
**/
BOOLEAN
IsVtdEngineSupported (
  IN VTD_ENGINE  VtdEngineNumber
  )
{
  if (VtdEngineNumber >= VTD_ENGINE_NUMBER) {
    return FALSE;
  }

  return TRUE;
}

/**
  Get VT-d engine BAR offset.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @return     VT-d engine BAR offset.
**/
UINT32
GetVtdBarOffset (
  IN  VTD_ENGINE VtdEngineNumber
  )
{
  return VTDBAR_NCU_CCF_IDP_MCHBAR_REG;
}

/**
  Get VT-d engine BAR value.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @return     VT-d engine BAR value.
**/
UINT32
GetVtdBarValue (
  IN  VTD_ENGINE VtdEngineNumber
  )
{
  return PcdGet32 (PcdVtdMmioBase);
}

/**
  Check if a specific VT-d engine is enabled.

  @param[in]   VtdEngineNumber   VT-d engine number.

  @retval      TRUE              Specific VT-d engine is enabled.
  @retval      FALSE             Specific VT-d engine is disabled.
**/
BOOLEAN
IsVtdEngineEnabled (
  IN VTD_ENGINE  VtdEngineNumber
  )
{
  UINT32                               MchBar;
  UINT32                               VtdBarOffset;
  VTDBAR_NCU_CCF_IDP_MCHBAR_STRUCT     VtdBar;
  UINT8                                Bitmask;

  //
  // Check whether VT-d is supported by HW
  //
  if (!IsVtdSupported ()) {
    return FALSE;
  }

  //
  // Check whether a specified VT-d engine is supported by HW
  //
  if (!IsVtdEngineSupported (VtdEngineNumber)) {
    return FALSE;
  }

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  VtdBarOffset = GetVtdBarOffset (VtdEngineNumber);

  //
  // Check if enable bit is set
  //
  VtdBar.Data32[0] = MmioRead32 (MchBar + VtdBarOffset);

  if (VtdBar.Data32[0] == 0xFFFFFFFF) {
    return FALSE;
  }

  Bitmask = (UINT8)(1 << VtdEngineNumber);
  if ((VtdBar.Bits.vtdbaren & Bitmask) == 0) {
    return FALSE;
  }

  return TRUE;
}

/**

  ReadVtdBar: Get 64 Bit Iop VT-d Engine Base Address.

  @retval 64-bit VT-d BAR Value.
**/
UINT64
ReadVtdBar (
  VOID
  )
{
  UINT64          Vtd;
  UINT32          VtdBarOffset;
  UINTN           SaMchBar;

  SaMchBar = (UINTN) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  if (GetVtdEngineInfo (IOP_VTD, NULL, NULL, &VtdBarOffset, NULL)) {
    Vtd = MmioRead64 (SaMchBar + VtdBarOffset);
    return Vtd & B_SA_MCHBAR_VTDBAR_MASK_64B;
  }
  return 0;
}

typedef struct {
  struct {
    UINT8  Bus;
    UINT8  Device   : 5;
    UINT8  Function : 3;
  } Bdf;
  VTD_ENGINE VtdEngine;
  UINT8      Properties; // DeviceScope[].Flags
} PCIE_INFO;

PCIE_INFO PcieInfo[] = {
  {{0, 0x2, 0x0}, GFX_VTD,     (BIT0 | BIT1 | BIT2 | BIT3 | BIT4)}, // GFX
  {{0, 0x4, 0x0}, NON_GFX_VTD, 0}, // Camarillo
  {{0, 0x5, 0x0}, NON_GFX_VTD, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4)}, // IPU
  {{0, 0xB, 0x0}, NON_GFX_VTD, (BIT2 | BIT3 | BIT4)}, // NPU
  {{0, 0xC, 0x0}, NON_GFX_VTD, (BIT3 | BIT4)}, // IAA
};

/**
  The functions returns the IOMMU instance which supports a given PCIe device.

  @attention
  The function doesn't check if a given PCIe device exists or is enabled. This is
  a responsibility of a caller.

  @param[in]  Bus          PCIe Bus number
  @param[in]  Device       PCIe Device number
  @param[in]  Function     PCIe Function number

  @retval  VT-d engine number
**/
VTD_ENGINE
GetVtdEngineForPciDevice (
  IN UINT8  Bus,
  IN UINT8  Device,
  IN UINT8  Function
  )
{
  UINT8  Index;

  for (Index = 0; Index < ARRAY_SIZE (PcieInfo); Index++) {
    if ((Bus      == PcieInfo[Index].Bdf.Bus) &&
        (Device   == PcieInfo[Index].Bdf.Device) &&
        (Function == PcieInfo[Index].Bdf.Function))
    {
      return PcieInfo[Index].VtdEngine;
    }
  }

  return GLOBAL_VTD;
}

/**
  The functions returns the properties for a given PCIe device.

  @attention
  The function doesn't check if a given PCIe device exists or is enabled. This is
  a responsibility of a caller.

  @param[in]  Bus          PCIe Bus number
  @param[in]  Device       PCIe Device number
  @param[in]  Function     PCIe Function number

  @retval  PCIe device properties
**/
UINT8
GetPropertiesForPciDevice (
  IN UINT8  Bus,
  IN UINT8  Device,
  IN UINT8  Function
  )
{
  UINT8  Index;

  for (Index = 0; Index < ARRAY_SIZE (PcieInfo); Index++) {
    if ((Bus      == PcieInfo[Index].Bdf.Bus) &&
        (Device   == PcieInfo[Index].Bdf.Device) &&
        (Function == PcieInfo[Index].Bdf.Function))
    {
      return PcieInfo[Index].Properties;
    }
  }

  return 0;
}