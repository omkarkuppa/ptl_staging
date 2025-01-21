/** @file
  This code provides a initialization of intel VT-d (Virtualization Technology for Directed I/O).

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _PEI_VTD_INIT_LIB_H_
#define _PEI_VTD_INIT_LIB_H_

///
/// Include files
///
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PostCodeLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/PciSegmentLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/CpuPlatformLib.h>
#include <IndustryStandard/Pci22.h>
#include <IpuPreMemConfig.h>


/**
  Configure VT-d Base and capabilities.

  @param[in]   VTD_CONFIG       VT-d config block from SA Policy PPI

  @retval      EFI_SUCCESS      VT-d initialization completed
  @exception   EFI_UNSUPPORTED  VT-d is not supported by silicon or not enabled by policy
**/
EFI_STATUS
VtdInit (
  IN  VTD_CONFIG           *Vtd
  );

/**
  Clear DMA Protection set by Boot Guard.

  @param[in]   Vtd              VTD config block from SA Policy PPI
**/
VOID
ClearDmaProtectionSetByBootGuard (
  IN  VTD_CONFIG     *Vtd
  );


/**
  Configure VT-d Base and capabilities for IPs available in PostMem.

  @param[in]   VTD_CONFIG       VTD config block from SA Policy PPI

  @retval EFI_SUCCESS         - Vtd initialization complete
  @exception EFI_UNSUPPORTED  - Vtd is not enabled by policy
**/
EFI_STATUS
VtdLateInit (
  IN  VTD_CONFIG           *Vtd
  );

/**
  Installs VTD Data Hob

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
InstallVtdDataHob (
  VOID
  );

/**
  Update Vtd Hob in PostMem

  @param[in]  Vtd               Instance of VTD_CONFIG

  @retval EFI_SUCCESS
**/
EFI_STATUS
UpdateVtdHobPostMem (
  IN  VTD_CONFIG    *Vtd
  );

/**
  Enable DMA buffer in VT-d.

  @param[in] VtdEngineNumber        VT-d engine number.

  @retval EFI_SUCCESS               Enable successfully.
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid.
  @retval EFI_UNSUPPORTED           VT-d isn't supported.
  @retval EFI_OUT_OF_RESOURCES      There is no additional space in the PPI database.
  @retval EFI_NOT_FOUND             Pointer to EnableDmaBuffer function is NULL.
**/
EFI_STATUS
VtdEnableDmaBuffer (
  IN  VTD_ENGINE    VtdEngineNumber
  );

#endif // _PEI_VTD_INIT_LIB_H_
