/** @file
  This file contains the tests for the SignedFirmwareUpdate bit

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

/**
  Run tests for SignedFirmwareUpdate bit
**/
VOID
CheckSignedFirmwareUpdate (
  VOID
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           Result;
  UINTN                             CpuNumber;
  UINTN                             CpuIndex;
  MSR_PLATFORM_INFO_REGISTER        PlatformInfo;
  MSR_PLAT_FRMW_PROT_CTRL_REGISTER  PlatFrmwProtCtrl;
  UINT64                            CodeReportedBitmap  = 0;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE) == 0) {
    return;
  }

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "    BIOS Guard Security per CPU Number Tests\n"));

  CpuNumber = GetCpuNumber ();
  for (CpuIndex = 0; CpuIndex < CpuNumber; CpuIndex++) {
    DEBUG ((DEBUG_INFO, "    [CPU - 0x%x]\n", CpuIndex));

    DEBUG ((DEBUG_INFO, "      BIOS Guard support Test\n"));

    PlatformInfo.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_PLATFORM_INFO);
    if (PlatformInfo.Bits.PfatEnable == 0) {
      DEBUG ((DEBUG_INFO, "        Unexpected Status: BIOS Guard not supported\n"));

      BuildAndAppendHstiUniqueStatusString (
         HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_CODE_1,
         HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_STRING_1,
         &CodeReportedBitmap,
         BIT1
         );
      Result = FALSE;
    } else {
      DEBUG ((DEBUG_INFO, "      BIOS Guard Lock Test\n"));

      PlatFrmwProtCtrl.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_PLAT_FRMW_PROT_CTRL);
      if (PlatFrmwProtCtrl.Bits.PfatLock == 0) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: BIOS Guard lock not set\n"));


        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_CODE_2,
          HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_STRING_2,
          &CodeReportedBitmap,
          BIT2
          );
        Result = FALSE;
      }

      DEBUG ((DEBUG_INFO, "      BIOS Guard Enable Test\n"));

      if (PlatFrmwProtCtrl.Bits.PfatEnable == 0) {
        DEBUG ((DEBUG_INFO, "        Unexpected Status: BIOS Guard not enabled\n"));


        BuildAndAppendHstiUniqueStatusString (
          HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_CODE_3,
          HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE_UNEXP_STATUS_STRING_3,
          &CodeReportedBitmap,
          BIT3
          );
        Result = FALSE;
      }
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
               HSTI_BYTE0_SIGNED_FIRMWARE_UPDATE
               );
    CheckStatusForHstiLibSet (Status);
  }

  return;
}
