/** @file
  This file contains the tests for the SecureDebugInterfaceConfiguration Bit

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

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

/**
  Run tests for SecureDebugInterfaceConfiguration bit
**/
VOID
CheckSecureDebugInterfaceConfiguration (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT32          RegEcx;
  MSR_IA32_DEBUG_INTERFACE_REGISTER  DebugInterfaceReg;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &RegEcx, NULL);

  if ((RegEcx & BIT11) != 0) {

    DebugInterfaceReg.Uint64 =  AsmReadMsr64 (MSR_IA32_DEBUG_INTERFACE);

    DEBUG ((DEBUG_INFO, "      Debug Interface Disabled\n"));

    if (DebugInterfaceReg.Bits.Enable) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: Debug Interface Enabled\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_CODE_1,
        HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_STRING_1
        );
      Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      Debug Interface Locked Test\n"));

    if (!(DebugInterfaceReg.Bits.Lock)) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: Debug interface is not locked\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_CODE_2,
        HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_UNEXP_STATUS_STRING_2
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
               HSTI_BYTE0_DEBUG_INTERFACE_SECURITY_CONFIGURATION
               );
   CheckStatusForHstiLibSet (Status);
  }

  return;
}
