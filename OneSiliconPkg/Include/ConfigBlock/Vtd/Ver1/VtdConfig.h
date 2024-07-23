/** @file
  VT-d policy definitions.

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
#ifndef _VTD_CONFIG_VER1_H_
#define _VTD_CONFIG_VER1_H_

#include <VtdInfo.h>

#pragma pack(push, 1)

#define VTD_CONFIG_REVISION 9

//
// Forward declaration for the VTd Enable DMA Buffer PPI.
//

/**
  Enable DMA buffer in VTd

  @param[in] VtdEngineNumber        VTd engine number.

  @retval EFI_SUCCESS               Enable successfully.
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid.
  @retval EFI_UNSUPPORTED           VTd base is zero.
  @retval EFI_OUT_OF_RESOURCES      There is no additional space in the PPI database.
**/
typedef
EFI_STATUS
(EFIAPI *VTD_ENABLE_DMA_BUFFER) (
  IN VTD_ENGINE VtdEngineNumber
  );

/**
  The data elements should be initialized by a Platform Module.
  The data structure is for VT-d driver initialization\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>
  - Removed VtdItbtEnable and BaseAddress fields.
  <b>Revision 3</b>
  - Restored BaseAddress field.
  <b>Revision 4</b>:
  - Added field EnableDmaBuffer.
  <b>Revision 5</b>:
  - Renamed VtdIgdEnable, VtdIpuEnable and VtdIopEnable fields.
  <b>Revision 6</b>:
  - Removed VtdIgdEnable, VtdIpuEnable and VtdIopEnable fields.
  <b>Revision 7</b>:
  - Removed EnableDmaBuffer field.
  <b>Revision 8</b>:
  - Removed Reserved fields.
  <b>Revision 9</b>:
  - Removed X2ApicOptOut field.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;                      ///< Offset 0-27 Config Block Header
  /**
    Offset 28:
    VT-D Support can be verified by reading CAP ID register as expalined in BIOS Spec.
    This policy is for debug purpose only.
    If VT-D is not supported, all other policies in this config block will be ignored.
    <b>0 = To use Vt-d</b>;
    1 = Avoids programming Vtd bars, Vtd overrides and DMAR table.
  **/
  UINT8                 VtdDisable;
  UINT8                 DmaControlGuarantee; ///< Offset 30: This field is used to enable the DMA_CONTROL_GUARANTEE bit in the DMAR table. 1=Enable/Set and <b>0=Disable/Clear</b>
  UINT8                 PreBootDmaMask;      ///< Offset 31: Convey PcdVTdPolicyPropertyMask value from EDK2 IntelSiliconPkg
  UINT8                 Reserved;
  UINT32                DmaBufferSize;       ///< Offset 32: Protect Memory Region (PMR) DMA buffer size
  UINT32                BaseAddress[VTD_ENGINE_NUMBER];  ///< Offset 36: This field is used to describe the base addresses for VT-d function
} VTD_CONFIG;

#pragma pack(pop)

#endif   //  _VTD_CONFIG_H_
