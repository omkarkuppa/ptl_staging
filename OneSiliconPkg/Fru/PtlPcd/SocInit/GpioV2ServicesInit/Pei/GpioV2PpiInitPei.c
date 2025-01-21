/** @file
  This file provides initialization code for Gpio V2 Controller PEI module

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
#include <PiPei.h>
#include <Library/PeimEntryPoint.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/P2sbPrivateLib.h>
#include <Library/PcdGpioTopologyLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/GpioV2ServicesCommonLib.h>
#include <Register/GpioV2ChipsetId.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/IocLib.h>
#include <Register/Sai.h>
#include <GpioV2AccessControl.h>
#include <Library/PmcLib.h>
#include <Library/P2SbSocLib.h>
#include <GpioV2Pwm.h>

/**
  This procedure sets Gpio Group to GPE mapping

  @param[in] This                 Gpio Services
  @param[in] GpioGroup0           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW0
  @param[in] GpioGroup1           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW1
  @param[in] GpioGroup2           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
SetGroupToGpeMapping (
  IN GPIOV2_SERVICES               *This,
  IN GPIOV2_PAD_GROUP              GpioGroup0,
  IN GPIOV2_PAD_GROUP              GpioGroup1,
  IN GPIOV2_PAD_GROUP              GpioGroup2
  );

/**
  This procedure sets GPIO Group mapping in PMC GPIO_CFG and GPIO MISCCFG

  @param[in] GpioServices         Gpio Services
  @param[in] GpioGroup0           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW0
  @param[in] GpioGroup1           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW1
  @param[in] GpioGroup2           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Not found GPE controller mapping value for input GPIO group
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
PtlPcdSetGpeMapping (
  IN GPIOV2_SERVICES              *GpioServices,
  IN GPIOV2_PAD_GROUP             GpioGroup0,
  IN GPIOV2_PAD_GROUP             GpioGroup1,
  IN GPIOV2_PAD_GROUP             GpioGroup2
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  UINT8             CommunityIndex;
  UINT8             GroupIndex;
  UINT32            PmcGpioCfgDwX[3];
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, "[%a]: Start\n", __FUNCTION__));

  // Inside GpioV2 GPE setter there is already input parameters validation
  Status = SetGroupToGpeMapping (
    GpioServices,
    GpioGroup0,
    GpioGroup1,
    GpioGroup2
  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[%a]: Invalid input arguments\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  CommunityIndex   = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup0);
  GroupIndex       = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup0);
  PmcGpioCfgDwX[0] = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].GroupToGpeMapping.GpeControllerDwxVal;

  CommunityIndex   = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup1);
  GroupIndex       = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup1);
  PmcGpioCfgDwX[1] = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].GroupToGpeMapping.GpeControllerDwxVal;

  CommunityIndex   = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup2);
  GroupIndex       = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup2);
  PmcGpioCfgDwX[2] = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].GroupToGpeMapping.GpeControllerDwxVal;

  if ((PmcGpioCfgDwX[0] == PmcGpioCfgDwX[1]) ||
      (PmcGpioCfgDwX[1] == PmcGpioCfgDwX[2]) ||
      (PmcGpioCfgDwX[0] == PmcGpioCfgDwX[2])) {
    DEBUG ((DEBUG_INFO, "[%a]: GPE controller mapped values are not unique! \
            Check uniqueness of input group and GPIO topology.\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  PwrmPmcSetGpioGpe (
    PmcGetPwrmBase (),
    PmcGpioCfgDwX[0],
    PmcGpioCfgDwX[1],
    PmcGpioCfgDwX[2]
  );

  DEBUG ((DEBUG_INFO, "[%a]: End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

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
  IN     GPIOV2_INTERFACE       *GpioInterface
  )
{
  EFI_STATUS                          Status;
  UINT32                              CommunityIndex;
  GPIOV2_ACCESS_CONTROL_SAI_DW_DATA   AcData;
  UINT32  UnlockDataLength;
  VOID    *Hob;

  DEBUG ((DEBUG_INFO, "[GPIOV2][PEI]: %a\n", __FUNCTION__));

  if (GpioInterface == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Signature                          = GPIOV2_INTERFACE_SIGNATURE;
  GpioInterface->Public.Revision                    = GPIOV2_CONTROLLER_REVISION;
  CopyMem (GpioInterface->Public.Hid, GPIO_HID_PTL_PCD_P, GPIOV2_CONTROLLER_HID_LENGTH);
  GpioInterface->Private.ChipsetId                  = GPIOV2_PTL_PCD_CHIPSET_ID;
  GpioInterface->Public.Uid                         = 0;
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
    FreePool (GpioInterface->Private.CommunityRegisterBase);
    FreePool (GpioInterface->Private.CommunityAccess);
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

  Hob = GetFirstGuidHob (&gGpioV2UnlockHobGuidPch0);
  if (Hob == NULL) {
    UnlockDataLength = 0;
    for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
      UnlockDataLength += GpioInterface->Private.Communities[CommunityIndex].GroupsNum * sizeof (GPIOV2_UNLOCK_GROUP_DATA);
    }
    GpioInterface->Private.UnlockData = BuildGuidHob (&gGpioV2UnlockHobGuidPch0, UnlockDataLength);
    ZeroMem (GpioInterface->Private.UnlockData, UnlockDataLength);
  } else {
    //
    // If HOB is already present on the system we simply need to refresh the pointer post
    // memory migration.
    //
    GpioInterface->Private.UnlockData = GET_GUID_HOB_DATA (Hob);
  }

  GpioV2ServicesInit (&GpioInterface->Public);

  //
  // In order to properly lock gpio registers enable POSTBOOT_SAI
  // access to lock registers even after SetBiosDone flag is set
  //
  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    GetAccessControl (&GpioInterface->Public, CommunityIndex, GpioInterface->Private.PadCfgLockCntrlGrpIndex, &AcData);
    AcData.Policy |= B_SAI_HOSTIA_POSTBOOT;
    AcData.Write  |= B_SAI_HOSTIA_POSTBOOT;
    SetAccessControl (&GpioInterface->Public, CommunityIndex, GpioInterface->Private.PadCfgLockCntrlGrpIndex, &AcData);
  }

  GpioInterface->Public.SetGroupToGpeMapping = PtlPcdSetGpeMapping;

  // GpioV2 PWM init
  if (GpioInterface->Pwm != NULL) {
    CopyMem (GpioInterface->Pwm->Hid, GpioInterface->Public.Hid, GPIOV2_CONTROLLER_HID_LENGTH);
    GpioInterface->Pwm->Uid = GpioInterface->Public.Uid;
    GpioV2PwmInit (GpioInterface->Pwm);
  } else {
    DEBUG ((DEBUG_INFO, "[%a] Memory not allocated for PWM protocol, skipping PWM init.\n", __FUNCTION__));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GpioReinitializePpi (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS          Status;
  GPIOV2_SERVICES     *GpioServices;

  Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return EFI_NOT_FOUND;
  }

  Status = InternalGpioInterfaceConstructor (GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices));

  return Status;
}

static EFI_PEI_NOTIFY_DESCRIPTOR  mReinitializeGpioPpiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  GpioReinitializePpi
};


/**
  The GPIOV2 PEI driver entry point - Intalls GPIOV2 Ppi.

  @param[in]  FileHandle        Handle of the file being invoked.
  @param[in]  PeiServices       Pointer to PEI Services table.

  @retval EFI_SUCCESS           The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER Any of the PEI PPI descriptors in the list do not have the
                                EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.
**/
EFI_STATUS
EFIAPI
GpioV2PpiPeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS              Status;
  EFI_PEI_PPI_DESCRIPTOR  *GpioPpiDescriptor;
  EFI_PEI_PPI_DESCRIPTOR  *GpioPwmPpiDescriptor;
  GPIOV2_INTERFACE        *GpioInterface;
  GPIOV2_SERVICES         *GpioServices;
  GPIOV2_PWM              *Pwm;

  Pwm = NULL;

  DEBUG ((DEBUG_INFO, "[GPIOV2][PEI]: Install PPI (HID: %a) Start\n", GPIO_HID_PTL_PCD_P));

  //
  // This check is needed in FSP dispatch mode since the wrapper instance of the GPIO
  // driver might have already installed Gpio Services.
  //
  Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  GpioPpiDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (GpioPpiDescriptor == NULL) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: Allocating memory for GpioPpiDescriptor failed\n"));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  GpioInterface = (GPIOV2_INTERFACE *) AllocateZeroPool (sizeof (GPIOV2_INTERFACE));
  if (GpioInterface == NULL) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: Allocating memory for GpioInterface failed\n"));
    FreePool (GpioPpiDescriptor);
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  GpioPwmPpiDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (GpioPwmPpiDescriptor == NULL) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: Allocating memory for GpioPwmPpiDescriptor failed\n"));
  } else {
    Pwm = (GPIOV2_PWM *)AllocateZeroPool (sizeof (GPIOV2_PWM));
    if (Pwm == NULL) {
      DEBUG ((DEBUG_ERROR, "[GPIOV2]: Allocating memory for Pwm failed\n"));
    }
  }
  GpioInterface->Pwm = Pwm;

  Status = InternalGpioInterfaceConstructor (GpioInterface);
  if (EFI_ERROR (Status)) {
    FreePool (GpioInterface);
    FreePool (GpioPpiDescriptor);
    if (GpioPwmPpiDescriptor != NULL) {
      FreePool (GpioPwmPpiDescriptor);
    }
    if (Pwm != NULL) {
      FreePool (Pwm);
    }
    return Status;
  }

  GpioPpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  GpioPpiDescriptor->Guid  = &gGpioV2PpiGuid;
  GpioPpiDescriptor->Ppi   = &GpioInterface->Public;

  //
  // Install the GPIO PPI
  //
  Status = PeiServicesInstallPpi (GpioPpiDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2]: Install PPI failed (%r)!\n", Status));
    FreePool (GpioInterface);
    FreePool (GpioPpiDescriptor);
    if (GpioPwmPpiDescriptor != NULL) {
      FreePool (GpioPwmPpiDescriptor);
    }
    if (Pwm != NULL) {
      FreePool (Pwm);
    }
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (GpioPwmPpiDescriptor != NULL) {
    GpioPwmPpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    GpioPwmPpiDescriptor->Guid  = &gGpioV2PwmPpiGuid;
    GpioPwmPpiDescriptor->Ppi   = GpioInterface->Pwm;

    //
    // Install the GPIO PWM PPI
    //
    if (Pwm != NULL) {
      Status = PeiServicesInstallPpi (GpioPwmPpiDescriptor);
      if (EFI_ERROR (Status)) {
        FreePool (GpioPwmPpiDescriptor);
        DEBUG ((DEBUG_ERROR, "[GPIOV2]: Install PWM PPI failed (%r)!\n", Status));
      }
    }
  }

  Status = PeiServicesNotifyPpi (&mReinitializeGpioPpiNotifyList);

  DEBUG ((DEBUG_INFO, "[GPIOV2][PEI]: Install PPI End\n"));

  return Status;
}
