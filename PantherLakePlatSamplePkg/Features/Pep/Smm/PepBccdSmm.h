/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _PEP_BCCD_SMM_H_
#define _PEP_BCCD_SMM_H_

#include "SetupVariable.h"
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>

//
// Callback function prototypes
//
EFI_STATUS
RegisterPepBccdSwSmi (
  VOID
  );
#endif

