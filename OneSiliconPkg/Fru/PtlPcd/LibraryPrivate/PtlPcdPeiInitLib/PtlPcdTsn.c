/** @file
  PTL PCH TSN init.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchPciBdfLib.h>
#include <PcdSbPortIds.h>
#include <Register/PchRegs.h>
#include <Library/P2SbSocLib.h>
#include <Library/PeiItssLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PeiTsnInitLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Register/GpioAcpiDefines.h>

/**
  Load default zero and NULL values for the TSN handle.

  @param[in out]  TsnHandle      Pointer to TSN handle structure
  @param[in out]  TsnController  Pointer to TSN controller structure
  @param[in out]  TsnPrivate     Pointer to TSN private configuration structure
**/
STATIC
VOID
TsnHandleLoadDefaults (
  IN OUT  TSN_HANDLE          *TsnHandle,
  IN OUT  TSN_CONTROLLER      *TsnController,
  IN OUT  TSN_PRIVATE_CONFIG  *TsnPrivate,
  IN OUT  TSN_MAC_ADDR        *TsnPort
  )
{
  ZeroMem (TsnController, sizeof (TSN_CONTROLLER));
  TsnHandle->Controller = TsnController;

  ZeroMem (TsnPrivate, sizeof (TSN_PRIVATE_CONFIG));
  TsnHandle->PrivateConfig = TsnPrivate;

  ZeroMem (TsnPort, sizeof (TSN_MAC_ADDR));
  TsnHandle->PrivateConfig->Port = TsnPort;

  TsnHandle->Config = NULL;
}

