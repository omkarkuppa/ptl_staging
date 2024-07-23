/** @file
  Initializes DCI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <IntelRcStatusCode.h>
#include <Library/PeiDciInitLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/DciPrivateLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>
/**
  DCI low power mode configuration

  @param[in]  *DciPreMemConfig    Instance of PCH_DCI_PREMEM_CONFIG
  @param[in]  *DciPcrAccess       Instance of DCI sideband access interface
**/
VOID
SocDciLpmConfig (
  IN  PCH_DCI_PREMEM_CONFIG      *DciPreMemConfig,
  IN  REGISTER_ACCESS            *DciPcrAccess
  )
{
  ///
  /// Configure clock request override
  ///
  if (DciPreMemConfig->DciClkEnable) {
    DciEnClkReq (DciPcrAccess);
  } else {
    DciDisClkReq (DciPcrAccess);
  }
}

/**
  DCI reset handling.

  @param[in]  ResetType           UEFI defined reset type
**/
VOID
SocDciResetHandling (
  IN EFI_RESET_TYPE               ResetType
  )
{
  PCH_RESET_DATA                  ResetData;

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_INST_PCD | PC_PEI_PREMEM_RESET_DCI);
  DEBUG ((DEBUG_INFO, "DciConfiguration - global reset due to user opt-in debug state change.\n"));

  // Defer global reset to schedule reset timing. For one to ensure global reset executed fully thru heci command, which is not available prior to MRC;
  // For another when debug is enabled which possibly brings about crc sync global reset later to apply chipset init table for debug, defer to that point
  // can reduce global reset times.
  //
  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
  if (!IsSimicsEnvironment () && !IsHfpgaEnvironment ()) {
    SiScheduleResetSetType (ResetType, &ResetData);
  }
}

/**
  Enable USB3DBC disconnect in Sx
**/
STATIC
VOID
PcdDisconnectUsb3dbcInSx (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "PcdDisconnectUsb3dbcInSx() enable USB3DBC disconnect in Sx via PMC\n"));
  PmcEnableUsb3dbcDisconnectForSx (PmcGetPwrmBase ());
}

/**
  Disable USB3DBC disconnect in Sx
**/
STATIC
VOID
PcdConnectUsb3dbcInSx (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "PcdConnectUsb3dbcInSx() disable USB3DBC disconnect in Sx via PMC\n"));
  PmcDisableUsb3dbcDisconnectForSx (PmcGetPwrmBase ());
}

/*
  Initialize Soc Dci handle

  @param[in] SiPreMemPolicyPpi       The SI PreMem Policy PPI instance
  @param[in] DciHandle               Pointer to DCI handle structure
  @param[in] Callback                DCI_CALLBACK structure
  @param[in] PrivateConfig           DCI_PRIVATE_CONFIG structure
  @param[in] P2SbController          P2SB controller pointer
  @param[in] DciPcrAccess            P2SB Register Access to DCI
*/
STATIC
VOID
InitPtlPcdDciHandle (
  IN SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi,
  IN DCI_HANDLE                      *DciHandle,
  IN DCI_CALLBACK                    *Callback,
  IN DCI_PRIVATE_CONFIG              *PrivateConfig,
  IN P2SB_CONTROLLER                 *P2SbController,
  IN P2SB_SIDEBAND_REGISTER_ACCESS   *DciPcrAccess
  )
{
  EFI_STATUS                     Status;
  PCH_DCI_PREMEM_CONFIG          *DciPreMemConfig;
  P2SB_PORT_16_ID                P2SBPid;

  ZeroMem (DciHandle, sizeof (DCI_HANDLE));
  ZeroMem (Callback, sizeof (DCI_CALLBACK));
  ZeroMem (DciPcrAccess, sizeof (P2SB_SIDEBAND_REGISTER_ACCESS));
  ZeroMem (PrivateConfig, sizeof (DCI_PRIVATE_CONFIG));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gDciPreMemConfigGuid, (VOID *) &DciPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  DciHandle->DciPreMemConfig = DciPreMemConfig;

  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  PtlPcdGetP2SbController (P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, DciPcrAccess);
  DciHandle->DciPcrAccess = (REGISTER_ACCESS *) DciPcrAccess;

  PrivateConfig->AuthDebugEn = TRUE;
  PrivateConfig->ResetHandle = TRUE;
  DciHandle->PrivateConfig = PrivateConfig;

  Callback->DciLpmConfig = SocDciLpmConfig;
  Callback->DciResetHandling = SocDciResetHandling;
  Callback->SetDbcBusNumber = NULL;
  Callback->DisconnectUsb3dbcInSx = PcdDisconnectUsb3dbcInSx;
  Callback->ConnectUsb3dbcInSx = PcdConnectUsb3dbcInSx;
  DciHandle->Callback = Callback;
}

/**
  Initialize DCI

  @param[in]  SiPreMemPolicyPpi   The SI PreMem Policy PPI instance
**/
VOID
PtlPcdDciInit (
  IN  SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi
  )
{
  DCI_HANDLE                     SocDciHandle;
  DCI_CALLBACK                   Callback;
  DCI_PRIVATE_CONFIG             PrivateConfig;
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  DciPcrAccess;

  InitPtlPcdDciHandle (
    SiPreMemPolicyPpi,
    &SocDciHandle,
    &Callback,
    &PrivateConfig,
    &P2SbController,
    &DciPcrAccess
    );
  DciConfiguration (&SocDciHandle);
}

