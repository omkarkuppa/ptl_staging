/** @file
  This file implements FSP loader PPI. FSP loader PPI defines APIs for
  FSP-M/S verification. BSP can consume these APIs to verify FSP-M/S
  once they are loaded.

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

@par Specification Reference:
**/

#include <PiPei.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/FspVerificationLib.h>
#include <Library/FspFbmSupportLib.h>

#include <Ppi/FspLoaderPpi.h>

//
// FspLoaderPeim is designed to be the only one who should links Crypto services
// in FSP-O, FspSecCoreO should reuse API pointers in mFspVerifyApiWrapper structure
// which has signature FSP_VERIFY_API_WRAPPER_STRUCTURE_ID by post FSP build patch.
// These APIs should be implemented as BASE type and shared between SEC and PEI use.
//
FSP_VERIFY_API_WRAPPER  mFspVerifyApiWrapper = {
  FSP_VERIFY_API_WRAPPER_STRUCTURE_ID,
  VerifyFspm,
  VerifyFsps,
  VerifyBsp
};

/**
  A service provided by FSP to verify FSP-M and published for BSP use in Dispatch mode.
  In API mode, it's not supported.

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_UNSUPPORTED         FBM is not found or valid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
FspLoaderVerifyFspm (
  VOID
  )
{
  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm;
  BSPM_ELEMENT                   *Bsss;
  UINT8                          AvailableMemoryBuffer[HASH_CTX_LEN_MAX];

  Fbm = LocateFbm ();
  if (Fbm == NULL) {
    return EFI_UNSUPPORTED;
  }

  Bsss = LocateBspm ();
  if (Bsss == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mFspVerifyApiWrapper.VerifyFspmApiWrapper (Bsss, Fbm, AvailableMemoryBuffer) == 0) {
    return EFI_SUCCESS;
  }

  return EFI_ACCESS_DENIED;
}

/**
  A service provided by FSP to verify FSP-S and published for BSP use in Dispatch mode.
  In API mode, it's not supported.

  @param[in] FspsImageBase        FSP-S image base.

  @retval EFI_UNSUPPORTED         FBM is not found or valid.
  @retval EFI_ACCESS_DENIED       Verification Fail.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
FspLoaderVerifyFsps (
  IN UINTN                        FspsImageBase
  )
{
  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm;
  UINT8                          AvailableMemoryBuffer[HASH_CTX_LEN_MAX];

  Fbm  = LocateFbm ();
  if (Fbm == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (mFspVerifyApiWrapper.VerifyFspsApiWrapper (FspsImageBase, Fbm, AvailableMemoryBuffer) == 0) {
    return EFI_SUCCESS;
  }

  return EFI_ACCESS_DENIED;
}

FSP_LOADER_PPI  mFspLoaderPpi = {
  FspLoaderVerifyFspm,
  FspLoaderVerifyFsps
};

EFI_PEI_PPI_DESCRIPTOR  mPeiFspLoaderPpiDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gFspLoaderPpiGuid,
  &mFspLoaderPpi
};

/**
  Main entry

  @param[in] FileHandle        Handle of the file being invoked.
  @param[in] PeiServices       Pointer to PEI Services table.

  @retval EFI_SUCCESS          Install function successfully.
  @retval EFI_OUT_OF_RESOURCES Insufficient resources to create buffer.

**/
EFI_STATUS
EFIAPI
FspLoaderPeimEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS    Status;

  Status = PeiServicesInstallPpi (&mPeiFspLoaderPpiDesc);
  return Status;
}