/** @file
  USB4 domain management implementation.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/Usb4HiCoreLib.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4HrInst.h>
#include <Library/Usb4DebugLib.h>

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

/**
  Dump USB4 Domain Information.

  @param[in] Domain - Pointer to USB4 Domain instance.
**/
VOID
Usb4DomainDumpInfo (
  IN USB4_DOMAIN    *Domain
  )
{
  UINT32          Index;
  PUSB4_ROUTER    CurRt;
  LIST_ENTRY      *CurNode;

  if (Domain != NULL) {
    DEBUG ((DEBUG_INFO, "USB4 Domain Information :\n"));
    DEBUG ((DEBUG_INFO, "Router Count = %d\n", Domain->RouterCount));

    if (Domain->RootRt != NULL) {
      CurNode = &(Domain->RootRt->DomainEntry);
      Index = 0;
      do {
        CurRt = GET_ROUTER (CurNode, DomainEntry);
        DEBUG ((CM_DEBUG, "Router[%d] - 0x%016llX , Depth = %d, DsRouterCount in next level = %d\n",
                Index,
                CurRt->TopologyId.Value,
                CurRt->Depth,
                CurRt->DsRouterCount
                ));
        CurNode = GetNextNode (&(Domain->RootRt->DomainEntry), CurNode);
        Index++;
      } while (CurNode != &(Domain->RootRt->DomainEntry));
      DEBUG ((DEBUG_INFO, "\n"));
    }
  }
}

