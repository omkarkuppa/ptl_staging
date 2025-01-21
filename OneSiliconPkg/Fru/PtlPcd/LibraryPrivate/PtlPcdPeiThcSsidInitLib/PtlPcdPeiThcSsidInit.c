/** @file
  The PEI THC Init Library Implements After Memory PEIM

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <ThcConfig.h>
#include <Library/PeiServicesLib.h>
#include <Register/PchRegs.h>
#include <Library/PchLimits.h>
#include <Library/PchPciBdfLib.h>

STATIC SVID_SID_SKIP_TABLE mThcSsidTable [PCH_MAX_THC_CONTROLLERS];

/**
  Update Thc Ssid Table

  @retval EFI_SUCCESS
**/

VOID
PrepareThcSsidTable (
  VOID
  )
{
  UINT8   ThcIndex;

  for (ThcIndex = 0; ThcIndex < GetPchMaxThcControllersNum (); ThcIndex++) {
    mThcSsidTable[ThcIndex].SkipBus             = DEFAULT_PCI_BUS_NUMBER_PCH;
    mThcSsidTable[ThcIndex].SkipDevice          = ThcDevNumber (ThcIndex);
    mThcSsidTable[ThcIndex].SkipFunction        = ThcFuncNumber (ThcIndex);
  }
}


/**
  Installs THC SSID data hob

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
InstallThcSsidDataHob (
  VOID
  )
{

  EFI_STATUS                  Status;
  THC_DATA_SSID_CONFIG_HOB    *ThcSsidHob;

  //
  // Create HOB for THC SSID Data
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (THC_DATA_SSID_CONFIG_HOB),
             (VOID **) &ThcSsidHob
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: PeiServicesCreateHob Status- %r \n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Initialize default HOB data
  //
  ThcSsidHob->EfiHobGuidType.Name = gThcSsidHobGuid;
  ThcSsidHob->ThcSsidTable = mThcSsidTable;
  ThcSsidHob->NumberOfThcSsidTable = sizeof (mThcSsidTable) / sizeof (mThcSsidTable[0]);
  DEBUG ((DEBUG_INFO, "ThcSsidHob->EfiHobGuidType.Name: %g\n", &ThcSsidHob->EfiHobGuidType.Name));
  DEBUG ((DEBUG_INFO, "&(ThcHob->ThcSsidTable) @ %X\n", &(ThcSsidHob->ThcSsidTable)));
  DEBUG ((DEBUG_INFO, "ThcSsidHob->NumberOfThcSsidTable: %X\n", ThcSsidHob->NumberOfThcSsidTable));

  return Status;
}

/**
  This function performs basic initialization for THC in PEI phase after Policy produced at Pre-Mem phase.

  @param[in]  ThcConfig              Pointer to THC config block
  @param[in]  HostBridgePreMemConfig  Pointer to Host Bridge config block
**/
VOID
EFIAPI
ThcInstallSubSystemIdHob (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  EFI_STATUS      Status;
  THC_CONFIG      *ThcConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gThcConfigGuid, (VOID *) &ThcConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: GetConfigBlock Status- %r \n", __FUNCTION__, Status));
    return;
  }

    //
    // Prepare THC SSID table and install SSID hob only if THC0 instance is available
    //
  if (ThcConfig->ThcPort[0].Assignment > ThcAssignmentNone) {
    //
    // Install THC SSID data hob
    //
    PrepareThcSsidTable ();

    Status = InstallThcSsidDataHob ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: InstallThcSsidDataHob Status- %r \n", __FUNCTION__, Status));
      return;
    }
  }
}

