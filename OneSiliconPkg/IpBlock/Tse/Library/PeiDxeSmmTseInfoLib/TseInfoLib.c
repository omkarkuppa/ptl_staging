/** @file
    TseInfoLib implementation.

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

#include <Uefi.h>
#include <Ppi/SiPolicy.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/TseInfoLib.h>
#include <Library/TseDataHob.h>
#include <Register/GenerationMsr.h>
#include <TseCpuId.h>

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
  if ((IsTseCapabilityPresent () == TRUE) &&
      (IsTsePconfigSupported () == TRUE)) {
    return TRUE;
  }

  return FALSE;
}

/**
   Reports if TSE is supported and enabled

   @retval TRUE             If TSE is enabled
   @retval FALSE            If TSE is not enabled
**/
BOOLEAN
IsTseEnabled (
  VOID
  )
{
  BOOLEAN TseEnabled;
  TSE_DATA_HOB *TseDataHobPtr;

  TseEnabled = FALSE;
  TseDataHobPtr = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);

  if (TseDataHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "TseInfoLib: Error getting TSE Data HOB!\n"));
    return TseEnabled;
  }

  if ((TseDataHobPtr->TseSupported) &&
      (TseDataHobPtr->TseEnabled)) {
    TseEnabled = TRUE;
  }

  return TseEnabled;
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
  MSR_TSE_CAPABILITY_REGISTER    TseCapability = {0};

  if (IsTseCapabilityPresent () == FALSE) {
    DEBUG ((DEBUG_ERROR, "TseInfoLib: TSE is not supported!\n"));
    return 0;
  }

  TseCapability.Uint64 = AsmReadMsr64 (MSR_TSE_CAPABILITY);

  return (UINT16)(TseCapability.Uint32 & TSE_SUPPORTED_ALGS_MASK);
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
  UINT16 SupportedAlgorithmsMask = 0;

  SupportedAlgorithmsMask = GetTseSupportedAlgorithms();
  return (SupportedAlgorithmsMask & TSE_AES_XTS_256_ALG_BIT_MASK) == TSE_AES_XTS_256_ALG_BIT_MASK ? TRUE : FALSE;
}

/**
  This function return TseMaxKey value (15 bits)

  The actual number of keys is TSE_MAX_KEYS + 1

  @retval TseMaxKey value
**/
UINT16
EFIAPI
GetTseMaxKeys (
  VOID
  )
{
  MSR_TSE_CAPABILITY_REGISTER    TseCapability = {0};

  if (IsTseCapabilityPresent () == FALSE) {
    DEBUG ((DEBUG_ERROR, "TseInfoLib: TSE is not supported!\n"));
    return 0;
  }

  TseCapability.Uint64 = AsmReadMsr64 (MSR_TSE_CAPABILITY);
  return (UINT16) TseCapability.Bits.TseMaxKeys + 1;
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
  MSR_TSE_CAPABILITY_REGISTER    TseCapability = {0};

  if (IsTseCapabilityPresent () == FALSE) {
    DEBUG ((DEBUG_ERROR, "TseInfoLib: TSE is not supported!\n"));
    return 0;
  }

  TseCapability.Uint64 = AsmReadMsr64 (MSR_TSE_CAPABILITY);
  return (UINT8) TseCapability.Bits.TseEngineKeySourcesSupported;
}

