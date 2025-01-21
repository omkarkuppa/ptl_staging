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

#include <Ppi/SiPolicy.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/VtdInfoFruLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/VtdRegs.h>

/**
  Check if VT-d is supported by Silicon.

  @retval     TRUE   Supported
  @retval     FALSE  Unsupported
**/
BOOLEAN
IsVtdSupported (
  VOID
  )
{
  //
  // Check if Silicon is VT-d capable
  //
  return UncoreCapIdRead (Vtd, 0);
}

/**
  Get maximum number of VT-d engines.

  @return     Maximum number of VT-d engines
**/
UINT8
GetMaxVtdEngineNumber (
  VOID
  )
{
  return (UINT8)VTD_ENGINE_NUMBER;
}

/**
  Check if PMR (protected memory region) is enabled for a specific VT-d engine.

  @param[in]   VtdEngineNumber   VT-d engine number.

  @retval      TRUE              PMR for specific VT-d engine is enabled.
  @retval      FALSE             PMR for specific VT-d engine is disabled.
**/
BOOLEAN
IsVtdPmrEnabled (
  IN VTD_ENGINE  VtdEngineNumber
  )
{
  BOOLEAN      VtdEngineSupported;
  UINT32       VtdUnitBaseAddress;
  PMEN_VTD_IOMMU_STRUCT      PmenReg;
  CAP_VTD_IOMMU_STRUCT       CapReg;

  VtdEngineSupported = GetVtdEngineInfo (
                         VtdEngineNumber,
                         &VtdUnitBaseAddress,
                         NULL,
                         NULL,
                         NULL
                         );

  //
  // check if specific VT-d engine is supported by HW
  //
  if (!VtdEngineSupported) {
    return FALSE;
  }

  //
  // check if specific VT-d engine is enabled
  //
  if (!IsVtdEngineEnabled (VtdEngineNumber)) {
    return FALSE;
  }

  //
  // check if PMHR and PLMR are supported
  //
  CapReg.Data = MmioRead64 (VtdUnitBaseAddress + CAP_VTD_IOMMU_REG);
  if ((CapReg.Bits.phmr == 0) || (CapReg.Bits.plmr == 0)) {
    return FALSE;
  }

  //
  // check if PMR is enabled
  //
  PmenReg.Data = MmioRead32 (VtdUnitBaseAddress + PMEN_VTD_IOMMU_REG);
  if (PmenReg.Bits.prs == 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Check if DMA remapping is enabled or not for a specific VT-d engine.

  @param[in]   VtdEngineNumber   VT-d engine number.

  @retval      TRUE              DMA remapping for specific VT-d engine is enabled.
  @retval      FALSE             DMA remapping for specific VT-d engine is disabled.
**/
BOOLEAN
IsVtdDmaRemappingEnabled (
  IN VTD_ENGINE  VtdEngineNumber
  )
{
  BOOLEAN  VtdEngineSupported;
  UINT32   VtdUnitBaseAddress;
  GSTS_VTD_IOMMU_STRUCT   GstsReg;

  VtdEngineSupported = GetVtdEngineInfo (
                         VtdEngineNumber,
                         &VtdUnitBaseAddress,
                         NULL,
                         NULL,
                         NULL
                         );

  //
  // check if specific VT-d engine is supported by HW
  //
  if (!VtdEngineSupported) {
    return FALSE;
  }

  //
  // check if specific VT-d engine is enabled
  //
  if (!IsVtdEngineEnabled (VtdEngineNumber)) {
    return FALSE;
  }

  //
  // check the state of TE
  //
  GstsReg.Data = MmioRead32 (VtdUnitBaseAddress + GSTS_VTD_IOMMU_REG);
  if (GstsReg.Bits.tes == 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Get basic information about a specific VT-d engine.

  The function returns information whether a specific VT-d engine is supported by HW. It also can provide data which can
  be used to enable specific VT-d engine: base address, size of VT-d register space, BAR offset. These data are optional,
  and are returned only if a caller provides buffers.

  @attention DMA Remapping Hardware Units on the SoC can be presented by a unfied BAR. In this case register space for
             VT-d engines must be continuous, and the unified BAR is used to assign memory to all supported VT-d engines.
             +========================+
             | VT-d(N) register space |
             +------------------------+ <=============== VT-d(N) base address
             |          ...           |
             +------------------------+
             | VT-d(1) register space |
             +------------------------+ <=============== VT-d(1) base address
             | VT-d(0) register space |
             +========================+ <=============== VT-d(0) base address (BAR must be programmed with a value
                                                                               returned in BarValueToProgram)

  @param[in]   VtdEngineNumber         VT-d engine number.
  @param[out]  BaseAddress             Optional. VT-d base address.
  @param[out]  RegisterSpaceSize       Optional. Size of VT-d register space.
  @param[out]  BarOffset               Optional. BAR Offset (this BAR is used to enable specific VT-d engine).
                                       @attention Sometimes one BAR can be used to enable a few VT-d engines, returned
                                       BAR should be programmed with BarValueToProgram value to enable specific VT-d engine.
  @param[out]  BarValueToProgram       Optional. BAR must be programmed with this value in order to enable a specific
                                       VT-d engine.

  @retval      TRUE                    Specific VT-d engine is supported by HW.
  @retval      FALSE                   Specific VT-d engine isn't supported by HW.
**/
BOOLEAN
GetVtdEngineInfo (
  IN  VTD_ENGINE VtdEngineNumber,
  OUT UINT32     *BaseAddress,         OPTIONAL
  OUT UINT32     *RegisterSpaceSize,   OPTIONAL
  OUT UINT32     *BarOffset,           OPTIONAL
  OUT UINT32     *BarValueToProgram    OPTIONAL
  )
{
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

  if (BaseAddress != NULL) {
    *BaseAddress = GetVtdBaseAddress (VtdEngineNumber);
  }

  if (RegisterSpaceSize != NULL) {
    *RegisterSpaceSize = GetVtdBarSize (VtdEngineNumber);
  }

  if (BarOffset != NULL) {
    *BarOffset = GetVtdBarOffset (VtdEngineNumber);
  }

  if (BarValueToProgram != NULL) {
    *BarValueToProgram = GetVtdBarValue (VtdEngineNumber);
  }

  return TRUE;
}