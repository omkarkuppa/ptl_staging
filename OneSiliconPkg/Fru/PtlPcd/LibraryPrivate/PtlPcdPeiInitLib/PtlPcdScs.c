/** @file
  PTL SoC SCS init.

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

#include <Library/PchPciBdfLib.h>
#include <Library/PmcSocLib.h>
#include "PtlPcdInitPei.h"
#include <Register/ScsRegs.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <PcdSbPortIds.h>
#include <Library/PmcPrivateLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Library/PcdInfoLib.h>

/**
  This function checks if SCS UFS device is Fuse disabled or not.

  @retval SCS device fuse disable state
**/
BOOLEAN
IsScsUfsFuseDisabled (
  VOID
  )
{
  return FALSE;
}

/**
  Schedules Global Reset required to change UFS static power gating state.
**/
VOID
UfsPerformGlobalReset (
  VOID
  )
{
  PCH_RESET_DATA             ResetData;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
  SiScheduleResetSetType (EfiResetPlatformSpecific, &ResetData);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Enables UFS IP in PMC FW by clearing BIT0 of SCRATCH_GCR_0 register.
**/
VOID
PmcStaticEnableUfs (
  VOID
  )
{
  UINTN                PchPwrmBase;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  PchPwrmBase = PmcGetPwrmBase ();
  //
  // Note: This will clear BIT0 of SCRATCH_GCR_0 register and do globel reset.
  //
  MmioAnd32 (PchPwrmBase + R_PMC_PWRM_RTC_SCRATCH_GCR_0, (UINT32)~(B_PMC_PWRM_RTC_SCRATCH_GCR_0_DISABLE_UFS));
  UfsPerformGlobalReset ();

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Called as a last step in UFS controller disable flow.

  @param[in] ScsUfsHandle  Handle
**/
VOID
PtlPcdScsUfsDisable (
  IN SCS_UFS_HANDLE *ScsUfsHandle
  )
{
  UINTN                PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();
  //
  // NOTE: IOSF2OCP controller index just happens to match the PSF and PMC controller index. In general they do not
  // have to be the same.
  //
  PtlPcdPsfDisableScsUfs (ScsUfsHandle->Controller.Iosf2OcpPort->ControllerIndex);
  //
  // Note: This will check and set BIT0 of SCRATCH_GCR_0 register and do globel reset.
  // If this is set, PMC FW will keep the UFS IP in IP-Inaccessible state.
  //
  if (!PmcIsScsUfsFunctionDisabled ()) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_RTC_SCRATCH_GCR_0, B_PMC_PWRM_RTC_SCRATCH_GCR_0_DISABLE_UFS);
    UfsPerformGlobalReset ();
  }
}

/**
  Called after disabling BAR1 in IOSF2OCP.

@param[in] ScsUfsHandle  Handle
**/
VOID
PtlPcdScsUfsBar1Disable (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  )
{
  //
  // NOTE: IOSF2OCP controller index just happens to match the PSF controller index. In general they do not
  // have to be the same.
  //
  PtlPcdPsfDisableScsUfsBar1 (ScsUfsHandle->Controller.Iosf2OcpPort->ControllerIndex);
}

/**
  Initializes UFS handle.

  @param[in]  ControllerNumber     Number of the controller
  @param[in]  SiPolicyPpi          SiPolicy
  @param[in]  ScsInfoHob           SCS info hob
  @param[in]  UfsControllerConfig  Pointer to UFS controller platform config
  @param[in]  Ufs0Port             UFS port handle
  @param[out] ScsUfsHandle         Initialized handle on output

  @retval EFI_SUCESS  Handle initialized
  @retval Others      Failed to initialize
**/
EFI_STATUS
PtlPcdScsInitUfsHandle (
  IN  SI_POLICY_PPI              *SiPolicyPpi,
  IN  SCS_INFO_HOB               *ScsInfoHob,
  IN  SCS_UFS_CONTROLLER_CONFIG  *UfsControllerConfig,
  IN  IOSF2OCP_PORT              *Ufs0Port,
  OUT SCS_UFS_HANDLE             *ScsUfsHandle
  )
{
  UINT8            InterruptPin;
  UINT8            Irq;

  ScsUfsHandle->Controller.Iosf2OcpPort = Ufs0Port;
  ScsUfsHandle->Controller.Mmp = NULL;

  ScsUfsHandle->Controller.PciCfgBase = ScsUfsPciCfgBase (Ufs0Port->ControllerIndex);
  ScsUfsHandle->Controller.MmioBase = PcdGet32 (PcdSiliconInitTempMemBaseAddr);

  ScsUfsHandle->SocConfig.IsBootMedium = FALSE;
  ScsUfsHandle->SocConfig.IsFuseDisabled = IsScsUfsFuseDisabled ();
  ScsUfsHandle->SocConfig.IsMPhyUlpUpdateRequired = FALSE;
  ScsUfsHandle->SocConfig.NumOfLanes = 2;
  ItssGetDevIntConfig (
    SiPolicyPpi,
    ScsUfsDevNumber (Ufs0Port->ControllerIndex),
    ScsUfsFuncNumber (Ufs0Port->ControllerIndex),
    &InterruptPin,
    &Irq
    );
  ScsUfsHandle->SocConfig.IntPin = InterruptPin;
  ScsUfsHandle->SocConfig.Irq = Irq;

  ScsUfsHandle->Config = UfsControllerConfig;

  ScsUfsHandle->Callbacks.Disable = PtlPcdScsUfsDisable;
  ScsUfsHandle->Callbacks.Bar1Disable = PtlPcdScsUfsBar1Disable;

  ScsUfsHandle->UfsInfo = &ScsInfoHob->UfsInfo[Ufs0Port->ControllerIndex];

  return EFI_SUCCESS;
}

