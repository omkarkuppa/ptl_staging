/** @file
  PtlPcdPolicyLib header file

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

#ifndef _PTL_PCD_POLICY_LIB_H_
#define _PTL_PCD_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print whole PCH_PREMEM_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
**/
VOID
EFIAPI
PtlPcdPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi
  );

/**
  Print whole SI_POLICY_PPI and serial out.

  @param[in] SiPolicyPpi               The RC Policy PPI instance
**/
VOID
EFIAPI
PtlPcdPrintPolicyPpi (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );

/**
  Get PCH PREMEM config block table total size.

  @retval                               Size of PCH PREMEM config block table
**/
UINT16
EFIAPI
PtlPcdGetPreMemConfigBlockTotalSize (
  VOID
  );

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
PtlPcdGetConfigBlockTotalSize (
  VOID
  );

/**
  PchAddPreMemConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PtlPcdAddPreMemConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

/**
  PchAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PtlPcdAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  );

#endif
