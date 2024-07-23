/** @file
  eSPI SMI Dispatch header

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _PCH_SMI_HELPER_H_
#define _PCH_SMI_HELPER_H_
#include <Uefi/UefiBaseType.h>

/**
  Get Root Port physical Number by CPU or PCH Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)
**/
VOID
GetPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  );

/**
  Get CPU or PCH Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.
**/
VOID
GetPcieRpDevFun (
  IN  UINTN   RpIndex,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

/**
  For each PCIE RP clear PME SCI status and disable SCI, then PCIEXP_WAKE_STS from PMC.
  This prevents platform from waking more than one time due to a single PCIE wake event.
  Normally it's up to OS to clear SCI statuses. But in a scenario where platform wakes
  and goes to S5 instead of booting to OS, the SCI status would remain set and would trigger another wake.
**/
VOID
ClearPcieSci (
  VOID
  );


/**
  Performs update of SmiDispatch descriptors with values that have to be evaluated during runtime.
**/
VOID
PchSmiDispatchUpdateDescriptors (
  VOID
  );

#endif
