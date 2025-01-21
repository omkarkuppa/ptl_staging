/** @file
  Header file for HECI Trusted Channnel Services.

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

#ifndef _TRUSTED_CHANNEL_SMM_H_
#define _TRUSTED_CHANNEL_SMM_H_

#include <TrustedChannelMsgs.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/SmmServicesTableLib.h>

/**
  Returns whether trusted channel is enabled.

  @retval     TRUE if trusted channel is enabled.
              FALSE if trusted channel is disabled.
**/
BOOLEAN
EFIAPI
HeciTrustedChannelIsTrustedChannelEnabled (
  VOID
  );

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
  );

/**
  Updates the Trusted Channel header for a new Trusted Channel message.

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
  );

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
  );

#endif
