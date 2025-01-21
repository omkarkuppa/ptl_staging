/** @file
    TseInfoLib NULL implementation.

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

/**
   Returns if the hardware supports TSE based on the detected hardware capabilities.

   @retval TRUE             If TSE is supported
   @retval FALSE            If TSE is not supported
**/
BOOLEAN
IsTseSupported (
  VOID
  )
{
  return FALSE;
}

/**
   Reports if TSE is enabled or not

   @retval TRUE             If TSE is enabled
   @retval FALSE            If TSE is not enabled
**/
BOOLEAN
IsTseEnabled (
  VOID
  )
{
  return FALSE;
}


/**
  This function returns the TSE capabilities supported algorithm mask.

  @retval Supported algorithm mask value
**/
UINT16
EFIAPI
GetTseSupportedAlgorithms (
VOID
  )
{
  return 0;
}

/**
   This function return if the AesXts256 encryption algorithm is supported

   @retval TRUE             If AesXts256 is supported
   @retval FALSE            If AesXts256 is not supported
**/
BOOLEAN
EFIAPI
IsAesXts256Supported (
  VOID
  )
{
  return FALSE;
}


/**
  This function return TseMaxKey value (15 bits)

  @retval TseMaxKey value
**/
UINT16
EFIAPI
GetTseMaxKeys (
  VOID
  )
{
  return 0;
}

/**
   This function returns the TseEngineKeySourcesSupported value (8 bits)
   from the TSE capabilities

   @retval TseEngineKeySourcesSupported value
**/
UINT8
EFIAPI
GetTseKeySources (
  VOID
  )
{
  return 0;
}
