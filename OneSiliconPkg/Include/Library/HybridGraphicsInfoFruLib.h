/** @file
  Header file for Hybrid Graphics Info Fru Lib implementation.

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
#ifndef HYBRID_GRAPHICS_INFO_FRU_LIB_H_
#define HYBRID_GRAPHICS_INFO_FRU_LIB_H_

/**
  Get Hybrid Graphics Root Port Device and Function Number by Root Port physical Number

  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
**/
EFI_STATUS
EFIAPI
GetHybridGraphicsRpDevFun (
  OUT UINTN   *RootPortDev,
  OUT UINTN   *RootPortFun
  );

#endif // HYBRID_GRAPHICS_INFO_FRU_LIB_H_
