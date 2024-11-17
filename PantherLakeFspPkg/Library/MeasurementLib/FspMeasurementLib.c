/** @file
  This file contains functions to provide FSP measurement operations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Library/FspMeasurementLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/DebugLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Library/PrintLib.h>
#include <Library/HobLib.h>
#include <Library/FspVerificationLib.h>

/**
  Check if the boot mode is S3.

  @retval TRUE   Boot mode is S3.
  @retval FALSE  Boot mode is not S3.

**/
UINT8
IsS3Resume (
  VOID
  )
{
  //ToDO: Find logic to detect S3 resume
  return FALSE;
}

/**
  Get the FvName from the FV header.

  Causion: The FV is untrusted input.

  @param[in]  FvBase            Base address of FV image.
  @param[in]  FvLength          Length of FV image.

  @return FvName pointer
  @retval NULL   FvName is NOT found
**/
VOID *
EFIAPI
TpmMeasurementGetFvName (
  IN EFI_PHYSICAL_ADDRESS  FvBase,
  IN UINT64                FvLength
  )
{
  EFI_FIRMWARE_VOLUME_HEADER      *FvHeader;
  EFI_FIRMWARE_VOLUME_EXT_HEADER  *FvExtHeader;

  if (FvBase >= MAX_ADDRESS) {
    return NULL;
  }

  if (FvLength >= MAX_ADDRESS - FvBase) {
    return NULL;
  }

  if (FvLength < sizeof (EFI_FIRMWARE_VOLUME_HEADER)) {
    return NULL;
  }

  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)FvBase;
  if (FvHeader->Signature != EFI_FVH_SIGNATURE) {
    return NULL;
  }

  if (FvHeader->ExtHeaderOffset < sizeof (EFI_FIRMWARE_VOLUME_HEADER)) {
    return NULL;
  }

  if (FvHeader->ExtHeaderOffset + sizeof (EFI_FIRMWARE_VOLUME_EXT_HEADER) > FvLength) {
    return NULL;
  }

  FvExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)(UINTN)(FvBase + FvHeader->ExtHeaderOffset);

  return &FvExtHeader->FvName;
}

