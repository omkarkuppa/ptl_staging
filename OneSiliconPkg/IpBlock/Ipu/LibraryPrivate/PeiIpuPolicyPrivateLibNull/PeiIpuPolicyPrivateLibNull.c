/** @file
  This file provides services for IPU PEI policy default initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <Ppi/SiPolicy.h>


/**
  @param[in] SiPolicyPreMemPpi - Instance of SI_PREMEM_POLICY_PPI
**/
VOID
EFIAPI
IpuPrintPolicyPpiPreMem (
  IN  SI_PREMEM_POLICY_PPI *SiPolicyPreMemPpi
  )
{
  return;
}

/**
  @retval      Size of IPU config block table in case IPU is disabled.

**/
UINT16
EFIAPI
IpuGetConfigBlockTotalSizePreMem (
VOID
  )
{
  return 0;
}

/**

  IpuAddConfigBlocksPreMem adds IPU config blocks.
  No Config Block is added,  return success always.

**/
EFI_STATUS
EFIAPI
IpuAddConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return EFI_SUCCESS;
}
