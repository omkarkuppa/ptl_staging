 /** @file
  PTL PCD ME Init specific routines for PEI

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

#include <Uefi.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PmcLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/MeChipsetLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/HeciRegs.h>
#include <MeHandle.h>
#include <MeDefines.h>
#include "MePcdInitLib.h"
#include <PciSbdf.h>

/**
  Enable/Disable CSME HECI devices on PCD

  @param[in] This                 The ME_HANDLE instance.
  @param[in] WhichDevice          ME device selection
  @param[in] DeviceFuncCtrl       Function control
**/
VOID
EFIAPI
PtlPcdSetCsmeHeciDeviceState (
  IN ME_HANDLE                    *This,
  IN HECI_DEVICE                  WhichDevice,
  IN HECI_DEVICE_FUNC_CTRL        DeviceFuncCtrl
  )
{
  PCI_SBDF                        HeciSbdf;

  HeciSbdf.PciSegmentLibAddress = This->Callbacks->GetCsmeHeciPciCfgBase (WhichDevice);

  switch (WhichDevice) {
    case HECI1:
    case HECI2:
    case HECI3:
      if (DeviceFuncCtrl == Enabled) {
        PtlPcdPsfEnableHeci ((UINT8) HECI_DEVICE_TO_INDEX (WhichDevice));
        ClearD0I3Bit (WhichDevice);
      } else {
        SetD0I3Bit (WhichDevice);
        PtlPcdPsfDisableHeci ((UINT8) HECI_DEVICE_TO_INDEX (WhichDevice));
      }
      break;
    case HECI4:
      if (DeviceFuncCtrl == Enabled) {
        DEBUG ((DEBUG_WARN, "[HECI4] Enabled mode is not supported\n"));
        ASSERT (FALSE);
        return;
      } else {
        SetD0I3Bit (HECI4);
        PtlPcdPsfDisableHeci ((UINT8) HECI_DEVICE_TO_INDEX (HECI4));
      }
      break;
    case IDER:
      if (DeviceFuncCtrl == Enabled) {
        DEBUG ((DEBUG_WARN, "[IDER] Enabled mode is not supported\n"));
        ASSERT (FALSE);
      } else {
        PciSegmentWrite32 (HeciSbdf.PciSegmentLibAddress + R_ME_CFG_PMCSR, (UINT32) V_ME_CFG_PMCSR_PS);
        PtlPcdPsfDisableIder ();
      }
      break;
    case SOL:
      if (DeviceFuncCtrl == Enabled) {
        PtlPcdPsfEnableSol ();
      } else {
        PciSegmentWrite32 (HeciSbdf.PciSegmentLibAddress + R_ME_CFG_PMCSR, (UINT32) V_ME_CFG_PMCSR_PS);
        PtlPcdPsfDisableSol ();
      }
      break;
    default:
      break;
  }

  DEBUG ((DEBUG_INFO, "%a CSME device %d:%d:%d\n",
    DeviceFuncCtrl == Enabled ? "Enabling" : "Disabling",
    HeciSbdf.Fields.Bus,
    HeciSbdf.Fields.Dev,
    HeciSbdf.Fields.Func
    ));
}

/**
  Get the interrupt pin for the given ME device

  @param[in] SiPolicy       Pointer to policy
  @param[in] Device         Device number
  @param[in] Function       Function number

  @retval Interrupt pin
**/
STATIC
UINT8
EFIAPI
PtlPcdGetCsmeHeciItssDevIntPin (
  IN  CONST SI_POLICY_PPI  *SiPolicy,
  IN  UINT8                Device,
  IN  UINT8                Function
  )
{
  return ItssGetDevIntPin (SiPolicy, Device, Function);
}

