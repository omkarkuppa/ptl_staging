/** @file
  Implementation of Pcd/Setup data initialization in PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <SetupVariable.h>
#include <Setup.h>

/**
  Build a HOB for debug configure data.

  The DebugConfigHob is built by platform code and consumed by PeiDxeSmmDebugPrintErrorLevelLib and
  PeiDxeSmmSerialPortParameterLib library instances.

  @param[in] DebugConfigData     A pointer to the DEBUG_CONFIG_DATA.

**/
VOID
EFIAPI
BuildDebugConfigDataHob (
  IN DEBUG_CONFIG_DATA     *DebugConfigData
  )
{
  DEBUG_CONFIG_DATA_HOB    *DebugConfigDataHob;

  DebugConfigDataHob = BuildGuidHob (&gDebugConfigHobGuid, sizeof (DEBUG_CONFIG_DATA_HOB));
  ASSERT (DebugConfigDataHob != NULL);
  if (DebugConfigDataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Build Debug Config Hob failed!\n"));
    return;
  }

  DebugConfigDataHob->SerialDebugMrcLevel    = DebugConfigData->SerialDebugMrcLevel;
  DebugConfigDataHob->SerialDebug            = DebugConfigData->SerialDebug;
  DebugConfigDataHob->SerialDebugBaudRate    = DebugConfigData->SerialDebugBaudRate;
  DebugConfigDataHob->RamDebugInterface      = 0;
  DebugConfigDataHob->UartDebugInterface     = (DebugConfigData->UartDebugInterface == 1 && FeaturePcdGet (PcdSerialPortEnable)) ? 1 : 0;
  DebugConfigDataHob->Usb3DebugInterface     = (DebugConfigData->Usb3DebugInterface == 1 && FeaturePcdGet (PcdUsb3SerialStatusCodeEnable)) ? 1 : 0;
  DebugConfigDataHob->TraceHubDebugInterface = (DebugConfigData->TraceHubDebugInterface == 1) ? 1 : 0;

  DebugConfigDataHob->SerialIoDebugInterface            = (DebugConfigData->SerialIoDebugInterface == 1 && FeaturePcdGet (PcdLpssUartEnable)) ? 1 : 0;
  DebugConfigDataHob->SerialIoUartDebugControllerNumber = DebugConfigData->SerialIoUartDebugControllerNumber;
  DebugConfigDataHob->SerialIoUartDebugBaudRate         = DebugConfigData->SerialIoUartDebugBaudRate;
  DebugConfigDataHob->SerialIoUartDebugStopBits         = DebugConfigData->SerialIoUartDebugStopBits;
  DebugConfigDataHob->SerialIoUartDebugParity           = DebugConfigData->SerialIoUartDebugParity;
  DebugConfigDataHob->SerialIoUartDebugFlowControl      = DebugConfigData->SerialIoUartDebugFlowControl;
  DebugConfigDataHob->SerialIoUartDebugDataBits         = DebugConfigData->SerialIoUartDebugDataBits;
}

/**
  Update PCDs for debug configure data.

  The DebugConfigHob is built by platform code and consumed by PeiDxeSmmDebugPrintErrorLevelLib and
  PeiDxeSmmSerialPortParameterLib library instances.

  @param[in] DebugConfigData     A pointer to the DEBUG_CONFIG_DATA.

**/
VOID
EFIAPI
UpdateDebugConfigPcds (
  IN DEBUG_CONFIG_DATA     *DebugConfigData
  )
{
  PcdSetBoolS (PcdStatusCodeUseSerial, DebugConfigData->UartDebugInterface == 1 && FeaturePcdGet(PcdSerialPortEnable) ? TRUE : FALSE);

  return ;
}

/**
  Init the Setup Debug Config.

  @retval         EFI_SUCCESS    The revision for the Setup structures was updated successfully.
**/
EFI_STATUS
EFIAPI
SetupDebugConfigInit (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  SETUP_DATA                        SystemConfiguration;
#ifndef MDEPKG_NDEBUG
  DEBUG_CONFIG_DATA                 DebugConfigData;
#endif

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    return Status;
  }

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SystemConfiguration
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to get System Configuration and set the configuration to production mode!\n"));
  }

  PcdSetBoolS (PcdAcpiDebugFeatureActive, (BOOLEAN) SystemConfiguration.AcpiDebug);

#ifndef MDEPKG_NDEBUG
  VariableSize = sizeof (DEBUG_CONFIG_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"DebugConfigData",
                               &gDebugConfigVariableGuid,
                               NULL,
                               &VariableSize,
                               &DebugConfigData
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get Debug Configuration data variable!\n"));
  } else {
    BuildDebugConfigDataHob (&DebugConfigData);
    UpdateDebugConfigPcds (&DebugConfigData);
  }
#endif

  return Status;
}