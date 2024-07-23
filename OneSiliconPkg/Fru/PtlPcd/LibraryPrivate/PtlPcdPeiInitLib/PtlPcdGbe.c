/** @file
  The GBE controller SoC specific implementation.

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

#include <Ppi/SiPolicy.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchInfoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiGbeInitLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PmcLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdPolicyLib.h>
#include <Register/PchRegs.h>
#include <PmConfig.h>
#include <GbeConfig.h>
#include <GbeHandle.h>
#include <PchPcieRpConfig.h>
#include <Library/PcdGbeSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>

/**
  This function checks if GbE controller is supported (not disabled by fuse)

  @param[in]  GbeController  Pointer to GBE controller structure

  @retval GbE support state
**/
STATIC
BOOLEAN
GbeIsSupported (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  return PmcIsGbeSupported (PmcGetPwrmBase ());
}

/**
  This function checks if GbE is function disabled
  by static power gating

  @param[in]  GbeController  Pointer to GBE controller structure

  @retval GbE device state
**/
STATIC
BOOLEAN
GbeIsFunctionDisabled (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  return PmcIsGbeFunctionDisabled ();
}

/**
  This function enables GbE controller by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.

  @param[in]  GbeController  Pointer to GBE controller structure
**/
STATIC
VOID
GbeEnable (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  PmcEnableGbe ();
}

/**
  This function disables GbE device by static power gating and enables ModPHY SPD gating (PCH-LP only).
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @param[in]  GbeController  Pointer to GBE controller structure
**/
STATIC
VOID
GbeDisable (
  IN  GBE_CONTROLLER          *GbeController
  )
{
  PmcStaticDisableGbe ();
}

/**
  Disables one PCIe clock.

  @param[in]  GbeController   Pointer to GBE controller structure
**/
STATIC
VOID
GbeDisableClock (
  IN  GBE_CONTROLLER       *GbeController
  )
{
  DisableClock (PchClockUsageLan);
}

/**
  This function enables GBE ModPHY SPD gating.

  @param[in]  GbeController   Pointer to GBE controller structure
**/
STATIC
VOID
GbeModPhyPowerGating (
  IN  GBE_CONTROLLER           *GbeController
  )
{
}

/**
  Load default values to GBE structures
  - Initialize data structures to zero
  - Initialize function pointers to NULL
  - Initialize pointes for GBE_HANDLE substructures

  @param[in out]  GbeHandle      Pointer to GBE handle structure
  @param[in out]  GbePrivate     Pointer to GBE private configuration structure
  @param[in out]  GbeController  Pointer to GBE controller identification structure
  @param[in out]  GbeCallback    Pointer to GBE callback structure
**/
STATIC
VOID
GbeHandleLoadDefaults (
  IN OUT  GBE_HANDLE          *GbeHandle,
  IN OUT  GBE_PRIVATE_CONFIG  *GbePrivate,
  IN OUT  GBE_CONTROLLER      *GbeController,
  IN OUT  GBE_CALLBACK        *GbeCallback
  )
{
  ZeroMem (GbePrivate, sizeof (GBE_PRIVATE_CONFIG));
  GbeHandle->PrivateConfig = GbePrivate;

  ZeroMem (GbeCallback, sizeof (GBE_CALLBACK));
  GbeHandle->Callback = GbeCallback;

  ZeroMem (GbeController, sizeof (GBE_CONTROLLER));
  GbeHandle->Controller = GbeController;

  GbeHandle->GbeConfig = NULL;
}

/**
  Initialize GBE structures
  This should be done according GBE IP integration in SoC

  @param[in]      SiPolicy       Pointer to Silicon Policy
  @param[in out]  GbeHandle      Pointer to GBE handle structure
  @param[in out]  GbePrivate     Pointer to GBE private configuration structure
  @param[in out]  GbeController  Pointer to GBE controller identification structure
  @param[in out]  GbeCallback    Pointer to GBE callback structure
**/
STATIC
VOID
GbeHandleInit (
  IN      SI_POLICY_PPI       *SiPolicy,
  IN OUT  GBE_HANDLE          *GbeHandle,
  IN OUT  GBE_PRIVATE_CONFIG  *GbePrivate,
  IN OUT  GBE_CONTROLLER      *GbeController,
  IN OUT  GBE_CALLBACK        *GbeCallback
  )
{
  EFI_STATUS            Status;
  PCH_PM_CONFIG         *PmConfig;

  //
  // Initialize: data with defaults, function pointers to NULL, substructures pointers
  //
  GbeHandleLoadDefaults (GbeHandle, GbePrivate, GbeController, GbeCallback);

  if (SiPolicy != NULL) {
    //
    // Initialize GBE Private Configuration
    //
    Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
    if (!EFI_ERROR (Status)) {
      GbePrivate->WolEnableOverride = PmConfig->WakeConfig.WolEnableOverride != 0;
    }
    GbePrivate->InterruptPinSupported = TRUE;
    GbePrivate->InterruptPin = ItssGetDevIntPin (SiPolicy, GbeDevNumber (), GbeFuncNumber ());
  }

  if (!PtlPcdFiaGetGbeLaneNum (&GbePrivate->LaneNum)) {
    GbePrivate->LaneNum = 0xFF; // no lane assigned
  }

  if (IsClkReqAssigned (PchClockUsageLan)) {
    GbePrivate->HasClkReq = TRUE;
  }

  GbePrivate->LockWriteFlashSupported = TRUE;


  //
  // Initialize GBE callback pointers
  //
  GbeCallback->IsSupported = GbeIsSupported;
  GbeCallback->IsFunctionDisabled = GbeIsFunctionDisabled;
  GbeCallback->Enable = GbeEnable;
  GbeCallback->Disable = GbeDisable;
  GbeCallback->ModPhyPowerGating = GbeModPhyPowerGating;
  GbeCallback->DisableClk = GbeDisableClock;

  //
  // Set pointer to GBE Config Block
  //
  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gGbeConfigGuid, (VOID *) &GbeHandle->GbeConfig);
    if (EFI_ERROR (Status) || (GbeHandle->GbeConfig == NULL)) {
      DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get Gbe Config Block!\n", __FUNCTION__));
      ASSERT (FALSE);
    }
  }

  //
  // Initialize GBE device data
  //
  Status = GbeGetController (GbeController);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get GBE controller!\n", __FUNCTION__));
    ASSERT (FALSE);
  }

  GbeHandle->Mmio           = PcdGet32 (PcdSiliconInitTempMemBaseAddr);
  GbeHandle->PwrmBase       = PmcGetPwrmBase ();
}

/**
  Initialize LAN device

  @param[in]      SiPolicy       Pointer to Silicon Policy
**/
VOID
PtlPcdGbeInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  GBE_HANDLE           GbeHandle;
  GBE_PRIVATE_CONFIG   GbePrivate;
  GBE_CONTROLLER       GbeController;
  GBE_CALLBACK         GbeCallback;

  GbeHandleInit (SiPolicy, &GbeHandle, &GbePrivate, &GbeController, &GbeCallback);

  //
  // Enable CLKREQ# if supported by board regardless of GbE being enabled
  // to allow clock shut-down.
  //
  if (GbePrivate.HasClkReq) {
    EnableClkReq (PchClockUsageLan);
  }

  GbeInit (&GbeHandle);
}
