/** @file
  Do Platform Stage TBT initialization.

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

#include "PeiTbtPolicyUpdate.h"
#include <Core/Pei/PeiMain.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CmosAccessLib.h>
#include <CmosMap.h>
#include <Library/Usb4CmMode.h>
#include <Library/PmcLib.h>
#include <Library/PeiITbtPolicyLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/RngLib.h>
#include <Library/SiPolicyLib.h>
#include <Pi/PiPeiCis.h>
#include <TcssDataHob.h>
#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Library/PeiDTbtPolicyLib.h>
#include <Ppi/PeiDTbtPolicy.h>
#endif

#include <Platform.h>
#include <PolicyUpdateMacro.h>

#if FixedPcdGetBool (PcdTcssSupport) == 1
#include <PeiITbtConfig.h>
#endif

#if FixedPcdGet8 (PcdFspModeSelection) == 1
#include <FspsUpd.h>
#endif

/**
  UpdatePeiTbtPolicy performs TBT PEI Policy initialization

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi             The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
EFIAPI
UpdatePeiTbtPolicy (
  IN SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi,
  IN SI_POLICY_PPI          *SiPolicyPpi
  )
{
  EFI_STATUS                       Status;
  UINTN                            VarSize;
  SETUP_DATA                       SystemConfiguration;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  UINT8                            Usb4CmMode;
  UINT8                            OsCmMode;
  UINT8                            InitSetupFlag;
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  UINT8                            Index;
#endif
#if FixedPcdGetBool (PcdTcssSupport) == 1
  SA_SETUP                         SaSetup;
  PEI_ITBT_CONFIG                  *PeiITbtConfig;
#endif
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  PEI_DTBT_POLICY                  *PeiDTbtConfig;
#endif

  DEBUG ((DEBUG_INFO, "Update PeiTbtPolicyUpdate Pos-Mem Start\n"));

#if FixedPcdGetBool (PcdTcssSupport) == 1
  PeiITbtConfig = NULL;
#endif
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  PeiDTbtConfig = NULL;
#endif
  Status       = EFI_NOT_FOUND;

  //
  // Get requisite IP Config Blocks which needs to be used here
  //
#if FixedPcdGetBool (PcdTcssSupport) == 1
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPeiITbtConfigGuid, (VOID *) &PeiITbtConfig);
  ASSERT_EFI_ERROR (Status);
#endif

#if FixedPcdGetBool (PcdDTbtEnable) == 1
  Status = PeiServicesLocatePpi (
             &gPeiDTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiDTbtConfig
             );
#endif

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  VarSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SystemConfiguration
                               );

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  //
  // USB4_SETUP_CM_MODE_FW : TBT FW should be set to Firmware CM mode. Connect Topology command should be sent.
  // USB4_SETUP_CM_MODE_SW : TBT FW should be set to Pass Through mode and Software CM should be executed.
  // USB4_SETUP_CM_MODE_OS : CM mode saved in platform settings should be applied.
  // USB4_SETUP_CM_MODE_DEBUG : TBT FW should be set to Pass Through mode without Software CM execution.
  //
  switch (SystemConfiguration.Usb4CmMode) {
    case USB4_SETUP_CM_MODE_FW:
      Usb4CmMode = USB4_CM_MODE_FW_CM;
      break;
    //
    // CMOS will only be used when the SystemConfiguration.Usb4CmMode is USB4_SETUP_CM_MODE_OS
    //
    case USB4_SETUP_CM_MODE_OS:
      VarSize = sizeof (InitSetupFlag);
      Status = VariableServices->GetVariable (
                                    VariableServices,
                                    L"InitSetupVariable",
                                    &gSetupVariableGuid,
                                    NULL,
                                    &VarSize,
                                    &InitSetupFlag);
      if (Status == EFI_NOT_FOUND || (VarSize != sizeof (InitSetupFlag))) {
        DEBUG ((DEBUG_INFO, "First boot, Init SW CM CMOS\n"));
        CmosWrite8 (CMOS_USB4_CM_MODE_REG, USB4_CM_MODE_SW_CM);
      } else if (!PmcIsRtcBatteryGood ()) {
        //
        // CMOS content may be cleared by RTC power failure, reset CM mode value to SW CM if RTC power failure is detected
        //
        DEBUG ((DEBUG_INFO, "RTC Power failure detected!!\n"));
        OsCmMode = CmosRead8 (CMOS_USB4_CM_MODE_REG);
        DEBUG ((DEBUG_INFO, "CMOS CM mode value = %d, reset to SW CM mode", OsCmMode));
        CmosWrite8 (CMOS_USB4_CM_MODE_REG, USB4_CM_MODE_SW_CM);
      }

      OsCmMode = CmosRead8 (CMOS_USB4_CM_MODE_REG);
      if ((OsCmMode != USB4_CM_MODE_SW_CM) && (OsCmMode != USB4_CM_MODE_FW_CM)) {
        DEBUG ((DEBUG_ERROR, "Unsupported OS CM mode 0x%X, default set to SW CM mode\n", OsCmMode));
        Usb4CmMode = USB4_CM_MODE_SW_CM;
      } else {
        Usb4CmMode = OsCmMode;
      }
      break;
    case USB4_SETUP_CM_MODE_SW:
    case USB4_SETUP_CM_MODE_DEBUG:
    default:
      Usb4CmMode = USB4_CM_MODE_SW_CM;
      break;
  }

  DEBUG ((DEBUG_INFO, "Usb4CmMode setup option = 0x%0x\n", SystemConfiguration.Usb4CmMode));
  DEBUG ((DEBUG_INFO, "USB4 CM mode is 0x%X\n", Usb4CmMode));

#if FixedPcdGetBool (PcdTcssSupport) == 1
  VarSize = sizeof (SA_SETUP);
  Status = VariableServices->GetVariable (
                                VariableServices,
                                L"SaSetup",
                                &gSaSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &SaSetup
                                );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Get SaSetup variable failure, %r\n", Status));
    return Status;
  }

  COMPARE_AND_UPDATE_POLICY (PeiITbtConfig->ITbtGenericConfig.Usb4CmMode,                     Usb4CmMode                                        );
  COMPARE_AND_UPDATE_POLICY (PeiITbtConfig->ITbtGenericConfig.ITbtForcePowerOnTimeoutInMs,    SystemConfiguration.ITbtForcePowerOnTimeoutInMs   );
  COMPARE_AND_UPDATE_POLICY (PeiITbtConfig->ITbtGenericConfig.ITbtConnectTopologyTimeoutInMs, SystemConfiguration.ITbtConnectTopologyTimeoutInMs);
  COMPARE_AND_UPDATE_POLICY (PeiITbtConfig->ITbtGenericConfig.ITbtPcieTunnelingForUsb4,       SystemConfiguration.EnablePcieTunnelingOverUsb4   );
#endif
#if FixedPcdGetBool (PcdDTbtEnable) == 1
  //
  // Update DTBT Policy
  //
  if (PeiDTbtConfig != NULL) {
    for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
      PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn  = SystemConfiguration.DTbtController[Index];
    }
    //
    // Update USB4 CM mode if CM mode switch is supported by platform
    //
    if (!(PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode & USB4_CM_MODE_SWITCH_UNSUPPORTED)) {
      DEBUG ((DEBUG_INFO, "DTBT CM mode switch is supported, Usb4CmMode = %x\n", Usb4CmMode));
      PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode = Usb4CmMode;
    } else {
      Usb4CmMode = PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode & 0x07;
      DEBUG ((DEBUG_INFO, "DTBT CM mode switch is not supported, Usb4CmMode = %x\n", Usb4CmMode));
    }
    //
    // USB4 class option
    //
    PeiDTbtConfig->DTbtGenericConfig.Usb4ClassOption = SystemConfiguration.Usb4ClassOption;

    //
    // Update TBT RTD3 PEP enable
    //
    if (SystemConfiguration.Rtd3Support && SystemConfiguration.DiscreteTbtSupport && SystemConfiguration.DTbtRtd3 &&
        SystemConfiguration.PepTcss && SystemConfiguration.LowPowerS0Idle) {
      PeiDTbtConfig->DTbtGenericConfig.TbtRtd3PepEnable = 1;
    } else {
      PeiDTbtConfig->DTbtGenericConfig.TbtRtd3PepEnable = 0;
    }
  }
  DTbtPrintPeiPolicyConfig ();
#endif

  return EFI_SUCCESS;
}
