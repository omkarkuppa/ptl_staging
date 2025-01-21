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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/Cpuid.h>

#define CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_TSE_CAPABILITY 0x1

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
  )
{
  UINT32 CpuidStructuredExtendedFeatureEdx;

  ///
  /// Verify PCONFIG ISA is supported through CPUID.7.0.EDX.18
  ///
  AsmCpuidEx (
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_SUB_LEAF_INFO,
    NULL,
    NULL,
    NULL,
    &CpuidStructuredExtendedFeatureEdx
    );

  DEBUG ((DEBUG_INFO, "[TSE] CPUID.7.0.EDX = 0x08%x\n", CpuidStructuredExtendedFeatureEdx));

  if ((CpuidStructuredExtendedFeatureEdx & BIT18) == BIT18) {
    return TRUE;
  }
  return FALSE;
}

/**
   This function returns if TSE capability and PBNDKB instruction exists.

   @retval TRUE             If TSE is supported
   @retval FALSE            If TSE is not supported
**/
BOOLEAN
EFIAPI
IsTseCapabilityPresent (
  VOID
  )
{
  UINT32 CpuidStructuredExtendedFeatureEbx;

  ///
  /// Verify TSE supported through CPUID.7.1.EBX.1
  ///
  AsmCpuidEx (
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS,
    CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_TSE_CAPABILITY,
    NULL,
    &CpuidStructuredExtendedFeatureEbx,
    NULL,
    NULL
    );

  DEBUG ((DEBUG_INFO, "[TSE] CPUID.7.1.EBX = 0x08%x\n", CpuidStructuredExtendedFeatureEbx));

  if ((CpuidStructuredExtendedFeatureEbx & BIT1) != BIT1) {
    return FALSE;
  }

  return TRUE;
}
