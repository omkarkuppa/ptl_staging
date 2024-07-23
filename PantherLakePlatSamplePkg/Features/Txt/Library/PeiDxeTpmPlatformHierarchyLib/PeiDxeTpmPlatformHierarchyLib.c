/** @file

    TPM Platform Hierarchy configuration library.

    This library provides functions for customizing the TPM's Platform Hierarchy
    Authorization Value (platformAuth) and Platform Hierarchy Authorization
    Policy (platformPolicy) can be defined through this function.

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
  https://trustedcomputinggroup.org/resource/tcg-tpm-v2-0-provisioning-guidance/
**/

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/RngLib.h>
#include <Library/Tpm2CommandLib.h>
#include <Library/Tpm2DeviceLib.h>

typedef enum {
  Tpm2NvAux,
  Tpm2NvPs,
  Tpm2NvPo,
  Tpm2NvTypeMax
} TPM2_PROVISION_NV_TYPE;

#define NV_INDEX_AUX  0x01800003
#define NV_INDEX_PS   0x01800001
#define NV_INDEX_PO   0x01400001

TPMI_RH_NV_INDEX   mTpm2ProvisionNvIndex[] = {
  NV_INDEX_AUX,
  NV_INDEX_PS,
  NV_INDEX_PO,
};

//
// The authorization value may be no larger than the digest produced by the hash
//   algorithm used for context integrity.
//
#define      MAX_NEW_AUTHORIZATION_SIZE SHA512_DIGEST_SIZE

UINT16       mAuthSize;
TPM2B_AUTH   mNewPlatformAuth;
TPM2B_AUTH   mNewLockoutAuth;
TPM2B_AUTH   mNewOwnerAuth;

//
// Requested feature:
//   1. Platform BIOS shall set PH's platformPolicy using TPM 2.0 command
//      TPM2_SetPrimaryPolicy()
//
//   2. It shall use HashAlg == SHA256
//
//   3. authPolicy digest to be set must be the digest of the following
//      negotiated policy command: TPM2_PolicyOR (0, SHA256 ("IntelTXT"))
//      This digest is selected to match value used by server platform BIOS
//      which creates consistency of support between TXT tools and provisioning
//      scripts.
//
//   4. Important note - the fixed selected passphrase "IntelTXT" is used to
//      enable Intel reference BIOS and internal and external testing. It is
//      assumed that each of the OEMs will select own passphrase and / or own
//      set of TPM2.0 EA policy commands.
//
TPM2B_DIGEST mNewPlatformPolicy = {
  0x20,
  {
    0x22, 0x03, 0x0B, 0x7E, 0x0B, 0xB1, 0xf9, 0xD5, 0x06, 0x57, 0x57, 0x1E, 0xE2, 0xF7, 0xFC, 0xE1,
    0xEB, 0x91, 0x99, 0x0C, 0x8B, 0x8A, 0xE9, 0x77, 0xFC, 0xB3, 0xF1, 0x58, 0xB0, 0x3E, 0xBA, 0x96
  }
};

TPM2B_DIGEST mNewPlatformPolicySHA384 = {
  0x30,
  {
    0x6A, 0x4D, 0x09, 0xD2, 0x71, 0xD1, 0x51, 0x5F, 0x1A, 0xDA, 0x9C, 0x49, 0x9F, 0xA7, 0x45, 0xA4,
    0x13, 0xC0, 0x5F, 0x2C, 0x6F, 0x15, 0x48, 0xBA, 0x60, 0xF2, 0xFB, 0x2A, 0x60, 0x93, 0x0A, 0xDF,
    0x0B, 0xCD, 0x49, 0x93, 0x6C, 0x04, 0xA8, 0x51, 0x40, 0xDF, 0xDB, 0xCB, 0x2C, 0xC9, 0x1F, 0x7B
  }
};

