/** @file
  Header file for VtdInfoLib.

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
#ifndef _VTD_INFO_LIB_H_
#define _VTD_INFO_LIB_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <VtdInfo.h>

#pragma pack(1)

/**
  Check if VT-d is supported by Silicon.

  @retval TRUE   Supported
  @retval FALSE  Unsupported
**/
BOOLEAN
IsVtdSupported (
  VOID
  );

/**
  Get VT-d BAR size.

  @return     VT-d BAR size
**/
UINT32
GetVtdBarSize (
  IN VTD_ENGINE VtdEngineNumber
  );

/**
  Get VT-d base address.

  @attention  This function returns VT-d base address even if a specific VT-d engine is not supported/enabled.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @return     VT-d engine base sddress
**/
UINT32
GetVtdBaseAddress (
  IN VTD_ENGINE VtdEngineNumber
  );

/**
  Get maximum number of VT-d engines.

  @retval Vtd Engine Number
**/
UINT8
GetMaxVtdEngineNumber (
  VOID
  );

/**
  Get basic information about a specific VT-d engine.

  The function returns also information whether a specific VT-d engine is supported by HW. It also can provide data
  which can be used to enable specific VT-d engine: base address, size of VT-d register space, BAR offset. These data are
  optional, and are returned only if a caller provides buffers.

  @attention DMA Remapping Hardware Units on the SoC can presented by a unfied BAR. In this case register space for
             VT-d engines must be continuous, and the unified BAR is used to assign memory to all supported VT-d engines.
             +========================+
             | VT-d(N) register space |
             +------------------------+ <=============== VT-d(N) base address
             |          ...           |
             +------------------------+
             | VT-d(1) register space |
             +------------------------+ <=============== VT-d(1) base address
             | VT-d(0) register space |
             +========================+ <=============== VT-d(0) base address (with this value BAR must be programmed,
                                                                               it is returned in BarValueToProgram)


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
  );

/**
  Check if specific VT-d is enabled.

  @param[in]   VtdEngineNumber   VT-d engine number.

  @retval      TRUE              Specific VT-d engine is enabled.
  @retval      FALSE             Specific VT-d engine is disabled.
**/
BOOLEAN
IsVtdEngineEnabled (
  IN VTD_ENGINE VtdEngineNumber
  );

/**
  Check if PMR (protected memory region) is enabled for a specific VT-d engine.

  @param[in]   VtdEngineNumber   VT-d engine number.

  @retval      TRUE              PMR for specific VT-d engine is enabled.
  @retval      FALSE             PMR for specific VT-d engine is disabled.
**/
BOOLEAN
IsVtdPmrEnabled (
  IN VTD_ENGINE VtdEngineNumber
  );

/**
  Check if DMA remapping is enabled or not for a specific VT-d engine.

  @param[in]   VtdEngineNumber   VT-d engine number.

  @retval      TRUE              DMA remapping for specific VT-d engine is enabled.
  @retval      FALSE             DMA remapping for specific VT-d engine is disabled.
**/
BOOLEAN
IsVtdDmaRemappingEnabled (
  IN VTD_ENGINE VtdEngineNumber
  );

/**
  Check if a specific VT-d engine is supported.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @retval     TRUE              Specific VT-d engine is supported.
  @retval     FALSE             Specific VT-d engine isn't supported.
**/
BOOLEAN
IsVtdEngineSupported (
  IN VTD_ENGINE  VtdEngineNumber
  );

/**

  ReadVtdBar: Get 64 Bit Iop VT-d Engine Base Address.

  @retval 64-bit VT-d BAR Value.
**/
UINT64
ReadVtdBar (
  VOID
  );

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
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Function
  );

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
  );

#pragma pack()
#endif // _VTD_INFO_LIB_H_
