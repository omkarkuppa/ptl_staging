/** @file
  Provide Platform specific PEI implementation for FSP verification.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiPlatformFspsVerificationLib.h>
#include <Library/BootGuardLib.h>
#include <Ppi/FspLoaderPpi.h>

/**
  Platform specific method to verify FSP-S, which should be executed after FSP-S is
  extracted onto permarnant mermory.
  This instance consumes FspLoaderPpi to verify FSP-S.

  @param[in]   FspsImageBase      FSP-S image base in memory.

  @retval EFI_SUCCESS             Verification Pass or verification can be skipped.
  @retval EFI_ACCESS_DENIED       Verification Fail.

**/
EFI_STATUS
EFIAPI
PlatformVerifyFsps (
  IN UINTN        FspsImageBase
  )
{
  EFI_STATUS        Status;
  FSP_LOADER_PPI    *FspLoaderPpi;

  //
  // Locate FSP Loader PPI
  //
  Status = PeiServicesLocatePpi (
             &gFspLoaderPpiGuid,
             0,
             NULL,
             (VOID **)&FspLoaderPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gFspLoaderPpiGuid is not found!\n"));
    return EFI_ACCESS_DENIED;
  }

  ASSERT (FspLoaderPpi != NULL);
  Status = FspLoaderPpi->FspVerifyFsps (FspsImageBase);

  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "FSP-S verification pass...\n"));
  } else if (EFI_ERROR (Status)) {
    if (Status == EFI_UNSUPPORTED) {
      //
      // No valid FBM is found for below cases, both should skip FSP-S verification:
      // 1. Non FSP Signing boot flow
      // 2. FBM is verified as invalid, but system is under profile 0
      //
      // Verified boot but FBM invalid should halt directly in ACM!!!
      //
      DEBUG ((DEBUG_INFO, "FSP signing is not supported, skip FSP-S verification!\n"));
      return EFI_SUCCESS;
    } else {
      DEBUG ((DEBUG_ERROR, "FSP-S verification failed with status: %r\n", Status));
    }
  }
  return EFI_SUCCESS;
}
