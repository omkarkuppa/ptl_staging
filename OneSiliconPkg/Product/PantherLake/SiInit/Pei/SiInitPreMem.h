/** @file
  Header file for Silicon Init Pre Memory module.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _SI_INIT_PRE_MEM_MODULE_H_
#define _SI_INIT_PRE_MEM_MODULE_H_

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/TempRamExitPpi.h>
#include <Library/PeiExtGfxCardDetectLib.h>
#include <Library/TxtPeiLib.h>
#include <Library/MeInitLib.h>
#include <CMemoryInitLib.h>
#include <Library/CpuPowerOnConfigLib.h>
#include <Library/PsmiInitLib.h>
extern EFI_GUID gEdkiiMigratedFvInfoGuid;

/**
  PPI function to print Silicon PreMem Policy settings.

  @retval EFI_STATUS       - Locate SiPreMemPolicy PPI successfully.
  @retval others           - Locate SiPreMemPolicy PPI fail.
**/
EFI_STATUS
EFIAPI
PeiPreMemSiPolicyPrint (
  VOID
  );

/**
  Slicon Initializes after Policy PPI produced, All required polices must be installed before the callback

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             Succeeds.
**/
EFI_STATUS
EFIAPI
SiInitPreMemOnPolicy (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  );

/**
  Migration memory address
**/
EFI_STATUS
EFIAPI
ResetMemoryAddressOneSilicon (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN  VOID                         *Ppi
  );

/**
  Program MTRR values and print MTRRs.
**/
EFI_STATUS
EFIAPI
TempRamExit (
  IN  VOID    *TempRamExitParamPtr
  );


/**
  This function disable CAR.

  @param[in] DisableCar       TRUE means use INVD, FALSE means use WBINVD

**/
VOID
EFIAPI
DisableCacheAsRam (
  IN BOOLEAN                   DisableCar
  );

/**
  Check some SA policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicyPpi    The SI PreMem Policy instance

**/
VOID
SaValidatePolicy (
  IN  SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  );
#endif // _SI_INIT_PRE_MEM_MODULE_H_
