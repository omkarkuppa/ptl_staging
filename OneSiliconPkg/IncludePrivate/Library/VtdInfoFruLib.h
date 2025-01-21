/** @file
  Vtd Info FRU Lib header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _VTD_INFO_FRU_LIB_H_
#define _VTD_INFO_FRU_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <VtdInfo.h>

/**
  Get VT-d engine BAR offset.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @return     VT-d engine BAR offset.
**/
UINT32
GetVtdBarOffset (
  IN  VTD_ENGINE VtdEngineNumber
  );

/**
  Get VT-d engine BAR value.

  @param[in]  VtdEngineNumber   VT-d engine number.

  @return     VT-d engine BAR value.
**/
UINT32
GetVtdBarValue (
  IN  VTD_ENGINE VtdEngineNumber
  );

#endif // _VTD_INFO_FRU_LIB_H_
