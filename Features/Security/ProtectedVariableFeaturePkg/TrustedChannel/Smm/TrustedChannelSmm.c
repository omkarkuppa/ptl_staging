/** @file
  Implementation file for HECI Trusted Channnel Services.

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

#include "TrustedChannelSmm.h"
#include <Library/BaseLib.h>
#include <Library/BaseCryptLib.h>
#include <MeBiosPayloadData.h>
#include <MeBiosPayloadHob.h>
#include <Ppi/KeyServicePpi.h>
#include <TrustedChannelMsgs.h>
#include <Protocol/TrustedChannelProtocol.h>
#include <Register/HeciRegs.h>
#include <Library/PciSegmentLib.h>
#include <KeyService.h>

STATIC UINT32  mSignMonotonicCount;
STATIC UINT32  mVerifyMonotonicCount;
STATIC UINT8   mHmacSha256Key[TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE] = {0};

HECI_TRUSTED_CHANNEL_PROTOCOL mHeciTrustedChannelProtocol = {
  HeciTrustedChannelDecrementMonotonicCount,
  HeciTrustedChannelIsTrustedChannelEnabled,
  HeciTrustedChannelUpdateTrustedHeader,
  HeciTrustedChannelVerifyTrustedHeader
};

/**
  Computes an HMAC SHA256 signature for a data message.

  @param[in]      HmacSha256Key   Pointer to the user-supplied key.
  @param[in]      KeySize         Key size in bytes.
  @param[in]      Data            The data buffer.
  @param[in]      DataSize        The size in bytes of data in the data buffer.
  @param[in, out] Signature       The HMAC SHA256 signature computed.

  @return     EFI_SUCCESS         The signature was computed successfully.
  @return     Others              Failed to compute the signature.
**/
EFI_STATUS
ComputeHmacSha256Signature (
  IN CONST UINT8  *HmacSha256Key,
  IN       UINTN   KeySize,
  IN CONST VOID    *Data,
  IN       UINTN   DataSize,
  IN OUT   UINT8   *Signature
  )
{
  VOID       *HmacContext;
  UINT8      Digest[64];
  BOOLEAN    Status;

  if (HmacSha256Key == NULL || Data == NULL || Signature == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // HMAC-SHA256 Digest Validation
  //
  ZeroMem (Digest, sizeof (Digest));

  HmacContext = HmacSha256New ();

  Status  = HmacSha256SetKey (HmacContext, HmacSha256Key, KeySize);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  Status  = HmacSha256Update (HmacContext, Data, DataSize);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  Status  = HmacSha256Final (HmacContext, Digest);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  CopyMem (Signature, Digest, SHA256_DIGEST_SIZE);
  HmacSha256Free (HmacContext);

  return EFI_SUCCESS;
}

/**
  Retrieves the Trusted Channel key.

  @param[out] TrustedChannelKey      A pointer to a data buffer to hold the trusted key.

  @return     EFI_SUCCESS            Retrieved the trusted channel key successfully.
  @retval     EFI_INVALID_PARAMETER  The TrustedChannelData pointer is NULL
  @retval     EFI_NOT_FOUND          The trusted channel key could not be found.
  @return     Others                 Failed to retrieve the trusted channel key.
**/
EFI_STATUS
GetTrustedChannelKey (
  OUT KEY_SERVICE_TRUSTED_DATA   *TrustedChannelData
  )
{
  VOID                     *KeyServiceTrustHob;
  KEY_SERVICE_TRUSTED_DATA *KeyServiceTrustHobData;

  if (TrustedChannelData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get the Key Service Trusted Channel HOB
  //
  KeyServiceTrustHob = GetFirstGuidHob (&gKeyServiceTrustedChannelHobGuid);
  if (KeyServiceTrustHob != NULL) {
    KeyServiceTrustHobData = (KEY_SERVICE_TRUSTED_DATA *) GET_GUID_HOB_DATA (KeyServiceTrustHob);

    CopyMem (
      (VOID *) TrustedChannelData,
      (VOID *) KeyServiceTrustHobData,
      sizeof (KEY_SERVICE_TRUSTED_DATA)
      );
  } else {
    DEBUG ((DEBUG_ERROR, "The Key Service Trusted Channel HOB item was not found.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Clear the TrustedKey item from the HOB
  //
  ZeroMem (
    (VOID *) &(KeyServiceTrustHobData->TrustedKey),
    sizeof (KEY_SERVICE_TRUSTED_DATA)
    );

  return EFI_SUCCESS;
}

/**
  Returns whether trusted channel is enabled.

  @retval     TRUE if trusted channel is enabled.
              FALSE if trusted channel is disabled.
**/
BOOLEAN
EFIAPI
HeciTrustedChannelIsTrustedChannelEnabled (
  VOID
  )
{
  return (mSignMonotonicCount != 0 && mVerifyMonotonicCount != 0);
}

/**
  Decrements the monotonic count.

  This function should only be called if an error occurred that prevented the monotonic count
  from being updated in the other agent.

  @param[in]        Count            The decrement count

  @retval           EFI_SUCCESSS     The monotonic count was updated successfully
  @retval           Others           The monotonic count could not be updated
**/
EFI_STATUS
EFIAPI
HeciTrustedChannelDecrementMonotonicCount (
  IN      UINT32    Count
  )
{
  if (Count > mSignMonotonicCount) {
    return EFI_INVALID_PARAMETER;
  }
  mSignMonotonicCount -= Count;

  return EFI_SUCCESS;
}

/**
  Updates the SHA256 signature and monotonic counter fields of a HECI message header.

  @param[in,out]  MessageHeader   A pointer to the message header
  @param[in]      TotalHeaderSize The total header size
  @param[in]      TotalDataSize   The total data size

  @retval         EFI_SUCCESSS    The header was updated successfully
  @retval         Others          The header could not be updated
**/
EFI_STATUS
EFIAPI
HeciTrustedChannelUpdateTrustedHeader (
  IN OUT  VOID        *MessageHeader,
  IN      UINT32      TotalHeaderSize,
  IN      UINT32      TotalDataSize
  )
{
  EFI_STATUS                   Status;
  TRUSTED_CHANNEL_BIOS_HEADER  *TrustedChannelHeader = NULL;
  UINT8                        Digest[64];

  if (MessageHeader == NULL || TotalHeaderSize < sizeof (TRUSTED_CHANNEL_BIOS_HEADER)) {
    return EFI_INVALID_PARAMETER;
  }

  TrustedChannelHeader = (TRUSTED_CHANNEL_BIOS_HEADER  *) MessageHeader;
  TrustedChannelHeader->MonotonicCounter = mSignMonotonicCount;

  //
  // Compute the HMAC SHA-256 digest
  //
  // Includes all fields except the signature field
  //
  Status = ComputeHmacSha256Signature (
             mHmacSha256Key,
             sizeof (mHmacSha256Key),
             (VOID *) ((UINT8 *) MessageHeader + TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE),
             (TotalHeaderSize + TotalDataSize) - TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE,
             Digest
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CopyMem (&TrustedChannelHeader->Signature, Digest, TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE);
  mSignMonotonicCount++;

  return EFI_SUCCESS;
}

/**
  Verifies the SHA 256 HMAC signature of a HECI message.

  @param[in]        MessageHeader             A pointer to the message header
  @param[in]        TotalDataSize             The total data size
  @param[out]       IsVerificationSuccessful  Set to TRUE if the signature in the given message is valid

  @retval           EFI_SUCCESSS     The verification was successful
  @retval           Others           An error occurred during verification
**/
EFI_STATUS
EFIAPI
HeciTrustedChannelVerifyTrustedHeader (
  IN CONST VOID      *MessageHeader,
  IN       UINT32    TotalDataSize,
  OUT      BOOLEAN   *IsVerificationSuccessful
  )
{
  EFI_STATUS                         Status;
  CONST TRUSTED_CHANNEL_BIOS_HEADER  *TrustedChannelHeader = NULL;
  UINT8                              Digest[64];

  *IsVerificationSuccessful = FALSE;
  ZeroMem (Digest, sizeof (Digest));

  if (mSignMonotonicCount == 0 || mVerifyMonotonicCount == 0 || CompareMem (mHmacSha256Key, Digest, sizeof (mHmacSha256Key)) == 0) {
    // Verification should always occur after the monotonic counter and key have been initialized
    ASSERT (FALSE);
    return EFI_NOT_READY;
  }

  TrustedChannelHeader = (CONST TRUSTED_CHANNEL_BIOS_HEADER  *) MessageHeader;

  if ((mVerifyMonotonicCount + 1) != TrustedChannelHeader->MonotonicCounter) {
    DEBUG ((DEBUG_ERROR, "Monotonic counter value is invalid!\n"));
    return EFI_SECURITY_VIOLATION;
  }

  //
  // Compute the HMAC SHA-256 digest
  //
  // Includes all fields except the signature field
  //
  Status = ComputeHmacSha256Signature (
             mHmacSha256Key,
             sizeof (mHmacSha256Key),
             (UINT8 *) MessageHeader + TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE,
             TotalDataSize - TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE,
             Digest
           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (CompareMem (Digest, TrustedChannelHeader->Signature, TRUSTED_CHANNEL_HMAC_SHA256_SIGNATURE_SIZE) == 0) {
    DEBUG ((DEBUG_INFO, "Trusted channel signature is valid.\n"));
    *IsVerificationSuccessful = TRUE;
    mVerifyMonotonicCount++;
  } else {
    DEBUG ((DEBUG_INFO, "Trusted channel signature is invalid!\n"));
  }

  return EFI_SUCCESS;
}

/**
  Entry point of the SMM Trusted Channel Driver.

  @param[in]  ImageHandle    The firmware allocated handle for the EFI image.
  @param[in]  SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS    The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
TrustedChannelSmmEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE                 Handle;
  EFI_STATUS                 Status;
  KEY_SERVICE_TRUSTED_DATA   TrustedChannelData;
  UINT32                     Index;

  Handle = NULL;

  if ((mSignMonotonicCount == 0) || (mVerifyMonotonicCount == 0)) {
    //
    // Get the Trusted Channel data for this boot
    //
    Status = GetTrustedChannelKey (&TrustedChannelData);
    if (!EFI_ERROR (Status)) {
      CopyMem ((VOID *) mHmacSha256Key, (VOID *) TrustedChannelData.TrustedKey, sizeof (mHmacSha256Key));
      mSignMonotonicCount = TrustedChannelData.EphemeralKey;
      mVerifyMonotonicCount = TrustedChannelData.EphemeralKey;

      DEBUG ((DEBUG_INFO, "TrustedKey: "));
      for (Index = 0; Index < sizeof (mHmacSha256Key); Index++) {
        DEBUG ((DEBUG_INFO, "%X", mHmacSha256Key[Index]));
      }
      DEBUG ((DEBUG_INFO, "\nHMAC Monotonic Count = 0x%x\n", mSignMonotonicCount));
    } else {
      DEBUG ((DEBUG_ERROR, "Unable to get the SMM trusted key. Cannot send HECI2 transactions.\n"));
      return Status;
    }
  }

  //
  // Install the Trusted Channel SMM Protocol
  //
  return gSmst->SmmInstallProtocolInterface (
                  &Handle,
                  &gHeciTrustedChannelProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mHeciTrustedChannelProtocol
                  );
}
