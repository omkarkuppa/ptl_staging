/** @file
  PEI WDT App library definitions.

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

#ifndef _PEI_WDT_APP_LIB_H_
#define _PEI_WDT_APP_LIB_H_

#include <Base.h>
#include <Uefi.h>
#include <Ppi/Wdt.h>
#include <Library/DebugLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <WdtAppVariable.h>


/**
  turns on WDT during PEI phase according to requests made by
  OS overclocking application (through WDT status) and BIOS modules (through flash variable)

  @param[in]  WdtPei

  @retval     EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PeiWdtApp (
  IN WDT_PPI    *WdtPei
  );

#endif
