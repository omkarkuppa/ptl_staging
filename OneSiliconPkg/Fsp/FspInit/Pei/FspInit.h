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

/**
  This function performs Active Management PEI Policy update.

  @param[in, out] SiPolicyPpi   The Si Policy PPI instance
  @param[in]      FspsUpd       The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiAmtPolicy (
  IN OUT SI_POLICY_PPI    *SiPolicyPpi,
  IN     FSPS_UPD         *FspsUpd
  );

/**
  This function performs CPU PEI Policy initialization.

  @param[in] SiPolicyPpi           The SI Policy PPI instance
  @param[in] FspsUpd               The pointer of FspsUpd

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiCpuPolicy (
  IN OUT SI_POLICY_PPI        *SiPolicyPpi,
  IN     FSPS_UPD             *FspsUpd
  );

/**
This function performs Security PEI Policy initialization.

@param[in] SiPolicyPpi           The SI Policy PPI instance
@param[in] FspsUpd               The pointer of FspsUpd

@retval EFI_SUCCESS              The PPI is installed and initialized.
@retval EFI ERRORS               The PPI is not successfully installed.
@retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSecurityPolicy (
  IN OUT SI_POLICY_PPI        *SiPolicyPpi,
  IN     FSPS_UPD             *FspsUpd
);

/**
  This function performs ME PEI Policy update.

  @param[in] SiPolicy       The SI Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiMePolicy (
  IN OUT SI_POLICY_PPI    *SiPolicy,
  IN     FSPS_UPD         *FspsUpd
  );

/**
  This function performs PCH PEI Policy update.

  @param[in, out] SiPolicy      The SI Policy PPI instance
  @param[in]      FspsUpd       The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiPchPolicy (
  IN OUT SI_POLICY_PPI    *SiPolicy,
  IN     FSPS_UPD         *FspsUpd
  );

/**
  This function performs PCH PEI Policy update.

  @param[in, out] PchPolicy     The PCH Policy PPI instance
  @param[in]      FspsUpd       The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiAttachedPchPolicy (
  IN OUT SI_POLICY_PPI    *PchPolicy,
  IN     FSPS_UPD         *FspsUpd
  );

/**
  This function performs SA PEI Policy update.

  @param[in out] SiPolicy          SI_POLICY_PPI PPI
  @param[in]     FspsUpd           The pointer of FspsUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
FspUpdatePeiSaPolicy (
  IN OUT SI_POLICY_PPI   *SiPolicy,
  IN     FSPS_UPD        *FspsUpd
  );

/**
  This function performs TBT PEI Policy update.

  @param[in] SiPolicy       The SI Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiTbtPolicy (
  IN OUT SI_POLICY_PPI    *SiPolicy,
  IN     FSPS_UPD         *FspsUpd
  );

/**
  This function performs Silicon PEI Policy initialization.

  @param[in] SiPolicy       The Silicon Policy PPI instance
  @param[in] FspsUpd        The pointer of FspsUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSiPolicy (
  IN OUT SI_POLICY_PPI  *SiPolicy,
  IN     FSPS_UPD       *FspsUpd
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

EFI_STATUS
EFIAPI
FspUpdatePeiFspVPolicy (
  IN OUT SI_POLICY_PPI  *SiPolicy,
  IN     FSPS_UPD       *FspsUpd
  );
#endif