/**
  Add a new entry to the Event Log.

  @param[in]     TpmDigestValues     Contains digest list.
  @param[in]     FirmwareBlobBase    Base address of this FirmwareBlob.
  @param[in]     FirmwareBlobLength  Size in bytes of this FirmwareBlob.
  @param[in]     TpmActivePcrBanks   Active PCR value
  @param[in]     Description         Description for this FirmwareBlob.

  @retval EFI_SUCCESS           The new event log entry was added.
  @retval EFI_NOT_FOUND         FV information was not found
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
**/
EFI_STATUS
EFIAPI
LogHashEvent (
  IN TPML_DIGEST_VALUES     *TpmDigestValues,
  IN EFI_PHYSICAL_ADDRESS   FirmwareBlobBase,
  IN UINT64                 FirmwareBlobLength,
  IN UINT32                 TpmActivePcrBanks,
  IN CHAR8                  *Description OPTIONAL
  )
{
  VOID                            *HobData;
  VOID                            *DigestBuffer;
  TCG_PCR_EVENT_HDR               *TcgEventHdr;
  VOID                            *FvName;
  VOID                            *EventLog;
  UINT32                          EventSize;
  PLATFORM_FIRMWARE_BLOB2_STRUCT  FvBlob2;

  FvName = TpmMeasurementGetFvName (FirmwareBlobBase, FirmwareBlobLength);

  if ((Description != NULL) || (FvName != NULL)) {
    if (Description != NULL) {
      AsciiSPrint ((CHAR8 *)FvBlob2.BlobDescription, sizeof (FvBlob2.BlobDescription), "%a", Description);
    } else {
      AsciiSPrint ((CHAR8 *)FvBlob2.BlobDescription, sizeof (FvBlob2.BlobDescription), "Fv(%g)", FvName);
    }
    FvBlob2.BlobDescriptionSize = sizeof (FvBlob2.BlobDescription);
    FvBlob2.BlobBase            = FirmwareBlobBase;
    FvBlob2.BlobLength          = FirmwareBlobLength;

    EventLog  = &FvBlob2;
    EventSize = sizeof (FvBlob2);
  } else {
    // FV information was not found
    return EFI_NOT_FOUND;
  }
  HobData = BuildGuidHob (
              &gTcgEvent2EntryHobGuid,
              sizeof (TcgEventHdr->PCRIndex) + sizeof (TcgEventHdr->EventType)
              + GetDigestListSize (TpmDigestValues) + sizeof (EventSize) + EventSize
              );
  if (HobData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  TcgEventHdr = HobData;
  TcgEventHdr->PCRIndex = 0;
  TcgEventHdr->EventType = EV_EFI_PLATFORM_FIRMWARE_BLOB;
  DigestBuffer            = (UINT8 *)&TcgEventHdr->Digest;
  DigestBuffer            = CopyDigestListToBuffer (DigestBuffer, TpmDigestValues, TpmActivePcrBanks);

  CopyMem (DigestBuffer, &EventSize, sizeof (EventSize));
  DigestBuffer = (UINT8 *)DigestBuffer + sizeof (EventSize);
  CopyMem (DigestBuffer, EventLog, EventSize);

  return EFI_SUCCESS;
}

/**
  Initializes TPM for S4/5 boot mode and handles S3 resume condition

  @param[out]   FspMeasurementInfo  Structure that points to the data

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Others            An error occurred, likely in communication with the TPM.

**/
EFI_STATUS
EFIAPI
FspInitializeTpm (
  OUT  FSP_BUILD_MEASUREMENT_INFO  FspMeasurementInfo
  )
{
  EFI_STATUS    Status;

  Status = Tpm2RequestUseTpm ();
  DEBUG ((DEBUG_INFO, "Tpm2RequestUseTpm: %r\n", Status));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  FspMeasurementInfo.Bits.TpmInitStatus = TPM_INIT_SKIPPED;

  if (!(AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS)) {
    if (IsS3Resume () == 1) {
      Status = Tpm2Startup (TPM_SU_STATE);
      if (EFI_ERROR (Status)) {
        Status = Tpm2Startup (TPM_SU_CLEAR);
      }
    } else {
      Status = Tpm2Startup (TPM_SU_CLEAR);
      if (Status == EFI_SUCCESS) {
        FspMeasurementInfo.Bits.TpmInitStatus = TPM_INIT_SUCCESS;
      } else {
        FspMeasurementInfo.Bits.TpmInitStatus = TPM_INIT_FAILED;
      }
    }
    DEBUG ((DEBUG_INFO, "Tpm2Startup: %r\n", Status));
  }
  return Status;
}

/**
  This function will query the TPM to determine which hashing algorithms
  are supported and which PCR banks are currently active.

  @param[out]  TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Others            An error occurred, likely in communication with the TPM.

**/
EFI_STATUS
EFIAPI
FspGetSupportedPcrs (
  OUT UINT32          *TpmActivePcrBanks
  )
{
  EFI_STATUS    Status;
  UINT32        TpmHashAlgorithmBitmap;

  //
  // Determine the current TPM support and the Platform PCR mask.
  //
  Status = Tpm2GetCapabilitySupportedAndActivePcrs (&TpmHashAlgorithmBitmap, TpmActivePcrBanks);
  DEBUG ((DEBUG_INFO, "Tpm2GetCapabilitySupportedAndActivePcrs: %r  (TpmActivePcrBanks = %d)\n", Status, *TpmActivePcrBanks));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Extend FSP digests to active PCR.

  @param[in]   Fbm                 Base address of Fbm
  @param[out]  TpmDigestValues     List of digests
  @param[in]   FspComponentId      FSP component ID
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval TpmDigestValues          List of digests

**/
EFI_STATUS
EFIAPI
FspRegionGetDigestList (
  IN   FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  OUT  TPML_DIGEST_VALUES             *TpmDigestValues,
  IN   UINT8                          FspComponentId,
  IN   UINT32                         TpmActivePcrBanks
  )
{
  FSP_REGION_DIGEST       *FspDigest;

  FspDigest  = COMPONENT_DIGEST0_PTR (Fbm);
  if (FspComponentId == FSP_REGION_TYPE_FSPS) {
    FspDigest += FSP_REGION_TYPE_FSPS;
  } else if (FspComponentId == FSP_REGION_TYPE_FSPM) {
    FspDigest += FSP_REGION_TYPE_FSPM;
  } else if (FspComponentId != FSP_REGION_TYPE_FSPOT) {
    return EFI_INVALID_PARAMETER;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SHA1) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA1;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &FspDigest->ComponentDigests.Sha1Digest.HashBuffer,
      FspDigest->ComponentDigests.Sha1Digest.Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SHA256) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA256;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &FspDigest->ComponentDigests.Sha256Digest.HashBuffer,
      FspDigest->ComponentDigests.Sha256Digest.Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SHA384) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA384;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &FspDigest->ComponentDigests.Sha384Digest.HashBuffer,
      FspDigest->ComponentDigests.Sha384Digest.Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SHA512) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA512;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &FspDigest->ComponentDigests.Sha512Digest.HashBuffer,
      FspDigest->ComponentDigests.Sha512Digest.Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SM3_256) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SM3_256;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &FspDigest->ComponentDigests.ShaSm3Digest.HashBuffer,
      FspDigest->ComponentDigests.ShaSm3Digest.Size
      );
    TpmDigestValues->count ++;
  }
  return EFI_SUCCESS;
}