/**
  Get a free router instance from domain for new router enumeration.

  @param[in]  DomainContext - USB4 domain context.
  @param[out] FreeRt        - Pointer of pointer to the returned router instance.

  @retval EFI_SUCCESS           - Get a free router instance successfully.
  @retval EFI_OUT_OF_RESOURCES  - Unable to find a free router instance.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainGetFreeRouter (
  IN  USB4_DOMAIN_CONTEXT    DomainContext,
  OUT USB4_ROUTER            **FreeRt
  )
{
  EFI_STATUS     Status;
  UINT32         Index;
  UINT32         Search;
  USB4_ROUTER    *Router;
  USB4_DOMAIN    *Domain;

  DEBUG ((DEBUG_INFO, "Usb4DomainGetFreeRouter entry\n"));

  Domain = (USB4_DOMAIN *) DomainContext;

  if ((Domain == NULL) || (FreeRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainGetFreeRouter: Invalid parameter (%p, %p)\n", Domain, FreeRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainGetFreeRouter: Invalid USB4 Domain signature!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainGetFreeRouter: Null Usb4Hr in Domain!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->RtInUse >= USB4_ROUTER_SUPPORT_MAX) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainGetFreeRouter: No free router instance!\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Index = Domain->NextRtSearch;
  if (Index >= USB4_ROUTER_SUPPORT_MAX) {
    //
    // This should not happen.
    //
    Index = 0;
  }

  //
  // Find a free router instance from router pool
  //
  for (Search = 0; Search < USB4_ROUTER_SUPPORT_MAX; Search++) {
    if (Domain->Router[Index].State.InUse == 0) {
      Domain->NextRtSearch = (Index + 1) % USB4_ROUTER_SUPPORT_MAX;
      break;
    }
    Index = (Index + 1) % USB4_ROUTER_SUPPORT_MAX;
  }
  if (Search >= USB4_ROUTER_SUPPORT_MAX) {
    //
    // This should not happen
    //
    DEBUG ((DEBUG_ERROR, "Usb4DomainGetFreeRouter: No free router instance can be found!\n"));
    ASSERT (0);
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Router = &(Domain->Router[Index]);
  CmZeroMem (Router, sizeof (USB4_ROUTER));

  Router->State.InUse = 1;
  Router->CmVer = (Domain->Usb4Hr->HrInfo.CmVer) ? Domain->Usb4Hr->HrInfo.CmVer : USB4_CM_VERSION_1;
  Router->DomainContext = (USB4_DOMAIN_CONTEXT) Domain;
  InitializeListHead (&(Router->DomainEntry));
  Domain->RtInUse++;

  *FreeRt = Router;
  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainGetFreeRouter exit\n"));
  return Status;
}

/**
  Free a router instance to the free router pool in domain.

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS           - Free the router instance successfully.
  @retval EFI_UNSUPPORTED       - Fail to free the router instance to domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainFreeRouter (
  IN USB4_ROUTER    *Router
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;

  DEBUG ((DEBUG_INFO, "Usb4DomainFreeRouter entry\n"));
  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainFreeRouter: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainFreeRouter: Null domain in router\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Router->State.InUse == 0) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainFreeRouter: Router is not in use state!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Domain->RtInUse == 0) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainFreeRouter: Domain RtInUse has reached 0 before free router instance.\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Router->State.InUse = 0;
  Domain->RtInUse--;
  DEBUG ((DEBUG_INFO, "Usb4DomainFreeRouter: Domain RtInUse = %d\n", Domain->RtInUse));
  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainFreeRouter exit\n"));
  return Status;
}

/**
  Get the Router node from domain topology by topology id.

  @param[in]  DomainContext - Pointer to Domain instance.
  @param[in]  TopologyId    - Router topology id to be searched.
  @param[out] RouterInst    - Pointer of pointer for retrieving Router pointer.

  @retval EFI_SUCCESS           - Get the Router node success.
  @retval EFI_NOT_FOUND         - The router node with the specified topology id can't be found in domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainGetRouter (
  IN  USB4_DOMAIN_CONTEXT    DomainContext,
  IN  TOPOLOGY_ID            *TopologyId,
  OUT USB4_ROUTER            **RouterInst
  )
{
  EFI_STATUS      Status;
  USB4_DOMAIN     *Domain;
  PUSB4_ROUTER    CurRt;
  LIST_ENTRY      *CurNode;

  Domain = (USB4_DOMAIN *) DomainContext;

  if ((Domain == NULL) || (TopologyId == NULL) || (RouterInst == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainGetRouter: Invalid parameter (%p,%p,%p)\n", Domain, TopologyId, RouterInst));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainGetRouter: Invalid USB4 Domain signature!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = EFI_NOT_FOUND;
  if (Domain->RootRt == NULL) {
    goto Exit;
  }

  //
  // Look up the matched topology id from domain router tree
  //
  CurNode = &(Domain->RootRt->DomainEntry);
  do {
    CurRt = GET_ROUTER (CurNode, DomainEntry);
    if (CurRt->TopologyId.Value == TopologyId->Value) {
      Status = EFI_SUCCESS;
      break;
    }
    CurNode = GetNextNode (&(Domain->RootRt->DomainEntry), CurNode);
  } while (CurNode != &(Domain->RootRt->DomainEntry));

  if (Status == EFI_SUCCESS) {
    *RouterInst = CurRt;
  }

Exit:
  return Status;
}

/**
  Add an enumerated Router into Domain Topology.

  @param[in] Router - Pointer to Router to be added.

  @retval EFI_SUCCESS           - Add a Router into Domain success.
  @retval EFI_UNSUPPORTED       - Fail to add the Router into Domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainAddRouter (
  IN USB4_ROUTER    *Router
  )
{
  EFI_STATUS      Status;
  USB4_DOMAIN     *Domain;
  LIST_ENTRY      *CurNode;
  LIST_ENTRY      *PrevNode;
  PUSB4_ROUTER    CurRt;
  TOPOLOGY_ID     TpIdInv;
  TOPOLOGY_ID     CurTpIdInv;

  DEBUG ((DEBUG_INFO, "Usb4DomainAddRouter entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainAddRouter: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain->Signature != USB4_DOMAIN_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainAddRouter: Invalid USB4 Domain signature!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Router->Depth > mUsb4TopologyDepth) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainAddRouter: Router depth %d exceeds maximum supported depth\n", Router->Depth));
    Status = EFI_UNSUPPORTED;;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Add Router 0x%016llX into Domain at depth %d\n", Router->TopologyId.Value, Router->Depth));

  //
  // If router is host router, then set it to root of domain router tree if root is not present.
  // If router is device router, then insert it to router tree by (depth, reversed byte order of topology id)
  //
  if (Router->TopologyId.Value == 0) {
    if (Domain->RootRt != NULL) {
      DEBUG ((DEBUG_ERROR, "Try to set host router as root router in domain, but root router has been present!\n"));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

    DEBUG ((DEBUG_INFO, "Host router is added as root router in domain topology\n"));
    Domain->RootRt = Router;
  } else {
    if (Domain->RootRt == NULL) {
      DEBUG ((DEBUG_ERROR, "Can't add Rt 0x%016llX to domain since root router is NULL!\n", Router->TopologyId));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    //
    // Swap topology id byte order as the sorting base in domain entry list
    //
    TpIdInv.Id.High = Router->TopologyId.Id.Low;
    TpIdInv.Id.Low  = Router->TopologyId.Id.High;
    CmSwapEndianness ((UINT32 *)&TpIdInv, sizeof(TpIdInv)/sizeof(UINT32), (UINT32 *)&TpIdInv);

    //
    // Insert domain entry by (depth, reversed byte order of topology id).
    // Entry with depth 0 will be at the list head and entry with max depth will be at list tail.
    // Entry with smaller reversed topology id value will be in front of entry with bigger reversed topology id value.
    // Example:
    //   Rt1 topology id 00 00 00 00 01, Rt2 topology id 00 00 00 03 01
    //   Rt3 topology id 00 00 00 00 03, Rt4 topology id 00 00 00 03 03
    // Reversed byte order :
    //   Rt1 = 01 00 00 00 00, Rt2 = 01 03 00 00 00
    //   Rt3 = 03 00 00 00 00, Rt4 = 03 03 00 00 00
    // Rooter tree sorted by (depth, reversed byte order of topology id):
    //   RootRt(depth 0) <-> Rt1(depth 1) <-> Rt3(depth 1) <-> Rt2(depth 2) <-> Rt4(depth 2)
    //
    CurNode = GetFirstNode (&(Domain->RootRt->DomainEntry));
    while (CurNode != &(Domain->RootRt->DomainEntry)) {
      CurRt = GET_ROUTER (CurNode, DomainEntry);
      if (Router->TopologyId.Value == CurRt->TopologyId.Value) {
        DEBUG ((DEBUG_ERROR, "Same Topology ID 0x%016llX is present in Domain!\n", Router->TopologyId));
        Status = EFI_UNSUPPORTED;
        goto Exit;
      }
      if (Router->Depth < CurRt->Depth) {
        break;
      } else if (Router->Depth == CurRt->Depth) {
        CurTpIdInv.Id.High = CurRt->TopologyId.Id.Low;
        CurTpIdInv.Id.Low  = CurRt->TopologyId.Id.High;
        CmSwapEndianness ((UINT32 *) &CurTpIdInv, sizeof (CurTpIdInv) / sizeof (UINT32), (UINT32 *) &CurTpIdInv);
        if (TpIdInv.Value < CurTpIdInv.Value) {
          break;
        }
      }
      CurNode = GetNextNode (&(Domain->RootRt->DomainEntry), CurNode);
    }

    PrevNode = GetPreviousNode (&(Domain->RootRt->DomainEntry), CurNode);
    InsertHeadList (PrevNode, &Router->DomainEntry);
  }

  Domain->RouterCount++;
  DEBUG ((DEBUG_INFO, "Add Router into Domain successfully,  Router Count = %d\n", Domain->RouterCount));
  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainAddRouter exit\n"));
  return Status;
}

/**
  Remove and the Router node from Domain and delete the Router instance.
  Note: The Router can't be removed from Domain if downstream Routers are still present.

  @param[in] Router   - Pointer to Router instance.

  @retval EFI_SUCCESS           - Remove a Router from Domain success.
  @retval EFI_UNSUPPORTED       - Remove Domain failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DomainDeleteRouter (
  IN USB4_ROUTER    *Router
  )
{
  EFI_STATUS     Status;
  USB4_DOMAIN    *Domain;

  DEBUG ((DEBUG_INFO, "DomainDeleteRouter entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "DomainDeleteRouter: Null router instance\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;

  //
  // The downstream routers should be removed first.
  //
  if (Router->DsRouterCount != 0) {
    DEBUG ((DEBUG_ERROR, "DomainDeleteRouter: Can't remove router since the downstream router is still present!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Router == Domain->RootRt) {
    //
    // Remove host router
    //
    Domain->RootRt = NULL;
  } else {
    //
    // Remove device router
    //
    RemoveEntryList (&Router->DomainEntry);
  }

  Domain->RouterCount--;
  DEBUG ((DEBUG_INFO, "Remove Rt 0x%016llX from Domain, RouterCount=%d\n", Router->TopologyId, Domain->RouterCount));

  //
  // Destroy Router instance
  //
  Status = Usb4RtDestroy (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DomainDeleteRouter: Destroy Router instance failure, %r\n", Status));
  }

Exit:
  DEBUG ((DEBUG_INFO, "DomainDeleteRouter exit\n"));
  return Status;
}

/**
  Remove a Router and its downstream Routers from Domain.

  @param[in] Router - Pointer to Router instance.

  @retval EFI_SUCCESS           - Remove Router from Domain success.
  @retval EFI_UNSUPPORTED       - Delete Router failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainRemoveRouter (
  IN USB4_ROUTER    *Router
  )
{
  EFI_STATUS      Status;
  USB4_DOMAIN     *Domain;
  USB4_ADAPTER    *Adapter;
  UINT32           Index;

  DEBUG ((DEBUG_INFO, "Usb4DomainRemoveRouter entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainRemoveRouter: Null router instance\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) Router->DomainContext;
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainRemoveRouter: Null domain context\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Remove downstream routers of Rt 0x%016llX\n", Router->TopologyId.Value));
  //
  // Remove all downstream Routers if any
  //
  if (Router->DsRouterCount != 0) {
    for (Index = 1; (Index <= Router->MaxAdapter) && (Index < USB4_ADAPTER_SUPPORT_MAX); Index++) {
      Adapter = &(Router->Adapter[Index]);
      if ((Adapter->Type == USB4_ADP_TYPE_LANE) && (Adapter->AdpDep.Lane.DsRouter != NULL)) {
        Status = Usb4DomainRemoveRouter (Adapter->AdpDep.Lane.DsRouter);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Usb4DomainRemoveRouter: Remove downstream Router failure, %r\n", Status));
          goto Exit;
        }
      }
    }
  }

  //
  // Remove Router node from Domain topology and destroy the Router instance.
  //
  Status = DomainDeleteRouter (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainRemoveRouter: Delete Router from Domain failure, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainRemoveRouter exit\n"));
  return Status;
}

/**
  Unbind the USB4 host router from USB4 domain.
  All USB4 config space access can't be done in this domain.

  @param[in] Domain - Pointer to USB4 domain instance.
**/
VOID
Usb4DomainUnbindHr (
  IN USB4_DOMAIN    *Domain
  )
{
  DEBUG ((DEBUG_INFO, "Usb4DomainUnbindHr entry\n"));
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainUnbindHr: Null domain!\n"));
    goto Exit;
  }

  //
  // Release reference to Usb4CsIo
  //
  DEBUG ((DEBUG_INFO, "Usb4DomainUnbindHr: Usb4Hr = %p, Usb4CsIo = %p\n", Domain->Usb4Hr, Domain->Usb4CsIo));

  if (Domain->Usb4CsIo != NULL) {
    //
    // Release reference to Usb4CsIo.
    // Host interface core can be destoryed only if its Usb4CsIo reference count is 0.
    //
    Domain->Usb4CsIo->ReleaseRef(Domain->Usb4CsIo);
    Domain->Usb4CsIo = NULL;
  }

  Domain->Usb4Hr = NULL;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainUnbindHr exit\n"));
}

