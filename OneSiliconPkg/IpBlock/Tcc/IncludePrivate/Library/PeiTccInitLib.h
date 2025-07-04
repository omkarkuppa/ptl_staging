/** @file
  Header file for PeiTccInitLib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _PEI_TCC_INIT_LIB_H_
#define _PEI_TCC_INIT_LIB_H_

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SiPolicy.h>


/**
  Allocate TCC SSRAM Memory Region for L2 and L3 Requirement

  @param[in]  SiPreMemPolicyPpi         The SI Pre Mem Policy PPI instance
  @param[out] MemoryAddressL3           TCC L3 Memory Address
  @param[out] MemorySizeL3              TCC L3 Memory Size
  @param[out] MemoryAddressL2           TCC L2 Memory Address
  @param[out] MemorySizeL2              TCC L2 Memory Size
  @retval     EFI_SUCCESS          Successful got TCC Memory address and size
**/
EFI_STATUS
EFIAPI
GetTccMemoryAddressAndSize (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi,
  OUT EFI_PHYSICAL_ADDRESS    *MemoryAddressL3,
  OUT UINT32                  *MemorySizeL3,
  OUT EFI_PHYSICAL_ADDRESS    *MemoryAddressL2,
  OUT UINT32                  *MemorySizeL2
  );

/**
  Initialize TCC configuration for TCC performance

  @param[in] PeiServices          Pointer to PEI Services Table.
  @param[in] SiPolicyPpi          The SI Policy PPI instance
**/
VOID
EFIAPI
TccInit (
  IN CONST EFI_PEI_SERVICES ** PeiServices,
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );

#endif
