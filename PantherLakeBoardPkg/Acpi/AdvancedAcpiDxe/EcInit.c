/** @file
  EC driver for initiation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/EcMiscLib.h>
#include <SetupVariable.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/EcNvsArea.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/PcdEspiLib.h>
#include <Library/GbeLib.h>

#include "AcpiPlatform.h"
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
#include <DptfConfig.h>
#endif

GLOBAL_REMOVE_IF_UNREFERENCED EC_NVS_AREA_PROTOCOL                mEcNvsAreaProtocol;

/**
  This function will provide the EC GPE number for ECDT update according to platform design

  @retval EcGpeNumber     EC GPE number to be reported in ECDT
**/
static
UINT32
GetEcGpeNumber (
  VOID
  )
{
  UINT32                                EcGpeNumber;
  EFI_STATUS                            Status;

  EcGpeNumber = 0;
  //
  // EC GPE number assignment should be same as Method _GPE under ACPI EC Device which is used for OS without ECDT support
  //
  if (PtlPcdIsEspiEnabled ()) {
    EcGpeNumber = 0x6E;
  } else {
    EcGpeNumber = 23;
    if (PcdGet32 (PcdSmcRuntimeSciPin) != 0) {
      Status = GpioV2GetGpeNumber (PcdGet32 (PcdSmcRuntimeSciPin), &EcGpeNumber);
      DEBUG ((DEBUG_INFO, "Get EcGpeNumber for Runtime SCI Pin - %r\n", Status));
    }
  }

  DEBUG ((DEBUG_INFO, "ACPI EcGpeNumber = 0x%x\n ", EcGpeNumber));

  return EcGpeNumber;
}

