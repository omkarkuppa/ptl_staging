/** @file
  The P2SB controller SoC specific implementation.

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

#include <Ppi/SiPolicy.h>

#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Library/P2SbSocLib.h>
#include <Register/PchRegs.h>
#include <P2SbHandle.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Defines/PcdPchBdfAssignment.h>
#include "PtlPcdInitPei.h"
#include <PcdSbPortIds.h>
GLOBAL_REMOVE_IF_UNREFERENCED P2SB_PID mPtlPcdFirstP2SbPostedWritesEndpoints[] = {
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_GPCOM0),
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_GPCOM1),
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_GPCOM3),
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_GPCOM4),
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_GPCOM5),
  GET_P2SB_LOCAL_PID(PTL_SID_F2_PID_ITSS)
};

/**
  Load default values to P2SB structures
  - Initialize data structures to zero
  - Initialize function pointers to NULL
  - Initialize pointes for P2SB_HANDLE substructures

  @param[in out]  P2SbHandle      Pointer to P2SB handle structure
  @param[in out]  P2SbPrivate     Pointer to P2SB private configuration structure
  @param[in out]  P2SbController  Pointer to P2SB controller identification structure
  @param[in out]  P2SbCallback    Pointer to P2SB callback structure
  @param[in out]  P2SbPcieAccess  Pointer to P2SB PCIe Register Access
**/
STATIC
VOID
P2SbHandleLoadDefaults (
  IN OUT P2SB_HANDLE             *P2SbHandle,
  IN OUT P2SB_PRIVATE_CONFIG     *P2SbPrivate,
  IN OUT P2SB_CONTROLLER         *P2SbController,
  IN OUT P2SB_CALLBACK           *P2SbCallback,
  IN OUT PCI_CONFIG_SPACE_ACCESS *P2SbPcieAccess
  )
{
  ZeroMem (P2SbPrivate, sizeof (P2SB_PRIVATE_CONFIG));
  P2SbHandle->PrivateConfig = P2SbPrivate;

  ZeroMem (P2SbCallback, sizeof (P2SB_CALLBACK));
  P2SbHandle->Callback = P2SbCallback;

  ZeroMem (P2SbController, sizeof (P2SB_CONTROLLER));
  P2SbHandle->Controller = P2SbController;

  ZeroMem (P2SbPcieAccess, sizeof (PCI_CONFIG_SPACE_ACCESS));
  P2SbHandle->PcieConfigAccess = (REGISTER_ACCESS *) P2SbPcieAccess;

  P2SbHandle->P2SbConfig = NULL;
}

/**
  Initialize P2SB structures - PCH Legacy
  This should be done according P2SB IP integration in SoC

  @param[in]      SiPolicy        Pointer to Silicon Policy
  @param[in out]  P2SbHandle      Pointer to P2SB handle structure
  @param[in out]  P2SbPrivate     Pointer to P2SB private configuration structure
  @param[in out]  P2SbController  Pointer to P2SB controller identification structure
  @param[in out]  P2SbCallback    Pointer to P2SB callback structure
  @param[in out]  P2SbPcieAccess  Pointer to P2SB PCIe Register Access
**/
VOID
PtlPcdP2SbHandleInit (
  IN     SI_POLICY_PPI           *SiPolicy,
  IN OUT P2SB_HANDLE             *P2SbHandle,
  IN OUT P2SB_PRIVATE_CONFIG     *P2SbPrivate,
  IN OUT P2SB_CONTROLLER         *P2SbController,
  IN OUT P2SB_CALLBACK           *P2SbCallback,
  IN OUT PCI_CONFIG_SPACE_ACCESS *P2SbPcieAccess
  )
{
  EFI_STATUS            Status;
  PCH_GENERAL_CONFIG    *PchGeneralConfig;

  //
  // Initialize: data with defaults, function pointers to NULL, substructures pointers
  //
  P2SbHandleLoadDefaults (P2SbHandle, P2SbPrivate, P2SbController, P2SbCallback, P2SbPcieAccess);

  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, P2sbDevNumber (), P2sbFuncNumber (), P2SbPcieAccess);

  //
  // Initialize P2SB Private Configuration
  //
  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
    }
    P2SbHandle->PrivateConfig->LegacyIoLowLatency = PchGeneralConfig->LegacyIoLowLatency != 0;
  }
  P2SbHandle->PrivateConfig->HaPowerGatingSupported = TRUE;
  P2SbHandle->PrivateConfig->IecSupportDisable = TRUE;
  P2SbHandle->PrivateConfig->DisableHpetAndApicBdfProgramming = FALSE;

  //
  // Set pointer to P2SB Config Block
  //
  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2SbHandle->P2SbConfig);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
    }
    ASSERT (P2SbHandle->P2SbConfig != NULL);
  }

  //
  // Initialize P2SB device data
  //
  Status = PtlPcdGetFirstP2SbController (P2SbController);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get P2SB controller!\n", __FUNCTION__));
    ASSERT (FALSE);
  }
}

