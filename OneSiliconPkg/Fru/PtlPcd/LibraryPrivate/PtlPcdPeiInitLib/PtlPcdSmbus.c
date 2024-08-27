/** @file
  PCH SMBUS init.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include "PtlPcdInitPei.h"
#include <Library/PeiItssLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiSmbusLib.h>
#include <Library/PcdInfoLib.h>
#include <Register/PchRegs.h>
#include <Register/SmbusRegs.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <GpioV2Services.h>
#include <Register/GpioAcpiDefines.h>
#include <PcdSbPortIds.h>

// Number of SMBUS controllers
#define MAX_SMBUS_CONTROLLER       1

// Number of ports per SATA controller
#define MAX_SMBUS_PORT             1

// Smbus Controller Index
#define SMBUS_1_CONTROLLER_INDEX   0

/**
Get Maximum Smbus Controller Number

@retval Maximum Smbus Controller Number
**/
UINT8
MaxSmbusPortNum (
  VOID
)
{
  //
  // Defined in SoC integration header
  //
  return MAX_SMBUS_PORT;
}

/**
Get Maximum Smbus Controller Number

@retval Maximum Smbus Controller Number
**/
UINT8
MaxSmbusControllerNum (
  VOID
)
{
  //
  // Defined in SoC integration header
  //
  return MAX_SMBUS_CONTROLLER;
}

