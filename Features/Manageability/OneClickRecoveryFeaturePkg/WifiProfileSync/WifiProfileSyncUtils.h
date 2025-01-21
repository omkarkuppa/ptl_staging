/** @file
  WiFi profile sync utility function definitions.

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

#ifndef _WIFI_PROFILE_SYNC_UTILS_H_
#define _WIFI_PROFILE_SYNC_UTILS_H_

#include <PiDxe.h>

enum {
// Encryption method from CSME
  EncryptionMethodUnknown                   = 0xff,
  EncryptionMethodNone                      = 0x01,
  EncryptionMethodWep                       = 0x02,  ///< (should not be supported any more)
  EncryptionMethodTkip                      = 0x08,  ///< (should not be supported any more)
  EncryptionMethodCcmp                      = 0x10,
  EncryptionMethodGcmp256                   = 0x100,

// AKM authentication method from CSME
  AuthenticationMethodOpen                  = 0x00, ///< open systems
  AuthenticationMethodSharedKey             = 0x01,
  AuthenticationMethodWpa                   = 0x02, ///< wpa (wpa ieee 802.1x)
  AuthenticationMethodWpaPsk                = 0x04,
  AuthenticationMethodRsn                   = 0x08, ///< rsn (wpa2 ieee 802.1x)
  AuthenticationMethodRsnPsk                = 0x10, ///< rsn psk (wpa2 psk)
  AuthenticationMethodWpa3Sae               = 0x20,
  AuthenticationMethodWpa3Owe               = 0x40,
  AuthenticationMethodUnknown               = 0xff,

// Authentication protocol EAP method from CSME
  AuthenticationProtocolEapMethodDisabled   = 0,
  AuthenticationProtocolEapMethodGtc        = 6,    ///< (credentials_8021x)
  AuthenticationProtocolEapMethodTls        = 13,   ///< (pki_8021x)
  AuthenticationProtocolEapMethodTtls       = 21,   ///< (pki_8021x)
  AuthenticationProtocolEapMethodPeap       = 25,   ///< (pki_8021x)
  AuthenticationProtocolEapMethodFast       = 43,   ///< (pac_8021x)
  AuthenticationProtocolEapMethodUnknown    = 0xff,

// Authentication protocol inner method from CSME
  AuthenticationProtocolInnerMethodNone     = 0,
  AuthenticationProtocolInnerMethodMschapv2 = 1,
  AuthenticationProtocolInnerMethodGtc      = 2,
  AuthenticationProtocolInnerMethodTls      = 3,
  AuthenticationProtocolInnerMethodUnknown  = 0xff
};

/**
  Converts CSME provided encryption method to WiFiConnectionManager value

  @param[in] AuthenticationMethod  - CSME provided encryption method

  @retval The corresponding WiFiConnectionManager cipher value
**/
UINT8
MapEncryptionToCipher (
  IN UINT32   EncryptionMethod
  );

/**
  Converts CSME provided  AuthenticationProtocol_eapMethod to WiFiConnectionManager EapAuthMethod value

  @param[in] AuthenticationProtocolEapMethod  - CSME provided encryption method

  @retval The corresponding WiFiConnectionManager EAP Method value
**/
UINT8
MapEapAuthMethod (
  IN UINT32   AuthenticationProtocolEapMethod
  );

/**
  Converts CSME provided authentication method to WiFiConnectionManager value

  @param[in] AuthenticationMethod  - CSME provided authentication method

  @retval The corresponding WiFiConnectionManager AKM value
**/
UINT8
MapAuthenticationToAKM (
  IN UINT32   AuthenticationMethod
  );

/**
  Converts CSME provided Inner/Secondary EAP method to WiFiConnectionManager value

  @param[in] AuthInnerMethod  - CSME provided Inner/Secondary EAP method

  @retval The corresponding WiFiConnectionManager secondary/inner EAP method value
**/
UINT8
MapAuthInnerMethodToEapSecondAuthMethod (
  IN UINT32   AuthInnerMethod
  );

/**
  Converts the low nibble of a byte  to hex unicode character.

  @param[in] Nibble - lower nibble of a byte.

  @retval Hex unicode character.
**/
CHAR16
NibbleToHexChar (
  IN UINT8      Nibble
  );

/**
  Converts binary buffer to Unicode string.
  At a minimum, any blob of data could be represented as a hex string.

  @param[in, out] Str   - Pointer to the string.
  @param[in] StrLen     - Length in bytes of buffer to hold the hex string. Includes tailing '\0' character.
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
  IN     UINTN    StrLen,
  IN     UINT8    *Buf,
  IN     UINTN    Len
  );

#endif
