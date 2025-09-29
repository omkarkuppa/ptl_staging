/** @file
  This file provides initialization code for Gpio V2 Controller DXE module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2sbPrivateLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/GpioV2ServicesCommonLib.h>
#include <GpioV2ControllerInterface.h>

#include <Register/GpioV2ChipsetId.h>
#include <Library/PcdGpioTopologyLib.h>
#include <Library/P2SbSocLib.h>
#include <Register/GpioAcpiDefines.h>
#include <GpioV2Pwm.h>
#include <GpioV2AccessControl.h>

/**
  This function initializes GPIOV2_INTERFACE Structure.

  @param[in]  GpioInterface     Pointer to GPIOV2_INTERFACE.

  @retval     EFI_SUCCESS                The function completed successfully.
  @retval     EFI_INVALID_PARAMETER      Gpio Interface Pointer is NULL.
  @retval     EFI_OUT_OF_RESOURCES       Unabled to allocate memory.
  @retval     EFI_NOT_FOUND              Unable to Get P2SB access Pointer.
**/
EFI_STATUS
InternalGpioInterfaceConstructor (
  IN     GPIOV2_INTERFACE      *GpioInterface
  )
{
  EFI_STATUS  Status;
  UINT32      CommunityIndex;

  DEBUG ((DEBUG_INFO, "[GPIOV2]: %a\n", __FUNCTION__));

  if (GpioInterface == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Signature                          = GPIOV2_INTERFACE_SIGNATURE;
  GpioInterface->Public.Revision                    = GPIOV2_CONTROLLER_REVISION;
    CopyMem (GpioInterface->Public.Hid, GPIO_HID_PTL_PCD_P, GPIOV2_CONTROLLER_HID_LENGTH);
    GpioInterface->Public.Uid                         = 0;
    GpioInterface->Private.ChipsetId                  = GPIOV2_PTL_PCD_CHIPSET_ID;
    GpioInterface->Private.CommunitiesNum             = PtlPcdGpioGetCommunitiesNum ();
    GpioInterface->Private.Communities                = PtlPcdGpioGetCommunities ();
    GpioInterface->Private.NativeSignalData           = PtlPcdGpioGetNativeSignals ();
    GpioInterface->Private.NativeSignalDataNum        = PtlPcdGpioGetNativeSignalsNum ();
  GpioInterface->Private.CommunityAccess            = (P2SB_SIDEBAND_REGISTER_ACCESS *) AllocateZeroPool (sizeof (P2SB_SIDEBAND_REGISTER_ACCESS) * GpioInterface->Private.CommunitiesNum);
  GpioInterface->Private.GpioV2PadOwnRegRev         = GpioV2PadOwnRegRev1;
  GpioInterface->Private.PadCfgLockCntrlGrpIndex    = GPIOV2_ACCESS_CONTROL_SAI_GROUPSMMACC_LOCK_REGISTERS;

  if (GpioInterface->Private.CommunityAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: Allocating memory for CommunityAccess failed\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  GpioInterface->Private.CommunityRegisterBase = AllocateZeroPool (sizeof (UINT64) * GpioInterface->Private.CommunitiesNum);
  if (GpioInterface->Private.CommunityRegisterBase == NULL) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: CommunityRegisterAccessBase allocation failed\n"));
    FreePool (GpioInterface->Private.CommunityAccess);
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = PtlPcdGetFirstP2SbController (&GpioInterface->Private.P2SbController);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: %a error (%d)\n", __FUNCTION__, Status));
    FreePool (GpioInterface->Private.CommunityAccess);
    FreePool (GpioInterface->Private.CommunityRegisterBase);
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    BuildP2SbSidebandAccess (
      &GpioInterface->Private.P2SbController,
      (P2SB_PID) (GpioInterface->Private.Communities[CommunityIndex].Pid),
      0,
      P2SbMemory,
      P2SbMmioAccess,
      TRUE,
      &GpioInterface->Private.CommunityAccess[CommunityIndex]
      );
    GpioInterface->Private.CommunityRegisterBase[CommunityIndex] = P2sbGetMmioBase (&GpioInterface->Private.P2SbController, (P2SB_PID) GpioInterface->Private.Communities[CommunityIndex].Pid);
  }

  GpioV2ServicesInit (&GpioInterface->Public);

  // GpioV2 PWM init
  /**if (GpioInterface->Pwm != NULL) {
    CopyMem (GpioInterface->Pwm->Hid, GpioInterface->Public.Hid, GPIOV2_CONTROLLER_HID_LENGTH);
    GpioInterface->Pwm->Uid = GpioInterface->Public.Uid;
    GpioV2PwmInit (GpioInterface->Pwm);
  } else {
    DEBUG ((DEBUG_INFO, "[%a] Memory not allocated for PWM protocol, skipping PWM init.\n", __FUNCTION__));
  }**/
  CopyMem (GpioInterface->Pwm.Hid, GpioInterface->Public.Hid, GPIOV2_CONTROLLER_HID_LENGTH);
  GpioInterface->Pwm.Uid = GpioInterface->Public.Uid;
  GpioV2PwmInit (&(GpioInterface->Pwm));

  return EFI_SUCCESS;
}
