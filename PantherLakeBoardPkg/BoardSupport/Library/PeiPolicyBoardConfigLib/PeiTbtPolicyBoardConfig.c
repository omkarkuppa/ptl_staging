/** @file
 Intel PEI TBT Policy update by board configuration

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
#include "PeiPolicyBoardConfig.h"
#include <PlatformBoardId.h>
#include <Library/PmcLib.h>
#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Library/Usb4CmMode.h>
#include <Ppi/PeiDTbtPolicy.h>
#include <DTbtBoardConfigPcd.h>
#endif

#if FixedPcdGetBool (PcdDTbtEnable) == 1
/**
  Initialize few dTBT Feature Package PCD from VPD Pcd.

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval EFI_INVALID_PARAMETER   Function received DTBT PEI policy or Feature PCD incorrectly.
**/
EFI_STATUS
UpdatePeiTbtPolicyFromPcd (
  IN OUT PEI_DTBT_POLICY        *PeiDTbtConfig
  )
{
  UINT8                         Index;
  DTBT_BOARD_CONFIG_PCD         *DtbtBoardConfigTable;

  //
  // Get dTBT Board Config Data from VPD and Save it to dTBT Common Package Pcd
  //
  DtbtBoardConfigTable = (DTBT_BOARD_CONFIG_PCD *) PcdGetPtr (VpdPcdDTbtBoardConfig);
  if (DtbtBoardConfigTable == NULL) {
    DEBUG ((DEBUG_ERROR, "%a get DTBT structural PCD pointer fail\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (PcdGet8 (PcdBoardDTbtControllerNumber) > MAX_DTBT_CONTROLLER_NUMBER) {
    DEBUG ((DEBUG_ERROR, "Max number of DTBT controller that platform support (%d) is more than %d\n", PcdGet8 (PcdBoardDTbtControllerNumber), MAX_DTBT_CONTROLLER_NUMBER));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Print DTBT Board Config Policy
  //
  DEBUG ((DEBUG_INFO, "\n ===================== dTBT Vpd PCD Board Config Info ===================== \n"));
  DEBUG ((DEBUG_INFO, "[dTBT] Total Available dTBT Controller Number = %d\n", PcdGet8 (PcdBoardDTbtControllerNumber)));
  for (Index = 0; (Index < PcdGet8 (PcdBoardDTbtControllerNumber)) && (Index < MAX_DTBT_CONTROLLER_NUMBER); Index++) {
    if (DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtBoardSupport == FALSE) {
      DEBUG ((DEBUG_ERROR, "[dTBT] Controller[%d] is not supported from Board or Hardware side \n", Index));
      PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn = 0;
      continue;
    }
    PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn = 1;
    PeiDTbtConfig->DTbtControllerConfig[Index].RpType           = DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpType;
    PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpNumber     = DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpSocketNumber;
    PeiDTbtConfig->DTbtControllerConfig[Index].WakeGpioPin      = DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtWakeGpioPin;
    PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpMps        = DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpMps;

    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] Board or HW Support = %d\n",   Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtBoardSupport));
    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] PcieRpType          = %d\n",   Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpType));
    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] PcieRpSocketNumber  = %d\n",   Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpSocketNumber));
    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] PcieRpSegNumber     = %d\n",   Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpSegNumber));
    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] PcieRpBusNumber     = %d\n",   Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpBusNumber));
    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] PcieRpNumber        = %d\n",   Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpNumber));
    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] WakeGpioPin         = 0x%X\n", Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtWakeGpioPin));
    DEBUG ((DEBUG_INFO, "[dTBT] Controller[%d] PcieRpMps           = 0x%X\n", Index, DtbtBoardConfigTable->DTbtBoardConfig[Index].DTbtPcieRpMps));
  }
  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));

  return EFI_SUCCESS;
}
#endif

/**
  This function performs PEI TBT Policy update by board configuration.
  RP number and type already update in Pre-Mem phase, here only update
  Usb4CmMode, PcieRpNumber and RpAcpiNumber according to BoardId

  @retval EFI_SUCCESS             The Policy is successfully updated.
  @retval Others                  The Policy is not successfully updated.
**/
EFI_STATUS
EFIAPI
UpdatePeiTbtPolicyBoardConfig (
  VOID
  )
{
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  EFI_STATUS                Status;
  UINT16                    BoardId;
  PEI_DTBT_POLICY           *PeiDTbtConfig;

  DEBUG ((DEBUG_INFO, "Updating TBT Policy by board config in Post-Mem\n"));

  Status        = EFI_SUCCESS;
  BoardId       = PcdGet16(PcdBoardId);
  PeiDTbtConfig = NULL;

  Status = PeiServicesLocatePpi (
             &gPeiDTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiDTbtConfig
             );

  if (!EFI_ERROR (Status)) {
    //
    // Set DTBT USB4 CM mode to SW CM and can't be switch
    //
    PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode = USB4_CM_MODE_SW_CM | USB4_CM_MODE_SWITCH_UNSUPPORTED;

    //
    // Update DTBT config from platform VPD PCD
    //
    Status = UpdatePeiTbtPolicyFromPcd (PeiDTbtConfig);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a - update DTBT config from platform VPD PCD fail (%r)\n", __FUNCTION__, Status));
    }

    switch (BoardId) {
      default:
        DEBUG ((DEBUG_INFO, "%a - Unhandled BoardId 0x%04X\n", __FUNCTION__, BoardId));
        break;
    }
  } else {
    DEBUG ((DEBUG_ERROR, " gPeiDTbtPolicyPpiGuid Not installed!!!\n"));
  }

  return Status;
#else
  return EFI_SUCCESS;
#endif
}