/**
  Extend BSP Pre-Mem digests to active PCR.

  @param[in]   Bspm                Base address of Bspm
  @param[out]  TpmDigestValues     List of digests
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval TpmDigestValues          List of digests

**/
VOID
BspRegionGetDigestList (
  IN   BSPM_ELEMENT                   *Bspm,
  OUT  TPML_DIGEST_VALUES             *TpmDigestValues,
  IN   UINT32                         TpmActivePcrBanks
  )
{
  SHAX_HASH_STRUCTURE     *DigestPtr;

  DigestPtr = (SHAX_HASH_STRUCTURE *) ((UINT8 *) Bspm + sizeof (BSPM_ELEMENT) + Bspm->BspSegmentCount * sizeof (REGION_SEGMENT));

  if ((TpmActivePcrBanks & HASH_ALG_SHA1) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA1;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &DigestPtr->HashBuffer,
      DigestPtr->Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SHA256) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA256;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &DigestPtr->HashBuffer,
      DigestPtr->Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SHA384) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA384;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &DigestPtr->HashBuffer,
      DigestPtr->Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SHA512) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SHA512;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &DigestPtr->HashBuffer,
      DigestPtr->Size
      );
    TpmDigestValues->count ++;
  }

  if ((TpmActivePcrBanks & HASH_ALG_SM3_256) != 0) {
    TpmDigestValues->digests[TpmDigestValues->count].hashAlg = TPM_ALG_SM3_256;
    CopyMem (
      &TpmDigestValues->digests[TpmDigestValues->count].digest,
      &DigestPtr->HashBuffer,
      DigestPtr->Size
      );
    TpmDigestValues->count ++;
  }
}