/**
  Initializes PTL SoC SCS subsystem.

  @param[in] SiPolicy                 The SI Policy PPI instance
**/
VOID
PtlPcdScsInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  SCS_UFS_HANDLE                ScsUfsHandle;
  P2SB_CONTROLLER               P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS ScsPcrAccess;
  VOID                          *HobPointer;
  SCS_INFO_HOB                  ScsInfoHob;
  EFI_STATUS                    Status;
  SCS_UFS_CONFIG                *UfsConfig;
  IOSF2OCP_PORT                 Ufs0Port;
  P2SB_PORT_16_ID               P2SBPid;

  DEBUG ((DEBUG_INFO, "%a(): Start\n", __FUNCTION__));

  Ufs0Port.ControllerIndex = 0;

  //
  // If UFS is not supported by PMC just return.
  // PSF access to the device has already been disabled in pre-mem.
  //
  if (!PmcIsScsUfsSupported (Ufs0Port.ControllerIndex)) {
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUfsConfigGuid, (VOID *) &UfsConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate UFS config block\n"));
    return;
  }
  //
  // If UFS is not supported by PMC and UFS is enabled by default.
  // Clear BIT0 of SCRATCH_GCR_0 to Enable UFS.
  //
  if (PmcIsScsUfsFunctionDisabled () && UfsConfig->UfsControllerConfig[Ufs0Port.ControllerIndex].Enable && UfsConfig->UfsControllerConfig[Ufs0Port.ControllerIndex].UfsDeviceConnected) {
    PmcStaticEnableUfs ();
    return;
  }

  ZeroMem (&ScsInfoHob, sizeof (SCS_INFO_HOB));

  P2SBPid.Pid16bit = PTL_SID_F2_PID_UFS;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &ScsPcrAccess);
  Ufs0Port.BridgeAccess = (REGISTER_ACCESS *) &ScsPcrAccess;

  //
  // Initialize IOSF2OCP bridge power gating. On PTL there is only
  // one IOSF2OCP bridge.
  //
  Iosf2OcpConfigurePowerAndClockGating (Ufs0Port.BridgeAccess, 0x9980F);
  Iosf2OcpEnableSnoopedTransactions (Ufs0Port.BridgeAccess);

  Status = PtlPcdScsInitUfsHandle (SiPolicy, &ScsInfoHob, &UfsConfig->UfsControllerConfig[Ufs0Port.ControllerIndex], &Ufs0Port, &ScsUfsHandle);
  if (!EFI_ERROR (Status)) {
    ScsUfsInit (&ScsUfsHandle);
    if ((ScsUfsHandle.Config->Enable || ScsUfsHandle.SocConfig.IsBootMedium) &&
       (ScsUfsHandle.SocConfig.NumOfLanes != 0) && ScsUfsHandle.Config->UfsDeviceConnected) {
        PciSegmentOr16(
          ScsUfsHandle.Controller.PciCfgBase + R_SCS_CFG_PG_CONFIG,
          (B_SCS_CFG_PG_CONFIG_PGE | B_SCS_CFG_PG_CONFIG_I3E)
      );
    }
  }

  HobPointer = BuildGuidDataHob (
                 &gScsInfoHobGuid,
                 &ScsInfoHob,
                 sizeof (SCS_INFO_HOB)
                 );
  ASSERT (HobPointer != NULL);

  DEBUG ((DEBUG_INFO, "%a(): Finished\n", __FUNCTION__));
}
