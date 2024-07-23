/** @file
  Key Service Pei Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Base.h>

#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiMeLib.h>
#include <Library/HobLib.h>

#include <Ppi/KeyServicePpi.h>

#include <BupMsgs.h>
#include <KeyService.h>

#define SEED_LENGTH_256_BITS    32
#define SEED_LENGTH_512_BITS    64

STATIC KEY_SERVICE_PPI mKeyServicePpi = {
  Generate256BitKey,
  Generate512BitKey
};

STATIC EFI_PEI_PPI_DESCRIPTOR mInstallKeyServicePpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gKeyServicePpiGuid,
  &mKeyServicePpi
};

/**
  Generate a 256 bit key.

  @param[in]   Salt                     Pointer to the salt (non-secret) value.
  @param[in]   SaltSize                 Salt size in bytes.  Must be less than 384-bits.
  @param[out]  NewKey                   Pointer to caller allocated buffer to receive new key.
  @param[in]   NewKeySize               Size of new key bytes to generate.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval OTHER                         The function completed with failure.
**/
EFI_STATUS
EFIAPI
Generate256BitKey (
  IN   UINT8        *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *NewKey,
  IN   UINTN        NewKeySize
  )
{
  EFI_STATUS        Status;
  SEED_PROPERTIES   BiosSeedProperties;
  UINT8             LockSeedGeneration;
  UINT32            EphemeralKeyCounter;
  SEED_INFO         BiosSeedInfo;
  UINT8             BiosSeed[BOOTLOADER_SEED_LEN_EX];

  ///
  /// Check for valid input parameters
  ///
  if ((Salt == NULL) || (NewKey == NULL) ||
      (SaltSize > BIOS_ADDITIONAL_DATA_LEN) ||
      (NewKeySize != SEED_LENGTH_256_BITS)) {
    return EFI_INVALID_PARAMETER;
  }

  LockSeedGeneration = 0;
  EphemeralKeyCounter = 0;
  ZeroMem (&BiosSeedProperties, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeedInfo, sizeof (SEED_INFO));

  //
  // Get the seed with following specification
  // - Request BIOS and CSME SVN based key
  // - Request 256 byte seed
  // - Request leave seed interface un-locked
  // - Use Salt data as BIOS additional data
  //
  BiosSeedProperties.SeedPropertiesBitMap.SeedLength = SeedLength256Bits;
  BiosSeedProperties.SeedPropertiesBitMap.SeedType   = BiosCsmeSvnBasedSeedType;
  BiosSeedProperties.CsmeSvn                         = 1;
  CopyMem (BiosSeedProperties.BiosAdditionalData, Salt, SaltSize);
  Status = PeiHeciGetBiosSeed (
             &BiosSeedProperties,
             LockSeedGeneration,
             &EphemeralKeyCounter,
             &BiosSeedInfo,
             (UINT8 *) &BiosSeed
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy value in BiosSeed to NewKey
  for (UINTN Index = 0; Index < NewKeySize; Index++) {
    NewKey[Index] = BiosSeed[Index];
  }

  //
  // Clear security data in infrequently mapped memory.
  //
  ZeroMem (&BiosSeedProperties, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeedInfo, sizeof (SEED_INFO));
  ZeroMem (&BiosSeed, sizeof (BiosSeed));

  return Status;
}

/**
  Generate a 512 bit key.

  @param[in]   Salt                     Pointer to the salt (non-secret) value.
  @param[in]   SaltSize                 Salt size in bytes.  Must be less than 384-bits.
  @param[out]  NewKey                   Pointer to caller allocated buffer to receive new key.
  @param[in]   NewKeySize               Size of new key bytes to generate.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval OTHER                         The function completed with failure.
**/
EFI_STATUS
EFIAPI
Generate512BitKey (
  IN   UINT8        *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *NewKey,
  IN   UINTN        NewKeySize
  )
{
  EFI_STATUS        Status;
  SEED_PROPERTIES   BiosSeedProperties;
  UINT8             LockSeedGeneration;
  UINT32            EphemeralKeyCounter;
  SEED_INFO         BiosSeedInfo;
  UINT8             BiosSeed[BOOTLOADER_SEED_LEN_EX];

  ///
  /// Check for valid input parameters
  ///
  if ((Salt == NULL) || (NewKey == NULL) ||
      (SaltSize > BIOS_ADDITIONAL_DATA_LEN) ||
      (NewKeySize != SEED_LENGTH_512_BITS)) {
    return EFI_INVALID_PARAMETER;
  }

  LockSeedGeneration = 0;
  EphemeralKeyCounter = 0;
  ZeroMem (&BiosSeedProperties, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeedInfo, sizeof (SEED_INFO));

  //
  // Get the seed with following specification
  // - Request BIOS and CSME SVN based key
  // - Request 512 byte seed
  // - Request leave seed interface un-locked
  // - Use Salt data as BIOS additional data
  //
  BiosSeedProperties.SeedPropertiesBitMap.SeedLength = SeedLength512Bits;
  BiosSeedProperties.SeedPropertiesBitMap.SeedType   = BiosCsmeSvnBasedSeedType;
  BiosSeedProperties.CsmeSvn                         = 1;
  CopyMem (BiosSeedProperties.BiosAdditionalData, Salt, SaltSize);
  Status = PeiHeciGetBiosSeed (
             &BiosSeedProperties,
             LockSeedGeneration,
             &EphemeralKeyCounter,
             &BiosSeedInfo,
             (UINT8 *) &BiosSeed
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Copy value in BiosSeed to NewKey
  for (UINTN Index = 0; Index < NewKeySize; Index++) {
    NewKey[Index] = BiosSeed[Index];
  }

  //
  // Clear security data in infrequently mapped memory.
  //
  ZeroMem (&BiosSeedProperties, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeedInfo, sizeof (SEED_INFO));
  ZeroMem (&BiosSeed, sizeof (BiosSeed));

  return Status;
}

/**
  Get Trusted Channel Key from CSME.

  @retval EFI_SUCCESS                   The function completed successfully
  @retval OTHER                         The function completed with failure.
**/
EFI_STATUS
GetTrustedChannelKey (
  VOID
  )
{
  EFI_STATUS                       Status;
  SEED_PROPERTIES                  BiosSeedProperties;
  UINT8                            LockSeedGeneration;
  UINT32                           EphemeralKeyCounter;
  SEED_INFO                        BiosSeedInfo;
  UINT8                            BiosSeed[BOOTLOADER_SEED_LEN_EX];
  VOID                             *KeyServiceTrustedHob;
  KEY_SERVICE_TRUSTED_DATA         KeyServiceTrustedHobData;
  UINT32                           Index;

  LockSeedGeneration = 0;
  EphemeralKeyCounter = 0;
  ZeroMem (&BiosSeedProperties, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeed, sizeof (BiosSeed));
  ZeroMem (&BiosSeedInfo, sizeof (SEED_INFO));

  //
  // Get the current Seed
  // - Request BIOS and CSME SVN based key
  // - Request 256 byte seed
  // - Request leave seed interface un-locked
  //
  BiosSeedProperties.SeedPropertiesBitMap.SeedType   = EphmeralKeyTrustedChannelSeedType;
  BiosSeedProperties.SeedPropertiesBitMap.SeedLength = SeedLength256Bits;
  BiosSeedProperties.CsmeSvn = 1;
  Status = PeiHeciGetBiosSeed (
             &BiosSeedProperties,
             LockSeedGeneration,
             &EphemeralKeyCounter,
             &BiosSeedInfo,
             (UINT8 *) &BiosSeed
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Get Trusted Key failed with Error: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Save current seed and ephemeral key to hob
  //
  ZeroMem (&KeyServiceTrustedHobData, sizeof (KEY_SERVICE_TRUSTED_DATA));
  CopyMem (KeyServiceTrustedHobData.TrustedKey, &BiosSeed, SEED_LENGTH_256_BITS);
  CopyMem (&(KeyServiceTrustedHobData.EphemeralKey), &EphemeralKeyCounter, sizeof (UINT32));

  ///
  /// Create Key Service Trusted Hob
  ///
  KeyServiceTrustedHob = BuildGuidDataHob (
                           &gKeyServiceTrustedChannelHobGuid,
                           &KeyServiceTrustedHobData,
                           sizeof (KEY_SERVICE_TRUSTED_DATA)
                           );
  ASSERT (KeyServiceTrustedHob != NULL);

  DEBUG ((DEBUG_INFO, "Trusted Key Value: "));
  for (Index = 0; Index < SEED_LENGTH_256_BITS; Index++) {
    DEBUG ((DEBUG_INFO, "%02x", BiosSeed[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  //
  // Clear security data in infrequently mapped memory.
  //
  ZeroMem (&BiosSeedProperties, sizeof (SEED_PROPERTIES));
  ZeroMem (&BiosSeedInfo, sizeof (SEED_INFO));
  ZeroMem (&BiosSeed, sizeof (BiosSeed));

  return Status;
}

/**
  Init routine for Key Service PEI.

  @param[in] FfsHeader            FfsHeader pointer.
  @param[in] PeiServices          PeiServices pointer.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
KeyServicePeiInit (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  ///
  /// Get Trusted Channel Key.
  /// This is used by the TrustedChannelSmm driver.
  ///
  Status = GetTrustedChannelKey ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetTrustedChannelKey() failed - %r\n", __FUNCTION__, Status));
  }

  ///
  /// Install Key Service PPI
  ///
  Status = PeiServicesInstallPpi (&mInstallKeyServicePpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to install PPI - %r\n", __FUNCTION__, Status));
    return Status;
  }

  return Status;
}