/**
This Function initializes SMBUS Controller

@param[in]  SmbusCtrlIndex        SMBUS controller index
@param[out] SmbusController       Pointer to SmbusController structure

@retval     EFI_SUCCESS           - Completted successfully
            EFI_INVALID_PARAMETER - Smbusontroller NULL or SmbusCtrlIndex out of the range
**/
EFI_STATUS
SmbusGetController (
  IN     UINT32             SmbusCtrlIndex,
  IN OUT SMBUS_CONTROLLER   *SmbusController
  )
{
  if (SmbusController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Null pointer at SmbusHandle \n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  if (SmbusCtrlIndex >= MaxSmbusControllerNum ()) {
    DEBUG ((DEBUG_ERROR, "%a: Controller number out of the range!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  SmbusController->SmbusCtrlIndex   = SmbusCtrlIndex;
  SmbusController->Segment          = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  SmbusController->Bus              = DEFAULT_PCI_BUS_NUMBER_PCH;
  SmbusController->Device           = SmbusDevNumber ();
  SmbusController->Function         = SmbusFuncNumber ();
  SmbusController->PciCfgBase       = SmbusPciCfgBase ();
  SmbusController->TotalCtrlPortNum = MaxSmbusPortNum ();
  SmbusController->AcpiBase         = PmcGetAcpiBase ();

  return EFI_SUCCESS;
}

/**
  Configures GPIO pins for SMBUS Interface

  @param[in]  SMBUS_HANDLE     Pointer to SMBUS Handle structure

  @retval EFI_STATUS      Status returned by worker function
**/
EFI_STATUS
EnableSmbusGpio (
  IN  SMBUS_HANDLE       *SmbusHandle
  )
{
  EFI_STATUS       Status;
  GPIOV2_SERVICES  *GpioServices;

  if (SmbusHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SMBUS: Failed to Configure SMBUS Pins\n"));
    return Status;
  }

  Status = PtlPcdGpioEnableSmbus (GpioServices);

  DEBUG ((DEBUG_INFO, "%a: Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Configures GPIO pins for SMBUS Alert

  @param[in]  SMBUS_HANDLE     Pointer to SMBUS Handle structure

  @retval EFI_STATUS      Status returned by worker function
**/
EFI_STATUS
EnableSmbusAlert (
  IN  SMBUS_HANDLE       *SmbusHandle
  )
{
  EFI_STATUS       Status;
  GPIOV2_SERVICES  *GpioServices;

  if (SmbusHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: NULL pointer detected!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SMBUS: Failed to Configure SMBUS Pins\n"));
    return Status;
  }

  Status = PtlPcdGpioEnableSmbusAlert (GpioServices);

  DEBUG ((DEBUG_INFO, "%a: Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Disables SMBUS controller in power controller and fabric

  @param[in]  SMBUS_HANDLE     Pointer to SMBUS Handle structure

**/
VOID
SmbusCtrlDisable (
  IN  SMBUS_HANDLE       *SmbusHandle
  )
{

  if (SmbusHandle == NULL) {
    DEBUG ((DEBUG_WARN, "%a: NULL pointer detected!\n", __FUNCTION__));
    return;
  }

  PtlPcdPsfDisableSmbus ();
}

/**
  Load default values to Smbus structures

  @param[in out]  SmbusHandle            Pointer to Smbus handle structure
  @param[in out]  SmbusController        Pointer to Smbus Controller structure
  @param[in out]  SmbusSocConfig         Pointer to Smbus Soc structure
  @param[in out]  SmbusCallbacks         Pointer to Smbus callback structure
**/
VOID
SmbusHandleLoadDefaults (
  IN OUT  SMBUS_HANDLE                *SmbusHandle,
  IN OUT  SMBUS_CONTROLLER            *SmbusController,
  IN OUT  SMBUS_SOC_CONFIG            *SmbusSocConfig,
  IN OUT  SMBUS_CALLBACKS             *SmbusCallbacks
)
{

  ZeroMem(SmbusController, sizeof(SMBUS_CONTROLLER));
  SmbusHandle->Controller = SmbusController;

  ZeroMem(SmbusSocConfig, sizeof(SMBUS_SOC_CONFIG));
  SmbusHandle->SocConfig = SmbusSocConfig;

  ZeroMem(SmbusCallbacks, sizeof(SMBUS_CALLBACKS));
  SmbusHandle->Callbacks = SmbusCallbacks;

  SmbusHandle->Config = NULL;
}
/**
  Initializes SMBUS handle.

  @param[in]  SiPolicyPpi         Pointer to SiPolicy
  @param[in]  SiPreMemPolicyPpi   Pointer to SiPreMemPolicyPpi
  @param[in]  SmbusHandle         P2SB Register Access to Smbus
  @param[in]  ControllerIndex     Controller Index of Smbus Controller

  @param[out] SmbusHandle         Initialized handle on output
  @param[out] Controller          Initialized Controller on output
  @param[out] SocConfig           Initialized SocConfig on output
  @param[out] Callbacks           Initialized Callbacks on output

  @retval     VOID
**/
VOID
InitSmbusHandle (
  IN  SI_POLICY_PPI                  *SiPolicyPpi,
  IN  SI_PREMEM_POLICY_PPI           *SiPreMemPolicyPpi,
  IN  UINT32                          ControllerIndex,
  IN  P2SB_SIDEBAND_REGISTER_ACCESS  *SmbusSbAccess,
  OUT P2SB_CONTROLLER                *P2SbController,
  OUT SMBUS_HANDLE                   *SmbusHandle,
  OUT SMBUS_CONTROLLER               *Controller,
  OUT SMBUS_SOC_CONFIG               *SocConfig,
  OUT SMBUS_CALLBACKS                *Callbacks
  )
{
  EFI_STATUS                   Status;
  UINT8                        InterruptPin;
  UINT8                        Irq;
  PCH_SMBUS_PREMEM_CONFIG      *SmbusPreMemConfig;
  P2SB_PORT_16_ID              P2SBPid;

  //
  // Initialize structures to default
  //
  SmbusHandleLoadDefaults (SmbusHandle, Controller, SocConfig, Callbacks);

  //
  // Get Smbus Controller
  //
  SmbusGetController (ControllerIndex, Controller);

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  SmbusHandle->Config = SmbusPreMemConfig;

  //
  //  Initialize Smbus Callback
  //
  Callbacks->SmbusDisable     = SmbusCtrlDisable;
  Callbacks->SmbusAlertEnable = EnableSmbusAlert;
  Callbacks->SmbusGpioEnable = EnableSmbusGpio;

  // Adding this check to make sure this SiPolicyPpi is not queried in PreMem phase
  if ( SiPolicyPpi != NULL) {
    ItssGetDevIntConfig (
        SiPolicyPpi,
        SmbusDevNumber (),
        SmbusFuncNumber (),
        &InterruptPin,
        &Irq
        );

    SocConfig->IntPin = InterruptPin;
    SocConfig->Irq = Irq;
    SocConfig->TcoIrq = ItssGetTcoDevIntConfig (SiPolicyPpi);
  }
  SocConfig->PowerManagementSupport = TRUE;
  SocConfig->TcoSmiTimeoutSupport = TRUE;
  SocConfig->HostNotifyWakeSupport = FALSE;
  SocConfig->HostTiming = 0x0A0A0000;
  SocConfig->InstallPpi = TRUE;
  SocConfig->TcoBase = PcdGet16 (PcdTcoBaseAddress);

  P2SBPid.Pid16bit = PTL_SID_F2_PID_SMB;
  Status = PtlPcdGetP2SbController (P2SbController, P2SBPid);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  //
  // Intialize Smbus P2SB Register Access
  //
  BuildP2SbSidebandAccess (
      P2SbController,
      P2SBPid.PortId.LocalPid,
      0,
      P2SbMemory,
      P2SbMmioAccess,
      FALSE,
      SmbusSbAccess
      );
  SmbusHandle->SmbusSbAccessMmio = (REGISTER_ACCESS *)SmbusSbAccess;

  return;
}

/**
  Initialize the Smbus PPI and program the Smbus BAR

  @param[in]  SiPreMemPolicyPpi   The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The function completes successfully
  @retval others                  Failed to Initialize
**/
EFI_STATUS
SmbusInit (
  IN  SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  SMBUS_HANDLE                    SmbusHandle;
  SMBUS_CONTROLLER                Controller;
  SMBUS_SOC_CONFIG                SocConfig;
  SMBUS_CALLBACKS                 Callbacks;
  P2SB_SIDEBAND_REGISTER_ACCESS   SmbusSbAccess;
  P2SB_CONTROLLER                 P2SbController;

  DEBUG ((DEBUG_INFO, "InitializePchSmbus() Start\n"));
  //
  // Initialize SMBUS handle.
  //
  InitSmbusHandle (NULL, SiPreMemPolicyPpi, SMBUS_1_CONTROLLER_INDEX, &SmbusSbAccess, &P2SbController, &SmbusHandle, &Controller, &SocConfig, &Callbacks);

  //
  // Initialize SMBUS Ppi
  //
  Status = SmbusPreMemInit (&SmbusHandle);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "InitializePchSmbus() End\n"));
  return EFI_SUCCESS;
}

/**
  The function performs SMBUS specific programming.

  @param[in] SiPolicyPpi       The SI Policy PPI instance

**/
VOID
SmbusConfigure (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  )
{
  SI_PREMEM_POLICY_PPI           *SiPreMemPolicyPpi = NULL;
  SMBUS_HANDLE                    SmbusHandle;
  SMBUS_CONTROLLER                Controller;
  SMBUS_SOC_CONFIG                SocConfig;
  SMBUS_CALLBACKS                 Callbacks;
  P2SB_SIDEBAND_REGISTER_ACCESS   SmbusSbAccess;
  P2SB_CONTROLLER                 P2SbController;
  //
  // Initialize SMBUS handle.
  //
  InitSmbusHandle (SiPolicyPpi, SiPreMemPolicyPpi, SMBUS_1_CONTROLLER_INDEX, &SmbusSbAccess, &P2SbController, &SmbusHandle, &Controller, &SocConfig, &Callbacks);
  //
  // Initialize SMBUS Controller
  //
  SmbusPostMemConfigure (&SmbusHandle);

  //
  // Set SmbSlv Bit this is SW WA part to enable S0i3.4 and S0i2.2
  //
  SmbusHandle.SmbusSbAccessMmio->AndThenOr32 (SmbusHandle.SmbusSbAccessMmio, R_SMBUS_PCR_GC, 0xFFFFFFFF, B_SMBUS_PCR_GC_SMBSLV);
}
