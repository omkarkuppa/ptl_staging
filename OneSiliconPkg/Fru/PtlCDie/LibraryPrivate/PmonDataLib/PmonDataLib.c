/**@file
  Library provide the interface to get the PMON (Performance Monitoring) data.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PmonLib.h>
#include <Library/CpuLib.h>
#include <Register/CpuGenInfoRegs.h>
#include <Register/CpuGenRegs.h>
#include "PmonGlobal.h"
#include "PmonUnit.h"

/**
  Create PMON Hob.

  @param[in]  SiPolicyPpi    Pointer to SI_POLICY_PPI.
  @param[out] PmonDataHobOut Pointer to the PMON Hob.

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
CreatePmonHob (
  IN  SI_POLICY_PPI       *SiPolicyPpi,
  OUT PMON_HOB_DATA       **PmonDataHobOut
  )
{
  PMON_HOB_DATA           *PmonDataHob;
  EFI_BOOT_MODE           BootMode;

  //
  // For recovery and update, no need to do PMON.
  // For S3, the memory is retained, no need to update pmon address
  //
  BootMode = GetBootModeHob();
  if ((BootMode == BOOT_ON_S3_RESUME) ||
    (BootMode == BOOT_ON_FLASH_UPDATE) ||
    (BootMode == BOOT_IN_RECOVERY_MODE)) {
    return EFI_UNSUPPORTED;
  }

  ///
  /// Create HOB for PMON Data
  ///
  PmonDataHob = (PMON_HOB_DATA *) BuildGuidHob (
                                    &gPmonDataHobGuid,
                                    sizeof (PMON_HOB_DATA)
                                    );
  if (PmonDataHob != NULL) {
    ///
    /// Initialize default HOB data
    ///
    PmonDataHob->PmonEnable = TRUE;
    *PmonDataHobOut = PmonDataHob;
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

/**
  Get the PMON global discovery.

  @param[out] Size      Size of the PMON global discovery.

  @retval               Return the pointer to the PMON global discovery.
**/
VOID*
GetPmonGlobalDiscoveryTable (
  OUT UINTN  *Size
  )
{
  if (Size == NULL) {
    return NULL;
  }
  *Size = sizeof (gPmonGlobal);
  return gPmonGlobal;
}

/**
  Get the PMON unit discovery.

  @param[out] Size      Size of the PMON global discovery.

  @retval               Return the pointer to the PMON unit discovery.
**/
VOID*
GetPmonUnitDiscoveryTable (
  OUT UINTN  *Size
  )
{
  if (Size == NULL) {
    return NULL;
  }
  *Size = sizeof (gPmonUnit);
  return gPmonUnit;
}

/**
  Check if PMON is support or not at that port.

  @param[in] PortId     Pmon port id.

  @retval               TRUE for support, FALSE for not support.
**/
BOOLEAN
GetPmonPortSupport (
  IN UINT16 PortId
  )
{
  switch (PortId) {
    case PMON_PORTID_MC0:
    case PMON_PORTID_HBO0:
    case PMON_PORTID_HBO1:
    case PMON_PORTID_NCU:
      return TRUE;

    case PMON_PORTID_MC1:
      return TRUE;
  }
  return FALSE;
}
