/** @file
  PEI Vtd Init FRU Lib header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _PEI_VTD_INIT_FRU_LIB_H_
#define _PEI_VTD_INIT_FRU_LIB_H_

#include <Uefi.h>
#include <Ppi/SiPolicy.h>

/**
  The function is responsible for setting vtden bit in all PSFs (RS0) in CPU, SOC and PCH (if present).

  Vtden bit will force all upstream transaction to go to IOMMU so will no bypass vtd logic. This has been necessary since PSF 2.0.
**/
VOID
PsfEnableVtd (
  VOID
  );

/**
  Configure VT-d BAR in pre-mem.

  In pre-mem BIOS FW initializes VT-d BAR and enables Gfx VT-d engine if needed and
  global VT-d engine.

  @param[in] Vtd               VT-d config block

  @retval    EFI_DEVICE_ERROR  VT-d BAR cannot be enabled
  @retval    EFI_SUCCESS       Function executed successfully
**/
EFI_STATUS
ConfigureVtdBarPreMem (
  IN  VTD_CONFIG  *Vtd
  );

/**
  Configure VT-d BAR in post-mem.

  It enables non-Gfx VT-d engine if at least one of devices which are within
  scope of non-Gfx VT-d engine is enabled.

  @param[in] Vtd               VT-d config block

  @retval    EFI_DEVICE_ERROR  VT-d BAR cannot be enabled
  @retval    EFI_SUCCESS       Function executed successfully
**/
EFI_STATUS
ConfigureVtdBarPostMem (
  IN  VTD_CONFIG  *Vtd
  );

/**
  Disable VT-d BAR.

  @retval    EFI_SUCCESS       Function executed successfully
**/
EFI_STATUS
DisableVtdBar (
  VOID
  );

/**
  Check if VT-d is disabled.


  @param[in] Vtd               VT-d config block

  @retval    FALSE  VT-d is enabled
  @retval    TRUE   VT-d is disabled
**/
BOOLEAN
IsVtdDisabled (
  IN  VTD_CONFIG  *Vtd
  );
#endif // _PEI_VTD_INIT_FRU_LIB_H_
