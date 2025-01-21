/** @file
  This file contains an implementation of the function call interfaces
  required by the main SiInit PEIM file. Hopefully, future platform porting
  tasks will be mostly limited to modifying the functions in this file.

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

#include <Uefi.h>
#include <DprInfoHob.h>

/**
  Init and Install DPR Info Hob

  @param[out] DprInfoHob         - DPR_INFO_HOB instance installed by this function
  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallDprInfoHob (
  OUT      DPR_INFO_HOB               **DprInfoHobOut
  )
{
  return EFI_SUCCESS;
}

/**
  Determine the Total DPR memory size needed based on the DPR directory in the SA Data HOB.

  @param[in, out] Size        - The memory size in MB to return.

  @retval Nothing.
**/
VOID
EFIAPI
CalculateTotalDprMemorySize (
  IN OUT   UINT8             *Size
  )
{
}

/**
  Calculates the bases for each technology consuming the DPR region
  and updates the SA Data HOB with the appropriate values in the Dpr
  directory

  @param[in] Base           - The memory base to return.
  @param[in] TotalDprSizeMB - The total DPR size in MB

  @retval Nothing.
**/
VOID
EFIAPI
UpdateDprHobInfo (
  IN       EFI_PHYSICAL_ADDRESS  Base,
  IN       UINT8                 TotalDprSizeMB
  )
{
}
