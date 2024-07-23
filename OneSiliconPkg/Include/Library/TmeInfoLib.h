/** @file
  Header file to get TME information.

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
#ifndef _TME_INFO_LIB_H_
#define _TME_INFO_LIB_H_

/**
  Reports if TME is enabled or not

  @retval TRUE             If TME is enabled
  @retval FALSE            If TME is not enabled
**/
BOOLEAN
IsTmeActivated (
  VOID
  );

/**
  Reports if Lock bit in TME Activate MSR locked or not

  @retval TRUE             If TME Activate MSR is locked
  @retval FALSE            If TME Activate MSR is not locked
**/
BOOLEAN
IsTmeActivateLocked (
  VOID
  );

/**
  Reports if TME Bypass is supported

  @retval TRUE             If TME Bypass is enabled
  @retval FALSE            If TME Bypass not enabled
**/
BOOLEAN
IsTmeBypassSupported (
  VOID
  );

/**
  Detects Multi-Key Total Memory Encryption capability in TME IP.

  @retval TRUE  - Supported
  @retval FALSE - Not Supported
**/
BOOLEAN
IsTmeCapabilityPresent (
  VOID
  );

/**
  Reports if MKTME is supported

  @retval TRUE             If MKTME is supported
  @retval FALSE            If MKTME not supported
**/
BOOLEAN
IsMkTmeSupported (
  VOID
  );

/**
  Reports if TME is enabled or not

  @retval TRUE             If TME is enabled
  @retval FALSE            If TME is not enabled
**/
BOOLEAN
EFIAPI
IsTmeEnabled (
  VOID
  );

/**
  Reports if Lock bit in TME Activate MSR locked or not

  @retval TRUE             If TME Activate MSR is locked
  @retval FALSE            If TME Activate MSR is not locked
**/
BOOLEAN
EFIAPI
IsTmeActivateLockSet (
  VOID
  );

/**
  Return if AesXts256 is supported

  @retval TRUE             If AesXts256 is supported
  @retval FALSE            If AesXts256 is not supported
**/
BOOLEAN
EFIAPI
IsTmeAesXts256Supported (
  VOID
  );

/**
  This function return MkTmeMaxKeyidBits value
  @retval MkTmeMaxKeyidBits value
**/
UINT32
EFIAPI
MsrGetMkTmeMaxKeyidBits (
  VOID
  );

/**
  This function return MkTmeMaxKey value

  @retval MkTmeMaxKey value
**/
UINT32
EFIAPI
MsrGetMkTmeMaxKey (
  VOID
  );

#endif /* _TME_INFO_LIB_H_ */
