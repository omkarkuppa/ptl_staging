/** @file
  This file contains Overclocking tolopolgy for specific generation.

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

@par Specification
**/
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Register/CpuGenInfoRegs.h>

/**
  This function return whether Granular Ratio Override supported or not.

  @retval TRUE  - Granular Ratio Override supported.
  @retval FALSE - Granular Ratio Override unsupported.
**/
BOOLEAN
EFIAPI
IsGranularRatioSupportFru (
  VOID
  )
{
  return FALSE;
}

/**
  This function return whether Per Core Votage supported or not.

  @retval TRUE  - Per Core Votage supported.
  @retval FALSE - Per Core Votage unsupported.
**/
BOOLEAN
EFIAPI
IsPerCoreVotageFru (
  VOID
  )
{
  return FALSE;
}