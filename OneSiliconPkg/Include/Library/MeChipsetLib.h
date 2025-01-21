/** @file
  Header file for Me Chipset Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef _ME_CHIPSET_LIB_H_
#define _ME_CHIPSET_LIB_H_

#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <MeDefines.h>

///
/// Function API
///

/**
  Checks if the given PCIe ME Device Function is HECI Device Function

  @param[in]     Function  ME Device Function

  @retval TRUE   This is a HECI Device Function
  @retval FALSE  This is not a HECI Device Function
**/
BOOLEAN
IsHeciDeviceFunction (
  IN HECI_DEVICE Function
  );

/**
  Take ME device out of D0I3

  @param[in] Function  ME function where D0I3 is to be changed

**/
VOID
ClearD0I3Bit (
  IN  HECI_DEVICE   Function
  );

/**
  Put ME device into D0I3

  @param[in] Function          Select of Me device

**/
VOID
SetD0I3Bit (
  IN  HECI_DEVICE   Function
  );

/**
  Get D0I3 status bit

  @param[in] Function          Select of Me device

  @retval    TRUE              D0I3 status bit is enabled
  @retval    FALSE             D0I3 status bit is not enabled
**/
BOOLEAN
IsD0I3BitEnabled (
  IN  HECI_DEVICE   Function
  );

#endif
