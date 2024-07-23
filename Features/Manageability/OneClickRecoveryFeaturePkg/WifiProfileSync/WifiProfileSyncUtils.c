/** @file
  Utility function support for WiFi Profile Sync driver

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

#include <PiDxe.h>
#include "WifiProfileSyncUtils.h"
#include "WifiProfileSyncConfig.h"
#include <Protocol/EapConfiguration.h>

/**
  Converts CSME provided encryption method to WiFiConnectionManager value

  @param[in] EncryptionMethod  - CSME provided encryption method

  @retval The corresponding WiFiConnectionManager cipher value
**/
UINT8
MapEncryptionToCipher (
  IN UINT32   EncryptionMethod
  )
{
  switch (EncryptionMethod) {
    case EncryptionMethodNone:
      return EncryptionMethodUnknown;
    case EncryptionMethodWep:
      return Ieee80211PairwiseCipherSuiteWEP40;
    case EncryptionMethodTkip:
      return Ieee80211PairwiseCipherSuiteTKIP;
    case EncryptionMethodCcmp:
      return Ieee80211PairwiseCipherSuiteCCMP;
    case EncryptionMethodGcmp256:
      return Ieee80211PairwiseCipherSuiteUseGroupCipherSuite;
    default:
      return EncryptionMethodUnknown;
  }
}

/**
  Converts CSME provided  AuthenticationProtocol_eapMethod to WiFiConnectionManager EapAuthMethod value

  @param[in] AuthenticationProtocolEapMethod  - CSME provided encryption method

  @retval The corresponding WiFiConnectionManager EAP Method value
**/
UINT8
MapEapAuthMethod (
  IN UINT32   AuthenticationProtocolEapMethod
  )
{
  switch (AuthenticationProtocolEapMethod) {
    case AuthenticationProtocolEapMethodDisabled:
      return AuthenticationProtocolEapMethodDisabled;
    case AuthenticationProtocolEapMethodTtls:
      return EapAuthMethodTtls;
    case AuthenticationProtocolEapMethodPeap:
      return EapAuthMethodPeap;
    case AuthenticationProtocolEapMethodTls:
      return EapAuthMethodTls;
    default:
      return EapAuthMethodUnknown;
  }
}

/**
  Converts CSME provided authentication method to WiFiConnectionManager value

  @param[in] AuthenticationMethod  - CSME provided authentication method

  @retval The corresponding WiFiConnectionManager AKM value
**/
UINT8
MapAuthenticationToAKM (
  IN UINT32   AuthenticationMethod
  )
{
  switch (AuthenticationMethod) {
    case AuthenticationMethodOpen:
      return AuthenticationMethodUnknown;
    case AuthenticationMethodSharedKey:
      return Ieee80211AkmSuitePSK;
    case AuthenticationMethodWpa:
      return Ieee80211AkmSuite8021XOrPMKSA;
    case AuthenticationMethodWpaPsk:
      return Ieee80211AkmSuitePSK;
    case AuthenticationMethodRsn:
      return Ieee80211AkmSuite8021XOrPMKSA;
    case AuthenticationMethodRsnPsk:
      return Ieee80211AkmSuitePSK;
    case AuthenticationMethodWpa3Sae:
      return Ieee80211AkmSuiteSAE;
    case AuthenticationMethodWpa3Owe:
      return Ieee80211AkmSuiteOWE;
    default:
      return AuthenticationMethodUnknown;
  }
}

/**
  Converts CSME provided Inner/Secondary EAP method to WiFiConnectionManager value

  @param[in] AuthInnerMethod  - CSME provided Inner/Secondary EAP method

  @retval The corresponding WiFiConnectionManager secondary/inner EAP method value
**/
UINT8
MapAuthInnerMethodToEapSecondAuthMethod (
  IN UINT32   AuthInnerMethod
  )
{
  switch (AuthInnerMethod) {
    case AuthenticationProtocolInnerMethodNone:
      return AuthenticationProtocolInnerMethodNone;
    case AuthenticationProtocolInnerMethodMschapv2:
      return EapSecondAuthMethodMschapV2;
    case AuthenticationProtocolInnerMethodGtc:
      return EapSecondAuthMethodUnknown;
    case AuthenticationProtocolInnerMethodTls:
      return EapSecondAuthMethodUnknown;
    default:
      return EapSecondAuthMethodUnknown;
  }
}

/**
  Converts the low nibble of a byte  to hex unicode character.

  @param[in] Nibble - lower nibble of a byte.

  @retval Hex unicode character.
**/
CHAR16
NibbleToHexChar (
  IN UINT8      Nibble
  )
{
  Nibble &= 0x0F;
  if (Nibble <= 0x9) {
    return (CHAR16)(Nibble + L'0');
  }
  return (CHAR16)(Nibble - 0xA + L'a');
}

/**
  Converts binary buffer to Unicode string.
  At a minimum, any blob of data could be represented as a hex string.

  @param[in, out] Str   - Pointer to the string.
  @param[in] DestLen    - Length in bytes of buffer to hold the hex string. Includes tailing '\0' character.
                            If routine return with EFI_SUCCESS, containing length of hex string buffer.
                            If routine return with EFI_BUFFER_TOO_SMALL, containing length of hex string buffer desired.
  @param[in] Buf        - Buffer to be converted from.
  @param[in] Len        - Length in bytes of the buffer to be converted.

  @retval EFI_SUCCESS: Routine success.
  @retval EFI_BUFFER_TOO_SMALL: The hex string buffer is too small.
**/
EFI_STATUS
BufferToHexArray (
  IN OUT CHAR16   *Str,
  IN     UINTN    DestLen,
  IN     UINT8    *Buf,
  IN     UINTN    Len
  )
{
  UINTN       Idx;
  UINTN       StrIdx;

  if (DestLen < ((Len * 2) + 1)) {
    return EFI_BUFFER_TOO_SMALL;
  }

  StrIdx = 0;
  for (Idx = 0; Idx < Len; Idx++) {
    Str[StrIdx] = NibbleToHexChar ((UINT8)(Buf[Idx] >> 4));
    StrIdx++;
    Str[StrIdx] = NibbleToHexChar ((UINT8)Buf[Idx]);
    StrIdx++;
  }
  Str[StrIdx] = L'\0';
  return EFI_SUCCESS;
}