/**
  Install the gEcNvsAreaProtocolGuid and update the content of it.

**/
static
EFI_STATUS
EcInstallNvs (
  IN  SETUP_DATA          *SetupDataPtr
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  UINT8                         PG3StateCounter;
  UINT32                        IsctSetupDataAttr;
  UINTN                         VarDataSize;

  Pages = EFI_SIZE_TO_PAGES (sizeof (EC_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.
  PG3StateCounter = 0;

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  mEcNvsAreaProtocol.Area = (VOID *) (UINTN) Address;
  SetMem (mEcNvsAreaProtocol.Area, sizeof (EC_NVS_AREA), 0);

  mEcNvsAreaProtocol.Area->IuerButtonEnable       = SetupDataPtr->IuerButtonEnable;
  mEcNvsAreaProtocol.Area->IuerConvertibleEnable  = SetupDataPtr->IuerConvertibleEnable;
  mEcNvsAreaProtocol.Area->IuerDockEnable         = SetupDataPtr->IuerDockEnable;
  mEcNvsAreaProtocol.Area->CSNotifyEC             = SetupDataPtr->CSNotifyEC;
  mEcNvsAreaProtocol.Area->EcLowPowerMode         = SetupDataPtr->EcLowPowerMode;

  mEcNvsAreaProtocol.Area->PcdBatterySupport                    = PcdGet8 (PcdBatterySupport);
  mEcNvsAreaProtocol.Area->PcdEcHotKeyF3Support                 = PcdGet8 (PcdEcHotKeyF3Support);
  mEcNvsAreaProtocol.Area->PcdEcHotKeyF4Support                 = PcdGet8 (PcdEcHotKeyF4Support);
  mEcNvsAreaProtocol.Area->PcdEcHotKeyF5Support                 = PcdGet8 (PcdEcHotKeyF5Support);
  mEcNvsAreaProtocol.Area->PcdEcHotKeyF6Support                 = PcdGet8 (PcdEcHotKeyF6Support);
  mEcNvsAreaProtocol.Area->PcdEcHotKeyF7Support                 = PcdGet8 (PcdEcHotKeyF7Support);
  mEcNvsAreaProtocol.Area->PcdEcHotKeyF8Support                 = PcdGet8 (PcdEcHotKeyF8Support);
  mEcNvsAreaProtocol.Area->PcdVirtualButtonVolumeUpSupport      = PcdGetBool (PcdVirtualButtonVolumeUpSupport);
  mEcNvsAreaProtocol.Area->PcdVirtualButtonVolumeDownSupport    = PcdGetBool (PcdVirtualButtonVolumeDownSupport);
  mEcNvsAreaProtocol.Area->PcdVirtualButtonHomeButtonSupport    = PcdGetBool (PcdVirtualButtonHomeButtonSupport);
  mEcNvsAreaProtocol.Area->PcdVirtualButtonRotationLockSupport  = PcdGetBool (PcdVirtualButtonRotationLockSupport);
  mEcNvsAreaProtocol.Area->PcdSlateModeSwitchSupport            = PcdGetBool (PcdSlateModeSwitchSupport);
  mEcNvsAreaProtocol.Area->PcdAcDcAutoSwitchSupport             = PcdGetBool (PcdAcDcAutoSwitchSupport);
  mEcNvsAreaProtocol.Area->PcdPmPowerButtonGpioPin              = PcdGet32 (PcdPmPowerButtonGpioPin);

  mEcNvsAreaProtocol.Area->EcdtGpeNumber                        = GetEcGpeNumber ();

  IsctSetupDataAttr = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
  VarDataSize = sizeof(UINT8);
  //
  // PG3StateCounter variable will be create by tools(like XmlCli).
  //
  Status = gRT->GetVariable (
                  L"PseudoG3StateCounter",
                  &gUefiIntelPlatformVariablesGuid,
                  &IsctSetupDataAttr,
                  &VarDataSize,
                  &PG3StateCounter
                  );
  if (Status == EFI_SUCCESS) {
    DEBUG((DEBUG_INFO, "ISCT virtual device enable.\n"));
    mEcNvsAreaProtocol.Area->PseudoG3StateCounter = PG3StateCounter;
  } else {
    DEBUG((DEBUG_INFO, "ISCT virtual device disable.\n"));
    mEcNvsAreaProtocol.Area->PseudoG3StateCounter = 0x00;
  }

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEcNvsAreaProtocolGuid,
                  &mEcNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Initilize the EC base on Setup, SaSetup, CpuSetup value.
  And update platform Setup base on the data report from EC.

  @param[in]  CpuSetupDataPtr     Pointer point to CpuSetup.

  @retval     EFI_SUCCESS         Initialize EC successfully.
  @retval     others              Fail to do EC initialization.
**/
EFI_STATUS
EFIAPI
EcInitialize (
  IN  CPU_SETUP           *CpuSetupDataPtr
  )
{
  EFI_STATUS              Status;
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  UINT8                   DataBuffer[2];
  UINT8                   Index;
  UINT8                   SensorData;
  UINT8                   FanData;
#endif
  UINTN                   VariableSize;
  UINT32                  SetupVariableAttributes;
  SETUP_DATA              SetupData;
#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  UINT32                  gDptfVariableAttributes;
  UINT8                   PowerSource;
  DPTF_CONFIG             DptfSetupData;
#endif
  UINT8                   PG3;

  Status = EFI_SUCCESS;

  EnableEcAcpiMode (FALSE);     // SMC_DISABLE_ACPI_MODE
  EcEnablePowerButton ();       // SMC_ENABLE_POWER_SWITCH
  EnableEcSmiMode (TRUE);       // SMC_SMI_ENABLE

  VariableSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &SetupVariableAttributes,
                  &VariableSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
  VariableSize = sizeof (DPTF_CONFIG);
  Status = gRT->GetVariable (
                  L"DptfConfig",
                  &gDptfConfigVariableGuid,
                  &gDptfVariableAttributes,
                  &VariableSize,
                  &DptfSetupData
                  );
#endif

  if (!EFI_ERROR (Status) && CpuSetupDataPtr != NULL) {
    SetEcCriticalShutdownTemperature (SetupData.CriticalThermalTripPoint);

#if FixedPcdGetBool (PcdDptfFeatureEnable) == 1
    //
    // Detect DTT EC based devices
    //
    Status = DetectDttParticipants((UINT8 *)DataBuffer);

    if (!EFI_ERROR (Status)) {
      FanData= DataBuffer[0];
      SensorData = DataBuffer[1];
      DEBUG ((DEBUG_INFO, "EC Fan data %x\n", FanData));
      DEBUG ((DEBUG_INFO, "EC Sensor data %x\n", SensorData));
      // Special Condition. CPU Fan 1 is enabled by default in PlatformSetup.hfr. Need to disable it, if Fan 1 is not detected by the EC
      if(!(FanData & 0x01)) {
        DptfSetupData.EnableFan1Device = 0;
      }
    } else {
      FanData = 0x01; // Default CPU Fan
      SensorData = 0x1E; // Default Sensor 2-5
    }

    //
    // Suppress fan participants in DTT Setup menu which weren't detected by EC
    //
    for(Index=0; Index < MAX_EC_FANS; Index++) {
      if(FanData & (0x01<<Index)) {
        SetupData.DetectEcFan[Index] = TRUE;
        DEBUG ((DEBUG_INFO, "EC Fan %d detected\n", Index+1));
      } else {
        SetupData.DetectEcFan[Index] = FALSE;
        DEBUG ((DEBUG_INFO, "EC Fan %d supressed\n", Index+1));
      }
    }

    //
    // Suppress sensor participants in DTT Setup menu which weren't detected by EC
    //
    for(Index=0; Index < MAX_EC_SENSORS; Index++) {
      if(SensorData & (0x01<<Index)) {
        SetupData.DetectEcSensors[Index] = TRUE;
        DEBUG ((DEBUG_INFO, "EC Sensor %d detected\n", Index+1));
      } else {
        SetupData.DetectEcSensors[Index] = FALSE;
        DEBUG ((DEBUG_INFO, "EC Sensor %d supressed\n", Index+1));
      }
    }

    //
    // Update the DPTF PPS OEM variable 1 (represent the PPS adaptor plugin status)
    // 1 - PPS adaptor is plugin
    // 0 - PPS adaptor is unpluged
    //
    Status = EcGetPowerSource(&PowerSource);

    if(!EFI_ERROR(Status)) {
      if((PowerSource & BIT2) == BIT2) {
        DptfSetupData.OemDesignVariable1 = 1;
      } else {
        DptfSetupData.OemDesignVariable1 = 0;
      }
    }

    VariableSize = sizeof (DPTF_CONFIG);
    Status = gRT->SetVariable (
                    L"DptfConfig",
                    &gDptfConfigVariableGuid,
                    gDptfVariableAttributes,
                    VariableSize,
                    &DptfSetupData
                    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR (Status) && !(FanData & 0x01)) {
      DEBUG ((DEBUG_ERROR, "Unable to disable CPU FAN enable in DPTF Config. Status [%r]\n", Status));
    }
#endif

    VariableSize = sizeof (SETUP_DATA);
    Status = gRT->SetVariable (
                    L"Setup",
                    &gSetupVariableGuid,
                    SetupVariableAttributes,
                    VariableSize,
                    &SetupData
                    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Unable to Set Fan/Sensor suppress information in setup menu. Status [%r]\n", Status));
    }

    SetEcLowPowerMode (
      SetupData.LowPowerS0Idle,
      SetupData.EcLowPowerMode,
      SetupData.CSNotifyEC,
      SetupData.EcDebugLed
      );

    if (SetupData.StcPeciInterfaceEnable == 1) {
      SetSkinThermalControlConfig (
        0,
        SetupData.StcTargetTemp[0],
        SetupData.StcEnable[0],
        SetupData.StcGain[0],
        SetupData.StcMinPerfLevel[0]
        );

      SetSkinThermalControlConfig (
        1,
        SetupData.StcTargetTemp[1],
        SetupData.StcEnable[1],
        SetupData.StcGain[1],
        SetupData.StcMinPerfLevel[1]
        );

      SetSkinThermalControlConfig (
        2,
        SetupData.StcTargetTemp[2],
        SetupData.StcEnable[2],
        SetupData.StcGain[2],
        SetupData.StcMinPerfLevel[2]
        );
    }

    PG3 = SetupData.PseudoG3State;
    // Program EC based on Pseudo G3 State
    EcUpdatePG3 (PG3);

    Status = EcInstallNvs (&SetupData);
  }
  return  Status;
}
