/** @file
    CPUID TSE related functions.

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

#ifndef _TSE_CPU_ID_H_
#define _TSE_CPU_ID_H_

/**
   This function return if PCONFIG instruction set for TSE key programming
   is supported

   @retval TRUE             If TSE PCONFIG is supported
   @retval FALSE            If TSE PCONFIG is not supported
**/
BOOLEAN
EFIAPI
IsTsePconfigSupported (
  VOID
  );

/**
   This function returns if TSE capability and PBNDKB instruction exists.

   @retval TRUE             If TSE is supported
   @retval FALSE            If TSE is not supported
**/
BOOLEAN
EFIAPI
IsTseCapabilityPresent (
  VOID
  );

#endif // _TSE_CPU_ID_H_