/**
  Configure CSME HECI device interrupts

  @param[in] This                 The ME_HANDLE instance.
  @param[in] SiPolicy             Pointer to Si policy
  @param[in] HeciSbdf             HECI device SBDF

  @retval EFI_SUCCESS             The operation was successful
  @retval EFI_INVALID_PARAMETER   Wrong DeviceSel parameter
  @retval EFI_ABORTED             Interrupt pin programming not required
  @retval EFI_DEVICE_ERROR        Not able to write data
**/
STATIC
EFI_STATUS
EFIAPI
PtlPcdSetCsmeHeciInterruptPin (
  IN        ME_HANDLE            *This,
  IN  CONST SI_POLICY_PPI        *SiPolicy,
  IN        PCI_SBDF             HeciSbdf
  )
{
  UINT8                          InterruptPin;
  UINT32                         Data32;
  UINT32                         Offset;
  P2SB_CONTROLLER                P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS  HeciPcrAccess;

  if (HeciSbdf.Fields.Func > HECI4) {
    return EFI_INVALID_PARAMETER;
  }

  InterruptPin = PtlPcdGetCsmeHeciItssDevIntPin (SiPolicy, (UINT8) HeciSbdf.Fields.Dev, (UINT8) HeciSbdf.Fields.Func);
  if (InterruptPin == PciSegmentRead8 (HeciSbdf.PciSegmentLibAddress + PCI_INT_PIN_OFFSET)) {
    return EFI_ABORTED;
  }

  This->Callbacks->GetCsmeHeciSbAccess (This, HeciSbdf.Fields.Func, &P2SbController, &HeciPcrAccess);

  Offset = (HeciSbdf.Fields.Func == SOL) ? R_HOST_KT_PCR_KTHIPINR : R_HOST_HECI1_SB_PCR_IPIN;
  HeciPcrAccess.Access.Write32 (&HeciPcrAccess.Access, Offset, InterruptPin);
  Data32 = HeciPcrAccess.Access.Read32 (&HeciPcrAccess.Access, Offset);

  //
  // Check to see if the operation was successful
  //
  if (Data32 != InterruptPin) {
    DEBUG ((DEBUG_ERROR, "ERROR: %a: Desired interrupt pin is 0x%x, readback value is 0x%x\n", __FUNCTION__, InterruptPin, Data32));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Initialize PCD ME handle structure

  @param[in] MeHandlePrivate        Pointer to the ME Private Handle
**/
STATIC
VOID
PtlPcdInitMeHandle (
  IN ME_HANDLE              *MeHandlePrivate
  )
{
  MeHandlePrivate->HeciTempMmio                        = PCH_TEMP_BASE_ADDRESS;
  MeHandlePrivate->PmcPwrmBase                         = PmcGetPwrmBase ();
  MeHandlePrivate->XhciPciCfgBase                      = PtlPcdGetXhciPciCfgBase ();
  MeHandlePrivate->UsbrKvmPortNum                      = PtlPcdGetUsbrKvmPortNum ();
  MeHandlePrivate->UsbrStoragePortNum                  = PtlPcdGetUsbrStoragePortNum ();
  MeHandlePrivate->Callbacks->GetCsmeHeciSbAccess      = PtlPcdGetCsmeHeciSbAccess;
  MeHandlePrivate->Callbacks->SetCsmeHeciDeviceState   = PtlPcdSetCsmeHeciDeviceState;
  MeHandlePrivate->Callbacks->SetCsmeHeciInterruptPin  = PtlPcdSetCsmeHeciInterruptPin;
  MeHandlePrivate->Callbacks->GetCsmeHeciPciCfgBase    = PtlPcdGetCsmeHeciPciCfgBase;
  MeHandlePrivate->Callbacks->GetPseHeciPciCfgBase     = PtlPcdGetPseHeciPciCfgBase;
}

/**
  Initializes the ME Handle and allocates the corresponding PPI descriptor.

  This function allocates and initializes the ME Handle structure and its callbacks.
  It also allocates the PPI descriptor for the ME Handle and sets it up for installation.
  The ME Handle is initialized differently based on whether the PCH is attached or not.

  @param[out] MeHandlePpiDescriptor  A pointer to a pointer to the EFI_PEI_PPI_DESCRIPTOR that will be updated with the ME Handle PPI descriptor.

  @retval EFI_SUCCESS                The ME Handle and its PPI descriptor were successfully initialized and allocated.
  @retval EFI_OUT_OF_RESOURCES       Memory allocation failed for the ME Handle, its callbacks, or the PPI descriptor.
  @retval EFI_NOT_FOUND              The PCH configuration HOB could not be found when the PCH is attached.
**/
EFI_STATUS
InitializeMeHandlePpi (
  EFI_PEI_PPI_DESCRIPTOR    **MeHandlePpiDescriptor
  )
{
  ME_HANDLE   *MeHandle;

  MeHandle = (ME_HANDLE *) AllocateZeroPool (sizeof (ME_HANDLE));
  if (MeHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Allocating memory for MeHandle failed\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  MeHandle->Callbacks = (ME_CALLBACKS *) AllocateZeroPool (sizeof (ME_CALLBACKS));
  if (MeHandle->Callbacks == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Allocating memory for MeCallbacks failed\n", __FUNCTION__));
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  PtlPcdInitMeHandle (MeHandle);

  //
  // Allocate the PPI descriptor and the ME Handle in memory
  //
  *MeHandlePpiDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (*MeHandlePpiDescriptor == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Allocating memory for MeHandlePpiDescriptor failed\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Configure the MeHandle PPI descriptor with the newly created MeHandle and install it
  //
  (*MeHandlePpiDescriptor)->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  (*MeHandlePpiDescriptor)->Guid  = &gMeHandlePpiGuid;
  (*MeHandlePpiDescriptor)->Ppi   = MeHandle;

  return EFI_SUCCESS;
}

/**
  Reinitializes the ME Handle PPI after memory is available.

  This function locates the existing ME Handle PPI and replaces it with a newly initialized instance.
  This is typically done after system memory has been initialized and the ME Handle needs to be updated to reflect the new state of the system.

  @param[in] PeiServices          A pointer to the PEI Services Table.
  @param[in] NotifyDescriptor     A pointer to the descriptor for the callback notification.
  @param[in] Ppi                  A pointer to the PPI that was installed.

  @retval EFI_SUCCESS             The ME Handle PPI was successfully reinitialized.
  @retval EFI_NOT_FOUND           The existing ME Handle PPI could not be located.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval Other                   The ME Handle PPI could not be reinstalled due to an error.

  @note This function is typically used as a notification callback and is triggered by a PEI phase event.
**/
EFI_STATUS
EFIAPI
MeHandleReinitializePpi (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  )
{
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *OldMeHandlePpiDescriptor;
  ME_HANDLE                 *OldMeHandlePrivate;
  EFI_PEI_PPI_DESCRIPTOR    *MeHandlePpiDescriptor;

  Status = PeiServicesLocatePpi (
             &gMeHandlePpiGuid,
             0,
             &OldMeHandlePpiDescriptor,
             (VOID **) &OldMeHandlePrivate
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: MeHandle was not able to be reinitialized for post-mem\n", __FUNCTION__));
    return Status;
  }

  Status = InitializeMeHandlePpi (&MeHandlePpiDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: MeHandle initialization failed! Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = PeiServicesReInstallPpi (OldMeHandlePpiDescriptor, MeHandlePpiDescriptor);
  DEBUG ((DEBUG_INFO, "%a: MeHandlePpi reinstallation status: %r\n", __FUNCTION__, Status));

  return Status;
}

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mReinitializeMeHandleNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  MeHandleReinitializePpi
  };

/**
  Installs the ME Handle PPI.

  This function initializes and installs the ME Handle PPI. It also sets up a notification for reinitializing the ME Handle.
  If any step of the process fails, an error message is logged.

  @retval VOID

  @note This function is typically used in the PEI phase of UEFI firmware initialization to provide a handle for ME-related operations.
**/
VOID
InstallMeHandle (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *MeHandlePpiDescriptor;

  Status = InitializeMeHandlePpi (&MeHandlePpiDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: MeHandle initialization failed! Status = %r\n", __FUNCTION__, Status));
    return;
  }

  Status = PeiServicesInstallPpi (MeHandlePpiDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: MeHandle PPI installation failed! Status = %r\n", __FUNCTION__, Status));
    return;
  }
  Status = PeiServicesNotifyPpi (&mReinitializeMeHandleNotify);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: MeHandle PPI reinstallation notify failed! Status = %r\n", __FUNCTION__, Status));
  }
}

/**
  Retrieves the ME_HANDLE using the ME Handle PPI.

  This function locates the ME Handle PPI and returns a pointer to the ME_HANDLE structure.
  If the PPI cannot be located, the function returns an error and sets the MeHandle to NULL.

  @param[out] MeHandle      A pointer to a pointer to the ME_HANDLE structure that will be updated with the ME Handle PPI pointer.

  @retval EFI_SUCCESS       The ME_HANDLE was successfully retrieved.
  @retval EFI_NOT_FOUND     The ME Handle PPI could not be located.
  @retval Other             An error occurred when trying to locate the ME Handle PPI.
**/
EFI_STATUS
GetMeHandle (
  OUT ME_HANDLE    **MeHandle
  )
{
  EFI_STATUS    Status;

  Status = PeiServicesLocatePpi (
             &gMeHandlePpiGuid,
             0,
             NULL,
             (VOID **) MeHandle
             );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a () LocatePpi failed! Status = %r\n", __FUNCTION__, Status));
    *MeHandle = NULL;
  }

  return Status;
}
