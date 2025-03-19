/** @file
  Internal file to support Foxville CRC operation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef __FOXVILLE_CRC_H__
#define __FOXVILLE_CRC_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleCommon.h"

/**
  Updates all the MNG sections CRC value.

  @param[in]  HwPtr  Pointer to the HW instance.

  @retval  EFI_SUCCESS            Succeed to update all the MNG section CRC value.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to update all the MNG section CRC value.

**/
EFI_STATUS
EFIAPI
UpdateMngCrcs (
  IN FOXVILLE_HW_INSTANCE  *HwPtr
  );

#endif
