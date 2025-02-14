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
#include <Library/IoLib.h>
#include <Register/PmcRegs.h>

/**
  Check if the boot mode is S3.

  @retval TRUE   Boot mode is S3.
  @retval FALSE  Boot mode is not S3.

**/
BOOLEAN
IsS3Resume (
  VOID
  )
{
  UINT16  AcpiBase;
  UINT16  PmconA;

  AcpiBase = FixedPcdGet16 (PcdAcpiBaseAddress);
  PmconA   = IoRead16 (AcpiBase + R_ACPI_IO_PM1_CNT);
  if ((PmconA & B_ACPI_IO_PM1_CNT_SLP_TYP) == V_ACPI_IO_PM1_CNT_S3) {
    return TRUE;
  } else {
    return FALSE;
  }
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
  Create HOB to save the event log data. The HOBs will be consumed
  by PeiBootGuardEventLogLib to create event logs.

  @param[in]     TpmDigestValues        Contains digest list.
  @param[in]     TpmActivePcrBanks      Active PCR value
  @param[in]     NewEventData           Pointer to the new event data.
  @param[in]     EventSize              Size of the new event data.
  @param[in]     EventType              Type of the new event

  @retval EFI_SUCCESS           The new event log entry was added.
  @retval EFI_NOT_FOUND         FV information was not found
  @retval EFI_OUT_OF_RESOURCES  No enough memory to log the new event.
**/
EFI_STATUS
EFIAPI
SaveHashEvent (
  IN TPML_DIGEST_VALUES     *TpmDigestValues,
  IN UINT32                 TpmActivePcrBanks,
  IN UINT8                  *NewEventData,
  IN UINT32                 EventSize,
  IN UINT32                 EventType
  )
{
  VOID                            *HobData;
  VOID                            *DigestBuffer;
  TCG_PCR_EVENT_HDR               *TcgEventHdr;

  HobData = BuildGuidHob (
              &gTcgEventDataHobGuid,
              sizeof (TcgEventHdr->PCRIndex) + sizeof (TcgEventHdr->EventType)
              + GetDigestListSize (TpmDigestValues) + sizeof (TcgEventHdr->EventSize) +
              EventSize
              );
  if (HobData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  TcgEventHdr = HobData;
  TcgEventHdr->PCRIndex = 0;
  TcgEventHdr->EventType = EventType;
  DigestBuffer            = (UINT8 *)&TcgEventHdr->Digest;
  DigestBuffer            = CopyDigestListToBuffer (DigestBuffer, TpmDigestValues, TpmActivePcrBanks);

  CopyMem (DigestBuffer, &EventSize, sizeof (EventSize));
  DigestBuffer = (UINT8 *)DigestBuffer + sizeof (EventSize);
  CopyMem (DigestBuffer, NewEventData, EventSize);
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
  OUT  FSP_BUILD_MEASUREMENT_INFO  *FspMeasurementInfo
  )
{
  EFI_STATUS    Status;

  Status = Tpm2RequestUseTpm ();
  DEBUG ((DEBUG_INFO, "Tpm2RequestUseTpm: %r\n", Status));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  FspMeasurementInfo->Bits.TpmInitStatus = TPM_INIT_SKIPPED;

  if (!(AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO) & B_BOOT_GUARD_SACM_INFO_TPM_SUCCESS)) {
    if (IsS3Resume () == 1) {
      Status = Tpm2Startup (TPM_SU_STATE);
      if (EFI_ERROR (Status)) {
        Status = Tpm2Startup (TPM_SU_CLEAR);
      }
    } else {
      Status = Tpm2Startup (TPM_SU_CLEAR);
      if (Status == EFI_SUCCESS) {
        FspMeasurementInfo->Bits.TpmInitStatus = TPM_INIT_SUCCESS;
      } else {
        FspMeasurementInfo->Bits.TpmInitStatus = TPM_INIT_FAILED;
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
  if (FspComponentId == FSP_REGION_TYPE_FSP_VERSION) {
    FspDigest  = VERSION_DIGEST_PTR (Fbm);
    if (FspDigest->ComponentID != FSP_REGION_TYPE_FSP_VERSION) {
      return EFI_INVALID_PARAMETER;
    }
  } else if (FspComponentId == FSP_REGION_TYPE_FSPS) {
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
  Extend FSP version to active PCR.

  @param[in]   Fbm                 Base address of Fbm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
FspExtendFspVersion (
  IN  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm,
  IN  UINT32                         TpmActivePcrBanks
  )
{
  TPML_DIGEST_VALUES  TpmDigestValues;
  EFI_STATUS          Status;

  ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
  FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSP_VERSION, TpmActivePcrBanks);
  Status = Tpm2PcrExtend (0, &TpmDigestValues);
  DEBUG ((DEBUG_INFO, "Tpm2PcrExtend: %r (%d FSP Version digests have been extended!)\n", Status, TpmDigestValues.count));
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
  Extend FSP Version to active PCR.

  @param[out]  FspMeasurementInfo  Structure that points to the data
  @param[in]   Fbm                 Base address of Fbm
  @param[in]   TpmActivePcrBanks   Active PCR value

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval EFI_DEVICE_ERROR  Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
ExtendFspVersion (
  OUT  FSP_BUILD_MEASUREMENT_INFO   *FspMeasurementInfo,
  IN   FSP_BOOT_MANIFEST_STRUCTURE  *Fbm,
  IN   UINT32                       TpmActivePcrBanks
  )
{
  EFI_STATUS  Status = EFI_ACCESS_DENIED;
  if (IsS3Resume () == 0) {
    Status = FspExtendFspVersion (Fbm, TpmActivePcrBanks);
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "FSP version extended to PCR Bank\n",
              Status, TpmActivePcrBanks));
    } else {
      DEBUG ((DEBUG_INFO, "FSP version measurement fail\n"));
    }
  }
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
  OUT  FSP_BUILD_MEASUREMENT_INFO   *FspMeasurementInfo,
  IN   FSP_BOOT_MANIFEST_STRUCTURE  *Fbm,
  IN   UINT32                       TpmActivePcrBanks
  )
{
  EFI_STATUS  Status = EFI_ACCESS_DENIED;
  if (IsS3Resume () == 0) {
    Status = FspExtendFspot (Fbm, TpmActivePcrBanks);
    if (Status == EFI_SUCCESS) {
      FspMeasurementInfo->Bits.IbbStatus = EFI_SUCCESS;
      DEBUG ((DEBUG_INFO, "FSP-OT extended to PCR Bank %d\n", TpmActivePcrBanks));
    } else {
      DEBUG ((DEBUG_INFO, "FSP-OT measurement fail\n"));
    }
  }
  return Status;
}

/**
  Save FSP-O/T event log data.

  @param[in]  Fbm        Base address of Fbm

**/
VOID
EFIAPI
SaveFspotEventData (
  IN   FSP_BOOT_MANIFEST_STRUCTURE  *Fbm,
  IN   UINT32                       TpmActivePcrBanks
  )
{
  TPML_DIGEST_VALUES             TpmDigestValues;
  EFI_STATUS                     Status;

  ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
  FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPOT, TpmActivePcrBanks);
  Status = SaveHashEvent (&TpmDigestValues,
                         TpmActivePcrBanks,
                         (UINT8 *) "FSPOT",
                         sizeof ("FSPOT"),
                         EV_POST_CODE
                         );
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "Hash event log saved successfully for FSP-OT\n"));
  }
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
  OUT  FSP_BUILD_MEASUREMENT_INFO   *FspMeasurementInfo,
  IN   FSP_BOOT_MANIFEST_STRUCTURE  *Fbm,
  IN   UINT32                       TpmActivePcrBanks
)
{
  EFI_STATUS  Status = EFI_ACCESS_DENIED;
  if (IsS3Resume () == 0) {
    Status = FspExtendFspm (Fbm, TpmActivePcrBanks);
    if (Status == EFI_SUCCESS) {
      FspMeasurementInfo->Bits.FspmStatus = EFI_SUCCESS;
      DEBUG ((DEBUG_INFO, "FSP-M extended to PCR Bank %d\n", TpmActivePcrBanks));
    } else {
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
  OUT  FSP_BUILD_MEASUREMENT_INFO   *FspMeasurementInfo,
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
      FspMeasurementInfo->Bits.BspPreMemStatus = EFI_SUCCESS;
      DEBUG ((DEBUG_INFO, "Tpm2PcrExtend: %r (%d BSP Pre-Mem digests have been extended!)\n", Status, TpmDigestValues.count));
    } else {
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
  OUT  FSP_BUILD_MEASUREMENT_INFO  *FspMeasurementInfo,
  IN   BSPM_ELEMENT                *Bspm,
  OUT  UINT32                      *TpmActivePcrBanks
  )
{
  EFI_STATUS   Status;
  TPML_DIGEST  EmptyHashList;
  TPML_DIGEST  EarlyHashList;

  Status = FspInitializeTpm (FspMeasurementInfo);

  //
  // Verify if FSP-OT and FSP version is already extended by ACM
  //
  Tpm2PcrReadForActiveBank (00, &EarlyHashList);
  ZeroMem (&EmptyHashList, sizeof (TPML_DIGEST));
  if (CompareMem (EmptyHashList.digests->buffer, EarlyHashList.digests->buffer, sizeof (EarlyHashList.digests->buffer)) != 0) {
    //
    // If PCR[0] digest list is not empty, this would mean that ACM has extended FSP-OT and ACM version
    // into the PCR. FSP version should not be extended as ACM is SCRTM.
    //
    DEBUG ((DEBUG_INFO, "FSP-OT and ACM version has already been measured by ACM\n"));
    FspMeasurementInfo->Bits.IbbStatus = EFI_SUCCESS;
  } else {
    //
    // FSP is SCRTM and FSP version should be extended.
    //
    FspMeasurementInfo->Bits.FspVersionStatus = EFI_SUCCESS;
  }

  if (FspMeasurementInfo->Bits.TpmInitStatus != TPM_INIT_FAILED) {
    Status = FspGetSupportedPcrs (TpmActivePcrBanks);
  }
  return Status;
}

/**
  Create HOB to save the event log data. The HOBs will be consumed
  by PeiBootGuardEventLogLib to create event logs. Event data will
  be saved for FSP Version, FSP-O/T, FSP-M, BSP Pre-Mem and FSP-S.
  FSP-S measurement will also be done.

**/
EFI_STATUS
EFIAPI
VerifiedComponentSaveHashEvent (
  VOID
  )
{
  UINT32                         Index;
  REGION_SEGMENT                 *IbbSegmentPtr;
  FSP_BUILD_MEASUREMENT_INFO     *FspMeasurementData;
  EFI_STATUS                     Status;
  FSP_BOOT_MANIFEST_STRUCTURE    *Fbm;
  BSPM_ELEMENT                   *Bspm;
  UINT32                         TpmActivePcrBanks;
  TPML_DIGEST_VALUES             TpmDigestValues;
  UINT64                         AcmPolicyStatus;

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
    AcmPolicyStatus = MmioRead64 (MMIO_ACM_POLICY_STATUS);
    //
    // ACM measures FSP-OT for BTG0 with TXT enabled. Save event log for this case.
    //
    if ((Fbm != NULL) && (DetectBootGuardProfile () == 0) && (AcmPolicyStatus & B_SCRTM_STATUS)) {
      Status = Tpm2RequestUseTpm ();
      Status = FspGetSupportedPcrs (&TpmActivePcrBanks);
      if (EFI_ERROR (Status)) {
        return EFI_DEVICE_ERROR;
      }
      SaveFspotEventData (Fbm, TpmActivePcrBanks);
    }
    return EFI_UNSUPPORTED;
  }

  Bspm = LocateBspm ();
  if (Bspm == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FspGetSupportedPcrs (&TpmActivePcrBanks);

  FspMeasurementData = (FSP_BUILD_MEASUREMENT_INFO *) (UINTN) (PcdGet32 (PcdTemporaryRamBase) +
                        PcdGet32 (PcdTemporaryRamSize) - PcdGet32 (PcdFspReservedBufferSize));

  if (FspMeasurementData->Bits.TpmInitStatus == TPM_INIT_FAILED) {
    return EFI_DEVICE_ERROR;
  }

  if (FspMeasurementData->Bits.FspVersionStatus == EFI_SUCCESS) {
    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSP_VERSION, TpmActivePcrBanks);
    Status = SaveHashEvent (&TpmDigestValues,
                           TpmActivePcrBanks,
                           (UINT8 *) "FSP Version",
                           sizeof ("FSP Version"),
                           EV_S_CRTM_VERSION
                           );
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Hash event log saved successfully for FSP Version\n"));
    }
  }

  if (FspMeasurementData->Bits.IbbStatus == EFI_SUCCESS) {
    SaveFspotEventData (Fbm, TpmActivePcrBanks);
  }

  if (FspMeasurementData->Bits.FspmStatus == EFI_SUCCESS) {
    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPM, TpmActivePcrBanks);
    Status = SaveHashEvent (&TpmDigestValues,
                           TpmActivePcrBanks,
                           (UINT8 *) "FSPM",
                           sizeof ("FSPM"),
                           EV_POST_CODE
                           );
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Hash event log saved successfully for FSP-M\n"));
    }
  }

  if (FspMeasurementData->Bits.BspPreMemStatus == EFI_SUCCESS) {
    ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
    BspRegionGetDigestList (Bspm, &TpmDigestValues, TpmActivePcrBanks);
    IbbSegmentPtr = SEGMENT_ARRAY_PTR (Bspm);
    for (Index = 0; Index < Bspm->BspSegmentCount; Index ++) {
      if (IbbSegmentPtr[Index].Size != 0 && (IbbSegmentPtr[Index].Flags & BIT_HASHED_IBB) == 0) {
        Status = SaveHashEvent (&TpmDigestValues,
                              TpmActivePcrBanks,
                              (UINT8 *) "BSPM",
                              sizeof ("BSPM"),
                              EV_POST_CODE
                              );
        if (Status == EFI_SUCCESS) {
          DEBUG ((DEBUG_INFO, "Hash event log saved successfully for BSP Pre-Mem\n"));
        }
      }
      IbbSegmentPtr ++;
    }
  }

  ZeroMem (&TpmDigestValues, sizeof (TPML_DIGEST_VALUES));
  FspRegionGetDigestList (Fbm, &TpmDigestValues, FSP_REGION_TYPE_FSPS, TpmActivePcrBanks);
  //
  // Measure and create event log for FSP-S.
  // FSP-S verification will happen later. We have deadloops to halt the boot
  // if verification fails.
  //
  Status = FspExtendFsps (Fbm, TpmActivePcrBanks);
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "FSP-S extended to PCR Bank %d\n", TpmActivePcrBanks));
    Status = SaveHashEvent (&TpmDigestValues,
                           TpmActivePcrBanks,
                           (UINT8 *) "FSPS",
                           sizeof ("FSPS"),
                           EV_POST_CODE
                           );
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Hash event log saved successfully for FSP-S\n"));
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Failed to extend FSP-S! Status: %r\n", Status));
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

  CopyMem (FspMeasurementData, &FspMeasurementInfo, sizeof (FSP_BUILD_MEASUREMENT_INFO));
  return EFI_SUCCESS;
}