/**
  Generate high-quality entropy source through RDRAND.

  @param[in]   Length        Size of the buffer, in bytes, to fill with.
  @param[out]  Entropy       Pointer to the buffer to store the entropy data.

  @retval EFI_SUCCESS            Entropy generation succeeded.
  @retval EFI_NOT_READY          Failed to request random data.
  @retval EFI_INVALID_PARAMETER  Pointer to entropy data buffer is invalid.

**/
EFI_STATUS
EFIAPI
RdRandGenerateEntropy (
  IN UINTN         Length,
  OUT UINT8        *Entropy
  )
{
  BOOLEAN     Status;
  UINTN       BlockCount;
  UINT64      Seed[2];
  UINT8       *Ptr;

  if (Entropy == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  BlockCount = Length / sizeof(Seed);
  Ptr = (UINT8 *)Entropy;

  //
  // Generate high-quality seed for DRBG Entropy
  //
  while (BlockCount > 0) {
    Status = GetRandomNumber128 (Seed);
    if (!Status) {
      return EFI_NOT_READY;
    }
    CopyMem (Ptr, Seed, sizeof(Seed));

    BlockCount--;
    Ptr = Ptr + sizeof(Seed);
  }

  //
  // Populate the remained data as request.
  //
  Status = GetRandomNumber128 (Seed);
  if (!Status) {
    return EFI_NOT_READY;
  }
  CopyMem (Ptr, Seed, (Length % sizeof(Seed)));

  return EFI_SUCCESS;
}

/**
  This function returns the maximum size of TPM2B_AUTH; this structure is used for an authorization value
  and limits an authValue to being no larger than the largest digest produced by a TPM.

  @param[out] AuthSize                 Tpm2 Auth size

  @retval EFI_SUCCESS                  Auth size returned.
  @retval EFI_DEVICE_ERROR             Can not return platform auth due to device error.

**/
EFI_STATUS
EFIAPI
GetAuthSize (
  OUT UINT16            *AuthSize
  )
{
  EFI_STATUS            Status;
  TPML_PCR_SELECTION    Pcrs;
  UINTN                 Index;
  UINT16                DigestSize;

  Status = EFI_SUCCESS;

  while (mAuthSize == 0) {

    mAuthSize = SHA1_DIGEST_SIZE;
    ZeroMem (&Pcrs, sizeof (TPML_PCR_SELECTION));
    Status = Tpm2GetCapabilityPcrs (&Pcrs);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Tpm2GetCapabilityPcrs fail!\n"));
      break;
    }

    DEBUG ((DEBUG_ERROR, "Tpm2GetCapabilityPcrs - %08x\n", Pcrs.count));

    for (Index = 0; Index < Pcrs.count; Index++) {
      DEBUG ((DEBUG_ERROR, "alg - %x\n", Pcrs.pcrSelections[Index].hash));

      switch (Pcrs.pcrSelections[Index].hash) {
      case TPM_ALG_SHA1:
        DigestSize = SHA1_DIGEST_SIZE;
        break;
      case TPM_ALG_SHA256:
        DigestSize = SHA256_DIGEST_SIZE;
        break;
      case TPM_ALG_SHA384:
        DigestSize = SHA384_DIGEST_SIZE;
        break;
      case TPM_ALG_SHA512:
        DigestSize = SHA512_DIGEST_SIZE;
        break;
      case TPM_ALG_SM3_256:
        DigestSize = SM3_256_DIGEST_SIZE;
        break;
      default:
        DigestSize = SHA1_DIGEST_SIZE;
        break;
      }

      if (DigestSize > mAuthSize) {
        mAuthSize = DigestSize;
      }
    }
    break;
  }

  *AuthSize = mAuthSize;
  return Status;
}

/**
  This function dump raw data.

  @param  Data  raw data
  @param  Size  raw data size

**/
VOID
InternalDumpData (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN  Index;
  for (Index = 0; Index < Size; Index++) {
    DEBUG ((DEBUG_ERROR, "%02x", (UINTN)Data[Index]));
  }
}

