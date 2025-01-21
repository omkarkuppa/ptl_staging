/** @file
  Connection manager implementation

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

@par Specification
**/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4PlatformHob.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4HiCoreLib.h>
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/CmEvtLib.h>
#include <Library/Usb4HrInst.h>
#include <Library/Usb4DebugLib.h>
#include "Usb4CmDxe.h"
#include "Usb4Hr.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

PUSB4_HR_INSTANCE   mUsb4HrInst[USB4_HR_SUPPORT_MAX] = {0};
UINT32              mUsb4HrCount = 0;

//
// USB4 Maximum topology depth, constraint USB4 CM to support two level of dock by default
// in pre-boot phase. Could be adjusted according to platform bus resource allocation.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT8     mUsb4TopologyDepth = 2;

/**
  Unpower USB4 host router in platform.
**/
VOID Usb4CmUnpowerHr (
  VOID
  )
{
  UINT32              HrIndex;

  DEBUG ((DEBUG_INFO, "Usb4CmUnpowerHr entry\n"));

  for (HrIndex = 0; HrIndex < mUsb4HrCount; HrIndex++) {
    Usb4HrUnPower (mUsb4HrInst[HrIndex]);
  }

  DEBUG ((DEBUG_INFO, "Usb4CmUnpowerHr exit\n"));
}

/**
  Stop Connection Manager on USB4 host router.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.
**/
VOID
CmStop (
  IN  USB4_HR_INSTANCE    *Usb4Hr
  )
{
  DEBUG ((DEBUG_INFO, "CmStop entry - Usb4Hr = %p\n", Usb4Hr));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "CmStop: Invalid parameter\n"));
    goto Exit;
  }

  //
  // Check and process the pending hot plug events in Rx event queue before stopping CM.
  //
  if (Usb4Hr->CmStart) {
    CmProcessRxEvents (Usb4Hr);
  }

  //
  // Unbind Domain from USB4 host router.
  //
  if (Usb4Hr->Usb4Domain != NULL) {
    Usb4DomainUnbindHr (Usb4Hr->Usb4Domain);
  }

  //
  // Stop USB4 host router.
  // Un-init PCI config space and free MMIO resources.
  // PCI resources will be assigned by PCI bus driver after PCI enumeration.
  //
  Usb4HrStop (Usb4Hr);

  Usb4Hr->CmStart = FALSE;

Exit:
  DEBUG ((DEBUG_INFO, "CmStop exit\n"));
}

/**
  Stop CM and release resources.
  The resources will be re-assigned in PCI enumeration phase.
**/
VOID
Usb4CmStop (
  VOID
  )
{
  UINT32              HrIndex;

  DEBUG ((DEBUG_INFO, "Usb4CmStop entry\n"));

  for (HrIndex = 0; HrIndex < mUsb4HrCount; HrIndex++) {
    CmStop (mUsb4HrInst[HrIndex]);
  }

  DEBUG ((DEBUG_INFO, "Usb4CmStop exit\n"));
}

/**
  Connection Manager destroy.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.
**/
VOID
CmDestroy (
  IN  USB4_HR_INSTANCE    *Usb4Hr
  )
{
  DEBUG ((DEBUG_INFO, "CmDestroy entry - Usb4Hr = %p\n", Usb4Hr));

  if (Usb4Hr != NULL) {
    //
    // Destroy USB4 domain instance.
    //
    if (Usb4Hr->Usb4Domain != NULL) {
      Usb4DomainDestroy (Usb4Hr->Usb4Domain);
    }

    //
    // Destroy USB4 host router instance.
    //
    Usb4HrDestroy (Usb4Hr);
  }

  DEBUG ((DEBUG_INFO, "CmDestroy exit\n"));
}

/**
  Destroy all CM resources and instances.
**/
VOID
Usb4CmDestroy (
  VOID
  )
{
  UINT32              HrIndex;
  UINT32              HrCount;

  DEBUG ((DEBUG_INFO, "Usb4CmDestroy entry\n"));

  HrCount = mUsb4HrCount;
  for (HrIndex = 0; HrIndex < HrCount; HrIndex++) {
    CmDestroy (mUsb4HrInst[HrIndex]);
    mUsb4HrInst[HrIndex] = NULL;
    mUsb4HrCount--;
  }

  DEBUG ((DEBUG_INFO, "Usb4CmDestroy exit\n"));
}

