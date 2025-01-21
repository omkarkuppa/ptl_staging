/** @file
  Private data structures and function prototypes.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _CPU_INIT_DXE_H
#define _CPU_INIT_DXE_H

#include <CpuPowerManagementPrivateDataHob.h>

#define TXT_OPT_IN_VMX_AND_SMX_MSR_VALUE    0xFF03

/**
  Copy Global MTRR data to S3
**/
VOID
SaveBspMtrrForS3 (
  VOID
  );

/**
  Initialize BIOS Guard NVS Area Protocol

  @param[in] ImageHandle - Image handle of the loaded driver

  @retval EFI_SUCCESS           - thread can be successfully created
**/
EFI_STATUS
BiosGuardAcpiInit (
  IN EFI_HANDLE       ImageHandle
  );

#endif
