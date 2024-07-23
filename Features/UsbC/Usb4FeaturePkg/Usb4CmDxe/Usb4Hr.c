/** @file
  USB4 host router routines used before PCI bus driver enumeration.

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
#include <Library/DebugLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4HrSrvLib.h>
#include <Library/Usb4HiCoreLib.h>
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/CmEvtLib.h>
#include <Library/Usb4HrInst.h>
#include <Library/Usb4DomainDefs.h>
#include <Library/Usb4DebugLib.h>
#include "Usb4Hr.h"
#include "Usb4IhrPci.h"
#include "Usb4DhrPci.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

/**
  Set host router state based on USB4 control fields.
  If PtOn is set to 0 in host router, PCIe tunneling is disabled for all downstream routers.
  If UtOn is set to 0 in host router, USB3 tunneling is disabled for all downstream routers.

  @param[in] Router   - Pointer to host router instance.
  @param[in] Usb4Ctrl - Pointer to USB4 control fields.

  @retval EFI_SUCCESS           - Set host router control fields successfully.
  @retval EFI_UNSUPPORTED       - Router is not host router.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HrSetUsb4Control (
  IN USB4_ROUTER    *Router,
  IN USB4_CONTROL   *Usb4Ctrl
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "Usb4HrSetUsb4Control entry\n"));

  if ((Router == NULL) || (Usb4Ctrl == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrSetUsb4Control: Invalid parameter (%p, %p)\n", Router, Usb4Ctrl));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Depth != 0) {
    DEBUG ((DEBUG_ERROR, "Usb4HrSetUsb4Control: Rt 0x%016llX is not host router\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Host Router can support PCIe Tunneling and USB3 Tunneling without setting Router CS5 PTO and UTO bit.
  // Router codes use these bits as one of conditions to determine whether PCIe/USB3 Tunneling should be turned on
  // for the downstream Router. Downstream Router will not turn on PTO or UTO if the upstream Router doesn't turn on
  // PTO or UTO.
  //
  // Set PtOn to 1 only if USB4 control PCIe bit is 1 and PCIe adapter is present in host router.
  // Set UtOn to 1 only if USB4 control USB3 bit is 1 and USB3 adapter is present in host router.
  //
  Router->State.PtOn = (Usb4Ctrl->Fields.Pcie == 1) ? Router->State.Pcie : 0;
  Router->State.UtOn = (Usb4Ctrl->Fields.Usb3 == 1) ? Router->State.Usb3 : 0;

  DEBUG ((DEBUG_INFO, "Host router PCIe Enable = %d, USB3 Enable = %d\n", Router->State.PtOn, Router->State.UtOn));
  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4HrSetUsb4Control exit\n"));
  return Status;
}

/**
  Create USB4 domain root router instance to represent host router in domain.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Create USB4 domain root successfully.
  @retval EFI_UNSUPPORTED       - Fail to create USB4 domain root.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HrCreateDomainRoot (
  IN USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS     Status;
  TOPOLOGY_ID    TopologyId;
  USB4_ROUTER    *Router;

  DEBUG ((DEBUG_INFO, "Usb4HrCreateDomainRoot entry\n"));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_ROOTRT_CREATE_ENTRY, 0, 0);

  Router = NULL;

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomainRoot: Null Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Create router instance to represent host router in domain.
  //
  TopologyId.Value = 0;
  Status = Usb4RtCreate (&TopologyId, 0, (USB4_DOMAIN_CONTEXT) Usb4Hr->Usb4Domain, &Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomainRoot: Create router instance failure, %r\n", Status));
    goto Exit;
  }
  Router->RouterType = USB4_ROUTER_HOST_ROUTER;

  //
  // Set Topology ID and Topology valid bit in router config space to enumerate the Router.
  // This must be done before any access to the router other than router config space.
  //
  Status = Usb4RtEnumerate (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomainRoot: Enumerate Router failure, %r\n", Status));
    goto Exit;
  }

  //
  // Scan router adapters and adapter capabilities
  //
  Status = Usb4RtScanAdapters (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomainRoot: Enumerate router adapters failure 0x%0x\n", Status));
    goto Exit;
  }

  //
  // Set host router protocol tunneling control based on USB4 control fields in HrInfo
  //
  DEBUG ((DEBUG_INFO, "USB4 control value = 0x%0x\n", Usb4Hr->HrInfo.Usb4Control.Value));
  Status = Usb4HrSetUsb4Control (Router, &(Usb4Hr->HrInfo.Usb4Control));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomainRoot: Set USB4 control failure, %r\n", Status));
    goto Exit;
  }

  //
  // Add the host router into domain
  //
  Status = Usb4DomainAddRouter (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomainRoot: Add Host Router to Domain failure, %r\n", Status));
    goto Exit;
  }

Exit:
  if (EFI_ERROR (Status) && (Router != NULL)) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_RT_DESTROY_ERROR, (UINT32) Status, 0);
    Usb4RtDestroy (Router);
  }

  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_ROOTRT_CREATE_EXIT, (UINT32) Status, 0);
  DEBUG ((DEBUG_INFO, "Usb4HrCreateDomainRoot exit\n"));
  return Status;
}

/**
  Create USB4 domain and add router instance of host router in the domain.
  Router instance represents the entity in domain topology defined in USB4 configuration layer.

  @param[in] Usb4Hr    - Pointer to USB4 hsot router instance.

  @retval EFI_SUCCESS           - Enumerate host router and all connected routers successfully.
  @retval EFI_OUT_OF_RESOURCES  - Insufficient resources to create router instance.
  @retval EFI_UNSUPPORTED       - Fail to enumerate all connected routers from host router.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HrCreateDomain (
  IN  USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS     Status;

  DEBUG ((DEBUG_INFO, "Usb4HrCreateDomain entry\n"));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomain: NULL Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Create USB4 domain instance for managing USB4 domain topology
  //
  Status = Usb4DomainCreate (Usb4Hr, &(Usb4Hr->Usb4Domain));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomain: Create USB4 domain failure, %r\n", Status));
    Usb4Hr->Usb4Domain = NULL;
    goto Exit;
  }

  //
  // Create root router instance to represent USB4 host router in domain.
  //
  Status = Usb4HrCreateDomainRoot (Usb4Hr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreateDomain: Create USB4 domain root router failure, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4HrCreateDomain exit\n"));
  return Status;
}

/**
  Build USB4 domain topology of host router.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Build USB4 topology successfully.
  @retval EFI_UNSUPPORTED       - Fail to build USB4 topology.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4HrBuildTopology (
  IN  USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS     Status;

  DEBUG ((DEBUG_INFO, "Usb4HrBuildTopology entry - HR = %p\n", Usb4Hr));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_BLD_TOPOLOGY_ENTRY, 0, 0);

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HrBuildTopology: NULL Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Create USB4 domain for domain topology management on host router.
  //
  Status = Usb4HrCreateDomain (Usb4Hr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrBuildTopology: Create USB4 domain for HR %p failure, %r\n", Usb4Hr, Status));
    Usb4Hr->Usb4Domain = NULL;
    goto Exit;
  }

  //
  // Enumerate and add all downstream routers of host router into domain topology
  //
  Status = Usb4RtEnumAllDsRouters (Usb4Hr->Usb4Domain->RootRt);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrBuildTopology: Enumerate the downstream Routers of Host Router failure, %r\n", Status));
    goto Exit;
  }

  //
  // Check and process the pending hot plug events in Rx event queue.
  // Usually, DP hot plug events may be received here.
  // DP HPD bit may not be set when CM enumerates new Router with DP monitor connected.
  // DP hot plug or router hot plug events may be received during topology establishment.
  //
  CmProcessRxEvents (Usb4Hr);

  //
  // Dump domain information
  //
  Usb4DomainDumpInfo (Usb4Hr->Usb4Domain);
Exit:
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_BLD_TOPOLOGY_EXIT, (UINT32) Status, 0);
  DEBUG ((DEBUG_INFO, "Usb4HrBuildTopology exit\n"));
  return Status;
}

/**
  Stop USB4 host router.
  - Un-initialize PCI config space.
  - Free the allocated MMIO space.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Usb4HrStop (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  DEBUG ((DEBUG_INFO, "Usb4HrStop entry\n"));

  if (Usb4HrInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HrStop: Invalid parameter\n"));
    goto Exit;
  }

  //
  // Destory host interface core.
  // DMA buffer should be allocated by PCI I/O protocol after PCI enumeration.
  //
  Usb4HiCoreDestroy (&(Usb4HrInst->HiCore));

  //
  // Un-init PCI actions done by SW CM Dxe prior to PCI enumeration
  //
  if (Usb4HrInst->HrInfo.IntegratedHr) {
    Iusb4HrPciUnInit (Usb4HrInst);
  } else {
    Dusb4HrPciUnInit (Usb4HrInst);
  }

  //
  // Free the MMIO space allocated for Tx/Rx Ring access.
  //
  if (Usb4HrInst->MmioBase != 0) {
    DEBUG ((DEBUG_INFO, "Free MMIO base = 0x%0X, len = 0x%x\n", Usb4HrInst->MmioBase, Usb4HrInst->MmioLen));
    CmFreeMmio (Usb4HrInst->MmioBase, Usb4HrInst->MmioLen);
    Usb4HrInst->MmioBase = 0;
    Usb4HrInst->MmioLen  = 0;
  }
Exit:
  DEBUG ((DEBUG_INFO, "Usb4HrStop exit\n"));
}

/**
  Destroy USB4 host router instance.
  - Un-initialize PCI config space.
  - Free the allocated MMIO space.
  - Free the host router instance.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Usb4HrDestroy (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  DEBUG ((DEBUG_INFO, "Usb4HrDestroy entry\n"));

  if (Usb4HrInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HrDestroy: Invalid parameter\n"));
    goto Exit;
  }

  Usb4HrStop (Usb4HrInst);

  //
  // Free host router router instance.
  //
  CmFreeMem (Usb4HrInst);

Exit:
  DEBUG ((DEBUG_INFO, "Usb4HrDestroy exit\n"));
}

/**
  Create and initialize host router instance.
  - Allocate and assign MMIO space for Tx/Rx ring access
  - Initialize PCI config space.

  @param[in]  ImageHandle  - Usb4CmDxe image handle.
  @param[in]  HrInfo       - Host router information.
  @param[out] Usb4HrInst   - Pointer of pointer to the created USB4 host router instance.

  @retval EFI_SUCCESS           - Create host router instance successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_NOT_FOUND         - Device is not present.
  @retval EFI_OUT_OF_RESOURCES  - Insufficient to initialize host router instance.
**/
EFI_STATUS
Usb4HrCreate (
  IN  EFI_HANDLE         ImageHandle,
  IN  PUSB4_HR_INFO      HrInfo,
  OUT USB4_HR_INSTANCE   **Usb4HrInst
  )
{
  EFI_STATUS          Status;
  USB4_HR_INSTANCE    *HrInst;
  PHYSICAL_ADDRESS    MmioBase;

  DEBUG ((DEBUG_INFO, "Usb4HrCreate entry\n"));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_HR_CREATE_ENTRY, 0, 0);

  HrInst = NULL;
  if ((ImageHandle == NULL) || (HrInfo == NULL) || (Usb4HrInst == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreate: Invalid parameter (%p, %p, %p)\n", ImageHandle, HrInfo, Usb4HrInst));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  HrInst = CmAllocateZeroMem (sizeof (USB4_HR_INSTANCE));
  if (HrInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreate: Insufficient resources\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  HrInst->Signature = USB4_HR_INSTANCE_SIGNATURE;

  //
  // Set host router information.
  //
  CmCopyMem (&(HrInst->HrInfo), HrInfo, sizeof(USB4_HR_INFO));

  //
  // Allocated MMIO space for the programming interface of Tx/Rx Ring access.
  //
  Status = CmAllocMmio (ImageHandle, USB4_RING_MMIO_LENGTH, &MmioBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreate: Alloc MMIO failure, %r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }
  HrInst->MmioBase       = MmioBase;
  HrInst->MmioLen        = USB4_RING_MMIO_LENGTH;
  HrInst->PciIo          = NULL;
  DEBUG ((DEBUG_INFO, "Allocated MMIO base = 0x%0X, len = 0x%x\n", HrInst->MmioBase, HrInst->MmioLen));

  //
  // Initialize PCI configuration space of host router
  //
  if (HrInfo->IntegratedHr) {
    Status = Iusb4HrPciInit (HrInst);
  } else {
    Status = Dusb4HrPciInit (HrInst);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreate: PCI configuration space init failure, %r\n", Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Dump BAR0 MMIO content:\n"));
  HrSrvMmioDump (HrInst->MmioBase, 16);

  //
  // Create USB4 host interface core for USB4 config space access
  //
  Status = Usb4HiCoreCreate (HrInst);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4HrCreate: Init Host Interface Core failure, %r\n", Status));
    goto Exit;
  }

  *Usb4HrInst = HrInst;

Exit:
  if ((EFI_ERROR (Status)) && (HrInst != NULL)) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_HR_DESTROY_ERROR, (UINT32) Status, 0);
    Usb4HrDestroy (HrInst);
  }
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_HR_CREATE_EXIT, (UINT32) Status, 0);
  DEBUG ((DEBUG_INFO, "Usb4HrCreate exit\n"));
  return Status;
}

/**
  Unpower USB4 host router

  @param[in] Usb4Hr - Pointer to USB4 host router instance.
**/
VOID
Usb4HrUnPower (
  IN USB4_HR_INSTANCE    *Usb4Hr
  )
{
  DEBUG ((DEBUG_INFO, "Usb4HrUnPower entry - Usb4Hr %p\n", Usb4Hr));
  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4HrUnPower: Invalid parameter\n"));
    goto Exit;
  }

  if (Usb4Hr->HrInfo.IntegratedHr) {
    Iusb4HrForcePwr (Usb4Hr, FALSE);
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4HrUnPower exit\n"));
  return;
}