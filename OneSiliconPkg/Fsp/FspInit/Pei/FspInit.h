/** @file
  Header file for FSP Init PEI module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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

#ifndef _FSP_INIT_PEI_H_
#define _FSP_INIT_PEI_H_

#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/FspCommonLib.h>
#include <Library/FspSwitchStackLib.h>
#include <FspsUpd.h>
#include <FspmUpd.h>
#include <Library/SiPolicyLib.h>
#include <Library/CpuPowerManagementPolicyLib.h>
#include <Library/PeiMemPolicyLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PeiMePolicyLib.h>
#include <Library/PeiAmtPolicyLib.h>
#include <Library/HobLib.h>
#include <Ppi/MpServices2.h>
#include <Ppi/SecPlatformInformation2.h>
#include <CpuRegs.h>
#include <IGpuDataHob.h>

/**
  Implementation of the FspPlatformInformation2 service in EFI_SEC_PLATFORM_INFORMATION2_PPI.

  @param  PeiServices                The pointer to the PEI Services Table.
  @param  StructureSize              The pointer to the variable describing size of the input buffer.
  @param  PlatformInformationRecord2 The pointer to the EFI_SEC_PLATFORM_INFORMATION_RECORD2.

  @retval EFI_SUCCESS                The data was successfully returned.
  @retval EFI_BUFFER_TOO_SMALL       The buffer was too small. The current buffer size needed to
                                     hold the record is returned in StructureSize.
**/
EFI_STATUS
EFIAPI
FspSecPlatformInformation2 (
  IN CONST EFI_PEI_SERVICES                   **PeiServices,
  IN OUT UINT64                               *StructureSize,
     OUT EFI_SEC_PLATFORM_INFORMATION_RECORD2 *PlatformInformationRecord2
  );

EFI_STATUS
EFIAPI
InstallMonoStatusCode (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

EFI_STATUS
EFIAPI
MpWrapperGetNumberOfProcessors (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  OUT UINTN                      *NumberOfProcessors,
  OUT UINTN                      *NumberOfEnabledProcessors
  );

EFI_STATUS
EFIAPI
MpWrapperGetProcessorInfo (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  UINTN                      ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION  *ProcessorInfoBuffer
  );

EFI_STATUS
EFIAPI
MpWrapperStartupAllAPs (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  EFI_AP_PROCEDURE           Procedure,
  IN  BOOLEAN                    SingleThread,
  IN  UINTN                      TimeoutInMicroSeconds,
  IN  VOID                       *ProcedureArgument      OPTIONAL
  );

EFI_STATUS
EFIAPI
MpWrapperStartupThisAP (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  EFI_AP_PROCEDURE           Procedure,
  IN  UINTN                      ProcessorNumber,
  IN  UINTN                      TimeoutInMicroseconds,
  IN  VOID                       *ProcedureArgument      OPTIONAL
  );

EFI_STATUS
EFIAPI
MpWrapperSwitchBSP (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  UINTN                     ProcessorNumber,
  IN  BOOLEAN                   EnableOldBSP
  );

EFI_STATUS
EFIAPI
MpWrapperEnableDisableAP (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  UINTN                      ProcessorNumber,
  IN  BOOLEAN                    EnableAP,
  IN  UINT32                     *HealthFlag OPTIONAL
  );

EFI_STATUS
EFIAPI
MpWrapperWhoAmI (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  OUT UINTN                      *ProcessorNumber
  );

EFI_STATUS
EFIAPI
MpWrapperStartupAllCPUs (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  EFI_AP_PROCEDURE           Procedure,
  IN  UINTN                      TimeoutInMicroSeconds,
  IN  VOID                       *ProcedureArgument      OPTIONAL
  );

#endif