/**
  Bind the USB4 host router to the USB4 domain.
  All USB4 config space access in this domain will go through the host interface of binding USB4 host router.

  @param[in] Usb4Hr - Pointer to USB4 host router instance.
  @param[in] Domain - Pointer to USB4 domain instance.

  @retval EFI_SUCCESS           - Start USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to start USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainBindHr (
  IN USB4_HR_INSTANCE    *Usb4Hr,
  IN USB4_DOMAIN         *Domain
  )
{
  EFI_STATUS    Status;
  USB4_CS_IO    *Usb4CsIo;

  DEBUG ((DEBUG_INFO, "Usb4DomainBindHr entry - Usb4Hr = %p\n", Usb4Hr));

  if ((Usb4Hr == NULL) || (Domain == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainBindHr: Invalid parameter (%p, %p)\n", Usb4Hr, Domain));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Domain->Usb4Hr != NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainBindHr: Usb4Hr has been present!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Usb4DomainBindHr: USB4 HR CM Version = %d\n", Usb4Hr->HrInfo.CmVer));
  if (Usb4Hr->HrInfo.CmVer == 0) {
    //
    // Host router CM version is not set
    // This situation should not happen
    // Set CM version to ver1 by default if it's not set
    //
    DEBUG ((DEBUG_ERROR, "Usb4DomainBindHr: USB4 CM version is not set for USB4 host router!\n"));
    Usb4Hr->HrInfo.CmVer = 1;
  }
  Domain->Usb4Hr = Usb4Hr;

  if (Domain->Usb4CsIo != NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainBindHr: Usb4CsIo has been present!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query USB4_CS_IO for USB4 config space access in this host router.
  // Release the reference to USB4_CS_IO when it will not be used any more.
  //
  Status = Usb4HiQueryCsIo (&(Usb4Hr->HiCore), &Usb4CsIo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainBindHr: Query USB4_CS_IO failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Usb4DomainBindHr: Usb4CsIo = %p\n", Usb4CsIo));
  Domain->Usb4CsIo = Usb4CsIo;
Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainBindHr exit\n"));
  return Status;
}

/**
  Destroy USB4 Domain.

  @param[in] DomainInst - Pointer to USB4 Domain instance.
**/
VOID
Usb4DomainDestroy (
  IN USB4_DOMAIN    *DomainInst
  )
{
  EFI_STATUS      Status;

  DEBUG ((DEBUG_INFO, "Usb4DomainDestroy entry\n"));

   if (DomainInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDestroy: Null Domain handle!\n"));
    goto Exit;
  }

  //
  // Remove and destroy all Router instances from Domain.
  //
  if (DomainInst->RootRt != NULL) {
    if (DomainInst->RootRt->DomainContext != (USB4_DOMAIN_CONTEXT) DomainInst) {
      DEBUG ((DEBUG_ERROR, "Usb4DomainDestroy: Domain context mismatches!\n"));
      ASSERT (0);
      goto Exit;
    }
    Status = Usb4DomainRemoveRouter (DomainInst->RootRt);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4DomainDestroy: Remove Routers from Domain failure, %r\n", Status));
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Usb4DomainDestroy: Domain root is empty!\n"));
  }

  //
  // Router count should be 0 after removing all routers in domain.
  //
  ASSERT (DomainInst->RouterCount == 0);

  //
  // Unbind USB4 host router from domain.
  // This will release Usb4CsIo reference on USB4 host interface.
  //
  Usb4DomainUnbindHr (DomainInst);

  //
  // Free domain instance memory.
  //
  CmFreeMem(DomainInst);
Exit:
  DEBUG ((DEBUG_INFO, "Usb4DomainDestroy exit\n"));
}

