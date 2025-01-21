/** @file

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution. The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include "DxeSaveMemoryConfigLib.h"

/**
  This is driver unload entry point to uninstall AIP.

  @param[in] ImageHandle  A handle for the image that is initializing this driver
  @param[in] SystemTable  A pointer to the EFI system table

  @retval    EFI_SUCCESS  The initialization finished successfully.
**/
EFI_STATUS
EFIAPI
DxeSaveMemoryConfigLibDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                        Status;
  UINTN                             VariableSize;

  DEBUG ((DEBUG_ERROR, "DxeSaveMemoryConfigLibDestructor is called!\n"));

  VariableSize  = 0;
  Status = GetLargeVariable (L"FspNvsBuffer", &gFspNvsBufferVariableGuid, &VariableSize, NULL);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    //
    // Set the DISB bit
    // after S3 Data is saved to NVRAM.
    //
    PmcSetDramInitScratchpad ();
    DEBUG ((DEBUG_ERROR, "PmcSetDramInitScratchpad() is called\n"));
  }

  return EFI_SUCCESS;
}