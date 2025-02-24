/** @file
  This file contains headers to provide FSP measurement operations.

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

#include <Library/FspFbmSupportLib.h>
#include <IndustryStandard/UefiTcgPlatform.h>

#define PLATFORM_FIRMWARE_BLOB_DESC  "Fv(XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX)"
#define MSR_ANC_BOOT_POLICY_0        (0x00000130)
#define TPM_INIT_SUCCESS             0
#define TPM_INIT_SKIPPED             1
#define TPM_INIT_FAILED              2

#define B_BOOT_GUARD_SACM_INFO_FORCE_ANCHOR_BOOT    BIT4
#define B_BOOT_GUARD_SACM_INFO_MEASURED_BOOT        BIT5
#define B_BOOT_GUARD_SACM_INFO_VERIFIED_BOOT        BIT6
#define B_FBM_VALID_STATUS                          BIT27
#define B_SCRTM_STATUS                              BIT32|BIT33|BIT34
#define R_CPU_ACM_POLICY_STATUS                     0x378
#define TXT_PUBLIC_BASE                             0xFED30000
#define MMIO_ACM_POLICY_STATUS                      (TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS)
#define FSP_MEASUREMENT_INFO_DEFAULT                0XFFFFFFFF
#define SCRTM_VERSION_UTF16_LENGTH                  13

#define  BIT_HASHED_IBB                             0x00000001    //0: Hashed IBB    1:Non-Hashed IBB

#pragma pack (1)

typedef union {
  struct {
    //
    // 0: TPM is initialized by FSP successfully.
    // 1: FSP skipped TPM initialization.
    // 2: FSP failed to initialize TPM.
    // 3: Reserved.
    //
    UINT32   TpmInitStatus      :   2;  // Bits 1:0
    UINT32   FspVersionStatus   :   1;  // Bit 2; 0 - Success, 1 - Failure
    UINT32   IbbStatus          :   1;  // Bit 3; 0 - Success, 1 - Failure
    UINT32   BspPreMemStatus    :   1;  // Bit 4; 0 - Success, 1 - Failure
    UINT32   FspmStatus         :   1;  // Bit 5; 0 - Success, 1 - Failure
    //
    // 0:   Initialization fail in S3 Resume when invoke TPM2_STARTUP with TPM_SU_STATE.
    // 1:   FSP SCRTM measurement fail.
    // Values 2 and 3 are reserved.
    //
    UINT32   ErrorStatus        :   2;  // Bits 7:6
    UINT32   Reserved           :   25; // Bits 31:8

  } Bits;
  UINT32 Data;
} FSP_BUILD_MEASUREMENT_INFO;

typedef struct {
  UINT8                   BlobDescriptionSize;
  UINT8                   BlobDescription[sizeof (PLATFORM_FIRMWARE_BLOB_DESC)];
  EFI_PHYSICAL_ADDRESS    BlobBase;
  UINT64                  BlobLength;
} PLATFORM_FIRMWARE_BLOB2_STRUCT;

#pragma pack ()

/**
  Check if the boot mode is S3.

  @retval TRUE   Boot mode is S3.
  @retval FALSE  Boot mode is not S3.

**/
BOOLEAN
IsS3Resume (
  VOID
  );

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
  );

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
);

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
);

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
  );

/**
  Extend FSP version to active PCR.

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
  );

/**
  Create HOB to save the event log data. The HOBs will be consumed
  by PeiBootGuardEventLogLib to create event logs.

  @param[in]     TpmDigestValues        Contains digest list.
  @param[in]     TpmActivePcrBanks      Active PCR value
  @param[in]     NewEventData           Pointer to the new event data.
  @param[in]     EventSize              Size of the new event data.
  @param[in]     EventType              Type of the new event

  @retval EFI_SUCCESS           The new event log entry was added.
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
  );

/**
  Initializes TPM for S4/5 boot mode and handles S3 resume condition

  @param[out]  FspMeasurementInfo  Structure that points to the data

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Others            An error occurred, likely in communication with the TPM.

**/
EFI_STATUS
EFIAPI
FspInitializeTpm (
  OUT  FSP_BUILD_MEASUREMENT_INFO  *FspMeasurementInfo
  );

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
  );


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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );