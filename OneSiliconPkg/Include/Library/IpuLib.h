/** @file
  This file provides IPU Public Library

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
#ifndef _IPU_LIB_H_
#define _IPU_LIB_H_


/**
  GetIpuBusNumber: Get IPU Bus Number

  @retval PCI bus number for IPU
**/
UINT64
GetIpuBusNumber (
  VOID
  );

/**
  GetIpuDevNumber: Get IPU Dev Number

  @retval PCI dev number for IPU
**/
UINT64
GetIpuDevNumber (
  VOID
  );

/**
  GetIpuFunNumber: Get IPU Fun Number

  @retval PCI fun number for IPU
**/
UINT64
GetIpuFuncNumber (
  VOID
  );

/**
  IsIpuSupported: Check if IPU is supported or not by Silicon.

  @retval FALSE = IPU is fused off. TRUE = IPU is supported by Sillicon.
**/
BOOLEAN
IsIpuSupported (
  VOID
  );

#endif // _IPU_LIB_H_
