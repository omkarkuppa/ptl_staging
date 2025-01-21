/** @file
  Declaration of the HECI Trusted Channel Protocol

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
#ifndef _HECI_TRUSTED_CHANNEL_PROTOCOL_H_
#define _HECI_TRUSTED_CHANNEL_PROTOCOL_H_

/**
  Updates the SHA256 signature and monotonic counter fields of a HECI message header.

  @param[in,out]    MessageHeader    A pointer to the message header
  @param[in]        TotalHeaderSize  The total header size
  @param[in]        TotalDataSize    The total data size

  @retval           EFI_SUCCESSS     The header was updated successfully
  @retval           Others           The header could not be updated
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRUSTED_CHANNEL_UPDATE_TRUSTED_HEADER) (
  IN OUT  VOID        *MessageHeader,
  IN      UINT32      TotalHeaderSize,
  IN      UINT32      TotalDataSize
  );

/**
  Verifies the SHA 256 HMAC signature of a HECI message.

  @param[in]        MessageHeader             A pointer to the message header
  @param[in]        TotalDataSize             The total data size
  @param[in]        IsVerificationSuccessful  Set to TRUE if the signature in the given message is valid

  @retval           EFI_SUCCESSS     The verification was successful
  @retval           Others           An error occurred during verification
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRUSTED_CHANNEL_VERIFY_TRUSTED_HEADER) (
  IN CONST VOID      *MessageHeader,
  IN       UINT32    TotalDataSize,
  OUT      BOOLEAN   *IsVerificationSuccessful
  );

/**
  Decrements the monotonic count.

  This function should only be called if an error occurred that prevented the monotonic count
  from being updated in the other agent.

  @param[in]        Count            The decrement count

  @retval           EFI_SUCCESSS     The monotonic count was updated successfully
  @retval           Others           The monotonic count could not be updated
**/
typedef
EFI_STATUS
(EFIAPI *HECI_TRUSTED_CHANNEL_DECREMENT_MONOTONIC_COUNT) (
  IN      UINT32    Count
  );

/**
  Returns whether the trusted channel is enabled.

  @param      None.

  @retval     TRUE if trusted channel is enabled.
              FALSE if trusted channel is disabled.
**/
typedef
BOOLEAN
(EFIAPI *HECI_TRUSTED_CHANNEL_IS_TRUSTED_CHANNEL_ENABLED) (
  VOID
  );

 ///
 /// HECI protocol provided for DXE phase The interface functions are for sending/receiving
 /// HECI messages between host and CSE.
 ///
typedef struct _EFI_HECI_TRUSTED_CHANNEL_PROTOCOL {
  HECI_TRUSTED_CHANNEL_DECREMENT_MONOTONIC_COUNT   DecrementMonotonicCount;
  HECI_TRUSTED_CHANNEL_IS_TRUSTED_CHANNEL_ENABLED  IsTrustedChannelEnabled;
  HECI_TRUSTED_CHANNEL_UPDATE_TRUSTED_HEADER       UpdateTrustedHeader;
  HECI_TRUSTED_CHANNEL_VERIFY_TRUSTED_HEADER       VerifyTrustedHeader;
} HECI_TRUSTED_CHANNEL_PROTOCOL;

#endif