/**
  Read the public area and Name of an NV Index.

  @param[in]  NvType             NV type.

  @retval EFI_SUCCESS            Operation completed successfully.
  @retval EFI_DEVICE_ERROR       The command was unsuccessful.
  @retval EFI_NOT_FOUND          The command was returned successfully, but NvIndex is not found.
**/
EFI_STATUS
ReadPublic (
  IN TPM2_PROVISION_NV_TYPE  NvType
  )
{
  EFI_STATUS                Status;
  TPMI_RH_NV_INDEX          NvIndex;
  TPM2B_NV_PUBLIC           NvPublic;
  TPM2B_NAME                NvName;

  ZeroMem (&NvPublic, sizeof(NvPublic));
  ZeroMem (&NvName, sizeof(NvName));

  NvIndex = mTpm2ProvisionNvIndex[NvType];

  Status = Tpm2NvReadPublic (NvIndex, &NvPublic, &NvName);
  if (EFI_ERROR (Status) && (Status != EFI_NOT_FOUND)) {
    DEBUG ((DEBUG_ERROR, "ReadPublic - TPM NvIndex(0x%08x) device error\n", NvIndex));
    return Status;
  }

  if (Status == EFI_NOT_FOUND) {
    // TPM NvIndex not found
    DEBUG ((DEBUG_ERROR, "ReadPublic - TPM NvIndex(0x%08x) not found\n", NvIndex));
  } else {
    // TPM NvIndex found
    DEBUG ((DEBUG_ERROR, "ReadPublic - TPM NvIndex(0x%08x) found\n", NvIndex));

    // DumpData
    DEBUG ((DEBUG_ERROR, "NvPublicSize        - 0x%04x\n", NvPublic.size));
    DEBUG ((DEBUG_ERROR, "  nvIndex           - 0x%08x\n", NvPublic.nvPublic.nvIndex));
    DEBUG ((DEBUG_ERROR, "  nameAlg           - 0x%04x\n", NvPublic.nvPublic.nameAlg));
    DEBUG ((DEBUG_ERROR, "  attributes        - 0x%08x\n", ReadUnaligned32 ((UINT32 *)&NvPublic.nvPublic.attributes)));
    DEBUG ((DEBUG_ERROR, "  authPolicy.size   - 0x%04x\n", NvPublic.nvPublic.authPolicy.size));
    DEBUG ((DEBUG_ERROR, "  authPolicy.buffer - "));
    InternalDumpData (NvPublic.nvPublic.authPolicy.buffer, NvPublic.nvPublic.authPolicy.size);
    DEBUG ((DEBUG_ERROR, "\n"));
    DEBUG ((DEBUG_ERROR, "  dataSize          - 0x%04x\n", NvPublic.nvPublic.dataSize));
    DEBUG ((DEBUG_ERROR, "NvNameSize          - 0x%04x\n", NvName.size));
    DEBUG ((DEBUG_ERROR, "  NvName.buffer - "));
    InternalDumpData (NvName.name, NvName.size);
    DEBUG ((DEBUG_ERROR, "\n"));
  }
  return Status;
}

