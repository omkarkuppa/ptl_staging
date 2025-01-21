/** @file
  Header file for PeiDmiPolicy Private library.

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
#ifndef __PEI_DMI_POLICY_LIB_H__
#define __PEI_DMI_POLICY_LIB_H__

#include <Ppi/SiPolicy.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>


/**
  This function prints the PEI DMI phase policy.

  @param[in] SiPreMemPolicyPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
DmiPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

/**
  Get PEI DMI config block table total size.

  @retval     Size of PEI DMI config block table
**/
UINT16
EFIAPI
DmiGetConfigBlockTotalSize (
  VOID
  );


/**
  DmiAddConfigBlocks add all PEI DMI config block.

  @param[in] ConfigBlockTableAddress    The pointer to add SA config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
DmiAddConfigBlocks (
  IN VOID           *ConfigBlockTableAddress
  );

#endif // __PEI_DMI_POLICY_LIB_H__
