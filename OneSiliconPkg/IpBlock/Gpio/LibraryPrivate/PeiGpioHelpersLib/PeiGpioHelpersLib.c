/** @file
  This file contains routines for PEI GPIO Helpers Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#include <Uefi.h>
#include <Library/GpioHelpersLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/ConfigBlockLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <ConfigBlock/PchGeneralConfig.h>


/**
  Obtains GpioOverride Information from PreMem config

  @retval  TRUE    GPIO Override obtained successfully
           FALSE   Unable to obtain GPIO Override data
**/
BOOLEAN
STATIC
GetGpioOverrideFromConfigBlock (
  IN OUT  UINT8 *GpioOverride
  )
{
  EFI_STATUS                   Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;
  PCH_GENERAL_PREMEM_CONFIG    *PchGeneralPreMemConfig;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPchGeneralPreMemConfigGuid, (VOID *) &PchGeneralPreMemConfig);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  *GpioOverride = (UINT8) PchGeneralPreMemConfig->GpioOverride;

  return TRUE;
}

/**
  Returns Gpio Override Level1 Information

  @retval  TRUE/FALSE GPIO Override Level 1 Enabled/Disabled
**/
BOOLEAN
GpioOverrideLevel1Enabled (
  VOID
  )
{
  UINT8           GpioOverride;

  GpioOverride = 0;

  if (GetGpioOverrideFromConfigBlock (&GpioOverride)) {
    if (GpioOverride == 1) { return TRUE; }
  }

  return FALSE;
}