/**
  Return if we have TXT provision.

  @retval TRUE  TXT provision detected.
  @retval FALSE TXT provision not detected.
**/
BOOLEAN
HasTxtProvision (
  VOID
  )
{
  EFI_STATUS  AuxStatus;

  AuxStatus = ReadPublic (Tpm2NvAux);
  ReadPublic (Tpm2NvPs);
  ReadPublic (Tpm2NvPo);

  //
  // Use AuxStatus as final result
  //
  if (!EFI_ERROR (AuxStatus)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  This service defines the configuration of the Platform Hierarchy Authorization Value (platformAuth)
  and Platform Hierarchy Authorization Policy (platformPolicy)

 **/
VOID
EFIAPI
ConfigureTpmPlatformHierarchy (
  )
{
  EFI_STATUS             Status;
  TPMS_AUTH_COMMAND      LocalAuthSession;
  UINT16                 AuthSize;
  UINT8                  *Rand;
  UINTN                  RandSize;
  TPMI_YES_NO            HierarchyState;
  BOOLEAN                EnablePlatformHierarchy;

  Rand = NULL;
  RandSize = 0;
  EnablePlatformHierarchy = FALSE;

  if (PcdGetBool (PcdRandomizePlatformHierarchy) ||
      PcdGetBool (PcdTpm2HierarchyChangeAuthPlatform) ||
      PcdGetBool (PcdTpm2TxtProvisionSupport)) {
    EnablePlatformHierarchy = TRUE;
  }

  while (EnablePlatformHierarchy) {
    //
    // Send Tpm2HierarchyChange Auth with random value to avoid PlatformAuth being null
    //
    GetAuthSize (&AuthSize);
    ZeroMem (mNewPlatformAuth.buffer, AuthSize);
    mNewPlatformAuth.size = AuthSize;

    //
    // Allocate one buffer to store random data.
    //
    RandSize = MAX_NEW_AUTHORIZATION_SIZE;
    Rand = AllocatePool (RandSize);

    //
    // If allocation fails, disable platform hierarchy.
    //
    if (Rand == NULL) {
      DEBUG ((DEBUG_ERROR, "ConfigureTpmPlatformHierarchy () - memory allocation error! Disabling TPM Platform hierarchy.\n"));
      EnablePlatformHierarchy = FALSE;
      break;
    }

    Status = RdRandGenerateEntropy (RandSize, Rand);

    //
    // If entropy generation fails, disable platform hierarchy.
    //
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ConfigureTpmPlatformHierarchy () - error generating entropy! Disabling TPM Platform hierarchy.\n"));
      EnablePlatformHierarchy = FALSE;
      break;
    }

    CopyMem (mNewPlatformAuth.buffer, Rand, AuthSize);
    ZeroMem (&LocalAuthSession, sizeof (LocalAuthSession));
    LocalAuthSession.sessionHandle = TPM_RS_PW;
    LocalAuthSession.hmac.size = mNewPlatformAuth.size;
    CopyMem (LocalAuthSession.hmac.buffer, mNewPlatformAuth.buffer, mNewPlatformAuth.size);

    //
    // Program Primary Policy, need localAuthSession to be NULL, response 9A2 if set
    //
    if (PcdGetBool (PcdTpm2TxtProvisionSupport)) {
      if (PcdGetBool (PcdTpm2TxtProvisionSHA384)) {
        Status = Tpm2SetPrimaryPolicy (
                 TPM_RH_PLATFORM,     // AuthHandle
                 NULL,                // AuthSession
                 &mNewPlatformPolicySHA384, // Policy
                 TPM_ALG_SHA384);     // HashAlg

        DEBUG ((DEBUG_INFO, "Tpm2SetPrimaryPolicy - %r\n", Status));
      } else {
        Status = Tpm2SetPrimaryPolicy (
                 TPM_RH_PLATFORM,     // AuthHandle
                 NULL,                // AuthSession
                 &mNewPlatformPolicy, // Policy
                 TPM_ALG_SHA256);     // HashAlg

        DEBUG ((DEBUG_INFO, "Tpm2SetPrimaryPolicy - %r\n", Status));
      }
    }

    //
    // Send Tpm2HierarchyChangeAuth command with the new Auth value
    //
    Status = Tpm2HierarchyChangeAuth (
               TPM_RH_PLATFORM,       // AuthHandle
               NULL,                  // AuthSession
               &mNewPlatformAuth);    // New Auth

    DEBUG ((DEBUG_INFO, "Tpm2HierarchyChangeAuth Result: - %r\n", Status));

    //
    // Reprovision Endorsement Hierarchy
    //
    if (PcdGetBool (PcdTpm2ChangeEps)) {
      Status = Tpm2ChangeEPS (TPM_RH_PLATFORM, &LocalAuthSession);
      DEBUG ((DEBUG_INFO, "Tpm2ChangeEPS - %r\n", Status));
      if (!EFI_ERROR (Status)) {
        PcdSetBoolS (PcdTpm2ChangeEps, FALSE);
      }
    }

    //
    // Reprovision Platform Hierarchy
    //
    if (PcdGetBool (PcdTpm2ChangePps)) {
      Status = Tpm2ChangePPS (TPM_RH_PLATFORM, &LocalAuthSession);
      DEBUG ((DEBUG_INFO, "Tpm2ChangePPS - %r\n", Status));
      if (!EFI_ERROR (Status)) {
        PcdSetBoolS (PcdTpm2ChangePps, FALSE);
      }
    }

    //
    // Reprovision Storage Hierarchy
    //
    if (PcdGetBool (PcdTpm2Clear)) {
      Status = Tpm2ClearControl (TPM_RH_PLATFORM, &LocalAuthSession, NO);
      DEBUG ((DEBUG_INFO, "Tpm2ClearControl - %r\n", Status));
      if (!EFI_ERROR (Status)) {
        Status = Tpm2Clear (TPM_RH_PLATFORM, &LocalAuthSession);
        DEBUG ((DEBUG_INFO, "Tpm2Clear - %r\n", Status));
        if (!EFI_ERROR (Status)) {
          PcdSetBoolS (PcdTpm2Clear, FALSE);
        }
      }
    }

    //
    // Enable or disable Endorsement Hierarchy
    //
    if (PcdGetBool (PcdTpm2HierarchyControlEndorsement)) {
      HierarchyState = YES;
    } else {
      HierarchyState = NO;
    }

    Status = Tpm2HierarchyControl (
               TPM_RH_PLATFORM,      // AuthHandle
               &LocalAuthSession,    // AuthSession
               TPM_RH_ENDORSEMENT,   // Hierarchy
               HierarchyState);      // State

    DEBUG ((DEBUG_INFO, "Tpm2HierarchyControl (0x%08x, %02x)- %r\n", TPM_RH_ENDORSEMENT, HierarchyState, Status));
    //
    // Enable or disable Storage Hierarchy
    //
    if (PcdGetBool (PcdTpm2HierarchyControlOwner)) {
      HierarchyState = YES;
    } else {
      HierarchyState = NO;
    }

    Status = Tpm2HierarchyControl (
               TPM_RH_PLATFORM,       // AuthHandle
               &LocalAuthSession,     // AuthSession
               TPM_RH_OWNER,          // Hierarchy
               HierarchyState);       // State

    DEBUG ((DEBUG_INFO, "Tpm2HierarchyControl (0x%08x, %02x)- %r\n", TPM_RH_OWNER, HierarchyState, Status));

    break;
  }

  //
  // Disable the PH.
  //
  if (!EnablePlatformHierarchy) {
  HierarchyState = NO;
  Status =  Tpm2HierarchyControl (
              TPM_RH_PLATFORM,      // AuthHandle
              NULL,                 // AuthSession
              TPM_RH_PLATFORM,      // Hierarchy
              HierarchyState        // State
              );
  }

  //
  // Clear Secrets
  //
  ZeroMem (&LocalAuthSession.hmac, sizeof (LocalAuthSession.hmac));
  ZeroMem (&mNewPlatformAuth, sizeof(mNewPlatformAuth));
  ZeroMem (&mNewLockoutAuth, sizeof(mNewLockoutAuth));
  ZeroMem (&mNewOwnerAuth, sizeof(mNewOwnerAuth));

  if (Rand != NULL) {
    ZeroMem (Rand, RandSize);
    FreePool (Rand);
  }
}
