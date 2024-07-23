/** @file
  Hybrid Graphics information library.

  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/HybridGraphicsInfoFruLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PcdInfoLib.h>

/**
  Get Hybrid Graphics Root Port Device and Function Number by Root Port physical Number

  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
**/
EFI_STATUS
EFIAPI
GetHybridGraphicsRpDevFun (
  OUT UINTN   *RootPortDev,
  OUT UINTN   *RootPortFun
  )
{
  UINTN       RpIndex;

  if (PtlIsPcdP ()) {
    RpIndex = 8;
  } else {
    RpIndex = 10;
  }
  GetPchPcieRpDevFun (RpIndex, RootPortDev, RootPortFun);

  return EFI_SUCCESS;
}