/**
  Extend FSP-O/T digests to active PCR.

  @param[in]   Fbm                 Base address of Fbm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
FspExtendFspot (
  IN  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN  UINT32                         TpmActivePcrBanks
  )
{
  TPML_DIGEST_VALUES  TpmDigestValues;
  EFI_STATUS          Status;

  ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
  FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPOT, TpmActivePcrBanks);
  Status = Tpm2PcrExtend (0, &TpmDigestValues);
  DEBUG ((DEBUG_INFO, "Tpm2PcrExtend: %r (%d FSP-OT digests have been extended!)\n", Status, TpmDigestValues.count));

  return Status;
}

/**
  Extend FSP-M digests to active PCR.

  @param[in]   Fbm                 Base address of Fbm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
FspExtendFspm (
  IN  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN  UINT32                         TpmActivePcrBanks
  )
{
  TPML_DIGEST_VALUES  TpmDigestValues;
  EFI_STATUS          Status;

  ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
  FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPM, TpmActivePcrBanks);
  Status = Tpm2PcrExtend (0, &TpmDigestValues);
  DEBUG ((DEBUG_INFO, "Tpm2PcrExtend: %r (%d FSP-M digests have been extended!)\n", Status, TpmDigestValues.count));

  return Status;
}

/**
  Extend FSP-S digests to active PCR.

  @param[in]   Fbm                 Base address of Fbm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
FspExtendFsps (
  IN  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN  UINT32                         TpmActivePcrBanks
  )
{
  TPML_DIGEST_VALUES  TpmDigestValues;
  EFI_STATUS          Status;

  ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
  FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPS, TpmActivePcrBanks);
  Status = Tpm2PcrExtend (0, &TpmDigestValues);
  DEBUG ((DEBUG_INFO, "Tpm2PcrExtend: %r (%d FSP-S digests have been extended!)\n", Status, TpmDigestValues.count));

  return Status;
}

/**
  Extend FSP-O/T digests to active PCR.

  @param[out]  FspMeasurementInfo  Structure that points to the data
  @param[in]   Fbm                 Base address of Fbm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
ExtendFspotRegion (
  OUT  FSP_BUILD_MEASUREMENT_INFO   FspMeasurementInfo,
  IN   FSP_BOOT_MANIFEST_STRUCTURE  *Fbm,
  IN   UINT32                       TpmActivePcrBanks
  )
{
  EFI_STATUS  Status = EFI_ACCESS_DENIED;
  //if ((DetectBootGuardProfile () == BOOT_GUARD_PROFILE_4) && (IsS3Resume () == 0)) {
    Status = FspExtendFspot (Fbm, TpmActivePcrBanks);
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "FSP-OT extended to PCR Bank %d\n", TpmActivePcrBanks));
    } else {
      FspMeasurementInfo.Bits.IbbStatus = 1;
      DEBUG ((DEBUG_INFO, "FSP-OT measurement fail\n"));
    }
  //}
  return Status;
}

/**
  Extend FSP-M digests to active PCR.

  @param[out]  FspMeasurementInfo  Structure that points to the data
  @param[in]   Fbm                 Base address of Fbm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
ExtendFspmRegion (
  OUT  FSP_BUILD_MEASUREMENT_INFO   FspMeasurementInfo,
  IN   FSP_BOOT_MANIFEST_STRUCTURE  *Fbm,
  IN   UINT32                       TpmActivePcrBanks
)
{
  EFI_STATUS  Status = EFI_ACCESS_DENIED;
  if (IsS3Resume () == 0) {
    Status = FspExtendFspm (Fbm, TpmActivePcrBanks);
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "FSP-M extended to PCR Bank %d\n", TpmActivePcrBanks));
    } else {
      FspMeasurementInfo.Bits.FspmStatus = 1;
      DEBUG ((DEBUG_ERROR, "Failed to extend FSP-M! Status: %r\n", Status));
    }
  }
  return Status;
}

/**
  Extend BSP Pre-Mem digests to active PCR.

  @param[out]  FspMeasurementInfo  Structure that points to the data
  @param[in]   Bspm                Base address of Bspm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
ExtendBspRegion (
  OUT  FSP_BUILD_MEASUREMENT_INFO   FspMeasurementInfo,
  IN   BSPM_ELEMENT                 *Bspm,
  IN   UINT32                       TpmActivePcrBanks
)
{
  EFI_STATUS          Status = EFI_ACCESS_DENIED;
  TPML_DIGEST_VALUES  TpmDigestValues;

  if (IsS3Resume () == 0) {
    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    BspRegionGetDigestList (Bspm, &TpmDigestValues, TpmActivePcrBanks);
    Status = Tpm2PcrExtend (0, &TpmDigestValues);
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Tpm2PcrExtend: %r (%d BSP Pre-Mem digests have been extended!)\n", Status, TpmDigestValues.count));
    } else {
      FspMeasurementInfo.Bits.BspPreMemStatus = 1;
      DEBUG ((DEBUG_ERROR, "Failed to extend BSP Pre-Mem! Status: %r\n", Status));
    }
  }
  return Status;
}

/**
  Initialize TPM and get the supported PCRs.

  @param[out]  FspMeasurementInfo  Structure that points to the data
  @param[in]   Bspm                Base address of Bspm
  @param[out]  TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS             Verification Pass.
  @retval EFI_NOT_FOUND           TPM2 not found.
  @retval EFI_DEVICE_ERROR        Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
InitializeTpmAndGetActivePcrs (
  OUT  FSP_BUILD_MEASUREMENT_INFO  FspMeasurementInfo,
  IN   BSPM_ELEMENT                *Bspm,
  OUT  UINT32                      *TpmActivePcrBanks
  )
{
  EFI_STATUS  Status;

  Status = FspInitializeTpm (FspMeasurementInfo);

  if (FspMeasurementInfo.Bits.TpmInitStatus != TPM_INIT_FAILED) {
    Status = FspGetSupportedPcrs (TpmActivePcrBanks);
  }
  return Status;
}

/**
  Capture TPM initialization status and measurement status in FSP
  reserved region.

  @param[in]  FspMeasurementInfo  Structure that points to the data

  @retval EFI_SUCCESS             Verification Pass.

**/
EFI_STATUS
EFIAPI
FspBuildMeasurementInfo (
  IN  FSP_BUILD_MEASUREMENT_INFO  FspMeasurementInfo
  )
{
  VOID  *FspMeasurementData;
  FspMeasurementData = (VOID *) (UINTN) (PcdGet32 (PcdTemporaryRamBase) + PcdGet32 (PcdTemporaryRamSize) - PcdGet32 (PcdFspReservedBufferSize));

  ZeroMem (FspMeasurementData, sizeof (FSP_BUILD_MEASUREMENT_INFO));
  CopyMem (FspMeasurementData, &FspMeasurementInfo, sizeof (FSP_BUILD_MEASUREMENT_INFO));

  return EFI_SUCCESS;
}