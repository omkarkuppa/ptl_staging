/** @file
  Header file for BIOS Guard Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _BIOSGUARD_SERVICES_DEBUG_H_
#define _BIOSGUARD_SERVICES_DEBUG_H_

#include <BiosGuard.h>

/**
  Display condition associated with additional data returned by the module
  with error code Unsupported Cpu

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToUnsupportedCpu (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code Bad Directory

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadDirectory (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code Bad BGPDT

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuarMatchDetailsStringToBadBgpdt (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code Bad BGUP

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadBgup (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code Bad BGUPC

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadBgupc (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code Bad SVN

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToBadSvn (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code Invalid Address Range Configuration

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToInvalidAddrRangeConf (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code General Config Error

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToGeneralConfigError (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code Internal Error

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToInternalError (
  IN UINT16 BiosGuardAdditionalData
  );

/**
  Display condition associated with additional data returned by the module
  with error code EC Operation Failed

  @param[in] BiosGuardAdditionalData  Additional data returned by the module
**/
VOID
EFIAPI
BiosGuardMatchDetailsStringToEcOperaionFailed (
  IN UINT16 BiosGuardAdditionalData
  );

#endif
