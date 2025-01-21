/** @file
  Initialize TME in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/TmeInfoLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MsrFruLib.h>
#include <Register/GenerationMsr.h>
#include <Library/CpuPlatformLib.h>

/**
  Reports if TME is enabled or not

  @retval TRUE             If TME is enabled
  @retval FALSE            If TME is not enabled
**/
BOOLEAN
IsTmeActivated (
  VOID
  )
{
  MSR_TME_ACTIVATE_REGISTER      TmeActivate;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeActivate.Uint64 = 0;
    TmeActivate.Uint64 = AsmReadMsr64 (MSR_TME_ACTIVATE);

    if ((TmeActivate.Bits.Lock == 1) &&
        (TmeActivate.Bits.TmeEnable == 1)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Reports if Lock bit in TME Activate MSR locked or not

  @retval TRUE             If TME Activate MSR is locked
  @retval FALSE            If TME Activate MSR is not locked
**/
BOOLEAN
IsTmeActivateLocked (
  VOID
  )
{
  MSR_TME_ACTIVATE_REGISTER      TmeActivate;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeActivate.Uint64 = 0;
    TmeActivate.Uint64 = AsmReadMsr64 (MSR_TME_ACTIVATE);
    return TmeActivate.Bits.Lock != 0;
  }

  return 0;
}

/**
  Reports if TME Bypass is supported

  @retval TRUE             If TME Bypass is enabled
  @retval FALSE            If TME Bypass not enabled
**/
BOOLEAN
IsTmeBypassSupported (
  VOID
  )
{
  MSR_TME_CAPABILITY_REGISTER      TmeCapability;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeCapability.Uint64 = AsmReadMsr64 (MSR_TME_CAPABILITY);

    if (TmeCapability.Bits.TmeBypassMode == 1) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Reports if MKTME is supported

  @retval TRUE             If MKTME is supported
  @retval FALSE            If MKTME not supported
**/
BOOLEAN
IsMkTmeSupported (
  VOID
  )
{
  if ((IsTmeSupported () == FALSE) ||
      (IsPConfigSupported() == FALSE) ||
      (IsMkTmePConfigSupported() == FALSE)) {
    return FALSE;
  }
  return TRUE;
}

/**
  This function return if AesXts256 is supported

  @retval TRUE             If AesXts256 is supported
  @retval FALSE            If AesXts256 is not supported
**/
BOOLEAN
EFIAPI
IsTmeAesXts256Supported (
  VOID
  )
{
  MSR_TME_CAPABILITY_REGISTER    TmeCapability;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeCapability.Uint64 = 0;
    TmeCapability.Uint64 = AsmReadMsr64 (MSR_TME_CAPABILITY);
    return TmeCapability.Bits.AesXts256 != 0;
  }

  return FALSE;
}

/**
  Return if TME is enabled

  @retval TRUE             If TME enable & lock bit is set
  @retval FALSE            If TME enable & lock bit is not set
**/
BOOLEAN
EFIAPI
IsTmeEnabled (
  VOID
  )
{
  MSR_TME_ACTIVATE_REGISTER      TmeActivate;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeActivate.Uint64 = 0;
    TmeActivate.Uint64 = AsmReadMsr64 (MSR_TME_ACTIVATE);

    if ((TmeActivate.Bits.Lock == 1) &&
        (TmeActivate.Bits.TmeEnable == 1)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Return if Lock bit in TME Activate MSR is set

  @retval TRUE             If lock bit is set
  @retval FALSE            If lock bit is not set
**/
BOOLEAN
EFIAPI
IsTmeActivateLockSet (
  VOID
  )
{
  MSR_TME_ACTIVATE_REGISTER      TmeActivate;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeActivate.Uint64 = 0;
    TmeActivate.Uint64 = AsmReadMsr64 (MSR_TME_ACTIVATE);
    return TmeActivate.Bits.Lock != 0;
  }

  return 0;
}

/**
  This function return MkTmeMaxKeyidBits value

  @retval MkTmeMaxKeyidBits value
**/
UINT32
EFIAPI
MsrGetMkTmeMaxKeyidBits (
  VOID
  )
{
  MSR_TME_CAPABILITY_REGISTER    TmeCapability;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeCapability.Uint64 = 0;
    TmeCapability.Uint64 = AsmReadMsr64 (MSR_TME_CAPABILITY);
    return (UINT32) TmeCapability.Bits.MkTmeMaxKeyidBits;
  }

  return 0;
}

/**
  This function return MkTmeMaxKey value

  @retval MkTmeMaxKey value
**/
UINT32
EFIAPI
MsrGetMkTmeMaxKey (
  VOID
  )
{
  MSR_TME_CAPABILITY_REGISTER    TmeCapability;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeCapability.Uint64 = 0;
    TmeCapability.Uint64 = AsmReadMsr64 (MSR_TME_CAPABILITY);
    return (UINT32) TmeCapability.Bits.MkTmeMaxKeys;
  }

  return 0;
}

/**
  Detects Multi-Key Total Memory Encryption capability in TME IP.

  @retval TRUE  - Supported
  @retval FALSE - Not Supported
**/
BOOLEAN
IsTmeCapabilityPresent (
  VOID
  )
{
  ///
  /// Detect TME hardware IP before accessing TME registers
  ///
  if (IsTmeSupported () == FALSE) {
    return FALSE;
  }

  ///
  /// Check if AES-XTS-256 encryption is supported
  ///
  if (IsTmeAesXts256Supported () == FALSE)
  {
    DEBUG ((DEBUG_INFO, "MK-TME: AES-XTS-256 encryption is not supported.\n"));
    return FALSE;
  }

  ///
  /// Read Max supported Keys
  ///
  if ((MsrGetMkTmeMaxKeyidBits () == 0) || (MsrGetMkTmeMaxKey () == 0)) {
    DEBUG ((DEBUG_INFO, "MK-TME: Multiple Keys are not supported.\n"));
    return FALSE;
  }

  return TRUE;
}
