/** @file
  This file contains the tests for the HardwareRootedBootIntegrity bit

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include "HstiIhvDxe.h"
#include <Library/BootGuardLib.h>

/**
  Run tests for HardwareRootedBootIntegrity bit
**/
VOID
CheckHardwareRootedBootIntegrity (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINTN           CpuIndex;
  UINT64          SAcmInfo;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY) == 0) {
    return;
  }

  Result = TRUE;

  // Set CPU 0 (BSP)
  CpuIndex = 0;

  SAcmInfo = ProcessorReadMsr64 (CpuIndex, MSR_BOOT_GUARD_SACM_INFO);
  DEBUG ((DEBUG_INFO, "    SACM INFO Register value %x\n", SAcmInfo));
  DEBUG ((DEBUG_INFO, "    SACM INFO Register B_BOOT_GUARD_SACM_INFO_CAPABILITY bit = %x\n", (SAcmInfo & B_BOOT_GUARD_SACM_INFO_CAPABILITY)));

  DEBUG ((DEBUG_INFO, "  SACM INFO Boot Guard Capability check for CPU during BSP only Test\n"));

  if ((SAcmInfo & B_BOOT_GUARD_SACM_INFO_CAPABILITY) == 0) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Boot Guard Capability Disabled\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_CODE_2,
      HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_STRING_2
      );
    Result = FALSE;
  }

  if ((SAcmInfo & B_BOOT_GUARD_SACM_INFO_CAPABILITY) != 0) {
    DEBUG ((DEBUG_INFO, "  SACM INFO Boot Guard Capability Configured with Verified Boot check for CPU during BSP only Test\n"));

    if ((SAcmInfo & B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT) == 0 ||
      (SAcmInfo & B_BOOT_GUARD_SACM_INFO_NEM_ENABLED) == 0) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: Boot Guard Capability Configured With Verified Boot Disabled\n"));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_CODE_1,
        HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY_UNEXP_STATUS_STRING_1
        );
      Result = FALSE;
    }
  }

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               0,
               HSTI_BYTE0_HARDWARE_ROOTED_BOOT_INTEGRITY
               );
    CheckStatusForHstiLibSet (Status);
  }

  return;
}
