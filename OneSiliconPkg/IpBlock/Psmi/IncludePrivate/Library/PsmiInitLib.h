/** @file
  Header file for PSMI Init Lib.

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
#ifndef _PSMI_INIT_LIB_H_
#define _PSMI_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <PsmiDataHob.h>
#include <CMrcInterface.h>

/**
  Enable TraceHub BAR based on PSMI support

  @param[in] IsTraceHubInit     TRUE if Trace Hub has already been initialized

  @retval EFI_SUCCESS           MTB BAR enabled successfully
  @retval EFI_UNSUPPORTED       Trace Hub doesn't exist
**/
EFI_STATUS
EnableTraceHubMtbBar (
  IN BOOLEAN    IsTraceHubInit
  );

/**
  Initializes PSMI Handler and Trace memory size

  @param[in] SiPreMemPolicyPpi       - The Silicon PreMem Policy PPI instance

  @retval EFI_SUCCESS           PsmiInit Function executed successfully
  @retval EFI_UNSUPPORTED       PsmiInit failed to execute
**/
EFI_STATUS
PsmiInit (
  IN    SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
  Configures PSMI Handler/Trace Memory

  @retval EFI_SUCCESS           ConfigurePsmi Function executed successfully
  @retval EFI_UNSUPPORTED       ConfigurePsmi failed to execute
**/
EFI_STATUS
ConfigurePsmi (
  VOID
  );

/**
  Configures PSMI Handler Base

  @retval EFI_SUCCESS           ConfigurePsmi Function executed successfully
  @retval EFI_UNSUPPORTED       ConfigurePsmi failed to execute
**/
EFI_STATUS
ConfigurePsmiHandler (
  VOID
  );

/**
  Init and Install PSMI Hob Ver1
  @param[in] SiPreMemPolicyPpi       - The Silicon PreMem Policy PPI instance
  @param[out] PSMI_DATA_HOB_VER1     - PSMI_DATA_HOB instance installed by this function

  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallPsmiHobVer1 (
  IN    SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi,
  OUT   PSMI_DATA_HOB_VER1        **PsmiDataHobOut
  );

/**
  Init and Install PSMI Hob Ver2
  @param[in] SiPreMemPolicyPpi       - The Silicon PreMem Policy PPI instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallPsmiHobVer2 (
  IN    SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  );

/**
  Check if Trace Hub has already been initialized

  @retval  TRUE   - if Trace Hub has already been initialized
           FALSE  - if Trace Hub has not been initialized

**/
BOOLEAN
CheckTraceHubInit (
  VOID
 );

/**
  Update PSMI Handler Base

  @param[in] BaseAddress  - Base Address of PSMI Hanlder

**/
VOID
EFIAPI
UpdatePsmiHandlerBaseAddress (
  IN UINT32     BaseAddress
  );

/**
  Reserve PSMI Handler Memory below 4G.

  @param[in] LowestBase          - Moving address pointer in MB
**/
EFI_STATUS
EFIAPI
ConfigurePsmiHandlerBelow4G (
  IN  UINT32         *LowestBase
  );

/**
  Reserve PSMI Handler Memory above 4G.

  @param[in] TopUseableMemAddr          - Moving address pointer
  @param[in] Touud                      - Size leftover pointer
**/
VOID
EFIAPI
ConfigurePsmiHandlerAbove4G (
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud
  );

/**
  Reserve PSMI Tarce Region Memory Above 4GB.
  @param[in] TopUseableMemAddr          - Moving address pointer
  @param[in] TopMemSize                 - Size leftover pointer

  @retval EFI_SUCCESS                  - GT Memory Allocation Success
**/
VOID
EFIAPI
ConfigurePsmiTraceRegion (
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud
  );

/**
  Reserve PSMI Memory Above 4GB.

  @param[in] TopUseableMemAddr          - Moving address pointer
  @param[in] TopMemSize                 - Size leftover pointer
  @param[in] ResourceAttributeTested    - Resource attribute flag
**/
VOID
EFIAPI
PeiPsmiMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE       ResourceAttributeTested
  );

/**
  Reserve PSMI Memory Above 4GB.

  @param[in] HandlerBaseAbove4G          - PSMI Handler Base Above 4G
  @param[in] HandlerSize                 - PSMI Handler Size
  @param[in] Region0Base                 - PSMI Trace Region 0 Base Address
  @param[in] Trace0Size                  - PSMI Trace Region 0 Size
  @param[in] Region1Base                 - PSMI Trace Region 1 Base Address
  @param[in] Trace2Size                  - PSMI Trace Region 1 Size
  @param[in] Region2Base                 - PSMI Trace Region 2 Base Address
  @param[in] Trace2Size                  - PSMI Trace Region 2 Size
  @param[in] ResourceAttributeTested    - Resource attribute flag
**/
VOID
EFIAPI
PeiPsmiMemoryAllocationV2 (
  IN EFI_PHYSICAL_ADDRESS          HandlerBaseAbove4G,
  IN UINT64                        HandlerSize,
  IN EFI_PHYSICAL_ADDRESS          Region0Base,
  IN UINT64                        Trace0Size,
  IN EFI_PHYSICAL_ADDRESS          Region1Base,
  IN UINT64                        Trace1Size,
  IN EFI_PHYSICAL_ADDRESS          Region2Base,
  IN UINT64                        Trace2Size,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributeTested
  );
#endif