/**
  The function performs SOC P2SB lock programming.

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdP2sbLock (
  IN  SI_POLICY_PPI           *SiPolicy
  )
{
  P2SB_HANDLE             P2SbHandle;
  P2SB_PRIVATE_CONFIG     P2SbPrivate;
  P2SB_CONTROLLER         P2SbController;
  P2SB_CALLBACK           P2SbCallback;
  PCI_CONFIG_SPACE_ACCESS PcieConfigAccess;

  PtlPcdP2SbHandleInit (SiPolicy, &P2SbHandle, &P2SbPrivate, &P2SbController, &P2SbCallback, &PcieConfigAccess);
  P2sbLock (&P2SbHandle);
}

/**
  The function performs P2SB lock programming on second instance of P2SB

  @param[in] SiPolicy         The SI Policy PPI instance
**/
VOID
PtlPcdSecondP2sbLock (
  IN  SI_POLICY_PPI           *SiPolicy
  )
{
  P2SB_HANDLE             P2SbHandle;
  P2SB_PRIVATE_CONFIG     P2SbPrivate;
  P2SB_CONTROLLER         P2SbController;
  P2SB_CALLBACK           P2SbCallback;
  PCI_CONFIG_SPACE_ACCESS PcieConfigAccess;

  PtlPcdSecondP2SbHandleInit (SiPolicy, &P2SbHandle, &P2SbPrivate, &P2SbController, &P2SbCallback, &PcieConfigAccess);
  P2sbLock (&P2SbHandle);
}

/**
  The function performs P2SB initialization for SOC.

  @param[in]      SiPolicy       Pointer to Silicon Policy
**/
VOID
PtlPcdP2SbInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  P2SB_HANDLE             P2SbHandle;
  P2SB_PRIVATE_CONFIG     P2SbPrivate;
  P2SB_CONTROLLER         P2SbController;
  P2SB_CALLBACK           P2SbCallback;
  PCI_CONFIG_SPACE_ACCESS PcieConfigAccess;

  PtlPcdP2SbHandleInit (SiPolicy, &P2SbHandle, &P2SbPrivate, &P2SbController, &P2SbCallback, &PcieConfigAccess);
  P2sbConfigure (&P2SbHandle);
}

/**
  The function performs P2SB initialization for IOE.

  @param[in]      SiPolicy       Pointer to Silicon Policy
**/
VOID
PtlPcdSecondP2SbInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  P2SB_HANDLE             P2SbHandle;
  P2SB_PRIVATE_CONFIG     P2SbPrivate;
  P2SB_CONTROLLER         P2SbController;
  P2SB_CALLBACK           P2SbCallback;
  PCI_CONFIG_SPACE_ACCESS PcieConfigAccess;

  PtlPcdSecondP2SbHandleInit (SiPolicy, &P2SbHandle, &P2SbPrivate, &P2SbController, &P2SbCallback, &PcieConfigAccess);
  P2sbConfigure (&P2SbHandle);
}

/**
  Initialize P2SB structures - second P2SB instance
  This should be done according P2SB IP integration

  @param[in]      SiPolicy        Pointer to Silicon Policy
  @param[in out]  P2SbHandle      Pointer to P2SB handle structure
  @param[in out]  P2SbPrivate     Pointer to P2SB private configuration structure
  @param[in out]  P2SbController  Pointer to P2SB controller identification structure
  @param[in out]  P2SbCallback    Pointer to P2SB callback structure
  @param[in out]  P2SbPcieAccess  Pointer to P2SB PCIe Register Access
**/
VOID
PtlPcdSecondP2SbHandleInit (
  IN     SI_POLICY_PPI           *SiPolicy,
  IN OUT P2SB_HANDLE             *P2SbHandle,
  IN OUT P2SB_PRIVATE_CONFIG     *P2SbPrivate,
  IN OUT P2SB_CONTROLLER         *P2SbController,
  IN OUT P2SB_CALLBACK           *P2SbCallback,
  IN OUT PCI_CONFIG_SPACE_ACCESS *P2SbPcieAccess
  )
{
  EFI_STATUS            Status;
  PCH_GENERAL_CONFIG    *PchGeneralConfig;

  //
  // Initialize: data with defaults, function pointers to NULL, substructures pointers
  //
  P2SbHandleLoadDefaults (P2SbHandle, P2SbPrivate, P2SbController, P2SbCallback, P2SbPcieAccess);

  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_SECOND_P2SB, PCI_FUNCTION_NUMBER_SECOND_P2SB, P2SbPcieAccess);

  //
  // Initialize P2SB Private Configuration
  //
  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
    }
    P2SbHandle->PrivateConfig->LegacyIoLowLatency = PchGeneralConfig->LegacyIoLowLatency != 0;
  }
  P2SbHandle->PrivateConfig->HaPowerGatingSupported = TRUE;
  P2SbHandle->PrivateConfig->IecSupportDisable = TRUE;
  P2SbHandle->PrivateConfig->DisableHpetAndApicBdfProgramming = TRUE;

  //
  // Set pointer to P2SB Config Block
  //
  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2SbHandle->P2SbConfig);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Status = %r\n", Status));
    }
    ASSERT (P2SbHandle->P2SbConfig != NULL);
  }

  //
  // Initialize P2SB device data
  //
  Status = PtlPcdGetSecondP2SbController (P2SbController);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR: Cannot get P2SB controller!\n", __FUNCTION__));
    ASSERT (FALSE);
  }
}

/**
  Early init P2SB configuration
**/
VOID
PtlPcdP2sbEarlyConfig (
  VOID
  )
{
  PCI_CONFIG_SPACE_ACCESS FirstPcieConfigAccess;
  PCI_CONFIG_SPACE_ACCESS SecondPcieConfigAccess;

  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, P2sbDevNumber (), P2sbFuncNumber (), &FirstPcieConfigAccess);
  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_SECOND_P2SB, PCI_FUNCTION_NUMBER_SECOND_P2SB, &SecondPcieConfigAccess);

  P2sbEarlyConfig (&FirstPcieConfigAccess.RegAccess);
  P2sbEarlyConfig (&SecondPcieConfigAccess.RegAccess);

  P2SbProgramPostedWriteMask (&FirstPcieConfigAccess.RegAccess, mPtlPcdFirstP2SbPostedWritesEndpoints, ARRAY_SIZE(mPtlPcdFirstP2SbPostedWritesEndpoints));
}
