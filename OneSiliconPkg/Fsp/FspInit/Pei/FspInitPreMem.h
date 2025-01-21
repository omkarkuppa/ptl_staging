/** @file
  Header file for FSP Init Pre-Memory PEI module

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

#ifndef _FSP_INIT_PRE_MEMORY_H_
#define _FSP_INIT_PRE_MEMORY_H_

#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/CacheAsRamLib.h>
#include <Library/MtrrLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Library/FspCommonLib.h>
#include <Library/FspPlatformLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/Reset.h>
#include <Ppi/Reset2.h>
#include <FspEas.h>
#include <FspmUpd.h>
#include <Ppi/MasterBootMode.h>
#include <Library/SiPolicyLib.h>
#include <Library/PeiMemPolicyLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PeiMePolicyLib.h>
#include <Library/PeiAmtPolicyLib.h>
#include <Library/MeChipsetLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/StallPpiLib.h>


/**
  This function will be called when MRC is done.

  @param  PeiServices General purpose services available to every PEIM.

  @param  NotifyDescriptor Information about the notify event..

  @param  Ppi The notify context.

  @retval EFI_SUCCESS If the function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

/**
  This function handles reset request inside FSP to redirect the flow to BootLoader with Reset
  required status.

  @param  PeiServices General purpose services available to every PEIM.

  @retval EFI_SUCCESS if it completed successfully.
**/
EFI_STATUS
EFIAPI
FspResetSystem (
  IN CONST EFI_PEI_SERVICES **PeiServices
  );

/**
  This function handles reset request inside FSP to redirect the flow to BootLoader with Reset
  required status.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.

**/
VOID
EFIAPI
FspResetSystem2 (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  );

/**
  This function performs PCH PEI Policy update in pre-memory.

  @param[in, out] SiPreMemPolicy  The SI PreMem Policy PPI instance
  @param[in] FspmUpd              The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiPchPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *SiPreMemPolicy,
  IN     FSPM_UPD                 *FspmUpd
  );

/**
  This function performs PCH PEI Policy update in pre-memory.

  @param[in, out] PchPreMemPolicy The PCH PreMem Policy PPI instance
  @param[in] FspmUpd              The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiAttachedPchPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *PchPreMemPolicy,
  IN     FSPM_UPD                 *FspmUpd
  );

/**
  This function performs CPU PEI Policy initialization in PreMem.

  @param[in, out] SiPreMemPolicyPpi  The Si PreMem Policy PPI instance
  @param[in]      FspmUpd            The pointer of FspmUpd

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiCpuPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi,
  IN     FSPM_UPD                   *FspmUpd
  );

/**
This function performs Security PEI Policy initialization in PreMem.

@param[in, out] SiPreMemPolicyPpi     The SI Pre-Mem Policy PPI instance
@param[in]      FspmUpd               The pointer of FspmUpd

@retval EFI_SUCCESS             The PPI is installed and initialized.
@retval EFI ERRORS              The PPI is not successfully installed.
@retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSecurityPolicyPreMem(
IN OUT  SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
IN      FSPM_UPD              *FspmUpd
);

/**
  This function performs Me PEI Policy update in pre-memory.

  @param[in out] SiPreMemPolicy    SI_PREMEM_POLICY_PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiMePolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI   *SiPreMemPolicy,
  IN     FSPM_UPD               *FspmUpd
  );

/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization in pre-memory

  @param[in out] SiPreMemPolicy    SI_PREMEM_POLICY_PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/

EFI_STATUS
FspUpdatePeiSaPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI   *SiPreMemPolicy,
  IN     FSPM_UPD               *FspmUpd
  );


/**
  This function performs Silicon PEI Policy initialization in pre-memory.

  @param[in, out] SiPreMemPolicy The Silicon Policy PPI instance
  @param[in]      FspmUpd        The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiSiPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI *SiPreMemPolicy,
  IN     FSPM_UPD             *FspmUpd
  );

/**
  This function performs FspV PEI Policy update in pre-memory.

  @param[in out] SiPreMemPolicy    SI_PREMEM_POLICY_PPI
  @param[in]     FspmUpd           The pointer of FspmUpd

  @retval EFI_SUCCESS  The function completed successfully
**/
EFI_STATUS
EFIAPI
FspUpdatePeiFspVPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI   *SiPreMemPolicy,
  IN     FSPM_UPD               *FspmUpd
  );

/**
  This function performs CPU Oveclock PEI Policy initialization in Pre-memory.

  @param[in] SiPreMemPolicyPpi     The SI Pre-Mem Policy PPI instance
  @param[in] FspmUpd               The pointer of FspmUpd

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
FspUpdatePeiOcPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  );

#endif
