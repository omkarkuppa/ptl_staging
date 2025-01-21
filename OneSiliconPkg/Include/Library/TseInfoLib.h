/** @file
    Header file for the TseInfoLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _TSE_INFO_LIB_H_
#define _TSE_INFO_LIB_H_

#define TSE_AES_XTS_256_ALG_BIT_MASK BIT0

#define TSE_SUPPORTED_ALGS_MASK 0x0000FFFF // Bits 15:0

#pragma pack(push, 1)

typedef union {
  struct {
    UINT16 AesXts256 :1;   // AES-XTS 256-bit encryption algorithm.
    UINT16 Rsvd      :15;
  } Bits;
  UINT16 Uint16;
} TSE_ALGS;

#pragma pack(pop)

/**
   Returns if the hardware supports TSE based on the detected hardware capabilities.

   @retval TRUE             If TSE is supported
   @retval FALSE            If TSE is not supported
**/
BOOLEAN
IsTseSupported (
  VOID
  );

/**
   Reports if TSE is enabled or not

   @retval TRUE             If TSE is enabled
   @retval FALSE            If TSE is not enabled
**/
BOOLEAN
IsTseEnabled (
  VOID
  );


/**
  This function returns the TSE capabilities supported algorithm mask.

  @retval Supported algorithm mask value
**/
UINT16
EFIAPI
GetTseSupportedAlgorithms (
  VOID
  );


/**
   This function return if the AesXts256 encryption algorithm is supported

   @retval TRUE             If AesXts256 is supported
   @retval FALSE            If AesXts256 is not supported
**/
BOOLEAN
EFIAPI
IsAesXts256Supported (
  VOID
  );

/**
  This function return TseMaxKey value (15 bits)

  @retval TseMaxKey value
**/
UINT16
EFIAPI
GetTseMaxKeys (
  VOID
  );

/**
   This function returns the TseEngineKeySourcesSupported value (8 bits)
   from the TSE capabilities

   @retval TseEngineKeySourcesSupported value
**/
UINT8
EFIAPI
GetTseKeySources (
  VOID
  );

#endif // _TSE_INFO_LIB_H_

