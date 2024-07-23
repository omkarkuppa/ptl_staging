/** @file
  Describes the functions visible to the rest of the CpuPeim.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#ifndef _CPU_INIT_PRE_MEM_H_
#define _CPU_INIT_PRE_MEM_H_

#include <Ppi/SiPolicy.h>
#include <PiPei.h>
#include <Guid/SmramMemoryReserve.h>
#include <Library/PeiServicesLib.h>

#define MAX_OVERCLOCKING_BINS 0x7

typedef struct {
  UINT64    TmeExcludeBase;
  UINT64    TmeExcludeSize;
  UINT32    PrmrrSize;
  UINT32    TmeEnable;
  UINT8     GenerateNewTmeKey;
  BOOLEAN   TdxEnable;
  UINT32    TsegSize;
} CPU_MEMORY_INIT_CONFIG;

/**
  Check some CPU policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicy    The Silicon PreMem Policy protocol instance

**/
VOID
EFIAPI
CpuValidatePolicy (
  IN  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

/**
  This function performs basic initialization for CPU in PEI phase after Policy produced.

  @param[in] SiPreMemPolicyPpi The Si PreMem Policy PPI instance
**/
EFI_STATUS
EFIAPI
CpuOnPolicyInstalled (
  IN  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

/**
  Detect if KeyLocker is supported.

  @retval TRUE -  Processor Support KeyLocker
  @retval FALSE - Processor doesnt support KeyLocker
**/
BOOLEAN
EFIAPI
IsKeyLockerSupported (
  VOID
  );

/**
  Driver Entry for AcpiVariableHobOnSmramReservHob PEIM

  @param   FileHandle       Handle of the file being invoked.
  @param   PeiServices      Describes the list of possible PEI Services.

  @retval EFI_SUCCESS      Success create variable hob with gEfiAcpiVariableGuid and
                           split gEfiSmmSmramMemoryGuid.
  @retval EFI_NOT_FOUND    Can not get gEfiSmmSmramMemoryGuid hob

**/
EFI_STATUS
EFIAPI
CreateS3AcpiVariableHob (
  VOID
  );

/**
  Obtain Memory Init Config

  @param[in, out] CpuMemoryInitConfig - A pointer to CPU_MEMORY_INIT_CONFIG
**/
VOID
EFIAPI
CpuGetMemoryInitConfig (
  IN OUT CPU_MEMORY_INIT_CONFIG *CpuMemoryInitConfig
  );

/**
  Get PRMRR consumed memory size

  @param[in, out] CpuMemoryInitConfig - A pointer to CPU_MEMORY_INIT_CONFIG
 **/
VOID
PeiPrmrrMemoryAllocation(
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud
);
#endif
