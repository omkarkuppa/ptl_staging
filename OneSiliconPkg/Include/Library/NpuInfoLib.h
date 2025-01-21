/** @file
  @ Header file to get NPU information.

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
#ifndef _NPU_INFO_LIB_H_
#define _NPU_INFO_LIB_H_

/**
  GetNpuBusNumber: Get NPU Bus Number

  @retval PCI bus number for NPU
**/
UINT8
GetNpuBusNumber (
  VOID
  );

/**
  GetNpuDevNumber: Get NPU Dev Number

  @retval PCI dev number for NPU
**/
UINT8
GetNpuDevNumber (
  VOID
  );

/**
  GetNpuFunNumber: Get NPU Fun Number

  @retval PCI fun number for NPU
**/
UINT8
GetNpuFuncNumber (
  VOID
  );

/**
  IsNpuSupported: Check if NPU is supported or not by Silicon.

  @retval FALSE = NPU is fused off. TRUE = NPU is supported by Sillicon.
**/
BOOLEAN
IsNpuSupported (
  VOID
  );

/**
  IsNpuEnabled: Check if NPU is enabled or not.

  @retval TRUE = NPU is enabled.
  @retval FALSE = NPU is disabled.
**/
BOOLEAN
IsNpuEnabled (
  VOID
  );

#endif /* _NPU_INFO_LIB_H_ */
