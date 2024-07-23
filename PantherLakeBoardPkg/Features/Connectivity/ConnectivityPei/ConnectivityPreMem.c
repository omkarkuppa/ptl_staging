/** @file
  Source code file for CNV Pre-Memory PEI module

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
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>
#include <SetupVariable.h>
#include <Library/GpioV2WrapperLib.h>
#include <Register/GpioV2PcdPins.h>
#include <CnvVfrSetupMenuHii.h>

static EFI_PEI_PPI_DESCRIPTOR  mFvCnvDispatchFlagPpi[] = {
  {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiFvCnvDispatchFlagPpiGuid,
  NULL
  }
};

/**
  Configures GPIO

  @param[in]  GpioDefinition  Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIOV2_INIT_CONFIG               *GpioDefinition,
  IN UINT16                           GpioTableCount
  )
{
  EFI_STATUS          Status;

  Status = GpioV2ConfigurePads (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  Status = GpioV2SetConflictCheckData (GpioDefinition, GpioTableCount);
  ASSERT_EFI_ERROR (Status);
}

/**
  This function handles CNV Init task after PeiReadOnlyVariable2 PPI produced

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
CnvInitPreMem (
  VOID
  )
{

  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VariableSize;
  SETUP_DATA                        SystemConfiguration;
  CNV_VFR_CONFIG_SETUP              CnvSetup;

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

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
    PcdSet8S (PcdPcieWwanEnable, 0);
    PcdSetBoolS (PcdWwanResetWorkaround, FALSE);
  } else {
    PcdSet8S (PcdPcieWwanEnable, SystemConfiguration.WwanEnable);
    PcdSetBoolS (PcdWwanResetWorkaround, SystemConfiguration.WwanResetWorkaround);
    PcdSet16S (PcdBoardWwanTOn2ResDelayMs, SystemConfiguration.WwanTOn2ResDelayMs);
    PcdSet16S (PcdBoardWwanTOnRes2PerDelayMs, SystemConfiguration.WwanTOnRes2PerDelayMs);
    PcdSet16S (PcdBoardWwanTOnPer2PdsDelayMs, SystemConfiguration.WwanTOnPer2PdsDelayMs);
    PcdSet16S (PcdBoardWwanTPer2ResDelayMs, SystemConfiguration.WwanTPer2ResDelayMs);
    PcdSet16S (PcdBoardWwanTRes2OffDelayMs, SystemConfiguration.WwanTRes2OffDelayMs);
    PcdSet16S (PcdBoardWwanTOffDisDelayMs, SystemConfiguration.WwanTOffDisDelayMs);
    PcdSet16S (PcdBoardWwanTResTogDelayMs, SystemConfiguration.WwanTResTogDelayMs);
    PcdSet16S (PcdBoardWwanTRes2PdsDelayMs, SystemConfiguration.WwanTRes2PdsDelayMs);

    DEBUG((DEBUG_INFO, "WwanEnable: %d\n", PcdGet8 (PcdPcieWwanEnable)));
    DEBUG((DEBUG_INFO, "WwanTOn2ResDelayMs: %d\n", PcdGet16 (PcdBoardWwanTOn2ResDelayMs)));
    DEBUG((DEBUG_INFO, "WwanTOnRes2PerDelayMs: %d\n", PcdGet16 (PcdBoardWwanTOnRes2PerDelayMs)));
    DEBUG((DEBUG_INFO, "WwanTOnPer2PdsDelayMs: %d\n", PcdGet16 (PcdBoardWwanTOnPer2PdsDelayMs)));
    DEBUG((DEBUG_INFO, "WwanTPer2ResDelayMs: %d\n", PcdGet16 (PcdBoardWwanTRes2OffDelayMs)));
    DEBUG((DEBUG_INFO, "WwanTRes2OffDelayMs: %d\n", PcdGet16 (PcdBoardWwanTRes2OffDelayMs)));
    DEBUG((DEBUG_INFO, "WwanTOffDisDelayMs: %d\n", PcdGet16 (PcdBoardWwanTOffDisDelayMs)));
    DEBUG((DEBUG_INFO, "WwanTResTogDelayMs: %d\n", PcdGet16 (PcdBoardWwanTResTogDelayMs)));
    DEBUG((DEBUG_INFO, "WwanTRes2PdsDelayMs: %d\n", PcdGet16 (PcdBoardWwanTRes2PdsDelayMs)));
  }

  VariableSize = sizeof (CNV_VFR_CONFIG_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"CnvSetup",
                               &gCnvFeatureSetupGuid,
                               NULL,
                               &VariableSize,
                               &CnvSetup
                               );

  //  install PPI descriptor for CNV, based on policy settings.
  if ((SystemConfiguration.EfiNetworkSupport != 0) ||
      (CnvSetup.PrebootBleEnable == 1)) {
    Status = PeiServicesInstallPpi (mFvCnvDispatchFlagPpi);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Basic GPIO configuration before memory is ready

**/
VOID
GpioInitEarlyWwanPreMem (
  VOID
  )
{
  UINT32                          WwanBbrstGpio;
  UINT32                          WwanPerstGpio;
  GPIOV2_PAD_STATE                PerstState;
  GPIOV2_PAD_STATE                BprstState;

  DEBUG ((DEBUG_ERROR, "GpioInitEarlyWwanPreMem entry\n"));
  WwanBbrstGpio = PcdGet32 (PcdWwanBbrstGpio);
  WwanPerstGpio = PcdGet32 (PcdWwanPerstGpio);

  if (WwanBbrstGpio) {
    //
    // BIOS needs to reset modem if modem RESET# is not asserted via PLTRST# in the previous sleep state
    //
    GpioV2GetTx (WwanBbrstGpio, &BprstState);
    if ((PcdGet8 (PcdPcieWwanEnable) == 0) ||
        ((PcdGetBool (PcdWwanResetWorkaround) == TRUE) &&
        (BprstState == GpioV2StateHigh) &&
        (PcdGet8 (PcdPcieWwanEnable) == 1))) {  // 4G - Intel Modem 7360/7560
      //
      // Assert FULL_CARD_POWER_OFF#, RESET# and PERST# GPIOs
      //
      if (PcdGet64 (PcdBoardGpioTableWwanOffEarlyPreMem) != 0 && PcdGet16 (PcdBoardGpioTableWwanOffEarlyPreMemSize) != 0) {
        ConfigureGpio ((VOID *) (UINTN) PcdGet64 (PcdBoardGpioTableWwanOffEarlyPreMem), (UINTN) PcdGet16 (PcdBoardGpioTableWwanOffEarlyPreMemSize));
      }
      MicroSecondDelay (1 * 1000); // Delay by 1ms
    }

    if (PcdGet8 (PcdPcieWwanEnable) == 1) { // 4G - Intel Modem 7360/7560
      if (PcdGet64 (PcdBoardGpioTableWwanOnEarlyPreMem) != 0 && PcdGet16 (PcdBoardGpioTableWwanOnEarlyPreMemSize) != 0) {
        ConfigureGpio ((VOID *) (UINTN) PcdGet64 (PcdBoardGpioTableWwanOnEarlyPreMem), (UINTN) PcdGet16 (PcdBoardGpioTableWwanOnEarlyPreMemSize));
      }
    } else if (PcdGet8 (PcdPcieWwanEnable) == 2) { // 5G - Mediatek Modem M80
      if (PcdGet64 (PcdBoardGpioTableM80WwanOnEarlyPreMem) != 0 && PcdGet16 (PcdBoardGpioTableM80WwanOnEarlyPreMemSize) != 0) {
        DEBUG((DEBUG_INFO, "5G M80 Modem: Power On Sequnce Start\n"));
        //
        // 1. Turn on EN_V3.3A_WWAN_LS, Turn off WWAN_RST_N & WWAN_PERST
        //
        ConfigureGpio ((VOID *) (UINTN) PcdGet64(PcdBoardGpioTableM80WwanOnEarlyPreMem), (UINTN) PcdGet16 (PcdBoardGpioTableM80WwanOnEarlyPreMemSize));
        MicroSecondDelay ((UINTN) PcdGet16 (PcdBoardWwanTOn2ResDelayMs) * 1000);
        //
        // 2. De-asserting WWAN_RST_N
        //
        if (WwanBbrstGpio) {
          GpioV2SetTx (WwanBbrstGpio, GpioV2StateHigh);
          MicroSecondDelay ((UINTN) PcdGet16 (PcdBoardWwanTOnRes2PerDelayMs) * 1000);
        }
        //
        // 3. De-asserting WWAN_PERST
        //
        if (WwanPerstGpio) {
          if (PcdGetBool (PcdWwanPerstGpioPolarity)) {
            PerstState = GpioV2StateLow;
          } else {
            PerstState = GpioV2StateHigh;
          }
          GpioV2SetTx (WwanPerstGpio, PerstState);
        }
        DEBUG((DEBUG_INFO, "5G M80 Modem: Power On Sequnce End\n"));
      }
    }
  }
}

/**
  CNV Init before memory PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
ConnectivityPreMemEntryPoint(
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;

  Status = CnvInitPreMem ();

  GpioInitEarlyWwanPreMem ();

  return Status;
}
