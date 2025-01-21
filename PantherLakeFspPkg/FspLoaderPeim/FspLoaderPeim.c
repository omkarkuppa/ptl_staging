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
#include <Library/FspMeasurementLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/FspVerificationLib.h>
#include <Library/FspFbmSupportLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Ppi/FspLoaderPpi.h>

#define  BIT_HASHED_IBB           0x00000001    //0: Hashed IBB    1:Non-Hashed IBB

//
// FspLoaderPeim is designed to be the only one who should links Crypto services
// in FSP-O, FspSecCoreO should reuse API pointers in mFspVerifyApiWrapper structure
// which has signature FSP_VERIFY_API_WRAPPER_STRUCTURE_ID by post FSP build patch.
// These APIs should be implemented as BASE type and shared between SEC and PEI use.
//
FSP_VERIFY_API_WRAPPER  mFspVerifyApiWrapper = {
  FSP_VERIFY_API_WRAPPER_STRUCTURE_ID,
  VerifyAndExtendFspm,
  VerifyAndLogEventFsps,
  VerifyBsp,
  VerifyFspVersion
};

/**
  Create hash event log for FSP-O/T and BSP Pre-Mem

  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_NOT_FOUND           FV information was not found
  @retval EFI_OUT_OF_RESOURCES    No enough memory to log the new event.
  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
VerifiedComponentLogHashEvent (
  VOID
  )
{
  UINT32                         Index;
  REGION_SEGMENT                 *IbbSegmentPtr;
  FSP_BUILD_MEASUREMENT_INFO     *FspMeasurementData;
  FSP_REGION_DIGEST              *FspDigest;
  EFI_STATUS                     Status;
  UINT32                         FspoSize;
  UINT32                         FsptSize;
  UINT32                         TopIbbSize;
  UINT32                         TopIbbBase;
  UINTN                          FspmImageBase;
  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm;
  BSPM_ELEMENT                   *Bspm;
  UINT32                         TpmActivePcrBanks;
  TPML_DIGEST_VALUES             TpmDigestValues;

  if (IsS3Resume () == 1) {
    //
    // Skip event log creation for S3 resume.
    //
    return EFI_UNSUPPORTED;
  }

  Fbm = LocateFbm ();
  //
  // Check if signing is supported
  //
  if (!(IsSigningSupported (Fbm))) {
    return EFI_UNSUPPORTED;
  }

  Bspm = LocateBspm ();
  if (Bspm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FspGetSupportedPcrs (&TpmActivePcrBanks);

  Status = EFI_SUCCESS;
  FspMeasurementData = (FSP_BUILD_MEASUREMENT_INFO *) (UINTN) (PcdGet32 (PcdTemporaryRamBase) +
                        PcdGet32 (PcdTemporaryRamSize) - PcdGet32 (PcdFspReservedBufferSize));

  if (FspMeasurementData->Bits.TpmInitStatus == TPM_INIT_FAILED) {
    return EFI_DEVICE_ERROR;
  }

  FspDigest  = COMPONENT_DIGEST0_PTR (Fbm);
  if (FspDigest->ComponentID != FSP_REGION_TYPE_FSPOT) {
    return EFI_INVALID_PARAMETER;
  }

  if (FspMeasurementData->Bits.IbbStatus == EFI_SUCCESS) {
    FspoSize      = FixedPcdGet32 (PcdFlashFvFspoSize);
    FsptSize      = FixedPcdGet32 (PcdFlashFvFsptSize);
    TopIbbSize    = FspoSize + FsptSize;
    TopIbbBase    = (UINT32) (BASE_4GB - TopIbbSize);

    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPOT, TpmActivePcrBanks);
    Status = LogHashEvent (&TpmDigestValues,
                           TopIbbBase,
                           TopIbbSize,
                           TpmActivePcrBanks,
                           "FSPOT"
                           );
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Hash event log created successfully for FSP-OT\n"));
    }
  }

  if (FspMeasurementData->Bits.FspmStatus == EFI_SUCCESS) {
    if (Bspm->FspmLoadingPolicy & FSPM_COMPRESSED) {
      FspmImageBase = (UINTN) PcdGet32 (PcdSecondaryDataStackBase) + SIZE_4KB;
    } else {
      FspmImageBase = (UINTN) Bspm->FspmBaseAddress;
    }

    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPM, TpmActivePcrBanks);
    Status = LogHashEvent (&TpmDigestValues,
                           FspmImageBase,
                           ((EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)FspmImageBase)->FvLength,
                           TpmActivePcrBanks,
                           "FSPM"
                           );
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Hash event log created successfully for FSP-M\n"));
    }
  }

  if (FspMeasurementData->Bits.BspPreMemStatus == EFI_SUCCESS) {
    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    BspRegionGetDigestList (Bspm, &TpmDigestValues, TpmActivePcrBanks);
    IbbSegmentPtr = SEGMENT_ARRAY_PTR (Bspm);
    for (Index = 0; Index < Bspm->BspSegmentCount; Index ++) {
      if (IbbSegmentPtr[Index].Size != 0 && (IbbSegmentPtr[Index].Flags & BIT_HASHED_IBB) == 0) {
        Status = LogHashEvent (&TpmDigestValues,
                              IbbSegmentPtr->Base,
                              IbbSegmentPtr->Size,
                              TpmActivePcrBanks,
                              "BSPM"
                              );
        if (Status == EFI_SUCCESS) {
          DEBUG ((DEBUG_INFO, "Hash event log created successfully for BSP Pre-Mem\n"));
        }
      }
      IbbSegmentPtr ++;
    }
  }

  return Status;
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
FspLoaderVerifyAndLogEventFsps (
  IN UINTN                        FspsImageBase
  )
{
  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm;
  BSPM_ELEMENT                   *Bspm;
  EFI_STATUS                     Status;
  UINT32                         TpmActivePcrBanks;
  TPML_DIGEST_VALUES             TpmDigestValues;
  UINT8                          AvailableMemoryBuffer[HASH_CTX_LEN_MAX];

  Fbm  = LocateFbm ();
  //
  // Check if signing is supported
  //
  if (!(IsSigningSupported (Fbm))) {
    return EFI_UNSUPPORTED;
  }

  Bspm = LocateBspm ();
  if (Bspm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Veify and create hash event log.
  //
  Status = mFspVerifyApiWrapper.VerifyFspsApiWrapper (FspsImageBase, Fbm, AvailableMemoryBuffer);

  if ((Status == EFI_SUCCESS) && (IsS3Resume () == 0)) {
    FspGetSupportedPcrs (&TpmActivePcrBanks);
    //
    // Extend the digest to PCR if not resume from S3.
    //
    Status = FspExtendFsps (Fbm, TpmActivePcrBanks);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Failed to extend FSP-S region\n"));
      //
      // Proceed with the boot even if measurement failed.
      //
      return EFI_SUCCESS;
    }

    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPS, TpmActivePcrBanks);
    Status = LogHashEvent (&TpmDigestValues,
                           FspsImageBase,
                           ((EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)FspsImageBase)->FvLength,
                           TpmActivePcrBanks,
                           "FSPS"
                           );
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Hash event log created successfully for FSP-S\n"));
    }
    return Status;
  }

  return Status;
}

FSP_LOADER_PPI  mFspLoaderPpi = {
  FspLoaderVerifyAndLogEventFsps
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

  @retval EFI_SUCCESS            Install function successfully.
  @retval EFI_INVALID_PARAMETER  One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer.

**/
EFI_STATUS
EFIAPI
FspLoaderPeimEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  //
  // Install PPI for FSP-S verification and create hsh logs
  // for FSP-OT, BSP Pre-Mem and FSP-M
  //
  PeiServicesInstallPpi (&mPeiFspLoaderPpiDesc);
  VerifiedComponentLogHashEvent ();

  return EFI_SUCCESS;
}