/**
  Create the PCR accesses for the given TSN controller.

  @param[in]      TsnIndex         Index of the TSN controller
  @param[in]      P2SbController   Pointer to the P2SB Controller
  @param[in]      P2SbAccess       Pointer to the Side band MMIO access protocol
  @param[in out]  TsnHandle        Pointer to the TSN handle structure

  @retval EFI_SUCCESS           - Completed successfully
          EFI_OUT_OF_RESOURCES  - Not enough resources to allocate P2SbCtrl for the context.
          EFI_INVALID_PARAMETER - P2SB_CONTROLLER or TSN_CONTROLLER is NULL or TsnIndex is incorrect
**/
STATIC
EFI_STATUS
TsnRegisterAccessInit (
  IN      UINT8                           TsnIndex,
  IN      P2SB_CONTROLLER                 *P2SbController,
  IN      P2SB_SIDEBAND_REGISTER_ACCESS   *P2SbAccess,
  IN OUT  TSN_HANDLE                      *TsnHandle
  )
{
  P2SB_PORT_16_ID   P2SbPid;

  if (P2SbController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: P2SbController is NULL\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  if (TsnHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: TsnHandle is NULL\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  TsnHandle->Controller->Segment        = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  TsnHandle->Controller->Bus            = DEFAULT_PCI_BUS_NUMBER_PCH;
  TsnHandle->Controller->Device         = TsnDevNumber (TsnIndex);
  TsnHandle->Controller->Function       = TsnFuncNumber (TsnIndex);
  TsnHandle->Controller->PciCfgBaseAddr = TsnPciCfgBase (TsnIndex);

  TsnHandle->Controller->Mmio = PcdGet32 (PcdSiliconInitTempMemBaseAddr);

  switch (TsnIndex) {
    case 0:
      P2SbPid.Pid16bit = PTL_A_SID_F2_PID_TSN0;
      break;

    case 1:
      P2SbPid.Pid16bit = PTL_A_SID_F2_PID_TSN1;
      break;

    case 2:
      P2SbPid.Pid16bit = PTL_A_SID_F2_PID_TSN2;
      break;

    case 3:
      P2SbPid.Pid16bit = PTL_A_SID_F2_PID_TSN3;
      break;

    default:
      DEBUG ((DEBUG_ERROR, "TSN Index %d not supported\n", TsnIndex));
      return EFI_INVALID_PARAMETER;
  }

  BuildP2SbSidebandAccess (
    P2SbController,
    P2SbPid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    TRUE,
    P2SbAccess
    );
  TsnHandle->Controller->TsnPcrAccess = &P2SbAccess->Access;

  return EFI_SUCCESS;
}

/**
  Enable the TSN GPIOs.

  @param[in]  TsnIndex   Index of the TSN controller
**/
STATIC
VOID
TsnEnableGpio (
  IN  UINT8   TsnIndex
  )
{
}

/**
  Get MAC address from TSN_CONFIG based on the TSN port index

  @param[in]      TsnIndex    Index of the TSN controller
  @param[in out]  TsnHandle   Pointer to the TSN handle structure
**/
VOID
TsnGetMacAddress (
  IN      UINT8        TsnIndex,
  IN OUT  TSN_HANDLE   *TsnHandle
  )
{
  TsnHandle->PrivateConfig->Port = &TsnHandle->Config->Port[TsnIndex];
  DEBUG ((DEBUG_INFO,
          "TSN[%d] Dev: 0x%x, Fun: 0x%x, MAC high: 0x%x, MAC low: 0x%x\n",
          TsnIndex, TsnDevNumber (TsnIndex), TsnFuncNumber (TsnIndex), TsnHandle->PrivateConfig->Port->MacAddr[1], TsnHandle->PrivateConfig->Port->MacAddr[0]
          ));
}

/**
  Initialize the TSN devices.

  @param[in] SiPolicy   The Silicon Policy PPI instance
**/
VOID
PtlPcdTsnInit (
  IN  SI_POLICY_PPI   *SiPolicy
  )
{
  EFI_STATUS                      Status;
  UINT8                           TsnIndex;
  TSN_HANDLE                      TsnHandle;
  TSN_CONTROLLER                  TsnController;
  TSN_CONFIG                      *TsnConfig;
  TSN_PRIVATE_CONFIG              TsnPrivate;
  P2SB_CONTROLLER                 P2SbCtrl;
  P2SB_SIDEBAND_REGISTER_ACCESS   P2SbAccess;
  TSN_MAC_ADDR                    TsnPort;
  BOOLEAN                         IsTsnEnabled;

  DEBUG ((DEBUG_VERBOSE, "%a start\n", __FUNCTION__));

  if (SiPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: SiPolicy is NULL\n", __FUNCTION__));
    return;
  }

  // Initialize handler with default zero values and NULL configuration pointer
  TsnHandleLoadDefaults (&TsnHandle, &TsnController, &TsnPrivate, &TsnPort);

  // Get P2SB controller
  Status = PtlPcdGetFirstP2SbController (&P2SbCtrl);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get P2SB controller (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gTsnConfigGuid, (VOID *) &TsnConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get TSN configuration (Status: %d)\n", __FUNCTION__, Status));
    return;
  }
  TsnHandle.Config = TsnConfig;

  IsTsnEnabled = FALSE;
  for (TsnIndex = 0; TsnIndex < PtlPcdGetTsnMaxCtrlNum (); TsnIndex++) {
    if (TsnConfig->Enable[TsnIndex]) {
      IsTsnEnabled = TRUE;
      break;
    }
  }

  if (!IsTsnEnabled) {
    DEBUG ((DEBUG_INFO, "%a: TSN ports are disabled\n", __FUNCTION__));
    return;
  }

  // Initialize each TSN controller
  for (TsnIndex = 0; TsnIndex < PtlPcdGetTsnMaxCtrlNum (); TsnIndex++) {
    if (TsnConfig->Enable[TsnIndex]) {
      Status = TsnRegisterAccessInit (TsnIndex, &P2SbCtrl, &P2SbAccess, &TsnHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: Failed to initialize TSN port %d register access (Status: %d)\n", __FUNCTION__, TsnIndex, Status));
        break;
      }

      // Enable GPIOs
      TsnEnableGpio (TsnIndex);

      // Get MAC address
      TsnGetMacAddress (TsnIndex, &TsnHandle);

      // Initialize TSN controller
      TsnInit (&TsnHandle);
    }
  }

  DEBUG ((DEBUG_VERBOSE, "%a end\n", __FUNCTION__));
}