/**
  Create and initialize USB4 Domain for USB4 host router.

  @param[in]  Usb4HrInst - Pointer to USB4 host router instance.
  @param[out] DomainInst - Pointer to the created USB4 domain instance.

  @retval EFI_SUCCESS           - Create Domain instance success.
  @retval EFI_OUT_OF_RESOURCES  - Insufficient resources.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainCreate (
  IN  USB4_HR_INSTANCE    *Usb4HrInst,
  OUT USB4_DOMAIN         **DomainInst
  )
{
  EFI_STATUS             Status;
  USB4_DOMAIN            *Domain;

  DEBUG ((DEBUG_INFO, "Usb4DomainCreate entry\n"));

  Domain = NULL;

  if ((Usb4HrInst == NULL) || (DomainInst == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainCreate: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Domain = (USB4_DOMAIN *) CmAllocateZeroMem (sizeof (USB4_DOMAIN));
  if (Domain == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainCreate: Allocate memory failure\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Domain->Signature     = USB4_DOMAIN_SIGNATURE;
  Domain->RouterCount   = 0;
  Domain->DpStreamCount = 0;

  Status = Usb4DomainBindHr (Usb4HrInst, Domain);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4DomainCreate: Fail to start USb4 domain, %r\n", Status));
    goto Exit;
  }

  *DomainInst = Domain;
  Status = EFI_SUCCESS;
Exit:
  if (EFI_ERROR (Status) && (Domain != NULL)) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_DOMAIN_DESTROY_ERROR, (UINT32) Status, 0);
    Usb4DomainDestroy (Domain);
  }
  DEBUG ((DEBUG_INFO, "Usb4DomainCreate exit\n"));
  return Status;
}
