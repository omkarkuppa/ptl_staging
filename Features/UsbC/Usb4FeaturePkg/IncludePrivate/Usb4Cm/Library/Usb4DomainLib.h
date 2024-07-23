/** @file
  USB4 domain management library header.

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

#ifndef _USB4_DOMAIN_LIB_H_
#define _USB4_DOMAIN_LIB_H_

#include <Uefi.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Usb4DomainDefs.h>
#include <Library/Usb4HrInst.h>

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
  );

/**
  Destroy USB4 Domain.

  @param[in] DomainInst - Pointer to USB4 Domain instance.
**/
VOID
Usb4DomainDestroy (
  IN USB4_DOMAIN    *DomainInst
  );

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
  );

/**
  Unbind the USB4 host router from USB4 domain.
  All USB4 config space access can't be done in this domain.

  @param[in] Domain - Pointer to USB4 domain instance.
**/
VOID
Usb4DomainUnbindHr (
  IN USB4_DOMAIN    *Domain
  );

/**
  Dump USB4 Domain Information.

  @param[in] Domain - Pointer to USB4 Domain instance.
**/
VOID
Usb4DomainDumpInfo (
  IN USB4_DOMAIN    *Domain
  );

/**
  Add an enumerated Router into Domain Topology.

  @param[in] Router - Pointer to Router to be added.

  @retval EFI_SUCCESS           - Add a Router into Domain success.
  @retval EFI_UNSUPPORTED       - Fail to add the Router into Domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainAddRouter (
  IN USB4_ROUTER            *Router
  );

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
  );

/**
  Get the Router node from domain topology by topology id.

  @param[in]  DomainContext - USB4 domain context.
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
  );

/**
  Add a DP source to USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Add a DP source to USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to add a DP source to USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSrcAdpAdd (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  );

/**
  Remove a DP source from USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Add a DP source to USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to add a DP source to USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSrcAdpRemove (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  );

/**
  Add a DP sink to USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Add a DP sink to USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to add a DP sink to USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSinkAdpAdd (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  );

/**
  Remove a DP sink from USB4 domain.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Pointer to router instance.
  @param[in] AdpNum        - DP adapter number.

  @retval EFI_SUCCESS           - Remove a DP sink from USB4 domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to remove a DP sink from USB4 domain.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpSinkAdpRemove (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router,
  IN UINT8                  AdpNum
  );

/**
  Remove DP source or sink from domain for router removal.

  @param[in] DomainContext - USB4 domain context.
  @param[in] Router        - Removed router.

  @retval EFI_SUCCESS           - Remove DP source or sink from domain successfully.
  @retval EFI_UNSUPPORTED       - Fail to remove DP source or sink from domain for router removal.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DomainDpDisableForRtRemoval (
  IN USB4_DOMAIN_CONTEXT    DomainContext,
  IN PUSB4_ROUTER           Router
  );

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
  );

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
  );

#endif