/**
  USB4 CM execution for USB4 host routers in the platform.

  @param[in] ImageHandle      - ImageHandle of the loaded driver.
  @param[in] Usb4PlatformInfo - Pointer to the platform USB4 host router information.

  @retval EFI_SUCCESS           - Software CM execution success.
  @retval EFI_UNSUPPORTED       - General USB4 CM execution failure.
  @retval EFI_OUT_OUT_RESOURCES - Insufficient resources to execute CM.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4CmExecute (
  IN EFI_HANDLE            ImageHandle,
  IN USB4_PLATFORM_INFO    *Usb4PlatformInfo
  )
{
  EFI_STATUS                Status;
  USB4_HR_INSTANCE          *Usb4Hr;
  UINT32                    HrIndex;
  UINT32                    HrCount;
  PCI_DEV_BDF               *Bdf;

  DEBUG ((DEBUG_INFO, "Usb4CmExecute entry\n"));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_CM_EXECUTE_ENTRY, 0, 0);

  Status = EFI_UNSUPPORTED;

  if ((ImageHandle == NULL) || (Usb4PlatformInfo == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4CmExecute: Invalid parameter (%p, %p)\n", ImageHandle, Usb4PlatformInfo));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check if the USB4 HR count exceeds the maximum HR count.
  //
  HrCount = Usb4PlatformInfo->Usb4HrCount;
  if (HrCount > USB4_HR_SUPPORT_MAX) {
    DEBUG ((DEBUG_ERROR, "USB4 HR count = %d, only %d HRs can be supported\n", HrCount, USB4_HR_SUPPORT_MAX));
    HrCount = USB4_HR_SUPPORT_MAX;
  }

  //
  // Update Maximum Depth of Tocology
  //
  mUsb4TopologyDepth = Usb4PlatformInfo->Usb4MaxTopologyDepth;
  if (mUsb4TopologyDepth > USB4_MAX_TOPOLOGY_DEPTH) {
    DEBUG ((DEBUG_ERROR, "USB4 Maximum Topology Depth = %d, only %d levels can be supported\n", mUsb4TopologyDepth, USB4_MAX_TOPOLOGY_DEPTH));
    mUsb4TopologyDepth = USB4_MAX_TOPOLOGY_DEPTH;
  }

  //
  // Create USB4 host router instance and connection manager for each USB4 host router on platform.
  //
  for (HrIndex = 0; HrIndex < HrCount; HrIndex++) {
    Bdf = &(Usb4PlatformInfo->Usb4Hr[HrIndex].NhiBdf);
    DEBUG ((DEBUG_INFO, "Create USB4 HR instance for BDF (0x%02x/0x%02x/0x%02x)\n", Bdf->Bus, Bdf->Dev, Bdf->Func));
    //
    // Create and initialize USB4 host router instance for USB4 host interface implementation.
    //
    Status = Usb4HrCreate (ImageHandle, &(Usb4PlatformInfo->Usb4Hr[HrIndex]), &Usb4Hr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4CmExecute: Create USB4 HR failure, %r\n", Status));
      continue;
    }

    mUsb4HrInst[mUsb4HrCount] = Usb4Hr;
    mUsb4HrCount++;

    //
    // Build domain topology on USB4 host router
    //
    Status = Usb4HrBuildTopology (Usb4Hr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4CmExecute: Fail to build domain topology for USB4 HR, %r\n", Status));
      Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_CM_DESTROY_ERROR, (UINT32) Status, 0);
      CmDestroy (Usb4Hr);
      mUsb4HrInst[mUsb4HrCount] = NULL;
      mUsb4HrCount--;
      continue;
    }

    Usb4Hr->CmStart = TRUE;
  }

Exit:
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_CM_EXECUTE_EXIT, (UINT32) Status, 0);
  DEBUG ((DEBUG_INFO, "Usb4CmExecute exit\n"));
  return Status;
}