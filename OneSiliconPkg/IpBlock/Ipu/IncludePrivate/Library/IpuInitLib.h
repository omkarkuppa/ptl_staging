/** @file
  IPU device header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#ifndef _IPU_INIT_H_
#define _IPU_INIT_H_

#include <IpuPreMemConfig.h>
#include <IpuDataHob.h>
#include <IpuConfigHob.h>
/**
  IpuInit: Initialize the IPU device
**/
VOID
IpuInit (
 VOID
  );

/**
  IsIpuEnabled: Check if IPU is Enabled or not

  @retval FALSE = IPU is not supported, and/or IPU policy is disabled. TRUE = IPU is supported and IPU policy is enabled.
**/
BOOLEAN
IsIpuEnabled (
  VOID
  );

/**
  Init and Install IPU Hob
  @param[out] IPU_DATA_HOB        - IPU_DATA_HOB instance installed by this function
  @param[out] IPU_CONFIG_HOB      - IPU_CONFIG_HOB instance installed by this function

  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallIpuHob (
  OUT      IPU_DATA_HOB                 **IpuDataHobOut,
  OUT      IPU_CONFIG_HOB               **IpuConfigHobOut
  );

/**
  Check IPU policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] IpuPreMemPolicy    The IPU PreMem Policy instance

**/
VOID
IpuValidatePolicy (
  IN    IPU_PREMEM_CONFIG    *IpuPreMemPolicy
  );

/**
  Configure IMGUCLK

  @param[in] IpuPreMemPolicy    The IPU PreMem Policy instance
**/
VOID
ConfigureImguClkOutGpio (
  IN    IPU_PREMEM_CONFIG    *IpuPreMemPolicy
  );
#endif
