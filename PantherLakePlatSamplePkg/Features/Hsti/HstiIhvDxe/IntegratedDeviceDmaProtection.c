/** @file
  This file contains the tests for the IntegratedDeviceDMAProtection bit

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
#include <Library/PeiHostBridgeIpStatusLib.h>

/**
  Run tests for IntegratedDeviceDMAProtection bit
**/
VOID
CheckIntegratedDeviceDmaProtection (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION) == 0) {
    return;
  }

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "      VTd Enabled Test\n"));
  if (!IsVtdEngineEnabled(IOP_VTD)) {
    DEBUG ((DEBUG_INFO, "       Unexpected Status: VTd Feature is Disabled\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_CODE_1,
      HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_STRING_1
      );
    Result = FALSE;
  } else {

    DEBUG ((DEBUG_INFO, "      VTd 0 Enabled Test\n"));
    if (!IsVtdEngineEnabled(GFX_VTD)) {
      DEBUG ((DEBUG_INFO, "       Unexpected Status: GFX VTd (IOMMU 0) is Disabled\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_CODE_2,
        HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_STRING_2
        );
      Result = FALSE;
    }

    DEBUG ((DEBUG_INFO, "      VTd 1 Enabled Test\n"));
    if (!IsVtdEngineEnabled(NON_GFX_VTD)) {
      DEBUG ((DEBUG_INFO, "       Unexpected Status: non-GFX VTd (IOMMU 1) is Disabled\n"));

      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_CODE_3,
        HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_STRING_3
        );
      Result = FALSE;
    }

    // VTd is enabled in the CPU SKU, check VTd 1 HW engine
    DEBUG ((DEBUG_INFO, "      Vtd Configured Test\n"));
    if (ReadVtdBar() == 0) {
      DEBUG ((DEBUG_INFO, "            Unexpected Status: Vtd enabled but IOC1 IOMMU 0 & 1 are not Configured with total BAR size of 512KB\n"));
      BuildAndAppendHstiStatusString (
        HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_CODE_4,
        HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_UNEXP_STATUS_STRING_4
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
               HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION
               );
    CheckStatusForHstiLibSet (Status);
  }

  return;
}
